/* StdAfx.h: pre-compiled header stuff				-*- C++ -*-

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of the Remove MiKTeX! Wizard.

   The Remove MiKTeX! Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The Remove MiKTeX! Wizard is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the Remove MiKTeX! Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#pragma once

#define VC_EXTRALEAN

#include <afxwin.h>
#include <afxext.h>
#include <afxdtctl.h>
#include <afxmt.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif

#include <regstr.h>
#include <shlwapi.h>

#include <vector>
#include <set>
#include <algorithm>
#include <string>

#include "Remove-version.h"

#include <miktex/core.h>
#include <miktex/mpm.h>
#include <miktex/paths.h>
#include <miktex/reg.h>

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeX;
using namespace std;
