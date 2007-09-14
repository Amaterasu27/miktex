/* filetypes.cpp: MiKTeX file type registry

   Copyright (C) 1996-2007 Christian Schenk

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
	     + StrLen(lpszElement1)
	     + (lpszElement2 == 0 ? 0 : StrLen(lpszElement2) + 1)
	     + (lpszElement3 == 0 ? 0 : StrLen(lpszElement3) + 1)
	     + (lpszElement4 == 0 ? 0 : StrLen(lpszElement4) + 1)
	     + (lpszElement5 == 0 ? 0 : StrLen(lpszElement5) + 1)
	     + (lpszElement6 == 0 ? 0 : StrLen(lpszElement6) + 1)
	     + (lpszElement7 == 0 ? 0 : StrLen(lpszElement7) + 1)
	     + (lpszElement8 == 0 ? 0 : StrLen(lpszElement8) + 1)
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
SessionImpl::RegisterFileType
(/*[in]*/ FileType		fileType,
 /*[in]*/ const MIKTEXCHAR *	lpszFileType,
 /*[in]*/ const MIKTEXCHAR *	lpszApplication,
 /*[in]*/ const MIKTEXCHAR *	lpszFileNameExtensions,
 /*[in]*/ const MIKTEXCHAR *	lpszDefaultSearchPath,
 /*[in]*/ const MIKTEXCHAR *	lpszEnvVarNames)
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
  tstring section = T_("ft.");
  section += lpszFileType;
  fti.fileNameExtensions =
    GetConfigValue(section.c_str(),
		   T_("extensions"),
		   lpszFileNameExtensions);
  fti.searchPath =
    GetConfigValue(section.c_str(),
		   T_("path"),
		   lpszDefaultSearchPath);
  fti.envVarNames =
    GetConfigValue(section.c_str(),
		   T_("env"),
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
  tstring applicationName = app.GetCurrent();
 
  tstring applicationSearchPath = CURRENT_DIRECTORY;
  applicationSearchPath += PATH_DELIMITER;
  applicationSearchPath += TEXMF_PLACEHOLDER;
  applicationSearchPath += MIKTEX_PATH_DIRECTORY_DELIMITER_STRING;
  applicationSearchPath += applicationName;
  applicationSearchPath += RECURSION_INDICATOR;

  RegisterFileType
    (FileType::AFM,
     T_("afm"),
     0,
     ListBuilder(
		 T_(".afm")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_AFM_DIR)
		 ),
     ListBuilder(
		 T_("AFMFONTS"),
		 T_("TEXFONTS")
		 ));
  
  RegisterFileType
    (FileType::BASE,
     T_("base"),
     T_("METAFONT"),
     ListBuilder(
		 T_(".base")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_BASE_DIR)
		 ),
     ListBuilder());
  
  RegisterFileType
    (FileType::BIB,
     T_("bib"),
     T_("BibTeX"),
     ListBuilder(
		 T_(".bib")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_BIBTEX_DIR)
		 ),
     ListBuilder(
		 T_("BIBINPUTS"),
		 T_("TEXBIB")
		 ));

  RegisterFileType
    (FileType::BST,
     T_("bst"),
     T_("BibTeX"),
     ListBuilder(
		 T_(".bst")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_BIBTEX_DIR)
		 ),
     ListBuilder(
		 T_("BSTINPUTS")
		 ));

  RegisterFileType
    (FileType::DVI,
     T_("dvi"),
     0,
     ListBuilder(
		 T_(".dvi")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_DOC_DIR)
		 ),
     ListBuilder());

  RegisterFileType
    (FileType::DVIPSCONFIG,
     T_("dvips config"),
     T_("Dvips"),
     ListBuilder(),
     ListBuilder(
		 SearchSpecBuilder(MIKTEX_PATH_DVIPS_DIR)
		 ),
     ListBuilder(
		 T_("TEXCONFIG")
		 ));
  
  RegisterFileType
    (FileType::ENC,
     T_("enc"),
     0,
     ListBuilder(
		 T_(".enc")
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
		 T_("ENCFONTS"),
		 T_("TEXFONTS")
		 ));
  
  tstring extensions;
  
#if defined(MIKTEX_WINDOWS)
  if (! Utils::GetEnvironmentString(T_("PATHEXT"), extensions)
      || extensions.empty())
    {
      extensions = ListBuilder(
			       T_(".com"),
			       T_(".exe"),
			       T_(".bat")
			       );
    }
#elif defined(MIKTEX_EXE_FILE_SUFFIX)
  extensions = MIKTEX_EXE_FILE_SUFFIX;
#endif
  
  PathName binDir = GetSpecialPath(SpecialPath::BinDirectory);
  PathName myLocation = GetMyLocation();

  tstring exePath = binDir.Get();
  if (binDir != myLocation)
    {
      exePath += PATH_DELIMITER;
      exePath += myLocation.Get();
    }

  RegisterFileType
    (FileType::EXE,
     T_("exe"),
     0,
     extensions.c_str(),
     exePath.c_str(),
     ListBuilder());

  RegisterFileType
    (FileType::FMT,
     T_("fmt"),
     T_("TeX"),
     ListBuilder(
		 T_(".fmt")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_FMT_DIR)
		 ),
     ListBuilder());
  
  RegisterFileType
    (FileType::HBF,
     T_("hbf"),
     0,
     ListBuilder(
		 T_(".hbf")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_HBF_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_TYPE1_DIR)
		 ),
     ListBuilder());
  
  RegisterFileType
    (FileType::GRAPHICS,
     T_("graphic/figure"),
     0,
     ListBuilder(
		 T_(".eps"),
		 T_(".epsi"),
		 T_(".png")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_DVIPS_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_PDFTEX_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_TEX_DIR)
		 ),
     ListBuilder(T_("TEXPICTS"),
		 T_("TEXINPUTS")));
  
  // <todo>FileType::GF</todo>

  RegisterFileType
    (FileType::IST,
     T_("ist"),
     T_("MakeIndex"),
     ListBuilder(
		 T_(".ist")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_MAKEINDEX_DIR)
		 ),
     ListBuilder(
		 T_("TEXINDEXSTYLE"),
		 T_("INDEXSTYLE")
		 ));
  
  RegisterFileType
    (FileType::MAP,
     T_("map"),
     0,
     ListBuilder(
		 T_(".map")
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
		 T_("TEXFONTMAPS"),
		 T_("TEXFONTS")
		 ));

  RegisterFileType
    (FileType::MEM,
     T_("mem"),
     T_("MetaPost"),
     ListBuilder(
		 T_(".mem")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_MEM_DIR)
		 ),
     ListBuilder());
  
  RegisterFileType
    (FileType::MF,
     T_("mf"),
     T_("METAFONT"),
     ListBuilder(
		 T_(".mf")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_METAFONT_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_FONT_SOURCE_DIR)
		 ),
     ListBuilder(
		 T_("MFINPUTS")
		 ));

  RegisterFileType
    (FileType::MFPOOL,
     T_("mfpool"),
     T_("METAFONT"),
     ListBuilder(
		 T_(".pool")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_BASE_DIR)
		 ),
     ListBuilder());
  
  // <todo>FileType::MFT</todo>
  // <todo>FileType::MISCFONT</todo>

  RegisterFileType
    (FileType::MP,
     T_("mp"),
     T_("MetaPost"),
     ListBuilder(
		 T_(".mp")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_METAPOST_DIR)
		 ),
     ListBuilder(
		 T_("MPINPUTS")
		 ));

  RegisterFileType
    (FileType::MPPOOL,
     T_("mppool"),
     T_("MetaPost"),
     ListBuilder(
		 T_(".pool")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_MEM_DIR)
		 ),
     ListBuilder());

  RegisterFileType
    (FileType::OCP,
     T_("ocp"),
     T_("Omega"),
     ListBuilder(
		 T_(".ocp")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OCP_DIR)
		 ),
     ListBuilder(
		 T_("OCPINPUTS")
		 ));

  RegisterFileType
    (FileType::OFM,
     T_("ofm"),
     T_("Omega"),
     ListBuilder(
		 T_(".ofm"),
		 T_(".tfm")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OFM_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_TFM_DIR)
		 ),
     ListBuilder(
		 T_("OFMFONTS"),
		 T_("TEXFONTS")
		 ));

  // <todo>FileType::OPL</todo>

  RegisterFileType
    (FileType::PROGRAMBINFILE,
     T_("other binary files"),
     0,
     ListBuilder(),
     applicationSearchPath.c_str(),
     ListBuilder());

  RegisterFileType
    (FileType::PROGRAMTEXTFILE,
     T_("other text files"),
     0,
     ListBuilder(),
     applicationSearchPath.c_str(),
     ListBuilder());
  
  RegisterFileType
    (FileType::OTP,
     T_("otp"),
     T_("otp2ocp"),
     ListBuilder(
		 T_(".otp")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OTP_DIR)
		 ),
     ListBuilder(
		 T_("OTPINPUTS")
		 ));

  RegisterFileType
    (FileType::OVF,
     T_("ovf"),
     0,
     ListBuilder(
		 T_(".ovf")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OVF_DIR)
		 ),
     ListBuilder(
		 T_("OVFFONTS"),
		 T_("TEXFONTS")
		 ));
  
  RegisterFileType
    (FileType::OVP,
     T_("ovp"),
     0,
     ListBuilder(
		 T_(".ovp")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OVP_DIR)
		 ),
     ListBuilder(
		 T_("OVPFONTS"),
		 T_("TEXFONTS")
		 ));

  RegisterFileType
    (FileType::PERLSCRIPT,
     T_("perlscript"),
     0,
     ListBuilder(
		 T_(".pl")
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

  tstring psFontDirs;
  if (! SessionImpl::GetSession()->GetPsFontDirs(psFontDirs))
    {
      psFontDirs = T_("");
    }

  RegisterFileType
    (FileType::PSHEADER,
     T_("PostScript header"),
     0,
     ListBuilder(
		 T_(".pro"),
		 T_(".enc")
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
		 T_("TEXPSHEADERS"),
		 T_("PSHEADERS")
		 ));

  RegisterFileType
    (FileType::SCRIPT,
     T_("texmfscripts"),
     0,
     ListBuilder(
		 T_(".lua"),
		 T_(".pl"),
		 T_(".py"),
		 T_(".rb")
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
		 T_("TEXMFSCRIPTS")
		 ));

  RegisterFileType
    (FileType::TCX,
     T_("tcx"),
     0,
     ListBuilder(
		 T_(".tcx")
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
     T_("tex"),
     T_("TeX"),
     ListBuilder(
		 T_(".tex")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_TEX_DIR)
		 ),
     ListBuilder(
		 T_("TEXINPUTS")
		 ));

  RegisterFileType
    (FileType::TEXPOOL,
     T_("texpool"),
     T_("TeX"),
     ListBuilder(
		 T_(".pool")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_FMT_DIR)
		 ),
     ListBuilder());
	    
  RegisterFileType
    (FileType::TEXSYSDOC,
     T_("TeX system documentation"),
     0,
     ListBuilder(
#if defined(MIKTEX_WINDOWS)
		 T_(".chm"),
#endif
		 T_(".dvi"),
		 T_(".html")
		 T_(".txt"),
		 T_(".pdf"),
		 T_(".ps")
		 ),
     ListBuilder(
		 SearchSpecBuilder(MIKTEX_PATH_MIKTEX_DOC_DIR),
		 SearchSpecBuilder(MIKTEX_PATH_DOC_DIR)
		 ),
     ListBuilder());

  RegisterFileType
    (FileType::TFM,
     T_("tfm"),
     0,
     ListBuilder(
		 T_(".tfm")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_TFM_DIR)
		 ),
     ListBuilder(
		 T_("TFMFONTS"),
		 T_("TEXFONTS")
		 ));

  tstring ttfFontDirs;
  if (! SessionImpl::GetSession()->GetTTFDirs(ttfFontDirs))
    {
      ttfFontDirs = T_("");
    }

  RegisterFileType
    (FileType::TTF,
     T_("truetype fonts"),
     0,
     ListBuilder(
		 T_(".ttf"),
		 T_(".ttc")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_TRUETYPE_DIR),
		 (ttfFontDirs.empty() ? 0 : ttfFontDirs.c_str())
		 ),
     ListBuilder(
		 T_("TTFONTS"),
		 T_("TEXFONTS")
		 ));

  tstring otfFontDirs;
  if (! SessionImpl::GetSession()->GetOTFDirs(otfFontDirs))
    {
      otfFontDirs = T_("");
    }

  RegisterFileType
    (FileType::OTF,
     T_("opentype fonts"),
     0,
     ListBuilder(
		 T_(".otf")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_OPENTYPE_DIR),
		 (otfFontDirs.empty() ? 0 : otfFontDirs.c_str())
		 ),
     ListBuilder(
		 T_("OPENTYPEFONTS"),
		 T_("TEXFONTS")
		 ));

  RegisterFileType
    (FileType::TYPE1,
     T_("type1 fonts"),
     0,
     ListBuilder(
		 T_(".pfb"),
		 T_(".pfa")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_TYPE1_DIR),
		 (psFontDirs.empty() ? 0 : psFontDirs.c_str())
		 ),
     ListBuilder(
		 T_("T1FONTS"),
		 T_("T1INPUTS"),
		 T_("TEXFONTS"),
		 T_("TEXPSHEADERS"),
		 T_("PSHEADERS")
		 ));

  RegisterFileType
    (FileType::VF,
     T_("vf"),
     0,
     ListBuilder(
		 T_(".vf")
		 ),
     ListBuilder(
		 CURRENT_DIRECTORY,
		 SearchSpecBuilder(MIKTEX_PATH_VF_DIR)
		 ),
     ListBuilder(
		 T_("VFFONTS"),
		 T_("TEXFONTS")
		 ));

  // <todo>FileType::WEB</todo>
  RegisterFileType
    (FileType::WEB2C,
     T_("web2c files"),
     0,
     ListBuilder(),
     ListBuilder(
		 SearchSpecBuilder(MIKTEX_PATH_WEB2C_DIR)
		 ),
     ListBuilder());

#if defined(MIKTEX_WINDOWS)
  RegisterFileType
    (FileType::WindowsCommandScriptFile,
     T_("Windows command script file"),
     0,
     ListBuilder(T_(".bat"),
		 T_(".cmd")),
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
      INVALID_ARGUMENT (T_("SessionImpl::GetFileTypeInfo"),
			NUMTOSTR(fileType.Get()));
    }
  return (&fileTypes[fileType.Get()]);
}

/* _________________________________________________________________________

   SessionImpl::DeriveFileType
   _________________________________________________________________________ */

FileType
SessionImpl::DeriveFileType (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  if (fileTypes.size() == 0)
    {
      RegisterFileTypes ();
    }

  const MIKTEXCHAR * lpszExt = GetFileNameExtension(lpszPath);

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
      INVALID_ARGUMENT (T_("SessionImpl::GetNextFileTypeInfo"),
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
