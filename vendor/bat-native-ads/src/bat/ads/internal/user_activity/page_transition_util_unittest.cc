/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/user_activity/page_transition_util.h"

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"
#include "ui/base/page_transition_types.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

class BatAdsPageTransitionTest : public UnitTestBase {
 protected:
  BatAdsPageTransitionTest() = default;

  ~BatAdsPageTransitionTest() override = default;
};

TEST_F(BatAdsPageTransitionTest, IsNewNavigation) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_TYPED;

  // Act
  const bool did_transition_page = IsNewNavigation(page_transition);

  // Assert
  EXPECT_TRUE(did_transition_page);
}

TEST_F(BatAdsPageTransitionTest, DidUseBackOrFowardButtonToTriggerNavigation) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_FORWARD_BACK;

  // Act
  const bool did_transition_page =
      DidUseBackOrFowardButtonToTriggerNavigation(page_transition);

  // Assert
  EXPECT_TRUE(did_transition_page);
}

TEST_F(BatAdsPageTransitionTest, DidUseAddressBarToTriggerNavigation) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_FROM_ADDRESS_BAR;

  // Act
  const bool did_transition_page =
      DidUseAddressBarToTriggerNavigation(page_transition);

  // Assert
  EXPECT_TRUE(did_transition_page);
}

TEST_F(BatAdsPageTransitionTest, DidNavigateToHomePage) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_HOME_PAGE;

  // Act
  const bool did_transition_page = DidNavigateToHomePage(page_transition);

  // Assert
  EXPECT_TRUE(did_transition_page);
}

TEST_F(BatAdsPageTransitionTest, DidTransitionFromExternalApplication) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_FROM_API;

  // Act
  const bool did_transition_page =
      DidTransitionFromExternalApplication(page_transition);

  // Assert
  EXPECT_TRUE(did_transition_page);
}

TEST_F(BatAdsPageTransitionTest, ToUserActivityClickedLinkEventType) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_LINK;

  // Act
  base::Optional<UserActivityEventType> event_type =
      ToUserActivityEventType(page_transition);

  // Assert
  EXPECT_EQ(UserActivityEventType::kClickedLink, event_type);
}

TEST_F(BatAdsPageTransitionTest, ToUserActivityTypedUrlEventType) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_TYPED;

  // Act
  base::Optional<UserActivityEventType> event_type =
      ToUserActivityEventType(page_transition);

  // Assert
  EXPECT_EQ(UserActivityEventType::kTypedUrl, event_type);
}

TEST_F(BatAdsPageTransitionTest, ToUserActivityClickedBookmarkEventType) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_AUTO_BOOKMARK;

  // Act
  base::Optional<UserActivityEventType> event_type =
      ToUserActivityEventType(page_transition);

  // Assert
  EXPECT_EQ(UserActivityEventType::kClickedBookmark, event_type);
}

TEST_F(BatAdsPageTransitionTest,
       ToUserActivityTypedAndSelectedNonUrlEventType) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_GENERATED;

  // Act
  base::Optional<UserActivityEventType> event_type =
      ToUserActivityEventType(page_transition);

  // Assert
  EXPECT_EQ(UserActivityEventType::kTypedAndSelectedNonUrl, event_type);
}

TEST_F(BatAdsPageTransitionTest, ToUserActivitySubmittedFormEventType) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_FORM_SUBMIT;

  // Act
  base::Optional<UserActivityEventType> event_type =
      ToUserActivityEventType(page_transition);

  // Assert
  EXPECT_EQ(UserActivityEventType::kSubmittedForm, event_type);
}

TEST_F(BatAdsPageTransitionTest, ToUserActivityClickedReloadButtonEventType) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_RELOAD;

  // Act
  base::Optional<UserActivityEventType> event_type =
      ToUserActivityEventType(page_transition);

  // Assert
  EXPECT_EQ(UserActivityEventType::kClickedReloadButton, event_type);
}

TEST_F(BatAdsPageTransitionTest,
       ToUserActivityTypedKeywordOtherThanDefaultSearchProviderEventType) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_KEYWORD;

  // Act
  base::Optional<UserActivityEventType> event_type =
      ToUserActivityEventType(page_transition);

  // Assert
  EXPECT_EQ(UserActivityEventType::kTypedKeywordOtherThanDefaultSearchProvider,
            event_type);
}

TEST_F(BatAdsPageTransitionTest, ToUserActivityGeneratedKeywordEventType) {
  // Arrange
  const int32_t page_transition = ui::PAGE_TRANSITION_KEYWORD_GENERATED;

  // Act
  base::Optional<UserActivityEventType> event_type =
      ToUserActivityEventType(page_transition);

  // Assert
  EXPECT_EQ(UserActivityEventType::kGeneratedKeyword, event_type);
}

}  // namespace ads
