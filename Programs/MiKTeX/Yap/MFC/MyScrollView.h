// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once

class CMyScrollView : public CView
{
	DECLARE_DYNAMIC(CMyScrollView)

// Constructors
protected:
	CMyScrollView();

public:
	static AFX_DATA const SIZE sizeDefault;
		// used to specify default calculated page and line sizes

	// in logical units - call one of the following Set routines
	void SetScaleToFitSize(SIZE sizeTotal);
	void SetScrollSizes(int nMapMode, SIZE sizeTotal,
				const SIZE& sizePage = sizeDefault,
				const SIZE& sizeLine = sizeDefault);

// Attributes
public:
	CPoint GetScrollPosition() const;       // upper corner of scrolling
	CSize GetTotalSize() const;             // logical size

	void CheckScrollBars(BOOL& bHasHorzBar, BOOL& bHasVertBar) const;

	// for device units
	CPoint GetDeviceScrollPosition() const;
	void GetDeviceScrollSizes(int& nMapMode, SIZE& sizeTotal,
			SIZE& sizePage, SIZE& sizeLine) const;

// Operations
public:
	void ScrollToPosition(POINT pt);    // set upper left position
	void FillOutsideRect(CDC* pDC, CBrush* pBrush);
	void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);

// Implementation
protected:
	_AFX_MOUSEANCHORWND* m_pAnchorWindow; // window for wheel mouse anchor
	friend class _AFX_MOUSEANCHORWND;
	int m_nMapMode;				 // mapping mode for window creation
	CSize m_totalLog;           // total size in logical units (no rounding)
	CSize m_totalDev;           // total size in device units
	CSize m_pageDev;            // per page scroll size in device units
	CSize m_lineDev;            // per line scroll size in device units

	BOOL m_bCenter;             // Center output if larger than total size
	BOOL m_bInsideUpdate;       // internal state for OnSize callback
	void CenterOnPoint(CPoint ptCenter);
	void ScrollToDevicePosition(POINT ptDev); // explicit scrolling no checking

protected:
	virtual void OnDraw(CDC* pDC) = 0;      // pass on pure virtual

	void UpdateBars();          // adjust scrollbars etc
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
		// size with no bars
	void GetScrollBarSizes(CSize& sizeSb);
	void GetScrollBarState(CSize sizeClient, CSize& needSb,
		CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);

public:
	virtual ~CMyScrollView();
#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG
	virtual void CalcWindowRect(LPRECT lpClientRect,
		UINT nAdjustType = adjustBorder);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	virtual CSize GetWheelScrollDistance(CSize sizeDistance,
		BOOL bHorz, BOOL bVert);

	// scrolling implementation support for OLE
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

	//{{AFX_MSG(CMyScrollView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
//	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT HandleMButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
#if defined(MIKTEX)
#if SIXTEENBITGDI
private:
  double m_scrollconv[2];

private:
  bool m_bBigView[2];

private:
  SCROLLINFO m_scrollInfo32[2];

public:
  BOOL GetScrollInfo32 (int nBar, LPSCROLLINFO lpScrollInfo,
			UINT nMask = SIF_ALL);

public:
  BOOL GetScrollInfo (int nBar, LPSCROLLINFO lpScrollInfo,
		      UINT nMask = SIF_ALL)
  {
    return (GetScrollInfo32(nBar, lpScrollInfo, nMask));
  }

public:
  int SetScrollPos32 (int nBar, int nPos, BOOL bRedraw = TRUE);

public:
  int SetScrollPos (int nBar, int nPos, BOOL bRedraw = TRUE)
  {
    return (SetScrollPos32(nBar, nPos, bRedraw));
  }

public:
  int GetScrollPos32 (int nBar) const;

public:
  int GetScrollPos (int nBar) const
  {
    return (GetScrollPos32(nBar));
  }

public:
  BOOL SetScrollInfo32 (int nBar, LPSCROLLINFO lpScrollInfo,
			BOOL bRedraw = TRUE);

public:
  BOOL SetScrollInfo (int nBar, LPSCROLLINFO lpScrollInfo,
		      BOOL bRedraw = TRUE)
  {
    return (SetScrollInfo32(nBar, lpScrollInfo, bRedraw));
  }

public:
  int GetScrollLimit32 (int nBar);

public:
  int GetScrollLimit (int nBar)
  {
    return (GetScrollLimit32(nBar));
  }

public:
  void SetScrollRange32 (int nBar, int nMinPos, int nMaxPos,
			 BOOL bRedraw = TRUE);

public:
  void SetScrollRange (int nBar, int nMinPos, int nMaxPos,
		       BOOL bRedraw = TRUE)
  {
    SetScrollRange32(nBar, nMinPos, nMaxPos, bRedraw);
  }
#endif
#endif
};

inline
CSize
CMyScrollView::GetTotalSize ()
  const
{
  ASSERT (this != NULL);
  return (m_totalLog);
}
