/* StdAfx.h:							-*- C++ -*-

   Copyright (C) 2003-2007 Christian Schenk

   This file is part of MTPrint.

   MTPrint is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MTPrint is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MTPrint; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <malloc.h>

#include "mtprint-version.h"

#include <miktex/Graphics/DibChunker>
#include <miktex/App/Application>
#include <miktex/Core/Core>
#include <miktex/Core/Debug>
#include <miktex/Core/Help>
#include <miktex/Core/Paths>
#include <miktex/Core/Trace>
#include <popt-miktex.h>

#include <iomanip>
#include <iostream>
#include <vector>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX::Graphics;
using namespace MiKTeX;
using namespace std;
