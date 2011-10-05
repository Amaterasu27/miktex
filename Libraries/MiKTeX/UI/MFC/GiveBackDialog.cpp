/* GiveBackDialog.cpp:

   Copyright (C) 2011 Christian Schenk

   This file is part of the MiKTeX UI Library.

   The MiKTeX UI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX UI Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"
#include "resource.h"

#include "GiveBackDialog.h"

IMPLEMENT_DYNAMIC(GiveBackDialog, CDialog);

/* _________________________________________________________________________

   GiveBackDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(GiveBackDialog, CDialog)
  ON_NOTIFY(NM_CLICK, IDC_SYSLINK_GIVE_BACK, &OnClickUrl)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   GiveBackDialog::GiveBackDialog
   _________________________________________________________________________ */

GiveBackDialog::GiveBackDialog (/*[in]*/ CWnd * pParent)
  : CDialog (GiveBackDialog::IDD, pParent)
{
}

/* _________________________________________________________________________

   GiveBackDialog::OnInitDialog
   _________________________________________________________________________ */

BOOL
GiveBackDialog::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();
  return (ret);
}

/* _________________________________________________________________________

   GiveBackDialog::DoDataExchange
   _________________________________________________________________________ */

void
GiveBackDialog::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
}

/* _________________________________________________________________________

   GiveBackDialog::OnClickUrl
   _________________________________________________________________________ */

void GiveBackDialog::OnClickUrl (NMHDR * pNMHDR,
				 LRESULT * pResult) 
{
  MiKTeX::Core::Utils::ShowWebPage (MIKTEX_URL_WWW_GIVE_BACK);
}