/* winMemoryMappedFile.h:					-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(MIKTEX__FD179BEF_B02A_4A18_90F6_6C7AB4A22B6D__)
#define MIKTEX__FD179BEF_B02A_4A18_90F6_6C7AB4A22B6D__

BEGIN_INTERNAL_NAMESPACE;

class winMemoryMappedFile : public MemoryMappedFile
{
public:
  winMemoryMappedFile ();

public:
  virtual
  ~winMemoryMappedFile ();

public:
  virtual
  void * 
  MIKTEXTHISCALL
  Open (/*[in]*/ const char *	lpszPath,
	/*[in]*/ bool			readWrite);

public:
  virtual
  void
  MIKTEXTHISCALL
  Close ();

public:
  virtual
  void *
  MIKTEXTHISCALL
  Resize (/*[in]*/ size_t newSize);

public:
  virtual
  void *
  MIKTEXTHISCALL
  GetPtr ()
  {
    return (ptr);
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetName ()
  {
    return (name.c_str());
  }

public:
  virtual
  size_t
  MIKTEXTHISCALL
  GetSize ()
  {
    return (size);
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  Flush ();

private:
  void
  OpenFile ();

private:
  void
  CreateMapping (/*[in]*/ size_t	maximumFileSize);

private:
  void
  CloseFile ();

private:
  void
  DestroyMapping ();

private:
  void * ptr;

private:
  bool readWrite;

private:
  string path;

private:
  string name;

private:
  size_t size;

private:
  HANDLE hFile;

private:
  HANDLE hMapping;

private:
  auto_ptr<TraceStream> traceStream;
};

END_INTERNAL_NAMESPACE;

#endif	// winMemoryMappedFile.h
