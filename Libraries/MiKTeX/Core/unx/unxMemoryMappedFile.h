/* unxMemoryMappedFile.h:					-*- C++ -*-

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

#if ! defined(MIKTEX__5B0110DD_E634_4A03_8D32_979229E45663__)
#define MIKTEX__5B0110DD_E634_4A03_8D32_979229E45663__

BEGIN_INTERNAL_NAMESPACE;

class unxMemoryMappedFile : public MemoryMappedFile
{
public:
  unxMemoryMappedFile ();

public:
  virtual
  ~unxMemoryMappedFile ();

public:
  virtual
  void *
  Open (/*[in]*/ const char *	lpszPath,
	/*[in]*/ bool			readWrite);

public:
  virtual
  void
  Close ();

public:
  virtual
  void *
  Resize (/*[in]*/ size_t newSize);

public:
  virtual
  void *
  GetPtr ()
  {
    return (ptr);
  }

public:
  virtual
  const char *
  GetName ()
  {
    return (name.c_str());
  }

public:
  virtual
  size_t
  GetSize ()
  {
    return (size);
  }

public:
  virtual
  void
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
  int filedes;
};

END_INTERNAL_NAMESPACE;

#endif	// unxMemoryMappedFile.h
