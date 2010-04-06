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

/* _________________________________________________________________________

   SearchSpecBuilder
   _________________________________________________________________________ */

class SearchSpecBuilder
  : protected CharBuffer<char, 512>
{
private:
  void
  Build (/*[in]*/ bool		recursive,
	 /*[in]*/ const char *	lpszDir)
  {
    Set (TEXMF_PLACEHOLDER);
    Append (PathName::DirectoryDelimiter);
    Append (lpszDir);
    if (recursive)
      {
	Append (RECURSION_INDICATOR);
      }
  }

public:
  operator
  const char * ()
    const
  {
    return (Get());
  }

public:
  SearchSpecBuilder (/*[in]*/ const char * lpszDir)
  {
    Build (true, lpszDir);
  }

public:
  SearchSpecBuilder (/*[in]*/ bool		recursive,
		     /*[in]*/ const char *	lpszDir)
  {
    Build (recursive, lpszDir);
  }
};

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
    if (lpszElement2 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement2);
      }
    if (lpszElement3 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement3);
      }
    if (lpszElement4 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement4);
      }
    if (lpszElement5 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement5);
      }
    if (lpszElement6 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement6);
      }
    if (lpszElement7 != 0)
      {
	Append (PATH_DELIMITER);
	Append (lpszElement7);
      }
    if (lpszElement8 != 0)
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
  fileTypes.resize (fileType.Get() + 20);
  fileTypes[fileType.Get()] = fti;
}

/* _________________________________________________________________________

   SessionImpl::RegisterFileTypes
   _________________________________________________________________________ */

void
SessionImpl::RegisterFileTypes ()
{
  if (fileTypes.size() > 0)
    {
      return;
    }

  fileTypes.reserve (30);

  CSVList app (applicationNames.c_str(), PATH_DELIMITER);
  MIKTEX_ASSERT (app.GetCurrent() != 0);
  string applicationName = app.GetCurrent();
 
  string applicationSearchPath = CURRENT_DIRECTORY;
  applicationSearchPath += PATH_DELIMITER;
  applicationSearchPath += TEXMF_PLACEHOLDER;
  applicationSearchPath += MIKTEX_PATH_DIRECTORY_DELIMITER_STRING;
  applicationSearchPath += applicationName;
  applicationSearchPath += RECURSION_INDICATOR;

  RegisterFileType
    (FileType::AFM,
     "afm",
     0,
     ListBuilder(
		 ".afm"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_AFM_DIR)
		 ),
     ListBuilder(
		 "AFMFONTS",
		 "TEXFONTS"
		 ));
  
  RegisterFileType
    (FileType::BASE,
     "base",
     "METAFONT",
     ListBuilder(
		 ".base"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_BASE_DIR)
		 ),
     ListBuilder());
  
  RegisterFileType
    (FileType::BIB,
     "bib",
     "BibTeX",
     ListBuilder(
		 ".bib"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_BIBTEX_DIR)
		 ),
     ListBuilder(
		 "BIBINPUTS",
		 "TEXBIB"
		 ));

  RegisterFileType
    (FileType::BST,
     "bst",
     "BibTeX",
     ListBuilder(
		 ".bst"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_BIBTEX_DIR)
		 ),
     ListBuilder(
		 "BSTINPUTS"
		 ));

  RegisterFileType
    (FileType::DVI,
     "dvi",
     0,
     ListBuilder(
		 ".dvi"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_DOC_DIR)
		 ),
     ListBuilder());

  RegisterFileType
    (FileType::DVIPSCONFIG,
     "dvips config",
     "Dvips",
     ListBuilder(),
     ListBuilder(
		 SearchSpecBuilder(MIKTEX_PATH_DVIPS_DIR)
		 ),
     ListBuilder(
		 "TEXCONFIG"
		 ));
  
  RegisterFileType
    (FileType::ENC,
     "enc",
     0,
     ListBuilder(
		 ".enc"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_ENC_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_MIKTEX_CONFIG_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_DVIPS_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_PDFTEX_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_DVIPDFM_DIR)
		 ),
     ListBuilder(
		 "ENCFONTS",
		 "TEXFONTS"
		 ));
  
  string extensions;
  
#if defined(MIKTEX_WINDOWS)
  if (! Utils::GetEnvironmentString("PATHEXT", extensions)
      || extensions.empty())
    {
      extensions = ListBuilder(
			       ".com",
			       ".exe",
			       ".bat"
			       );
    }
#elif defined(MIKTEX_EXE_FILE_SUFFIX)
  extensions = MIKTEX_EXE_FILE_SUFFIX;
#endif
  
  string exePath;

  PathName userBinDir = GetSpecialPath(SpecialPath::UserInstallRoot);
  userBinDir += MIKTEX_PATH_BIN_DIR;
  userBinDir.Canonicalize ();

  if (! IsAdminMode()
      && ! Utils::Contains(exePath.c_str(), userBinDir.Get(), PATH_DELIMITER_STRING))
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

  if (! Utils::Contains(exePath.c_str(), commonBinDir.Get(), PATH_DELIMITER_STRING))
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
    if (! Utils::Contains(exePath.c_str(), binDir.Get(), PATH_DELIMITER_STRING))
    {
      if (! exePath.empty())
      {
	exePath += PATH_DELIMITER;
      }
      exePath += binDir.Get();
    }
  }

  PathName myLocation = GetMyLocation(true);

  if (! Utils::Contains(exePath.c_str(), myLocation.Get(), PATH_DELIMITER_STRING))
  {
    if (! exePath.empty())
    {
      exePath += PATH_DELIMITER;
    }
    exePath += myLocation.Get();
  }

  RegisterFileType
    (FileType::EXE,
     "exe",
     0,
     extensions.c_str(),
     exePath.c_str(),
     ListBuilder());

  Tokenizer engine (theNameOfTheGame.c_str(), ",;:");
  if (engine.GetCurrent() != 0)
    {
      PathName engineDir (MIKTEX_PATH_FMT_DIR);
      engineDir += engine.GetCurrent();
      RegisterFileType
	(FileType::FMT,
	 "fmt",
	 "TeX",
	 ListBuilder(
		     ".fmt"
		     ),
	 ListBuilder(
		     CURRENT_DIRECTORY,
		     SearchSpecBuilder(engineDir.Get()),
		     SearchSpecBuilder(MIKTEX_PATH_FMT_DIR)
		     ),
	 ListBuilder());
    }
  else
    {
      RegisterFileType
	(FileType::FMT,
	 "fmt",
	 "TeX",
	 ListBuilder(
		     ".fmt"
		     ),
	 ListBuilder(
		     CURRENT_DIRECTORY,
		     SearchSpecBuilder(MIKTEX_PATH_FMT_DIR)
		     ),
	 ListBuilder());
    }
  
  RegisterFileType
    (FileType::HBF,
     "hbf",
     0,
     ListBuilder(
		 ".hbf"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_HBF_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_TYPE1_DIR)
		 ),
     ListBuilder());
  
  RegisterFileType
    (FileType::GRAPHICS,
     "graphic/figure",
     0,
     ListBuilder(
		 ".eps",
		 ".epsi",
		 ".png"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_DVIPS_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_PDFTEX_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_TEX_DIR)
		 ),
     ListBuilder("TEXPICTS",
		 "TEXINPUTS"));
  
  // <todo>FileType::GF</todo>

  RegisterFileType
    (FileType::IST,
     "ist",
     "MakeIndex",
     ListBuilder(
		 ".ist"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_MAKEINDEX_DIR)
		 ),
     ListBuilder(
		 "TEXINDEXSTYLE",
		 "INDEXSTYLE"
		 ));
  
  RegisterFileType
    (FileType::MAP,
     "map",
     0,
     ListBuilder(
		 ".map"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_MAP_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_MIKTEX_CONFIG_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_DVIPS_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_PDFTEX_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_DVIPDFM_DIR)
		 ),
     ListBuilder(
		 "TEXFONTMAPS",
		 "TEXFONTS"
		 ));

  RegisterFileType
    (FileType::MEM,
     "mem",
     "MetaPost",
     ListBuilder(
		 ".mem"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_MEM_DIR)
		 ),
     ListBuilder());
  
  RegisterFileType
    (FileType::MF,
     "mf",
     "METAFONT",
     ListBuilder(
		 ".mf"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_METAFONT_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_FONT_SOURCE_DIR)
		 ),
     ListBuilder(
		 "MFINPUTS"
		 ));

  // <todo>FileType::MFT</todo>
  // <todo>FileType::MISCFONT</todo>

  RegisterFileType
    (FileType::MP,
     "mp",
     "MetaPost",
     ListBuilder(
		 ".mp"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_METAPOST_DIR)
		 ),
     ListBuilder(
		 "MPINPUTS"
		 ));

  RegisterFileType
    (FileType::OCP,
     "ocp",
     "Omega",
     ListBuilder(
		 ".ocp"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OCP_DIR)
		 ),
     ListBuilder(
		 "OCPINPUTS"
		 ));

  RegisterFileType
    (FileType::OFM,
     "ofm",
     "Omega",
     ListBuilder(
		 ".ofm",
		 ".tfm"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OFM_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_TFM_DIR)
		 ),
     ListBuilder(
		 "OFMFONTS",
		 "TEXFONTS"
		 ));

  // <todo>FileType::OPL</todo>

  RegisterFileType
    (FileType::PROGRAMBINFILE,
     "other binary files",
     0,
     ListBuilder(),
     applicationSearchPath.c_str(),
     ListBuilder());

  RegisterFileType
    (FileType::PROGRAMTEXTFILE,
     "other text files",
     0,
     ListBuilder(),
     applicationSearchPath.c_str(),
     ListBuilder());
  
  RegisterFileType
    (FileType::OTP,
     "otp",
     "otp2ocp",
     ListBuilder(
		 ".otp"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OTP_DIR)
		 ),
     ListBuilder(
		 "OTPINPUTS"
		 ));

  RegisterFileType
    (FileType::OVF,
     "ovf",
     0,
     ListBuilder(
		 ".ovf"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OVF_DIR)
		 ),
     ListBuilder(
		 "OVFFONTS",
		 "TEXFONTS"
		 ));
  
  RegisterFileType
    (FileType::OVP,
     "ovp",
     0,
     ListBuilder(
		 ".ovp"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OVP_DIR)
		 ),
     ListBuilder(
		 "OVPFONTS",
		 "TEXFONTS"
		 ));

  RegisterFileType
    (FileType::PERLSCRIPT,
     "perlscript",
     0,
     ListBuilder(
		 ".pl"
		 ),
     ListBuilder(
		 SearchSpecBuilder(MIKTEX_PATH_SCRIPT_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_CONTEXT_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_MIKTEX_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_NTS_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_PDFTEX_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_PSUTILS_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_SOURCE_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_TEX_DIR)
		 ),
     ListBuilder());

  // <todo>FileType::PK</todo>

  string psFontDirs;
  if (! SessionImpl::GetSession()->GetPsFontDirs(psFontDirs))
    {
      psFontDirs = "";
    }

  RegisterFileType
    (FileType::PSHEADER,
     "PostScript header",
     0,
     ListBuilder(
		 ".pro",
		 ".enc"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_MIKTEX_CONFIG_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_DVIPS_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_PDFTEX_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_DVIPDFM_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_TYPE1_DIR),
		 (psFontDirs.empty() ? 0 : psFontDirs.c_str())
		 ),
     ListBuilder(
		 "TEXPSHEADERS",
		 "PSHEADERS"
		 ));

  RegisterFileType
    (FileType::SCRIPT,
     "texmfscripts",
     0,
     ListBuilder(
		 ".lua",
		 ".pl",
		 ".py",
		 ".rb"
		 ),
     ListBuilder(
		 SearchSpecBuilder(MIKTEX_PATH_SCRIPT_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_CONTEXT_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_MIKTEX_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_NTS_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_PDFTEX_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_PSUTILS_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_SOURCE_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_TEX_DIR)
		 ),
     ListBuilder(
		 "TEXMFSCRIPTS"
		 ));

  RegisterFileType
    (FileType::TCX,
     "tcx",
     0,
     ListBuilder(
		 ".tcx"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(false,
				   MIKTEX_PATH_MIKTEX_CONFIG_DIR),
		 SearchSpecBuilder(false,
				   MIKTEX_PATH_WEB2C_DIR)
		 ),
     ListBuilder());
  
  RegisterFileType
    (FileType::TEX,
     "tex",
     "TeX",
     ListBuilder(
		 ".tex"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_TEX_DIR)
		 ),
     ListBuilder(
		 "TEXINPUTS"
		 ));

  RegisterFileType
    (FileType::TEXSYSDOC,
     "TeX system documentation",
     0,
     ListBuilder(
#if defined(MIKTEX_WINDOWS)
		 ".chm",
#endif
		 ".dvi",
		 ".html"
		 ".txt",
		 ".pdf",
		 ".ps"
		 ),
     ListBuilder(
		 SearchSpecBuilder(MIKTEX_PATH_MIKTEX_DOC_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_DOC_DIR)
		 ),
     ListBuilder());

  RegisterFileType
    (FileType::TFM,
     "tfm",
     0,
     ListBuilder(
		 ".tfm"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_TFM_DIR)
		 ),
     ListBuilder(
		 "TFMFONTS",
		 "TEXFONTS"
		 ));

  string ttfFontDirs;
  if (! SessionImpl::GetSession()->GetTTFDirs(ttfFontDirs))
    {
      ttfFontDirs = "";
    }

  RegisterFileType
    (FileType::TTF,
     "truetype fonts",
     0,
     ListBuilder(
		 ".ttf",
		 ".ttc"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_TRUETYPE_DIR),
		 (ttfFontDirs.empty() ? 0 : ttfFontDirs.c_str())
		 ),
     ListBuilder(
		 "TTFONTS",
		 "TEXFONTS"
		 ));

  string otfFontDirs;
  if (! SessionImpl::GetSession()->GetOTFDirs(otfFontDirs))
    {
      otfFontDirs = "";
    }

  RegisterFileType
    (FileType::OTF,
     "opentype fonts",
     0,
     ListBuilder(
		 ".otf"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OPENTYPE_DIR),
		 (otfFontDirs.empty() ? 0 : otfFontDirs.c_str())
		 ),
     ListBuilder(
		 "OPENTYPEFONTS",
		 "TEXFONTS"
		 ));

  RegisterFileType
    (FileType::TYPE1,
     "type1 fonts",
     0,
     ListBuilder(
		 ".pfb",
		 ".pfa"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_TYPE1_DIR),
		 (psFontDirs.empty() ? 0 : psFontDirs.c_str())
		 ),
     ListBuilder(
		 "T1FONTS",
		 "T1INPUTS",
		 "TEXFONTS",
		 "TEXPSHEADERS",
		 "PSHEADERS"
		 ));

  RegisterFileType
    (FileType::SFD,
     "subfont definition files",
     0,
     ListBuilder(
		 ".sfd"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_SFD_DIR)
		 ),
     ListBuilder(
		 "SFDFONTS",
		 "TEXFONTS"
		 ));

  RegisterFileType
    (FileType::CMAP,
     "cmap files",
     0,
     ListBuilder(),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_CMAP_DIR)
		 ),
     ListBuilder(
		 "CMAPFONTS",
		 "TEXFONTS"
		 ));

  RegisterFileType
    (FileType::VF,
     "vf",
     0,
     ListBuilder(
		 ".vf"
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_VF_DIR)
		 ),
     ListBuilder(
		 "VFFONTS",
		 "TEXFONTS"
		 ));

  // <todo>FileType::WEB</todo>
  RegisterFileType
    (FileType::WEB2C,
     "web2c files",
     0,
     ListBuilder(),
     ListBuilder(
		 SearchSpecBuilder(MIKTEX_PATH_WEB2C_DIR)
		 ),
     ListBuilder());

#if defined(MIKTEX_WINDOWS)
  RegisterFileType
    (FileType::WindowsCommandScriptFile,
     "Windows command script file",
     0,
     ListBuilder(".bat",
		 ".cmd"),
     ListBuilder(
		 SearchSpecBuilder(MIKTEX_PATH_SCRIPT_DIR),
		 exePath.c_str()
		 ),
     ListBuilder());
#endif
}

/* _________________________________________________________________________

   SessionImpl::GetFileTypeInfo
   _________________________________________________________________________ */

InternalFileTypeInfo *
SessionImpl::GetFileTypeInfo (/*[in]*/ FileType		fileType)
{
  if (fileTypes.size() == 0)
    {
      RegisterFileTypes ();
    }
  if (static_cast<size_t>(fileType.Get()) >= fileTypes.size())
    {
      INVALID_ARGUMENT ("SessionImpl::GetFileTypeInfo",
			NUMTOSTR(fileType.Get()));
    }
  return (&fileTypes[fileType.Get()]);
}

/* _________________________________________________________________________

   SessionImpl::DeriveFileType
   _________________________________________________________________________ */

FileType
SessionImpl::DeriveFileType (/*[in]*/ const char * lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  if (fileTypes.size() == 0)
    {
      RegisterFileTypes ();
    }

  const char * lpszExt = GetFileNameExtension(lpszPath);

  for (vector<InternalFileTypeInfo>::const_iterator it = fileTypes.begin();
       it != fileTypes.end();
       ++ it)
    {
      if (lpszExt == 0)
	{
	  if (StringCompare(it->fileTypeString.c_str(), lpszPath) == 0)
	    {
	      return (it->fileType);
	    }
	}
      else
	{
	  for (CSVList ext (it->fileNameExtensions.c_str(), PATH_DELIMITER);
	       ext.GetCurrent() != 0;
	       ++ ext)
	    {
	      if (PathName::Compare(ext.GetCurrent(), lpszExt) == 0)
		{
		  return (it->fileType);
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
  if (fileTypes.size() == 0)
    {
      RegisterFileTypes ();
    }
  if (index == fileTypes.size())
    {
      return (false);
    }
  if (index > fileTypes.size())
    {
      INVALID_ARGUMENT ("SessionImpl::GetNextFileTypeInfo",
			NUMTOSTR(index));
    }
  fileTypeInfo = fileTypes[index];
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
