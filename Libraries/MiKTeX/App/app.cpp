/* app.cpp:

   Copyright (C) 2005-2013 Christian Schenk
 
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
    autoAdmin (TriState::Undetermined),
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
     
   Application::Init
   _________________________________________________________________________ */

void
Application::Init (/*[in]*/ const Session::InitInfo & initInfo)
{
  initialized = true;
  pSession.CreateSession (initInfo);
  beQuiet = false;
  EnableInstaller
    (pSession->GetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			      MIKTEX_REGVAL_AUTO_INSTALL,
			      TriState(TriState::Undetermined)));
  autoAdmin = pSession->GetConfigValue(
				       MIKTEX_REGKEY_PACKAGE_MANAGER,
				       MIKTEX_REGVAL_AUTO_ADMIN,
				       TriState(TriState::Undetermined));
  InstallSignalHandler (SIGINT);
  InstallSignalHandler (SIGTERM);
  time_t lastAdminMaintenance = static_cast<time_t>(
    _atoi64(pSession->GetConfigValue(MIKTEX_REGKEY_CORE, MIKTEX_REGVAL_LAST_ADMIN_MAINTENANCE, "0").c_str()));
  PathName mpmDatabasePath (pSession->GetMpmDatabasePathName());
  bool mustRefreshFndb = ! File::Exists(mpmDatabasePath)
    || (! pSession->IsAdminMode() && lastAdminMaintenance + 30 > File::GetLastWriteTime(mpmDatabasePath));
  PathName userLanguageDat = pSession->GetSpecialPath(SpecialPath::UserConfigRoot);
  userLanguageDat += MIKTEX_PATH_LANGUAGE_DAT;
  bool mustRefreshUserLanguageDat = ! pSession->IsAdminMode()
    && File::Exists(userLanguageDat) && lastAdminMaintenance + 30 > File::GetLastWriteTime(userLanguageDat);
  PathName initexmf;
  if ((mustRefreshFndb || mustRefreshUserLanguageDat) && pSession->FindFile(MIKTEX_INITEXMF_EXE, FileType::EXE, initexmf))
  {
    if (mustRefreshFndb)
    {
      SessionWrapper(true)->UnloadFilenameDatabase ();
      if (pSession->IsAdminMode())
      {
	Process::Run (initexmf, "--admin --quiet --update-fndb");
      }
      else
      {
	Process::Run (initexmf, "--quiet --update-fndb");
      }
    }
    if (mustRefreshUserLanguageDat)
    {
      MIKTEX_ASSERT (! pSession->IsAdminMode());
      Process::Run (initexmf, "--quiet --mklangs");
    }
  }
}

/* _________________________________________________________________________
     
   Application::Init
   _________________________________________________________________________ */

void
Application::Init (/*[in,out]*/ vector</*[const]*/ char *> & args)
{
  Session::InitInfo initInfo (args[0]);
  vector</*[const]*/ char *>::iterator it = args.begin();
  while (it != args.end() && *it != 0)
  {
    if (strcmp(*it, "--miktex-admin") == 0)
    {
      initInfo.AddFlags (Session::InitFlags::AdminMode);
      it = args.erase(it);
    }
    else
    {
      ++ it;
    }
  }
  Init (initInfo);
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
      autoAdmin = (((msgBoxRet & MiKTeX::UI::ADMIN) != 0) ? TriState::True : TriState::False);
    }
  string url;
  RepositoryType repositoryType (RepositoryType::Unknown);
  ProxySettings proxySettings;
  if (PackageManager::TryGetDefaultPackageRepository(repositoryType, url)
      && repositoryType == RepositoryType::Remote
      && PackageManager::TryGetProxy(proxySettings)
      && proxySettings.useProxy
      && proxySettings.authenticationRequired
      && proxySettings.user.empty())
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
  bool switchToAdminMode =
    (autoAdmin == TriState::True && ! pSession->IsAdminMode());
  if (switchToAdminMode)
  {
    pSession->SetAdminMode (true);
  }
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
  if (switchToAdminMode)
  {
    pSession->SetAdminMode (false);
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
