/* PropPageFiles.cpp:

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

#include "StdAfx.h"
#include "internal.h"
#include "resource.h"

#include "PropPageFiles.h"

/* _________________________________________________________________________

   PropPageFiles::PropPageFiles
   _________________________________________________________________________ */

PropPageFiles::PropPageFiles (const PackageInfo & packageInfo)
  : CPropertyPage(IDD_PROPPAGE_FILES),
    packageInfo (packageInfo)
{
}

/* _________________________________________________________________________

   PropPageFiles::DoDataExchange
   _________________________________________________________________________ */

void
PropPageFiles::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_LIST, listControl);
}


/* _________________________________________________________________________

   PropPageFiles Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PropPageFiles, CPropertyPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PropPageFiles::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropPageFiles::OnInitDialog ()
{
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      if (listControl.InsertColumn(0,
				   T_("File name"),
				   LVCFMT_LEFT,
				   listControl.GetStringWidth(T_("\
xxx mmmmmmmm.txt")),
				   0)
	  < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertColumn"), 0);
	}

      if (listControl.InsertColumn(1,
				   T_("Path"),
				   LVCFMT_LEFT,
				   listControl.GetStringWidth(T_("\
xxx mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm")),
				   1)
	  < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertColumn"), 0);
	}

      vector<tstring>::const_iterator it;
      int idx = 0;
      
      for (it = packageInfo.runFiles.begin();
	   it != packageInfo.runFiles.end();
	   ++ it, ++ idx)
	{
	  AddFile (idx, it->c_str());
	}
      
      for (it = packageInfo.docFiles.begin();
	   it != packageInfo.docFiles.end();
	   ++ it, ++ idx)
	{
	  AddFile (idx, it->c_str());
	}
      
      for (it = packageInfo.sourceFiles.begin();
       it != packageInfo.sourceFiles.end();
	   ++ it, ++ idx)
	{
	  AddFile (idx, it->c_str());
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

  return (ret);
}

/* _________________________________________________________________________

   PropPageFiles::AddFile
   _________________________________________________________________________ */

void
PropPageFiles::AddFile (/*[in]*/ int			idx,
			/*[in]*/ const MIKTEXCHAR *	lpszFileName)
{
  PathName fileName (lpszFileName);
  fileName.RemoveDirectorySpec ();
  PathName path (lpszFileName);
  path.RemoveFileSpec ();
  LVITEM lvitem;
  lvitem.iItem = idx;
  lvitem.mask = LVIF_TEXT;
  lvitem.iSubItem = 0;
  lvitem.pszText = fileName.GetBuffer();
  if (listControl.InsertItem(&lvitem) < 0)
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertItem"), 0);
    }
  lvitem.iItem = idx;
  lvitem.iSubItem = 1;
  lvitem.pszText = path.GetBuffer();
  if (! listControl.SetItem(&lvitem))
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
    }
}
