/* MainFrame.cpp: main frame window

   Copyright (C) 1996-2007 Christian Schenk

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

#include "AdvancedOptionsPage.h"
#include "DisplayOptionsPage.h"
#include "DviDoc.h"
#include "DviView.h"
#include "InverseSearchOptionsPage.h"
#include "MagnifyingGlassOptionsPage.h"
#include "MainFrame.h"
#include "PrintOptionsPage.h"
#include "SecurityOptionsPage.h"
#include "UIOptionsPage.h"
#include "winplace.h"
#include "yaphelp.h"

IMPLEMENT_DYNAMIC(MainFrame, CMDIFrameWnd);

/* _________________________________________________________________________

   MainFrame Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(MainFrame, CMDIFrameWnd)
  ON_COMMAND(ID_CANCEL, &MainFrame::OnCancel)
  ON_COMMAND(ID_CONTEXT_HELP, &CMDIFrameWnd::OnContextHelp)
  ON_COMMAND(ID_CONTINUOUS_DOUBLE, &MainFrame::OnContinuousDouble)
  ON_COMMAND(ID_CONTINUOUS_SINGLE, &MainFrame::OnContinuousSingle)
  ON_COMMAND(ID_DEFAULT_HELP, &CMDIFrameWnd::OnHelpFinder)
  ON_COMMAND(ID_DOUBLE_PAGE, &MainFrame::OnDoublePage)
  ON_COMMAND(ID_HELP, &CMDIFrameWnd::OnHelp)
  ON_COMMAND(ID_HELP_FINDER, &CMDIFrameWnd::OnHelpFinder)
  ON_COMMAND(ID_HELP_KEYBOARD, &MainFrame::OnHelpKeyboard)
  ON_COMMAND(ID_SINGLE_PAGE, &MainFrame::OnSinglePage)
  ON_COMMAND(ID_VIEW_FULLSCREEN, &MainFrame::OnViewFullScreen)
  ON_COMMAND(ID_VIEW_OPTIONS, &MainFrame::OnViewOptions)
  ON_UPDATE_COMMAND_UI(ID_CONTINUOUS_DOUBLE, &MainFrame::OnUpdateContinuousDouble)
  ON_UPDATE_COMMAND_UI(ID_CONTINUOUS_SINGLE, &MainFrame::OnUpdateContinuousSingle)
  ON_UPDATE_COMMAND_UI(ID_DOUBLE_PAGE, &MainFrame::OnUpdateDoublePage)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_COMMAND_PREFIX, &MainFrame::OnUpdateCommandPrefix)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_PAGE_M_OF_N, &MainFrame::OnUpdatePageMofN)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_POINT, &MainFrame::OnUpdatePoint)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_SOURCE, &MainFrame::OnUpdateSource)
  ON_UPDATE_COMMAND_UI(ID_SINGLE_PAGE, &MainFrame::OnUpdateSinglePage)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, &MainFrame::OnUpdateViewFullScreen)
  ON_WM_CHAR()
  ON_WM_CLOSE()
  ON_WM_CREATE()
  ON_WM_GETMINMAXINFO()
  ON_COMMAND(ID_VIEW_CENTIMETERS, &MainFrame::OnViewCentimeters)
  ON_UPDATE_COMMAND_UI(ID_VIEW_CENTIMETERS, &MainFrame::OnUpdateViewCentimeters)
  ON_COMMAND(ID_VIEW_INCHES, &MainFrame::OnViewInches)
  ON_UPDATE_COMMAND_UI(ID_VIEW_INCHES, &MainFrame::OnUpdateViewInches)
  ON_COMMAND(ID_VIEW_MILLIMETERS, &MainFrame::OnViewMillimeters)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MILLIMETERS, &MainFrame::OnUpdateViewMillimeters)
  ON_COMMAND(ID_VIEW_PICAS, &MainFrame::OnViewPicas)
  ON_UPDATE_COMMAND_UI(ID_VIEW_PICAS, &MainFrame::OnUpdateViewPicas)
  ON_COMMAND(ID_VIEW_POINTS, &MainFrame::OnViewPoints)
  ON_UPDATE_COMMAND_UI(ID_VIEW_POINTS, &MainFrame::OnUpdateViewPoints)
END_MESSAGE_MAP();
  
/* _________________________________________________________________________
   
   Status Line Indicators
   _________________________________________________________________________ */

namespace {

  UINT
  BASED_CODE
  indicators[] =
    {
      ID_SEPARATOR,           // status line indicator
      ID_INDICATOR_COMMAND_PREFIX,
      ID_INDICATOR_SOURCE,
      ID_INDICATOR_POINT,
      ID_INDICATOR_PAGE_M_OF_N,
    };

}

/* _________________________________________________________________________

   MainFrame::MainFrame
   _________________________________________________________________________ */

MainFrame::MainFrame ()
  : pFullScreenBar (0),
    isFullScreen (false)
{
}

/* _________________________________________________________________________

   MainFrame::OnCreate
   _________________________________________________________________________ */

int
MainFrame::OnCreate (/*[in,out]*/ LPCREATESTRUCT lpCreateStruct)
{
  try
    {
      if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
	  return (-1);
	}
	
      if (! toolBar.CreateEx(this,
			     TBSTYLE_FLAT,
			     (0
			      | CBRS_FLYBY
			      | CBRS_GRIPPER
			      | CBRS_SIZE_DYNAMIC
			      | CBRS_TOOLTIPS
			      | CBRS_TOP
			      | WS_CHILD
			      | WS_VISIBLE
			      | 0))
	  || ! toolBar.LoadToolBar(IDR_MAINFRAME))
	{
	  UNEXPECTED_CONDITION (T_("MainFrame::OnCreate"));
	}

      if (! statusBar.Create(this)
	  || ! statusBar.SetIndicators(indicators,
				       sizeof(indicators) / sizeof(UINT)))
	{
	  UNEXPECTED_CONDITION (T_("MainFrame::OnCreate"));
	}

      toolBar.SetBarStyle (toolBar.GetBarStyle()
			   | CBRS_FLYBY
			   | CBRS_SIZE_DYNAMIC
			   | CBRS_TOOLTIPS
			   | 0);
      
      toolBar.EnableDocking (CBRS_ALIGN_ANY);
      EnableDocking (CBRS_ALIGN_ANY);
      DockControlBar (&toolBar);

      LoadBarState (T_("Settings"));

      return (0);
    }

  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
      return (-1);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
      return (-1);
    }
}

/* _________________________________________________________________________

   MainFrame::PreCreateWindow
   _________________________________________________________________________ */

BOOL
MainFrame::PreCreateWindow (/*[in,out]*/ CREATESTRUCT & cs) 
{
  SetTitle (T_("Yap"));
  cs.lpszClass = YAP_WND_CLASS;
  return (CMDIFrameWnd::PreCreateWindow(cs));
}

/* _________________________________________________________________________

   MainFrame::OnViewOptions
   _________________________________________________________________________ */

void
MainFrame::OnViewOptions ()
{
  try
    {
      CPropertySheet dlg (T_("Options"), 0, 0);
      
#if 0
      dlg.EnableStackedTabs (FALSE);
#endif

      UIOptionsPage pageUI;
      dlg.AddPage (&pageUI);

      DisplayOptionsPage pageDisplay;
      dlg.AddPage (&pageDisplay);
      
      PrintOptionsPage pagePrint;
      dlg.AddPage (&pagePrint);
      
      MagnifyingGlassOptionsPage pageMag;
      dlg.AddPage (&pageMag);
      
      InverseSearchOptionsPage pageIsearch;
      dlg.AddPage (&pageIsearch);
      
      SecurityOptionsPage pageSec;
      dlg.AddPage (&pageSec);
      
      AdvancedOptionsPage pageAdvanced;
      dlg.AddPage (&pageAdvanced);
      
      if (dlg.DoModal() == IDOK)
	{
	  UpdateAllDviViews (true);
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
}

/* _________________________________________________________________________

   MainFrame::OnUpdatePageMofN
   _________________________________________________________________________ */

void
MainFrame::OnUpdatePageMofN (/*[in,out]*/ CCmdUI * pCmdUI)
{
  try
    {
      CMDIChildWnd * pChild = MDIGetActive();
      pCmdUI->Enable (pChild != 0);
      if (pChild == 0)
	{
	  pCmdUI->SetText (T_(""));
	  return;
	}
      CView * pView = pChild->GetActiveView();
      if (pView == 0 || ! pView->IsKindOf(RUNTIME_CLASS(DviView)))
	{
	  pCmdUI->SetText (T_(""));
	  return;
	}
      DviView * pDviView = reinterpret_cast<DviView*>(pView);
      CDocument * pDoc = pChild->GetActiveDocument();
      DviDoc * pDviDoc = reinterpret_cast<DviDoc*>(pDoc);
      if (pDviDoc->GetDviFileStatus() != DviDoc::DVIFILE_LOADED
	  || pDviView->GetCurrentPageIdx() < 0)
	{
	  pCmdUI->SetText (T_(""));
	  return;
	}
      int m = pDviView->GetCurrentPageIdx() + 1;
      int n = pDviDoc->GetPageCount();
      CString str;
      str.Format (T_("Page: %s (%d%s of %d)"),
		  pDviDoc->GetPageName(pDviView->GetCurrentPageIdx()),
		  m,
		  (m % 10 == 1
		   ? (m % 100 == 11
		      ? T_("th")
		      : T_("st"))
		   : (m % 10 == 2
		      ? (m % 100 == 12
			 ? T_("th")
			 : T_("nd"))
		      : (m % 10 == 3
			 ? (m % 100 == 13
			    ? T_("th")
			    : T_("rd"))
			 : T_("th")))),
		  n);
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

   MainFrame::OnUpdateCommandPrefix
   _________________________________________________________________________ */

void
MainFrame::OnUpdateCommandPrefix (/*[in,out]*/ CCmdUI * pCmdUI)
{
  pCmdUI->SetText (commandPrefix); 
}

/* _________________________________________________________________________

   MainFrame::OnClose
   _________________________________________________________________________ */

void
MainFrame::OnClose () 
{
  if (! IsFullScreen())
    {
      SaveBarState ("Settings");
      CWindowPlacement wp;
      wp.Save ("Settings", this);
    }
  CMDIFrameWnd::OnClose ();
}

/* _________________________________________________________________________

   MainFrame::OnHelpKeyboard
   _________________________________________________________________________ */

void
MainFrame::OnHelpKeyboard ()
{
  AfxGetApp()->HtmlHelp (KEYBOARD_SHORTCUTS);
}

/* _________________________________________________________________________

   MainFrame::OnUpdateSource
   _________________________________________________________________________ */

void
MainFrame::OnUpdateSource (/*[in,out]*/ CCmdUI * pCmdUI)
{
  try
    {
      CMDIChildWnd * pChild = MDIGetActive();
      pCmdUI->Enable (pChild != 0);
      if (pChild == 0)
	{
	  pCmdUI->SetText (T_(""));
	  return;
	}
      CView * pView = pChild->GetActiveView();
      if (pView == 0 || ! pView->IsKindOf(RUNTIME_CLASS(DviView)))
	{
	  pCmdUI->SetText (T_(""));
	  return;
	}
      DviView * pDviView = reinterpret_cast<DviView*>(pView);
      CDocument * pDoc = pChild->GetActiveDocument();
      DviDoc * pDviDoc = reinterpret_cast<DviDoc*>(pDoc);
      if (pDviDoc->GetDviFileStatus() != DviDoc::DVIFILE_LOADED
	  || pDviView->IsZooming())
	{
	  // <fixme/>
	  return;
	}
      PathName sourceFileName;
      int line;
      if (pDviView->GetSource(sourceFileName, line))
	{
	  CString str;
	  str.Format (T_("%s L:%d"), sourceFileName.Get(), line);
	  pCmdUI->SetText (str); 
	}
      else
	{
	  CString s;
	  if (! s.LoadString(ID_INDICATOR_SOURCE))
	    {
	      UNEXPECTED_CONDITION (T_("MainFrame::OnUpdateSource"));
	    }
	  pCmdUI->SetText (s);
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
}

/* _________________________________________________________________________

   MainFrame::OnUpdatePoint
   _________________________________________________________________________ */

void
MainFrame::OnUpdatePoint (/*[in,out]*/ CCmdUI * pCmdUI)
{
  try
    {
      CMDIChildWnd * pChild = MDIGetActive();
      pCmdUI->Enable (pChild != 0);
      if (pChild == 0)
	{
	  pCmdUI->SetText (T_(""));
	  return;
	}
      CView * pView = pChild->GetActiveView();
      if (pView == 0 || ! pView->IsKindOf(RUNTIME_CLASS(DviView)))
	{
	  pCmdUI->SetText (T_(""));
	  return;
	}
      CDocument * pDoc = pChild->GetActiveDocument();
      DviDoc * pDviDoc = reinterpret_cast<DviDoc*>(pDoc);
      int x, y;
      if (pDviDoc->GetDviFileStatus() != DviDoc::DVIFILE_LOADED
	  || ! reinterpret_cast<DviView*>(pView)->GetPoint(x, y))
	{
	  pCmdUI->SetText (T_(""));
	  return;
	}
#define pxl2bp(pxl) (((pxl) * 72.0) / pDviDoc->GetDisplayResolution())
#define pxl2cm(pxl) (((pxl) * 2.54) / pDviDoc->GetDisplayResolution())
#define pxl2in(pxl) (((pxl) * 1.00) / pDviDoc->GetDisplayResolution())
#define pxl2mm(pxl) (((pxl) * 25.4) / pDviDoc->GetDisplayResolution())
#define pxl2pc(pxl) (((pxl) * 12.0) / pDviDoc->GetDisplayResolution())
      double x2, y2;
      const char * lpszUnit = 0;
      switch (g_pYapConfig->unit.Get())
	{
	case Units::Centimeters:
	  x2 = pxl2cm(x);
	  y2 = pxl2cm(y);
	  lpszUnit = T_("cm");
	  break;
	case Units::Inches:
	  x2 = pxl2in(x);
	  y2 = pxl2in(y);
	  lpszUnit = T_("in");
	  break;
	case Units::Millimeters:
	  x2 = pxl2mm(x);
	  y2 = pxl2mm(y);
	  lpszUnit = T_("mm");
	  break;
	case Units::Picas:
	  x2 = pxl2pc(x);
	  y2 = pxl2pc(y);
	  lpszUnit = T_("pc");
	  break;
	case Units::BigPoints:
	  x2 = pxl2bp(x);
	  y2 = pxl2bp(y);
	  lpszUnit = T_("pt");
	  break;
	default:
	  UNEXPECTED_CONDITION (T_("MainFrame::OnUpdatePoint"));
	}
      CString str;
      int precision = 2;
      str.Format (T_("%.*f,%.*f%s"),
		  precision, x2,
		  precision, y2,
		  lpszUnit);
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

   MainFrame::AddCommandPrefixChar
   _________________________________________________________________________ */

bool
MainFrame::AddCommandPrefixChar (/*[in]*/ char	ch)
{
  bool done = true;
  if (isdigit(ch))
    {
      commandPrefix += ch;
    }
  else if (ch == '-')
    {
      if (commandPrefix.GetLength() > 0
	  && commandPrefix.GetAt(0) == '-')
	{
	  CString tmp =
	    commandPrefix.Right(commandPrefix.GetLength() - 1);
	  commandPrefix = tmp;
	}
      else
	{
	  CString tmp = commandPrefix;
	  commandPrefix = "-";
	  commandPrefix += tmp;
	}
    }
  else if (ch == VK_BACK && commandPrefix.GetLength() > 0)
    {
      CString tmp =
	commandPrefix.Left(commandPrefix.GetLength() - 1);
      commandPrefix = tmp;
    }
  else
    {
      done = false;
    }
  return (done);
}

/* _________________________________________________________________________

   MainFrame::GetCommandPrefix
   _________________________________________________________________________ */

CString
MainFrame::GetCommandPrefix (/*[in]*/ bool clear)
{
  CString str = commandPrefix;
  if (clear)
    {
      commandPrefix = T_("");
    }
  return (str);
}

/* _________________________________________________________________________

   MainFrame::OnContinuousSingle
   _________________________________________________________________________ */

void
MainFrame::OnContinuousSingle ()
{
  try
    {
      g_pYapConfig->continuousView = true;
      g_pYapConfig->doublePage = false;
      UpdateAllDviViews ();
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

   MainFrame::OnUpdateContinuousSingle
   _________________________________________________________________________ */

void
MainFrame::OnUpdateContinuousSingle (/*[in]*/ CCmdUI * pCmdUI)
{
#if WIN95NOCONTVIEW
  bool supported = ! g_pYapConfig->win95;
#else
  bool supported = true;
#endif
  pCmdUI->Enable (supported);
  pCmdUI->SetCheck ((supported
		     && g_pYapConfig->continuousView
		     && ! g_pYapConfig->doublePage)
		    ? 1
		    : 0);
}

/* _________________________________________________________________________

   MainFrame::OnContinuousDouble
   _________________________________________________________________________ */

void
MainFrame::OnContinuousDouble ()
{
  try
    {
      g_pYapConfig->continuousView = true;
      g_pYapConfig->doublePage = true;
      UpdateAllDviViews ();
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

   MainFrame::OnUpdateContinuousDouble
   _________________________________________________________________________ */

void
MainFrame::OnUpdateContinuousDouble (/*[in]*/ CCmdUI * pCmdUI)
{
#if WIN95NOCONTVIEW
  bool supported = ! g_pYapConfig->win95;
#else
  bool supported = true;
#endif
  pCmdUI->Enable (supported);
  pCmdUI->SetCheck ((supported
		     && g_pYapConfig->continuousView
		     && g_pYapConfig->doublePage)
		    ? 1
		    : 0);
}

/* _________________________________________________________________________

   MainFrame::OnSinglePage
   _________________________________________________________________________ */

void
MainFrame::OnSinglePage ()
{
  try
    {
      g_pYapConfig->continuousView = false;
      g_pYapConfig->doublePage = false;
      UpdateAllDviViews ();
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

   MainFrame::OnUpdateSinglePage
   _________________________________________________________________________ */

void
MainFrame::OnUpdateSinglePage (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->SetCheck ((! g_pYapConfig->doublePage
		     && ! g_pYapConfig->continuousView)
		    ? 1 
		    : 0);
}

/* _________________________________________________________________________

   MainFrame::OnDoublePage
   _________________________________________________________________________ */

void
MainFrame::OnDoublePage ()
{
  try
    {
      g_pYapConfig->continuousView = false;
      g_pYapConfig->doublePage = true;
      UpdateAllDviViews ();
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

   MainFrame::OnUpdateDoublePage
   _________________________________________________________________________ */

void
MainFrame::OnUpdateDoublePage (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->SetCheck ((g_pYapConfig->doublePage
		     && ! g_pYapConfig->continuousView)
		    ? 1
		    : 0);
}

/* _________________________________________________________________________

   MainFrame::OnViewFullScreen
   _________________________________________________________________________ */

void
MainFrame::OnViewFullScreen ()
{
  try
    {
      CMDIChildWnd * pChild = MDIGetActive();

      if (pChild == 0)
	{
	  return;
	}

      WINDOWPLACEMENT wpNew;

      if (! IsFullScreen())
	{

	  statusBar.ShowWindow (SW_HIDE);
	  toolBar.ShowWindow (SW_HIDE);
      
	  previousWindowPlacement.length = sizeof(previousWindowPlacement);

	  GetWindowPlacement (&previousWindowPlacement);

	  RECT rectDesktop;

	  if (! ::GetWindowRect(::GetDesktopWindow(), &rectDesktop))
	    {
	      FATAL_WINDOWS_ERROR (T_("GetWindowRect"), 0);
	    }

	  rectDesktop.top -= 2;
	  rectDesktop.left -= 2;
	  rectDesktop.bottom += 2;
	  rectDesktop.right += 2;
	  
	  if (! ::AdjustWindowRectEx(&rectDesktop,
				     GetStyle(),
				     TRUE,
				     GetExStyle()))
	    {
	      FATAL_WINDOWS_ERROR (T_("AdjustWindowRectEx"), 0);
	    }

	  fullScreenWindowRect = rectDesktop;

	  wpNew.length = sizeof(wpNew);
	  wpNew.showCmd = SW_SHOWNORMAL;
	  wpNew.rcNormalPosition = rectDesktop;

	  pFullScreenBar = new CToolBar;

	  if (! pFullScreenBar->Create(this,
					  (CBRS_SIZE_DYNAMIC
					   | CBRS_TOOLTIPS
					   | CBRS_FLOATING))
	      || !pFullScreenBar->LoadToolBar(IDR_FULLSCREEN))
	    {
	      UNEXPECTED_CONDITION (T_("MainFrame::OnViewFullScreen"));
	    }
	      
	  pFullScreenBar->EnableDocking (0);

	  if(! pFullScreenBar->SetWindowPos(0,
					       100,
					       100,
					       0,
					       0,
					       (SWP_NOSIZE
						| SWP_NOZORDER
						| SWP_NOACTIVATE
						| SWP_SHOWWINDOW)))
	    {
	      UNEXPECTED_CONDITION (T_("MainFrame::OnViewFullScreen"));
	    }
	     
	     pFullScreenBar->SetWindowText (_T("Full Screen"));

	  FloatControlBar (pFullScreenBar, CPoint(100,100));

	  isFullScreen = true;

	  pChild->ModifyStyle (WS_CAPTION, 0, 0);
	}
      else
	{
	  pFullScreenBar->DestroyWindow ();
	  delete pFullScreenBar;
	  isFullScreen = false;
	  statusBar.ShowWindow (SW_SHOWNORMAL);
	  toolBar.ShowWindow (SW_SHOWNORMAL);
	  wpNew = previousWindowPlacement;
	  pChild->ModifyStyle (0, WS_CAPTION, 0);
	}

      if (!SetWindowPlacement(&wpNew))
	{
	  UNEXPECTED_CONDITION (T_("MainFrame::OnViewFullScreen"));
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
}

/* _________________________________________________________________________

   MainFrame::OnUpdateViewFullScreen
   _________________________________________________________________________ */

void
MainFrame::OnUpdateViewFullScreen (/*[in]*/ CCmdUI * pCmdUI)
{
  CMDIChildWnd* pChild = MDIGetActive();
  pCmdUI->Enable (pChild != 0);
  if (IsFullScreen())
    {
      pCmdUI->SetCheck ();
    }
  else
    {
      pCmdUI->SetCheck (0);
    }
}

/* _________________________________________________________________________

   MainFrame::OnGetMinMaxInfo
   _________________________________________________________________________ */

void
MainFrame::OnGetMinMaxInfo (/*[in]*/ MINMAXINFO * lpMMI)
{
  if (IsFullScreen())
    {
      lpMMI->ptMaxSize.y = fullScreenWindowRect.Height();
      lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y;
      lpMMI->ptMaxSize.x = fullScreenWindowRect.Width();
      lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
    }
  CFrameWnd::OnGetMinMaxInfo (lpMMI);
}

/* _________________________________________________________________________

   MainFrame::OnCancel
   _________________________________________________________________________ */

void
MainFrame::OnCancel ()
{
  if (IsFullScreen())
    {
      OnViewFullScreen ();
    }
  commandPrefix = T_("");
}

/* _________________________________________________________________________

   MainFrame::OnViewCentimeters
   _________________________________________________________________________ */

void
MainFrame::OnViewCentimeters ()
{
  g_pYapConfig->unit = Units::Centimeters;
}

/* _________________________________________________________________________

   MainFrame::OnUpdateViewCentimeters
   _________________________________________________________________________ */

void
MainFrame::OnUpdateViewCentimeters (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->Enable (TRUE);
  pCmdUI->SetCheck (g_pYapConfig->unit == Units::Centimeters);
}

/* _________________________________________________________________________

   MainFrame::OnViewInches
   _________________________________________________________________________ */

void
MainFrame::OnViewInches ()
{
  g_pYapConfig->unit = Units::Inches;
}

/* _________________________________________________________________________

   MainFrame::OnUpdateViewInches
   _________________________________________________________________________ */

void
MainFrame::OnUpdateViewInches (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->Enable (TRUE);
  pCmdUI->SetCheck (g_pYapConfig->unit == Units::Inches);
}

/* _________________________________________________________________________

   MainFrame::OnViewMillimeters
   _________________________________________________________________________ */

void
MainFrame::OnViewMillimeters ()
{
  g_pYapConfig->unit = Units::Millimeters;
}

/* _________________________________________________________________________

   MainFrame::OnUpdateViewMillimeters
   _________________________________________________________________________ */

void
MainFrame::OnUpdateViewMillimeters (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->Enable (TRUE);
  pCmdUI->SetCheck (g_pYapConfig->unit == Units::Millimeters);
}

/* _________________________________________________________________________

   MainFrame::OnViewPicas
   _________________________________________________________________________ */

void
MainFrame::OnViewPicas ()
{
  g_pYapConfig->unit = Units::Picas;
}

/* _________________________________________________________________________

   MainFrame::OnUpdateViewPicas
   _________________________________________________________________________ */

void
MainFrame::OnUpdateViewPicas (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->Enable (TRUE);
  pCmdUI->SetCheck (g_pYapConfig->unit == Units::Picas);
}

/* _________________________________________________________________________

   MainFrame::OnViewPoints
   _________________________________________________________________________ */

void
MainFrame::OnViewPoints ()
{
  g_pYapConfig->unit = Units::BigPoints;
}

/* _________________________________________________________________________

   MainFrame::OnUpdateViewPoints
   _________________________________________________________________________ */

void
MainFrame::OnUpdateViewPoints (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->Enable (TRUE);
  pCmdUI->SetCheck (g_pYapConfig->unit == Units::BigPoints);
}
