/* makepk.c:

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

#define OPT_MAP_FILE 1
#define OPT_FORCE 2

/* _________________________________________________________________________

   MakePk
   _________________________________________________________________________ */

class MakePk : public MakeUtility
{
public:
  MakePk ()
    : overwriteExisting (false)
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
  BEGIN_OPTION_MAP(MakePk)
    OPTION_ENTRY (OPT_MAP_FILE, mapFiles.push_back(lpszOptArg))
    OPTION_ENTRY_TRUE(OPT_FORCE, overwriteExisting)
  END_OPTION_MAP();

private:
  void
  MakeModeName (/*[out]*/ tstring &	mode,
		/*[in]*/ int		bdpi);

private:
  void
  MakePKFilename (/*[in]*/ const MIKTEXCHAR *	lpszName,
		  /*[in]*/ int			bdpi,
		  /*[in]*/ int			dpi,
		  /*[out]*/ PathName &		result);

private:
  void
  CheckOptions (/*[in,out]*/ int *		pBaseDPI,
		/*[in]*/ int			dpi,
		/*[in]*/ const MIKTEXCHAR *	lpszMode);
  
private:
  void
  ExtraPS2PKOptions (/*[in]*/ const FontMapEntry &	mapEntry,
		     /*[out]*/ CommandLineBuilder &	arguments);

private:
  void
  RunGSF2PK (/*[in]*/ const FontMapEntry &	mapEntry,
	     /*[in]*/ const MIKTEXCHAR *	lpszPkName,
	     /*[in]*/ int			dpi);

private:
  void
  RunPS2PK (/*[in]*/ const FontMapEntry &		mapEntry,
	    /*[in]*/ const MIKTEXCHAR *		lpszPkName,
	    /*[in]*/ int			dpi);

private:
  bool
  FindFontMapping (/*[in]*/ const MIKTEXCHAR *	lpszTeXFontName,
		   /*[in]*/ const MIKTEXCHAR *	lpszMapFileName,
		   /*[out]*/ FontMapEntry &	mapEntry);

private:
  bool
  SearchPostScriptFont (/*[in]*/ const MIKTEXCHAR *	lpszTeXFontName,
			/*[out]*/ FontMapEntry &	mapEntry);

private:
  bool
  IsHbf (/*[in]*/ const MIKTEXCHAR * lpszName);

private:
  bool overwriteExisting;

private:
  bool modeless;

private:
  int dpi;

private:
  int bdpi;

private:
  tstring magnification;

private:
  tstring mfMode;

private:
  vector<tstring> mapFiles;
};

/* _________________________________________________________________________

   MakePk::Usage
   _________________________________________________________________________ */

void
MakePk::Usage ()
{
  OUT__ << T_("Usage: ") << Utils::GetExeName()
	<< T_(" [OPTION]... name dpi bdpi magnification [MODE]\n\
\n\
This program makes a PK font.\n\
\n\
NAME is the name of the TeX font, such as 'cmr10'.  DPI is\n\
the resolution the font is needed at.  BDPI is the base\n\
resolution, useful for figuring out the mode to make the font\n\
in.  MAGNIFICATION is a string to pass to METAFONT as the\n\
magnification.  MODE, if supplied, is the mode to use.\n\
\n\
You can specify 0 as BDPI. In that case, BDPI is calculated from\n\
the MODE.\n\
\n\
Options:\n\
--debug, -d                         Print debugging information.\n\
--force                             Make PK font, even if it exists already.\n\
--help, -h                          Print this help screen and exit.\n\
--map-file=FILE                     Consult additional map file.\n\
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
    T_("force"),		no_argument,		0,	OPT_FORCE,
    T_("map-file"),		required_argument,	0,	OPT_MAP_FILE,
    0,				no_argument,		0,	0,
  };
}

/* _________________________________________________________________________

   MakePk::CreateDestinationDirectory
   _________________________________________________________________________ */

void
MakePk::CreateDestinationDirectory ()
{
  // get basic font information (supplier & typeface)
  PathName supplier;
  PathName typeface;
  if (! pSession->GetFontInfo(name.c_str(),
			      supplier.GetBuffer(),
			      typeface.GetBuffer(),
			      0))
    {
      Verbose (T_("No information available for font %s.\n"), Q_(name));
      supplier = T_("unknown");
      typeface = T_("unknown");
    }
  if (supplier.Empty())
    {
      supplier = T_("unknown");
    }
  if (typeface.Empty())
    {
      typeface = T_("unknown");
    }

  // get destination path template
  tstring templ1
    = pSession->GetConfigValue(MIKTEX_REGKEY_MAKEPK,
			       MIKTEX_REGVAL_DESTDIR,
			       T_("%R/fonts/pk/%m/%s/%t/dpi%d"));

  tstring templ2;
  for (const MIKTEXCHAR * lpsz = templ1.c_str(); *lpsz != 0; ++ lpsz)
    {
      if (lpsz[0] == T_('%'))
	{
	  switch (lpsz[1])
	    {
	    default:
	      break;
	    case T_('%'):
	      templ2 += T_('%');
	      break;
	    case T_('R'):
	      templ2 += T_("%R");
	      break;
	    case T_('b'):
	      templ2 += NUMTOSTR(bdpi);
	      break;
	    case T_('d'):
	      templ2 += NUMTOSTR(dpi);
	      break;
	    case T_('m'):
	      templ2 += (modeless ? T_("modeless") : mfMode);
	      break;
	    case T_('s'):
	      templ2 += supplier.Get();
	      break;
	    case T_('t'):
	      templ2 += typeface.Get();
	      break;
	    }
	  ++ lpsz;
	}
      else
	{
	  templ2 += *lpsz;
	}
    }
  
  // create destination directory
  CreateDirectory (templ2.c_str(), destinationDirectory);
}

/* _________________________________________________________________________

   MakePk::MakePKFilename
   _________________________________________________________________________ */

void
MakePk::MakePKFilename (/*[in]*/ const MIKTEXCHAR *	lpszName,
			/*[in]*/ int			bdpi,
			/*[in]*/ int			dpi,
			/*[out]*/ PathName &		result)
{
  tstring templ =
    pSession->GetConfigValue(MIKTEX_REGKEY_CORE,
			     MIKTEX_REGVAL_PK_FILE_NAME,
			     T_("%f.pk"));
  tstring temp;
  for (const MIKTEXCHAR * lpsz = templ.c_str(); *lpsz != 0; ++ lpsz)
    {
      if (lpsz[0] == T_('%'))
	{
	  switch (lpsz[1])
	    {
	    case T_('%'):
	      temp += T_('%');
	      break;
	    case T_('f'):
	      temp += lpszName;
	      break;
	    case T_('b'):
	      temp += NUMTOSTR(bdpi);
	      break;
	    case T_('d'):
	      temp += NUMTOSTR(dpi);
	      break;
	    }
	  ++ lpsz;
	}
      else
	{
	  temp += *lpsz;
	}
    }
  result = temp;
}

/* _________________________________________________________________________

   MakePk::MakeModeName
   _________________________________________________________________________ */

void
MakePk::MakeModeName (/*[out]*/ tstring &	mode,
		      /*[in]*/ int		bdpi)
{
  if (bdpi == 0)
    {
      FatalError (T_("Neither BDPI nor MODE were specified."));
    }
  MIKTEXMFMODE mfmode;
  if (! pSession->DetermineMETAFONTMode(bdpi, &mfmode))
    {
      FatalError (T_("The METAFONT mode could not be determined."));
    }
  mode = mfmode.szMnemonic;
}

/* _________________________________________________________________________

   GetInstructionParam
   _________________________________________________________________________ */

bool
GetInstructionParam (/*[in]*/ const tstring &	str,
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

   MakePk::ExtraPS2PKOptions
   _________________________________________________________________________ */

void
MakePk::ExtraPS2PKOptions (/*[in]*/ const FontMapEntry &	mapEntry,
			   /*[out]*/ CommandLineBuilder &	arguments)
{
  if (mapEntry.encFile.length() > 0)
    {
      arguments.AppendOption (T_("-e"));
      arguments.AppendArgument (mapEntry.encFile);
    }

  tstring param;

  if (GetInstructionParam(mapEntry.specialInstructions,
			  T_("ExtendFont"),
			  param))
    {
      arguments.AppendOption (T_("-E"));
      arguments.AppendArgument (param);
    }

  if (GetInstructionParam(mapEntry.specialInstructions,
			  T_("SlantFont"),
			  param))
    {
      arguments.AppendOption (T_("-S"));
      arguments.AppendArgument (param);
    }
}

/* _________________________________________________________________________

   MakePk::RunGSF2PK
   _________________________________________________________________________ */

void
MakePk::RunGSF2PK (/*[in]*/ const FontMapEntry &	mapEntry,
		   /*[in]*/ const MIKTEXCHAR *		lpszPkName,
		   /*[in]*/ int				dpi)
{
  CommandLineBuilder arguments;
  arguments.AppendArgument (mapEntry.texName);
  arguments.AppendArgument (mapEntry.psName);
  arguments.AppendArgument (mapEntry.specialInstructions);
  arguments.AppendArgument (mapEntry.encFile);
  arguments.AppendArgument (mapEntry.fontFile);
  arguments.AppendArgument (NUMTOSTR(dpi));
  arguments.AppendArgument (lpszPkName);
  if (! RunProcess(T_("gsf2pk"), arguments.Get()))
    {
      FatalError (T_("GSF2PK failed on %s."), Q_(mapEntry.fontFile));
    }
}

/* _________________________________________________________________________

   MakePk::RunPS2PK
   _________________________________________________________________________ */

void
MakePk::RunPS2PK (/*[in]*/ const FontMapEntry &	mapEntry,
		  /*[in]*/ const MIKTEXCHAR *	lpszPkName,
		  /*[in]*/ int			dpi)
{
  bool oldFonts = false;	// <fixme/>

  CommandLineBuilder arguments;

  if (verbose)
    {
      arguments.AppendOption (T_("-v"));
    }

  if (oldFonts)
    {
      arguments.AppendOption (T_("-O"));
    }

  arguments.AppendOption (T_("-X"), NUMTOSTR(dpi));

  ExtraPS2PKOptions (mapEntry, arguments);

  arguments.AppendArgument (mapEntry.fontFile);

  arguments.AppendArgument (lpszPkName);

  if (! RunProcess(T_("ps2pk"), arguments.Get()))
    {
      FatalError (T_("PS2PK failed on %s."), Q_(mapEntry.fontFile));
    }
}

/* _________________________________________________________________________

   MakePk::CheckOptions
   _________________________________________________________________________ */

void
MakePk::CheckOptions (/*[in,out]*/ int *		pBaseDPI,
		      /*[in]*/ int			dpi,
		      /*[in]*/ const MIKTEXCHAR *	lpszMode)
{
  UNUSED_ALWAYS (dpi);
  MIKTEXMFMODE mfmode;
#if defined(_MSC_VER)
  // inhibit compiler warning C4701
  memset (&mfmode, 0, sizeof(mfmode));
#endif
  int i = 0;
  bool found = false;
  while (! found && pSession->GetMETAFONTMode(i, &mfmode))
    {
      if (StringCompare(lpszMode, mfmode.szMnemonic) == 0)
	{
	  found = true;
	}
      else
	{
	  ++ i;
	}
    }
  if (! found)
    {
      FatalError (T_("%s is an unknown METAFONT mode."), Q_(lpszMode));
    }
  else
    {
      if (*pBaseDPI == 0)
	{
	  *pBaseDPI = mfmode.iHorzRes;
	}
      if (mfmode.iHorzRes != *pBaseDPI)
	{
	  FatalError (T_("\
Specified BDPI (%d) doesn't match %s resolution (%d)."),
		      *pBaseDPI,
		      Q_(lpszMode),
		      mfmode.iHorzRes);
	}
    }
}

/* _________________________________________________________________________

   MakePk::FindFontMapping
   _________________________________________________________________________ */

bool
MakePk::FindFontMapping (/*[in]*/ const MIKTEXCHAR *	lpszTeXFontName,
			 /*[in]*/ const MIKTEXCHAR *	lpszMapFileName,
			 /*[out]*/ FontMapEntry &	mapEntry)
{
  // locate the map file
  PathName mapFile;
  if (! pSession->FindFile(lpszMapFileName, FileType::MAP, mapFile))
    {
      return (false);
    }

  // open the map file
  AutoFILE pStream (File::Open(mapFile, FileMode::Open, FileAccess::Read));

  // try to find the font mapping
  bool found = false;
  tstring line;
  while (! found && Utils::ReadUntilDelim(line, T_('\n'), pStream.Get()))
    {
      if (Utils::ParseDvipsMapLine(line, mapEntry)
	  && mapEntry.texName == lpszTeXFontName)
	{
	  found = true;
	}
    }

  pStream.Reset ();

  return (found);
}

/* _________________________________________________________________________

   MakePk::SearchPostScriptFont
   _________________________________________________________________________ */

bool
MakePk::SearchPostScriptFont (/*[in]*/ const MIKTEXCHAR * lpszTeXFontName,
			      /*[out]*/ FontMapEntry &	mapEntry)
{
  // search via "ps2pk.map" (also used for gsf2pk)
  if (FindFontMapping(lpszTeXFontName, T_("ps2pk.map"), mapEntry))
    {
      return (true);
    }

  // search via user supplied map files
  for (vector<tstring>::iterator it = mapFiles.begin();
       it != mapFiles.end();
       ++ it)
    {
      if (FindFontMapping(lpszTeXFontName, it->c_str(), mapEntry))
	{
	  return (mapEntry.fontFile.length() > 0);
	}
    }

  return (false);
}

/* _________________________________________________________________________

   MakePk::IsHbf
   _________________________________________________________________________ */

bool
MakePk::IsHbf (/*[in]*/	const MIKTEXCHAR *	lpszName)
{
  PathName hbfcfg (lpszName);
  size_t l = hbfcfg.GetLength();
  if (l > 2)
    {
      hbfcfg[ l - 2 ] = 0;
    }
  hbfcfg.SetExtension (T_(".cfg"), false);
  PathName path;
  return (pSession->FindFile(hbfcfg, T_("%R/HBF2GF//"), path));
}

/* _________________________________________________________________________

   MakePk::Run
   _________________________________________________________________________ */

void
MakePk::Run (/*[in]*/ int			argc,
	     /*[in]*/ const MIKTEXCHAR **	argv)
{
  // get command line options and arguments
  int optionIndex = 0;
  GetOptions (argc, argv, aLongOptions, optionIndex);
  if (argc - optionIndex < 4 || argc - optionIndex > 5)
    {
      FatalError (T_("Invalid command-line."));
    }
  name = argv[optionIndex++];
  dpi = _ttoi(argv[optionIndex++]);
  bdpi = _ttoi(argv[optionIndex++]);
  magnification = argv[optionIndex++];
  if (optionIndex < argc)
    {
      mfMode = argv[optionIndex++];
    }

  Verbose (T_("Trying to make PK font %s at %d DPI...\n"),
	   Q_(name),
	   static_cast<int>(dpi));

  // make a mode name if none was specified
  if (mfMode.empty() || mfMode == T_("default"))
    {
      MakeModeName (mfMode, bdpi);
    }
      
  // validate command-line arguments
  CheckOptions (&bdpi, dpi, mfMode.c_str());

  // enter the scratch directory
  ScratchDirectory scratchDirectory;
  scratchDirectory.Enter ();

  tstring gfName (name);
  gfName += T_('.');
  gfName += NUMTOSTR(dpi);
  gfName += T_("gf");

  FontMapEntry mapEntry;

  modeless = false;

  bool isTTF = false;
  bool isHBF = false;

  bool haveSource = false;

  PathName path;

  // try to convert a METAFONT file
  if (pSession->FindFile(name.c_str(), FileType::MF, path))
    {
      haveSource = true;
    }
  else
    {
      CommandLineBuilder arguments;
      if (debug)
	{
	  arguments.AppendOption (T_("--debug"));
	}
      if (verbose)
	{
	  arguments.AppendOption (T_("--verbose"));
	}
      if (printOnly)
	{
	  arguments.AppendOption (T_("--print-only"));
	}
      arguments.AppendArgument (name);
      if (RunProcess(T_("makemf"), arguments.Get()))
	{
	  haveSource = true;
	}
    }

  // try to convert a TTF file
  if (! haveSource)
    {
      CommandLineBuilder arguments;
      arguments.AppendOption (T_("-q")); // suppress informational output
      arguments.AppendOption (T_("-t")); // test for font (returns 0 on succ.)
      arguments.AppendArgument (name);
      if (RunProcess(T_("ttf2pk"), arguments.Get()))
	{
	  arguments.Clear ();
	  if (! debug)
	    {
	      arguments.AppendOption (T_("-q"));
	    }
	  arguments.AppendOption (T_("-n")); // only use '.pk' as extension
	  arguments.AppendArgument (name);
	  arguments.AppendArgument (NUMTOSTR(dpi));
	  if (RunProcess(T_("ttf2pk"), arguments.Get()))
	    {
	      isTTF = true;
	      modeless = true;
	      haveSource = true;
	    }
	}
    }

  // try to convert an HBF file
  if (! haveSource && IsHbf(name.c_str()))
    {
      CommandLineBuilder arguments;
      if (debug)
	{
	  arguments.AppendOption (T_("-q"));
	}
      arguments.AppendOption (T_("-p"));
      arguments.AppendArgument (name);
      arguments.AppendArgument (NUMTOSTR(dpi));
      if (RunProcess(T_("hbf2gf"), arguments.Get()))
	{
	  isHBF = true;
	  modeless = true;
	  haveSource = true;
	}
    }

  // try to find a PFB file
  if (! haveSource && SearchPostScriptFont(name.c_str(), mapEntry))
    {
      modeless = true;
      haveSource = true;
    }

  if (! haveSource)
    {
      FatalError (T_("PK font %s could not be created."), Q_(name));
    }
      
  // create destination directory
  CreateDestinationDirectory ();
      
  // make PK file name
  PathName pkName;
  MakePKFilename (name.c_str(), bdpi, dpi, pkName);
      
  // make fully qualified destination file name
  PathName pathDest (0, destinationDirectory.Get(), pkName.Get(), 0);

  // quit, if destination file already exists
  if (File::Exists(pathDest))
    {
      Message (T_("The PK font file %s already exists.\n"), Q_(pathDest));
      if (! overwriteExisting)
	{
	  return;
	}
    }
      
  Verbose (T_("Creating %s...\n"), Q_(pkName));
  
  // now make the font
  if (modeless)
    {
      if (isTTF)
	{
	  // ttf2pk made it already
	}
      else if (isHBF)
	{
	  // convert GF file into PK file
	  CommandLineBuilder arguments;
	  arguments.AppendArgument (gfName);
	  arguments.AppendArgument (pkName);
	  if (! RunProcess(T_("gftopk"), arguments.Get()))
	    {
	      FatalError (T_("GFtoPK failed on %s."), Q_(gfName));
	    }
	}
      else
	{
	  bool done = false;
	  // run gsf2pk/ps2pk to make a PK font from the PFB file
	  try
	    {
	      RunGSF2PK (mapEntry, pkName.Get(), dpi);
	      done = true;
	    }
	  catch (int)
	    {
	    }
	  if (! done)
	    {
	      RunPS2PK (mapEntry, pkName.Get(), dpi);
	    }
	}
    }
  else
    {
      // run METAFONT/GFtoPK to make a PK font
      if (! RunMETAFONT(name.c_str(),
			mfMode.c_str(),
			magnification.c_str()))
	{
	  FatalError (T_("METAFONT failed on %s."), Q_(name));
	}
      CommandLineBuilder arguments;
      arguments.AppendArgument (gfName);
      arguments.AppendArgument (pkName);
      if (! RunProcess(T_("gftopk"), arguments.Get()))
	{
	  FatalError (T_("GFtoPK failed on %s."), Q_(gfName));
	}
    }
  
  // install PK font file
  Install (pkName, pathDest);

  scratchDirectory.Leave ();
}

/* _________________________________________________________________________

   makepk
   _________________________________________________________________________ */

MKTEXAPI(makepk) (/*[in]*/ int			argc,
		  /*[in]*/ const MIKTEXCHAR **	argv)
{
  try
    {
      MakePk app;
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
