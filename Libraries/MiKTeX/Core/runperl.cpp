/* runperl.cpp:

   Copyright (C) 1996-2010 Christian Schenk

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
#  define PERL_EXE "perl.exe"
#else
#  define PERL_EXE "perl"
#endif

bool
SessionImpl::FindPerl (/*[out]*/ PathName & perl)
{
  string path;
  if (! Utils::GetEnvironmentString("PATH", path))
    {
      return (false);
    }
  return (FindFile(PERL_EXE, path.c_str(), perl));
}

/* _________________________________________________________________________

   SessionImpl::RunPerl
   _________________________________________________________________________ */

int
SessionImpl::RunPerl (/*[in]*/ int		argc,
		      /*[in]*/ const char **	argv)
{
  MIKTEX_ASSERT (argc > 0);

  // find Perl
  PathName perl;
  if (! FindPerl(perl))
    {
      FATAL_MIKTEX_ERROR ("SessionImpl::RunPerl",
			  T_("The Perl interpreter could not be found."),
			  0);
    }

  // determine script name
  char szName[BufferSizes::MaxPath];
  PathName::Split (argv[0],
		   0, 0,
		   szName, BufferSizes::MaxPath,
		   0, 0);

  // get relative script path
  PathName scriptsIni = GetSpecialPath(SpecialPath::DistRoot);
  scriptsIni += MIKTEX_PATH_SCRIPTS_INI;
  SmartPointer<Cfg> pConfig (Cfg::Create());
  pConfig->Read (scriptsIni);
  string relScriptPath;
  if (! pConfig->TryGetValue("perl", szName, relScriptPath))
  {
    FATAL_MIKTEX_ERROR ("",
      MIKTEXTEXT("The Perl script is not registered."),
      szName);
  }
  pConfig.Release ();

  // find Perl script
  PathName scriptPath;
  if (! FindFile(relScriptPath.c_str(), MIKTEX_PATH_TEXMF_PLACEHOLDER, scriptPath))
    {
      FATAL_MIKTEX_ERROR ("SessionImpl::RunPerl",
			  T_("The Perl script could not be found."),
			  relScriptPath.c_str());
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
