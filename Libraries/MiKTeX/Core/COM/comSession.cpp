/* comSession.cpp: MiKTeX session

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

#include "StdAfx.h"

#include "internal.h"

#include "COM/comSession.h"
#include "COM/session.h"

using namespace MiKTeX::Core;
using namespace std;

/* _________________________________________________________________________

   comSession::comSession
   _________________________________________________________________________ */

comSession::comSession ()
  : pSession (0)
{
}

/* _________________________________________________________________________

   comSession::~comSession
   _________________________________________________________________________ */

comSession::~comSession ()
{
  try
    {
      if (pSession != 0)
	{
	  Session::Release (pSession);
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   comSession::FinalRelease
   _________________________________________________________________________ */

void
comSession::FinalRelease ()
{
  try
    {
      if (pSession != 0)
	{
	  Session::Release (pSession);
	}
    }
  catch (const exception &)
    {
    }
  pSession = 0;
}

/* _________________________________________________________________________

   comSession::InterfaceSupportsErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comSession::InterfaceSupportsErrorInfo (/*[in]*/ REFIID riid)
{
  static const IID* arr[] = 
    {
      &IID_ISession
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

   comSession::RegisterRootDirectories
   _________________________________________________________________________ */

STDMETHODIMP
comSession::RegisterRootDirectories (/*[in]*/ BSTR	rootDirectories)
{
  SessionImpl::runningAsLocalServer = true;
  HRESULT hr = S_OK;
  try
    {
      CreateSession ();
      pSession->RegisterRootDirectories(tstring(CW2CT(rootDirectories)));
    }
  catch (const _com_error & e)
    {
      hr = e.Error();
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException(T_("sessionsvc"),
			e.what(),
			0,
			T_(__FILE__),
			__LINE__);
      hr = E_FAIL;
    }
  return (hr);
}

/* _________________________________________________________________________

   comSession::FindPkFile
   _________________________________________________________________________ */

STDMETHODIMP
comSession::FindPkFile (/*[in]*/ BSTR		fontName,
			/*[in]*/ BSTR		mode,
			/*[in]*/ LONG		dpi,
			/*[out]*/ BSTR *	path,
			/*[out,retval]*/ VARIANT_BOOL * found)
{
  SessionImpl::runningAsLocalServer = true;
  HRESULT hr = S_OK;
  try
    {
      CreateSession ();
      PathName path_;
      if (pSession->FindPkFile(CW2CT(fontName),
			       CW2CT(mode),
			       dpi,
			       path_))
	{
	  *path = _bstr_t(path_.Get()).Detach();
	  *found = VARIANT_TRUE;
	}
      else
	{
	  *path = _bstr_t(L"").Detach();
	  *found = VARIANT_FALSE;
	}
    }
  catch (const _com_error & e)
    {
      hr = e.Error();
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException(T_("sessionsvc"),
			e.what(),
			0,
			T_(__FILE__),
			__LINE__);
      hr = E_FAIL;
    }
  return (hr);
}

/* _________________________________________________________________________

   comSession::GetErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comSession::GetErrorInfo (/*[out,retval]*/ ErrorInfo * pErrorInfo)
{
  if (lastMiKTeXException.what() == 0)
    {
      return (E_FAIL);
    }
  try
    {
      _bstr_t message = lastMiKTeXException.what();
      _bstr_t info = lastMiKTeXException.GetInfo().c_str();
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

   comSession::GetMiKTeXSetupInfo
   _________________________________________________________________________ */

STDMETHODIMP
comSession::GetMiKTeXSetupInfo (/*[out,retval]*/ MiKTeXSetupInfo * setupInfo)
{
  SessionImpl::runningAsLocalServer = true;
  HRESULT hr = S_OK;
  try
    {
      CreateSession ();
      _bstr_t version = Utils::GetMiKTeXVersionString().c_str();
      _bstr_t binDirectory =
	pSession->GetSpecialPath(SpecialPath::BinDirectory).Get();
      _bstr_t installRoot =
	pSession->GetSpecialPath(SpecialPath::InstallRoot).Get();
      _bstr_t commonConfigRoot;
      _bstr_t commonDataRoot;
      if (pSession->IsSharedMiKTeXSetup() == TriState::True)
	{
	  commonConfigRoot =
	    pSession->GetSpecialPath(SpecialPath::CommonConfigRoot).Get();
	  commonDataRoot =
	    pSession->GetSpecialPath(SpecialPath::CommonDataRoot).Get();
	}
      else
	{
	  commonConfigRoot = L"";
	  commonDataRoot = L"";
	}
      _bstr_t userConfigRoot =
	pSession->GetSpecialPath(SpecialPath::UserConfigRoot).Get();
      _bstr_t userDataRoot =
	pSession->GetSpecialPath(SpecialPath::UserDataRoot).Get();
      setupInfo->sharedSetup =
	(pSession->IsSharedMiKTeXSetup() == TriState::True);
      setupInfo->series = MIKTEX_SERIES_INT;
      setupInfo->numRoots = pSession->GetNumberOfTEXMFRoots();
      setupInfo->version = version.Detach();
      setupInfo->binDirectory = binDirectory.Detach();
      setupInfo->installRoot = installRoot.Detach();
      setupInfo->commonConfigRoot = commonConfigRoot.Detach();
      setupInfo->commonDataRoot = commonDataRoot.Detach();
      setupInfo->userConfigRoot = userConfigRoot.Detach();
      setupInfo->userDataRoot = userDataRoot.Detach();
      return (S_OK);

    }
  catch (const _com_error & e)
    {
      hr = e.Error();
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException(T_("sessionsvc"),
			e.what(),
			0,
			T_(__FILE__),
			__LINE__);
      hr = E_FAIL;
    }
  return (hr);
}

/* _________________________________________________________________________

   comSession::GetRootDirectory
   _________________________________________________________________________ */

STDMETHODIMP
comSession::GetRootDirectory (/*[in]*/ LONG		rootIdx,
			      /*[out,retval]*/ BSTR *	rootDirectory)
{
  SessionImpl::runningAsLocalServer = true;
  HRESULT hr = S_OK;
  try
    {
      CreateSession ();
      *rootDirectory =
	_bstr_t(pSession->GetRootDirectory(rootIdx).Get()).Detach();
      return (S_OK);
    }
  catch (const _com_error & e)
    {
      hr = e.Error();
    }
  catch (const MiKTeXException & e)
    {
      lastMiKTeXException = e;
      hr = E_FAIL;
    }
  catch (const exception & e)
    {
      lastMiKTeXException =
	MiKTeXException(T_("sessionsvc"),
			e.what(),
			0,
			T_(__FILE__),
			__LINE__);
      hr = E_FAIL;
    }
  return (hr);
}

/* _________________________________________________________________________

   comSession::CreateSession
   _________________________________________________________________________ */

void
comSession::CreateSession ()
{
  if (pSession == 0)
    {
      pSession = Session::TryGet();
      if (pSession == 0)
	{
	  // we are running standalone; create a new session
	  pSession = Session::Get(Session::InitInfo(SESSIONSVC));
	}
    }
}

/* _________________________________________________________________________

   GetAccessPermissionsForLUAServer

   See MSDN "The COM Elevation Moniker"
   _________________________________________________________________________ */

static
ULONG
GetAccessPermissionsForLUAServer (/*[out]*/ SECURITY_DESCRIPTOR **	ppSD)
{
  DllProc4<BOOL, LPCWSTR, DWORD, PSECURITY_DESCRIPTOR, PULONG>
    pConvert (T_("Advapi32.dll"),
	      T_("ConvertStringSecurityDescriptorToSecurityDescriptorW"));
  LPWSTR lpszSDDL = L"O:BAG:BAD:(A;;0x3;;;IU)(A;;0x3;;;SY)";
  SECURITY_DESCRIPTOR * pSD = 0;
  ULONG size = 0;
  if (! pConvert(lpszSDDL,
		 SDDL_REVISION_1,
		 reinterpret_cast<PSECURITY_DESCRIPTOR *>(&pSD),
		 &size))
    {
      FATAL_WINDOWS_ERROR
	(T_("ConvertStringSecurityDescriptorToSecurityDescriptorW"),
	 0);
    }
  *ppSD = pSD;
  return (size);
}

/* _________________________________________________________________________

   comSession::UpdateRegistry
   _________________________________________________________________________ */

HRESULT
WINAPI
comSession::UpdateRegistry (/*[in]*/ BOOL doRegister)
{
  HRESULT hr;
  try
    {
      vector<_ATL_REGMAP_ENTRY> regMapEntries;
      _ATL_REGMAP_ENTRY rme;
      tstring str;
      if (IsWindowsNT())
	{
	  SECURITY_DESCRIPTOR * pSd;
	  ULONG sizeSd = GetAccessPermissionsForLUAServer(&pSd);
	  AutoLocalMem toBeFreed (pSd);
	  str = Utils::Hexify(pSd, sizeSd, true);
	}
      else
	{
	  str = T_("00");
	}
      rme.szKey = L"ACCESS_SD";
      CT2W wstr (str.c_str());
      rme.szData = wstr;
      regMapEntries.push_back (rme);
      rme.szKey = 0;
      rme.szData = 0;
      regMapEntries.push_back (rme);
#if defined(_AFX_DLL)
      hr =
	_AtlModule.UpdateRegistryFromResourceD(IDR_MIKTEXSESSIONOBJECT,
					       doRegister,
					       &regMapEntries[0]);
#else
      hr =
	_AtlModule.UpdateRegistryFromResourceS(IDR_MIKTEXSESSIONOBJECT,
					       doRegister,
					       &regMapEntries[0]);
#endif
      if (FAILED(hr))
	{
	  //
	}
    }
  catch (const exception &)
    {
      hr = E_FAIL;
    }
  return (hr);
}
