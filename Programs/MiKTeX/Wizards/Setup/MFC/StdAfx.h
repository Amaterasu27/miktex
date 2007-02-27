/* StdAfx.h: pre-compiled header stuff				-*- C++ -*-

   Copyright (C) 1999-2007 Christian Schenk

   This file is part of MiKTeX Setup Wizard.

   MiKTeX Setup Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   MiKTeX Setup Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Setup Wizard; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#pragma once

#define VC_EXTRALEAN

#include <afxwin.h>
#include <afxinet.h>
#include <afxext.h>
#include <afxdtctl.h>
#include <afxmt.h>
#include <afxtempl.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#  include <afxcmn.h>
#endif

#include <intshcut.h>
#define SECURITY_WIN32
#include <Security.h>

#include <regstr.h>
#include <shlobj.h>

#include <comdef.h>
#include <comip.h>

#include <algorithm>
#include <vector>

#include <getopt.h>

#include <miktex/core.h>
#include <miktex/mpm.h>
#include <miktex/version.h>
#include <miktex/urls.h>
#include <miktex/paths.h>
#include <miktex/reg.h>
#include <miktex/win/DllProc.h>

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeX;
using namespace std;
