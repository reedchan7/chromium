// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/views/frame/window_controls_manager.h"

#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/frame/opaque_browser_frame_view.h"
#include "chrome/browser/ui/view_ids.h"
#include "chrome/common/pref_names_window_controls.h"
#include "components/prefs/pref_service.h"
#include "ui/views/controls/button/button.h"
#include "build/build_config.h"

WindowControlsManager::WindowControlsManager(BrowserView* browser_view)
    : browser_view_(browser_view) {
  if (browser_view_ && browser_view_->browser()) {
    Profile* profile = browser_view_->browser()->profile();
    if (profile) {
      pref_service_ = profile->GetPrefs();
    }
  }
}

WindowControlsManager::~WindowControlsManager() = default;

void WindowControlsManager::Initialize() {
  if (!pref_service_)
    return;

  // Register preference change callbacks
  pref_change_registrar_.Init(pref_service_);
  pref_change_registrar_.Add(
      prefs::kShowMinimizeButton,
      base::BindRepeating(&WindowControlsManager::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      prefs::kShowMaximizeButton,
      base::BindRepeating(&WindowControlsManager::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      prefs::kShowCloseButton,
      base::BindRepeating(&WindowControlsManager::OnPreferencesChanged,
                          base::Unretained(this)));

  // Apply initial visibility settings
  UpdateButtonVisibility();
}

void WindowControlsManager::UpdateButtonVisibility() {
  if (!browser_view_)
    return;

#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_WIN)
  if (!browser_view_->browser_widget())
    return;

  // Get the frame view
  auto* frame_view = browser_view_->browser_widget()->GetFrameView();
  if (!frame_view)
    return;

  // Cast to OpaqueBrowserFrameView which owns caption buttons on desktop.
  auto* opaque_frame_view = static_cast<OpaqueBrowserFrameView*>(frame_view);
  if (!opaque_frame_view)
    return;

  // Find buttons by view ID to avoid accessing protected members.
  if (auto* minimize = views::AsViewClass<views::Button>(
          opaque_frame_view->GetViewByID(VIEW_ID_MINIMIZE_BUTTON))) {
    UpdateButtonVisibilityInternal(minimize, prefs::kShowMinimizeButton);
  }
  if (auto* maximize = views::AsViewClass<views::Button>(
          opaque_frame_view->GetViewByID(VIEW_ID_MAXIMIZE_BUTTON))) {
    UpdateButtonVisibilityInternal(maximize, prefs::kShowMaximizeButton);
  }
  if (auto* restore = views::AsViewClass<views::Button>(
          opaque_frame_view->GetViewByID(VIEW_ID_RESTORE_BUTTON))) {
    UpdateButtonVisibilityInternal(restore, prefs::kShowMaximizeButton);
  }
  if (auto* close = views::AsViewClass<views::Button>(
          opaque_frame_view->GetViewByID(VIEW_ID_CLOSE_BUTTON))) {
    UpdateButtonVisibilityInternal(close, prefs::kShowCloseButton);
  }

  // Request layout update
  frame_view->Layout();
  frame_view->SchedulePaint();
#else
  // Not supported on this platform yet.
  return;
#endif
}

void WindowControlsManager::UpdateButtonVisibilityInternal(
    views::Button* button,
    const char* pref_name) {
  if (!button || !pref_service_)
    return;

  bool should_show = pref_service_->GetBoolean(pref_name);
  button->SetVisible(should_show);
}

bool WindowControlsManager::ShouldShowMinimizeButton() const {
  return pref_service_ ? pref_service_->GetBoolean(prefs::kShowMinimizeButton)
                       : false;
}

bool WindowControlsManager::ShouldShowMaximizeButton() const {
  return pref_service_ ? pref_service_->GetBoolean(prefs::kShowMaximizeButton)
                       : false;
}

bool WindowControlsManager::ShouldShowCloseButton() const {
  return pref_service_ ? pref_service_->GetBoolean(prefs::kShowCloseButton)
                       : false;
}

void WindowControlsManager::SetMinimizeButtonVisible(bool visible) {
  if (pref_service_) {
    pref_service_->SetBoolean(prefs::kShowMinimizeButton, visible);
  }
}

void WindowControlsManager::SetMaximizeButtonVisible(bool visible) {
  if (pref_service_) {
    pref_service_->SetBoolean(prefs::kShowMaximizeButton, visible);
  }
}

void WindowControlsManager::SetCloseButtonVisible(bool visible) {
  if (pref_service_) {
    pref_service_->SetBoolean(prefs::kShowCloseButton, visible);
  }
}

void WindowControlsManager::OnPreferencesChanged() {
  UpdateButtonVisibility();
}
