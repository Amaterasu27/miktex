/* ErrorDialog.cpp:

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of MiKTeX UI Library.

   MiKTeX UI Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX UI Library; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"
#include "internal.h"
#include "resource.h"

#include <MiKTeX/UI/MFC/ErrorDialog>

/* _________________________________________________________________________

   ErrorDialogImpl
   _________________________________________________________________________ */

class ErrorDialogImpl
  : public CDialog
{
private:
  DECLARE_MESSAGE_MAP();

private:
  enum { IDD = IDD_ERROR };

public:
  ErrorDialogImpl (/*[in]*/ CWnd *			pParent,
		   /*[in]*/ const MiKTeXException &	e);

public:
  ErrorDialogImpl (/*[in]*/ CWnd *		pParent,
		   /*[in]*/ const exception &	e);

private:
  virtual
  BOOL
  OnInitDialog ();

protected:
  afx_msg
  void
  OnCopy ();

private:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

private:
  string
  CreateReport ();

private:
  CString message;

private:
  CString info;

private:
  MiKTeXException miktexException;

private:
  exception stdException;

private:
  bool isMiKTeXException;
};

/* _________________________________________________________________________

   ErrorDialogImpl Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(ErrorDialogImpl, CDialog)
  ON_BN_CLICKED(IDC_COPY, &ErrorDialogImpl::OnCopy)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   ErrorDialogImpl::ErrorDialogImpl
   _________________________________________________________________________ */

ErrorDialogImpl::ErrorDialogImpl (/*[in]*/ CWnd *		pParent,
				  /*[in]*/ const MiKTeXException & e)
  : CDialog (IDD, pParent),
    isMiKTeXException (true),
    miktexException (e),
    message (e.what()),
    info (e.GetInfo().c_str())
{
}

/* _________________________________________________________________________

   ErrorDialogImpl::ErrorDialogImpl
   _________________________________________________________________________ */

ErrorDialogImpl::ErrorDialogImpl (/*[in]*/ CWnd *		pParent,
				  /*[in]*/ const exception & e)
  : CDialog (IDD, pParent),
    isMiKTeXException (false),
    stdException (e),
    message (e.what()),
    info ("")
{
}

/* _________________________________________________________________________

   ErrorDialogImpl::DoDataExchange
   _________________________________________________________________________ */

void
ErrorDialogImpl::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  DDX_Text (pDX, IDC_MESSAGE, message);
  DDX_Text (pDX, IDC_INFO, info);
}

/* _________________________________________________________________________

   ErrorDialogImpl::OnInitDialog
   _________________________________________________________________________ */

BOOL
ErrorDialogImpl::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();
  return (ret);
}

/* _________________________________________________________________________

   ErrorDialog::DoModal
   _________________________________________________________________________ */

INT_PTR
MIKTEXUICALL
ErrorDialog::DoModal (/*[in]*/ CWnd *			pParent,
		      /*[in]*/ const MiKTeXException &	e)
{
  try
    {
      ErrorDialogImpl dlg (pParent, e);
      return (dlg.DoModal());
    }
  catch (const exception & e)
    {
      AfxMessageBox (e.what());
      return (-1);
    }
}

/* _________________________________________________________________________

   ErrorDialog::DoModal
   _________________________________________________________________________ */

INT_PTR
MIKTEXUICALL
ErrorDialog::DoModal (/*[in]*/ CWnd *			pParent,
		      /*[in]*/ const exception &	e)
{
  try
    {
      ErrorDialogImpl dlg (pParent, e);
      return (dlg.DoModal());
    }
  catch (const exception & e)
    {
      AfxMessageBox (e.what());
      return (-1);
    }
}

/* _________________________________________________________________________

   ErrorDialogImpl::OnCopy
   _________________________________________________________________________ */

void
ErrorDialogImpl::OnCopy ()
{
  bool opened = false;
  HGLOBAL hGlobal = 0;
  try
    {
      string report = CreateReport();
      if (! OpenClipboard())
	{
	  FATAL_MIKTEX_ERROR ("ErrorDialogImpl::OnCopy",
			      T_("The Clipboard could not be opened."),
			      0);
	}
      opened = true;
      if (! EmptyClipboard())
	{
	  FATAL_WINDOWS_ERROR ("EmptyClipboard", 0);
	}
      hGlobal =	GlobalAlloc(GMEM_DDESHARE, report.length() + 1);
      if (hGlobal == 0)
	{
	  FATAL_WINDOWS_ERROR ("GlobalAlloc", 0);
	}
      void * pGlobal = GlobalLock(hGlobal);
      if (pGlobal == 0)
	{
	  FATAL_WINDOWS_ERROR ("GlobalLock", 0);
	}
      else
	{
	  AutoGlobal autoGlobal (pGlobal);
	  Utils::CopyString (reinterpret_cast<char *>(pGlobal),
			     report.length() + 1,
			     report.c_str());
	}
      if (SetClipboardData(CF_TEXT, hGlobal) == 0)
	{
	  FATAL_WINDOWS_ERROR ("SetClipboardData", 0);
	}
      hGlobal = 0;
      AfxMessageBox (T_("\
The error report has been copied to the Clipboard."));
    }
  catch (const exception & e)
    {
      AfxMessageBox (e.what());
    }
  if (hGlobal != 0)
    {
      GlobalFree (hGlobal);
    }
  if (opened)
    {
      CloseClipboard ();
    }
}

/* _________________________________________________________________________

   ErrorDialogImpl::CreateReport
   _________________________________________________________________________ */

string
ErrorDialogImpl::CreateReport ()
{
  otstringstream s;
  s << T_("MiKTeX Problem Report") << endl
    << T_("Message: ")
    << (isMiKTeXException ? miktexException.what() : stdException.what())
    << endl;
  if (isMiKTeXException)
    {
      s << T_("Data: ") << miktexException.GetInfo() << endl
	<< T_("Source: ") << miktexException.GetSourceFile() << endl
	<< T_("Line: ") << miktexException.GetSourceLine() << endl;
    }
  Session * pSession = Session::TryGet();
  if (pSession != 0)
    {
      try
	{
	  TriState sharedSetup = pSession->IsSharedMiKTeXSetup();
	  s << "MiKTeX: "
	    << Utils::GetMiKTeXVersionString() << endl
	    << "OS: " << Utils::GetOSVersionString() << endl;
	  if (IsWindowsNT())
	    {
	      s << "SystemAdmin: " << (pSession->RunningAsAdministrator()
				       ? T_("yes")
				       : T_("no"))
		<< endl;
	      s << "PowerUser: " << (pSession->RunningAsPowerUser()
				     ? T_("yes")
				     : T_("no"))
		<< endl;
	    }
	  s << "SharedSetup: " << (sharedSetup == TriState::True
				   ? T_("yes")
				   : (sharedSetup == TriState::False
				      ? T_("no")
				      : T_("unknown")))
	    << endl
	    << "BinDir: "
	    << pSession->GetSpecialPath(SpecialPath::BinDirectory).Get()
	    << endl;
	  for (unsigned idx = 0;
	       idx < pSession->GetNumberOfTEXMFRoots();
	       ++ idx)
	    {
	      PathName absFileName;
	      PathName root = pSession->GetRootDirectory(idx);
	      s << "Root" << idx << ": " << root.Get() << endl;
	    }
	  s << "Install: "
	    << pSession->GetSpecialPath(SpecialPath::InstallRoot).Get()
	    << endl;
	  s << "UserConfig: "
	    << pSession->GetSpecialPath(SpecialPath::UserConfigRoot).Get()
	    << endl;
	  s << "UserData: "
	    << pSession->GetSpecialPath(SpecialPath::UserDataRoot).Get()
	    << endl;
	  if (pSession->IsSharedMiKTeXSetup() == TriState::True)
	    {
	      s << "CommonConfig: "
		<< (pSession
		    ->GetSpecialPath(SpecialPath::CommonConfigRoot).Get())
		<< endl;
	      s << "CommonData: "
		<< pSession->GetSpecialPath(SpecialPath::CommonDataRoot).Get()
		<< endl;
	    }
	}
      catch (const exception &)
	{
	  Session::Release (pSession);
	}
    }
  return (s.str());
}
