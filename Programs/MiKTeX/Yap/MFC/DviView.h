/* DviView.h:							-*- C++ -*-

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

#pragma once

#include "DviDoc.h"
#include "DviDraw.h"
#include "DviMagnifyingGlass.h"

#if USEMYSCROLLVIEW
#include "MyScrollView.h"
#else
#define CMyScrollView CScrollView
#endif

class DrawingCancelledException : public MiKTeXException
{
public:
  MIKTEXTHISCALL
  DrawingCancelledException ()
    : MiKTeXException (0,
		       T_("Drawing cancelled."),
		       0,
		       0,
		       0)
  {
  }
};

class SourceSpecialsDialog;
class DviMagnifyingGlass;

class DviView
  : public CMyScrollView,
    public DviDraw
{
protected:
  DECLARE_MESSAGE_MAP();

protected:
  DECLARE_DYNCREATE (DviView);

protected:
  DviView ();

public:
  virtual ~
  DviView ();

protected:
  virtual
  void
  OnActivateView (/*[in]*/ BOOL		activate,
		  /*[in]*/ CView *	pActivateView,
		  /*[in]*/ CView *	pDeactivateView);

protected:
  virtual
  void
  OnBeginPrinting (/*[in]*/ CDC *		pDC,
		   /*[in]*/ CPrintInfo *	pInfo);

protected:
  virtual
  void
  OnDraw (/*[in]*/ CDC * pDC);

protected:
  virtual
  BOOL
  OnScrollBy (/*[in]*/ CSize	sizeScroll,
	      /*[in]*/ BOOL	doScroll);

protected:
  virtual
  void
  OnEndPrinting (/*[in]*/ CDC *		pDC,
		 /*[in]*/ CPrintInfo *	pInfo);

protected:
  virtual
  BOOL
  OnPreparePrinting (/*[in]*/ CPrintInfo * pInfo);

protected:
  virtual
  void
  OnInitialUpdate ();

protected:
  virtual
  void
  OnPrint (/*[in]*/ CDC *		pDC,
	   /*[in]*/ CPrintInfo *	pInfo);

protected:
  virtual
  BOOL
  OnScroll (/*[in]*/ UINT	scrollCode,
	    /*[in]*/ UINT	pos,
	    /*[in]*/ BOOL	doScroll);

protected:
  virtual
  void
  OnUpdate (/*[in]*/ CView *	pSender,
	    /*[in]*/ LPARAM	lHint,
	    /*[in]*/ CObject *	pHint);

protected:
  afx_msg
  void
  OnLighterText ();
  
protected:
  afx_msg
  void
  OnUpdateLighterText (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdateDarkerText (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnDarkerText ();
  
protected:
  afx_msg
  void
  OnDviPageModeAuto ();
  
protected:
  afx_msg
  void
  OnUpdateDviPageModeAuto (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnDviPageModePk ();
  
protected:
  afx_msg
  void
  OnUpdateDviPageModePk (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg void OnDviPageModeDvips ();
  
protected:
  afx_msg
  void
  OnUpdateDviPageModeDvips (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  BOOL
  OnEraseBkgnd (/*[in]*/ CDC * pDC);

protected:
  afx_msg
  BOOL
  OnSetCursor (/*[in]*/ CWnd *	pWnd,
	       /*[in]*/ UINT	hitTest,
	       /*[in]*/ UINT	message);

protected:
  afx_msg
  LRESULT
  OnMakeFonts (/*[in]*/ WPARAM wParam,
	       /*[in]*/ LPARAM lParam);

protected:
  afx_msg
  void
  OnBack ();

protected:
  afx_msg
  void
  OnChar (/*[in]*/ UINT chhar,
	  /*[in]*/ UINT repeatCount,
	  /*[in]*/ UINT flags);

protected:
  afx_msg
  void
  OnContextMenu (/*[in]*/ CWnd * pWnd,
		 /*[in]*/ CPoint point);

protected:
  afx_msg
  void
  OnDocumentDown ();

protected:
  afx_msg
  void
  OnDocumentUp ();

protected:
  afx_msg
  void
  OnEnd ();

protected:
  afx_msg
  void
  OnFileDocumentProperties ();

protected:
  afx_msg
  void
  OnUpdateFileDocumentProperties (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnFileDvips ();

protected:
  afx_msg
  void
  OnFirstPage ();

protected:
  afx_msg
  void
  OnForward ();

protected:
  afx_msg
  void
  OnGotoPage ();

protected:
  afx_msg
  void
  OnHome ();

protected:
  afx_msg
  void
  OnLButtonDblClk (/*[in]*/ UINT	flags,
		   /*[in]*/ CPoint	point);

protected:
  afx_msg
  void
  OnLButtonDown (/*[in]*/ UINT		flags,
		 /*[in]*/ CPoint	point);

protected:
  afx_msg
  void
  OnLButtonUp (/*[in]*/ UINT	flags,
	       /*[in]*/ CPoint	point);

protected:
  afx_msg
  void
  OnLastPage ();

protected:
  afx_msg
  void
  OnLineDown ();

protected:
  afx_msg
  void
  OnLineLeft ();

protected:
  afx_msg
  void
  OnLineRight ();

protected:
  afx_msg
  void
  OnLineUp ();

protected:
  afx_msg
  void
  OnMouseMove (/*[in]*/ UINT	flags,
	       /*[in]*/ CPoint	point);

protected:
  afx_msg
  void
  OnNextPage ();

protected:
  afx_msg
  void
  OnPageDown ();

protected:
  afx_msg
  void
  OnPageEditor ();

protected:
  afx_msg
  void
  OnPageLeft ();

protected:
  afx_msg
  void
  OnPageRight ();

protected:
  afx_msg
  void
  OnPageUp ();

protected:
  afx_msg
  void
  OnPrevPage ();

protected:
  afx_msg
  void
  OnToolsArrow ();

protected:
  afx_msg
  void
  OnToolsHand ();

protected:
  afx_msg
  void
  OnToolsMagnifyingGlass ();

protected:
  afx_msg
  void
  OnToolsSourcespecials ();

protected:
  afx_msg
  void
  OnUpdateBack (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateFilePrint (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateFirstPage (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateForward (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateLastPage (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateNextPage (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdatePrevPage (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateToolsArrow (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateToolsHand (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateToolsMagnifyingGlass (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateZoomIn (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateFileDvips (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdatePageEditor (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateZoomOut (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateToolsSourcespecials (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnUpdateGotoPage (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnViewRefresh ();

protected:
  afx_msg
  void
  OnZoomIn ();

protected:
  afx_msg
  void
  OnZoomOut ();

public:
  bool
  IsZooming ()
    const
  {
    return (dynamic_cast<DviMagnifyingGlass*>(pToolWindow) != 0);
  }

public:
  int
  PixelShrink (/*[in]*/ int pxl)
    const
  {
    ASSERT_VALID (m_pDocument);
    return (pxl
	    / (reinterpret_cast<DviDoc*>(m_pDocument)->GetShrinkFactor()));
  }

public:
  void
  CloseSourceSpecialsDialog ();

public:
  DviDoc *
  GetDocument ();

public:
  const DviDoc *
  GetDocument ()
    const;

public:
  bool
  GetPageUnderCursor (/*[in]*/ int & pageIdx,
		      /*[in]*/ int & x,
		      /*[in]*/ int & y);

public:
  bool
  GetPoint (/*[in]*/ int & x,
	    /*[in]*/ int & y);

public:
  bool
  GotoSrcSpecial (/*[in]*/ int			line,
		  /*[in]*/ const CString &	fileName);

public:
  bool
  GetSource (/*[out]*/ PathName &	fileName,
	     /*[out]*/ int &		line);
  
public:
  bool
  Navigate (/*[in]*/ const char *	lpszUrl,
	    /*[in]*/ bool		remember = true);

private:
  bool warnPostScript;

private:
  struct location
  {
    int pageidx;
    CPoint pt;
  };

private:
  struct PrintRange
  {
    enum { All, Range } nRange;
    enum { EvenOdd, EvenOnly, OddOnly } nEvenOdd;
    int nFirst;
    int nLast;
  };

private:
  enum MouseTool { RulerTool = 0, Hand = 1, MagnifyingGlass = 2 };

private:
  int
  GetHorizontalInterPageOffset ()
    const
  {
    return (6);
  }

private:
  int
  GetVerticalInterPageOffset ()
    const
  {
    return (6);
  }

private:
  int
  GetPagePositionX (/*[in]*/ int pageIdx);

private:
  bool
  IsPointInView (/*[in]*/ CPoint pt);

private:
  int
  GetPagePositionY (/*[in]*/ int pageIdx);

private:
  int
  GetPageBorderWidth ()
    const
  {
    return (1);
  }

private:
  int
  GetPageBorderShadow ()
    const
  {
    return (2);
  }

private:
  int
  GetLeftMargin ()
    const;

private:
  unsigned
  GetMode ()
  {
    DviDoc * pDoc = GetDocument();
    ASSERT (pDoc != 0);
    return (pDoc->IsPrintContext() ? DVIVIEW_PRINTER : DVIVIEW_DISPLAY);
  }

private:
  int
  GetRulerHeight ()
    const
  {
    return (0);
  }

private:
  int
  GetRulerWidth ()
    const
  {
    return (0);
  }

private:
  int
  GetTopMargin ()
    const;

private:
  void
  ChangePage (/*[in]*/ int pageIdx);

private:
  void
  ClearSearchPosition ();

private:
  TempFile
  CreateTempBitmapFile (/*[in]*/ const PathName &	origFileName);
  
private:
  void
  DrawPage (/*[in]*/ CDC * pDC,
	    /*[in]*/ int pageIdx);

private:
  void
  DrawPaper (/*[in]*/ CDC * pDC);

private:
  void
  DrawPsdefSpecial /*[in]*/ (PsdefSpecial * pPsdefSpecial);

private:
  void
  DrawPsfileSpecial (/*[in]*/ PsfileSpecial * pPsfileSpecial);

private:
  void
  DrawPsSpecial (/*[in]*/ DvipsSpecial * pDvipsSpecial);

private:
  void
  DrawRulers (/*[in]*/ CDC * pDC);

private:
  void
  DrawSourcePosition (/*[in]*/ CDC * pDC);

private:
  void
  DrawSpecials (/*[in]*/ CDC *		pDC,
		/*[in]*/ int		iteration,
		/*[in]*/ DviPage *	pPage,
		/*[in]*/ int		pageIdx);

private:
  void
  DrawTpicArc (/*[in]*/ CDC *			pDC,
	       /*[in]*/ TpicArcSpecial *	pArc);

private:
  void
  DrawTpicEllipse (/*[in]*/ CDC *	pDC,
		   /*[in]*/ int		x,
		   /*[in]*/ int		y,
		   /*[in]*/ int		xc,
		   /*[in]*/ int		yc,
		   /*[in]*/ int		xr,
		   /*[in]*/ int		yr);

private:
  void
  DrawTpicLine (/*[in]*/ CDC *				pDC,
		/*[in]*/ int				x,
		/*[in]*/ int				y,
		/*[in]*/ const TpicSpecial::point &	ptFrom,
		/*[in]*/ const TpicSpecial::point &	ptTo);

private:
  void
  DrawTpicPoly (/*[in]*/ CDC *			pDC,
		/*[in]*/ TpicPolySpecial *	pPoly);

private:
  void
  DrawTpicSpecial (/*[in]*/ CDC *		pDC,
		   /*[in]*/ TpicSpecial *	pTpicSpecial);

private:
  void
  DrawTpicSpline (/*[in]*/ CDC *		pDC,
		  /*[in]*/ TpicPolySpecial *	pPoly);

private:
  bool
  FindDviFile (/*[in]*/ const char *	lpszFileName,
	       /*[out]*/ PathName &		result);

private:
  bool
  FindGraphicsFile (/*[in]*/ const char *	lpszFileName,
		    /*[in]*/ PathName &		result);

private:
  void
  FlushTpicPath (/*[in]*/ CDC *				pDC,
		 /*[in]*/ int				x,
		 /*[in]*/ int				y,
		 /*[in]*/ const TpicSpecial::path &	path);

private:
  void
  FlushTpicPathD (/*[in]*/ CDC *			pDC,
		  /*[in]*/ int				x,
		  /*[in]*/ int				y,
		  /*[in]*/ const TpicSpecial::path &	path,
		  /*[in]*/ double			inchesPerDash,
		  /*[in]*/ bool				dotted);

private:
  void
  FlushTpicSpline (/*[in]*/ CDC *			pDC,
		   /*[in]*/ int				x,
		   /*[in]*/ int				y,
		   /*[in]*/ const TpicSpecial::path &	path);

private:
  bool
  GetHyperTeXSpecialAtCursor (/*[in]*/ CString & hyperTarget);

private:
  void
  GotoLocation (/*[in]*/ const location & loc);

private:
  void
  IncludeGraphics (/*[in]*/ int			pageIdx,
		   /*[in]*/ GraphicsSpecial *	pGraphicsSpecial);

private:
  void
  InitPostScript ();

private:
  void
  RenderGraphicsInclusions (/*[in]*/ CDC *	pDC,
			    /*[in]*/ int	pageIdx);
  
private:
  bool
  IsOtherDviFileLabel (/*[in]*/ const char *	lpszLabel,
		       /*[in]*/ CString &		dviFileName,
		       /*[in]*/ CString &		hashLabel);

private:
  HENHMETAFILE
  LoadEnhMetaFile (/*[in]*/ const char * lpszFileName);

private:
  void
  PrintPostScript (/*[in]*/ const CString &		dviFileName,
		   /*[in]*/ const CString &		printerName,
		   /*[in]*/ const DviView::PrintRange &	printRange);

private:
  void
  RememberCurrentLocation (/*[in]*/ bool goingBack = false);

private:
  void
  Zoom (/*[in]*/ bool zoomIn);

private:
  CSize
  DeterminePageSize ();

private:
  void
  GetVisiblePages (/*[in]*/ int &	pageIdx1,
		   /*[in]*/ int &	pageIdx2)
    const;

public:
  int
  GetCurrentPageIdx ()
    const;

private:
  int
  DetermineRowIdx (/*[in]*/ int y)
    const;

private:
  static
  void
  InitializeGammaTable ();

private:
  static
  size_t
  GetGammaTableIndex ();



private:
  stack<location> backLocations;

private:
  stack<location> forwardLocations;

private:
  int curPageIdx;

private:
  CPoint cursorPosition;

private:
  CPoint searchPosition;

private:
  int pageSearchPosition;

private:
  SourceSpecialsDialog * pSourceSpecialDialog;

private:
  MouseTool currentMouseTool;

private:
  CWnd * pToolWindow;

private:
  bool mouseCaptured;

private:
  bool toolActive;

private:
  bool leftButtonDown;

private:
  CPrintInfo * pPrintInfo;

private:
  double tpicConv;

private:
  map<string, TempFile> tempFiles;

private:
  static vector<double> gammaTable;
};

/* _________________________________________________________________________

   DviView::GetDocument
   _________________________________________________________________________ */

inline
const DviDoc *
DviView::GetDocument ()
  const
{
  return (reinterpret_cast<const DviDoc*>(m_pDocument));
}

/* _________________________________________________________________________

   DviView::GetDocument
   _________________________________________________________________________ */

inline
DviDoc *
DviView::GetDocument ()
{
  return (reinterpret_cast<DviDoc*>(m_pDocument));
}

/* _________________________________________________________________________

   UnPixelShrink
   _________________________________________________________________________ */

inline int
UnPixelShrink (DviDoc *	pDoc,
	       int		pxl)
{
  return (pxl * pDoc->GetShrinkFactor());
}
