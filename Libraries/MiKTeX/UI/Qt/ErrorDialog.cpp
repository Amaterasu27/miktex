/* ErrorDialog.cpp:

   Copyright (C) 2008 Christian Schenk

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

#include "ErrorDialog.h"

#include <miktex/UI/Qt/ErrorDialog>

/* _________________________________________________________________________

   ErrorDialogImpl::ErrorDialogImpl
   _________________________________________________________________________ */

ErrorDialogImpl::ErrorDialogImpl (/*[in]*/ QWidget *		pParent,
				  /*[in]*/ const MiKTeXException &	e)
  : QDialog (pParent),
    isMiKTeXException (true),
    miktexException (e)
{
  setupUi (this);
  tbMessage->setText (QString::fromLocal8Bit(e.what()));
  tbInfo->setText (QString::fromLocal8Bit(e.what()));
}

/* _________________________________________________________________________

   ErrorDialogImpl::ErrorDialogImpl
   _________________________________________________________________________ */

ErrorDialogImpl::ErrorDialogImpl (/*[in]*/ QWidget *		pParent,
				  /*[in]*/ const exception &	e)
  : QDialog (pParent),
    isMiKTeXException (false),
    stdException (e)
{
  setupUi (this);
  tbMessage->setText (QString::fromLocal8Bit(e.what()));
  tbInfo->setText ("");
}

/* _________________________________________________________________________

   MiKTeX::UI::Qt::ErrorDialog::DoModal
   _________________________________________________________________________ */

int
MiKTeX::UI::Qt::ErrorDialog::DoModal (/*[in]*/ QWidget *	pParent,
				      /*[in]*/ const MiKTeXException &	e)
{
  ErrorDialogImpl dlg (pParent, e);
  return (dlg.exec());
}

/* _________________________________________________________________________

   MiKTeX::UI::Qt::ErrorDialog::DoModal
   _________________________________________________________________________ */

int
MiKTeX::UI::Qt::ErrorDialog::DoModal (/*[in]*/ QWidget *	pParent,
				      /*[in]*/ const exception &	e)
{
  ErrorDialogImpl dlg (pParent, e);
  return (dlg.exec());
}

/* _________________________________________________________________________

   ErrorDialogImpl::on_btnCopy_clicked
   _________________________________________________________________________ */

void
ErrorDialogImpl::on_btnCopy_clicked ()
{
  try
    {
      string report = CreateReport();
      QApplication::clipboard()->setText
	(QString::fromLocal8Bit(report.c_str()));
      QMessageBox::information (this,
				T_("Report Copied"),
				T_("\
The error report has been copied to the Clipboard."));
    }
  catch (const exception & e)
    {
    }
}

/* _________________________________________________________________________

   ErrorDialogImpl::CreateReport
   _________________________________________________________________________ */

string
ErrorDialogImpl::CreateReport ()
{
  ostringstream s;
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
