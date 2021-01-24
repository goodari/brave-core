/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_LEDGER_INTERNAL_LOGGING_LOGGING_UTIL_H_
#define BAT_LEDGER_INTERNAL_LOGGING_LOGGING_UTIL_H_

#include <map>
#include <string>
#include <vector>

#include "bat/ledger/mojom_structs.h"

namespace ledger {

// TODO(zenparsing): Move unit tests for this function to URLFetcher tests
bool ShouldLogHeader(const std::string& header);

}  // namespace ledger

#endif  // BAT_LEDGER_INTERNAL_LOGGING_LOGGING_UTIL_H_
