/* DllProc.cpp:

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

#include "StdAfx.h"

#include "internal.h"

#include <miktex/win/DllProc.h>

/* _________________________________________________________________________

   DllProcBase::DllProcBase
   _________________________________________________________________________ */

MIKTEXCALL
DllProcBase::DllProcBase ()
  : hModule (0),
    proc (0)
{
}

/* _________________________________________________________________________

   DllProcBase::DllProcBase
   _________________________________________________________________________ */

MIKTEXCALL
DllProcBase::DllProcBase (/*[in]*/ const MIKTEXCHAR *	lpszDllName,
			  /*[in]*/ const MIKTEXCHAR *	lpszProcName)
  : dllName (lpszDllName),
    procName (lpszProcName),
    hModule (0),
    proc (0)
{
}

/* _________________________________________________________________________

   DllProcBase::~DllProcBase
   _________________________________________________________________________ */

MIKTEXCALL
DllProcBase::~DllProcBase ()
{
  try
    {
      if (hModule != 0 && ! FreeLibrary(hModule))
	{
	  FATAL_WINDOWS_ERROR (T_("FreeLibrary"), 0);
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   DllProcBase::GetProc
   _________________________________________________________________________ */

FARPROC
MIKTEXCALL
DllProcBase::GetProc ()
{
  if (proc != 0)
    {
      return (proc);
    }
  if (hModule == 0)
    {
      hModule = LoadLibrary(dllName.c_str());
      if (hModule == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("DllProcBase::GetProc"),
			       dllName.c_str());
	}
    }
  proc = GetProcAddress(hModule, procName.c_str());
  if (proc == 0)
    {
      FATAL_WINDOWS_ERROR (T_("DllProcBase::GetProc"),
			   procName.c_str());
    }
  return (proc);
}
