/* dvipdft.cpp: run dvipdfm-gswin32-dvipdfm

   Copyright (C) 2000-2005 Christian Schenk

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
FatalError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	    /*[in]*/			...)
{
  _ftprintf (stderr, _T("%s: "), Utils::GetExeName().c_str());
  va_list marker;
  va_start (marker, lpszFormat);
  _vftprintf (stderr, lpszFormat, marker);
  va_end (marker);
  fputc ('\n', stderr);
  throw (1);
}

/* _________________________________________________________________________

   FatalFileError
   _________________________________________________________________________ */

void
FatalFileError (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  _ftprintf (stderr, _T("%s: "), Utils::GetExeName().c_str());
  _tperror (lpszPath);
  throw (1);
}

/* _________________________________________________________________________

   BadUsage
   _________________________________________________________________________ */

void
BadUsage ()
{
  FatalError (_T("No DVI file name specified."));
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
  const MIKTEXCHAR *
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
	path = _T("");
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
dvipdft (/*[in]*/ int			argc,
	 /*[in]*/ const MIKTEXCHAR **	argv)
{
  // must have at least one argument: the DVI file name
  if (argc == 1)
    {
      BadUsage ();
    }

  MIKTEXCHAR szGSExePath[BufferSizes::MaxPath];
  SessionWrapper(true)->GetGhostscript (szGSExePath, 0);

  PathName dvipdfmExe;
  if (! SessionWrapper(true)->FindFile(_T("dvipdfm"),
				       FileType::EXE,
				       dvipdfmExe))
    {
      FatalError (_T("The Dvipdfm executable could not be found."));
    }

  // create a temporary directory
  TempDirectory tempDir;

  CommandLineBuilder arguments;
  PathName fileNoExt;

  // loop over all arguments except the last one
  for (int i = 1; i < argc - 1; ++ i)
    {
      arguments.AppendArgument (argv[i]);
      if (_tcscmp(argv[i], _T("-o")) == 0 && i + 1 < argc - 1)
	{
	  ++ i;
	  arguments.AppendArgument (argv[i]);
	  fileNoExt = argv[i];
	  fileNoExt.SetExtension (0);
	}
    }

  const MIKTEXCHAR * lpszUserFilename = argv[argc - 1];
  if (fileNoExt.GetLength() == 0)
    {
      fileNoExt = lpszUserFilename;
      fileNoExt.SetExtension (0);
    }

  // run dvipdfm with the fastest options for the first pass
  arguments.AppendOption (_T("-e"));
  arguments.AppendOption (_T("-z"), _T("0"));
  arguments.AppendArgument (lpszUserFilename);
  int exitCode = 0;
  if (! Process::Run(dvipdfmExe.Get(), arguments.Get(), 0, &exitCode, 0))
    {
      FatalError (_T("%s could not be started."), dvipdfmExe.Get());
    }
  if (exitCode != 0)
    {
      throw (exitCode);
    }

  // run GhostScript to create thumbnails
  PathName outFileTemplate (tempDir.Get(), fileNoExt.Get(), _T("%d"));
  arguments.Clear ();
  arguments.AppendOption (_T("-r"), _T("10"));
  arguments.AppendOption (_T("-dNOPAUSE"));
  arguments.AppendOption (_T("-dBATCH"));
  arguments.AppendOption (_T("-sDEVICE:"), _T("png256"));
  arguments.AppendOption (_T("-sOutputFile:"), outFileTemplate.Get());
  arguments.AppendArgument (PathName(0, fileNoExt.Get(), _T(".pdf")).Get());
  if (! Process::Run(szGSExePath, arguments.Get(), 0, &exitCode, 0))
    {
      FatalError (_T("%s could not be started."), szGSExePath);
    }
  if (exitCode != 0)
    {
      throw (exitCode);
    }

  // run dvipdfm with the users specified options for the last pass
  arguments.Clear ();
  arguments.AppendOption (_T("-dt"));
  arguments.AppendArguments (argc - 1, &argv[1]);
  printf (_T("dvipdfm %s\n"), arguments.Get());
  Utils::SetEnvironmentString (_T("MIKTEX_TEMP"), tempDir.Get());
  if (! Process::Run(dvipdfmExe.Get(), arguments.Get(), 0, &exitCode, 0))
    {
      FatalError (_T("%s could not be started."), dvipdfmExe.Get());
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
main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR **	argv)
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
