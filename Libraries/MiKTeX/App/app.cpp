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

static bool volatile cancelled;

/* _________________________________________________________________________
     
   OnKeybordInterrupt
   _________________________________________________________________________ */

static
void
#if defined(MIKTEX_WINDOWS)
CDECL
#endif
OnKeyboardInterrupt (/*[in]*/ int signalToBeHandled)
{
  UNUSED_ALWAYS (signalToBeHandled);
  signal (SIGINT, SIG_IGN);
  cancelled = true;
  signal (SIGINT, OnKeyboardInterrupt);
}

/* _________________________________________________________________________
     
   Application::Cancelled
   _________________________________________________________________________ */

bool
MIKTEXCALL
Application::Cancelled ()
{
  return (cancelled);
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
      tstring programInvocationName = Utils::GetExeName();
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
     
   Application::Init
   _________________________________________________________________________ */

MIKTEXAPPAPI(void)
Application::Init (/*[in]*/ const Session::InitInfo & initInfo)
{
  initialized = true;
  pSession.CreateSession (initInfo);
#if defined(MIKTEX_WINDOWS)
  MiKTeX::UI::InitializeFramework ();
#endif
  beQuiet = false;
  EnableInstaller
    (pSession->GetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			      MIKTEX_REGVAL_AUTO_INSTALL,
			      TriState(TriState::Undetermined)));
  
  void (* oldHandlerFunc) (int);
  oldHandlerFunc = signal(SIGINT, OnKeyboardInterrupt);
  if (oldHandlerFunc == SIG_ERR)
    {
      FATAL_CRT_ERROR (T_("signal"), 0);
    }
  if (oldHandlerFunc != SIG_DFL)
    {
      if (signal(SIGINT, oldHandlerFunc) == SIG_ERR)
	{
	  FATAL_CRT_ERROR (T_("signal"), 0);
	}
    }
}

/* _________________________________________________________________________
     
   Application::Init
   _________________________________________________________________________ */

MIKTEXAPPAPI(void)
Application::Init (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName)
{
  Init (Session::InitInfo(lpszProgramInvocationName));
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
Application::ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine)
{
  if (! GetQuietFlag())
    {
      FPutS (lpszLine, stdout);
      FPutC (T_('\n'), stdout);
    }
}

/* _________________________________________________________________________

   Application::OnRetryableError
   _________________________________________________________________________ */

bool
MPMCALL
Application::OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage)
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

   Application::RunIniTeXMF
   _________________________________________________________________________ */

void
Application::RunIniTeXMF (/*[in]*/ const MIKTEXCHAR *	lpszArguments)
{
#if defined(MIKTEX_WINDOWS)
  // find initexmf
  PathName exe;
  if (! pSession->FindFile(MIKTEX_INITEXMF_EXE,
			   FileType::EXE,
			   exe))
    {
      FATAL_MIKTEX_ERROR (T_("Application::RunIniTeXMF"),
			  (T_("\
The MiKTeX configuration utility could not be found.")),
			  0);
    }

  // run initexmf.exe
  tstring arguments = lpszArguments;
  if (GetQuietFlag())
    {
      arguments += T_(" --quiet");
    }
  return (Process::Run(exe, arguments.c_str()));
#else
  UNUSED_ALWAYS (lpszArguments);
#  warning Unimplemented::Application::RunIniTeXMF
#endif
}

/* _________________________________________________________________________

   Application::InstallPackage
   _________________________________________________________________________ */

bool
MIKTEXCALL
Application::InstallPackage (/*[in]*/ const MIKTEXCHAR * lpszPackageName,
			     /*[in]*/ const MIKTEXCHAR * lpszTrigger)
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
#if defined(MIKTEX_WINDOWS)
      UINT msgBoxRet =
	InstallPackageMessageBox(0,
				 pPackageManager.Get(),
				 lpszPackageName,
				 lpszTrigger);
      bool doInstall = ((msgBoxRet & MiKTeX::UI::YES) != 0);
      if ((msgBoxRet & MiKTeX::UI::DONTASKAGAIN) != 0)
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
  tstring url;
  RepositoryType repositoryType (RepositoryType::Unknown);
  if (PackageManager::TryGetDefaultPackageRepository(repositoryType, url)
      && repositoryType == RepositoryType::Remote
      && ! ProxyAuthenticationDialog(0))
    {
      return (false);
    }
  if (pInstaller.get() == 0)
    {
      pInstaller.reset (pPackageManager->CreateInstaller());
    }
  pInstaller->SetCallback (this);
  vector<tstring> fileList;
  fileList.push_back (lpszPackageName);
  pInstaller->SetFileLists (fileList, vector<tstring>());
  if (! GetQuietFlag())
    {
      FPutS (T_("\n\
======================================================================\n"),
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
  if (done)
    {
      RunIniTeXMF (T_("--mkmaps"));
    }
  if (! GetQuietFlag())
    {
      FPutS (T_("\
======================================================================\n"),
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
