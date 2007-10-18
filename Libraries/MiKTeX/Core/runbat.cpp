/* runbat.cpp:

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

using namespace MiKTeX::Core;

/* _________________________________________________________________________

   SessionImpl::FindBatchFile
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
bool
SessionImpl::FindBatchFile (/*[in]*/ const char *	lpszName,
			    /*[out]*/ PathName &	path)
{
  return (FindFile(lpszName, FileType::WindowsCommandScriptFile, path));
}
#endif

/* _________________________________________________________________________

   SessionImpl::RunBatch
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
int
SessionImpl::RunBatch (/*[in]*/ int			argc,
		       /*[in]*/ const char **	argv)
{
  MIKTEX_ASSERT (argc > 0);

  // determine batch name
  char szName[BufferSizes::MaxPath];
  PathName::Split (argv[0],
		   0, 0,
		   szName, BufferSizes::MaxPath,
		   0, 0);

  // find batch file
  PathName scriptPath;
  if (! FindBatchFile(szName, scriptPath))
    {
      FATAL_MIKTEX_ERROR
	("",
	 T_("The Windows command script file could not be found."),
	 szName);
    }
  
  // we cannot quote the command => remove all blanks from the script path
  Utils::RemoveBlanksFromPathName (scriptPath);

  // build command line
  CommandLineBuilder batchCommandLine;
  batchCommandLine.AppendUnquoted (scriptPath.Get());
  if (argc > 1)
    {
      batchCommandLine.AppendArguments (argc - 1, &argv[1]);
    }

  int exitCode;

  Process::ExecuteSystemCommand (batchCommandLine.Get(), &exitCode);

  return (exitCode);
}
#endif
