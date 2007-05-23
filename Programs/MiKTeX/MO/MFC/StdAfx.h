/* StdAfx.h: pre-compiled header stuff				-*- C++ -*-

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of MiKTeX Options.

   MiKTeX Options is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX Options is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Options; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#pragma once

#define VC_EXTRALEAN

// C4786: identifier was truncated to 'number'
//        characters in the debug information
// C4251: 'identifier' : class 'type' needs to
//        have dll-interface to be used by clients
//        of class 'type2'
#pragma warning ( disable : 4786 4251 )

#if ! defined(WINVER)
#  define WINVER 0x0600
#endif

#if ! defined(_WIN32_WINNT)
#  define _WIN32_WINNT 0x0600
#endif						

#if ! defined(_WIN32_WINDOWS)
#  define _WIN32_WINDOWS 0x0600
#endif

#if ! defined(_WIN32_IE)
#  define _WIN32_IE 0x0700
#endif

#include <afxwin.h>
#include <afxext.h>
#include <afxdtctl.h>
#include <afxmt.h>
#include <afxtempl.h>

#include <HtmlHelp.h>

#include <shlwapi.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#  include <afxcmn.h>
#endif

#include "mo-version.h"

#include <miktex/core.h>
#include <miktex/paths.h>
#include <miktex/reg.h>
#include <miktex/win/DllProc.h>

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <miktex/ErrorDialog.h>
#include <miktex/PackageTreeCtrl.h>
#include <miktex/ProgressDialog.h>
#include <miktex/SiteWizSheet.h>
#include <miktex/TextViewerDialog.h>
#include <miktex/UpdateDialog.h>

#if defined(_UNICODE)
#  if defined(_M_IX86)
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  elif defined(_M_IA64)
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  elif defined(_M_X64)
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  else
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  endif
#endif

using namespace MiKTeX;
using namespace MiKTeX::Packages;
using namespace MiKTeX::UI;
using namespace MiKTeX::Core;
using namespace std;
