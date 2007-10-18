/* dvipdft.cpp: run dvipdfm-gswin32-dvipdfm

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of dvipdm.

   dvipdfm is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   dvipdfm is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with dvipdfm; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <cstdlib>
#include <cstdarg>
#include <string>
#include <direct.h>
#include <io.h>

#include <MiKTeX/App/Application>
#include <MiKTeX/Core/Core>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX;
using namespace std;

/* _________________________________________________________________________

   FatalError
   _________________________________________________________________________ */

void
FatalError (/*[in]*/ const char *	lpszFormat,
	    /*[in]*/			...)
{
  fprintf (stderr, "%s: ", Utils::GetExeName().c_str());
  va_list marker;
  va_start (marker, lpszFormat);
  vfprintf (stderr, lpszFormat, marker);
  va_end (marker);
  fputc ('\n', stderr);
  throw (1);
}

/* _________________________________________________________________________

   FatalFileError
   _________________________________________________________________________ */

void
FatalFileError (/*[in]*/ const char * lpszPath)
{
  fprintf (stderr, "%s: ", Utils::GetExeName().c_str());
  perror (lpszPath);
  throw (1);
}

/* _________________________________________________________________________

   BadUsage
   _________________________________________________________________________ */

void
BadUsage ()
{
  FatalError (MIKTEXTEXT("No DVI file name specified."));
}

/* _________________________________________________________________________

   TempDirectory
   _________________________________________________________________________ */

class TempDirectory
{
public:
  TempDirectory ()
  {
    ScratchDirectory::Create (path);
  }

public:
  const char *
  Get ()
    const
  {
    return (path.Get());
  }

public:
  void
  Delete ()
  {
    if (path[0] != 0)
      {
	PathName tmp (path);
	path = "";
	Directory::Delete (tmp, true);
      }
  }

public:
  ~TempDirectory ()
  {
    try
      {
	Delete ();
      }
    catch (const exception &)
      {
      }
  }

private:
  PathName path;
};

/* _________________________________________________________________________

   dvipdft
   _________________________________________________________________________ */

void
dvipdft (/*[in]*/ int		argc,
	 /*[in]*/ const char **	argv)
{
  // must have at least one argument: the DVI file name
  if (argc == 1)
    {
      BadUsage ();
    }

  char szGSExePath[BufferSizes::MaxPath];
  SessionWrapper(true)->GetGhostscript (szGSExePath, 0);

  PathName dvipdfmExe;
  if (! SessionWrapper(true)->FindFile("dvipdfm",
				       FileType::EXE,
				       dvipdfmExe))
    {
      FatalError (MIKTEXTEXT("The Dvipdfm executable could not be found."));
    }

  // create a temporary directory
  TempDirectory tempDir;

  CommandLineBuilder arguments;
  PathName fileNoExt;

  // loop over all arguments except the last one
  for (int i = 1; i < argc - 1; ++ i)
    {
      arguments.AppendArgument (argv[i]);
      if (strcmp(argv[i], "-o") == 0 && i + 1 < argc - 1)
	{
	  ++ i;
	  arguments.AppendArgument (argv[i]);
	  fileNoExt = argv[i];
	  fileNoExt.SetExtension (0);
	}
    }

  const char * lpszUserFilename = argv[argc - 1];
  if (fileNoExt.GetLength() == 0)
    {
      fileNoExt = lpszUserFilename;
      fileNoExt.SetExtension (0);
    }

  // run dvipdfm with the fastest options for the first pass
  arguments.AppendOption ("-e");
  arguments.AppendOption ("-z", "0");
  arguments.AppendArgument (lpszUserFilename);
  int exitCode = 0;
  if (! Process::Run(dvipdfmExe.Get(), arguments.Get(), 0, &exitCode, 0))
    {
      FatalError (MIKTEXTEXT("%s could not be started."), dvipdfmExe.Get());
    }
  if (exitCode != 0)
    {
      throw (exitCode);
    }

  // run GhostScript to create thumbnails
  PathName outFileTemplate (tempDir.Get(), fileNoExt.Get(), "%d");
  arguments.Clear ();
  arguments.AppendOption ("-r", "10");
  arguments.AppendOption ("-dNOPAUSE");
  arguments.AppendOption ("-dBATCH");
  arguments.AppendOption ("-sDEVICE:", "png256");
  arguments.AppendOption ("-sOutputFile:", outFileTemplate.Get());
  arguments.AppendArgument (PathName(0, fileNoExt.Get(), ".pdf").Get());
  if (! Process::Run(szGSExePath, arguments.Get(), 0, &exitCode, 0))
    {
      FatalError (MIKTEXTEXT("%s could not be started."), szGSExePath);
    }
  if (exitCode != 0)
    {
      throw (exitCode);
    }

  // run dvipdfm with the users specified options for the last pass
  arguments.Clear ();
  arguments.AppendOption ("-dt");
  arguments.AppendArguments (argc - 1, &argv[1]);
  printf ("dvipdfm %s\n", arguments.Get());
  Utils::SetEnvironmentString ("MIKTEX_TEMP", tempDir.Get());
  if (! Process::Run(dvipdfmExe.Get(), arguments.Get(), 0, &exitCode, 0))
    {
      FatalError (MIKTEXTEXT("%s could not be started."), dvipdfmExe.Get());
    }
  if (exitCode != 0)
    {
      throw (exitCode);
    }

  tempDir.Delete ();
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
  try
    {
      MiKTeX::App::Application app;
      app.Init (argv[0]);
      dvipdft (argc, argv);
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
