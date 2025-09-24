// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SETTINGS_WINDOW_CONTROLS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_SETTINGS_WINDOW_CONTROLS_HANDLER_H_

#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "components/prefs/pref_change_registrar.h"

class Profile;

namespace settings {

// Handler for window controls settings in chrome://settings
class WindowControlsHandler : public SettingsPageUIHandler {
 public:
  explicit WindowControlsHandler(Profile* profile);
  ~WindowControlsHandler() override;

  // WebUIMessageHandler:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override;
  void OnJavascriptDisallowed() override;

 private:
  // Message handlers
  void HandleGetWindowControlsVisibility(const base::Value::List& args);
  void HandleSetMinimizeButtonVisible(const base::Value::List& args);
  void HandleSetMaximizeButtonVisible(const base::Value::List& args);
  void HandleSetCloseButtonVisible(const base::Value::List& args);

  // Send current visibility state to JavaScript
  void SendWindowControlsVisibility();

  // Callback when preferences change
  void OnPreferencesChanged();

  raw_ptr<Profile> profile_;
  PrefChangeRegistrar pref_change_registrar_;
};

}  // namespace settings

#endif  // CHROME_BROWSER_UI_WEBUI_SETTINGS_WINDOW_CONTROLS_HANDLER_H_

