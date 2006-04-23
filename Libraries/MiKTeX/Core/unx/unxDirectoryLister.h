/* unxDirectoryLister.h:					-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(MIKTEX__917B21F1_CDC9_4791_A00B_3125B7A0F9B6__)
#define MIKTEX__917B21F1_CDC9_4791_A00B_3125B7A0F9B6__

BEGIN_INTERNAL_NAMESPACE;

class unxDirectoryLister : public DirectoryLister
{
public:
  virtual
  void
  MIKTEXCALL
  Close ();

public:
  virtual
  bool
  MIKTEXCALL
  GetNext (/*[out]*/ DirectoryEntry & direntry);

public:
  virtual
  bool
  MIKTEXCALL
  GetNext (/*[out]*/ DirectoryEntry2 & direntry2);

private:
  bool
  GetNext (/*[out]*/ DirectoryEntry2 &	direntry2,
	   /*[in]*/ bool		simple);

private:
  unxDirectoryLister (/*[in]*/ const PathName &		directory,
		      /*[in]*/ const MIKTEXCHAR *	lpszPattern);

private:
  virtual
  ~unxDirectoryLister ();

private:
  PathName directory;

private:
  DIR * pDir;

private:
  tstring pattern;

private:
  friend class DirectoryLister;
};

END_INTERNAL_NAMESPACE;

#endif	// unxDirectoryLister.h
