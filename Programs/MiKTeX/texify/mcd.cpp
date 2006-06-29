/* mcd.cpp: MiKTeX compiler driver

   Copyright (C) 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2001,
   2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   Copyright (C) 1998-2006 Christian Schenk

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
#include <vector>

#include <miktex/app.h>
#include <miktex/core.h>
#include <miktex/trace.h>
#include <miktex/env.h>
#include <popt-miktex.h>
#include <regex.h>

#include "mcd-version.h"

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace std;

tstring progName;

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

#define PROGRAM_NAME T_("texify")

#if ! defined(THE_NAME_OF_THE_GAME)
#  define THE_NAME_OF_THE_GAME T_("MiKTeX Compiler Driver")
#endif

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

#define T_(x) MIKTEXTEXT(x)
#define Q_(x) Quoted(x).c_str()

const MIKTEXCHAR * DEFAULT_TRACE_STREAMS =
  MIKTEX_TRACE_ERROR T_(",")
  MIKTEX_TRACE_PROCESS T_(",")
  PROGRAM_NAME;

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
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

   FatalError
   _________________________________________________________________________ */

MIKTEXNORETURN
void
FatalError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << PROGRAM_NAME << T_(": ")
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
  MIKTEXCALL
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
  MIKTEXCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n)
  {
    output.append (reinterpret_cast<const char*>(pOutput), n);
    return (true);
  }
public:
  const tstring &
  GetOutput ()
    const
  {
    return (output);
  }
private:
  tstring output;
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
CopyFiles (/*[in]*/ const vector<tstring> &	vec,
	   /*[in]*/ const PathName &		destDir)
{
  for (vector<tstring>::const_iterator it = vec.begin();
       it != vec.end();
       ++ it)
    {
      File::Copy (*it, PathName(destDir, *it));
    }
}

/* _________________________________________________________________________

   ReadFile
   _________________________________________________________________________ */

vector<MIKTEXCHAR>
ReadFile (/*[in]*/ const PathName &	fileName)
{
  size_t fileSize = File::GetSize(fileName);
  vector<MIKTEXCHAR> vec (fileSize + 1);
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
  vector<MIKTEXCHAR> file = ReadFile(fileName);
  return (regexec(preg, &file[0], 0, 0, 0) == 0);
}

/* _________________________________________________________________________

   Contains
   _________________________________________________________________________ */

bool
Contains (/*[in]*/ const PathName &	fileName,
	  /*[in]*/ const MIKTEXCHAR *	lpszText)
{
  auto_ptr<MemoryMappedFile> pMappedFile (MemoryMappedFile::Create());
  const MIKTEXCHAR * ptr =
    reinterpret_cast<MIKTEXCHAR*>(pMappedFile->Open(fileName, false));
  size_t size = pMappedFile->GetSize();
  const MIKTEXCHAR * p = lpszText;
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

tstring
FlattenStringVector (/*[in]*/ const vector<tstring> &	vec,
		     /*[in]*/ MIKTEXCHAR		sep)
{
  tstring str = T_("");
  for (vector<tstring>::const_iterator it = vec.begin();
       it != vec.end();
       ++ it)
    {
      if (it != vec.begin())
	{
	  str += sep;
	}
      bool mustQuote = (it->find(sep) != tstring::npos);
      if (mustQuote)
	{
	  str += T_('"');
	}
      str += *it;
      if (mustQuote)
	{
	  str += T_('"');
	}
    }
  return (str);
}

/* _________________________________________________________________________

   IsPrefixOf
   _________________________________________________________________________ */

bool
IsPrefixOf (/*[in]*/ const MIKTEXCHAR *	lpszPrefix,
	    /*[in]*/ const tstring &	str)
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
  vector<tstring> includeDirectories;

public:
  vector<tstring> texinfoCommands;

public:
  MacroLanguage macroLanguage;

public:
  tstring sourceSpecialsWhere;

public:
  tstring bibtexProgram;

public:
  tstring latexProgram;

public:
  tstring pdflatexProgram;

public:
  tstring makeindexProgram;

public:
  tstring makeinfoProgram;

public:
  tstring texProgram;

public:
  tstring pdftexProgram;

public:
  tstring texindexProgram;

public:
  vector<tstring> makeindexOptions;

public:
  vector<tstring> texOptions;

public:
  vector<tstring> viewerOptions;

public:
  tstring traceStreams;

private:
  void
  SetProgramName (/*[out]*/ tstring &		str,
		  /*[in]*/ const MIKTEXCHAR *	lpszEnvName,
		  /*[in]*/ const MIKTEXCHAR *	lpszDefault)
  {
    if (! Utils::GetEnvironmentString(lpszEnvName, str))
      {
	str = lpszDefault;
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
    macroLanguage (MacroLanguage::None)
{
  if (regcomp(&regex_bibdata,
	      T_("^\\\\bibdata"),
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }
  
  if (regcomp(&regex_bibstyle,
	      T_("^\\\\bibstyle"),
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }
  
  if (regcomp(&regex_citation_undefined,
	      T_("Warning:.*Citation.*undefined"),
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }
  
  if (regcomp(&regex_no_file_bbl,
	      T_("No file .*\\.bbl\\."),
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }

  if (regcomp(&regex_texinfo_version,
	      T_("\\[(.*)version (....)-(..)-(..)"),
	      REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }

#if defined(SF464378__CHAPTERBIB)
  if (regcomp(&regex_chapterbib,
	      T_("^Package: chapterbib"),
	      REG_NOSUB | REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }

  if (regcomp(&regex_input_aux,
	      T_("^\\\\@input\\{.*\\.aux\\}"),
	      REG_EXTENDED | REG_NEWLINE)
      != 0)
    {
      FatalError (T_("A regular expression could not be compiled."));
    }
#endif /* SF464378__CHAPTERBIB */

  startDirectory.SetToCurrentDirectory ();

  startDirectory.ToUnix ();
    
  SetProgramName (bibtexProgram, T_("BIBTEX"), T_("bibtex"));
  SetProgramName (latexProgram, T_("LATEX"), T_("latex"));
  SetProgramName (makeindexProgram, T_("MAKEINDEX"), T_("makeindex"));
  SetProgramName (makeinfoProgram, T_("MAKEINFO"), T_("makeinfo"));
  SetProgramName (pdflatexProgram, T_("PDFLATEX"), T_("pdflatex"));
  SetProgramName (pdftexProgram, T_("PDFTEX"), T_("pdftex"));
  SetProgramName (texProgram, T_("TEX"), T_("tex"));
  SetProgramName (texindexProgram, T_("TEXINDEX"), T_("texindex"));
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
  Run (/*[in]*/ int			argc,
       /*[in]*/ const MIKTEXCHAR **	argv);

public:
  void
  Trace (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	 /*[in]*/			...);

public:
  void
  Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
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
McdApp::Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		 /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  traceStream->WriteLine (PROGRAM_NAME,
			  Utils::FormatString(lpszFormat, arglist).c_str());
  if (options.verbose)
    {
      tcout << Utils::FormatString(lpszFormat, arglist)
	    << endl;
    }
  va_end (arglist);
}

/* _________________________________________________________________________

   McdApp::Trace
   _________________________________________________________________________ */

void
McdApp::Trace (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	       /*[in]*/				...)
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
  tcout << Utils::MakeProgramVersionString(THE_NAME_OF_THE_GAME,
					   VersionNumber(VER_FILEVERSION))
	    << T_("\n\
Copyright (C) 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2001,\n\
              2002, 2003, 2004, 2005 Free Software Foundation, Inc.\n\
Copyright (C) 1998-2006 Christian Schenk\n\
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
  Initialize (/*[in]*/ McdApp *			pApplication,
	      /*[in]*/ Options *		pOptions,
	      /*[in]*/ const MIKTEXCHAR *	lpszFileName);

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
  Set_MIKTEX_CWD ();

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
  void
  RunTeX ();

private:
  void
  RunIndexGenerator (/*[in]*/ const vector<tstring> & idxFiles);

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
  GetAuxFiles (/*[out]*/ vector<tstring> &	auxFiles,
	       /*[out]*/ vector<tstring> *	pIdxFiles = 0);

private:
  void
  GetAuxFiles (/*[in]*/ const PathName &	baseName,
	       /*[in]*/ const MIKTEXCHAR *	lpszExtension,
	       /*[out]*/ vector<tstring> &	auxFiles);

private:
  void
  InstallProgram (/*[in]*/ const MIKTEXCHAR * lpszProgram);

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
  vector<tstring> previousAuxFiles;

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
		    /*[in]*/ const MIKTEXCHAR *	lpszFileName)
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
  scratchDirectory.Set (tempDirectory, T_("_src"));
  Directory::Create (scratchDirectory);
  scratchDirectory.ToUnix ();
  pApplication->Trace (T_("scratch directory: %s"), Q_(scratchDirectory));

  // create extra directory
  extraDirectory.Set (tempDirectory, T_("_xtr"));
  Directory::Create (extraDirectory);
  extraDirectory.ToUnix ();
  pApplication->Trace (T_("extra directory: %s"), Q_(extraDirectory));

  // create aux directory
  auxDirectory.Set (tempDirectory, T_("_aux"));
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
  tstring firstLine;
  if (! reader.ReadLine(firstLine))
    {
      return (MacroLanguage::None);
    }
  reader.Close ();
  if (firstLine.find(T_("input texinfo")) != tstring::npos)
    {
      return (MacroLanguage::Texinfo);
    }
  if (fileName.HasExtension(T_(".dtx"))
      || fileName.HasExtension(T_(".tex"))
      || fileName.HasExtension(T_(".latex"))
      || fileName.HasExtension(T_(".ltx")))
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
  tstring line;
  while (reader.ReadLine(line))
    {
      if (IsPrefixOf(T_("@tex"), line))
	{
	  at_tex = true;
	}
      else if (IsPrefixOf(T_("@iftex"), line))
	{
	  at_iftex = true;
	}
      else if (IsPrefixOf(T_("@macro"), line))
	{
	  at_macro = true;
	}
      else if (IsPrefixOf(T_("@html"), line))
	{
	  at_html = true;
	}
      else if (IsPrefixOf(T_("@ifnottex"), line))
	{
	  at_ifnottex = true;
	}
      else if (IsPrefixOf(T_("@ifinfo"), line))
	{
	  at_ifinfo = true;
	}
      else if (IsPrefixOf(T_("@menu"), line))
	{
	  at_menu = true;
	}

      bool commentingOut = (at_tex || (at_iftex && ! at_macro));
      bool deleting = (at_html || at_ifhtml || at_ifnottex
		       || (at_ifinfo
			   && ! at_menu
			   && ! IsPrefixOf(T_("@node"), line)));

      if (commentingOut)
	{
	  writer.WriteLine (tstring(T_("@c texi2dvi")) + line);
	}
      else if (! deleting)
	{
	  writer.WriteLine (line);
	}

      if (IsPrefixOf(T_("@end tex"), line))
	{
	  at_tex = false;
	}
      else if (IsPrefixOf(T_("@end iftex"), line))
	{
	  at_iftex = false;
	}
      else if (IsPrefixOf(T_("@end macro"), line))
	{
	  at_macro = false;
	}
      else if (IsPrefixOf(T_("@end html"), line))
	{
	  at_html = false;
	}
      else if (IsPrefixOf(T_("@end ifnottex"), line))
	{
	  at_ifnottex = false;
	}
      else if (IsPrefixOf(T_("@end ifinfo"), line))
	{
	  at_ifinfo = false;
	}
      else if (IsPrefixOf(T_("@end menu"), line))
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
  tstring line;
  while (reader.ReadLine(line))
    {
      if (IsPrefixOf(T_("@c texi2dvi"), line))
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

   Driver::Set_MIKTEX_CWD

   Set MIKTEX_CWD.

   Source file might include additional sources.  Put `.' and
   directory where source file(s) reside in MIKTEX_CWD before anything
   else.  Include orig_pwd in case we are in clean mode, where we've
   cd'd to a temp directory.
   _________________________________________________________________________ */

void
Driver::Set_MIKTEX_CWD ()
{
  tstring MIKTEX_CWD;
  MIKTEX_CWD.reserve (256);
  MIKTEX_CWD += pOptions->startDirectory.Get();
  MIKTEX_CWD += T_(';');
  MIKTEX_CWD += originalInputDirectory.Get();
  for (vector<tstring>::iterator it = pOptions->includeDirectories.begin();
       it != pOptions->includeDirectories.end();
       ++ it)
    {
      MIKTEX_CWD += T_(';');
      MIKTEX_CWD += *it;
    }
  Utils::SetEnvironmentString (MIKTEX_ENV_CWD_LIST, MIKTEX_CWD.c_str());
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

  commandLine.AppendOption (T_("--footnote-style="), T_("end"));
  commandLine.AppendOption (T_("-I "), originalInputDirectory);

  for (vector<tstring>::iterator it = pOptions->includeDirectories.begin();
       it != pOptions->includeDirectories.end();
       ++ it)
    {
      commandLine.AppendOption (T_("-I "), *it);
    }

#if defined(MIKTEX_WINDOWS)
  commandLine.AppendOption (T_("-o "), T_("nul"));
#else
  commandLine.AppendOption (T_("-o "), T_("/dev/null"));
#endif

  commandLine.AppendOption (T_("--macro-expand="), pathTo);

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
const MIKTEXCHAR * txiprereq = T_("19990129");

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
  scratchDirectory.Enter (T_("mcd"));

  StreamWriter writer (T_("txiversion.tex"));
  writer.WriteLine (T_("\\input texinfo.tex @bye"));
  writer.Close ();

  int exitCode = 0;
  ProcessOutputSaver processOutput;
  if (! Process::Run(pathExe,
		     T_("txiversion.tex"),
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
	  tstring txiformat;
	  for (int i = regMatch[1].rm_so; i < regMatch[1].rm_eo; ++ i)
	    {
	      txiformat += processOutput.GetOutput()[i];
	    }
	  tstring version;
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
	  newer = (_ttoi(txiprereq) <= _ttoi(version.c_str()));
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
  tstring extra = FlattenStringVector(pOptions->texinfoCommands, T_('\n'));
  pApplication->Verbose (T_("Inserting extra commands: %s"), extra.c_str());
  PathName path (extraDirectory, inputName);
  StreamWriter writer (path);
  bool inserted = false;
  StreamReader reader (pathInputFile);
  tstring line;
  while (reader.ReadLine(line))
    {
      writer.WriteLine (line);
      if (! inserted && IsPrefixOf(T_("@setfilename"), line))
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

  PathName logName (0, inputNameNoExt, T_(".log"));
  PathName auxName (0, inputNameNoExt, T_(".aux"));

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
      vector<MIKTEXCHAR> auxFile = ReadFile(auxName);

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
	  tstring tmp (&auxFile[offset + regMatch[0].rm_so + 8],
		       regMatch[0].rm_eo - regMatch[0].rm_so - 13);
	  PathName subAuxNameNoExt (tmp);

	  // append .aux extension
	  PathName subAuxName (0, subAuxNameNoExt, T_(".aux"));
          if (! (File::Exists(subAuxName)
		 && Contains(subAuxName, &pOptions->regex_bibdata)
		 && Contains(subAuxName, &pOptions->regex_bibstyle)))
	    {
	      continue;
	    }

	  PathName subDir;

	  if (_tcschr(subAuxNameNoExt.Get(), PathName::AltDirectoryDelimiter)
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
Driver::RunIndexGenerator (/*[in]*/ const vector<tstring> & idxFiles)
{
  const MIKTEXCHAR * lpszExeName =
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
Driver::InstallProgram (/*[in]*/ const MIKTEXCHAR *	lpszProgram)
{
  ALWAYS_UNUSED (lpszProgram);
  PathName pathExe;
  if (! pSession->FindFile(T_("initexmf"), FileType::EXE, pathExe))
    {
      FatalError (T_("The MiKTeX configuration utility could not be found."));
    }
  ProcessOutputTrash trash;
  Process::Run (pathExe,
		T_("--mklinks"),
		(pOptions->quiet ? &trash : 0));
}

/* _________________________________________________________________________

   Driver::RunTeX
   _________________________________________________________________________ */

void
Driver::RunTeX ()
{
  const MIKTEXCHAR * lpszExeName =
    (macroLanguage == MacroLanguage::Texinfo
     ? (pOptions->outputType == OutputType::PDF
	? pOptions->pdftexProgram.c_str()
	: pOptions->texProgram.c_str())
     : (pOptions->outputType == OutputType::PDF
	? pOptions->pdflatexProgram.c_str()
	: pOptions->latexProgram.c_str()));
  
  PathName pathExe;
  if (! pSession->FindFile(lpszExeName, FileType::EXE, pathExe))
    {
      InstallProgram (lpszExeName);
      if (! pSession->FindFile(lpszExeName, FileType::EXE, pathExe))
	{
	  FatalError (T_("%s could not be found."), Q_(lpszExeName));
	}
    }
  
  CommandLineBuilder commandLine;

#if defined(SUPPORT_OPT_SRC_SPECIALS)
  if (pOptions->sourceSpecials)
    {
      if (pOptions->sourceSpecialsWhere.length() > 0)
	{
	  commandLine.AppendOption (T_("--src-specials="),
				    pOptions->sourceSpecialsWhere);
	}
      else
	{
	  commandLine.AppendOption (T_("--src-specials"));
	}
    }
#endif
  if (pOptions->quiet)
    {
      commandLine.AppendOption (T_("--quiet"));
    }
  if (pOptions->batch && ! pOptions->quiet)
    {
      commandLine.AppendOption (T_("--interaction="), T_("scrollmode"));
    }
  commandLine.AppendArguments (pOptions->texOptions);
#if 0
  if (pOptions->traceStreams.length() > 0)
    {
      commandLine.AppendOption (T_("--trace="), pOptions->traceStreams);
    }
#endif
  commandLine.AppendArgument (pathInputFile);
	
  pApplication->Verbose (T_("Running %s %s..."),
			 Q_(lpszExeName),
			 commandLine.Get());

  int exitCode = 0;
  Process::Run (pathExe, commandLine.Get(), 0, &exitCode, 0);
  if (exitCode != 0)
    {
      PathName logName (0, inputNameNoExt, T_(".log"));
      if (pOptions->clean)
	{
	  try
	    {
	      File::Copy (logName,
			  PathName(pOptions->startDirectory, logName));
	    }
	  catch (const MiKTeXException &)
	    {
	      FatalError (T_("%s failed for some reason."), Q_(lpszExeName));
	    }
	}
      FatalError (T_("%s failed for some reason (see log file)."),
		  Q_(lpszExeName));
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
  PathName logName (0, inputNameNoExt, T_(".log"));

  if (Contains(logName, T_("Rerun to get")))
    {
      return (false);
    }

  vector<tstring> auxFiles;

  GetAuxFiles (auxFiles);

  // If old and new lists don't at least have the same file list, then
  // one file or another has definitely changed.
  if (previousAuxFiles != auxFiles)
    {
      return (false);
    }

  // File list is the same.  We must compare each file until we find
  // a difference.
  for (vector<tstring>::iterator it = auxFiles.begin();
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
  const MIKTEXCHAR * lpszExt
    = (pOptions->outputType == OutputType::PDF ? T_(".pdf") : T_(".dvi"));
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
		      /*[in]*/ const MIKTEXCHAR *	lpszExtension,
		      /*[in]*/ vector<tstring> &	vec)
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
      MIKTEXCHAR buf[1];
      if (stream.Read(buf, 1) == 1
	  && (buf[0] == T_('\\')
	      || buf[0] == T_('\'')))
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
Driver::GetAuxFiles (/*[out]*/ vector<tstring> &		auxFiles,
		     /*[out]*/ vector<tstring> *		pIdxFiles)
{
  auxFiles.clear ();

  if (pIdxFiles != 0)
    {
      pIdxFiles->clear ();
    }

  GetAuxFiles (inputNameNoExt, T_(".?o?"), auxFiles);
  GetAuxFiles (inputNameNoExt, T_(".aux"), auxFiles);

  vector<tstring> files;

  GetAuxFiles (inputNameNoExt, T_(".??"), files);

  auxFiles.insert (auxFiles.end(), files.begin(), files.end());

  if (pIdxFiles != 0)
    {
      pIdxFiles->insert (pIdxFiles->end(), files.begin(), files.end());
    }

  files.clear ();

  GetAuxFiles (inputNameNoExt, T_(".idx"), files);

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
  const MIKTEXCHAR * lpszExt
    = (pOptions->outputType == OutputType::PDF ? T_(".pdf") : T_(".dvi"));

  PathName pathFileName (0, inputNameNoExt, lpszExt);

  PathName pathDest (pOptions->startDirectory, pathFileName);

  if (pOptions->viewerOptions.size() == 0)
    {
      pApplication->Verbose (T_("Opening %s..."), Q_(pathDest));
      if (ShellExecute(0,
		       T_("open"),
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
      MIKTEXCHAR szExecutable[BufferSizes::MaxPath];
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

  Set_MIKTEX_CWD ();

  // If clean mode was specified, then move to the temporary directory.
  if (pOptions->clean)
    {
      pApplication->Verbose (T_("cd %s"), Q_(scratchDirectory));
      Directory::SetCurrentDirectory (scratchDirectory);
    }

  for (int i = 0; i < pOptions->maxIterations; ++ i)
    {
      Application::CheckCancel ();
      vector<tstring> idxFiles;
      GetAuxFiles (previousAuxFiles, &idxFiles);
      if (previousAuxFiles.size() > 0)
	{
	  pApplication->Verbose (T_("Backing up xref files: %s"),
				 (FlattenStringVector(previousAuxFiles,
						      T_(' '))
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
    T_("texiat"), T_('@'),
    POPT_ARG_NONE, 0,
    OPT_AT,
    T_("Use @input instead of \\input; for preloaded Texinfo."),
    0,
  },

  {
    T_("batch"), T_('b'),
    POPT_ARG_NONE, 0,
    OPT_BATCH,
    T_("No interaction."),
    0,
  },

  {
    T_("clean"), T_('c'),
    POPT_ARG_NONE, 0,
    OPT_CLEAN,
    T_("Remove all auxiliary files."),
    0,
  },

  {
    T_("debug"), T_('D'),
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_DEBUG,
    T_("Print debug information."),
    0,
  },

  {
    T_("expand"), T_('e'),
    POPT_ARG_NONE, 0,
    OPT_EXPAND,
    T_("Force macro expansion using makeinfo."),
    0,
  },

  {
    T_("include-directory"), T_('I'),
    POPT_ARG_STRING, 0,
    OPT_INCLUDE,
    T_("Prepend DIR to the input search path."),
    T_("DIR"),
  },

  {
    T_("language"), T_('l'),
    POPT_ARG_STRING, 0,
    OPT_LANGUAGE,
    T_("Specify the LANG of FILE: LaTeX or Texinfo."),
    T_("LANG"),
  },

  {
    T_("pdf"), T_('p'),
    POPT_ARG_NONE, 0,
    OPT_PDF,
    T_("Use pdftex or pdflatex for processing."),
    0,
  },

  {
    T_("quiet"), T_('q'),
    POPT_ARG_NONE, 0,
    OPT_QUIET,
    T_("No output unless errors (implies --batch)."),
    0,
  },

  {
    T_("silent"), T_('s'),
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_QUIET,
    T_("No output unless errors (implies --batch)."),
    0,
  },

  {
    T_("texinfo"), T_('t'),
    POPT_ARG_STRING, 0,
    OPT_TEXINFO,
    T_("Insert CMD after @setfilename in copy of input file."),
    T_("CMD"),
  },

  {
    T_("version"), T_('v'),
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Display version information and exit successfully."),
    0,
  },

  {
    T_("verbose"), T_('V'),
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Report on what is done."),
    0,
  },


  // --- now the MiKTeX extensions
  
  {
    T_("max-iterations"), 0,
    POPT_ARG_STRING, 0,
    OPT_MAX_ITER,
    T_("Limit number of iterations."),
    T_("N"),
  },

  {
    T_("mkidx-option"), 0,
    POPT_ARG_STRING, 0,
    OPT_MKIDX_OPTION,
    T_("Pass OPTION to the index generator."),
    T_("OPTION"),
  },

#if defined(SUPPORT_OPT_SRC_SPECIALS)
  {
    T_("src"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SRC,
    T_("Pass option --src-specials to the TeX compiler."),
    0,
  },

  {
    T_("src-specials"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_SRC_SPECIALS,
    T_("Pass option --src-specials[=SRCSPECIALS] to the TeX compiler."),
    T_("SRCSPECIALS"),
  },
#endif

  {
    T_("tex-option"), 0,
    POPT_ARG_STRING, 0,
    OPT_TEX_OPTION,
    T_("Pass OPTION to (La)TeX."),
    T_("OPTION"),
  },

  {
    T_("trace"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_TRACE,
    T_("\
Turn on tracing.\
  TRACESTREAMS, if specified, is a comma-separated list of trace stream names\
 (see the MiKTeX manual)."),
    T_("TRACESTREAMS"),
  },

  {
    T_("run-viewer"), 0,
    POPT_ARG_NONE, 0,
    OPT_RUN_VIEWER,
    T_("Run a viewer on the resulting DVI/PDF file."),
    0,
  },

  {
    T_("viewer-option"), 0,
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
McdApp::Run (/*[in]*/ int			argc,
	     /*[in]*/ const MIKTEXCHAR **	argv)
{
  Session::InitInfo initInfo (argv[0]);

  progName = Utils::GetExeName();

  Cpopt popt (argc, argv, optionTable);

  int option;

  while ((option = popt.GetNextOpt()) >= 0)
    {
      const MIKTEXCHAR * lpszOptArg = popt.GetOptArg();
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
	  if (_tcsicmp(lpszOptArg, T_("latex")) == 0)
	    {
	      options.macroLanguage = MacroLanguage::LaTeX;
	    }
	  else if (_tcsicmp(lpszOptArg, T_("texinfo")) == 0)
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
	  options.sourceSpecialsWhere = T_("");
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
	  options.makeindexOptions.push_back (lpszOptArg);
	  break;
	case OPT_TEX_OPTION:
	  options.texOptions.push_back (lpszOptArg);
	  break;
	case OPT_VIEWER_OPTION:
	  options.viewerOptions.push_back (lpszOptArg);
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
      tstring msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += T_(": ");
      msg += popt.Strerror(option);
      FatalError (T_("%s"), msg.c_str());
    }
      
  int argCount = popt.GetArgCount();

  if (argCount == 0)
    {
      FatalError (T_("Missing file argument."));
    }

  const MIKTEXCHAR ** leftovers = popt.GetArgs();

  if (options.traceStreams.length() > 0)
    {
      initInfo.SetTraceFlags (options.traceStreams.c_str());
    }

  Init (initInfo);

  for (int i = 0; i < argCount; ++ i)
    {
      Verbose (T_("Processing %s..."), Q_(leftovers[i]));
      
      // See if the file exists.  If it doesn't we're in trouble since,
      // even though the user may be able to reenter a valid filename at
      // the tex prompt (assuming they're attending the terminal), this
      // script won't be able to find the right xref files and so forth.
      if (! File::Exists(leftovers[i]))
	{
	  FatalError (T_("The input file could not be found."));
	}

      Driver driver;
      driver.Initialize (this, &options, leftovers[i]);
      driver.Run ();
    }

  Finalize ();
}

/* _________________________________________________________________________

   mcdmain
   _________________________________________________________________________ */

extern "C"
int
__cdecl
mcdmain (/*[in]*/ int			argc,
	 /*[in]*/ const MIKTEXCHAR **	argv)
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
