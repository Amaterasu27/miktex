/* PackageManager.h:						-*- C++ -*-

   Copyright (C) 2001-2007 Christian Schenk

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

class ATL_NO_VTABLE comPackageManager
  : public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<comPackageManager, &CLSID_PackageManager>,
    public ISupportErrorInfo,
    public IDispatchImpl<IPackageManager,
			 &IID_IPackageManager,
			 &LIBID_MiKTeXPackageManagerLib,
			 /*wMajor =*/ 1,
			 /*wMinor =*/ 0>
{
public:
  comPackageManager ();

public:
  virtual
  ~comPackageManager ();

public:
#if 0
  DECLARE_REGISTRY_RESOURCEID(IDR_PACKAGEMANAGER);
#else
  static
  HRESULT
  WINAPI
  UpdateRegistry (/*[in]*/ BOOL doRegister);
#endif

public:
  DECLARE_CLASSFACTORY_SINGLETON(comPackageManager);

public:
  BEGIN_COM_MAP(comPackageManager)
    COM_INTERFACE_ENTRY(IPackageManager)
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
  STDMETHOD(CreateInstaller) (/*[out,retval]*/
			      IPackageInstaller ** ppInstaller);

public:
  STDMETHOD(GetPackageInfo) (/*[in]*/ BSTR			deploymentName,
			     /*[out,retval]*/ PackageInfo *	pPackageInfo);


private:
  void
  CreateSession ();

private:
  MiKTeX::Core::Session * pSession;

private:
  MiKTeX::Packages::PackageManagerPtr pManager;
};

OBJECT_ENTRY_AUTO(__uuidof(PackageManager), comPackageManager);
