/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/features/user_activity/user_activity_features.h"

#include "base/metrics/field_trial_params.h"

namespace ads {
namespace features {

namespace {

const char kFeatureName[] = "UserActivity";
const char kFieldTrialParameterTriggers[] = "triggers";
const char kFieldTrialParameterTimeWindow[] = "time_window";
const char kFieldTrialParameterThreshold[] = "threshold";

const char kDefaultTriggers[] = "00=.5;01=.5;07=1;08=1;0C=1;0D=1";
const base::TimeDelta kDefaultTimeWindow = base::TimeDelta::FromHours(1);
const double kDefaultTreshold = 2.0;

}  // namespace

const base::Feature kUserActivity{kFeatureName,
                                  base::FEATURE_ENABLED_BY_DEFAULT};

bool IsUserActivityEnabled() {
  return base::FeatureList::IsEnabled(kUserActivity);
}

std::string GetUserActivityTriggers() {
  const std::string value = GetFieldTrialParamValueByFeature(
      kUserActivity, kFieldTrialParameterTriggers);

  if (value.empty()) {
    return kDefaultTriggers;
  }

  return value;
}

base::TimeDelta GetUserActivityTimeWindow() {
  const std::string value_as_string = GetFieldTrialParamValueByFeature(
      kUserActivity, kFieldTrialParameterTimeWindow);

  if (value_as_string.empty()) {
    return kDefaultTimeWindow;
  }

  base::Optional<base::TimeDelta> time_delta =
      base::TimeDelta::FromString(value_as_string);
  if (!time_delta.has_value()) {
    return kDefaultTimeWindow;
  }

  return time_delta.value();
}

double GetUserActivityThreshold() {
  return GetFieldTrialParamByFeatureAsDouble(
      kUserActivity, kFieldTrialParameterThreshold, kDefaultTreshold);
}

}  // namespace features
}  // namespace ads
