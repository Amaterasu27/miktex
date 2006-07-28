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

void
Utils::UncompressFile (/*[in]*/ const MIKTEXCHAR *	lpszPathIn,
		       /*[out]*/ PathName &		pathOut)
{
  MIKTEX_ASSERT_STRING (lpszPathIn);

  SessionImpl::GetSession()->trace_process->WriteFormattedLine
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

  FILE * pInputStream = 0;

  if (PathName(lpszPathIn).HasExtension(T_(".gz")))
    {
      CommandLineBuilder cmd (T_("zcat"));
      cmd.AppendArgument (lpszPathIn);
      pInputStream =
	SessionImpl::GetSession()->OpenFile(cmd.Get(),
					    FileMode::Command,
					    FileAccess::Read,
					    false);
    }
  else if (PathName(lpszPathIn).HasExtension(T_(".bz2")))
    {
      CommandLineBuilder cmd (T_("bzcat"));
      cmd.AppendArgument (lpszPathIn);
      pInputStream =
	SessionImpl::GetSession()->OpenFile(cmd.Get(),
					    FileMode::Command,
					    FileAccess::Read,
					    false);
    }
  else
    {
      FATAL_MIKTEX_ERROR (T_("MiKTeX::Core::UncompressFile"),
			  T_("Could not uncompress file."),
			  lpszPathIn);
    }

  unsigned char szBuf[4096];

  size_t len;

  try
    {
      while ((len = fread(szBuf, 1, ARRAY_SIZE(szBuf), pInputStream)) > 0)
	{
	  stream.Write (szBuf, len);
	}
    }
  catch (const exception &)
    {
      SessionImpl::GetSession()->CloseFile (pInputStream);
      throw;
    }

  SessionImpl::GetSession()->CloseFile (pInputStream);

  pathOut = pathTempFileName;
}

/* _________________________________________________________________________

   miktex_uncompress_file
   _________________________________________________________________________ */

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
