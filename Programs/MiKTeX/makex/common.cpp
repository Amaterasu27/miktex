/* common.cpp: common stuff

   Copyright (C) 1998-2009 Christian Schenk

   This file is part of the MiKTeX Maker Library.

   The MiKTeX Maker Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Maker Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Maker Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "internal.h"

/* _________________________________________________________________________

   ProcessOutputTrash
   _________________________________________________________________________ */

class
ProcessOutputTrash
  : public IRunProcessCallback
{
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n)
  {
    UNUSED_ALWAYS (pOutput);
    UNUSED_ALWAYS (n);
    return (true);
  }
};

/* _________________________________________________________________________

   ProcessOutputStderr
   _________________________________________________________________________ */

class
ProcessOutputStderr
  : public IRunProcessCallback
{
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n)
  {
    fwrite (pOutput, 1, n, stderr);
    return (true);
  }
};

/* _________________________________________________________________________

   MakeUtility::RunMETAFONT
   _________________________________________________________________________ */

bool
MakeUtility::RunMETAFONT (/*[in]*/ const char *	lpszName,
			  /*[in]*/ const char *	lpszMode,
			  /*[in]*/ const char *	lpszMag)
{
  string arguments;
  arguments += "--undump=mf";
  arguments += " \\mode:=";
  arguments += (lpszMode == 0 ? "cx" : lpszMode);
  arguments += ';';
  if (lpszMag != 0)
    {
      arguments += T_(" \\mag:=");
      arguments += lpszMag;
      arguments += ';';
    }
  if (! debug)
    {
      arguments += T_("nonstopmode;");
    }
  arguments += T_("input ");
  arguments += lpszName;
  int exitCode = RunProcess(MIKTEX_MF_EXE, arguments.c_str());
  if (exitCode != 0)
    {
      Verbose (T_("METAFONT failed for some reason\n"));
      PathName pathLogFile (0, lpszName, ".log");
      AutoFILE pLogFile
	(File::Open(pathLogFile, FileMode::Open, FileAccess::Read));
      string line;
      bool noError = true;
      size_t nStrangePaths = 0;
      while (noError && Utils::ReadUntilDelim(line, '\n', pLogFile.Get()))
	{
	  if (line[0] != '!')
	    {
	      continue;
	    }
	  if (strncmp(line.c_str(), T_("! Strange path"), 14) == 0)
	    {
	      ++ nStrangePaths;
	      continue;
	    }
	  noError = false;
	}
      if (noError)
	{
	  Verbose (T_("ignoring %u strange path(s)\n"), nStrangePaths);
	}
      pLogFile.Reset ();
      return (noError);
    }
  return (exitCode == 0);
}

/* _________________________________________________________________________

   MakeUtility::RunProcess
   _________________________________________________________________________ */

bool
MakeUtility::RunProcess (/*[in]*/ const char *	lpszExeName,
			 /*[in]*/ const char *	lpszArguments)
{
  // find the executable; make sure it contains no blanks
  PathName exe;
  if (! pSession->FindFile(lpszExeName, FileType::EXE, exe))
    {
      FatalError (T_("The application file %s could not be found."), Q_(exe));
    }

  Message (T_("Running %s...\n"), Q_(lpszExeName));
  PrintOnly ("%s %s", Q_(lpszExeName), lpszArguments);

  // run the program
  int exitCode = 0;
  if (! printOnly || strstr(lpszArguments, "--print-only") != 0)
    {
      ProcessOutputTrash trash;
      ProcessOutputStderr toStderr;
      IRunProcessCallback * pCallback = 0;
      if (quiet)
	{
	  pCallback = &trash;
	}
      else if (stdoutStderr)
	{
	   pCallback = &toStderr;
	}
      if (! Process::Run(exe,
			 lpszArguments,
			 pCallback,
			 &exitCode,
			 0))
	{
	  FatalError (T_("The application file %s could not be started."),
		      Q_(lpszExeName));
	}
    }

  return (exitCode == 0);
}

/* _________________________________________________________________________

   MakeUtility::FatalError
   _________________________________________________________________________ */

MIKTEXNORETURN
void
MakeUtility::FatalError (/*[in]*/ const char *	lpszFormat,
			 /*[in]*/		...)
  const
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << Utils::GetExeName() << ": "
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   MakeUtility::ShowVersion
   _________________________________________________________________________ */

void
MakeUtility::ShowVersion ()
{
  OUT__ << Utils::MakeProgramVersionString(Utils::GetExeName().c_str(),
					   VersionNumber(MIKTEX_MAJOR_VERSION,
							 MIKTEX_MINOR_VERSION,
							 MIKTEX_COMP_J2000_VERSION,
							 0))
	<< T_("\n\
Copyright (C) 1998-2009 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	<< endl;
}

/* _________________________________________________________________________

   MakeUtility::Verbose
   _________________________________________________________________________ */

void
MakeUtility::Verbose (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
{
  if (! verbose || printOnly || quiet)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  OUT__ << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________

   MakeUtility::Message
   _________________________________________________________________________ */

void
MakeUtility::Message (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
{
  if (quiet || printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  OUT__ << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________

   MakeUtility::PrintOnly
   _________________________________________________________________________ */

void
MakeUtility::PrintOnly (/*[in]*/ const char *	lpszFormat,
			/*[in]*/		...)
{
  if (! printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  OUT__ << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   MakeUtility::Install
   _________________________________________________________________________ */

void
MakeUtility::Install (/*[in]*/ const PathName &	source,
		      /*[in]*/ const PathName &	dest)
{
  PrintOnly ("cp %s %s", Q_(source), Q_(dest));
  PrintOnly (T_("initexmf --update-fndb"));
  
  if (printOnly)
    {
      return;
    }

  // copy the file
  File::Copy (source, dest);

  // add file to file name database
  if (! Fndb::FileExists(dest))
    {
      Fndb::Add (dest);
    }
}

/* _________________________________________________________________________

   GetShortOptions
   _________________________________________________________________________ */

void
GetShortOptions (/*[in]*/ const struct option *	pLongOptions,
		 /*[out]*/ string &		shortOptions)
{
  shortOptions = "";
  for (const struct option * opt = pLongOptions; opt->name != 0; ++ opt)
    {
      if (isprint(opt->val))
	{
	  shortOptions += static_cast<char>(opt->val);
	  if (opt->has_arg == required_argument)
	    {
	      shortOptions += ':';
	    }
	  else if (opt->has_arg == optional_argument)
	    {
	      shortOptions += "::";
	    }
	}
    }
}

/* _________________________________________________________________________

   MakeUtility::GetOptions
   _________________________________________________________________________ */

void
MakeUtility::GetOptions (/*[in]*/ int				argc,
			 /*[in]*/ const char **			argv,
			 /*[in]*/ const struct option *		pLongOptions,
			 /*[in]*/ int &				optionIndex)
{
  string shortOptions;
  GetShortOptions (pLongOptions, shortOptions);

  int c;
  int idx;
  optind = 0;
  while ((c = getopt_long(argc,
			  const_cast<char * const *>(argv),
			  shortOptions.c_str(),
			  pLongOptions,
			  &idx))
	 != EOF)
    {
      switch (c)
	{
	case 'h':
	  Usage ();
	  throw (0);
	  break;
	case 'n':
	  printOnly = true;
	  break;
	case 'd':
	  debug = true;
	  break;
	case 'v':
	  verbose = true;
	  break;
	case 'q':
	  quiet = true;
	  break;
	case 'V':
	  ShowVersion ();
	  throw (0);
	  break;
	default:
	  {
	    bool handled = true;
	    HandleOption (c, optarg, handled);
	    if (! handled)
	      {
		FatalError (T_("Unknown command-line option."));
	      }
	    break;
	  }
	}
    }

  optionIndex = optind;
}

/* _________________________________________________________________________

   MakeUtility::CreaDirectory
   _________________________________________________________________________ */

void
MakeUtility::CreateDirectory (/*[in]*/ const string &	templ,
			      /*[out]*/ PathName &	path)
{
  const char * lpszTemplate = templ.c_str();
  if (lpszTemplate[0] == '%'
      && lpszTemplate[1] == 'R'
      && IsDirectoryDelimiter(lpszTemplate[2]))
    {
      path = pSession->GetSpecialPath(SpecialPath::DataRoot);
      path += lpszTemplate + 3;
    }
  else
    {
      path = lpszTemplate;
    }

  if (! Directory::Exists(path))
    {
      PrintOnly (CommandLineBuilder(T_("mkdir"), path.Get()).Get());
      if (! printOnly)
	{
	  Directory::Create (path);
	}
    }
}

