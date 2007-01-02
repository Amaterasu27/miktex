/* WebSession.cpp:

   Copyright (C) 2001-2006 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#if defined(MIKTEX_WINDOWS)
#  include "win/winWebSession.h"
static bool USE_WININET = false;
#endif

#if defined(HAVE_LIBCURL)
#  include "CurlWebSession.h"
#endif

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

/* _________________________________________________________________________

   WebSession::~WebSession
   _________________________________________________________________________ */

WebSession::~WebSession ()
{
}

/* _________________________________________________________________________

   WebFile::~WebFile
   _________________________________________________________________________ */

WebFile::~WebFile ()
{
}

/* _________________________________________________________________________

   WebSession::Create
   _________________________________________________________________________ */

WebSession *
WebSession::Create ()
{
#if defined (MIKTEX_WINDOWS)
  if (USE_WININET)
    {
      return (new winWebSession);
    }
#endif
#if defined(HAVE_LIBCURL)
  return (new CurlWebSession);
#else
#  warning Unimplemented: WebSession::Create()
  FATAL_MIKTEX_ERROR (T_("WebSession::Create"),
		      T_("libCURL does not seem to be available."),
		      0);
#endif
}
