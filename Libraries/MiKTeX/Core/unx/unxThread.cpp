/* unxThread.cpp:

   Copyright (C) 1996-2009 Christian Schenk

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

#if ! defined(HAVE_PTHREAD)
#  error this system does not have POSIX threads libraries
#endif

#include "unx/unxThread.h"

/* _________________________________________________________________________

   Thread::Start
   _________________________________________________________________________ */

Thread *
Thread::Start (/*[in]*/ void (MIKTEXCALLBACK * function) (void *),
	       /*[in]*/ void * argument)
{
  return (new unxThread (function, argument));
}

/* _________________________________________________________________________

   Thread::Sleep
   _________________________________________________________________________ */

void
Thread::Sleep (/*[in]*/ int milliseconds)
{
  struct timespec requestedTime;
  requestedTime.tv_sec = milliseconds / 1000;
  requestedTime.tv_nsec = (milliseconds % 1000) * 1000000;
  if (nanosleep(&requestedTime, 0) < 0)
    {
      FATAL_CRT_ERROR ("nanosleep", NUMTOSTR(milliseconds));
    }
}

/* _________________________________________________________________________

   Thread::IsCurrentThread
   _________________________________________________________________________ */

bool
Thread::IsCurrentThread (/*[in]*/ Thread * pThread)
{
  unxThread * pUnxThread = dynamic_cast<unxThread *>(pThread);
  return (pthread_equal(pUnxThread->pthread, pthread_self()));
}

/* _________________________________________________________________________

   unxThread::unxThread
   _________________________________________________________________________ */

struct Data
{
  Data (/*[in]*/ void (MIKTEXCALLBACK * function) (void *),
	/*[in]*/ void * argument)
    : function(function),
      argument(argument)
  {
  }
  void (MIKTEXCALLBACK * function) (void *);
  void * argument;
};

unxThread::unxThread (/*[in]*/ void (MIKTEXCALLBACK * function) (void *),
		      /*[in]*/ void * argument)
{
  Data * pData = new Data (function, argument);
  int err = pthread_create(&pthread,
			   0,
			   ThreadFunc,
			   pData);
  if (err != 0)
    {
      free (pData);
      FATAL_CRT_ERROR_2 ("pthread_create", err, 0);
    }
  // thread is running => Data will be (has been) deleted by
  // ThreadFunc
}

/* _________________________________________________________________________

   unxThread::~unxThread
   _________________________________________________________________________ */

unxThread::~unxThread ()
{
  pthread_detach (pthread);
}

/* _________________________________________________________________________

   unxThread::ThreadFunc
   _________________________________________________________________________ */

void *
unxThread::ThreadFunc (/*[in]*/ void * p)
{
  try
    {
      Data * pData = reinterpret_cast<Data*>(p);
      void (MIKTEXCALLBACK * function) (void *) = pData->function;
      void * argument = pData->argument;
      delete pData;
      function (argument);
    }
  catch (const MiKTeXException &)
    {
    }
  return (0);
}

/* _________________________________________________________________________

   unxThread::Join
   _________________________________________________________________________ */

void
unxThread::Join ()
{
  int err = pthread_join(pthread, 0);
  if (err != 0)
    {
      FATAL_CRT_ERROR_2 ("pthread_join", err, 0);
    }
}

/* _________________________________________________________________________

   CriticalSectionMonitor::Initialize_
   _________________________________________________________________________ */

void
CriticalSectionMonitor::Initialize_ ()
{
  int err = pthread_mutexattr_init(&mutexAttr);
  if (err != 0)
    {
      FATAL_CRT_ERROR_2 ("pthread_mutexattr_init", err, 0);
    }
  err = pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
  if (err != 0)
    {
      FATAL_CRT_ERROR_2 ("pthread_mutexattr_settype", err, 0);
    }
  err = pthread_mutex_init(&mutex, &mutexAttr);
  if (err != 0)
    {
      FATAL_CRT_ERROR_2 ("pthread_mutex_init", err, 0);
    }
}

/* _________________________________________________________________________

   CriticalSectionMonitor::Delete_
   _________________________________________________________________________ */

void
CriticalSectionMonitor::Delete_ ()
{
  int err = pthread_mutex_destroy(&mutex);
  if (err != 0)
    {
      FATAL_CRT_ERROR_2 ("pthread_mutex_destroy", err, 0);
    }
  err = pthread_mutexattr_destroy(&mutexAttr);
  if (err != 0)
    {
      FATAL_CRT_ERROR_2 ("pthread_mutexattr_destroy", err, 0);
    }
}

/* _________________________________________________________________________

   CriticalSectionMonitor::Enter
   _________________________________________________________________________ */

void
CriticalSectionMonitor::Enter ()
{
  int err = pthread_mutex_lock(&mutex);
  if (err != 0)
    {
      FATAL_CRT_ERROR_2 ("pthread_mutex_lock", err, 0);
    }
  ++ nLocks;
}

/* _________________________________________________________________________

   CriticalSectionMonitor::Leave
   _________________________________________________________________________ */

void
CriticalSectionMonitor::Leave ()
{
  -- nLocks;
  int err = pthread_mutex_unlock(&mutex);
  if (err != 0)
    {
      ++ nLocks;
      FATAL_CRT_ERROR_2 ("pthread_mutex_unlock", err, 0);
    }
}

/* _________________________________________________________________________

   AutoResetEvent::AutoResetEvent
   _________________________________________________________________________ */

AutoResetEvent::AutoResetEvent ()
{
};

/* _________________________________________________________________________

   AutoResetEvent::~AutoResetEvent
   _________________________________________________________________________ */

AutoResetEvent::~AutoResetEvent ()
{
  try
  {
  }
  catch (const exception &)
  {
  }
};

/* _________________________________________________________________________

   AutoResetEvent::Reset
   _________________________________________________________________________ */

bool
AutoResetEvent::Reset ()
{
  UNIMPLEMENTED ("AutoResetEvent::Reset");
  return (true);
};

/* _________________________________________________________________________

   AutoResetEvent::Set
   _________________________________________________________________________ */

bool
AutoResetEvent::Set ()
{
  UNIMPLEMENTED ("AutoResetEvent::Set");
  return (true);
};

/* _________________________________________________________________________

   AutoResetEvent::WaitOne
   _________________________________________________________________________ */

bool
AutoResetEvent::WaitOne ()
{
  UNIMPLEMENTED ("AutoResetEvent::WaitOne");
  return (true);
};
