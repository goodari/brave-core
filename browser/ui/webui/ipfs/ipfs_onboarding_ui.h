/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_WEBUI_IPFS_ONBOARDING_UI_H_
#define BRAVE_BROWSER_UI_WEBUI_IPFS_ONBOARDING_UI_H_

#include <memory>
#include <string>
#include <vector>

#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/web_ui_message_handler.h"

class IPFSOnboardingDOMHandler : public content::WebUIMessageHandler {
 public:
  IPFSOnboardingDOMHandler();
  ~IPFSOnboardingDOMHandler() override;

  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

 private:
  base::WeakPtrFactory<IPFSOnboardingDOMHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(IPFSOnboardingDOMHandler);
};

// The Onboarding WebUI for brave users
class IPFSOnboardingUI : public content::WebUIController {
 public:
  IPFSOnboardingUI(content::WebUI* web_ui, const std::string& host);
  ~IPFSOnboardingUI() override;
  IPFSOnboardingUI(const IPFSOnboardingUI&) = delete;
  IPFSOnboardingUI& operator=(const IPFSOnboardingUI&) = delete;
};

#endif  // BRAVE_BROWSER_UI_WEBUI_IPFS_ONBOARDING_UI_H_
