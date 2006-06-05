/* miktex/reg.h: MiKTeX settings

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#include <miktex/version.h>

#if ! defined(A5D53972CA7CCD40B4DB3ABBC0D957AF)
#define A5D53972CA7CCD40B4DB3ABBC0D957AF

#include <miktex/char.h>

#if defined(MIKTEX_WINDOWS)
#  define MIKTEX_REGPATH_COMPANY \
  MIKTEXTEXT("Software\\MiKTeX.org")
#  define MIKTEX_REGPATH_PRODUCT \
  MIKTEXTEXT("Software\\MiKTeX.org\\MiKTeX")
#  define MIKTEX_REGPATH_SERIES \
  MIKTEXTEXT("Software\\MiKTeX.org\\MiKTeX\\") MIKTEXTEXT(MIKTEX_SERIES_STR)
#endif

#define MIKTEX_GPL_GHOSTSCRIPT \
  MIKTEXTEXT("Software\\MiKTeX GPL Ghostscript")

#define MIKTEX_REGKEY_CORE \
  MIKTEXTEXT("Core")

#define MIKTEX_REGVAL_TRACE_OPTIONS \
  MIKTEXTEXT("Trace")

#define MIKTEX_REGKEY_PACKAGE_MANAGER \
  MIKTEXTEXT("MPM")

#define MIKTEX_REGKEY_BIBTEX \
  MIKTEXTEXT("BibTeX")

#define MIKTEX_REGKEY_TEX \
  MIKTEXTEXT("TeX")

#define MIKTEX_REGKEY_WEB \
  MIKTEXTEXT("WEB")

#define MIKTEX_REGKEY_METAPOST \
  MIKTEXTEXT("MetaPost")

#define MIKTEX_REGKEY_MAKEFMT \
  MIKTEXTEXT("MakeFMT")

#define MIKTEX_REGKEY_MAKEPK \
  MIKTEXTEXT("MakePk")

#define MIKTEX_REGKEY_MAKETFM \
  MIKTEXTEXT("MakeTFM")

#define MIKTEX_REGKEY_MAKEMEM \
  MIKTEXTEXT("MakeMEM")

#define MIKTEX_REGKEY_MAKEBASE \
  MIKTEXTEXT("MakeBase")

#define MIKTEX_REGVAL_ROOTS \
  MIKTEXTEXT("Roots")

#define MIKTEX_REGVAL_INSTALL \
  MIKTEXTEXT("Install")

#define MIKTEX_REGVAL_USER_DATA \
  MIKTEXTEXT("UserData")

#define MIKTEX_REGVAL_COMMON_DATA \
  MIKTEXTEXT("CommonData")

#define MIKTEX_REGVAL_USER_CONFIG	\
  MIKTEXTEXT("UserConfig")

#define MIKTEX_REGVAL_COMMON_CONFIG	\
  MIKTEXTEXT("CommonConfig")

#define MIKTEX_REGVAL_STARTUP_FILE \
  MIKTEXTEXT("StartupFile")

#define MIKTEX_REGVAL_SHARED_SETUP \
  MIKTEXTEXT("SharedSetup")

#define MIKTEX_REGVAL_AUTO_INSTALL \
  MIKTEXTEXT("InstallPackagesOnTheFly")

#define MIKTEX_REGVAL_FTP_MODE \
  MIKTEXTEXT("FTPMode")

#define MIKTEX_REGVAL_REMOTE_PACKAGE_REPOSITORY \
  MIKTEXTEXT("RemoteRepository")

#define MIKTEX_REGVAL_LOCAL_PACKAGE_REPOSITORY \
  MIKTEXTEXT("LocalRepository")

#define MIKTEX_REGVAL_MIKTEXDIRECT_ROOT \
  MIKTEXTEXT("MiKTeXDirectRoot")

#define MIKTEX_REGVAL_PACKAGE_REPOSITORY_TYPE \
  MIKTEXTEXT("RepositoryType")

#define MIKTEX_REGVAL_USE_PROXY \
  MIKTEXTEXT("UseProxy")

#define MIKTEX_REGVAL_PROXY_HOST	\
  MIKTEXTEXT("ProxyHost")

#define MIKTEX_REGVAL_PROXY_PORT	\
  MIKTEXTEXT("ProxyPort")

#define MIKTEX_REGVAL_PROXY_AUTH_REQ \
  MIKTEXTEXT("ProxyAuthReq")

#define MIKTEX_REGVAL_PARSE_FIRST_LINE \
  MIKTEXTEXT("ParseFirstLine")

#define MIKTEX_REGVAL_FILE_LINE_ERRORS \
  MIKTEXTEXT("FileLineErrors")

#define MIKTEX_REGVAL_WRITE18 \
  MIKTEXTEXT("EnableWrite18")

#define MIKTEX_REGVAL_EDITOR \
  MIKTEXTEXT("Editor")

#define MIKTEX_REGVAL_DESTDIR \
  MIKTEXTEXT("DestDir")

#define MIKTEX_REGVAL_TEMPDIR \
  MIKTEXTEXT("Temp")

#define MIKTEX_REGVAL_PK_FILE_NAME \
  MIKTEXTEXT("PKFnTemplate")

#define MIKTEX_REGVAL_DOC_SUFFIXES \
  MIKTEXTEXT("DocExtensions")

#define MIKTEX_DVI_FILE_TYPE_IDENTIFIER \
  MIKTEXTEXT("MiKTeX.Yap.dvi.") MIKTEXTEXT(MIKTEX_SERIES_STR)

#endif	/* miktex/reg.h */
