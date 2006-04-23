/* makemem.cpp:

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

#define OPT_DESTNAME	1

/* _________________________________________________________________________

   MakeMem
   _________________________________________________________________________ */

class MakeMem
  : public MakeUtility
{
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
  BEGIN_OPTION_MAP(MakeMem)
    OPTION_ENTRY_SET(OPT_DESTNAME, destinationName);
  END_OPTION_MAP();
  
private:
  PathName destinationName;
};

/* _________________________________________________________________________

   MakeMem::Usage
   _________________________________________________________________________ */

void
MakeMem::Usage ()
{
  OUT__ << T_("Usage: ") << Utils::GetExeName() << T_(" [OPTION]... NAME\n\
\n\
This program makes a MetaPost mem file.\n\
\n\
NAME is the name of the mem file.\n\
\n\
Options:\n\
--debug, -d                         Print debugging information.\n\
--dest-name NAME                    Destination file name.\n\
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
    T_("dest-name"),	required_argument,	0,	OPT_DESTNAME,
    0,			no_argument,		0,	0,
  };
}

/* _________________________________________________________________________

   MakeMem::CreateDestinationDirectory
   _________________________________________________________________________ */

void
MakeMem::CreateDestinationDirectory ()
{
  CreateDirectory (pSession->GetConfigValue(MIKTEX_REGKEY_MAKEMEM,
					    MIKTEX_REGVAL_DESTDIR,
					    T_("%R/miktex/mem")),
		   destinationDirectory);
}

/* _________________________________________________________________________

   MakeMem::Run
   _________________________________________________________________________ */

void
MakeMem::Run (/*[in]*/ int			argc,
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

  // create destination directory
  CreateDestinationDirectory ();

  // make the mem file name
  PathName memFile (name);
  memFile.SetExtension (T_(".mem"));

  // make fully qualified destination path name
  PathName pathDest (destinationDirectory, destinationName, T_(".mem"));
  
  Verbose (T_("Creating the %s memory file...\n"), Q_(destinationName));

  // enter the scratch directory
  ScratchDirectory scratchDirectory;
  scratchDirectory.Enter ();

  // invoke MetaPost to make the mem file
  CommandLineBuilder arguments;
  arguments.AppendOption (T_("--initialize"));
  arguments.AppendOption (T_("--interaction="), T_("nonstopmode"));
  arguments.AppendOption (T_("--halt-on-error"));
  arguments.AppendArgument (name + T_(" dump"));
  if (! RunProcess(T_("mp"), arguments.Get()))
    {
      FatalError (T_("MetaPost failed on %s."), Q_(name));
    }

  // install the result
  Install (memFile,  pathDest);
  
  scratchDirectory.Leave ();
}

/* _________________________________________________________________________

   makemem
   _________________________________________________________________________ */

MKTEXAPI(makemem) (/*[in]*/ int			argc,
		   /*[in]*/ const MIKTEXCHAR **	argv)
{
  try
    {
      MakeMem app;
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
