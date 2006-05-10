/* makefmt.cpp: make TeX format files

   Copyright (C) 1998-2006 Christian Schenk

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
  OPT_ETEX_COMP,
  OPT_ETEX_EXT,
  OPT_JOB_TIME,
  OPT_NO_DUMP,
  OPT_OMEGA,
  OPT_PDFETEX_COMP,
  OPT_PDFETEX_EXT,
  OPT_PDFTEX,
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
    eTeX,
    pdfTeX,
    pdfETeX,
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
      compatibilityMode (false),
      noDumpPrimitive (false)
  {
  }

public:
  virtual
  void
  Run (/*[in]*/ int			argc,
       /*[in]*/ const MIKTEXCHAR **	argv);

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
    OPTION_ENTRY (OPT_ENGINE,
		  SetEngine(lpszOptArg))
    OPTION_ENTRY (OPT_ETEX_COMP,
		  (engine = Engine::eTeX,
		   compatibilityMode = true))
    OPTION_ENTRY (OPT_ETEX_EXT,
		  (engine = Engine::eTeX,
		   compatibilityMode = false))
    OPTION_ENTRY (OPT_OMEGA,
		  (engine = Engine::Omega,
		   compatibilityMode = false))
    OPTION_ENTRY (OPT_PDFETEX_COMP,
		  (engine = Engine::pdfETeX,
		   compatibilityMode = true))
    OPTION_ENTRY (OPT_PDFETEX_EXT,
		  (engine = Engine::pdfETeX,
		   compatibilityMode = false))
    OPTION_ENTRY (OPT_PDFTEX,
		  (engine = Engine::pdfTeX,
		   compatibilityMode = false))
    OPTION_ENTRY_TRUE (OPT_NO_DUMP, noDumpPrimitive)
    OPTION_ENTRY_SET (OPT_DESTNAME, destinationName)
    OPTION_ENTRY_SET (OPT_JOB_TIME, jobTime)
    OPTION_ENTRY_SET (OPT_PRELOAD, preloadedFormat)
  END_OPTION_MAP();

private:
  void
  SetEngine (/*[in]*/ const MIKTEXCHAR * lpszEngine)
  {
    if (StringCompare(lpszEngine, T_("tex"), true) == 0)
      {
	engine = Engine::TeX;
      }
    else if (StringCompare(lpszEngine, T_("etex"), true) == 0)
      {
	engine = Engine::eTeX;
      }
    else if (StringCompare(lpszEngine, T_("pdftex"), true) == 0)
      {
	engine = Engine::pdfTeX;
      }
    else if (StringCompare(lpszEngine, T_("pdfetex"), true) == 0)
      {
	engine = Engine::pdfETeX;
      }
    else if (StringCompare(lpszEngine, T_("omega"), true) == 0)
      {
	engine = Engine::Omega;
      }
    else
      {
	FatalError (T_("Invalid engine."));
      }
    compatibilityMode = false;
  }

private:
  const MIKTEXCHAR *
  GetEngineName ()
  {
    switch (engine.Get())
      {
      case Engine::TeX:
	return (T_("tex"));
      case Engine::eTeX:
	return (T_("etex"));
      case Engine::pdfTeX:
	return (T_("pdftex"));
      case Engine::pdfETeX:
	return (T_("pdfetex"));
      case Engine::Omega:
	return (T_("omega"));
      }
    throw (1);
  }

private:
  const MIKTEXCHAR *
  GetFormatFileExtension ()
  {
    switch (engine.Get())
      {
      case Engine::TeX:
      case Engine::pdfTeX:
      case Engine::Omega:
	return (T_(".fmt"));
      case Engine::eTeX:
      case Engine::pdfETeX:
	return (T_(".efmt"));
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
    return (engine == Engine::pdfTeX || engine == Engine::pdfETeX);
  }

private:
  bool
  IsExtended ()
    const
  {
    return (engine == Engine::eTeX || engine == Engine::pdfETeX);
  }

private:
  void
  ParsePdfConfigFiles (/*[in,out]*/ vector<tstring> & primitives)
    const;
  
private:
  void
  ParsePdfConfigFile (/*[in]*/ const PathName &		cfgFile,
		      /*[in,out]*/ vector<tstring> &	primitives)
    const;

private:
  void
  InstallPdftexConfigTex ()
    const;
  
private:
  Engine engine;

private:
  bool compatibilityMode;

private:
  PathName destinationName;

private:
  tstring preloadedFormat;

private:
  tstring jobTime;

private:
  bool noDumpPrimitive;
};

/* _________________________________________________________________________

   MakeFmt::Usage
   _________________________________________________________________________ */

void
MakeFmt::Usage ()
{
  OUT__ << T_("Usage: ") << Utils::GetExeName() << T_(" [OPTION]... NAME\n\
\n\
This program makes a format file.\n\
\n\
NAME is the name of the format, such as 'tex'.\n\
\n\
Options:\n\
--debug, -d                         Print debugging information.\n\
--dest-name NAME                    Destination file name.\n\
--engine=ENGINE                     Set the engine.\n\
--etex-compatibility-mode           Create an e-TeX format file in\n\
                                    compatibility mode.\n\
--etex-extended-mode                Create an e-TeX format file in\n\
                                    extended mode.\n\
--help, -h                          Print this help screen and exit.\n\
--job-time=FILE                     Job time is file's modification time.\n\
--no-dump                           Don't issue the \\dump command.\n\
--omega                             Create an Omega format file.\n\
--pdftex                            Create a pdfTeX format file.\n\
--pdfetex-compatibility-mode        Create a pdf-e-TeX format file in\n\
                                    compatibility mode.\n\
--pdfetex-extended-mode             Create a pdf-e-TeX format file in\n\
                                    extended mode.\n\
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
    T_("dest-name"),	required_argument,	0,	OPT_DESTNAME,
    T_("engine"),	required_argument,	0,	OPT_ENGINE,
    T_("etex-compatibility-mode"), no_argument,	0,	OPT_ETEX_COMP,
    T_("etex-extended-mode"), no_argument,	0,	OPT_ETEX_EXT,
    T_("job-time"),		required_argument, 0,	OPT_JOB_TIME,
    T_("no-dump"),		no_argument,	0,	OPT_NO_DUMP,
    T_("omega"),		no_argument,	0,	OPT_OMEGA,
    T_("pdfetex-compatibility-mode"), no_argument, 0,	OPT_PDFETEX_COMP,
    T_("pdfetex-extended-mode"), no_argument,	0,	OPT_PDFETEX_EXT,
    T_("pdftex"),		no_argument,	0,	OPT_PDFTEX,
    T_("preload"),		required_argument, 0,	OPT_PRELOAD,
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
  CreateDirectory (pSession->GetConfigValue(MIKTEX_REGKEY_MAKEFMT,
					    MIKTEX_REGVAL_DESTDIR,
					    DEFAULT_DESTDIR),
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
MakeFmt::ParsePdfConfigFiles (/*[in,out]*/ vector<tstring> & primitives)
  const
{
  // read all configuration files in reverse order
  for (unsigned i = pSession->GetNumberOfTEXMFRoots(); i > 0; -- i)
    {
      PathName root = pSession->GetRootDirectory(i - 1);
      root.AppendDirectoryDelimiter ();
      tstring searchSpec = root.ToString();
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
    const MIKTEXCHAR *	lpszCfgKey;
    const MIKTEXCHAR *	lpszPrimitive;
  };
  const MAPPING mappings[] =
  {
    T_("adjust_spacing"), T_("\\pdfadjustspacing=%1"),
    T_("always_use_pdfpagebox"), T_("\\pdfoptionalwaysusepdfpagebox=%1"),
    T_("compress_level"), T_("\\pdfcompresslevel=%1"),
    T_("decimal_digits"), T_("\\pdfdecimaldigits=%1"),
    T_("dest_margin"), T_("\\pdfdestmargin=%1"),
    T_("horigin"), T_("\\pdfhorigin=%1"),
    T_("image_resolution"), T_("\\pdfimageresolution=%1"),
    T_("link_margin"), T_("\\pdflinkmargin=%1"),
#if 0
    T_("map"), T_("\\pdfmapfile{%1}"),
#endif
    T_("move_chars"), T_("\\pdfmovechars=%1"),
    T_("output_format"), T_("\\pdfoutput=%1"),
    T_("page_height"), T_("\\pdfpageheight=%1"),
    T_("page_width"), T_("\\pdfpagewidth=%1"),
    T_("pdf_inclusion_errorlevel"), T_("\\pdfoptionpdfinclusionerrorlevel=%1"),
    T_("pdf_minorversion"), T_("\\pdfoptionpdfminorversion=%1"),
    T_("pk_resolution"), T_("\\pdfpkresolution=%1"),
    T_("protrude_chars"), T_("\\pdfprotrudechars=%1"),
    T_("thread_margin"), T_("\\pdfthreadmargin=%1"),
    T_("unique_resname"), T_("\\pdfuniqueresname=%1"),
    T_("vorigin"), T_("\\pdfvorigin=%1"),
  };
}

/* _________________________________________________________________________

   ParseLine
   _________________________________________________________________________ */

bool
ParseLine (/*[in]*/ const tstring &	line,
	   /*[out]*/ tstring &		primitive)
{
  Tokenizer tok (line.c_str(), T_(" \t\r\n"));
  if (tok.GetCurrent() == 0)
    {
      return (false);
    }
  tstring key = tok.GetCurrent();
  tok.SetDelim (T_("\r\n"));
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (false);
    }
  tstring val = tok.GetCurrent();
  for (size_t i = 0; i < sizeof(mappings) / sizeof(mappings[0]); ++ i)
    {
      if (key == mappings[i].lpszCfgKey)
	{
	  primitive = mappings[i].lpszPrimitive;
	  tstring::size_type pos = primitive.find(T_("%1"));
	  if (pos != tstring::npos)
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
			     /*[in,out]*/ vector<tstring> &	primitives)
  const
{
  AutoFILE pFile (File::Open(cfgFile, FileMode::Open, FileAccess::Read));
  tstring line;
  while (Utils::ReadUntilDelim(line, T_('\n'), pFile.Get()))
    {
      tstring primitive;
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
  vector<tstring> primitives;
  ParsePdfConfigFiles (primitives);
  PathName configFile (pSession->GetSpecialPath(SpecialPath::ConfigRoot),
		       MIKTEX_PATH_PDFTEXCONFIG_TEX);
  StreamWriter writer (configFile);
  bool havePdfMinorVersion = false;
  for (vector<tstring>::iterator it = primitives.begin();
       it != primitives.end();
       ++ it)
    {
      writer.WriteLine (*it);
      if (it->compare(0, 25, T_("\\pdfoptionpdfminorversion")) == 0)
	{
	  havePdfMinorVersion = true;
	}
    }
  if (! havePdfMinorVersion)
    {
      writer.WriteLine (T_("\\pdfoptionpdfminorversion=4"));
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
	      /*[in]*/ const MIKTEXCHAR **	argv)
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
  formatFile.SetExtension (GetFormatFileExtension());

  // make fully qualified destination file name
  PathName pathDest (destinationDirectory,
		     destinationName,
		     GetFormatFileExtension());
  
  Verbose (T_("Creating the %s format file...\n"), Q_(destinationName));

  // enter the scratch directory
  ScratchDirectory scratchDirectory;
  scratchDirectory.Enter ();

  // make command line
  CommandLineBuilder arguments;
  bool useInputPrimitive = false;
  arguments.AppendOption (T_("--initialize"));
  arguments.AppendOption (T_("--interaction="), T_("nonstopmode"));
  arguments.AppendOption (T_("--halt-on-error"));
  if (destinationName != GetEngineName())
    {
      arguments.AppendOption (T_("--alias="), destinationName);
    }
  if (jobTime.length() > 0)
    {
      arguments.AppendOption (T_("--job-time="), jobTime);
    }
  if (preloadedFormat.length() > 0)
    {
      arguments.AppendArgument (tstring(T_("&")) + preloadedFormat);
    }
  if (IsExtended() && ! compatibilityMode && preloadedFormat.length() == 0)
    {
      arguments.AppendArgument (T_("*\\relax"));
      useInputPrimitive = true;
    }
  if (IsPdf())
    {
      InstallPdftexConfigTex ();
    }
  if (useInputPrimitive)
    {
      arguments.AppendArgument (T_("\\input"));
    }
  arguments.AppendArgument (inputFile);
  if (! noDumpPrimitive)
    {
      arguments.AppendArgument (T_("\\dump"));
    }

  // start the engine
  if (! RunProcess(GetEngineName(), arguments.Get()))
    {
      FatalError (T_("%s failed on %s."), GetEngineName(), Q_(name));
    }

  // install format file
  Install (formatFile, pathDest);

  scratchDirectory.Leave ();
}

/* _________________________________________________________________________

   makefmt
   _________________________________________________________________________ */

MKTEXAPI(makefmt) (/*[in]*/ int			argc,
		   /*[in]*/ const MIKTEXCHAR **	argv)
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
