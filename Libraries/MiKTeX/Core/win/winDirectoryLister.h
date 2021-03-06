/* winDirectoryLister.h:					-*- C++ -*-

   Copyright (C) 1996-2010 Christian Schenk

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(MIKTEX__F05A2700_545C_4487_889C_F7A396F930A1__)
#define MIKTEX__F05A2700_545C_4487_889C_F7A396F930A1__

BEGIN_INTERNAL_NAMESPACE;

class winDirectoryLister : public DirectoryLister
{
public:
  virtual
  void
  MIKTEXTHISCALL
  Close ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetNext (/*[out]*/ DirectoryEntry & direntry);

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetNext (/*[out]*/ DirectoryEntry2 & direntry2);

public:
  winDirectoryLister (/*[in]*/ const PathName &		directory,
		      /*[in]*/ const char *		lpszPattern,
		      /*[in]*/ Options			options);

public:
  virtual
  MIKTEXTHISCALL
  ~winDirectoryLister ();

private:
  PathName directory;

private:
  string pattern;

private:
  Options options;

private:
  HANDLE handle;

private:
  friend class DirectoryLister;
};

END_INTERNAL_NAMESPACE;

#endif	// winDirectoryLister.h
