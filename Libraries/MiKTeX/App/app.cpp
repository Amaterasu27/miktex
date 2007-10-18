/* app.cpp:

   Copyright (C) 2005-2007 Christian Schenk
 
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
MIKTEXCALL
Application::Cancelled ()
{
  return (cancelled == 0 ? false : true);
}

/* _________________________________________________________________________
     
   Application::CheckCancel
   _________________________________________________________________________ */

void
MIKTEXCALL
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

MIKTEXAPPEXPORT
MIKTEXAPPCALL
Application::Application ()
  : enableInstaller (TriState::Undetermined),
    initialized (false)
{
}

/* _________________________________________________________________________

   Application::~Application
   _________________________________________________________________________ */

MIKTEXAPPCALL
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

MIKTEXAPPAPI(void)
Application::Init (/*[in]*/ const Session::InitInfo & initInfo)
{
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

MIKTEXAPPAPI(void)
Application::Init (/*[in]*/ const char *	lpszProgramInvocationName,
		   /*[in]*/ const char *	lpszTheNameOfTheGame)
{
  Session::InitInfo initInfo (lpszProgramInvocationName);
  if (lpszTheNameOfTheGame != 0)
    {
      initInfo.SetTheNameOfTheGame (lpszTheNameOfTheGame);
    }
#if defined(MIKTEX_WINDOWS)
  initInfo.SetFlags (Session::InitFlags::InitializeCOM);
#endif
  Init (initInfo);
}

/* _________________________________________________________________________
     
   Application::Init
   _________________________________________________________________________ */

MIKTEXAPPAPI(void)
Application::Init (/*[in]*/ const char *	lpszProgramInvocationName)
{
  Init (lpszProgramInvocationName, 0);
}

/* _________________________________________________________________________
   
   Application::Finalize
   _________________________________________________________________________ */

MIKTEXAPPAPI(void)
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
  initialized = false;
}

/* _________________________________________________________________________

   Application::ReportLine
   _________________________________________________________________________ */

void
MPMCALL
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
MPMCALL
Application::OnRetryableError (/*[in]*/ const char * lpszMessage)
{
  UNUSED_ALWAYS (lpszMessage);
  return (false);
}

/* _________________________________________________________________________

   Application::OnProgress
   _________________________________________________________________________ */

bool
MPMCALL
Application::OnProgress (/*[in]*/ Notification		nf)
{
  UNUSED_ALWAYS (nf);
  return (true);
}

/* _________________________________________________________________________

   Application::InstallPackage
   _________________________________________________________________________ */

bool
MIKTEXCALL
Application::InstallPackage (/*[in]*/ const char * lpszPackageName,
			     /*[in]*/ const char * lpszTrigger)
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
#if defined(MIKTEX_WINDOWS)
  static bool initUiFrameworkDone = false;
#endif
  if (enableInstaller == TriState::Undetermined)
    {
#if defined(MIKTEX_WINDOWS)
      static bool initUiFrameworkDone = false;
      if (! initUiFrameworkDone)
	{
	  MiKTeX::UI::MFC::InitializeFramework ();
	  initUiFrameworkDone = true;
	}
      UINT msgBoxRet =
	MiKTeX::UI::MFC::InstallPackageMessageBox(0,
						  pPackageManager.Get(),
						  lpszPackageName,
						  lpszTrigger);
      bool doInstall = ((msgBoxRet & MiKTeX::UI::MFC::YES) != 0);
      if ((msgBoxRet & MiKTeX::UI::MFC::DONTASKAGAIN) != 0)
	{
	  enableInstaller =
	    (doInstall ? TriState::True : TriState::False);
	}
#else
      bool doInstall = false;
#endif
      if (! doInstall)
	{
	  ignoredPackages.insert (lpszPackageName);
	  return (false);
	}
    }
#if defined(MIKTEX_WINDOWS)
  string url;
  RepositoryType repositoryType (RepositoryType::Unknown);
  if (PackageManager::TryGetDefaultPackageRepository(repositoryType, url)
      && repositoryType == RepositoryType::Remote)
    {
      if (! initUiFrameworkDone)
	{
	  MiKTeX::UI::MFC::InitializeFramework ();
	  initUiFrameworkDone = true;
	}
      if (! MiKTeX::UI::MFC::ProxyAuthenticationDialog(0))
	{
	  return (false);
	}
    }
#endif
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

MIKTEXAPPAPI(void)
Application::EnableInstaller (/*[in]*/ TriState tri)
{
  enableInstaller = tri;
  pSession->SetFindFileCallback (enableInstaller.Get() == TriState::False
				 ? 0
				 : this);
}
