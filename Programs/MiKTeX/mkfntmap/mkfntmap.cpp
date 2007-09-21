/* mkfntmap.cpp:

   Copyright (C) 2002-2007 Christian Schenk

   This file is part of MkFntMap.

   MkFntMap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MkFntMap is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MkFntMap; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

/* This program is based on the updmap shell script written by Thomas
   Esser. */

#include "mkfntmap-version.h"
   
#include <miktex/core.h>
#include <miktex/paths.h>
#include <popt-miktex.h>

#include <cstdarg>

#pragma warning (push, 1)
#pragma warning (disable: 4702)
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <map>
#pragma warning (pop)

#include "internal.h"

using namespace MiKTeX::Core;
using namespace std;

#define PROGRAM_NAME T_("mkfntmap")

#if ! defined(THE_NAME_OF_THE_GAME)
#  define THE_NAME_OF_THE_GAME T_("MiKTeX Fontmap Maintenance Utility")
#endif

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

#if MIKTEX_SERIES_INT < 207
#  define CREATE_DEPRECATED_MAP_FILES 1
#else
#  define CREATE_DEPRECATED_MAP_FILES 0
#endif

/* _________________________________________________________________________

   aoption
   _________________________________________________________________________ */

enum Option
{
  OPT_OUTPUT_DIRECTORY,
  OPT_VERBOSE,
  OPT_VERSION,
};

const struct poptOption aoption[] = {
  {
    T_("output-directory"), 0,
    POPT_ARG_STRING, 0,
    OPT_OUTPUT_DIRECTORY,
    T_("Set the output directory."),
    T_("DIR")
  },

  {
    T_("verbose"), 0,
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Turn on verbose mode."),
    0
  },

  {
    T_("version"), 0,
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   FileContext
   _________________________________________________________________________ */

struct FileContext
{
  FileContext ()
    : line(0)
  {
  }
  PathName path;
  int line;
};

/* _________________________________________________________________________

   MakeFontMapApp
   _________________________________________________________________________ */

class MakeFontMapApp
{
public:
  MakeFontMapApp ()
    : dvipdfmDownloadBase14 (true),
      dvipsDownloadBase35 (false),
      dvipsPreferOutline (true),
      pdftexDownloadBase14 (true),
      verbose (false),
      namingConvention (URWkb)
  {
  }

private:
  void
  ShowVersion ();

private:
  void
  ProcessOptions (/*[in]*/ int			argc,
		  /*[in]*/ const MIKTEXCHAR **	argv);

private:
  void
  Initialize ();

private:
  bool
  ToBool (/*[in]*/ const tstring & param);

private:
  bool
  ScanConfigLine (/*[in]*/ const tstring &	line,
		  /*[out]*/ tstring &		directive,
		  /*[out]*/ tstring &		param);

private:
  void
  ParseConfigFile (/*[in]*/ const PathName & path);

public:
  void
  Run (/*[in]*/ int			argc,
       /*[in]*/ const MIKTEXCHAR **	argv);

private:
  bool
  LocateMap (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	     /*[out]*/ PathName &		path,
	     /*[in]*/ bool			mustExist);

private:
  void
  ReadMap (/*[in]*/ const tstring &		fileName,
	   /*[in,out]*/ set<FontMapEntry> &	fontMapEntries,
	   /*[in]*/ bool			mustExist);

private:
  void
  WriteHeader (/*[in]*/ StreamWriter &		writer,
	       /*[in]*/ const PathName &	fileName);

private:
  PathName
  CreateOutputDir (/*[in]*/ const MIKTEXCHAR * lpszRelPath);

private:
  PathName
  GetDvipsOutputDir ()
  {
    return (CreateOutputDir(MIKTEX_PATH_DVIPS_CONFIG_DIR));
  }

private:
  PathName
  GetDvipdfmOutputDir ()
  {
    return (CreateOutputDir(MIKTEX_PATH_DVIPDFM_CONFIG_DIR));
  }

private:
  PathName
  GetPdfTeXOutputDir ()
  {
    return (CreateOutputDir(MIKTEX_PATH_PDFTEX_CONFIG_DIR));
  }

private:
  void
  WriteMap (/*[in]*/ StreamWriter &		writer,
	    /*[in]*/ const set<FontMapEntry> &	set1);

private:
  void
  WriteDvipdfmMap (/*[in]*/ StreamWriter &		writer,
		   /*[in]*/ const set<FontMapEntry> &	set1);
  
private:
  bool
  GetInstructionParam (/*[in]*/ const tstring &	str,
		       /*[in]*/ const tstring &	instruction,
		       /*[out]*/ tstring &	param);

private:
  void
  WriteDvipsMapFile (/*[in]*/ const PathName &		fileName,
		     /*[in]*/ const set<FontMapEntry> &	set1,
		     /*[in]*/ const set<FontMapEntry> &	set2,
		     /*[in]*/ const set<FontMapEntry> &	set3);

private:
  void
  WritePdfTeXMapFile (/*[in]*/ const PathName &			fileName,
		      /*[in]*/ const set<FontMapEntry> &	set1,
		      /*[in]*/ const set<FontMapEntry> &	set2,
		      /*[in]*/ const set<FontMapEntry> &	set3);

private:
  void
  WriteDvipdfmMapFile (/*[in]*/ const PathName &		fileName,
		       /*[in]*/ const set<FontMapEntry> &	set1,
		       /*[in]*/ const set<FontMapEntry> &	set2,
		       /*[in]*/ const set<FontMapEntry> &	set3);
 
private:
  set<FontMapEntry>
  CatMaps (/*[in]*/ const set<tstring> & fileNames);

private:
  set<FontMapEntry>
  TranslateLW35 (/*[in]*/ const set<FontMapEntry> &	set1);

private:
  void
  TranslateFontFile (/*[in]*/ const map<tstring, tstring> &	transMap,
		     /*[in,out]*/ FontMapEntry &		fontMapEntry);

private:
  void
  TranslatePSName (/*[in]*/ const map<tstring, tstring> & files,
		   /*[in,out]*/ FontMapEntry &		fontMapEntry);

private:
  void
  CopyFile (/*[in]*/ const PathName & pathSrc,
	    /*[in]*/ const PathName & pathDest);

private:
  void
  CopyFiles ();

private:
  void
  BuildFontconfigCache ();

private:
  void
  MakeMaps ();

private:
  void
  Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	   /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  Abort (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	 /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  CfgError (/*[in]*/ const MIKTEXCHAR *	lpszMessage,
	    /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  MapError (/*[in]*/ const MIKTEXCHAR *	lpszMessage,
	    /*[in]*/			...);

private:
  void
  ParseDvipsMapFile (/*[in]*/ const PathName &		mapFile,
		     /*[int,out]*/ set<FontMapEntry> &	fontMapEntries);


private:
  bool dvipdfmDownloadBase14;

private:
  bool dvipsDownloadBase35;

private:
  bool dvipsPreferOutline;

private:
  bool pdftexDownloadBase14;

private:
  enum NamingConvention {
    URW,
    URWkb,
    ADOBE,
    ADOBEkb
  };

private:
  NamingConvention namingConvention;

private:
  set<tstring> mapFiles;

private:
  set<tstring> mixedMapFiles;

private:
  bool verbose;

  // transform file names from URWkb (berry names) to URW (vendor
  // names)
private:
  static map<tstring, tstring> fileURW;

  // transform file names from URWkb (berry names) to ADOBE (vendor
  // names)
private:
  static map<tstring, tstring> fileADOBE;

  // transform file names from URW to ADOBE (both berry names)
private:
  static map<tstring, tstring> fileADOBEkb;

  // transform font names from URW to Adobe
private:
  static map<tstring, tstring> psADOBE;

private:
  tstring outputDirectory;

private:
  SessionWrapper pSession;

private:
  FileContext cfgContext;

private:
  FileContext mapContext;
};

/* _________________________________________________________________________

   Local Variables
   _________________________________________________________________________ */

map<tstring, tstring> MakeFontMapApp::fileURW;
map<tstring, tstring> MakeFontMapApp::fileADOBE;
map<tstring, tstring> MakeFontMapApp::fileADOBEkb;
map<tstring, tstring> MakeFontMapApp::psADOBE;

/* _________________________________________________________________________

   MakeFontMapApp::ShowVersion
   _________________________________________________________________________ */

void
MakeFontMapApp::ShowVersion ()
{
  tcout << Utils::MakeProgramVersionString(THE_NAME_OF_THE_GAME,
					   VersionNumber(VER_FILEVERSION))
	<< T_("\n\
Copyright (C) 2002-2007 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	<< endl;
}

/* _________________________________________________________________________

    MakeFontMapApp::ProcessOptions
   _________________________________________________________________________ */

void
MakeFontMapApp::ProcessOptions (/*[in]*/ int			argc,
				/*[in]*/ const MIKTEXCHAR **	argv)
{
  Cpopt popt (argc, argv, aoption);

  int option;
  
  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{
	case OPT_OUTPUT_DIRECTORY:
	  outputDirectory = popt.GetOptArg();
	  break;
	case OPT_VERBOSE:
	  verbose = true;
	  break;
	case OPT_VERSION:
	  ShowVersion ();
	  throw (0);
	  break;
	}
    }
  
  if (option != -1)
    {
      tstring msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += T_(": ");
      msg += popt.Strerror(option);
      Abort (T_("%s"), msg.c_str());
    }
      
  if (popt.GetArgs() != 0)
    {
      Abort (T_("This utility does not accept non-option arguments."));
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::Verbose
   _________________________________________________________________________ */

void
MakeFontMapApp::Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
			 /*[in]*/			...)
{
  if (! verbose)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcout << Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   MakeFontMapApp::CfgError
   _________________________________________________________________________ */

MIKTEXNORETURN
void
MakeFontMapApp::CfgError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
			  /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << cfgContext.path.Get() << T_(":")
	<< cfgContext.line << T_(": error: ")
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   MakeFontMapApp::MapError
   _________________________________________________________________________ */

MIKTEXNORETURN
void
MakeFontMapApp::MapError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
			  /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << mapContext.path.Get() << T_(":")
	<< mapContext.line << T_(": error: ")
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   MakeFontMapApp::Abort
   _________________________________________________________________________ */

MIKTEXNORETURN
void
MakeFontMapApp::Abort (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		       /*[in]*/				...)
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

   MakeFontMapApp::ToBool
   _________________________________________________________________________ */

bool
MakeFontMapApp::ToBool (/*[in]*/ const tstring & param)
{
  if (param.empty())
    {
      CfgError (T_("missing bool value"));
    }
  if (StringCompare(param.c_str(), BOOLSTR(false), true) == 0)
    {
      return (false);
    }
  else if (StringCompare(param.c_str(), BOOLSTR(true), true) == 0)
    {
      return (true);
    }
  else
    {
      CfgError (T_("invalid bool value"));
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::ScanConfigLine
   _________________________________________________________________________ */

bool
MakeFontMapApp::ScanConfigLine (/*[in]*/ const tstring &	line,
				/*[out]*/ tstring &		directive,
				/*[out]*/ tstring &		param)
{
  if (line.empty()
      || tstring(T_("*#;%")).find_first_of(line[0]) != tstring::npos)
    {
      return (false);
    }
  Tokenizer tok (line.c_str(), T_(" \t\n"));
  if (tok.GetCurrent() == 0)
    {
      return (false);
    }
  directive = tok.GetCurrent();
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      param = T_("");
    }
  else
    {
      param = tok.GetCurrent();
    }
  return (true);
}

/* _________________________________________________________________________

   MakeFontMapApp::ParseConfigFile
   _________________________________________________________________________ */

void
MakeFontMapApp::ParseConfigFile (/*[in]*/ const PathName & path)
{
  Verbose (T_("Parsing config file %s..."), Q_(path));
  StreamReader reader (path);
  cfgContext.path = path;
  cfgContext.line = 0;
  tstring line;
  while (reader.ReadLine(line))
    {
      ++ cfgContext.line;
      tstring directive;
      tstring param;
      if (! ScanConfigLine(line, directive, param))
	{
	  continue;
	}
      if (StringCompare(directive.c_str(), T_("dvipsPreferOutline"), true)
	  == 0)
	{
	  dvipsPreferOutline = ToBool(param);
	}
      else if (StringCompare(directive.c_str(), T_("LW35"), true) == 0)
	{
	  if (param.empty())
	    {
	      CfgError (T_("missing value"));
	    }
	  if (StringCompare(param.c_str(), T_("URW"), true) == 0)
	    {
	      namingConvention = URW;
	    }
	  else if (StringCompare(param.c_str(), T_("URWkb"), true) == 0)
	    {
	      namingConvention = URWkb;
	    }
	  else if (StringCompare(param.c_str(), T_("ADOBE"), true) == 0)
	    {
	      namingConvention = ADOBE;
	    }
	  else if (StringCompare(param.c_str(), T_("ADOBEkb"), true) == 0)
	    {
	      namingConvention = ADOBEkb;
	    }
	  else
	    {
	      CfgError (T_("invalid value"));
	    }
	}
      else if (StringCompare(directive.c_str(),
			     T_("dvipsDownloadBase35"),
			     true)
	       == 0)
	{
	  dvipsDownloadBase35 = ToBool(param);
	}
      else if (StringCompare(directive.c_str(),
			     T_("pdftexDownloadBase14"),
			     true)
	       == 0)
	{
	  pdftexDownloadBase14 = ToBool(param);
	}
      else if (StringCompare(directive.c_str(),
			     T_("dvipdfmDownloadBase14"),
			     true)
	       == 0)
	{
	  dvipdfmDownloadBase14 = ToBool(param);
	}
      else if (StringCompare(directive.c_str(), T_("Map"), true) == 0)
	{
	  if (param.empty())
	    {
	      CfgError (T_("missing map file name"));
	    }
	  mapFiles.insert (param);
	}
      else if (StringCompare(directive.c_str(), T_("MixedMap"), true) == 0)
	{
	  if (param.empty())
	    {
	      CfgError (T_("missing map file name"));
	    }
	  mixedMapFiles.insert (param);
	}
      else
	{
	  CfgError (T_("invalid configuration setting"));
	}
    }
  reader.Close ();
}

/* _________________________________________________________________________

   MakeFontMapApp::Initialize
   _________________________________________________________________________ */

void
MakeFontMapApp::Initialize ()
{
  bool parsedConfig = false;

  for (unsigned i = pSession->GetNumberOfTEXMFRoots(); i > 0; -- i)
    {
      static const MIKTEXCHAR * lpszConfigFiles[] = {
	MIKTEX_PATH_MKFNTMAP_CFG,
	(MIKTEX_PATH_MIKTEX_CONFIG_DIR
	 MIKTEX_PATH_DIRECTORY_DELIMITER_STRING
	 T_("updmap.cfg")),
	(MIKTEX_PATH_WEB2C_DIR
	 MIKTEX_PATH_DIRECTORY_DELIMITER_STRING
	 T_("updmap.cfg"))
      };

      for (size_t j = sizeof(lpszConfigFiles) / sizeof(lpszConfigFiles[0]);
	   j > 0;
	   -- j)
	{
	  PathName cfgFile (pSession->GetRootDirectory(i - 1),
			    lpszConfigFiles[j - 1]);
	  if (File::Exists(cfgFile))
	    {
	      ParseConfigFile (cfgFile);
	      parsedConfig = true;
	    }
	}
    }

  if (! parsedConfig)
    {
      Abort (T_("The config file could not be found."));
    }

  fileURW[T_("uagd8a.pfb")] = T_("a010015l.pfb");
  fileURW[T_("uagdo8a.pfb")] = T_("a010035l.pfb");
  fileURW[T_("uagk8a.pfb")] = T_("a010013l.pfb");
  fileURW[T_("uagko8a.pfb")] = T_("a010033l.pfb");
  fileURW[T_("ubkd8a.pfb")] = T_("b018015l.pfb");
  fileURW[T_("ubkdi8a.pfb")] = T_("b018035l.pfb");
  fileURW[T_("ubkl8a.pfb")] = T_("b018012l.pfb");
  fileURW[T_("ubkli8a.pfb")] = T_("b018032l.pfb");
  fileURW[T_("ucrb8a.pfb")] = T_("n022004l.pfb");
  fileURW[T_("ucrbo8a.pfb")] = T_("n022024l.pfb");
  fileURW[T_("ucrr8a.pfb")] = T_("n022003l.pfb");
  fileURW[T_("ucrro8a.pfb")] = T_("n022023l.pfb");
  fileURW[T_("uhvb8a.pfb")] = T_("n019004l.pfb");
  fileURW[T_("uhvb8ac.pfb")] = T_("n019044l.pfb");
  fileURW[T_("uhvbo8a.pfb")] = T_("n019024l.pfb");
  fileURW[T_("uhvbo8ac.pfb")] = T_("n019064l.pfb");
  fileURW[T_("uhvr8a.pfb")] = T_("n019003l.pfb");
  fileURW[T_("uhvr8ac.pfb")] = T_("n019043l.pfb");
  fileURW[T_("uhvro8a.pfb")] = T_("n019023l.pfb");
  fileURW[T_("uhvro8ac.pfb")] = T_("n019063l.pfb");
  fileURW[T_("uncb8a.pfb")] = T_("c059016l.pfb");
  fileURW[T_("uncbi8a.pfb")] = T_("c059036l.pfb");
  fileURW[T_("uncr8a.pfb")] = T_("c059013l.pfb");
  fileURW[T_("uncri8a.pfb")] = T_("c059033l.pfb");
  fileURW[T_("uplb8a.pfb")] = T_("p052004l.pfb");
  fileURW[T_("uplbi8a.pfb")] = T_("p052024l.pfb");
  fileURW[T_("uplr8a.pfb")] = T_("p052003l.pfb");
  fileURW[T_("uplri8a.pfb")] = T_("p052023l.pfb");
  fileURW[T_("usyr.pfb")] = T_("s050000l.pfb");
  fileURW[T_("utmb8a.pfb")] = T_("n021004l.pfb");
  fileURW[T_("utmbi8a.pfb")] = T_("n021024l.pfb");
  fileURW[T_("utmr8a.pfb")] = T_("n021003l.pfb");
  fileURW[T_("utmri8a.pfb")] = T_("n021023l.pfb");
  fileURW[T_("uzcmi8a.pfb")] = T_("z003034l.pfb");
  fileURW[T_("uzdr.pfb")] = T_("d050000l.pfb");

  fileADOBE[T_("uagd8a.pfb")] = T_("agd_____.pfb");
  fileADOBE[T_("uagdo8a.pfb")] = T_("agdo____.pfb");
  fileADOBE[T_("uagk8a.pfb")] = T_("agw_____.pfb");
  fileADOBE[T_("uagko8a.pfb")] = T_("agwo____.pfb");
  fileADOBE[T_("ubkd8a.pfb")] = T_("bkd_____.pfb");
  fileADOBE[T_("ubkdi8a.pfb")] = T_("bkdi____.pfb");
  fileADOBE[T_("ubkl8a.pfb")] = T_("bkl_____.pfb");
  fileADOBE[T_("ubkli8a.pfb")] = T_("bkli____.pfb");
  fileADOBE[T_("ucrb8a.pfb")] = T_("cob_____.pfb");
  fileADOBE[T_("ucrbo8a.pfb")] = T_("cobo____.pfb");
  fileADOBE[T_("ucrr8a.pfb")] = T_("com_____.pfb");
  fileADOBE[T_("ucrro8a.pfb")] = T_("coo_____.pfb");
  fileADOBE[T_("uhvb8a.pfb")] = T_("hvb_____.pfb");
  fileADOBE[T_("uhvb8ac.pfb")] = T_("hvnb____.pfb");
  fileADOBE[T_("uhvbo8a.pfb")] = T_("hvbo____.pfb");
  fileADOBE[T_("uhvbo8ac.pfb")] = T_("hvnbo___.pfb");
  fileADOBE[T_("uhvr8a.pfb")] = T_("hv______.pfb");
  fileADOBE[T_("uhvr8ac.pfb")] = T_("hvn_____.pfb");
  fileADOBE[T_("uhvro8a.pfb")] = T_("hvo_____.pfb");
  fileADOBE[T_("uhvro8ac.pfb")] = T_("hvno____.pfb");
  fileADOBE[T_("uncb8a.pfb")] = T_("ncb_____.pfb");
  fileADOBE[T_("uncbi8a.pfb")] = T_("ncbi____.pfb");
  fileADOBE[T_("uncr8a.pfb")] = T_("ncr_____.pfb");
  fileADOBE[T_("uncri8a.pfb")] = T_("nci_____.pfb");
  fileADOBE[T_("uplb8a.pfb")] = T_("pob_____.pfb");
  fileADOBE[T_("uplbi8a.pfb")] = T_("pobi____.pfb");
  fileADOBE[T_("uplr8a.pfb")] = T_("por_____.pfb");
  fileADOBE[T_("uplri8a.pfb")] = T_("poi_____.pfb");
  fileADOBE[T_("usyr.pfb")] = T_("sy______.pfb");
  fileADOBE[T_("utmb8a.pfb")] = T_("tib_____.pfb");
  fileADOBE[T_("utmbi8a.pfb")] = T_("tibi____.pfb");
  fileADOBE[T_("utmr8a.pfb")] = T_("tir_____.pfb");
  fileADOBE[T_("utmri8a.pfb")] = T_("tii_____.pfb");
  fileADOBE[T_("uzcmi8a.pfb")] = T_("zcmi____.pfb");
  fileADOBE[T_("uzdr.pfb")] = T_("zd______.pfb");

  fileADOBEkb[T_("uagd8a.pfb")] = T_("pagd8a.pfb");
  fileADOBEkb[T_("uagdo8a.pfb")] = T_("pagdo8a.pfb");
  fileADOBEkb[T_("uagk8a.pfb")] = T_("pagk8a.pfb");
  fileADOBEkb[T_("uagko8a.pfb")] = T_("pagko8a.pfb");
  fileADOBEkb[T_("ubkd8a.pfb")] = T_("pbkd8a.pfb");
  fileADOBEkb[T_("ubkdi8a.pfb")] = T_("pbkdi8a.pfb");
  fileADOBEkb[T_("ubkl8a.pfb")] = T_("pbkl8a.pfb");
  fileADOBEkb[T_("ubkli8a.pfb")] = T_("pbkli8a.pfb");
  fileADOBEkb[T_("ucrb8a.pfb")] = T_("pcrb8a.pfb");
  fileADOBEkb[T_("ucrbo8a.pfb")] = T_("pcrbo8a.pfb");
  fileADOBEkb[T_("ucrr8a.pfb")] = T_("pcrr8a.pfb");
  fileADOBEkb[T_("ucrro8a.pfb")] = T_("pcrro8a.pfb");
  fileADOBEkb[T_("uhvb8a.pfb")] = T_("phvb8a.pfb");
  fileADOBEkb[T_("uhvb8ac.pfb")] = T_("phvb8an.pfb");
  fileADOBEkb[T_("uhvbo8a.pfb")] = T_("phvbo8a.pfb");
  fileADOBEkb[T_("uhvbo8ac.pfb")] = T_("phvbo8an.pfb");
  fileADOBEkb[T_("uhvr8a.pfb")] = T_("phvr8a.pfb");
  fileADOBEkb[T_("uhvr8ac.pfb")] = T_("phvr8an.pfb");
  fileADOBEkb[T_("uhvro8a.pfb")] = T_("phvro8a.pfb");
  fileADOBEkb[T_("uhvro8ac.pfb")] = T_("phvro8an.pfb");
  fileADOBEkb[T_("uncb8a.pfb")] = T_("pncb8a.pfb");
  fileADOBEkb[T_("uncbi8a.pfb")] = T_("pncbi8a.pfb");
  fileADOBEkb[T_("uncr8a.pfb")] = T_("pncr8a.pfb");
  fileADOBEkb[T_("uncri8a.pfb")] = T_("pncri8a.pfb");
  fileADOBEkb[T_("uplb8a.pfb")] = T_("pplb8a.pfb");
  fileADOBEkb[T_("uplbi8a.pfb")] = T_("pplbi8a.pfb");
  fileADOBEkb[T_("uplr8a.pfb")] = T_("pplr8a.pfb");
  fileADOBEkb[T_("uplri8a.pfb")] = T_("pplri8a.pfb");
  fileADOBEkb[T_("usyr.pfb")] = T_("psyr.pfb");
  fileADOBEkb[T_("utmb8a.pfb")] = T_("ptmb8a.pfb");
  fileADOBEkb[T_("utmbi8a.pfb")] = T_("ptmbi8a.pfb");
  fileADOBEkb[T_("utmr8a.pfb")] = T_("ptmr8a.pfb");
  fileADOBEkb[T_("utmri8a.pfb")] = T_("ptmri8a.pfb");
  fileADOBEkb[T_("uzcmi8a.pfb")] = T_("pzcmi8a.pfb");
  fileADOBEkb[T_("uzdr.pfb")] = T_("pzdr.pfb");

  psADOBE[T_("URWGothicL-Demi")] = T_("AvantGarde-Demi");
  psADOBE[T_("URWGothicL-DemiObli")] = T_("AvantGarde-DemiOblique");
  psADOBE[T_("URWGothicL-Book")] = T_("AvantGarde-Book");
  psADOBE[T_("URWGothicL-BookObli")] = T_("AvantGarde-BookOblique");
  psADOBE[T_("URWBookmanL-DemiBold")] = T_("Bookman-Demi");
  psADOBE[T_("URWBookmanL-DemiBoldItal")] = T_("Bookman-DemiItalic");
  psADOBE[T_("URWBookmanL-Ligh")] = T_("Bookman-Light");
  psADOBE[T_("URWBookmanL-LighItal")] = T_("Bookman-LightItalic");
  psADOBE[T_("NimbusMonL-Bold")] = T_("Courier-Bold");
  psADOBE[T_("NimbusMonL-BoldObli")] = T_("Courier-BoldOblique");
  psADOBE[T_("NimbusMonL-Regu")] = T_("Courier");
  psADOBE[T_("NimbusMonL-ReguObli")] = T_("Courier-Oblique");
  psADOBE[T_("NimbusSanL-Bold")] = T_("Helvetica-Bold");
  psADOBE[T_("NimbusSanL-BoldCond")] = T_("Helvetica-Narrow-Bold");
  psADOBE[T_("NimbusSanL-BoldItal")] = T_("Helvetica-BoldOblique");
  psADOBE[T_("NimbusSanL-BoldCondItal")] = T_("Helvetica-Narrow-BoldOblique");
  psADOBE[T_("NimbusSanL-Regu")] = T_("Helvetica");
  psADOBE[T_("NimbusSanL-ReguCond")] = T_("Helvetica-Narrow");
  psADOBE[T_("NimbusSanL-ReguItal")] = T_("Helvetica-Oblique");
  psADOBE[T_("NimbusSanL-ReguCondItal")] = T_("Helvetica-Narrow-Oblique");
  psADOBE[T_("CenturySchL-Bold")] = T_("NewCenturySchlbk-Bold");
  psADOBE[T_("CenturySchL-BoldItal")] = T_("NewCenturySchlbk-BoldItalic");
  psADOBE[T_("CenturySchL-Roma")] = T_("NewCenturySchlbk-Roman");
  psADOBE[T_("CenturySchL-Ital")] = T_("NewCenturySchlbk-Italic");
  psADOBE[T_("URWPalladioL-Bold")] = T_("Palatino-Bold");
  psADOBE[T_("URWPalladioL-BoldItal")] = T_("Palatino-BoldItalic");
  psADOBE[T_("URWPalladioL-Roma")] = T_("Palatino-Roman");
  psADOBE[T_("URWPalladioL-Ital")] = T_("Palatino-Italic");
  psADOBE[T_("StandardSymL")] = T_("Symbol");
  psADOBE[T_("NimbusRomNo9L-Medi")] = T_("Times-Bold");
  psADOBE[T_("NimbusRomNo9L-MediItal")] = T_("Times-BoldItalic");
  psADOBE[T_("NimbusRomNo9L-Regu")] = T_("Times-Roman");
  psADOBE[T_("NimbusRomNo9L-ReguItal")] = T_("Times-Italic");
  psADOBE[T_("URWChanceryL-MediItal")] = T_("ZapfChancery-MediumItalic");
  psADOBE[T_("Dingbats")] = T_("ZapfDingbats");
}

/* _________________________________________________________________________

   MakeFontMapApp::LocateMap
   _________________________________________________________________________ */

bool
MakeFontMapApp::LocateMap (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
			   /*[out]*/ PathName &		path,
			   /*[in]*/ bool		mustExist)
{
  bool found = pSession->FindFile(lpszFileName, FileType::MAP, path);
  if (! found && mustExist)
    {
      Abort (T_("Font map file %s could not be found."), Q_(lpszFileName));
    }
  if (! found)
    {
      Verbose (T_("Not using map file %s"), Q_(lpszFileName));
    }
  return (found);
}

/* _________________________________________________________________________

   MakeFontMapApp::WriteHeader
   _________________________________________________________________________ */

void
MakeFontMapApp::WriteHeader (/*[in]*/ StreamWriter &	writer,
			     /*[in]*/ const PathName &	fileName)
{
  UNUSED_ALWAYS (fileName);
  writer.WriteLine (T_("\
%%% DO NOT EDIT THIS FILE! It will be overwritten."));
  writer.WriteLine (T_("\
%%% Run 'initexmf --edit-config-file updmap' to integrate new"));
  writer.WriteLine (T_("\
%%% font map files."));
}

/* _________________________________________________________________________

   MakeFontMapApp::WriteMap
   _________________________________________________________________________ */

void
MakeFontMapApp::WriteMap (/*[in]*/ StreamWriter &		writer,
			  /*[in]*/ const set<FontMapEntry> &	set1)
{
  for (set<FontMapEntry>::const_iterator it = set1.begin();
       it != set1.end();
       ++ it)
    {
      writer.WriteFormatted (T_("%s"), it->texName.c_str());
      writer.WriteFormatted (T_(" %s"), it->psName.c_str());
      if (! it->specialInstructions.empty())
	{
	  writer.WriteFormatted (T_(" \" %s \""),
				 it->specialInstructions.c_str());
	}
      for (Tokenizer tok (it->headerList.c_str(), T_(";"));
	   tok.GetCurrent() != 0;
	   ++ tok)
	{
	  writer.WriteFormatted (T_(" %s"), tok.GetCurrent());
	}
      writer.WriteLine ();
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::GetInstructionParam
   _________________________________________________________________________ */

bool
MakeFontMapApp::GetInstructionParam (/*[in]*/ const tstring &	str,
				     /*[in]*/ const tstring &	instruction,
				     /*[out]*/ tstring &	param)
{
  param = T_("");
  for (Tokenizer tok (str.c_str(), T_(" \t")); tok.GetCurrent() != 0; ++ tok)
    {
      if (instruction == tok.GetCurrent())
	{
	  return (true);
	}
      param = tok.GetCurrent();
    }
  return (false);
}

/* _________________________________________________________________________

   MakeFontMapApp::WriteDvipdfmMap
   _________________________________________________________________________ */

void
MakeFontMapApp::WriteDvipdfmMap (/*[in]*/ StreamWriter &		writer,
				 /*[in]*/ const set<FontMapEntry> &	set1)
{
  for (set<FontMapEntry>::const_iterator it = set1.begin();
       it != set1.end();
       ++ it)
    {
      tstring field1 = it->texName;
      tstring field2;
      if (! it->encFile.empty())
	{
	  field2 = PathName(it->encFile).GetFileNameWithoutExtension().Get();
	}
      tstring field3;
      if (! it->fontFile.empty())
	{
	  field3 = PathName(it->fontFile).GetFileNameWithoutExtension().Get();
	}
      else if (it->texName != it->psName)
	{
	  field3 = it->psName;
	}
      tstring options;
      tstring param;
      if (GetInstructionParam(it->specialInstructions,
			      T_("ExtendFont"),
			      param))
	{
	  options += T_(" -e ");
	  options += param;
	}
      if (GetInstructionParam(it->specialInstructions,
			      T_("SlantFont"),
			      param))
	{
	  options += T_(" -s ");
	  options += param;
	}
      if (it->texName.substr(0, 2) == T_("cm")
	  || it->texName.substr(0, 2) == T_("eu")
	  || (it->texName.substr(0, 2) == T_("la")
	      && ! (it->encFile.substr(0, 12) == T_("cm-super-t2a")))
	  || (it->texName.substr(0, 2) == T_("lc")
	      && ! (it->encFile.substr(0, 12) == T_("cm-super-t2c")))
	  || it->texName.substr(0, 4) == T_("line")
	  || it->texName.substr(0, 4) == T_("msam")
	  || it->texName.substr(0, 2) == T_("xy"))
	{
	  if (! (it->fontFile.substr(0, 4)  == T_("fmex")))
	    {
	      options += T_(" -r");
	    }
	}
      if (field2 == T_("") && field3 == T_("") && options == T_(""))
	{
	  continue;
	}
      writer.Write (field1);
      if (! field2.empty())
	{
	  writer.WriteFormatted (T_(" %s"), field2.c_str());
	}
      else if (! field3.empty())
	{
	  writer.WriteFormatted (T_(" %s"), T_("default"));
	}
      if (! field3.empty())
	{
	  writer.WriteFormatted (T_(" %s"), field3.c_str());
	}
      if (! options.empty())
	{
	  writer.WriteFormatted (T_("%s"), options.c_str());
	}
      writer.WriteLine ();
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::CreateOutputDir
   _________________________________________________________________________ */

PathName
MakeFontMapApp::CreateOutputDir (/*[in]*/ const MIKTEXCHAR * lpszRelPath)
{
  PathName path;
  if (! outputDirectory.empty())
    {
      path = outputDirectory;
    }
  else
    {
      path.Set (pSession->GetSpecialPath(SpecialPath::DataRoot), lpszRelPath);
    }
  if (! Directory::Exists(path))
    {
      Directory::Create (path);
    }
  return (path);
}

/* _________________________________________________________________________

   MakeFontMapApp::WriteDvipsMapFile
   _________________________________________________________________________ */

void
MakeFontMapApp::WriteDvipsMapFile (/*[in]*/ const PathName &	fileName,
				   /*[in]*/ const set<FontMapEntry> &	set1,
				   /*[in]*/ const set<FontMapEntry> &	set2,
				   /*[in]*/ const set<FontMapEntry> &	set3)
{
  PathName path (GetDvipsOutputDir());
  path += fileName;
  Verbose (T_("Writing %s..."), Q_(path));
  // <todo>backup old file</todo>
  StreamWriter writer (path);
  WriteHeader (writer, path);
  set<FontMapEntry> setAll = set1;
  setAll.insert (set2.begin(), set2.end());
  setAll.insert (set3.begin(), set3.end());
  WriteMap (writer, setAll);
  writer.Close ();
  if (! Fndb::FileExists(path))
    {
      Fndb::Add (path);
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::WriteDvipdfmMapFile
   _________________________________________________________________________ */

void
MakeFontMapApp::WriteDvipdfmMapFile (/*[in]*/ const PathName &	fileName,
				     /*[in]*/ const set<FontMapEntry> &	set1,
				     /*[in]*/ const set<FontMapEntry> &	set2,
				     /*[in]*/ const set<FontMapEntry> &	set3)
{
  PathName path (GetDvipdfmOutputDir());
  path += fileName;
  Verbose (T_("Writing %s..."), Q_(path));
  // <todo>backup old file</todo>
  StreamWriter writer (path);
  WriteHeader (writer, path);
  set<FontMapEntry> setAll = set1;
  setAll.insert (set2.begin(), set2.end());
  setAll.insert (set3.begin(), set3.end());
  WriteDvipdfmMap (writer, setAll);
  writer.Close ();
  if (! Fndb::FileExists(path))
    {
      Fndb::Add (path);
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::WritePdfTeXMapFile
   _________________________________________________________________________ */

void
MakeFontMapApp::WritePdfTeXMapFile (/*[in]*/ const PathName &	fileName,
				    /*[in]*/ const set<FontMapEntry> &	set1,
				    /*[in]*/ const set<FontMapEntry> &	set2,
				    /*[in]*/ const set<FontMapEntry> &	set3)
{
  PathName path (GetPdfTeXOutputDir());
  path += fileName;
  Verbose (T_("Writing %s..."), Q_(path));
  // <todo>backup old file</todo>
  StreamWriter writer (path);
  WriteHeader (writer, path);
  set<FontMapEntry> setAll = set1;
  setAll.insert (set2.begin(), set2.end());
  setAll.insert (set3.begin(), set3.end());
  WriteMap (writer, setAll);
  writer.Close ();
  if (! Fndb::FileExists(path))
    {
      Fndb::Add (path);
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::ParseDvipsMapFile
   _________________________________________________________________________ */


void
MakeFontMapApp::ParseDvipsMapFile
(/*[in]*/ const PathName &		mapFile,
 /*[in,out]*/ set<FontMapEntry> &	fontMapEntries)
{
  Verbose (T_("Parsing %s..."), Q_(mapFile));

  StreamReader reader (mapFile);

  tstring line;

  mapContext.path = mapFile;
  mapContext.line = 0;

  while (reader.ReadLine(line))
    {
      ++ mapContext.line;
      FontMapEntry fontMapEntry;
      try
	{
	  if (Utils::ParseDvipsMapLine(line, fontMapEntry))
	    {
	      fontMapEntries.insert (fontMapEntry);
	    }
	}
      catch (const MiKTeXException & e)
	{
	  MapError (T_("%s"), e.what());
	}
    }

  reader.Close ();
}

/* _________________________________________________________________________

   MakeFontMapApp::ReadMap
   _________________________________________________________________________ */

void
MakeFontMapApp::ReadMap (/*[in]*/ const tstring &		fileName,
			 /*[in,out]*/ set<FontMapEntry> &	result,
			 /*[in]*/ bool				mustExist)
{
  PathName path;
  if (! LocateMap(fileName.c_str(), path, mustExist))
    {
      return;
    }
  ParseDvipsMapFile (path, result);
}

/* _________________________________________________________________________

   MakeFontMapApp::CatMaps
   _________________________________________________________________________ */

set<FontMapEntry>
MakeFontMapApp::CatMaps (/*[in]*/ const set<tstring> &	fileNames)
{
  set<FontMapEntry> result;
  for (set<tstring>::const_iterator it = fileNames.begin();
       it != fileNames.end();
       ++ it)
    {
      ReadMap (*it, result, false);
    }
  return (result);
}

/* _________________________________________________________________________

   MakeFontMapApp::TranslateFontFile
   _________________________________________________________________________ */

void
MakeFontMapApp::TranslateFontFile
(/*[in]*/ const map<tstring, tstring> &	transMap,
 /*[in,out]*/ FontMapEntry &	fontMapEntry)
{
  map<tstring, tstring>::const_iterator it;
  it = transMap.find(fontMapEntry.fontFile);
  if (it != transMap.end())
    {
      fontMapEntry.fontFile = it->second;
    }
  Tokenizer header (fontMapEntry.headerList.c_str(), T_(";"));
  fontMapEntry.headerList = T_("");
  for (; header.GetCurrent() != 0; ++ header)
    {
      if (! fontMapEntry.headerList.empty())
	{
	  fontMapEntry.headerList += T_(';');
	}
      const MIKTEXCHAR * lpsz;
      for (lpsz = header.GetCurrent();
	   *lpsz == T_('<') || *lpsz == T_('[');
	   ++ lpsz)
	{
	  fontMapEntry.headerList += *lpsz;
	}
      it = transMap.find(lpsz);
      if (it == transMap.end())
	{
	  fontMapEntry.headerList += lpsz;
	}
      else
	{
	  fontMapEntry.headerList += it->second;
	}
    }  
}

/* _________________________________________________________________________

   MakeFontMapApp::TranslatePSName
   _________________________________________________________________________ */

void
MakeFontMapApp::TranslatePSName (/*[in]*/ const map<tstring, tstring> &	names,
				 /*[in,out]*/ FontMapEntry &	fontMapEntry)
{
  map<tstring, tstring>::const_iterator it;
  it = names.find(fontMapEntry.psName);
  if (it != names.end())
    {
      fontMapEntry.psName = it->second;
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::TranslateLW35
   _________________________________________________________________________ */

set<FontMapEntry>
MakeFontMapApp::TranslateLW35 (/*[in]*/ const set<FontMapEntry> &	set1)
{
  set<FontMapEntry> result;
  for (set<FontMapEntry>::const_iterator it = set1.begin();
       it != set1.end();
       ++ it)
    {
      FontMapEntry fontMapEntry = *it;
      switch (namingConvention)
	{
	case URWkb:
	  break;
	case URW:
	  TranslateFontFile (fileURW, fontMapEntry);
	  break;
	case ADOBE:
	  TranslatePSName (psADOBE, fontMapEntry);
	  TranslateFontFile (fileADOBE, fontMapEntry);
	  break;
	case ADOBEkb:
	  TranslatePSName (psADOBE, fontMapEntry);
	  TranslateFontFile (fileADOBEkb, fontMapEntry);
	  break;
	}
      result.insert (fontMapEntry);
    }
  return (result);
}

/* _________________________________________________________________________

   MakeFontMapApp::CopyFile
   _________________________________________________________________________ */

void
MakeFontMapApp::CopyFile (/*[in]*/ const PathName & pathSrc,
			  /*[in]*/ const PathName & pathDest)
{
  Verbose (T_("Copying %s"), Q_(pathSrc));
  Verbose (T_("     to %s..."), Q_(pathDest));
  File::Copy (pathSrc, pathDest);
  if (! Fndb::FileExists(pathDest))
    {
      Fndb::Add (pathDest);
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::CopyFiles
   _________________________________________________________________________ */

void
MakeFontMapApp::CopyFiles ()
{
  PathName dvipsOutputDir (GetDvipsOutputDir());
  PathName dvipdfmOutputDir (GetDvipdfmOutputDir());
  PathName pdftexOutputDir (GetPdfTeXOutputDir());

  PathName pathSrc;

  pathSrc.Set (dvipsOutputDir,
	       (dvipsPreferOutline ? T_("psfonts_t1") : T_("psfonts_pk")),
	       T_(".map"));
  CopyFile (pathSrc, PathName(dvipsOutputDir, T_("psfonts.map")));

  pathSrc.Set (dvipdfmOutputDir,
	       (dvipdfmDownloadBase14
		? T_("dvipdfm_dl14")
		: T_("dvipdfm_ndl14")),
	       T_(".map"));
  CopyFile (pathSrc, PathName(dvipdfmOutputDir, T_("dvipdfm.map")));
#if CREATE_DEPRECATED_MAP_FILES
  CopyFile (pathSrc, PathName(dvipdfmOutputDir, T_("psfonts.map")));
#endif

  pathSrc.Set (pdftexOutputDir,
	       (pdftexDownloadBase14
		? T_("pdftex_dl14")
		: T_("pdftex_ndl14")),
	       T_(".map"));
  CopyFile (pathSrc, PathName(pdftexOutputDir, T_("pdftex.map")));
#if CREATE_DEPRECATED_MAP_FILES
  CopyFile (pathSrc, PathName(pdftexOutputDir, T_("psfonts.map")));
#endif
}

/* _________________________________________________________________________

   MakeFontMapApp::BuildFontconfigCache
   _________________________________________________________________________ */

static const MIKTEXCHAR * const topDirs[] = {
  T_("fonts/type1"),
  T_("fonts/opentype"),
  T_("fonts/truetype"),
};

void
MakeFontMapApp::BuildFontconfigCache ()
{
  PathName configFile (pSession->GetSpecialPath(SpecialPath::ConfigRoot));
  configFile += MIKTEX_PATH_FONTCONFIG_LOCALFONTS_FILE;
  StreamWriter writer (configFile);
  writer.WriteLine (T_("<?xml version=\"1.0\"?>"));
  writer.WriteLine (T_("<fontconfig>"));
  vector<tstring> paths;
#if defined(MIKTEX_WINDOWS)
  PathName path;
  UINT l =
    GetWindowsDirectory(path.GetBuffer(), static_cast<UINT>(path.GetCapacity()));
  if (l == 0 || l >= path.GetCapacity())
    {
      Abort (T_("GetWindowsDirectory() failed for some reason."));
    }
  path += T_("Fonts");
  paths.push_back (path.Get());
#endif
  for (unsigned r = 0; r < pSession->GetNumberOfTEXMFRoots(); ++ r)
    {
      PathName root = pSession->GetRootDirectory(r);
      for (size_t idx = 0; idx < sizeof(topDirs) / sizeof(topDirs[0]); ++ idx)
	{
	  PathName path = root;
	  path += topDirs[idx];
	  if (Directory::Exists(path))
	    {
	      paths.push_back (path.Get());
	    }
	}
    }
  for (vector<tstring>::const_iterator it = paths.begin();
       it != paths.end();
       ++ it)
    {
      writer.WriteFormattedLine (T_("<dir>%s</dir>"), it->c_str());
    }
  writer.WriteLine (T_("</fontconfig>"));
  writer.Close ();
  PathName pathFcCache;
  if (! pSession->FindFile (T_("fc-cache"), FileType::EXE, pathFcCache))
    {
      Abort (T_("The fc-cache executable could not be found."));
    }
  CommandLineBuilder arguments;
  arguments.AppendOption (T_("--force"));
  if (verbose)
    {
      arguments.AppendOption (T_("--verbose"));
    }
  Process::Run (pathFcCache, arguments.Get());
}

/* _________________________________________________________________________

   MakeFontMapApp::MakeMaps
   _________________________________________________________________________ */

bool
HasPaintType (/*[in]*/ const FontMapEntry & fontMapEntry)
{
  return (fontMapEntry.specialInstructions.find(T_("PaintType"))
	  != tstring::npos);
}

void
MakeFontMapApp::MakeMaps ()
{
  set<FontMapEntry> dvips35;
  ReadMap (T_("dvips35.map"), dvips35, true);
  set<FontMapEntry> pdftex35;
  ReadMap (T_("pdftex35.map"), pdftex35, true);
  set<FontMapEntry> dvipdfm35;
  ReadMap (T_("dvipdfm35.map"), dvipdfm35, true);
  set<FontMapEntry> ps2pk35;
  ReadMap (T_("ps2pk35.map"), ps2pk35, true);

  set<FontMapEntry> transLW35_ps2pk35 (TranslateLW35(ps2pk35));

  set<FontMapEntry> transLW35_dvips35 (TranslateLW35(dvips35));

  set<FontMapEntry> transLW35_pdftex35 (TranslateLW35(pdftex35));

  set<FontMapEntry> transLW35_dvipdfm35 (TranslateLW35(dvipdfm35));

  set<FontMapEntry> tmp1 (CatMaps(mixedMapFiles));

  set<FontMapEntry> tmp2 (CatMaps(mapFiles));

  WriteDvipsMapFile (T_("ps2pk.map"), transLW35_ps2pk35, tmp1, tmp2);

  set<FontMapEntry> empty;

  WriteDvipsMapFile (T_("download35.map"), transLW35_ps2pk35, empty, empty);

  WriteDvipsMapFile (T_("builtin35.map"), transLW35_dvips35, empty, empty);

  set<FontMapEntry> transLW35_dftdvips
    (TranslateLW35(dvipsDownloadBase35 ? ps2pk35 : dvips35));

  WriteDvipsMapFile (T_("psfonts_t1.map"), transLW35_dftdvips, tmp1, tmp2);

  WriteDvipsMapFile (T_("psfonts_pk.map"), transLW35_dftdvips, empty, tmp2);

  set<FontMapEntry> tmp3 = transLW35_pdftex35;
  tmp3.insert (tmp1.begin(), tmp1.end());
  tmp3.insert (tmp2.begin(), tmp2.end());
  set<FontMapEntry>::iterator it = tmp3.begin();
  while (it != tmp3.end())
    {
      if (HasPaintType(*it))
	{
	  it = tmp3.erase (it);
	}
      else
	{
	  ++ it;
	}
    }

  set<FontMapEntry> tmp6 = transLW35_dvipdfm35;
  tmp6.insert (tmp1.begin(), tmp1.end());
  tmp6.insert (tmp2.begin(), tmp2.end());
  it = tmp6.begin();
  while (it != tmp6.end())
    {
      if (HasPaintType(*it))
	{
	  it = tmp6.erase (it);
	}
      else
	{
	  ++ it;
	}
    }

  set<FontMapEntry> tmp7 = transLW35_ps2pk35;
  tmp7.insert (tmp1.begin(), tmp1.end());
  tmp7.insert (tmp2.begin(), tmp2.end());
  it = tmp7.begin();
  while (it != tmp7.end())
    {
      if (HasPaintType(*it))
	{
	  it = tmp7.erase (it);
	}
      else
	{
	  ++ it;
	}
    }

  WritePdfTeXMapFile (T_("pdftex_ndl14.map"), tmp3, empty, empty);
  WritePdfTeXMapFile (T_("pdftex_dl14.map"), tmp7, empty, empty);

  WriteDvipdfmMapFile (T_("dvipdfm_dl14.map"), tmp7, empty, empty);
  WriteDvipdfmMapFile (T_("dvipdfm_ndl14.map"), tmp6, empty, empty);

  CopyFiles ();

  BuildFontconfigCache ();
}

/* _________________________________________________________________________

   MakeFontMapApp::Run
   _________________________________________________________________________ */

void
MakeFontMapApp::Run (/*[in]*/ int			argc,
		     /*[in]*/ const MIKTEXCHAR **	argv)
{
  pSession.CreateSession (Session::InitInfo(argv[0]));
  ProcessOptions (argc, argv);
  Initialize ();
  MakeMaps ();
  pSession.Reset ();
}

/* _________________________________________________________________________

   mkfntmap
   _________________________________________________________________________ */

extern "C"
__declspec(dllexport)
int
__cdecl
mkfntmap (/*[in]*/ int			argc,
	  /*[in]*/ const MIKTEXCHAR **	argv)
{
  try
    {
      MakeFontMapApp app;
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
