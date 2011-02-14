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

class PostScript  
{
protected:
  virtual
  ~PostScript ();

protected:
  virtual
  void
  Finalize ();

protected:
  virtual
  void
  Write (/*[in]*/ const void *	p,
	 /*[in]*/ unsigned	n)
    = 0;

protected:
  virtual
  void
  Execute (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...)
    = 0;

protected:
  PostScript ();

public:
  GraphicsInclusion *
  GetGraphicsInclusion (/*[in]*/ int idx)
  {
    if (idx == graphicsInclusions.size())
    {
      return (0);
    }
    else if (idx > graphicsInclusions.size())
    {
      INVALID_ARGUMENT ("GetGraphicsInclusion", 0);
    }
    return (graphicsInclusions[idx].Get());
  }

public:
  bool
  IsOpen ()
    const
  {
    return (openFlag);
  }

public:
  bool
  IsPageEmpty ()
    const
  {
    return (! pageBegunFlag);
  }

public:
  void
  AddDefinition (/*[in]*/ class PsdefSpecial * ppsdefspecial);

public:
  void
  BeginPage ();

public:
  void
  Close ();

public:
  void
  DoSpecial (/*[in]*/ class PsfileSpecial * ppsfilespecial);

public:
  void
  DoSpecial (/*[in]*/ class DvipsSpecial * pdvipsspecial);

public:
  void
  EndPage ();

public:
  void
  Open (/*[in]*/ class DviImpl * pDviImpl,
        /*[in]*/ int shrinkFactor);

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
  CopyFile (/*[in]*/ FileStream & stream,
	    /*[in]*/ unsigned	  length);

protected:
  void
  DoDefinitions ();

protected:
  void
  DoProlog ();

protected:
  void
  ExecuteBatch (/*[in]*/ const char * lpszFileName);

protected:
  void
  ExecuteEncapsulatedPostScript (/*[in]*/ const char * lpszFileName);

protected:
  bool
  FindGraphicsFile (/*[in]*/ const char *	lpszFileName,
		    /*[out]*/ PathName &	result);

protected:
  void
  Initialize ();

protected:
  void
  SendHeader (/*[in]*/ const char * lpszHeaderName);

protected:
  void
  Uncompress (/*[in]*/ const char *	lpszFileName,
	      /*[out]*/ PathName &	result);

protected:
  DviImpl * pDviImpl;

protected:
  int shrinkFactor;

protected:
  bool openFlag;

protected:
  bool pageBegunFlag;

protected:
  vector<string> definitions;

protected:
  vector<string> headers;

protected:
  vector<SmartPointer<GraphicsInclusion> > graphicsInclusions;

protected:
  auto_ptr<TraceStream> tracePS;
};
