/* runperl.cpp:

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

   SessionImpl::FindPerl
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
#  define PERL_EXE T_("perl.exe")
#else
#  define PERL_EXE T_("perl")
#endif

bool
SessionImpl::FindPerl (/*[out]*/ PathName & perl)
{
  tstring path;
  if (! Utils::GetEnvironmentString(T_("PATH"), path))
    {
      return (false);
    }
  return (SlowFindFile(PERL_EXE, path.c_str(), perl));
}

/* _________________________________________________________________________

   SessionImpl::FindPerlScript
   _________________________________________________________________________ */

bool
SessionImpl::FindPerlScript (/*[in]*/ const MIKTEXCHAR *	lpszName,
			     /*[out]*/ PathName &		path)
{
  return (FindFile(PathName(0, lpszName, T_(".pl")).Get(),
		   FileType::PERLSCRIPT,
		   path));
}

/* _________________________________________________________________________

   SessionImpl::RunPerl
   _________________________________________________________________________ */

int
SessionImpl::RunPerl (/*[in]*/ int			argc,
		      /*[in]*/ const MIKTEXCHAR **	argv)
{
  MIKTEX_ASSERT (argc > 0);

  // find Perl
  PathName perl;
  if (! FindPerl(perl))
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::RunPerl"),
			  T_("The Perl interpreter could not be found."),
			  0);
    }

  // determine script name
  MIKTEXCHAR szName[BufferSizes::MaxPath];
  PathName::Split (argv[0],
		   0, 0,
		   szName, BufferSizes::MaxPath,
		   0, 0);

  // find Perl script
  PathName scriptPath;
  if (! FindPerlScript(szName, scriptPath))
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::RunPerl"),
			  T_("The Perl script could not be found."),
			  szName);
    }
  
  // build command-line
  CommandLineBuilder perlCommandLine;
  perlCommandLine.AppendArgument (scriptPath.Get());
  if (argc > 1)
    {
      perlCommandLine.AppendArguments (argc - 1, &argv[1]);
    }

  int exitCode;

  Process::Run (perl.Get(), perlCommandLine.Get(), 0, &exitCode, 0);

  return (exitCode);
}
