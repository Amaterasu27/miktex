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
winRegistry::TryGetRegistryValue (/*[in]*/ HKEY		hkeyParent,
				  /*[in]*/ const char *	lpszPath,
				  /*[in]*/ const char *	lpszValueName,
				  /*[out]*/ string &	value,
				  /*[in]*/ const char * lpszDefaultValue)
{
  HKEY hkey;

  long result =
    RegOpenKeyExA(hkeyParent,
		  lpszPath,
		  0,
		  KEY_READ,
		  &hkey);
  
  if (result == ERROR_SUCCESS)
    {
      unsigned long valueType;
      unsigned long valueSize = 0;
      unsigned char * pValue = 0;
      result =
	RegQueryValueExA(hkey,
			 lpszValueName,
			 0,
			 &valueType,
			 0,
			 &valueSize);
      if (result == ERROR_SUCCESS)
	{
	  pValue = reinterpret_cast<unsigned char *>(_alloca(valueSize));
	  result =
	    RegQueryValueExA(hkey,
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
			     lpszPath,
			     __FILE__,
			     __LINE__);
	}
      if (result == ERROR_SUCCESS)
	{
	  switch (valueType)
	  {
	  case REG_SZ:
	    value = reinterpret_cast<const char *>(pValue);
	    return (true);
	  case REG_EXPAND_SZ:
	    {
	      DWORD n = ExpandEnvironmentStringsA(reinterpret_cast<const char *>(pValue),
		0,
		0);
	      if (n == 0)
	      {
		FATAL_WINDOWS_ERROR ("ExpandEnvironmentStringsA", lpszPath);
	      }
	      char * lpszBuf = reinterpret_cast<char *>(_alloca(n));
	      DWORD n2 = ExpandEnvironmentStringsA(reinterpret_cast<const char *>(pValue),
		lpszBuf,
		n);
	      if (n2 == 0)
	      {
		FATAL_WINDOWS_ERROR ("ExpandEnvironmentStringsA", lpszPath);
	      }
	      MIKTEX_ASSERT (n2 <= n);
	      value = lpszBuf;
	      return (true);
	    }
	  default:
	    TraceError (T_("ignoring value %s of type %u"),
			Q_(lpszValueName),
			static_cast<unsigned>(valueType));
	    break;
	  }
	}
      else if (result != ERROR_FILE_NOT_FOUND)
	{
	  TraceWindowsError ("RegQueryValueExA",
			     result,
			     lpszValueName,
			     __FILE__,
			     __LINE__);
	}
    }
  else if (result != ERROR_FILE_NOT_FOUND)
    {
      TraceWindowsError ("RegOpenKeyExA",
			 result,
			 lpszPath,
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
   
   winRegistry::TryDeleteRegistryKey
   _________________________________________________________________________ */
  
bool
winRegistry::TryDeleteRegistryKey (/*[in]*/ HKEY	  hkeyParent,
				   /*[in]*/ const char *  lpszPath)
{
  long result = SHDeleteKeyW(hkeyParent, Utils::AnsiToWideChar(lpszPath).c_str());
  if (result != ERROR_SUCCESS)
  {
    if (result == ERROR_FILE_NOT_FOUND)
    {
      return (false);
    }
    FATAL_WINDOWS_ERROR_2 ("SHDeleteKeyW",
      result,
      lpszPath);
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
  HKEY hkey;

  long result =
    RegOpenKeyExA(hkeyParent,
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
      FATAL_WINDOWS_ERROR_2 ("RegOpenKeyExA",
			     result,
			     lpszPath);
    }

  AutoHKEY autoClose (hkey);

  result = RegDeleteValueA(hkey, lpszValueName);
  
  if (result != ERROR_SUCCESS)
    {
      if (result == ERROR_FILE_NOT_FOUND)
	{
	  return (false);
	}
      FATAL_WINDOWS_ERROR_2 ("RegDeleteValueA",
			     result,
			     lpszValueName);
    }

  return (true);
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
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_STRING (lpszValueName);
  MIKTEX_ASSERT_STRING (lpszValue);

  SessionImpl::GetSession()->trace_config->WriteFormattedLine
    ("core",
     "RegCreateKey (%p, \"%s\")",
     reinterpret_cast<void*>(hkeyParent),
     lpszPath);

  HKEY hkey;
  unsigned long disp;

  long result =
    RegCreateKeyExA(hkeyParent,
		    lpszPath,
		    0,
		    "",
		    REG_OPTION_NON_VOLATILE,
		    KEY_ALL_ACCESS,
		    0,
		    &hkey,
		    &disp);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 ("RegCreateKeyExA",
			     result,
			     lpszPath);
    }

  AutoHKEY autoClose (hkey);

  unsigned long valueSize = static_cast<unsigned long>(StrLen(lpszValue) + 1);
  valueSize *= sizeof(char);

  result =
    RegSetValueExA(hkey,
		   lpszValueName,
		   0,
		   REG_SZ,
		   const_cast<unsigned char *>
		   (reinterpret_cast<const unsigned char *>(lpszValue)),
		   valueSize);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 ("RegSetValueExA",
			     result,
			     lpszValueName);
    }
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

  string registryPath =
    (SessionImpl::GetSession()->IsMiKTeXDirect()
     ? MIKTEX_REGPATH_SERIES_MIKTEXDIRECT
     : MIKTEX_REGPATH_SERIES);
  registryPath += '\\';
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
				  /*[out]*/ PathName &	path,
				  /*[in]*/ const char * lpszDefaultPath)
{
  string value;
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
   
   winRegistry::TryDeleteRegistryValue
   _________________________________________________________________________ */
  
bool
winRegistry::TryDeleteRegistryValue (/*[in]*/ TriState		shared,
				     /*[in]*/ const char * lpszKeyName,
				     /*[in]*/ const char * lpszValueName)
{
  if (shared == TriState::Undetermined)
    {
      // <recursivecall>
      bool done =
	TryDeleteRegistryValue (TriState::False, lpszKeyName, lpszValueName);
      return (TryDeleteRegistryValue(TriState::True,
				     lpszKeyName,
				     lpszValueName)
	      || done);
      // </recursivecall>
    }

  string registryPath =
    (SessionImpl::GetSession()->IsMiKTeXDirect()
     ? MIKTEX_REGPATH_SERIES_MIKTEXDIRECT
     : MIKTEX_REGPATH_SERIES);
  registryPath += '\\';
  registryPath += lpszKeyName;

  return (TryDeleteRegistryValue((shared == TriState::False
				  ? HKEY_CURRENT_USER
				  : HKEY_LOCAL_MACHINE),
				 registryPath.c_str(),
				 lpszValueName));
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
 
  string registryPath =
    (SessionImpl::GetSession()->IsMiKTeXDirect()
     ? MIKTEX_REGPATH_SERIES_MIKTEXDIRECT
     : MIKTEX_REGPATH_SERIES);
  registryPath += '\\';
  registryPath += lpszKeyName;

  HKEY hkeyParent =
    (shared == TriState::False
     ? HKEY_CURRENT_USER
     : HKEY_LOCAL_MACHINE);

  SetRegistryValue (hkeyParent, registryPath.c_str(), lpszValueName, lpszValue);

  string value;

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
