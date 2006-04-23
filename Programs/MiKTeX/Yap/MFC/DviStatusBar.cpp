/* DviStatusBar.cpp:

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

#include "DviStatusBar.h"

/* _________________________________________________________________________

   DviStatusBar Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(DviStatusBar, CStatusBar)
  ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   DviStatusBar::OnRButtonDown
   _________________________________________________________________________ */

void
DviStatusBar::OnRButtonDown (/*[in]*/ UINT	nFlags,
			     /*[in]*/ CPoint	point)
{
  UNUSED_ALWAYS (nFlags);
  try
    {
      UINT menuId = 0;
      CRect rect;
      GetItemRect (CommandToIndex(ID_INDICATOR_POINT), rect);
      if (rect.PtInRect(point))
	{
	  menuId = IDR_POINT_MENU;
	}
      if (menuId == 0)
	{
	  return;
	}
      CMenu menu;
      ClientToScreen (&point);
      ASSERT_VALID (AfxGetApp());
      ASSERT_VALID (AfxGetApp()->m_pMainWnd);
      if (! menu.LoadMenu(menuId))
	{
	  FATAL_WINDOWS_ERROR (T_("CMenu::LoadMenu"), 0);
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
	  UNEXPECTED_CONDITION (T_("DviStatusBar::OnRButtonDown"));
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
