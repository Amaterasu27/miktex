/* winThread.cpp:

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#include "win/winThread.h"

/* _________________________________________________________________________

   Thread::Start
   _________________________________________________________________________ */

Thread *
Thread::Start (/*[in]*/ void (MIKTEXCALLBACK * function) (void *),
	       /*[in]*/ void * argument)
{
  return (new winThread (function, argument));
}

/* _________________________________________________________________________

   Thread::Sleep
   _________________________________________________________________________ */

void
Thread::Sleep (/*[in]*/ int milliseconds)
{
  ::Sleep (milliseconds);
}

/* _________________________________________________________________________

   Thread::IsCurrentThread
   _________________________________________________________________________ */

bool
Thread::IsCurrentThread (/*[in]*/ Thread * pThread)
{
  winThread * pWinThread = dynamic_cast<winThread *>(pThread);
  return (pWinThread->id == GetCurrentThreadId());
}

/* _________________________________________________________________________

   winThread::winThread
   _________________________________________________________________________ */

winThread::winThread (/*[in]*/ void (MIKTEXCALLBACK * function) (void *),
		      /*[in]*/ void * argument)
{
  this->function = function;
  this->argument = argument;
  handle =
    reinterpret_cast<HANDLE>(_beginthreadex(0,
					    0,
					    ThreadFunc,
					    this,
					    0,
					    &id));
  if (handle == 0)
    {
      FATAL_CRT_ERROR (T_("_beginthreadex"), 0);
    }
}

/* _________________________________________________________________________

   winThread::~winThread
   _________________________________________________________________________ */

winThread::~winThread ()
{
  if (handle != 0)
    {
      CloseHandle (handle);
      handle = 0;
    }
}

/* _________________________________________________________________________

   winThread::ThreadFunc
   _________________________________________________________________________ */

unsigned
__stdcall
winThread::ThreadFunc (/*[in]*/ void * p)
{
  try
    {
      winThread * This = reinterpret_cast<winThread*>(p);
      This->function (This->argument);
    }
  catch (const MiKTeXException &)
    {
    }
  return (0);
}

/* _________________________________________________________________________

   winThread::Join
   _________________________________________________________________________ */

void
winThread::Join ()
{
  WaitForSingleObject (handle, INFINITE);
}

/* _________________________________________________________________________

   CriticalSectionMonitor::Initialize_
   _________________________________________________________________________ */

void
CriticalSectionMonitor::Initialize_ ()
{
  InitializeCriticalSection (&critsec);
}

/* _________________________________________________________________________

   CriticalSectionMonitor::Delete_
   _________________________________________________________________________ */

void
CriticalSectionMonitor::Delete_ ()
{
  DeleteCriticalSection (&critsec);
}

/* _________________________________________________________________________

   CriticalSectionMonitor::Enter
   _________________________________________________________________________ */

void
CriticalSectionMonitor::Enter ()
{
  EnterCriticalSection (&critsec);
  InterlockedIncrement (const_cast<long*>(&nLocks));
}

/* _________________________________________________________________________

   CriticalSectionMonitor::Leave
   _________________________________________________________________________ */

void
CriticalSectionMonitor::Leave ()
{
  MIKTEX_ASSERT (IsLocked());
  InterlockedDecrement (const_cast<long*>(&nLocks));
  LeaveCriticalSection (&critsec);
}
