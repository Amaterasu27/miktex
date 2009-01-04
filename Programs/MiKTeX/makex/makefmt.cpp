/* makefmt.cpp: make TeX format files

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

   Options
   _________________________________________________________________________ */

enum {
  OPT_AAA = 1,
  OPT_DESTNAME,
  OPT_ENGINE,
  OPT_ENGINE_OPTION,
  OPT_JOB_TIME,
  OPT_NO_DUMP,
  OPT_PRELOAD
};

/* _________________________________________________________________________

   Engine
   _________________________________________________________________________ */

class EngineEnum
{
public:
  enum EnumType {
    TeX,
    pdfTeX,
    XeTeX,
    Omega,
  };
};

typedef EnumWrapper<EngineEnum> Engine;

/* _________________________________________________________________________

   MakeFmt
   _________________________________________________________________________ */

class MakeFmt : public MakeUtility
{
public:
  MakeFmt ()
    : engine (Engine::TeX),
      noDumpPrimitive (false)
  {
  }

public:
  virtual
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv);

private:
  virtual
  void
  Usage ();

private:
  virtual
  void
  CreateDestinationDirectory ();

private:
  BEGIN_OPTION_MAP(MakeFmt)
    OPTION_ENTRY (OPT_ENGINE, SetEngine(lpszOptArg))
    OPTION_ENTRY (OPT_ENGINE_OPTION, AppendEngineOption(lpszOptArg))
    OPTION_ENTRY_SET (OPT_DESTNAME, destinationName)
    OPTION_ENTRY_SET (OPT_JOB_TIME, jobTime)
    OPTION_ENTRY_SET (OPT_PRELOAD, preloadedFormat)
    OPTION_ENTRY_TRUE (OPT_NO_DUMP, noDumpPrimitive)
  END_OPTION_MAP();

private:
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
    else if (StringCompare(lpszEngine, "omega", true) == 0)
      {
	engine = Engine::Omega;
      }
    else
      {
	FatalError (T_("Unknown engine: %s"), lpszEngine);
      }
  }

private:
  void
  AppendEngineOption (/*[in]*/ const char * lpszOption)
  {
    engineOptions.Append (lpszOption);
  }

private:
  const char *
  GetEngineName ()
  {
    switch (engine.Get())
      {
      case Engine::TeX:
	return ("tex");
      case Engine::pdfTeX:
	return ("pdftex");
      case Engine::XeTeX:
	return ("xetex");
      case Engine::Omega:
	return ("omega");
      }
    throw (1);
  }

private:
  const char *
  GetEngineExeName ()
  {
    switch (engine.Get())
      {
      case Engine::TeX:
	return (MIKTEX_TEX_EXE);
      case Engine::pdfTeX:
	return (MIKTEX_PDFTEX_EXE);
      case Engine::XeTeX:
	return (MIKTEX_XETEX_EXE);
      case Engine::Omega:
	return (MIKTEX_OMEGA_EXE);
      }
    throw (1);
  }

private:
  void
  FindInputFile (/*[in]*/ const PathName &	inputName,
		 /*[out]*/ PathName &		inputFile);

private:
  bool
  IsPdf ()
    const
  {
    return (engine == Engine::pdfTeX);
  }

private:
  bool
  IsExtended ()
    const
  {
    return (engine == Engine::pdfTeX || engine == Engine::XeTeX);
  }

private:
  void
  ParsePdfConfigFiles (/*[in,out]*/ vector<string> & primitives)
    const;
  
private:
  void
  ParsePdfConfigFile (/*[in]*/ const PathName &		cfgFile,
		      /*[in,out]*/ vector<string> &	primitives)
    const;

private:
  void
  InstallPdftexConfigTex ()
    const;
  
private:
  Engine engine;

private:
  PathName destinationName;

private:
  string preloadedFormat;

private:
  string jobTime;

private:
  bool noDumpPrimitive;

private:
  Argv engineOptions;
};

/* _________________________________________________________________________

   MakeFmt::Usage
   _________________________________________________________________________ */

void
MakeFmt::Usage ()
{
  cout << T_("Usage: ") << Utils::GetExeName() << T_(" [OPTION]... NAME\n\
\n\
This program makes a format file.\n\
\n\
NAME is the name of the format, such as 'tex'.\n\
\n\
Options:\n\
--debug, -d                         Print debugging information.\n\
--dest-name NAME                    Destination file name.\n\
--engine=ENGINE                     Set the engine.\n\
--engine-option=OPTION              Add an engine option.\n\
--help, -h                          Print this help screen and exit.\n\
--job-time=FILE                     Job time is file's modification time.\n\
--no-dump                           Don't issue the \\dump command.\n\
--preload FORMAT                    Format to be preloaded.\n\
--print-only, -n                    Print what commands would be executed.\n\
--verbose, -v                       Print information on what is being done.\n\
--version, -V                       Print the version number and exit.")
       << endl;
}

/* _________________________________________________________________________

   aLongOptions
   _________________________________________________________________________ */

namespace {
  const struct option aLongOptions[] =
  {
    COMMON_OPTIONS,
    "dest-name",	required_argument,	0,	OPT_DESTNAME,
    "engine",		required_argument,	0,	OPT_ENGINE,
    "engine-option",	required_argument,	0,	OPT_ENGINE_OPTION,
    "job-time",		required_argument,	0,	OPT_JOB_TIME,
    "no-dump",		no_argument,		0,	OPT_NO_DUMP,
    "preload",		required_argument,	0,	OPT_PRELOAD,
    0,			no_argument,		0,	0,
  };
}

/* _________________________________________________________________________

   MakeFmt::CreateDestinationDirectory
   _________________________________________________________________________ */

#define DEFAULT_DESTDIR				\
  MIKTEX_PATH_TEXMF_PLACEHOLDER			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_PATH_FMT_DIR

void
MakeFmt::CreateDestinationDirectory ()
{
  PathName defDestDir;
  defDestDir = MIKTEX_PATH_TEXMF_PLACEHOLDER;
  defDestDir += MIKTEX_PATH_FMT_DIR;
  defDestDir += GetEngineName();
  CreateDirectory (pSession->GetConfigValue(MIKTEX_REGKEY_MAKEFMT,
					    MIKTEX_REGVAL_DESTDIR,
					    defDestDir.Get()),
		   destinationDirectory);
}

/* _________________________________________________________________________

   MakeFmt::FindInputFile
   _________________________________________________________________________ */

void
MakeFmt::FindInputFile (/*[in]*/  const PathName &	inputName,
			/*[out]*/ PathName &		inputFile)
{
  if (! pSession->FindFile(inputName, FileType::TEX, inputFile))
    {
      FatalError (T_("The input file could be found."));
    }

  // remove the directory specification from the path; alternatevly,
  // we could convert backslashes into forward slashes
  inputFile.RemoveDirectorySpec ();
}

/* _________________________________________________________________________

   MakeFmt::ParsePdfConfigFiles
   _________________________________________________________________________ */

void
MakeFmt::ParsePdfConfigFiles (/*[in,out]*/ vector<string> & primitives)
  const
{
  // read all configuration files in reverse order
  for (unsigned i = pSession->GetNumberOfTEXMFRoots(); i > 0; -- i)
    {
      PathName root = pSession->GetRootDirectory(i - 1);
      root.AppendDirectoryDelimiter ();
      string searchSpec = root.ToString();
      searchSpec += MIKTEX_PATH_PDFTEX_DIR;
      searchSpec += MIKTEX_PATH_RECURSION_INDICATOR;
      PathName cfgFile;
      if (pSession->FindFile(MIKTEX_PDFTEX_CFG_FILENAME,
			     searchSpec.c_str(),
			     cfgFile))
	{
	  ParsePdfConfigFile (cfgFile, primitives);
	}
    }
}

/* _________________________________________________________________________

   mappings
   _________________________________________________________________________ */

namespace {
  struct MAPPING
  {
    const char *	lpszCfgKey;
    const char *	lpszPrimitive;
  };
  const MAPPING mappings[] =
  {
    "adjust_spacing", "\\pdfadjustspacing=%1",
    "always_use_pdfpagebox", "\\pdfoptionalwaysusepdfpagebox=%1",
    "compress_level", "\\pdfcompresslevel=%1",
    "decimal_digits", "\\pdfdecimaldigits=%1",
    "dest_margin", "\\pdfdestmargin=%1",
    "horigin", "\\pdfhorigin=%1",
    "image_resolution", "\\pdfimageresolution=%1",
    "link_margin", "\\pdflinkmargin=%1",
#if 0
    "map", "\\pdfmapfile{%1}",
#endif
    "move_chars", "\\pdfmovechars=%1",
    "output_format", "\\pdfoutput=%1",
    "page_height", "\\pdfpageheight=%1",
    "page_width", "\\pdfpagewidth=%1",
    "pdf_inclusion_errorlevel", "\\pdfinclusionerrorlevel=%1",
    "pdf_minorversion", "\\pdfminorversion=%1",
    "pk_resolution", "\\pdfpkresolution=%1",
    "protrude_chars", "\\pdfprotrudechars=%1",
    "thread_margin", "\\pdfthreadmargin=%1",
    "unique_resname", "\\pdfuniqueresname=%1",
    "vorigin", "\\pdfvorigin=%1",
  };
}

/* _________________________________________________________________________

   ParseLine
   _________________________________________________________________________ */

bool
ParseLine (/*[in]*/ const string &	line,
	   /*[out]*/ string &		primitive)
{
  Tokenizer tok (line.c_str(), " \t\r\n");
  if (tok.GetCurrent() == 0)
    {
      return (false);
    }
  string key = tok.GetCurrent();
  tok.SetDelim ("\r\n");
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (false);
    }
  string val = tok.GetCurrent();
  for (size_t i = 0; i < sizeof(mappings) / sizeof(mappings[0]); ++ i)
    {
      if (key == mappings[i].lpszCfgKey)
	{
	  primitive = mappings[i].lpszPrimitive;
	  string::size_type pos = primitive.find("%1");
	  if (pos != string::npos)
	    {
	      primitive.replace (pos, 2, val);
	    }
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   MakeFmt::ParsePdfConfigFile
   _________________________________________________________________________ */

void
MakeFmt::ParsePdfConfigFile (/*[in]*/ const PathName &		cfgFile,
			     /*[in,out]*/ vector<string> &	primitives)
  const
{
  AutoFILE pFile (File::Open(cfgFile, FileMode::Open, FileAccess::Read));
  string line;
  while (Utils::ReadUntilDelim(line, '\n', pFile.Get()))
    {
      string primitive;
      if (ParseLine(line, primitive))
	{
	  primitives.push_back (primitive);
	}
    }
  pFile.Reset ();
}

/* _________________________________________________________________________

   MakeFmt::InstallPdftexConfigTex
   _________________________________________________________________________ */

void
MakeFmt::InstallPdftexConfigTex ()
  const
{
  vector<string> primitives;
  ParsePdfConfigFiles (primitives);
  PathName configFile (pSession->GetSpecialPath(SpecialPath::ConfigRoot),
		       MIKTEX_PATH_PDFTEXCONFIG_TEX);
  Directory::Create (PathName(configFile).RemoveFileSpec());
  StreamWriter writer (configFile);
  bool havePdfMinorVersion = false;
  writer.WriteLine (T_("\
%%% DO NOT EDIT THIS FILE! It will be overwritten."));
  writer.WriteLine (T_("\
%%% Run 'initexmf --edit-config-file pdftex' to edit configuration settings"));
  writer.WriteLine (T_("\
%%% for pdfTeX."));
  for (vector<string>::iterator it = primitives.begin();
       it != primitives.end();
       ++ it)
    {
      writer.WriteLine (*it);
      if (it->compare(0, 25, "\\pdfoptionpdfminorversion") == 0
	  || it->compare(0, 16, "\\pdfminorversion") == 0)
	{
	  havePdfMinorVersion = true;
	}
    }
  if (! havePdfMinorVersion)
    {
      writer.WriteLine ("\\pdfminorversion=4");
    }
  writer.Close ();
  if (! Fndb::FileExists(configFile))
    {
      Fndb::Add (configFile);
    }
}

/* _________________________________________________________________________

   MakeFmt::Run
   _________________________________________________________________________ */

void
MakeFmt::Run (/*[in]*/ int			argc,
	      /*[in]*/ const char **	argv)
{
  // get options and file name
  int optionIndex = 0;
  GetOptions (argc, argv, aLongOptions, optionIndex);
  if (argc - optionIndex != 1)
    {
      FatalError (T_("Invalid command-line."));
    }
  name = argv[optionIndex];

  if (destinationName.Empty())
    {
      destinationName = name;
      destinationName.SetExtension (0);
    }

  // pretend to be the engine / format
  if (! (destinationName == GetEngineName()))
    {
      pSession->PushAppName (GetEngineName());
    }
  pSession->PushAppName (destinationName.Get());

  // find the TeX input file
  PathName inputFile;
  FindInputFile (name, inputFile);

  // create destination directory
  CreateDestinationDirectory ();

  // make the format file name
  PathName formatFile (name);
  formatFile.SetExtension (MIKTEX_FORMAT_FILE_SUFFIX);

  // make fully qualified destination file name
  PathName pathDest (destinationDirectory,
		     destinationName,
		     MIKTEX_FORMAT_FILE_SUFFIX);
  
  Verbose (T_("Creating the %s format file...\n"), Q_(destinationName));

  // enter the scratch directory
  ScratchDirectory scratchDirectory;
  scratchDirectory.Enter ();

  // make command line
  CommandLineBuilder arguments;
  arguments.AppendOption ("--initialize");
  arguments.AppendOption ("--interaction=", "nonstopmode");
  arguments.AppendOption ("--halt-on-error");
  if (destinationName != GetEngineName())
    {
      arguments.AppendOption ("--alias=", destinationName);
    }
  if (jobTime.length() > 0)
    {
      arguments.AppendOption ("--job-time=", jobTime);
    }
  arguments.AppendArguments (engineOptions);
  if (preloadedFormat.length() > 0)
    {
      arguments.AppendArgument (string("&") + preloadedFormat);
    }
  if (IsExtended() && preloadedFormat.empty())
    {
      arguments.AppendOption ("--enable-etex");
    }
#if MIKTEX_STANDALONE
  if (IsPdf())
    {
      InstallPdftexConfigTex ();
    }
#endif
  arguments.AppendArgument (inputFile);
  if (! noDumpPrimitive)
    {
      arguments.AppendArgument ("\\dump");
    }

  // start the engine
  if (! RunProcess(GetEngineExeName(), arguments.Get()))
    {
      FatalError (T_("%s failed on %s."), GetEngineExeName(), Q_(name));
    }

  // install format file
  Install (formatFile, pathDest);

  scratchDirectory.Leave ();
}

/* _________________________________________________________________________

   makefmt
   _________________________________________________________________________ */

MKTEXAPI(makefmt) (/*[in]*/ int			argc,
		   /*[in]*/ const char **	argv)
{
  try
    {
      MakeFmt app;
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
