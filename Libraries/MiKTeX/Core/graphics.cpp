/* graphics.cpp: graphics utilities

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

   SessionImpl::FindGraphicsRule
   _________________________________________________________________________ */

bool
SessionImpl::FindGraphicsRule (/*[in]*/ const MIKTEXCHAR *	lpszFrom,
			       /*[in]*/ const MIKTEXCHAR *	lpszTo,
			       /*[out]*/ MIKTEXCHAR *		lpszRule,
			       /*[in]*/ size_t			bufSize)
{
  MIKTEX_ASSERT_STRING (lpszFrom);
  MIKTEX_ASSERT_STRING (lpszTo);

  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszRule, bufSize);

  tstring strKey = lpszFrom;
  strKey += lpszTo;
  
  tstring strRule;

  bool b = GetSessionValue(T_("Graphics"), strKey.c_str(), strRule, 0);

  if (! b)
    {
      return (false);
    }

  if (lpszRule != 0)
    {
      Utils::CopyString (lpszRule, bufSize, strRule.c_str());
    }

  return (true);
}

/* _________________________________________________________________________

   SessionImpl::ConvertToBitmapFile
   _________________________________________________________________________ */

bool
SessionImpl::ConvertToBitmapFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
				  /*[out]*/ MIKTEXCHAR *	lpszBmpFile,
				  /*[in]*/ IRunProcessCallback * pCallback)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_PATH_BUFFER (lpszBmpFile);

  const MIKTEXCHAR * lpszExt = GetFileNameExtension(lpszPath);

  if (lpszExt == 0)
    {
      FATAL_MIKTEX_ERROR (T_(""),
			  T_("no file name extension"),
			  lpszPath);
    }

  const size_t MAXRULE = 1024;
  MIKTEXCHAR szRule[MAXRULE];

  if (! FindGraphicsRule(lpszExt, T_(".bmp"), szRule, MAXRULE))
    {
      FATAL_MIKTEX_ERROR (T_(""),
			  T_("no conversion rule"),
			  lpszPath);
    }

  PathName temp;
  temp.SetToTempFile ();
#if defined(MIKTEX_WINDOWS)
  Utils::RemoveBlanksFromPathName (temp);
#endif

  Utils::CopyString (lpszBmpFile, BufferSizes::MaxPath, temp.Get());
  
  tstring strCommandLine;
  strCommandLine.reserve (256);

  for (const MIKTEXCHAR * lpsz = szRule; *lpsz != 0; ++ lpsz)
    {
      if (*lpsz == T_('%'))
	{
	  ++ lpsz;
	  switch (*lpsz)
	    {
	    case T_('i'):
	      strCommandLine += lpszPath;
	      break;
	    case T_('o'):
	      strCommandLine += lpszBmpFile;
	      break;
	    }
	}
      else
	{
	  strCommandLine += *lpsz;
	}
    }

  bool bDone =
    Process::ExecuteSystemCommand(strCommandLine.c_str(),
				  0,
				  pCallback,
				  0);
  
  if (! bDone)
    {
      File::Delete (lpszBmpFile, true);
    }

  return (bDone);
}
