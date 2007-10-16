/* mktex.h: MiKTeX Maker Library

   Copyright (C) 1998-2006 Christian Schenk

   This file is part of the MiKTeX Maker Library.

   The MiKTeX Maker Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Maker Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Maker Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#include <MiKTeX/Core/Text>

#if ! defined(MKTEXAPI)
#  if defined(_MSC_VER)
#    define MKTEXAPI(func) extern "C" __declspec(dllimport) int __cdecl func
#  else
#    define MKTEXAPI(func) extern "C" int func
#  endif
#endif

MKTEXAPI(makebase) (/*[in]*/ int			argc,
		    /*[in]*/ const MIKTEXCHAR **	argv);

MKTEXAPI(makefmt) (/*[in]*/ int			argc,
		   /*[in]*/ const MIKTEXCHAR **	argv);

MKTEXAPI(makemem) (/*[in]*/ int			argc,
		   /*[in]*/ const MIKTEXCHAR **	argv);

MKTEXAPI(makemf) (/*[in]*/ int			argc,
		  /*[in]*/ const MIKTEXCHAR **	argv);

MKTEXAPI(makepk) (/*[in]*/ int			argc,
		  /*[in]*/ const MIKTEXCHAR **	argv);

MKTEXAPI(maketfm) (/*[in]*/ int			argc,
		   /*[in]*/ const MIKTEXCHAR **	argv);
