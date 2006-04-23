/* uncompress.cpp: uncompressing files

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

   Utils::UncompressFile
   _________________________________________________________________________ */

#if defined(MIKTEX_DLL)
void
Utils::UncompressFile (/*[in]*/ const MIKTEXCHAR *	lpszPathIn,
		       /*[out]*/ PathName &		pathOut)
{
  MIKTEX_ASSERT_STRING (lpszPathIn);

  SessionImpl::theSession->trace_process->WriteFormattedLine
    (T_("core"),
     T_("uncompressing %s..."),
     Q_(lpszPathIn));

  if (! File::Exists(lpszPathIn))
    {
      FATAL_MIKTEX_ERROR (T_("Utils::UncompressFile"),
			  T_("The file could not be found."),
			  lpszPathIn);
    }

  PathName pathTempFileName;

  pathTempFileName.SetToTempFile ();

  FileStream stream (File::Open(pathTempFileName,
				FileMode::Create,
				FileAccess::Write,
				false));

  gzFile gzfileIn = 0;
  BZFILE * bzfileIn = 0;

#if defined(MIKTEX_UNICODE)
#  error Unimplemented: UncompressFile()
#else
  if (PathName(lpszPathIn).HasExtension(T_(".gz")))
    {
      gzfileIn = gzopen(lpszPathIn, T_("rb"));
    }
  else if (PathName(lpszPathIn).HasExtension(T_(".bz2")))
    {
      bzfileIn = BZ2_bzopen(lpszPathIn, T_("rb"));
    }
  else
    {
      FATAL_MIKTEX_ERROR (T_("MiKTeX::Core::UncompressFile"),
			  T_("Could not uncompress file."),
			  lpszPathIn);
    }
#endif

  if (gzfileIn == 0 && bzfileIn == 0)
    {
      FATAL_MIKTEX_ERROR (T_("MiKTeX::Core::UncompressFile"),
			  T_("The compressed file could not be opened."),
			  lpszPathIn);
    }

  unsigned char szBuf[4096];
  size_t len;

  if (gzfileIn != 0)
    {
      try
	{
	  while ((len = gzread(gzfileIn, szBuf, ARRAY_SIZE(szBuf))) > 0)
	    {
	      stream.Write (szBuf, len);
	    }
	}
      catch (const exception &)
	{
	  gzclose (gzfileIn);
	  throw;
	}
      gzclose (gzfileIn);
    }
  else if (bzfileIn != 0)
    {
      try
	{
	  while ((len = BZ2_bzread(bzfileIn, szBuf, ARRAY_SIZE (szBuf))) > 0)
	    {
	      stream.Write (szBuf, len);
	    }
	}
      catch (const exception &)
	{
	  BZ2_bzclose (bzfileIn);
	  throw;
	}
      BZ2_bzclose (bzfileIn);
    }

  pathOut = pathTempFileName;
}
#endif

/* _________________________________________________________________________

   miktex_uncompress_file
   _________________________________________________________________________ */

#if defined(MIKTEX_DLL)
MIKTEXAPI(void)
miktex_uncompress_file (/*[in]*/ const MIKTEXCHAR *	lpszPathIn,
			/*[out]*/ MIKTEXCHAR *		lpszPathOut)
{
  C_FUNC_BEGIN ();
  PathName temp;
  Utils::UncompressFile (lpszPathIn, temp);
  Utils::CopyString (lpszPathOut, BufferSizes::MaxPath, temp.Get());
  C_FUNC_END ();
}
#endif
