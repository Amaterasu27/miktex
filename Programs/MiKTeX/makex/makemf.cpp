/* makemf.cpp:

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

/* Some algorithms are borrowed from the web2c mktex* shell scripts. */

#include "internal.h"

/* _________________________________________________________________________

   lhpref
   _________________________________________________________________________ */

const char *
const lhpref[] = {
  "la",
  "lb",
  "lc",
  "lh",
  "ll",
  "rx",
  "wn",
  0,
};

/* _________________________________________________________________________

   cspref
   _________________________________________________________________________ */

const char *
const cspref[] = {
  "cs",
  T_("ics"),
  "lcs",
  0,
};

/* _________________________________________________________________________

   cbpref
   _________________________________________________________________________ */

const char *
const cbpref[] = {
  T_("glic"),
  T_("glii"),
  T_("glin"),
  T_("glio"),
  T_("gliu"),
  "gljc",
  "gljn",
  T_("gljo"),
  "glmc",
  T_("glmi"),
  "glmn",
  T_("glmo"),
  T_("glmu"),
  "gltc",
  "gltn",
  T_("glto"),
  "glwc",
  T_("glwi"),
  "glwn",
  T_("glwo"),
  T_("glwu"),
  "glxc",
  T_("glxi"),
  "glxn",
  T_("glxo"),
  T_("glxu"),
  "gmmn",
  T_("gmmo"),
  "gmtr",
  "gmxn",
  T_("gmxo"),
  T_("gomc"),
  T_("gomi"),
  T_("gomn"),
  T_("gomo"),
  T_("gomu"),
  T_("goxc"),
  T_("goxi"),
  T_("goxn"),
  T_("goxo"),
  T_("goxu"),
  "grbl",
  "grmc",
  T_("grmi"),
  "grml",
  "grmn",
  T_("grmo"),
  T_("grmu"),
  "grxc",
  T_("grxi"),
  "grxl",
  "grxn",
  T_("grxo"),
  T_("grxu"),
  "gsmc",
  T_("gsmi"),
  "gsmn",
  T_("gsmo"),
  T_("gsmu"),
  "gsxc",
  T_("gsxi"),
  "gsxn",
  T_("gsxo"),
  T_("gsxu"),
  "gttc",
  T_("gtti"),
  "gttn",
  T_("gtto"),
  T_("gttu"),
  0
};

/* _________________________________________________________________________

   HasPrefix
   _________________________________________________________________________ */

bool
HasPrefix (/*[in]*/ const char *		lpsz,
	   /*[in]*/ const char * const	lpszPrefixes[])
{
  for (size_t i = 0; lpszPrefixes[i] != 0; ++ i)
    {
      if (HasPrefix(lpsz, lpszPrefixes[i]))
	{
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   MakeMf
   _________________________________________________________________________ */

class MakeMf : public MakeUtility
{
public:
  MakeMf ()
    : toStdout (false)
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
  CreateDestinationDirectory ();

private:
  virtual
  void
  Usage ();

private:
  BEGIN_OPTION_MAP(MakeMf)
    OPTION_ENTRY_TRUE('c', toStdout)
  END_OPTION_MAP();

private:
  bool toStdout;

private:
  PathName supplier;

private:
  PathName typeface;
};

/* _________________________________________________________________________

   MakeMf::Usage
   _________________________________________________________________________ */

void
MakeMf::Usage ()
{
  OUT__ << T_("Usage: ") << Utils::GetExeName() << T_(" [OPTION]... name\n\
\n\
This program makes a METAFONT file.\n\
\n\
NAME is the name of the font, such as 'cmr10'.\n\
\n\
Options:\n\
--debug, -d                         Print debugging information.\n\
--help, -h                          Print this help screen and exit.\n\
--print-only, -n                    Print what commands would be executed.\n\
--stdout, -c                        Write MF file on standard output.\n\
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
    T_("stdout"),	no_argument,		0,	'c',
    0,			no_argument,		0,	0,
  };
}

/* _________________________________________________________________________

   MakeMf::CreateDestinationDirectory
   _________________________________________________________________________ */

void
MakeMf::CreateDestinationDirectory ()
{
  string templ;
  templ = "%R";
  templ += PathName::DirectoryDelimiter;
  templ += T_("fonts");
  templ += PathName::DirectoryDelimiter;
  templ += T_("source");
  if (! supplier.Empty())
    {
      templ += PathName::DirectoryDelimiter;
      templ += supplier.Get();
    }
  templ += PathName::DirectoryDelimiter;
  templ += typeface.Get();
  CreateDirectory (templ.c_str(), destinationDirectory);
}

/* _________________________________________________________________________

   MakeMf::Run
   _________________________________________________________________________ */

void
MakeMf::Run (/*[in]*/ int		argc,
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

  // derive TeX font name from name (e.g., "ecbi3583.mf" =>
  // "ecbi3583")
  char szTeXFontname[BufferSizes::MaxPath];
  PathName::Split (name.c_str(),
		   0, 0,
		   szTeXFontname, BufferSizes::MaxPath,
		   0, 0);

  // derive driver name from the TeX font name (e.g., "ecbi3583" =>
  // "ecbi"
  char szDriverName[BufferSizes::MaxPath];
  pSession->SplitFontPath (szTeXFontname,
			   0,
			   0,
			   0,
			   szDriverName,
			   0);

  // find the driver file
  {
    // try a sauterized driver first
    string strSauterDriverName = "b-";
    strSauterDriverName += szDriverName;
    PathName driverPath;
    if (! pSession->FindFile(strSauterDriverName.c_str(),
			     FileType::MF,
			     driverPath))
      {
	// lh fonts get special treatment
	if (HasPrefix(szDriverName, lhpref))
	  {
	    string strLHDriverName;
	    strLHDriverName += szDriverName[0];
	    strLHDriverName += szDriverName[1];
	    strLHDriverName += T_("codes");
	    if (! pSession->FindFile(strLHDriverName.c_str(),
				     FileType::MF,
				     driverPath))
	      {
		FatalError (T_("The %s source file could not be found."),
			    strLHDriverName.c_str());
	      }
	  }
	else if (HasPrefix(szDriverName, cspref))
	  {
	    if (! pSession->FindFile(T_("cscode"), FileType::MF, driverPath))
	      {
		FatalError (T_("The cscode source file could not be found."));
	      }
	  }
	else if (HasPrefix(szDriverName, cbpref))
	  {
	    if (! pSession->FindFile(T_("cbgreek"), FileType::MF, driverPath))
	      {
		FatalError (T_("The cbgreek source file could not be found."));
	      }
	  }
	else if (! pSession->FindFile(szDriverName, FileType::MF, driverPath))
	  {
	    FatalError (T_("The %s source file could not be found."),
			szDriverName);
	  }
      }
  }

  // get information about the font
  double true_pt_size;
  if (! pSession->GetFontInfo(szTeXFontname,
			      supplier.GetBuffer(),
			      typeface.GetBuffer(),
			      &true_pt_size))
    {
      FatalError (T_("No info available for %s."), szTeXFontname);
    }

  // create destination directory
  CreateDestinationDirectory ();

  // open the output stream
  FILE * stream = 0;
  PathName pathDest;
  AutoFILE autoClose;
  if (toStdout || printOnly)
    {
      stream = stdout;
    }
  else
    {
      // make fully qualified destination file name
      pathDest.Set (destinationDirectory, szTeXFontname, ".mf");
      Verbose (T_("Writing on %s...\n"), Q_(pathDest));
      if (! printOnly)
	{
	  stream = File::Open(pathDest, FileMode::Create, FileAccess::Write);
	  autoClose.Attach (stream);
	}
    }

  PrintOnly (T_("cat <<__END__ > %s"), Q_(pathDest));

  if (HasPrefix(szTeXFontname, "ec") || HasPrefix(szTeXFontname, "tc"))
    {
      fprintf (stream, T_("if unknown exbase: input exbase fi;\n"));
      fprintf (stream, T_("gensize:=%0.2f;\n"), true_pt_size);
      fprintf (stream, T_("generate %s;\n"), szDriverName);
    }
  else if (HasPrefix(szTeXFontname, "dc"))
    {
      fprintf (stream, T_("if unknown dxbase: input dxbase fi;\n"));
      fprintf (stream, T_("gensize:=%f;\n"), true_pt_size);
      fprintf (stream, T_("generate %s;\n"), szDriverName);
    }
  else if (HasPrefix(szTeXFontname, lhpref))
    {
      fprintf (stream, T_("input fikparm;\n"));
    }
  else if (HasPrefix(szTeXFontname, cspref))
    {
      fprintf (stream, T_("input cscode\nuse_driver;\n"));
    }
  else if (HasPrefix(szTeXFontname, cbpref))
    {
      fprintf (stream, T_("input cbgreek;\n"));
    }
  else
    {
      fprintf (stream, T_("design_size:=%f;\n"), true_pt_size);
      fprintf (stream, T_("input b-%s;\n"), szDriverName);
    }

  PrintOnly (T_("__END__"));
  
  // close output stream
  if (stream != stdout)
    {
      autoClose.Reset ();

      // add to file name database
      if (! Fndb::FileExists(pathDest))
	{
	  Fndb::Add (pathDest);
	}
    }
}

/* _________________________________________________________________________

   makemf
   _________________________________________________________________________ */

MKTEXAPI(makemf) (/*[in]*/ int			argc,
		  /*[in]*/ const char **	argv)
{
  try
    {
      MakeMf app;
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
