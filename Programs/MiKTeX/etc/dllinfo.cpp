/* dllinfo.cpp: print DLL version information

   Copyright (C) 2001-2006 Christian Schenk

   This file is part of DllInfo.

   DllInfo is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   DllInfo is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with DllInfo; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <Shlwapi.h>
#include <malloc.h>

/* _________________________________________________________________________

   GetDllVersionInfo
   _________________________________________________________________________ */

bool
GetDllVersionInfo (const char *		lpszPath,
		   VS_FIXEDFILEINFO &	ffi)
{
  DWORD dwHandle;
  DWORD dwSize = GetFileVersionInfoSize(const_cast<char*>(lpszPath), &dwHandle);
  if (dwSize == 0)
    {
      return (false);
    }
  void * lpBuffer = _alloca(dwSize);
  if (! GetFileVersionInfo(const_cast<char*>(lpszPath), 0, dwSize, lpBuffer))
    {
      return (false);
    }
  VS_FIXEDFILEINFO * pffi;
  UINT uInfoSize;
  if (! VerQueryValue(lpBuffer, "\\", reinterpret_cast<void**>(&pffi), &uInfoSize)
      || uInfoSize == 0)
    {
      return (false);
    }
  ffi = *pffi;
  return (true);
}

/* _________________________________________________________________________

   GetDllVersionInfo
   _________________________________________________________________________ */

bool
GetDllVersionInfo (HINSTANCE	hinstDll,
	       DLLVERSIONINFO &	dvi)
{
  DLLGETVERSIONPROC pDllGetVersion;
  
  pDllGetVersion
    = reinterpret_cast<DLLGETVERSIONPROC>(GetProcAddress(hinstDll,
							 "DllGetVersion"));
  
  if(! pDllGetVersion)
    {
      return (false);
    }
  
  HRESULT hr;
  ZeroMemory(&dvi, sizeof(dvi));
  dvi.cbSize = sizeof(dvi);
  hr = pDllGetVersion(&dvi);
  if (FAILED(hr))
    {
      return (false);
    }

  return (true);
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (int	argc,
      char **	argv)
{
  if (argc != 2)
    {
      fprintf (stderr, "Usage: %s DLLNAME\n", argv[0]);
      return (1);
    }
  HMODULE hmodule = LoadLibrary(argv[1]);
  if (hmodule == 0)
    {
      fprintf (stderr, "LoadLibrary() failed\n");
      return (1);
    }
  char szPath[_MAX_PATH];
  DWORD dw = GetModuleFileName(hmodule, szPath, _MAX_PATH);
  if (dw == 0)
    {
      fprintf (stderr, "GetModuleFileName() failed\n");
      return (1);
    }
  printf ("%s\n", szPath);
  DLLVERSIONINFO dvi;
  if (GetDllVersionInfo(hmodule, dvi))
    {
      printf ("%u.%u\n", dvi.dwMajorVersion, dvi.dwMinorVersion);
    }
  VS_FIXEDFILEINFO ffi;
  if (GetDllVersionInfo(szPath, ffi))
    {
      printf ("%u.%u.%u.%u\n",
	      HIWORD(ffi.dwFileVersionMS),
	      LOWORD(ffi.dwFileVersionMS),
	      HIWORD(ffi.dwFileVersionLS),
	      LOWORD(ffi.dwFileVersionLS));
    }
  FreeLibrary (hmodule);
  return (0);
  
}
