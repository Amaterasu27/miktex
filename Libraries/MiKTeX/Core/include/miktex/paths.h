/* miktex/paths.h: hard-coded path names

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

#if ! defined(MIKTEX__43B34B76_6294_45D5_B2BD_634D4B012B33__)
#define MIKTEX__43B34B76_6294_45D5_B2BD_634D4B012B33__

#include <miktex/char.h>

#define MIKTEX_PATH_TEXMF_PLACEHOLDER MIKTEXTEXT("%R")
#define MIKTEX_PATH_TEXMF_PLACEHOLDER_NO_MPM MIKTEXTEXT("%r")

#define MIKTEX_PATH_RECURSION_INDICATOR T_("//")


#if defined(_WIN32) || defined(_WIN64)
#  define MIKTEX_PATH_DIRECTORY_DELIMITER_STRING MIKTEXTEXT("\\")
#else
#  define MIKTEX_PATH_DIRECTORY_DELIMITER_STRING MIKTEXTEXT("/")
#endif

#if defined(_WIN32) || defined(_WIN64)
#  define MIKTEX_EXE_FILE_SUFFIX MIKTEXTEXT(".exe")
#else
#  define MIKTEX_EXE_FILE_SUFFIX MIKTEXTEXT("")
#endif

/* suffix for file name database files */
#define MIKTEX_FNDB_FILE_SUFFIX MIKTEXTEXT(".fndb")

#define MIKTEX_FORMAT_FILE_SUFFIX MIKTEXTEXT(".fmt")

#define MIKTEX_POOL_FILE_SUFFIX MIKTEXTEXT(".pool")

#define MIKTEX_CABINET_FILE_SUFFIX MIKTEXTEXT(".cab")

#define MIKTEX_TARBZIP2_FILE_SUFFIX MIKTEXTEXT(".tar.bz2")

#define MIKTEX_TARLZMA_FILE_SUFFIX MIKTEXTEXT(".tar.lzma")

#define MIKTEX_TAR_FILE_SUFFIX MIKTEXTEXT(".tar")

#define MIKTEX_ZIP_FILE_SUFFIX MIKTEXTEXT(".zip")

#define MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX MIKTEXTEXT(".tpm")

#define MIKTEX_MPM_DB_LIGHT_FILE_NAME_NO_SUFFIX \
  MIKTEXTEXT("miktex")				\
  MIKTEXTEXT("-")				\
  MIKTEXTEXT("zzdb") MIKTEXTEXT("1")		\
  MIKTEXTEXT("-")				\
  MIKTEXTEXT(MIKTEX_SERIES_STR)

#define MIKTEX_MPM_DB_LIGHT_FILE_NAME		\
  MIKTEX_MPM_DB_LIGHT_FILE_NAME_NO_SUFFIX	\
  MIKTEX_TARBZIP2_FILE_SUFFIX

#define MIKTEX_MPM_DB_FULL_FILE_NAME_NO_SUFFIX	\
  MIKTEXTEXT("miktex")				\
  MIKTEXTEXT("-")				\
  MIKTEXTEXT("zzdb") MIKTEXTEXT("2")		\
  MIKTEXTEXT("-")				\
  MIKTEXTEXT(MIKTEX_SERIES_STR)

#define MIKTEX_MPM_DB_FULL_FILE_NAME		\
  MIKTEX_MPM_DB_FULL_FILE_NAME_NO_SUFFIX	\
  MIKTEX_TARBZIP2_FILE_SUFFIX

#define MIKTEX_PATH_BIBTEX_DIR			\
  MIKTEXTEXT("bibtex")

#define MIKTEX_PATH_BIB_DIR			\
  MIKTEX_PATH_BIBTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("bib")

#define MIKTEX_PATH_BST_DIR			\
  MIKTEX_PATH_BIBTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("bst")

#define MIKTEX_PATH_CONTEXT_DIR			\
  MIKTEXTEXT("context")

#define MIKTEX_PATH_DOC_DIR			\
  MIKTEXTEXT("doc")

#define MIKTEX_PATH_MIKTEX_DOC_DIR		\
  MIKTEX_PATH_DOC_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("miktex")

#define MIKTEX_PATH_RELNOTES_HTML		\
  MIKTEX_PATH_MIKTEX_DOC_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("RelNotes.html")

#define MIKTEX_PATH_DVIPDFM_DIR			\
  MIKTEXTEXT("dvipdfm")

#define MIKTEX_PATH_DVIPDFM_CONFIG_DIR		\
  MIKTEX_PATH_DVIPDFM_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("config")

#define MIKTEX_PATH_DVIPDFMX_CONFIG_DIR		\
  MIKTEX_PATH_DVIPDFM_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("config")

#define MIKTEX_PATH_DVIPS_DIR			\
  MIKTEXTEXT("dvips")

#define MIKTEX_PATH_DVIPS_CONFIG_DIR		\
  MIKTEX_PATH_DVIPS_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("config")

#define MIKTEX_PATH_FONT_DIR			\
  MIKTEXTEXT("fonts")

#define MIKTEX_PATH_AFM_DIR			\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("afm")

#define MIKTEX_PATH_ENC_DIR			\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("enc")

#define MIKTEX_PATH_MAP_DIR			\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("map")

#define MIKTEX_PATH_MISCFONT_DIR		\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("misc")

#define MIKTEX_PATH_HBF_DIR			\
  MIKTEX_PATH_MISCFONT_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("hbf")

#define MIKTEX_PATH_FONT_SOURCE_DIR		\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("source")

#define MIKTEX_PATH_OFM_DIR			\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("ofm")

#define MIKTEX_PATH_OVF_DIR			\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("ovf")

#define MIKTEX_PATH_OVP_DIR			\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("ovp")

#define MIKTEX_PATH_TFM_DIR			\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("tfm")

#define MIKTEX_PATH_TRUETYPE_DIR		\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("truetype")

#define MIKTEX_PATH_TYPE1_DIR			\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("type1")

#define MIKTEX_PATH_VF_DIR			\
  MIKTEX_PATH_FONT_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("vf")

#define MIKTEX_PATH_MAKEINDEX_DIR		\
  MIKTEXTEXT("makeindex")

#define MIKTEX_PATH_METAFONT_DIR		\
  MIKTEXTEXT("metafont")

#define MIKTEX_PATH_METAPOST_DIR		\
  MIKTEXTEXT("metapost")

#define MIKTEX_PATH_MIKTEX_DIR			\
  MIKTEXTEXT("miktex")

#define MIKTEX_PATH_MIKTEX_CONFIG_DIR		\
  MIKTEX_PATH_MIKTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("config")

#define MIKTEX_PATH_MIKTEX_MTHELP_DIR		\
  MIKTEX_PATH_MIKTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("mthelp")

#define MIKTEX_PATH_BASE_DIR			\
  MIKTEX_PATH_MIKTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("base")

#define MIKTEX_PATH_FMT_DIR			\
  MIKTEX_PATH_MIKTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("fmt")

#define MIKTEX_PATH_MEM_DIR			\
  MIKTEX_PATH_MIKTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("mem")

#define MIKTEX_PATH_FORMATS_INI			\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_FORMATS_INI_FILENAME

#define MIKTEX_PATH_CONFIG_PS			\
  MIKTEX_PATH_DVIPS_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_CONFIG_PS_FILENAME

#define MIKTEX_PATH_DVIPDFM_CONFIG		\
  MIKTEX_PATH_DVIPDFM_CONFIG_DIR		\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_DVIPDFM_CONFIG_FILENAME

#define MIKTEX_PATH_DVIPDFMX_CONFIG		\
  MIKTEX_PATH_DVIPDFMX_CONFIG_DIR		\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_DVIPDFMX_CONFIG_FILENAME

#define MIKTEX_PATH_PDFTEX_CFG			\
  MIKTEX_PATH_PDFTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_PDFTEX_CFG_FILENAME

#define MIKTEX_PATH_PDFTEXCONFIG_TEX		\
  MIKTEX_PATH_TEX_GENERIC_CONFIG_DIR		\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_PDFTEXCONFIG_TEX_FILENAME

#define MIKTEX_PATH_EDITORS_INI			\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_EDITORS_INI_FILENAME

#define MIKTEX_PATH_MD_INI			\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("md.ini")

#define MIKTEX_PATH_UPDMAP_CFG			\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_UPDMAP_CFG_FILENAME

#define MIKTEX_FORMATS_INI_FILENAME MIKTEXTEXT("formats.ini")

#define MIKTEX_EDITORS_INI_FILENAME MIKTEXTEXT("editors.ini")

#define MIKTEX_MPM_INI_FILENAME MIKTEXTEXT("mpm.ini")

#define MIKTEX_PDFTEX_CFG_FILENAME MIKTEXTEXT("pdftex.cfg")

#define MIKTEX_PDFTEXCONFIG_TEX_FILENAME MIKTEXTEXT("pdftexconfig.tex")

#define MIKTEX_UPDMAP_CFG_FILENAME MIKTEXTEXT("updmap.cfg")

#define MIKTEX_CONFIG_PS_FILENAME MIKTEXTEXT("config.ps")

#define MIKTEX_DVIPDFM_CONFIG_FILENAME MIKTEXTEXT("config")
#define MIKTEX_DVIPDFMX_CONFIG_FILENAME MIKTEXTEXT("dvipdfmx.cfg")

#define MIKTEX_PATH_MKFNTMAP_CFG		\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("mkfntmap.cfg")

#define MIKTEX_PATH_MPM_FNDB			\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("mpm")				\
  MIKTEX_FNDB_FILE_SUFFIX

#define MIKTEX_PATH_PACKAGES_INI		\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("packages.ini")

#define MIKTEX_PATH_STARTUP_CONFIG_FILE		\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("miktexstartup.ini")

#define MIKTEX_PATH_TEXMF_FNDB			\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("texmf")				\
  MIKTEX_FNDB_FILE_SUFFIX

#if defined(_WIN32) || defined(_WIN64)
#  if defined(_MSC_VER) && defined(_M_IX86)
#      define MIKTEX_PATH_BIN_DIR		\
  MIKTEX_PATH_MIKTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("bin")
#  elif defined(_MSC_VER) && defined(_M_AMD64)
#    define MIKTEX_PATH_BIN_DIR			\
  MIKTEX_PATH_MIKTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("bin")
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT ("amd64")
#  else
#    error Unimplemented: MIKTEX_PATH_BIN_DIR
#  endif
#endif

#if defined(_WIN32) || defined(_WIN64)
#  define MIKTEX_PATH_COPYSTART_EXE		\
  MIKTEX_PATH_BIN_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_COPYSTART_EXE
#endif

#if defined(_WIN32) || defined(_WIN64)
#  define MIKTEX_PATH_UNINSTALL_DAT		\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_UNINSTALL_DAT
#endif

#if defined(_WIN32) || defined(_WIN64)
#  define MIKTEX_PATH_UNINST_LOG		\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_UNINSTALL_LOG
#endif

#if defined(_WIN32) || defined(_WIN64)
#  define MIKTEX_PATH_UPDATE_DAT		\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_UPDATE_DAT
#endif

#define MIKTEX_PATH_NTS_DIR			\
  MIKTEXTEXT("nts")

#define MIKTEX_PATH_OMEGA_DIR			\
  MIKTEXTEXT("omega")

#define MIKTEX_PATH_OCP_DIR			\
  MIKTEX_PATH_OMEGA_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("ocp")

#define MIKTEX_PATH_OTP_DIR			\
  MIKTEX_PATH_OMEGA_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("otp")

#define MIKTEX_PATH_PDFTEX_DIR			\
  MIKTEXTEXT("pdftex")

#define MIKTEX_PATH_TEX_DIR			\
  MIKTEXTEXT("tex")

#define MIKTEX_PATH_TEX_GENERIC_DIR		\
  MIKTEX_PATH_TEX_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("generic")

#define MIKTEX_PATH_TEX_GENERIC_CONFIG_DIR	\
  MIKTEX_PATH_TEX_GENERIC_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("config")

#define MIKTEX_PATH_PDFTEX_CONFIG_DIR		\
  MIKTEX_PATH_PDFTEX_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("config")

#define MIKTEX_PATH_PSUTILS_DIR			\
  MIKTEXTEXT("psutils")

#define MIKTEX_PATH_SCRIPT_DIR			\
  MIKTEXTEXT("scripts")

#define MIKTEX_PATH_SOURCE_DIR			\
  MIKTEXTEXT("source")

#define MIKTEX_PATH_TEX_DIR			\
  MIKTEXTEXT("tex")

#define MIKTEX_PATH_WEB2C_DIR			\
  MIKTEXTEXT("web2c")

#if defined(MIKTEX_WINDOWS)
# define MIKTEX_TEMP_DIR \
    MIKTEX_PATH_DIRECTORY_DELIMITER_STRING MIKTEXTEXT("MiKTeXTemp")
#endif

/* _________________________________________________________________________

   MIKTEX_PATH_MPM_INI
   _________________________________________________________________________ */

#define MIKTEX_PATH_MPM_INI			\
  MIKTEX_PATH_MIKTEX_CONFIG_DIR			\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEX_MPM_INI_FILENAME

/* _________________________________________________________________________

   MIKTEX_PATH_TPM_DIR
   _________________________________________________________________________ */

#define MIKTEX_PATH_TPM_DIR			\
  MIKTEXTEXT("tpm")

/* _________________________________________________________________________

   MIKTEX_PATH_PACKAGE_DEFINITION_DIR
   _________________________________________________________________________ */

#define MIKTEX_PATH_PACKAGE_DEFINITION_DIR	\
  MIKTEX_PATH_TPM_DIR				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  MIKTEXTEXT("packages")

#define MIKTEX_DVIPS_EXE MIKTEXTEXT("dvips") MIKTEX_EXE_FILE_SUFFIX
#define MIKTEX_MAKEPK_EXE MIKTEXTEXT("makepk") MIKTEX_EXE_FILE_SUFFIX
#define MIKTEX_MAKETFM_EXE MIKTEXTEXT("maketfm") MIKTEX_EXE_FILE_SUFFIX
#define MIKTEX_GS_EXE MIKTEXTEXT("mgs") MIKTEX_EXE_FILE_SUFFIX
#define MIKTEX_INITEXMF_EXE MIKTEXTEXT("initexmf") MIKTEX_EXE_FILE_SUFFIX
#define MIKTEX_YAP_EXE MIKTEXTEXT("yap") MIKTEX_EXE_FILE_SUFFIX
#define MIKTEX_MO_EXE MIKTEXTEXT("mo") MIKTEX_EXE_FILE_SUFFIX
#define MIKTEX_COPYSTART_EXE MIKTEXTEXT("copystart") MIKTEX_EXE_FILE_SUFFIX

#define MIKTEX_UNINSTALL_DAT MIKTEXTEXT("uninstall.dat")
#define MIKTEX_UNINSTALL_LOG MIKTEXTEXT("uninst.log")
#define MIKTEX_UPDATE_DAT MIKTEXTEXT("update.dat")
#define MIKTEX_MPM_EXE MIKTEXTEXT("mpm") MIKTEX_EXE_FILE_SUFFIX
#define MIKTEX_MAIN_HELP_FILE MIKTEXTEXT("miktex.chm")
#define MIKTEX_FAQ_HELP_FILE MIKTEXTEXT("faq.chm")
#define MIKTEX_LATEX_HELP_FILE MIKTEXTEXT("latex2e.chm")
#define MIKTEX_YAP_HELP_FILE MIKTEXTEXT("yap.chm")

#endif
