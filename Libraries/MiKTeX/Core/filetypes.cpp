/* filetypes.cpp: MiKTeX file type registry

   Copyright (C) 1996-2010 Christian Schenk

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

#include "StdAfx.h"

#include "internal.h"

#include "miktex/Core/Environment"

#if defined(MIKTEX_WINDOWS)
#  define DELIM ";"
#else
#  define DELIM ":"
#endif

#define R_(x) S_(x) "//"
#define r_(x) s_(x) "//"
#define S_(x) "%R" x
#define s_(x) "%r" x

#define P2_(a, b) a DELIM b
#define P3_(a, b, c) a DELIM b DELIM c
#define P4_(a, b, c, d) a DELIM b DELIM c DELIM d
#define P5_(a, b, c, d, e) a DELIM b DELIM c DELIM d DELIM e
#define P6_(a, b, c, d, e, f) a DELIM b DELIM c DELIM d DELIM e DELIM f
#define P7_(a, b, c, d, e, f, g) a DELIM b DELIM c DELIM d DELIM e DELIM f DELIM g
#define P8_(a, b, c, d, e, f, g, h) a DELIM b DELIM c DELIM d DELIM e DELIM f DELIM g DELIM h
#define P9_(a, b, c, d, e, f, g, h, i) a DELIM b DELIM c DELIM d DELIM e DELIM f DELIM g DELIM h DELIM i

#define P_ ListBuilder

/* _________________________________________________________________________

   ListBuilder
   _________________________________________________________________________ */

class ListBuilder
  : protected CharBuffer<char, 512>
{
public:
  ListBuilder ()
  {
  }

public:
  operator
  const char * ()
    const
  {
    return (Get());
  }

public:
  ListBuilder (/*[in]*/ const char *	lpszElement1,
	       /*[in]*/ const char *	lpszElement2 = 0,
	       /*[in]*/ const char *	lpszElement3 = 0,
	       /*[in]*/ const char *	lpszElement4 = 0,
	       /*[in]*/ const char *	lpszElement5 = 0,
	       /*[in]*/ const char *	lpszElement6 = 0,
	       /*[in]*/ const char *	lpszElement7 = 0,
	       /*[in]*/ const char *	lpszElement8 = 0)
  {
    Reserve (0
	     + strlen(lpszElement1)
	     + (lpszElement2 == 0 ? 0 : strlen(lpszElement2) + 1)
	     + (lpszElement3 == 0 ? 0 : strlen(lpszElement3) + 1)
	     + (lpszElement4 == 0 ? 0 : strlen(lpszElement4) + 1)
	     + (lpszElement5 == 0 ? 0 : strlen(lpszElement5) + 1)
	     + (lpszElement6 == 0 ? 0 : strlen(lpszElement6) + 1)
	     + (lpszElement7 == 0 ? 0 : strlen(lpszElement7) + 1)
	     + (lpszElement8 == 0 ? 0 : strlen(lpszElement8) + 1)
	     + 1
	     );
    Set (lpszElement1);
    if (lpszElement2 != 0 && *lpszElement2 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement2);
      }
    if (lpszElement3 != 0 && *lpszElement3 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement3);
      }
    if (lpszElement4 != 0 && *lpszElement4 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement4);
      }
    if (lpszElement5 != 0 && *lpszElement5 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement5);
      }
    if (lpszElement6 != 0 && *lpszElement6 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement6);
      }
    if (lpszElement7 != 0 && *lpszElement7 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement7);
      }
    if (lpszElement8 != 0 && *lpszElement8 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement8);
      }
  }
};

/* _________________________________________________________________________

   RegisterFileType
   _________________________________________________________________________ */

void
SessionImpl::RegisterFileType (/*[in]*/ FileType	fileType,
			       /*[in]*/ const char *	lpszFileType,
			       /*[in]*/ const char *	lpszApplication,
			       /*[in]*/ const char *	lpszFileNameExtensions,
			       /*[in]*/ const char *	lpszDefaultSearchPath,
			       /*[in]*/ const char *	lpszEnvVarNames)
{
  MIKTEX_ASSERT_STRING (lpszFileType);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszApplication);
  InternalFileTypeInfo fti;
  fti.fileType = fileType;
  fti.fileTypeString = lpszFileType;
  if (lpszApplication != 0)
    {
      fti.applicationName = lpszApplication;
    }
  string section = "ft.";
  section += lpszFileType;
  fti.fileNameExtensions =
    GetConfigValue(section.c_str(),
		   "extensions",
		   lpszFileNameExtensions);
  fti.searchPath =
    GetConfigValue(section.c_str(),
		   "path",
		   lpszDefaultSearchPath);
  fti.envVarNames =
    GetConfigValue(section.c_str(),
		   "env",
		   lpszEnvVarNames);
  fileTypes.resize (FileType::E_N_D);
  fileTypes[fileType.Get()] = fti;
}

/* _________________________________________________________________________

SessionImpl::RegisterFileType
_________________________________________________________________________ */

void
SessionImpl::RegisterFileType (/*[in]*/ FileType fileType)
{
  if (fileType.Get() >= fileTypes.size())
  {
    fileTypes.resize (FileType::E_N_D);
  }


  if (fileTypes[fileType.Get()].fileType == fileType.Get())
  {
    // already registered
    return;
  }

  switch (fileType.Get())
    {

    case FileType::AFM:
      RegisterFileType (
	FileType::AFM,
	"afm",
	0,
	".afm",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_AFM_DIR)),
	P2_("AFMFONTS", "TEXFONTS"));
      break;

    case FileType::BASE:
      RegisterFileType (
	FileType::BASE,
	"base",
	"METAFONT",
	".base",
	P2_(CURRENT_DIRECTORY, s_(MIKTEX_PATH_BASE_DIR)),
	"");
      break;

    case FileType::BIB:
      RegisterFileType (
	FileType::BIB,
	"bib",
	"BibTeX",
	".bib",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_BIBTEX_DIR "/bib")),
	P2_("BIBINPUTS", "TEXBIB"));
      break;

    case FileType::BST:
      RegisterFileType (
	FileType::BST,
	"bst",
	"BibTeX",
	".bst",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_BIBTEX_DIR "/{bst,csf}")),
	"BSTINPUTS");
      break;

    case FileType::CID:
      RegisterFileType (
	FileType::CID,
	"cid maps",
	0,
	P2_(".cid", ".cidmap"),
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_CIDMAP_DIR)),
	"FONTCIDMAPS");
      break;
      
    case FileType::CLUA:
      RegisterFileType (
	FileType::CLUA,
	"clua",
	0,
	P2_(".dll", ".so"),
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_SCRIPT_DIR "/{$progname,$engine,}/lua")),
	"CLUAINPUTS");
      break;
      
    case FileType::CNF:
      RegisterFileType (
	FileType::CNF,
	"cnf",
	0,
	".cnf",
	S_(MIKTEX_PATH_WEB2C_DIR),
	"TEXMFCNF");
      break;
      
    case FileType::CMAP:
      RegisterFileType (
	FileType::CMAP,
	"cmap files",
	0,
	"",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_CMAP_DIR)),
	P2_("CMAPFONTS", "TEXFONTS"));
      break;
      
    case FileType::CWEB:
      RegisterFileType (
	FileType::CWEB,
	"cweb",
	"CWeb",
	".w",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_CWEB_DIR)),
	"CWEBINPUTS");
      break;
 
    case FileType::DB:
      RegisterFileType (
	FileType::DB,
	"ls-R",
	0,
	"",
	"",
	"TEXMFDBS");
      break;
      
    case FileType::DVI:
      RegisterFileType (
	FileType::DVI,
	"dvi",
	0,
	".dvi",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_DOC_DIR)),
	"");
      break;
      
    case FileType::DVIPSCONFIG:
      RegisterFileType (
	FileType::DVIPSCONFIG,
	"dvips config",
	"Dvips",
	"",
	R_(MIKTEX_PATH_DVIPS_DIR),
	"TEXCONFIG");
      break;
      
    case FileType::ENC:
      RegisterFileType (
	FileType::ENC,
	"enc",
	0,
	".enc",
	P6_(CURRENT_DIRECTORY,
	    R_(MIKTEX_PATH_ENC_DIR),
	    R_(MIKTEX_PATH_MIKTEX_CONFIG_DIR),
	    R_(MIKTEX_PATH_DVIPS_DIR),
	    R_(MIKTEX_PATH_PDFTEX_DIR),
	    R_(MIKTEX_PATH_DVIPDFM_DIR)),
	    P2_("ENCFONTS", "TEXFONTS"));
      break;

    case FileType::EXE:
#if defined(MIKTEX_WINDOWS)
    case FileType::WindowsCommandScriptFile:
#endif
      {
	string extensions;
#if defined(MIKTEX_WINDOWS)
	if (! Utils::GetEnvironmentString("PATHEXT", extensions)
	    || extensions.empty())
	  {
	    extensions = P3_(".com",".exe", ".bat");
	  }
#elif defined(MIKTEX_EXE_FILE_SUFFIX)
	extensions = MIKTEX_EXE_FILE_SUFFIX;
#endif
	string exePath;
	PathName userBinDir = GetSpecialPath(SpecialPath::UserInstallRoot);
	userBinDir += MIKTEX_PATH_BIN_DIR;
	userBinDir.Canonicalize ();
	if (! IsAdminMode()
	    && ! Utils::Contains(exePath.c_str(),
				 userBinDir.Get(),
				 PATH_DELIMITER_STRING))
	  {
	    if (! exePath.empty())
	      {
		exePath += PATH_DELIMITER;
	      }
	    exePath += userBinDir.Get();
	  }
	PathName commonBinDir = GetSpecialPath(SpecialPath::CommonInstallRoot);
	commonBinDir += MIKTEX_PATH_BIN_DIR;
	commonBinDir.Canonicalize ();
	if (! Utils::Contains(exePath.c_str(),
			      commonBinDir.Get(),
			      PATH_DELIMITER_STRING))
	  {
	    if (! exePath.empty())
	      {
		exePath += PATH_DELIMITER;
	      }
	    exePath += commonBinDir.Get();
	  }
	string str;
	if (Utils::GetEnvironmentString(MIKTEX_ENV_BIN_DIR, str))
	  {
	    PathName binDir = str;
	    binDir.Canonicalize ();
	    if (! Utils::Contains(exePath.c_str(),
				  binDir.Get(),
				  PATH_DELIMITER_STRING))
	      {
		if (! exePath.empty())
		  {
		    exePath += PATH_DELIMITER;
		  }
		exePath += binDir.Get();
	      }
	  }
	PathName myLocation = GetMyLocation(true);
	if (! Utils::Contains(exePath.c_str(),
			      myLocation.Get(),
			      PATH_DELIMITER_STRING))
	  {
	    if (! exePath.empty())
	      {
		exePath += PATH_DELIMITER;
	      }
	    exePath += myLocation.Get();
	  }
	if (fileType.Get() == FileType::EXE)
	  {
	    RegisterFileType (
	      FileType::EXE,
	      "exe",
	      0,
	      extensions.c_str(),
	      exePath.c_str(),
	      "");
	  }
#if defined(MIKTEX_WINDOWS)
	else if (fileType.Get() == FileType::WindowsCommandScriptFile)
	  {
	    RegisterFileType (
	      FileType::WindowsCommandScriptFile,
	      "Windows command script file",
	      0,
	      P2_(".bat", ".cmd"),
	      P_(R_(MIKTEX_PATH_SCRIPT_DIR), exePath.c_str()),
	      "");
	  }
#endif
	break;
      }

    case FileType::FEA:
      RegisterFileType (
	FileType::FEA,
	"font feature files",
	0,
	".fea",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_FONT_FEATURE_DIR)),
	"FONTFEATURES");
      break;

    case FileType::FMT:
      RegisterFileType (
	FileType::FMT,
	"fmt",
	"TeX",
	".fmt",
	P2_(CURRENT_DIRECTORY, s_(MIKTEX_PATH_FMT_DIR "/{$engine,}")),
	"");
      break;

    case FileType::GF:
      RegisterFileType (
	FileType::GF,
	"gf",
	0,
	".gf",
	// <todo>
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_FONT_DIR)),
	// <todo/>
	P3_("GFFONTS", "GLYPHFONTS", "TEXFONTS"));
      break;

    case FileType::GLYPHFONT:
      RegisterFileType (
	FileType::GLYPHFONT,
	"bitmap font",
	0,
	"",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_FONT_DIR)),
	P2_("GLYPHFONTS", "TEXFONTS"));
      break;
      
    case FileType::GRAPHICS:
      RegisterFileType (
	FileType::GRAPHICS,
	"graphic/figure",
	0,
	P3_(".eps", ".epsi",".png"),
	P4_(CURRENT_DIRECTORY,
	    R_(MIKTEX_PATH_DVIPS_DIR),
	    R_(MIKTEX_PATH_PDFTEX_DIR),
	    R_(MIKTEX_PATH_TEX_DIR)),
	P2_("TEXPICTS", "TEXINPUTS"));
      break;
      
    case FileType::HBF:
      RegisterFileType (
	FileType::HBF,
	"hbf",
	0,
	".hbf",
	P3_(CURRENT_DIRECTORY,
	    R_(MIKTEX_PATH_HBF_DIR),
	    R_(MIKTEX_PATH_TYPE1_DIR)),
	"");
      break;
  
    case FileType::IST:
      RegisterFileType (
	FileType::IST,
	"ist",
	"MakeIndex",
	".ist",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_MAKEINDEX_DIR)),
	P2_("TEXINDEXSTYLE", "INDEXSTYLE"));
      break;
  
    case FileType::LIG:
      RegisterFileType (
	FileType::LIG,
	"lig files",
	0,
	".lig",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_LIG_DIR)),
	"TEXFONTS");
      break;

    case FileType::LUA:
      RegisterFileType (
	FileType::LUA,
	"lua",
	0,
	P6_(".luc", ".luctex", ".texluc", ".lua", ".luatex", ".texlua"),
	P_(CURRENT_DIRECTORY,
	   R_(MIKTEX_PATH_SCRIPT_DIR "/{$progname,$engine,}/{lua,}"),
	   GetFileTypeInfo(FileType::TEX).searchPath.c_str()),
	"LUAINPUTS");
      break;

    case FileType::MAP:
      RegisterFileType (
	FileType::MAP,
	"map",
	0,
	".map",
	P6_(CURRENT_DIRECTORY,
	    R_(MIKTEX_PATH_MAP_DIR "/{$progname,pdftex,dvips,}"),
	    R_(MIKTEX_PATH_MIKTEX_CONFIG_DIR),
	    R_(MIKTEX_PATH_DVIPS_DIR),
	    R_(MIKTEX_PATH_PDFTEX_DIR),
	    R_(MIKTEX_PATH_DVIPDFM_DIR)),
	P2_("TEXFONTMAPS", "TEXFONTS"));
      break;

    case FileType::MEM:
      RegisterFileType (
	FileType::MEM,
	"mem",
	"MetaPost",
	".mem",
	P2_(CURRENT_DIRECTORY, s_(MIKTEX_PATH_MEM_DIR)),
	"");
      break;
  
    case FileType::MF:
      RegisterFileType (
	FileType::MF,
	"mf",
	"METAFONT",
	".mf",
	P3_(CURRENT_DIRECTORY,
	    R_(MIKTEX_PATH_METAFONT_DIR),
	    R_(MIKTEX_PATH_FONT_SOURCE_DIR)),
	"MFINPUTS");
      break;

    case FileType::MFPOOL:
      RegisterFileType (
	FileType::MFPOOL,
	"mfpool",
	0,
	".pool",
	CURRENT_DIRECTORY,
	P2_("MFPOOL", "TEXMFINI"));
      break;

    case FileType::MFT:
      RegisterFileType (
	FileType::MFT,
	"mft",
	0,
	".mft",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_MFT_DIR)),
	"MFTINPUTS");
      break;

    case FileType::MISCFONT:
      RegisterFileType (
	FileType::MISCFONT,
	"misc fonts",
	0,
	"",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_MISCFONTS_DIR)),
	P2_("MISCFONTS","TEXFONTS"));
      break;

    case FileType::MLBIB:
      RegisterFileType (
	FileType::MLBIB,
	"mlbib",
	0,
	P2_(".mlbib", ".bib"),
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_BIBTEX_DIR "/bib/{mlbib,}")),
	P3_("MLBIBINPUTS", "BIBINPUTS", "TEXBIB"));
      break;

    case FileType::MLBST:
      RegisterFileType (
	FileType::MLBST,
	"mlbst",
	0,
	".bst",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_BIBTEX_DIR "/{mlbst,bst}")),
	P2_( "MLBSTINPUTS", "BSTINPUTS"));
      break;

    case FileType::MP:
      RegisterFileType (
	FileType::MP,
	"mp",
	"MetaPost",
	".mp",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_METAPOST_DIR)),
	"MPINPUTS");
      break;

    case FileType::MPPOOL:
      RegisterFileType (
	FileType::MPPOOL,
	"mppool",
	0,
	".pool",
	CURRENT_DIRECTORY,
	P2_("MPPOOL", "TEXMFINI"));
      break;

    case FileType::MPSUPPORT:
      RegisterFileType (
	FileType::MPSUPPORT,
	"MetaPost support",
	0,
	"",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_MPSUPPORT_DIR)),
	"MPSUPPORT");
      break;

    case FileType::OCP:
      RegisterFileType (
	FileType::OCP,
	"ocp",
	"Omega",
	".ocp",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_OCP_DIR)),
	"OCPINPUTS");
      break;

    case FileType::OFM:
      RegisterFileType (
	FileType::OFM,
	"ofm",
	"Omega",
	P2_(".ofm", ".tfm"),
	P3_(CURRENT_DIRECTORY,
	    R_(MIKTEX_PATH_OFM_DIR),
	    R_(MIKTEX_PATH_TFM_DIR)),
	P2_("OFMFONTS", "TEXFONTS"));
      break;

    case FileType::OPL:
      RegisterFileType (
	FileType::OPL,
	"opl",
	0,
	".opl",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_OPL_DIR)),
	P2_("OPLFONTS", "TEXFONTS"));
      break;

    case FileType::OTP:
      RegisterFileType (
	FileType::OTP,
	"otp",
	"otp2ocp",
	".otp",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_OTP_DIR)),
	"OTPINPUTS");
      break;

    case FileType::OTF:
      RegisterFileType (
	FileType::OTF,
	"opentype fonts",
	0,
	".otf",
	P_(CURRENT_DIRECTORY,
	   R_(MIKTEX_PATH_OPENTYPE_DIR),
	   GetLocalFontDirectories().c_str()),
	P2_("OPENTYPEFONTS", "TEXFONTS"));
      break;

    case FileType::OVF:
      RegisterFileType (
	FileType::OVF,
	"ovf",
	0,
	".ovf",
	P3_(CURRENT_DIRECTORY,
	    R_(MIKTEX_PATH_OVF_DIR),
	    R_(MIKTEX_PATH_VF_DIR)),
	P2_("OVFFONTS", "TEXFONTS"));
      break;
  
    case FileType::OVP:
      RegisterFileType (
	FileType::OVP,
	"ovp",
	0,
	".ovp",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_OVP_DIR)),
	P2_("OVPFONTS", "TEXFONTS"));
      break;

    case FileType::PDFTEXCONFIG:
      RegisterFileType (
	FileType::PDFTEXCONFIG,
	"pdftex config",
	0,
	"",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_PDFTEX_DIR "/{$progname,}")),
	"PDFTEXCONFIG");
      break;

    case FileType::PERLSCRIPT:
      RegisterFileType (
	FileType::PERLSCRIPT,
	"perlscript",
	0,
	".pl",
	P8_(R_(MIKTEX_PATH_SCRIPT_DIR),
	    R_(MIKTEX_PATH_CONTEXT_DIR),
	    R_(MIKTEX_PATH_MIKTEX_DIR),
	    R_(MIKTEX_PATH_NTS_DIR),
	    R_(MIKTEX_PATH_PDFTEX_DIR),
	    R_(MIKTEX_PATH_PSUTILS_DIR),
	    R_(MIKTEX_PATH_SOURCE_DIR),
	    R_(MIKTEX_PATH_TEX_DIR)),
	"");
      break;

    case FileType::PK:
      RegisterFileType (
	FileType::PK,
	"pk",
	0,
	".pk",
	// <todo>
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_FONT_DIR)),
	// </todo>
	"");
      break;

    case FileType::PROGRAMBINFILE:
      RegisterFileType (
	FileType::PROGRAMBINFILE,
	"other binary files",
	0,
	"",
	P2_(CURRENT_DIRECTORY, R_("/$progname")),
	"");
      break;

    case FileType::PROGRAMTEXTFILE:
      RegisterFileType (
	FileType::PROGRAMTEXTFILE,
	"other text files",
	0,
	"",
	P2_(CURRENT_DIRECTORY, R_("/$progname")),
	"");
      break;
  
    case FileType::PSHEADER:
      RegisterFileType (
	FileType::PSHEADER,
	"PostScript header",
	0,
	P2_(".pro", ".enc"),
	P9_(CURRENT_DIRECTORY,
	    R_(MIKTEX_PATH_MIKTEX_CONFIG_DIR),
	    R_(MIKTEX_PATH_DVIPS_DIR),
	    R_(MIKTEX_PATH_PDFTEX_DIR),
	    R_(MIKTEX_PATH_DVIPDFM_DIR),
	    R_(MIKTEX_PATH_ENC_DIR),
	    R_(MIKTEX_PATH_TYPE1_DIR),
	    R_(MIKTEX_PATH_TYPE42_DIR),
	    R_(MIKTEX_PATH_TYPE3_DIR),
	    "$psfontdirs"),
	P2_("TEXPSHEADERS", "PSHEADERS"));
      break;

    case FileType::SCRIPT:
      RegisterFileType (
	FileType::SCRIPT,
	"texmfscripts",
	0,
	"",
	R_(MIKTEX_PATH_SCRIPT_DIR "/{$progname,$engine,}"),
	"TEXMFSCRIPTS");
      break;

    case FileType::SFD:
      RegisterFileType (
	FileType::SFD,
	"subfont definition files",
	0,
	".sfd",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_SFD_DIR)),
	P2_("SFDFONTS", "TEXFONTS"));
      break;

    case FileType::TCX:
      RegisterFileType (
	FileType::TCX,
	"tcx",
	0,
	".tcx",
	P3_(CURRENT_DIRECTORY,
	    S_(MIKTEX_PATH_MIKTEX_CONFIG_DIR),
            S_(MIKTEX_PATH_WEB2C_DIR)),
	"");
      break;
  
    case FileType::TEX:
      RegisterFileType (
	FileType::TEX,
	"tex",
	"TeX",
	".tex",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_TEX_DIR "/{$progname,generic,}")),
	"TEXINPUTS");
      break;

    case FileType::TEXPOOL:
      RegisterFileType (
	FileType::TEXPOOL,
	"texpool",
	0,
	".pool",
	CURRENT_DIRECTORY,
	P2_("TEXPOOL", "TEXMFINI"));
      break;

    case FileType::TEXSOURCE:
      RegisterFileType (
	FileType::TEXSOURCE,
	"TeX system sources",
	0,
	"",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_SOURCE_DIR)),
	"TEXSOURCES");
      break;

    case FileType::TEXSYSDOC:
      RegisterFileType (
	FileType::TEXSYSDOC,
	"TeX system documentation",
	0,
#if defined(MIKTEX_WINDOWS)
	P6_(".chm", ".dvi", ".html", ".txt", ".pdf", ".ps"),
#else
	P5_(".dvi", ".html", ".txt", ".pdf", ".ps"),
#endif
	P2_(R_(MIKTEX_PATH_MIKTEX_DOC_DIR),
	    R_(MIKTEX_PATH_DOC_DIR)),
	"TEXDOCS");
      break;

    case FileType::TFM:
      RegisterFileType (
	FileType::TFM,
	"tfm",
	0,
	".tfm",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_TFM_DIR)),
	P2_("TFMFONTS", "TEXFONTS"));
      break;

    case FileType::TROFF:
      RegisterFileType (
	FileType::TROFF,
	"troff fonts",
	0,
	"",
	"",
	"TRFONTS");
      break;

    case FileType::TTF:
      RegisterFileType (
	FileType::TTF,
	"truetype fonts",
	0,
	P2_(".ttf", ".ttc"),
	P_(CURRENT_DIRECTORY,
	   R_(MIKTEX_PATH_TRUETYPE_DIR),
	   GetLocalFontDirectories().c_str()),
	P2_("TTFONTS", "TEXFONTS"));
      break;

    case FileType::TYPE1:
      RegisterFileType (
	FileType::TYPE1,
	"type1 fonts",
	0,
	P2_(".pfb", ".pfa"),
	P_(CURRENT_DIRECTORY,
	   R_(MIKTEX_PATH_TYPE1_DIR),
	   GetLocalFontDirectories().c_str()),
	P5_("T1FONTS", "T1INPUTS", "TEXFONTS", "TEXPSHEADERS", "PSHEADERS"));
      break;

    case FileType::TYPE42:
      RegisterFileType (
	FileType::TYPE42,
	"type42 fonts",
	0,
#if defined(MIKTEX_WINDOWS)
	".t42",
#else
	P2_(".t42", ".T42"),
#endif
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_TYPE42_DIR)),
	P2_("T42FONTS", "TEXFONTS"));
      break;

    case FileType::VF:
      RegisterFileType (
	FileType::VF,
	"vf",
	0,
	".vf",
	P2_(CURRENT_DIRECTORY, R_(MIKTEX_PATH_VF_DIR)),
	P2_("VFFONTS", "TEXFONTS"));
      break;

    case FileType::WEB:
      RegisterFileType (
	FileType::WEB,
	"web",
	0,
	".web",
	R_(MIKTEX_PATH_WEB_DIR),
	"WEBINPUTS");
      break;

    case FileType::WEB2C:
      RegisterFileType (
	FileType::WEB2C,
	"web2c files",
	0,
	"",
	R_(MIKTEX_PATH_WEB2C_DIR),
	"");
      break;

    default:
      UNEXPECTED_CONDITION ("SessionImpl::RegisterFileType");
    }
}

/* _________________________________________________________________________

   SessionImpl::RegisterFileTypes
   _________________________________________________________________________ */

void
SessionImpl::RegisterFileTypes ()
{
  for (int ft = FileType::None + 1; ft < FileType::E_N_D; ++ ft)
  {
    RegisterFileType (static_cast<FileType::EnumType>(ft));
  }
}

/* _________________________________________________________________________

   SessionImpl::GetInternalFileTypeInfo
   _________________________________________________________________________ */

InternalFileTypeInfo *
SessionImpl::GetInternalFileTypeInfo (/*[in]*/ FileType fileType)
{
  RegisterFileType (fileType);
  return (&fileTypes[fileType.Get()]);
}

/* _________________________________________________________________________

   SessionImpl::GetFileTypeInfo
   _________________________________________________________________________ */

FileTypeInfo
SessionImpl::GetFileTypeInfo (/*[in]*/ FileType fileType)
{
  return (*GetInternalFileTypeInfo(fileType));
}

/* _________________________________________________________________________

   SessionImpl::DeriveFileType
   _________________________________________________________________________ */

FileType
SessionImpl::DeriveFileType (/*[in]*/ const char * lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  RegisterFileTypes ();

  const char * lpszExt = GetFileNameExtension(lpszPath);

  for (int idx = 1; idx < fileTypes.size(); ++ idx)
    {
      if (lpszExt == 0)
      {
	if (StringCompare(fileTypes[idx].fileTypeString.c_str(), lpszPath) == 0)
	{
	  return (fileTypes[idx].fileType);
	}
      }
      else
      {
	for (CSVList ext (fileTypes[idx].fileNameExtensions.c_str(), PATH_DELIMITER);
	  ext.GetCurrent() != 0;
	  ++ ext)
	{
	  if (PathName::Compare(ext.GetCurrent(), lpszExt) == 0)
	  {
	    return (fileTypes[idx].fileType);
	  }
	}
      }
  }

  return (FileType::None);
}

/* _________________________________________________________________________

   SessionImpl::GetNextFileTypeInfo
   _________________________________________________________________________ */

bool
SessionImpl::GetNextFileTypeInfo (/*[in]*/ unsigned		index,
				  /*[out]*/ FileTypeInfo &	fileTypeInfo)
{
  // skip FileType::None
  unsigned index1 = index + 1;
  if (index1 == FileType::E_N_D)
  {
    return (false);
  }
  if (index1 > FileType::E_N_D)
  {
    INVALID_ARGUMENT ("SessionImpl::GetNextFileTypeInfo",
      NUMTOSTR(index));
  }
  fileTypeInfo = GetFileTypeInfo(static_cast<FileType::EnumType>(index1));
  return (true);
}

/* _________________________________________________________________________

   SessionImpl::ClearSearchVectors
   _________________________________________________________________________ */

void
SessionImpl::ClearSearchVectors ()
{
  for (vector<InternalFileTypeInfo>::iterator it = fileTypes.begin();
       it != fileTypes.end();
       ++ it)
    {
      it->searchVec.clear ();
    }
}
