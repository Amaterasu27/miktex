/* PropSheet.cpp:

   Copyright (C) 2000-2009 Christian Schenk

   This file is part of MiKTeX Options.

   MiKTeX Options is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX Options is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Options; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"
#include "mo.h"

#include "PropSheet.h"

IMPLEMENT_DYNAMIC(PropSheet, CPropertySheet);

/* _________________________________________________________________________

   PropSheet::PropSheet
   _________________________________________________________________________ */

PropSheet::PropSheet (/*[in]*/ PackageManager * pManager)
  : mustBuildFormats (false),
    pProgressDialog (0),
    pSession (true),
    pManager (pManager),
    generalPage (pManager),
    packagesPage (pManager)
{
  m_psh.dwFlags &= ~(PSH_HASHELP);
  AddPage (&generalPage);
  AddPage (&texmfRootsPage);
  AddPage (&formatsPage);
  AddPage (&languagesPage);
  AddPage (&packagesPage);
  if (pSession->IsAdminMode())
  {
    SetTitle (T_("# MiKTeX Options"));
  }
  else
  {
    SetTitle (T_("MiKTeX Options"));
  }
}

/* _________________________________________________________________________

   PropSheet::~PropSheet
   _________________________________________________________________________ */

PropSheet::~PropSheet ()
{
}

/* _________________________________________________________________________

   PropSheet Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PropSheet, CPropertySheet)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PropSheet::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropSheet::OnInitDialog () 
{
  BOOL result = CPropertySheet::OnInitDialog();
  try
    {
      HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
      if (hIcon == 0)
	{
	  FATAL_WINDOWS_ERROR ("CWinApp::LoadIcon", 0);
	}
      SetIcon (hIcon, TRUE);
      ModifyStyleEx (0, WS_EX_CONTEXTHELP);
      m_psh.dwFlags &= ~(PSH_HASHELP);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  return (result);
}

/* _________________________________________________________________________

   PropSheet::OnProcessOutput
   _________________________________________________________________________ */

bool
PropSheet::OnProcessOutput
(/*[in]*/ const void *	pOutput,
 /*[in]*/ size_t	n)
{
  processOutput.append (reinterpret_cast<const char*>(pOutput), n);
  return (! pProgressDialog->HasUserCancelled());
}

/* _________________________________________________________________________

   PropSheet::RunIniTeXMF
   _________________________________________________________________________ */

bool
PropSheet::RunIniTeXMF
  (/*[in]*/ const char *		lpszTitle,
   /*[in]*/ const CommandLineBuilder &	cmdLine,
   /*[in]*/ ProgressDialog *		pProgressDialog)
{
  PathName exePath;

  if (! pSession->FindFile("initexmf", FileType::EXE, exePath))
    {
      FATAL_MIKTEX_ERROR ("PropSheet::RunIniTeXMF",
			  T_("\
The MiKTeX configuration utility could not be found."),
			  0);
    }

  CommandLineBuilder commandLine (cmdLine);

  commandLine.AppendOption ("--verbose");

  if (pSession->IsAdminMode())
  {
    commandLine.AppendOption ("--admin");
  }

  this->pProgressDialog = pProgressDialog;

  if (pProgressDialog->HasUserCancelled())
    {
      return (false);
    }

  pProgressDialog->SetLine (2, lpszTitle);

  pSession->UnloadFilenameDatabase ();

  processOutput = "";
  int exitCode;
  Process::Run (exePath,
		commandLine.Get(),
		this,
		&exitCode,
		0);

  this->pProgressDialog = 0;
      
  if (exitCode == 0)
    {
      return (true);
    }

  if (AfxMessageBox(T_("The MiKTeX configuration utility failed. \
Do you want to see the transcript?"),
		    MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
    {
      TextViewerDialog::DoModal (this,
				 T_("Transcript"),
				 processOutput.c_str());
    }

  return (false);
}

/* _________________________________________________________________________

   PropSheet::BuildFormats
   _________________________________________________________________________ */

void
PropSheet::BuildFormats ()
{
  auto_ptr<ProgressDialog> pProgDlg (ProgressDialog::Create());
  pProgDlg->StartProgressDialog (GetSafeHwnd());
  pProgDlg->SetTitle (T_("MiKTeX Maintenance"));
  pProgDlg->SetLine (1, T_("Creating format file for:"));
  FormatInfo formatInfo;
  for (unsigned idx = 0;
       pSession->GetFormatInfo(idx, formatInfo);
       ++ idx)
    {  
      if (formatInfo.exclude)
	{
	  continue;
	}
      CommandLineBuilder cmdLine;
      cmdLine.AppendOption ("--dump=", formatInfo.name);
      RunIniTeXMF (formatInfo.description.c_str(), cmdLine, pProgDlg.get());
    }
  pProgDlg->StopProgressDialog ();
  pProgDlg.reset ();;
  mustBuildFormats = false;

}
