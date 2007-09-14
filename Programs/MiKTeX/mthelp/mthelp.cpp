/* mthelp.cpp:

   Copyright (C) 2004-2007 Christian Schenk

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

#include "mthelp-version.h"

#include <miktex/app.h>
#include <miktex/core.h>
#include <miktex/mpm.h>
#include <miktex/paths.h>
#include <miktex/reg.h>
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
#define Q_(x) Quoted(x).c_str()

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

const MIKTEXCHAR * const TheNameOfTheGame = T_("MiKTeX Help Utility");
const MIKTEXCHAR * const PROGNAME = T_("mthelp");

#if defined(MIKTEX_WINDOWS)
const MIKTEXCHAR PATH_DELIMITER = T_(';');
#define PATH_DELIMITER_STRING T_(";")
#else
const MIKTEXCHAR PATH_DELIMITER = T_(':');
#define PATH_DELIMITER_STRING T_(":")
#endif

#if defined(MIKTEX_WINDOWS)
const MIKTEXCHAR * const DEFAULT_DOC_EXTENSIONS =
  T_(".chm;.html;.dvi;.pdf;.ps;.txt");
#else
const MIKTEXCHAR * const DEFAULT_DOC_EXTENSIONS =
  T_(".html:.pdf:.dvi:.ps:.txt");
#endif

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  bool needQuotes = (strchr(lpsz, T_(' ')) != 0);
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

inline
tstring
Quoted (/*[in]*/ const tstring & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const PathName & path)
{
  return (Quoted(path.Get()));
}

/* _________________________________________________________________________

   MiKTeXHelp
   _________________________________________________________________________ */

class MiKTeXHelp : public Application
{
public:
  MiKTeXHelp ();

public:
  void
  MIKTEXAPPCALL
  Init (/*[in]*/ const Session::InitInfo & initInfo);

public:
  void
  Run (/*[in]*/ int			argc,
       /*[in]*/ const MIKTEXCHAR **	argv);

private:
  void
  ShowVersion ();
  
private:
  void
  Warning (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	   /*[in]*/				...);

private:
  MIKTEXNORETURN
  void
  FatalError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	      /*[in]*/				...);

private:
  bool
  SkipPrefix (/*[in]*/ const tstring &		str,
	      /*[in]*/ const MIKTEXCHAR *	lpszPrefix,
	      /*[out]*/ tstring &		result);

private:
  bool
  SkipTeXMFPrefix (/*[in]*/ const tstring &	str,
		   /*[out]*/ tstring &		result);

private:
  void
  FindDocFilesByPackage
  (/*[in]*/ const tstring &			packageName,
   /*[out]*/ map<tstring, vector<tstring> > &	filesByExtension);

private:
  void
  FindDocFilesByPackage
  (/*[in]*/ const tstring &		packageName,
   /*[out]*/ vector<tstring> &		files);

private:
  void
  FindDocFilesByName (/*[in]*/ const tstring &		name,
		      /*[out]*/ vector<tstring> &	files);

private:
  void
  PrintFiles (/*[in]*/ const vector<tstring> & files);

private:
  void
  ViewFile (/*[in]*/ const PathName & fileName);

private:
  void
  CreateHtmlAndView (/*[in]*/ const MIKTEXCHAR * lpszPackageName,
		     /*[in]*/ const vector<tstring> & files);

private:
  void
  WriteText (/*[in]*/ StreamWriter &	writer,
	     /*[in]*/ const tstring &	text);


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
    T_("list-only"), T_('l'), POPT_ARG_NONE, 0, OPT_LIST_ONLY,
    T_("List documentation files but do not start a viewer."),
    0
  },
  {
    T_("print-only"), T_('n'), POPT_ARG_NONE, 0, OPT_PRINT_ONLY,
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
MIKTEXAPPCALL
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
					   VER_FILEVERSION_STR)
	<< T_("\n")
	<< T_("Copyright (C) 2004-2006 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	<< endl;
}

/* _________________________________________________________________________

   MiKTeXHelp::Warning
   _________________________________________________________________________ */

void
MiKTeXHelp::Warning (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
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
MiKTeXHelp::FatalError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
			/*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << PROGNAME << T_(": ")
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   MiKTeXHelp::SkipPrefix
   _________________________________________________________________________ */

bool
MiKTeXHelp::SkipPrefix (/*[in]*/ const tstring &	str,
			/*[in]*/ const MIKTEXCHAR *	lpszPrefix,
			/*[out]*/ tstring &		result)
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
MiKTeXHelp::SkipTeXMFPrefix (/*[in]*/ const tstring &	str,
			     /*[out]*/ tstring &	result)
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
(/*[in]*/ const tstring &			packageName,
 /*[out]*/ map<tstring, vector<tstring> > &	filesByExtension)
{
  PackageInfo pi;
  if (! pManager->TryGetPackageInfo(packageName, pi))
    {
      return;
    }
  for (vector<tstring>::const_iterator it = pi.docFiles.begin();
       it != pi.docFiles.end();
       ++ it)
    {
      MIKTEXCHAR szExt[BufferSizes::MaxPath];
      PathName::Split (it->c_str(),
		       0, 0,
		       0, 0,
		       szExt, BufferSizes::MaxPath);
      tstring file;
      PathName path;
      if (SkipTeXMFPrefix(*it, file)
	  && pSession->FindFile(file.c_str(),
				MIKTEX_PATH_TEXMF_PLACEHOLDER_NO_MPM,
				path))
	{
	  vector<tstring> & files = filesByExtension[szExt];
	  files.push_back (path.ToString());
	}
    }
}

/* _________________________________________________________________________

   MiKTeXHelp::FindDocFilesByPackage
   _________________________________________________________________________ */

void
MiKTeXHelp::FindDocFilesByPackage (/*[in]*/ const tstring &	packageName,
				   /*[out]*/ vector<tstring> &	files)
{
  map<tstring, vector<tstring> > filesByExtension;
  FindDocFilesByPackage (packageName, filesByExtension);
  tstring extensions =
    pSession->GetConfigValue(0,
			     MIKTEX_REGVAL_DOC_EXTENSIONS,
			     DEFAULT_DOC_EXTENSIONS);
  for (Tokenizer ext (extensions.c_str(), PATH_DELIMITER_STRING);
       ext.GetCurrent() != 0;
       ++ ext)
    {
      vector<tstring> & vec = filesByExtension[ext.GetCurrent()];
      vector<tstring>::iterator it = vec.begin();
      while (it != vec.end())
	{
	  MIKTEXCHAR szName[BufferSizes::MaxPath];
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
      vector<tstring> & vec = filesByExtension[ext.GetCurrent()];
      files.insert (files.end(), vec.begin(), vec.end());
    }
}

/* _________________________________________________________________________

   MiKTeXHelp::FindDocFilesByName
   _________________________________________________________________________ */

void
MiKTeXHelp::FindDocFilesByName (/*[in]*/ const tstring &	name,
				/*[out]*/ vector<tstring> &	files)
{
  tstring extensions =
    pSession->GetConfigValue(0,
			     MIKTEX_REGVAL_DOC_EXTENSIONS,
			     DEFAULT_DOC_EXTENSIONS);
  tstring searchSpec = MIKTEX_PATH_TEXMF_PLACEHOLDER_NO_MPM;
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
MiKTeXHelp::PrintFiles (/*[in]*/ const vector<tstring> & files)
{
  for (vector<tstring>::const_iterator it = files.begin();
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
  tstring viewer;

  const MIKTEXCHAR * lpszExt = fileName.GetExtension();

  if (lpszExt != 0)
    {
      tstring env = T_("MIKTEX_VIEW_");
      env += (lpszExt + 1);
      if (! Utils::GetEnvironmentString(env.c_str(), viewer))
	{
	  viewer = T_("");
	}
    }

#if defined(MIKTEX_WINDOWS)
  if (viewer.empty())
    {
      MIKTEXCHAR szExecutable[BufferSizes::MaxPath];
      PathName directory (fileName);
      HINSTANCE hInst =
	FindExecutable(fileName.Get(), T_("C:\\"), szExecutable);
      if (hInst >= reinterpret_cast<HINSTANCE>(32))
	{
	  viewer = szExecutable;
	  if (printOnly)
	    {
	      tcout << Q_(szExecutable) << T_(' ') << Q_(fileName) << endl;
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
      tstring pager;
      if (! Utils::GetEnvironmentString(T_("PAGER"), pager))
	{
	  pager = T_("more");
	}
      if (lpszExt == 0)
	{
	  viewer = pager + T_(" %f");
	}
      else if (PathName::Compare(lpszExt, T_(".dvi")) == 0)
	{
	  viewer = T_("xdvi %f &");
	}
      else if (PathName::Compare(lpszExt, T_(".pdf")) == 0)
	{
	  viewer = T_("xpdf %f &");
	}
      else if (PathName::Compare(lpszExt, T_(".ps")) == 0)
	{
	  viewer = T_("gv %f &");
	}
      else if (PathName::Compare(lpszExt, T_(".html")) == 0)
	{
	  viewer = T_("firefox %f &");
	}
      else if (PathName::Compare(lpszExt, T_(".txt")) == 0)
	{
	  viewer = pager + T_(" %f");
	}
    }
#endif

  if (viewer.empty())
    {
      FatalError (T_("No viewer associated with file type."));
    }

  tstring commandLine = viewer;

  tstring::size_type pos = 0;

  while ((pos = commandLine.find(T_("%f"), pos)) != tstring::npos)
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
MiKTeXHelp::CreateHtmlAndView (/*[in]*/ const MIKTEXCHAR * lpszPackageName,
			       /*[in]*/ const vector<tstring> & files)
{
  PackageInfo pi = pManager->GetPackageInfo(lpszPackageName);
  PathName fileName = pSession->GetSpecialPath(SpecialPath::DataRoot);
  fileName += MIKTEX_PATH_MIKTEX_MTHELP_DIR;
  Directory::Create (fileName);
  fileName += lpszPackageName;
  fileName.SetExtension (T_(".html"));
  StreamWriter writer (fileName);
  int idx = 0;
  for (const MIKTEXCHAR * lpsz =
	 reinterpret_cast<const MIKTEXCHAR *>(templateHtml);
       idx < sizeof(templateHtml);
       ++ lpsz, ++ idx)
    {
      if (*lpsz == T_('%'))
	{
	  tstring tag;
	  ++ lpsz;
	  ++ idx;
	  while (idx < sizeof(templateHtml) && *lpsz != T_('%'))
	    {
	      tag += *lpsz;
	      ++ lpsz;
	      ++ idx;
	    }
	  if (tag == T_("NAME"))
	    {
	      WriteText (writer, pi.deploymentName);
	    }
	  else if (tag == T_("TITLE"))
	    {
	      WriteText (writer, pi.title);
	    }
	  else if (tag == T_("DESCRIPTION"))
	    {
	      WriteText (writer, pi.description);
	    }
	  else if (tag == T_("TRURL"))
	    {
	      for (vector<tstring>::const_iterator it = files.begin();
		   it != files.end();
		   ++ it)
		{
		  writer.WriteFormattedLine
		    (T_("<tr><td><a href=\"file://%s\">%s</td></tr>"),
		     it->c_str(),
		     it->c_str());
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
		       /*[in]*/ const tstring &		text)
{
  for (tstring::const_iterator it = text.begin(); it != text.end(); ++ it)
    {
      switch (*it)
	{
	case T_('<'):
	  writer.Write (T_("&lt;"));
	  break;
	case T_('>'):
	  writer.Write (T_("&gt;"));
	  break;
	case T_('&'):
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
      tstring msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += T_(": ");
      msg += popt.Strerror(option);
      FatalError (T_("%s"), msg.c_str());
    }
      
  const MIKTEXCHAR ** leftovers = popt.GetArgs();

  if (leftovers == 0)
    {
      FatalError (T_("Missing NAME argument."));
    }

  for (; *leftovers != 0; ++ leftovers)
    {
      vector<tstring> filesByPackage;
      FindDocFilesByPackage (*leftovers, filesByPackage);
      vector<tstring> filesByName;
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
	/*[in]*/ const MIKTEXCHAR **	argv)
#else
extern "C"
int
mthelp (/*[in]*/ int			argc,
	/*[in]*/ const MIKTEXCHAR **	argv)
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
