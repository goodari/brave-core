/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/webui/ipfs/ipfs_onboarding_ui.h"

#include <utility>

#include "brave/browser/ipfs/ipfs_service_factory.h"
#include "brave/browser/ui/webui/brave_webui_source.h"
#include "brave/components/ipfs_onboarding_ui/resources/grit/ipfs_onboarding_generated_map.h"
#include "components/grit/brave_components_resources.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"

namespace {}  // namespace

IPFSOnboardingDOMHandler::IPFSOnboardingDOMHandler()
    : weak_ptr_factory_{this} {}

IPFSOnboardingDOMHandler::~IPFSOnboardingDOMHandler() {}

void IPFSOnboardingDOMHandler::RegisterMessages() {
}


IPFSOnboardingUI::IPFSOnboardingUI(content::WebUI* web_ui,
                                   const std::string& name)
    : WebUIController(web_ui) {
  CreateAndAddWebUIDataSource(web_ui, name, kIpfsOnboardingGenerated, kIpfsOnboardingGeneratedSize,
                              IDR_IPFS_ONBOARDING_HTML);
  web_ui->AddMessageHandler(std::make_unique<IPFSOnboardingDOMHandler>());
}

IPFSOnboardingUI::~IPFSOnboardingUI() = default;