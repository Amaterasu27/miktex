/* Extractor.cpp:

   Copyright (C) 2001-2007 Christian Schenk

   This file is part of MiKTeX Extractor.

   MiKTeX Extractor is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.
   
   MiKTeX Extractor is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Extractor; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"

#include "internal.h"

#include "CabExtractor.h"
#include "TarBzip2Extractor.h"

using namespace MiKTeX::Core;
using namespace std;

#if defined (MIKTEX_WINDOWS)
#  include "win/winCabExtractor.h"
static bool USE_WINDOWS_CABEXTRACTOR = false;
#endif

/* _________________________________________________________________________

   Extractor::~Extractor
   _________________________________________________________________________ */

Extractor::~Extractor ()
{
}

/* _________________________________________________________________________

   Extractor::CreateExtractor
   _________________________________________________________________________ */

Extractor *
Extractor::CreateExtractor (/*[in]*/ ArchiveFileType archiveFileType)
{
  switch (archiveFileType.Get())
    {
    case ArchiveFileType::MSCab:
#if defined (MIKTEX_WINDOWS)
      if (USE_WINDOWS_CABEXTRACTOR)
	{
	  return (new winCabExtractor);
	}
#endif
      return (new CabExtractor);
    case ArchiveFileType::TarBzip2:
      return (new TarBzip2Extractor);
    default:
      UNIMPLEMENTED (T_("Extractor::CreateExtractor"));
    }
}
