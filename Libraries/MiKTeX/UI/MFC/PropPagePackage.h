/* PropPagePackage.h:						-*- C++ -*-

   Copyright (C) 2000-2011 Christian Schenk

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

#if defined(_MSC_VER)
#  pragma once
#endif

class PropPagePackage : public CPropertyPage
{
protected:
  DECLARE_MESSAGE_MAP();

public:
  PropPagePackage (/*[in]*/ const PackageInfo & packageInfo);

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

private:
  virtual
  BOOL
  OnInitDialog ();

private:
  void
  SetUtf8Text (/*[in]*/ UINT	      ctrlId,
	       /*[in]*/ const char *  lpszUtf8Text);

private:
  PackageInfo packageInfo;

private:
  CString date;

private:
  CString size;
};
