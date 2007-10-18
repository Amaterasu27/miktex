/* 1.cpp:

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

#include <MiKTeX/Core/Test>

#define N 1000

BEGIN_TEST_SCRIPT();

bool stopped;

int n;
int n1;
int n2;

MIKTEX_DEFINE_LOCK(n);

BEGIN_TEST_FUNCTION(1);
{
  TEST (n == N);
  TEST (n1 + n2 == n);
}
END_TEST_FUNCTION();

void
Incrementer1 ()
{
  while (! stopped)
    {
      MIKTEX_LOCK(n)
	{
	  if (n == N)
	    {
	      return;
	    }
	  ++ n;
	  ++ n1;
	}
      MIKTEX_UNLOCK();
      MiKTeX::Core::Thread::Sleep (0);
    }
}

void
Incrementer2()
{
  while (! stopped)
    {
      MIKTEX_LOCK(n)
	{
	  MIKTEX_LOCK(n)
	    {
	      if (n == N)
		{
		  return;
		}
	      ++ n;
	      ++ n2;
	    }
	  MIKTEX_UNLOCK();
	}
      MIKTEX_UNLOCK();
      MiKTeX::Core::Thread::Sleep (0);
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
  stopped = false;
  n = 0;
  n1 = 0;
  n2 = 0;

  std::auto_ptr<MiKTeX::Core::Thread> thread1 (MiKTeX::Core::Thread::Start(ThreadFunc1,
							       this));
					 
  std::auto_ptr<MiKTeX::Core::Thread> thread2 (MiKTeX::Core::Thread::Start(ThreadFunc2,
							       this));

  thread1->Join ();
  thread2->Join ();

  PRINT ("%d + %d\n", n1, n2);

  CALL_TEST_FUNCTION(1);
}
END_TEST_PROGRAM();

END_TEST_SCRIPT();

RUN_TEST_SCRIPT ();
