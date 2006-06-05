/* ProxyAuthenticationDialog.h:					-*- C++ -*-

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

class ProxyAuthenticationDialog : public CDialog
{
private:
  enum { IDD = IDD_PROXY_AUTHENTICATION };

protected:
  DECLARE_MESSAGE_MAP();

private:
  DECLARE_DYNAMIC(ProxyAuthenticationDialog)
    
public:
  ProxyAuthenticationDialog (/*[in]*/ CWnd * pParent = 0);

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

protected:
  virtual
  BOOL
  OnInitDialog ();

public:
  MiKTeX::Core::tstring
  GetName ()
    const
  {
    return (static_cast<const MIKTEXCHAR *>(name));
  }

public:
  MiKTeX::Core::tstring
  GetPassword ()
    const
  {
    return (static_cast<const MIKTEXCHAR *>(password));
  }

protected:
  afx_msg
  void
  OnChangeName ();

private:
  CString name;

private:
  CString password;
};
