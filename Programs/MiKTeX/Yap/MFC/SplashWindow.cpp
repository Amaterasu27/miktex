/* SplashWindow.cpp:

   Copyright (C) 2011 Christian Schenk
   
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

#include "SplashWindow.h"

/* _________________________________________________________________________

   SplashWindow Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SplashWindow, CDialog)
  ON_WM_TIMER()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SplashWindow::SplashWindow
   _________________________________________________________________________ */

SplashWindow::SplashWindow ()
{
}

/* _________________________________________________________________________

   SplashWindow::Show
   _________________________________________________________________________ */

void
SplashWindow::Show (/*[in]*/ unsigned seconds)
{
  if (Create(IDD_SPLASH, GetDesktopWindow()))
  {
    ShowWindow (SW_SHOWNORMAL);
    SetTimer (314, seconds * 1000, 0);
  }
}

/* _________________________________________________________________________

   SplashWindow::OnTimer
   _________________________________________________________________________ */

void
SplashWindow::OnTimer (/*[in]*/ UINT_PTR id)
{
  DestroyWindow ();
  CDialog::OnTimer (id);
}
