/* mcd.cpp: MiKTeX compiler driver

   Copyright (C) 1998-2008 Christian Schenk

   Copyright (C) 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2001,
   2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This file is part of the MiKTeX Compiler Driver.

   The MiKTeX Compiler Driver is free software; you can redistribute
   it and/or modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX Compiler Driver is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with The MiKTeX Compiler Driver; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

/* This program imitates the shell script texi2dvi, which is a part of
   the GNU Texinfo package.  Texi2dvi was originally written by Noah
   Friedman.

   Most of the source comments are stolen from the texi2dvi script. */

#include <cctype>
#include <cstdarg>
#include <cstdlib>

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include <miktex/App/Application>
#include <miktex/Core/Core>
#include <miktex/Core/Trace>
#include <miktex/Core/Environment>
#include <popt-miktex.h>
#include <regex.h>

#include "mcd-version.h"

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace std;

#define ALWAYS_UNUSED(x) (x)

#define SUPPORT_OPT_SRC_SPECIALS

/* Patch (#464378) by Thomas Schimming (http://sf.net/users/schimmin/):

   The BibTeX handler is extended to work in a chapterbib compliant
   mode, that is, bibtex is run on all 1st level AUX files referenced
   in the main AUX file.  After the special treatment (if use of the
   chapterbib package is detected), the regular bibtex treatment
   occurs (as before).

   The code uses some heuristics to determine when it's necessary to
   run this extension.  However, this can still be improved as for
   example changes in bibstyle do not correctly trigger a re-bibtex
   yet.

   Ultimately, the behavior should probably be included into bibtex,
   however in this way, an "orthodox" latex distribution is
   maintained... */

#define SF464378__CHAPTERBIB

#define PROGRAM_NAME "texify"

#if ! defined(THE_NAME_OF_THE_GAME)
#  define THE_NAME_OF_THE_GAME T_("MiKTeX Compiler Driver")
#endif

#define T_(x) MIKTEXTEXT(x)
#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

const char * DEFAULT_TRACE_STREAMS =
  MIKTEX_TRACE_ERROR ","
  MIKTEX_TRACE_PROCESS ","
  PROGRAM_NAME;

/* _________________________________________________________________________

   FatalError
   _________________________________________________________________________ */

MIKTEXNORETURN
void
FatalError (/*[in]*/ const char *	lpszFormat,
	    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << PROGRAM_NAME << ": "
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
  throw (1);
}

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
    ALWAYS_UNUSED (pOutput);
    ALWAYS_UNUSED (n);
    return (true);
  }
};

/* _________________________________________________________________________

   ProcessOutputSaver
   _________________________________________________________________________ */

class
ProcessOutputSaver
  : public IRunProcessCallback
{
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n)
  {
    output.append (reinterpret_cast<const char*>(pOutput), n);
    return (true);
  }
public:
  const string &
  GetOutput ()
    const
  {
    return (output);
  }
private:
  string output;
};

/* _________________________________________________________________________

   CompareFiles
   _________________________________________________________________________ */

bool
CompareFiles (/*[in]*/ const PathName &	fileName1,
	      /*[in]*/ const PathName &	fileName2)
{
  auto_ptr<MemoryMappedFile> pFile1 (MemoryMappedFile::Create());
  void * ptr1 = pFile1->Open(fileName1, false);
  auto_ptr<MemoryMappedFile> pFile2 (MemoryMappedFile::Create());
  void * ptr2 = pFile2->Open(fileName2, false);
  if (pFile1->GetSize() != pFile2->GetSize())
    {
      return (false);
    }
  bool ret = (memcmp(ptr1, ptr2, pFile1->GetSize()) == 0);
  pFile1->Close ();
  pFile2->Close ();
  return (ret);
}

/* _________________________________________________________________________

   CopyFiles
   _________________________________________________________________________ */

void
CopyFiles (/*[in]*/ const vector<string> &	vec,
	   /*[in]*/ const PathName &		destDir)
{
  for (vector<string>::const_iterator it = vec.begin();
       it != vec.end();
       ++ it)
    {
      File::Copy (*it, PathName(destDir, *it));
    }
}

/* _________________________________________________________________________

   ReadFile
   _________________________________________________________________________ */

vector<char>
ReadFile (/*[in]*/ const PathName &	fileName)
{
  size_t fileSize = File::GetSize(fileName);
  vector<char> vec (fileSize + 1);
  FileStream stream (File::Open(fileName,
				FileMode::Open,
				FileAccess::Read));
  fileSize = stream.Read(&vec[0], fileSize);
  stream.Close ();
  vec[fileSize] = 0;
  return (vec);
}

/* _________________________________________________________________________

   Contains
   _________________________________________________________________________ */

bool
Contains (/*[in]*/ const PathName &		fileName,
	  /*[in]*/ regex_t *			preg)
{
  vector<char> file = ReadFile(fileName);
  return (regexec(preg, &file[0], 0, 0, 0) == 0);
}

/* _________________________________________________________________________

   Contains
   _________________________________________________________________________ */

bool
Contains (/*[in]*/ const PathName &	fileName,
	  /*[in]*/ const char *		lpszText)
{
  auto_ptr<MemoryMappedFile> pMappedFile (MemoryMappedFile::Create());
  const char * ptr =
    reinterpret_cast<char*>(pMappedFile->Open(fileName, false));
  size_t size = pMappedFile->GetSize();
  const char * p = lpszText;
  for (size_t i = 0; *p != 0 && i < size; ++ i, ++ ptr)
    {
      if (*ptr == *p)
	{
	  ++ p;
	}
      else
	{
	  p = lpszText;
	}
    }
  pMappedFile->Close ();
  return (*p == 0);
}

/* _________________________________________________________________________

   FlattenStringVector
   _________________________________________________________________________ */

string
FlattenStringVector (/*[in]*/ const vector<string> &	vec,
		     /*[in]*/ char			sep)
{
  string str = "";
  for (vector<string>::const_iterator it = vec.begin();
       it != vec.end();
       ++ it)
    {
      if (it != vec.begin())
	{
	  str += sep;
	}
      bool mustQuote = (it->find(sep) != string::npos);
      if (mustQuote)
	{
	  str += '"';
	}
      str += *it;
      if (mustQuote)
	{
	  str += '"';
	}
    }
  return (str);
}

/* _________________________________________________________________________

   IsPrefixOf
   _________________________________________________________________________ */

bool
IsPrefixOf (/*[in]*/ const char *	lpszPrefix,
	    /*[in]*/ const string &	str)
{
  return (str.compare(0, StrLen(lpszPrefix), lpszPrefix) == 0);
}

/* _________________________________________________________________________

   MacroLanguage
   _________________________________________________________________________ */

class MacroLanguageEnum {
public:
  enum EnumType { None, LaTeX, Texinfo };
};

typedef EnumWrapper<MacroLanguageEnum> MacroLanguage;

/* _________________________________________________________________________

   Engine
   _________________________________________________________________________ */

class EngineEnum
{
public:
  enum EnumType {
    NotSet,
    TeX,
    pdfTeX,
    XeTeX,
    Omega,
  };
};

typedef EnumWrapper<EngineEnum> Engine;

/* _________________________________________________________________________

   OutputType
   _________________________________________________________________________ */

class OutputTypeEnum {
public:
  enum EnumType { None, DVI, PDF, PS };
};

typedef EnumWrapper<OutputTypeEnum> OutputType;

/* _________________________________________________________________________

   Options
   _________________________________________________________________________ */

class Options
{
public:
  Options ();

public:
  ~Options ();

public:
  PathName startDirectory;

public:
  regex_t regex_bibdata;

public:
  regex_t regex_bibstyle;

public:
  regex_t regex_citation_undefined;

public:
  regex_t regex_no_file_bbl;

public:
  regex_t regex_texinfo_version;

#ifdef SF464378__CHAPTERBIB
public:
  regex_t regex_chapterbib;
public:
  regex_t regex_input_aux;
#endif /* SF464378__CHAPTERBIB */

public:
  bool batch;

public:
  bool clean;

public:
  bool expand;

public:
  bool quiet;

public:
  bool verbose;

public:
  OutputType outputType;

public:
  bool runViewer;

#if defined(SUPPORT_OPT_SRC_SPECIALS)
public:
  bool sourceSpecials;
#endif

public:
  int maxIterations;

public:
  vector<string> includeDirectories;

public:
  vector<string> texinfoCommands;

public:
  MacroLanguage macroLanguage;

public:
  string sourceSpecialsWhere;

public:
  string bibtexProgram;

public:
  string latexProgram;

public:
  string pdflatexProgram;

public:
  string xelatexProgram;

public:
  string makeindexProgram;

public:
  string makeinfoProgram;

public:
  string texProgram;

public:
  string pdftexProgram;

public:
  string xetexProgram;

public:
  string texindexProgram;

public:
  Argv makeindexOptions;

public:
  Argv texOptions;

public:
  Argv viewerOptions;

public:
  string traceStreams;

private:
  void
  SetProgramName (/*[out]*/ string &	str,
		  /*[in]*/ const char *	lpszEnvName,
		  /*[in]*/ const char *	lpszDefault)
  {
    if (! Utils::GetEnvironmentString(lpszEnvName, str))
      {
	str = lpszDefault;
      }
  }

public:
  Engine engine;

public:
  void
  SetEngine (/*[in]*/ const char * lpszEngine)
  {
    if (StringCompare(lpszEngine, "tex", true) == 0)
      {
	engine = Engine::TeX;
      }
    else if (StringCompare(lpszEngine, "pdftex", true) == 0)
      {
	engine = Engine::pdfTeX;
      }
    else if (StringCompare(lpszEngine, "xetex", true) == 0)
      {
	engine = Engine::XeTeX;
      }
    else
      {
	FatalError (T_("Unknown engine: %s"), lpszEngine);
      }
  }
};

/* _________________________________________________________________________

   Options::Options
   _________________________________________________________________________ */

Options::Options ()
  : batch (false),
    clean (false),
    expand (false),
    outputType (OutputType::DVI),
    quiet (false),
    runViewer (false),
#if defined(SUPPORT_OPT_SRC_SPECIALS)
    sourceSpecials (false),
#endif
    verbose (false),
    maxIterations (5),
    macroLanguage (MacroLanguage::None),
    engine (Engine::NotSet)
{
  if (regcomp(&regex_bibdata,
	      "^\\\\bibdata",
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }
  
  if (regcomp(&regex_bibstyle,
	      "^\\\\bibstyle",
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }
  
  if (regcomp(&regex_citation_undefined,
	      "Warning:.*Citation.*undefined",
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }
  
  if (regcomp(&regex_no_file_bbl,
	      "No file .*\\.bbl\\.",
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }

  if (regcomp(&regex_texinfo_version,
	      "\\[(.*)version (....)-(..)-(..)",
	      REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }

#if defined(SF464378__CHAPTERBIB)
  if (regcomp(&regex_chapterbib,
	      "^Package: chapterbib",
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }

  if (regcomp(&regex_input_aux,
	      "^\\\\@input\\{.*\\.aux\\}",
	      REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }
#endif /* SF464378__CHAPTERBIB */

  startDirectory.SetToCurrentDirectory ();

  startDirectory.ToUnix ();
    
  SetProgramName (bibtexProgram, "BIBTEX", "bibtex");
  SetProgramName (latexProgram, "LATEX", "latex");
  SetProgramName (makeindexProgram, "MAKEINDEX", "makeindex");
  SetProgramName (makeinfoProgram, "MAKEINFO", "makeinfo");
  SetProgramName (pdflatexProgram, "PDFLATEX", "pdflatex");
  SetProgramName (pdftexProgram, "PDFTEX", "pdftex");
  SetProgramName (texProgram, "TEX", "tex");
  SetProgramName (texindexProgram, "TEXINDEX", "texindex");
  SetProgramName (xelatexProgram, "XELATEX", "xelatex");
  SetProgramName (xetexProgram, "XETEX", "xetex");
}

/* _________________________________________________________________________

   Options::~Options
   _________________________________________________________________________ */

Options::~Options ()
{
  regfree (&regex_bibdata);
  regfree (&regex_bibstyle);
  regfree (&regex_citation_undefined);
  regfree (&regex_no_file_bbl);
  regfree (&regex_texinfo_version);
#if defined(SF464378__CHAPTERBIB)
  regfree (&regex_chapterbib);
  regfree (&regex_input_aux);
#endif
}

/* _________________________________________________________________________

   McdApp
   _________________________________________________________________________ */

class McdApp : public Application
{
public:
  McdApp ()
    : traceStream (TraceStream::Open(PROGRAM_NAME))
  {
  };

public:
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv);

public:
  void
  Trace (/*[in]*/ const char *	lpszFormat,
	 /*[in]*/		...);

public:
  void
  Verbose (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  void
  Version ();

private:
  auto_ptr<TraceStream> traceStream;

public:
  Options options;
};

/* _________________________________________________________________________

   McdApp::Verbose
   _________________________________________________________________________ */

void
McdApp::Verbose (/*[in]*/ const char *	lpszFormat,
		 /*[in]*/		...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  traceStream->WriteLine (PROGRAM_NAME,
			  Utils::FormatString(lpszFormat, arglist).c_str());
  if (options.verbose)
    {
      cout << Utils::FormatString(lpszFormat, arglist)
	   << endl;
    }
  va_end (arglist);
}

/* _________________________________________________________________________

   McdApp::Trace
   _________________________________________________________________________ */

void
McdApp::Trace (/*[in]*/ const char *	lpszFormat,
	       /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  traceStream->WriteLine (PROGRAM_NAME,
			  Utils::FormatString(lpszFormat, arglist).c_str());
  va_end (arglist);
}

/* _________________________________________________________________________

   McdApp::Version
   _________________________________________________________________________ */

void
McdApp::Version ()
{
  cout << Utils::MakeProgramVersionString(THE_NAME_OF_THE_GAME,
					  VersionNumber(VER_FILEVERSION))
       << T_("\n\
Copyright (C) 1998-2008 Christian Schenk\n\
Copyright (C) 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2001,\n\
              2002, 2003, 2004, 2005 Free Software Foundation, Inc.\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
       << endl;
}

/* _________________________________________________________________________

   Driver
   _________________________________________________________________________ */

class Driver
{
public:
  Driver ();

public:
  ~Driver ();

public:
  void
  Initialize (/*[in]*/ McdApp *		pApplication,
	      /*[in]*/ Options *	pOptions,
	      /*[in]*/ const char *	lpszFileName);

public:
  void
  Run ();

private:
  MacroLanguage
  GuessMacroLanguage (/*[in]*/ const PathName & fileName);

private:
  void
  TexinfoPreprocess (/*[in]*/ const PathName &	pathFrom,
		     /*[in]*/ const PathName &	pathTo);

private:
  void
  TexinfoUncomment (/*[in]*/ const PathName &	pathFrom,
		    /*[in]*/ const PathName &	pathTo);


private:
  void
  SetIncludeDirectories ();

private:
  void
  ExpandMacros ();

private:
  void
  InsertCommands ();

private:
  bool
  RunMakeinfo (/*[in]*/ const PathName &	pathFrom,
	       /*[in]*/ const PathName &	pathTo);

private:
  void
  RunBibTeX ();

private:
  PathName
  GetTeXEnginePath (/*[out]*/ string & exeName);

private:
  void
  RunTeX ();

private:
  void
  RunIndexGenerator (/*[in]*/ const vector<string> & idxFiles);

private:
  void
  RunViewer ();

private:
  bool
  Ready ();


private:
  bool
  Check_texinfo_tex ();

private:
  void
  InstallOutputFile ();

private:
  void
  GetAuxFiles (/*[out]*/ vector<string> &	auxFiles,
	       /*[out]*/ vector<string> *	pIdxFiles = 0);

private:
  void
  GetAuxFiles (/*[in]*/ const PathName &	baseName,
	       /*[in]*/ const char *		lpszExtension,
	       /*[out]*/ vector<string> &	auxFiles);

private:
  void
  InstallProgram (/*[in]*/ const char * lpszProgram);

  // the macro language
private:
  MacroLanguage macroLanguage;

  // file name given on the command-line
private:
  PathName givenFileName;

  // fully qualified path to the original input file
private:
  PathName m_pathOriginalInputFile;

  // super-temporary directory
private:
  PathName tempDirectory;

  // directory which contains the original input file
private:
  PathName originalInputDirectory;

  // name of the input file
private:
  PathName inputName;

  // name of the input file, no extension
private:
  PathName inputNameNoExt;

  // the scratch directory: it will be made the current directory if
  // we are in clean mode; contains the input file, if --expand was
  // specified on the command-line
private:
  PathName scratchDirectory;

  // the extra directory: contains the input file, if --texinfo was
  // specified on the command-line
private:
  PathName extraDirectory;

  // the aux directory: contains backed up auxiliary files
private:
  PathName auxDirectory;

  // fully qualified path to the input file
private:
  PathName pathInputFile;

  // contains auxiliary files from the last run
private:
  vector<string> previousAuxFiles;

private:
  bool madeTempDirectory;

private:
  McdApp * pApplication;

private:
  Options * pOptions;

protected:
  SessionWrapper pSession;
};

/* _________________________________________________________________________

   Driver::Driver
   _________________________________________________________________________ */

Driver::Driver ()
  : madeTempDirectory (false),
    pApplication (0),
    pOptions (0),
    pSession (true),
    macroLanguage (MacroLanguage::None)
{
}

/* _________________________________________________________________________

   Driver::~Driver
   _________________________________________________________________________ */

Driver::~Driver ()
{
  try
    {
      if (pOptions != 0 && pOptions->clean)
	{
	  Directory::SetCurrentDirectory (pOptions->startDirectory);
	}
      if (madeTempDirectory)
	{
	  Directory::Delete (tempDirectory, true);
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   Driver::Initialize
   _________________________________________________________________________ */

void
Driver::Initialize (/*[in]*/ McdApp *		pApplication,
		    /*[in]*/ Options *		pOptions,
		    /*[in]*/ const char *	lpszFileName)
{
  this->pApplication = pApplication;
  this->pOptions = pOptions;

  pApplication->Trace (T_("Initializing driver..."));
 
  givenFileName = lpszFileName;
  
  // Get the name of the current directory.  We want the full path
  // because in clean mode we are in tmp, in which case a relative
  // path has no meaning.
  m_pathOriginalInputFile = givenFileName;
  m_pathOriginalInputFile.MakeAbsolute ();
  m_pathOriginalInputFile.ToUnix ();

  pApplication->Trace (T_("input file: %s"), Q_(m_pathOriginalInputFile));

  givenFileName.GetFileNameWithoutExtension (inputNameNoExt);

  inputName = givenFileName;
  inputName.RemoveDirectorySpec ();

  // create a super-temp directory
  ScratchDirectory::Create (tempDirectory);
  madeTempDirectory = true;

  // create scratch directory
  scratchDirectory.Set (tempDirectory, "_src");
  Directory::Create (scratchDirectory);
  scratchDirectory.ToUnix ();
  pApplication->Trace (T_("scratch directory: %s"), Q_(scratchDirectory));

  // create extra directory
  extraDirectory.Set (tempDirectory, "_xtr");
  Directory::Create (extraDirectory);
  extraDirectory.ToUnix ();
  pApplication->Trace (T_("extra directory: %s"), Q_(extraDirectory));

  // create aux directory
  auxDirectory.Set (tempDirectory, "_aux");
  Directory::Create (auxDirectory);
  auxDirectory.ToUnix ();
  pApplication->Trace (T_("aux directory: %s"), Q_(auxDirectory));

  // If the user explicitly specified the language, use that.
  // Otherwise, if the first line is \input texinfo, assume it's
  // texinfo.  Otherwise, guess from the file extension.
  macroLanguage = pOptions->macroLanguage;
  if (macroLanguage == MacroLanguage::None)
    {
      macroLanguage = GuessMacroLanguage(givenFileName);
    }

  // make fully qualified path to the given input file
  if (Utils::IsAbsolutePath(givenFileName.Get()))
    {
      pathInputFile = givenFileName;
    }
  else
    {
      pathInputFile.Set (pOptions->startDirectory, givenFileName);
    }

  originalInputDirectory = pathInputFile;
  originalInputDirectory.RemoveFileSpec ();
  originalInputDirectory.ToUnix ();
}

/* _________________________________________________________________________

   Driver::GuessMacroLanguage
   _________________________________________________________________________ */

MacroLanguage
Driver::GuessMacroLanguage (/*[in]*/ const PathName & fileName)
{
  StreamReader reader (fileName);
  string firstLine;
  if (! reader.ReadLine(firstLine))
    {
      return (MacroLanguage::None);
    }
  reader.Close ();
  if (firstLine.find("input texinfo") != string::npos)
    {
      return (MacroLanguage::Texinfo);
    }
  if (fileName.HasExtension(".dtx")
      || fileName.HasExtension(".tex")
      || fileName.HasExtension(".latex")
      || fileName.HasExtension(".ltx"))
    {
      return (MacroLanguage::LaTeX);
    }
  else
    {
      return (MacroLanguage::Texinfo);
    }
}

/* _________________________________________________________________________

   Driver::TexinfoPreprocess

   A function that preprocesses Texinfo sources in order to keep the
   iftex sections only.  We want to remove non TeX sections, and
   comment (with `@c texi2dvi') TeX sections so that makeinfo does not
   try to parse them.  Nevertheless, while commenting TeX sections,
   don't comment @macro/@end macro so that makeinfo does propagate
   them.  Unfortunately makeinfo --iftex --no-ifhtml --no-ifinfo
   doesn't work well enough (yet) to use that, so work around with
   sed.
   _________________________________________________________________________ */

void
Driver::TexinfoPreprocess (/*[in]*/ const PathName &	pathFrom,
			   /*[in]*/ const PathName &	pathTo)
{
  StreamReader reader (pathFrom);
  StreamWriter writer (pathTo);
  bool at_tex = false;
  bool at_iftex = false;
  bool at_macro = false;
  bool at_html = false;
  bool at_ifhtml = false;
  bool at_ifnottex = false;
  bool at_ifinfo = false;
  bool at_menu = false;
  string line;
  while (reader.ReadLine(line))
    {
      if (IsPrefixOf("@tex", line))
	{
	  at_tex = true;
	}
      else if (IsPrefixOf("@iftex", line))
	{
	  at_iftex = true;
	}
      else if (IsPrefixOf("@macro", line))
	{
	  at_macro = true;
	}
      else if (IsPrefixOf("@html", line))
	{
	  at_html = true;
	}
      else if (IsPrefixOf("@ifnottex", line))
	{
	  at_ifnottex = true;
	}
      else if (IsPrefixOf("@ifinfo", line))
	{
	  at_ifinfo = true;
	}
      else if (IsPrefixOf("@menu", line))
	{
	  at_menu = true;
	}

      bool commentingOut = (at_tex || (at_iftex && ! at_macro));
      bool deleting = (at_html || at_ifhtml || at_ifnottex
		       || (at_ifinfo
			   && ! at_menu
			   && ! IsPrefixOf("@node", line)));

      if (commentingOut)
	{
	  writer.WriteLine (string("@c texi2dvi") + line);
	}
      else if (! deleting)
	{
	  writer.WriteLine (line);
	}

      if (IsPrefixOf("@end tex", line))
	{
	  at_tex = false;
	}
      else if (IsPrefixOf("@end iftex", line))
	{
	  at_iftex = false;
	}
      else if (IsPrefixOf("@end macro", line))
	{
	  at_macro = false;
	}
      else if (IsPrefixOf("@end html", line))
	{
	  at_html = false;
	}
      else if (IsPrefixOf("@end ifnottex", line))
	{
	  at_ifnottex = false;
	}
      else if (IsPrefixOf("@end ifinfo", line))
	{
	  at_ifinfo = false;
	}
      else if (IsPrefixOf("@end menu", line))
	{
	  at_menu = false;
	}
    }
}

/* _________________________________________________________________________

   Driver::TexinfoUncomment

   Uncommenting is simple: Remove any leading `@c texi2dvi'.
   _________________________________________________________________________ */

void
Driver::TexinfoUncomment (/*[in]*/ const PathName &	pathFrom,
			  /*[in]*/ const PathName &	pathTo)
{
  StreamReader reader (pathFrom);
  StreamWriter writer (pathTo);
  string line;
  while (reader.ReadLine(line))
    {
      if (IsPrefixOf("@c texi2dvi", line))
	{
	  writer.WriteLine (line.c_str() + 11);
	}
      else
	{
	  writer.WriteLine (line);
	}
    }
  writer.Close ();
  reader.Close ();
}

/* _________________________________________________________________________

   Driver::SetIncludeDirectories

   Source file might include additional sources.  Put `.' and
   directory where source file(s) reside in MIKTEX_CWD before anything
   else.  Include orig_pwd in case we are in clean mode, where we've
   cd'd to a temp directory.
   _________________________________________________________________________ */

void
Driver::SetIncludeDirectories ()
{
  pSession->AddWorkingDirectory (pOptions->startDirectory.Get(), true);
  if (originalInputDirectory != pOptions->startDirectory)
    {
      pSession->AddWorkingDirectory (originalInputDirectory.Get(), true);
    }
  for (vector<string>::iterator it = pOptions->includeDirectories.begin();
       it != pOptions->includeDirectories.end();
       ++ it)
    {
      pSession->AddWorkingDirectory (it->c_str(), true);
    }
}

/* _________________________________________________________________________

   Driver::RunMakeinfo

   Expand macro commands in the original source file using Makeinfo.
   Always use `end' footnote style, since the `separate' style
   generates different output (arguably this is a bug in -E).  Discard
   main info output, the user asked to run TeX, not makeinfo.
   _________________________________________________________________________ */

bool
Driver::RunMakeinfo (/*[in]*/ const PathName &	pathFrom,
		     /*[in]*/ const PathName &	pathTo)
{
  PathName pathExe;

  if (! pSession->FindFile(pOptions->makeinfoProgram.c_str(),
			   FileType::EXE,
			   pathExe))
    {
      FatalError (T_("%s could not be found."), Q_(pOptions->makeinfoProgram));
    }

  CommandLineBuilder commandLine;

  commandLine.AppendOption ("--footnote-style=", "end");
  commandLine.AppendOption ("-I ", originalInputDirectory);

  for (vector<string>::iterator it = pOptions->includeDirectories.begin();
       it != pOptions->includeDirectories.end();
       ++ it)
    {
      commandLine.AppendOption ("-I ", *it);
    }

#if defined(MIKTEX_WINDOWS)
  commandLine.AppendOption ("-o ", "nul");
#else
  commandLine.AppendOption ("-o ", "/dev/null");
#endif

  commandLine.AppendOption ("--macro-expand=", pathTo);

  commandLine.AppendArgument (pathFrom);

  int exitCode = 0;

  ProcessOutputTrash trash;
  Process::Run (pathExe,
		commandLine.Get(),
		&trash,
		&exitCode,
		0);

  return (exitCode == 0);
}

/* _________________________________________________________________________

   Driver::Check_texinfo_tex

   Check if texinfo.tex performs macro expansion by looking for its
   version.  The version is a date of the form YEAR-MO-DA.
   _________________________________________________________________________ */

// minimum texinfo.tex version to have macro expansion
const char * txiprereq = "19990129";

bool
Driver::Check_texinfo_tex ()
{
  PathName pathExe;

  if (! pSession->FindFile(pOptions->texProgram, FileType::EXE, pathExe))
    {
      FatalError (T_("%s could not be found."), Q_(pOptions->texProgram));
    }
  
  bool newer = false;

  ScratchDirectory scratchDirectory;
  scratchDirectory.Enter ("mcd");

  StreamWriter writer ("txiversion.tex");
  writer.WriteLine ("\\input texinfo.tex @bye");
  writer.Close ();

  int exitCode = 0;
  ProcessOutputSaver processOutput;
  if (! Process::Run(pathExe,
		     "txiversion.tex",
		     &processOutput,
		     &exitCode,
		     0))
    {
      FatalError (T_("%s could not be started."), Q_(pathExe));
    }

  if (exitCode == 0)
    {
      regmatch_t regMatch[5];
      if (regexec(&pOptions->regex_texinfo_version,
		  processOutput.GetOutput().c_str(),
		  5,
		  regMatch,
		  0)
	  == 0)
	{
	  string txiformat;
	  for (int i = regMatch[1].rm_so; i < regMatch[1].rm_eo; ++ i)
	    {
	      txiformat += processOutput.GetOutput()[i];
	    }
	  string version;
	  version += processOutput.GetOutput()[regMatch[2].rm_so + 0];
	  version += processOutput.GetOutput()[regMatch[2].rm_so + 1];
	  version += processOutput.GetOutput()[regMatch[2].rm_so + 2];
	  version += processOutput.GetOutput()[regMatch[2].rm_so + 3];
	  version += processOutput.GetOutput()[regMatch[3].rm_so + 0];
	  version += processOutput.GetOutput()[regMatch[3].rm_so + 1];
	  version += processOutput.GetOutput()[regMatch[4].rm_so + 0];
	  version += processOutput.GetOutput()[regMatch[4].rm_so + 1];
	  pApplication->Verbose (T_("\
texinfo.tex preloaded as %s, version is %s..."),
				 txiformat.c_str(),
				 version.c_str());
	  newer = (atoi(txiprereq) <= atoi(version.c_str()));
	}
    }

  scratchDirectory.Leave ();

  return (! newer);
}

/* _________________________________________________________________________

   Driver::ExpandMacros
   _________________________________________________________________________ */

void
Driver::ExpandMacros ()
{
  if (macroLanguage != MacroLanguage::Texinfo)
    {
      return;
    }

  // Unless required by the user, makeinfo expansion is wanted only if
  // texinfo.tex is too old.
  bool expand = pOptions->expand;
  if (! expand)
    {
      expand = Check_texinfo_tex();
    }
  if (! expand)
    {
      return;
    }

  PathName pathTmpFile1;
  PathName pathTmpFile2;

  pathTmpFile1.SetToTempFile ();
  pathTmpFile2.SetToTempFile ();

  PathName path (scratchDirectory, inputName);

  pApplication->Verbose (T_("Macro-expanding %s to %s..."),
			 Q_(givenFileName),
			 Q_(path));

  TexinfoPreprocess (m_pathOriginalInputFile, pathTmpFile1);

  bool expanded = RunMakeinfo(pathTmpFile1, pathTmpFile2);

  TexinfoUncomment (pathTmpFile2, path);

  File::Delete (pathTmpFile1);
  File::Delete (pathTmpFile2);

  if (expanded && File::Exists(path))
    {
      pathInputFile = path;
      pathInputFile.ToUnix ();
    }
  else
    {
      // If makeinfo failed (or was not even run), use the original
      // file as input.
      pApplication->Verbose (T_("Reverting to %s..."), Q_(givenFileName));
    }
}

/* _________________________________________________________________________

   Driver::InsertCommands

   Used most commonly for @finalout, @smallbook, etc.
   _________________________________________________________________________ */

void
Driver::InsertCommands ()
{
  if (! (pOptions->texinfoCommands.size() > 0
	 && macroLanguage == MacroLanguage::Texinfo))
    {
      return;
    }
  string extra = FlattenStringVector(pOptions->texinfoCommands, '\n');
  pApplication->Verbose (T_("Inserting extra commands: %s"), extra.c_str());
  PathName path (extraDirectory, inputName);
  StreamWriter writer (path);
  bool inserted = false;
  StreamReader reader (pathInputFile);
  string line;
  while (reader.ReadLine(line))
    {
      writer.WriteLine (line);
      if (! inserted && IsPrefixOf("@setfilename", line))
	{
	  writer.WriteLine (extra);
	  inserted = true;
	}
    }
  writer.Close ();
  reader.Close ();
  pathInputFile = path;
  pathInputFile.ToUnix ();
}

/* _________________________________________________________________________

   Driver::RunBibTeX

   Run bibtex on current file:
   - If its input (AUX) exists.
   - If AUX contains both '\bibdata' and '\bibstyle'.
   - If some citations are missing (LOG contains 'Citation') or the
     LOG complains of a missing .bbl.
    
   We run bibtex first, because I can see reasons for the indexes to
   change after bibtex is run, but I see no reason for the converse.
    
   Don't try to be too smart.  Running bibtex only if the bbl file
   exists and is older than the LaTeX file is wrong, since the
   document might include files that have changed.  Because there can
   be several AUX (if there are \include's), but a single LOG, looking
   for missing citations in LOG is easier, though we take the risk to
   match false messages.
   _________________________________________________________________________ */

void
Driver::RunBibTeX ()
{
  PathName pathExe;

  if (! pSession->FindFile(pOptions->bibtexProgram, FileType::EXE, pathExe))
    {
      FatalError (T_("%s could not be found."), Q_(pOptions->bibtexProgram));
    }

  PathName logName (0, inputNameNoExt, ".log");
  PathName auxName (0, inputNameNoExt, ".aux");

  int exitCode;

#if defined(SF464378__CHAPTERBIB)
  if ((File::Exists(auxName)
       && File::Exists(logName)
       && Contains(logName, &pOptions->regex_chapterbib)
       && (Contains(logName, &pOptions->regex_citation_undefined)
	   || Contains(logName, &pOptions->regex_no_file_bbl))))
    {
      pApplication->Verbose (T_("\
ChapterBib detected. Preparing to run BibTeX on first-level aux files..."));

      // read the main .aux file
      vector<char> auxFile = ReadFile(auxName);

      regmatch_t regMatch[2];

      for (int offset = 0;
	   ((regexec(&pOptions->regex_input_aux,
		     &auxFile[offset],
		     1,
		     regMatch,
		     offset != 0 ? REG_NOTBOL : 0)
	     == 0)
	    && (regMatch[0].rm_so > -1));
	   offset += regMatch[0].rm_eo)
        {
	  // get SubAuxNameNoExt out of \@input{SubAuxNameNoExt.aux}
	  string tmp (&auxFile[offset + regMatch[0].rm_so + 8],
		       regMatch[0].rm_eo - regMatch[0].rm_so - 13);
	  PathName subAuxNameNoExt (tmp);

	  // append .aux extension
	  PathName subAuxName (0, subAuxNameNoExt, ".aux");
          if (! (File::Exists(subAuxName)
		 && Contains(subAuxName, &pOptions->regex_bibdata)
		 && Contains(subAuxName, &pOptions->regex_bibstyle)))
	    {
	      continue;
	    }

	  PathName subDir;

	  if (strchr(subAuxNameNoExt.Get(), PathName::AltDirectoryDelimiter)
	      != 0)
	    {
	      // we have \@input{SubDir/SubAuxNameNoExt.aux}
	      if (pOptions->clean)
		{
		  FatalError (T_("\
Sub-directories not supported when --clean is in effect."));
		}
	      subDir = subAuxNameNoExt;
	      subDir.RemoveFileSpec ();
	      subDir.MakeAbsolute ();
	      subAuxNameNoExt.RemoveDirectorySpec ();
	    }
	  
	  CommandLineBuilder commandLine;
	  
	  commandLine.AppendArgument (subAuxNameNoExt);
	  
	  pApplication->Verbose (T_("Running %s %s..."),
				 Q_(pOptions->bibtexProgram.c_str()),
				 Q_(commandLine.Get()));
	  
	  exitCode = 0;
	  
	  ProcessOutputTrash trash;
	  
	  Process::Run (pathExe,
			commandLine.Get(),
			(pOptions->quiet
			 ? &trash
			 : 0),
			&exitCode,
			(subDir.GetLength() > 0
			 ? subDir.Get()
			 : 0));
	  
	  if (exitCode != 0)
	    {
	      FatalError (T_("%s failed for some reason."), Q_(pathExe));
	    }
        }
    }
#endif	// SF464378__CHAPTERBIB

  if (! (File::Exists(auxName)
	 && Contains(auxName, &pOptions->regex_bibdata)
	 && Contains(auxName, &pOptions->regex_bibstyle)
	 && File::Exists(logName)
	 && (Contains(logName, &pOptions->regex_citation_undefined)
	     || Contains(logName, &pOptions->regex_no_file_bbl))))
    {
      return;
    }

  CommandLineBuilder commandLine;

  commandLine.AppendArgument (inputNameNoExt);

  pApplication->Verbose (T_("Running %s %s..."),
			 Q_(pOptions->bibtexProgram),
			 commandLine.Get());

  ProcessOutputTrash trash;

  Process::Run (pathExe,
		commandLine.Get(),
		(pOptions->quiet ? &trash : 0),
		&exitCode,
		0);

  if (exitCode != 0)
    {
      FatalError (T_("%s failed for some reason."), Q_(pathExe));
    }
}

/* _________________________________________________________________________

   Driver::RunIndexGenerator

   Run texindex (or makeindex) on current index files.  If they
   already exist, and after running TeX a first time the index files
   don't change, then there's no reason to run TeX again.  But we
   won't know that if the index files are out of date or nonexistent.
   _________________________________________________________________________ */

void
Driver::RunIndexGenerator (/*[in]*/ const vector<string> & idxFiles)
{
  const char * lpszExeName =
    (macroLanguage == MacroLanguage::Texinfo
     ? pOptions->texindexProgram.c_str()
     : pOptions->makeindexProgram.c_str());

  PathName pathExe;

  if (! pSession->FindFile(lpszExeName, FileType::EXE, pathExe))
    {
      FatalError (T_("%s could not be found."), Q_(lpszExeName));
    }


  CommandLineBuilder commandLine;

  commandLine.AppendArguments (pOptions->makeindexOptions);
  commandLine.AppendArguments (idxFiles);

  ProcessOutputTrash trash;

  int exitCode = 0;

  pApplication->Verbose (T_("Running %s %s..."),
			 Q_(lpszExeName),
			 commandLine.Get());

  Process::Run (pathExe,
		commandLine.Get(),
		(pOptions->quiet ? &trash : 0),
		&exitCode,
		0);

  if (exitCode != 0)
    {
      FatalError (T_("%s failed for some reason."), Q_(lpszExeName));
    }
}

/* _________________________________________________________________________

   Driver::InstallProgram
   _________________________________________________________________________ */

void
Driver::InstallProgram (/*[in]*/ const char *	lpszProgram)
{
  ALWAYS_UNUSED (lpszProgram);
  PathName pathExe;
  if (! pSession->FindFile("initexmf", FileType::EXE, pathExe))
    {
      FatalError (T_("The MiKTeX configuration utility could not be found."));
    }
  ProcessOutputTrash trash;
  Process::Run (pathExe,
		"--mklinks",
		(pOptions->quiet ? &trash : 0));
}

/* _________________________________________________________________________

   Driver::GetTeXEnginePath
   _________________________________________________________________________ */

PathName
Driver::GetTeXEnginePath (/*[out]*/ string & exeName)
{
  if (macroLanguage == MacroLanguage::Texinfo)
    {
      if (pOptions->outputType == OutputType::PDF)
	{
	  if (pOptions->engine == Engine::XeTeX)
	    {
	      exeName = pOptions->xetexProgram;
	    }
	  else
	    {
	      exeName = pOptions->pdftexProgram;
	    }
	}
      else
	{
	  if (pOptions->engine == Engine::pdfTeX)
	    {
	      exeName = pOptions->pdftexProgram;
	    }
	  else
	    {
	      exeName = pOptions->texProgram;

	    }
	}
    }	  
  else
    {
      if (pOptions->outputType == OutputType::PDF)
	{
	  if (pOptions->engine == Engine::XeTeX)
	    {
	      exeName = pOptions->xelatexProgram;
	    }
	  else
	    {
	      exeName = pOptions->pdflatexProgram;
	    }
	}
      else
	{
	  exeName = pOptions->latexProgram;
	}
    }
  PathName pathExe;
  if (! pSession->FindFile(exeName.c_str(), FileType::EXE, pathExe))
    {
      InstallProgram (exeName.c_str());
      if (! pSession->FindFile(exeName.c_str(), FileType::EXE, pathExe))
	{
	  FatalError (T_("%s could not be found."), Q_(exeName));
	}
    }
  return (pathExe);
}

/* _________________________________________________________________________

   Driver::RunTeX
   _________________________________________________________________________ */

void
Driver::RunTeX ()
{
  string exeName;
  PathName pathExe = GetTeXEnginePath(exeName);
  
  CommandLineBuilder commandLine;

#if defined(SUPPORT_OPT_SRC_SPECIALS)
  if (pOptions->sourceSpecials)
    {
      if (pOptions->sourceSpecialsWhere.length() > 0)
	{
	  commandLine.AppendOption ("--src-specials=",
				    pOptions->sourceSpecialsWhere);
	}
      else
	{
	  commandLine.AppendOption ("--src-specials");
	}
    }
#endif
  if (pOptions->quiet)
    {
      commandLine.AppendOption ("--quiet");
    }
  if (pOptions->batch && ! pOptions->quiet)
    {
      commandLine.AppendOption ("--interaction=", "scrollmode");
    }
  commandLine.AppendArguments (pOptions->texOptions);
#if 0
  if (pOptions->traceStreams.length() > 0)
    {
      commandLine.AppendOption ("--trace=", pOptions->traceStreams);
    }
#endif
  commandLine.AppendArgument (pathInputFile);
	
  pApplication->Verbose (T_("Running %s %s..."),
			 Q_(exeName),
			 commandLine.Get());

  int exitCode = 0;
  Process::Run (pathExe, commandLine.Get(), 0, &exitCode, 0);
  if (exitCode != 0)
    {
      PathName logName (0, inputNameNoExt, ".log");
      if (pOptions->clean)
	{
	  try
	    {
	      File::Copy (logName,
			  PathName(pOptions->startDirectory, logName));
	    }
	  catch (const exception &)
	    {
	      FatalError (T_("%s failed for some reason."), Q_(exeName));
	    }
	}
      FatalError (T_("%s failed for some reason (see log file)."),
		  Q_(exeName));
    }
}

/* _________________________________________________________________________

   Driver::Ready

   Decide if looping again is needed.

   LaTeX (and the package changebar) report in the LOG file if it
   should be rerun.  This is needed for files included from subdirs,
   since texi2dvi does not try to compare xref files in subdirs.
   Performing xref files test is still good since LaTeX does not
   report changes in xref files.
   _________________________________________________________________________ */

bool
Driver::Ready ()
{
  PathName logName (0, inputNameNoExt, ".log");

  if (Contains(logName, "Rerun to get"))
    {
      return (false);
    }

  vector<string> auxFiles;

  GetAuxFiles (auxFiles);

  // If old and new lists don't at least have the same file list, then
  // one file or another has definitely changed.
  if (previousAuxFiles != auxFiles)
    {
      return (false);
    }

  // File list is the same.  We must compare each file until we find
  // a difference.
  for (vector<string>::iterator it = auxFiles.begin();
       it != auxFiles.end();
       ++ it)
    {
      PathName auxFile (auxDirectory, *it);
      pApplication->Verbose (T_("Comparing xref file %s..."), Q_(*it));
      // We only need to keep comparing until we find one that
      // differs, because we'll have to run texindex & tex again no
      // matter how many more there might be.
      if (! CompareFiles(*it, auxFile))
	{
	  pApplication->Verbose (T_("xref file %s differed..."), Q_(*it));
	  return (false);
	}
    }

  return (true);
}

/* _________________________________________________________________________

   Driver::InstallOutputFile
   _________________________________________________________________________ */

void
Driver::InstallOutputFile ()
{
  const char * lpszExt
    = (pOptions->outputType == OutputType::PDF ? ".pdf" : ".dvi");
  pApplication->Verbose (T_("Copying %s file from %s to %s..."),
			 lpszExt,
			 Q_(scratchDirectory),
			 Q_(pOptions->startDirectory));
  PathName pathFileName (0, inputNameNoExt, lpszExt);
  PathName pathDest (pOptions->startDirectory, pathFileName);
  File::Copy (pathFileName, pathDest);
}

/* _________________________________________________________________________

   Driver::GetAuxFiles
   _________________________________________________________________________ */

void
Driver::GetAuxFiles (/*[in]*/ const PathName &		baseName,
		      /*[in]*/ const char *	lpszExtension,
		      /*[in]*/ vector<string> &	vec)
{
  PathName pattern (0, baseName, lpszExtension);

  PathName curDir;
  curDir.SetToCurrentDirectory();

  pApplication->Trace (T_("collecting %s in %s..."), Q_(pattern), Q_(curDir));

  auto_ptr<DirectoryLister>
    pLister (DirectoryLister::Open(curDir, pattern.Get()));

  DirectoryEntry2 entry;

  while (pLister->GetNext(entry))
    {
      // If file is empty, skip it.
      if (entry.isDirectory || entry.size == 0)
	{
	  continue;
	}

      // If the file is not suitable to be an index or xref
      // file, don't process it.  The file can't be if its
      // first character is not a backslash or single quote.
      FileStream stream
	(File::Open(entry.name, FileMode::Open, FileAccess::Read));
      char buf[1];
      if (stream.Read(buf, 1) == 1
	  && (buf[0] == '\\'
	      || buf[0] == '\''))
	{
	  vec.push_back (entry.name);
	}
      stream.Close ();
    }

  pLister.reset ();
}

/* _________________________________________________________________________

   Driver::GetAuxFiles

   Get list of xref files (indexes, tables and lists).  Find all files
   having root filename with a two-letter extension, saves the ones
   that are really Texinfo-related files.  .?o? catches LaTeX tables
   and lists.
   _________________________________________________________________________ */

void
Driver::GetAuxFiles (/*[out]*/ vector<string> &		auxFiles,
		     /*[out]*/ vector<string> *		pIdxFiles)
{
  auxFiles.clear ();

  if (pIdxFiles != 0)
    {
      pIdxFiles->clear ();
    }

  GetAuxFiles (inputNameNoExt, ".?o?", auxFiles);
  GetAuxFiles (inputNameNoExt, ".aux", auxFiles);

  vector<string> files;

  GetAuxFiles (inputNameNoExt, ".??", files);

  auxFiles.insert (auxFiles.end(), files.begin(), files.end());

  if (pIdxFiles != 0)
    {
      pIdxFiles->insert (pIdxFiles->end(), files.begin(), files.end());
    }

  files.clear ();

  GetAuxFiles (inputNameNoExt, ".idx", files);

  auxFiles.insert (auxFiles.end(), files.begin(), files.end());

  if (pIdxFiles != 0)
    {
      pIdxFiles->insert (pIdxFiles->end(), files.begin(), files.end());
    }

  sort (auxFiles.begin(), auxFiles.end());
}

/* _________________________________________________________________________

   Driver::RunViewer
   _________________________________________________________________________ */

void
Driver::RunViewer ()
{
  const char * lpszExt
    = (pOptions->outputType == OutputType::PDF ? ".pdf" : ".dvi");

  PathName pathFileName (0, inputNameNoExt, lpszExt);

  PathName pathDest (pOptions->startDirectory, pathFileName);

  if (pOptions->viewerOptions.GetArgc() == 0)
    {
      pApplication->Verbose (T_("Opening %s..."), Q_(pathDest));
      if (ShellExecute(0,
		       "open",
		       pathDest.Get(),
		       0,
		       pOptions->startDirectory.Get(),
		       SW_SHOW)
	  <= reinterpret_cast<HINSTANCE>(32))
	{
	  FatalError (T_("The viewer could not be started."));
	}
    }
  else
    {
      char szExecutable[BufferSizes::MaxPath];
      if (FindExecutable(pathDest.Get(),
			 pOptions->startDirectory.Get(),
			 szExecutable)
	  <= reinterpret_cast<HINSTANCE>(32))
	{
	  FatalError (T_("The viewer could not be located."));
	}
      CommandLineBuilder commandLine;
      commandLine.AppendArguments (pOptions->viewerOptions);
      commandLine.AppendArgument (pathDest);
      pApplication->Verbose (T_("Running %s %s..."),
			     Q_(szExecutable),
			     commandLine.Get());
      Process::Start (szExecutable,
		      commandLine.Get(),
		      0,
		      0,
		      0,
		      0,
		      pOptions->startDirectory.Get());
    }
}

/* _________________________________________________________________________

   Driver::Run
   _________________________________________________________________________ */

void
Driver::Run ()
{
  ExpandMacros ();
  Application::CheckCancel ();
  InsertCommands ();

  SetIncludeDirectories ();

  // If clean mode was specified, then move to the temporary directory.
  if (pOptions->clean)
    {
      pApplication->Verbose ("cd %s", Q_(scratchDirectory));
      Directory::SetCurrentDirectory (scratchDirectory);
    }

  for (int i = 0; i < pOptions->maxIterations; ++ i)
    {
      Application::CheckCancel ();
      vector<string> idxFiles;
      GetAuxFiles (previousAuxFiles, &idxFiles);
      if (previousAuxFiles.size() > 0)
	{
	  pApplication->Verbose (T_("Backing up xref files: %s"),
				 (FlattenStringVector(previousAuxFiles,
						      ' ')
				  .c_str()));
	  CopyFiles (previousAuxFiles, auxDirectory);
	}
      RunBibTeX ();
      if (idxFiles.size() > 0)
	{
	  Application::CheckCancel ();
	  RunIndexGenerator (idxFiles);
	}
      Application::CheckCancel ();
      RunTeX ();
      if (Ready())
	{
	  break;
	}
    }

  // If we were in clean mode, compilation was in a tmp directory.
  // Copy the DVI (or PDF) file into the directory where the
  // compilation has been done.  (The temp dir is about to get removed
  // anyway.)  We also return to the original directory so that
  // - the next file is processed in correct conditions
  // - the temporary file can be removed
  if (pOptions->clean)
    {
      InstallOutputFile ();
    }

  if (pOptions->runViewer)
    {
      RunViewer ();
    }
}

/* _________________________________________________________________________

   CommandLineOptions
   _________________________________________________________________________ */

enum CommandLineOptions {
  OPT_AAA = 1000,
  OPT_AT,
  OPT_BATCH,
  OPT_CLEAN,
  OPT_DEBUG,
  OPT_ENGINE,
  OPT_EXPAND,
  OPT_INCLUDE,
  OPT_LANGUAGE,
  OPT_MAX_ITER,
  OPT_MKIDX_OPTION,
  OPT_PDF,
  OPT_QUIET,
  OPT_RUN_VIEWER,
#if defined(SUPPORT_OPT_SRC_SPECIALS)
  OPT_SRC,
  OPT_SRC_SPECIALS,
#endif
  OPT_TEXINFO,
  OPT_TEX_OPTION,
  OPT_TRACE,
  OPT_VERBOSE,
  OPT_VERSION,
  OPT_VIEWER_OPTION,
};

/* _________________________________________________________________________

   optionTable
   _________________________________________________________________________ */

const struct poptOption optionTable[] = {
  {
    "texiat", '@',
    POPT_ARG_NONE, 0,
    OPT_AT,
    T_("Use @input instead of \\input; for preloaded Texinfo."),
    0,
  },

  {
    "batch", 'b',
    POPT_ARG_NONE, 0,
    OPT_BATCH,
    T_("No interaction."),
    0,
  },

  {
    "clean", 'c',
    POPT_ARG_NONE, 0,
    OPT_CLEAN,
    T_("Remove all auxiliary files."),
    0,
  },

  {
    "debug", 'D',
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_DEBUG,
    T_("Print debug information."),
    0,
  },

  {
    "engine", 0,
    POPT_ARG_STRING, 0,
    OPT_ENGINE,
    T_("Use the specified TeX engine."),
    T_("ENGINE"),
  },

  {
    "expand", 'e',
    POPT_ARG_NONE, 0,
    OPT_EXPAND,
    T_("Force macro expansion using makeinfo."),
    0,
  },

  {
    "include-directory", 'I',
    POPT_ARG_STRING, 0,
    OPT_INCLUDE,
    T_("Prepend DIR to the input search path."),
    T_("DIR"),
  },

  {
    "language", 'l',
    POPT_ARG_STRING, 0,
    OPT_LANGUAGE,
    T_("Specify the LANG of FILE: LaTeX or Texinfo."),
    T_("LANG"),
  },

  {
    "pdf", 'p',
    POPT_ARG_NONE, 0,
    OPT_PDF,
    T_("Use pdftex or pdflatex for processing."),
    0,
  },

  {
    "quiet", 'q',
    POPT_ARG_NONE, 0,
    OPT_QUIET,
    T_("No output unless errors (implies --batch)."),
    0,
  },

  {
    "silent", 's',
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_QUIET,
    T_("No output unless errors (implies --batch)."),
    0,
  },

  {
    "texinfo", 't',
    POPT_ARG_STRING, 0,
    OPT_TEXINFO,
    T_("Insert CMD after @setfilename in copy of input file."),
    "CMD",
  },

  {
    "version", 'v',
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Display version information and exit successfully."),
    0,
  },

  {
    "verbose", 'V',
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Report on what is done."),
    0,
  },


  // --- now the MiKTeX extensions
  
  {
    "max-iterations", 0,
    POPT_ARG_STRING, 0,
    OPT_MAX_ITER,
    T_("Limit number of iterations."),
    "N",
  },

  {
    "mkidx-option", 0,
    POPT_ARG_STRING, 0,
    OPT_MKIDX_OPTION,
    T_("Pass OPTION to the index generator."),
    T_("OPTION"),
  },

#if defined(SUPPORT_OPT_SRC_SPECIALS)
  {
    "src", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SRC,
    T_("Pass option --src-specials to the TeX engine."),
    0,
  },

  {
    "src-specials", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_SRC_SPECIALS,
    T_("Pass option --src-specials[=SRCSPECIALS] to the TeX engine."),
    T_("SRCSPECIALS"),
  },
#endif

  {
    "tex-option", 0,
    POPT_ARG_STRING, 0,
    OPT_TEX_OPTION,
    T_("Pass OPTION to the TeX engine."),
    T_("OPTION"),
  },

  {
    "trace", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_TRACE,
    T_("\
Turn on tracing.\
  TRACESTREAMS, if specified, is a comma-separated list of trace stream names\
 (see the MiKTeX manual)."),
    T_("TRACESTREAMS"),
  },

  {
    "run-viewer", 0,
    POPT_ARG_NONE, 0,
    OPT_RUN_VIEWER,
    T_("Run a viewer on the resulting DVI/PDF file."),
    0,
  },

  {
    "viewer-option", 0,
    POPT_ARG_STRING, 0,
    OPT_VIEWER_OPTION,
    T_("Pass OPTION to the viewer."),
    T_("OPTION"),
  },


  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   McdApp::Run
   _________________________________________________________________________ */

void
McdApp::Run (/*[in]*/ int		argc,
	     /*[in]*/ const char **	argv)
{
  Session::InitInfo initInfo (argv[0]);

  Cpopt popt (argc, argv, optionTable);

  int option;

  while ((option = popt.GetNextOpt()) >= 0)
    {
      const char * lpszOptArg = popt.GetOptArg();
      switch (option)
	{
	case OPT_AT:
	  break;
	case OPT_BATCH:
	  options.batch = true;
	  break;
	case OPT_CLEAN:
	  options.clean = true;
	  break;
	case OPT_DEBUG:
	  options.traceStreams = DEFAULT_TRACE_STREAMS;
	  break;
	case OPT_ENGINE:
	  options.SetEngine (lpszOptArg);
	  break;
	case OPT_EXPAND:
	  options.expand = true;
	  break;
	case OPT_INCLUDE:
	  {
	    PathName path;
	    if (Utils::IsAbsolutePath(lpszOptArg))
	      {
		path = lpszOptArg;
	      }
	    else
	      {
		path.Set (options.startDirectory, lpszOptArg);
	      }
	    path.ToUnix ();
	    options.includeDirectories.push_back (path.Get());
	    break;
	  }
	case OPT_LANGUAGE:
	  if (_stricmp(lpszOptArg, "latex") == 0)
	    {
	      options.macroLanguage = MacroLanguage::LaTeX;
	    }
	  else if (_stricmp(lpszOptArg, "texinfo") == 0)
	    {
	      options.macroLanguage = MacroLanguage::Texinfo;
	    }
	  else
	    {
	      FatalError (T_("%s: unknown language"), lpszOptArg);
	    }
	  break;
	case OPT_PDF:
	  options.outputType = OutputType::PDF;
	  break;
	case OPT_QUIET:
	  options.quiet = true;
	  options.batch = true;
	  break;
	case OPT_TEXINFO:
	  options.texinfoCommands.push_back (lpszOptArg);
	  break;
	case OPT_VERSION:
	  Version ();
	  return;
	case OPT_VERBOSE:
	  options.verbose = true;
	  break;
#if defined(SUPPORT_OPT_SRC_SPECIALS)
	case OPT_SRC:
	  options.sourceSpecialsWhere = "";
	  options.sourceSpecials = true;
	  break;
	case OPT_SRC_SPECIALS:
	  if (lpszOptArg != 0)
	    {
	      options.sourceSpecialsWhere = lpszOptArg;
	    }
	  options.sourceSpecials = true;
	  break;
#endif
	case OPT_MKIDX_OPTION:
	  options.makeindexOptions.Append (lpszOptArg);
	  break;
	case OPT_TEX_OPTION:
	  options.texOptions.Append (lpszOptArg);
	  break;
	case OPT_VIEWER_OPTION:
	  options.viewerOptions.Append (lpszOptArg);
	  break;
	case OPT_RUN_VIEWER:
	  options.runViewer = true;
	  break;
	case OPT_MAX_ITER:
	  options.maxIterations = atoi(lpszOptArg);
	  break;
	case OPT_TRACE:
	  if (lpszOptArg == 0)
	    {
	      options.traceStreams = DEFAULT_TRACE_STREAMS;
	    }
	  else
	    {
	      options.traceStreams = lpszOptArg;
	    }
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
      
  int argCount = popt.GetArgCount();

  if (argCount == 0)
    {
      FatalError (T_("Missing file argument."));
    }

  const char ** leftovers = popt.GetArgs();

  if (options.traceStreams.length() > 0)
    {
      initInfo.SetTraceFlags (options.traceStreams.c_str());
    }


  for (int idx = 0; idx < argCount; ++ idx)
    {
      Init (initInfo);

      Verbose (T_("Processing %s..."), Q_(leftovers[idx]));
      
      // See if the file exists.  If it doesn't we're in trouble since,
      // even though the user may be able to reenter a valid filename at
      // the tex prompt (assuming they're attending the terminal), this
      // script won't be able to find the right xref files and so forth.
      if (! File::Exists(leftovers[idx]))
	{
	  FatalError (T_("The input file could not be found."));
	}

      Driver driver;
      driver.Initialize (this, &options, leftovers[idx]);
      driver.Run ();

      Finalize ();
    }
}

/* _________________________________________________________________________

   mcdmain
   _________________________________________________________________________ */

extern "C"
__declspec(dllexport)
int
__cdecl
mcdmain (/*[in]*/ int		argc,
	 /*[in]*/ const char **	argv)
{
  try
    {
      McdApp app;
      app.Run (argc, argv);
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
