// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_WINDOW_CONTROLS_MANAGER_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_WINDOW_CONTROLS_MANAGER_H_

#include "base/memory/raw_ptr.h"
#include "components/prefs/pref_change_registrar.h"

class BrowserView;
class PrefService;

namespace views {
class Button;
}

// Manages the visibility of window control buttons (minimize, maximize, close)
// based on user preferences.
class WindowControlsManager {
 public:
  explicit WindowControlsManager(BrowserView* browser_view);
  ~WindowControlsManager();

  // Initialize the manager and register preference observers
  void Initialize();

  // Update button visibility based on current preferences
  void UpdateButtonVisibility();

  // Get/Set individual button visibility
  bool ShouldShowMinimizeButton() const;
  bool ShouldShowMaximizeButton() const;
  bool ShouldShowCloseButton() const;

  void SetMinimizeButtonVisible(bool visible);
  void SetMaximizeButtonVisible(bool visible);
  void SetCloseButtonVisible(bool visible);

 private:
  // Callback when preferences change
  void OnPreferencesChanged();

  // Update visibility for a specific button
  void UpdateButtonVisibilityInternal(views::Button* button, 
                                       const char* pref_name);

  raw_ptr<BrowserView> browser_view_;
  raw_ptr<PrefService> pref_service_;
  PrefChangeRegistrar pref_change_registrar_;
};

#endif  // CHROME_BROWSER_UI_VIEWS_FRAME_WINDOW_CONTROLS_MANAGER_H_

