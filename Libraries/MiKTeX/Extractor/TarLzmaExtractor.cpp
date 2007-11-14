/* TarLzmaExtractor.cpp:

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

#include "TarLzmaExtractor.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Extractor;
using namespace std;

/* _________________________________________________________________________

   TarLzmaExtractor::Extract
   _________________________________________________________________________ */

void
TarLzmaExtractor::Extract (/*[in]*/ const PathName &	tarlzmaPath,
			   /*[in]*/ const PathName &	destDir,
			   /*[in]*/ bool		makeDirectories,
			   /*[in]*/ IExtractCallback *	pCallback,
			   /*[in]*/ const char *	lpszPrefix)
{
  traceStream->WriteFormattedLine ("libextractor",
				   T_("extracting %s"),
				   Q_(tarlzmaPath));

  FileStream stream (File::Open(tarlzmaPath,
				FileMode::Open,
				FileAccess::Read,
				false));

  LzmaStream lzmaStream (stream, true);

  TarExtractor::Extract (&lzmaStream,
			 destDir,
			 makeDirectories,
			 pCallback,
			 lpszPrefix);
  lzmaStream.Close ();

  stream.Close ();
}
