/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/features/user_activity/user_activity_features.h"

#include <vector>

#include "base/feature_list.h"
#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

TEST(BatAdsUserActivityFeaturesTest, UserActivityEnabled) {
  // Arrange

  // Act
  const bool is_enabled = features::IsUserActivityEnabled();

  // Assert
  EXPECT_TRUE(is_enabled);
}

TEST(BatAdsUserActivityFeaturesTest, UserActivityDisabled) {
  // Arrange
  const std::vector<base::test::ScopedFeatureList::FeatureAndParams>
      enabled_features;

  std::vector<base::Feature> disabled_features;
  disabled_features.push_back(features::kUserActivity);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  const bool is_enabled = features::IsUserActivityEnabled();

  // Assert
  EXPECT_FALSE(is_enabled);
}

TEST(BatAdsUserActivityFeaturesTest, UserActivityTriggers) {
  // Arrange
  std::vector<base::test::ScopedFeatureList::FeatureAndParams> enabled_features;
  base::FieldTrialParams kUserActivityParameters;
  const char kTriggersParameter[] = "triggers";
  kUserActivityParameters[kTriggersParameter] = "01=0.5;010203=1.0;0203=0.75";
  enabled_features.push_back(
      {features::kUserActivity, kUserActivityParameters});

  const std::vector<base::Feature> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  const std::string triggers = features::GetUserActivityTriggers();

  // Assert
  const std::string expected_triggers = "01=0.5;010203=1.0;0203=0.75";
  EXPECT_EQ(expected_triggers, triggers);
}

TEST(BatAdsUserActivityFeaturesTest, UserActivityDefaultTriggers) {
  // Arrange
  const std::vector<base::test::ScopedFeatureList::FeatureAndParams>
      enabled_features;

  const std::vector<base::Feature> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);
  // Act
  const std::string triggers = features::GetUserActivityTriggers();

  // Assert
  const std::string expected_triggers = "00=.5;01=.5;07=1;08=1;0C=1;0D=1";
  EXPECT_EQ(expected_triggers, triggers);
}

TEST(BatAdsUserActivityFeaturesTest, DisabledUserActivityDefaultTriggers) {
  // Arrange
  const std::vector<base::test::ScopedFeatureList::FeatureAndParams>
      enabled_features;

  std::vector<base::Feature> disabled_features;
  disabled_features.push_back(features::kUserActivity);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  const std::string triggers = features::GetUserActivityTriggers();

  // Assert
  const std::string expected_triggers = "00=.5;01=.5;07=1;08=1;0C=1;0D=1";
  EXPECT_EQ(expected_triggers, triggers);
}

TEST(BatAdsUserActivityFeaturesTest, UserActivityTimeWindow) {
  // Arrange
  std::vector<base::test::ScopedFeatureList::FeatureAndParams> enabled_features;
  base::FieldTrialParams kUserActivityParameters;
  const char kTimeWindowParameter[] = "time_window";
  kUserActivityParameters[kTimeWindowParameter] = "7200s";
  enabled_features.push_back(
      {features::kUserActivity, kUserActivityParameters});

  const std::vector<base::Feature> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  const base::TimeDelta time_delta = features::GetUserActivityTimeWindow();

  // Assert
  const base::TimeDelta expected_time_delta = base::TimeDelta::FromHours(2);
  EXPECT_EQ(expected_time_delta, time_delta);
}

TEST(BatAdsUserActivityFeaturesTest, UserActivityDefaultTimeWindow) {
  // Arrange
  const std::vector<base::test::ScopedFeatureList::FeatureAndParams>
      enabled_features;

  const std::vector<base::Feature> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  const base::TimeDelta time_delta = features::GetUserActivityTimeWindow();

  // Assert
  const base::TimeDelta expected_time_delta = base::TimeDelta::FromHours(1);
  EXPECT_EQ(expected_time_delta, time_delta);
}

TEST(BatAdsUserActivityFeaturesTest, DisabledUserActivityDefaultTimeWindow) {
  // Arrange
  const std::vector<base::test::ScopedFeatureList::FeatureAndParams>
      enabled_features;

  std::vector<base::Feature> disabled_features;
  disabled_features.push_back(features::kUserActivity);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  const base::TimeDelta time_delta = features::GetUserActivityTimeWindow();

  // Assert
  const base::TimeDelta expected_time_delta = base::TimeDelta::FromHours(1);
  EXPECT_EQ(expected_time_delta, time_delta);
}

TEST(BatAdsUserActivityFeaturesTest, UserActivityThreshold) {
  // Arrange
  std::vector<base::test::ScopedFeatureList::FeatureAndParams> enabled_features;
  base::FieldTrialParams kUserActivityParameters;
  const char kThresholdParameter[] = "threshold";
  kUserActivityParameters[kThresholdParameter] = "7.0";
  enabled_features.push_back(
      {features::kUserActivity, kUserActivityParameters});

  const std::vector<base::Feature> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  const double threshold = features::GetUserActivityThreshold();

  // Assert
  const double expected_threshold = 7.0;
  EXPECT_EQ(expected_threshold, threshold);
}

TEST(BatAdsUserActivityFeaturesTest, UserActivityDefaultThreshold) {
  // Arrange
  const std::vector<base::test::ScopedFeatureList::FeatureAndParams>
      enabled_features;

  const std::vector<base::Feature> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  const double threshold = features::GetUserActivityThreshold();

  // Assert
  const double expected_threshold = 2.0;
  EXPECT_EQ(expected_threshold, threshold);
}

TEST(BatAdsUserActivityFeaturesTest, DisabledUserActivityDefaultThreshold) {
  // Arrange
  const std::vector<base::test::ScopedFeatureList::FeatureAndParams>
      enabled_features;

  std::vector<base::Feature> disabled_features;
  disabled_features.push_back(features::kUserActivity);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  const double threshold = features::GetUserActivityThreshold();

  // Assert
  const double expected_threshold = 2.0;
  EXPECT_EQ(expected_threshold, threshold);
}

}  // namespace ads
