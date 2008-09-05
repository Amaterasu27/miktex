/* 1.cpp:

   Copyright (C) 1996-2008 Christian Schenk

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

#include <miktex/Core/Test>

#define N 1000000

#define RECURSIVE 0

BEGIN_TEST_SCRIPT();

MIKTEX_DEFINE_LOCK(started);
MIKTEX_DEFINE_LOCK(stopped);
MIKTEX_DEFINE_LOCK(n);

volatile bool started;
volatile bool stopped;

volatile int n;
volatile int n1;
volatile int n2;

BEGIN_TEST_FUNCTION(1);
{
  TEST (n == N);
  TEST (n1 + n2 == n);
}
END_TEST_FUNCTION();

void
Incrementer1 ()
{
  for (;;)
    {
      MIKTEX_LOCK(started)
	{
	  if (started)
	    {
	      break;
	    }
	}
      MIKTEX_UNLOCK();
      MiKTeX::Core::Thread::Sleep (0);
    }
  for (;;)
    {
      MIKTEX_LOCK(stopped)
	{
	  if (stopped)
	    {
	      break;
	    }
	}
      MIKTEX_UNLOCK();
      unsigned wait = 0;
      MIKTEX_LOCK(n)
	{
#if RECURSIVE
	  MIKTEX_LOCK(n)
	    {
	      MIKTEX_LOCK(n)
		{
#endif
		  if (n == N)
		    {
		      return;
		    }
		  ++ n;
		  ++ n1;
		  if (n % 7 == 0)
		    {
		      wait = 1;
		    }
#if RECURSIVE
		}
	      MIKTEX_UNLOCK();
	    }
	  MIKTEX_UNLOCK();
#endif
	}
      MIKTEX_UNLOCK();
      MiKTeX::Core::Thread::Sleep (wait);
    }
}

void
Incrementer2()
{
  for (;;)
    {
      MIKTEX_LOCK(started)
	{
	  if (started)
	    {
	      break;
	    }
	}
      MIKTEX_UNLOCK();
      MiKTeX::Core::Thread::Sleep (0);
    }
  for (;;)
    {
      MIKTEX_LOCK(stopped)
	{
	  if (stopped)
	    {
	      break;
	    }
	}
      MIKTEX_UNLOCK();
      unsigned wait = 0;
      MIKTEX_LOCK(n)
	{
#if RECURSIVE
	  MIKTEX_LOCK(n)
	    {
	      MIKTEX_LOCK(n)
		{
#endif
		  if (n == N)
		    {
		      return;
		    }
		  ++ n;
		  ++ n2;
		  if (n % 7 == 0)
		    {
		      wait = 1;
		    }
#if RECURSIVE
		}
	      MIKTEX_UNLOCK();
	    }
	  MIKTEX_UNLOCK();
#endif
	}
      MIKTEX_UNLOCK();
      MiKTeX::Core::Thread::Sleep (wait);
    }
}

void
Thread1 ()
{
  try
    {
      Incrementer1 ();
    }
  catch (const TestScript::FAILURE & f)
    {
      PRINT_FAILURE (f);
    }
  catch (const MiKTeX::Core::MiKTeXException & e)
    {
      MiKTeX::Core::Utils::PrintException (e);
    }
}

static
void
MIKTEXCALLBACK
ThreadFunc1 (/*[in]*/ void * p)
{
  MyTestScript * This = reinterpret_cast<MyTestScript*>(p);
  This->Thread1 ();
}

void
Thread2 ()
{
  try
    {
      Incrementer2 ();
    }
  catch (const MiKTeX::Core::MiKTeXException & e)
    {
      MiKTeX::Core::Utils::PrintException (e);
    }
}

static
void
MIKTEXCALLBACK
ThreadFunc2 (/*[in]*/ void * p)
{
  MyTestScript * This = reinterpret_cast<MyTestScript*>(p);
  This->Thread2 ();
}

BEGIN_TEST_PROGRAM();
{
  started = false;
  stopped = false;
  n = 0;
  n1 = 0;
  n2 = 0;

  std::auto_ptr<MiKTeX::Core::Thread> thread1
    (MiKTeX::Core::Thread::Start(ThreadFunc1, this));
					 
  std::auto_ptr<MiKTeX::Core::Thread> thread2
    (MiKTeX::Core::Thread::Start(ThreadFunc2, this));

  MIKTEX_LOCK(started)
    {
      started = true;
    }
  MIKTEX_UNLOCK();

  thread1->Join ();
  thread2->Join ();

  PRINT ("%d + %d\n", n1, n2);

  CALL_TEST_FUNCTION(1);
}
END_TEST_PROGRAM();

END_TEST_SCRIPT();

RUN_TEST_SCRIPT ();

