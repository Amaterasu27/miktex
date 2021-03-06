/* comPackageInstaller.cpp:

   Copyright (C) 2001-2011 Christian Schenk

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
using namespace MiKTeXPackageManagerLib;
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
  try
    {
      if (trace_mpm.get() != 0)
	{
	  trace_mpm->Close ();
	  trace_mpm.reset ();
	}
      if (trace_error.get() != 0)
	{
	  trace_error->Close ();
	  trace_error.reset ();
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   comPackageInstaller::FinalRelease
   _________________________________________________________________________ */

void
comPackageInstaller::FinalRelease ()
{
}

/* _________________________________________________________________________

   comPackageInstaller::Initialize
   _________________________________________________________________________ */

void
comPackageInstaller::Initialize ()
{
  if (trace_error.get() == 0)
    {
      trace_error.reset (TraceStream::Open(MIKTEX_TRACE_ERROR));
    }
  if (trace_mpm.get() == 0)
    {
      trace_mpm.reset (TraceStream::Open(MIKTEX_TRACE_MPM));
    }
}

/* _________________________________________________________________________

   comPackageInstaller::InterfaceSupportsErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::InterfaceSupportsErrorInfo (/*[in]*/ REFIID riid)
{
  static const IID* arr[] = 
    {
      &__uuidof(IPackageInstaller)
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
comPackageInstaller::ReportLine (/*[in]*/ const char * lpszLine)
{
  if (pCallback == 0)
    {
      return;
    }
  HRESULT hr = pCallback->ReportLine(_bstr_t(Utils::UTF8ToWideChar(lpszLine).c_str()));
  if (FAILED(hr))
    {
      // what todo?
    }
}

/* _________________________________________________________________________

   comPackageInstaller::OnRetryableError
   _________________________________________________________________________ */

bool
comPackageInstaller::OnRetryableError (/*[in]*/ const char * lpszMessage)
{
  if (pCallback == 0)
    {
      return (false);
    }
  VARIANT_BOOL doContinue;
  HRESULT hr = pCallback->OnRetryableError(_bstr_t(Utils::UTF8ToWideChar(lpszMessage).c_str()), &doContinue);
  if (FAILED(hr))
    {
      doContinue = VARIANT_FALSE;
    }
  return (doContinue ? true : false);
}

/* _________________________________________________________________________

   comPackageInstaller::OnProgress
   _________________________________________________________________________ */

bool
comPackageInstaller::OnProgress (/*[in]*/ Notification	nf)
{
  if (pCallback == 0)
    {
      return (true);
    }
  VARIANT_BOOL doContinue;
  HRESULT hr = pCallback->OnProgress(nf.Get(), &doContinue);
  if (FAILED(hr))
    {
      doContinue = VARIANT_FALSE;
    }
  return (doContinue ? true : false);
}

/* _________________________________________________________________________

   comPackageInstaller::Add
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::Add (/*[in]*/ BSTR		packageName,
			  /*[in]*/ VARIANT_BOOL	toBeInstalled)
{
  HRESULT hr = S_OK;
  try
    {
      if (toBeInstalled)
	{
	  packagesToBeInstalled.push_back (Utils::WideCharToUTF8(packageName));
	  trace_mpm->WriteFormattedLine ("mpmsvc",
					 T_("to be installed: %s"),
					 packagesToBeInstalled.back().c_str());
	}
      else
	{
	  packagesToBeRemoved.push_back (string(CW2A(packageName)));
	  trace_mpm->WriteFormattedLine ("mpmsvc",
					 T_("to be removed: %s"),
					 packagesToBeRemoved.back().c_str());
	}
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException("mpmsvc",
			e.what(),
			0,
			__FILE__,
			__LINE__);
      hr = E_FAIL;
    }
  return (hr);
}

/* _________________________________________________________________________

   comPackageInstaller::SetCallback
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::SetCallback (/*[in]*/ IUnknown * pCallback)
{
  this->pCallback = pCallback;
  return (S_OK);
}

/* _________________________________________________________________________

   comPackageInstaller::InstallRemove
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::InstallRemove ()
{
  HRESULT hr = S_OK;
  try
    {
      trace_mpm->WriteLine ("mpmsvc", T_("install/remove"));
      if (pInstaller.get() == 0)
	{
	  if (pManager.Get() == 0)
	    {
	      pManager.Create ();
	    }
	  pInstaller.reset (pManager->CreateInstaller());
	}
      pInstaller->SetCallback (this);
      pInstaller->SetFileLists (packagesToBeInstalled, packagesToBeRemoved);
      pInstaller->InstallRemove ();
      packagesToBeInstalled.clear ();
      packagesToBeRemoved.clear ();
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException("mpmsvc",
			e.what(),
			0,
			__FILE__,
			__LINE__);
      hr = E_FAIL;
    }
  return (hr);
}

/* _________________________________________________________________________

   comPackageInstaller::GetErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::GetErrorInfo (/*[out,retval]*/ ErrorInfo * pErrorInfo)
{
  if (lastMiKTeXException.what() == 0)
    {
      return (E_FAIL);
    }
  try
    {
      _bstr_t message = Utils::UTF8ToWideChar(lastMiKTeXException.what()).c_str();
      _bstr_t info = Utils::UTF8ToWideChar(lastMiKTeXException.GetInfo().c_str()).c_str();
      _bstr_t sourceFile = lastMiKTeXException.GetSourceFile().c_str();
      pErrorInfo->message = message.Detach();
      pErrorInfo->info = info.Detach();
      pErrorInfo->sourceFile = sourceFile.Detach();
      pErrorInfo->sourceLine = lastMiKTeXException.GetSourceLine();
      return (S_OK);
    }
  catch (const _com_error & e)
    {
      return (e.Error());
    }
  catch (const exception &)
    {
      return (E_FAIL);
    }
}

/* _________________________________________________________________________

   comPackageInstaller::UpdateDb
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::UpdateDb ()
{
  HRESULT hr = S_OK;
  try
    {
      trace_mpm->WriteLine ("mpmsvc", T_("update db"));
      if (pInstaller.get() == 0)
	{
	  if (pManager.Get() == 0)
	    {
	      pManager.Create ();
	    }
	  pInstaller.reset (pManager->CreateInstaller());
	}
      pInstaller->UpdateDb ();
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException("mpmsvc",
			e.what(),
			0,
			__FILE__,
			__LINE__);
      hr = E_FAIL;
    }
  return (hr);
}

/* _________________________________________________________________________

   comPackageInstaller::SetRepository
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInstaller::SetRepository (/*[in]*/ BSTR repository)
{
  HRESULT hr = S_OK;
  try
    {
      trace_mpm->WriteLine ("mpmsvc", T_("set repository"));
      if (pInstaller.get() == 0)
	{
	  if (pManager.Get() == 0)
	    {
	      pManager.Create ();
	    }
	  pInstaller.reset (pManager->CreateInstaller());
	}
      pInstaller->SetRepository (Utils::WideCharToUTF8(repository));
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException("mpmsvc",
			e.what(),
			0,
			__FILE__,
			__LINE__);
      hr = E_FAIL;
    }
  return (hr);
}
