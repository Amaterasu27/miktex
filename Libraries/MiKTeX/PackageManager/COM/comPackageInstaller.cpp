/* PackageInstaller.cpp:

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

#include "StdAfx.h"

#include "internal.h"

#include "COM/comPackageInstaller.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

/* _________________________________________________________________________

   comPackageInstaller::comPackageInstaller
   _________________________________________________________________________ */

comPackageInstaller::comPackageInstaller ()
{
}

/* _________________________________________________________________________

   comPackageInstaller::~comPackageInstaller
   _________________________________________________________________________ */


comPackageInstaller::~comPackageInstaller ()
{
}

/* _________________________________________________________________________

   comPackageInstaller::FinalRelease
   _________________________________________________________________________ */

void
comPackageInstaller::FinalRelease ()
{
}

/* _________________________________________________________________________

   comPackageInstaller::InterfaceSupportsErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::InterfaceSupportsErrorInfo (/*[in]*/ REFIID riid)
{
  static const IID* arr[] = 
    {
      &IID_IPackageInstaller
    };
  
  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++ i)
    {
      if (InlineIsEqualGUID(*arr[i], riid))
	{
	  return (S_OK);
	}
    }
  return (S_FALSE);
}

/* _________________________________________________________________________

   comPackageInstaller::ReportLine
   _________________________________________________________________________ */

void
MPMCALL
comPackageInstaller::ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine)
{
  if (pCallback == 0)
    {
      return;
    }
  HRESULT hr = pCallback->ReportLine(CT2CW(lpszLine));
  if (FAILED(hr))
    {
      // todo
    }
}

/* _________________________________________________________________________

   comPackageInstaller::OnRetryableError
   _________________________________________________________________________ */

bool
MPMCALL
comPackageInstaller::OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage)
{
  if (pCallback == 0)
    {
      return (true);
    }
  BOOL doContinue;
  HRESULT hr = pCallback->OnRetryableError(CT2CW(lpszMessage), &doContinue);
  if (FAILED(hr))
    {
      return (false);
    }
  return (doContinue ? true : false);
}

/* _________________________________________________________________________

   comPackageInstaller::OnProgress
   _________________________________________________________________________ */

bool
MPMCALL
comPackageInstaller::OnProgress (/*[in]*/ Notification	nf)
{
  if (pCallback == 0)
    {
      return (true);
    }
  BOOL doContinue;
  HRESULT hr = pCallback->OnProgress(nf.Get(), &doContinue);
  if (FAILED(hr))
    {
      return (false);
    }
  return (doContinue ? true : false);
}

/* _________________________________________________________________________

   comPackageInstaller::Add
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::Add (/*[in]*/ BSTR packageName,
			  /*[in]*/ BOOL toBeInstalled)
{
  if (toBeInstalled)
    {
      packagesToBeInstalled.push_back (tstring(CW2CT(packageName)));
    }
  else
    {
      packagesToBeRemoved.push_back (tstring(CW2CT(packageName)));
    }
  return (S_OK);
}

/* _________________________________________________________________________

   comPackageInstaller::InstallRemove
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::InstallRemove
(/*[in]*/ IPackageInstallerCallback * pCallback)
{
  HRESULT hr;
  this->pCallback = pCallback;
  try
    {
      if (pInstaller.get() == 0)
	{
	  if (pManager.Get() == 0)
	    {
	      pManager.Create ();
	    }
	  pInstaller = pManager->CreateInstaller();
	}
      pInstaller->SetFileLists (packagesToBeInstalled, packagesToBeRemoved);
      pInstaller->SetCallback (this);
      pInstaller->InstallRemove ();
#if 0
      // todo
      RunIniTeXMF
#endif
      hr = S_OK;
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException(T_("msvsvc"),
			e.what(),
			0,
			T_(__FILE__),
			__LINE__);
      hr = E_FAIL;
    }
  this->pCallback = 0;
  return (hr);
}

/* _________________________________________________________________________

   comPackageInstaller::GetErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::GetErrorInfo (/*[out,retval]*/ ErrorInfo ** pErrorInfo)
{
  if (lastMiKTeXException.what() == 0)
    {
      return (E_FAIL);
    }
  if (*pErrorInfo == 0)
    {
      return (E_INVALIDARG);
    }
  *pErrorInfo =
    reinterpret_cast<ErrorInfo*>(CoTaskMemAlloc(sizeof(**pErrorInfo)));
  if (*pErrorInfo == 0)
    {
      return (E_OUTOFMEMORY);
    }
  try
    {
      _bstr_t message = lastMiKTeXException.what();
      _bstr_t info = lastMiKTeXException.GetInfo().c_str();
      _bstr_t sourceFile = lastMiKTeXException.GetSourceFile().c_str();
      (*pErrorInfo)->message = message.Detach();
      (*pErrorInfo)->info = info.Detach();
      (*pErrorInfo)->sourceFile = sourceFile.Detach();
      (*pErrorInfo)->sourceLine = lastMiKTeXException.GetSourceLine();
      return (S_OK);
    }
  catch (const _com_error & e)
    {
      CoTaskMemFree (*pErrorInfo);
      return (e.Error());
    }
  catch (const exception &)
    {
      CoTaskMemFree (*pErrorInfo);
      return (E_FAIL);
    }
}

/* _________________________________________________________________________

   comPackageInstaller::UpdateDb
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::UpdateDb ()
{
  HRESULT hr;
  try
    {
      if (pInstaller.get() == 0)
	{
	  if (pManager.Get() == 0)
	    {
	      pManager.Create ();
	    }
	  pInstaller = pManager->CreateInstaller();
	}
      pInstaller->UpdateDb ();
      hr = S_OK;
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException(T_("msvsvc"),
			e.what(),
			0,
			T_(__FILE__),
			__LINE__);
      hr = E_FAIL;
    }
  this->pCallback = 0;
  return (hr);
}
