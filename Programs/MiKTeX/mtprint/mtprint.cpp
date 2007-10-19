/* mtprint.cpp: MiKTeX Print Utility

   Copyright (C) 2003-2007 Christian Schenk

   This file is part of MiKTeX Print Utility.

   MiKTeX Print Utility is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Print Utility is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Print Utility; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"

#include "internal.h"

#include "BitmapPrinter.h"

#ifndef THE_NAME_OF_THE_GAME
#  define THE_NAME_OF_THE_GAME T_("MiKTeX Print Utility")
#endif

const char * DEFAULT_TRACE_STREAMS =
  MIKTEX_TRACE_ERROR "," MIKTEX_TRACE_MTPRINT
  ;

/* _________________________________________________________________________

   PAPERSIZEINFO
   _________________________________________________________________________ */

struct PAPERSIZEINFO
{
  short			paperSize;
  const char *	lpszName;
  const char *	lpszDvipsSize;
  unsigned		width;
  unsigned		height;
};


/* _________________________________________________________________________

   DVIPSOPTS
   _________________________________________________________________________ */

struct DVIPSOPTS
{
  struct PAGERANGE
  {
    int firstPage, lastPage;
  };
  bool evenPagesOnly;
  bool oddPagesOnly;
  bool runAsFilter;
  bool runQuietly;
  bool sendCtrlDAtEnd;
  bool shiftLowCharsToHigherPos;
  string config;
  string offsetX;
  string offsetY;
  string paperFormat;
  vector<PAGERANGE> pageRanges;
  DVIPSOPTS ()
    : evenPagesOnly (false),
      oddPagesOnly (false),
      runAsFilter (false),
      runQuietly (false),
      sendCtrlDAtEnd (false),
      shiftLowCharsToHigherPos (false)
  {
  }
};

/* _________________________________________________________________________

   GSOPTS
   _________________________________________________________________________ */

struct GSOPTS
{
  GSOPTS ()
  {
  }
};

/* _________________________________________________________________________

   PrintUtility
   _________________________________________________________________________ */

class PrintUtility
  : public Application,
    public IPrinterCallback
{
private:
  enum PrintMethod {
    PrintMethodPostScript,
    PrintMethodPostScriptBMP
  };

public:
  PrintUtility ()
    : verbose (false),
      dryRun (false),
      printMethod (PrintMethodPostScriptBMP),
      trace_mtprint (TraceStream::Open(MIKTEX_TRACE_MTPRINT)),
      trace_error (TraceStream::Open(MIKTEX_TRACE_ERROR))
  {
  }

public:
  virtual
  ~PrintUtility ()
  {
    try
      {
	trace_mtprint->Close ();
	trace_error->Close ();
      }
    catch (const exception &)
      {
      }
  }

public:
  virtual
  void
  Report (/*[in]*/ const char *	lpszFormat,
	  /*[in]*/			...);

public:
  void
  Run (/*[in]*/ int			argc,
       /*[in]*/ const char **	argv);

private:
  void
  ShowVersion ();

private:
  bool
  GetPaperSizeInfo (/*[in]*/ short		paperSize,
		    /*[out]*/ PAPERSIZEINFO &	psi);

private:
  void
  StartDvips (/*[in]*/ const char *	lpszDviFileName,
	      /*[in]*/ const DVIPSOPTS &	dvipsOpts,
	      /*[in]*/ unsigned			resolution,
	      /*[in]*/ const char *	lpszPrinterName,
	      /*[in]*/ short			paperSize,
	      /*[out]*/ FILE * *		ppfileDvipsOutRd = 0,
	      /*[out]*/ FILE * *		ppfileDvipsErrRd = 0);

private:
  void
  StartGhostscript (/*[in]*/ const GSOPTS &	gsOpts,
		    /*[in]*/ unsigned		resolution,
		    /*[in]*/ short		paperSize,
		    /*[in]*/ FILE *		pfileGsIn,
		    /*[out]*/ FILE **		ppfileGsOut);

private:
  void
  Spool (/*[in]*/ const char *	lpszFileName,
	 /*[in]*/ PrintMethod		printMethod,
	 /*[in]*/ const DVIPSOPTS &	dvipsOpts,
	 /*[in]*/ const GSOPTS &	gsOpts,
	 /*[in]*/ const string &	printerName);


private:
  string printerName;

private:
  PrintMethod printMethod;

private:
  bool verbose;

private:
  bool dryRun;

private:
  auto_ptr<TraceStream> trace_mtprint;

private:
  auto_ptr<TraceStream> trace_error;

private:
  static const PAPERSIZEINFO paperSizes[];

private:
  static const struct poptOption aoption[];
};


/* _________________________________________________________________________

   PrintUtility::paperSizes
   _________________________________________________________________________ */

#define mm2pt(mm) static_cast<int>((mm) * (1.0 / 25.4) * 72.0)
#define in2pt(inch) static_cast<int>((inch) * 72.0)

#define MM2PT(a, b) #a "mm," #b "mm", mm2pt(a), mm2pt(b)
#define IN2PT(a, b) #a T_("in,") #b "in", in2pt(a), in2pt(b)

#define INCHPAPER(a, b, c, d) DMPAPER_##a, b, IN2PT(c, d)
#define MMPAPER(a, b, c, d) DMPAPER_##a, b, MM2PT(c, d)

const PAPERSIZEINFO PrintUtility::paperSizes[] = {
  INCHPAPER (LETTER, T_("Letter"), 8.5, 11),
  INCHPAPER (LETTERSMALL, T_("Letter Small"), 8.5, 11),
  INCHPAPER (TABLOID, T_("Tabloid"), 11, 17),
  INCHPAPER (LEDGER, T_("Ledger"), 17, 11),
  INCHPAPER (LEGAL, T_("Legal"), 8.5, 14),
  INCHPAPER (STATEMENT, T_("Statement"), 5.5, 8.5),
  INCHPAPER (EXECUTIVE, T_("Executive"), 7.25, 10.5),
  MMPAPER (A3, "A3", 297, 420),
  MMPAPER (A4, "A4", 210, 297),
  MMPAPER (A4SMALL, T_("A4 Small"), 210, 297),
  MMPAPER (A5, "A5", 148, 210),
  MMPAPER (B4, "B4", 250, 354), // <fixme/>
  MMPAPER (B5, "B5", 182, 257),
  INCHPAPER (FOLIO, T_("Folio"), 8.5, 13),
  MMPAPER (QUARTO, T_("Quarto"), 215, 275), // <fixme/>
  INCHPAPER (10X14, "10x14", 10, 14),
  INCHPAPER (11X17, "11x17", 11, 17),
  INCHPAPER (NOTE, T_("Note"), 8.5, 11),
  INCHPAPER (ENV_9, T_("Envelope #9"), 3.875, 8.875),
  INCHPAPER (ENV_10, T_("Envelope #10"), 4.125, 9.5),
  INCHPAPER (ENV_11, T_("Envelope #11"), 4.5, 10.375),
  INCHPAPER (ENV_12, T_("Envelope #12"), 4.75, 11),
  INCHPAPER (ENV_14, T_("Envelope #14"), 5, 11.5),
  INCHPAPER (CSHEET, "C", 17, 22),
  INCHPAPER (DSHEET, "D", 22, 34),
  INCHPAPER (ESHEET, "E", 34, 44),
  MMPAPER (ENV_DL, T_("Envelope DL"), 110, 220),
  MMPAPER (ENV_C5, T_("Envelope C5"), 162, 229),
  MMPAPER (ENV_C3, T_("Envelope C3 "), 324, 458),
  MMPAPER (ENV_C4, T_("Envelope C4 "), 229, 324),
  MMPAPER (ENV_C6, T_("Envelope C6 "), 114, 162),
  MMPAPER (ENV_C65, T_("Envelope C65"), 114, 229),
  MMPAPER (ENV_B4, T_("Envelope B4 "), 250, 353),
  MMPAPER (ENV_B5, T_("Envelope B5 "), 176, 250),
  MMPAPER (ENV_B6, T_("Envelope B6 "), 176, 125),
  MMPAPER (ENV_ITALY, T_("Italy Envelope"), 110, 230),
  INCHPAPER (ENV_MONARCH, T_("Envelope Monarch"), 3.875, 7.5),
  INCHPAPER (ENV_PERSONAL, T_("6 3/4 Envelope"), 3.625, 6.5),
  INCHPAPER (FANFOLD_US, T_("US Std Fanfold"), 14.875, 11),
  INCHPAPER (FANFOLD_STD_GERMAN, T_("German Std Fanfold"), 8.5, 12),
  INCHPAPER (FANFOLD_LGL_GERMAN, T_("German Legal Fanfold"), 8.5, 13),
#if(WINVER >= 0x0400)
  MMPAPER (ISO_B4, T_("B4 (ISO)"), 250, 353),
  MMPAPER (JAPANESE_POSTCARD, T_("Japanese Postcard"), 100, 148),
  INCHPAPER (9X11, "9x11", 9, 11),
  INCHPAPER (10X11, "10x11", 10, 11),
  INCHPAPER (15X11, "15x11", 15, 11),
  MMPAPER (ENV_INVITE, T_("Envelope Invite"), 220, 220),
  INCHPAPER (LETTER_EXTRA, T_("Letter Extra"), 9.5, 12),
  INCHPAPER (LEGAL_EXTRA, T_("Legal Extra"), 9.5, 15),
  INCHPAPER (TABLOID_EXTRA, T_("Tabloid Extra"), 11.69, 18),
  INCHPAPER (A4_EXTRA, T_("A4 Extra"), 9.27, 12.69),
  INCHPAPER (LETTER_TRANSVERSE, T_("Letter Transverse"), 8.5, 11),
  MMPAPER (A4_TRANSVERSE, T_("A4 Transverse"), 210, 297),
  INCHPAPER (LETTER_EXTRA_TRANSVERSE, T_("Letter Extra Transverse"), 9.5, 12),
  MMPAPER (A_PLUS, T_("SuperA/SuperA/A4"), 227, 356),
  MMPAPER (B_PLUS, T_("SuperB/SuperB/A3"), 305, 487),
  INCHPAPER (LETTER_PLUS, T_("Letter Plus"), 8.5, 12.69),
  MMPAPER (A4_PLUS, T_("A4 Plus"), 210, 330),
  MMPAPER (A5_TRANSVERSE, T_("A5 Transverse"), 148, 210),
  MMPAPER (B5_TRANSVERSE, T_("B5 (JIS) Transverse"), 182, 257),
  MMPAPER (A3_EXTRA, T_("A3 Extra"), 322, 445),
  MMPAPER (A5_EXTRA, T_("A5 Extra"), 174, 235),
  MMPAPER (B5_EXTRA, T_("B5 (ISO) Extra"), 201, 276),
  MMPAPER (A2, "A2", 420, 594),
  MMPAPER (A3_TRANSVERSE, T_("A3 Transverse"), 297, 420),
  MMPAPER (A3_EXTRA_TRANSVERSE, T_("A3 Extra Transverse"), 322, 445),
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
  MMPAPER (DBL_JAPANESE_POSTCARD, T_("Japanese Double Postcard"), 200, 148),
  MMPAPER (A6, "A6", 105, 148),
  // DMPAPER_JENV_KAKU2          71  /* Japanese Envelope Kaku #2 */
  // DMPAPER_JENV_KAKU3          72  /* Japanese Envelope Kaku #3 */
  // DMPAPER_JENV_CHOU3          73  /* Japanese Envelope Chou #3 */
  // DMPAPER_JENV_CHOU4          74  /* Japanese Envelope Chou #4 */
  INCHPAPER (LETTER_ROTATED, T_("Letter Rotated"), 11, 8.5),
  MMPAPER (A3_ROTATED, T_("A3 Rotated"), 420, 297),
  MMPAPER (A4_ROTATED, T_("A4 Rotated"), 297, 210),
  MMPAPER (A5_ROTATED, T_("A5 Rotated"), 210, 148),
  MMPAPER (B4_JIS_ROTATED, T_("B4 (JIS) Rotated"), 364, 257),
  MMPAPER (B5_JIS_ROTATED, T_("B5 (JIS) Rotated"), 257, 182),
  MMPAPER (JAPANESE_POSTCARD_ROTATED,
	   T_("Japanese Postcard Rotated"), 148, 100),
  MMPAPER (DBL_JAPANESE_POSTCARD_ROTATED,
	   T_("Double Japanese Postcard Rotated"), 148, 200),
  MMPAPER (A6_ROTATED, T_("A6 Rotated"), 148, 105),
  // DMPAPER_JENV_KAKU2_ROTATED  84  /* Japanese Envelope Kaku #2 Rotated */
  // DMPAPER_JENV_KAKU3_ROTATED  85  /* Japanese Envelope Kaku #3 Rotated */
  // DMPAPER_JENV_CHOU3_ROTATED  86  /* Japanese Envelope Chou #3 Rotated */
  // DMPAPER_JENV_CHOU4_ROTATED  87  /* Japanese Envelope Chou #4 Rotated */
  MMPAPER (B6_JIS, T_("B6 (JIS)"), 128, 182),
  MMPAPER (B6_JIS_ROTATED, T_("B6 (JIS) Rotated"), 182, 128),
  DMPAPER_12X11, "12x11", IN2PT(12, 11),
  // DMPAPER_JENV_YOU4           91  /* Japanese Envelope You #4        */
  // DMPAPER_JENV_YOU4_ROTATED   92  /* Japanese Envelope You #4 Rotated*/
  MMPAPER (P16K, "PRC 16K", 146, 215),
  MMPAPER (P32K, "PRC 32K", 97, 151),
  MMPAPER (P32KBIG, T_("PRC 32K(Big)"), 97, 151),
  MMPAPER (PENV_1, T_("PRC Envelope #1"), 102, 165),
  MMPAPER (PENV_2, T_("PRC Envelope #2"), 102, 176),
  MMPAPER (PENV_3, T_("PRC Envelope #3"), 125, 176),
  MMPAPER (PENV_4, T_("PRC Envelope #4"), 110, 208),
  MMPAPER (PENV_5, T_("PRC Envelope #5"), 110, 220),
  MMPAPER (PENV_6, T_("PRC Envelope #6"), 120, 230),
  MMPAPER (PENV_7, T_("PRC Envelope #7"), 160, 230),
  MMPAPER (PENV_8, T_("PRC Envelope #8"), 120, 309),
  MMPAPER (PENV_9, T_("PRC Envelope #9"), 229, 324),
  MMPAPER (PENV_10, T_("PRC Envelope #10"), 324, 458),
  MMPAPER (P16K_ROTATED, T_("PRC 16K Rotated"), 215, 146),
  MMPAPER (P32K_ROTATED, T_("PRC 32K Rotated"), 151, 97),
  MMPAPER (P32KBIG_ROTATED, T_("PRC 32K(Big) Rotated"), 151, 97),
  MMPAPER (PENV_1_ROTATED, T_("PRC Envelope #1 Rotated"), 165, 102),
  MMPAPER (PENV_2_ROTATED, T_("PRC Envelope #2 Rotated"), 176, 102),
  MMPAPER (PENV_3_ROTATED, T_("PRC Envelope #3 Rotated"), 176, 125),
  MMPAPER (PENV_4_ROTATED, T_("PRC Envelope #4 Rotated"), 208, 110),
  MMPAPER (PENV_5_ROTATED, T_("PRC Envelope #5 Rotated"), 220, 110),
  MMPAPER (PENV_6_ROTATED, T_("PRC Envelope #6 Rotated"), 230, 120),
  MMPAPER (PENV_7_ROTATED, T_("PRC Envelope #7 Rotated"), 230, 160),
  MMPAPER (PENV_8_ROTATED, T_("PRC Envelope #8 Rotated"), 309, 120),
  MMPAPER (PENV_9_ROTATED, T_("PRC Envelope #9 Rotated"), 324, 229),
  MMPAPER (PENV_10_ROTATED, T_("PRC Envelope #10 Rotated"), 458, 324),
#endif /* WINVER >= 0x0500 */
};

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_PRINTER = 1000,
  OPT_PRINT_METHOD,
  OPT_PAGE_RANGE,
  OPT_EVEN_ONLY,
  OPT_ODD_ONLY,
  OPT_LANDSCAPE,
  OPT_PRINT_NOTHING,
  OPT_HHELP,
  OPT_VERSION,
};

/* _________________________________________________________________________

   PrintUtility::aoption
   _________________________________________________________________________ */

const struct poptOption PrintUtility::aoption[] = {
  {
    T_("even-only"), 0, POPT_ARG_NONE, 0, OPT_EVEN_ONLY,
    T_("Prints only even TeX pages."), 0,
  },
  {
    T_("hhelp"), 0, POPT_ARG_NONE, 0, OPT_HHELP,
    T_("Show the manual page."), 0
  },
  {
    T_("landscape"), 0, POPT_ARG_NONE, 0, OPT_LANDSCAPE,
    T_("Selects landscape output format."), 0
  },
  {
    T_("odd-only"), 0, POPT_ARG_NONE, 0, OPT_ODD_ONLY,
    T_("Prints only odd TeX pages."), 0,
  },
  {
    T_("page-range"), 0, POPT_ARG_STRING, 0, OPT_PAGE_RANGE,
    T_("Selects a TeX page range (e.g. 20-21). ")
    T_("Multiple --page-range options accumulate."),
    T_("RANGE"),
  },
  {
    T_("print-method"), 0, POPT_ARG_STRING,
    0, OPT_PRINT_METHOD,
    T_("Selects a print method. One of: psbmp, ps."), T_("METHOD"),
  },
  {
    T_("print-nothing"), 0, POPT_ARG_NONE, 0, OPT_PRINT_NOTHING,
    T_("Simulates printing."), 0
  },
  { T_("printer"), 0, POPT_ARG_STRING,
    0, OPT_PRINTER,
    T_("Selects a printing device."), T_("PRINTER"),
  },
  {
    T_("version"), 0,
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },
  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   PrintUtility::Report
   _________________________________________________________________________ */

void
PrintUtility::Report (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
{
  if (! verbose)
    {
      return;
    }
  va_list argList;
  va_start (argList, lpszFormat);
  cout << Utils::FormatString(lpszFormat, argList);
  va_end (argList);
}

/* _________________________________________________________________________

   PrintUtility::GetPaperSizeInfo
   _________________________________________________________________________ */

bool
PrintUtility::GetPaperSizeInfo (/*[in]*/ short			paperSize,
				/*[out]*/ PAPERSIZEINFO &	paperSizeInfo)
{
  for (size_t idx = 0;
       idx < sizeof(paperSizes) / sizeof(paperSizes[0]);
       ++ idx)
    {
      if (paperSizes[idx].paperSize == paperSize)
	{
	  paperSizeInfo = paperSizes[idx];
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   PrintUtility::StartDvips
   _________________________________________________________________________ */

void
PrintUtility::StartDvips (/*[in]*/ const char *		lpszDviFileName,
			  /*[in]*/ const DVIPSOPTS &	dvipsOpts,
			  /*[in]*/ unsigned		resolution,
			  /*[in]*/ const char *		lpszPrinterName,
			  /*[in]*/ short		paperSize,
			  /*[out]*/ FILE * *		ppfileDvipsOutRd,
			  /*[out]*/ FILE * *		ppfileDvipsErrRd)
{
  // locate dvips.exe
  PathName dvipsPath;
  if (! SessionWrapper(true)->FindFile(MIKTEX_DVIPS_EXE,
				       FileType::EXE,
				       dvipsPath))
    {
      FATAL_MIKTEX_ERROR ("PrintUtility::StartDvips",
			  T_("The Dvips utility could not be found."),
			  0);
    }

  // make dvips command line
  CommandLineBuilder commandLine;
  if (dvipsOpts.oddPagesOnly)
    {
      commandLine.AppendOption (T_("-A"));
    }
  if (dvipsOpts.evenPagesOnly)
    {
      commandLine.AppendOption ("-B");
    }
  if (resolution > 0)
    {
      commandLine.AppendOption ("-D", NUMTOSTR(resolution));
    }
  PAPERSIZEINFO paperSizeInfo;
  if (GetPaperSizeInfo(paperSize, paperSizeInfo))
    {
      commandLine.AppendOption ("-T", paperSizeInfo.lpszDvipsSize);
    }
  if (dvipsOpts.runAsFilter)
    {
      commandLine.AppendOption ("-f", "1");
    }
  if (dvipsOpts.sendCtrlDAtEnd)
    {
      commandLine.AppendOption ("-F"), "1";
    }
  if (dvipsOpts.shiftLowCharsToHigherPos)
    {
      commandLine.AppendOption ("-G"), "1";
    }
  if (dvipsOpts.offsetX.length() > 0
      && dvipsOpts.offsetY.length() > 0)
    {
      string str (dvipsOpts.offsetX);
      str += ",";
      str += dvipsOpts.offsetY;
      commandLine.AppendOption (T_("-O"), str.c_str());
    }
  if (! dvipsOpts.runAsFilter)
    {
      PRINTER_INFO_2 * pi2 = Printer::GetPrinterInfo(lpszPrinterName, 0);
      AutoMemoryPointer autoFree (pi2);
      commandLine.AppendOption (T_("-o "),
				dryRun ? T_("nul") : pi2->pPortName);
    }
  for (vector<DVIPSOPTS::PAGERANGE>::const_iterator it =
	 dvipsOpts.pageRanges.begin();
       it != dvipsOpts.pageRanges.end();
       ++ it)
    {
      string str (NUMTOSTR(it->firstPage));
      str += ':';
      str += NUMTOSTR(it->lastPage);
      commandLine.AppendOption ("-pp ", str.c_str());
    }
  if (dvipsOpts.config.length() > 0)
    {
      commandLine.AppendOption ("-P ", dvipsOpts.config.c_str());
    }
  if (dvipsOpts.runQuietly)
    {
      commandLine.AppendOption ("-q", "1");
    }
  if (dvipsOpts.paperFormat.length() > 0)
    {
      commandLine.AppendOption ("-t ", dvipsOpts.paperFormat.c_str());
    }
  commandLine.AppendArgument (lpszDviFileName);

  trace_mtprint->WriteLine (T_("mtprint"), commandLine.Get());

  // start Dvips
  if (dvipsOpts.runAsFilter)
    {
      Process::Start (dvipsPath.Get(),
		      commandLine.Get(),
		      0,
		      0,
		      ppfileDvipsOutRd,
		      ppfileDvipsErrRd,
		      0);
    }
  else
    {
      Process::Run (dvipsPath.Get(), commandLine.Get());
    }
}

/* _________________________________________________________________________

   PrintUtility::StartGhostscript
   _________________________________________________________________________ */

void
PrintUtility::StartGhostscript (/*[in]*/ const GSOPTS &	gsOpts,
				/*[in]*/ unsigned	resolution,
				/*[in]*/ short		paperSize,
				/*[in]*/ FILE *		pfileGsIn,
				/*[out]*/ FILE **	ppfileGsOut)
{
  gsOpts;

  PathName gsPath;
  pSession->GetGhostscript (gsPath.GetBuffer(), 0);

  // make GS command line
  CommandLineBuilder commandLine;
  commandLine.AppendOption (T_("-sDEVICE="), "bmp16m");
  MIKTEX_ASSERT (ppfileGsOut != 0);
  commandLine.AppendOption (T_("-sOutputFile="), "-");
  if (resolution > 0)
    {
      commandLine.AppendOption ("-r", NUMTOSTR(resolution));
    }
  PAPERSIZEINFO paperSizeInfo;
  if (GetPaperSizeInfo(paperSize, paperSizeInfo))
    {
      commandLine.AppendOption (T_("-dDEVICEWIDTHPOINTS="),
				NUMTOSTR(paperSizeInfo.width));
      commandLine.AppendOption (T_("-dDEVICEHEIGHTPOINTS="),
				NUMTOSTR(paperSizeInfo.height));
    }
  commandLine.AppendOption ("-q");
  commandLine.AppendOption (T_("-dBATCH"));
  commandLine.AppendOption (T_("-dNOPAUSE"));
  commandLine.AppendOption (T_("-dSAFER"));
  commandLine.AppendArgument ("-");

  trace_mtprint->WriteLine (T_("mtprint"), commandLine.Get());

  // start Ghostscript
  Process::Start (gsPath.Get(),
		  commandLine.Get(),
		  pfileGsIn,
		  0,
		  ppfileGsOut,
		  0,
		  0);
}

/* _________________________________________________________________________

   PrintUtility::Spool
   _________________________________________________________________________ */

void
PrintUtility::Spool (/*[in]*/ const char *		lpszFileName,
		     /*[in]*/ PrintMethod		printMethod,
		     /*[in]*/ const DVIPSOPTS &		dvipsOpts,
		     /*[in]*/ const GSOPTS &		gsOpts,
		     /*[in]*/ const string &		printerName)
{
  // get printer resolution and paper size
  unsigned resolution;
  Printer::GetPrinterCaps (printerName.c_str(), resolution);
  trace_mtprint->WriteFormattedLine
    (T_("mtprint"),
     T_("Resolution: %u"),
     resolution);
  DEVMODE * pdm = Printer::GetDevMode(printerName.c_str());
  short paperSize = pdm->dmPaperSize;
  free (pdm);
  pdm = 0;

  if (printMethod == PrintMethodPostScript)
    {
      StartDvips (lpszFileName,
		  dvipsOpts,
		  resolution,
		  printerName.c_str(),
		  paperSize,
		  0,
		  0);
      return;
    }

  MIKTEX_ASSERT (printMethod == PrintMethodPostScriptBMP);

  FILE * pfileDvipsOutRd = 0;
  FILE * pfileDvipsErrRd = 0;

  StartDvips (lpszFileName,
	      dvipsOpts,
	      resolution,
	      printerName.c_str(),
	      paperSize,
	      &pfileDvipsOutRd,
	      &pfileDvipsErrRd);

  FileStream dvipsOut (pfileDvipsOutRd);
  FileStream dvipsErr (pfileDvipsErrRd);
  
  FILE * pfileGsRead = 0;

  StartGhostscript (gsOpts,
		    resolution,
		    paperSize,
		    pfileDvipsOutRd,
		    &pfileGsRead);

  FileStream gsOut (pfileGsRead);

  Printer::PRINTINFO pi;
  pi.printerName = printerName;
  pi.pCallback = this;
  BitmapPrinter printer (pi, dryRun);
  printer.Print (pfileGsRead);
}

/* _________________________________________________________________________

   PrintUtility::ShowVersion
   _________________________________________________________________________ */

void
PrintUtility::ShowVersion ()
{
  cout << Utils::MakeProgramVersionString(THE_NAME_OF_THE_GAME,
					   VER_FILEVERSION_STR)
       << "\n"
       << T_("Copyright (C) 2003-2007 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
       << endl;
}

/* _________________________________________________________________________

   PrintUtility::Run
   _________________________________________________________________________ */

void
PrintUtility::Run (/*[in]*/ int			argc,
		   /*[in]*/ const char **	argv)
{
  // initialize printerName with the default printer
  char szPrinterName[300];
  size_t size = 300;
  if (Utils::GetDefPrinter(szPrinterName, &size))
    {
      printerName = szPrinterName;
    }

  PrintMethod printMethod = PrintMethodPostScriptBMP;
  DVIPSOPTS dvipsOpts;
  GSOPTS gsOpts;
  
  Cpopt popt (argc, argv, aoption);

  popt.SetOtherOptionHelp (T_("[OPTION...] FILE..."));

  // process command-line options
  int option;
  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{
	case OPT_EVEN_ONLY:
	  dvipsOpts.evenPagesOnly = true;
	  break;
	case OPT_HHELP:
	  pSession->ShowManualPageAndWait (0, MIKTEXHELP_MTPRINT);
	  return;
	case OPT_ODD_ONLY:
	  dvipsOpts.oddPagesOnly = true;
	  break;
	case OPT_PAGE_RANGE:
	  {
	    DVIPSOPTS::PAGERANGE pagerange;
	    if (sscanf_s(popt.GetOptArg(),
			 "%d-%d",
			 &pagerange.firstPage,
			 &pagerange.lastPage)
		!= 2)
	      {
		FATAL_MIKTEX_ERROR ("PrintUtility::Run",
				    T_("Invalid page range."),
				    popt.GetOptArg());
	      }
	    dvipsOpts.pageRanges.push_back (pagerange);
	    break;
	  }
	case OPT_PRINTER:
	  printerName = popt.GetOptArg();
	  break;
	case OPT_PRINT_METHOD:
	  {
	    const char * lpszMethod = popt.GetOptArg();
	    if (stricmp(lpszMethod, "ps") == 0)
	      {
		printMethod = PrintMethodPostScript;
	      }
	    else if (stricmp(lpszMethod, "psbmp") == 0)
	      {
		printMethod = PrintMethodPostScriptBMP;
	      }
	    else
	      {
		FATAL_MIKTEX_ERROR ("PrintUtility::Run",
				    T_("Unknown print method."),
				    lpszMethod);
	      }
	    break;
	  }
	case OPT_PRINT_NOTHING:
	  dryRun = true;
	  break;
	case OPT_VERSION:
	  ShowVersion ();
	  return;
	}
    }

  if (option != -1)
    {
      cerr << popt.BadOption(POPT_BADOPTION_NOALIAS) << ": "
	   << popt.Strerror(option)
	   << endl;
      throw (1);
    }
      
  const char ** leftovers = popt.GetArgs();

  if (leftovers == 0)
    {
      ; // <todo>send stdin to the printer</todo>
      return;
    }

  if (printMethod != PrintMethodPostScript)
    {
      dvipsOpts.runAsFilter = true;
    }

  TraceStream::SetTraceFlags (DEFAULT_TRACE_STREAMS);

  for (; *leftovers != 0; ++ leftovers)
    {
      Spool (*leftovers,
	     printMethod,
	     dvipsOpts,
	     gsOpts,
	     printerName);
    }
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
  try
    {
      PrintUtility app;
      app.Init (argv[0]);
      app.Run (argc, argv);
      app.Finalize ();
      return (0);
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (int exitCode)
    {
      return (exitCode);
    }
}
