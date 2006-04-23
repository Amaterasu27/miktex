/* ObsoleteFiles.cpp: obsolete files

   Copyright (C) 1999-2006 Christian Schenk

   This file is part of the MiKTeX Setup Wizard.

   The MiKTeX Setup Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Setup Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Setup Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"
#include "Setup.h"

const MIKTEXCHAR * g_apszObsoleteFiles[] =
{
  MIKTEX_PATH_BIN_DIR T_("\\bigtex.exe"),
  MIKTEX_PATH_BIN_DIR T_("\\biglatex.exe"),
  MIKTEX_PATH_BIN_DIR T_("\\hugetex.exe"),
  MIKTEX_PATH_BIN_DIR T_("\\hugelatex.exe"),
  MIKTEX_PATH_BIN_DIR T_("\\mfc42.dll"),
  MIKTEX_PATH_BIN_DIR T_("\\msvcp50.dll"),
  MIKTEX_PATH_BIN_DIR T_("\\msvcp60.dll"),
  MIKTEX_PATH_BIN_DIR T_("\\msvcrt.dll"),
  0,
};
