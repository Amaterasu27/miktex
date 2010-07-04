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
  char * lpszPath = miktex_kpathsea_find_file(kpse_def, lpszFileName, format, 0);
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

   miktex_web2c_recorder_enabled
   _________________________________________________________________________ */

MIKTEXW2CDATA(boolean)
miktex_web2c_recorder_enabled = 0;

/* _________________________________________________________________________

   miktex_web2c_output_directory
   _________________________________________________________________________ */

MIKTEXW2CDATA(const_string)
miktex_web2c_output_directory = 0;

/* _________________________________________________________________________

   miktex_web2c_fullnameoffile
   _________________________________________________________________________ */

MIKTEXW2CDATA(char *)
miktex_web2c_fullnameoffile = 0;

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

/* _________________________________________________________________________

   miktex_zround

   Implementation found in texk/web2c/lib/zround.c.

   zround.c: round R to the nearest whole number.  This is supposed to
   implement the predefined Pascal round function.  Public domain.
   _________________________________________________________________________ */

MIKTEXW2CCEEAPI(integer)
miktex_zround (/*[in]*/ double r)
{
  integer i;

  /* R can be outside the range of an integer if glue is stretching or
     shrinking a lot.  We can't do any better than returning the largest
     or smallest integer possible in that case.  It doesn't seem to make
     any practical difference.  Here is a sample input file which
     demonstrates the problem, from phil@cs.arizona.edu:
     	\documentstyle{article}
	\begin{document}
	\begin{flushleft}
	$\hbox{} $\hfill 
	\filbreak
	\eject
    
     djb@silverton.berkeley.edu points out we should testing against
     TeX's largest or smallest integer (32 bits), not the machine's.  So
     we might as well use a floating-point constant, and avoid potential
     compiler bugs (also noted by djb, on BSDI).  */
  if (r > 2147483647.0)
    i = 2147483647;
  /* should be ...8, but atof bugs are too common */
  else if (r < -2147483647.0)
    i = -2147483647;
  /* Admittedly some compilers don't follow the ANSI rules of casting
     meaning truncating toward zero; but it doesn't matter enough to do
     anything more complicated here.  */
  else if (r >= 0.0)
    i = (integer)(r + 0.5);
  else
    i = (integer)(r - 0.5);

  return i;
}

/* _________________________________________________________________________

   miktex_uexit
   _________________________________________________________________________ */

MIKTEXW2CEXPORT MIKTEXNORETURN void MIKTEXCEECALL
miktex_uexit (int status)
{
  int final_code;
  if (status == 0)
  {
    final_code = EXIT_SUCCESS;
  }
  else if (status == 1)
  {
    final_code = EXIT_FAILURE;
  }
  else
  {
    final_code = status;
  }
  miktex_exit (final_code);
}

/* _________________________________________________________________________

   miktex_setupboundvariable
   _________________________________________________________________________ */

MIKTEXW2CCEEAPI(void)
miktex_setupboundvariable (/*[in]*/ integer *      pVar,
			   /*[in]*/ const char *   lpszVarName,
			   /*[in]*/ integer	      dflt)
{
  int ret = SessionWrapper(true)->GetConfigValue(0, lpszVarName, dflt);
  if (ret >= 0)
  {
    *pVar = ret;
  }
}

/* _________________________________________________________________________

   miktex_usagehelp
   _________________________________________________________________________ */

MIKTEXW2CEXPORT MIKTEXNORETURN void MIKTEXCEECALL
miktex_usagehelp (/*[in]*/ const char **	lpszLines,
		  /*[in]*/ const char *		lpszBugEmail)
{
  for ( ; *lpszLines != 0; ++ lpszLines)
    {
      puts (*lpszLines);
    }
  if (lpszBugEmail == 0)
    {
      printf ("\nVisit miktex.org for bug reports.\n");
    }
  else
    {
      printf ("\nEmail bug reports to %s.\n", lpszBugEmail);
    }
  miktex_exit (0);
}
