/* print.cpp: DVI printing

   Copyright (C) 1996-2011 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include "StdAfx.h"

#include "yap.h"

#include "DviView.h"
#include "Dvidoc.h"
#include "DvipsDialog.h"

/* _________________________________________________________________________

   DviView::OnPreparePrinting
   _________________________________________________________________________ */

BOOL
DviView::OnPreparePrinting (/*[in]*/ CPrintInfo * pInfo)
{
  try
    {
      DviDoc * pDoc = GetDocument ();
      ASSERT_VALID (pDoc);

      pInfo->m_nCurPage = GetCurrentPageIdx() + 1;
      pInfo->SetMinPage (1);
      pInfo->SetMaxPage (pDoc->GetPageCount());
      
      if (! DoPreparePrinting(pInfo))
	{
	  return (FALSE);
	}
      
      if (pInfo->m_bPreview)
	{
	  UNEXPECTED_CONDITION ("DviView::OnPreparePrinting");
	}
      
      int dpi = GetDeviceCaps(pInfo->m_pPD->m_pd.hDC, LOGPIXELSX);
      if (dpi != pDoc->GetPrinterResolution())
	{
	  string str = NUMTOSTR(dpi);
	  str += " vs. ";
	  str += NUMTOSTR(pDoc->GetPrinterResolution());
	  FATAL_MIKTEX_ERROR ("DviView::OnPreparePrinting",
			      T_("METAFONT mode mismatch."),
			      str.c_str());
	}

      return (TRUE);
    }

  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
}

/* _________________________________________________________________________

   DviView::OnBeginPrinting
   _________________________________________________________________________ */

void
DviView::OnBeginPrinting (/*[in]*/ CDC *	pDC,
			  /*[in]*/ CPrintInfo *	pInfo)
{
  try
    {
      CScrollView::OnBeginPrinting (pDC, pInfo);
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      pDoc->BeginDviPrinting (pDC);
    }
  
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   DviView::OnEndPrinting
   _________________________________________________________________________ */

void
DviView::OnEndPrinting (/*[in]*/ CDC *		pDC,
			/*[in]*/ CPrintInfo *	pInfo)
{
  try
    {
      CScrollView::OnEndPrinting (pDC, pInfo);
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      pDoc->EndDviPrinting ();
      pPrintInfo = 0;
    }
  
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   DviView::OnFileDvips
   _________________________________________________________________________ */

void
DviView::OnFileDvips ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      DvipsDialog dlg (this);
      dlg.m_iFirstPage = pDoc->GetPageNum(0);
      dlg.m_iLastPage = pDoc->GetPageNum(pDoc->GetPageCount() - 1);
      dlg.m_nPageRange = 0;
      if (dlg.DoModal() != IDOK)
	{
	  return;
	}
      PrintRange pr;
      switch (dlg.m_nPageRange)
	{
	case 0:			// all
	  pr.nRange = PrintRange::All;
	  break;
	case 1:			// current
	  pr.nRange = PrintRange::Range;
	  pr.nFirst = pr.nLast = GetCurrentPageIdx() + 1;
	  break;
	case 2:			// range
	  pr.nRange = PrintRange::Range;
	  pr.nFirst = dlg.m_iFirstPage;
	  pr.nLast = dlg.m_iLastPage;
	  break;
	}
      switch (dlg.m_nEvenOdd)
	{
	case 0:
	  pr.nEvenOdd = PrintRange::EvenOdd;
	  break;
	case 1:
	  pr.nEvenOdd = PrintRange::EvenOnly;
	  break;
	case 2:
	  pr.nEvenOdd = PrintRange::OddOnly;
	  break;
	}
      PrintPostScript (pDoc->GetPathName(),
		       dlg.GetPrinterName(),
		       pr);
      g_pYapConfig->Save ();
    }
  
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   DviView::OnUpdateFileDvips
   _________________________________________________________________________ */

void
DviView::OnUpdateFileDvips (/*[in]*/ CCmdUI * pCmdUI)
{
  BOOL enable = FALSE;
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  enable = TRUE;
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  pCmdUI->Enable (enable);
}

/* _________________________________________________________________________

   DviView::PrintPostScript
   _________________________________________________________________________ */

void
DviView::PrintPostScript (/*[in]*/ const CString &		dviFileName,
			  /*[in]*/ const CString &		printerName,
			  /*[in]*/ const DviView::PrintRange &	pr)
{
  // locate mtprint.exe
  PathName mtprint;
  if (! SessionWrapper(true)->FindFile("mtprint.exe", FileType::EXE, mtprint))
    {
      FATAL_MIKTEX_ERROR ("DviView::PrintPostScript",
			  T_("The MiKTeX print utility could not be found."),
			  T_("mtprint.exe"));
    }

  Utils::RemoveBlanksFromPathName (mtprint);

  // make command-line
  CommandLineBuilder commandLine;
  commandLine.AppendOption ("--printer=", printerName);
  commandLine.AppendOption ("--print-method=", "ps");
  switch (pr.nRange)
    {
    case PrintRange::All:
      break;
    case PrintRange::Range:
      {
	CString str;
	str.Format ("%d-%d", pr.nFirst, pr.nLast);
	commandLine.AppendOption ("--page-range=", str);
	break;
      }
    }
  switch (pr.nEvenOdd)
    {
    case PrintRange::EvenOnly:
      commandLine.AppendOption ("--even-only");
      break;
    case PrintRange::OddOnly:
      commandLine.AppendOption ("--odd-only");
      break;
    }
  commandLine.AppendArgument (dviFileName);

  auto_ptr<ProgressDialog> pProgDlg (ProgressDialog::Create());
  pProgDlg->StartProgressDialog (GetSafeHwnd());
  pProgDlg->SetTitle (T_("Print"));
  pProgDlg->SetLine (1, T_("Being printed:"));
  pProgDlg->SetLine (2, dviFileName);
  char szBuf[4096];
  size_t n = 4096;
  int exitCode;
  bool done =
    Process::Run(mtprint.Get(),
		 commandLine.Get(),
		 szBuf,
		 &n,
		 &exitCode);
  pProgDlg->StopProgressDialog ();
  pProgDlg.reset ();

  if (! done)
    {
      FATAL_MIKTEX_ERROR ("DviView::PrintPostScript",
			  T_("The MiKTeX print utility could not be run."),
			  T_("mtprint.exe"));
    }

  if (exitCode != 0)
    {
      FATAL_MIKTEX_ERROR ("DviView::PrintPostScript",
			  (T_("The MiKTeX print utility failed for some ")
			   T_("reason.")),
			  T_("mtprint.exe"));
    }
}

/* _________________________________________________________________________

   DviView::OnPrint
   _________________________________________________________________________ */

void
DviView::OnPrint (/*[in]*/ CDC *	pDC,
		  /*[in]*/ CPrintInfo *	pInfo)
{
  try
    {
      pPrintInfo = pInfo;
      CScrollView::OnPrint (pDC, pInfo);
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
#if DVI_DONT_RENDER_GRAPHICS_SPECIALS
      pDoc->ForgetGraphicsInclusions ();
#endif
    }
  
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   DviView::OnUpdateFilePrint
   _________________________________________________________________________ */

void
DviView::OnUpdateFilePrint (/*[in]*/ CCmdUI * pCmdUI)
{
  BOOL enable = FALSE;
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  enable = TRUE;
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  pCmdUI->Enable (enable);
}

