/* MainFrame.cpp:

   Copyright (C) 2002-2007 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "stdafx.h"
#include "mpm.h"

#include "MainFrame.h"
#include "MpmDoc.h"
#include "MpmView.h"

IMPLEMENT_DYNCREATE(MainFrame, CFrameWnd);

/* _________________________________________________________________________

   Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
  ON_WM_CREATE()
  ON_UPDATE_COMMAND_UI(IDOK, &MainFrame::OnUpdateFilterOut)
  ON_COMMAND(ID_EDIT_PASTE, &MainFrame::OnEditPaste)
  ON_COMMAND(ID_EDIT_CUT, &MainFrame::OnEditCut)
  ON_COMMAND(ID_EDIT_COPY, &MainFrame::OnEditCopy)
  ON_COMMAND(ID_EDIT_UNDO, &MainFrame::OnEditUndo)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM_PACKAGES, &MainFrame::OnUpdateNumPackages)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   Status Bar Indicators
   _________________________________________________________________________ */

namespace {

UINT indicators[] =
{
  ID_SEPARATOR,
  ID_INDICATOR_NUM_PACKAGES,
};

}

/* _________________________________________________________________________

   MainFrame::MainFrame
   _________________________________________________________________________ */

MainFrame::MainFrame ()
{
}

/* _________________________________________________________________________

   MainFrame::~MainFrame
   _________________________________________________________________________ */

MainFrame::~MainFrame ()
{
}


/* _________________________________________________________________________

   MainFrame::OnCreate
   _________________________________________________________________________ */

int
MainFrame::OnCreate (/*[in,out]*/ LPCREATESTRUCT pCreateStruct)
{
  try
    {
      if (CFrameWnd::OnCreate(pCreateStruct) < 0)
	{
	  FATAL_MIKTEX_ERROR ("MainFrame::OnCreate",
			      T_("The main window could not be created."),
			      0);
	}
      
      if (! toolBar.CreateEx(this) || ! toolBar.LoadToolBar(IDR_MAINFRAME))
	{
	  FATAL_MIKTEX_ERROR ("MainFrame::OnCreate",
			      T_("The toolbar control could not be created."),
			      0);
	}
      
      if (! dialogBar.Create(this,
			     IDR_MAINFRAME,
			     CBRS_ALIGN_TOP,
			     AFX_IDW_DIALOGBAR))
	{
	  FATAL_MIKTEX_ERROR ("MainFrame::OnCreate",
		      T_("The dialog bar controlcould not be created."),
			      0);
	}
      
      if (! reBar.Create(this)
	  || ! reBar.AddBar(&toolBar)
	  || ! reBar.AddBar(&dialogBar))
	{
	  FATAL_MIKTEX_ERROR ("MainFrame::OnCreate",
			      T_("The coolbar control could not be created."),
			      0);
	}
      
      if (! statusBar.Create(this)
	  || ! statusBar.SetIndicators(indicators,
				       sizeof(indicators) / sizeof(UINT)))
	{
	  FATAL_MIKTEX_ERROR ("MainFrame::OnCreate",
		      T_("The status bar control could not be created."),
			      0);
	}
      
      toolBar.SetBarStyle (toolBar.GetBarStyle()
			   | CBRS_TOOLTIPS
			   | CBRS_FLYBY);
      
      return (0);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (-1);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (-1);
    }
}

/* _________________________________________________________________________

   MainFrame::PreCreateWindow
   _________________________________________________________________________ */

BOOL
MainFrame::PreCreateWindow (/*[in,out]*/ CREATESTRUCT & cs)
{
  try
    {
      if (! CFrameWnd::PreCreateWindow(cs))
	{
	  FATAL_MIKTEX_ERROR ("MainFrame::PreCreateWindow",
			      T_("The main window could not be initialized."),
			      0);
	}
      cs.style &= ~FWS_ADDTOTITLE;
      if (SessionWrapper(true)->IsAdminMode())
      {
	SetTitle (T_("MiKTeX Package Manager (Admin)"));
      }
      else
      {
	SetTitle (T_("MiKTeX Package Manager"));
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

   MainFrame::OnUpdateFilterOut
   _________________________________________________________________________ */

void
MainFrame::OnUpdateFilterOut (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->Enable (TRUE);
}

/* _________________________________________________________________________

   MainFrame::OnUpdateNumPackages
   _________________________________________________________________________ */

void
MainFrame::OnUpdateNumPackages (/*[in]*/ CCmdUI * pCmdUI)
{
  try
    {
      CView * pView = GetActiveView();
      pCmdUI->Enable (pView != 0);
      if (pView == 0)
	{
	  pCmdUI->SetText ("");
	  return;
	}
      if (! pView->IsKindOf(RUNTIME_CLASS(MpmView)))
	{
	  pCmdUI->SetText ("");
	  return;
	}
      MpmView * pMpmView = reinterpret_cast<MpmView*>(pView);
      CString str;
      str.Format (T_("Total: %d"), pMpmView->GetNumberOfPackages());
      pCmdUI->SetText (str); 
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

   MainFrame::OnEditCut
   _________________________________________________________________________ */

void
MainFrame::OnEditCut ()
{
  CWnd * pWnd = GetFocus();
  if (pWnd != 0)
    {
      pWnd->SendMessage (WM_CUT, 0, 0);
    }
}

/* _________________________________________________________________________

   MainFrame::OnEditPaste
   _________________________________________________________________________ */

void
MainFrame::OnEditPaste ()
{
  CWnd * pWnd = GetFocus();
  if (pWnd != 0)
    {
      pWnd->SendMessage (WM_PASTE, 0, 0);
    }
}

/* _________________________________________________________________________

   MainFrame::OnEditCopy
   _________________________________________________________________________ */

void
MainFrame::OnEditCopy ()
{
  CWnd * pWnd = GetFocus();
  if (pWnd != 0)
    {
      pWnd->SendMessage (WM_COPY, 0, 0);
    }
}

/* _________________________________________________________________________

   MainFrame::OnEditUndo
   _________________________________________________________________________ */

void
MainFrame::OnEditUndo ()
{
  CWnd * pWnd = GetFocus();
  if (pWnd != 0)
    {
      pWnd->SendMessage (WM_UNDO, 0, 0);
    }
}
