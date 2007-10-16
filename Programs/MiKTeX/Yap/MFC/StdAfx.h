/* StdAfx.h: includes precompiled header files			-*- C++ -*-

   Copyright (C) 1996-2007 Christian Schenk

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

#if ! defined(VC_EXTRALEAN)
#  define VC_EXTRALEAN
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>
#include <afxmt.h>

#if ! defined(_AFX_NO_AFXCMN_SUPPORT)
#  include <afxcmn.h>
#endif

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
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

#include "yap-version.h"

#include <miktex/DibChunker.h>
#include <MiKTeX/UI/MFC/ErrorDialog>
#include <MiKTeX/UI/MFC/ProgressDialog>
#include <MiKTeX/UI/MFC/TextViewerDialog>
#include <MiKTeX/Core/Core>
#include <miktex/debug.h>
#include <miktex/dvi.h>
#include <miktex/paths.h>
#include <miktex/reg.h>
#include <miktex/trace.h>
#include <MiKTeX/Core/win/DllProc>

#include <popt-miktex.h>

#include <afxdlgs.h>

using namespace MiKTeX::Core;
using namespace MiKTeX::DVI;
using namespace MiKTeX::Graphics;
using namespace MiKTeX::UI::MFC;
using namespace MiKTeX;
using namespace std;
