/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/user_activity/user_activity.h"

#include "bat/ads/internal/user_activity/page_transition_util.h"

namespace ads {

namespace {
UserActivity* g_user_activity = nullptr;
}  // namespace

UserActivity::UserActivity() {
  DCHECK_EQ(g_user_activity, nullptr);
  g_user_activity = this;
}

UserActivity::~UserActivity() {
  DCHECK(g_user_activity);
  g_user_activity = nullptr;
}

// static
UserActivity* UserActivity::Get() {
  DCHECK(g_user_activity);
  return g_user_activity;
}

// static
bool UserActivity::HasInstance() {
  return g_user_activity;
}

void UserActivity::RecordEvent(const UserActivityEventType event_type) {
  UserActivityEventInfo user_activity_event;
  user_activity_event.type = event_type;
  user_activity_event.time = base::Time::Now();

  history_.push_back(user_activity_event);

  if (history_.size() > kMaximumHistoryEntries) {
    history_.pop_front();
  }
}

void UserActivity::RecordEventForPageTransition(const int8_t page_transition) {
  if (page_transition == -1) {
    return;
  }

  if (IsNewNavigation(page_transition)) {
    RecordEvent(UserActivityEventType::kNewNavigation);
  }

  if (DidUseBackOrFowardButtonToTriggerNavigation(page_transition)) {
    RecordEvent(UserActivityEventType::kClickedBackOrForwardNavigationButtons);
  }

  if (DidUseAddressBarToTriggerNavigation(page_transition)) {
    RecordEvent(UserActivityEventType::kUsedAddressBar);
  }

  if (DidNavigateToHomePage(page_transition)) {
    RecordEvent(UserActivityEventType::kClickedHomePageButton);
  }

  if (DidTransitionFromExternalApplication(page_transition)) {
    RecordEvent(UserActivityEventType::kOpenedLinkFromExternalApplication);
  }

  const base::Optional<UserActivityEventType> event_type =
      ToUserActivityEventType(page_transition);
  if (event_type) {
    RecordEvent(event_type.value());
  }
}

UserActivityEvents UserActivity::GetHistoryForTimeWindow(
    const base::TimeDelta time_window) const {
  UserActivityEvents filtered_history = history_;

  const base::Time time = base::Time::Now() - time_window;

  const auto iter =
      std::remove_if(filtered_history.begin(), filtered_history.end(),
                     [&time](const UserActivityEventInfo& event) {
                       return event.time < time;
                     });

  filtered_history.erase(iter, filtered_history.end());

  return filtered_history;
}

}  // namespace ads
