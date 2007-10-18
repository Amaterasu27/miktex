/* PackageInstaller.h:						-*- C++ -*-

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

class ATL_NO_VTABLE comPackageInstaller
  : public CComObjectRootEx<CComMultiThreadModel>,
    public ISupportErrorInfo,
    public IDispatchImpl<MiKTeXPackageManagerLib::IPackageInstaller,
			 &__uuidof(MiKTeXPackageManagerLib::IPackageInstaller),
			 &__uuidof(MiKTeXPackageManagerLib
				   ::MAKE_CURVER_ID(__MiKTeXPackageManager)),
			 /*wMajor =*/ 1,
			 /*wMinor =*/ 0>,
    public MiKTeX::Packages::PackageInstallerCallback
{
public:
  comPackageInstaller ();

public:
  virtual
  ~comPackageInstaller ();

public:
  BEGIN_COM_MAP(comPackageInstaller)
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
  void
  Initialize ();

public:
  virtual
  void
  MPMCALL
  ReportLine (/*[in]*/ const char * lpszLine);

public:
  virtual
  bool
  MPMCALL
  OnRetryableError (/*[in]*/ const char * lpszMessage);

public:
  virtual
  bool
  MPMCALL
  OnProgress (/*[in]*/ MiKTeX::Packages::Notification	nf);

public:
  STDMETHOD(Add) (/*[in]*/ BSTR packageName,
		  /*[in]*/ VARIANT_BOOL toBeInstalled);

public:
  STDMETHOD(SetCallback) (/*[in]*/ IUnknown * pCallback);

public:
  STDMETHOD(InstallRemove) ();

public:
  STDMETHOD(GetErrorInfo)
    (/*[out,retval]*/ MiKTeXPackageManagerLib::ErrorInfo * pErrorInfo);

public:
  STDMETHOD(UpdateDb) ();

public:
  STDMETHOD(SetRepository) (/*[in]*/ BSTR repository);

private:
  std::vector<std::string> packagesToBeInstalled;

private:
  std::vector<std::string> packagesToBeRemoved;

private:
  CComQIPtr<MiKTeXPackageManagerLib::IPackageInstallerCallback> pCallback;

private:
  MiKTeX::Packages::PackageManagerPtr pManager;

private:
  std::auto_ptr<MiKTeX::Packages::PackageInstaller> pInstaller;

private:
  MiKTeX::Core::MiKTeXException lastMiKTeXException;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_error;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_mpm;
};
