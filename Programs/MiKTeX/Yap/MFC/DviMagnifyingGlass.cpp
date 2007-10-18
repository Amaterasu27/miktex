/* DviMagnifyingGlass.cpp:

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

#include "DviDoc.h"
#include "DviMagnifyingGlass.h"
#include "DviView.h"

namespace {
  const char * const YAP_MAGNIFYING_GLASS_CLASS = "MiKTeX_mag";
  bool classRegistered = false;
}

/* _________________________________________________________________________

   DviMagnifyingGlass Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(DviMagnifyingGlass, CWnd)
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   DviMagnifyingGlass::DviMagnifyingGlass
   _________________________________________________________________________ */

DviMagnifyingGlass::DviMagnifyingGlass (/*[in]*/ DviView *	pDviView,
					/*[in]*/ DviDoc *	pDviDoc,
					/*[in]*/ DviMagnifyingGlass::Size size,
					/*[in]*/ int		shrinkFactor)

  : pDviView (pDviView),
    pDviDoc (pDviDoc),
    size (size),
    shrinkFactor (shrinkFactor)

{
  switch (size)
    {
    default:
    case Small:
      sizeWindow.cx = g_pYapConfig->magGlassSmallWidth;
      sizeWindow.cy = g_pYapConfig->magGlassSmallHeight;
      break;
    case Medium:
      sizeWindow.cx = g_pYapConfig->magGlassMediumWidth;
      sizeWindow.cy = g_pYapConfig->magGlassMediumHeight;
      break;
    case Large:
      sizeWindow.cx = g_pYapConfig->magGlassLargeWidth;
      sizeWindow.cy = g_pYapConfig->magGlassLargeHeight;
      break;
    }
}

/* _________________________________________________________________________

   DviMagnifyingGlass::OnPaint
   _________________________________________________________________________ */

void
DviMagnifyingGlass::OnPaint () 
{
  try
    {
      ASSERT_VALID (pDviDoc);

      pDviDoc->Lock ();

      AutoUnlockDviDoc autoUnlockDviDoc (pDviDoc);
      
      int pageIdx, x, y;
      
      if (! pDviView->GetPageUnderCursor(pageIdx, x, y))
	{
	  return;
	}
      
      DviPage * pPage = pDviDoc->GetLoadedPage(pageIdx);

      if (pPage == 0)
	{
	  UNEXPECTED_CONDITION (T_("DviMagnifyingGlass::OnPaint"));
	}

      AutoUnlockPage autoUnlockPage (pPage);
      
      MIKTEX_ASSERT (shrinkFactor > 0);

      AutoRestoreDisplayShrinkFactor
	autoRestorShrinkFactor (pDviDoc,
				pDviDoc->SetDisplayShrinkFactor(shrinkFactor));
      
      CPaintDC dc (this);
      
      CDC dcMem;

      if (! dcMem.CreateCompatibleDC(&dc))
	{
	  UNEXPECTED_CONDITION (T_("DviMagnifyingGlass::OnPaint"));
	}
      
      CBitmap bitmap;

      if (! bitmap.CreateCompatibleBitmap(&dc,
					  sizeWindow.cx,
					  sizeWindow.cy))
	{
	  UNEXPECTED_CONDITION (T_("DviMagnifyingGlass::OnPaint"));
	}

      AutoDeleteObject autoDeleteBitmap (&bitmap);
      
      CBitmap * pBitmapOld = dcMem.SelectObject (&bitmap);

      if (pBitmapOld == 0)
	{
	  UNEXPECTED_CONDITION (T_("DviMagnifyingGlass::OnPaint"));
	}

      AutoSelectObject autoSelectOldBitmap (&dcMem, pBitmapOld);
      
      if (! dcMem.PatBlt(0,
			 0,
			 sizeWindow.cx,
			 sizeWindow.cy,
			 WHITENESS))
	{
	  UNEXPECTED_CONDITION (T_("DviMagnifyingGlass::OnPaint"));
	}
      
      dcMem.SetViewportOrg (CPoint(- (x / shrinkFactor) + sizeWindow.cx / 2,
				   - (y / shrinkFactor) + sizeWindow.cy / 2));

      try
	{
	  DrawRules (&dcMem, true, pDviDoc, pPage);
	  DrawDviBitmaps (&dcMem, pDviDoc, pPage);
	  DrawDibChunks (&dcMem, pDviDoc, pPage);
	  DrawRules (&dcMem, false, pDviDoc, pPage);
	}
      catch (const exception &)
	{
	  dcMem.SetViewportOrg (CPoint(0, 0));
	  throw;
	}

      dcMem.SetViewportOrg (CPoint(0, 0));
      
      if (! dc.BitBlt(0,
		      0,
		      sizeWindow.cx,
		      sizeWindow.cy,
		      &dcMem,
		      0,
		      0,
		      SRCCOPY))
	{
	  UNEXPECTED_CONDITION (T_("DviMagnifyingGlass::OnPaint"));
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

   DviMagnifyingGlass::OnEraseBkgnd
   _________________________________________________________________________ */

BOOL
DviMagnifyingGlass::OnEraseBkgnd (/*[in]*/ CDC * pDC)
{
  UNUSED_ALWAYS (pDC);
  return (TRUE);
}

/* _________________________________________________________________________

   DviMagnifyingGlass::PreCreateWindow
   _________________________________________________________________________ */

BOOL
DviMagnifyingGlass::PreCreateWindow (/*[in,out]*/ CREATESTRUCT & cs) 
{
  cs.lpszClass = YAP_MAGNIFYING_GLASS_CLASS;
  return (CWnd::PreCreateWindow(cs));
}

/* _________________________________________________________________________

   DviMagnifyingGlass::Create
   _________________________________________________________________________ */

void
DviMagnifyingGlass::Create (/*[in]*/ const CPoint & point)
{
  if (! classRegistered)
    {
      WNDCLASS wndcls;
      ZeroMemory (&wndcls, sizeof (WNDCLASS));
      wndcls.style = 0; // CS_HREDRAW | CS_VREDRAW | CS_SAVEBITS;
      wndcls.lpfnWndProc = ::DefWindowProc;
      wndcls.hInstance = AfxGetInstanceHandle();
      wndcls.hIcon = 0;
      wndcls.hCursor = 0;
      wndcls.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
      wndcls.lpszMenuName = 0;
      wndcls.lpszClassName = YAP_MAGNIFYING_GLASS_CLASS;
      if (! AfxRegisterClass(&wndcls))
	{
	  UNEXPECTED_CONDITION (T_("DviMagnifyingGlass::Create"));
	}
      else
	{
	  classRegistered = true;
	}
    }
  int cx = GetSystemMetrics(SM_CXBORDER);
  int cy = GetSystemMetrics(SM_CXBORDER);
  if (! CreateEx(0,
		 YAP_MAGNIFYING_GLASS_CLASS,
		 0,
		 WS_BORDER | WS_POPUP | WS_DISABLED,
		 point.x - sizeWindow.cx / 2 - cx,
		 point.y - sizeWindow.cy / 2 - cy,
		 sizeWindow.cx + cx * 2,
		 sizeWindow.cy + cy * 2,
		 pDviView->m_hWnd,
		 0))
    {
      UNEXPECTED_CONDITION (T_("DviMagnifyingGlass::Create"));
    }
  if (g_pYapConfig->magGlassOpacity >= 0
      && g_pYapConfig->magGlassOpacity < 100)
    {
      
      MakeTransparent
	(m_hWnd,
	 RGB(255, 255, 255),
	 static_cast<BYTE>((g_pYapConfig->magGlassOpacity * 255) / 100));
    }
  ShowWindow (SW_SHOWNA);
}

/* _________________________________________________________________________

   DviMagnifyingGlass::MoveGlass
   _________________________________________________________________________ */

void
DviMagnifyingGlass::MoveGlass (/*[in]*/ const CPoint & point)
{
  int cx = GetSystemMetrics(SM_CXBORDER);
  int cy = GetSystemMetrics(SM_CXBORDER);
  Invalidate ();
  MoveWindow (point.x - sizeWindow.cx / 2 - cx,
	      point.y - sizeWindow.cy / 2 - cy,
	      sizeWindow.cx + cx * 2,
	      sizeWindow.cy + cy * 2);
}

/* _________________________________________________________________________

   DviMagnifyingGlass::PostNcDestroy
   _________________________________________________________________________ */

void
DviMagnifyingGlass::PostNcDestroy () 
{
  CWnd::PostNcDestroy ();
  delete this;
}
