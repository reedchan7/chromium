// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/views/frame/window_controls_manager.h"

#include "base/test/scoped_feature_list.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/frame/opaque_browser_frame_view.h"
#include "chrome/common/pref_names_window_controls.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "ui/views/controls/button/button.h"

class WindowControlsManagerBrowserTest : public InProcessBrowserTest {
 public:
  WindowControlsManagerBrowserTest() = default;
  ~WindowControlsManagerBrowserTest() override = default;

 protected:
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();
    browser_view_ = BrowserView::GetBrowserViewForBrowser(browser());
    ASSERT_TRUE(browser_view_);
    
    auto* frame_view = browser_view_->frame()->GetFrameView();
    ASSERT_TRUE(frame_view);
    
    // Try to cast to OpaqueBrowserFrameView to access buttons
    opaque_frame_view_ = static_cast<OpaqueBrowserFrameView*>(frame_view);
  }

  BrowserView* browser_view() { return browser_view_; }
  OpaqueBrowserFrameView* opaque_frame_view() { return opaque_frame_view_; }
  
  PrefService* GetPrefs() {
    return browser()->profile()->GetPrefs();
  }

  bool IsMinimizeButtonVisible() {
    if (!opaque_frame_view_ || !opaque_frame_view_->minimize_button())
      return false;
    return opaque_frame_view_->minimize_button()->GetVisible();
  }

  bool IsMaximizeButtonVisible() {
    if (!opaque_frame_view_ || !opaque_frame_view_->maximize_button())
      return false;
    return opaque_frame_view_->maximize_button()->GetVisible();
  }

  bool IsCloseButtonVisible() {
    if (!opaque_frame_view_ || !opaque_frame_view_->close_button())
      return false;
    return opaque_frame_view_->close_button()->GetVisible();
  }

 private:
  raw_ptr<BrowserView> browser_view_ = nullptr;
  raw_ptr<OpaqueBrowserFrameView> opaque_frame_view_ = nullptr;
};

// Test that window control buttons are visible by default
IN_PROC_BROWSER_TEST_F(WindowControlsManagerBrowserTest, 
                       ButtonsVisibleByDefault) {
  EXPECT_TRUE(IsMinimizeButtonVisible());
  EXPECT_TRUE(IsMaximizeButtonVisible());
  EXPECT_TRUE(IsCloseButtonVisible());
}

// Test hiding minimize button via preferences
IN_PROC_BROWSER_TEST_F(WindowControlsManagerBrowserTest, 
                       HideMinimizeButton) {
  GetPrefs()->SetBoolean(prefs::kShowMinimizeButton, false);
  base::RunLoop().RunUntilIdle();
  
  EXPECT_FALSE(IsMinimizeButtonVisible());
  EXPECT_TRUE(IsMaximizeButtonVisible());
  EXPECT_TRUE(IsCloseButtonVisible());
}

// Test hiding maximize button via preferences
IN_PROC_BROWSER_TEST_F(WindowControlsManagerBrowserTest, 
                       HideMaximizeButton) {
  GetPrefs()->SetBoolean(prefs::kShowMaximizeButton, false);
  base::RunLoop().RunUntilIdle();
  
  EXPECT_TRUE(IsMinimizeButtonVisible());
  EXPECT_FALSE(IsMaximizeButtonVisible());
  EXPECT_TRUE(IsCloseButtonVisible());
}

// Test hiding close button via preferences
IN_PROC_BROWSER_TEST_F(WindowControlsManagerBrowserTest, 
                       HideCloseButton) {
  GetPrefs()->SetBoolean(prefs::kShowCloseButton, false);
  base::RunLoop().RunUntilIdle();
  
  EXPECT_TRUE(IsMinimizeButtonVisible());
  EXPECT_TRUE(IsMaximizeButtonVisible());
  EXPECT_FALSE(IsCloseButtonVisible());
}

// Test hiding all buttons
IN_PROC_BROWSER_TEST_F(WindowControlsManagerBrowserTest, 
                       HideAllButtons) {
  GetPrefs()->SetBoolean(prefs::kShowMinimizeButton, false);
  GetPrefs()->SetBoolean(prefs::kShowMaximizeButton, false);
  GetPrefs()->SetBoolean(prefs::kShowCloseButton, false);
  base::RunLoop().RunUntilIdle();
  
  EXPECT_FALSE(IsMinimizeButtonVisible());
  EXPECT_FALSE(IsMaximizeButtonVisible());
  EXPECT_FALSE(IsCloseButtonVisible());
}

// Test toggling button visibility
IN_PROC_BROWSER_TEST_F(WindowControlsManagerBrowserTest, 
                       ToggleButtonVisibility) {
  // Hide minimize button
  GetPrefs()->SetBoolean(prefs::kShowMinimizeButton, false);
  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(IsMinimizeButtonVisible());
  
  // Show it again
  GetPrefs()->SetBoolean(prefs::kShowMinimizeButton, true);
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(IsMinimizeButtonVisible());
}

