/* PackageInstaller.h:						-*- C++ -*-

   Copyright (C) 2001-2006 Christian Schenk

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

#include "mpmidl.h"

class ATL_NO_VTABLE PackageInstallerCOM
  : public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<PackageInstallerCOM, &CLSID_PackageInstaller>,
    public MiKTeX::Packages::PackageInstallerCallback,
    public ISupportErrorInfo,
    public IDispatchImpl<IPackageInstaller,
			 &IID_IPackageInstaller,
			 &LIBID_MiKTeXPackageManagerLib,
			 /*wMajor =*/ 1,
			 /*wMinor =*/ 0>
{
public:
  PackageInstallerCOM ();

public:
  virtual
  ~PackageInstallerCOM ();

public:  
  DECLARE_REGISTRY_RESOURCEID(IDR_PACKAGEINSTALLER);

public:
  BEGIN_COM_MAP(PackageInstallerCOM)
    COM_INTERFACE_ENTRY(IPackageInstaller)
    COM_INTERFACE_ENTRY(IDispatch)
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
  virtual
  void
  MPMCALL
  ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine);

public:
  virtual
  bool
  MPMCALL
  OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage);

public:
  virtual
  bool
  MPMCALL
  OnProgress (/*[in]*/ MiKTeX::Packages::Notification	nf);

public:
  STDMETHOD(Add) (/*[in]*/ BSTR packageName, /*[in]*/ BOOL toBeInstalled);

public:
  STDMETHOD(InstallRemove) (/*[in]*/ IPackageInstallerCallback * pCallback);

private:
  std::vector<MiKTeX::Core::tstring> packagesToBeInstalled;

private:
  std::vector<MiKTeX::Core::tstring> packagesToBeRemoved;

private:
  CComPtr<IPackageInstallerCallback> pCallback;

private:
  MiKTeX::Packages::PackageManagerPtr pManager;

private:
  std::auto_ptr<MiKTeX::Packages::PackageInstaller> pInstaller;
};
