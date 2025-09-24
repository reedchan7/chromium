// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/webui/settings/window_controls_handler.h"

#include "base/functional/bind.h"
#include "base/values.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names_window_controls.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"

namespace settings {

WindowControlsHandler::WindowControlsHandler(Profile* profile)
    : profile_(profile) {}

WindowControlsHandler::~WindowControlsHandler() = default;

void WindowControlsHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "getWindowControlsVisibility",
      base::BindRepeating(&WindowControlsHandler::HandleGetWindowControlsVisibility,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setMinimizeButtonVisible",
      base::BindRepeating(&WindowControlsHandler::HandleSetMinimizeButtonVisible,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setMaximizeButtonVisible",
      base::BindRepeating(&WindowControlsHandler::HandleSetMaximizeButtonVisible,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setCloseButtonVisible",
      base::BindRepeating(&WindowControlsHandler::HandleSetCloseButtonVisible,
                          base::Unretained(this)));
}

void WindowControlsHandler::OnJavascriptAllowed() {
  PrefService* prefs = profile_->GetPrefs();
  pref_change_registrar_.Init(prefs);
  
  pref_change_registrar_.Add(
      prefs::kShowMinimizeButton,
      base::BindRepeating(&WindowControlsHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      prefs::kShowMaximizeButton,
      base::BindRepeating(&WindowControlsHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      prefs::kShowCloseButton,
      base::BindRepeating(&WindowControlsHandler::OnPreferencesChanged,
                          base::Unretained(this)));
}

void WindowControlsHandler::OnJavascriptDisallowed() {
  pref_change_registrar_.RemoveAll();
}

void WindowControlsHandler::HandleGetWindowControlsVisibility(
    const base::Value::List& args) {
  AllowJavascript();
  SendWindowControlsVisibility();
}

void WindowControlsHandler::HandleSetMinimizeButtonVisible(
    const base::Value::List& args) {
  CHECK_EQ(1U, args.size());
  if (args[0].is_bool()) {
    profile_->GetPrefs()->SetBoolean(prefs::kShowMinimizeButton, 
                                      args[0].GetBool());
  }
}

void WindowControlsHandler::HandleSetMaximizeButtonVisible(
    const base::Value::List& args) {
  CHECK_EQ(1U, args.size());
  if (args[0].is_bool()) {
    profile_->GetPrefs()->SetBoolean(prefs::kShowMaximizeButton, 
                                      args[0].GetBool());
  }
}

void WindowControlsHandler::HandleSetCloseButtonVisible(
    const base::Value::List& args) {
  CHECK_EQ(1U, args.size());
  if (args[0].is_bool()) {
    profile_->GetPrefs()->SetBoolean(prefs::kShowCloseButton, 
                                      args[0].GetBool());
  }
}

void WindowControlsHandler::SendWindowControlsVisibility() {
  PrefService* prefs = profile_->GetPrefs();
  
  base::Value::Dict visibility;
  visibility.Set("minimize", prefs->GetBoolean(prefs::kShowMinimizeButton));
  visibility.Set("maximize", prefs->GetBoolean(prefs::kShowMaximizeButton));
  visibility.Set("close", prefs->GetBoolean(prefs::kShowCloseButton));
  
  FireWebUIListener("window-controls-visibility-changed", visibility);
}

void WindowControlsHandler::OnPreferencesChanged() {
  SendWindowControlsVisibility();
}

}  // namespace settings

