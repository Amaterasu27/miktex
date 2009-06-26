/* winRegistry.h:						-*- C++ -*-

   Copyright (C) 1996-2009 Christian Schenk

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(MIKTEX__8E7CD540_44F4_4C23_B9AA_82C3FDF959DA__)
#define MIKTEX__8E7CD540_44F4_4C23_B9AA_82C3FDF959DA__

BEGIN_INTERNAL_NAMESPACE;

class winRegistry
{
public:
  static
  bool
  TryGetRegistryValue (/*[in]*/ HKEY		hkeyParent,
		       /*[in]*/ const char *	lpszPath,
		       /*[in]*/ const char *	lpszValueName,
		       /*[out]*/ string &	value,
		       /*[in]*/ const char * lpszDefaultValue);

public:
  static
  bool
  TryDeleteRegistryValue (/*[in]*/ HKEY	   hkeyParent,
			  /*[in]*/ const char * lpszPath,
			  /*[in]*/ const char * lpszValueName);

public:
  static
  bool
  TryDeleteRegistryKey (/*[in]*/ HKEY	   hkeyParent,
			/*[in]*/ const char * lpszPath);

public:
  static
  void
  SetRegistryValue (/*[in]*/ HKEY		hkeyParent,
		    /*[in]*/ const char *	lpszPath,
		    /*[in]*/ const char *	lpszValueName,
		    /*[in]*/ const char *	lpszValue);

public:
  static
  bool
  TryGetRegistryValue (/*[in]*/ TriState	shared,
		       /*[in]*/ const char *	lpszKeyName,
		       /*[in]*/ const char *	lpszValueName,
		       /*[out]*/ string &	value,
		       /*[in]*/ const char *	lpszDefaultValue);

public:
  static
  bool
  TryGetRegistryValue (/*[in]*/ TriState	shared,
		       /*[in]*/ const char *	lpszKeyName,
		       /*[in]*/ const char *	lpszValueName,
		       /*[out]*/ PathName &	path,
		       /*[in]*/ const char *	lpszDefaultPath);


public:
  static
  bool
  TryDeleteRegistryValue (/*[in]*/ TriState	shared,
			  /*[in]*/ const char *	lpszKeyName,
			  /*[in]*/ const char *	lpszValueName);

public:
  static
  void
  SetRegistryValue (/*[in]*/ TriState		shared,
		    /*[in]*/ const char *	lpszKeyName,
		    /*[in]*/ const char *	lpszValueName,
		    /*[in]*/ const char *	lpszValue);
};

END_INTERNAL_NAMESPACE;

#endif	// winRegistry.h
