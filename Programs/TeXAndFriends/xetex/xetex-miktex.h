/* xetex-miktex.h:						-*- C++ -*-
   
   Copyright (C) 2007 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#pragma once

#include "xetexdefs.h"

#if ! defined(C4PEXTERN)
#  define C4PEXTERN extern
#endif

#include "xetexd.h"

#if ! defined(THEDATA)
#  define THEDATA(x) XETEXDATA.m_##x
#endif

#include "xetex.rc"

#include <miktex/paths.h>
#include <miktex/etexapp.h>

#if 0
namespace xetex {
#include "xetex.defaults.h"
}
#endif

#if ! defined(MIKTEXHELP_XETEX)
#  include <miktex/help.h>
#endif

extern char * papersize;
extern char * outputdriver;

/* _________________________________________________________________________

   XeTeXApp
   _________________________________________________________________________ */

class XETEXCLASS

  : public MiKTeX::TeXAndFriends::ETeXApp

{
public:
  enum {
    OPT_OUTPUT_DRIVER = 10000,
    OPT_NO_PDF,
    OPT_PAPERSIZE,
  };

public:
  virtual
  void
  MIKTEXMFCALL
  AddOptions ()
  {
    ETeXApp::AddOptions ();

    AddOption (MIKTEXTEXT("\
no-pdf\0Generate XDV (extended DVI) output rather than PDF."),
	       OPT_NO_PDF);

    AddOption (MIKTEXTEXT("\
output-driver\0Use CMD as the XDV-to-PDF driver instead of xdvipdfmx."),
	       OPT_OUTPUT_DRIVER,
	       required_argument,
	       MIKTEXTEXT("CMD"));

    AddOption (MIKTEXTEXT("\
papersize\0Set PDF media size to STRING"),
	       OPT_PAPERSIZE,
	       required_argument,
	       MIKTEXTEXT("STRING"));
  }

public:
  virtual
  bool
  MIKTEXMFCALL
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg)
  {
    bool done = true;
    switch (opt)
      {
      case OPT_NO_PDF:
	THEDATA(nopdfoutput) = true;
	break;
      case OPT_OUTPUT_DRIVER:
	outputdriver = strdup(lpszOptArg);
	break;
      case OPT_PAPERSIZE:
	papersize = strdup(lpszOptArg);
	break;
      default:
	done = ETeXApp::ProcessOption(opt, lpszOptArg);
	break;
      }
    return (done);
  }

public:
  void
  AllocateMemory ()
  {
    ETeXApp::AllocateMemory ();

    size_t nFonts = THEDATA(fontmax) - constfontbase;

    Allocate ("fontmapping", THEDATA(fontmapping), nFonts);
    Allocate ("fontlayoutengine", THEDATA(fontlayoutengine), nFonts);
    Allocate ("fontflags", THEDATA(fontflags), nFonts);
    Allocate ("fontletterspace", THEDATA(fontletterspace), nFonts);
  }

public:
  void
  FreeMemory ()
  {
    ETeXApp::FreeMemory ();

    Free (THEDATA(fontmapping));
    Free (THEDATA(fontlayoutengine));
    Free (THEDATA(fontflags));
    Free (THEDATA(fontletterspace));
  }

public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
  {
    ETeXApp::Init (lpszProgramInvocationName);
    EnableFeature (MiKTeX::TeXAndFriends::Feature::EightBitChars);
  }

public:
  virtual
  const char *
  MIKTEXMFCALL
  GetMemoryDumpFileName ()
    const
  {
    return ("xetex.fmt");
  }

public:
  virtual
  const char *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    return ("xetex.pool");
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetInitProgramName ()
    const
  {
    return (MIKTEXTEXT("inixetex"));
  }
  
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetVirginProgramName ()
    const
  {
    return (MIKTEXTEXT("virxetex"));
  }
  
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  TheNameOfTheGame ()
    const
  {
    return (MIKTEXTEXT("XeTeX"));
  }

public:
  virtual
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_XETEX);
  }
};

extern XETEXCLASS XETEXAPP;
#define THEAPP XETEXAPP
#include <miktex/etexapp.inl>

#include <miktex/kpsemu.h>

#include "xetex.h"

#if defined(COMPILING_XETEX_CC)
#  define MAKE_GLOBAL(type, name) type & name = XETEXDATA.m_##name;
#else
#  define MAKE_GLOBAL(type, name) extern type & name;
#endif

// special case: Web2C likes to add 1 to the nameoffile base address
inline
utf8code *
GetNameOfFileForWeb2C ()
{
  return ((&XETEXDATA.m_nameoffile[-1]));
}

#define nameoffile (GetNameOfFileForWeb2C())

MAKE_GLOBAL(C4P_boolean, nopdfoutput);
MAKE_GLOBAL(C4P_integer*, depthbase);
MAKE_GLOBAL(C4P_integer*, heightbase);
MAKE_GLOBAL(C4P_integer*, parambase);
MAKE_GLOBAL(C4P_integer, bufsize);
MAKE_GLOBAL(C4P_integer, nativefonttypeflag);
MAKE_GLOBAL(C4P_signed16, namelength);
MAKE_GLOBAL(C4P_signed16, namelength16);
MAKE_GLOBAL(C4P_signed32, first);
MAKE_GLOBAL(C4P_signed32, last);
MAKE_GLOBAL(C4P_signed32, maxbufstack);
MAKE_GLOBAL(char*, fontflags);
MAKE_GLOBAL(char*, xdvbuffer);
MAKE_GLOBAL(char, loadedfontflags);
MAKE_GLOBAL(memoryword*, fontinfo);
MAKE_GLOBAL(scaled*, fontletterspace);
MAKE_GLOBAL(scaled*, fontsize);
MAKE_GLOBAL(scaled, loadedfontletterspace);
MAKE_GLOBAL(strnumber*, fontarea);
MAKE_GLOBAL(unicodescalar*, buffer);
MAKE_GLOBAL(utf16code*, mappedtext);
MAKE_GLOBAL(utf16code*, nameoffile16);
MAKE_GLOBAL(voidpointer*, fontlayoutengine);
MAKE_GLOBAL(voidpointer, loadedfontmapping);

#define c4p_sizeof(x) sizeof(x)
#define addressof(x) &(x)
#undef nullptr
#define nullptr() 0
#define isnullptr(p) ((p) == 0)
#define stringcast(x) ((char*)(x))

#define afield aField
#define bfield bField
#define cfield cField
#define dfield dField
#define fix2x Fix2X
#define htfield htField
#define setpoint setPoint
#define txfield txField
#define tyfield tyField
#define wdfield wdField
#define x2fix X2Fix
#define xcoord xCoord
#define xfield xField
#define ycoord yCoord
#define yfield yField

boolean
open_dvi_output(/*out*/ bytefile & dviFile);

void
dviclose (/*[in,out]*/ bytefile & dviFile);

/* _________________________________________________________________________

   gettexstring
   _________________________________________________________________________ */

inline
char *
gettexstring (/*[in]*/ strnumber stringNumber)
{
  int stringStart = MiKTeX::TeXAndFriends::GetTeXStringStart(stringNumber);
  int stringLength = MiKTeX::TeXAndFriends::GetTeXStringLength(stringNumber);
  char * lpsz = reinterpret_cast<char*>(xmalloc(stringLength + 1));
  return (MiKTeX::TeXAndFriends::GetTeXString(lpsz,
					      stringLength + 1,
					      stringStart,
					      stringLength));
}

/* _________________________________________________________________________

   maketexstring
   _________________________________________________________________________ */

inline
strnumber
maketexstring (/*[in]*/ const char * lpsz)
{
  return (THEAPP.MakeTeXString(lpsz));
}

/* _________________________________________________________________________

   eightbitp
   _________________________________________________________________________ */

inline
bool
eightbitp ()
{
  return (THEAPP.Enable8BitCharsP());
}

/* _________________________________________________________________________

   inertsrcspecialauto
   _________________________________________________________________________ */

#define insertsrcspecialauto miktexinsertsrcspecialauto

/* _________________________________________________________________________

   miktexgetpoolfilename
   _________________________________________________________________________ */

inline
void
miktexgetpoolfilename (/*[out]*/ utf8code * lpszPath)
{
  // FIXME: we assume that the pool file name contains characters in
  // the range 32..127
  MiKTeX::TeXAndFriends::miktexgetpoolfilename
    (reinterpret_cast<char*>(lpszPath));
}

/* _________________________________________________________________________

   miktexopentfmfile
   _________________________________________________________________________ */

inline
bool
miktexopentfmfile (/*[out]*/ bytefile &		f,
		   /*[in]*/ const utf8code *	lpszFileName_)
{
  const char * lpszFileName = reinterpret_cast<const char *>(lpszFileName_);
  MiKTeX::Core::PathName fileName
    (MiKTeX::Core::Utils::UTF8ToWideChar(lpszFileName));
  return (MiKTeX::TeXAndFriends::OpenTFMFile(&f, fileName.Get()));
}

/* _________________________________________________________________________

   xmallocchararray
   _________________________________________________________________________ */

inline
char *
xmallocchararray (/*[in]*/ size_t size)
{
  return (reinterpret_cast<char*>(xmalloc(size + 1)));
}

/* _________________________________________________________________________

   libcfree
   _________________________________________________________________________ */

inline
void
libcfree (/*[in]*/ void * ptr)
{
  free (ptr);
}

/* _________________________________________________________________________

   uopenin
   _________________________________________________________________________ */

inline
int
uopenin (/*[in,out]*/ unicodefile &	f,
	 /*[in]*/ int			mode,
	 /*[in]*/ int			encodingData)
{
  return (u_open_in(&f, mode, encodingData));
}

/* _________________________________________________________________________

   otpartcount
   _________________________________________________________________________ */

inline
int
otpartcount (const voidpointer a)
{
  return (otpartcount((const GlyphAssembly*) a));
}

/* _________________________________________________________________________

   printutf8str
   _________________________________________________________________________ */

inline
void
printutf8str (const voidpointer str, int len)
{
  printutf8str((const unsigned char*) str, len);
}


/* _________________________________________________________________________

   otpartisextender
   _________________________________________________________________________ */

inline
int
otpartisextender (const voidpointer a, int i)
{
  return (otpartisextender((const GlyphAssembly*) a, i));
}

/* _________________________________________________________________________

   otpartstartconnector
   _________________________________________________________________________ */

inline
int
otpartstartconnector (int f, const voidpointer a, int i)
{
  return (otpartstartconnector(f, (const GlyphAssembly*) a, i));
}

/* _________________________________________________________________________

   otpartfulladvance
   _________________________________________________________________________ */

inline
int
otpartfulladvance (int f, const voidpointer a, int i)
{
  return (otpartfulladvance(f, (const GlyphAssembly*) a, i));
}

/* _________________________________________________________________________

   otpartendconnector
   _________________________________________________________________________ */

inline
int
otpartendconnector (int f, const voidpointer a, int i)
{
  return (otpartendconnector(f, (const GlyphAssembly*) a, i));
}

/* _________________________________________________________________________

   otpartglyph
   _________________________________________________________________________ */

inline
int
otpartglyph(const voidpointer a, int i)
{
  return (otpartglyph((GlyphAssembly*) a, i));
}

/* _________________________________________________________________________

   c4p_break
   _________________________________________________________________________ */

inline
void
c4p_break (/*[in]*/ unicodefile & f)
{
  if (fflush(f->f) == EOF)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("flush"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

/* _________________________________________________________________________

   inputln
   _________________________________________________________________________ */

inline
bool
inputln (/*[in]*/ unicodefile &		f,
	 /*[in]*/ C4P::C4P_boolean	bypassEndOfLine = true)
{
  bypassEndOfLine;
  return (input_line(f));
}

/* _________________________________________________________________________

   miktexopenin
   _________________________________________________________________________ */

inline
void
miktexopenin ()
{
  static UFILE termin_file;
  if (THEDATA(termin) == 0)
    {
      THEDATA(termin) = &termin_file;
      THEDATA(termin)->f = stdin;
      THEDATA(termin)->savedChar = -1;
      THEDATA(termin)->skipNextLF = 0;
      THEDATA(termin)->encodingMode = UTF8;
      THEDATA(termin)->conversionData = 0;
      THEDATA(inputfile)[0] = THEDATA(termin);
    }
}

/* _________________________________________________________________________

   usinggraphite
   _________________________________________________________________________ */

inline
boolean
usinggraphite (/*[in]*/ const voidpointer p)
{
  return (usingGraphite(reinterpret_cast<XeTeXLayoutEngine>(p)));
}

/* _________________________________________________________________________

   usingopentype
   _________________________________________________________________________ */

inline
boolean
usingopentype (/*[in]*/ const voidpointer p)
{
  return (usingOpenType(reinterpret_cast<XeTeXLayoutEngine>(p)));
}


/* _________________________________________________________________________

   initgraphitebreaking
   _________________________________________________________________________ */

inline
void
initgraphitebreaking (/*[in]*/ const voidpointer	p,
		      /*[in]*/ const utf16code *	pUtf16Code,
		      /*[in]*/ int			len)
{
  initGraphiteBreaking (reinterpret_cast<XeTeXLayoutEngine>(p),
			reinterpret_cast<const UniChar *>(pUtf16Code),
			len);
}

/* _________________________________________________________________________

   findnextgraphitebreak
   _________________________________________________________________________ */

#define findnextgraphitebreak findNextGraphiteBreak