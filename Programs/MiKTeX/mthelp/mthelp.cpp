/* mthelp.cpp:

   Copyright (C) 2004-2013 Christian Schenk

   This file is a part of MTHelp.

   MTHelp is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MTHelp is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MTHelp; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#if defined(HAVE_CONFIG_H)
#  include <config.h>
#endif

#if defined(MIKTEX_WINDOWS)
#  include <Windows.h>
#endif

#include "mthelp-version.h"

#include <miktex/App/Application>
#include <miktex/Core/Core>
#include <miktex/PackageManager/PackageManager>
#include <miktex/Core/Paths>
#include <miktex/Core/Registry>
#include <popt-miktex.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeX;
using namespace std;

#include "template.html.h"

#define T_(x) MIKTEXTEXT(x)
#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

const char * const TheNameOfTheGame = T_("MiKTeX Help Utility");
const char * const PROGNAME = T_("mthelp");

#if defined(MIKTEX_WINDOWS)
const char PATH_DELIMITER = ';';
#define PATH_DELIMITER_STRING ";"
#else
const char PATH_DELIMITER = ':';
#define PATH_DELIMITER_STRING ":"
#endif

#if defined(MIKTEX_WINDOWS)
const char * const DEFAULT_DOC_EXTENSIONS =
  T_(".chm;.html;.dvi;.pdf;.ps;.txt");
#else
const char * const DEFAULT_DOC_EXTENSIONS =
  T_(".html:.pdf:.dvi:.ps:.txt");
#endif

/* _________________________________________________________________________

   MiKTeXHelp
   _________________________________________________________________________ */

class MiKTeXHelp : public Application
{
public:
  MiKTeXHelp ();

public:
  virtual
  void
  MIKTEXTHISCALL
  Init (/*[in]*/ const Session::InitInfo & initInfo);

public:
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv);

private:
  void
  ShowVersion ();
  
private:
  void
  Warning (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  FatalError (/*[in]*/ const char *	lpszFormat,
	      /*[in]*/			...);

private:
  bool
  SkipPrefix (/*[in]*/ const string &	str,
	      /*[in]*/ const char *	lpszPrefix,
	      /*[out]*/ string &	result);

private:
  bool
  SkipTeXMFPrefix (/*[in]*/ const string &	str,
		   /*[out]*/ string &		result);

private:
  void
  FindDocFilesByPackage
  (/*[in]*/ const string &			packageName,
   /*[out]*/ map<string, vector<string> > &	filesByExtension);

private:
  void
  FindDocFilesByPackage
  (/*[in]*/ const string &		packageName,
   /*[out]*/ vector<string> &		files);

private:
  void
  FindDocFilesByName (/*[in]*/ const string &		name,
		      /*[out]*/ vector<string> &	files);

private:
  void
  PrintFiles (/*[in]*/ const vector<string> & files);

private:
  void
  ViewFile (/*[in]*/ const PathName & fileName);

private:
  void
  CreateHtmlAndView (/*[in]*/ const char * lpszPackageName,
		     /*[in]*/ const vector<string> & files);

private:
  void
  WriteText (/*[in]*/ StreamWriter &	writer,
	     /*[in]*/ const string &	text);


private:
  bool printOnly;

private:
  bool quiet;

private:
  PackageManagerPtr pManager;

private:
  static const struct poptOption aoption[];
};

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 256,
  OPT_LIST_ONLY,
  OPT_PRINT_ONLY,
  OPT_QUIET,
  OPT_VERSION,
  OPT_VIEW,
};

/* _________________________________________________________________________

   MiKTeXHelp::aoption
   _________________________________________________________________________ */

const struct poptOption MiKTeXHelp::aoption[] = {
  {
    T_("list-only"), 'l', POPT_ARG_NONE, 0, OPT_LIST_ONLY,
    T_("List documentation files but do not start a viewer."),
    0
  },
  {
    T_("print-only"), 'n', POPT_ARG_NONE, 0, OPT_PRINT_ONLY,
    T_("Print the commands that would be executed, but do not execute them."),
    0
  },
  {
    T_("quiet"), 0, POPT_ARG_NONE, 0, OPT_QUIET,
    T_("Suppress all output (except errors)."),
    0
  },
  {
    T_("version"), 0, POPT_ARG_NONE, 0, OPT_VERSION,
    T_("Show version information and exit."),
    0
  },
  {
    T_("view"), 0, POPT_ARG_NONE, 0, OPT_VIEW,
    T_("View the main documentation file of the package."),
    0
  },
  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   MiKTeXHelp::MiKTeXHelp
   _________________________________________________________________________ */

MiKTeXHelp::MiKTeXHelp ()
  : printOnly (false),
    quiet (false)
{
}

/* _________________________________________________________________________

   MiKTeXHelp::MiKTeXHelp
   _________________________________________________________________________ */

void
MiKTeXHelp::Init (/*[in]*/ const Session::InitInfo & initInfo)
{
  Application::Init (initInfo);
  pManager = PackageManager::Create();
}

/* _________________________________________________________________________

   MiKTeXHelp::ShowVersion
   _________________________________________________________________________ */

void
MiKTeXHelp::ShowVersion ()
{
  tcout << Utils::MakeProgramVersionString(TheNameOfTheGame,
					   MIKTEX_COMPONENT_VERSION_STR)
	<< "\n"
	<< T_("Copyright (C) 2004-2008 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	<< endl;
}

/* _________________________________________________________________________

   MiKTeXHelp::Warning
   _________________________________________________________________________ */

void
MiKTeXHelp::Warning (/*[in]*/ const char *	lpszFormat,
		     /*[in]*/				...)
{
  if (quiet)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
}

/* _________________________________________________________________________
     
   MiKTeXHelp::FatalError
   _________________________________________________________________________ */
  
MIKTEXNORETURN
void
MiKTeXHelp::FatalError (/*[in]*/ const char *	lpszFormat,
			/*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << PROGNAME << ": "
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   MiKTeXHelp::SkipPrefix
   _________________________________________________________________________ */

bool
MiKTeXHelp::SkipPrefix (/*[in]*/ const string &	str,
			/*[in]*/ const char *	lpszPrefix,
			/*[out]*/ string &		result)
{
  size_t n = StrLen(lpszPrefix);
  if (str.compare(0, n, lpszPrefix) != 0)
    {
      return (false);
    }
  result = str.c_str() + n;
  return (true);
}

/* _________________________________________________________________________

   MiKTeXHelp::SkipTeXMFPrefix
   _________________________________________________________________________ */

bool
MiKTeXHelp::SkipTeXMFPrefix (/*[in]*/ const string &	str,
			     /*[out]*/ string &	result)
{
  return (SkipPrefix(str, T_("texmf/"), result)
	  || SkipPrefix(str, T_("texmf\\"), result)
	  || SkipPrefix(str, T_("./texmf/"), result)
	  || SkipPrefix(str, T_(".\\texmf\\"), result));
}

/* _________________________________________________________________________

   MiKTeXHelp::FindDocFilesByPackage
   _________________________________________________________________________ */

void
MiKTeXHelp::FindDocFilesByPackage
(/*[in]*/ const string &			packageName,
 /*[out]*/ map<string, vector<string> > &	filesByExtension)
{
  PackageInfo pi;
  if (! pManager->TryGetPackageInfo(packageName, pi))
    {
      return;
    }
  for (vector<string>::const_iterator it = pi.docFiles.begin();
       it != pi.docFiles.end();
       ++ it)
    {
      char szExt[BufferSizes::MaxPath];
      PathName::Split (it->c_str(),
		       0, 0,
		       0, 0,
		       szExt, BufferSizes::MaxPath);
      string file;
      PathName path;
      if (SkipTeXMFPrefix(*it, file)
	  && pSession->FindFile(file.c_str(),
				MIKTEX_PATH_TEXMF_PLACEHOLDER_NO_MPM,
				path))
	{
	  vector<string> & files = filesByExtension[szExt];
	  files.push_back (path.ToString());
	}
    }
}

/* _________________________________________________________________________

   MiKTeXHelp::FindDocFilesByPackage
   _________________________________________________________________________ */

void
MiKTeXHelp::FindDocFilesByPackage (/*[in]*/ const string &	packageName,
				   /*[out]*/ vector<string> &	files)
{
  map<string, vector<string> > filesByExtension;
  FindDocFilesByPackage (packageName, filesByExtension);
  string extensions =
    pSession->GetConfigValue(0,
			     MIKTEX_REGVAL_DOC_EXTENSIONS,
			     DEFAULT_DOC_EXTENSIONS);
  for (Tokenizer ext (extensions.c_str(), PATH_DELIMITER_STRING);
       ext.GetCurrent() != 0;
       ++ ext)
    {
      vector<string> & vec = filesByExtension[ext.GetCurrent()];
      vector<string>::iterator it = vec.begin();
      while (it != vec.end())
	{
	  char szName[BufferSizes::MaxPath];
	  PathName::Split (it->c_str(),
			   0, 0,
			   szName, BufferSizes::MaxPath,
			   0, 0);
	  if (PathName::Compare(szName, packageName.c_str()) == 0)
	    {
	      files.push_back (*it);
	      it = vec.erase(it);
	    }
	  else
	    {
	      ++ it;
	    }
	}
    }
  for (Tokenizer ext (extensions.c_str(), PATH_DELIMITER_STRING);
       ext.GetCurrent() != 0;
       ++ ext)
    {
      vector<string> & vec = filesByExtension[ext.GetCurrent()];
      files.insert (files.end(), vec.begin(), vec.end());
    }
}

/* _________________________________________________________________________

   MiKTeXHelp::FindDocFilesByName
   _________________________________________________________________________ */

void
MiKTeXHelp::FindDocFilesByName (/*[in]*/ const string &	name,
				/*[out]*/ vector<string> &	files)
{
  string extensions =
    pSession->GetConfigValue(0,
			     MIKTEX_REGVAL_DOC_EXTENSIONS,
			     DEFAULT_DOC_EXTENSIONS);
  string searchSpec = MIKTEX_PATH_TEXMF_PLACEHOLDER_NO_MPM;
  searchSpec += MIKTEX_PATH_DIRECTORY_DELIMITER_STRING;
  searchSpec += MIKTEX_PATH_DOC_DIR;
  searchSpec += MIKTEX_PATH_RECURSION_INDICATOR;
  for (Tokenizer ext (extensions.c_str(), PATH_DELIMITER_STRING);
       ext.GetCurrent() != 0;
       ++ ext)
    {
      PathName fileName (0, name, ext.GetCurrent());
      PathName path;
      if (pSession->FindFile(fileName.Get(), searchSpec.c_str(), path))
	{
	  files.push_back(path.Get());
	}
    }
}

/* _________________________________________________________________________

   MiKTeXHelp::PrintFiles
   _________________________________________________________________________ */

void
MiKTeXHelp::PrintFiles (/*[in]*/ const vector<string> & files)
{
  for (vector<string>::const_iterator it = files.begin();
       it != files.end();
       ++ it)
    {
      tcout << *it << endl;
    }
}

/* _________________________________________________________________________

   MiKTeXHelp::ViewFile
   _________________________________________________________________________ */

void
MiKTeXHelp::ViewFile (/*[in]*/ const PathName & fileName)
{
  string viewer;

  const char * lpszExt = fileName.GetExtension();

  if (lpszExt != 0)
    {
      string env = T_("MIKTEX_VIEW_");
      env += (lpszExt + 1);
      if (! Utils::GetEnvironmentString(env.c_str(), viewer))
	{
	  viewer = "";
	}
    }

#if defined(MIKTEX_WINDOWS)
  if (viewer.empty())
    {
      char szExecutable[BufferSizes::MaxPath];
      PathName directory (fileName);
      HINSTANCE hInst =
	FindExecutable(fileName.Get(), "C:\\", szExecutable);
      if (hInst >= reinterpret_cast<HINSTANCE>(32))
	{
	  viewer = szExecutable;
	  if (printOnly)
	    {
	      tcout << Q_(szExecutable) << ' ' << Q_(fileName) << endl;
	    }
	  else
	    {
	      Process::Start (szExecutable, Q_(fileName));
	    }
	  return;
	}
      else if (hInst != reinterpret_cast<HINSTANCE>(SE_ERR_NOASSOC))
	{
	  FatalError (T_("The viewer could not be started."));
	}
    }
#else


  if (viewer.empty())
    {
      string pager;
      if (! Utils::GetEnvironmentString(T_("PAGER"), pager))
	{
	  pager = T_("more");
	}
      if (lpszExt == 0)
	{
	  viewer = pager + " %f";
	}
      else if (PathName::Compare(lpszExt, ".dvi") == 0)
	{
	  viewer = T_("xdvi %f &");
	}
      else if (PathName::Compare(lpszExt, ".pdf") == 0)
	{
	  viewer = "xpdf %f &";
	}
      else if (PathName::Compare(lpszExt, ".ps") == 0)
	{
	  viewer = "gv %f &";
	}
      else if (PathName::Compare(lpszExt, ".html") == 0)
	{
	  viewer = T_("firefox %f &");
	}
      else if (PathName::Compare(lpszExt, ".txt") == 0)
	{
	  viewer = pager + " %f";
	}
    }
#endif

  if (viewer.empty())
    {
      FatalError (T_("No viewer associated with file type."));
    }

  string commandLine = viewer;

  string::size_type pos = 0;

  while ((pos = commandLine.find("%f", pos)) != string::npos)
    {
      commandLine.replace (pos, 2, fileName.Get());
      pos += fileName.GetLength();
    }

  if (printOnly)
    {
      tcout << commandLine << endl;
    }
  else
    {
      Process::ExecuteSystemCommand (commandLine.c_str());
    }
}

/* _________________________________________________________________________

   MiKTeXHelp::CreateHtmlAndView
   _________________________________________________________________________ */

void
MiKTeXHelp::CreateHtmlAndView (/*[in]*/ const char * lpszPackageName,
			       /*[in]*/ const vector<string> & files)
{
  PackageInfo pi = pManager->GetPackageInfo(lpszPackageName);
  PathName fileName = pSession->GetSpecialPath(SpecialPath::DataRoot);
  fileName += MIKTEX_PATH_MIKTEX_MTHELP_DIR;
  Directory::Create (fileName);
  fileName += lpszPackageName;
  fileName.SetExtension (".html");
  StreamWriter writer (fileName);
  int idx = 0;
  for (const char * lpsz =
	 reinterpret_cast<const char *>(templateHtml);
       idx < sizeof(templateHtml);
       ++ lpsz, ++ idx)
    {
      if (*lpsz == '%')
	{
	  string tag;
	  ++ lpsz;
	  ++ idx;
	  while (idx < sizeof(templateHtml) && *lpsz != '%')
	    {
	      tag += *lpsz;
	      ++ lpsz;
	      ++ idx;
	    }
	  if (tag == T_("NAME"))
	    {
#if defined(MIKTEX_WINDOWS)
	      WriteText (writer, Utils::AnsiToUTF8(pi.deploymentName.c_str()));
#else
	      WriteText (writer, pi.deploymentName);
#endif
	    }
	  else if (tag == T_("TITLE"))
	    {
#if defined(MIKTEX_WINDOWS)
	      WriteText (writer, Utils::AnsiToUTF8(pi.title.c_str()));
#else
	      WriteText (writer, pi.title);
#endif
	    }
	  else if (tag == T_("DESCRIPTION"))
	    {
#if defined(MIKTEX_WINDOWS)
	      WriteText (writer, Utils::AnsiToUTF8(pi.description.c_str()));
#else
	      WriteText (writer, pi.description);
#endif
	    }
	  else if (tag == T_("TRURL"))
	    {
	      for (vector<string>::const_iterator it = files.begin();
		   it != files.end();
		   ++ it)
		{
		  writer.WriteFormattedLine
		    (T_("<tr><td><a href=\"file://%s\">%s</td></tr>"),
#if defined(MIKTEX_WINDOWS)
		     Utils::AnsiToUTF8(it->c_str()).c_str(),
		     Utils::AnsiToUTF8(it->c_str()).c_str());
#else
		     it->c_str(),
		     it->c_str());
#endif
		}
	    }
	}
      else
	{
	  writer.Write (*lpsz);
	}
    }
  writer.Close ();
  ViewFile (fileName);
}

/* _________________________________________________________________________

   MiKTeXHelp::WriteText
   _________________________________________________________________________ */

void
MiKTeXHelp::WriteText (/*[in]*/ StreamWriter &		writer,
		       /*[in]*/ const string &		text)
{
  for (string::const_iterator it = text.begin(); it != text.end(); ++ it)
    {
      switch (*it)
	{
	case '<':
	  writer.Write ("&lt;");
	  break;
	case '>':
	  writer.Write ("&gt;");
	  break;
	case '&':
	  writer.Write (T_("&amp;"));
	  break;
	default:
	  writer.Write (*it);
	  break;
	}
    }
}

/* _________________________________________________________________________

   MiKTeXHelp::Run
   _________________________________________________________________________ */

void
MiKTeXHelp::Run (/*[in]*/ int		argc,
		 /*[in]*/ const char **	argv)
{
  bool optListOnly = false;
  bool optView = false;

  Cpopt popt (argc, argv, aoption);

  popt.SetOtherOptionHelp (T_("[OPTION...] NAME..."));

  int option;
  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{
	case OPT_LIST_ONLY:
	  optListOnly = true;
	  break;
	case OPT_PRINT_ONLY:
	  printOnly = true;
	  break;
	case OPT_QUIET:
	  quiet = true;
	  break;
	case OPT_VERSION:
	  ShowVersion ();
	  throw (0);
	case OPT_VIEW:
	  optView = true;
	  break;
	}
    }

  if (option != -1)
    {
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += ": ";
      msg += popt.Strerror(option);
      FatalError ("%s", msg.c_str());
    }
      
  const char ** leftovers = popt.GetArgs();

  if (leftovers == 0)
    {
      FatalError (T_("Missing NAME argument."));
    }

  for (; *leftovers != 0; ++ leftovers)
    {
      vector<string> filesByPackage;
      FindDocFilesByPackage (*leftovers, filesByPackage);
      vector<string> filesByName;
      FindDocFilesByName (*leftovers, filesByName);
      if (filesByPackage.size() + filesByName.size() == 0)
	{
	  Warning (T_("Documentation for %s could not be found."),
		   Q_(*leftovers));
	  continue;
	}
      if (optListOnly)
	{
	  PrintFiles (filesByPackage);
	  PrintFiles (filesByName);
	}
      else if (optView || filesByPackage.size() == 0)
	{
	  if (filesByPackage.size() > 0)
	    {
	      ViewFile (filesByPackage[0]);
	    }
	  else
	    {
	      ViewFile (filesByName[0]);
	    }
	}
      else
	{
	  if (filesByPackage.size() == 0)
	    {
	      Warning (T_("Documentation for package %s could not be found."),
		       Q_(*leftovers));
	      continue;
	    }
	  CreateHtmlAndView (*leftovers, filesByPackage);
	}
    }
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
extern "C"
__declspec(dllexport)
int
__cdecl
mthelp (/*[in]*/ int			argc,
	/*[in]*/ const char **	argv)
#else
extern "C"
int
mthelp (/*[in]*/ int			argc,
	/*[in]*/ const char **	argv)
#endif
{
  try
    {
      MiKTeXHelp app;
      app.Init (argv[0]);
      app.EnableInstaller (TriState::False);
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
