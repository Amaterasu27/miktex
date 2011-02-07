/* PostScript.h:					-*- C++ -*-

   Copyright (C) 1996-2004 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#pragma once

class CDviDoc;

class CPostScript  
{
protected:
  CPostScript ();

protected:
  virtual ~CPostScript ();

public:
  struct BitmapFile
  {
    _TCHAR szBitmapFileName[_MAX_PATH];
    int x;
    int y;
    int cx;
    int cy;
  };

public:
  bool
  GetBitmapFile (/*[in]*/ size_t	i,
		 /*[out]*/ BitmapFile &	bmf)
  {
    if (i >= m_vecBitmapFiles.size())
      {
	return (false);
      }
    else
      {
	bmf = m_vecBitmapFiles[i];
	return (true);
      }
  }

public:
  bool
  IsOpen ()
    const
  {
    return (m_bOpen);
  }

public:
  bool
  IsPageEmpty ()
    const
  {
    return (! m_bPageBegun);
  }

public:
  void
  Clear ();

public:
  void
  ClearChunkTable ()
  {
    m_vecBitmapFiles.clear ();
  }

public:
  bool
  IsError () const
  {
    return (m_bError);
  }

public:
  void
  Restart ()
  {
    Clear ();
    m_bError = false;
  }

public:
  void
  AddDefinition (/*[in]*/ const MiKTeX::psdefspecial * ppsdefspecial);

public:
  bool
  BeginPage ();

public:
  bool
  Close ();

public:
  bool
  DoSpecial (/*[in]*/ const MiKTeX::psfilespecial * ppsfilespecial);

public:
  bool
  DoSpecial (/*[in]*/ const MiKTeX::dvipsspecial * pdvipsspecial);

public:
  bool
  EndPage ();

public:
  bool
  Open (/*[in]*/ CDviDoc * pDviDoc);

public:
  static
  bool
  IsEnabled ();

protected:
  void
  AddHeader (/*[in]*/ LPCSTR lpszFileName);

protected:
  FILE *
  ConvertToEPS (/*[in]*/ LPCSTR lpszFileName);

private:
  static
  unsigned
  __stdcall
  ConvertToEPSThread (/*[in]*/ void * pv);

protected:
  bool
  CopyFile (/*[in]*/ FILE *	pfile,
	    /*[in]*/ unsigned	length);

protected:
  bool
  DoDefinitions ();

protected:
  bool
  DoProlog ();

protected:
  virtual
  bool
  Execute (/*[in]*/ LPCSTR	lpszFormat,
	   /*[in]*/		...)
    = 0;

protected:
  bool
  ExecuteBatch (/*[in]*/ LPCSTR lpszFileName);

protected:
  bool
  ExecuteEncapsulatedPostScript (/*[in]*/ LPCSTR lpszFileName);

protected:
  virtual
  bool
  Finalize ();

protected:
  bool
  FindGraphicsFile (/*[in]*/ LPCSTR	lpszFileName,
		    /*[out]*/ LPTSTR	lpszResult);

protected:
  bool
  Initialize ();

protected:
  bool
  InternalFindGraphicsFile (/*[in]*/ LPCSTR	lpszFileName,
			    /*[out]*/ LPTSTR	lpszResult);

protected:
  bool
  SendHeader (/*[in]*/ LPCSTR lpszHeaderName);

protected:
  bool
  Uncompress (/*[in]*/ LPCSTR	lpszFileName,
	      /*[out]*/ LPTSTR	lpszTempFileName);

protected:
  virtual
  bool
  Write (/*[in]*/ const void *	p,
	 /*[in]*/ unsigned	n)
    = 0;

protected:
  FILE * m_pfile;

protected:
  CDviDoc * m_pDviDoc;

protected:
  bool m_bOpen;

protected:
  bool m_bPageBegun;

protected:
  bool m_bError;

protected:
  std::vector<std::string> m_setDefinitions;

protected:
  std::vector<std::string> m_setHeaders;

protected:
  std::vector<BitmapFile> m_vecBitmapFiles;
};
