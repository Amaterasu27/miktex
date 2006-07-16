/* gsinfo.cpp: getting Ghostscript information

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

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   ScanString
   _________________________________________________________________________ */

inline
bool
ScanString (/*[in]*/ const char *	lpszString,
	    /*[in]*/ const char *	lpszFormat,
	    /*[out]*/ unsigned &	u1,
	    /*[out]*/ unsigned &	u2)
{
  int n;
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  n = sscanf_s(lpszString, lpszFormat, &u1, &u2);
#else
  n = sscanf(lpszString, lpszFormat, &u1, &u2);
#endif
  if (n < 0 && n != EOF)
    {
      FATAL_CRT_ERROR (T_("sscanf"), 0);
    }
  return (n == 2);
}

/* _________________________________________________________________________

   SessionImpl::GetGhostscript
   _________________________________________________________________________ */

void
SessionImpl::GetGhostscript (/*[out]*/ MIKTEXCHAR *	lpszPath,
			     /*[out]*/ unsigned long *	pVersionNumber)
{
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  MIKTEX_ASSERT_BUFFER_OR_NIL (pVersionNumber, sizeof(*pVersionNumber));

  if (pathGsExe.Empty())
    {
      // try MiKTeX Ghostscript
      if (! FindFile(MIKTEX_GS_EXE, FileType::EXE, pathGsExe))
	{
	  FATAL_MIKTEX_ERROR (T_("SessionImpl::GetGhostscript"),
			      T_("MiKTeX Ghostscript could not be not found."),
			      0);
	}
    }

  if (pVersionNumber != 0 && gsVersion.n1 == 0)
    {
#if defined(_UNICODE)
#  error Unimplemented:	SessionImpl::GetGhostscript()	  
#endif
      int exitCode;
      AutoBuffer gsout (4096);
      size_t n = static_cast<unsigned>(gsout.GetSize());
      if (! Process::Run(pathGsExe.Get(),
			 T_("--version"),
			 gsout.GetBuffer(),
			 &n,
			 &exitCode))
	{
	  FATAL_MIKTEX_ERROR (T_("SessionImpl::GetGhostscript"),
			      T_("MiKTeX Ghostscript could not be run."),
			      0);
	}
      gsout.GetBuffer()[gsout.GetSize() - 1] = 0;
      gsVersion = VersionNumber::Parse(gsout.Get());
      if (exitCode != 0)
	{
	  TraceError (T_("%s"), gsout.GetBuffer());
	}
      trace_config->WriteFormattedLine (T_("core"),
					T_("Ghostscript version: %s"),
					gsVersion.ToString().c_str());
    }
  
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, pathGsExe.Get());

  if (pVersionNumber != 0)
    {
      *pVersionNumber = static_cast<unsigned long>(gsVersion.GetHighWord());
    }
}

/* _________________________________________________________________________

   miktex_get_gs_exe
   _________________________________________________________________________ */

MIKTEXAPI(void)
miktex_get_gs_exe (/*[out]*/ MIKTEXCHAR *	lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  SessionImpl::theSession->GetGhostscript (lpszPath, 0);
  C_FUNC_END ();
}
