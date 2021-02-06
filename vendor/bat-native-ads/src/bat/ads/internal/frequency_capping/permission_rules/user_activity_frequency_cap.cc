/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/frequency_capping/permission_rules/user_activity_frequency_cap.h"

#include "base/time/time.h"
#include "bat/ads/internal/features/user_activity/user_activity_features.h"
#include "bat/ads/internal/user_activity/user_activity.h"
#include "bat/ads/internal/user_activity/user_activity_scoring.h"
#include "bat/ads/internal/user_activity/user_activity_trigger_info.h"
#include "bat/ads/internal/user_activity/user_activity_util.h"

namespace ads {

UserActivityFrequencyCap::UserActivityFrequencyCap() = default;

UserActivityFrequencyCap::~UserActivityFrequencyCap() = default;

bool UserActivityFrequencyCap::ShouldAllow() {
  const base::TimeDelta time_window = features::GetUserActivityTimeWindow();

  const UserActivityEvents events =
      UserActivity::Get()->GetHistoryForTimeWindow(time_window);

  if (!DoesRespectCap(events)) {
    return false;
  }

  return true;
}

std::string UserActivityFrequencyCap::get_last_message() const {
  return last_message_;
}

bool UserActivityFrequencyCap::DoesRespectCap(
    const UserActivityEvents& events) {
  const UserActivityTriggers triggers =
      ToUserActivityTriggers(features::GetUserActivityTriggers());

  const double score = GetUserActivityScore(triggers, events);

  const double threshold = features::GetUserActivityThreshold();
  if (score < threshold) {
#if defined(DEBUG)
    last_message_ = StringPrintf(
        "User activity score of %f is below the required threshold of %f",
        score, threshold);
#endif

    return false;
  }

  return true;
}

}  // namespace ads
