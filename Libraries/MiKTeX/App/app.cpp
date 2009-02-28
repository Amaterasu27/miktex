/* app.cpp:

   Copyright (C) 2005-2009 Christian Schenk
 
   This file is part of the MiKTeX App Library.

   The MiKTeX App Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX App Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX App Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________
     
   initUiFrameworkDone
   _________________________________________________________________________ */

static bool initUiFrameworkDone = false;

/* _________________________________________________________________________
     
   cancelled
   _________________________________________________________________________ */

static volatile sig_atomic_t cancelled;

/* _________________________________________________________________________
     
   OnKeybordInterrupt
   _________________________________________________________________________ */

static
void
#if defined(MIKTEX_WINDOWS)
CDECL
#endif
SignalHandler (/*[in]*/ int signalToBeHandled)
{
  switch (signalToBeHandled)
    {
    case SIGINT:
    case SIGTERM:
      signal (SIGINT, SIG_IGN);
      cancelled = true;
      break;
    }
}

/* _________________________________________________________________________
     
   Application::Cancelled
   _________________________________________________________________________ */

bool
Application::Cancelled ()
{
  return (cancelled == 0 ? false : true);
}

/* _________________________________________________________________________
     
   Application::CheckCancel
   _________________________________________________________________________ */

void
Application::CheckCancel ()
{
  if (Cancelled())
    {
      string programInvocationName = Utils::GetExeName();
      throw MiKTeXException (programInvocationName.c_str(),
			     T_("Operation cancelled (Ctrl-C)."),
			     0,
			     0,
			     0);
    }
}

/* _________________________________________________________________________

   Application::Application
   _________________________________________________________________________ */

Application::Application ()
  : enableInstaller (TriState::Undetermined),
    initialized (false)
{
}

/* _________________________________________________________________________

   Application::~Application
   _________________________________________________________________________ */

Application::~Application ()
{
  try
    {
      if (initialized)
	{
	  Finalize ();
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________
     
   InstallSignalHandler
   _________________________________________________________________________ */

void
InstallSignalHandler (/*[in]*/ int sig)
{
  void (* oldHandlerFunc) (int);
  oldHandlerFunc = signal(sig, SignalHandler);
  if (oldHandlerFunc == SIG_ERR)
    {
      FATAL_CRT_ERROR ("signal", 0);
    }
  if (oldHandlerFunc != SIG_DFL)
    {
      if (signal(sig, oldHandlerFunc) == SIG_ERR)
	{
	  FATAL_CRT_ERROR ("signal", 0);
	}
    }
}

/* _________________________________________________________________________
     
   Setup
   _________________________________________________________________________ */

static
void
Setup ()
{
  system ("initexmf --update-fndb");
}

/* _________________________________________________________________________
     
   Is1stRun
   _________________________________________________________________________ */

static
bool
Is1stRun ()
{
  PathName testDir;
#if defined(MIKTEX_WINDOWS)
  testDir = Utils::GetFolderPath(CSIDL_LOCAL_APPDATA, CSIDL_APPDATA, true);
  testDir += "MiKTeX";
  testDir += MIKTEX_SERIES_STR;
#else
  const char * lpszHome = getenv("HOME");
  if (lpszHome == 0)
  {
    return (false);
  }
  testDir = lpszHome;
  testDir += ".miktex";
#endif
  return (! Directory::Exists(testDir));
}

/* _________________________________________________________________________
     
   Application::Init
   _________________________________________________________________________ */

void
Application::Init (/*[in]*/ const Session::InitInfo & initInfo)
{
#if 1
  // kludge
  if (Is1stRun())
  {
    Setup ();
  }
#ensif
  initialized = true;
  pSession.CreateSession (initInfo);
  beQuiet = false;
  EnableInstaller
    (pSession->GetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			      MIKTEX_REGVAL_AUTO_INSTALL,
			      TriState(TriState::Undetermined)));
  InstallSignalHandler (SIGINT);
  InstallSignalHandler (SIGTERM);
}

/* _________________________________________________________________________
     
   Application::Init
   _________________________________________________________________________ */

void
Application::Init (/*[in]*/ const char *	lpszProgramInvocationName,
		   /*[in]*/ const char *	lpszTheNameOfTheGame)
{
  Session::InitInfo initInfo (lpszProgramInvocationName);
  if (lpszTheNameOfTheGame != 0)
    {
      initInfo.SetTheNameOfTheGame (lpszTheNameOfTheGame);
    }
#if defined(MIKTEX_WINDOWS) && 0
  initInfo.SetFlags (Session::InitFlags::InitializeCOM);
#endif
  Init (initInfo);
}

/* _________________________________________________________________________
     
   Application::Init
   _________________________________________________________________________ */

void
Application::Init (/*[in]*/ const char *	lpszProgramInvocationName)
{
  Init (lpszProgramInvocationName, 0);
}

/* _________________________________________________________________________
   
   Application::Finalize
   _________________________________________________________________________ */

void
Application::Finalize ()
{
  if (pInstaller.get() != 0)
    {
      pInstaller->Dispose ();
      pInstaller.reset ();
    }
  if (pPackageManager.Get() != 0)
    {
      pPackageManager.Release ();
    }
  pSession.Reset ();
  ignoredPackages.clear ();
  if (initUiFrameworkDone)
    {
      MiKTeX::UI::FinalizeFramework ();      
      initUiFrameworkDone = false;
    }
  initialized = false;
}

/* _________________________________________________________________________

   Application::ReportLine
   _________________________________________________________________________ */

void
Application::ReportLine (/*[in]*/ const char * lpszLine)
{
  if (! GetQuietFlag())
    {
      fputs (lpszLine, stdout);
      putc ('\n', stdout);
    }
}

/* _________________________________________________________________________

   Application::OnRetryableError
   _________________________________________________________________________ */

bool
Application::OnRetryableError (/*[in]*/ const char * lpszMessage)
{
  UNUSED_ALWAYS (lpszMessage);
  return (false);
}

/* _________________________________________________________________________

   Application::OnProgress
   _________________________________________________________________________ */

bool
Application::OnProgress (/*[in]*/ Notification		nf)
{
  UNUSED_ALWAYS (nf);
  return (true);
}

/* _________________________________________________________________________

   Application::InstallPackage
   _________________________________________________________________________ */

bool
Application::InstallPackage (/*[in]*/ const char * lpszPackageName,
			     /*[in]*/ const char * lpszTrigger,
	    		     /*[out]*/ PathName &  installRoot)
{
  if (ignoredPackages.find(lpszPackageName) != ignoredPackages.end())
    {
      return (false);
    }
  if (enableInstaller == TriState::False)
    {
      return (false);
    }
  if (pPackageManager.Get() == 0)
    {
      pPackageManager.Create ();
    }
  if (enableInstaller == TriState::Undetermined)
    {
      if (! initUiFrameworkDone)
	{
	  MiKTeX::UI::InitializeFramework ();
	  initUiFrameworkDone = true;
	}
      bool doInstall = false;
      unsigned int msgBoxRet =
	MiKTeX::UI::InstallPackageMessageBox(pPackageManager.Get(),
					     lpszPackageName,
					     lpszTrigger);
      doInstall = ((msgBoxRet & MiKTeX::UI::YES) != 0);
      if ((msgBoxRet & MiKTeX::UI::DONTASKAGAIN) != 0)
	{
	  enableInstaller =
	    (doInstall ? TriState::True : TriState::False);
	}
      if (! doInstall)
	{
	  ignoredPackages.insert (lpszPackageName);
	  return (false);
	}
    }
  string url;
  RepositoryType repositoryType (RepositoryType::Unknown);
  if (PackageManager::TryGetDefaultPackageRepository(repositoryType, url)
      && repositoryType == RepositoryType::Remote)
    {
      if (! initUiFrameworkDone)
	{
	  MiKTeX::UI::InitializeFramework ();
	  initUiFrameworkDone = true;
	}
      if (! MiKTeX::UI::ProxyAuthenticationDialog())
	{
	  return (false);
	}
    }
  if (pInstaller.get() == 0)
    {
      pInstaller.reset (pPackageManager->CreateInstaller());
    }
  pInstaller->SetCallback (this);
  vector<string> fileList;
  fileList.push_back (lpszPackageName);
  pInstaller->SetFileLists (fileList, vector<string>());
  if (! GetQuietFlag())
    {
      fputs ("\n\
======================================================================\n",
	     stdout);
    }
  bool done = false;
  try
    {
      pInstaller->InstallRemove ();
      installRoot = pSession->GetSpecialPath(SpecialPath::InstallRoot);
      done = true;
    }
  catch (const MiKTeXException & e)
    {
      enableInstaller = TriState::False;
      ignoredPackages.insert (lpszPackageName);
      Utils::PrintException (e);
    }
  if (! GetQuietFlag())
    {
      fputs ("\
======================================================================\n",
	     stdout);
    }
  return (done);
}

/* _________________________________________________________________________
     
   Application::EnableInstaller
   _________________________________________________________________________ */

void
Application::EnableInstaller (/*[in]*/ TriState tri)
{
  enableInstaller = tri;
  pSession->SetFindFileCallback (enableInstaller.Get() == TriState::False
				 ? 0
				 : this);
}
