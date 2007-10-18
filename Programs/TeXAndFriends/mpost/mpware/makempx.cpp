/* makempx.cpp: making mpx files

   Copyright (C) 1998-2007 Christian Schenk

   This file is part of MakeMpx.

   MakeMpx is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MakeMpx is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MakeMpx; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* Based on the makempx shell script. */

#include <iomanip>
#include <iostream>

#include <cstdio>
#include <cassert>
#include <cstdlib>

#include <sys/stat.h>

#include <popt-miktex.h>

#include "makempx.rc"

#include <MiKTeX/App/Application>
#include <MiKTeX/Core/Core>
#include <MiKTeX/TeXAndFriends/TeXMFApp>
#include <MiKTeX/Core/Registry>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX::TeXAndFriends;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

/* _________________________________________________________________________

   MakeMpx
   _________________________________________________________________________ */

class MakeMpx : public Application
{
public:
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv);

private:
  void
  PrintOnly (/*[in]*/ const char *	lpszFormat,
	     /*[in]*/			...);

private:
  void
  Verbose (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  void
  Message (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  Error (/*[in]*/ const char *	lpszFormat,
	 /*[in]*/		...);

private:
  void
  GetOptions (/*[in]*/ int			argc,
	      /*[in]*/ const char **	argv);

private:
  void
  InvokeMPTO (/*[in]*/ const char *	lpszMpFileName,
	      /*[in]*/ const char *	lpszTeXFileName);

private:
  void
  InvokeTeX (/*[in]*/ const char * lpszTeXFileName);

private:
  void
  InvokeDVItoMP (/*[in]*/ const char * lpszDviFileName,
		 /*[in]*/ const char * lpszMpxFileName,
		 /*[in]*/ const char * lpszDestDir);

private:
  PathName
  MakeTeXFileName (/*[in]*/ const char *	lpszBaseName);

private:
  int
  IsNewerFile (/*[in]*/ const char * lpszFileName1,
	       /*[in]*/ const char * lpszFileName2);

private:
  bool verbose;

private:
  bool debug;

private:
  bool quiet;

private:
  bool printOnly;

private:
  PathName mpFile;

private:
  PathName mpxFile;

private:
  string texProgram;

private:
  static const struct poptOption aoption[];
};

/* _________________________________________________________________________

   MakeMpx::Message
   _________________________________________________________________________ */

void
MakeMpx::Message (/*[in]*/ const char *	lpszFormat,
		  /*[in]*/			...)
{
  if (quiet || printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________

   MakeMpx::PrintOnly
   _________________________________________________________________________ */

void
MakeMpx::PrintOnly (/*[in]*/ const char *	lpszFormat,
		    /*[in]*/			...)
{
  if (! printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   MakeMpx::Verbose
   _________________________________________________________________________ */

void
MakeMpx::Verbose (/*[in]*/ const char *	lpszFormat,
		  /*[in]*/				...)
{
  if (! verbose || printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________

   MakeMpx::Error
   _________________________________________________________________________ */

MIKTEXNORETURN
void
MakeMpx::Error (/*[in]*/ const char *		lpszFormat,
		/*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << T_("makempx") << T_(": ")
       << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   MakeMpx::GetOptions
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1000,
  OPT_TEX_PROGRAM,
  OPT_DEBUG,
  OPT_PRINT_ONLY,
  OPT_QUIET,
  OPT_TEX,
  OPT_VERBOSE,
  OPT_VERSION
};

/* _________________________________________________________________________

   MakeMpx::aoption
   _________________________________________________________________________ */

const struct poptOption MakeMpx::aoption[] = {

  {
    T_("debug"), T_('d'), POPT_ARG_NONE, 0, OPT_DEBUG,
    T_("Print debugging information."), 0,
  },

  {
    T_("print-only"), T_('n'), POPT_ARG_NONE, 0, OPT_PRINT_ONLY,
    T_("Print what commands would be executed."), 0,
  },

  {
    T_("quiet"), T_('q'), POPT_ARG_NONE, 0, OPT_QUIET,
    T_("Suppress all output (except errors)."), 0,
  },

  {
    T_("tex"), 0, POPT_ARG_STRING, 0, OPT_TEX_PROGRAM,
    T_("Run PROG instead of tex."), T_("PROG"),
  },

  {
    T_("verbose"), 0, POPT_ARG_NONE, 0, OPT_VERBOSE,
    T_("Turn on verbose output mode."), 0
  },

  {
    T_("version"), 0, POPT_ARG_NONE, 0, OPT_VERSION,
    T_("Show version information and exit."), 0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

void
MakeMpx::GetOptions (/*[in]*/ int			argc,
			 /*[in]*/ const char **	argv)
{
  debug = false;
  printOnly = false;
  quiet = false;
  verbose = false;

  Cpopt popt (argc, argv, aoption);

  int option;
  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{
	case OPT_PRINT_ONLY:
	  printOnly = true;
	  break;
	case OPT_TEX_PROGRAM:
	  texProgram = popt.GetOptArg();
	  break;
	case OPT_DEBUG:
	  debug = true;
	  break;
	case OPT_VERBOSE:
	  verbose = true;
	  break;
	case OPT_QUIET:
	  quiet = true;
	  break;
	case OPT_VERSION:
	  cout <<
	    Utils::MakeProgramVersionString(T_("makempx"),
					    VersionNumber(VER_FILEVERSION))
	       << T_("\n\
Copyright (C) 2005-2007 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	    << endl;
	  return;
	}
    }

  if (option != -1)
    {
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += T_(": ");
      msg += popt.Strerror(option);
      Error (T_("%s"), msg.c_str());
    }

  const char * * leftovers = popt.GetArgs();
      
  PathName path;

  if (leftovers == 0 || *leftovers == 0)
    {
      Error (T_("Missing MP file argument."));
    }

  if (! pSession->FindFile(*leftovers, FileType::MP, path))
    {
      Error ("File %s could not be found.", Q_(*leftovers));
    }

  mpFile = path;
  mpFile.MakeAbsolute ();

  ++ leftovers;

  if (*leftovers == 0)
    {
      Error (T_("Missing MPX file argument."));
    }

  mpxFile = *leftovers;
}

/* _________________________________________________________________________
   
   MakeMpx::InvokeMPTO
   _________________________________________________________________________ */

void
MakeMpx::InvokeMPTO (/*[in]*/ const char *	lpszMpFileName,
		     /*[in]*/ const char *	lpszTeXFileName)
{
  PathName mpto;

  if (! pSession->FindFile(T_("mpto"), FileType::EXE, mpto))
    {
      Error (T_("The MPTO executable could not be found."));
    }

   string mptexpre;

  if (! Utils::GetEnvironmentString(T_("MPTEXPRE"), mptexpre))
    {
      mptexpre = T_("mptexpre.tex");
    }

  if (File::Exists(mptexpre))
    {
      PrintOnly (T_("cp %s %s"), Q_(mptexpre), Q_(lpszTeXFileName));
      if (! printOnly)
	{
	  File::Copy (mptexpre, lpszTeXFileName, false);
	}
    }

  FileStream pTeXFile (File::Open(lpszTeXFileName,
				  FileMode::Append,
				  FileAccess::Write,
				  false));

  CommandLineBuilder commandLineBuilder;

  commandLineBuilder.AppendArgument (lpszMpFileName);

  ProcessStartInfo psi (mpto.Get());

  psi.Arguments = commandLineBuilder.Get();
  psi.StandardOutput = pTeXFile.Get();

  PrintOnly (T_("%s %s"), Q_(mpto), commandLineBuilder.Get());

  if (! printOnly)
    {
      auto_ptr<Process> pProcess (Process::Start(psi));
      pProcess->WaitForExit ();
      if (pProcess->get_ExitCode() != 0)
	{
	  Error (T_("mpto failed on %s."), Q_(lpszMpFileName));
	}
    }
}

/* _________________________________________________________________________

   MakeMpx::InvokeTeX
   _________________________________________________________________________ */

void
MakeMpx::InvokeTeX (/*[in]*/ const char * lpszTeXFileName)
{
  if (texProgram.empty()
      && ! Utils::GetEnvironmentString(T_("TEX"), texProgram))
    {
      Argv argv;
      if (TeXMFApp::ParseFirstLine(lpszTeXFileName, argv))
	{
	  if (argv.GetArgc() > 1 && argv[1][0] != T_('-'))
	    {
	      texProgram = argv[1];
	      texProgram += T_(" -parse-first-line");
	    }
	}
    }

  if (texProgram.empty())
    {
      texProgram = T_("tex -parse-first-line");
    }
  
  CommandLineBuilder commandLineBuilder;

  commandLineBuilder.AppendUnquoted (texProgram.c_str());

  if (debug)
    {
      commandLineBuilder.AppendOption (T_("-interaction="), T_("nonstopmode"));
    }

  commandLineBuilder.AppendArgument (lpszTeXFileName);
  
  PrintOnly (T_("%s"), commandLineBuilder.Get());

  if (! printOnly)
    {
      if (! Process::ExecuteSystemCommand(commandLineBuilder.Get()))
	{
	  Error (T_("%s failed on %s."),
		 Q_(texProgram.c_str()),
		 Q_(lpszTeXFileName));
	}
    }
}

/* _________________________________________________________________________

   MakeMpx::InvokeDVItoMP
   _________________________________________________________________________ */

void
MakeMpx::InvokeDVItoMP (/*[in]*/ const char * lpszDviFileName,
			/*[in]*/ const char * lpszMpxFileName,
			/*[in]*/ const char * lpszDestDir)
{
  PathName dvitomp;

  if (! pSession->FindFile(T_("dvitomp"), FileType::EXE, dvitomp))
    {
      Error (T_("The dvitomp executable could not be found."));
    }

  CommandLineBuilder commandLineBuilder;

  commandLineBuilder.AppendArgument (lpszDviFileName);
  commandLineBuilder.AppendArgument (PathName(lpszDestDir, lpszMpxFileName));

  PrintOnly (T_("%s %s"), Q_(dvitomp), commandLineBuilder.Get());

  if (! printOnly)
    {
      Process::Run (dvitomp, commandLineBuilder.Get());
    }
}

/* _________________________________________________________________________

   MakeMpx::MakeTeXFileName
   _________________________________________________________________________ */

PathName
MakeMpx::MakeTeXFileName (/*[in]*/ const char *	lpszBaseName)
{
  PathName texFile;
  texFile.SetToCurrentDirectory ();
  texFile += lpszBaseName;
  texFile.SetExtension (T_(".tex"));
  fclose (File::Open(texFile, FileMode::Create, FileAccess::Write, false));
  return (texFile);
}

/* _________________________________________________________________________

   MakeMpx::IsNewerFile
   _________________________________________________________________________ */

int
MakeMpx::IsNewerFile (/*[in]*/ const char * lpszFileName1,
		      /*[in]*/ const char * lpszFileName2)
{
  struct stat x, y;
  if (stat(lpszFileName1, &x) < 0)
    {
      return (1);
    }
  else if (stat(lpszFileName2, &y) < 0)
    {
      return (0);
    }
  else if (x.st_mtime < y.st_mtime)
    {
      return (1);
    }
  else
    {
      return (0);
    }
}

/* _________________________________________________________________________

   MakeMpx::Run
   _________________________________________________________________________ */

void
MakeMpx::Run (/*[in]*/ int			argc,
	      /*[in]*/ const char **	argv)

{
  // get command line options
  GetOptions (argc, argv);

  // return if mp file exists and is newer than mpx file
  if (IsNewerFile(mpFile.Get(), mpxFile.Get()))
    {
      Verbose (T_("MPX file %s is up-to-date.\n"), Q_(mpxFile));
      return;
    }
  
  Message (T_("Creating %s...\n"), Q_(mpxFile.Get()));

  PathName destDir;
  destDir.SetToCurrentDirectory ();

  // enter a scratch directory
  ScratchDirectory scratchDirectory;
  scratchDirectory.Enter ();

  const char * lpszName = T_("mpx314");

  PathName texFile = MakeTeXFileName(lpszName);

  // invoke mpto to make a TeX file
  InvokeMPTO (mpFile.Get(), texFile.Get());

  // invoke TeX
  InvokeTeX (texFile.Get());

  // invoke dvitomp
  InvokeDVItoMP (PathName(0, lpszName, T_(".dvi")).Get(),
		 mpxFile.Get(),
		 destDir.Get());

  Message (T_("Done.\n"));

  // remove scratch directory
  scratchDirectory.Leave ();
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)

{
  try
    {
      MakeMpx app;
      app.Init (argv[0]);
      app.Run (argc, argv);
      app.Finalize ();
      return (0);
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (int exitCode)
    {
      return (exitCode);
    }
}
