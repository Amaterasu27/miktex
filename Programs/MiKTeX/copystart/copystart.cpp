/* copystart.cpp: MiKTeX copy starter

   Copyright (C) 2001-2007 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

#include <miktex/Core/Core>
#include <miktex/Core/Environment>

#define UNUSED_ALWAYS(x) (x)

using namespace MiKTeX::Core;
using namespace std;

/* _________________________________________________________________________

   CopyStart
   _________________________________________________________________________ */

void
CopyStart (/*[in]*/ const char *	lpszFileName,
	   /*[in]*/ const char *	lpszArgs)
{
  SessionWrapper pSession (Session::InitInfo("copystart"));

  MD5 digest = MD5::FromFile(lpszFileName);

  // set MIKTEX_BINDIR
  Utils::SetEnvironmentString (MIKTEX_ENV_BIN_DIR,
			       pSession->GetMyLocation().Get());

  PathName pathExe;

  bool alreadyCopied = false;

  // create the destination file name
  pathExe.SetToTempDirectory ();
  pathExe += PathName(lpszFileName).GetFileNameWithoutExtension();
  pathExe.SetExtension (".tmp");
  if (File::Exists(pathExe))
    {
      if (MD5::FromFile(pathExe.Get()) == digest)
	{
	  alreadyCopied = true;
	}
      else
	{
	  File::Delete (pathExe, true);
	}
    }

  if (! alreadyCopied)
    {
      // copy the executable
      File::Copy (lpszFileName, pathExe);
      if (! IsWindowsNT() || pSession->RunningAsAdministrator())
	{
	  // schedule the removal of the destination file
	  pSession->ScheduleFileRemoval (pathExe.Get());
	}
    }

  // start the executable
  Process::Start (pathExe.Get(), lpszArgs);
}

/* _________________________________________________________________________

   WinMain
   _________________________________________________________________________ */

int
WINAPI
WinMain (/*[in]*/ HINSTANCE	hInstance,
	 /*[in]*/ HINSTANCE	hPrevInstance,
	 /*[in]*/ char *	lpCmdLine,
	 /*[in]*/ int		nCmdShow)
{
  UNUSED_ALWAYS (hInstance);
  UNUSED_ALWAYS (hPrevInstance);

  try
    {
      if (lpCmdLine == 0 || *lpCmdLine == 0)
	{
	  throw (MIKTEXTEXT("Internal error (1)."));
	}

      bool quoted = (*lpCmdLine == '"');

      char endChar = (quoted ? '"' : ' ');

      if (quoted)
	{
	  ++ lpCmdLine;
	  if (*lpCmdLine == 0)
	    {
	      throw (MIKTEXTEXT("Internal error (2)."));
	    }
	}

      string exe;

      for (; *lpCmdLine != endChar && *lpCmdLine != 0; ++ lpCmdLine)
	{
	  exe += *lpCmdLine;
	}

      if (quoted && *lpCmdLine == 0)
	{
	  throw (MIKTEXTEXT("Internal error (3)."));
	}

      if (quoted)
	{
	  ++ lpCmdLine;
	}

      CopyStart (exe.c_str(), lpCmdLine);

      return (0);
    }
  catch (const MiKTeXException & e)
    {
      MessageBox (0, e.what(), "MiKTeX", MB_OK | MB_ICONSTOP);
      return (1);
    }
  catch (const exception & e)
    {
      MessageBox (0, e.what(), "MiKTeX", MB_OK | MB_ICONSTOP);
      return (1);
    }
  catch (const char * lpszMessage)
    {
      MessageBox (0, lpszMessage, "MiKTeX", MB_OK | MB_ICONSTOP);
      return (1);
    }
}