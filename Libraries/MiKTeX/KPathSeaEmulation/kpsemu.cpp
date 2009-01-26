/* kpsemu.cpp: texk compatibility functions

   Copyright (C) 1994, 95 Karl Berry
   Copyright (C) 2000-2009 Christian Schenk

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
  std::string kpse_mode;
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

   KPSE::FindGlyph
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
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
  char * lpsz = MIKTEX_STRDUP(path.Get());
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

MIKTEXKPSCEEAPI(char *)
miktex_kpse_find_glyph (/*[in]*/ char *				lpszFontName,
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

MIKTEXKPSCEEAPI(char *)
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
    case kpse_program_binary_format:
      found = pSession->FindFile(lpszFileName,
				 FileType::PROGRAMBINFILE,
				 result);
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
      found = pSession->FindFile(lpszFileName, "%R/fonts/misc//", result);
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
    case kpse_opentype_format:
      found = pSession->FindFile(lpszFileName, FileType::OTF, result);
      break;
    case kpse_ovp_format:
      found = pSession->FindFile(lpszFileName, FileType::OVP, result);
      break;
    case kpse_sfd_format:
      found = pSession->FindFile(lpszFileName, FileType::SFD, result);
      break;
    case kpse_cmap_format:
      found = pSession->FindFile(lpszFileName, FileType::CMAP, result);
      break;
    default:
      found = false;
      Session::FatalMiKTeXError
	("KPSE::FindFile",
	 ((std::string(T_("Unsupported file format type: "))
	   + NUMTOSTR(format))
	  .c_str()),
	 0,
	 __FILE__,
	 __LINE__);
      break;
    }
  if (! found)
    {
      return (0);
    }
  result.ToUnix ();
  return (MIKTEX_STRDUP(result.Get()));
}

/* _________________________________________________________________________

   miktex_kpse_find_file
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
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
    case kpse_opentype_format:
      bBin = true;
      break;
    }
  return (bBin);
}

/* _________________________________________________________________________

   KPSE::OpenFile
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(FILE *)
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

   miktex_kpse_open_file
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(FILE *)
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

MIKTEXKPSCEEAPI(void *)
KPSE::Malloc (/*[in]*/ size_t		size,
	      /*[in]*/ const char *	lpszFileName,
	      /*[in]*/ int		line)
{
  return (Debug::Malloc(size, lpszFileName, line));
}

/* _________________________________________________________________________

   miktex_malloc
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void *)
miktex_malloc (/*[in]*/ size_t		size,
	       /*[in]*/ const char *	lpszFileName,
	       /*[in]*/ int		line)
{
  C_FUNC_BEGIN ();
  return (Debug::Malloc(size, lpszFileName, line));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::Realloc
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void *)
KPSE::Realloc (/*[in]*/ void *		ptr,
	       /*[in]*/ size_t		size,
	       /*[in]*/ const char *	lpszFileName,
	       /*[in]*/ int		line)
{
  return (Debug::Realloc(ptr, size, lpszFileName, line));
}

/* _________________________________________________________________________

   miktex_realloc
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void *)
miktex_realloc (/*[in]*/ void *		ptr,
		/*[in]*/ size_t		size,
		/*[in]*/ const char *	lpszFileName,
		/*[in]*/ int		line)
{
  C_FUNC_BEGIN();
  return (Debug::Realloc(ptr, size, lpszFileName, line));
  C_FUNC_END();
}

/* _________________________________________________________________________

   KPSE::StrDup
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
KPSE::StrDup (/*[in]*/ const char *	lpsz,
	      /*[in]*/ const char *	lpszFileName,
	      /*[in]*/ int		line)
{
  return (Debug::StrDup(lpsz, lpszFileName, line));
}

/* _________________________________________________________________________

   miktex_strdup
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_strdup (/*[in]*/ const char *	lpsz,
	       /*[in]*/ const char *	lpszFileName,
	       /*[in]*/ int		line)
{
  C_FUNC_BEGIN ();
  return (Debug::StrDup(lpsz, lpszFileName, line));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::BaseName
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(const char *)
KPSE::BaseName (/*[in]*/ const char * lpszFileName)
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

   miktex_basename
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(const char *)
miktex_basename (/*[in]*/ const char * lpszFileName)
{
  //C_FUNC_BEGIN ();
  return (KPSE::BaseName(lpszFileName));
  //C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_strcasecmp
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(int)
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

MIKTEXKPSCEEAPI(int)
KPSE::FSeek (/*[in]*/ FILE *			pfile,
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

   KPSE::FSeek64
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(int)
KPSE::FSeek64 (/*[in]*/ FILE *			pfile,
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
#  error Unimplemented: KPSE::FSeek64()
#endif
}

/* _________________________________________________________________________

   KPSE::FTell
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(long)
KPSE::FTell (/*[in]*/ FILE *		pfile,
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

   KPSE::FTell64
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(MIKTEX_INT64)
KPSE::FTell64 (/*[in]*/ FILE *		pfile,
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
#  error Unimplemented: KPSE::FTell64
#endif
}

/* _________________________________________________________________________

   KPSE::FClose
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
KPSE::FClose (/*[in]*/ FILE *		pfile,
	      /*[in]*/ const char *	lpszFileName)
{
  lpszFileName;
  SessionWrapper(true)->CloseFile (pfile);
}

/* _________________________________________________________________________

   miktex_fclose
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
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

   KPSE::TryFOpen
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(FILE *)
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

MIKTEXKPSCEEAPI(FILE *)
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

MIKTEXKPSCEEAPI(FILE *)
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

MIKTEXKPSCEEAPI(int)
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
      MIKTEX_FREE (lpszPath);
      throw;
    }
  if (*ppfile != 0)
    {
      Utils::CopyString (lpszFileName, BufferSizes::MaxPath, lpszPath);
    }
  MIKTEX_FREE (lpszPath);
  return (*ppfile == 0 ? 0 : 1);
}

/* _________________________________________________________________________

   miktex_program_invocation_name
   _________________________________________________________________________ */

#if MIKTEX_SERIES_INT <= 207
extern "C"
MIKTEXKPSDATA(char *)
miktex_program_invocation_name = 0;
#else
namespace {
  char * miktex_program_invocation_name = 0;
}
#endif

/* _________________________________________________________________________

   miktex_kpse_bug_address
   _________________________________________________________________________ */

MIKTEXKPSDATA(char *)
miktex_kpse_bug_address = 0;

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
miktex_web2c_version_string = "0.0";

/* _________________________________________________________________________

   KPSE::BitmapTolerance

   [KB] The tolerances change whether we base things on DPI1 or DPI2.
   _________________________________________________________________________ */

#define KPSE_BITMAP_TOLERANCE(r) ((r) / 500.0 + 1)

MIKTEXKPSCEEAPI(int)
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

MIKTEXKPSCEEAPI(int)
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

MIKTEXKPSCEEAPI(unsigned)
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

MIKTEXKPSCEEAPI(unsigned)
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

MIKTEXKPSCEEAPI(void)
KPSE::InitProg (/*[in]*/ const char *		prefix,
		/*[in]*/ unsigned		dpi,
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

MIKTEXKPSCEEAPI(void)
miktex_kpse_init_prog (/*[in]*/ const char *	prefix,
		       /*[in]*/ unsigned	dpi,
		       /*[in]*/ const char *	mode,
		       /*[in]*/ const char *	fallback)
{
  C_FUNC_BEGIN ();
  KPSE::InitProg (prefix, dpi, mode, fallback);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::SetProgramName
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
KPSE::SetProgramName (/*[in]*/ const char *	lpszArgv0,
		      /*[in]*/ const char *	lpszProgramName)
{
  if (miktex_program_invocation_name != 0)
    {
      MIKTEX_FREE (miktex_program_invocation_name);
    }
  miktex_program_invocation_name = MIKTEX_STRDUP(lpszArgv0);
  if (lpszProgramName != 0)
    {
      SessionWrapper(true)->PushAppName (lpszProgramName);
    }
}

/* _________________________________________________________________________

   miktex_kpse_set_program_name
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
miktex_kpse_set_program_name (/*[in]*/ const char *	lpszArgv0,
			      /*[in]*/ const char *	lpszProgramName)
{
  C_FUNC_BEGIN ();
  KPSE::SetProgramName (lpszArgv0, lpszProgramName);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::GetProgramInvocationName
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(const char *)
KPSE::GetProgramInvocationName ()
{
  if (miktex_program_invocation_name == 0)
    {
      miktex_program_invocation_name =
	MIKTEX_STRDUP(Utils::GetExeName().c_str());
    }
  return (miktex_program_invocation_name);
}

/* _________________________________________________________________________

   miktex_get_program_invocation_name
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(const char *)
miktex_get_program_invocation_name ()
{
  C_FUNC_BEGIN ();
  KPSE::GetProgramInvocationName ();
  C_FUNC_END ();
}

/* _________________________________________________________________________

   KPSE::FindSuffix
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
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

   KPSE::VarValue
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
KPSE::VarValue (/*[in]*/ const char * lpszVarName)
{
  PathName path;
  std::string val;
  const char * lpszRet = 0;
  if (StringCompare(lpszVarName, "SELFAUTOLOC") == 0)
    {
      path = SessionWrapper(true)->GetMyLocation();
      lpszRet = path.Get();
    }
  else if (SessionWrapper(true)->TryGetConfigValue(0, lpszVarName, val))
    {
      lpszRet = val.c_str();
    }
  return (lpszRet == 0 ? 0 : MIKTEX_STRDUP(lpszRet));
}

/* _________________________________________________________________________

   miktex_kpse_var_value
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(char *)
miktex_kpse_var_value (/*[in]*/ const char * lpszVarName)
{
  C_FUNC_BEGIN ();
  return (KPSE::VarValue(lpszVarName));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_putenv
   _________________________________________________________________________ */

MIKTEXKPSCEEAPI(void)
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

MIKTEXKPSCEEAPI(void)
Web2C::GetSecondsAndMicros (/*[out]*/ integer * pSeconds,
			    /*[out]*/ integer * pMicros)
{
#if defined(MIKTEX_WINDOWS)
  unsigned long clock = GetTickCount();
  *pSeconds = clock / 1000;
  *pMicros = clock % 1000;
#else
  struct timeval tv;
  gettimeofday (&tv, 0);
  *pSeconds = tv.tv_sec;
  *pMicros = tv.tv_usec;
#endif
}
