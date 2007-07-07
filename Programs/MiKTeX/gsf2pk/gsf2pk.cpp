/* gsf2pk.cpp: Gsf-to-Pk converter (based on gsftopk)

   Copyright (C) 1993-2000  Paul Vojta
   Copyright (C) 2004-2007 Christian Schenk
   
   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT.  IN NO EVENT SHALL PAUL VOJTA BE LIABLE FOR ANY
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include <cstdio>

#if defined(_MSC_VER)
#  pragma warning (disable: 4702)
#endif

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <miktex/app.h>
#include <miktex/core.h>

#include <popt-miktex.h>

#include "gsf2pk-version.h"

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX;
using namespace std;

#if defined(_MSC_VER)
#  if (_MSC_VER < 1400)
#    define SScanF _stscanf
#  else
#    define SScanF _stscanf_s
#  endif
#else
#  define SScanF sscanf
#endif

#define T_(x) MIKTEXTEXT(x)

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

#define Q_(x) Quoted(x).c_str()

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

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

tstring
Quoted (/*[in]*/ const tstring & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

tstring
Quoted (/*[in]*/ const PathName & path)
{
  return (Quoted(path.Get()));
}

/* _________________________________________________________________________

   Error Macros
   _________________________________________________________________________ */

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,		\
			  lpszInfo,			\
			  T_(__FILE__),			\
			  __LINE__)

/* _________________________________________________________________________

   Converter
   _________________________________________________________________________ */


class Converter
  : public Application
{
public:
  Converter ();

public:
  ~Converter ();

public:
  void
  Main (/*[in]*/ int			argc,
	/*[in]*/ const MIKTEXCHAR * *	argv);

private:
  MIKTEXNORETURN
  void
  Error (/*[in]*/ const MIKTEXCHAR *		lpszFormat,
	 /*[in]*/				...)
    const;

private:
  void
  Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	   /*[in]*/			...);

private:
  int
  GetByte (/*[in]*/ FILE * pfile)
    const;

private:
  int
  GetByte ()
    const
  {
    return (GetByte(pFileGsf.Get()));
  }

private:
  int
  GetDword (/*[in]*/ FILE * pfile)
    const;

private:
  int
  GetDword ()
    const
  {
    return (GetDword(pFileGsf.Get()));
  }

private:
  void
  Read (/*[out]*/ void *  pv,
	/*[in]*/ size_t   len);

private:
  void
  Expect (/*[in]*/ const char *	lpszWaitingFor,
	  /*[out]*/ string *	pLine = 0);

private:
  int
  GetInt ();

private:
  void
  WhiteSpace ();

private:
  void PutByte (/*[in]*/ int	by)
    const;

private:
  void
  PutDword (/*[in]*/ int	dword)
    const;

private:
  void
  PutByteArray (/*[in]*/ const void *	pv,
		/*[in]*/ size_t		len)
    const;
    
private:
  int
  GetFirstByte (/*[in]*/ const PathName & file);

private:
  void
  ReadTFMFile (/*[in]*/ const MIKTEXCHAR * lpszTeXFontName);
  
private:
  void
  tallyup (/*[in]*/ int n);

private:
  void
  pk_put_nyb (/*[in]*/ int n);

private:
  void
  pk_put_long (/*[in]*/ int n);

private:
  void
  pk_put_count (/*[in]*/ int n);

private:
  void
  trim_bitmap ();

private:
  bool
  pk_rll_cvt ();

private:
  void
  pk_bm_cvt ();

private:
  void
  putshort (/*[in]*/ int w);

private:
  void
  putmed (/*[in]*/ long w);

private:
  void
  putlong (/*[in]*/ long w);

private:
  void
  putspecl (/*[in]*/ const MIKTEXCHAR * str1,
	    /*[in]*/ const MIKTEXCHAR * str2 = 0);

private:
  void
  Write (/*[out]*/ const void * pv,
  	 /*[in]*/ size_t	len);

private:
  void
  PutGlyph (/*[in]*/ int cc);

private:
  void
  WritePkFile (/*[in]*/ const MIKTEXCHAR *	lpszPkFile);

private:
  Process *
  StartGhostscript (/*[in]*/ const MIKTEXCHAR *	lpszFontFile,
		    /*[in]*/ const MIKTEXCHAR *	lpszEncFile,
		    /*[in]*/ const MIKTEXCHAR *	lpszFontName,
		    /*[in]*/ const MIKTEXCHAR *	lpszSpecInfo,
		    /*[in]*/ const MIKTEXCHAR *	lpszDPI,
		    /*[out]*/ FILE **		ppGsOut,
		    /*[out]*/ FILE **		ppGsErr);

private:
  static
  void
  MIKTEXCALLBACK
  StderrReader (/*[in]*/ void * pArg);

private:
  void
  Convert (/*[in]*/ const MIKTEXCHAR *	lpszTeXFontName,
	   /*[in]*/ const MIKTEXCHAR *	lpszFontName,
	   /*[in]*/ const MIKTEXCHAR *	lpszSpecInfo,
	   /*[in]*/ const MIKTEXCHAR *	lpszEncFile,
	   /*[in]*/ const MIKTEXCHAR *	lpszFontFile,
	   /*[in]*/ const MIKTEXCHAR *	lpszDPI,
	   /*[in]*/ const MIKTEXCHAR *	lpszPkFile);

public:
  void
  ShowGhostscriptTranscript ()
    const;

private:
  bool quiet;

private:
  bool verbose;

private:
  AutoFILE pFilePk;

private:
  AutoFILE pFileGsf;

private:
  AutoFILE pFileGsErr;

private:
  tstring gsStdErr;

private:
  vector<int> lengths;

private:
  vector<int> widths;

private:
  vector<int> widthIndex;

private:
  tstring chars;

private:
  int checkSum;

private:
  int designSize;

private:
  int dpi;

private:
  int lh() const { return (lengths[1]); }
  int bc() const { return (lengths[2]); }
  int ec() const { return (lengths[3]); }
  int nw() const { return (lengths[4]); }
  
private:
  bool haveFirstLine;

private:
  int llx, lly, urx, ury;

private:
  float charWidth;

private:
  typedef unsigned char Byte;

private:
  vector<Byte> area1;

private:
  Byte * bitmap;
  Byte * bitmap_end;

  int pk_len;

private:
  int idx;

private:
  int hoff, voff;

private:
  int width;

private:
  int height;

private:
  int bytesWide;

private:
  size_t bitmapSize;

private:
  tstring fontName;

  // area for saving bit counts
private:
  vector<int> counts;

private:
  int skip;

private:
  Byte part;

private:
  Byte flag;

private:
  int pk_dyn_f;

private:
  int pk_dyn_g;

  // cost of this character if pk_dyn_f = 0
private:
  int base;

  // cost of increasing pk_dyn_f from i to i+1
private:
  int deltas[13];

private:
  static bool odd;

private:
  static Byte masks[];

private:
  static const struct poptOption aoption[];
};

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1,
  OPT_QUIET,
  OPT_VERBOSE,
  OPT_VERSION,
};

/* _________________________________________________________________________

   Converter::aoption
   _________________________________________________________________________ */

const struct poptOption Converter::aoption[] = {

  {
    T_("quiet"), 0, POPT_ARG_NONE, 0, OPT_QUIET,
    T_("Suppress all output (except errors)."), 0
  },

  {
    T_("verbose"), 0, POPT_ARG_NONE, 0, OPT_VERBOSE,
    T_("Turn on verbose output mode."), 0
  },

  {
    T_("version"), 0, POPT_ARG_NONE, 0, OPT_VERSION,
    T_("Show version information and exit."), 0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   Global Variables
   _________________________________________________________________________ */

bool Converter::odd = false;

Converter::Byte Converter::masks[] = {
  0, 1, 3, 7, 017, 037, 077, 0177, 0377
};

/* _________________________________________________________________________

   Converter::Converter
   _________________________________________________________________________ */

Converter::Converter ()
  : haveFirstLine (false),
    quiet (false),
    verbose (false)
{
}

/* _________________________________________________________________________

   Converter::~Converter
   _________________________________________________________________________ */

Converter::~Converter ()
{
#if 0
  try
    {
      pFileGsErr->Reset ();
    }
  catch (const exception &)
    {
    }
#endif
}

/* _________________________________________________________________________

   Converter::Error
   _________________________________________________________________________ */

MIKTEXNORETURN
void
Converter::Error (/*[in]*/ const MIKTEXCHAR *		lpszFormat,
		  /*[in]*/				...)
  const
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << T_("gsf2pk") << T_(": ")
	<< Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
  ShowGhostscriptTranscript ();
  throw (1);
}

/* _________________________________________________________________________

   Verbose
   _________________________________________________________________________ */

void
Converter::Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		    /*[in]*/			...)
{
  if (! verbose)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcout << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________

   Converter::ShowGhostscriptTranscript
   _________________________________________________________________________ */

void
Converter::ShowGhostscriptTranscript ()
  const
{
  if (gsStdErr.length() == 0)
    {
      return;
    }
  tcerr << T_("Ghostscript transcript follows:\n")
	<< gsStdErr
	<< endl;
}

/* _________________________________________________________________________

   Converter::GetByte
   _________________________________________________________________________ */

int
Converter::GetByte (/*[in]*/ FILE * pfile)
  const
{
  int c = fgetc(pfile);
  if (c == EOF)
    {
      if (ferror(pfile))
	{
	  FATAL_CRT_ERROR (T_("fgetc"), 0);
	}
      else
	{
	  Error (T_("Unexecpected end of file."));
	}
    }
  return (c & 0xff);
}

/* _________________________________________________________________________

   Converter::GetDword
   _________________________________________________________________________ */

int
Converter::GetDword (/*[in]*/ FILE * pfile)
  const
{
  int ret = GetByte(pfile) << 24;
  ret |= GetByte(pfile) << 16;
  ret |= GetByte(pfile) << 8;
  ret |= GetByte(pfile);
  return (ret);
}

/* _________________________________________________________________________

   Converter::Read
   _________________________________________________________________________ */

void
Converter::Read (/*[out]*/ void *	pv,
		  /*[in]*/ size_t	len)
{
  if (fread(pv, 1, len, pFileGsf.Get()) != len)
    {
      FATAL_CRT_ERROR (T_("fread"), 0);
    }
}

/* _________________________________________________________________________

   Converter::Write
   _________________________________________________________________________ */

void
Converter::Write (/*[out]*/ const void *	pv,
		  /*[in]*/ size_t		len)
{
  if (fwrite(pv, 1, len, pFilePk.Get()) != len)
    {
      FATAL_CRT_ERROR (T_("fwrite"), 0);
    }
}

/* _________________________________________________________________________

   Converter::Expect
   _________________________________________________________________________ */

void
Converter::Expect (/*[in]*/ const char *	lpszWaitingFor,
		   /*[out]*/ string *		pLine)
{
  bool found = false;
  string line;
  if (pLine == 0)
    {
      pLine = &line;
    }
  size_t l = strlen(lpszWaitingFor);
  while (! found)
    {
      if (! Utils::ReadUntilDelim(*pLine, '\n', pFileGsf.Get()))
	{
	  Error (T_("Premature end of file."));
	}
      found = (memcmp(pLine->c_str(), lpszWaitingFor, l) == 0);
    }
}

/* _________________________________________________________________________

   Converter::GetInt
   _________________________________________________________________________ */

int
Converter::GetInt ()
{
  int c;
  do
    {
      c = GetByte();
    }
  while (isspace(c));
  if (c < '0' || c > '9')
    {
      Error (T_("Invalid Gsf file (digit expected)."));
    }
  int i = 0;
  do
    {
      i = i * 10 + (c - T_('0'));
      c = GetByte();
    }
  while (c >= '0' && c <= '9');
  if (! feof(pFileGsf.Get()))
    {
      ungetc (c, pFileGsf.Get());
    }
  return (i);
}

/* _________________________________________________________________________

   Converter::WhiteSpace
   _________________________________________________________________________ */

void
Converter::WhiteSpace ()
{
  bool cont = true;
  while (cont)
    {
      int c = GetByte();
      if (c == '#')
	{
	  do
	    {
	      c = GetByte();
	    }
	  while (! feof(pFileGsf.Get()) && c != '\n');
	}
      else if (! isspace(c))
	{
	  ungetc (c, pFileGsf.Get());
	  cont = false;
	}
    }
}

/* _________________________________________________________________________

   Converter::PutByte
   _________________________________________________________________________ */

void
Converter::PutByte (/*[in]*/ int	by)
  const
{
  if (fputc(by, pFilePk.Get()) == EOF)
    {
      FATAL_CRT_ERROR (T_("fputc"), 0);
    }
}

/* _________________________________________________________________________

   Converter::PutDword
   _________________________________________________________________________ */

void
Converter::PutDword (/*[in]*/ int	dword)
  const
{
  PutByte ((dword >> 24) & 0xff);
  PutByte ((dword >> 16) & 0xff);
  PutByte ((dword >> 8) & 0xff);
  PutByte (dword & 0xff);
}

/* _________________________________________________________________________

   Converter::PutByteArray
   _________________________________________________________________________ */

void
Converter::PutByteArray (/*[in]*/ const void *	pv,
			 /*[in]*/ size_t	len)
  const
{
  PutByte (static_cast<int>(len));
  if (fwrite(pv, 1, len, pFilePk.Get()) != len)
    {
      FATAL_CRT_ERROR (T_("fwrite"), 0);
    }
}

/* _________________________________________________________________________

   Converter::GetFirstByte
   _________________________________________________________________________ */

int
Converter::GetFirstByte (/*[in]*/ const PathName & file)
{
  AutoFILE pFile
    (File::Open(file, FileMode::Open, FileAccess::Read, false));
  int ch = fgetc(pFile.Get());
  pFile.Reset ();
  return (ch);
}

/* _________________________________________________________________________

   Converter::ReadTFMFile
   _________________________________________________________________________ */

void
Converter::ReadTFMFile (/*[in]*/ const MIKTEXCHAR *	lpszTeXFontName)
{
  lengths.resize (12, 0);
  widths.resize (256, 0);
  widthIndex.resize (256, 0);
  chars.reserve (1000);

  PathName pathTFMFile;

  if (! pSession->FindFile(lpszTeXFontName, FileType::TFM, pathTFMFile))
    {
      Error (T_("The TFM file could not be found."));
    }

  Verbose (T_("Reading TFM file %s...\n"), Q_(pathTFMFile));

  AutoFILE pFile
    (File::Open(pathTFMFile, FileMode::Open, FileAccess::Read, false));

  for (size_t i = 0; i < 12; ++ i)
    {
      int j = GetByte(pFile.Get()) << 8;
      lengths[i] = j | GetByte(pFile.Get());
    }

  checkSum = GetDword(pFile.Get());
  designSize = GetDword(pFile.Get());

  if (fseek(pFile.Get(), 4 * (lh() + 6), SEEK_SET) != 0)
    {
      FATAL_CRT_ERROR (T_("fseek"), 0);
    }

  for (int cc = bc(); cc <= ec(); ++ cc)
    {
      widthIndex[cc] = GetByte(pFile.Get());
      if (widthIndex[cc] != 0)
	{
	  if (chars.length() > 0)
	    {
	      chars += T_(' ');
	    }
	  chars += NUMTOSTR(cc);
	}
      GetByte (pFile.Get());
      GetByte (pFile.Get());
      GetByte (pFile.Get());
    }
  chars += T_('\n');

  for (int i = 0; i < nw(); ++ i)
    {
      widths[i] = GetDword(pFile.Get());
    }

  pFile.Reset ();
}

/* _________________________________________________________________________

   Converter::StartGhostscript
   _________________________________________________________________________ */

Process *
Converter::StartGhostscript (/*[in]*/ const MIKTEXCHAR *	lpszFontFile,
			      /*[in]*/ const MIKTEXCHAR *	lpszEncFile,
			      /*[in]*/ const MIKTEXCHAR *	lpszFontName,
			      /*[in]*/ const MIKTEXCHAR *	lpszSpecInfo,
			      /*[in]*/ const MIKTEXCHAR *	lpszDPI,
			      /*[out]*/ FILE **			ppGsOut,
			      /*[out]*/ FILE **			ppGsErr)
{
  PathName pathGs;

  pSession->GetGhostscript (pathGs.GetBuffer(), 0);

  //
  // build the command-line
  //

  CommandLineBuilder commandLine;

  // - no device output
  commandLine.AppendOption (T_("-dNODISPLAY"));

  // - no garbage collection
  commandLine.AppendOption (T_("-dNOGC"));

  // - set font substitution
  commandLine.AppendOption (T_("-sSUBSTFONT="), lpszFontName);

  // - be quiet
  commandLine.AppendOption (T_("-q"));

  // ???
  commandLine.AppendOption (T_("--"));

  // - path to render.ps
  PathName pathRenderPS;
  if (! pSession->FindFile(T_("render.ps"),
			   FileType::PSHEADER,
			   pathRenderPS))
    {
      Error (T_("The driver file render.ps could not be found."));
    }
  commandLine.AppendArgument (pathRenderPS.ToUnix());

  // - font name
  commandLine.AppendArgument (lpszFontName);

  // - font/enc load string
  tstring loadString;
  PathName pathFont;
  if (! pSession->FindFile(lpszFontFile,
			   FileType::TYPE1,
			   pathFont))
    {
      Error (T_("The font file could not be found."));
    }
  loadString = T_("(");
  loadString += pathFont.ToUnix().Get();
  loadString += T_(") ");
  int by = GetFirstByte(pathFont);
  if (by == 0)
    {
      loadString += T_("ttload");
    }
  else if (by == 0200)
    {
      loadString += T_("brun");
    }
  else
    {
      loadString += T_("run");
    }
  if (lpszEncFile != 0 && *lpszEncFile != 0)
    {
      PathName pathEnc;
      if (! pSession->FindFile(lpszEncFile,
			       FileType::ENC,
			       pathEnc))
	{
	  Error (T_("The encoding file could not be found."));
	}
      loadString += T_(" (");
      loadString += pathEnc.ToUnix().Get();
      loadString += T_(" ) run");
    }
  commandLine.AppendArgument (loadString.c_str());

  // - special info
  commandLine.AppendArgument (lpszSpecInfo == 0 ? T_("") : lpszSpecInfo);

  // - DPI
  commandLine.AppendArgument (lpszDPI);

  Verbose (T_("Starting Ghostscript with arguments:\n%s\n"),
	   commandLine.Get());

  ProcessStartInfo startinfo;

  startinfo.FileName = pathGs.ToString();
  startinfo.Arguments = commandLine.Get();
  startinfo.StandardInput = 0;
  startinfo.RedirectStandardInput = true;
  startinfo.RedirectStandardOutput = true;
  startinfo.RedirectStandardError = true;

  auto_ptr<Process> pProcess (Process::Start(startinfo));

  AutoFILE pFileGsIn (pProcess->get_StandardInput());

  *ppGsOut = pProcess->get_StandardOutput();
  AutoFILE pFileGsOut (*ppGsOut);

  *ppGsErr = pProcess->get_StandardError();
  AutoFILE pFileGsErr (*ppGsErr);
  
  tstring designSizeString =
    NUMTOSTR(static_cast<float>(designSize) / (1 << 20));
  designSizeString += '\n';

  // write the design size and character list to Gs stdin
  size_t n;
  if (((n = fwrite(designSizeString.c_str(),
		   1,
		   designSizeString.length(),
		   pFileGsIn.Get()))
       != designSizeString.length())
      || ((n = fwrite(chars.c_str(),
		      1,
		      chars.length(),
		      pFileGsIn.Get()))
	  != chars.length()))
    {
      Error (T_("Ghostscript communication failure."));
    }

  // close input stream
  pFileGsIn.Reset ();

  // keep output streams open
  pFileGsOut.Detach ();
  pFileGsErr.Detach ();

  return (pProcess.release());
}

/* _________________________________________________________________________

   Converter::StderrReader
   _________________________________________________________________________ */

void
MIKTEXCALLBACK
Converter::StderrReader (/*[in]*/ void * pArg)
{
  try
    {
      Converter * This = reinterpret_cast<Converter*>(pArg);
#define CHUNK_SIZE 64
      char buf[ CHUNK_SIZE ];
      size_t n;
      This->gsStdErr = "";
      while ((n = fread(buf, 1, CHUNK_SIZE, This->pFileGsErr.Get())) > 0)
	{
	  for (size_t i = 0; i < n; ++ i)
	    {
	      This->gsStdErr += buf[i];
	    }
	}
    }
  catch (const exception &)
    {
    }
} 

/* _________________________________________________________________________

   Converter::tallyup

   Add up statistics for putting out the given shift count.
   _________________________________________________________________________ */

void
Converter::tallyup (/*[in]*/ int n)
{
  int m;
  if (n > 208)
    {
      ++ base;
      n -= 192;
      for (m = 0x100; m != 0 && m < n; m <<= 4)
	{
	  base += 2;
	}
      if (m != 0 && (m = (m - n) / 15) < 13)
	{
	  deltas[m] += 2;
	}
    }
  else if (n > 13)
    {
      ++ deltas[(208 - n) / 15];
    }
  else
    {
      -- deltas[n - 1];
    }
}

/* _________________________________________________________________________

   Converter::pk_put_nyb

   Routines for storing the shift counts.
   _________________________________________________________________________ */

void
Converter::pk_put_nyb (/*[in]*/ int n)
{
  if (odd)
    {
      *bitmap_end++ = static_cast<Byte>(((part << 4) | n) & 0xff);
      odd = false;
    }
  else
    {
      part = static_cast<Byte>(n & 0xff);
      odd = true;
    }
}

/* _________________________________________________________________________

   Converter::pk_put_long
   _________________________________________________________________________ */

void
Converter::pk_put_long (/*[in]*/ int n)
{
  if (n >= 16)
    {
      pk_put_nyb(0);
      pk_put_long(n / 16);
    }
  pk_put_nyb(n % 16);
}

/* _________________________________________________________________________

   Converter::pk_put_count
   _________________________________________________________________________ */

void
Converter::pk_put_count (/*[in]*/ int n)
{
  if (n > pk_dyn_f)
    {
      if (n > pk_dyn_g)
	{
	  pk_put_long (n - pk_dyn_g + 15);
	}
      else
	{
	  pk_put_nyb (pk_dyn_f + (n - pk_dyn_f + 15) / 16);
	  pk_put_nyb ((n - pk_dyn_f - 1) % 16);
	}
    }
  else
    {
      pk_put_nyb (n);
    }
}

/* _________________________________________________________________________

   Converter::trim_bitmap
   _________________________________________________________________________ */

void
Converter::trim_bitmap ()
{
  Byte * p;
  Byte mask;

  // clear out garbage bits in bitmap
  if (width % 8 != 0)
    {
      mask = ~masks[8 - width % 8];
      for (p = bitmap + bytesWide - 1; p < bitmap_end; p += bytesWide)
	{
	  *p &= mask;
	}
    }

  /*
   *	Find the bounding box of the bitmap.
   */

  // trim top
  skip = 0;
  mask = 0;
  for (;;)
    {
      if (bitmap >= bitmap_end)
	{			// if bitmap is empty
	  width = height = hoff = voff = 0;
	  return;
	}
      p = bitmap + bytesWide;
      while (p > bitmap)
	{
	  mask |= *--p;
	}
      if (mask)
	{
	  break;
	}
      ++ skip;
      bitmap += bytesWide;
    }
  height -= skip;
  voff -= skip;

  // trim bottom
  skip = 0;
  mask = 0;
  for (;;)
    {
      p = bitmap_end - bytesWide;
      while (p < bitmap_end)
	{
	  mask |= *p++;
	}
      if (mask)
	{
	  break;
	}
      ++ skip;
      bitmap_end -= bytesWide;
    }
  height -= skip;

  // trim right
  skip = 0;
  -- width;
  for (;;)
    {
      mask = 0;
      for (p = bitmap + width / 8; p < bitmap_end; p += bytesWide)
	{
	  mask |= *p;
	}
      if (mask & (0x80 >> (width % 8)))
	{
	  break;
	}
      -- width;
      ++ skip;
    }
  ++ width;

  // trim left
  skip = 0;
  for (;;)
    {
      mask = 0;
      for (p = bitmap + skip / 8; p < bitmap_end; p += bytesWide)
	{
	  mask |= *p;
	}
      if (mask & (0x80 >> (skip % 8)))
	{
	  break;
	}
      ++ skip;
    }
  width -= skip;
  hoff -= skip;
  bitmap += skip / 8;
  skip = skip % 8;
}

/* _________________________________________________________________________

   Converter::pk_rll_cvt

   Pack the bitmap using the rll method.  (Return false if it's better
   to just pack the bits.)
   _________________________________________________________________________ */

bool
Converter::pk_rll_cvt ()
{
  unsigned int ncounts;		// max to allow this time
  int * nextcount;		// next count value
  int * counts_end;		// pointer to end
  Byte * rowptr;
  Byte * p;
  Byte mask;
  Byte * rowdup;		// last row checked for dup
  Byte paint_switch;		// 0 or 0xff
  int bits_left;		// bits left in row
  int cost;
  int i;

  /*
   *	Allocate space for bit counts.
   */

  ncounts = (width * height + 3) / 4;
  if (ncounts > counts.size())
    {
      counts.resize (ncounts);
    }
  counts_end = &counts[0] + ncounts;

  /*
   *	Form bit counts and collect statistics
   */

  base = 0;
  ZeroMemory (deltas, sizeof(deltas));
  rowdup = 0;			// last row checked for duplicates
  p = rowptr = bitmap;
  mask = static_cast<Byte>(0x80 >> skip);
  flag = 0;
  paint_switch = 0;
  if (*p & mask)
    {
      flag = 8;
      paint_switch = 0xff;
    }
  bits_left = width;
  nextcount = &counts[0];
  while (rowptr < bitmap_end)
    {				// loop over shift counts
      int shift_count = bits_left;
      
      for (;;)
	{
	  if (bits_left == 0)
	    {
	      if ((p = rowptr += bytesWide) >= bitmap_end) break;
	      mask = static_cast<Byte>(0x80 >> skip);
	      bits_left = width;
	      shift_count += width;
	    }
	  if (((*p ^ paint_switch) & mask) != 0)
	    {
	      break;
	    }
	  -- bits_left;
	  mask >>= 1;
	  if (mask == 0)
	    {
	      ++ p;
	      while (*p == paint_switch && bits_left >= 8)
		{
		  ++ p;
		  bits_left -= 8;
		}
	      mask = 0x80;
	    }
	}
      if (nextcount >= counts_end)
	{
	  return (false);
	}
      shift_count -= bits_left;
      *nextcount++ = shift_count;
      tallyup(shift_count);
      // check for duplicate rows
      if (rowptr != rowdup && bits_left != width)
	{
	  Byte * p1 = rowptr;
	  Byte * q = rowptr + bytesWide;
	  int repeat_count;
	  
	  while (q < bitmap_end && *p1 == *q)
	    {
	      ++ p1;
	      ++ q;
	    }
	  repeat_count = static_cast<int>((p1 - rowptr) / bytesWide);
	  if (repeat_count > 0)
	    {
	      *nextcount++ = -repeat_count;
	      if (repeat_count == 1)
		{
		  -- base;
		}
	      else
		{
		  ++ base;
		  tallyup(repeat_count);
		}
	      rowptr += repeat_count * bytesWide;
	    }
	  rowdup = rowptr;
	}
      paint_switch = ~paint_switch;
    }

  /*
   *	Determine the best pk_dyn_f
   */

  pk_dyn_f = 0;
  cost = base += static_cast<int>(2 * (nextcount - &counts[0]));
  for (i = 1; i < 14; ++i)
    {
      base += deltas[i - 1];
      if (base < cost)
	{
	  pk_dyn_f = i;
	  cost = base;
	}
    }
  // last chance to bail out
  if (cost * 4 > width * height)
    {
      return (false);
    }
  
  /*
   *	Pack the bit counts
   */
  
  pk_dyn_g = 208 - 15 * pk_dyn_f;
  flag |= pk_dyn_f << 4;
  bitmap_end = bitmap;
  *nextcount = 0;
  nextcount = &counts[0];
  while (*nextcount != 0)
    {
      if (*nextcount > 0)
	{
	  pk_put_count (*nextcount);
	}
      else
	{
	  if (*nextcount == -1)
	    {
	      pk_put_nyb (15);
	    }
	  else
	    {
	      pk_put_nyb (14);
	      pk_put_count (-*nextcount);
	    }
	}
      ++ nextcount;
    }
  if (odd)
    {
      pk_put_nyb (0);
      ++ cost;
    }
  if (cost != 2 * (bitmap_end - bitmap))
    {
      printf ("Cost miscalculation:  expected %d, got %d\n",
	      cost,
	      2 * (bitmap_end - bitmap));
    }
  pk_len = static_cast<int>(bitmap_end - bitmap);
  return (true);
}

/* _________________________________________________________________________

   Converter::pk_bm_cvt
   _________________________________________________________________________ */

void
Converter::pk_bm_cvt ()
{
  Byte * rowptr;
  Byte * p;
  int blib1;			// bits left in byte
  int bits_left;		// bits left in row
  Byte * q;
  int blib2;
  Byte nextbyte;

  flag = 14 << 4;
  q = bitmap;
  blib2 = 8;
  nextbyte = 0;
  for (rowptr = bitmap; rowptr < bitmap_end; rowptr += bytesWide)
    {
      p = rowptr;
      blib1 = 8 - skip;
      bits_left = width;
      if (blib2 != 8)
	{
	  int n;

	  if (blib1 < blib2)
	    {
	      nextbyte |= *p << (blib2 - blib1);
	      n = blib1;
	    }
	  else
	    {
	      nextbyte |= *p >> (blib1 - blib2);
	      n = blib2;
	    }
	  blib2 -= n;
	  if ((bits_left -= n) < 0)
	    {
	      blib2 -= bits_left;
	      continue;
	    }
	  if ((blib1 -= n) == 0)
	    {
	      blib1 = 8;
	      ++p;
	      if (blib2 > 0)
		{
		  nextbyte |= *p >> (8 - blib2);
		  blib1 -= blib2;
		  bits_left -= blib2;
		  if (bits_left < 0)
		    {
		      blib2 = -bits_left;
		      continue;
		    }
		}
	    }
	  *q++ = nextbyte;
	}
      // fill up whole (destination) bytes
      while (bits_left >= 8)
	{
	  nextbyte = static_cast<Byte>(((*p++ << (8 - blib1)) & 0xff) & 0xff);
	  *q++ = static_cast<Byte>(((nextbyte | (*p >> blib1)) & 0xff) & 0xff);
	  bits_left -= 8;
	}
      // now do the remainder
      nextbyte = static_cast<Byte>((*p << (8 - blib1)) & 0xff);
      if (bits_left > blib1)
	{
	  nextbyte |= p[1] >> blib1;
	}
      blib2 = 8 - bits_left;
    }
  if (blib2 != 8)
    {
      *q++ = nextbyte;
    }
  pk_len = static_cast<int>(q - bitmap);
}

/* _________________________________________________________________________

   Converter::putshort
   _________________________________________________________________________ */

void
Converter::putshort (/*[in]*/ int w)
{
  PutByte (w >> 8);
  PutByte (w);
}

/* _________________________________________________________________________

   Converter::putmed
   _________________________________________________________________________ */

void
Converter::putmed (/*[in]*/ long w)
{
  PutByte (w >> 16);
  PutByte (w >> 8);
  PutByte (w);
}

/* _________________________________________________________________________

   Converter::putlong
   _________________________________________________________________________ */

void
Converter::putlong (/*[in]*/ long w)
{
  PutByte (w >> 24);
  PutByte (w >> 16);
  PutByte (w >> 8);
  PutByte (w);
}

/* _________________________________________________________________________

   Converter::PutGlyph
   _________________________________________________________________________ */

void
Converter::PutGlyph (/*[in]*/ int cc)
{
  if (! haveFirstLine)
    {
      tstring line;
      Expect ("#^", &line);
      if (SScanF(line.c_str(),
		 "#^ %d %d %d %d %d %f\n",
		 &idx,
		 &llx,
		 &lly,
		 &urx,
		 &ury,
		 &charWidth)
	  != 6)
	{
	  Error (T_("Invalid Gsf file."));
	}
    }
  if (idx < cc)
    {
      Error (T_("Invalid Gsf file."));
    }
  if (idx > cc)
    {
      tcerr << T_("Character ") << cc << T_(" is missing.") << endl;
      haveFirstLine = true;
      return;
    }
  haveFirstLine = false;
  hoff = -llx + 2;
  voff = ury + 2 - 1;
  Expect ("P4\n");
  WhiteSpace ();
  width = GetInt();
  WhiteSpace ();
  height = GetInt();
  GetByte ();
  if (width != urx - llx + 4 || height != ury - lly + 4)
    {
      Error (T_("Dimensions do not match"));
    }
  bytesWide = (width + 7) / 8;
  bitmapSize = bytesWide * height;
  area1.resize (bitmapSize);
  for (Byte * p = &area1[(height - 1) * bytesWide];
       p >= &area1[0];
       p -= bytesWide)
    {
      Read (p, bytesWide);
    }
  bitmap = &area1[0];
  bitmap_end = bitmap + bitmapSize;
  trim_bitmap ();
  if (height == 0 || ! pk_rll_cvt())
    {
      pk_bm_cvt ();
    }
  int tfm_wid = widths[widthIndex[cc]];
  int dm =
    static_cast<int>( (charWidth + 0.5) - (charWidth < -0.5));
  if (pk_len + 8 < 4 * 256 && tfm_wid < (1<<24)
      && dm >= 0 && dm < 256 && width < 256 && height < 256
      && hoff >= -128 && hoff < 128 && voff >= -128 && voff < 128)
    {
      PutByte (flag | ((pk_len + 8) >> 8));
      PutByte (pk_len + 8);
      PutByte (cc);
      putmed (tfm_wid);
      PutByte (dm);
      PutByte (width);
      PutByte (height);
      PutByte (hoff);
      PutByte (voff);
    }
  else if (pk_len + 13 < 3 * 65536L && tfm_wid < (1<<24)
	   && dm >= 0 && dm < 65536 && width < 65536 && height < 65536
	   && hoff >= -65536 && hoff < 65536
	   && voff >= -65536 && voff < 65536)
    {
      PutByte (flag | 4 | ((pk_len + 13) >> 16));
      putshort (pk_len + 13);
      PutByte (cc);
      putmed (tfm_wid);
      putshort (dm);
      putshort (width);
      putshort (height);
      putshort (hoff);
      putshort (voff);
    }
  else
    {
      PutByte (flag | 7);
      putlong (pk_len + 28);
      putlong (cc);
      putlong (tfm_wid);
      putlong (static_cast<int>((charWidth * 65536.0 + 0.5)
				- (charWidth < -0.5)));
      putlong (0);
      putlong (width);
      putlong (height);
      putlong (hoff);
      putlong (voff);
    }

  Write (bitmap, pk_len);
}

/* _________________________________________________________________________

   Converter::putspecl
   _________________________________________________________________________ */

void
Converter::putspecl (/*[in]*/ const MIKTEXCHAR * str1,
		     /*[in]*/ const MIKTEXCHAR * str2)
{
  int len1 = static_cast<int>(strlen(str1));
  int len2 = 0;
  if (str2 != 0)
    {
      len2 = static_cast<int>(strlen(str2));
    }
  if (len1 + len2 > 255)
    {
      return;
    }
  const int PK_SPC = 240;
  PutByte (PK_SPC);
  PutByte (len1 + len2);
  Write (str1, len1);
  if (len2 != 0)
    {
      Write (str2, len2);
    }
}

/* _________________________________________________________________________

   Converter::WritePkFile
   _________________________________________________________________________ */

void
Converter::WritePkFile (/*[in]*/ const MIKTEXCHAR *	lpszPkFile)
{
  Verbose (T_("Writing Pk file %s...\n"), Q_(lpszPkFile));

  pFilePk.Reset
    (File::Open(lpszPkFile, FileMode::Create, FileAccess::Write, false));

  const int PK_PRE = 247;
  const int PK_ID = 89;
  const int PK_POST = 245;
  const int PK_NOP = 246;

  PutByte (PK_PRE);
  PutByte (PK_ID);

  tstring line;

  // read version number / write id string
  Expect ("V", &line);
  tstring id = "gsf2pk ";
  id += VER_FILEVERSION_STR;
  int gsVersion;
  if (SScanF(line.c_str(), "V %d", &gsVersion) == 1)
    {
      id += '/';
      id += NUMTOSTR(gsVersion);
    }
  PutByteArray (id.c_str(), id.length());

  // write design size / checksum / page size
  PutDword (designSize);
  PutDword (checkSum);
  int ppp = static_cast<int>(dpi / 72.27 * 65536.0 + 0.5);
  PutDword (ppp);		// hppp
  PutDword (ppp);		// vppp

  // write bitmaps
  for (int cc = bc(); cc <= ec(); ++ cc)
    {
      if (widthIndex[cc] != 0)
	{
	  PutGlyph (cc);
	}
    }
  
  // write info specials
  putspecl ("jobname=", fontName.c_str());
  putspecl ("mag=1");
  putspecl ("mode=modeless");
  putspecl ("pixels_per_inch=", NUMTOSTR(dpi));

  // write post-amble
  PutByte (PK_POST);
  while (ftell(pFilePk.Get()) % 4 != 0)
    {
      PutByte (PK_NOP);
    }

  pFilePk.Reset ();
}

/* _________________________________________________________________________

   Converter::Convert
   _________________________________________________________________________ */

void
Converter::Convert (/*[in]*/ const MIKTEXCHAR *		lpszTeXFontName,
		    /*[in]*/ const MIKTEXCHAR *		lpszFontName,
		    /*[in]*/ const MIKTEXCHAR *		lpszSpecInfo,
		    /*[in]*/ const MIKTEXCHAR *		lpszEncFile,
		    /*[in]*/ const MIKTEXCHAR *		lpszFontFile,
		    /*[in]*/ const MIKTEXCHAR *		lpszDPI,
		    /*[out]*/ const MIKTEXCHAR *	lpszPkFile)
{
  fontName = lpszTeXFontName;
  dpi = _ttoi(lpszDPI);

  ReadTFMFile (lpszTeXFontName);

  try
    {
      auto_ptr<Process> pGhostscript (StartGhostscript(lpszFontFile,
						       lpszEncFile,
						       lpszFontName,
						       lpszSpecInfo,
						       lpszDPI,
						       &pFileGsf,
						       &pFileGsErr));
      
      auto_ptr<Thread> pStderrReader (Thread::Start(StderrReader, this));
      
      WritePkFile (lpszPkFile);
      
      pFileGsf.Reset ();
      
      if (! pGhostscript->WaitForExit(10000))
	{
	  Error (T_("Ghostscript didn't complete."));
	}
      
      if (pGhostscript->get_ExitCode() != 0)
	{
	  Error (T_("Ghostscript failed."));
	}

      pFileGsErr.Reset ();
      pStderrReader.reset ();
      pGhostscript.reset ();
    }

  catch (const exception &)
    {
      pFileGsf.Reset ();
      throw;
    }

}

/* _________________________________________________________________________

   Converter::Main
   _________________________________________________________________________ */

void
Converter::Main (/*[in]*/ int			argc,
		 /*[in]*/ const MIKTEXCHAR * *	argv)
{
  Cpopt popt (argc, argv, aoption);

  // process command-line options
  int option;
  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{
	case OPT_QUIET:
	  if (verbose)
	    {
	      Error (T_("Cannot be --verbose and --quiet at the same time."));
	    }
	  quiet = true;
	  break;
	case OPT_VERBOSE:
	  if (quiet)
	    {
	      Error (T_("Cannot be --verbose and --quiet at the same time."));
	    }
	  verbose = true;
	  break;
	case OPT_VERSION:
	  tcout << Utils::MakeProgramVersionString(T_("gsf2pk"),
						   VersionNumber(VER_FILEVERSION))
		<< T_("\n\
Copyright (C) 1993-2000  Paul Vojta\n\
Copyright (C) 2004-2006 Christian Schenk\n\
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
      Error (T_("%s"), msg.c_str());
    }

  if (popt.GetArgCount() != 7)
    {
      Error (T_("Wrong number of command-line arguments."));
    }

  const MIKTEXCHAR * * leftovers = popt.GetArgs();

  Init (argv[0]);

  Convert (leftovers[0],	// "utmr8r"
	   leftovers[1],	// "NimbusRomNo9L-Regu"
	   leftovers[2],	// " TeXBase1Encoding ReEncodeFont "
	   leftovers[3],	// "8r.enc"
	   leftovers[4],	// "utmr8a.pfb"
	   leftovers[5],	// "600"
	   leftovers[6]);	// "utmr8a.pk"

  Finalize ();
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR * *	argv)
{
  try
    {
      Converter conv;
      conv.Main (argc, argv);
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
