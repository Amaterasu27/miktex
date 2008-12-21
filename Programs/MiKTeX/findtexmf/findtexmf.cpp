/* findtexmf.cpp: finding TeXMF related files

   Copyright (C) 2001-2008 Christian Schenk

   This file is part of FindTeXMF.

   FindTeXMF is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   FindTeXMF is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with FindTeXMF; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include <map>
#include <string>
#include <vector>

#include <iomanip>
#include <iostream>
#include <cstdarg>
#include <cstdio>

#include "findtexmf-version.h"

#include <miktex/Core/Core>
#include <miktex/App/Application>
#include <miktex/Core/Paths>
#include <popt-miktex.h>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

const char * const TheNameOfTheGame = T_("MiKTeX Finder");
const char * const PROGNAME = "findtexmf";

/* _________________________________________________________________________

   FindTeXMF
   _________________________________________________________________________ */

class FindTeXMF : public Application
{
public:
  FindTeXMF ();
  
private:
  void
  ShowVersion ();
  
private:
  MIKTEXNORETURN
  void
  FatalError (/*[in]*/ const char *	lpszFormat,
	      /*[in]*/			...);

private:
  void
  ListFileTypes ();
  
private:
  void
  PrintPath (/*[in]*/ const PathName & path);
  
private:
  void
  PrintSearchPath (/*[in]*/ const char * lpszSearchPath);
  
public:
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv);
  
private:
  bool kpseMode;
  
private:
  bool mustExist;
  
private:
  bool start;
  
private:
  FileType fileType;

private:
  static const struct poptOption aoption[];

private:
  static const struct poptOption aoptionKpse[];
};

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 256,
  OPT_ALIAS,
  OPT_EXPAND_PATH,
  OPT_EXPAND_VAR,
  OPT_FILE_TYPE,
  OPT_LIST_FILE_TYPES,
  OPT_MUST_EXIST,
  OPT_SHOW_PATH,
  OPT_START,
  OPT_THE_NAME_OF_THE_GAME,
  OPT_VERSION,
};

/* _________________________________________________________________________

   FindTeXMF::aoption
   _________________________________________________________________________ */

const struct poptOption FindTeXMF::aoption[] =
{
  {
    T_("alias"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_ALIAS,
    T_("\
Pretend to be APP, i.e., use APP's configuration settings\
 when searching for files."),
    T_("APP"),
  },

  {
    T_("file-type"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_FILE_TYPE,
    T_("\
The type of the file to search for."),
    T_("FILETYPE"),
  },

  {
    T_("list-file-types"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, 0,
    OPT_LIST_FILE_TYPES,
    T_("\
List known file types."),
    0,
  },

  {
    T_("must-exist"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, 0,
    OPT_MUST_EXIST,
    T_("\
Run the package installer, if necessary."),
    0,
  },

  {
    T_("show-path"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_SHOW_PATH,
    T_("\
Show the search path for a certain file type."),
    T_("FILETYPE"),
  },

  {
    T_("start"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, 0,
    OPT_START,
    T_("\
Start the program which is associated with the file name extension."),
    0,
  },

  {
    T_("the-name-of-the-game"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_THE_NAME_OF_THE_GAME,
    T_("\
Set the name of the program. Relevant when searching for format files."),
    T_("NAME"),
  },

  {
    T_("version"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, 0,
    OPT_VERSION,
    T_("\
Print version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   FindTeXMF::aoptionKpse
   _________________________________________________________________________ */

enum KpseOption
{
  OPT_AAAA = 1024,
};

const struct poptOption FindTeXMF::aoptionKpse[] =
{
  {
    T_("alias"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_ALIAS,
    T_("\
Pretend to be APP, i.e., use APP's configuration settings\
 when searching for files."),
    T_("APP"),
  },

  {
    T_("expand-path"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_EXPAND_PATH,
    T_("\
Deprecated."),
    T_("PATH"),
  },

  {
    T_("expand-var"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_EXPAND_VAR,
    T_("\
Deprecated."),
    T_("VAR"),
  },

  {
    T_("engine"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_THE_NAME_OF_THE_GAME,
    T_("\
Deprecated (Use -the-name-of-the-game instead)."), T_("ENGINE"),
  },

  {
    T_("file-type"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_FILE_TYPE,
    T_("\
The type of the file to search for."),
    T_("FILETYPE"),
  },

  {
    T_("format"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_FILE_TYPE,
    T_("\
Deprecated (use -file-type instead)."),
    T_("FORMAT"),
  },

  {
    T_("list-file-types"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, 0,
    OPT_LIST_FILE_TYPES,
    T_("\
List known file types."),
    0,
  },

  {
    T_("must-exist"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, 0,
    OPT_MUST_EXIST,
    T_("\
Run the package installer, if necessary."),
    0,
  },

  {
    T_("progname"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_ALIAS,
    T_("\
Deprecated (use -alias instead)."),
    T_("PROGNAME"),
  },

  {
    T_("show-path"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_SHOW_PATH,
    T_("\
Show the search path for a certain file type."),
    T_("FILETYPE"),
  },

  {
    T_("start"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, 0,
    OPT_START,
    T_("\
Start the program which is associated with the file name extension."),
    0,
  },

  {
    T_("the-name-of-the-game"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_ONEDASH, 0,
    OPT_THE_NAME_OF_THE_GAME,
    T_("\
Set the name of the program. Relevant when searching for format files."),
    T_("NAME"),
  },

  {
    T_("version"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, 0,
    OPT_VERSION,
    T_("\
Print version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   FindTeXMF::FindTeXMF
   _________________________________________________________________________ */

FindTeXMF::FindTeXMF ()
  : kpseMode (false),
    mustExist (false),
    start (false),
    fileType (FileType::None)
{
}

/* _________________________________________________________________________

   FindTeXMF::ShowVersion
   _________________________________________________________________________ */

void
FindTeXMF::ShowVersion ()
{
  cout << Utils::MakeProgramVersionString(TheNameOfTheGame,
					  MIKTEX_COMPONENT_VERSION_STR)
	<< "\n"
	<< T_("Copyright (C) 2001-2008 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	<< endl;
  if (kpseMode)
    {
      cout << T_("warning: running in deprecated kpathsea emulation mode")
	    << endl;
    }
}

/* _________________________________________________________________________
     
   FindTeXMF::FatalError
   _________________________________________________________________________ */
  
MIKTEXNORETURN
void
FindTeXMF::FatalError (/*[in]*/ const char *	lpszFormat,
			 /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << PROGNAME << ": "
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   FindTeXMF::ListFileTypes
   _________________________________________________________________________ */

void
FindTeXMF::ListFileTypes ()
{
  FileTypeInfo fti;
  for (unsigned i = 0; pSession->GetNextFileTypeInfo(i, fti); ++ i)
    {
      if (fti.fileNameExtensions.length() == 0)
	{
	  continue;
	}
      cout << "  " << fti.fileTypeString
	   << " (" << fti.fileNameExtensions << ")"
	   << endl;
    }
}

/* _________________________________________________________________________

   FindTeXMF::PrintPath
   _________________________________________________________________________ */

void
FindTeXMF::PrintPath (/*[in]*/ const PathName & path)
{
  if (kpseMode)
    {
      puts (PathName(path).ToUnix().Get());
    }
  else
    {
      puts (path.Get());
    }
}

/* _________________________________________________________________________

   FindTeXMF::PrintSearchPath
   _________________________________________________________________________ */

void
FindTeXMF::PrintSearchPath (/*[in]*/ const char * lpszSearchPath)
{
  bool first = true;
  for (CSVList p (lpszSearchPath, ';');
       p.GetCurrent() != 0;
       ++ p)
    {
      const char * lpszPath = p.GetCurrent();
      if ((PathName::Compare(lpszPath,
			     MPM_ROOT_PATH,
			     static_cast<DWORD>(MPM_ROOT_PATH_LEN))
	   == 0)
	  && (lpszPath[MPM_ROOT_PATH_LEN] == 0
	      || IsDirectoryDelimiter(lpszPath[MPM_ROOT_PATH_LEN])))
	{
	  continue;
	}
      if (first)
	{
	  first = false;
	}
      else
	{
	  putchar (';');
	}
      if (kpseMode)
	{
	  printf ("%s", PathName(lpszPath).ToUnix().Get());
	}
      else
	{
	  printf ("%s", lpszPath);
	}
    }
  putchar ('\n');
}

/* _________________________________________________________________________

   FindTeXMF::Run
   _________________________________________________________________________ */

void
FindTeXMF::Run (/*[in]*/ int			argc,
		/*[in]*/ const char **		argv)
{
  kpseMode = (PathName::Compare(Utils::GetExeName(), T_("kpsewhich")) == 0);

  bool needArg = true;
  
  Cpopt popt (argc, argv, (kpseMode ? aoptionKpse : aoption));

  int option;
  while ((option = popt.GetNextOpt()) >= 0)
    {
      const char * lpszOptArg = popt.GetOptArg();
      switch (option)
	{

	case OPT_ALIAS:
	    
	  pSession->PushAppName (lpszOptArg);
	  break;
	  
	case OPT_EXPAND_VAR:

	  if (StringCompare(lpszOptArg, T_("$TEXFORMATS")) == 0)
	    {
	      PrintPath
		(PathName(pSession->GetSpecialPath(SpecialPath::DataRoot),
			  MIKTEX_PATH_FMT_DIR,
			  0));
	    }
	  else if (StringCompare(lpszOptArg, T_("$TEXMFMAIN")) == 0)
	    {
	      PrintPath (pSession->GetSpecialPath(SpecialPath::InstallRoot));
	    }
	  else if (StringCompare(lpszOptArg, T_("$VARTEXMF")) == 0)
	    {
	      PrintPath (pSession->GetSpecialPath(SpecialPath::UserDataRoot));
	    }
	  else
	    {
#if 0
	      FatalError (T_("Unsupported kpathsea feature: %s."),
			  lpszOptArg);
#endif
	    }
	  needArg = false;
	  break;

	case OPT_EXPAND_PATH:

	  if (StringCompare(lpszOptArg, T_("$TEXMFLOCAL")) == 0
	      || StringCompare(lpszOptArg, T_("$TEXMFFONTS")) == 0)
	    {
	      PrintPath (pSession->GetSpecialPath(SpecialPath::DataRoot));
	    }
	  else if (StringCompare(lpszOptArg, T_("$TEXMFMAIN")) == 0)
	    {
	      PrintPath (pSession->GetSpecialPath(SpecialPath::InstallRoot));
	    }
	  else
	    {
#if 0
	      FatalError (T_("Unsupported kpathsea feature: %s."),
			  lpszOptArg);
#endif
	    }
	  needArg = false;
	  break;

	case OPT_FILE_TYPE:
	  
	  fileType = pSession->DeriveFileType(lpszOptArg);
	  if (fileType == FileType::None)
	    {
	      FatalError (T_("Unknown file type: %s."), lpszOptArg);
	    }
	  break;

	case OPT_LIST_FILE_TYPES:

	  ListFileTypes ();
	  needArg = false;
	  break;
	    
	case OPT_MUST_EXIST:

	  mustExist = true;
	  break;

	case OPT_SHOW_PATH:

	  {
	    FileType filetype = pSession->DeriveFileType(lpszOptArg);
	    if (filetype == FileType::None)
	      {
		FatalError (T_("Unknown file type: %s."), lpszOptArg);
	      }
	    string searchPath = pSession->GetExpandedSearchPath(filetype);
	    if (searchPath.length() != 0)
	      {
		PrintSearchPath (searchPath.c_str());
	      }
	    needArg = false;
	    break;
	  }

	case OPT_START:

	  start = true;
	  break;

	case OPT_THE_NAME_OF_THE_GAME:
	  
	  pSession->SetTheNameOfTheGame (lpszOptArg);
	  break;

	case OPT_VERSION:
	  
	  ShowVersion ();
	  throw (0);
	}
    }

  if (option != -1)
    {
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += ": ";
      msg += popt.Strerror(option);
      FatalError ("%s", msg.c_str());
    }
      
  EnableInstaller (mustExist ? TriState::True : TriState::False);

  const char ** leftovers = popt.GetArgs();

  if (leftovers == 0)
    {
      if (! needArg)
	{
	  return;
	}
      else if (kpseMode)
	{
	  cout << T_("warning: running in deprecated kpathsea emulation mode")
	       << endl;
	  throw (1);
	}
      else
	{
	  FatalError (T_("Missing argument. Try 'findtexmf --help'."));
	}
    }

  for (; *leftovers != 0; ++ leftovers)
    {
      PathName path;
      FileType filetype = fileType;
      if (filetype == FileType::None)
	{
	  filetype = pSession->DeriveFileType(*leftovers);
	  if (filetype == FileType::None)
	    {
	      filetype = FileType::TEX;
	    }
	}
      bool found = pSession->FindFile(*leftovers, filetype, path);
      if (found)
	{
	  PrintPath (path);
	  if (start)
	    {
	      PathName pathDir (path);
	      pathDir.RemoveFileSpec ();
	      if (ShellExecute(0,
			       T_("open"),
			       path.Get(),
			       0,
			       pathDir.Get(),
			       SW_SHOW)
		  <= reinterpret_cast<HINSTANCE>(32))
		{
		  FatalError (T_("%s could not be started."), Q_(*leftovers));
		}
	    }
	}
    }
}

/* _________________________________________________________________________

   findtexmf
   _________________________________________________________________________ */

extern "C"
__declspec(dllexport)
int
__cdecl
findtexmf (/*[in]*/ int			argc,
	   /*[in]*/ const char **	argv)
{
  try
    {
      FindTeXMF app;
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
