/* xetex-miktex.h:						-*- C++ -*-
   
   Copyright (C) 2007-2010 Christian Schenk

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

#include <miktex/TeXAndFriends/config.h>

#include "xetexd.h"

#if ! defined(THEDATA)
#  define THEDATA(x) C4P_VAR(x)
#endif

#include "xetex.rc"

#include <miktex/Core/Paths>
#include <miktex/TeXAndFriends/ETeXApp>

#if 0
namespace xetex {
#include "xetex.defaults.h"
}
#endif

#if ! defined(MIKTEXHELP_XETEX)
#  include <MiKTeX/Core/Help>
#endif

extern const char * papersize;
extern const char * outputdriver;

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
  MIKTEXTHISCALL
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
	       "CMD");

    AddOption (MIKTEXTEXT("\
papersize\0Set PDF media size to STRING"),
	       OPT_PAPERSIZE,
	       required_argument,
	       "STRING");
  }

public:
  virtual
  bool
  MIKTEXTHISCALL
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const char *	lpszOptArg)
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
#if 1
    for (int idx = 0; idx < nFonts; ++ idx)
      {
	THEDATA(fontmapping)[idx] = 0;
      }
#endif
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
  MIKTEXTHISCALL
  Init (/*[in]*/ const char * lpszProgramInvocationName)
  {
    ETeXApp::Init (lpszProgramInvocationName);
    EnableFeature (MiKTeX::TeXAndFriends::Feature::EightBitChars);
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetMemoryDumpFileName ()
    const
  {
    return ("xetex.fmt");
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetInitProgramName ()
    const
  {
    return ("inixetex");
  }
  
public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetVirginProgramName ()
    const
  {
    return ("virxetex");
  }
  
public:
  virtual
  const char *
  MIKTEXTHISCALL
  TheNameOfTheGame ()
    const
  {
    return ("XeTeX");
  }

public:
  virtual
  unsigned long
  MIKTEXTHISCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_XETEX);
  }
};

extern XETEXCLASS XETEXAPP;
#define THEAPP XETEXAPP
#include <miktex/TeXAndFriends/ETeXApp.inl>

#include <miktex/KPSE/Emulation>

#include "xetex.h"
#include "synctex.h"

// special case: Web2C likes to add 1 to the nameoffile base address
inline
utf8code *
GetNameOfFileForWeb2C ()
{
  return (&((THEDATA(nameoffile))[-1]));
}

#define nameoffile (GetNameOfFileForWeb2C())

#define eqtb THEDATA(eqtb)
#define nopdfoutput THEDATA(nopdfoutput)
#define depthbase THEDATA(depthbase)
#define heightbase THEDATA(heightbase)
#define parambase THEDATA(parambase)
#define bufsize THEDATA(bufsize)
#define nativefonttypeflag THEDATA(nativefonttypeflag)
#define namelength THEDATA(namelength)
#define namelength16 THEDATA(namelength16)
#define first THEDATA(first)
#define last THEDATA(last)
#define maxbufstack THEDATA(maxbufstack)
#define fontflags THEDATA(fontflags)
#define xdvbuffer THEDATA(xdvbuffer)
#define loadedfontflags THEDATA(loadedfontflags)
#define fontinfo THEDATA(fontinfo)
#define fontletterspace THEDATA(fontletterspace)
#define fontsize THEDATA(fontsize)
#define loadedfontletterspace THEDATA(loadedfontletterspace)
#define fontarea THEDATA(fontarea)
#define buffer THEDATA(buffer)
#define mappedtext THEDATA(mappedtext)
#define nameoffile16 THEDATA(nameoffile16)
#define fontlayoutengine THEDATA(fontlayoutengine)
#define loadedfontmapping THEDATA(loadedfontmapping)
#define loadedfontdesignsize THEDATA(loadedfontdesignsize)
#define zmem THEDATA(zmem)
#define eq THEDATA(eq)
#define jobname THEDATA(jobname)
#define synctexoption THEDATA(synctexoption)
#define synctexoffset THEDATA(synctexoffset)
#define curinput THEDATA(curinput)
#define totalpages THEDATA(totalpages)
#define curh THEDATA(curh)
#define curv THEDATA(curv)
#define rulewd THEDATA(rulewd)
#define ruleht THEDATA(ruleht)
#define ruledp THEDATA(ruledp)

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
#define zxnoverd xnoverd
#define zenddiagnostic enddiagnostic
#define zprintnl printnl
#define zprintint printint
#define zprintscaled printscaled

boolean
open_dvi_output(/*out*/ bytefile & dviFile);

int
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
	("flush", 0, MIKTEXTEXT(__FILE__), __LINE__);
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

   isopentypemathfont
   _________________________________________________________________________ */

inline
int
isopentypemathfont (/*[in]*/ const voidpointer	p)
{
  return (isOpenTypeMathFont(reinterpret_cast<XeTeXLayoutEngine>(p)));
}

/* _________________________________________________________________________

   findnextgraphitebreak
   _________________________________________________________________________ */

#define findnextgraphitebreak findNextGraphiteBreak

/* _________________________________________________________________________

   printcstring
   _________________________________________________________________________ */

template<class CharType>
void
printcstring (const CharType * lpsz)
{
  for (; *lpsz != 0; ++ lpsz)
    {
      printchar (*lpsz);
    }
}
