/* screen.cpp: METAFONT online displays

   Copyright (C) 1998-2005 Christian Schenk
   Copyright (C) 1998 Wolfgang Kleinschmidt

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with This file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

#if defined(_MSC_VER)
#include <conio.h>
#endif

#define C4PEXTERN extern
#if defined(TRAPMF)
#include "trapmf.h"
#else
#include "mf.h"
#endif

#include "mf-miktex.h"

/* _________________________________________________________________________

   Global Variables
   _________________________________________________________________________ */

namespace {
  DWORD g_dwThreadID;
  HANDLE g_hEvtReady;
  HANDLE g_hThread;
  HDC g_hdc;
  HINSTANCE g_hinstance;
  HWND g_hwnd;
  MiKTeX::Core::CriticalSectionMonitor g_cs;
  POINT g_pt;
}

/* _________________________________________________________________________

   ScreenWindowProc
   _________________________________________________________________________ */

LRESULT
CALLBACK
ScreenWindowProc (/*[in]*/ HWND		hwnd,
		  /*[in]*/ UINT		uMsg,
		  /*[in]*/ WPARAM	wParam,
		  /*[in]*/ LPARAM	lParam)
{
  static HBITMAP hbitmap;	// <fixme/>

  MIKTEX_BEGIN_CRITICAL_SECTION(&g_cs)
    {
      switch (uMsg)
	{
	  
	case WM_CREATE:
	  {
	    HDC hPaintDC = GetDC(hwnd);
	    g_hdc = CreateCompatibleDC(hPaintDC);
	    if (g_hdc == 0)
	      {
		SetEvent (g_hEvtReady);
		PostQuitMessage (1);
	      }
	    hbitmap = CreateCompatibleBitmap(hPaintDC, g_pt.x, g_pt.y);
	    if (hbitmap == 0)
	      {
		DeleteObject (g_hdc);
		g_hdc = 0;
		SetEvent (g_hEvtReady);
		PostQuitMessage (1);
	      }
	    ReleaseDC (hwnd, hPaintDC);
	    hPaintDC = 0;
	    SelectObject (g_hdc, hbitmap);
	    SelectObject (g_hdc, GetStockObject(LTGRAY_BRUSH));
	    SelectObject (g_hdc, GetStockObject(NULL_PEN));
	    Rectangle (g_hdc, 0, 0, g_pt.x, g_pt.y);
	    return (0);
	    break;
	  }
	  
	case WM_DESTROY:
	  DeleteObject (hbitmap);
	  hbitmap = 0;
	  DeleteDC (g_hdc);
	  g_hdc = 0;
	  PostQuitMessage (0);
	  return (0);
	  break;
	  
	case WM_PAINT:
	  {
	    PAINTSTRUCT ps;
	    HDC hPaintDC = BeginPaint(hwnd, &ps);
	    BitBlt (hPaintDC, 0, 0, g_pt.x, g_pt.y, g_hdc, 0, 0, SRCCOPY);
	    EndPaint (hwnd, &ps);
	    return (0);
	    break;
	  }

	default:
	  return (DefWindowProc(hwnd, uMsg, wParam, lParam));
	}
    }
  MIKTEX_END_CRITICAL_SECTION();
}


/* _________________________________________________________________________

   ScreenThreadFunc
   _________________________________________________________________________ */

DWORD
WINAPI
ScreenThreadFunc (/*[in]*/ LPVOID lpv)
{
  LPPOINT ppt = reinterpret_cast<LPPOINT>(lpv);

  _TCHAR szWindowName[100];
#if _MSC_VER >= 1400
  _stprintf_s (szWindowName,
	       100,
	       _T("METAFONT Screen (%d x %d)"),
	       ppt->x,
	       ppt->y);
#else
  _stprintf (szWindowName, _T("METAFONT Screen (%d x %d)"), ppt->x, ppt->y);
#endif
  g_hwnd =
    CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		   _T("MF_Screen"),
		   szWindowName,
		   WS_OVERLAPPEDWINDOW,
		   CW_USEDEFAULT,
		   0,
		   CW_USEDEFAULT,
		   0,
		   0,
		   0,
		   g_hinstance,
		   0);
  if (g_hwnd == 0)
    {
      SetEvent (g_hEvtReady);
      PostQuitMessage (1);
    }
  
  ShowWindow (g_hwnd, SW_SHOWNOACTIVATE);

  SetEvent (g_hEvtReady);
  
  MSG msg;
  while (GetMessage(&msg, g_hwnd, 0, 0))
    {
      TranslateMessage (&msg);
      DispatchMessage (&msg);
    }

  UnregisterClass (_T("MF_Screen"), g_hinstance);

  return (0);
}


/* _________________________________________________________________________

   miktexinitscreen
   _________________________________________________________________________ */

C4P_boolean
miktexinitscreen (/*[in]*/ int	w,
		  /*[in]*/ int	h)
{
#if defined(TRAPMF)
  return (true);
#endif

  if (! THEAPP.ScreenEnabled())
    {
      return (false);
    }

  WNDCLASS wc;
  ZeroMemory (&wc, sizeof(WNDCLASS));
  wc.style = CS_NOCLOSE;
  wc.lpfnWndProc = ScreenWindowProc;
  wc.hInstance = g_hinstance;
  wc.hIcon = LoadIcon(g_hinstance, MAKEINTRESOURCE(IDR_MFSCREEN));
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
  wc.lpszClassName = _T("MF_Screen");
  RegisterClass (&wc);

  g_pt.x = w;
  g_pt.y = h;

  g_hEvtReady = CreateEvent(0, FALSE, FALSE, 0);
  if (g_hEvtReady == 0)
    {
      return (false);
    }
  
  g_hThread = CreateThread(0, 0, ScreenThreadFunc, &g_pt, 0, &g_dwThreadID);
  if (g_hThread == 0)
    {
      return (false);
    }

  WaitForSingleObject (g_hEvtReady, INFINITE);
  
  if (g_hwnd == 0)
    {
      return (false);
    }

  if (g_hdc == 0)
    {
      return (false);
    }
        
  return (true);
}

/* _________________________________________________________________________

   miktexblankrectangle
   _________________________________________________________________________ */

void
miktexblankrectangle (/*[in]*/ screencol	left_col,
		      /*[in]*/ screencol	right_col,
		      /*[in]*/ screenrow	top_row,
		      /*[in]*/ screenrow	bot_row)
{
#if defined(TRAPMF)
  return;
#endif
  MIKTEX_BEGIN_CRITICAL_SECTION(&g_cs)
    {
      HGDIOBJ hgdiobj1 = SelectObject(g_hdc, GetStockObject(WHITE_BRUSH));
      HGDIOBJ hgdiobj2 = SelectObject(g_hdc, GetStockObject(NULL_PEN));
      Rectangle (g_hdc, left_col, top_row, right_col, bot_row);
      SelectObject (g_hdc, hgdiobj1);
      SelectObject (g_hdc, hgdiobj2);
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   miktexupdatescreen
   _________________________________________________________________________ */

void
miktexupdatescreen ()
{
#if defined(TRAPMF)
  return;
#endif
  MIKTEX_BEGIN_CRITICAL_SECTION(&g_cs)
    {
      // ShowWindow (g_hwnd, SW_SHOWNORMAL);
      InvalidateRect (g_hwnd, 0, FALSE);
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   miktexpaintrow
   _________________________________________________________________________ */

void
miktexpaintrow (/*[in]*/ screenrow	r,
		/*[in]*/ pixelcolor	b,
		/*[in]*/ transspec 	a,
		/*[in]*/ screencol	n)
{
#if defined(TRAPMF)
  return;
#endif
  MIKTEX_BEGIN_CRITICAL_SECTION(&g_cs)
    {
      HGDIOBJ hgdiobj = SelectObject(g_hdc, GetStockObject(BLACK_PEN));
      screencol k = n;
      MoveToEx (g_hdc, a[k], r, 0);
      do
	{
	  -- k;
	  if (b)
	    {
	      LineTo (g_hdc, a[k]-1, r);
	    }
	  else
	    {
	      MoveToEx (g_hdc, a[k], r, 0);
	    }
	  b = ! b;
	}
      while (k);
      SelectObject (g_hdc, hgdiobj);
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   DllMain
   _________________________________________________________________________ */

int
APIENTRY 
DllMain (/*[in]*/ HINSTANCE	hinstance,
	 /*[in]*/ DWORD		reason,
	 /*[in]*/ LPVOID	ignore2)
{
  if (reason == DLL_PROCESS_ATTACH)
    {
      g_hinstance = hinstance;
    }
  return (1);
}
