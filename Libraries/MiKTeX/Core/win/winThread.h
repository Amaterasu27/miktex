/* winThread.h:							-*- C++ -*-

   Copyright (C) 1996-2007 Christian Schenk

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

class winThread : public Thread
{
public:
  virtual
  void
  MIKTEXTHISCALL
  Join ();

private:
  winThread (/*[in]*/ void (MIKTEXCALLBACK * function) (void *),
	     /*[in]*/ void * argument);

private:
  virtual
  ~winThread ();

private:
  static
  unsigned
  __stdcall
  ThreadFunc (/*[in]*/ void * p);

private:
  void (MIKTEXCALLBACK * function) (void *);

private:
  void * argument;

private:
  HANDLE handle;

private:
  unsigned id;

private:
  HANDLE hStarted;

private:
  friend class Thread;
};
