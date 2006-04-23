/* StdAfx.h:							-*- C++ -*-

   Copyright (C) 2002-2006 Christian Schenk

   This file is part of MiKTeX Update Wizard.

   MiKTeX Update Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   MiKTeX Update Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Update Wizard; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#define VC_EXTRALEAN

#include <afxwin.h>
#include <afxinet.h>
#include <afxext.h>
#include <afxdtctl.h>
#include <afxmt.h>

#if ! defined(_AFX_NO_AFXCMN_SUPPORT)
#  include <afxcmn.h>
#endif

#include <algorithm>
#include <vector>

#include <cfg.h>
#include <getopt.h>
#include <miktex/core.h>
#include <miktex/mpm.h>
#include <miktex/version.h>
#include <miktex/paths.h>
#include <miktex/reg.h>

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeX;
using namespace std;
