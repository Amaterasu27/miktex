/* BitmapPrinter.h:						-*- C++ -*-

   Copyright (C) 2003-2006 Christian Schenk

   This file is part of MTPrint.

   MTPrint is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MTPrint is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MTPrint; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#pragma once

class IPrinterCallback
{
public:
  virtual
  void
  Report (/*[in]*/ const char *	lpszFormat,
	  /*[in]*/			...)
    = 0;
};

class Printer
{
public:
  Printer ();

public:
  virtual
  ~Printer ();

public:
  struct PRINTINFO
  {
    string printerName;
    IPrinterCallback * pCallback;
  };

public:
  Printer (/*[in]*/ const PRINTINFO &	printInfo,
	   /*[in]*/ bool		printNothing = false);

public:
  static
  PRINTER_INFO_2 *
  GetPrinterInfo (/*[in]*/ const char *	lpszPrinterName,
		  /*[out]*/ DEVMODE **		ppDevMode);

public:
  static
  DEVMODE *
  GetDevMode (/*[in]*/ const char * lpszPrinterName);

public:
  static
  void
  GetPrinterCaps (/*[in]*/ const char *	lpszPrinterName,
		  /*[in]*/ unsigned &		resolution);

protected:
  void
  StartJob ();

protected:
  void
  EndJob ();

protected:
  void
  StartPage ();

protected:
  void
  EndPage ();

protected:
  void
  Finalize ();

protected:
  bool
  JobStarted ()
    const
  {
    return (jobStarted);
  }

protected:
  bool
  PageStarted ()
    const
  {
    return (pageStarted);
  }

protected:
  HDC
  GetDC ()
    const
  {
    return (hdcPrinter);
  }

private:
  static
  HDC
  CreateDC (/*[in]*/ const char *	lpszPrinterName);

private:
  void
  CreateDC ();

private:
  HDC hdcPrinter;

private:
  bool jobStarted;

private:
  bool pageStarted;

private:
  int jobId;

private:
  PRINTINFO printInfo;

private:
  size_t currentPageNum;

protected:
  bool dryRun;

protected:
  auto_ptr<TraceStream> trace_mtprint;
};
