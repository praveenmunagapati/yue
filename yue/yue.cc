// Copyright 2016 Cheng Zhao. All rights reserved.
// Use of this source code is governed by the license that can be found in the
// LICENSE file.

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "lua/callback.h"
#include "nativeui/init.h"
#include "nativeui/group.h"
#include "nativeui/label.h"
#include "nativeui/layout/box_layout.h"
#include "nativeui/window.h"

int main(int argc, const char *argv[]) {
  base::AtExitManager exit_manager;
  base::CommandLine::Init(argc, argv);

  logging::LoggingSettings settings;
  settings.logging_dest = logging::LOG_TO_SYSTEM_DEBUG_LOG;
  logging::InitLogging(settings);

  auto* cmd = base::CommandLine::ForCurrentProcess();
  if (cmd->GetArgs().size() != 1) {
    LOG(ERROR) << "Script expected";
    return 1;
  }

  base::MessageLoop message_loop(base::MessageLoop::TYPE_UI);

  lua::ManagedState state;
  if (!state) {
    LOG(ERROR) << "Failed to create state.";
    return 1;
  }
  luaL_openlibs(state);

#if 0
  if (luaL_loadfile(state, cmd->GetArgs()[0].c_str()) != LUA_OK ||
      !lua::PCall(state, nullptr)) {
    std::string error;
    lua::Pop(state, &error);
    LOG(ERROR) << "Error run script: " << error;
    return 1;
  }
#endif

  nu::Initialize();
  nu::Window::Options options = { gfx::Rect(100, 100, 400, 400) };
  scoped_refptr<nu::Window> window(new nu::Window(options));
  nu::Container* container = window->GetContentView();
  container->SetLayoutManager(new nu::BoxLayout(nu::BoxLayout::Horizontal));
  container->AddChildView(new nu::Label("col1"));
  container->AddChildView(new nu::Label("col2"));
  nu::Container* sub = new nu::Container;
  sub->SetLayoutManager(new nu::BoxLayout(nu::BoxLayout::Vertical));
  sub->AddChildView(new nu::Label("line1"));
  sub->AddChildView(new nu::Label("line2"));
  nu::Group* group = new nu::Group;
  group->SetTitle("Button Group");
  group->SetContentView(sub);
  container->AddChildView(group);
  window->SetVisible(true);

  base::RunLoop().Run();

  return 0;
}
