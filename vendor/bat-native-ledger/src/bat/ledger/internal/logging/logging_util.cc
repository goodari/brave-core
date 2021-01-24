/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/logging/logging_util.h"

#include <sstream>

#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "bat/ledger/internal/logging/logging.h"

namespace ledger {

bool ShouldLogHeader(const std::string& header) {
  const std::vector<std::string> allowed_headers {
      "digest",
      "signature",
      "accept",
      "content-type"
  };

  for (const auto& item : allowed_headers) {
    if (base::StartsWith(header, item, base::CompareCase::INSENSITIVE_ASCII)) {
      return true;
    }
  }

  return false;
}

}  // namespace ledger
