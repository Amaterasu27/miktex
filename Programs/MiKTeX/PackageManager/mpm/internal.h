/* internal.h:							-*- C++ -*-

   Copyright (C) 2003-2006 Christian Schenk

   This file is part of MpmCon.

   MpmCon is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MpmCon is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MpmCon; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

/* _________________________________________________________________________

   AutoResource
   _________________________________________________________________________ */

template<class HandleType>
HandleType
InvalidHandleValue ()
{
  return (0);
}

template<class HandleType,
	 class Destroyer>
class AutoResource
{
public:
  AutoResource (/*[in]*/ HandleType handle = InvalidHandleValue<HandleType>())
    : handle (handle)
  {
  }

public:
  ~AutoResource ()
  {
    try
      {
	Reset ();
      }
    catch (const exception &)
      {
      }
  }

public:
  HandleType
  Get ()
    const
  {
    return (handle);
  }

public:
  void
  Detach ()
  {
    handle = InvalidHandleValue<HandleType>();
  }

public:
  void
  Reset ()
  {
    if (handle != InvalidHandleValue<HandleType>())
      {
	HandleType tmp = handle;
	handle = InvalidHandleValue<HandleType>();
	Destroyer() (tmp);
      }
  }

public:
  HandleType *
  operator & ()
  {
    return (&handle);
  }

private:
  HandleType handle;
};

/* _________________________________________________________________________

   AutoFILE
   _________________________________________________________________________ */

class fclose_
{
public:
  void
  operator() (/*[in]*/ FILE * pFile)
  {
    fclose (pFile);
  }
};

typedef AutoResource<FILE *, fclose_> AutoFILE;
