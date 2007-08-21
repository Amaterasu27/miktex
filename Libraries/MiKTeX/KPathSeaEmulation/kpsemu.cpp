/* kpsemu.cpp: texk compatibility functions

   Copyright (C) 1994, 95 Karl Berry
   Copyright (C) 2000-2007 Christian Schenk

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
using namespace MiKTeX::KPSE;

namespace {
  unsigned kpse_baseResolution = 600;
  tstring kpse_mode;
}

/* _________________________________________________________________________

   magstep

   <fixme>duplicate</fixme>
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(int)
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

   KPSE::FindGlyph
   _________________________________________________________________________ */

MIKTEXKPSAPI(char *)
KPSE::FindGlyph (/*[in]*/ char *			lpszFontName,
		 /*[in]*/ unsigned			dpi,
		 /*[in]*/ kpse_file_format_type		format,
		 /*[out]*/ kpse_glyph_file_type *	glyph_file)
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
  char * lpsz = KPSE::StrDup(path.Get());
  if (glyph_file != 0)
    {
      glyph_file->name = lpszFontName;
      glyph_file->dpi = dpi;
      glyph_file->format = format;
      glyph_file->source = kpse_glyph_source_normal;
    }
  return (lpsz);
}

/* _________________________________________________________________________

   miktex_kpse_find_glyph
   _________________________________________________________________________ */

MIKTEXKPSAPI(char *)
miktex_kpse_find_glyph (/*[in]*/ char *			lpszFontName,
			/*[in]*/ unsigned			dpi,
			/*[in]*/ kpse_file_format_type		format,
			/*[out]*/ kpse_glyph_file_type *	glyph_file)
{
  C_FUNC_BEGIN ();
  return (KPSE::FindGlyph(lpszFontName, dpi, format, glyph_file));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::FindFile
   _________________________________________________________________________ */

MIKTEXKPSAPI(char *)
KPSE::FindFile (/*[in]*/ const char *		lpszFileName,
		/*[in]*/ kpse_file_format_type	format,
		/*[in]*/ int			mustExist)
{
  MIKTEX_ASSERT (lpszFileName != 0);
  bool found = false;
  PathName result;
  Session * pSession = Session::Get();
  switch (format)
    {
    case kpse_bib_format:
      found = pSession->FindFile(lpszFileName, FileType::BIB, result);
      break;
    case kpse_bst_format:
      found = pSession->FindFile(lpszFileName, FileType::BST, result);
      break;
    case kpse_dvips_config_format:
      found = pSession->FindFile(lpszFileName, FileType::DVIPSCONFIG, result);
      break;
    case kpse_enc_format:
      found = pSession->FindFile(lpszFileName, FileType::ENC, result);
      break;
    case kpse_fontmap_format:
      found = pSession->FindFile(lpszFileName, FileType::MAP, result);
      break;
    case kpse_ist_format:
      found = pSession->FindFile(lpszFileName, FileType::IST, result);
      break;
    case kpse_program_text_format:
      found = pSession->FindFile(lpszFileName,
				 FileType::PROGRAMTEXTFILE,
				 result);
      break;
    case kpse_tex_format:
      found = pSession->FindFile(lpszFileName, FileType::TEX, result);
      break;
    case kpse_ofm_format:
      found = pSession->FindFile(lpszFileName, FileType::OFM, result);
      break;
    case kpse_tfm_format:
      found =
	pSession->FindTfmFile(lpszFileName, result, mustExist ? true : false);
      break;
    case kpse_miscfonts_format:
      found = pSession->FindFile(lpszFileName, T_("%R/fonts/misc//"), result);
      break;
    case kpse_afm_format:
      found = pSession->FindFile(lpszFileName, FileType::AFM, result);
      break;
    case kpse_pict_format:
      found = pSession->FindFile(lpszFileName, FileType::GRAPHICS, result);
      break;
    case kpse_type1_format:
      found = pSession->FindFile(lpszFileName, FileType::TYPE1, result);
      break;
    case kpse_tex_ps_header_format:
      found = pSession->FindFile(lpszFileName, FileType::PSHEADER, result);
      break;
    case kpse_vf_format:
      found = pSession->FindFile(lpszFileName, FileType::VF, result);
      break;
    case kpse_ovf_format:
      found = pSession->FindFile(lpszFileName, FileType::OVF, result);
      break;
    case kpse_otp_format:
      found = pSession->FindFile(lpszFileName, FileType::OTP, result);
      break;
    case kpse_ocp_format:
      found = pSession->FindFile(lpszFileName, FileType::OCP, result);
      break;
    case kpse_truetype_format:
      found = pSession->FindFile(lpszFileName, FileType::TTF, result);
      break;
    case kpse_ovp_format:
      found = pSession->FindFile(lpszFileName, FileType::OVP, result);
      break;
    default:
      found = false;
      Session::FatalMiKTeXError
	(T_("KPSE::FindFile"),
	 ((tstring(T_("Unsupported file format type: "))
	   + NUMTOSTR(format))
	  .c_str()),
	 0,
	 T_(__FILE__),
	 __LINE__);
      break;
    }
  if (! found)
    {
      return (0);
    }
  result.ToUnix ();
  return (KPSE::StrDup(result.Get()));
}

/* _________________________________________________________________________

   miktex_kpse_find_file
   _________________________________________________________________________ */

MIKTEXKPSAPI(char *)
miktex_kpse_find_file (/*[in]*/ const char *		lpszFileName,
		       /*[in]*/ kpse_file_format_type	format,
		       /*[in]*/ int			mustExist)
{
  C_FUNC_BEGIN ();
  return (KPSE::FindFile(lpszFileName, format, mustExist));
  C_FUNC_END ();
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
      bBin = true;
      break;
    }
  return (bBin);
}

/* _________________________________________________________________________

   KPSE::OpenFile
   _________________________________________________________________________ */

MIKTEXKPSAPI(FILE *)
KPSE::OpenFile (/*[in]*/ const char *		lpszFileName,
		/*[in]*/ kpse_file_format_type	format)
{
  MIKTEX_ASSERT (lpszFileName != 0);
  char * lpszPath = KPSE::FindFile(lpszFileName, format, 1);
  if (lpszPath == 0)
    {
      return (0);
    }
  FILE * pfile;
  try
    {
      pfile = TryFOpen(lpszPath, IsBinary(format) ? T_("rb") : T_("r"));
    }
  catch (const exception &)
    {
      free (lpszPath);
      throw;
    }
  free (lpszPath);
  return (pfile);
}

/* _________________________________________________________________________

   miktex_kpse_open_file
   _________________________________________________________________________ */

MIKTEXKPSAPI(FILE *)
miktex_kpse_open_file (/*[in]*/ const char *		lpszFileName,
		       /*[in]*/ kpse_file_format_type	format)
{
  C_FUNC_BEGIN ();
  return (KPSE::OpenFile(lpszFileName, format));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::Malloc
   _________________________________________________________________________ */

MIKTEXKPSAPI(void *)
KPSE::Malloc (/*[in]*/ size_t size)
{
  void * ptr = malloc(size);
  if (ptr == 0)
    {
      OUT_OF_MEMORY (T_("Malloc"));
    }
  return (ptr);
}

/* _________________________________________________________________________

   miktex_malloc
   _________________________________________________________________________ */

MIKTEXKPSAPI(void *)
miktex_malloc (/*[in]*/ size_t size)
{
  C_FUNC_BEGIN ();
  return (KPSE::Malloc(size));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::Realloc
   _________________________________________________________________________ */

MIKTEXKPSAPI(void *)
KPSE::Realloc (/*[in]*/ void *	ptr,
	       /*[in]*/ size_t	size)
{
  ptr = realloc(ptr, size);
  if (ptr == 0)
    {
      OUT_OF_MEMORY (T_("xrealloc"));
    }
  return (ptr);
}

/* _________________________________________________________________________

   miktex_realloc
   _________________________________________________________________________ */

MIKTEXKPSAPI(void *)
miktex_realloc (/*[in]*/ void *	ptr,
		/*[in]*/ size_t	size)
{
  C_FUNC_BEGIN();
  return (KPSE::Realloc(ptr, size));
  C_FUNC_END();
}

/* _________________________________________________________________________

   KPSE::StrDup
   _________________________________________________________________________ */

MIKTEXKPSAPI(char *)
KPSE::StrDup (/*[in]*/ const char * lpsz)
{
  size_t len = StrLen(lpsz);
  char * lpsz2 = reinterpret_cast<char *>(Malloc(len + 1));
  Utils::CopyString (lpsz2, len + 1, lpsz);
  return (lpsz2);
}

/* _________________________________________________________________________

   miktex_strdup
   _________________________________________________________________________ */

MIKTEXKPSAPI(char *)
miktex_strdup (/*[in]*/ const char * lpsz)
{
  C_FUNC_BEGIN ();
  return (KPSE::StrDup(lpsz));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::BaseName
   _________________________________________________________________________ */

MIKTEXKPSAPI(const char *)
KPSE::BaseName (/*[in]*/ const char * lpszFileName)
{
  const char * lpsz = lpszFileName + StrLen(lpszFileName);
  while (lpsz != lpszFileName)
    {
      -- lpsz;
      if (IsDirectoryDelimiter(*lpsz) || *lpsz == T_(':'))
	{
	  return (lpsz + 1);
	}
    }
  return (lpszFileName);
}

/* _________________________________________________________________________

   miktex_basename
   _________________________________________________________________________ */

MIKTEXKPSAPI(const char *)
miktex_basename (/*[in]*/ const char * lpszFileName)
{
  //C_FUNC_BEGIN ();
  return (KPSE::BaseName(lpszFileName));
  //C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_strcasecmp
   _________________________________________________________________________ */

MIKTEXKPSAPI(int)
miktex_strcasecmp (/*[in]*/ const char * lpsz1,
		   /*[in]*/ const char * lpsz2)
{
  //C_FUNC_BEGIN ();
  return (StringCompare(lpsz1, lpsz2, true));
  //C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::FSeek
   _________________________________________________________________________ */

MIKTEXKPSAPI(int)
KPSE::FSeek (/*[in]*/ FILE *			pfile,
	     /*[in]*/ long			offset,
	     /*[in]*/ int			where,
	     /*[in]*/ const char *		lpszFileName)
{
  if (fseek(pfile, offset, where) != 0)
    {
      FATAL_CRT_ERROR (T_("fseek"), lpszFileName);
    }
  return (0);
}

/* _________________________________________________________________________

   KPSE::FTell
   _________________________________________________________________________ */

MIKTEXKPSAPI(long)
KPSE::FTell (/*[in]*/ FILE *		pfile,
	     /*[in]*/ const char *	lpszFileName)
{
  long pos = ftell(pfile);
  if (pos < 0)
    {
      FATAL_CRT_ERROR (T_("ftell"), lpszFileName);
    }
  return (pos);
}

/* _________________________________________________________________________

   KPSE::FClose
   _________________________________________________________________________ */

MIKTEXKPSAPI(void)
KPSE::FClose (/*[in]*/ FILE *		pfile,
	      /*[in]*/ const char *	lpszFileName)
{
  lpszFileName;
  SessionWrapper(true)->CloseFile (pfile);
}

/* _________________________________________________________________________

   miktex_fclose
   _________________________________________________________________________ */

MIKTEXKPSAPI(void)
miktex_fclose (/*[in]*/ FILE *		pfile,
	       /*[in]*/ const char *	lpszFileName)
{
  C_FUNC_BEGIN ();
  KPSE::FClose (pfile, lpszFileName);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
TranslateModeString (/*[in]*/ const char *	lpszMode,
		     /*[out]*/ FileMode &	mode,
		     /*[out]*/ FileAccess &	access,
		     /*[out]*/ bool &		isTextFile)
{
  if (StringCompare(lpszMode, T_("r")) == 0)
    {
      mode = FileMode::Open;
      access = FileAccess::Read;
      isTextFile = true;
    }
  else if (StringCompare(lpszMode, T_("w")) == 0)
    {
      mode = FileMode::Create;
      access = FileAccess::Write;
      isTextFile = true;
    }
  else if (StringCompare(lpszMode, T_("rb")) == 0)
    {
      mode = FileMode::Open;
      access = FileAccess::Read;
      isTextFile = false;
    }
  else if (StringCompare(lpszMode, T_("wb")) == 0)
    {
      mode = FileMode::Create;
      access = FileAccess::Write;
      isTextFile = false;
    }
  else if (StringCompare(lpszMode, T_("ab")) == 0)
    {
      mode = FileMode::Append;
      access = FileAccess::Write;
      isTextFile = false;
    }
  else
    {
      Session::FatalMiKTeXError (T_("TranslateModeString"),
				 T_("Unsupported mode."),
				 lpszMode,
				 T_(__FILE__),
				 __LINE__);

    }
}

/* _________________________________________________________________________

   KPSE::TryFOpen
   _________________________________________________________________________ */

MIKTEXKPSAPI(FILE *)
KPSE::TryFOpen (/*[in]*/ const char *	lpszFileName,
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

   KPSE::FOpen
   _________________________________________________________________________ */

MIKTEXKPSAPI(FILE *)
KPSE::FOpen (/*[in]*/ const char *	lpszFileName,
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

   miktex_fopen
   _________________________________________________________________________ */

MIKTEXKPSAPI(FILE *)
miktex_fopen (/*[in]*/ const char *	lpszFileName,
	      /*[in]*/ const char *	lpszMode)
{
  C_FUNC_BEGIN ();
  return (KPSE::FOpen(lpszFileName, lpszMode));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   Web2C::OpenInput
   _________________________________________________________________________ */

MIKTEXKPSAPI(int)
Web2C::OpenInput (/*[in,out]*/ char *			lpszFileName,
		  /*[in]*/ FILE **			ppfile,
		  /*[in]*/ kpse_file_format_type	format,
		  /*[in]*/ const char *			lpszMode)
{
  char * lpszPath = KPSE::FindFile(lpszFileName, format, 0);
  if (lpszPath == 0)
    {
      return (0);
    }
  try
    {
      *ppfile = KPSE::TryFOpen(lpszPath, lpszMode);
    }
  catch (const exception &)
    {
      free (lpszPath);
      throw;
    }
  if (*ppfile != 0)
    {
      Utils::CopyString (lpszFileName, BufferSizes::MaxPath, lpszPath);
    }
  free (lpszPath);
  return (*ppfile == 0 ? 0 : 1);
}

/* _________________________________________________________________________

   miktex_program_invocation_name
   _________________________________________________________________________ */

MIKTEXKPSDATA(char *)
miktex_program_invocation_name = T_("");

/* _________________________________________________________________________

   miktex_kpse_bug_address
   _________________________________________________________________________ */

MIKTEXKPSDATA(char *)
miktex_kpse_bug_address = T_("");

/* _________________________________________________________________________

   miktex_kpse_make_tex_discard_errors
   _________________________________________________________________________ */

MIKTEXKPSDATA(int)
miktex_kpse_make_tex_discard_errors = 0;

/* _________________________________________________________________________

   miktex_kpathsea_debug
   _________________________________________________________________________ */

MIKTEXKPSDATA(unsigned)
miktex_kpathsea_debug = 0;

/* _________________________________________________________________________

   miktex_kpse_fallback_resolutions_string
   _________________________________________________________________________ */

MIKTEXKPSDATA(const char *)
miktex_kpse_fallback_resolutions_string = 0;

/* _________________________________________________________________________

   miktex_kpathsea_version_string
   _________________________________________________________________________ */

MIKTEXKPSDATA(char *)
miktex_kpathsea_version_string = KPSEVERSION;

/* _________________________________________________________________________

   miktex_web2c_version_string
   _________________________________________________________________________ */

MIKTEXKPSDATA(char *)
miktex_web2c_version_string = T_("0.0");

/* _________________________________________________________________________

   KPSE::BitmapTolerance

   [KB] The tolerances change whether we base things on DPI1 or DPI2.
   _________________________________________________________________________ */

#define KPSE_BITMAP_TOLERANCE(r) ((r) / 500.0 + 1)

MIKTEXKPSAPI(int)
KPSE::BitmapTolerance (/*[in]*/ double	dpi1,
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

   miktex_kpse_bitmap_tolerance
   _________________________________________________________________________ */

MIKTEXKPSAPI(int)
miktex_kpse_bitmap_tolerance (/*[in]*/ double	dpi1,
			      /*[in]*/ double	dpi2)
{
  //C_FUNC_BEGIN ();
  return (KPSE::BitmapTolerance(dpi1, dpi2));
  //C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::MagStepFix

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

MIKTEXKPSAPI(unsigned)
KPSE::MagStepFix (/*[in]*/ unsigned	dpi,
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

   miktex_kpse_magstep_fix
   _________________________________________________________________________ */

MIKTEXKPSAPI(unsigned)
miktex_kpse_magstep_fix (/*[in]*/ unsigned	dpi,
			 /*[in]*/ unsigned	bdpi,
			 /*[in]*/ int *		m_ret)
{
  //C_FUNC_BEGIN ();
  return (KPSE::MagStepFix(dpi, bdpi, m_ret));
  //C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::InitProg
   _________________________________________________________________________ */

MIKTEXKPSAPI(void)
KPSE::InitProg (/*[in]*/ const char *		prefix,
		/*[in]*/ unsigned			dpi,
		/*[in]*/ const char *		mode,
		/*[in]*/ const char *		fallback)
{
  prefix;
  fallback;
  kpse_baseResolution = dpi;
  if (mode != 0)
    {
      kpse_mode = mode;
    }
}

/* _________________________________________________________________________

   miktex_kpse_init_prog
   _________________________________________________________________________ */

MIKTEXKPSAPI(void)
miktex_kpse_init_prog (/*[in]*/ const char *	prefix,
		       /*[in]*/ unsigned		dpi,
		       /*[in]*/ const char *	mode,
		       /*[in]*/ const char *	fallback)
{
  C_FUNC_BEGIN ();
  KPSE::InitProg (prefix, dpi, mode, fallback);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::FindSuffix
   _________________________________________________________________________ */

MIKTEXKPSAPI(char *)
KPSE::FindSuffix (const char * lpszPath)
{
  const char * lpszExt = 0;
  for (; *lpszPath != 0; ++ lpszPath)
    {
      if (IsDirectoryDelimiter(*lpszPath))
	{
	  lpszExt = 0;
	}
#if defined(MIKTEX_WINDOWS)
      else if (*lpszPath == T_(':'))
	{
	  lpszExt = 0;
	}
#endif
      else if (*lpszPath == T_('.'))
	{
	  lpszExt = lpszPath + 1;
	}
    }
  return (lpszExt == 0 ? 0 : const_cast<char *>(lpszExt));
}

/* _________________________________________________________________________

   KPSE::VarValue
   _________________________________________________________________________ */

MIKTEXKPSAPI(char *)
KPSE::VarValue (/*[in]*/ const char * lpszVarName)
{
  PathName path;
  tstring val;
  const char * lpszRet = 0;
  if (StringCompare(lpszVarName, T_("SELFAUTOLOC")) == 0)
    {
      if (GetModuleFileName(0,
			    path.GetBuffer(),
			    static_cast<DWORD>(path.GetSize()))
	  != 0)
	{
	  lpszRet = path.Get();
	}
    }
  else if (SessionWrapper(true)->TryGetConfigValue(0, lpszVarName, val))
    {
      lpszRet = val.c_str();
    }
  return (lpszRet == 0 ? 0 : StrDup(lpszRet));
}

/* _________________________________________________________________________

   miktex_kpse_var_value
   _________________________________________________________________________ */

MIKTEXKPSAPI(char *)
miktex_kpse_var_value (/*[in]*/ const char * lpszVarName)
{
  C_FUNC_BEGIN ();
  return (KPSE::VarValue(lpszVarName));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_putenv
   _________________________________________________________________________ */

MIKTEXKPSAPI(void)
miktex_putenv (/*[in]*/ const char * lpszVarName,
	       /*[in]*/ const char * lpszValue)
{
  C_FUNC_BEGIN ();
  Utils::SetEnvironmentString (lpszVarName, lpszValue);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   Web2C::GetSecondsAndMicros
   _________________________________________________________________________ */

MIKTEXKPSAPI(void)
Web2C::GetSecondsAndMicros (/*[out]*/ integer * pSeconds,
			    /*[out]*/ integer * pMicros)
{
  unsigned long clock = GetTickCount();
  *pSeconds = clock / 1000;
  *pMicros = clock % 1000;
}

