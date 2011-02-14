/* DviView.cpp:

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

#include "DocumentFontsPage.h"
#include "DviMagnifyingGlass.h"
#include "DviView.h"
#include "Dvidoc.h"
#include "GotoPageDialog.h"
#include "MainFrame.h"
#include "Ruler.h"
#include "SourceSpecialsDialog.h"

IMPLEMENT_DYNCREATE(DviView, CMyScrollView);

vector<double> DviView::gammaTable;

/* _________________________________________________________________________

   DviView Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(DviView, CMyScrollView)
  ON_COMMAND(ID_BACK, &DviView::OnBack)
  ON_COMMAND(ID_DARKER_TEXT, &DviView::OnDarkerText)
  ON_COMMAND(ID_DOCUMENT_DOWN, &DviView::OnDocumentDown)
  ON_COMMAND(ID_DOCUMENT_UP, &DviView::OnDocumentUp)
  ON_COMMAND(ID_END, &DviView::OnEnd)
  ON_COMMAND(ID_FILE_DOCUMENT_PROPERTIES, &DviView::OnFileDocumentProperties)
  ON_COMMAND(ID_FILE_DVIPS, &DviView::OnFileDvips)
  ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
  ON_COMMAND(ID_FIRST_PAGE, &DviView::OnFirstPage)
  ON_COMMAND(ID_FORWARD, &DviView::OnForward)
  ON_COMMAND(ID_GOTO_PAGE, &DviView::OnGotoPage)
  ON_COMMAND(ID_HOME, &DviView::OnHome)
  ON_COMMAND(ID_LAST_PAGE, &DviView::OnLastPage)
  ON_COMMAND(ID_LIGHTER_TEXT, &DviView::OnLighterText)
  ON_COMMAND(ID_NEXT_PAGE, &DviView::OnNextPage)
#if DVI_DONT_RENDER_POSTSCRIPT_SPECIALS
  ON_COMMAND(ID_PAGEMODE_AUTO, &DviView::OnDviPageModeAuto)
#endif
  ON_COMMAND(ID_PAGEMODE_DVIPS, &DviView::OnDviPageModeDvips)
  ON_COMMAND(ID_PAGEMODE_PK, &DviView::OnDviPageModePk)
  ON_COMMAND(ID_PAGE_EDITOR, &DviView::OnPageEditor)
  ON_COMMAND(ID_PREV_PAGE, &DviView::OnPrevPage)
  ON_COMMAND(ID_SB_LINEDOWN, &DviView::OnLineDown)
  ON_COMMAND(ID_SB_LINELEFT, &DviView::OnLineLeft)
  ON_COMMAND(ID_SB_LINERIGHT, &DviView::OnLineRight)
  ON_COMMAND(ID_SB_LINEUP, &DviView::OnLineUp)
  ON_COMMAND(ID_SB_PAGEDOWN, &DviView::OnPageDown)
  ON_COMMAND(ID_SB_PAGELEFT, &DviView::OnPageLeft)
  ON_COMMAND(ID_SB_PAGERIGHT, &DviView::OnPageRight)
  ON_COMMAND(ID_SB_PAGEUP, &DviView::OnPageUp)
  ON_COMMAND(ID_TOOLS_ARROW, &DviView::OnToolsArrow)
  ON_COMMAND(ID_TOOLS_HAND, &DviView::OnToolsHand)
  ON_COMMAND(ID_TOOLS_MAGNIFYING_GLASS, &DviView::OnToolsMagnifyingGlass)
  ON_COMMAND(ID_TOOLS_SOURCESPECIALS, &DviView::OnToolsSourcespecials)
  ON_COMMAND(ID_VIEW_REFRESH, &DviView::OnViewRefresh)
  ON_COMMAND(ID_ZOOM_IN, &DviView::OnZoomIn)
  ON_COMMAND(ID_ZOOM_OUT, &DviView::OnZoomOut)
  ON_MESSAGE(WM_MAKEFONTS, &DviView::OnMakeFonts)
  ON_UPDATE_COMMAND_UI(ID_BACK, &DviView::OnUpdateBack)
  ON_UPDATE_COMMAND_UI(ID_DARKER_TEXT, &DviView::OnUpdateDarkerText)
  ON_UPDATE_COMMAND_UI(ID_FILE_DOCUMENT_PROPERTIES, &DviView::OnUpdateFileDocumentProperties)
  ON_UPDATE_COMMAND_UI(ID_FIRST_PAGE, &DviView::OnUpdateFirstPage)
  ON_UPDATE_COMMAND_UI(ID_FORWARD, &DviView::OnUpdateForward)
  ON_UPDATE_COMMAND_UI(ID_LAST_PAGE, &DviView::OnUpdateLastPage)
  ON_UPDATE_COMMAND_UI(ID_LIGHTER_TEXT, &DviView::OnUpdateLighterText)
  ON_UPDATE_COMMAND_UI(ID_NEXT_PAGE, &DviView::OnUpdateNextPage)
#if DVI_DONT_RENDER_POSTSCRIPT_SPECIALS
  ON_UPDATE_COMMAND_UI(ID_PAGEMODE_AUTO, &DviView::OnUpdateDviPageModeAuto)
#endif
  ON_UPDATE_COMMAND_UI(ID_PAGEMODE_DVIPS, &DviView::OnUpdateDviPageModeDvips)
  ON_UPDATE_COMMAND_UI(ID_PAGEMODE_PK, &DviView::OnUpdateDviPageModePk)
  ON_UPDATE_COMMAND_UI(ID_PREV_PAGE, &DviView::OnUpdatePrevPage)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ARROW, &DviView::OnUpdateToolsArrow)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_HAND, &DviView::OnUpdateToolsHand)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_MAGNIFYING_GLASS, &DviView::OnUpdateToolsMagnifyingGlass)
  ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, &DviView::OnUpdateZoomIn)
  ON_WM_CHAR()
  ON_WM_CONTEXTMENU()
  ON_WM_ERASEBKGND()
  ON_WM_GETMINMAXINFO()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_WM_RBUTTONDOWN()
  ON_WM_SETCURSOR()
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, &DviView::OnUpdateFilePrint)
  ON_UPDATE_COMMAND_UI(ID_FILE_DVIPS, &DviView::OnUpdateFileDvips)
  ON_UPDATE_COMMAND_UI(ID_PAGE_EDITOR, &DviView::OnUpdatePageEditor)
  ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, &DviView::OnUpdateZoomOut)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_SOURCESPECIALS, &DviView::OnUpdateToolsSourcespecials)
  ON_UPDATE_COMMAND_UI(ID_GOTO_PAGE, &DviView::OnUpdateGotoPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   DviView::DviView
   _________________________________________________________________________ */

DviView::DviView ()

  : pSourceSpecialDialog (0),
    currentMouseTool (static_cast<MouseTool>(g_pYapConfig->lastTool)),
    pToolWindow (0),
    mouseCaptured (false),
    leftButtonDown (false),
    toolActive (false),
    curPageIdx (-1),
    cursorPosition (0, 0),
    pPrintInfo (0),
    warnPostScript (true)

{
  ClearSearchPosition ();
}

/* _________________________________________________________________________

   DviView::~DviView
   _________________________________________________________________________ */

DviView::~DviView ()
{
  try
    {
      if (pSourceSpecialDialog != 0)
	{
	  if (! pSourceSpecialDialog->DestroyWindow())
	    {
	      TraceStream::TraceLastWin32Error ("DestroyWindow",
						0,
						__FILE__,
						__LINE__);
	    }
	  pSourceSpecialDialog = 0;
	}
      if (pToolWindow != 0)
	{
	  if (! pToolWindow->DestroyWindow())
	    {
	      TraceStream::TraceLastWin32Error ("DestroyWindow",
						0,
						__FILE__,
						__LINE__);
	    }
	  pToolWindow = 0;
	}
      if (mouseCaptured)
	{
	  if (g_pYapConfig->magGlassHidesCursor)
	    {
	      ShowCursor (TRUE);
	    }
	  if (! ReleaseCapture())
	    {
	      TraceStream::TraceLastWin32Error ("ReleaseCapture",
						0,
						__FILE__,
						__LINE__);
	    }
	  mouseCaptured = false;
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   DviView::OnActivateView
   _________________________________________________________________________ */

void
DviView::OnActivateView (/*[in]*/ BOOL		activate,
			 /*[in]*/ CView *	pActivateView,
			 /*[in]*/ CView *	pDeactiveView)
{
  try
    {
      if (activate)
	{
	  DviDoc * pDoc = GetDocument();
	  ASSERT_VALID (pDoc);
	  DviDoc::DviFileStatus fileStatus = pDoc->GetDviFileStatus();
	  switch (fileStatus)
	    {
	    case DviDoc::DVIFILE_LOST:
	    case DviDoc::DVIFILE_MODIFIED:
	      OnViewRefresh ();
	      break;
	    }
	}
      else
	{
	  if (mouseCaptured)
	    {
	      if (g_pYapConfig->magGlassHidesCursor)
		{
		  ShowCursor (TRUE);
		}
	      if (! ReleaseCapture())
		{
		  FATAL_WINDOWS_ERROR ("ReleaseCapture", 0);
		}
	      mouseCaptured = false;
	    }
	  if (pToolWindow != 0)
	    {
	      if (! pToolWindow->DestroyWindow())
		{
		  FATAL_WINDOWS_ERROR ("DestroyWindow", 0);
		}
	      pToolWindow = 0;
	    }
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
  CMyScrollView::OnActivateView (activate, pActivateView, pDeactiveView);
}

/* _________________________________________________________________________

   DviView::OnInitialUpdate
   _________________________________________________________________________ */

void
DviView::OnInitialUpdate ()
{
  CMyScrollView::OnInitialUpdate ();
  curPageIdx = 0;
  try
    {
      if (g_pYapConfig->dviPageMode != DviPageMode::Dvips)
	{
	  PostMessage (WM_MAKEFONTS);
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
    }
}

/* _________________________________________________________________________

   DviView::DeterminePageSize
   _________________________________________________________________________ */

CSize
DviView::DeterminePageSize ()
{
  CRect rect;
  GetClientRect (&rect);
  CSize sizePage (rect.right / 3, rect.bottom / 3);
  return (sizePage);
}

/* _________________________________________________________________________

   DviView::OnUpdate
   _________________________________________________________________________ */

void
DviView::OnUpdate (/*[in]*/ CView *	pSender,
		   /*[in]*/ LPARAM	lHint,
		   /*[in]*/ CObject *	pHint)
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);

      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  CSize sizePage = pDoc->GetPaperSize();

	  int n = (g_pYapConfig->continuousView
		   ? (g_pYapConfig->doublePage
		      ? pDoc->GetRowCount()
		      : pDoc->GetPageCount())
		   : 1);

	  if (n <= 0)
	    {
	      n = 1;
	    }

	  CSize sizeTotal;

	  sizeTotal.cy = GetTopMargin();
	  sizeTotal.cy += n * sizePage.cy;
	  sizeTotal.cy += (n - 1) * GetVerticalInterPageOffset();

	  sizeTotal.cx = GetLeftMargin();
	  sizeTotal.cx += sizePage.cx;
	  if (g_pYapConfig->doublePage)
	    {
	      sizeTotal.cx += sizePage.cx;
	      sizeTotal.cx += GetHorizontalInterPageOffset();
	    }
	  sizeTotal.cx += 10;

	  SetScrollSizes (MM_TEXT, sizeTotal, DeterminePageSize());
	}

      CMyScrollView::OnUpdate (pSender, lHint, pHint);
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

   DviView::OnScroll

   _________________________________________________________________________ */

BOOL
DviView::OnScroll (/*[in]*/ UINT	scrollCode,
		   /*[in]*/ UINT	pos,
		   /*[in]*/ BOOL	doScroll)
{
  try
    {
      // See Q166473.
      SCROLLINFO info;
      info.cbSize = sizeof(SCROLLINFO);
      info.fMask = SIF_TRACKPOS;
      
      if (LOBYTE(scrollCode) == SB_THUMBTRACK)
	{
	  if (! GetScrollInfo(SB_HORZ, &info, SIF_TRACKPOS))
	    {
	      FATAL_WINDOWS_ERROR ("OnScroll", 0);
	    }
	  pos = info.nTrackPos;
	}
      
      if (HIBYTE(scrollCode) == SB_THUMBTRACK)
	{
	  if (! GetScrollInfo(SB_VERT, &info, SIF_TRACKPOS))
	    {
	      FATAL_WINDOWS_ERROR ("OnScroll", 0);
	    }
	  pos = info.nTrackPos;
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

  return (CMyScrollView::OnScroll(scrollCode, pos, doScroll));
}

/* _________________________________________________________________________

   DviView::OnScrollBy
   _________________________________________________________________________ */

BOOL
DviView::OnScrollBy (/*[in]*/ CSize	sizeScroll,
		     /*[in]*/ BOOL	doScroll)
{
  return (CMyScrollView::OnScrollBy(sizeScroll, doScroll));
}

/* _________________________________________________________________________

   DviView::IsPointInView
   _________________________________________________________________________ */

bool
DviView::IsPointInView (/*[in]*/ CPoint pt)
{
  CRect rect;
  GetClientRect (&rect);
  CPoint ptScroll = GetScrollPosition();
  if (pt.x < ptScroll.x)
    {
      return (false);
    }
  if (pt.x >= ptScroll.x + rect.right)
    {
      return (false);
    }
  if (pt.y < ptScroll.y)
    {
      return (false);
    }
  if (pt.y >= ptScroll.y + rect.bottom)
    {
      return (false);
    }
  return (true);
}

/* _________________________________________________________________________

   DviView::GetPagePositionX
   _________________________________________________________________________ */

int
DviView::GetPagePositionX (/*[in]*/ int pageIdx)
{
  int x = GetLeftMargin();
  if (! g_pYapConfig->doublePage)
    {
      return (x);
    }
  MIKTEX_ASSERT (pageIdx >= 0);
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  if (pDoc->IsOnRight(pageIdx))
    {
      x += pDoc->GetPaperSize().cx;
      x += GetHorizontalInterPageOffset();
    }
  return (x);
}

/* _________________________________________________________________________

   DviView::GetPagePositionY
   _________________________________________________________________________ */

int
DviView::GetPagePositionY (/*[in]*/ int pageIdx)
{
  int y = GetTopMargin();
  if (! g_pYapConfig->continuousView)
    {
      return (y);
    }
  MIKTEX_ASSERT (pageIdx >= 0);
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  int n = (g_pYapConfig->doublePage
	   ? pDoc->GetRowIdx(pageIdx)
	   : pageIdx);
  MIKTEX_ASSERT (n >= 0);
  if (n > 0)
    {
      y += n * (pDoc->GetPaperSize().cy);
      y += n * GetVerticalInterPageOffset();
    }
  return (y);
}

/* _________________________________________________________________________

   DviView::ChangePage
   _________________________________________________________________________ */

void
DviView::ChangePage (/*[in]*/ int pageIdx)
{
  MIKTEX_ASSERT (pageIdx >= 0);
  curPageIdx = pageIdx;

  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);

  CPoint pt = GetScrollPosition();
  if (! g_pYapConfig->maintainHorizontalPosition)
    {
      pt.x = 0;
    }

  if (g_pYapConfig->continuousView)
    {
      ScrollToPosition (CPoint(pt.x, GetPagePositionY(pageIdx)));
      return;
    }
  
  if (! g_pYapConfig->maintainVerticalPosition)
    {
      pt.y = 0;
    }

  ScrollToPosition (pt);

  int lm = GetLeftMargin();
  int tm = GetTopMargin();

  CRect rectClient;
  GetClientRect (&rectClient);

  // clear the search cursor
  if (pageSearchPosition != pageIdx)
    {
      ClearSearchPosition ();
    }

  // check to see whether we're on the right-hand side of the row
  bool onRight = pDoc->IsOnRight(pageIdx);

  // invalidate left margin
  CRect rectLM (0, 0, lm, rectClient.bottom);
  InvalidateRect (&rectLM, TRUE);

  // invalidate top margin
  CRect rectTM (0, 0, rectClient.right, tm);
  InvalidateRect (&rectTM, TRUE);

  // check to see whether the page exists
  BOOL pageExists = ! (g_pYapConfig->doublePage && onRight);
  
  // invalidate paper
  CSize sizePage = pDoc->GetPaperSize();
  CRect rectPaper (lm, tm, lm + sizePage.cx, tm + sizePage.cy);
  InvalidateRect (&rectPaper, ! pageExists);

  // invalidate top border
  CRect rectTB (lm,
		tm - GetPageBorderWidth(),
		lm + sizePage.cx + GetPageBorderWidth(),
		tm);
  InvalidateRect (&rectTB, ! pageExists);

  // invalidate right border
  CRect rectRB (lm + sizePage.cx,
		tm,
		lm + sizePage.cx + GetPageBorderWidth(),
		tm + sizePage.cy + GetPageBorderWidth());
  InvalidateRect (&rectRB, ! pageExists);

  // invalidate bottom border
  CRect rectBB (lm - GetPageBorderWidth(),
		tm + sizePage.cy,
		lm + sizePage.cx,
		tm + sizePage.cy + GetPageBorderWidth());
  InvalidateRect (&rectBB, ! pageExists);

  // invalidate left border
  CRect rectLB (lm - GetPageBorderWidth(),
		tm - GetPageBorderWidth(),
		lm,
		tm + sizePage.cy + GetPageBorderWidth());
  InvalidateRect (&rectLB, ! pageExists);
  
  // invalidate right shadow
  CRect rectRS (lm + sizePage.cx + GetPageBorderWidth(),
		tm + GetPageBorderShadow(),
		(lm + sizePage.cx + GetPageBorderWidth()
		 + GetPageBorderShadow()),
		(tm + sizePage.cy + GetPageBorderWidth()
		 + GetPageBorderShadow()));
  InvalidateRect (&rectRS, ! pageExists);

  // invalidate bottom shadow
  CRect rectBS (lm + GetPageBorderShadow(),
		tm + sizePage.cy + GetPageBorderWidth(),
		lm + sizePage.cx + GetPageBorderWidth(),
		(tm + sizePage.cy + GetPageBorderWidth()
		 + GetPageBorderShadow()));
  InvalidateRect (&rectBS, ! pageExists);

  // same procedure for page on the right
  if (g_pYapConfig->doublePage)
    {
#if 0
      CRect rectInter (lm + sizePage.cx
		       + GetPageBorderWidth() + GetPageBorderShadow(),
		       0,
		       GetHorizontalInterPageOffset(),
		       rectClient.bottom);
      InvalidateRect (rectInter, TRUE);
#else
      rectLM.OffsetRect (sizePage.cx + GetHorizontalInterPageOffset(), 0);
      InvalidateRect (rectLM, TRUE);
#endif
      rectPaper.OffsetRect (sizePage.cx + GetHorizontalInterPageOffset(), 0);
      rectTB.OffsetRect (sizePage.cx + GetHorizontalInterPageOffset(), 0);
      rectRB.OffsetRect (sizePage.cx + GetHorizontalInterPageOffset(), 0);
      rectBB.OffsetRect (sizePage.cx + GetHorizontalInterPageOffset(), 0);
      rectLB.OffsetRect (sizePage.cx + GetHorizontalInterPageOffset(), 0);
      rectRS.OffsetRect (sizePage.cx + GetHorizontalInterPageOffset(), 0);
      rectBS.OffsetRect (sizePage.cx + GetHorizontalInterPageOffset(), 0);
      pageExists =
	(pageIdx + 1 < pDoc->GetPageCount() && pDoc->IsOnRight(pageIdx + 1));
      InvalidateRect (&rectPaper, ! pageExists);
      InvalidateRect (&rectTB, ! pageExists);
      InvalidateRect (&rectRB, ! pageExists);
      InvalidateRect (&rectBB, ! pageExists);
      InvalidateRect (&rectLB, ! pageExists);
      InvalidateRect (&rectRS, ! pageExists);
      InvalidateRect (&rectBS, ! pageExists);
    }
}

/* _________________________________________________________________________

   DviView::OnNextPage
   _________________________________________________________________________ */

void
DviView::OnNextPage ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      int pageIdx = GetCurrentPageIdx();
      if (pageIdx < 0)
	{
	  UNEXPECTED_CONDITION ("DviView::OnNextPage");
	}
      int d = 1;
      CString commandPrefix = GetCommandPrefix(true);
      if (commandPrefix.GetLength() > 0)
	{
	  d = atoi(commandPrefix);
	}
      if (g_pYapConfig->doublePage
	  && pDoc->IsOnLeft(pageIdx)
	  && pageIdx + 2 * d >= 0
	  && pageIdx + 2 * d <= pDoc->GetPageCount())
	{
	  d *= 2;
	}
      if (pageIdx + d < 0 || pageIdx + d >= pDoc->GetPageCount())
	{
	  return;
	}
      ChangePage (pageIdx + d);
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

   DviView::OnUpdateNextPage
   _________________________________________________________________________ */

void
DviView::OnUpdateNextPage (/*[in]*/ CCmdUI * pCmdUI)
{
  BOOL enable = FALSE;
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  int pageIdx = GetCurrentPageIdx();
	  if (pageIdx < 0)
	    {
	      UNEXPECTED_CONDITION ("DviView::OnUpdateNextPage");
	    }
	  if (g_pYapConfig->doublePage)
	    {
	      enable =
		(pDoc->IsOnRight(pageIdx)
		 ? (pDoc->GetPageCount() - 1 > pageIdx)
		 : (pDoc->GetPageCount() - 2 > pageIdx));
	    }
	  else
	    {
	      enable = (pDoc->GetPageCount() - 1 > pageIdx);
	    }
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

   DviView::OnPrevPage
   _________________________________________________________________________ */

void
DviView::OnPrevPage ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      int pageIdx = GetCurrentPageIdx();
      if (pageIdx < 0)
	{
	  UNEXPECTED_CONDITION ("DviView::OnNextPage");
	}
      int pageIdxOld = pageIdx;
      int d = -1;
      CString commandPrefix = GetCommandPrefix(true);
      if (commandPrefix.GetLength() > 0)
	{
	  d = atoi(commandPrefix) * -1;
	}
      if (g_pYapConfig->doublePage
	  && pDoc->IsOnLeft(pageIdx)
	  && pageIdx + 2 * d >= 0
	  && pageIdx + 2 * d <= pDoc->GetPageCount())
	{
	  d *= 2;
	}
      if (pageIdx + d < 0 || pageIdx + d >= pDoc->GetPageCount())
	{
	  return;
	}
      pageIdx += d;
      if (g_pYapConfig->doublePage && pDoc->IsOnRight(pageIdx))
	{
	  if (pageIdx + 1 != pageIdxOld)
	    {
	      pageIdx += 1;
	    }
	  else if (pageIdx > 0 && pDoc->IsOnLeft(pageIdx - 1))
	    {
	      pageIdx -= 1;
	    }
	}
      ChangePage (pageIdx);
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

   DviView::OnUpdatePrevPage
   _________________________________________________________________________ */

void
DviView::OnUpdatePrevPage (/*[in]*/ CCmdUI * pCmdUI)
{
  BOOL enable = FALSE;
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  int pageIdx = GetCurrentPageIdx();
	  if (pageIdx < 0)
	    {
	      UNEXPECTED_CONDITION ("DviView::OnUpdateNextPage");
	    }
	  enable = (pageIdx > 0);
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

   DviView::OnPageDown
   _________________________________________________________________________ */

void
DviView::OnPageDown ()
{
  try
    {
      OnScrollBy (CSize(0, DeterminePageSize().cy), TRUE);
      UpdateWindow ();
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

   DviView::OnDocumentDown
   _________________________________________________________________________ */

void
DviView::OnDocumentDown ()
{
  try
    {
      if (OnScrollBy(CSize (0, DeterminePageSize().cy), TRUE))
	{
	  UpdateWindow ();
	}
      else if (! g_pYapConfig->continuousView)
	{
	  AutoRestore<bool>
	    autoRestore (g_pYapConfig->maintainVerticalPosition);
	  g_pYapConfig->maintainVerticalPosition = false;
	  OnNextPage ();
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

   DviView::OnPageUp
   _________________________________________________________________________ */

void
DviView::OnPageUp ()
{
  try
    {
      if (OnScrollBy(CSize(0, - DeterminePageSize().cy), TRUE))
	{
	  UpdateWindow ();
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

   DviView::OnDocumentUp
   _________________________________________________________________________ */

void
DviView::OnDocumentUp ()
{
  try
    {
      if (OnScrollBy(CSize(0, - DeterminePageSize().cy), TRUE))
	{
	  UpdateWindow ();
	}
      else if (! g_pYapConfig->continuousView)
	{
	  AutoRestore<bool>
	    autoRestore (g_pYapConfig->maintainVerticalPosition);
	  g_pYapConfig->maintainVerticalPosition = false;
	  OnPrevPage ();
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

   DviView::OnLineDown
   _________________________________________________________________________ */

void
DviView::OnLineDown ()
{
  int mapMode;
  CSize sizeTotal, sizePage, sizeLine;
  GetDeviceScrollSizes (mapMode, sizeTotal, sizePage, sizeLine);
  if (OnScrollBy(CSize(0, sizeLine.cy), TRUE))
    {
      UpdateWindow ();
    }
}

/* _________________________________________________________________________

   DviView::OnLineUp
   _________________________________________________________________________ */

void
DviView::OnLineUp ()
{
  int mapMode;
  CSize sizeTotal, sizePage, sizeLine;
  GetDeviceScrollSizes (mapMode, sizeTotal, sizePage, sizeLine);
  if (OnScrollBy(CSize(0, - sizeLine.cy), TRUE))
    {
      UpdateWindow ();
    }
}

/* _________________________________________________________________________

   DviView::OnLineLeft
   _________________________________________________________________________ */

void
DviView::OnLineLeft ()
{
  int mapMode;
  CSize sizeTotal, sizePage, sizeLine;
  GetDeviceScrollSizes (mapMode, sizeTotal, sizePage, sizeLine);
  if (OnScrollBy(CSize(- sizeLine.cx, 0), TRUE))
    {
      UpdateWindow ();
    }
}

/* _________________________________________________________________________

   DviView::OnLineRight
   _________________________________________________________________________ */

void
DviView::OnLineRight ()
{
  int mapMode;
  CSize sizeTotal, sizePage, sizeLine;
  GetDeviceScrollSizes (mapMode, sizeTotal, sizePage, sizeLine);
  if (OnScrollBy(CSize(sizeLine.cx, 0), TRUE))
    {
      UpdateWindow ();
    }
}

/* _________________________________________________________________________

   DviView::OnPageLeft
   _________________________________________________________________________ */

void
DviView::OnPageLeft ()
{
  try
    {
      if (OnScrollBy(CSize(- DeterminePageSize().cx, 0), TRUE))
	{
	  UpdateWindow ();
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

   DviView::OnPageRight
   _________________________________________________________________________ */

void
DviView::OnPageRight ()
{
  try
    {
      if (OnScrollBy(CSize(DeterminePageSize().cx, 0), TRUE))
	{
	  UpdateWindow ();
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

   DviView::OnLastPage
   _________________________________________________________________________ */

void
DviView::OnLastPage ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      int p = pDoc->GetPageCount();
      if (p <= 0)
	{
	  UNEXPECTED_CONDITION ("DviView::OnLastPage");
	}
      ChangePage (p - 1);
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

   DviView::OnUpdateLastPage
   _________________________________________________________________________ */

void
DviView::OnUpdateLastPage (/*[in]*/ CCmdUI * pCmdUI) 
{
  BOOL enable = FALSE;
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  enable = (pDoc->GetPageCount() - 1 > GetCurrentPageIdx());
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

   DviView::OnFirstPage
   _________________________________________________________________________ */

void
DviView::OnFirstPage ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetPageCount() <= 0)
	{
	  UNEXPECTED_CONDITION ("DviView::OnLastPage");
	}
      ChangePage (0);
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

   DviView::OnUpdateFirstPage
   _________________________________________________________________________ */

void
DviView::OnUpdateFirstPage (/*[in]*/ CCmdUI * pCmdUI) 
{
  BOOL enable = FALSE;
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  enable = (GetCurrentPageIdx() > 0);
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

   DviView::OnGotoPage
   _________________________________________________________________________ */

void
DviView::OnGotoPage ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      CString commandPrefix = GetCommandPrefix(true);
      int pageIdx;
      if (commandPrefix.GetLength() > 0)
	{
	  int pageNum = atoi(commandPrefix);
	  pageIdx = pDoc->FindPage(pageNum);
	}
      else
	{
	  GotoPageDialog dlg (this, pDoc, curPageIdx);
	  if (dlg.DoModal() != IDOK)
	    {
	      return;
	    }
	  pageIdx = dlg.GetPageIdx();
	}
      if (pageIdx < 0)
	{
	  UNEXPECTED_CONDITION ("DviView::OnGotoPage");
	}
      RememberCurrentLocation ();
      while (! forwardLocations.empty())
	{
	  forwardLocations.pop ();
	}
      ChangePage (pageIdx);
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

   DviView::OnUpdateGotoPage
   _________________________________________________________________________ */

void
DviView::OnUpdateGotoPage (/*[in]*/ CCmdUI * pCmdUI)
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

   DviView::OnHome
   _________________________________________________________________________ */

void
DviView::OnHome ()
{
  CPoint pt = GetScrollPosition();
  if (! g_pYapConfig->maintainHorizontalPosition)
    {
      pt.x = 0;
    }
  pt.y = 0;
  ScrollToPosition (pt);
}

/* _________________________________________________________________________

   DviView::OnEnd
   _________________________________________________________________________ */

void
DviView::OnEnd ()
{
  try
    {
      CPoint pt = GetScrollPosition();
      if (! g_pYapConfig->maintainHorizontalPosition)
	{
	  pt.x = 0;
	}
      pt.y = GetTotalSize().cy;
      ScrollToPosition (pt);
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

   DviView::Zoom
   _________________________________________________________________________ */

void
DviView::Zoom (/*[in]*/ bool zoomIn)
{
  int sign = (zoomIn ? -1 : 1);
  int d = 1 * sign;
  CString commandPrefix = GetCommandPrefix(true);
  if (commandPrefix.GetLength() > 0)
    {
      d = atoi(commandPrefix) * sign;
    }
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  int oldShrink = pDoc->GetDisplayShrinkFactor();
  int newShrink = pDoc->GetDisplayShrinkFactor() + d;
  if (newShrink < 1)
    {
      return;
    }
  double f = static_cast<double>(oldShrink) / newShrink;
  YapLog (T_("Yap zoom: %d/%d=%f"), oldShrink, newShrink, f);
  CPoint oldScrollPosition = GetScrollPosition();
  pDoc->Shrink (d);
  g_pYapConfig->displayShrinkFactor = newShrink;
#if DVI_DONT_RENDER_GRAPHICS_SPECIALS
  pDoc->ForgetGraphicsInclusions ();
  tempFiles.clear ();
#endif
  pDoc->UpdateAllViews (0);
  CPoint pt;
  CSize size = GetTotalSize();
  pt.x = static_cast<int>(oldScrollPosition.x * f + 0.5);
  if (pt.x > size.cx)
    {
      pt.x = size.cx;
    }
  pt.y = static_cast<int>(oldScrollPosition.y * f + 0.5);
  if (pt.y > size.cy)
    {
      pt.y = size.cy;
    }
  ScrollToPosition (pt);
}

/* _________________________________________________________________________

   DviView::OnZoomIn
   _________________________________________________________________________ */

void
DviView::OnZoomIn ()
{
  try
    {
      Zoom (true);
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

   DviView::OnUpdateZoomIn
   _________________________________________________________________________ */

void
DviView::OnUpdateZoomIn (/*[in]*/ CCmdUI * pCmdUI) 
{
  BOOL enable = FALSE;
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  enable = (pDoc->GetDisplayShrinkFactor() > 1);
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

   DviView::OnZoomOut
   _________________________________________________________________________ */

void
DviView::OnZoomOut ()
{
  try
    {
      Zoom (false);
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

   DviView::OnUpdateZoomOut
   _________________________________________________________________________ */

void
DviView::OnUpdateZoomOut (/*[in]*/ CCmdUI * pCmdUI)
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

   DviView::OnLButtonDown
   _________________________________________________________________________ */

void
DviView::OnLButtonDown (/*[in]*/ UINT	flags,
			/*[in]*/ CPoint	point) 
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);

      cursorPosition = point;

      leftButtonDown = true;

      CString hyperTarget;

      int pageIdx;
      int x, y;
      
      bool havePage = GetPageUnderCursor(pageIdx, x, y);

      if (havePage && pageIdx < 0)
	{
	  UNEXPECTED_CONDITION ("DviView::OnLButtonDown");
	}

      if (! havePage || GetHyperTeXSpecialAtCursor(hyperTarget))
	{
	}
      else if (currentMouseTool == RulerTool)
	{
	  MIKTEX_ASSERT (! mouseCaptured);
	  MIKTEX_ASSERT (pToolWindow == 0);
	  SetCapture ();
	  if (g_pYapConfig->magGlassHidesCursor)
	    {
	      ShowCursor (FALSE);
	    }
	  mouseCaptured = true;
	  CPoint screenPoint = point;
	  ClientToScreen (&screenPoint);
	  Ruler * pRuler =
	    new Ruler (this,
		       pDoc->GetPaperSize(),
		       g_pYapConfig->unit,
		       (static_cast<double>(pDoc->GetDisplayResolution())
			/ pDoc->GetDisplayShrinkFactor()),
		       (((flags & MK_SHIFT) != 0)
			? Ruler::Vertical
			: Ruler::Horizontal));
	  pRuler->Create (screenPoint);
	  pToolWindow = pRuler;
	}
      else if (currentMouseTool == Hand)
	{
	  ::SetCursor (AfxGetApp()->LoadCursor(IDC_PRESSED_HAND));
	  toolActive = true;
	}
      else if (currentMouseTool == MagnifyingGlass)
	{
	  MIKTEX_ASSERT (! mouseCaptured);
	  MIKTEX_ASSERT (pToolWindow == 0);
	  DviMagnifyingGlass::Size size = DviMagnifyingGlass::Small;
	  int magGlassShrinkFactor = 0;
	  if ((flags & MK_SHIFT) != 0)
	    {
	      size = DviMagnifyingGlass::Medium;
	      magGlassShrinkFactor = g_pYapConfig->magGlassMediumShrinkFactor;
	    }
	  if ((flags & MK_CONTROL) != 0)
	    {
	      size = DviMagnifyingGlass::Large;
	      magGlassShrinkFactor = g_pYapConfig->magGlassLargeShrinkFactor;
	    }
	  CString commandPrefix = GetCommandPrefix(true);
	  if (commandPrefix.GetLength() > 0)
	    {
	      magGlassShrinkFactor = atoi(commandPrefix);
	    }
	  if (magGlassShrinkFactor <= 0)
	    {
	      magGlassShrinkFactor = g_pYapConfig->magGlassSmallShrinkFactor;
	    }
	  SetCapture ();
	  if (g_pYapConfig->magGlassHidesCursor)
	    {
	      ShowCursor (FALSE);
	    }
	  mouseCaptured = true;
	  CPoint screenPoint = point;
	  ClientToScreen (&screenPoint);
	  DviMagnifyingGlass * pGlass =
	    new DviMagnifyingGlass (this,
				    GetDocument(),
				    size,
				    magGlassShrinkFactor);
	  pGlass->Create (screenPoint);
	  pToolWindow = pGlass;
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

  CMyScrollView::OnLButtonDown (flags, point);
}

/* _________________________________________________________________________

   DviView::OnLButtonUp
   _________________________________________________________________________ */

void
DviView::OnLButtonUp (/*[in]*/ UINT	flags,
		      /*[in]*/ CPoint	point) 
{
  try
    {
      cursorPosition = point;

      leftButtonDown = false;

      toolActive = false;

      if (pToolWindow != 0)
	{
	  MIKTEX_ASSERT (mouseCaptured);
	  if (g_pYapConfig->magGlassHidesCursor)
	    {
	      ShowCursor (TRUE);
	    }
	  if (! ReleaseCapture())
	    {
	      UNEXPECTED_CONDITION ("DviView::OnLButtonUp");
	    }
	  mouseCaptured = false;
	  pToolWindow->ShowWindow (SW_HIDE);
	  pToolWindow->DestroyWindow ();
	  pToolWindow = 0;
	}

      CString hyperTarget;

      if (GetHyperTeXSpecialAtCursor(hyperTarget))
	{
	  Navigate (hyperTarget);
	}
      else if (currentMouseTool == Hand)
	{
	  ::SetCursor (AfxGetApp()->LoadCursor(IDC_HAND_TOOL));
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

  CMyScrollView::OnLButtonUp (flags, point);
}

/* _________________________________________________________________________

   DviView::OnMouseMove
   _________________________________________________________________________ */

void
DviView::OnMouseMove (/*[in]*/ UINT	flags,
		      /*[in]*/ CPoint	point) 
{
  try
    {
      switch (currentMouseTool)
	{
	case RulerTool:
	  if (pToolWindow != 0)
	    {
	      Ruler * pRuler = dynamic_cast<Ruler *>(pToolWindow);
	      if (pRuler == 0)
		{
		  UNEXPECTED_CONDITION ("DviView::OnMouseMove");
		}
	      CPoint screenPoint = point;
	      ClientToScreen (&screenPoint);
	      pRuler->MoveRuler (screenPoint);
	    }
	  break;
	case Hand:
	  if ((flags & MK_LBUTTON) != 0
	      && toolActive
	      && OnScrollBy(CSize(cursorPosition.x - point.x,
				  cursorPosition.y - point.y),
			    TRUE))
	    {
	      UpdateWindow ();
	    }
	  break;
	case MagnifyingGlass:
	  if (pToolWindow != 0)
	    {
	      DviMagnifyingGlass * pGlass =
		dynamic_cast<DviMagnifyingGlass *>(pToolWindow);
	      if (pGlass == 0)
		{
		  UNEXPECTED_CONDITION ("DviView::OnMouseMove");
		}
	      CPoint screenPoint = point;
	      ClientToScreen (&screenPoint);
	      pGlass->MoveGlass (screenPoint);
	    }
	  break;
	}

      cursorPosition = point;

      ASSERT_VALID (AfxGetApp ());
      MainFrame * pMain =
	reinterpret_cast<MainFrame*>((AfxGetApp())->m_pMainWnd);
      ASSERT_VALID (pMain);
      MIKTEX_ASSERT (pMain->IsKindOf(RUNTIME_CLASS(MainFrame)));

      CString hyperTarget;

      if (! leftButtonDown && (GetHyperTeXSpecialAtCursor(hyperTarget)))
	{
	  pMain->SetMessageText (hyperTarget);
	}
      else
	{
	  pMain->SetMessageText ("");
	}

#if 0
      {
	int pageIdx, x, y;
	bool bPage = GetPageUnderCursor(pageIdx, x, y);
	int sheetIdx
	  = (g_pYapConfig->continuousView
	     ? DetermineRowIdx(GetScrollPosition().y + point.y)
	     : 0);
	CString info;
	info.Format ("r:%d p:%d(%d) x:%d y:%d",
		     sheetIdx,
		     pageIdx,
		     static_cast<int>(bPage),
		     x,
		     y);
	pMain->SetWindowText (strInfo);
      }
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

  CMyScrollView::OnMouseMove (flags, point);
}

/* _________________________________________________________________________

   DviView::OnSetCursor
   _________________________________________________________________________ */

BOOL
DviView::OnSetCursor (/*[in]*/ CWnd *	pWnd,
		      /*[in]*/ UINT	hitTest,
		      /*[in]*/ UINT	message) 
{
  if (hitTest != HTCLIENT)
    {
      return (CMyScrollView::OnSetCursor(pWnd, hitTest, message));
    }

  try
    {
      CString hyperTarget;

      int pageIdx;
      int x, y;
      if (! GetPageUnderCursor(pageIdx, x, y))
	{
	  return (CMyScrollView::OnSetCursor(pWnd, hitTest, message));
	}
      if (pageIdx < 0)
	{
	  UNEXPECTED_CONDITION ("DviView::OnSetCursor");
	}
      if (! leftButtonDown && GetHyperTeXSpecialAtCursor(hyperTarget))
	{
	  ::SetCursor (AfxGetApp()->LoadCursor(IDC_HAND_POINT));
	}
      else
	{
	  switch (currentMouseTool)
	    {
	    case Hand:
	      if (leftButtonDown)
		{
		  ::SetCursor (AfxGetApp()->LoadCursor(IDC_PRESSED_HAND));
		}
	      else
		{
		  ::SetCursor (AfxGetApp()->LoadCursor(IDC_HAND_TOOL));
		}
	      break;
	    case MagnifyingGlass:
	      ::SetCursor (AfxGetApp()->LoadCursor(IDC_MAG));
	      break;
	    default:
	      ::SetCursor (AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	      break;
	    }
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

  return (FALSE);
}

/* _________________________________________________________________________

   DviView::OnContextMenu
   _________________________________________________________________________ */

void
DviView::OnContextMenu (/*[in]*/ CWnd * pWnd,
			/*[in]*/ CPoint point) 
{
  UNUSED_ALWAYS (pWnd);
  try
    {
      if (mouseCaptured)
	{
	  if (g_pYapConfig->magGlassHidesCursor)
	    {
	      ShowCursor (TRUE);
	    }
	  if (! ReleaseCapture())
	    {
	      UNEXPECTED_CONDITION ("DviView::OnContextMenu");
	    }
	  mouseCaptured = false;
	}
      if (pToolWindow)
	{
	  if (! pToolWindow->DestroyWindow())
	    {
	      UNEXPECTED_CONDITION ("DviView::OnContextMenu");
	    }
	  pToolWindow = 0;
	}
      ASSERT_VALID (AfxGetApp());
      ASSERT_VALID (AfxGetApp()->m_pMainWnd);
      CMenu menu;
      if (! menu.LoadMenu(IDR_CONTEXT_MENU))
	{
	  FATAL_WINDOWS_ERROR ("CMenu::LoadMenu", 0);
	}
      AutoDetachMenu autoDetachMenu (&menu);
      CMenu * pPopup = menu.GetSubMenu(0);
      MIKTEX_ASSERT (pPopup != 0);
      if (! pPopup->TrackPopupMenu(TPM_RIGHTBUTTON,
				   point.x,
				   point.y,
				   AfxGetApp()->m_pMainWnd,
				   0))
	{
	  UNEXPECTED_CONDITION ("DviView::OnContextMenu");
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

   DviView::OnToolsHand
   _________________________________________________________________________ */

void
DviView::OnToolsHand () 
{
  currentMouseTool = Hand;
  g_pYapConfig->lastTool = Hand;
}

/* _________________________________________________________________________

   DviView::OnUpdateToolsHand
   _________________________________________________________________________ */

void
DviView::OnUpdateToolsHand (/*[in]*/ CCmdUI * pCmdUI) 
{
  pCmdUI->SetCheck (currentMouseTool == Hand ? 1 : 0);
}

/* _________________________________________________________________________

   DviView::OnToolsMagnifyingGlass
   _________________________________________________________________________ */

void
DviView::OnToolsMagnifyingGlass () 
{
  currentMouseTool = MagnifyingGlass;
  g_pYapConfig->lastTool = MagnifyingGlass;
}

/* _________________________________________________________________________

   DviView::OnUpdateToolsMagnifyingGlass
   _________________________________________________________________________ */

void
DviView::OnUpdateToolsMagnifyingGlass (/*[in]*/ CCmdUI * pCmdUI) 
{
  pCmdUI->SetCheck (currentMouseTool == MagnifyingGlass ? 1 : 0);
}

/* _________________________________________________________________________

   DviView::OnToolsArrow
   _________________________________________________________________________ */

void
DviView::OnToolsArrow () 
{
  currentMouseTool = RulerTool;
  g_pYapConfig->lastTool = RulerTool;
}

/* _________________________________________________________________________

   DviView::OnUpdateToolsArrow
   _________________________________________________________________________ */

void
DviView::OnUpdateToolsArrow (/*[in]*/ CCmdUI * pCmdUI) 
{
  pCmdUI->SetCheck (currentMouseTool == RulerTool ? 1 : 0);
}

/* _________________________________________________________________________

   DviView::OnLButtonDblClk
   _________________________________________________________________________ */

void
DviView::OnLButtonDblClk (/*[in]*/ UINT		flags,
			  /*[in]*/ CPoint	point) 
{
  UNUSED_ALWAYS (flags);
  cursorPosition = point;
  OnPageEditor ();
}

/* _________________________________________________________________________

   DviView::GetPoint
   _________________________________________________________________________ */

bool
DviView::GetPoint (/*[out]*/ int & x,
		   /*[out]*/ int & y)
{
  int pageIdx;
  if (! GetPageUnderCursor(pageIdx, x, y))
    {
      return (false);
    }
  if (pageIdx < 0)
    {
      UNEXPECTED_CONDITION ("");
    }
  return (true);
}

/* _________________________________________________________________________

   DviView::ClearSearchPosition
   _________________________________________________________________________ */

void
DviView::ClearSearchPosition ()
{
  searchPosition.x = searchPosition.y = -1;
  pageSearchPosition = -1;
}

/* _________________________________________________________________________

   DviView::OnChar
   _________________________________________________________________________ */

void
DviView::OnChar (/*[in]*/ UINT	ch,
		 /*[in]*/ UINT	repeatCount,
		 /*[in]*/ UINT	flags)
{
  try
    {
      if (! AddCommandPrefixChar(static_cast<char>(ch)))
	{
	  CMyScrollView::OnChar(ch, repeatCount, flags);
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

   DviView::GetPageUnderCursor
   _________________________________________________________________________ */

bool
DviView::GetPageUnderCursor (/*[in]*/ int &	pageIdx,
			     /*[in]*/ int &	x,
			     /*[in]*/ int &	y)
{
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);

  if (pDoc->GetDviFileStatus() != DviDoc::DVIFILE_LOADED)
    {
      return (false);
    }

  CPoint ptScroll = GetScrollPosition();

  if (g_pYapConfig->continuousView)
    {
      int n = DetermineRowIdx(ptScroll.y + cursorPosition.y);
      if (g_pYapConfig->doublePage)
	{
	  pageIdx = pDoc->DeterminePageIdx(n, false);
	}
      else
	{
	  pageIdx = n;
	}
    }
  else
    {
      pageIdx = curPageIdx;
    }

  if (pageIdx < 0)
    {
      UNEXPECTED_CONDITION ("DviView::GetPageUnderCursor");
    }

  CSize sizePage = pDoc->GetPaperSize();

  if (ptScroll.x + cursorPosition.x > GetLeftMargin() + sizePage.cx)
    {
      if (g_pYapConfig->doublePage)
	{
	  if (ptScroll.x + cursorPosition.x
	      >= (GetLeftMargin() + sizePage.cx
		  + GetHorizontalInterPageOffset()))
	    {
	      if (pDoc->IsOnLeft(pageIdx)
		  && pageIdx + 1 < pDoc->GetPageCount())
		{
		  ++ pageIdx;
		}
	    }
	}
    }

  if (g_pYapConfig->doublePage)
    {
      if (pDoc->IsOnLeft(pageIdx))
	{
	  x = ptScroll.x + cursorPosition.x - GetLeftMargin();
	}
      else
	{
	  x = (ptScroll.x + cursorPosition.x - GetLeftMargin()
	       - sizePage.cx - GetHorizontalInterPageOffset());
	}
    }
  else
    {
      x = ptScroll.x + cursorPosition.x - GetLeftMargin();
    }
      
  int n = (g_pYapConfig->continuousView
	   ? (g_pYapConfig->doublePage
	      ? pDoc->GetRowIdx(pageIdx)
	      : pageIdx)
	   : 0);

  y = ptScroll.y + cursorPosition.y;
  
  y -= GetTopMargin();
  y -= n * sizePage.cy;
  y -= n * GetVerticalInterPageOffset();

  bool havePage = (x >= 0 && x < sizePage.cx && y >= 0 && y < sizePage.cy);
      
  x = UnPixelShrink(pDoc, x);
  y = UnPixelShrink(pDoc, y);
  
  return (havePage);
}

/* _________________________________________________________________________

   DviView::GetLeftMargin
   _________________________________________________________________________ */

int
DviView::GetLeftMargin ()
  const
{
  int x = 10;
  bool center = true;
  if (center)
    {
      RECT rect;
      GetClientRect (&rect);
      int widthWindow = rect.right - rect.left;
      const DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      CSize sizePage = pDoc->GetPaperSize();
      int widthDisplay;
      if (g_pYapConfig->doublePage)
	{
	  widthDisplay = 2 * sizePage.cx + GetHorizontalInterPageOffset();
	}
      else
	{
	  widthDisplay = sizePage.cx;
	}
      if (widthDisplay < widthWindow)
	{
	  x = (widthWindow - widthDisplay) / 2;
	}
    }
  return (x + GetRulerWidth());
}

/* _________________________________________________________________________

   DviView::GetTopMargin
   _________________________________________________________________________ */

int
DviView::GetTopMargin () const
{
  int y = 10;
  bool center = true;
  if (center && ! g_pYapConfig->continuousView)
    {
      RECT rect;
      GetClientRect (&rect);
      int heightWindow = rect.bottom - rect.top;
      const DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      CSize sizePage = pDoc->GetPaperSize();
      if (sizePage.cy < heightWindow)
	{
	  y = (heightWindow - sizePage.cy) / 2;
	}
    }
  return (y + GetRulerHeight());
}

/* _________________________________________________________________________

   DviView::OnViewRefresh
   _________________________________________________________________________ */

void
DviView::OnViewRefresh ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      int pageIdx = curPageIdx;
      pDoc->Reread ();
      if (pageIdx < pDoc->GetPageCount())
	{
	  curPageIdx = pageIdx;
	}
      else
	{
	  curPageIdx = pDoc->GetPageCount() - 1;
	}
      pDoc->UpdateAllViews (0);
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

   DviView::GetVisiblePages
   _________________________________________________________________________ */

void
DviView::GetVisiblePages (/*[in]*/ int &	pageIdx1,
			  /*[in]*/ int &	pageIdx2)
  const
{
  const DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  if (pPrintInfo != 0)
    {
      pageIdx1 = pageIdx2 = pPrintInfo->m_nCurPage - 1;
      MIKTEX_ASSERT (pageIdx1 >= 0);
    }
  else if (g_pYapConfig->continuousView)
    {
      CRect rectClient;
      GetClientRect (&rectClient);
      int iRowIdx1 = DetermineRowIdx(GetScrollPosition().y);
      int iRowIdx2 = DetermineRowIdx(GetScrollPosition().y
				     + rectClient.bottom - 1);
      if (! g_pYapConfig->doublePage)
	{
	  pageIdx1 = iRowIdx1;
	  pageIdx2 = iRowIdx2;
	}
      else
	{
	  pageIdx1 = pDoc->DeterminePageIdx(iRowIdx1, false);
	  pageIdx2 = pDoc->DeterminePageIdx(iRowIdx2, true);
	}
      return;
    }
  else if (! g_pYapConfig->doublePage)
    {
      pageIdx1 = pageIdx2 = curPageIdx;
    }
  else
    {
      pageIdx1 = pageIdx2 = curPageIdx;
      if (pDoc->IsOnLeft(pageIdx1)
	  && pageIdx1 + 1 < pDoc->GetPageCount()
	  && pDoc->IsOnRight(pageIdx1 + 1))
	{
	  pageIdx2 = pageIdx1 + 1;
	}
    }
}

/* _________________________________________________________________________

   DviView::GetCurrentPageIdx
   _________________________________________________________________________ */

int
DviView::GetCurrentPageIdx ()
  const
{
  if (g_pYapConfig->continuousView)
    {
      int sheetIdx = DetermineRowIdx(GetScrollPosition().y);
      if (g_pYapConfig->doublePage)
	{
	  const DviDoc * pDoc = GetDocument();
	  ASSERT_VALID (pDoc);
	  return (pDoc->DeterminePageIdx(sheetIdx, false));
	}
      else
	{
	  return (sheetIdx);
	}
    }
  else
    {
      return (curPageIdx);
    }
}

/* _________________________________________________________________________

   DviView::DetermineRowIdx
   _________________________________________________________________________ */

int
DviView::DetermineRowIdx (/*[in]*/ int y)
  const
{
  MIKTEX_ASSERT (g_pYapConfig->continuousView);
#if 0
  if (y < 0)
    {
      UNEXPECTED_CONDITION ("DviView::DetermineRowIdx");
    }
#endif
  const DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  int n = (g_pYapConfig->doublePage
	   ? pDoc->GetRowCount()
	   : pDoc->GetPageCount());
  if (n <= 0)
    {
      UNEXPECTED_CONDITION ("DviView::DetermineRowIdx");
    }
  int cy = pDoc->GetPaperSize().cy * n;
  cy += (GetTopMargin() + (n - 1) * GetVerticalInterPageOffset());
  if (y >= cy)
    {
      return (n - 1);
    }
  return (static_cast<int>((static_cast<double>(y) * n) / cy));
}

/* _________________________________________________________________________

   DviView::OnFileDocumentProperties
   _________________________________________________________________________ */

void
DviView::OnFileDocumentProperties ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      CPropertySheet dlg (T_("Document Properties"), 0, 0);
      DocumentFontsPage pageFonts;
      pageFonts.fontTable = pDoc->GetFontTable();
      dlg.AddPage (&pageFonts);
      dlg.DoModal ();
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

   DviView::OnUpdateFileDocumentProperties
   _________________________________________________________________________ */

void
DviView::OnUpdateFileDocumentProperties (/*[in]*/ CCmdUI * pCmdUI)
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

   DviView::OnMakeFonts
   _________________________________________________________________________ */

LRESULT
DviView::OnMakeFonts (/*[in]*/ WPARAM wParam,
		      /*[in]*/ LPARAM lParam)
{
  UNUSED_ALWAYS (lParam);
  UNUSED_ALWAYS (wParam);
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetPageCount() > 0)
	{
	  curPageIdx = 0;
	  CWaitCursor wait;
	  pDoc->MakeFonts ();
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
  return (0);
}

/* _________________________________________________________________________

   DviView::GetGammaTableIndex
   _________________________________________________________________________ */

size_t
DviView::GetGammaTableIndex ()
{
  if (gammaTable.size() == 0)
    {
      InitializeGammaTable ();
    }
  size_t bestIdx = UINT_MAX;
  double bestd = 100000.0;
  for (size_t idx = 0; idx < gammaTable.size(); ++ idx)
    {
      double d = fabs(g_pYapConfig->gamma - gammaTable[idx]);
      if (d < bestd)
	{
	  bestIdx = idx;
	  bestd = d;
	}
    }
  return (bestIdx);
}

/* _________________________________________________________________________

   DviView::OnLighterText
   _________________________________________________________________________ */

void
DviView::OnLighterText() 
{
  try
    {
      size_t idx = GetGammaTableIndex();
      MIKTEX_ASSERT (idx > 0 && idx < gammaTable.size());
      g_pYapConfig->gamma = gammaTable[idx - 1];
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

   DviView::OnUpdateLighterText
   _________________________________________________________________________ */

void
DviView::OnUpdateLighterText (/*[in]*/ CCmdUI * pCmdUI) 
{
  try
    {
      size_t idx = GetGammaTableIndex();
      MIKTEX_ASSERT (idx >= 0 && idx < gammaTable.size());
      pCmdUI->Enable (idx > 0
		      && g_pYapConfig->dviPageMode != DviPageMode::Dvips);
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

   DviView::OnDarkerText
   _________________________________________________________________________ */

void
DviView::OnDarkerText () 
{
  try
    {
      size_t idx = GetGammaTableIndex();
      MIKTEX_ASSERT (idx + 1 < gammaTable.size());
      g_pYapConfig->gamma = gammaTable[idx + 1];
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

   DviView::OnUpdateDarkerText
   _________________________________________________________________________ */

void
DviView::OnUpdateDarkerText (/*[in]*/ CCmdUI * pCmdUI) 
{
  try
    {
      size_t idx = GetGammaTableIndex();
      MIKTEX_ASSERT (idx >= 0 && idx < gammaTable.size());
      pCmdUI->Enable (idx + 1 < gammaTable.size()
		      && g_pYapConfig->dviPageMode != DviPageMode::Dvips);
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

   DviView::InitializeGammaTable
   _________________________________________________________________________ */

void
DviView::InitializeGammaTable ()
{
  gammaTable.clear ();
  CSVList gammas (g_pYapConfig->gammaValues, _T(' '));
  while (gammas.GetCurrent() != 0)
    {
      gammaTable.push_back (_tstof(gammas.GetCurrent()));
      ++ gammas;
    }
  if (gammaTable.size() == 0)
    {
      gammaTable.push_back (1.0);
    }
  sort (gammaTable.begin(), gammaTable.end());
}

/* _________________________________________________________________________

   DviView::OnDviPageModeAuto
   _________________________________________________________________________ */

#if DVI_DONT_RENDER_POSTSCRIPT_SPECIALS
void
DviView::OnDviPageModeAuto ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      pDoc->SetDviPageMode (DviPageMode::Auto);
      pDoc->Reread ();
      pDoc->UpdateAllViews (0);
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
#endif

/* _________________________________________________________________________

   DviView::OnUpdateDviPageModeAuto
   _________________________________________________________________________ */

#if DVI_DONT_RENDER_POSTSCRIPT_SPECIALS
void
DviView::OnUpdateDviPageModeAuto (/*[in]*/ CCmdUI * pCmdUI)
{
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  pCmdUI->SetCheck (pDoc->GetDviPageMode() == DviPageMode::Auto);
}
#endif

/* _________________________________________________________________________

   DviView::OnDviPageModePk
   _________________________________________________________________________ */

void
DviView::OnDviPageModePk ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      pDoc->SetDviPageMode (DviPageMode::Pk);
      pDoc->Reread ();
      pDoc->UpdateAllViews (0);
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

   DviView::OnUpdateDviPageModePk
   _________________________________________________________________________ */

void
DviView::OnUpdateDviPageModePk (/*[in]*/ CCmdUI * pCmdUI)
{
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  pCmdUI->SetCheck (pDoc->GetDviPageMode() == DviPageMode::Pk);
}

/* _________________________________________________________________________

   DviView::OnDviPageModeDvips
   _________________________________________________________________________ */

void
DviView::OnDviPageModeDvips ()
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      pDoc->SetDviPageMode (DviPageMode::Dvips);
      pDoc->Reread ();
      pDoc->UpdateAllViews (0);
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

   DviView::OnUpdateDviPageModeDvips
   _________________________________________________________________________ */

void
DviView::OnUpdateDviPageModeDvips (/*[in]*/ CCmdUI * pCmdUI)
{
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  pCmdUI->SetCheck (pDoc->GetDviPageMode() == DviPageMode::Dvips);
}
