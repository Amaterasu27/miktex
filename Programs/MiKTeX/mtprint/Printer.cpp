/* BitmapPrinter.cpp:

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

#include "StdAfx.h"

#include "internal.h"

#include "Printer.h"

/* _________________________________________________________________________

   Printer::Printer
   _________________________________________________________________________ */

Printer::Printer ()

  : hdcPrinter (0),
    jobStarted (false),
    pageStarted (false),
    dryRun (false)

{
}

/* _________________________________________________________________________

   Printer::Printer
   _________________________________________________________________________ */

Printer::Printer (/*[in]*/ const PRINTINFO &	printInfo,
		  /*[in]*/ bool			printNothing)

  : printInfo (printInfo),
    hdcPrinter (0),
    jobStarted (false),
    pageStarted (false),
    dryRun (printNothing),
    trace_mtprint (TraceStream::Open(MIKTEX_TRACE_MTPRINT))

{
}

/* _________________________________________________________________________

   Printer::~Printer
   _________________________________________________________________________ */

Printer::~Printer ()
{
  try
    {
      Finalize ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   Printer::GetDevMode
   _________________________________________________________________________ */

DEVMODE *
Printer::GetDevMode (/*[in]*/ const char *	lpszPrinterName)
{
  HANDLE hPrinter;
  if (! OpenPrinter(const_cast<char *>(lpszPrinterName), &hPrinter, 0))
    {
      FATAL_WINDOWS_ERROR (T_("OpenPrinter"), lpszPrinterName);
    }
  AutoClosePrinter autoClosePrinter (hPrinter);
  long bytesNeeded =
    DocumentProperties(0,
		       hPrinter,
		       const_cast<char *>(lpszPrinterName),
		       0,
		       0,
		       0);
  if (bytesNeeded < 0)
    {
      FATAL_WINDOWS_ERROR (T_("DocumentProperties"), lpszPrinterName);
    }
  DEVMODE * pDevMode = reinterpret_cast<DEVMODE*>(malloc(bytesNeeded));
  if (pDevMode == 0)
    {
      OUT_OF_MEMORY ("Printer::GetDevMode");
    }
  if (DocumentProperties(0,
			 hPrinter,
			 const_cast<char *>(lpszPrinterName),
			 pDevMode,
			 0,
			 DM_OUT_BUFFER)
      < 0)
    {
      free (pDevMode);
      FATAL_WINDOWS_ERROR (T_("DocumentProperties"), lpszPrinterName);
    }
  return (pDevMode);
}

/* _________________________________________________________________________

   Printer::GetPrinterInfo
   _________________________________________________________________________ */

PRINTER_INFO_2 *
Printer::GetPrinterInfo (/*[in]*/ const char *	lpszPrinterName,
			 /*[in]*/ DEVMODE **		ppDevMode)
{
  HANDLE hPrinter;
  if (! OpenPrinter(const_cast<char*>(lpszPrinterName), &hPrinter, 0))
    {
      FATAL_WINDOWS_ERROR (T_("OpenPrinter"), lpszPrinterName);
    }
  AutoClosePrinter autoClosePrinter (hPrinter);
  unsigned long bytesNeeded;
  if (! (GetPrinter(hPrinter, 2, 0, 0, &bytesNeeded)
	 || GetLastError() == ERROR_INSUFFICIENT_BUFFER))
    {
      FATAL_WINDOWS_ERROR (T_("GetPrinter"), lpszPrinterName);
    }
  PRINTER_INFO_2 * p2 =
    reinterpret_cast<PRINTER_INFO_2*>(malloc(bytesNeeded));
  if (p2 == 0)
    {
      OUT_OF_MEMORY ("Printer::GetPrinterInfo");
    }
  AutoMemoryPointer autoFreeResult (p2);
  unsigned long bytesReturned;
  if (! GetPrinter(hPrinter,
		   2,
		   reinterpret_cast<BYTE*>(p2),
		   bytesNeeded,
		   &bytesReturned))
    {
      FATAL_WINDOWS_ERROR (T_("GetPrinter"), lpszPrinterName);
    }
  if (ppDevMode != 0)
    {
      long bytesNeeded =
	DocumentProperties(0,
			   hPrinter,
			   const_cast<char*>(lpszPrinterName),
			   0,
			   0,
			   0);
      if (bytesNeeded < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("DocumentProperties"), lpszPrinterName);
	}
      *ppDevMode = reinterpret_cast<DEVMODE*>(malloc(bytesNeeded));
      if (ppDevMode == 0)
	{
	  OUT_OF_MEMORY ("Printer::GetPrinterInfo");
	}
      if (DocumentProperties(0,
			     hPrinter,
			     const_cast<char*>(lpszPrinterName),
			     *ppDevMode,
			     0,
			     DM_OUT_BUFFER)
	  < 0)
	{
	  free (*ppDevMode);
	  FATAL_WINDOWS_ERROR (T_("DocumentProperties"), lpszPrinterName);
	}
    }
  autoFreeResult.Detach ();
  return (p2);
}

/* _________________________________________________________________________

   Printer::CreateDC
   _________________________________________________________________________ */

HDC
Printer::CreateDC (/*[in]*/ const char *	lpszPrinterName)
{
  DEVMODE * pDevMode = 0;
  PRINTER_INFO_2 * p2 = GetPrinterInfo(lpszPrinterName, &pDevMode);
  AutoMemoryPointer autoFree1 (p2);
  AutoMemoryPointer autoFree2 (pDevMode);
  size_t size = pDevMode->dmSize + pDevMode->dmDriverExtra;
  DEVMODE * pDevMode2 = reinterpret_cast<DEVMODE*>(_alloca(size));
  memcpy (pDevMode2, p2->pDevMode, size);
#if 0
  trace_mtprint->WriteFormattedLine (T_("mtprint"),
				     T_("CreateDC(\"%s\", \"%s\")"),
				     p2->pDriverName,
				     p2->pPrinterName);
#endif
  HDC hdc = ::CreateDC(p2->pDriverName, p2->pPrinterName, 0, pDevMode2);
  if (hdc == 0)
    {
      FATAL_MIKTEX_ERROR ("Printer::CreateDC",
			  T_("The device context could not be created."),
			  lpszPrinterName);
    }
  return (hdc);
} 

/* _________________________________________________________________________

   Printer::CreateDC
   _________________________________________________________________________ */

void
Printer::CreateDC ()
{
  hdcPrinter = CreateDC(printInfo.printerName.c_str());
} 

/* _________________________________________________________________________

   Printer::GetPrinterCaps
   _________________________________________________________________________ */

void
Printer::GetPrinterCaps (/*[in]*/ const char *	lpszPrinterName,
			 /*[in]*/ unsigned &		resolution)
{
  HDC hdc = CreateDC(lpszPrinterName);
  resolution = static_cast<unsigned>(GetDeviceCaps(hdc, LOGPIXELSX));
  ::DeleteDC (hdc);
} 

/* _________________________________________________________________________

   Printer::StartJob
   _________________________________________________________________________ */

void
Printer::StartJob ()
{
  trace_mtprint->WriteLine (T_("mtprint"), T_("Starting print job"));
  CreateDC ();
  DOCINFO di;
  di.cbSize = sizeof(di);
  di.lpszDocName = T_("My Document");
  di.lpszOutput = 0;
  di.lpszDatatype = 0;
  di.fwType = 0;
  if (! dryRun)
    {
      jobId = StartDoc(hdcPrinter, &di);
      if (jobId <= 0)
	{
	  DeleteDC (hdcPrinter);
	  hdcPrinter = 0;
	  FATAL_MIKTEX_ERROR ("Printer::StartJob",
			      T_("The print job could not be started."),
			      0);
	}
    }
  jobStarted = true;
  pageStarted = false;
  currentPageNum = 0;
}

/* _________________________________________________________________________

   Printer::EndJob
   _________________________________________________________________________ */

void
Printer::EndJob ()
{
  trace_mtprint->WriteLine (T_("mtprint"), T_("Ending print job"));
  if (! dryRun)
    {
      ::EndDoc (hdcPrinter);
    }
  jobStarted = false;
}

/* _________________________________________________________________________

   Printer::StartPage
   _________________________________________________________________________ */

void
Printer::StartPage ()
{
  currentPageNum += 1;
  trace_mtprint->WriteFormattedLine
    (T_("mtprint"),
     T_("Starting page %u"),
     currentPageNum);
  if (! dryRun)
    {
      if (::StartPage(hdcPrinter) <= 0)
	{
	  FATAL_MIKTEX_ERROR ("Printer::StartJob",
			      T_("The page could not be started."),
			      NUMTOSTR(static_cast<unsigned>(currentPageNum)));
	}
    }
  pageStarted = true;
  if (printInfo.pCallback != 0)
    {
      printInfo.pCallback->Report (T_("[%u"), currentPageNum);
    }
}

/* _________________________________________________________________________

   Printer::EndPage
   _________________________________________________________________________ */

void
Printer::EndPage ()
{
  trace_mtprint->WriteLine (T_("mtprint"), T_("Ending page"));
  if (! dryRun)
    {
      ::EndPage (hdcPrinter);
    }
  if (printInfo.pCallback != 0)
    {
      printInfo.pCallback->Report ("]");
    }
  pageStarted = false;
}

/* _________________________________________________________________________

   Printer::Finalize
   _________________________________________________________________________ */

void
Printer::Finalize ()
{
  if (PageStarted())
    {
      EndPage ();
    }
  if (JobStarted())
    {
      EndJob ();
    }
  if (hdcPrinter != 0)
    {
      DeleteDC (hdcPrinter);
      hdcPrinter = 0;
    }
  if (trace_mtprint.get() != 0)
    {
      trace_mtprint->Close ();
      trace_mtprint.reset ();
    }
}
