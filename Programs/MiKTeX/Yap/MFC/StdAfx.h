/* StdAfx.h: includes precompiled header files			-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <miktex/first.h>

#define VC_EXTRALEAN

#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>
#include <afxmt.h>

#if ! defined(_AFX_NO_AFXCMN_SUPPORT)
#  include <afxcmn.h>
#endif

#include <winspool.h>
#include <shlwapi.h>

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cmath>

#include <algorithm>
#include <stack>
#include <map>
#include <vector>
#include <string>

#include <malloc.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <sys/stat.h>
#include <shellapi.h>

#include <htmlhelp.h>

#include <ddeml.h>

#include <bmeps.h>

#include <miktex/DibChunker.h>
#include <miktex/ErrorDialog.h>
#include <miktex/ProgressDialog.h>
#include <miktex/TextViewerDialog.h>
#include <miktex/core.h>
#include <miktex/dvi.h>
#include <miktex/paths.h>
#include <miktex/reg.h>
#include <miktex/trace.h>
#include <miktex/version.h>

#include <popt-miktex.h>

#include <afxdlgs.h>

using namespace MiKTeX::Core;
using namespace MiKTeX::DVI;
using namespace MiKTeX::Graphics;
using namespace MiKTeX::UI;
using namespace MiKTeX;
using namespace std;
