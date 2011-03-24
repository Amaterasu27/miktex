/* winRegistry.cpp:

   Copyright (C) 1996-2011 Christian Schenk

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

#include "win/winRegistry.h"
#include "miktex/Core/Registry"

/* _________________________________________________________________________

   winRegistry::TryGetRegistryValue
   _________________________________________________________________________ */

bool
winRegistry::TryGetRegistryValue (/*[in]*/ HKEY			hkeyParent,
				  /*[in]*/ const wchar_t *	lpszPath,
				  /*[in]*/ const wchar_t *	lpszValueName,
				  /*[out]*/ wstring &		value,
				  /*[in]*/ const wchar_t *	lpszDefaultValue)
{
  HKEY hkey;

  long result =
    RegOpenKeyExW(hkeyParent,
		  lpszPath,
		  0,
		  KEY_READ,
		  &hkey);
  
  if (result == ERROR_SUCCESS)
    {
      DWORD valueType;
      DWORD valueSize = 0;
      LPBYTE pValue = 0;
      result =
	RegQueryValueExW(hkey,
			 lpszValueName,
			 0,
			 &valueType,
			 0,
			 &valueSize);
      if (result == ERROR_SUCCESS)
	{
	  pValue = reinterpret_cast<LPBYTE>(_alloca(valueSize));
	  result =
	    RegQueryValueExW(hkey,
			     lpszValueName,
			     0,
			     &valueType,
			     pValue,
			     &valueSize);
	}
      long result2 = RegCloseKey(hkey);
      if (result2 != ERROR_SUCCESS)
	{
	  TraceWindowsError ("RegCloseKey",
			     result2,
			     WA_(lpszPath),
			     __FILE__,
			     __LINE__);
	}
      if (result == ERROR_SUCCESS)
	{
	  switch (valueType)
	  {
	  case REG_SZ:
	    value = reinterpret_cast<const wchar_t *>(pValue);
	    return (true);
	  case REG_EXPAND_SZ:
	    {
	      DWORD n = ExpandEnvironmentStringsW(reinterpret_cast<const wchar_t *>(pValue),
		0,
		0);
	      if (n == 0)
	      {
		FATAL_WINDOWS_ERROR ("ExpandEnvironmentStringsW", WA_(lpszPath));
	      }
	      wchar_t * lpszBuf = reinterpret_cast<wchar_t *>(_alloca(sizeof(wchar_t) * n));
	      DWORD n2 = ExpandEnvironmentStringsW(reinterpret_cast<const wchar_t *>(pValue),
		lpszBuf,
		n);
	      if (n2 == 0)
	      {
		FATAL_WINDOWS_ERROR ("ExpandEnvironmentStringsW", WA_(lpszPath));
	      }
	      MIKTEX_ASSERT (n2 <= n);
	      value = lpszBuf;
	      return (true);
	    }
	  default:
	    TraceError (T_("ignoring value %s of type %u"),
			Q_(WA_(lpszValueName)),
			static_cast<unsigned>(valueType));
	    break;
	  }
	}
      else if (result != ERROR_FILE_NOT_FOUND)
	{
	  TraceWindowsError ("RegQueryValueExW",
			     result,
			     WA_(lpszValueName),
			     __FILE__,
			     __LINE__);
	}
    }
  else if (result != ERROR_FILE_NOT_FOUND)
    {
      TraceWindowsError ("RegOpenKeyExW",
			 result,
			 WA_(lpszPath),
			 __FILE__,
			 __LINE__);
    }

  if (lpszDefaultValue != 0)
    {
      value = lpszDefaultValue;
      return (true);
    }
 
  return (false);
}

/* _________________________________________________________________________

   winRegistry::TryGetRegistryValue
   _________________________________________________________________________ */

bool
winRegistry::TryGetRegistryValue (/*[in]*/ HKEY		hkeyParent,
				  /*[in]*/ const char *	lpszPath,
				  /*[in]*/ const char *	lpszValueName,
				  /*[out]*/ string &	value,
				  /*[in]*/ const char * lpszDefaultValue)
{
  wstring wvalue;
  bool result = TryGetRegistryValue(
    hkeyParent,
    AW_(lpszPath),
    AW_(lpszValueName),
    wvalue,
    lpszDefaultValue == 0 ? 0 : AW_(lpszDefaultValue));
  if (result)
  {
    value = WA_(wvalue.c_str());
  }
  return (result);
}

/* _________________________________________________________________________
   
   winRegistry::TryDeleteRegistryKey
   _________________________________________________________________________ */
  
bool
winRegistry::TryDeleteRegistryKey (/*[in]*/ HKEY	      hkeyParent,
				   /*[in]*/ const wchar_t *   lpszPath)
{
  long result = SHDeleteKeyW(hkeyParent, lpszPath);
  if (result != ERROR_SUCCESS)
  {
    if (result == ERROR_FILE_NOT_FOUND)
    {
      return (false);
    }
    FATAL_WINDOWS_ERROR_2 ("SHDeleteKeyW",
      result,
      WA_(lpszPath));
  }
  return (true);
}

/* _________________________________________________________________________
   
   winRegistry::TryDeleteRegistryKey
   _________________________________________________________________________ */
  
bool
winRegistry::TryDeleteRegistryKey (/*[in]*/ HKEY	  hkeyParent,
				   /*[in]*/ const char *  lpszPath)
{
  return (TryDeleteRegistryKey(hkeyParent, AW_(lpszPath)));
}

/* _________________________________________________________________________
   
   winRegistry::TryDeleteRegistryValue
   _________________________________________________________________________ */
  
bool
winRegistry::TryDeleteRegistryValue (/*[in]*/ HKEY	      hkeyParent,
				     /*[in]*/ const wchar_t * lpszPath,
				     /*[in]*/ const wchar_t * lpszValueName)
{
  HKEY hkey;

  long result =
    RegOpenKeyExW(hkeyParent,
		  lpszPath,
		  0,
		  KEY_ALL_ACCESS,
		  &hkey);

  if (result != ERROR_SUCCESS)
    {
      if (result == ERROR_FILE_NOT_FOUND)
	{
	  return (false);
	}
      FATAL_WINDOWS_ERROR_2 ("RegOpenKeyExW",
			     result,
			     WA_(lpszPath));
    }

  AutoHKEY autoClose (hkey);

  result = RegDeleteValueW(hkey, lpszValueName);
  
  if (result != ERROR_SUCCESS)
    {
      if (result == ERROR_FILE_NOT_FOUND)
	{
	  return (false);
	}
      FATAL_WINDOWS_ERROR_2 ("RegDeleteValueW",
			     result,
			     WA_(lpszValueName));
    }

  return (true);
}

/* _________________________________________________________________________
   
   winRegistry::TryDeleteRegistryValue
   _________________________________________________________________________ */
  
bool
winRegistry::TryDeleteRegistryValue (/*[in]*/ HKEY	   hkeyParent,
				     /*[in]*/ const char * lpszPath,
				     /*[in]*/ const char * lpszValueName)
{
  return (TryDeleteRegistryValue(hkeyParent, AW_(lpszPath), AW_(lpszValueName)));
}

/* _________________________________________________________________________
   
   SetRegistryValue
   _________________________________________________________________________ */

void
winRegistry::SetRegistryValue (/*[in]*/ HKEY		hkeyParent,
			       /*[in]*/ const wchar_t *	lpszPath,
			       /*[in]*/ const wchar_t *	lpszValueName,
			       /*[in]*/ const wchar_t *	lpszValue)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_STRING (lpszValueName);
  MIKTEX_ASSERT_STRING (lpszValue);

  SessionImpl::GetSession()->trace_config->WriteFormattedLine
    ("core",
     "RegCreateKeyExW (%p, \"%s\")",
     reinterpret_cast<void*>(hkeyParent),
     WA_(lpszPath));

  HKEY hkey;
  DWORD disp;

  long result =
    RegCreateKeyExW(hkeyParent,
		    lpszPath,
		    0,
		    L"",
		    REG_OPTION_NON_VOLATILE,
		    KEY_ALL_ACCESS,
		    0,
		    &hkey,
		    &disp);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 ("RegCreateKeyExW",
			     result,
			     WA_(lpszPath));
    }

  AutoHKEY autoClose (hkey);

  DWORD valueSize = static_cast<DWORD>(StrLen(lpszValue) + 1);
  valueSize *= sizeof(*lpszValue);

  result =
    RegSetValueExW(hkey,
		   lpszValueName,
		   0,
		   REG_SZ,
		   const_cast<LPBYTE>(
		   reinterpret_cast<const BYTE *>(lpszValue)),
		   valueSize);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 ("RegSetValueExW",
			     result,
			     WA_(lpszValueName));
    }
}

/* _________________________________________________________________________
   
   SetRegistryValue
   _________________________________________________________________________ */

void
winRegistry::SetRegistryValue (/*[in]*/ HKEY		hkeyParent,
			       /*[in]*/ const char *	lpszPath,
			       /*[in]*/ const char *	lpszValueName,
			       /*[in]*/ const char *	lpszValue)
{
  SetRegistryValue (hkeyParent, AW_(lpszPath), AW_(lpszValueName), AW_(lpszValue));
}

/* _________________________________________________________________________

   winRegistry::TryGetRegistryValue

   Get a value from the Windows Registry.
   _________________________________________________________________________ */

bool
winRegistry::TryGetRegistryValue (/*[in]*/ TriState		shared,
				  /*[in]*/ const wchar_t *	lpszKeyName,
				  /*[in]*/ const wchar_t *	lpszValueName,
				  /*[out]*/ wstring &		value,
				  /*[in]*/ const wchar_t *	lpszDefaultValue)
{
  if (shared == TriState::Undetermined)
    {
#if 1
      if (TryGetRegistryValue(TriState::False,
			      lpszKeyName,
			      lpszValueName,
			      value,
			      lpszDefaultValue))
	{
	  return (true);
	}
      return (TryGetRegistryValue(TriState::True,
				  lpszKeyName,
				  lpszValueName,
				  value,
				  lpszDefaultValue));
#else
      shared = SessionImpl::GetSession()->IsSharedMiKTeXSetup();
      if (shared == TriState::Undetermined)
	{
	  shared = TriState::False;
	}
      // <recursivecall>
      return (TryGetRegistryValue(shared,
				  lpszKeyName,
				  lpszValueName,
				  value,
				  lpszDefaultValue));
      // </recursivecall>
#endif
    }

  wstring registryPath =
    (SessionImpl::GetSession()->IsMiKTeXDirect()
     ? AW_(MIKTEX_REGPATH_SERIES_MIKTEXDIRECT)
     : AW_(MIKTEX_REGPATH_SERIES));
  registryPath += L'\\';
  registryPath += lpszKeyName;

  return (TryGetRegistryValue((shared == TriState::False
			       ? HKEY_CURRENT_USER
			       : HKEY_LOCAL_MACHINE),
			      registryPath.c_str(),
			      lpszValueName,
			      value,
			      lpszDefaultValue));
}

/* _________________________________________________________________________

   winRegistry::TryGetRegistryValue

   Get a value from the Windows Registry.
   _________________________________________________________________________ */

bool
winRegistry::TryGetRegistryValue (/*[in]*/ TriState	shared,
				  /*[in]*/ const char *	lpszKeyName,
				  /*[in]*/ const char *	lpszValueName,
				  /*[out]*/ string &	value,
				  /*[in]*/ const char * lpszDefaultValue)
{
  wstring wvalue;
  bool result = TryGetRegistryValue(
    shared,
    AW_(lpszKeyName),
    AW_(lpszValueName),
    wvalue,
    lpszDefaultValue == 0 ? 0 : AW_(lpszDefaultValue));
  if (result)
  {
    value = WA_(wvalue.c_str());
  }
  return (result);
}

/* _________________________________________________________________________

   winRegistry::TryGetRegistryValue

   Get a value from the Windows Registry.
   _________________________________________________________________________ */

bool
winRegistry::TryGetRegistryValue (/*[in]*/ TriState		shared,
				  /*[in]*/ const wchar_t *	lpszKeyName,
				  /*[in]*/ const wchar_t *	lpszValueName,
				  /*[out]*/ PathName &		path,
				  /*[in]*/ const wchar_t *	lpszDefaultPath)
{
  wstring value;
  if (! TryGetRegistryValue(shared,
			 lpszKeyName,
			 lpszValueName,
			 value,
			 lpszDefaultPath))
    {
      return (false);
    }
  path = value;
  return (true);
}

/* _________________________________________________________________________

   winRegistry::TryGetRegistryValue

   Get a value from the Windows Registry.
   _________________________________________________________________________ */

bool
winRegistry::TryGetRegistryValue (/*[in]*/ TriState	shared,
				  /*[in]*/ const char *	lpszKeyName,
				  /*[in]*/ const char *	lpszValueName,
				  /*[out]*/ PathName &	path,
				  /*[in]*/ const char * lpszDefaultPath)
{
  return (TryGetRegistryValue(
    shared,
    AW_(lpszKeyName),
    AW_(lpszValueName),
    path,
    AW_(lpszDefaultPath)));
}

/* _________________________________________________________________________
   
   winRegistry::TryDeleteRegistryValue
   _________________________________________________________________________ */
  
bool
winRegistry::TryDeleteRegistryValue (/*[in]*/ TriState		shared,
				     /*[in]*/ const wchar_t *	lpszKeyName,
				     /*[in]*/ const wchar_t *	lpszValueName)
{
  if (shared == TriState::Undetermined)
    {
      // <recursivecall>
      bool done =
	TryDeleteRegistryValue(TriState::False, lpszKeyName, lpszValueName);
      return (TryDeleteRegistryValue(TriState::True,
				     lpszKeyName,
				     lpszValueName)
	      || done);
      // </recursivecall>
    }

  wstring registryPath =
    (SessionImpl::GetSession()->IsMiKTeXDirect()
     ? AW_(MIKTEX_REGPATH_SERIES_MIKTEXDIRECT)
     : AW_(MIKTEX_REGPATH_SERIES));
  registryPath += L'\\';
  registryPath += lpszKeyName;

  return (TryDeleteRegistryValue((shared == TriState::False
				  ? HKEY_CURRENT_USER
				  : HKEY_LOCAL_MACHINE),
				 registryPath.c_str(),
				 lpszValueName));
}

/* _________________________________________________________________________
   
   winRegistry::TryDeleteRegistryValue
   _________________________________________________________________________ */
  
bool
winRegistry::TryDeleteRegistryValue (/*[in]*/ TriState	    shared,
				     /*[in]*/ const char *  lpszKeyName,
				     /*[in]*/ const char *  lpszValueName)
{
  return (TryDeleteRegistryValue(shared, AW_(lpszKeyName), AW_(lpszValueName)));
}

/* _________________________________________________________________________
   
   SetRegistryValue
   _________________________________________________________________________ */

void
winRegistry::SetRegistryValue (/*[in]*/ TriState	shared,
			       /*[in]*/ const wchar_t *	lpszKeyName,
			       /*[in]*/ const wchar_t *	lpszValueName,
			       /*[in]*/ const wchar_t *	lpszValue)
{
  MIKTEX_ASSERT_STRING (lpszKeyName);
  MIKTEX_ASSERT_STRING (lpszValueName);
  MIKTEX_ASSERT_STRING (lpszValue);

  if (shared == TriState::Undetermined)
    {
      shared = (SessionImpl::GetSession()->IsAdminMode()
	? TriState::True
	: TriState::False);
      // <recursivecall>
      SetRegistryValue (shared, lpszKeyName, lpszValueName, lpszValue);
      // </recursivecall>
      return;
    }
 
  wstring registryPath =
    (SessionImpl::GetSession()->IsMiKTeXDirect()
     ? AW_(MIKTEX_REGPATH_SERIES_MIKTEXDIRECT)
     : AW_(MIKTEX_REGPATH_SERIES));
  registryPath += L'\\';
  registryPath += lpszKeyName;

  HKEY hkeyParent =
    (shared == TriState::False
     ? HKEY_CURRENT_USER
     : HKEY_LOCAL_MACHINE);

  SetRegistryValue (hkeyParent, registryPath.c_str(), lpszValueName, lpszValue);

  wstring value;

  if (hkeyParent == HKEY_LOCAL_MACHINE
      && TryGetRegistryValue(TriState::False,
			     lpszKeyName,
			     lpszValueName,
			     value,
			     0))
    {
      TryDeleteRegistryValue (TriState::False, lpszKeyName, lpszValueName);
    }
}

/* _________________________________________________________________________
   
   SetRegistryValue
   _________________________________________________________________________ */

void
winRegistry::SetRegistryValue (/*[in]*/ TriState	shared,
			       /*[in]*/ const char *	lpszKeyName,
			       /*[in]*/ const char *	lpszValueName,
			       /*[in]*/ const char *	lpszValue)
{
  SetRegistryValue(
    shared,
    AW_(lpszKeyName),
    AW_(lpszValueName),
    AW_(lpszValue));
}
