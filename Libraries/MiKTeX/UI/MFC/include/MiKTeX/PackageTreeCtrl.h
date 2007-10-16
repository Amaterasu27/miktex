/* MiKTeX/UI/MFC/PackageTreeCtrl:					-*- C++ -*-

   Copyright (C) 2000-2007 Christian Schenk

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

#include <miktex/core.h>
#include <miktex/mpm.h>
#include <MiKTeX/UI/MFC/Prototypes>
#include <vector>

MIKUI_MFC_BEGIN_NAMESPACE;

class
MIKTEXNOVTABLE
PackageTreeCtrl : public CTreeCtrl
{
public:
  enum { WM_ONTOGGLE = WM_APP + 22 };

public:
  AFX_EXT_CLASS
  static
  PackageTreeCtrl *
  Create (/*[in]*/ MiKTeX::Packages::PackageManager * pManager);

public:
  AFX_EXT_CLASS
  static
  void
  Destroy (/*[in]*/ PackageTreeCtrl * pCtrl);

public:
  virtual
  void
  Refresh ()
    = 0;

public:
  virtual
  bool
  GetPackageInfo (/*[in]*/ HTREEITEM				hItem,
		  /*[out]*/ MiKTeX::Packages::PackageInfo &	pi)
    = 0;

public:
  virtual
  void
  GetDelta (/*[out]*/ std::vector<MiKTeX::Core::tstring> & toBeInstalled,
	    /*[out]*/ std::vector<MiKTeX::Core::tstring> & toBeRemoved)
    = 0;
};

MIKUI_MFC_END_NAMESPACE;
