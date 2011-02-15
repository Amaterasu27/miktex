/* draw.cpp: DVI drawing routines

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
#include "MainFrame.h"

/* _________________________________________________________________________

   DviView::OnDraw
   _________________________________________________________________________ */

void
DviView::OnDraw (/*[in]*/ CDC * pDC)
{
  try
    {
      ASSERT_VALID (pDC);

      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);

      // do nothing if the DVI file could not be loaded
      DviDoc::DviFileStatus fileStatus = pDoc->GetDviFileStatus();
      if (fileStatus != DviDoc::DVIFILE_LOADED)
	{
	  return;
	}

      // disable display updates while printing the document
      if (pDoc->IsPrintContext() && ! pDC->IsPrinting())
	{
	  return;
	}

      // prevent recursive invocations
      static bool insideOnDraw = false;
      if (insideOnDraw)
	{
	  return;
	}
      AutoRestore<bool> restore_insideOnDraw (insideOnDraw);
      insideOnDraw = true;

      // find visible pages
      int pageIdx1;
      int pageIdx2;
      GetVisiblePages (pageIdx1, pageIdx2);
      if (pageIdx1 < 0)
	{
	  return;
	}
      MIKTEX_ASSERT (pageIdx1 <= pageIdx2);

      // initialize glyph palettes
      if (gamma != g_pYapConfig->gamma)
	{
	  gamma = g_pYapConfig->gamma;
	  InitializeDviBitmapPalettes ();
	}
  
      // select default glyph palette
      foreback fb;
      fb.fore = RGB(0,0,0);
      fb.back = RGB(255,255,255);
      fb.numcolors = (pDoc->GetShrinkFactor() == 1 ? 2 : 16);
      if (! SelectPalette(pDC->GetSafeHdc(),
			  foregroundPalettes[fb],
			  FALSE))
	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("SelectPalette", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::OnDraw");
	    }
	}
      pDC->RealizePalette ();

      // get the viewport origin
      CPoint ptViewportOrg (0, 0);
      if (! pDoc->IsPrintContext())
	{
	  ptViewportOrg = - GetScrollPosition();
	}

      int savedDC = pDC->SaveDC();

      if (savedDC == 0)
	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("SaveDC", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::OnDraw");
	    }
	}

      AutoRestoreDC autoRestoreDC (pDC, savedDC);

      CSize sizePage = pDoc->GetPaperSize();
  
      // loop visible pages
      for (int pageIdx = pageIdx1; pageIdx <= pageIdx2; ++ pageIdx)
	{
	  // calculate horizontal/vertical offsets
	  CSize shift (0, 0);
	  if (pDoc->IsPrintContext())
	    {
	      shift.cx -= pDC->GetDeviceCaps(PHYSICALOFFSETX);
	      shift.cx += g_pYapConfig->pageXShift;
	      shift.cy -= pDC->GetDeviceCaps(PHYSICALOFFSETY);
	      shift.cy += g_pYapConfig->pageYShift;
	    }
	  else
	    {
	      shift.cx = GetLeftMargin();
	      shift.cy = GetTopMargin();
	      int n = (g_pYapConfig->continuousView
		       ? (g_pYapConfig->doublePage
			  ? pDoc->GetRowIdx(pageIdx)
			  : pageIdx)
		       : 0);
	      if (n > 0)
		{
		  shift.cy += n * sizePage.cy;
		  shift.cy += n * GetVerticalInterPageOffset();
		}
	    }
      
	  // change the viewport origin
	  CPoint ptOrigin = ptViewportOrg;
	  ptOrigin += shift;
	  if (! pDoc->IsPrintContext()
	      && g_pYapConfig->doublePage
	      && pDoc->IsOnRight(pageIdx))
	    {
	      ptOrigin.x += sizePage.cx;
	      ptOrigin.x += GetHorizontalInterPageOffset();
	    }
	  pDC->SetViewportOrg (ptOrigin);
      
	  // draw the page
	  DrawPage (pDC, pageIdx);
	}
    }

  catch (const DrawingCancelledException &)
    {
      ;
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

   DviView::DrawPage
   _________________________________________________________________________ */

void
DviView::DrawPage (/*[in]*/ CDC *	pDC,
		   /*[in]*/ int		pageIdx)
{
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);

  // get the status of the DVI page
  PageStatus pageStatus = pDoc->GetPageStatus(pageIdx);

  if (pageStatus == PageStatus::Changed)
    {
      YapLog (T_("DVI document has been changed"));
    }

  bool pageLoaded = (pageStatus == PageStatus::Loaded);

  CWaitCursor * pWaitCursor = 0;

  // display a wait cursor if the page is not loaded already
  if (! pageLoaded && ! pDoc->IsPrintContext())
    {
      pWaitCursor = new CWaitCursor;
    }

  auto_ptr<CWaitCursor> autoWaitCursor (pWaitCursor);

  // draw page edges
  if (! pDoc->IsPrintContext())
    {
      DrawRulers (pDC);
      DrawPaper (pDC);
    }

  // lock the DVI page
  DviPage * pPage = pDoc->GetLoadedPage(pageIdx);
  if (pPage == 0)
    {
      UNEXPECTED_CONDITION ("DviView::DrawPage");
    }
  AutoUnlockPage autoUnlockPage (pPage);

  // render graphics
  if (g_pYapConfig->renderGraphicsInBackground)
    {
      RenderGraphicsInclusions (pDC, pPage);
    }

  // draw background rules
  DrawRules (pDC, true, pDoc, pPage);

  // draw DVI bitmaps
  DrawDviBitmaps (pDC, pDoc, pPage);

  // draw DIB chunks
  DrawDibChunks (pDC, pDoc, pPage);

  // draw foreground rules
  DrawRules (pDC, false, pDoc, pPage);

  // interpret non-graphics specials
  DrawSpecials (pDC, 3, pPage, pageIdx);

  if (! g_pYapConfig->renderGraphicsInBackground)
    {
      RenderGraphicsInclusions (pDC, pPage);
    }

  // draw search marker
  if (! pDoc->IsPrintContext() && pageSearchPosition == pageIdx)
    {
      DrawSourcePosition (pDC);
    }
}

/* _________________________________________________________________________

   DviView::DrawSpecials
   _________________________________________________________________________ */

void
DviView::DrawSpecials (/*[in]*/ CDC *			pDC,
		       /*[in]*/ int			iteration,
		       /*[in]*/ DviPage *		pPage,
		       /*[in]*/ int			pageIdx)
{
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);

  MIKTEX_ASSERT (pPage != 0);

  DviSpecial * pSpecial;

  for (int idx = 0; (pSpecial = pPage->GetSpecial(idx)) != 0; ++ idx)
    {
      switch (iteration)
	{
	case 3:
	  switch (pSpecial->GetType().Get())
	    {
	    case DviSpecialType::Hypertex:
	      if (g_pYapConfig->dviPageMode != DviPageMode::Dvips)
		{
		  HypertexSpecial * pHypertexSpecial
		    = reinterpret_cast<HypertexSpecial*>(pSpecial);
		  if (! pDoc->IsPrintContext() && ! pHypertexSpecial->IsName())
		    {
		      CPen pen;
		      if (! pen.CreatePen(PS_SOLID,
					  1,
					  GetSysColor(COLOR_WINDOWFRAME)))
			{
			  if (IsWindowsNT())
			    {
			      FATAL_WINDOWS_ERROR ("CreatePen", 0);
			    }
			  else
			    {
			      UNEXPECTED_CONDITION
				("DviView::DrawSpecials");
			    }
			}
		      AutoDeleteObject autoDeletePen (&pen);
		      CPen * pOldPen = pDC->SelectObject(&pen);
		      if (pOldPen == 0)
			{
			  UNEXPECTED_CONDITION ("DviView::DrawSpecials");
			}
		      AutoSelectObject autoSelectOldPen (pDC, pOldPen);
		      int llx, lly, urx, ury;
		      pHypertexSpecial->GetBbox (llx, lly, urx, ury);
		      pDC->MoveTo (PixelShrink(llx), PixelShrink(lly) + 2);
		      if (! pDC->LineTo(PixelShrink(urx) + 1,
					PixelShrink(lly) + 2))
			{
			  if (IsWindowsNT())
			    {
			      FATAL_WINDOWS_ERROR ("LineTo", 0);
			    }
			  else
			    {
			      UNEXPECTED_CONDITION
				("DviView::DrawSpecials");
			    }
			}
		    }
		}
	      break;
	    case DviSpecialType::Source:
	      if (! pDoc->IsPrintContext()
		  && g_pYapConfig->showSourceSpecials)
		{
		  CPen pen;
		  if (! pen.CreatePen(PS_SOLID,
				      1,
				      GetSysColor(COLOR_BTNSHADOW)))
		    {
		      if (IsWindowsNT())
			{
			  FATAL_WINDOWS_ERROR ("CreatePen", 0);
			}
		      else
			{
			  UNEXPECTED_CONDITION ("DviView::DrawSpecials");
			}
		    }
		  AutoDeleteObject autoDeletePen (&pen);
		  CPen * pOldPen = pDC->SelectObject (&pen);
		  if (pOldPen == 0)
		    {
		      UNEXPECTED_CONDITION ("DviView::DrawSpecials");
		    }
		  AutoSelectObject autoSelectOldPen (pDC, pOldPen);
		  if (! pDC->Ellipse(PixelShrink(pSpecial->GetX()) - 4,
				     PixelShrink(pSpecial->GetY()) - 4,
				     PixelShrink(pSpecial->GetX()) + 5,
				     PixelShrink(pSpecial->GetY()) + 5))
		    {
		      if (IsWindowsNT())
			{
			  FATAL_WINDOWS_ERROR ("Ellipse", 0);
			}
		      else
			{
			  UNEXPECTED_CONDITION ("DviView::DrawSpecials");
			}
		    }
		}
	      break;
	    case DviSpecialType::Tpic:
	      DrawTpicSpecial (pDC,
			       reinterpret_cast<TpicSpecial*>(pSpecial));
	      break;
	    case DviSpecialType::SolidLine:
	      {
		SolidLineSpecial * pSolidLineSpecial
		  = reinterpret_cast<SolidLineSpecial*>(pSpecial);
		unsigned w = PixelShrink(pSolidLineSpecial->GetWidth());
		if (w == 0)
		  {
		    w = 1;
		  }
		CPen pen;
		if (! pen.CreatePen(PS_SOLID,
				    w,
				    pSolidLineSpecial->GetColor()))
		  {
		    if (IsWindowsNT())
		      {
			FATAL_WINDOWS_ERROR ("CreatePen", 0);
		      }
		    else
		      {
			UNEXPECTED_CONDITION ("DviView::DrawSpecials");
		      }
		  }
		AutoDeleteObject autoDeletePen (&pen);
		CPen * pOldPen = pDC->SelectObject(&pen);
		if (pOldPen == 0)
		  {
		    UNEXPECTED_CONDITION ("DviView::DrawSpecials");
		  }
		AutoSelectObject autoSelectOldPen (pDC, pOldPen);
		pDC->MoveTo (PixelShrink(pSolidLineSpecial->GetStartX()),
			     PixelShrink(pSolidLineSpecial->GetStartY()));
		if (! pDC->LineTo((PixelShrink(pSolidLineSpecial->GetEndX())
				   + 1),
				  (PixelShrink(pSolidLineSpecial->GetEndY())
				   + 1)))
		  {
		    if (IsWindowsNT())
		      {
			FATAL_WINDOWS_ERROR ("LineTo", 0);
		      }
		    else
		      {
			UNEXPECTED_CONDITION ("DviView::DrawSpecials");
		      }
		  }
	      }
	    break;
	    }
	}
    }
}

/* _________________________________________________________________________

   DviView::DrawPaper
   _________________________________________________________________________ */

void
DviView::DrawPaper (/*[in]*/ CDC * pDC)
{
  ASSERT_VALID (pDC);

  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);

  int savedDC = pDC->SaveDC();

  if (savedDC == 0)
    {
      if (IsWindowsNT())
	{
	  FATAL_WINDOWS_ERROR ("SaveDC", 0);
	}
      else
	{
	  UNEXPECTED_CONDITION ("DviView::DrawPaper");
	}
    }

  AutoRestoreDC autoRestoreDC (pDC, savedDC);

  CSize sizePage = pDoc->GetPaperSize();
  CRect rectPage (0, 0, sizePage.cx, sizePage.cy);

  // fill paper
  {
    CBrush brushWhite (0x00ffffff); // <fixme>use page background color<fixme/>
    AutoDeleteObject autoDeleteBrush (&brushWhite);
    CBrush * pOldBrush = pDC->SelectObject(&brushWhite);
    if (pOldBrush == 0)
      {
	UNEXPECTED_CONDITION ("DviView::DrawPaper");
      }
    AutoSelectObject autoSelectOldBrush (pDC, pOldBrush);
    if (! pDC->PatBlt(rectPage.left,
		      rectPage.top,
		      rectPage.Width(),
		      rectPage.Height(),
		      PATCOPY))
      {
	if (IsWindowsNT())
	  {
	    FATAL_WINDOWS_ERROR ("PatBlt", 0);
	  }
	else
	  {
	    UNEXPECTED_CONDITION ("DviView::DrawPaper");
	  }
      }
  }
  
  MainFrame * pMain =
    reinterpret_cast<MainFrame*>((AfxGetApp())->m_pMainWnd);
  ASSERT_VALID (pMain);
  MIKTEX_ASSERT (pMain->IsKindOf(RUNTIME_CLASS(MainFrame)));

  // draw edges
  if (! pMain->IsFullScreen())
    {
      int lm = 0;
      int tm = 0;

      CBrush * pOldBrush =
	pDC->SelectObject(CBrush::FromHandle(reinterpret_cast<HBRUSH>
					     (::GetStockObject(BLACK_BRUSH))));
      
      if (pOldBrush == 0)
	{
	  UNEXPECTED_CONDITION ("DviView::DrawPaper");
	}

      AutoSelectObject autoSelectOldBrush (pDC, pOldBrush);

      // top border
      if (! pDC->Rectangle(lm,
			   tm - GetPageBorderWidth(),
			   lm + sizePage.cx + GetPageBorderWidth(),
			   tm))
	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("Rectangle", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::DrawPaper");
	    }
	}
      
      // right border
      if (! pDC->Rectangle(lm + sizePage.cx,
			   tm,
			   lm + sizePage.cx + GetPageBorderWidth(),
			   tm + sizePage.cy + GetPageBorderWidth()))
	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("Rectangle", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::DrawPaper");
	    }
	}
      
      // bottom border
      if (! pDC->Rectangle(lm - GetPageBorderWidth(),
			   tm + sizePage.cy,
			   lm + sizePage.cx,
			   tm + sizePage.cy + GetPageBorderWidth()))
	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("Rectangle", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::DrawPaper");
	    }
	}

      // left border
      if (! pDC->Rectangle(lm - GetPageBorderWidth(),
			   tm - GetPageBorderWidth(),
			   lm,
			   tm + sizePage.cy + GetPageBorderWidth()))
      	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("Rectangle", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::DrawPaper");
	    }
	}

      // right shadow
      if (! pDC->Rectangle(lm + sizePage.cx + GetPageBorderWidth(),
			   tm + GetPageBorderShadow(),
			   (lm + sizePage.cx + GetPageBorderWidth()
			    + GetPageBorderShadow()),
			   (tm + sizePage.cy + GetPageBorderWidth()
			    + GetPageBorderShadow())))
	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("Rectangle", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::DrawPaper");
	    }
	}

      // bottom shadow
      if (! pDC->Rectangle(lm + GetPageBorderShadow(),
			   tm + sizePage.cy + GetPageBorderWidth(),
			   lm + sizePage.cx + GetPageBorderWidth(),
			   (tm + sizePage.cy + GetPageBorderWidth()
			    + GetPageBorderShadow())))
	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("Rectangle", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::DrawPaper");
	    }
	}
    }
}

/* _________________________________________________________________________

   DviView::DrawRulers
   _________________________________________________________________________ */

void
DviView::DrawRulers (/*[in]*/ CDC *	pDC)
{
  // <todo/>
  UNUSED_ALWAYS (pDC);
}

/* _________________________________________________________________________

   DviView::DrawSourcePosition
   _________________________________________________________________________ */

void
DviView::DrawSourcePosition (/*[in]*/ CDC * pDC)
{
  MIKTEX_ASSERT (searchPosition.x >= 0 && searchPosition.y >= 0);
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  int savedDC = pDC->SaveDC();
  if (savedDC == 0)
    {
      if (IsWindowsNT())
	{
	  FATAL_WINDOWS_ERROR ("SaveDC", 0);
	}
      else
	{
	  UNEXPECTED_CONDITION ("DviView::DrawSourcePosition");
	}
    }
  AutoRestoreDC autoRestoreDC (pDC, savedDC);
  CPen pen;
  if (! pen.CreatePen(PS_SOLID, 3, GetSysColor(COLOR_BTNSHADOW)))
    {
      if (IsWindowsNT())
	{
	  FATAL_WINDOWS_ERROR ("CreatePen", 0);
	}
      else
	{
	  UNEXPECTED_CONDITION ("DviView::DrawSourcePosition");
	}
    }
  AutoDeleteObject autoDeletePen (&pen);
  if (pDC->SelectObject(&pen) == 0)
    {
      UNEXPECTED_CONDITION ("DviView::DrawSourcePosition");
    }
  if (! pDC->Ellipse(PixelShrink(searchPosition.x) - 4,
		     PixelShrink(searchPosition.y) - 4,
		     PixelShrink(searchPosition.x) + 5,
		     PixelShrink(searchPosition.y) + 5))
    {
      if (IsWindowsNT())
	{
	  FATAL_WINDOWS_ERROR ("Ellipse", 0);
	}
      else
	{
	  UNEXPECTED_CONDITION ("DviView::DrawSourcePosition");
	}
    }
}


/* _________________________________________________________________________

   DviView::OnEraseBkgnd
   _________________________________________________________________________ */

BOOL
DviView::OnEraseBkgnd (/*[in]*/ CDC * pDC)
{
  try
    {
      ASSERT_VALID (pDC);
      MainFrame * pMain =
	reinterpret_cast<MainFrame*>((AfxGetApp())->m_pMainWnd);
      ASSERT_VALID (pMain);
      MIKTEX_ASSERT (pMain->IsKindOf(RUNTIME_CLASS(MainFrame)));
      CBrush brushBack (pMain->IsFullScreen()
			? RGB(0, 0, 0)
			: GetSysColor(COLOR_APPWORKSPACE));
      CBrush * pOldBrush = pDC->SelectObject(&brushBack);
      if (pOldBrush == 0)
	{
	  UNEXPECTED_CONDITION ("DviView::OnEraseBkgnd");
	}
      AutoSelectObject autoSelectOldBrush (pDC, pOldBrush);
      CRect rect;
      if (pDC->GetClipBox(&rect) == ERROR)
	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("GetClipBox", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::OnEraseBkgnd");
	    }
	}
      if (! pDC->PatBlt(rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			PATCOPY))
	{
	  if (IsWindowsNT())
	    {
	      FATAL_WINDOWS_ERROR ("PatBlt", 0);
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("DviView::OnEraseBkgnd");
	    }
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

   DviView::RenderGraphicsInclusions
   _________________________________________________________________________ */

void
DviView::RenderGraphicsInclusions (/*[in]*/ CDC *	pDC,
				   /*[in]*/ DviPage *	pPage)
{
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  int shrinkFactor = pDoc->GetShrinkFactor();
  int nGraphicsInclusions = pPage->GetNumberOfGraphicsInclusions(shrinkFactor);
  for (int idx = 0; idx < nGraphicsInclusions; ++ idx)
  {
    pPage->GetGraphicsInclusion(shrinkFactor, idx)->Render (pDC->GetSafeHdc());
  }
}
