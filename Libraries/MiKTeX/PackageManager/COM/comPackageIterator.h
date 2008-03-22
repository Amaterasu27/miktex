/* comPackageIterator.h:					-*- C++ -*-

   Copyright (C) 2001-2008 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#pragma once

#include "Resource.h"

inline
void
CopyPackageInfo (/*[out]*/ MiKTeXPackageManagerLib::PackageInfo &	left,
		 /*[in]*/ const MiKTeX::Packages::PackageInfo &		right)
{
  left.deploymentName = _bstr_t(right.deploymentName.c_str()).Detach();
  left.displayName = _bstr_t(right.displayName.c_str()).Detach();
  left.title = _bstr_t(right.title.c_str()).Detach();
  left.version = _bstr_t(right.version.c_str()).Detach();
  left.description = _bstr_t(right.description.c_str()).Detach();
  left.creator = _bstr_t(right.creator.c_str()).Detach();
  left.sizeRunFiles = right.sizeRunFiles;
  left.sizeDocFiles = right.sizeDocFiles;
  left.sizeSourceFiles = right.sizeSourceFiles;
  left.numRunFiles = right.runFiles.size();
  left.numDocFiles = right.docFiles.size();
  left.numSourceFiles = right.sourceFiles.size();
  if (right.timePackaged == static_cast<time_t>(-1)
      || right.timePackaged == static_cast<time_t>(0))
    {
      left.timePackaged = COleDateTime();
    }
  else
    {
      left.timePackaged = COleDateTime(right.timePackaged);
    }
  if (right.timeInstalled == static_cast<time_t>(-1)
      || right.timeInstalled == static_cast<time_t>(0))
    {
      left.timeInstalled = COleDateTime();
      left.isInstalled = VARIANT_FALSE;
    }
  else
    {
      left.timeInstalled = COleDateTime(right.timeInstalled);
      left.isInstalled = VARIANT_TRUE;
    }
  left.archiveFileSize = right.archiveFileSize;
  memcpy (&left.digest, &right.digest, sizeof(left.digest));
}

class ATL_NO_VTABLE comPackageIterator
  : public CComObjectRootEx<CComMultiThreadModel>,
    public MiKTeXPackageManagerLib::IPackageIterator,
    public ISupportErrorInfo
{
public:
  comPackageIterator ();

public:
  virtual
  ~comPackageIterator ();

public:
  BEGIN_COM_MAP(comPackageIterator)
    COM_INTERFACE_ENTRY(MiKTeXPackageManagerLib::IPackageIterator)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP();

public:
  STDMETHOD(InterfaceSupportsErrorInfo) (/*[in]*/ REFIID riid);
  
public:
  DECLARE_PROTECT_FINAL_CONSTRUCT();
  
public:
  HRESULT
  FinalConstruct ()
  {
    return (S_OK);
  }
  
public:
  void
  FinalRelease ();

public:
  void
  Initialize ();

public:
  STDMETHOD(GetNextPackageInfo)
  (/*[out]*/ MiKTeXPackageManagerLib::PackageInfo *	pPackageInfo,
   /*[out,retval]*/ VARIANT_BOOL *			pDone);
     
private:
  MiKTeX::Packages::PackageManagerPtr pManager;

private:
  std::auto_ptr<MiKTeX::Packages::PackageIterator> pIter;
};
