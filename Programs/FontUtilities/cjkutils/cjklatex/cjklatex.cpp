/* cjklatex.cpp:

   Copyright (C) 2004-2006 Christian Schenk

   This file is part of CJKLaTeX.

   CJKLaTeX is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   CJKLaTeX is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with CJKLaTeX; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

/* Based on cjklatex.c by F. Popineau. */

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <cstdarg>
#include <cstdio>

#include <getopt.h>

#include <miktex/core.h>
#include <miktex/app.h>
#include <miktex/paths.h>

#include "cjklatex-version.h"

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

#define Q_(x) Quoted(x).c_str()

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

tstring
Quoted (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  bool needQuotes = (_tcschr(lpsz, T_(' ')) != 0);
  tstring result;
  if (needQuotes)
    {
      result += T_('"');
    }
  result += lpsz;
  if (needQuotes)
    {
      result += T_('"');
    }
  return (result);
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

tstring
Quoted (/*[in]*/ const tstring & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

tstring
Quoted (/*[in]*/ const PathName & path)
{
  return (Quoted(path.Get()));
}

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT__FIRST = 1000,
  OPT_CONV,
  OPT_HELP,
  OPT_LATEX,
  OPT_NOCLEANUP,
  OPT_PRINT_ONLY,
  OPT_VERBOSE,
  OPT_VERSION,
};

/* _________________________________________________________________________

   aoption
   _________________________________________________________________________ */

const struct option aoption[] =
{
  T_("conv"),		required_argument,	0,	OPT_CONV,
  T_("latex"),		required_argument,	0,	OPT_LATEX,
  T_("help"),		no_argument,		0,	OPT_HELP,
  T_("nocleanup"),	no_argument,		0,	OPT_NOCLEANUP,
  T_("print-only"),	no_argument,		0,	OPT_PRINT_ONLY,
  T_("verbose"),	no_argument,		0,	OPT_VERBOSE,
  T_("version"),	no_argument,		0,	OPT_VERSION,
  0,			no_argument,		0,	0,
};

/* _________________________________________________________________________

   Converters
   _________________________________________________________________________ */

struct CONVERTER {
  const MIKTEXCHAR *	lpszName;
  const MIKTEXCHAR *	lpszProgram;
};

const CONVERTER Converters [] =
{
  T_("bg5"),	T_("bg5conv"),
  T_("bg5+"),	T_("extconv"),
  T_("bg5p"),	T_("extconv"),
  T_("cef"),	T_("cefconv"),
  T_("cef5"),	T_("cef5conv"),
  T_("cefs"),	T_("cefsconv"),
  T_("cjk"),	0,
  T_("gbk"),	T_("extconv"),
  T_("sjis"),	T_("sjisconv"),
};

/* _________________________________________________________________________

   CJKLaTeXApp
   _________________________________________________________________________ */

class CJKLaTeXApp : public Application
{
public:
  CJKLaTeXApp ();
  
private:
  void
  Error (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	 /*[in]*/			...);

private:
  void
  PrintOnly (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	     /*[in]*/				...);

private:
  const MIKTEXCHAR *
  GetConverterProgram (/*[in]*/ const MIKTEXCHAR *	lpszName);

private:
  void
  ParseInvocationName (/*[out]*/ tstring &	converterProgram,
		       /*[out]*/ tstring &	engine);
    
private:
  void
  RunConverter (/*[in]*/ const PathName &	inputFile,
		/*[in]*/ const PathName &	intermediateFile);

private:
  void
  RunEngine (/*[in]*/ const PathName &	inputFile);

private:
  void
  ShowVersion ();
  
private:
  void
  Help ();
  
private:
  void
  ProcessOptions (/*[in]*/ int			argc,
		  /*[in]*/ MIKTEXCHAR **	argv);

public:
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ MIKTEXCHAR **	argv);

private:
  tstring engine;

private:
  tstring converterProgram;

private:
  bool cleanUp;

private:
  bool printOnly;
};

/* _________________________________________________________________________

   CJKLaTeXApp::CJKLaTeXApp
   _________________________________________________________________________ */

CJKLaTeXApp::CJKLaTeXApp ()
  : cleanUp (true),
    printOnly (false)
{
}

/* _________________________________________________________________________

   CJKLaTeXApp::ShowVersion
   _________________________________________________________________________ */

void
CJKLaTeXApp::ShowVersion ()
{
  tcout << Utils::MakeProgramVersionString(Utils::GetExeName().c_str(),
					   VER_FILEVERSION_STR)
	<< T_("\n\
Copyright (C) 2001 Fabrice Popineau\n\
Copyright (C) 2004-2006 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	<< endl;
}

/* _________________________________________________________________________

   CJKLaTeXApp::Help
   _________________________________________________________________________ */

void
CJKLaTeXApp::Help ()
{
  tcout << T_("Usage: ") << Utils::GetExeName() << T_(" [OPTION...] FILE\n\
\n\
This program runs a converter on FILE and then LaTeX on the result.\n\
\n\
Options:\n\
--conv=FILTER             Set the conversion filter. One of:\n\
                            bg5+\n\
                            bg5\n\
                            bg5p\n\
                            cef5\n\
                            cef\n\
                            cefs\n\
                            gbk\n\
                            sjis\n\
--nocleanup               Keep intermediate files.\n\
--latex=PROGRAM           Use PROGRAM instead of latex.\n\
--help                    Print this help screen and exit.\n\
--verbose                 Print info about what is being done.\n\
--version                 Print the version number and exit.")
	<< endl;
}

/* _________________________________________________________________________
     
   CJKLaTeXApp::Error
   _________________________________________________________________________ */

void
CJKLaTeXApp::Error (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << Utils::GetExeName() << T_(": ")
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________
     
   CJKLaTeXApp::PrintOnly
   _________________________________________________________________________ */

void
CJKLaTeXApp::PrintOnly (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
			/*[in]*/			...)
{
  if (! printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcout << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   CJKLaTeXApp::RunConverter
   _________________________________________________________________________ */

void
CJKLaTeXApp::RunConverter (/*[in]*/ const PathName &	inputFile,
			   /*[in]*/ const PathName &	intermediateFile)
{
  PathName converter;
  if (! pSession->FindFile(converterProgram.c_str(),
			   FileType::EXE,
			   converter))
    {
      Error (T_("Converter %s not found."), converterProgram.c_str());
    }
  Utils::RemoveBlanksFromPathName (converter);
  CommandLineBuilder cmdLine;
  cmdLine.AppendArgument (converter);
  cmdLine.AppendStdinRedirection (inputFile);
  cmdLine.AppendStdoutRedirection (intermediateFile);
  PrintOnly (T_("%s"), cmdLine.Get());
  if (! printOnly && ! Process::ExecuteSystemCommand(cmdLine.Get()))
    {
      Error (T_("Converter %s failed on %s."),
	     converterProgram.c_str(),
	     Q_(inputFile));
    }
}

/* _________________________________________________________________________

   CJKLaTeXApp::RunEngine
   _________________________________________________________________________ */

void
CJKLaTeXApp::RunEngine (/*[in]*/ const PathName &	inputFile)
{
  PathName engineExe;
  if (! pSession->FindFile(engine.c_str(), FileType::EXE, engineExe))
    {
      Error (T_("Engine %s not found."), engine.c_str());
    }
  CommandLineBuilder arguments;
  arguments.AppendArgument (inputFile);
  PrintOnly (T_("%s %s"), Q_(engineExe), arguments.Get());
  if (! printOnly)
    {
      Process::Run (engineExe, arguments.Get());
    }
}

/* _________________________________________________________________________

   CJKLaTeXApp::GetConverterProgram
   _________________________________________________________________________ */

const MIKTEXCHAR *
CJKLaTeXApp::GetConverterProgram (/*[in]*/ const MIKTEXCHAR *	lpszName)
{
  for (size_t i = 0; i < sizeof(Converters) / sizeof(Converters[0]); ++ i)
    {
      if (PathName::Compare(lpszName, Converters[i].lpszName) == 0)
	{
	  return (Converters[i].lpszProgram);
	}
    }
  return (0);
}

/* _________________________________________________________________________

   CJKLaTeXApp::ParseInvocationName
   _________________________________________________________________________ */

void
CJKLaTeXApp::ParseInvocationName
(/*[out]*/ tstring &		converterProgram,
 /*[out]*/ tstring &		engine)
{
  tstring invocationName = Utils::GetExeName();
  const MIKTEXCHAR * lpszInvocationName = invocationName.c_str();
  size_t j = UINT_MAX;
  size_t len = 0;
  for (size_t i = 0; i < sizeof(Converters) / sizeof(Converters[0]); ++ i)
    {
      size_t l = _tcslen(Converters[i].lpszName);
      if (_tcsnicmp(lpszInvocationName, Converters[i].lpszName, l) == 0
	  && l > len)
	{
	  j = i;
	  len = l;
	}
    }
  if (j == UINT_MAX)
    {
      Error (T_("Bad invocation."));
    }
  if (Converters[j].lpszProgram != 0)
    {
      converterProgram = Converters[j].lpszProgram;
    }
  engine = &lpszInvocationName[len];
}

/* _________________________________________________________________________

   CJKLaTeXApp::ProcessOptions
   _________________________________________________________________________ */

void
CJKLaTeXApp::ProcessOptions (/*[in]*/ int		argc,
			     /*[in]*/ MIKTEXCHAR **	argv)
{
  ParseInvocationName (converterProgram, engine);
  
  optind = 0;
  int optionChar;
  int optionIndex;
  
  while ((optionChar = getopt_long_only(argc,
				      argv,
				      T_(""),
				      aoption,
				      &optionIndex))
	 != EOF)
    {
      switch (static_cast<Option>(optionChar))
	{

	case OPT_CONV:

	  {
	    const MIKTEXCHAR * lpszConverterProgram =
	      GetConverterProgram(optarg);
	    if (lpszConverterProgram == 0)
	      {
		Error (T_("The converter %s is unknown."), optarg);
	      }
	    converterProgram = lpszConverterProgram;
	    break;
	  }
	    
	case OPT_HELP:
	  
	  Help ();
	  throw (0);

	case OPT_LATEX:

	  engine = optarg;
	  break;

	case OPT_PRINT_ONLY:
	  
	  printOnly = true;
	  break;

	case OPT_NOCLEANUP:

	  cleanUp = false;
	  break;

	case OPT_VERBOSE:
	  
	  break;
	  
	case OPT_VERSION:
	  
	  ShowVersion ();
	  throw (0);
	  
	default:

	  Error (T_("Invalid command-line."));
	  break;
	}
    }

  EnableInstaller (TriState::True);
}

/* _________________________________________________________________________

   CJKLaTeXApp::Run
   _________________________________________________________________________ */

void
CJKLaTeXApp::Run (/*[in]*/ int			argc,
		  /*[in]*/ MIKTEXCHAR **	argv)
{
  ProcessOptions (argc, argv);
  for (int i = optind; i != argc; ++ i)
    {
      PathName pathInputFile (argv[i]);
      pathInputFile.MakeAbsolute ();
      PathName pathIntermediateFile (pathInputFile);
      pathIntermediateFile.SetExtension (T_(".cjk"));
      RunConverter (pathInputFile, pathIntermediateFile);
      RunEngine (pathIntermediateFile);
      if (cleanUp && ! printOnly)
	{
	  File::Delete (pathIntermediateFile);
	}
    }
}

/* _________________________________________________________________________

   cjklatexmain
   _________________________________________________________________________ */

extern "C"
int
__cdecl
cjklatexmain (/*[in]*/ int		argc,
	      /*[in]*/ MIKTEXCHAR **	argv)
{
  try
    {
      CJKLaTeXApp app;
      app.Init (argv[0]);
      app.Run(argc, argv);
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
