/* path.cpp: path name utilities

   Copyright (C) 1996-2014 Christian Schenk

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
   C:\ef.gh			C:\			ef.gh
   _________________________________________________________________________ */

const char *
Utils::GetRelativizedPath (/*[in]*/ const char * lpszPath,
			   /*[in]*/ const char * lpszRoot)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_STRING (lpszRoot);

  MIKTEX_ASSERT (Utils::IsAbsolutePath(lpszPath));
  MIKTEX_ASSERT (Utils::IsAbsolutePath(lpszRoot));

  PathName pathNorm (lpszPath);
  pathNorm.Normalize ();

  PathName pathRootNorm (lpszRoot);
  pathRootNorm.Normalize ();

  size_t rootLen = strlen(lpszRoot);

  MIKTEX_ASSERT (rootLen > 0);

#if defined(MIKTEX_WINDOWS)
  int cmp = StrNCmpI(pathNorm.Get(), pathRootNorm.Get(), rootLen);
#else
  int cmp = strncmp(pathNorm.Get(), pathRootNorm.Get(), rootLen);
#endif
  if (cmp != 0)
  {
    return (0);
  }

  // special case: both paths are equal
  if (rootLen == strlen(lpszPath))
  {
#if 1
    return (lpszPath + rootLen);
#else
    return (".");
#endif
  }

  int ch = lpszRoot[rootLen - 1];

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
Utils::IsAbsolutePath (/*[in]*/ const char * lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  // "\xyz\foo.txt", "\\server\xyz\foo.txt"
  if (IsDirectoryDelimiter(lpszPath[0]))
    {
      return (true);
    }
#if defined(MIKTEX_WINDOWS)
  else if (IsDriveLetter(lpszPath[0]) // "C:\xyz\foo.txt"
	   && lpszPath[1] == ':'
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

   Utils::IsSafeFileName
   _________________________________________________________________________ */

static const char * const forbiddenFileNames[] = {
#if defined(MIKTEX_WINDOWS)
  "desktop.ini",
  "folder.htt",
#endif
  0,
};

static const char * const forbiddenExtensions[] = {
#if defined(MIKTEX_WINDOWS)
  ".bat",
  ".cmd",
  ".com",
  ".exe",
  ".js",
  ".jse",
  ".lnk",
  ".msc",
  ".pif",
  ".reg",
  ".scr",
  ".pl"
  ".py",
  ".pyw",
  ".tcl",
  ".url",
  ".vbe",
  ".vbs",
  ".ws",
  ".wsf",
  ".wsh",
#endif
  0
};

bool
Utils::IsSafeFileName (/*[in]*/ const char *	lpszPath,
		       /*[in]*/ bool		forInput)
{
  if (forInput)
  {
    return (true);
  }
  if (IsAbsolutePath(lpszPath))
    {
      return (false);
    }
  const char * lpszFileName = 0;
  for (PathNameParser parser (lpszPath);
       parser.GetCurrent() != 0;
       ++ parser)
    {
      if (PathName::Compare(parser.GetCurrent(), PARENT_DIRECTORY) == 0)
	{
	  return (false);
	}
      lpszFileName = parser.GetCurrent();
    }
  MIKTEX_ASSERT (lpszFileName != 0);
  for (int idx = 0;
       forbiddenFileNames[idx] != 0;
       ++ idx)
    {
      if (PathName::Compare(forbiddenFileNames[idx], lpszFileName) == 0)
	{
	  return (false);
	}
    }  
#if defined(MIKTEX_WINDOWS)
  const char * lpszExtension = GetFileNameExtension(lpszFileName);
  string forbiddenExtensions;
  if (::GetEnvironmentString("PATHEXT", forbiddenExtensions) && lpszExtension != 0)
    {
      for (CSVList ext (forbiddenExtensions.c_str(), PATH_DELIMITER);
	   ext.GetCurrent() != 0;
	   ++ ext)
	{
	  if (PathName::Compare(ext.GetCurrent(), lpszExtension) == 0)
	    {
	      return (false);
	    }
	}
    }
#endif
  return (true);
}

/* _________________________________________________________________________

   PathName::Compare
   _________________________________________________________________________ */

int
PathName::Compare (/*[in]*/ const char *	lpszPath1,
		   /*[in]*/ const char *	lpszPath2)
{
  MIKTEX_ASSERT_STRING (lpszPath1);
  MIKTEX_ASSERT_STRING (lpszPath2);

  int ret;
  int cmp;
  
  while ((cmp = CompareFileNameChars(*lpszPath1, *lpszPath2)) == 0
	 && *lpszPath2 != 0)
    {
      ++ lpszPath1;
      ++ lpszPath2;
    }

  if (cmp != 0)
  {
    MIKTEX_ASSERT (! (*lpszPath1 == 0 && *lpszPath2 == 0));
    if (
      *lpszPath1 == 0 && IsDirectoryDelimiter(*lpszPath2) && *(lpszPath2 + 1) == 0
      || *lpszPath2 == 0 && IsDirectoryDelimiter(*lpszPath1) && *(lpszPath1 + 1) == 0)
    {
      return (0);
    }
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
NormalizePath (/*[in,out]*/ char *	lpszPath)
{
#if defined(MIKTEX_WINDOWS)
  for (size_t l = 0; lpszPath[l] != 0; ++ l)
    {
      if (lpszPath[l] == PathName::DirectoryDelimiter)
	{
	  lpszPath[l] = PathName::AltDirectoryDelimiter;
	}
      else if (lpszPath[l] >= 'A' && lpszPath[l] <= 'Z')
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
PathName::Compare (/*[in]*/ const char *	lpszPath1,
		   /*[in]*/ const char *	lpszPath2,
		   /*[in]*/ size_t		count)
 
{
  MIKTEX_ASSERT_STRING (lpszPath1);
  MIKTEX_ASSERT_STRING (lpszPath2);

  if (count == 0)
    {
      return (0);
    }

  char szPath1[BufferSizes::MaxPath];
  char szPath2[BufferSizes::MaxPath];

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
	  return (*reinterpret_cast<const unsigned char *>(lpszPath1)
		  - *reinterpret_cast<const unsigned char *>(lpszPath2));
	}
    }
  
  return (0);
}

/* _________________________________________________________________________

   PathName::Convert
   _________________________________________________________________________ */

PathName &
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

  if ((flags & ConvertPathNameFlags::Canonicalize) != 0)
    {
      Utils::CanonicalizePathName (*this);
    }

  for (char * lpsz = GetBuffer(); *lpsz != 0; ++ lpsz)
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
  if (StrChr(path.Get(), ' ') != 0)
    {
      FATAL_MIKTEX_ERROR ("Utils::MakeTeXPathName",
			  T_("Path name contains the space character."),
			  path.Get());
    }
#endif
}

/* _________________________________________________________________________

   PathName::Match
   _________________________________________________________________________ */

bool
PathName::Match (/*[in]*/ const char *	lpszPattern,
		 /*[in]*/ const char *	lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_STRING (lpszPattern);

  switch (*lpszPattern)
    {
    case 0:
      return (*lpszPath == 0);
      
    case '*':
      return (Match(lpszPattern + 1, lpszPath) // <recursivecall/>
	      || (*lpszPath != 0
		  && Match(lpszPattern, lpszPath + 1)));
      
    case '?':
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
Utils::IsParentDirectoryOf (/*[in]*/ const char * lpszParentDir,
			    /*[in]*/ const char * lpszFileName)
{
  size_t len1 = strlen(lpszParentDir);
  if (PathName::Compare(lpszParentDir, lpszFileName, len1) != 0)
    {
      return (false);
    }
  size_t len2 = strlen(lpszFileName);
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
Utils::GetUncRootFromPath (/*[in]*/ const char *	lpszPath,
			   /*[out]*/ PathName &		uncRoot)
{
  // must start with "\\"
  if (! (IsDirectoryDelimiter(lpszPath[0])
	 && IsDirectoryDelimiter(lpszPath[1])))
    {
      return (false);
    }
  
  uncRoot = lpszPath;

  char * lpsz = uncRoot.GetBuffer() + 2;

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
AppendDirectoryDelimiter (/*[in,out]*/ string & path)
{
  size_t l = path.length();
  if (l > 0 && ! IsDirectoryDelimiter(path[l - 1]))
    {
      path += PathName::DirectoryDelimiter;
    }
}

/* _________________________________________________________________________

   AppendDirectoryDelimiter
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
AppendDirectoryDelimiter (/*[in,out]*/ char *	lpszPath,
			  /*[in]*/ size_t	size)
{
  MIKTEX_ASSERT (size > 0);
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_CHAR_BUFFER (lpszPath, size);
  size_t l = strlen(lpszPath);
  MIKTEX_ASSERT (l < size);
  if (l > 0 && ! IsDirectoryDelimiter(lpszPath[l - 1]))
    {
      if (l + 1 >= size)
	{
	  INVALID_ARGUMENT ("AppendDirectoryDelimiter", lpszPath);
	}
      lpszPath[l] = PathName::DirectoryDelimiter;
      lpszPath[l + 1] = 0;
    }
}

/* _________________________________________________________________________

   RemoveDirectoryDelimiter
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
RemoveDirectoryDelimiter (/*[in,out]*/ char * lpszPath)
{
  size_t l = strlen(lpszPath);
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
PathName::Combine (/*[out]*/ char *		lpszPath,
		   /*[in]*/ size_t		sizePath,
		   /*[in]*/ const char *	lpszAbsPath,
		   /*[in]*/ const char *	lpszRelPath,
		   /*[in]*/ const char *	lpszExtension)
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
	  BUF_TOO_SMALL ("PathName::Combine");
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
      if (*lpszExtension != '.')
	{
	  if (n + 1 >= sizePath)
	    {
	      BUF_TOO_SMALL ("PathName::Combine");
	    }
	  lpszPath[n] = '.';
	  ++ n;
	}
      n += Utils::CopyString(&lpszPath[n], sizePath - n, lpszExtension);
    }

  MIKTEX_ASSERT (strlen(lpszPath) == n);
}

/* _________________________________________________________________________

   PathName::Split
   _________________________________________________________________________ */

void
PathName::Split (/*[in]*/ const char *	lpszPath,
		 /*[out]*/ char *	lpszDir,
		 /*[in]*/ size_t	sizeDir,
		 /*[out]*/ char *	lpszName,
		 /*[in]*/ size_t	sizeName,
		 /*[out]*/ char *	lpszExtension,
		 /*[in]*/ size_t	sizeExtension)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszDir, sizeDir);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszName, sizeName);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszExtension, sizeExtension);

  const char * lpsz;

  const char * lpszName_ = 0;

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
  const char * lpszExtension_ = 0;
  for (lpsz = lpszName_; *lpsz != 0; ++ lpsz)
    {
      if (*lpsz == '.')
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

MIKTEXINTERNALFUNC(const char *)
GetFileNameExtension (/*[in]*/ const char * lpszPath)
{
  const char * lpszExtension = 0;
  for (const char * lpsz = lpszPath; *lpsz != 0; ++ lpsz)
    {
      if (IsDirectoryDelimiter(*lpsz))
	{
	  lpszExtension = 0;
	}
      else if (*lpsz == '.')
	{
	  lpszExtension = lpsz;
	}
    }
  return (lpszExtension);
}

/* _________________________________________________________________________

   PathName::GetExtension
   _________________________________________________________________________ */

const char *
PathName::GetExtension ()
  const
{
  return (GetFileNameExtension(Get()));
}

/* _________________________________________________________________________

   PathName::SetExtension
   _________________________________________________________________________ */

PathName &
PathName::SetExtension (/*[in]*/ const char *	lpszExtension,
			/*[in]*/ bool		override)
{
  char szDir[BufferSizes::MaxPath];
  char szFileName[BufferSizes::MaxPath];
  char szExtOld[BufferSizes::MaxPath];

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
  size_t l = GetLength();
  if (l == 0 || ! IsDirectoryDelimiter(Base::operator[](l - 1)))
    {
      Base::Append (DirectoryDelimiter);
    }
  return (*this);
}

/* _________________________________________________________________________

   PathName::CutOffLastComponent
   _________________________________________________________________________ */

PathName &
PathName::CutOffLastComponent (/*[in]*/ bool allowSelfCutting)
{
  RemoveDirectoryDelimiter (GetBuffer());
  bool noCut = true;
  for (size_t end = GetLength(); noCut && end > 0; -- end)
    {
      if (end > 0 && IsDirectoryDelimiter(Base::operator[](end - 1)))
	{
#if defined(MIKTEX_WINDOWS)
	  if (end > 1
	      && Base::operator[](end - 2) == PathName::VolumeDelimiter)
	    {
	      Base::operator[](end) = 0;
	    }
	  else
#endif
	  if (end == 1)
	    {
	      Base::operator[](1) = 0;
	    }
	  else
	    {
	      Base::operator[](end - 1) = 0;
	    }
	  noCut = false;
	}
    }
  if (noCut && allowSelfCutting)
  {
    Base::operator[](0) = 0;
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
  for (const char * lpsz = GetBuffer(); *lpsz != 0; ++ lpsz)
    {
      char ch = *lpsz;
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
GetFullPath (/*[in]*/ const char * lpszPath)
{
  PathName path;

  if (! Utils::IsAbsolutePath(lpszPath))
    {
#if defined(MIKTEX_WINDOWS)
      if (IsDriveLetter(lpszPath[0])
	  && lpszPath[1] == ':'
	  && lpszPath[2] == 0)
	{
	  path = lpszPath;
	  path += PathName::DirectoryDelimiter;
	  return (path);
	}
#endif
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
IsExplicitlyRelativePath (/*[in]*/ const char * lpszPath)
{
  if (lpszPath[0] == '.')
    {
      return (IsDirectoryDelimiter(lpszPath[1])
	      || (lpszPath[1] == '.'
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
    case SpecialPath::InternalBinDirectory:
      path = GetBinDirectory();
      path += "internal";
      break;
    case SpecialPath::CommonInstallRoot:
      path = GetRootDirectory(GetCommonInstallRoot());
      break;
    case SpecialPath::UserInstallRoot:
      path = GetRootDirectory(GetUserInstallRoot());
      break;
    case SpecialPath::InstallRoot:
      path = GetRootDirectory(GetInstallRoot());
      break;
    case SpecialPath::DistRoot:
      path = GetRootDirectory(GetDistRoot());
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
    case SpecialPath::PortableRoot:
      if (! IsMiKTeXPortable())
      {
	INVALID_ARGUMENT ("SessionImpl::GetSpecialPath", 0);
      }
      path = GetRootDirectory(GetInstallRoot());
      break;
    case SpecialPath::PortableMount:
      if (! IsMiKTeXPortable())
      {
	INVALID_ARGUMENT ("SessionImpl::GetSpecialPath", 0);
      }
#if MIKTEX_WINDOWS
      if (! GetVolumePathNameA(GetRootDirectory(GetInstallRoot()).Get(),
	path.GetBuffer(),
	path.GetCapacity()))
      {
	FATAL_WINDOWS_ERROR ("GetVolumePathNameA", GetRootDirectory(GetInstallRoot()).Get());
      }
#else
      INVALID_ARGUMENT ("SessionImpl::GetSpecialPath", 0);
#endif
      break;
    default:
      UNEXPECTED_CONDITION ("SessionImpl::GetSpecialPath");
      break;
    }
  return (path);
}

/* _________________________________________________________________________

   Utils::GetPathNamePrefix
   _________________________________________________________________________ */

bool
Utils::GetPathNamePrefix (/*[in]*/ const PathName &	path,
			  /*[in]*/ const PathName &	suffix,
			  /*[out]*/ PathName &		prefix)
{
  MIKTEX_ASSERT (! Utils::IsAbsolutePath(suffix.Get()));

  PathName path_ (path);
  PathName suffix_ (suffix);

  while (! suffix_.Empty())
  {
    char p[BufferSizes::MaxPath];
    char s[BufferSizes::MaxPath];
    if (PathName::Compare(path_.GetFileName(p), suffix_.GetFileName(s)) != 0)
    {
      return (false);
    }
    path_.CutOffLastComponent (true);
    suffix_.CutOffLastComponent (true);
  }

  prefix = path_;

  return (true);
}

/* _________________________________________________________________________

   miktex_pathcmp
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_pathcmp (/*[in]*/ const char *	lpszPath1,
		/*[in]*/ const char *	lpszPath2)
{
  C_FUNC_BEGIN ();
  return (PathName::Compare(lpszPath1, lpszPath2));
  C_FUNC_END ();
}
