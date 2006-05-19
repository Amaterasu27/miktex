/* DviDoc.cpp:

   Copyright (C) 1996-2006 Christian Schenk

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

#include "Dib.h"
#include "DviDoc.h"
#include "DviView.h"
#include "MainFrame.h"

IMPLEMENT_DYNCREATE(DviDoc, CDocument);

/* _________________________________________________________________________

   DviDoc Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(DviDoc, CDocument)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   DviDoc::DviDoc
   _________________________________________________________________________ */

namespace {
  DviDoc * pLastDoc = 0;
}

DviDoc::DviDoc ()
  : pDvi (0),
    pDviSave (0),
    isPrintContext (false),
    displayShrinkFactor (g_pYapConfig->displayShrinkFactor),
    fileStatus (DVIFILE_NOT_LOADED),
    pProgressDialog (0),
    dviPageMode (g_pYapConfig->dviPageMode),
    pSession (true),
    landscape (false)
{
  pLastDoc = this;
  modificationTime = 0;
  PaperSizeInfo paperSizeInfo;
  if (! pSession->GetPaperSizeInfo(-1, paperSizeInfo))
    {
      UNEXPECTED_CONDITION (T_("YapConfig::Load"));
    }
  dvipsPaperName = paperSizeInfo.dvipsName;
}

/* _________________________________________________________________________

   DviDoc::~DviDoc
   _________________________________________________________________________ */

DviDoc::~DviDoc ()
{
  try
    {
      MIKTEX_ASSERT (pDviSave == 0);
      MIKTEX_ASSERT (! isPrintContext);
      if (pDvi != 0)
	{
	  delete pDvi;
	  pDvi = 0;
	  fileStatus = DVIFILE_NOT_LOADED;
	}
      ForgetGraphicsInclusions ();
      if (pLastDoc == this)
	{
	  pLastDoc = 0;
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   DviDoc::OnOpenDocument
   _________________________________________________________________________ */

BOOL
DviDoc::OnOpenDocument (/*[in]*/ const MIKTEXCHAR * lpszPathName) 
{
  try
    {
      MIKTEX_ASSERT (pDvi == 0);
      MIKTEX_ASSERT (pDviSave == 0);
      MIKTEX_ASSERT (! isPrintContext);
      CreateDocument (lpszPathName);
      return (TRUE);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
      return (FALSE);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
      return (FALSE);
    }
}

/* _________________________________________________________________________

   DviDoc::GetLoadedPage
   _________________________________________________________________________ */

DviPage *
DviDoc::GetLoadedPage (/*[in]*/ int pageIdx)
{
  MIKTEX_ASSERT (pDvi != 0);
  return (pDvi->GetLoadedPage(pageIdx));
}

/* _________________________________________________________________________

   DviDoc::BeginDviPrinting
   _________________________________________________________________________ */

void
DviDoc::BeginDviPrinting (/*[in]*/ const CDC * pPrinterDC)
{
  UNUSED_ALWAYS (pPrinterDC);
  MIKTEX_ASSERT (! isPrintContext);
  MIKTEX_ASSERT (pDviSave == 0);
  MIKTEX_ASSERT (pDvi != 0);
  isPrintContext = true;
  pDviSave = pDvi;
  pDvi = 0;
  try
    {
      CreateDocument (GetPathName());
    }
  catch (const exception &)
    {
      this->pDvi = pDviSave;
      pDviSave = 0;
      isPrintContext = false;
      throw;
    }
}

/* _________________________________________________________________________

   DviDoc::EndDviPrinting
   _________________________________________________________________________ */

void
DviDoc::EndDviPrinting ()
{
  MIKTEX_ASSERT (isPrintContext);
  MIKTEX_ASSERT (this->pDvi != 0);
  isPrintContext = false;
  Dvi * pDvi = this->pDvi;
  this->pDvi = pDviSave;
  pDviSave = 0;
  if (pDvi != 0)
    {
      delete pDvi;
    }
}

/* _________________________________________________________________________

   DviDoc::CreateDocument
   _________________________________________________________________________ */

void
DviDoc::CreateDocument (/*[in]*/ const MIKTEXCHAR * lpszPathName)
{
  fileStatus = DVIFILE_NOT_LOADED;
  modificationTime = File::GetLastWriteTime(lpszPathName);
  ForgetGraphicsInclusions ();
  MIKTEXMFMODE mfmode;
  if (! pSession->GetMETAFONTMode(GetMetafontMode(), &mfmode))
    {
      UNEXPECTED_CONDITION (T_("DviDoc::CreateDocument"));
    }
  MIKTEX_ASSERT (pDvi == 0);
  pDvi =
    Dvi::Create(lpszPathName,
		mfmode.szMnemonic,
		GetResolution(),
		GetShrinkFactor(),
		(IsPrintContext()
		 ? DviAccess::Sequential
		 : DviAccess::Random),
		(IsPrintContext()
		 ? DviPageMode::Dvips
		 : dviPageMode),
		pSession->GetPaperSizeInfo(dvipsPaperName.c_str()),
		landscape,
		this);
  pDvi->Scan ();
  fileStatus = DVIFILE_LOADED;
}

/* _________________________________________________________________________

   DviDoc::Reread
   _________________________________________________________________________ */

void
DviDoc::Reread ()
{
  MIKTEX_ASSERT (! isPrintContext);
  Dvi * pDvi = this->pDvi;
  this->pDvi = 0;
  fileStatus = DVIFILE_NOT_LOADED;
  if (pDvi != 0)
    {
      delete pDvi;
    }
  CreateDocument (GetPathName());
}

/* _________________________________________________________________________

   DviDoc::GetPaperSize
   _________________________________________________________________________ */

CSize
DviDoc::GetPaperSize ()
  const
{
  MIKTEX_ASSERT (pDvi != 0);
  PaperSizeInfo size = pDvi->GetPaperSizeInfo();
  return (CSize(pt2pxl(size.width), pt2pxl(size.height)));
}

/* _________________________________________________________________________

   DviDoc::GetDisplayResolution
   _________________________________________________________________________ */

int
DviDoc::GetDisplayResolution ()
  const
{
  MIKTEXMFMODE mfmode;
  MIKTEX_ASSERT (g_pYapConfig.get() != 0);
  if (! pSession->GetMETAFONTMode(g_pYapConfig->displayMetafontMode, &mfmode))
    {
      UNEXPECTED_CONDITION (T_("DviDoc::GetDisplayResolution"));
    }
  return (mfmode.iHorzRes);
}

/* _________________________________________________________________________

   DviDoc::GetPrinterResolution
   _________________________________________________________________________ */

int
DviDoc::GetPrinterResolution ()
  const
{
  MIKTEXMFMODE mfmode;
  MIKTEX_ASSERT (g_pYapConfig.get() != 0);
  if (! pSession->GetMETAFONTMode(g_pYapConfig->printerMetafontMode, &mfmode))
    {
      UNEXPECTED_CONDITION (T_("DviDoc::GetPrinterResolution"));
    }
  return (mfmode.iHorzRes);
}

/* _________________________________________________________________________

   DviDoc::Shrink
   _________________________________________________________________________ */

void
DviDoc::Shrink (/*[in]*/ int d)
{
  displayShrinkFactor += d;
}

/* _________________________________________________________________________

   DviDoc::Unshrink
   _________________________________________________________________________ */

void
DviDoc::Unshrink ()
{
  if (displayShrinkFactor > 1)
    {
      Shrink (-1);
    }
}

/* _________________________________________________________________________

   DviDoc::GetPageName
   _________________________________________________________________________ */

const MIKTEXCHAR *
DviDoc::GetPageName (/*[in]*/ unsigned pageIdx)
{
  MIKTEX_ASSERT (pDvi != 0);
  DviPage * pDviPage = pDvi->GetPage(pageIdx);
  if (pDviPage == 0)
    {
      UNEXPECTED_CONDITION (T_("DviDoc::GetPageName"));
    }
  AutoUnlockPage autoUnlockPage (pDviPage);
  return (pDviPage->GetName());
}

/* _________________________________________________________________________

   DviDoc::GetPageNum
   _________________________________________________________________________ */

int
DviDoc::GetPageNum (/*[in]*/ int pageIdx)
  const
{
  MIKTEX_ASSERT (pDvi != 0);
  DviPage * pDviPage = pDvi->GetPage(pageIdx);
  if (pDviPage == 0)
    {
      UNEXPECTED_CONDITION (T_("DviDoc::GetPageNum"));
    }
  AutoUnlockPage autoUnlockPage (pDviPage);
  return (pDviPage->GetReg(0));
}

/* _________________________________________________________________________

   DviDoc::GetMinPageNum
   _________________________________________________________________________ */

int
DviDoc::GetMinPageNum ()
  const
{
  MIKTEX_ASSERT (pDvi != 0);
  return (pDvi->GetMinPageNumber());
}

/* _________________________________________________________________________

   DviDoc::GetMaxPageNum
   _________________________________________________________________________ */

int
DviDoc::GetMaxPageNum ()
  const

{
  MIKTEX_ASSERT (pDvi != 0);
  return (pDvi->GetMaxPageNumber());
}

/* _________________________________________________________________________

   DviDoc::GetMetafontMode
   _________________________________________________________________________ */

DWORD
DviDoc::GetMetafontMode ()
  const
{
  MIKTEX_ASSERT (g_pYapConfig.get() != 0);
  return (isPrintContext
	  ? g_pYapConfig->printerMetafontMode
	  : g_pYapConfig->displayMetafontMode);
}

/* _________________________________________________________________________

   DviDoc::IsPortrait
   _________________________________________________________________________ */

bool
DviDoc::IsPortrait ()
  const
{
  return (! landscape);
}

/* _________________________________________________________________________

   DviDoc::SetDisplayShrinkFactor
   _________________________________________________________________________ */

int
DviDoc::SetDisplayShrinkFactor (/*[in]*/ int shrinkFactor)
{
  int oldDisplayShrinkFactor = displayShrinkFactor;
  displayShrinkFactor = shrinkFactor;
  return (oldDisplayShrinkFactor);
}

/* _________________________________________________________________________

   DviDoc::SetDviPageMode
   _________________________________________________________________________ */

DviPageMode
DviDoc::SetDviPageMode (/*[in]*/ const DviPageMode & dviPageMode)
{
  DviPageMode old (this->dviPageMode);
  this->dviPageMode = dviPageMode;
  return (old);
}

/* _________________________________________________________________________

   DviDoc::GetActiveDocument
   _________________________________________________________________________ */

DviDoc *
DviDoc::GetActiveDocument ()
{
  ASSERT_VALID (AfxGetApp());
  MainFrame *
    pMain = reinterpret_cast<MainFrame*>((AfxGetApp())->m_pMainWnd);
  ASSERT_VALID (pMain);
  MIKTEX_ASSERT (pMain->IsKindOf(RUNTIME_CLASS(MainFrame)));
  CMDIChildWnd * pChild = pMain->MDIGetActive();
  ASSERT_VALID (pChild);
  DviDoc * pDviDoc = reinterpret_cast<DviDoc*>(pChild->GetActiveDocument());
  if (pDviDoc == 0)
    {
      pDviDoc = pLastDoc;
    }
  MIKTEX_ASSERT (pDviDoc != 0);
  MIKTEX_ASSERT (pDviDoc->IsKindOf(RUNTIME_CLASS(DviDoc)));
  return (pDviDoc);
}

/* _________________________________________________________________________

   DviDoc::GetDocDir
   _________________________________________________________________________ */

CString
DviDoc::GetDocDir ()
{
  PathName pathDocDir = GetPathName();
  pathDocDir.RemoveFileSpec ();
  return (pathDocDir.Get());
}

/* _________________________________________________________________________

   DviDoc::FindPage
   _________________________________________________________________________ */

int
DviDoc::FindPage (/*[in]*/ int pageNum)
  const
{
  for (int pageIdx = 0; pageIdx < GetPageCount(); ++ pageIdx)
    {
      if (GetPageNum(pageIdx) == pageNum)
	{
	  return (pageIdx);
	}
    }
  return (-1);
}

/* _________________________________________________________________________
   
   DviDoc::ForgetGraphicsInclusions
   _________________________________________________________________________ */

void
DviDoc::ForgetGraphicsInclusions ()
{
  graphicsInclusions.clear ();
  graphicsDone.clear ();
}

/* _________________________________________________________________________
   
   DviDoc::OnIdle
   _________________________________________________________________________ */

void
DviDoc::OnIdle ()
{
  try
    {
      CDocument::OnIdle ();
      if (pDvi != 0)
	{
	  pSession->UnloadFilenameDatabase (); // <fixme/>
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________
   
   DviDoc::GetDviFileStatus
   _________________________________________________________________________ */

DviDoc::DviFileStatus
DviDoc::GetDviFileStatus ()
{
  if (fileStatus == DVIFILE_LOADED)
    {
      if (! File::Exists(PathName(GetPathName())))
	{
	  fileStatus = DVIFILE_LOST;
	}
      else
	{
	  time_t timeMod = modificationTime;
	  modificationTime = File::GetLastWriteTime(PathName(GetPathName()));
	  if (timeMod != modificationTime)
	    {
	      modificationTime = timeMod;
	      fileStatus = DVIFILE_MODIFIED;
	    }
	}
    }
  return (fileStatus);
}

/* _________________________________________________________________________

   DviDoc::OnProgress
   _________________________________________________________________________ */

void
MIKTEXDVICALL
DviDoc::OnProgress (/*[in]*/ DviNotification	nf)
{
  if (nf == DviNotification::BeginLoadFont)
    {
      MIKTEX_ASSERT (pDvi != 0);
      tstring statusText = pDvi->GetStatusText();
      if (statusText.empty())
	{
	  return;
	}
      if (pProgressDialog == 0)
	{
	  pProgressDialog = ProgressDialog::Create();
	  pProgressDialog->StartProgressDialog
	    (AfxGetMainWnd()->GetSafeHwnd());
	  pProgressDialog->SetTitle ("Yap (Making Fonts)");
	  pProgressDialog->SetLine (1, "Yap is creating PK fonts:");
	}
      pProgressDialog->SetLine (2, statusText.c_str());
    }
}

/* _________________________________________________________________________
   
   DviDoc::MakeFonts
   _________________________________________________________________________ */

void
DviDoc::MakeFonts ()
{
  MIKTEX_ASSERT (pDvi != 0);
  bool done = pDvi->MakeFonts();
  if (pProgressDialog != 0)
    {
      pProgressDialog->StopProgressDialog ();
      delete pProgressDialog;
      pProgressDialog = 0;
    }
  if (! done)
    {
      FATAL_MIKTEX_ERROR (T_("DviDoc::MakeFonts"),
			  T_("Not all fonts could be loaded. \
See 'File->Document Properties', for details."),
			  0);
    }
}
