/* miktex/TeXAndFriends/Prototypes.h:				-*- C++ -*-

   Copyright (C) 1996-2008 Christian Schenk

   This file is part of the MiKTeX TeXMF Library.

   The MiKTeX TeXMF Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX TeXMF Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX TeXMF Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#include <miktex/Core/First>

#include <miktex/Core/Core>

#if ! defined(GUARD_EA5A87F23904AC44BC85743B1568911C_)
#define GUARD_EA5A87F23904AC44BC85743B1568911C_

// DLL import/export switch
#if ! defined(B8C7815676699B4EA2DE96F0BD727276)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define MIKTEXMFEXPORT __declspec(dllimport)
#  else
#    define MIKTEXMFEXPORT
#  endif
#endif

// API decoration for exported member functions and data
#define MIKTEXAPI_(type, cc) MIKTEXMFEXPORT type cc
#define MIKTEXMFTHISAPI(type) MIKTEXAPI_(type, MIKTEXTHISCALL)
#define MIKTEXMFCEEAPI(type) MIKTEXAPI_(type, MIKTEXCEECALL)
#define MIKTEXMFDATA(type) MIKTEXMFEXPORT type

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

const unsigned long ICT_TCX = 1;
const unsigned long ICT_8BIT = 2;

MIKTEXMFCEEAPI(bool)
InitializeCharTables (/*[in]*/ unsigned long	flags,
		      /*[in]*/ const char *	lpszFileName,
		      /*[out]*/ void *		pChar,
		      /*[out]*/ void *		pOrd,
		      /*[out]*/ void *		pPrintable );

MIKTEXMFCEEAPI(bool)
OpenMAPFile (/*[in]*/ void *		p,
	     /*[in]*/ const char *	lpszName);

MIKTEXMFCEEAPI(bool)
OpenMETAFONTFile (/*[in]*/ void *	p,
		  /*[in]*/ const char *	lpszName);

MIKTEXMFCEEAPI(bool)
OpenTFMFile (/*[in]*/ void *		p,
	     /*[in]*/ const char *	lpszName);

MIKTEXMFCEEAPI(bool)
OpenVFFile (/*[in]*/ void *		p,
	    /*[in]*/ const char *	lpszName);

MIKTEXMFCEEAPI(bool)
OpenXFMFile (/*[in]*/ void *		p,
	     /*[in]*/ const char *	lpszName);

MIKTEXMFCEEAPI(int)
OpenXVFFile (/*[in]*/ void *		p,
	     /*[in]*/ const char *	lpszName);

MIKTEXMFCEEAPI(bool)
Write18 (/*[in]*/ const char *		lpszCommand,
	 /*[in]*/ unsigned long *	lpExitCode = 0);

MIKTEXAPI_(int, __stdcall)
TakeFraction (/*[in]*/ int	p,
	      /*[in]*/ int	q,
	      /*[in]*/ bool &	arithError);

MIKTEXAPI_(int, __stdcall)
TakeScaled (/*[in]*/ int	p,
	    /*[in]*/ int	q,
	    /*[in]*/ bool &	arithError);

MIKTEXAPI_(int, __stdcall)
MakeFraction (/*[in]*/ int	p,
	      /*[in]*/ int	q,
	      /*[in]*/ bool &	arithError);

MIKTEXAPI_(int, __stdcall)
MakeScaled (/*[in]*/ int	p,
	    /*[in]*/ int	q,
	    /*[in]*/ bool &	arithError);

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif