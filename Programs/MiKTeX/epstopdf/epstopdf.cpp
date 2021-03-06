/* epstopdf.cpp: epstopdf

   Copyright (C) 2000-2008 Christian Schenk
   Copyright (C) 1998-2001 by Sebastian Rahtz et al.

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
#include <memory>
#include <string>

#include <miktex/App/Application>
#include <miktex/Core/Core>
#include <miktex/Core/Trace>
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

#define T_(x) MIKTEXTEXT(x)
#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

const char * DEFAULT_TRACE_STREAMS =
  MIKTEX_TRACE_ERROR ","
  MIKTEX_TRACE_PROCESS ","
  PROGRAM_NAME;

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
       /*[in]*/ const char **	argv);

private:
  void
  Trace (/*[in]*/ const char *	lpszFormat,
	 /*[in]*/			...);

private:
  void
  Verbose (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  void
  PrintOnly (/*[in]*/ const char *	lpszFormat,
	     /*[in]*/				...);

private:
  void
  Warning (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  Fatal (/*[in]*/ const char *	lpszFormat,
	 /*[in]*/		...);

private:
  bool
  GetLine (/*[out]*/ string & line);

private:
  void
  PutFormattedLine (/*[in]*/ const char *	lpszFormat,
		    /*[in]*/			...);

private:
  void
  PutLine (/*[in]*/ const string &	line);

private:
  void
  CorrectBoundingBox (/*[in]*/ double llx,
		      /*[in]*/ double lly,
		      /*[in]*/ double urx,
		      /*[in]*/ double ury);

private:
  bool
  BoundingBoxWithValues (/*[in]*/ const string &	line,
			 /*[out]*/ double &		llx,
			 /*[out]*/ double &		lly,
			 /*[out]*/ double &		urx,
			 /*[out]*/ double &		ury);

private:
  bool
  BoundingBoxWithAtEnd (/*[in]*/ const string &	line);

private:
  void
  ScanHeader ();

private:
  int
  ReadDosBinary4 ();

private:
  void
  GetFirstLine (/*[out]*/ string & line);

private:
  void
  PrepareInput (/*[in]*/ bool			runAsFilter,
		/*[in]*/ const PathName &	inputFile);

private:
  void
  PrepareOutput (/*[in]*/ bool				runAsFilter,
		 /*[in]*/ bool				runGhostscript,
		 /*[in]*/ const char *			lpszGSExe,
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
  string boundingBoxName;
  
private:
  FileStream inStream;

private:
  long stopReadingAt;
  
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
  string pdfVersion;

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
    "N",
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
    "gs", 0,
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
    T_("nopdfvers"), 0,
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
    T_("print-only"), 'n',
    POPT_ARG_NONE, 0,
    OPT_PRINT_ONLY,
    T_("Print what would be done."),
    0
  },
  
  {
    T_("pdf-version"), 0,
    POPT_ARG_STRING, 0,
    OPT_PDF_VERSION,
    T_("Set the PDF version."),
    T_("PDFVER")
  },
  
  {
    T_("pdfvers"), 0,
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
EpsToPdfApp::Trace (/*[in]*/ const char *	lpszFormat,
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
EpsToPdfApp::Verbose (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
{
  if (! verbose || printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   EpsToPdfApp::PrintOnly
   _________________________________________________________________________ */

void
EpsToPdfApp::PrintOnly (/*[in]*/ const char *	lpszFormat,
			/*[in]*/		...)
{
  if (! printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   EpsToPdfApp::Warning
   _________________________________________________________________________ */

void
EpsToPdfApp::Warning (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << T_("warning: ")
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   EpsToPdfApp::Fatal
   _________________________________________________________________________ */

MIKTEXNORETURN
void
EpsToPdfApp::Fatal (/*[in]*/ const char *	lpszFormat,
		    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << PROGRAM_NAME << ": "
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   EpsToPdfApp::GetLine
   _________________________________________________________________________ */

bool
EpsToPdfApp::GetLine (/*[out]*/ string & line)
{
  if (stopReadingAt > 0)
    {
      long pos = inStream.GetPosition();
      if (pos >= stopReadingAt)
	{
	  return (false);
	}
    }
  bool done = Utils::ReadUntilDelim(line, '\n', inStream.Get());
  if (done)
    {
      string::size_type l = line.length();
      for (string::const_reverse_iterator it = line.rbegin();
	   it != line.rend();
	   ++ it)
	{
	  if (*it != '\n' && *it != '\r')
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
EpsToPdfApp::PutFormattedLine (/*[in]*/ const char *	lpszFormat,
			       /*[in]*/			...)
{
  if (! printOnly)
    {
      va_list marker;
      va_start (marker, lpszFormat);
      vfprintf (outStream.Get(), lpszFormat, marker);
      va_end (marker);
      fputc ('\n', outStream.Get());
    }
}

/* _________________________________________________________________________

   EpsToPdfApp::PutLine
   _________________________________________________________________________ */

void
EpsToPdfApp::PutLine (/*[in]*/ const string &	line)
{
  if (! printOnly)
    {
      fprintf (outStream.Get(), "%s\n", line.c_str());
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
EpsToPdfApp::BoundingBoxWithValues (/*[in]*/ const string &	line,
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
#  define sscanf_s sscanf
#endif
  if (sscanf_s(line.c_str() + boundingBoxName.length(),
	       " %lf %lf %lf %lf",
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
EpsToPdfApp::BoundingBoxWithAtEnd (/*[in]*/ const string &	line)
{
  if (line.compare(0, boundingBoxName.length(), boundingBoxName) != 0)
    {
      return (false);
    }
  const char * lpsz = line.c_str() + boundingBoxName.length();
  while (isspace(*lpsz))
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
  string line;

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

   EpsToPdfApp::ReadDosBinary4
   _________________________________________________________________________ */

int
EpsToPdfApp::ReadDosBinary4 ()
{
  unsigned char buf[4];
  if (inStream.Read(buf, 4) != 4)
    {
      Fatal (T_("Not a valid EPS file."));
    }
  return (((static_cast<int>(buf[3]) & 0xff) << 24)
	  | ((static_cast<int>(buf[2]) & 0xff) << 16)
	  | ((static_cast<int>(buf[1]) & 0xff) << 8)
	  | ((static_cast<int>(buf[0]) & 0xff)));
}

/* _________________________________________________________________________

   EpsToPdfApp::GetFirstLine
   _________________________________________________________________________ */

void
EpsToPdfApp::GetFirstLine (/*[out]*/ string & line)
{
  unsigned char buf[4];
  if (inStream.Read(buf, 4) != 4)
    {
      Fatal (T_("Not a valid EPS file."));
    }
  if (buf[0] == '%' && buf[1] == '!' && buf[2] == 'P' && buf[3] == 'S')
    {
      line = "%!PS";
      string line1;
      stopReadingAt = 0;
      if (GetLine(line1))
	{
	  line += line1;
	}
    }
  else if (buf[0] != 0xc5 || buf[1] != 0xd0
	   || buf[2] != 0xd3 || buf[3] != 0xc6)
    
    {
      Fatal (T_("Invalid binary DOS header."));
    }
  else
    {
      int startPS = ReadDosBinary4();
      int lengthPS = ReadDosBinary4();
      stopReadingAt = startPS + lengthPS;
      inStream.Seek (startPS, SeekOrigin::Begin);
      if (! GetLine(line))
	{
	  Fatal (T_("Not a valid EPS file."));
	}
    }
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
			    /*[in]*/ const char *		lpszGSExe,
			    /*[in]*/ const CommandLineBuilder & gsOptions,
			    /*[in]*/ const PathName &		outFile)
{
  if (runGhostscript)
    {
      CommandLineBuilder cmdLine (gsOptions);
      cmdLine.AppendOption ("-q");
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
	  cmdLine.AppendOption (T_("-sOutputFile="), "-");
	}
      else
	{
	  cmdLine.AppendOption (T_("-sOutputFile="), outFile);
	}
      cmdLine.AppendOption ("-");
      cmdLine.AppendOption ("-c");
      cmdLine.AppendArgument (T_("quit"));
      PrintOnly ("%s %s\n", Q_(lpszGSExe), cmdLine.Get());
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
		  /*[in]*/ const char **	argv)
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
      const char * lpszOptArg = popt.GetOptArg();
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
	  pdfVersion = "";
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
	  cout << Utils::MakeProgramVersionString(THE_NAME_OF_THE_GAME,
						  VersionNumber(MIKTEX_MAJOR_VERSION,
								MIKTEX_MINOR_VERSION,
								MIKTEX_COMP_J2000_VERSION,
								0))
	       << T_("\n\
Copyright (C) 2000-2008 Christian Schenk\n\
Copyright (C) 1998-2001 by Sebastian Rahtz et al.\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	       << endl;
	  return;
	}
    }

  if (option != -1)
    {
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += ": ";
      msg += popt.Strerror(option);
      Fatal ("%s", msg.c_str());
    }

  Init (initInfo);

  PathName inputFile;

  int argCount = popt.GetArgCount();
  const char ** leftovers = popt.GetArgs();

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
      if (! pSession->FindFile(leftovers[0], ".", inputFile))
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
      gsOptions.AppendOption (T_("-dTextAlphaBits="), "4");
      gsOptions.AppendOption (T_("-dGraphicsAlphaBits="), "4");
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
	  outFile.SetExtension (".pdf");
	}
      else
	{
	  outFile = inputFile;
	  outFile.SetExtension (0);
	  outFile.Append ("2", false);
	  outFile.SetExtension (".eps");
	}
    }

  if (! runAsFilter)
    {
      Verbose (T_("Making %s from %s..."), Q_(outFile), Q_(inputFile));
    }

  char szGSExe[BufferSizes::MaxPath];

  if (runGhostscript)
    {
      pSession->GetGhostscript (szGSExe, 0);
    }

  PrepareInput (runAsFilter, inputFile);
  PrepareOutput (runAsFilter, runGhostscript, szGSExe, gsOptions, outFile);

  string line;

  GetFirstLine (line);

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
      /*[in]*/ const char **	argv)
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
