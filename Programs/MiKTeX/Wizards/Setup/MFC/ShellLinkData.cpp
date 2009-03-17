/* ShellLinkData.cpp: shell link data

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

#include "StdAfx.h"

#include "Setup.h"
#include "ShellLinkData.h"

#define EXEPATH(name) \
 T_("%MIKTEX_INSTALL%\\") MIKTEX_PATH_BIN_DIR "\\" name

#define CONFIGPATH(name) \
 T_("%MIKTEX_INSTALL%\\") MIKTEX_PATH_MIKTEX_CONFIG_DIR "\\" name

#define DOCPATH(name) \
 T_("%MIKTEX_INSTALL%\\") MIKTEX_PATH_MIKTEX_DOC_DIR "\\" name

const ShellLinkData shellLinks[] = {

  // "Previewer"
  {
    false,
    0,
    IDS_DVI_VIEWER,
    EXEPATH(MIKTEX_YAP_EXE),
    LD_USESHOWCMD,
    0,
    0,
    0,
    0,
    0,
    SW_SHOWNORMAL,
    0,
  },

  // "Maintenance (current user)->Settings"
  {
    false,
    IDS_MAINTENANCE,
    IDS_OPTIONS,
    EXEPATH(MIKTEX_MO_EXE),
    LD_USESHOWCMD,
    0,
    0,
    0,
    0,
    0,
    SW_SHOWNORMAL,
    0,
  },

  // "Maintenance (current user)->Update"
  {
    false,
    IDS_MAINTENANCE,
    IDS_UPDATEWIZ,
    EXEPATH(MIKTEX_COPYSTART_EXE),
    LD_USESHOWCMD | LD_USEARGS | LD_USEICON,
    0,
    "\"" CONFIGPATH(MIKTEX_UPDATE_DAT) "\"",
    CONFIGPATH(MIKTEX_UPDATE_DAT),
    0,
    0,
    SW_SHOWNORMAL,
    0,
  },

  // "Maintenance (current user)->Browse Packages"
  {
    false,
    IDS_MAINTENANCE,
    IDS_MPM,
    EXEPATH(MIKTEX_MPM_MFC_EXE),
    LD_USESHOWCMD,
    0,
    0,
    0,
    0,
    0,
    SW_SHOWNORMAL,
    0,
  },

  // "Maintenance (system-wide)->Settings"
  {
    false,
    IDS_MAINTENANCE,
    IDS_OPTIONS,
    EXEPATH(MIKTEX_MO_ADMIN_EXE),
    LD_IFCOMMON | LD_USESHOWCMD,
    0,
    0,
    0,
    0,
    0,
    SW_SHOWNORMAL,
    0,
  },

  // "Maintenance (system-wide)->Update"
  {
    false,
    IDS_MAINTENANCE,
    IDS_UPDATEWIZ,
    EXEPATH(MIKTEX_COPYSTART_ADMIN_EXE),
    LD_IFCOMMON | LD_USESHOWCMD | LD_USEARGS | LD_USEICON,
    0,
    "\"" CONFIGPATH(MIKTEX_UPDATE_ADMIN_DAT) "\"",
    CONFIGPATH(MIKTEX_UPDATE_ADMIN_DAT),
    0,
    0,
    SW_SHOWNORMAL,
    0,
  },

  // "Maintenance (system-wide)->Browse Packages"
  {
    false,
    IDS_MAINTENANCE,
    IDS_MPM,
    EXEPATH(MIKTEX_MPM_ADMIN_MFC_EXE),
    LD_IFCOMMON | LD_USESHOWCMD,
    0,
    0,
    0,
    0,
    0,
    SW_SHOWNORMAL,
    0,
  },

  // "Help->Manual"
  {
    false,
    IDS_HELP,
    IDS_LOCAL_GUIDE,
    DOCPATH(MIKTEX_MAIN_HELP_FILE),
    LD_USESHOWCMD,
    0,
    0,
    0,
    0,
    0,
    SW_SHOWNORMAL,
    0,
  },

  // "Help->FAQ"
  {
    false,
    IDS_HELP,
    IDS_FAQ,
    DOCPATH(MIKTEX_FAQ_HELP_FILE),
    LD_USESHOWCMD,
    0,
    0,
    0,
    0,
    0,
    SW_SHOWNORMAL,
    0,
  },

  // "MiKTeX on the Web->MiKTeX Project Page"
  {
    true,
    IDS_MIKTEX_ON_THE_WEB,
    IDS_PROJECT_PAGE,
    MIKTEX_URL_WWW,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
  },

  // "MiKTeX on the Web->Support"
  {
    true,
    IDS_MIKTEX_ON_THE_WEB,
    IDS_SUPPORT,
    MIKTEX_URL_WWW_SUPPORT,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
  },

  // "MiKTeX on the Web->Give back"
  {
    true,
    IDS_MIKTEX_ON_THE_WEB,
    IDS_GIVE_BACK,
    MIKTEX_URL_WWW_GIVE_BACK,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
  },

  // "MiKTeX on the Web->Known Issues"
  {
    true,
    IDS_MIKTEX_ON_THE_WEB,
    IDS_KNOWN_ISSUES,
    MIKTEX_URL_WWW_KNOWN_ISSUES,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
  },

};

size_t nShellLinks = sizeof(shellLinks) / sizeof(shellLinks[0]);
