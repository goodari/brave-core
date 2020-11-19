/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_shields/browser/domain_blocked_navigation_throttle.h"

#include <memory>
#include <utility>

#include "base/bind.h"
#include "base/task/post_task.h"
#include "base/threading/thread_task_runner_handle.h"
#include "brave/components/brave_shields/browser/ad_block_custom_filters_service.h"
#include "brave/components/brave_shields/browser/ad_block_service.h"
#include "brave/components/brave_shields/browser/domain_blocked_controller_client.h"
#include "brave/components/brave_shields/browser/domain_blocked_page.h"
#include "brave/components/brave_shields/browser/domain_blocked_tab_storage.h"
#include "components/prefs/pref_service.h"
#include "components/security_interstitials/content/security_interstitial_tab_helper.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"
#include "net/base/net_errors.h"

namespace {

bool ShouldBlockDomainOnTaskRunner(
    brave_shields::AdBlockService* ad_block_service,
    const GURL& url) {
  // Don't bother checking whether to block non-HTTP(S) pages. Along with being
  // a speed optimization, this avoids a crash on the new tab page and other
  // WebUI pages.
  if (!url.SchemeIsHTTPOrHTTPS())
    return false;

#if 1
  return (url.host() == "www.example.com");
#else
  bool did_match_exception = false;
  bool did_match_rule = false;
  bool did_match_important = false;
  std::string mock_data_url;
  ad_block_service->ShouldStartRequest(
      url, blink::mojom::ResourceType::kMainFrame, url.host(),
      &did_match_exception, &did_match_rule, &did_match_exception,
      &mock_data_url);
  return (did_match_important || (did_match_rule && !did_match_exception));
#endif
}

}  // namespace

namespace brave_shields {

// static
std::unique_ptr<DomainBlockedNavigationThrottle>
DomainBlockedNavigationThrottle::MaybeCreateThrottleFor(
    content::NavigationHandle* navigation_handle,
    AdBlockService* ad_block_service,
    AdBlockCustomFiltersService* ad_block_custom_filters_service,
    const std::string& locale) {
  if (!ad_block_service)
    return nullptr;
  return std::make_unique<DomainBlockedNavigationThrottle>(
      navigation_handle, ad_block_service, ad_block_custom_filters_service,
      locale);
}

DomainBlockedNavigationThrottle::DomainBlockedNavigationThrottle(
    content::NavigationHandle* navigation_handle,
    AdBlockService* ad_block_service,
    AdBlockCustomFiltersService* ad_block_custom_filters_service,
    const std::string& locale)
    : content::NavigationThrottle(navigation_handle),
      ad_block_service_(ad_block_service),
      ad_block_custom_filters_service_(ad_block_custom_filters_service),
      locale_(locale) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
}

DomainBlockedNavigationThrottle::~DomainBlockedNavigationThrottle() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
}

content::NavigationThrottle::ThrottleCheckResult
DomainBlockedNavigationThrottle::WillStartRequest() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  // If user has just chosen to proceed on our interstitial, don't show
  // another one.
  content::NavigationHandle* handle = navigation_handle();
  content::WebContents* web_contents = handle->GetWebContents();
  DomainBlockedTabStorage* tab_storage =
      DomainBlockedTabStorage::GetOrCreate(web_contents);
  if (tab_storage->IsProceeding())
    return content::NavigationThrottle::PROCEED;

  // Otherwise, call the ad block service on a task runner to determine whether
  // this domain should be blocked.
  GURL request_url = handle->GetURL();
  ad_block_service_->GetTaskRunner()->PostTaskAndReplyWithResult(
      FROM_HERE,
      base::BindOnce(&ShouldBlockDomainOnTaskRunner, ad_block_service_,
                     request_url),
      base::BindOnce(&DomainBlockedNavigationThrottle::OnShouldBlockDomain,
                     weak_ptr_factory_.GetWeakPtr()));

  // Since the call to the ad block service is asynchronous, we defer the final
  // decision of whether to allow or block this navigation. The callback from
  // the task runner will call a method to give our final answer.
  return content::NavigationThrottle::DEFER;
}

void DomainBlockedNavigationThrottle::OnShouldBlockDomain(
    bool should_block_domain) {
  if (should_block_domain) {
    ShowInterstitial();
  } else {
    // Navigation was deferred while we called the ad block service on a task
    // runner, but now we know that we want to allow navigation to continue.
    Resume();
  }
}

void DomainBlockedNavigationThrottle::ShowInterstitial() {
  content::NavigationHandle* handle = navigation_handle();
  content::WebContents* web_contents = handle->GetWebContents();
  const GURL& request_url = handle->GetURL();
  content::BrowserContext* context =
      handle->GetWebContents()->GetBrowserContext();
  PrefService* pref_service = user_prefs::UserPrefs::Get(context);

  // The controller client implements the actual logic to "go back" or "proceed"
  // from the interstitial.
  auto controller_client = std::make_unique<DomainBlockedControllerClient>(
      web_contents, request_url, ad_block_custom_filters_service_, pref_service,
      locale_);

  // This handles populating the HTML template of the interstitial page with
  // localized strings and other information we only know at runtime,
  // including the URL of the page we're blocking. Once the user interacts with
  // the interstitial, this translates those actions into method calls on the
  // controller client.
  auto blocked_page = std::make_unique<DomainBlockedPage>(
      web_contents, request_url, std::move(controller_client));

  // Get the page content before giving up ownership of |blocked_page|.
  std::string blocked_page_content = blocked_page->GetHTMLContents();

  // Replace the tab contents with our interstitial page.
  security_interstitials::SecurityInterstitialTabHelper::AssociateBlockingPage(
      web_contents, handle->GetNavigationId(),
      std::move(blocked_page));

  // Navigation was deferred rather than canceled outright because the
  // call to the ad blocking service happens on a task runner, but now we
  // know that we definitely want to cancel the navigation.
  CancelDeferredNavigation(content::NavigationThrottle::ThrottleCheckResult(
      content::NavigationThrottle::CANCEL, net::ERR_BLOCKED_BY_CLIENT,
      blocked_page_content));
}

const char* DomainBlockedNavigationThrottle::GetNameForLogging() {
  return "DomainBlockedNavigationThrottle";
}

}  // namespace brave_shields
