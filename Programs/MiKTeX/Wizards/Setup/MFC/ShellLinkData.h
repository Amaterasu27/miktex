/* ShellLinkData.h: shell link data				-*- C++ -*-

   Copyright (C) 1999-2009 Christian Schenk

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

#pragma once

struct ShellLinkData  
{
  bool isUrl;
  UINT subFolderID;
  UINT nameID;
  const char * lpszPathName;
  unsigned long flags;
  const char * lpszDescription;
  const char * lpszArgs;
  const char * lpszIconPath;
  int iconIndex;
  const char * lpszWorkingDir;
  int showCmd;
  WORD hotKey;
};

#define LD_USEDESC     0x00000001
#define LD_USEARGS     0x00000002
#define LD_USEICON     0x00000004
#define LD_USEWORKDIR  0x00000008
#define LD_USEHOTKEY   0x00000010
#define LD_USESHOWCMD  0x00000020
#define LD_IFCOMMON    0x00000040

extern const ShellLinkData shellLinks[];
extern size_t nShellLinks;
