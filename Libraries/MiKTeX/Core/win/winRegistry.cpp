/* winRegistry.cpp:

   Copyright (C) 1996-2006 Christian Schenk

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
#include "miktex/reg.h"

/* _________________________________________________________________________

   winRegistry::TryGetRegistryValue

   Get a value from the Windows Registry.
   _________________________________________________________________________ */

bool
winRegistry::TryGetRegistryValue (/*[in]*/ TriState		shared,
				  /*[in]*/ const MIKTEXCHAR *	lpszKeyName,
				  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
				  /*[out]*/ tstring &		value,
				  /*[in]*/ const MIKTEXCHAR * lpszDefaultValue)
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

  tstring registryPath = MIKTEX_REGPATH_SERIES;
  registryPath += T_('\\');
  registryPath += lpszKeyName;

  HKEY hkey;

  long result =
    RegOpenKeyEx((shared == TriState::False
		  ? HKEY_CURRENT_USER
		  : HKEY_LOCAL_MACHINE),
		 registryPath.c_str(),
		 0,
		 KEY_READ,
		 &hkey);

  if (result == ERROR_SUCCESS)
    {
      unsigned long valueType;
      unsigned long valueSize = 0;
      unsigned char * pValue = 0;
      result =
	RegQueryValueEx(hkey,
			lpszValueName,
			0,
			&valueType,
			0,
			&valueSize);
      if (result == ERROR_SUCCESS)
	{
	  pValue = reinterpret_cast<unsigned char *>(_alloca(valueSize));
	  result =
	    RegQueryValueEx(hkey,
			    lpszValueName,
			    0,
			    &valueType,
			    pValue,
			    &valueSize);
	}
      long result2 = RegCloseKey(hkey);
      if (result2 != ERROR_SUCCESS)
	{
	  TraceWindowsError (T_("RegCloseKey"),
			     result2,
			     registryPath.c_str(),
			     T_(__FILE__),
			     __LINE__);
	}
      if (result == ERROR_SUCCESS)
	{
	  if (valueType == REG_SZ)
	    {
	      value = reinterpret_cast<const MIKTEXCHAR *>(pValue);
	      return (true);
	    }
	  TraceError (T_("ignoring value \"%s\" of type %u"),
		      lpszValueName,
		      static_cast<unsigned>(valueType));
	}
      else if (result != ERROR_FILE_NOT_FOUND)
	{
	  TraceWindowsError (T_("RegQueryValueEx"),
			     result,
			     lpszValueName,
			     T_(__FILE__),
			     __LINE__);
	}
    }
  else if (result != ERROR_FILE_NOT_FOUND)
    {
      TraceWindowsError (T_("RegOpenKeyEx"),
			 result,
			 registryPath.c_str(),
			 T_(__FILE__),
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

   Get a value from the Windows Registry.
   _________________________________________________________________________ */

bool
winRegistry::TryGetRegistryValue (/*[in]*/ TriState		shared,
				  /*[in]*/ const MIKTEXCHAR *	lpszKeyName,
				  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
				  /*[out]*/ PathName &		path,
				  /*[in]*/ const MIKTEXCHAR * lpszDefaultPath)
{
  tstring value;
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
				     /*[in]*/ const MIKTEXCHAR * lpszKeyName,
				     /*[in]*/ const MIKTEXCHAR * lpszValueName)
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

  tstring registryPath = MIKTEX_REGPATH_SERIES;
  registryPath += T_('\\');
  registryPath += lpszKeyName;

  HKEY hkey;

  long result =
    RegOpenKeyEx((shared == TriState::False
		  ? HKEY_CURRENT_USER
		  : HKEY_LOCAL_MACHINE),
		 registryPath.c_str(),
		 0,
		 KEY_ALL_ACCESS,
		 &hkey);

  if (result != ERROR_SUCCESS)
    {
      if (result == ERROR_FILE_NOT_FOUND)
	{
	  return (false);
	}
      FATAL_WINDOWS_ERROR_2 (T_("RegOpenKeyEx"),
			     result,
			     registryPath.c_str());
    }

  AutoHKEY autoClose (hkey);

  result = RegDeleteValue(hkey, lpszValueName);

  if (result != ERROR_SUCCESS)
    {
      if (result == ERROR_FILE_NOT_FOUND)
	{
	  return (false);
	}
      FATAL_WINDOWS_ERROR_2 (T_("RegDeleteValue"),
			     result,
			     lpszValueName);
    }

  return (true);
}

/* _________________________________________________________________________
   
   SetRegistryValue
   _________________________________________________________________________ */

void
winRegistry::SetRegistryValue (/*[in]*/ TriState		shared,
			       /*[in]*/ const MIKTEXCHAR *	lpszKeyName,
			       /*[in]*/ const MIKTEXCHAR *	lpszValueName,
			       /*[in]*/ const MIKTEXCHAR *	lpszValue)
{
  MIKTEX_ASSERT_STRING (lpszKeyName);
  MIKTEX_ASSERT_STRING (lpszValueName);
  MIKTEX_ASSERT_STRING (lpszValue);

  if (shared == TriState::Undetermined)
    {
      shared = SessionImpl::GetSession()->IsSharedMiKTeXSetup();
      if (shared == TriState::Undetermined)
	{
	  if (! IsWindowsNT()
	      || SessionImpl::GetSession()->RunningAsAdministrator())
	    {
	      shared = TriState::True;
	    }
	  else
	    {
	      shared = TriState::False;
	    }
	}
      // <recursivecall>
      SetRegistryValue (shared, lpszKeyName, lpszValueName, lpszValue);
      // </recursivecall>
      return;
    }
 
  tstring registryPath = MIKTEX_REGPATH_SERIES;
  registryPath += T_('\\');
  registryPath += lpszKeyName;

  HKEY hkey;

  unsigned long disp;

  HKEY hkeyRoot =
    (shared == TriState::False
     ? HKEY_CURRENT_USER
     : HKEY_LOCAL_MACHINE);

  SessionImpl::GetSession()->trace_config->WriteFormattedLine
    (T_("core"),
     T_("RegCreateKey (%p, \"%s\")"),
     reinterpret_cast<void*>(hkeyRoot),
     registryPath.c_str());

  long result =
    RegCreateKeyEx(hkeyRoot,
		   registryPath.c_str(),
		   0,
		   T_(""),
		   REG_OPTION_NON_VOLATILE,
		   KEY_ALL_ACCESS,
		   0,
		   &hkey,
		   &disp);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegCreateKeyEx"),
			     result,
			     registryPath.c_str());
    }

  AutoHKEY autoClose (hkey);

  unsigned long sizeValue = static_cast<unsigned long>(StrLen(lpszValue) + 1);
  sizeValue *= sizeof(MIKTEXCHAR);

  result =
    RegSetValueEx(hkey,
		  lpszValueName,
		  0,
		  REG_SZ,
		  const_cast<unsigned char *>
		  (reinterpret_cast<const unsigned char *>(lpszValue)),
		  sizeValue);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegSetValueEx"),
			     result,
			     lpszValueName);
    }

  tstring value;

  if (hkeyRoot == HKEY_LOCAL_MACHINE
      && TryGetRegistryValue(TriState::False,
			  lpszKeyName,
			  lpszValueName,
			  value,
			  0))
    {
      TryDeleteRegistryValue (TriState::False, lpszKeyName, lpszValueName);
    }
}
