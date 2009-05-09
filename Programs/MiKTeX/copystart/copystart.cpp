/* copystart.cpp: MiKTeX copy starter

   Copyright (C) 2001-2009 Christian Schenk

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
#include <miktex/Core/Paths>

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

  // set MIKTEX_*STARTUPFILE
  if (pSession->IsMiKTeXPortable())
  {
    PathName startupConfig (pSession->GetSpecialPath(SpecialPath::InstallRoot));
    startupConfig += MIKTEX_PATH_STARTUP_CONFIG_FILE;
    Utils::SetEnvironmentString (MIKTEX_ENV_COMMON_STARTUP_FILE,
				 startupConfig.Get());
    Utils::SetEnvironmentString (MIKTEX_ENV_USER_STARTUP_FILE,
				 startupConfig.Get());
  }

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
      char szMyPath[MAX_PATH];

      if (GetModuleFileNameA(0, szMyPath, MAX_PATH) == 0)
      {
	throw (MIKTEXTEXT("Internal error (4)."));
      }

      char szMyDrive[MAX_PATH];
      char szMyDir[MAX_PATH];
      char szMyName[MAX_PATH];
      char szMySuffix[MAX_PATH];

      if (_splitpath_s(szMyPath,
		       szMyDrive, MAX_PATH,
		       szMyDir, MAX_PATH,
		       szMyName, MAX_PATH,
		       szMySuffix, MAX_PATH)
	  != 0)
      {
	throw (MIKTEXTEXT("Internal error (5)."));
      }

      strcat_s (szMyName, MAX_PATH, szMySuffix);

      string exe;

      if (strcmp(szMyName, MIKTEX_COPYSTART_EXE) == 0
          || strcmp(szMyName, MIKTEX_COPYSTART_ADMIN_EXE) == 0)
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
      }
      else
      {
	exe = szMyDrive;
	exe += szMyDir;
	exe += "internal\\";
	exe += szMyName;
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
