// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base3/process.h"
#include "base3/logging.h"
#include "base3/process_util.h"
#include "base3/scoped_ptr.h"

namespace base {

void Process::Close() {
  if (!process_)
    return;
  ::CloseHandle(process_);
  process_ = NULL;
}

void Process::Terminate(int result_code) {
  if (!process_)
    return;
  ::TerminateProcess(process_, result_code);
}

bool Process::IsProcessBackgrounded() const {
  if (!process_)
    return false;  // Failure case.
  DWORD priority = GetPriority();
  if (priority == 0)
    return false;  // Failure case.
  return priority == BELOW_NORMAL_PRIORITY_CLASS;
}

bool Process::SetProcessBackgrounded(bool value) {
  if (!process_)
    return false;
  DWORD priority = value ? BELOW_NORMAL_PRIORITY_CLASS : NORMAL_PRIORITY_CLASS;
  return (SetPriorityClass(process_, priority) != 0);
}

ProcessId Process::pid() const {
  if (process_ == 0)
    return 0;

  return GetProcId(process_);
}

bool Process::is_current() const {
  return process_ == GetCurrentProcess();
}

// static
Process Process::Current() {
  return Process(GetCurrentProcess());
}

int Process::GetPriority() const {
  DCHECK(process_);
  return GetPriorityClass(process_);
}

}  // namespace base
