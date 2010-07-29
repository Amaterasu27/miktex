/* kpsemu.cpp: kpathsea emulation

   Copyright (C) 1994, 95 Karl Berry
   Copyright (C) 2000-2010 Christian Schenk

   This file is part of the MiKTeX KPSEMU Library.

   The MiKTeX KPSEMU Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX KPSEMU Library is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX KPSEMU Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

using namespace std;
using namespace MiKTeX;
using namespace MiKTeX::Core;

namespace {
  unsigned kpse_baseResolution = 600;
  std::string kpse_mode;
}

/* _________________________________________________________________________

   miktex_kpathsea_version_string
   _________________________________________________________________________ */

MIKTEXKPSDATA(char *)
miktex_kpathsea_version_string = KPSEVERSION;

/* _________________________________________________________________________

   miktex_kpathsea_bug_address
   _________________________________________________________________________ */

MIKTEXKPSDATA(char *)
miktex_kpathsea_bug_address = T_("Visit miktex.org for bug reports.");

/* _________________________________________________________________________

   kpse_def
   kpse_def_inst
   _________________________________________________________________________ */

#if defined(KPSE_COMPAT_API)

MIKTEXKPSDATA(kpathsea_instance)
miktex_kpse_def_inst;

MIKTEXKPSDATA(kpathsea)
miktex_kpse_def = &miktex_kpse_def_inst;

#endif

/* _________________________________________________________________________

   ToUnix
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(char *)
ToUnix (/*[in]*/ char * lpsz)
{
  char * lpszRet = lpsz;
#if defined(MIKTEX_WINDOWS)
  for (; *lpsz != 0; ++ lpsz)
  {
    if (*lpsz == '\\')
    {
      *lpsz = '/';
    }
  }
#endif
  return (lpszRet);
}

/* _________________________________________________________________________

   ToUnix
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(std::string &)
ToUnix (/*[in]*/ std::string & s)
{
#if defined(MIKTEX_WINDOWS)
  for (std::string::iterator it = s.begin(); it != s.end(); ++ it)
  {
    if (*it == '\\')
    {
      *it = '/';
    }
  }
#endif
  return (s);
}

/* _________________________________________________________________________

   magstep

   <fixme>duplicate</fixme>
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(int)
magstep (/*[in]*/ int	n,
	 /*[in]*/ int	bdpi)
{
  double t;
  bool neg = false;
  if (n < 0)
    {
      neg = true;
      n = -n;
    }
  if (n & 1)
    {
      n &= ~1;
      t = 1.095445115;
    }
  else
    {
      t = 1.0;
    }
  while (n > 8)
    {
      n -= 8;
      t *= 2.0736;
    }
  while (n > 0)
    {
      n -= 2;
      t *= 1.2;
    }
  if (neg)
    {
      return (static_cast<int>(0.5 + bdpi / t));
    }
  else
    {
      return (static_cast<int>(0.5 + bdpi * t));
    }
}

/* _________________________________________________________________________

   miktex_kpathsea_find_glyph
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_kpathsea_find_glyph (/*[in]*/ kpathsea		      pKpseInstance,
			    /*[in]*/ const char *	      lpszFontName,
			    /*[in]*/ unsigned		      dpi,
			    /*[in]*/ kpse_file_format_type    format,
			    /*[out]*/ kpse_glyph_file_type *  glyph_file)
{
  if (format != kpse_pk_format)
    {
      // <todo/>
      return (0);
    }
  PathName path;
  if (! SessionWrapper(true)->FindPkFile(lpszFontName,
					 (kpse_mode.length() > 0
					  ? kpse_mode.c_str()
					  : 0),
					 dpi,
					 path))
    {
       PathName pathMakePk;
       char szArguments[ 1024 ];
       SessionWrapper(true)->MakeMakePkCommandLine (lpszFontName,
						    dpi,
						    kpse_baseResolution,
						    (kpse_mode.length() > 0
						     ? kpse_mode.c_str()
						     : 0),
						    pathMakePk,
						    szArguments,
						    1024);
       if (! (Process::Run(pathMakePk.Get(),
			   szArguments,
			   reinterpret_cast<IRunProcessCallback*>(0),
			   0,
			   0)
	      && SessionWrapper(true)->FindPkFile(lpszFontName, 0, dpi, path)))
	 {
	   return (0);
	 }
    }
  char * lpsz = xstrdup(path.Get());
  if (glyph_file != 0)
    {
      glyph_file->name = const_cast<char*>(lpszFontName);
      glyph_file->dpi = dpi;
      glyph_file->format = format;
      glyph_file->source = kpse_glyph_source_normal;
    }
  return (lpsz);
}

/* _________________________________________________________________________

   ToStringList
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(const char **)
ToStringList (/*[in]*/ const vector<std::string> & vec)
{
  const char ** pStringList = XTALLOC(vec.size() + 1, const char *);
  for (int idx = 0; idx < vec.size(); ++ idx)
  {
    pStringList[idx] = xstrdup(vec[idx].c_str());
  }
  pStringList[vec.size()] = 0;
  return (pStringList);
}

/* _________________________________________________________________________

   ToStringList
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
const char PATH_DELIMITER = ';';
#else
const char PATH_DELIMITER = ':';
#endif

MIKTEXSTATICFUNC(const char **)
ToStringList (/*[in]*/ const std::string & str)
{
  vector<std::string> vec;
  for (CSVList s (str.c_str(), PATH_DELIMITER);
    s.GetCurrent() != 0;
    ++ s)
  {
    vec.push_back (s.GetCurrent());
  }
  return (ToStringList(vec));
}

/* _________________________________________________________________________

   ToFileType
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(FileType)
ToFileType (/*[in]*/ kpse_file_format_type format)
{
  FileType ft (FileType::None);
  switch (format)
  {
  case kpse_afm_format: ft = FileType::AFM; break;
  case kpse_any_glyph_format: ft = FileType::GLYPHFONT; break;
  case kpse_base_format: ft = FileType::BASE; break;
  case kpse_bib_format: ft = FileType::BIB; break;
  case kpse_bst_format: ft = FileType::BST; break;
  case kpse_cid_format: ft = FileType::CID; break;
  case kpse_clua_format: ft = FileType::CLUA; break;
  case kpse_cmap_format: ft = FileType::CMAP; break;
  case kpse_cnf_format: ft = FileType::CNF; break;
  case kpse_cweb_format: ft = FileType::CWEB; break;
  case kpse_db_format: ft = FileType::DB; break;
  case kpse_dvips_config_format: ft = FileType::DVIPSCONFIG; break;
  case kpse_enc_format: ft = FileType::ENC; break;
  case kpse_fea_format: ft = FileType::FEA; break;
  case kpse_fontmap_format: ft = FileType::MAP; break;
  case kpse_fmt_format: ft = FileType::FMT; break;
  case kpse_gf_format: ft = FileType::GF; break;
  case kpse_ist_format: ft = FileType::IST; break;
  case kpse_lig_format: ft = FileType::LIG; break;
  case kpse_lua_format: ft = FileType::LUA; break;
  case kpse_mem_format: ft = FileType::MEM; break;
  case kpse_mf_format: ft = FileType::MF; break;
  case kpse_mfpool_format: ft = FileType::MFPOOL; break;
  case kpse_mft_format: ft = FileType::MFT; break;
  case kpse_mlbib_format: ft = FileType::MLBIB; break;
  case kpse_mlbst_format: ft = FileType::MLBST; break;
  case kpse_miscfonts_format: ft = FileType::MISCFONT; break;
  case kpse_mp_format: ft = FileType::MP; break;
  case kpse_mppool_format: ft = FileType::MPPOOL; break;
  case kpse_mpsupport_format: ft = FileType::MPSUPPORT; break;
  case kpse_ocp_format: ft = FileType::OCP; break;
  case kpse_ofm_format: ft = FileType::OFM; break;
  case kpse_opentype_format: ft = FileType::OTF; break;
  case kpse_opl_format: ft = FileType::OPL; break;
  case kpse_otp_format: ft = FileType::OTP; break;
  case kpse_ovf_format: ft = FileType::OVF; break;
  case kpse_ovp_format: ft = FileType::OVP; break;
  case kpse_pdftex_config_format: ft = FileType::PDFTEXCONFIG; break;
  case kpse_pict_format: ft = FileType::GRAPHICS; break;
  case kpse_pk_format: ft = FileType::PK; break;
  case kpse_program_binary_format: ft = FileType::PROGRAMBINFILE; break;
  case kpse_program_text_format: ft = FileType::PROGRAMTEXTFILE; break;
  case kpse_sfd_format: ft = FileType::SFD; break;
  case kpse_tex_format: ft = FileType::TEX; break;
  case kpse_tex_ps_header_format: ft = FileType::PSHEADER; break;
  case kpse_texpool_format: ft = FileType::TEXPOOL; break;
  case kpse_texsource_format: ft = FileType::TEXSOURCE; break;
  case kpse_texdoc_format: ft = FileType::TEXSYSDOC; break;
  case kpse_texmfscripts_format: ft = FileType::SCRIPT; break;
  case kpse_tfm_format: ft = FileType::TFM; break;
  case kpse_troff_font_format: ft = FileType::TROFF; break;
  case kpse_truetype_format: ft = FileType::TTF; break;
  case kpse_type1_format: ft = FileType::TYPE1; break;
  case kpse_type42_format: ft = FileType::TYPE42; break;
  case kpse_vf_format: ft = FileType::VF; break;
  case kpse_web2c_format: ft = FileType::WEB2C; break;
  case kpse_web_format: ft = FileType::WEB; break;

  default:
    Session::FatalMiKTeXError
      ("ToFileType",
      (std::string(T_("Unsupported kpse file format: "))
      + NUMTOSTR(format)).c_str(),
      0,
      __FILE__,
      __LINE__);
  }
  return (ft);
}

/* _________________________________________________________________________

   miktex_kpathsea_find_file
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_kpathsea_find_file (/*[in]*/ kpathsea			pKpseInstance,
			   /*[in]*/ const char *		lpszFileName,
			   /*[in]*/ kpse_file_format_type	format,
			   /*[in]*/ int				mustExist)
{
  MIKTEX_ASSERT (pKpseInstance != 0);
  MIKTEX_ASSERT (lpszFileName != 0);
  bool found = false;
  PathName result;
  Session * pSession = Session::Get();
  FileType ft = ToFileType(format);
  switch (ft.Get())
  {
  case FileType::TFM:
    found = pSession->FindTfmFile(lpszFileName, result, mustExist ? true : false);
    break;
  default:
    found = pSession->FindFile(lpszFileName, ft, result);
    break;
  }
  if (! found)
  {
    return (0);
  }
  result.ToUnix ();
  return (xstrdup(result.Get()));
}

/* _________________________________________________________________________

   miktex_kpathsea_find_file_generic
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char **)
miktex_kpathsea_find_file_generic (/*in*/ kpathsea			pKpseInstance,
				   /*[in]*/ const char *		lpszFileName,
				   /*[in]*/ kpse_file_format_type	format,
				   /*[in]*/ boolean			mustExist,
				   /*[in]*/ boolean			all)
{
  MIKTEX_ASSERT (pKpseInstance != 0);
  MIKTEX_ASSERT (lpszFileName != 0);
  bool found = false;
  PathNameArray result;
  Session * pSession = Session::Get();
  FileType ft = ToFileType(format);
  switch (ft.Get())
  {
  case FileType::TFM:
    {
      PathName path;
      found = pSession->FindTfmFile(lpszFileName, path, mustExist ? true : false);
      if (found)
      {
	result.push_back (path);
      }
    }
    break;
  default:
    found = pSession->FindFile(lpszFileName, ft, result);
    break;
  }
  if (! found)
  {
    return (0);
  }
  char ** pStringList = XTALLOC(result.size() + 1, char *);
  for (int idx = 0; idx < result.size(); ++ idx)
  {
    result[idx].ToUnix ();
    pStringList[idx] = xstrdup(result[idx].Get());
  }
  pStringList[result.size()] = 0;
  return (pStringList);
}

/* _________________________________________________________________________

   IsBinary
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
IsBinary (/*[in]*/ kpse_file_format_type	format)
{
  bool bBin = false;
  switch (format)
    {
    case kpse_tfm_format:
    case kpse_miscfonts_format:
    case kpse_pict_format:
    case kpse_type1_format:
    case kpse_vf_format:
    case kpse_otp_format:
    case kpse_ocp_format:
    case kpse_truetype_format:
    case kpse_opentype_format:
      bBin = true;
      break;
    }
  return (bBin);
}

/* _________________________________________________________________________

   TranslateModeString
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
TranslateModeString (/*[in]*/ const char *	lpszMode,
		     /*[out]*/ FileMode &	mode,
		     /*[out]*/ FileAccess &	access,
		     /*[out]*/ bool &		isTextFile)
{
  if (StringCompare(lpszMode, "r") == 0)
    {
      mode = FileMode::Open;
      access = FileAccess::Read;
      isTextFile = true;
    }
  else if (StringCompare(lpszMode, "w") == 0)
    {
      mode = FileMode::Create;
      access = FileAccess::Write;
      isTextFile = true;
    }
  else if (StringCompare(lpszMode, "rb") == 0)
    {
      mode = FileMode::Open;
      access = FileAccess::Read;
      isTextFile = false;
    }
  else if (StringCompare(lpszMode, "wb") == 0)
    {
      mode = FileMode::Create;
      access = FileAccess::Write;
      isTextFile = false;
    }
  else if (StringCompare(lpszMode, "ab") == 0)
    {
      mode = FileMode::Append;
      access = FileAccess::Write;
      isTextFile = false;
    }
  else
    {
      Session::FatalMiKTeXError ("TranslateModeString",
				 T_("Unsupported mode."),
				 lpszMode,
				 __FILE__,
				 __LINE__);

    }
}

/* _________________________________________________________________________

   TryFOpen
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(FILE *)
TryFOpen (/*[in]*/ const char *	lpszFileName,
	  /*[in]*/ const char *	lpszMode)
{
  FileMode mode (FileMode::Open);
  FileAccess access (FileAccess::Read);
  bool isTextFile;
  TranslateModeString (lpszMode, mode, access, isTextFile);
  return (SessionWrapper(true)->TryOpenFile(lpszFileName,
					    mode,
					    access,
					    isTextFile));
}

/* _________________________________________________________________________

   miktex_kpathsea_open_file
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(FILE *)
miktex_kpathsea_open_file (/*in*/ kpathsea	pKpathseaInstance,
			   /*[in]*/ const char *		lpszFileName,
			   /*[in]*/ kpse_file_format_type	format)
{
  MIKTEX_ASSERT (lpszFileName != 0);
  char * lpszPath = kpse_find_file(lpszFileName, format, 1);
  if (lpszPath == 0)
    {
      Session::FatalMiKTeXError ("miktex_kpse_open_file",
				 T_("File not found."),
				 lpszFileName,
				 __FILE__,
				 __LINE__);
    }
  FILE * pfile;
  try
    {
      pfile = TryFOpen(lpszPath, IsBinary(format) ? "rb" : "r");
    }
  catch (const exception &)
    {
      MIKTEX_FREE (lpszPath);
      throw;
    }
  MIKTEX_FREE (lpszPath);
  return (pfile);
}

/* _________________________________________________________________________

   miktex_concatn
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_concatn (/*[in]*/ const char * lpsz1, ...)
{
  va_list marker;
  va_start (marker, lpsz1);
  CharBuffer<char> buf;
  for (const char * lpsz = lpsz1;
       lpsz != 0;
       lpsz = va_arg(marker, const char *))
  {
    buf.Append (lpsz);
  }
  char * lpszRet = xstrdup(buf.Get());
  va_end (marker);
  return (lpszRet);
}

/* _________________________________________________________________________

   miktex_xbasename
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(const char *)
miktex_xbasename (/*[in]*/ const char * lpszFileName)
{
  const char * lpsz = lpszFileName + StrLen(lpszFileName);
  while (lpsz != lpszFileName)
    {
      -- lpsz;
      if (IsDirectoryDelimiter(*lpsz) || *lpsz == ':')
	{
	  return (lpsz + 1);
	}
    }
  return (lpszFileName);
}

/* _________________________________________________________________________

   miktex_xdirname
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_xdirname (/*[in]*/ const char * lpszFileName)
{
  return (xstrdup(PathName(lpszFileName).RemoveFileSpec().Get()));
}

/* _________________________________________________________________________

   miktex_strcasecmp
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(int)
miktex_strcasecmp (/*[in]*/ const char * lpsz1,
		   /*[in]*/ const char * lpsz2)
{
  return (StringCompare(lpsz1, lpsz2, true));
}

/* _________________________________________________________________________

   miktex_xfseek
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(int)
miktex_xfseek (/*[in]*/ FILE *			pfile,
	       /*[in]*/ long			offset,
	       /*[in]*/ int			where,
	       /*[in]*/ const char *		lpszFileName)
{
  if (fseek(pfile, offset, where) != 0)
    {
      FATAL_CRT_ERROR ("fseek", lpszFileName);
    }
  return (0);
}

/* _________________________________________________________________________

   miktex_xfseeko
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(int)
miktex_xfseeko (/*[in]*/ FILE *			pfile,
	        /*[in]*/ off_t			offset,
		/*[in]*/ int			where,
		/*[in]*/ const char *		lpszFileName)
{
  if (fseek(pfile, offset, where) != 0)
    {
      FATAL_CRT_ERROR ("fseek", lpszFileName);
    }
  return (0);
}

/* _________________________________________________________________________

   miktex_xfseeko64
   _________________________________________________________________________ */


MIKTEXKPSCEEAPI(int)
miktex_xfseeko64 (/*[in]*/ FILE *		pfile,
	          /*[in]*/ MIKTEX_INT64		offset,
		  /*[in]*/ int			where,
		  /*[in]*/ const char *		lpszFileName)
{
#if defined(_MSC_VER)
  if (_fseeki64(pfile, offset, where) != 0)
    {
      FATAL_CRT_ERROR ("_fseeki64", lpszFileName);
    }
  return (0);
#elif defined(HAVE_FSEEKO64)
  if (fseeko64(pfile, offset, where) != 0)
    {
      FATAL_CRT_ERROR ("fseeko64", lpszFileName);
    }
  return (0);
#else
#  error Unimplemented: miktex_xfseeko64()
#endif
}

/* _________________________________________________________________________

   miktex_xftell
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(long)
miktex_xftell (/*[in]*/ FILE *		pfile,
	      /*[in]*/ const char *	lpszFileName)
{
  long pos = ftell(pfile);
  if (pos < 0)
    {
      FATAL_CRT_ERROR ("ftell", lpszFileName);
    }
  return (pos);
}

/* _________________________________________________________________________

   miktex_xftello
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(off_t)
miktex_xftello (/*[in]*/ FILE *		pfile,
		/*[in]*/ const char *	lpszFileName)
{
  long pos = ftell(pfile);
  if (pos < 0)
    {
      FATAL_CRT_ERROR ("ftell", lpszFileName);
    }
  return (pos);
}

/* _________________________________________________________________________

   miktex_xftello64
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(MIKTEX_INT64)
miktex_xftello64 (/*[in]*/ FILE *	pfile,
	          /*[in]*/ const char *	lpszFileName)
{
#if defined(_MSC_VER)
  MIKTEX_INT64 pos = _ftelli64(pfile);
  if (pos < 0)
    {
      FATAL_CRT_ERROR ("_ftelli64", lpszFileName);
    }
  return (pos);
#elif defined(HAVE_FTELLO64)
  MIKTEX_INT64 pos = ftello64(pfile);
  if (pos < 0)
    {
      FATAL_CRT_ERROR ("ftello64", lpszFileName);
    }
  return (pos);
  
#else
#  error Unimplemented: miktex_xftello64
#endif
}

/* _________________________________________________________________________

   miktex_xfclose
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
miktex_xfclose (/*[in]*/ FILE *		pfile,
	        /*[in]*/ const char *	lpszFileName)
{
  lpszFileName;
  SessionWrapper(true)->CloseFile (pfile);
}

/* _________________________________________________________________________

   miktex_xfopen
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(FILE *)
miktex_xfopen (/*[in]*/ const char *	lpszFileName,
	       /*[in]*/ const char *	lpszMode)
{
  FileMode mode (FileMode::Open);
  FileAccess access (FileAccess::Read);
  bool isTextFile;
  TranslateModeString (lpszMode, mode, access, isTextFile);
  return (SessionWrapper(true)->OpenFile(lpszFileName,
					 mode,
					 access,
					 isTextFile));
}

/* _________________________________________________________________________

   miktex_kpse_bitmap_tolerance

   [KB] The tolerances change whether we base things on DPI1 or DPI2.
   _________________________________________________________________________ */

#define KPSE_BITMAP_TOLERANCE(r) ((r) / 500.0 + 1)

MIKTEXKPSCEEAPI(int)
miktex_kpathsea_bitmap_tolerance (/*in*/ kpathsea	pKpathseaInstance,
				  /*[in]*/ double	dpi1,
				  /*[in]*/ double	dpi2)
{
  unsigned tolerance = static_cast<unsigned>(KPSE_BITMAP_TOLERANCE(dpi2));
  unsigned lower_bound = (static_cast<int>(dpi2 - tolerance) < 0
			  ? 0
			  : static_cast<unsigned>(dpi2 - tolerance));
  unsigned upper_bound = static_cast<unsigned>(dpi2 + tolerance);
  return (lower_bound <= dpi1 && dpi1 <= upper_bound);
}  

/* _________________________________________________________________________

   miktex_kpathsea_magstep_fix

   [KB]: This is adapted from code written by Tom Rokicki for dvips.
   It's part of Kpathsea now so all the drivers can use it.  The idea
   is to return the true dpi corresponding to DPI with a base
   resolution of BDPI.  If M_RET is non-null, we also set that to the
   mag value.
   
   [KB]: Don't bother trying to use fabs or some other ``standard''
   routine which can only cause trouble; just roll our own
   simple-minded absolute-value function that is all we need.

   _________________________________________________________________________ */

#undef ABS
#define ABS(expr) ((expr) < 0 ? -(expr) : (expr))

#define MAGSTEP_MAX 40

MIKTEXKPSCEEAPI(unsigned)
miktex_kpathsea_magstep_fix (/*in*/ kpathsea	pKpathseaInstance,
			     /*[in]*/ unsigned	dpi,
			     /*[in]*/ unsigned	bdpi,
			     /*[in]*/ int *	m_ret)
{
  int m;
  int mdpi = -1;
  unsigned real_dpi = 0;
  int sign = dpi < bdpi ? -1 : 1;
  
  for (m = 0; !real_dpi && m < MAGSTEP_MAX; ++ m)
    {
      mdpi = magstep (m * sign, bdpi);
      if (ABS(mdpi - (int) dpi) <= 1)
	{
	  real_dpi = mdpi;
	}
      else if ((mdpi - (int) dpi) * sign > 0)
	{
	  real_dpi = dpi;
	}
    }
  
  if (m_ret)
    {
      *m_ret = real_dpi == (unsigned)(mdpi ? (m - 1) * sign : 0);
    }

  return real_dpi ? real_dpi : dpi;
}

/* _________________________________________________________________________

   miktex_kpathsea_init_prog
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
miktex_kpathsea_init_prog (/*in*/ kpathsea		pKpathseaInstance,
			   /*[in]*/ const char *	lpszPrefix,
			   /*[in]*/ unsigned		dpi,
			   /*[in]*/ const char *	lpszMode,
			   /*[in]*/ const char *	lpszFallback)
{
  kpse_baseResolution = dpi;
  if (lpszMode != 0)
  {
    kpse_mode = lpszMode;
  }
}

/* _________________________________________________________________________

   miktex_kpathsea_set_program_name
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
miktex_kpathsea_set_program_name (/*in*/ kpathsea	kpse,
				  /*[in]*/ const char *	lpszArgv0,
				  /*[in]*/ const char *	lpszProgramName)
{
  if (kpse->invocation_name != 0)
  {
    MIKTEX_FREE (kpse->invocation_name);
  }
  kpse->invocation_name = xstrdup(lpszArgv0);
  std::string programName;
  if (lpszProgramName == 0)
  {
    programName = Utils::GetExeName();
    lpszProgramName = programName.c_str();
  }
  if (kpse->program_name != 0)
  {
    MIKTEX_FREE (kpse->program_name);
  }
  kpse->program_name = xstrdup(lpszProgramName);
  if (Utils::GetExeName() != lpszProgramName)
  {
    SessionWrapper(true)->PushAppName (lpszProgramName);
  }
}

/* _________________________________________________________________________

   miktex_kpathsea_set_program_enabled
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
miktex_kpathsea_set_program_enabled (/*in*/ kpathsea	kpse,
				     /*in*/ kpse_file_format_type fmt,
				     /*in*/ boolean value,
				     /*in*/ kpse_src_type level)
{
}

/* _________________________________________________________________________

   miktex_find_suffix
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_find_suffix (const char * lpszPath)
{
  const char * lpszExt = 0;
  for (; *lpszPath != 0; ++ lpszPath)
    {
      if (IsDirectoryDelimiter(*lpszPath))
	{
	  lpszExt = 0;
	}
#if defined(MIKTEX_WINDOWS)
      else if (*lpszPath == ':')
	{
	  lpszExt = 0;
	}
#endif
      else if (*lpszPath == '.')
	{
	  lpszExt = lpszPath + 1;
	}
    }
  return (lpszExt == 0 ? 0 : const_cast<char *>(lpszExt));
}

/* _________________________________________________________________________

   miktex_remove_suffix
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_remove_suffix (const char * lpszPath)
{
  char * lpszRet;
  char * lpszExt = find_suffix(lpszPath);
  if (lpszExt == 0)
  {
    lpszRet = const_cast<char*>(lpszPath);
  }
  else
  {
    MIKTEX_ASSERT (lpszExt > lpszPath);
    -- lpszExt;
    MIKTEX_ASSERT (*lpszExt == '.');
    size_t n = (lpszExt - lpszPath);
    lpszRet = reinterpret_cast<char*>(xmalloc((n + 1) * sizeof(*lpszRet)));
    strncpy (lpszRet, lpszPath, n);
    lpszRet[n] = 0;
  }
  return (lpszRet);
}

/* _________________________________________________________________________

   VarValue
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
VarValue (/*[in]*/ const char *	  lpszVarName,
	  /*[in]*/ std::string &  varValue)
{
  PathName path;
  bool result = false;
  // read-only values
  if (StringCompare(lpszVarName, "SELFAUTOLOC") == 0)
  {
    path = SessionWrapper(true)->GetMyLocation();
    varValue = path.Get();
    result = true;
  }
  else if (StringCompare(lpszVarName, "SELFAUTODIR") == 0)
  {
    result = false;
  }
  else if (StringCompare(lpszVarName, "SELFAUTOPARENT") == 0)
  {
    result = false;
  }
  else if (StringCompare(lpszVarName, "TEXMFSYSVAR") == 0)
  {
    path = SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonDataRoot);
    varValue = path.Get();
    result = true;
  }
  else if (StringCompare(lpszVarName, "TEXMFVAR") == 0)
  {
    path = SessionWrapper(true)->GetSpecialPath(SpecialPath::UserDataRoot);
    varValue = path.Get();
    result = true;
  }
  // configuration files and environment
  else if (SessionWrapper(true)->TryGetConfigValue(0, lpszVarName, varValue))
  {
    result = true;
  }
#if defined (MIKTEX_WINDOWS)
  else if (StringCompare(lpszVarName, "OSFONTDIR") == 0)
  {
    if (GetWindowsDirectoryA(path.GetBuffer(), static_cast<UINT>(path.GetCapacity())) > 0)
    {
      path += "Fonts";
      varValue = path.Get();
      result = true;
    }
  }
#endif
  return (result);
}
/* _________________________________________________________________________

   miktex_kpathsea_var_value
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_kpathsea_var_value (/*in*/ kpathsea	  kpse,
			   /*[in]*/ const char *  lpszVarName)
{
  std::string varValue;
  return (VarValue(lpszVarName, varValue) ? xstrdup(varValue.c_str()) : 0);
}

/* _________________________________________________________________________

   miktex_kpathsea_var_expand
   _________________________________________________________________________ */

class VarExpand : public IExpandCallback
{
public:
  virtual
  bool
  MIKTEXTHISCALL
  GetValue (/*[in]*/ const char *   lpszValueName,
	    /*[out]*/ std::string & varValue)
  {
    return (VarValue(lpszValueName, varValue));
  }
};

MIKTEXKPSCEEAPI(char *)
miktex_kpathsea_var_expand (/*in*/ kpathsea	  kpse,
			    /*[in]*/ const char * lpszSource)
{
  Session * pSession = Session::Get();  
  return (xstrdup(pSession->Expand(lpszSource , &VarExpand()).c_str()));
}

/* _________________________________________________________________________

   miktex_kpathsea_xputenv
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
miktex_kpathsea_xputenv (/*in*/ kpathsea	pKpseInstance,
			 /*[in]*/ const char *	lpszVarName,
			 /*[in]*/ const char *	lpszValue)
{
  Utils::SetEnvironmentString (lpszVarName, lpszValue);
}

/* _________________________________________________________________________

   miktex_kpathsea_in_name_ok
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(int)
miktex_kpathsea_in_name_ok (/*in*/ kpathsea	pKpseInstance,
			    /*[in]*/ const char *lpszFileName)
{
  return (Utils::IsSafeFileName(lpszFileName, true) ? 1 : 0);
}

/* _________________________________________________________________________

   miktex_kpathsea_out_name_ok
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(int)
miktex_kpathsea_out_name_ok (/*in*/ kpathsea	pKpseInstance,
			     /*[in]*/ const char *lpszFileName)
{
  return (Utils::IsSafeFileName(lpszFileName, false) ? 1 : 0);
}

/* _________________________________________________________________________

   miktex_kpathsea_absolute_p
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(boolean)
miktex_kpathsea_absolute_p (/*in*/ kpathsea		pKpseInstance,
			    /*[in]*/ const char *	lpszFileName,
			    /*[in]*/ boolean		relativeOk)
{
  MIKTEX_ASSERT (relativeOk == 0);
  return (Utils::IsAbsolutePath(lpszFileName));
}

/* _________________________________________________________________________

   miktex_str_list_add
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
miktex_str_list_add (/*[in]*/ str_list_type * pStringList,
		     /*[in]*/ char *	      lpsz)
{
  ++ pStringList->length;
  pStringList->list = reinterpret_cast<char**>(
    xrealloc(pStringList->list, sizeof(char*) * pStringList->length));
  pStringList->list[pStringList->length - 1] = lpsz;
}

/* _________________________________________________________________________

   miktex_kpathsea_new
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(kpathsea)
miktex_kpathsea_new ()
{
  kpathsea pKpseInstance = reinterpret_cast<kpathsea>(xmalloc(sizeof(kpathsea_instance)));
  memset (pKpseInstance, 0, sizeof(*pKpseInstance));
  return (pKpseInstance);
}

/* _________________________________________________________________________

   miktex_kpathsea_finish
   _________________________________________________________________________ */

#if 1
#  define DOIT(x)
#else
#  define DOIT(x) (x)
#endif

MIKTEXSTATICFUNC(void)
Free (/*[in]*/ char ** pStringList)
{
  if (pStringList != 0)
  {
    for (char ** p = pStringList; *p != 0; ++ p)
    {
      MIKTEX_FREE (p);
    }
    MIKTEX_FREE (pStringList);
  }
}

MIKTEXSTATICFUNC(void)
Reset (/*[in]*/ kpse_format_info_type * pFormatInfoTable,
       /*[in]*/ size_t			size)
{
  for (int idx = 0; idx < size; ++ idx)
  {
    kpse_format_info_type & formatInfo = pFormatInfoTable[idx];
    MIKTEX_FREE (const_cast<char*>(formatInfo.path));
    MIKTEX_FREE (const_cast<char*>(formatInfo.type));
    Free (const_cast<char **>(formatInfo.suffix));
    Free (const_cast<char **>(formatInfo.alt_suffix));
    memset (&formatInfo, 0, sizeof(FormatInfo));
  }
}

MIKTEXKPSCEEAPI(void)
miktex_kpathsea_finish (/*in*/ kpathsea pKpseInstance)
{
  DOIT (MIKTEX_FREE(const_cast<char*>(pKpseInstance->fallback_resolutions_string)));
  DOIT (MIKTEX_FREE(const_cast<char*>(pKpseInstance->program_name)));
  DOIT (MIKTEX_FREE(const_cast<char*>(pKpseInstance->invocation_name)));
  DOIT (Reset(pKpseInstance->format_info,
	      sizeof(pKpseInstance->format_info) / sizeof(pKpseInstance->format_info[0])));
  if (pKpseInstance != &miktex_kpse_def_inst)
  {
    MIKTEX_FREE (pKpseInstance);
  }
}

/* _________________________________________________________________________

   miktex_kpathsea_brace_expand
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_kpathsea_brace_expand (/*in*/ kpathsea			kpse,
			      /*[in]*/ const char *		lpszPath)
{
  Session * pSession = Session::Get();
  std::string result = pSession->Expand(lpszPath,
    ExpandFlags::Values | ExpandFlags::Braces,
    &VarExpand());
  return (xstrdup(result.c_str()));
}

/* _________________________________________________________________________

   miktex_kpathsea_path_expand
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_kpathsea_path_expand (/*in*/ kpathsea			kpse,
			     /*[in]*/ const char *		lpszPath)
{
  Session * pSession = Session::Get();
  std::string result = pSession->Expand(lpszPath,
    ExpandFlags::Values | ExpandFlags::Braces | ExpandFlags::PathPatterns,
    &VarExpand());
  return (xstrdup(result.c_str()));
}

/* _________________________________________________________________________

   miktex_kpathsea_readable_file
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_kpathsea_readable_file (/*in*/ kpathsea		kpse,
			       /*[in]*/ const char *	lpszName)
{
  if (File::Exists(lpszName))
  {
    return (const_cast<char*>(lpszName));
  }
  else
  {
    return (0);
  }
}

/* _________________________________________________________________________

   miktex_kpathsea_path_search
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_kpathsea_path_search (/*in*/ kpathsea			kpse,
			     /*[in]*/ const char *		lpszPath,
			     /*[in]*/ const char *		lpszName,
			     /*[in]*/ boolean			mustExist)
{
  Session * pSession = Session::Get();
  PathName result;
  if (! pSession->FindFile(lpszName, lpszPath, result))
  {
    return (0);
  }
  result.ToUnix ();
  return (xstrdup(result.Get()));
}

/* _________________________________________________________________________

   miktex_kpathsea_all_path_search
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char **)
miktex_kpathsea_all_path_search (/*in*/ kpathsea		kpse,
				 /*[in]*/ const char *		lpszPath,
				 /*[in]*/ const char *		lpszName)
{
  Session * pSession = Session::Get();
  vector<PathName> result;
  if (! pSession->FindFile(lpszName, lpszPath, result))
  {
    return (0);
  }
  char ** pStringList = XTALLOC(result.size() + 1, char *);
  for (int idx = 0; idx < result.size(); ++ idx)
  {
   result[idx].ToUnix ();
   pStringList[idx] = xstrdup(result[idx].Get());
  }
  pStringList[result.size()] = 0;
  return (pStringList);
}

/* _________________________________________________________________________

   miktex_kpathsea_maketex_option
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
miktex_kpathsea_maketex_option (/*in*/ kpathsea		kpse,
			        /*[in]*/ const char *	lpszFmtName,
			        /*[in]*/ boolean		value)
{
  // <todo/>
}

/* _________________________________________________________________________

   miktex_kpathsea_selfdir
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_kpathsea_selfdir (/*in*/ kpathsea	kpse,
			 /*[in]*/ const char *	lpszArgv0)
{
  return (xstrdup(SessionWrapper(true)->GetMyLocation().Get()));
}

/* _________________________________________________________________________

   miktex_uppercasify
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_uppercasify (const char * lpsz)
{
  char *lpszResult = xstrdup(lpsz);
  for (char * lpsz2 = lpszResult; *lpsz2 != 0; ++ lpsz2)
    {
      *lpsz2 = toupper(*lpsz2);
    }
  return (lpszResult);
}

/* _________________________________________________________________________

   miktex_read_line
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_read_line (/*[in]*/ FILE * pFile)
{
  std::string line;
  Utils::ReadLine (line, pFile, false);
  return (xstrdup(line.c_str()));
}

/* _________________________________________________________________________

   miktex_kpathsea_init_format
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(const char *)
miktex_kpathsea_init_format (/*[in]*/ kpathsea pKpseInstance,
			     /*[in]*/ kpse_file_format_type format)
{
  MIKTEX_ASSERT (pKpseInstance != 0);
  MIKTEX_ASSERT (format < kpse_last_format);
  kpse_format_info_type & formatInfo = pKpseInstance->format_info[format];
  if (formatInfo.path == 0)
  {
    Session * pSession = Session::Get();
    FileType ft = ToFileType(format);
    FileTypeInfo fti = pSession->GetFileTypeInfo(ft);
    std::string searchPath = pSession->Expand(
      fti.searchPath.c_str(),
      ExpandFlags::Values | ExpandFlags::Braces,
      &VarExpand());
    formatInfo.path = ToUnix(xstrdup(searchPath.c_str()));
    formatInfo.type = xstrdup(fti.fileTypeString.c_str());
    formatInfo.suffix = ToStringList(fti.fileNameExtensions);
  }
  return (formatInfo.path);
}
