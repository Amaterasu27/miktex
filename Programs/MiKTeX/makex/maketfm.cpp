/* maketfm.cpp:

   Copyright (C) 1998-2008 Christian Schenk

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

   MakeTfm
   _________________________________________________________________________ */

class MakeTfm : public MakeUtility
{
public:
  virtual
  void
  Run (/*[in]*/ int			argc,
       /*[in]*/ const char **	argv);

private:
  virtual
  void
  CreateDestinationDirectory ();

private:
  virtual
  void
  Usage ();

private:
#if defined(_MSC_VER)
#  pragma warning (disable: 4065)
#endif
  BEGIN_OPTION_MAP(MakeTfm)
  END_OPTION_MAP();
#if defined(_MSC_VER)
#  pragma warning (default: 4065)
#endif

private:
  bool
  MakeFromHBF (/*[in]*/ const char * lpszName);
};

/* _________________________________________________________________________

   MakeTfm::Usage
   _________________________________________________________________________ */

void
MakeTfm::Usage ()
{
  OUT__ << T_("Usage: ") << Utils::GetExeName() << T_(" [OPTION]... name\n\
\n\
This program makes a TeX font metric file.\n\
\n\
NAME is the name of the font, such as 'cmr10'.\n\
\n\
Options:\n\
--debug, -d                         Print debugging information.\n\
--help, -h                          Print this help screen and exit.\n\
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
    0,			no_argument,		0,	0,
  };
}

/* _________________________________________________________________________

   MakeTfm::CreateDestinationDirectory
   _________________________________________________________________________ */

void
MakeTfm::CreateDestinationDirectory ()
{
  // get basic font information (supplier & typeface)
  PathName supplier;
  PathName typeface;
  if (! pSession->GetFontInfo(name.c_str(),
			      supplier.GetBuffer(),
			      typeface.GetBuffer(),
			      0))
    {
      supplier = T_("public");
      typeface = T_("misc");
    }

  // get destination path template
  string templ1 =
    pSession->GetConfigValue(MIKTEX_REGKEY_MAKETFM,
			     MIKTEX_REGVAL_DESTDIR,
			     T_("%R/fonts/tfm/%s/%t"));

  string templ2;
  for (const char * lpsz = templ1.c_str(); *lpsz != 0; ++ lpsz)
    {
      if (lpsz[0] == '%')
	{
	  switch (lpsz[1])
	    {
	    case '%':
	      templ2 += '%';
	      break;
	    case 'R':
	      templ2 += "%R";
	      break;
	    case 's':
	      templ2 += supplier.Get();
	      break;
	    case 't':
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

   MakeTfm::MakeFromHBF
   _________________________________________________________________________ */

bool
MakeTfm::MakeFromHBF (/*[in]*/ const char * lpszName)
{
  // run hbf2gf to make a .pl file
  CommandLineBuilder arguments;
  if (debug)
    {
      arguments.AppendOption ("-q");
    }
  arguments.AppendOption ("-g");
  arguments.AppendArgument (lpszName);
  arguments.AppendArgument (NUMTOSTR(300));
  if (! RunProcess(MIKTEX_HBF2GF_EXE, arguments.Get()))
    {
      return (false);
    }

  // run PLtoTF
  arguments.Clear ();
  arguments.AppendArgument (PathName(0, lpszName, ".pl"));
  arguments.AppendArgument (PathName(0, lpszName, ".tfm"));
  if (! RunProcess(MIKTEX_PLTOTF_EXE, arguments.Get()))
    {
      FatalError (T_("PLtoTF failed on %s."), Q_(lpszName));
    }

  return (true);
}

/* _________________________________________________________________________

   MakeTfm::Run
   _________________________________________________________________________ */

void
MakeTfm::Run (/*[in]*/ int			argc,
	      /*[in]*/ const char **	argv)
{
  // get command line options and name
  int optionIndex = 0;
  GetOptions (argc, argv, aLongOptions, optionIndex);
  if (argc - optionIndex != 1)
    {
      FatalError (T_("Invalid command-line."));
    }
  name = argv[optionIndex];

  // enter the scratch directory
  ScratchDirectory scratchDirectory;
  scratchDirectory.Enter ();

  // create destination directory
  CreateDestinationDirectory ();

  // make TFM file name
  PathName pathTFMName (0, name, ".tfm");

  // make fully qualified destination path name
  PathName pathDest (destinationDirectory, pathTFMName, 0);

  // quit, if destination font file already exists
  if (File::Exists(pathDest))
    {
      Message (T_("TFM file %s already exists.\n"), Q_(pathDest));
      return;
    }

  // try to create the METAFONT if it does not exist
  PathName mfPath;
  bool done = false;
  if (! pSession->FindFile(name.c_str(), FileType::MF, mfPath))
    {
      CommandLineBuilder arguments;
      if (debug)
	{
	  arguments.AppendOption ("--debug");
	}
      if (verbose)
	{
	  arguments.AppendOption ("--verbose");
	}
      if (printOnly)
	{
	  arguments.AppendOption ("--print-only");
	}
      arguments.AppendArgument (name);
      if (! RunProcess(MIKTEX_MAKEMF_EXE, arguments.Get()))
	{
	  // no METAFONT input file; try to make from HBF file
	  if (! MakeFromHBF(name.c_str()))
	    {
	      FatalError (T_("No creation rule for font %s."), Q_(name));
	    }
	  done = true;
	}
    }

  // run METAFONT, if an appropriate input file exists
  if (! done)
    {
      Verbose (T_("Making %s from %s...\n"),
	       Q_(pathTFMName),
	       Q_(mfPath));
      if (! RunMETAFONT(name.c_str(), "cx", 0))
	{
	  FatalError (T_("METAFONT failed on %s."), Q_(name));
	}
      done = true;
    }
  
  // install TFM font file in destination directory
  Install (pathTFMName, pathDest);

  scratchDirectory.Leave ();
}

/* _________________________________________________________________________

   maketfm
   _________________________________________________________________________ */

MKTEXAPI(maketfm) (/*[in]*/ int			argc,
		   /*[in]*/ const char **	argv)
{
  try
    {
      MakeTfm app;
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
