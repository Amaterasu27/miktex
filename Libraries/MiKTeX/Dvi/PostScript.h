/* PostScript.h:					-*- C++ -*-

   Copyright (C) 1996-2011 Christian Schenk

   This file is part of the MiKTeX DVI Library.

   The MiKTeX DVI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX DVI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the MiKTeX DVI Library; if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
   USA.  */

#pragma once

class DviImpl;

class CPostScript  
{
protected:
  CPostScript ();

protected:
  virtual ~CPostScript ();

public:
  struct BitmapFile
  {
    char szBitmapFileName[BufferSizes::MaxPath];
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
  AddDefinition (/*[in]*/ PsdefSpecialImpl * ppsdefspecial);

public:
  void
  BeginPage ();

public:
  void
  Close ();

public:
  void
  DoSpecial (/*[in]*/ PsfileSpecialImpl * ppsfilespecial);

public:
  void
  DoSpecial (/*[in]*/ DvipsSpecialImpl * pdvipsspecial);

public:
  void
  EndPage ();

public:
  void
  Open (/*[in]*/ DviImpl * pDviImpl,
        /*[in]*/ int shrinkFactor);

public:
  static
  bool
  IsEnabled ();

protected:
  void
  AddHeader (/*[in]*/ const char * lpszFileName);

protected:
  FILE *
  ConvertToEPS (/*[in]*/ const char * lpszFileName);

private:
  static
  void
  MIKTEXCALLBACK
  ConvertToEPSThread (/*[in]*/ void * pv);

protected:
  void
  CopyFile (/*[in]*/ FILE *	pfile,
	    /*[in]*/ unsigned	length);

protected:
  void
  DoDefinitions ();

protected:
  void
  DoProlog ();

protected:
  virtual
  void
  Execute (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...)
    = 0;

protected:
  void
  ExecuteBatch (/*[in]*/ const char * lpszFileName);

protected:
  void
  ExecuteEncapsulatedPostScript (/*[in]*/ const char * lpszFileName);

protected:
  virtual
  void
  Finalize ();

protected:
  bool
  FindGraphicsFile (/*[in]*/ const char *	lpszFileName,
		    /*[out]*/ char *		lpszResult);

protected:
  void
  Initialize ();

protected:
  bool
  InternalFindGraphicsFile (/*[in]*/ const char *	lpszFileName,
			    /*[out]*/ char *		lpszResult);

protected:
  void
  SendHeader (/*[in]*/ const char * lpszHeaderName);

protected:
  void
  Uncompress (/*[in]*/ const char *	lpszFileName,
	      /*[out]*/ char *		lpszTempFileName);

protected:
  virtual
  void
  Write (/*[in]*/ const void *	p,
	 /*[in]*/ unsigned	n)
    = 0;

protected:
  FILE * m_pfile;

protected:
  DviImpl * pDviImpl;

protected:
  int shrinkFactor;

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

protected:
  auto_ptr<TraceStream> tracePS;
};
