// Copyright 2016 Cheng Zhao. All rights reserved.
// Use of this source code is governed by the license that can be found in the
// LICENSE file.

#ifndef NATIVEUI_STATE_H_
#define NATIVEUI_STATE_H_

#include <memory>

#include "base/memory/ref_counted.h"
#include "nativeui/app.h"

namespace nu {

class Font;

#if defined(OS_WIN)
class ClassRegistrar;
class NativeTheme;
class SubwinHolder;
#endif

class NATIVEUI_EXPORT State {
 public:
  State();
  ~State();

  static State* GetCurrent();

  // Returns the default GUI font.
  Font* GetDefaultFont();

  // Returns the instance of App.
  App* app() { return &app_; }

  // Internal classes.
#if defined(OS_WIN)
  HWND GetSubwinHolder();
  ClassRegistrar* GetClassRegistrar();
  NativeTheme* GetNativeTheme();
  UINT GetNextCommandID();
#endif

 private:
  void PlatformInit();
  void PlatformDestroy();

  scoped_refptr<Font> default_font_;

#if defined(OS_WIN)
  ULONG_PTR token_;
  std::unique_ptr<ClassRegistrar> class_registrar_;
  std::unique_ptr<SubwinHolder> subwin_holder_;
  std::unique_ptr<NativeTheme> native_theme_;

  // Next ID for custom WM_COMMAND items, the number came from:
  // https://msdn.microsoft.com/en-us/library/11861byt.aspx
  UINT next_command_id_ = 0x8000;
#endif

  // The app instance.
  App app_;

  DISALLOW_COPY_AND_ASSIGN(State);
};

}  // namespace nu

#endif  // NATIVEUI_STATE_H_
