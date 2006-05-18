/* epstopdf.cpp: epstopdf

   Copyright (C) 1998-2001 by Sebastian Rahtz et al.
   Copyright (C) 2000-2006 Christian Schenk

   This file is part of EPStoPDF.

   EPStoPDF is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   EPStoPDF is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with EPStoPDF; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

/* This program reimplements the epstopdf Perl script written by
   Sebastian Rahtz. */

#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <string>

#include <miktex/app.h>
#include <miktex/core.h>
#include <miktex/trace.h>
#include <popt-miktex.h>

#include "epstopdf-version.h"

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX;
using namespace std;

#define PROGRAM_NAME T_("epstopdf")

#if ! defined(THE_NAME_OF_THE_GAME)
#  define THE_NAME_OF_THE_GAME T_("MiKTeX EPS-to-PDF Converter")
#endif

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

#define T_(x) MIKTEXTEXT(x)
#define Q_(x) Quoted(x).c_str()

const MIKTEXCHAR * DEFAULT_TRACE_STREAMS =
  MIKTEX_TRACE_ERROR T_(",")
  MIKTEX_TRACE_PROCESS T_(",")
  PROGRAM_NAME;

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  bool needQuotes = (_tcschr(lpsz, T_(' ')) != 0);
  tstring result;
  if (needQuotes)
    {
      result += T_('"');
    }
  result += lpsz;
  if (needQuotes)
    {
      result += T_('"');
    }
  return (result);
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const tstring & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const PathName & path)
{
  return (Quoted(path.Get()));
}

/* _________________________________________________________________________

   EpsToPdfApp
   _________________________________________________________________________ */

class EpsToPdfApp : public Application
{
public:
  EpsToPdfApp ()
    : exactBoundingBox (false),
      runAsFilter (false),
      runGhostscript (true),
      hiResBoundingBox (false),
      boundingBoxCorrected (false),
      traceStream (TraceStream::Open(PROGRAM_NAME)),
      verbose (false),
      printOnly (false),
      enlarge (0)
  {
  }

public:
  void
  Run (/*[in]*/ int			argc,
       /*[in]*/ const MIKTEXCHAR **	argv);

private:
  void
  Trace (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	 /*[in]*/			...);

private:
  void
  Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	   /*[in]*/			...);

private:
  void
  PrintOnly (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	     /*[in]*/				...);

private:
  void
  Warning (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	   /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  Fatal (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	 /*[in]*/			...);

private:
  bool
  GetLine (/*[out]*/ tstring & line);

private:
  void
  PutFormattedLine (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		    /*[in]*/			...);

private:
  void
  PutLine (/*[in]*/ const tstring &	line);

private:
  void
  CorrectBoundingBox (/*[in]*/ double llx,
		      /*[in]*/ double lly,
		      /*[in]*/ double urx,
		      /*[in]*/ double ury);

private:
  bool
  BoundingBoxWithValues (/*[in]*/ const tstring &	line,
			 /*[out]*/ double &		llx,
			 /*[out]*/ double &		lly,
			 /*[out]*/ double &		urx,
			 /*[out]*/ double &		ury);

private:
  bool
  BoundingBoxWithAtEnd (/*[in]*/ const tstring &	line);

private:
  void
  ScanHeader ();

private:
  void
  ScanFirstLine (/*[out]*/ tstring & line);

private:
  void
  PrepareInput (/*[in]*/ bool			runAsFilter,
		/*[in]*/ const PathName &	inputFile);

private:
  void
  PrepareOutput (/*[in]*/ bool				runAsFilter,
		 /*[in]*/ bool				runGhostscript,
		 /*[in]*/ const MIKTEXCHAR *		lpszGSExe,
		 /*[in]*/ const CommandLineBuilder &	gsOptions,
		 /*[in]*/ const PathName &		outFile);

private:
  auto_ptr<Process> gsProcess;

private:
  bool exactBoundingBox;
  
private:
  bool runAsFilter;
  
private:
  bool runGhostscript;
  
private:
  bool hiResBoundingBox;
  
private:
  tstring boundingBoxName;
  
private:
  FileStream inStream;
  
private:
  FileStream outStream;
  
private:
  bool boundingBoxCorrected;

private:
  auto_ptr<TraceStream> traceStream;

private:
  bool verbose;

private:
  bool printOnly;

private:
  double enlarge;

private:
  tstring pdfVersion;

private:
  static struct poptOption aoption[];
};

/* _________________________________________________________________________

   EpsToPdfApp::aoption
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1000,
  OPT_ANTIALIASING,
  OPT_COMPRESS,
  OPT_DEBUG,
  OPT_ENLARGE,
  OPT_EXACT,
  OPT_FILTER,
  OPT_GS,
  OPT_GSOPT,
  OPT_HIRES,
  OPT_NOANTIALIASING,
  OPT_NOCOMPRESS,
  OPT_NODEBUG,
  OPT_NOEXACT,
  OPT_NOFILTER,
  OPT_NOGS,
  OPT_NOHIRES,
  OPT_NOPDFVERS,
  OPT_OUTFILE,
  OPT_PDF_VERSION,
  OPT_PRINT_ONLY,
  OPT_TRACE,
  OPT_VERBOSE,
  OPT_VERSION
};

struct poptOption EpsToPdfApp::aoption[] = {

  {
    T_("antialias"), 0,
    POPT_ARG_NONE, 0,
    OPT_ANTIALIASING,
    T_("Enable anti-aliasing of bitmaps."),
    0,
  },

  {
    T_("compress"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMPRESS,
    T_("Enable PDF compression."),
    0,
  },

  {
    T_("debug"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_DEBUG,
    T_("Write trace messages."),
    0,
  },

  {
    T_("enlarge"), 0,
    POPT_ARG_STRING, 0,
    OPT_ENLARGE,
    T_("Enlarge bounding box by N PostScript points."),
    T_("N"),
  },

  {
    T_("exact"), 0,
    POPT_ARG_NONE, 0,
    OPT_EXACT,
    T_("Scan the EPS file for %%ExactBoundingBox."),
    0,
  },

  {
    T_("filter"), 0,
    POPT_ARG_NONE, 0,
    OPT_FILTER,
    T_("Run as a filter, i.e., read the EPS file from the \
standard input stream and write the output file to the standard output \
stream."),
    0,
  },

  {
    T_("gs"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_GS,
    T_("Run Ghostscript."),
    0,
  },

  {
    T_("gsopt"), 0,
    POPT_ARG_STRING, 0,
    OPT_GSOPT,
    T_("Add OPTIONS to the Ghostscript command-line."),
    T_("OPTIONS"),
  },

  {
    T_("hires"), 0,
    POPT_ARG_NONE, 0,
    OPT_HIRES,
    T_("Scan the EPS file for %%HiResBoundingBox."),
    0,
  },
  
  {
    T_("noantialias"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_NOANTIALIASING,
    T_("Disable anti-aliasing of bitmaps."),
    0,
  },

  {
    T_("nocompress"), 0,
    POPT_ARG_NONE, 0,
    OPT_NOCOMPRESS,
    T_("Disable PDF compression."),
    0,
  },

  {
    T_("nodebug"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_NODEBUG,
    T_("Do not print debug information."),
    0,
  },

  {
    T_("noexact"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_NOEXACT,
    T_("Do not scan the EPS file for %%ExactBoundingBox."),
    0,
  },

  {
    T_("nofilter"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_NOFILTER,
    T_("Do not run as a filter."),
    0,
  },

  {
    T_("nogs"), 0,
    POPT_ARG_NONE, 0,
    OPT_NOGS,
    T_("Correct the bounding box but do not run Ghostscript."),
    0,
  },

  {
    T_("nohires"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_NOHIRES,
    T_("Do not scan the EPS file for %%HiResBoundingBox."),
    0,
  },

  {
    _T("nopdfvers"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_NOPDFVERS,
    T_("Do not set the PDF version."),
    0
  },
  
  {
    T_("outfile"), 0,
    POPT_ARG_STRING, 0,
    OPT_OUTFILE,
    T_("Write the result to FILE."),
    T_("FILE"),
  },

  {
    _T("print-only"), T_('n'),
    POPT_ARG_NONE, 0,
    OPT_PRINT_ONLY,
    T_("Print what would be done."),
    0
  },
  
  {
    _T("pdf-version"), 0,
    POPT_ARG_STRING, 0,
    OPT_PDF_VERSION,
    T_("Set the PDF version."),
    T_("PDFVER")
  },
  
  {
    _T("pdfvers"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_PDF_VERSION,
    T_("Set the PDF version."),
    T_("PDFVER")
  },
  
  {
    T_("trace"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_TRACE,
    T_("\
Turn on tracing.\
  TRACESTREAMS, if specified, is a comma-separated list of trace stream names\
 (see the MiKTeX manual)."),
    T_("TRACESTREAMS"),
  },

  {
    T_("verbose"), 0, 
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Turn on verbose output mode."), 0
  },

  {
    T_("version"), 0,
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Show version information and exit."),
    0,
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   EpsToPdfApp::Trace
   _________________________________________________________________________ */

void
EpsToPdfApp::Trace (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  traceStream->WriteLine (PROGRAM_NAME,
			  Utils::FormatString(lpszFormat, arglist).c_str());
  va_end (arglist);
}

/* _________________________________________________________________________

   EpsToPdfApp::Verbose
   _________________________________________________________________________ */

void
EpsToPdfApp::Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		      /*[in]*/				...)
{
  if (! verbose || printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcout << Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   EpsToPdfApp::PrintOnly
   _________________________________________________________________________ */

void
EpsToPdfApp::PrintOnly (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
			/*[in]*/			...)
{
  if (! printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcout << Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   EpsToPdfApp::Warning
   _________________________________________________________________________ */

void
EpsToPdfApp::Warning (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		      /*[in]*/				...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << T_("warning: ")
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   EpsToPdfApp::Fatal
   _________________________________________________________________________ */

MIKTEXNORETURN
void
EpsToPdfApp::Fatal (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << PROGRAM_NAME << T_(": ")
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   EpsToPdfApp::GetLine
   _________________________________________________________________________ */

bool
EpsToPdfApp::GetLine (/*[out]*/ tstring & line)
{
  bool done = Utils::ReadUntilDelim(line, T_('\n'), inStream.Get());
  if (done)
    {
      tstring::size_type l = line.length();
      for (tstring::const_reverse_iterator it = line.rbegin();
	   it != line.rend();
	   ++ it)
	{
	  if (*it != T_('\n') && *it != T_('\r'))
	    {
	      break;
	    }
	  -- l;
	}
      line.erase (l);
    }
  return (done);
}


/* _________________________________________________________________________

   EpsToPdfApp::PutFormattedLine
   _________________________________________________________________________ */

void
EpsToPdfApp::PutFormattedLine (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
			       /*[in]*/				...)
{
  if (! printOnly)
    {
      va_list marker;
      va_start (marker, lpszFormat);
      _vftprintf (outStream.Get(), lpszFormat, marker);
      va_end (marker);
      _fputtc (T_('\n'), outStream.Get());
    }
}

/* _________________________________________________________________________

   EpsToPdfApp::PutLine
   _________________________________________________________________________ */

void
EpsToPdfApp::PutLine (/*[in]*/ const tstring &	line)
{
  if (! printOnly)
    {
      _ftprintf (outStream.Get(), T_("%s\n"), line.c_str());
    }
}

/* _________________________________________________________________________

   EpsToPdfApp::CorrectBoundingBox
   _________________________________________________________________________ */

void
EpsToPdfApp::CorrectBoundingBox (/*[in]*/ double llx,
				 /*[in]*/ double lly,
				 /*[in]*/ double urx,
				 /*[in]*/ double ury)
{
  Trace (T_("Old BoundingBox: %f %f %f %f"), llx, lly, urx, ury);
  llx -= enlarge;
  lly -= enlarge;
  urx += enlarge;
  ury += enlarge;
  int width = static_cast<int>(ceil(urx - llx));
  int height = static_cast<int>(ceil(ury - lly));
  int xoffset = static_cast<int>(- llx);
  int yoffset = static_cast<int>(- lly);
  Trace (T_("New BoundingBox: 0 0 %d %d"), width, height);
  Trace (T_("Offset: %d %d"), xoffset, yoffset);
  PutFormattedLine (T_("%%%%BoundingBox: 0 0 %d %d"), width, height);
  PutFormattedLine (T_("<< /PageSize [%d %d] >> setpagedevice"),
		    width,
		    height);
  PutFormattedLine (T_("gsave %d %d translate"), xoffset, yoffset);
}

/* _________________________________________________________________________

   EpsToPdfApp::BoundingBoxWithValues
   _________________________________________________________________________ */

bool
EpsToPdfApp::BoundingBoxWithValues (/*[in]*/ const tstring &	line,
				    /*[out]*/ double &		llx,
				    /*[out]*/ double &		lly,
				    /*[out]*/ double &		urx,
				    /*[out]*/ double &		ury)
{
  if (line.compare(0,  boundingBoxName.length(), boundingBoxName) != 0)
    {
      return (false);
    }
#if _MSC_VER < 1400
#  define _stscanf_s _stscanf
#endif
  if (_stscanf_s(line.c_str() + boundingBoxName.length(),
		 T_(" %lf %lf %lf %lf"),
		 &llx,
		 &lly,
		 &urx,
		 &ury)
      != 4)
    {
      return (false);
    }
  return (true);
}

/* _________________________________________________________________________

   EpsToPdfApp::BoundingBoxWithAtEnd
   _________________________________________________________________________ */

bool
EpsToPdfApp::BoundingBoxWithAtEnd (/*[in]*/ const tstring &	line)
{
  if (line.compare(0, boundingBoxName.length(), boundingBoxName) != 0)
    {
      return (false);
    }
  const MIKTEXCHAR * lpsz = line.c_str() + boundingBoxName.length();
  while (_istspace(*lpsz))
    {
      ++ lpsz;
    }
  return (StringCompare(lpsz, T_("(atend)")) == 0);
}

/* _________________________________________________________________________

   EpsToPdfApp::ScanHeader
   _________________________________________________________________________ */

void
EpsToPdfApp::ScanHeader ()
{
  tstring line;

  while (GetLine(line))
    {
      if (line.compare(0, 13, T_("%%EndComments")) == 0)
	{
	  PutLine (line);
	  break;
	}

      double llx, lly, urx, ury;

      if (BoundingBoxWithValues(line, llx, lly, urx, ury))
	{
	  CorrectBoundingBox (llx, lly, urx, ury);
	  boundingBoxCorrected = true;
	  break;
	}
      
      if (BoundingBoxWithAtEnd(line))
	{
	  Trace (T_("%s (atend)"), boundingBoxName.c_str());
	  if (runAsFilter)
	    {
	      Warning (T_("\
Cannot look for BoundingBox in the trailer with option --filter."));
	      break;
	    }
	  long pos = inStream.GetPosition();
	  Trace (T_("Current file position: %d"), pos);
	  while (GetLine(line))
	    {
	      if (line.compare(0, 15, T_("%%BeginDocument")) == 0)
		{
		  while (GetLine(line))
		    {
		      if (line.compare(0, 13, T_("%%EndDocument")) == 0)
			{
			  break;
			}
		    }
		}
	      if (BoundingBoxWithValues(line, llx, lly, urx, ury))
		{
		  CorrectBoundingBox (llx, lly, urx, ury);
		  boundingBoxCorrected = true;
		  break;
		}
	    }
	  inStream.Seek (pos, SeekOrigin::Begin);
	  break;
	}

      PutLine (line);
    }
}

/* _________________________________________________________________________

   EpsToPdfApp::ScanFirstLine
   _________________________________________________________________________ */

struct DosEpsBinaryFileHeader
{
  size_t startPS;
  size_t lengthPS;
  size_t startMF;
  size_t lengthMF;
  size_t startTIFF;
  size_t lengthTIFF;
};

void
EpsToPdfApp::ScanFirstLine (/*[out]*/ tstring & line)
{
  const MIKTEXCHAR * lpsz = strstr(line.c_str(), T_("%!"));
  if (lpsz == 0)
    {
      Fatal (T_("Not a valid EPS file."));
    }
  tstring line1 = lpsz;
  line = line1;
}

/* _________________________________________________________________________

   EpsToPdfApp::PrepareInput
   _________________________________________________________________________ */

void
EpsToPdfApp::PrepareInput (/*[in]*/ bool		runAsFilter,
			   /*[in]*/ const PathName &	inputFile)
{
  if (runAsFilter)
    {
      if (stdin == 0)
	{
	  Fatal (T_("No standard input stream."));
	}
      inStream.Attach (stdin);
    }
  else
    {
      inStream.Attach (File::Open(inputFile,
				  FileMode::Open,
				  FileAccess::Read));
    }
}

/* _________________________________________________________________________

   EpsToPdfApp::PrepareOutput
   _________________________________________________________________________ */

void
EpsToPdfApp::PrepareOutput (/*[in]*/ bool			runAsFilter,
			    /*[in]*/ bool			runGhostscript,
			    /*[in]*/ const MIKTEXCHAR *		lpszGSExe,
			    /*[in]*/ const CommandLineBuilder & gsOptions,
			    /*[in]*/ const PathName &		outFile)
{
  if (runGhostscript)
    {
      CommandLineBuilder cmdLine (gsOptions);
      cmdLine.AppendOption (T_("-q"));
      cmdLine.AppendOption (T_("-sDEVICE="), T_("pdfwrite"));
      cmdLine.AppendOption (T_("-dSAFER"));
#if 1				// 642845
      cmdLine.AppendOption (T_("-dAutoRotatePages="), T_("/None"));
#endif
      if (! pdfVersion.empty())
	{
	  cmdLine.AppendOption (T_("-dCompatibilityLevel="), pdfVersion);
	}
      if (runAsFilter)
	{
	  cmdLine.AppendOption (T_("-sOutputFile="), T_("-"));
	}
      else
	{
	  cmdLine.AppendOption (T_("-sOutputFile="), outFile);
	}
      cmdLine.AppendOption (T_("-"));
      cmdLine.AppendOption (T_("-c"));
      cmdLine.AppendArgument (T_("quit"));
      PrintOnly (T_("%s %s\n"), Q_(lpszGSExe), cmdLine.Get());
      if (! printOnly)
	{
	  ProcessStartInfo processStartInfo;
	  processStartInfo.FileName = lpszGSExe;
	  processStartInfo.Arguments = cmdLine.Get();
	  processStartInfo.StandardInput = 0;
	  processStartInfo.RedirectStandardError = false;
	  processStartInfo.RedirectStandardInput = true;
	  processStartInfo.RedirectStandardOutput = false;
	  gsProcess = auto_ptr<Process>(Process::Start(processStartInfo));
	  outStream.Attach (gsProcess->get_StandardInput());
	}
    }
  else if (runAsFilter)
    {
      if (stdout == 0)
	{
	  Fatal (T_("No standard output stream."));
	}
      outStream.Attach (stdout);
      outStream.SetBinary ();
    }
  else
    {
      if (! printOnly)
	{
	  outStream.Attach (File::Open(outFile,
				       FileMode::Create,
				       FileAccess::Write,
				       false));
	}
    }
}

/* _________________________________________________________________________

   EpsToPdfApp::Run
   _________________________________________________________________________ */

void
EpsToPdfApp::Run (/*[in]*/ int			argc,
		  /*[in]*/ const MIKTEXCHAR **	argv)
{
  PathName outFile;
  
  Cpopt popt (argc, argv, aoption);

  int option;

  bool antiAliasing = false;
  bool doCompress = true;

  Session::InitInfo initInfo (argv[0]);

  CommandLineBuilder gsOptions;

  while ((option = popt.GetNextOpt()) >= 0)
    {
      const MIKTEXCHAR * lpszOptArg = popt.GetOptArg();
      switch (option)
	{
	case OPT_ANTIALIASING:
	  antiAliasing = true;
	  break;
	case OPT_COMPRESS:
	  doCompress = true;
	  break;
	case OPT_DEBUG:
	  initInfo.SetTraceFlags (DEFAULT_TRACE_STREAMS);
	  break;
	case OPT_ENLARGE:
	  enlarge = atoi(lpszOptArg);
	  break;
	case OPT_EXACT:
	  exactBoundingBox = true;
	  break;
	case OPT_FILTER:
	  runAsFilter = true;
	  break;
	case OPT_GS:
	  runGhostscript = true;
	  break;
	case OPT_GSOPT:
	  gsOptions.AppendUnquoted (lpszOptArg);
	  break;
	case OPT_HIRES:
	  hiResBoundingBox = true;
	  break;
	case OPT_NOANTIALIASING:
	  antiAliasing = false;
	  break;
	case OPT_NOCOMPRESS:
	  doCompress = false;
	  break;
	case OPT_NODEBUG:
	  // unsupported option
	  break;
	case OPT_NOEXACT:
	  exactBoundingBox = false;
	  break;
	case OPT_NOFILTER:
	  runAsFilter = false;
	  break;
	case OPT_NOGS:
	  runGhostscript = false;
	  break;
	case OPT_NOHIRES:
	  hiResBoundingBox = false;
	  break;
	case OPT_NOPDFVERS:
	  pdfVersion = T_("");
	  break;
	case OPT_OUTFILE:
	  outFile = lpszOptArg;
	  break;
	case OPT_PDF_VERSION:
	  pdfVersion = lpszOptArg;
	  break;
	case OPT_PRINT_ONLY:
	  printOnly = true;
	  break;
	case OPT_TRACE:
	  if (lpszOptArg == 0)
	    {
	      initInfo.SetTraceFlags (DEFAULT_TRACE_STREAMS);
	    }
	  else
	    {
	      initInfo.SetTraceFlags (lpszOptArg);
	    }
	  break;
	case OPT_VERBOSE:
	  verbose = true;
	  break;
	case OPT_VERSION:
	  tcout << Utils::MakeProgramVersionString(THE_NAME_OF_THE_GAME,
					       VersionNumber(VER_FILEVERSION))
		<< T_("\n\
Copyright (C) 1998-2001 by Sebastian Rahtz et al.\n\
Copyright (C) 2000-2006 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
		<< endl;
	  return;
	}
    }

  if (option != -1)
    {
      tstring msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += T_(": ");
      msg += popt.Strerror(option);
      Fatal (T_("%s"), msg.c_str());
    }

  Init (initInfo);

  PathName inputFile;

  int argCount = popt.GetArgCount();
  const MIKTEXCHAR ** leftovers = popt.GetArgs();

  if (runAsFilter)
    {
      if (argCount > 0)
	{
	  Fatal (T_("\
Input file cannot be specified together with --filter option."));
	}
      Trace (T_("Input file: standard input"));
    }
  else
    {
      if (argCount == 0 || leftovers == 0 || *leftovers == 0)
	{
	  Fatal (T_("Missing input file."));
	}
      else if (argCount != 1)
	{
	  Fatal (T_("Too many input files."));
	}
      if (! pSession->FindFile(leftovers[0], T_("."), inputFile))
	{
	  Fatal (T_("The input file does not exist."));
	}
      Trace (T_("Input filename: %s"), inputFile.Get());
    }

  if (runAsFilter && verbose)
    {
      Fatal (T_("Options --filter and --verbose cannot be used together."));
    }

  if (antiAliasing)
    {
      gsOptions.AppendOption (T_("-dTextAlphaBits="), T_("4"));
      gsOptions.AppendOption (T_("-dGraphicsAlphaBits="), T_("4"));
    }

  if (! doCompress)
    {
      gsOptions.AppendOption (T_("-dUseFlateCompression="), T_("false"));
    }

  boundingBoxName = T_("%%BoundingBox:");
  if (hiResBoundingBox && exactBoundingBox)
    {
      Fatal (T_("Options --hires and --exact cannot be used together."));
    }
  if (hiResBoundingBox)
    {
      boundingBoxName = T_("%%HiResBoundingBox:");
    }
  if (exactBoundingBox)
    {
      boundingBoxName = T_("%%ExactBoundingBox:");
    }

  if (outFile.GetLength() > 0 && runAsFilter)
    {
      Fatal (T_("\
Input file cannot be specified together with --filter option."));
    }

  if (outFile.GetLength() == 0)
    {
      if (runGhostscript)
	{
	  outFile = inputFile;
	  outFile.SetExtension (T_(".pdf"));
	}
      else
	{
	  outFile = inputFile;
	  outFile.SetExtension (0);
	  outFile.Append (T_("2"), false);
	  outFile.SetExtension (T_(".eps"));
	}
    }

  if (! runAsFilter)
    {
      Verbose (T_("Making %s from %s..."), Q_(outFile), Q_(inputFile));
    }

  MIKTEXCHAR szGSExe[BufferSizes::MaxPath];

  if (runGhostscript)
    {
      pSession->GetGhostscript (szGSExe, 0);
    }

  PrepareInput (runAsFilter, inputFile);
  PrepareOutput (runAsFilter, runGhostscript, szGSExe, gsOptions, outFile);

  tstring line;

  GetLine (line);

  ScanFirstLine (line);

  PutLine (line);
  
  ScanHeader ();

  while (GetLine(line))
    {
      PutLine (line);
    }

  if (boundingBoxCorrected)
    {
      PutLine (T_("\ngrestore"));
    }

  outStream.Close ();

  if (! boundingBoxCorrected)
    {
      Fatal (T_("BoundingBox not found."));
    }

  if (gsProcess.get() != 0)
    {
      gsProcess->WaitForExit ();
      gsProcess.reset ();
    }

  Finalize ();
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR **	argv)
{
  try
    {
      EpsToPdfApp app;
      app.Run (argc, argv);
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
