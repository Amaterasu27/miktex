/* mkfntmap.cpp:

   Copyright (C) 2002-2009 Christian Schenk

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
   
#include <miktex/Core/Core>
#include <miktex/Core/Paths>
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

#define PROGRAM_NAME "mkfntmap"

#if ! defined(THE_NAME_OF_THE_GAME)
#  define THE_NAME_OF_THE_GAME T_("MiKTeX Fontmap Maintenance Utility")
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
  OPT_AAA = 1,
  OPT_ADMIN,
  OPT_OUTPUT_DIRECTORY,
  OPT_VERBOSE,
  OPT_VERSION,
};

const struct poptOption aoption[] = {
  {
    "admin", 0,
    POPT_ARG_NONE, 0,
    OPT_ADMIN,
    T_("Run in administrative mode."),
    0,
  },

  {
    "output-directory", 0,
    POPT_ARG_STRING, 0,
    OPT_OUTPUT_DIRECTORY,
    T_("Set the output directory."),
    "DIR"
  },

  {
    "verbose", 0,
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Turn on verbose mode."),
    0
  },

  {
    "version", 0,
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
      adminMode (false),
      namingConvention (URWkb)
  {
  }

private:
  void
  ShowVersion ();

private:
  void
  ProcessOptions (/*[in]*/ int			argc,
		  /*[in]*/ const char **	argv);

private:
  void
  Initialize ();

private:
  bool
  ToBool (/*[in]*/ const string & param);

private:
  bool
  ScanConfigLine (/*[in]*/ const string &	line,
		  /*[out]*/ string &		directive,
		  /*[out]*/ string &		param);

private:
  void
  ParseConfigFile (/*[in]*/ const PathName & path);

public:
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv);

private:
  bool
  LocateMap (/*[in]*/ const char *	lpszFileName,
	     /*[out]*/ PathName &	path,
	     /*[in]*/ bool		mustExist);

private:
  void
  ReadMap (/*[in]*/ const string &		fileName,
	   /*[in,out]*/ set<FontMapEntry> &	fontMapEntries,
	   /*[in]*/ bool			mustExist);

private:
  void
  WriteHeader (/*[in]*/ StreamWriter &		writer,
	       /*[in]*/ const PathName &	fileName);

private:
  PathName
  CreateOutputDir (/*[in]*/ const char * lpszRelPath);

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
  GetInstructionParam (/*[in]*/ const string &	str,
		       /*[in]*/ const string &	instruction,
		       /*[out]*/ string &	param);

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
  CatMaps (/*[in]*/ const set<string> & fileNames);

private:
  set<FontMapEntry>
  TranslateLW35 (/*[in]*/ const set<FontMapEntry> &	set1);

private:
  void
  TranslateFontFile (/*[in]*/ const map<string, string> &	transMap,
		     /*[in,out]*/ FontMapEntry &		fontMapEntry);

private:
  void
  TranslatePSName (/*[in]*/ const map<string, string> & files,
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
  Verbose (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  Abort (/*[in]*/ const char *	lpszFormat,
	 /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  CfgError (/*[in]*/ const char *	lpszMessage,
	    /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  MapError (/*[in]*/ const char *	lpszMessage,
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
  set<string> mapFiles;

private:
  set<string> mixedMapFiles;

private:
  bool verbose;

  // transform file names from URWkb (berry names) to URW (vendor
  // names)
private:
  static map<string, string> fileURW;

  // transform file names from URWkb (berry names) to ADOBE (vendor
  // names)
private:
  static map<string, string> fileADOBE;

  // transform file names from URW to ADOBE (both berry names)
private:
  static map<string, string> fileADOBEkb;

  // transform font names from URW to Adobe
private:
  static map<string, string> psADOBE;

private:
  string outputDirectory;

private:
  bool adminMode;

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

map<string, string> MakeFontMapApp::fileURW;
map<string, string> MakeFontMapApp::fileADOBE;
map<string, string> MakeFontMapApp::fileADOBEkb;
map<string, string> MakeFontMapApp::psADOBE;

/* _________________________________________________________________________

   MakeFontMapApp::ShowVersion
   _________________________________________________________________________ */

void
MakeFontMapApp::ShowVersion ()
{
  cout << Utils::MakeProgramVersionString(THE_NAME_OF_THE_GAME,
					  VersionNumber(MIKTEX_MAJOR_VERSION,
							MIKTEX_MINOR_VERSION,
							MIKTEX_COMP_J2000_VERSION,
							0))
       << T_("\n\
Copyright (C) 2002-2009 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
       << endl;
}

/* _________________________________________________________________________

    MakeFontMapApp::ProcessOptions
   _________________________________________________________________________ */

void
MakeFontMapApp::ProcessOptions (/*[in]*/ int		argc,
				/*[in]*/ const char **	argv)
{
  Cpopt popt (argc, argv, aoption);

  int option;
  
  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{
	case OPT_ADMIN:
	  adminMode = true;
	  break;
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
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += ": ";
      msg += popt.Strerror(option);
      Abort ("%s", msg.c_str());
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
MakeFontMapApp::Verbose (/*[in]*/ const char *	lpszFormat,
			 /*[in]*/		...)
{
  if (! verbose)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   MakeFontMapApp::CfgError
   _________________________________________________________________________ */

MIKTEXNORETURN
void
MakeFontMapApp::CfgError (/*[in]*/ const char *	lpszFormat,
			  /*[in]*/		...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << cfgContext.path.Get() << ":"
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
MakeFontMapApp::MapError (/*[in]*/ const char *	lpszFormat,
			  /*[in]*/		...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << mapContext.path.Get() << ":"
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
MakeFontMapApp::Abort (/*[in]*/ const char *	lpszFormat,
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

   MakeFontMapApp::ToBool
   _________________________________________________________________________ */

bool
MakeFontMapApp::ToBool (/*[in]*/ const string & param)
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
MakeFontMapApp::ScanConfigLine (/*[in]*/ const string &		line,
				/*[out]*/ string &		directive,
				/*[out]*/ string &		param)
{
  if (line.empty()
      || string("*#;%").find_first_of(line[0]) != string::npos)
    {
      return (false);
    }
  Tokenizer tok (line.c_str(), " \t\n");
  if (tok.GetCurrent() == 0)
    {
      return (false);
    }
  directive = tok.GetCurrent();
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      param = "";
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
  string line;
  while (reader.ReadLine(line))
    {
      ++ cfgContext.line;
      string directive;
      string param;
      if (! ScanConfigLine(line, directive, param))
	{
	  continue;
	}
      if (StringCompare(directive.c_str(), "dvipsPreferOutline", true)
	  == 0)
	{
	  dvipsPreferOutline = ToBool(param);
	}
      else if (StringCompare(directive.c_str(), "LW35", true) == 0)
	{
	  if (param.empty())
	    {
	      CfgError (T_("missing value"));
	    }
	  if (StringCompare(param.c_str(), "URW", true) == 0)
	    {
	      namingConvention = URW;
	    }
	  else if (StringCompare(param.c_str(), "URWkb", true) == 0)
	    {
	      namingConvention = URWkb;
	    }
	  else if (StringCompare(param.c_str(), "ADOBE", true) == 0)
	    {
	      namingConvention = ADOBE;
	    }
	  else if (StringCompare(param.c_str(), "ADOBEkb", true) == 0)
	    {
	      namingConvention = ADOBEkb;
	    }
	  else
	    {
	      CfgError (T_("invalid value"));
	    }
	}
      else if (StringCompare(directive.c_str(),
			     "dvipsDownloadBase35",
			     true)
	       == 0)
	{
	  dvipsDownloadBase35 = ToBool(param);
	}
      else if (StringCompare(directive.c_str(),
			     "pdftexDownloadBase14",
			     true)
	       == 0)
	{
	  pdftexDownloadBase14 = ToBool(param);
	}
      else if (StringCompare(directive.c_str(),
			     "dvipdfmDownloadBase14",
			     true)
	       == 0)
	{
	  dvipdfmDownloadBase14 = ToBool(param);
	}
      else if (StringCompare(directive.c_str(), "Map", true) == 0)
	{
	  if (param.empty())
	    {
	      CfgError (T_("missing map file name"));
	    }
	  mapFiles.insert (param);
	}
      else if (StringCompare(directive.c_str(), "MixedMap", true) == 0)
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
      static const char * lpszConfigFiles[] = {
	MIKTEX_PATH_MKFNTMAP_CFG,
	(MIKTEX_PATH_MIKTEX_CONFIG_DIR
	 MIKTEX_PATH_DIRECTORY_DELIMITER_STRING
	 "updmap.cfg"),
	(MIKTEX_PATH_WEB2C_DIR
	 MIKTEX_PATH_DIRECTORY_DELIMITER_STRING
	 "updmap.cfg")
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

  fileURW["uagd8a.pfb"] = "a010015l.pfb";
  fileURW["uagdo8a.pfb"] = "a010035l.pfb";
  fileURW["uagk8a.pfb"] = "a010013l.pfb";
  fileURW["uagko8a.pfb"] = "a010033l.pfb";
  fileURW["ubkd8a.pfb"] = "b018015l.pfb";
  fileURW["ubkdi8a.pfb"] = "b018035l.pfb";
  fileURW["ubkl8a.pfb"] = "b018012l.pfb";
  fileURW["ubkli8a.pfb"] = "b018032l.pfb";
  fileURW["ucrb8a.pfb"] = "n022004l.pfb";
  fileURW["ucrbo8a.pfb"] = "n022024l.pfb";
  fileURW["ucrr8a.pfb"] = "n022003l.pfb";
  fileURW["ucrro8a.pfb"] = "n022023l.pfb";
  fileURW["uhvb8a.pfb"] = "n019004l.pfb";
  fileURW["uhvb8ac.pfb"] = "n019044l.pfb";
  fileURW["uhvbo8a.pfb"] = "n019024l.pfb";
  fileURW["uhvbo8ac.pfb"] = "n019064l.pfb";
  fileURW["uhvr8a.pfb"] = "n019003l.pfb";
  fileURW["uhvr8ac.pfb"] = "n019043l.pfb";
  fileURW["uhvro8a.pfb"] = "n019023l.pfb";
  fileURW["uhvro8ac.pfb"] = "n019063l.pfb";
  fileURW["uncb8a.pfb"] = "c059016l.pfb";
  fileURW["uncbi8a.pfb"] = "c059036l.pfb";
  fileURW["uncr8a.pfb"] = "c059013l.pfb";
  fileURW["uncri8a.pfb"] = "c059033l.pfb";
  fileURW["uplb8a.pfb"] = "p052004l.pfb";
  fileURW["uplbi8a.pfb"] = "p052024l.pfb";
  fileURW["uplr8a.pfb"] = "p052003l.pfb";
  fileURW["uplri8a.pfb"] = "p052023l.pfb";
  fileURW["usyr.pfb"] = "s050000l.pfb";
  fileURW["utmb8a.pfb"] = "n021004l.pfb";
  fileURW["utmbi8a.pfb"] = "n021024l.pfb";
  fileURW["utmr8a.pfb"] = "n021003l.pfb";
  fileURW["utmri8a.pfb"] = "n021023l.pfb";
  fileURW["uzcmi8a.pfb"] = "z003034l.pfb";
  fileURW["uzdr.pfb"] = "d050000l.pfb";

  fileADOBE["uagd8a.pfb"] = "agd_____.pfb";
  fileADOBE["uagdo8a.pfb"] = "agdo____.pfb";
  fileADOBE["uagk8a.pfb"] = "agw_____.pfb";
  fileADOBE["uagko8a.pfb"] = "agwo____.pfb";
  fileADOBE["ubkd8a.pfb"] = "bkd_____.pfb";
  fileADOBE["ubkdi8a.pfb"] = "bkdi____.pfb";
  fileADOBE["ubkl8a.pfb"] = "bkl_____.pfb";
  fileADOBE["ubkli8a.pfb"] = "bkli____.pfb";
  fileADOBE["ucrb8a.pfb"] = "cob_____.pfb";
  fileADOBE["ucrbo8a.pfb"] = "cobo____.pfb";
  fileADOBE["ucrr8a.pfb"] = "com_____.pfb";
  fileADOBE["ucrro8a.pfb"] = "coo_____.pfb";
  fileADOBE["uhvb8a.pfb"] = "hvb_____.pfb";
  fileADOBE["uhvb8ac.pfb"] = "hvnb____.pfb";
  fileADOBE["uhvbo8a.pfb"] = "hvbo____.pfb";
  fileADOBE["uhvbo8ac.pfb"] = "hvnbo___.pfb";
  fileADOBE["uhvr8a.pfb"] = "hv______.pfb";
  fileADOBE["uhvr8ac.pfb"] = "hvn_____.pfb";
  fileADOBE["uhvro8a.pfb"] = "hvo_____.pfb";
  fileADOBE["uhvro8ac.pfb"] = "hvno____.pfb";
  fileADOBE["uncb8a.pfb"] = "ncb_____.pfb";
  fileADOBE["uncbi8a.pfb"] = "ncbi____.pfb";
  fileADOBE["uncr8a.pfb"] = "ncr_____.pfb";
  fileADOBE["uncri8a.pfb"] = "nci_____.pfb";
  fileADOBE["uplb8a.pfb"] = "pob_____.pfb";
  fileADOBE["uplbi8a.pfb"] = "pobi____.pfb";
  fileADOBE["uplr8a.pfb"] = "por_____.pfb";
  fileADOBE["uplri8a.pfb"] = "poi_____.pfb";
  fileADOBE["usyr.pfb"] = "sy______.pfb";
  fileADOBE["utmb8a.pfb"] = "tib_____.pfb";
  fileADOBE["utmbi8a.pfb"] = "tibi____.pfb";
  fileADOBE["utmr8a.pfb"] = "tir_____.pfb";
  fileADOBE["utmri8a.pfb"] = "tii_____.pfb";
  fileADOBE["uzcmi8a.pfb"] = "zcmi____.pfb";
  fileADOBE["uzdr.pfb"] = "zd______.pfb";

  fileADOBEkb["uagd8a.pfb"] = "pagd8a.pfb";
  fileADOBEkb["uagdo8a.pfb"] = "pagdo8a.pfb";
  fileADOBEkb["uagk8a.pfb"] = "pagk8a.pfb";
  fileADOBEkb["uagko8a.pfb"] = "pagko8a.pfb";
  fileADOBEkb["ubkd8a.pfb"] = "pbkd8a.pfb";
  fileADOBEkb["ubkdi8a.pfb"] = "pbkdi8a.pfb";
  fileADOBEkb["ubkl8a.pfb"] = "pbkl8a.pfb";
  fileADOBEkb["ubkli8a.pfb"] = "pbkli8a.pfb";
  fileADOBEkb["ucrb8a.pfb"] = "pcrb8a.pfb";
  fileADOBEkb["ucrbo8a.pfb"] = "pcrbo8a.pfb";
  fileADOBEkb["ucrr8a.pfb"] = "pcrr8a.pfb";
  fileADOBEkb["ucrro8a.pfb"] = "pcrro8a.pfb";
  fileADOBEkb["uhvb8a.pfb"] = "phvb8a.pfb";
  fileADOBEkb["uhvb8ac.pfb"] = "phvb8an.pfb";
  fileADOBEkb["uhvbo8a.pfb"] = "phvbo8a.pfb";
  fileADOBEkb["uhvbo8ac.pfb"] = "phvbo8an.pfb";
  fileADOBEkb["uhvr8a.pfb"] = "phvr8a.pfb";
  fileADOBEkb["uhvr8ac.pfb"] = "phvr8an.pfb";
  fileADOBEkb["uhvro8a.pfb"] = "phvro8a.pfb";
  fileADOBEkb["uhvro8ac.pfb"] = "phvro8an.pfb";
  fileADOBEkb["uncb8a.pfb"] = "pncb8a.pfb";
  fileADOBEkb["uncbi8a.pfb"] = "pncbi8a.pfb";
  fileADOBEkb["uncr8a.pfb"] = "pncr8a.pfb";
  fileADOBEkb["uncri8a.pfb"] = "pncri8a.pfb";
  fileADOBEkb["uplb8a.pfb"] = "pplb8a.pfb";
  fileADOBEkb["uplbi8a.pfb"] = "pplbi8a.pfb";
  fileADOBEkb["uplr8a.pfb"] = "pplr8a.pfb";
  fileADOBEkb["uplri8a.pfb"] = "pplri8a.pfb";
  fileADOBEkb["usyr.pfb"] = "psyr.pfb";
  fileADOBEkb["utmb8a.pfb"] = "ptmb8a.pfb";
  fileADOBEkb["utmbi8a.pfb"] = "ptmbi8a.pfb";
  fileADOBEkb["utmr8a.pfb"] = "ptmr8a.pfb";
  fileADOBEkb["utmri8a.pfb"] = "ptmri8a.pfb";
  fileADOBEkb["uzcmi8a.pfb"] = "pzcmi8a.pfb";
  fileADOBEkb["uzdr.pfb"] = "pzdr.pfb";

  psADOBE["URWGothicL-Demi"] = "AvantGarde-Demi";
  psADOBE["URWGothicL-DemiObli"] = "AvantGarde-DemiOblique";
  psADOBE["URWGothicL-Book"] = "AvantGarde-Book";
  psADOBE["URWGothicL-BookObli"] = "AvantGarde-BookOblique";
  psADOBE["URWBookmanL-DemiBold"] = "Bookman-Demi";
  psADOBE["URWBookmanL-DemiBoldItal"] = "Bookman-DemiItalic";
  psADOBE["URWBookmanL-Ligh"] = "Bookman-Light";
  psADOBE["URWBookmanL-LighItal"] = "Bookman-LightItalic";
  psADOBE["NimbusMonL-Bold"] = "Courier-Bold";
  psADOBE["NimbusMonL-BoldObli"] = "Courier-BoldOblique";
  psADOBE["NimbusMonL-Regu"] = "Courier";
  psADOBE["NimbusMonL-ReguObli"] = "Courier-Oblique";
  psADOBE["NimbusSanL-Bold"] = "Helvetica-Bold";
  psADOBE["NimbusSanL-BoldCond"] = "Helvetica-Narrow-Bold";
  psADOBE["NimbusSanL-BoldItal"] = "Helvetica-BoldOblique";
  psADOBE["NimbusSanL-BoldCondItal"] = "Helvetica-Narrow-BoldOblique";
  psADOBE["NimbusSanL-Regu"] = "Helvetica";
  psADOBE["NimbusSanL-ReguCond"] = "Helvetica-Narrow";
  psADOBE["NimbusSanL-ReguItal"] = "Helvetica-Oblique";
  psADOBE["NimbusSanL-ReguCondItal"] = "Helvetica-Narrow-Oblique";
  psADOBE["CenturySchL-Bold"] = "NewCenturySchlbk-Bold";
  psADOBE["CenturySchL-BoldItal"] = "NewCenturySchlbk-BoldItalic";
  psADOBE["CenturySchL-Roma"] = "NewCenturySchlbk-Roman";
  psADOBE["CenturySchL-Ital"] = "NewCenturySchlbk-Italic";
  psADOBE["URWPalladioL-Bold"] = "Palatino-Bold";
  psADOBE["URWPalladioL-BoldItal"] = "Palatino-BoldItalic";
  psADOBE["URWPalladioL-Roma"] = "Palatino-Roman";
  psADOBE["URWPalladioL-Ital"] = "Palatino-Italic";
  psADOBE["StandardSymL"] = "Symbol";
  psADOBE["NimbusRomNo9L-Medi"] = "Times-Bold";
  psADOBE["NimbusRomNo9L-MediItal"] = "Times-BoldItalic";
  psADOBE["NimbusRomNo9L-Regu"] = "Times-Roman";
  psADOBE["NimbusRomNo9L-ReguItal"] = "Times-Italic";
  psADOBE["URWChanceryL-MediItal"] = "ZapfChancery-MediumItalic";
  psADOBE["Dingbats"] = "ZapfDingbats";
}

/* _________________________________________________________________________

   MakeFontMapApp::LocateMap
   _________________________________________________________________________ */

bool
MakeFontMapApp::LocateMap (/*[in]*/ const char *	lpszFileName,
			   /*[out]*/ PathName &		path,
			   /*[in]*/ bool		mustExist)
{
  bool found = pSession->FindFile(lpszFileName, FileType::MAP, path);
  if (! found && mustExist)
    {
      Abort (T_("Font map file %s could not be found."), Q_(lpszFileName));
    }
#if 0
  if (! found)
    {
      Verbose (T_("Not using map file %s"), Q_(lpszFileName));
    }
#endif
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
%%% DO NOT EDIT THIS FILE! It will be replaced when MiKTeX is updated."));
  writer.WriteLine (T_("\
%%% Run the following command to edit a local version of this file:"));
  writer.WriteLine ("\
%%%   initexmf --edit-config-file updmap");
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
      writer.WriteFormatted ("%s", it->texName.c_str());
      writer.WriteFormatted (" %s", it->psName.c_str());
      if (! it->specialInstructions.empty())
	{
	  writer.WriteFormatted (" \" %s \"",
				 it->specialInstructions.c_str());
	}
      for (Tokenizer tok (it->headerList.c_str(), ";");
	   tok.GetCurrent() != 0;
	   ++ tok)
	{
	  writer.WriteFormatted (" %s", tok.GetCurrent());
	}
      writer.WriteLine ();
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::GetInstructionParam
   _________________________________________________________________________ */

bool
MakeFontMapApp::GetInstructionParam (/*[in]*/ const string &	str,
				     /*[in]*/ const string &	instruction,
				     /*[out]*/ string &	param)
{
  param = "";
  for (Tokenizer tok (str.c_str(), " \t"); tok.GetCurrent() != 0; ++ tok)
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
      string field1 = it->texName;
      string field2;
      if (! it->encFile.empty())
	{
	  field2 = PathName(it->encFile).GetFileNameWithoutExtension().Get();
	}
      string field3;
      if (! it->fontFile.empty())
	{
	  field3 = PathName(it->fontFile).GetFileNameWithoutExtension().Get();
	}
      else if (it->texName != it->psName)
	{
	  field3 = it->psName;
	}
      string options;
      string param;
      if (GetInstructionParam(it->specialInstructions,
			      "ExtendFont",
			      param))
	{
	  options += " -e ";
	  options += param;
	}
      if (GetInstructionParam(it->specialInstructions,
			      "SlantFont",
			      param))
	{
	  options += " -s ";
	  options += param;
	}
      if (it->texName.substr(0, 2) == "cm"
	  || it->texName.substr(0, 2) == "eu"
	  || (it->texName.substr(0, 2) == "la"
	      && ! (it->encFile.substr(0, 12) == "cm-super-t2a"))
	  || (it->texName.substr(0, 2) == "lc"
	      && ! (it->encFile.substr(0, 12) == "cm-super-t2c"))
	  || it->texName.substr(0, 4) == "line"
	  || it->texName.substr(0, 4) == "msam"
	  || it->texName.substr(0, 2) == "xy")
	{
	  if (! (it->fontFile.substr(0, 4)  == "fmex"))
	    {
	      options += " -r";
	    }
	}
      if (field2 == "" && field3 == "" && options == "")
	{
	  continue;
	}
      writer.Write (field1);
      if (! field2.empty())
	{
	  writer.WriteFormatted (" %s", field2.c_str());
	}
      else if (! field3.empty())
	{
	  writer.WriteFormatted (" %s", "default");
	}
      if (! field3.empty())
	{
	  writer.WriteFormatted (" %s", field3.c_str());
	}
      if (! options.empty())
	{
	  writer.WriteFormatted ("%s", options.c_str());
	}
      writer.WriteLine ();
    }
}

/* _________________________________________________________________________

   MakeFontMapApp::CreateOutputDir
   _________________________________________________________________________ */

PathName
MakeFontMapApp::CreateOutputDir (/*[in]*/ const char * lpszRelPath)
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

  string line;

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
	  MapError ("%s", e.what());
	}
    }

  reader.Close ();
}

/* _________________________________________________________________________

   MakeFontMapApp::ReadMap
   _________________________________________________________________________ */

void
MakeFontMapApp::ReadMap (/*[in]*/ const string &		fileName,
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
MakeFontMapApp::CatMaps (/*[in]*/ const set<string> &	fileNames)
{
  set<FontMapEntry> result;
  for (set<string>::const_iterator it = fileNames.begin();
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
(/*[in]*/ const map<string, string> &	transMap,
 /*[in,out]*/ FontMapEntry &		fontMapEntry)
{
  map<string, string>::const_iterator it;
  it = transMap.find(fontMapEntry.fontFile);
  if (it != transMap.end())
    {
      fontMapEntry.fontFile = it->second;
    }
  Tokenizer header (fontMapEntry.headerList.c_str(), ";");
  fontMapEntry.headerList = "";
  for (; header.GetCurrent() != 0; ++ header)
    {
      if (! fontMapEntry.headerList.empty())
	{
	  fontMapEntry.headerList += ';';
	}
      const char * lpsz;
      for (lpsz = header.GetCurrent();
	   *lpsz == '<' || *lpsz == '[';
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
MakeFontMapApp::TranslatePSName (/*[in]*/ const map<string, string> &	names,
				 /*[in,out]*/ FontMapEntry &	fontMapEntry)
{
  map<string, string>::const_iterator it;
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
	       (dvipsPreferOutline ? "psfonts_t1" : "psfonts_pk"),
	       ".map");
  CopyFile (pathSrc, PathName(dvipsOutputDir, "psfonts.map"));

  pathSrc.Set (dvipdfmOutputDir,
	       (dvipdfmDownloadBase14 ? "dvipdfm_dl14" : "dvipdfm_ndl14"),
	       ".map");
  CopyFile (pathSrc, PathName(dvipdfmOutputDir, "dvipdfm.map"));
#if CREATE_DEPRECATED_MAP_FILES
  CopyFile (pathSrc, PathName(dvipdfmOutputDir, "psfonts.map"));
#endif

  pathSrc.Set (pdftexOutputDir,
	       (pdftexDownloadBase14 ? "pdftex_dl14" : "pdftex_ndl14"),
	       ".map");
  CopyFile (pathSrc, PathName(pdftexOutputDir, "pdftex.map"));
#if CREATE_DEPRECATED_MAP_FILES
  CopyFile (pathSrc, PathName(pdftexOutputDir, "psfonts.map"));
#endif
}

/* _________________________________________________________________________

   MakeFontMapApp::BuildFontconfigCache
   _________________________________________________________________________ */

static const char * const topDirs[] = {
  "fonts" MIKTEX_PATH_DIRECTORY_DELIMITER_STRING "type1",
  "fonts" MIKTEX_PATH_DIRECTORY_DELIMITER_STRING "opentype",
  "fonts" MIKTEX_PATH_DIRECTORY_DELIMITER_STRING "truetype",
};

void
MakeFontMapApp::BuildFontconfigCache ()
{
  pSession->ConfigureFile (MIKTEX_PATH_FONTCONFIG_CONFIG_FILE);
  PathName configFile (pSession->GetSpecialPath(SpecialPath::ConfigRoot));
  configFile += MIKTEX_PATH_FONTCONFIG_LOCALFONTS_FILE;
  StreamWriter writer (configFile);
  writer.WriteLine ("<?xml version=\"1.0\"?>");
  writer.WriteLine ();
  writer.WriteLine ("<!--");
  writer.WriteLine (T_("\
  DO NOT EDIT THIS FILE! It will be replaced when MiKTeX is updated."));
  writer.WriteFormattedLine (T_("\
  Instead, edit the configuration file %s."),
			     MIKTEX_LOCALFONTS2_CONF);
  writer.WriteLine ("-->");
  writer.WriteLine ();
  writer.WriteLine ("<fontconfig>");
  writer.WriteFormattedLine ("<include>%s</include>", MIKTEX_LOCALFONTS2_CONF);
  vector<string> paths;
  for (CSVList path
	 (pSession->GetLocalFontDirectories().c_str(),
	  PathName::PathNameDelimiter);
       path.GetCurrent() != 0;
       ++ path)
    {
      paths.push_back (path.GetCurrent());
    }
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
  for (vector<string>::const_iterator it = paths.begin();
       it != paths.end();
       ++ it)
    {
      writer.WriteFormattedLine ("<dir>%s</dir>", it->c_str());
    }
  writer.WriteLine ("</fontconfig>");
  writer.Close ();
  configFile.RemoveFileSpec ();
  configFile += MIKTEX_LOCALFONTS2_CONF;
  if (! File::Exists(configFile))
    {
      StreamWriter writer (configFile);
      writer.WriteLine ("<?xml version=\"1.0\"?>");
      writer.WriteLine ("<fontconfig>");
      writer.WriteLine (T_("<!-- REMOVE THIS LINE"));
      writer.WriteLine (T_("<dir>Your font directory here</dir>"));
      writer.WriteLine (T_("<dir>Your font directory here</dir>"));
      writer.WriteLine (T_("<dir>Your font directory here</dir>"));
      writer.WriteLine (T_("     REMOVE THIS LINE -->"));
      writer.WriteLine ("</fontconfig>");
      writer.Close ();
    }
  PathName pathFcCache;
  if (! pSession->FindFile ("fc-cache", FileType::EXE, pathFcCache))
    {
      Abort (T_("The fc-cache executable could not be found."));
    }
  CommandLineBuilder arguments;
  if (adminMode)
  {
    arguments.AppendOption ("--miktex-admin");
  }
  arguments.AppendOption ("--force");
  if (verbose)
    {
      arguments.AppendOption ("--verbose");
    }
  Process::Run (pathFcCache, arguments.Get());
}

/* _________________________________________________________________________

   MakeFontMapApp::MakeMaps
   _________________________________________________________________________ */

bool
HasPaintType (/*[in]*/ const FontMapEntry & fontMapEntry)
{
  return (fontMapEntry.specialInstructions.find("PaintType") != string::npos);
}

void
MakeFontMapApp::MakeMaps ()
{
  set<FontMapEntry> dvips35;
  ReadMap ("dvips35.map", dvips35, true);
  set<FontMapEntry> pdftex35;
  ReadMap ("pdftex35.map", pdftex35, true);
  set<FontMapEntry> dvipdfm35;
  ReadMap ("dvipdfm35.map", dvipdfm35, true);
  set<FontMapEntry> ps2pk35;
  ReadMap ("ps2pk35.map", ps2pk35, true);

  set<FontMapEntry> transLW35_ps2pk35 (TranslateLW35(ps2pk35));

  set<FontMapEntry> transLW35_dvips35 (TranslateLW35(dvips35));

  set<FontMapEntry> transLW35_pdftex35 (TranslateLW35(pdftex35));

  set<FontMapEntry> transLW35_dvipdfm35 (TranslateLW35(dvipdfm35));

  set<FontMapEntry> tmp1 (CatMaps(mixedMapFiles));

  set<FontMapEntry> tmp2 (CatMaps(mapFiles));

  WriteDvipsMapFile ("ps2pk.map", transLW35_ps2pk35, tmp1, tmp2);

  set<FontMapEntry> empty;

  WriteDvipsMapFile ("download35.map", transLW35_ps2pk35, empty, empty);

  WriteDvipsMapFile ("builtin35.map", transLW35_dvips35, empty, empty);

  set<FontMapEntry> transLW35_dftdvips
    (TranslateLW35(dvipsDownloadBase35 ? ps2pk35 : dvips35));

  WriteDvipsMapFile ("psfonts_t1.map", transLW35_dftdvips, tmp1, tmp2);

  WriteDvipsMapFile ("psfonts_pk.map", transLW35_dftdvips, empty, tmp2);

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

  WritePdfTeXMapFile ("pdftex_ndl14.map", tmp3, empty, empty);
  WritePdfTeXMapFile ("pdftex_dl14.map", tmp7, empty, empty);

  WriteDvipdfmMapFile ("dvipdfm_dl14.map", tmp7, empty, empty);
  WriteDvipdfmMapFile ("dvipdfm_ndl14.map", tmp6, empty, empty);

  CopyFiles ();

  BuildFontconfigCache ();
}

/* _________________________________________________________________________

   MakeFontMapApp::Run
   _________________________________________________________________________ */

void
MakeFontMapApp::Run (/*[in]*/ int		argc,
		     /*[in]*/ const char **	argv)
{
  ProcessOptions (argc, argv);
  Session::InitInfo initInfo (argv[0]);
  if (adminMode)
  {
    Verbose (T_("Entering administrative mode..."));
    initInfo.AddFlags (Session::InitFlags::AdminMode);
  }
  pSession.CreateSession (initInfo);
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
	  /*[in]*/ const char **	argv)
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
