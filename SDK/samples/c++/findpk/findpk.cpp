/* findpk.cpp: find a PK file

   Copyright (C) 2006-2007 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with This file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

#include <cstdio>

#import <session.tlb>

using namespace MiKTeXSessionLib;

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
  if (argc != 4)
    {
      puts ("Usage: findpk FONTNAME MODE DPI");
      puts (" e.g.: findpk cmr10 ljfour 600");
      return (1);
    }

  if (FAILED(CoInitialize(0)))
  {
    puts ("COM library could not be initialized.");
    return (1);
  }

  int retCode = 0;

  try
    {
      ISessionPtr pSession;
      HRESULT hr = pSession.CreateInstance(__uuidof(MiKTeXSession));
      if (FAILED(hr))
      {
	_com_raise_error (hr);
      }
      _bstr_t fontName (argv[1]);
      _bstr_t mode (argv[2]);
      long dpi = atoi(argv[3]);
      _bstr_t path;
      if (pSession->FindPkFile(fontName, mode, dpi, path.GetAddress()))
      {
	puts (path);
      }
    }
  catch (const _com_error & e)
    {
      puts (e.Description());
      retCode = 1;
    }

  CoUninitialize ();

  return (retCode);
}
