/* ProxyAuthenticationDialog.cpp:

   Copyright (C) 2000-2006 Christian Schenk

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

// ProxyAuthenticationDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"

#include "ProxyAuthenticationDialog.h"


// ProxyAuthenticationDialog dialog

IMPLEMENT_DYNAMIC(ProxyAuthenticationDialog, CDialog)

/* _________________________________________________________________________

   ProxyAuthenticationDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(ProxyAuthenticationDialog, CDialog)
  ON_EN_CHANGE(IDC_PROXY_NAME, &ProxyAuthenticationDialog::OnChangeName)
END_MESSAGE_MAP();


/* _________________________________________________________________________

   ProxyAuthenticationDialog::ProxyAuthenticationDialog
   _________________________________________________________________________ */

ProxyAuthenticationDialog::ProxyAuthenticationDialog (/*[in]*/ CWnd * pParent)
  : CDialog (ProxyAuthenticationDialog::IDD, pParent)
{
}

/* _________________________________________________________________________

   ProxyAuthenticationDialog::OnInitDialog
   _________________________________________________________________________ */

BOOL
ProxyAuthenticationDialog::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();
  OnChangeName ();
  return (ret);
}

/* _________________________________________________________________________

   ProxyAuthenticationDialog::DoDataExchange
   _________________________________________________________________________ */

void
ProxyAuthenticationDialog::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  DDX_Text(pDX, IDC_PROXY_NAME, name);
  DDX_Text(pDX, IDC_PROXY_PASSWORD, password);
}

/* _________________________________________________________________________

   ProxyAuthenticationDialog::OnChangeName
   _________________________________________________________________________ */

void
ProxyAuthenticationDialog::OnChangeName ()
{
  CWnd * pWnd = GetDlgItem(IDC_PROXY_NAME);
  if (pWnd == 0)
    {
      return;
    }
  CWnd * pOK = GetDlgItem(IDOK);
  if (pOK == 0)
    {
      return;
    }
  CString str;
  pWnd->GetWindowText (str);
  pOK->EnableWindow (! str.IsEmpty());
}
