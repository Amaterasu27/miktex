/* pdftex-miktex.h:						-*- C++ -*-
   
   Copyright (C) 1998-2006 Christian Schenk

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

#if defined(MIKTEX_PDFTEX)
#  include "pdftexdefs.h"
#endif

#ifndef C4PEXTERN
#  define C4PEXTERN extern
#endif

#if defined(MIKTEX_PDFTEX)
#include "pdftexd.h"
#endif

#ifndef THEDATA
#  define THEDATA(x) PDFTEXDATA.m_##x
#endif

#if defined(MIKTEX_PDFTEX)
#  include "pdftex.rc"
#endif

#ifdef __cplusplus

#include <miktex/paths.h>
#include <miktex/texapp.h>

#ifndef MIKTEX_VIRTUAL_TEXAPP
#  define MIKTEX_VIRTUAL_TEXAPP
#endif

#ifndef MIKTEXHELP_PDFTEX
#  include <miktex/help.h>
#endif

using namespace MiKTeX::TeXAndFriends;

class PDFTEXCLASS

  : MIKTEX_VIRTUAL_TEXAPP public TeXApp

{
 public:
  void
  AllocateMemory ()
  {
    TeXApp::AllocateMemory ();

    GETPARAM (-1, pdfmemsize, pdf_mem_size, 65536);
    Allocate (THEDATA(pdfmem), THEDATA(pdfmemsize));
    
    GETPARAM (-1, objtabsize, obj_tab_size, 300000);
    Allocate (THEDATA(objtab), THEDATA(objtabsize));

    GETPARAM (-1, destnamessize, dest_names_size, 300000);
    Allocate (THEDATA(destnames), THEDATA(destnamessize));
    
    Allocate (THEDATA(pdfcharused), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdfcharmap), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontsize), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontnum), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontmap), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffonttype), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontblink), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontelink), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontstretch), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontshrink), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontstep), THEDATA(fontmax) + 1 - constfontbase);
    
    Allocate (THEDATA(pdffontexpandratio),
	      THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontautoexpand),
	      THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(vfpacketbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(vfdefaultfont), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(vflocalfontnum), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(vfefnts), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(vfifnts), THEDATA(fontmax) + 1 - constfontbase);

    Allocate (THEDATA(pdffontattr), THEDATA(fontmax) + 1 - constfontbase);

    Allocate (THEDATA(pdffontlpbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontrpbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(pdffontefbase), THEDATA(fontmax) + 1 - constfontbase);
  }

 public:
  void
  FreeMemory ()
  {
    TeXApp::FreeMemory ();
    Free (THEDATA(pdfmem));
    Free (THEDATA(objtab));
    Free (THEDATA(destnames));
    Free (THEDATA(pdfcharused));
    Free (THEDATA(pdfcharmap));
    Free (THEDATA(pdffontsize));
    Free (THEDATA(pdffontnum));
    Free (THEDATA(pdffontmap));
    Free (THEDATA(pdffonttype));
    Free (THEDATA(pdffontblink));
    Free (THEDATA(pdffontelink));
    Free (THEDATA(pdffontstretch));
    Free (THEDATA(pdffontshrink));
    Free (THEDATA(pdffontstep));
    Free (THEDATA(pdffontexpandratio));
    Free (THEDATA(pdffontautoexpand));
    Free (THEDATA(vfpacketbase));
    Free (THEDATA(vfdefaultfont));
    Free (THEDATA(vflocalfontnum));
    Free (THEDATA(vfefnts));
    Free (THEDATA(vfifnts));
    Free (THEDATA(pdffontlpbase));
    Free (THEDATA(pdffontrpbase));
    Free (THEDATA(pdffontefbase));
  }

 public:
  void __stdcall
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
  {
    TeXApp::Init (lpszProgramInvocationName);
#ifdef IMPLEMENT_TCX
    EnableFeature (Feature::TCX);
#endif
  }

 public:
  LPCTSTR __stdcall
  GetMemoryDumpFileName () const
  {
    return (_T("pdftex.fmt"));
  }

 public:
  LPCTSTR __stdcall
  GetPoolFileName () const
  {
    return (_T("pdftex.pool"));
  }

 public:
  LPCTSTR __stdcall
  GetInitProgramName () const
  {
    return (_T("pdfinitex"));
  }
  
 public:
  LPCTSTR __stdcall
  GetVirginProgramName () const
  {
    return (_T("pdfvirtex"));
  }
  
 public:
  LPCTSTR __stdcall
  TheNameOfTheGame () const
  {
    return (_T("pdfTeX"));
  }

 public:
  DWORD __stdcall
  GetHelpId () const
  {
    return (MIKTEXHELP_PDFTEX);
  }
};

#if defined(MIKTEX_PDFTEX)
extern PDFTEXCLASS PDFTEXAPP;
#define THEAPP PDFTEXAPP
#include <miktex/texapp.inl>
#endif

inline int
getbyte (/*[in]*/ bytefile & f)
{
  eightbits ret = *f;
  if (! feof(f))
    {
      get (f);
    }
  return (ret & 0xff);
}

#endif // __cplusplus

#include "miktex/kpsemu.h"

//#ifdef __cplusplus
//extern "C" {
//#endif
#define dim100bp THEDATA(dim100bp)
#define dim100in THEDATA(dim100in)
#define dim1bp THEDATA(dim1bp)
#define dim1in THEDATA(dim1in)
#define dim1inoverpkres THEDATA(dim1inoverpkres)
#define fixedcompresslevel THEDATA(fixedcompresslevel)
#define fixeddecimaldigits THEDATA(fixeddecimaldigits)
#define fixedmovechars THEDATA(fixedmovechars)
#define fixedpkresolution THEDATA(fixedpkresolution)
#define fontbc THEDATA(fontbc)
#define fontdsize THEDATA(fontdsize)
#define fontec THEDATA(fontec)
#define fontname THEDATA(fontname)
#define fontptr THEDATA(fontptr)
#define fontsize THEDATA(fontsize)
#define fontused THEDATA(fontused)
#define formatident THEDATA(formatident)
#define jobname THEDATA(jobname)
#define lasttokensstring THEDATA(lasttokensstring)
#define nameoffile THEDATA(nameoffile)
#define objptr THEDATA(objptr)
#define objtab THEDATA(objtab)
#define onehundredbp THEDATA(onehundredbp)
#define pdfbuf THEDATA(pdfbuf)
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
#define pdffontlpbase THEDATA(pdffontlpbase)
#define pdffontmap THEDATA(pdffontmap)
#define pdffontrpbase THEDATA(pdffontrpbase)
#define pdffontsize THEDATA(pdffontsize)
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
#define fixedpdfminorversion THEDATA(fixedpdfminorversion)
#define fixedimagehicolor THEDATA(fixedimagehicolor)
#define fixedimageapplygamma THEDATA(fixedimageapplygamma)
#define fixedgamma THEDATA(fixedgamma)
#define fixedimagegamma THEDATA(fixedimagegamma)
//#ifdef __cplusplus
//}
//#endif

#include "pdftex.h"

#ifdef texbopenin
#undef texbopenin
#define texbopenin(f) \
    open_input(&f.fileref(), kpse_tex_format, FOPEN_RBIN_MODE)
#endif

#ifdef vfbopenin
#undef vfbopenin
#define vfbopenin(f) \
    miktexopenvffile(f, THEDATA(nameoffile))
#endif

#ifndef __cplusplus
#undef pdffile
#define pdffile C4P_FILE_FP(THEDATA(pdffile))
#endif
