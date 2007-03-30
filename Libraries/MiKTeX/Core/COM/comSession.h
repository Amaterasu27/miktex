/* comSession.h: MiKTeX session					-*- C++ -*-

   Copyright (C) 2006-2007 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#pragma once

#include "Resource.h"

#include "sessionidl.h"

class ATL_NO_VTABLE comSession
  : public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<comSession, &CLSID_MiKTeXSession>,
    public ISupportErrorInfo,
    public IDispatchImpl<ISession,
			 &IID_ISession,
			 &LIBID_MiKTeXSessionLib,
			 /*wMajor =*/ 1,
			 /*wMinor =*/ 0>
{
public:
  comSession ();

public:
  virtual
  ~comSession ();

public:
  static
  HRESULT
  WINAPI
  UpdateRegistry (/*[in]*/ BOOL doRegister);

public:
  DECLARE_CLASSFACTORY_SINGLETON(comSession);

public:
  BEGIN_COM_MAP(comSession)
    COM_INTERFACE_ENTRY(ISession)
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
  STDMETHOD(RegisterRootDirectories) (/*[in]*/ BSTR	rootDirectories);

public:
  STDMETHOD(FindPkFile) (/*[in]*/ BSTR			fontName,
			 /*[in]*/ BSTR			mode,
			 /*[in]*/ LONG			dpi,
			 /*[out]*/ BSTR *		path,
			 /*[out,retval]*/ VARIANT_BOOL * found);

public:
  STDMETHOD(GetErrorInfo) (/*[out,retval]*/ ErrorInfo * pErrorInfo);

public:
  STDMETHOD(GetMiKTeXSetupInfo) (/*[out,retval]*/ MiKTeXSetupInfo * setupInfo);

public:
  STDMETHOD(GetRootDirectory) (/*[in]*/ LONG		rootIdx,
			       /*[out,retval]*/ BSTR *	rootDirectory);

private:
  void
  CreateSession ();

private:
  MiKTeX::Core::MiKTeXException lastMiKTeXException;

private:
  MiKTeX::Core::Session * pSession;
};

OBJECT_ENTRY_AUTO(__uuidof(MiKTeXSession), comSession);
