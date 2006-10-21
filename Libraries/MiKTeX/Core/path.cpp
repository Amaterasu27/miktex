/* path.cpp: path name utilities

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

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   Utils::GetRelativizedPath

   Return the part of PATH that is relative to ROOT.

   lpszPath			lpszRoot		return
   --------                     --------                ------
   C:\ab\cd\ef.gh		C:\ab\			cd\ef.gh
   C:\ab\cd\ef.gh		C:\ab			cd\ef.gh
   C:\abcd\ef.gh		C:\ab\			<NULL>
   C:\abcd\ef.gh		C:\ab			<NULL>
   _________________________________________________________________________ */

const MIKTEXCHAR *
Utils::GetRelativizedPath (/*[in]*/ const MIKTEXCHAR * lpszPath,
			   /*[in]*/ const MIKTEXCHAR * lpszRoot)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_STRING (lpszRoot);

  MIKTEX_ASSERT (Utils::IsAbsolutePath(lpszPath));
  MIKTEX_ASSERT (Utils::IsAbsolutePath(lpszRoot));

  PathName pathNorm (lpszPath);
  pathNorm.Normalize ();

  PathName pathRootNorm (lpszRoot);
  pathRootNorm.Normalize ();

  size_t rootLen = StrLen(lpszRoot);

  MIKTEX_ASSERT (rootLen > 0);

  if (StrNCmpI(pathNorm.Get(), pathRootNorm.Get(), rootLen) != 0)
    {
      return (0);
    }

  MIKTEXCHARINT ch = lpszRoot[rootLen - 1];

  if (IsDirectoryDelimiter(ch))
    {
      return (lpszPath + rootLen);
    }

  if (! IsDirectoryDelimiter(lpszPath[rootLen]))
    {
      return (0);
    }

  return (lpszPath + rootLen + 1);
}

/* _________________________________________________________________________

   Utils::IsAbsolutePath
   _________________________________________________________________________ */

bool
Utils::IsAbsolutePath (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  // "\xyz\foo.txt", "\\server\xyz\foo.txt"
  if (IsDirectoryDelimiter(lpszPath[0]))
    {
      return (true);
    }
#if defined(MIKTEX_WINDOWS)
  else if (IsAlpha(static_cast<MIKTEXUCHAR>(lpszPath[0])) // "C:\xyz\foo.txt"
	   && lpszPath[1] == T_(':')
	   && IsDirectoryDelimiter(lpszPath[2]))
    {
      return (true);
    }
#endif
  else
    {
      return (false);
    }
}

/* _________________________________________________________________________

   CompareFileNameChars
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(MIKTEXCHARINT)
CompareFileNameChars (/*[in]*/ MIKTEXCHAR	ch1,
		      /*[in]*/ MIKTEXCHAR	ch2)
{
  if (IsDirectoryDelimiter(ch1) && IsDirectoryDelimiter(ch2))
    {
      return (0);
    }
#if defined(MIKTEX_WINDOWS)
  MIKTEXCHARINT norm1 = ToLower(ch1);
  MIKTEXCHARINT norm2 = ToLower(ch2);
#else
  MIKTEXCHARINT norm1 = ch1;
  MIKTEXCHARINT norm2 = ch2;
#endif
  return (norm1 - norm2);
}

/* _________________________________________________________________________

   PathName::Compare
   _________________________________________________________________________ */

int
PathName::Compare (/*[in]*/ const MIKTEXCHAR *	lpszPath1,
		   /*[in]*/ const MIKTEXCHAR *	lpszPath2)
{
  MIKTEX_ASSERT_STRING (lpszPath1);
  MIKTEX_ASSERT_STRING (lpszPath2);

  int ret;
  MIKTEXCHARINT cmp;
  
  while ((cmp = CompareFileNameChars(*lpszPath1, *lpszPath2)) == 0
	 && *lpszPath2 != 0)
    {
      ++ lpszPath1;
      ++ lpszPath2;
    }

  if (cmp < 0)
    {
      ret = -1;
    }
  else if (cmp > 0)
    {
      ret = 1;
    }
  else
    {
      ret = 0;
    }

  return (ret);
}

/* _________________________________________________________________________

   NormalizePath
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
NormalizePath (/*[in,out]*/ MIKTEXCHAR *	lpszPath)
{
#if defined(MIKTEX_WINDOWS)
  for (size_t l = 0; lpszPath[l] != 0; ++ l)
    {
      if (lpszPath[l] == PathName::DirectoryDelimiter)
	{
	  lpszPath[l] = PathName::AltDirectoryDelimiter;
	}
      else
	{
	  lpszPath[l] = ToLower(lpszPath[l]);
	}
    }
#endif
}

/* _________________________________________________________________________

   PathName::Compare
   _________________________________________________________________________ */

int
PathName::Compare (/*[in]*/ const MIKTEXCHAR *	lpszPath1,
		   /*[in]*/ const MIKTEXCHAR *	lpszPath2,
		   /*[in]*/ size_t		count)
 
{
  MIKTEX_ASSERT_STRING (lpszPath1);
  MIKTEX_ASSERT_STRING (lpszPath2);

  if (count == 0)
    {
      return (0);
    }

  MIKTEXCHAR szPath1[BufferSizes::MaxPath];
  MIKTEXCHAR szPath2[BufferSizes::MaxPath];

  Utils::CopyString (szPath1, BufferSizes::MaxPath, lpszPath1);
  Utils::CopyString (szPath2, BufferSizes::MaxPath, lpszPath2);

  NormalizePath (szPath1);
  NormalizePath (szPath2);

  lpszPath1 = szPath1;
  lpszPath2 = szPath2;

  for (size_t i = 0; i < count; ++ i, ++ lpszPath1, ++ lpszPath2)
    {
      if (*lpszPath1 == 0 || *lpszPath1 != *lpszPath2)
	{
	  return (*reinterpret_cast<const MIKTEXUCHAR *>(lpszPath1)
		  - *reinterpret_cast<const MIKTEXUCHAR *>(lpszPath2));
	}
    }
  
  return (0);
}

/* _________________________________________________________________________

   PathName::Convert
   _________________________________________________________________________ */

PathName &
MIKTEXCALL
PathName::Convert (/*[in]*/ ConvertPathNameFlags	flags)
{
  bool toUnix = ((flags & ConvertPathNameFlags::ToUnix) != 0 ? true : false);
  bool toDos = ((flags & ConvertPathNameFlags::ToDos) != 0 ? true : false);
  MIKTEX_ASSERT (! (toUnix && toDos));

  bool toUpper =
    ((flags & ConvertPathNameFlags::MakeUpper) != 0 ? true : false);
  bool toLower =
    ((flags & ConvertPathNameFlags::MakeLower) != 0 ? true : false);
  MIKTEX_ASSERT (! (toUpper && toLower));

  bool relativize =
    ((flags & ConvertPathNameFlags::MakeRelative) != 0 ? true : false);
  UNUSED (relativize);
  bool makeFQ =
    ((flags & ConvertPathNameFlags::MakeAbsolute) != 0 ? true : false);
  MIKTEX_ASSERT (! (relativize && makeFQ));

  if (makeFQ)
    {
      PathName temp = GetFullPath(Get());
      *this = temp;
    }

#if defined(MIKTEX_WINDOWS)
  if ((flags & ConvertPathNameFlags::RemoveBlanks) != 0)
    {
      Utils::RemoveBlanksFromPathName (*this);
    }
#endif

  for (MIKTEXCHAR * lpsz = GetBuffer(); *lpsz != 0; ++ lpsz)
    {
      if (toUnix && *lpsz == PathName::DosDirectoryDelimiter)
	{
	  *lpsz = PathName::UnixDirectoryDelimiter;
	}
      else if (toDos && *lpsz == PathName::UnixDirectoryDelimiter)
	{
	  *lpsz = PathName::DosDirectoryDelimiter;
	}
      if (toUpper)
	{
	  *lpsz = ToUpper(*lpsz);
	}
      else if (toLower)
	{
	  *lpsz = ToLower(*lpsz);
	}
    }

  return (*this);
}

/* _________________________________________________________________________

   Utils::MakeTeXPathName
   _________________________________________________________________________ */

void
Utils::MakeTeXPathName (/*[in,out]*/ PathName & path)
{
#if defined(MIKTEX_WINDOWS)
  path.Convert (ConvertPathNameFlags(ConvertPathNameFlags::RemoveBlanks)
		| ConvertPathNameFlags::ToUnix);
#else
#  warning Unimplemented: Utils::MakeTeXPathName()
  if (StrChr(path.Get(), T_(' ')) != 0)
    {
      FATAL_MIKTEX_ERROR (T_("Utils::MakeTeXPathName"),
			  T_("Path name contains the space character."),
			  path.Get());
    }
#endif
}

/* _________________________________________________________________________

   PathName::Match
   _________________________________________________________________________ */

bool
PathName::Match (/*[in]*/ const MIKTEXCHAR *	lpszPattern,
		 /*[in]*/ const MIKTEXCHAR *	lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_STRING (lpszPattern);

  switch (*lpszPattern)
    {
    case 0:
      return (*lpszPath == 0);
      
    case T_('*'):
      return (Match(lpszPattern + 1, lpszPath) // <recursivecall/>
	      || (*lpszPath != 0
		  && Match(lpszPattern, lpszPath + 1)));
      
    case T_('?'):
      return (*lpszPath != 0	// <recursivecall/>
	      && Match(lpszPattern + 1, lpszPath + 1));
      
    default:			// <recursivecall/>
      return (CompareFileNameChars(*lpszPattern, *lpszPath) == 0
	      && Match(lpszPattern + 1, lpszPath + 1));
    }
}

/* _________________________________________________________________________

   Utils::IsParentDirectoryOf
   _________________________________________________________________________ */

bool
Utils::IsParentDirectoryOf (/*[in]*/ const MIKTEXCHAR * lpszParentDir,
			    /*[in]*/ const MIKTEXCHAR * lpszFileName)
{
  size_t len1 = StrLen(lpszParentDir);
  if (PathName::Compare(lpszParentDir, lpszFileName, len1) != 0)
    {
      return (false);
    }
  size_t len2 = StrLen(lpszFileName);
  if (len1 >= len2)
    {
      return (false);
    }
#if defined(MIKTEX_WINDOWS)
  if (len1 == 3)		// C:/
    {
      return (true);
    }
#endif
  return (IsDirectoryDelimiter(lpszFileName[len1]));
}

/* _________________________________________________________________________

   Utils::GetUncRootFromPath
   _________________________________________________________________________ */

bool
Utils::GetUncRootFromPath (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			   /*[out]*/ PathName &		uncRoot)
{
  // must start with "\\"
  if (! (IsDirectoryDelimiter(lpszPath[0])
	 && IsDirectoryDelimiter(lpszPath[1])))
    {
      return (false);
    }
  
  uncRoot = lpszPath;

  MIKTEXCHAR * lpsz = uncRoot.GetBuffer() + 2;

  if (lpsz[0] == 0 || lpsz[1] == 0)
    {
      return (false);
    }

  // skip server name
  while (*++lpsz != 0)
    {
      if (IsDirectoryDelimiter(*lpsz))
	{
	  break;
	}
    }

  if (lpsz[0] == 0 || lpsz[1] == 0)
    {
      return (false);
    }

  // skip share name
  while (*++lpsz != 0)
    {
      if (IsDirectoryDelimiter(*lpsz))
	{
	  break;
	}
    }

  if (! (*lpsz == 0 || IsDirectoryDelimiter(*lpsz)))
    {
      return (false);
    }
  
  *lpsz++ = PathName::DirectoryDelimiter;
  *lpsz = 0;

#if defined(MIKTEX_WINDOWS)
  uncRoot.ToDos ();
#endif

  return (true);
}

/* _________________________________________________________________________

   AppendDirectoryDelimiter
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
AppendDirectoryDelimiter (/*[in,out]*/ tstring & path)
{
  size_t l = path.length();
  if (l > 0 && ! IsDirectoryDelimiter(path[l - 1]))
    {
      path += PathName::DirectoryDelimiter;
    }
}

/* _________________________________________________________________________

   RemoveDirectoryDelimiter
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
RemoveDirectoryDelimiter (/*[in,out]*/ MIKTEXCHAR * lpszPath)
{
  size_t l = StrLen(lpszPath);
  if (l > 1 && IsDirectoryDelimiter(lpszPath[l - 1]))
    {
#if defined(MIKTEX_WINDOWS)
      if (lpszPath[l - 2] == PathName::VolumeDelimiter)
	{
	  return;
	}
#endif
      lpszPath[l - 1] = 0;
    }
}

/* _________________________________________________________________________

   PathName::Combine
   _________________________________________________________________________ */

void
PathName::Combine (/*[out]*/ MIKTEXCHAR *	lpszPath,
		   /*[in]*/ size_t		sizePath,
		   /*[in]*/ const MIKTEXCHAR *	lpszAbsPath,
		   /*[in]*/ const MIKTEXCHAR *	lpszRelPath,
		   /*[in]*/ const MIKTEXCHAR *	lpszExtension)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszAbsPath);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszRelPath);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszExtension);

  size_t n = 0;

  if (lpszAbsPath != 0 && *lpszAbsPath != 0)
    {
      n = Utils::CopyString(lpszPath, sizePath, lpszAbsPath);
    }

  if (((lpszRelPath != 0 && *lpszRelPath != 0)
       || (lpszExtension != 0 && *lpszExtension != 0))
      && (n > 0 && ! IsDirectoryDelimiter(lpszPath[n - 1])))
    {
      if (n + 1 >= sizePath)
	{
	  BUF_TOO_SMALL (T_("PathName::Combine"));
	}
      lpszPath[n] = DirectoryDelimiter;
      ++ n;
      lpszPath[n] = 0;
    }

  if (lpszRelPath != 0 && *lpszRelPath != 0)
    {
      n += Utils::CopyString(&lpszPath[n], sizePath - n, lpszRelPath);
    }

  if (lpszExtension != 0 && *lpszExtension != 0)
    {
      if (*lpszExtension != T_('.'))
	{
	  if (n + 1 >= sizePath)
	    {
	      BUF_TOO_SMALL (T_("PathName::Combine"));
	    }
	  lpszPath[n] = T_('.');
	  ++ n;
	}
      n += Utils::CopyString(&lpszPath[n], sizePath - n, lpszExtension);
    }

  MIKTEX_ASSERT (StrLen(lpszPath) == n);
}

/* _________________________________________________________________________

   PathName::Split
   _________________________________________________________________________ */

void
PathName::Split (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		 /*[out]*/ MIKTEXCHAR *		lpszDir,
		 /*[in]*/ size_t		sizeDir,
		 /*[out]*/ MIKTEXCHAR *		lpszName,
		 /*[in]*/ size_t		sizeName,
		 /*[out]*/ MIKTEXCHAR *		lpszExtension,
		 /*[in]*/ size_t		sizeExtension)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszDir, sizeDir);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszName, sizeName);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszExtension, sizeExtension);

  const MIKTEXCHAR * lpsz;

  const MIKTEXCHAR * lpszName_ = 0;

  // find the beginning of the name
  for (lpsz = lpszPath; *lpsz != 0; ++ lpsz)
    {
      if (IsDirectoryDelimiter(*lpsz))
	{
	  lpszName_ = lpsz + 1;
	}
    }
  if (lpszName_ == 0)
    {
      lpszName_ = lpszPath;
    }

  if (lpszDir != 0)
    {
      CopyString2 (lpszDir, sizeDir, lpszPath, lpszName_ - lpszPath);
    }

  // find the extension
  const MIKTEXCHAR * lpszExtension_ = 0;
  for (lpsz = lpszName_; *lpsz != 0; ++ lpsz)
    {
      if (*lpsz == T_('.'))
	{
	  lpszExtension_ = lpsz;
	}
    }
  if (lpszExtension_ == 0)
    {
      lpszExtension_ = lpsz;
    }

  if (lpszName != 0)
    {
      CopyString2 (lpszName, sizeName, lpszName_, lpszExtension_ - lpszName_);
    }

  if (lpszExtension != 0)
    {
      Utils::CopyString (lpszExtension, sizeExtension, lpszExtension_);
    }
}

/* _________________________________________________________________________

   GetFileNameExtension
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(const MIKTEXCHAR *)
GetFileNameExtension (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  const MIKTEXCHAR * lpszExtension = 0;
  for (const MIKTEXCHAR * lpsz = lpszPath; *lpsz != 0; ++ lpsz)
    {
      if (IsDirectoryDelimiter(*lpsz))
	{
	  lpszExtension = 0;
	}
      else if (*lpsz == T_('.'))
	{
	  lpszExtension = lpsz;
	}
    }
  return (lpszExtension);
}

/* _________________________________________________________________________

   PathName::GetExtension
   _________________________________________________________________________ */

const MIKTEXCHAR *
PathName::GetExtension ()
  const
{
  return (GetFileNameExtension(buffer));
}

/* _________________________________________________________________________

   PathName::SetExtension
   _________________________________________________________________________ */

PathName &
MIKTEXCALL
PathName::SetExtension (/*[in]*/ const MIKTEXCHAR *	lpszExtension,
			/*[in]*/ bool			override)
{
  MIKTEXCHAR szDir[BufferSizes::MaxPath];
  MIKTEXCHAR szFileName[BufferSizes::MaxPath];
  MIKTEXCHAR szExtOld[BufferSizes::MaxPath];

  PathName::Split (Get(),
		   szDir, BufferSizes::MaxPath,
		   szFileName, BufferSizes::MaxPath,
		   szExtOld, BufferSizes::MaxPath);
  
  if (szExtOld[0] == 0 || override)
    {
      Set (szDir, szFileName, lpszExtension);
    }

  return (*this);
}

/* _________________________________________________________________________

   PathName::AppendDirectoryDelimiter
   _________________________________________________________________________ */

PathName &
PathName::AppendDirectoryDelimiter ()
{
  size_t l = StrLen(buffer);
  if (l == 0 || ! IsDirectoryDelimiter(buffer[l - 1]))
    {
      if (l >= BufferSizes::MaxPath - 1)
	{
	  BUF_TOO_SMALL (T_("PathName::AppendDirectoryDelimiter"));
	}
      buffer[l] = static_cast<MIKTEXCHAR>(DirectoryDelimiter);
      buffer[l + 1] = 0;
    }
  return (*this);
}

/* _________________________________________________________________________

   PathName::CutOffLastComponent
   _________________________________________________________________________ */

PathName &
PathName::CutOffLastComponent ()
{
  RemoveDirectoryDelimiter (buffer);
  for (size_t end = StrLen(buffer); end > 0; -- end)
    {
      if (end > 0 && IsDirectoryDelimiter(buffer[end - 1]))
	{
#if defined(MIKTEX_WINDOWS)
	  if (end > 1 && buffer[end - 2] == PathName::VolumeDelimiter)
	    {
	      buffer[end] = 0;
	      break;
	    }
#endif
	  if (end == 1)
	    {
	      buffer[1] = 0;
	    }
	  else
	    {
	      buffer[end - 1] = 0;
	    }
	  break;
	}
    }
  return (*this);
}

/* _________________________________________________________________________

   PathName::GetHash
   _________________________________________________________________________ */

size_t
PathName::GetHash ()
  const
{
  size_t h = 0;
  for (const MIKTEXCHAR * lpsz = buffer; *lpsz != 0; ++ lpsz)
    {
      MIKTEXCHAR ch = *lpsz;
#if defined(MIKTEX_WINDOWS)
      if (ch == DirectoryDelimiter)
	{
	  ch = AltDirectoryDelimiter;
	}
      else
	{
	  ch = ToLower(ch);
	}
#endif
      h = 5 * h + ch;
    }
  return (h);
}

/* _________________________________________________________________________

   GetFullPath
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(PathName)
GetFullPath (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  PathName path;

  if (! Utils::IsAbsolutePath(lpszPath))
    {
      path.SetToCurrentDirectory ();
    }

  for (PathNameParser parser (lpszPath); parser.GetCurrent() != 0; ++ parser)
    {
      if (PathName::Compare(parser.GetCurrent(), PARENT_DIRECTORY) == 0)
	{
	  path.CutOffLastComponent ();
	}
      else if (PathName::Compare(parser.GetCurrent(), CURRENT_DIRECTORY) != 0)
	{
	  path.AppendComponent (parser.GetCurrent());
	}
    }

  return (path);
}

/* _________________________________________________________________________

   IsExplicitlyRelativePath
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
IsExplicitlyRelativePath (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  if (lpszPath[0] == T_('.'))
    {
      return (IsDirectoryDelimiter(lpszPath[1])
	      || (lpszPath[1] == T_('.')
		  && IsDirectoryDelimiter(lpszPath[2])));
    }
  else
    {
      return (false);
    }
}

/* _________________________________________________________________________

   SessionImpl::GetSpecialPath
   _________________________________________________________________________ */

PathName
SessionImpl::GetSpecialPath (/*[in]*/ SpecialPath	specialPath)
{
  PathName path;
  switch (specialPath.Get())
    {
    case SpecialPath::BinDirectory:
      path = GetBinDirectory();
      break;
    case SpecialPath::InstallRoot:
      path = GetRootDirectory(GetInstallRoot());
      break;
    case SpecialPath::CommonDataRoot:
      path = GetRootDirectory(GetCommonDataRoot());
      break;
    case SpecialPath::UserDataRoot:
      path = GetRootDirectory(GetUserDataRoot());
      break;
    case SpecialPath::DataRoot:
      path = GetRootDirectory(GetDataRoot());
      break;
    case SpecialPath::CommonConfigRoot:
      path = GetRootDirectory(GetCommonConfigRoot());
      break;
    case SpecialPath::UserConfigRoot:
      path = GetRootDirectory(GetUserConfigRoot());
      break;
    case SpecialPath::ConfigRoot:
      path = GetRootDirectory(GetConfigRoot());
      break;
    default:
      UNEXPECTED_CONDITION (T_("SessionImpl::GetSpecialPath"));
      break;
    }
  return (path);
}

/* _________________________________________________________________________

   miktex_pathcmp
   _________________________________________________________________________ */

MIKTEXAPI(int)
miktex_pathcmp (/*[in]*/ const MIKTEXCHAR *	lpszPath1,
		/*[in]*/ const MIKTEXCHAR *	lpszPath2)
{
  C_FUNC_BEGIN ();
  return (PathName::Compare(lpszPath1, lpszPath2));
  C_FUNC_END ();
}
