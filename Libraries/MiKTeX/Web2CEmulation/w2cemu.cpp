/* w2cemu.cpp: texk compatibility functions

   Copyright (C) 2010 Christian Schenk

   This file is part of the MiKTeX W2CEMU Library.

   The MiKTeX W2CEMU Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX W2CEMU Library is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX W2CEMU Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

using namespace std;
using namespace MiKTeX;
using namespace MiKTeX::Core;

/* _________________________________________________________________________

   TranslateModeString
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
TranslateModeString (/*[in]*/ const char *	lpszMode,
		     /*[out]*/ FileMode &	mode,
		     /*[out]*/ FileAccess &	access,
		     /*[out]*/ bool &		isTextFile)
{
  if (StringCompare(lpszMode, "r") == 0)
    {
      mode = FileMode::Open;
      access = FileAccess::Read;
      isTextFile = true;
    }
  else if (StringCompare(lpszMode, "w") == 0)
    {
      mode = FileMode::Create;
      access = FileAccess::Write;
      isTextFile = true;
    }
  else if (StringCompare(lpszMode, "rb") == 0)
    {
      mode = FileMode::Open;
      access = FileAccess::Read;
      isTextFile = false;
    }
  else if (StringCompare(lpszMode, "wb") == 0)
    {
      mode = FileMode::Create;
      access = FileAccess::Write;
      isTextFile = false;
    }
  else if (StringCompare(lpszMode, "ab") == 0)
    {
      mode = FileMode::Append;
      access = FileAccess::Write;
      isTextFile = false;
    }
  else
    {
      Session::FatalMiKTeXError ("TranslateModeString",
				 T_("Unsupported mode."),
				 lpszMode,
				 __FILE__,
				 __LINE__);

    }
}

/* _________________________________________________________________________

   TryFOpen
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(FILE *)
TryFOpen (/*[in]*/ const char *	lpszFileName,
	  /*[in]*/ const char *	lpszMode)
{
  FileMode mode (FileMode::Open);
  FileAccess access (FileAccess::Read);
  bool isTextFile;
  TranslateModeString (lpszMode, mode, access, isTextFile);
  return (SessionWrapper(true)->TryOpenFile(lpszFileName,
					    mode,
					    access,
					    isTextFile));
}

/* _________________________________________________________________________

   Web2C::OpenInput
   _________________________________________________________________________ */

MIKTEXW2CCEEAPI(int)
Web2C::OpenInput (/*[in,out]*/ char *			lpszFileName,
		  /*[in]*/ FILE **			ppfile,
		  /*[in]*/ kpse_file_format_type	format,
		  /*[in]*/ const char *			lpszMode)
{
  char * lpszPath = miktex_kpse_find_file(lpszFileName, format, 0);
  if (lpszPath == 0)
    {
      return (0);
    }
  try
    {
      *ppfile = TryFOpen(lpszPath, lpszMode);
    }
  catch (const exception &)
    {
      MIKTEX_FREE (lpszPath);
      throw;
    }
  if (*ppfile != 0)
    {
      Utils::CopyString (lpszFileName, BufferSizes::MaxPath, lpszPath);
    }
  MIKTEX_FREE (lpszPath);
  return (*ppfile == 0 ? 0 : 1);
}

/* _________________________________________________________________________

   miktex_web2c_version_string
   _________________________________________________________________________ */

MIKTEXW2CDATA(const char *)
miktex_web2c_version_string = "MiKTeX-web2c-emulation";

/* _________________________________________________________________________

   Web2C::GetSecondsAndMicros
   _________________________________________________________________________ */

MIKTEXW2CCEEAPI(void)
Web2C::GetSecondsAndMicros (/*[out]*/ integer * pSeconds,
			    /*[out]*/ integer * pMicros)
{
#if defined(MIKTEX_WINDOWS)
  unsigned long clock = GetTickCount();
  *pSeconds = clock / 1000;
  *pMicros = clock % 1000;
#else
  struct timeval tv;
  gettimeofday (&tv, 0);
  *pSeconds = tv.tv_sec;
  *pMicros = tv.tv_usec;
#endif
}
