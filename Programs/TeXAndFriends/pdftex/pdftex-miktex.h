/* pdftex-miktex.h:						-*- C++ -*-
   
   Copyright (C) 1998-2007 Christian Schenk

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

#include "pdftexdefs.h"

#if ! defined(C4PEXTERN)
#  define C4PEXTERN extern
#endif

#include "pdftexd.h"

#if ! defined(THEDATA)
#  define THEDATA(x) PDFTEXDATA.m_##x
#endif

#include "pdftex.rc"

#include <miktex/paths.h>
#include <miktex/etexapp.h>

namespace pdftex {
#include "pdftex.defaults.h"
}

#if ! defined(MIKTEXHELP_PDFTEX)
#  include <miktex/help.h>
#endif

/* _________________________________________________________________________

   pdfTeXApp
   _________________________________________________________________________ */

class PDFTEXCLASS

  : public MiKTeX::TeXAndFriends::ETeXApp

{
public:
  enum {
    OPT_DRAFTMODE = 10000,
    OPT_OUTPUT_FORMAT,
  };


public:
  virtual
  void
  MIKTEXMFCALL
  AddOptions ()
  {
    ETeXApp::AddOptions ();

    AddOption (MIKTEXTEXT("draftmode\0\
Switch on draft mode (generates no output)."),
	       OPT_DRAFTMODE);

    AddOption (MIKTEXTEXT("output-format\0Set the output format."),
	       OPT_OUTPUT_FORMAT,
	       required_argument,
	       MIKTEXTEXT("FORMAT"));
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
      case OPT_DRAFTMODE:
	THEDATA(pdfdraftmodeoption) = 1;
	THEDATA(pdfdraftmodevalue) = 1;
	break;
      case OPT_OUTPUT_FORMAT:
	THEDATA(pdfoutputoption) = 1;
	if (MiKTeX::Core::StringCompare(lpszOptArg, MIKTEXTEXT("dvi")) == 0)
	  {
	    THEDATA(pdfoutputvalue) = 0;
	  }
	else if (MiKTeX::Core::StringCompare(lpszOptArg, MIKTEXTEXT("pdf"))
		 == 0)
	  {
	    THEDATA(pdfoutputvalue) = 2;
	  }
	else
	  {
	    FatalError (MIKTEXTEXT("Unkown output option value."));
	  }
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

    GETPARAMCHECK (-1,
		   pdfmemsize,
		   pdf_mem_size,
		   pdftex::pdftex::pdf_mem_size());
    GETPARAMCHECK (-1,
		   objtabsize,
		   obj_tab_size,
		   pdftex::pdftex::obj_tab_size());
    GETPARAMCHECK (-1,
		   destnamessize,
		   dest_names_size,
		   pdftex::pdftex::dest_names_size());
    GETPARAMCHECK (-1,
		   pdfosbufsize,
		   pdf_os_buf_size,
		   pdftex::pdftex::pdf_os_buf_size());
    
    size_t nFonts = THEDATA(fontmax) + 1 - constfontbase;

    // pdftex
    Allocate (THEDATA(destnames), THEDATA(destnamessize));
    Allocate (THEDATA(objtab), THEDATA(objtabsize));
    Allocate (THEDATA(pdfcharused), nFonts);
    Allocate (THEDATA(pdffontattr), nFonts);
    Allocate (THEDATA(pdffontautoexpand), nFonts);
    Allocate (THEDATA(pdffontblink), nFonts);
    Allocate (THEDATA(pdffontefbase), nFonts);
    Allocate (THEDATA(pdffontelink), nFonts);
    Allocate (THEDATA(pdffontexpandratio), nFonts);
    Allocate (THEDATA(pdffontknacbase), nFonts);
    Allocate (THEDATA(pdffontknbcbase), nFonts);
    Allocate (THEDATA(pdffontknbsbase), nFonts);
    Allocate (THEDATA(pdffontlpbase), nFonts);
    Allocate (THEDATA(pdffontmap), nFonts);
    Allocate (THEDATA(pdffontnum), nFonts);
    Allocate (THEDATA(pdffontrpbase), nFonts);
    Allocate (THEDATA(pdffontshbsbase), nFonts);
    Allocate (THEDATA(pdffontshrink), nFonts);
    Allocate (THEDATA(pdffontsize), nFonts);
    Allocate (THEDATA(pdffontstbsbase), nFonts);
    Allocate (THEDATA(pdffontstep), nFonts);
    Allocate (THEDATA(pdffontstretch), nFonts);
    Allocate (THEDATA(pdffonttype), nFonts);
    Allocate (THEDATA(pdfmem), THEDATA(pdfmemsize));
    Allocate (THEDATA(pdfopbuf), pdfopbufsize);
    Allocate (THEDATA(pdfosbuf), THEDATA(pdfosbufsize));
    Allocate (THEDATA(pdfosobjnum), pdfosmaxobjs);
    Allocate (THEDATA(pdfosobjoff), pdfosmaxobjs);
    Allocate (THEDATA(vfdefaultfont), nFonts);
    Allocate (THEDATA(vfefnts), nFonts);
    Allocate (THEDATA(vfifnts), nFonts);
    Allocate (THEDATA(vflocalfontnum), nFonts);
    Allocate (THEDATA(vfpacketbase), nFonts);
  }

public:
  void
  FreeMemory ()
  {
    ETeXApp::FreeMemory ();

    // pdftex
    Free (THEDATA(destnames));
    Free (THEDATA(objtab));
    Free (THEDATA(pdfcharused));
    Free (THEDATA(pdffontattr));
    Free (THEDATA(pdffontautoexpand));
    Free (THEDATA(pdffontblink));
    Free (THEDATA(pdffontefbase));
    Free (THEDATA(pdffontelink));
    Free (THEDATA(pdffontexpandratio));
    Free (THEDATA(pdffontknacbase));
    Free (THEDATA(pdffontknbcbase));
    Free (THEDATA(pdffontknbsbase));
    Free (THEDATA(pdffontlpbase));
    Free (THEDATA(pdffontmap));
    Free (THEDATA(pdffontnum));
    Free (THEDATA(pdffontrpbase));
    Free (THEDATA(pdffontshbsbase));
    Free (THEDATA(pdffontshrink));
    Free (THEDATA(pdffontsize));
    Free (THEDATA(pdffontstbsbase));
    Free (THEDATA(pdffontstep));
    Free (THEDATA(pdffontstretch));
    Free (THEDATA(pdffonttype));
    Free (THEDATA(pdfmem));
    Free (THEDATA(pdfopbuf));
    Free (THEDATA(pdfosbuf));
    Free (THEDATA(pdfosobjnum));
    Free (THEDATA(pdfosobjoff));
    Free (THEDATA(vfdefaultfont));
    Free (THEDATA(vfefnts));
    Free (THEDATA(vfifnts));
    Free (THEDATA(vflocalfontnum));
    Free (THEDATA(vfpacketbase));
  }

public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
  {
    ETeXApp::Init (lpszProgramInvocationName);
#if defined(IMPLEMENT_TCX)
    EnableFeature (MiKTeX::TeXAndFriends::Feature::EightBitChars);
    EnableFeature (MiKTeX::TeXAndFriends::Feature::TCX);
#endif
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetMemoryDumpFileName ()
    const
  {
    return (MIKTEXTEXT("pdftex.fmt"));
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    return (MIKTEXTEXT("pdftex.pool"));
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetInitProgramName ()
    const
  {
    return (MIKTEXTEXT("pdfinitex"));
  }
  
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetVirginProgramName ()
    const
  {
    return (MIKTEXTEXT("pdfvirtex"));
  }
  
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  TheNameOfTheGame ()
    const
  {
    return (MIKTEXTEXT("pdfTeX"));
  }

public:
  virtual
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_PDFTEX);
  }
};

extern PDFTEXCLASS PDFTEXAPP;
#define THEAPP PDFTEXAPP
#include <miktex/etexapp.inl>

/* _________________________________________________________________________

   miktexptrequal (miktex_ptr_equal)
   _________________________________________________________________________ */

inline
bool
miktexptrequal(/*[in]*/ const void * ptr1,
	       /*[in]*/ const void * ptr2)
{
  return (ptr1 == ptr2);
}

/* _________________________________________________________________________

   Web2C Compatibility
   _________________________________________________________________________ */

#include <miktex/kpsemu.h>
#include "pdftex.h"

#define printid printID

inline
int
getbyte (/*[in]*/ bytefile & f)
{
  eightbits ret = *f;
  if (! feof(f))
    {
      get (f);
    }
  return (ret & 0xff);
}

#define zpdfosgetosbuf pdfosgetosbuf
#define zpdfbeginobj pdfbeginobj

#if defined(texbopenin)
#  undef texbopenin
#  define texbopenin(f) \
    open_input(&f.fileref(), kpse_tex_format, FOPEN_RBIN_MODE)
#endif

#if defined(vfbopenin)
#  undef vfbopenin
#  define vfbopenin(f) \
    miktexopenvffile(f, THEDATA(nameoffile)+1)
#endif

#define __attribute__(x)

/* _________________________________________________________________________

   Gloabel Variables
   _________________________________________________________________________ */

#if defined(COMPILING_PDFTEX_CC)
#  define MAKE_GLOBAL(type, name) type & name = PDFTEXDATA.m_##name;
#else
#  define MAKE_GLOBAL(type, name) extern type & name;
#endif

// special case: Web2C likes to add 1 to the nameoffile base address
#define nameoffile (&PDFTEXDATA.m_nameoffile[-1])

C4PEXTERN C4P_integer k;

MAKE_GLOBAL(C4P_boolean, pdfosmode);
MAKE_GLOBAL(C4P_integer*, vfefnts);
MAKE_GLOBAL(C4P_integer, fixedgentounicode);
MAKE_GLOBAL(C4P_integer, fixedpdfdraftmode);
MAKE_GLOBAL(C4P_integer, fontmax);
MAKE_GLOBAL(C4P_integer, objptr);
MAKE_GLOBAL(C4P_integer, pdfboxspecart);
MAKE_GLOBAL(C4P_integer, pdfboxspecbleed);
MAKE_GLOBAL(C4P_integer, pdfboxspeccrop);
MAKE_GLOBAL(C4P_integer, pdfboxspecmedia);
MAKE_GLOBAL(C4P_integer, pdfboxspectrim);
MAKE_GLOBAL(C4P_integer, pdfbufsize);
MAKE_GLOBAL(C4P_integer, pdflastbyte);
MAKE_GLOBAL(eightbits*, pdfbuf);
MAKE_GLOBAL(internalfontnumber*, vfifnts);
MAKE_GLOBAL(internalfontnumber, f);
MAKE_GLOBAL(strnumber*, fontname);
MAKE_GLOBAL(strnumber, outputfilename);
MAKE_GLOBAL(C4P_integer, fixedinclusioncopyfont);

// todo: use MAKE_GLOBAL
#define dim100bp THEDATA(dim100bp)
#define dim100in THEDATA(dim100in)
#define dim1bp THEDATA(dim1bp)
#define dim1in THEDATA(dim1in)
#define dim1inoverpkres THEDATA(dim1inoverpkres)
#define fixedcompresslevel THEDATA(fixedcompresslevel)
#define fixeddecimaldigits THEDATA(fixeddecimaldigits)
#define fixedgamma THEDATA(fixedgamma)
#define fixedimageapplygamma THEDATA(fixedimageapplygamma)
#define fixedimagegamma THEDATA(fixedimagegamma)
#define fixedimagehicolor THEDATA(fixedimagehicolor)
#define fixedmovechars THEDATA(fixedmovechars)
#define fixedpdfminorversion THEDATA(fixedpdfminorversion)
#define fixedpkresolution THEDATA(fixedpkresolution)
#define fontbc THEDATA(fontbc)
#define fontdsize THEDATA(fontdsize)
#define fontec THEDATA(fontec)
#define fontptr THEDATA(fontptr)
#define fontsize THEDATA(fontsize)
#define fontused THEDATA(fontused)
#define formatident THEDATA(formatident)
#define jobname THEDATA(jobname)
#define lasttokensstring THEDATA(lasttokensstring)
#define objtab THEDATA(objtab)
#define onehundredbp THEDATA(onehundredbp)
#define pdfcharmap THEDATA(pdfcharmap)
#define pdfcharused THEDATA(pdfcharused)
#define pdfcryptdate THEDATA(pdfcryptdate)
#define pdfcryptid1 THEDATA(pdfcryptid1)
#define pdfcryptid2 THEDATA(pdfcryptid2)
#define pdfcrypting THEDATA(pdfcrypting)
#define pdfcryptobjnum THEDATA(pdfcryptobjnum)
#define pdfcryptovalue THEDATA(pdfcryptovalue)
#define pdfcryptpermit THEDATA(pdfcryptpermit)
#define pdfcryptuvalue THEDATA(pdfcryptuvalue)
#define pdfcryptversion THEDATA(pdfcryptversion)
#define pdffile THEDATA(pdffile)
#define pdffontattr THEDATA(pdffontattr)
#define pdffontefbase THEDATA(pdffontefbase)
#define pdffontexpandfont THEDATA(pdffontexpandfont)
#define pdffontexpandratio THEDATA(pdffontexpandratio)
#define pdffontknacbase THEDATA(pdffontknacbase)
#define pdffontknbcbase THEDATA(pdffontknbcbase)
#define pdffontknbsbase THEDATA(pdffontknbsbase)
#define pdffontlpbase THEDATA(pdffontlpbase)
#define pdffontmap THEDATA(pdffontmap)
#define pdffontrpbase THEDATA(pdffontrpbase)
#define pdffontshbsbase THEDATA(pdffontshbsbase)
#define pdffontsize THEDATA(pdffontsize)
#define pdffontstbsbase THEDATA(pdffontstbsbase)
#define pdfgone THEDATA(pdfgone)
#define pdfimageprocset THEDATA(pdfimageprocset)
#define pdflastpdfboxspec THEDATA(pdflastpdfboxspec)
#define pdfmem THEDATA(pdfmem)
#define pdfptr THEDATA(pdfptr)
#define pdfsaveoffset THEDATA(pdfsaveoffset)
#define pdfstreamlength THEDATA(pdfstreamlength)
#define pdftexbanner THEDATA(pdftexbanner)
#define pkscalefactor THEDATA(pkscalefactor)
#define poolptr THEDATA(poolptr)
#define poolsize THEDATA(poolsize)
#define strpool THEDATA(strpool)
#define strstart THEDATA(strstart)
#define tmpf THEDATA(tmpf)
#define vfpacketbase THEDATA(vfpacketbase)
#define vfpacketlength THEDATA(vfpacketlength)
