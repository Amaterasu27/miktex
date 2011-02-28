/* findfile.cpp: finding files

   Copyright (C) 1996-2011 Christian Schenk

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

#include "fnamedb.h"

/* _________________________________________________________________________

   SessionImpl::SetFindFileCallback
   _________________________________________________________________________ */

void
SessionImpl::SetFindFileCallback (/*[in]*/ IFindFileCallback *	pCallback)
{
  pInstallPackageCallback = pCallback;
}

/* _________________________________________________________________________

   SessionImpl::CheckCandidate

   Try to access a file.  Possibly install the package.
   _________________________________________________________________________ */

bool
SessionImpl::CheckCandidate (/*[in,out]*/ PathName &	path,
			     /*[in]*/ const char *	lpszFileInfo)
{
  bool found = false;
  if (IsMpmFile(path.Get()))
  {
    PathName trigger (Utils::GetRelativizedPath(path.Get(), MPM_ROOT_PATH));
    PathName installRoot;
    if (lpszFileInfo != 0 && pInstallPackageCallback != 0
	&& pInstallPackageCallback->InstallPackage(
	      lpszFileInfo, trigger.Get(), installRoot))
    {
      PathName temp = installRoot;
      temp += path.Get() + MPM_ROOT_PATH_LEN;
      if (File::Exists(temp))
      {
	path = temp;
	found = true;
      }
    }
  }
  else
  {
    found = File::Exists(path);
  }
  return (found);
}

/* _________________________________________________________________________

   SessionImpl::SearchFileSystem
   _________________________________________________________________________ */

bool
SessionImpl::SearchFileSystem (/*[in]*/ const char *		lpszFileName,
			       /*[in]*/ const char *		lpszDirectoryPattern,			       
			       /*[in]*/ bool			firstMatchOnly,
			       /*[out]*/ PathNameArray &	result)
{
  MIKTEX_ASSERT (result.empty());

  if (IsMpmFile(lpszDirectoryPattern))
  {
    return (false);
  }

  trace_filesearch->WriteFormattedLine
    ("core",
     T_("file system search: filename=%s, directory=%s"),
     Q_(lpszFileName),
     Q_(lpszDirectoryPattern));

  PathNameArray directories;

  SearchPathDictionary::const_iterator it =
    expandedPathPatterns.find(lpszDirectoryPattern);

  if (it == expandedPathPatterns.end())
  {
    ExpandPathPattern ("", lpszDirectoryPattern, directories);
    expandedPathPatterns[lpszDirectoryPattern] = directories;
  }
  else
  {
    directories = it->second;
  }

  bool found = false;

  for (PathNameArray::const_iterator it = directories.begin();
       ! (found && firstMatchOnly) && it != directories.end();
       ++ it)
  {
    PathName path (*it, lpszFileName);
    if (CheckCandidate(path, 0))
    {
      found = true;
      result.push_back (path);
    }
  }

  return (found);
}

/* _________________________________________________________________________

   SessionImpl::FindFileInternal
   _________________________________________________________________________ */

bool
SessionImpl::FindFileInternal (/*[in]*/ const char *		lpszFileName,
			       /*[in]*/ const PathNameArray &	directoryPatterns,			       
			       /*[in]*/ bool			firstMatchOnly,
			       /*[in]*/ bool			useFndb,
			       /*[in]*/ bool			searchFileSystem,
			       /*[out]*/ bool &			dontLookOutAnyFurther,
			       /*[out]*/ vector<PathName> &	result)
     
{
  AutoTraceTime att ("find file", lpszFileName);

  MIKTEX_ASSERT (useFndb || searchFileSystem);

  bool found = false;
  dontLookOutAnyFurther = false;

  // if a fully qualified path name is given, then don't look out any
  // further
  if (Utils::IsAbsolutePath(lpszFileName))
  {
    dontLookOutAnyFurther = true;
    PathName path (lpszFileName);
    found = CheckCandidate(path, 0);
    if (found)
    {
      result.push_back (path);
    }
    return (found);
  }

  // if an explicitly relative path name is given, then don't look out
  // any further
  if (IsExplicitlyRelativePath(lpszFileName))
  {
    dontLookOutAnyFurther = true;
    PathName pathWD;
    for (unsigned idx = 0; ! (found && firstMatchOnly) && GetWorkingDirectory(idx, pathWD); ++ idx)
    {
      PathName path (pathWD);
      path += lpszFileName;
      path.MakeAbsolute ();
      if (CheckCandidate(path, 0))
      {
	found = true;
	result.push_back (path);
      }
    }
    return (found);
  }

  // make use of the file name database
  if (useFndb)
  {
    for (PathNameArray::const_iterator it = directoryPatterns.begin();
      ! (found && firstMatchOnly) && it != directoryPatterns.end();
      ++ it)
    {
      if (found && ! firstMatchOnly && IsMpmFile(it->Get()))
      {
	// don't trigger the package installer
	continue;
      }
      FileNameDatabase * pFndb = GetFileNameDatabase(it->Get());
      if (pFndb != 0)
      {
	// search fndb
	AutoFndbRelease autoRelease (pFndb);
	vector<PathName> paths;
	vector<string> fileNameInfo;
	if (pFndb->Search(lpszFileName,
	  it->Get(),
	  firstMatchOnly,
	  paths,
	  fileNameInfo))
	{
	  for (int idx = 0; idx < paths.size(); ++ idx)
	  {
	    if (CheckCandidate(paths[idx], fileNameInfo[idx].c_str()))
	    {
	      found = true;
	      result.push_back (paths[idx]);
	    }
	  }
	}
      }
      else
      {
	// search the file system because the file name database does not exist
	vector<PathName> paths;
	if (SearchFileSystem(lpszFileName, it->Get(), firstMatchOnly, paths))
	{
	  found = true;
	  result.insert (result.end(), paths.begin(), paths.end());
	}
      }
    }
  }

  if (found || ! searchFileSystem)
  {
    return (found);
  }

  // search the file system
  for (PathNameArray::const_iterator it = directoryPatterns.begin();
       ! (found && firstMatchOnly) && it != directoryPatterns.end();
       ++ it)
  {
    if (found && ! firstMatchOnly && IsMpmFile(it->Get()))
    {
      // don't search the virtual MPM directory tree
      continue;
    }
    FileNameDatabase * pFndb = GetFileNameDatabase(it->Get());
    if (pFndb == 0)
    {
      // fndb does not exist => we already searched the file system (see above)
      continue;
    }
    pFndb->Release ();
    vector<PathName> paths;
    if (SearchFileSystem(lpszFileName, it->Get(), firstMatchOnly, paths))
    {
      found = true;
      result.insert (result.end(), paths.begin(), paths.end());
    }
  }

  return (found);
}

/* _________________________________________________________________________

   SessionImpl::FindFileInternal
   _________________________________________________________________________ */

inline
 bool
IsNewer (/*[in]*/ const PathName & path1,
	 /*[in]*/ const PathName & path2)
{
  return (File::Exists(path1) && File::Exists(path2) && File::GetLastWriteTime(path1) > File::GetLastWriteTime(path2));
}

bool
SessionImpl::FindFileInternal (/*[in]*/ const char *	      lpszFileName,
			       /*[in]*/ FileType	      fileType,
			       /*[in]*/ bool		      firstMatchOnly,
			       /*[in]*/ bool		      tryHard,
			       /*[in]*/ bool		      create,
			       /*[in]*/ bool		      renew,
			       /*[out]*/ vector<PathName> &   result)
{
  MIKTEX_ASSERT (result.empty());

  // try to derive the file type
  if (fileType == FileType::None)
  {
    fileType = DeriveFileType(lpszFileName);
    if (fileType == FileType::None)
    {
      trace_filesearch->WriteFormattedLine (
	"core",
	T_("cannot derive file type from %s"),
	Q_(lpszFileName));
      return (false);
    }
  }

  if (renew)
  {
    if (! TryCreateFile(lpszFileName, fileType))
    {
      return (false);
    }
  }

  // construct the search vector
  PathNameArray vec = ConstructSearchVector(fileType);

  // get the file type information
  const InternalFileTypeInfo * pFileTypeInfo = GetInternalFileTypeInfo(fileType);
  MIKTEX_ASSERT (pFileTypeInfo != 0);

  // check to see whether the file name has a registered file name extension
  const char * lpszExtension = GetFileNameExtension(lpszFileName);
  bool hasRegisteredExtension = false;
  if (lpszExtension != 0)
  {
    for (CSVList ext (pFileTypeInfo->fileNameExtensions.c_str(),
		      PATH_DELIMITER);
	 ext.GetCurrent() != 0 && ! hasRegisteredExtension;
	 ++ ext)
    {
      if (PathName::Compare(lpszExtension, ext.GetCurrent()) == 0)
      {
	hasRegisteredExtension = true;
      }
    }
    for (CSVList ext (pFileTypeInfo->alternateExtensions.c_str(),
		      PATH_DELIMITER);
	 ext.GetCurrent() != 0 && ! hasRegisteredExtension;
	 ++ ext)
    {
      if (PathName::Compare(lpszExtension, ext.GetCurrent()) == 0)
      {
	hasRegisteredExtension = true;
      }
    }
  }

  vector<PathName> fileNamesToTry;

  // try each registered file name extension, if none was specified
  if (! hasRegisteredExtension)
  {
    for (CSVList ext (pFileTypeInfo->fileNameExtensions.c_str(),
		      PATH_DELIMITER);
	 ext.GetCurrent() != 0;
	 ++ ext)
    {
      PathName fileName (lpszFileName);
      fileName.AppendExtension (ext.GetCurrent());
      fileNamesToTry.push_back (fileName);
    }
  }

  // try it with the given file name
  fileNamesToTry.push_back (lpszFileName);

  bool dontLookOutAnyFurther = false;

  // first round: use the fndb
  for (vector<PathName>::const_iterator it = fileNamesToTry.begin();
    it != fileNamesToTry.end() && ! dontLookOutAnyFurther;
    ++ it)
  {
    if (FindFileInternal(it->Get(), vec, firstMatchOnly, true, false, dontLookOutAnyFurther, result) && firstMatchOnly)
    {
      return (true);
    }
  }

  // second round: don't use the fndb
  if (tryHard)
  {
    for (vector<PathName>::const_iterator it = fileNamesToTry.begin();
      it != fileNamesToTry.end() && ! dontLookOutAnyFurther;
      ++ it)
    {
      if (FindFileInternal(it->Get(), vec, firstMatchOnly, false, true, dontLookOutAnyFurther, result) && firstMatchOnly)
      {
	return (true);
      }
    }
  }

  if (create)
  {
    if (result.empty())
    {
      if (TryCreateFile(lpszFileName, fileType))
      {
	FindFileInternal (lpszFileName, vec, firstMatchOnly, true, false, dontLookOutAnyFurther, result);
      }
    }
    else if ((fileType == FileType::BASE || fileType == FileType::FMT || fileType == FileType::MEM)
	     && GetConfigValue(MIKTEX_REGKEY_TEXMF,
			       MIKTEX_REGVAL_RENEW_FORMATS_ON_UPDATE,
			       true))
    {
      PathName pathPackagesIniC (
	GetSpecialPath(SpecialPath::CommonInstallRoot),
	MIKTEX_PATH_PACKAGES_INI,
	0);
      PathName pathPackagesIniU (
	GetSpecialPath(SpecialPath::UserInstallRoot),
	MIKTEX_PATH_PACKAGES_INI,
	0);
      if (IsNewer(pathPackagesIniC, result[0])
	|| (pathPackagesIniU != pathPackagesIniC && IsNewer(pathPackagesIniU, result[0])))
      {
	if (TryCreateFile(lpszFileName, fileType))
	{
	  result.clear ();
	  FindFileInternal (lpszFileName, vec, firstMatchOnly, true, false, dontLookOutAnyFurther, result);
	}
      }
    }
  }

  return (! result.empty());
}

/* _________________________________________________________________________

   SessionImpl::FindFile
   _________________________________________________________________________ */

bool
SessionImpl::FindFile (/*[in]*/ const char *	  lpszFileName,
		       /*[in]*/ const char *	  lpszPathList,
		       /*[in]*/ FindFileFlags	  flags,
		       /*[out]*/ PathNameArray &  result)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING (lpszPathList);

  bool dontLookOutAnyFurther;

  bool found = FindFileInternal(
    lpszFileName,
    SplitSearchPath(lpszPathList),
    (flags & FindFileFlags::All) == 0,
    true,
    false,
    dontLookOutAnyFurther,
    result);

  if (! found && ! dontLookOutAnyFurther && (flags & FindFileFlags::TryHard) != 0)
  {
    found = FindFileInternal(
      lpszFileName,
      SplitSearchPath(lpszPathList),
      (flags & FindFileFlags::All) == 0,
      false,
      true,
      dontLookOutAnyFurther,
      result);
  }

  return (found);
}

/* _________________________________________________________________________

   SessionImpl::FindFile
   _________________________________________________________________________ */

bool
SessionImpl::FindFile (/*[in]*/ const char *	lpszFileName,
		       /*[in]*/ const char *	lpszPathList,
		       /*[in]*/ FindFileFlags	flags,
		       /*[out]*/ PathName &	result)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING (lpszPathList);
  MIKTEX_ASSERT ((flags & FindFileFlags::All) == 0);

  PathNameArray paths;

  bool found = FindFile(lpszFileName, lpszPathList, flags, paths);

  if (found)
  {
    result = paths[0];
  }

  return (found);
}

/* _________________________________________________________________________

   SessionImpl::FindFile
   _________________________________________________________________________ */

bool
SessionImpl::FindFile (/*[in]*/ const char *	  lpszFileName,
		       /*[in]*/ FileType	  fileType,
		       /*[in]*/ FindFileFlags	  flags,
		       /*[out]*/ PathNameArray &  result)
{
  return (FindFileInternal(
    lpszFileName,
    fileType,
    (flags & FindFileFlags::All) == 0,
    (flags & FindFileFlags::TryHard) != 0,
    (flags & FindFileFlags::Create) != 0,
    (flags & FindFileFlags::Renew) != 0,
    result));
}

/* _________________________________________________________________________

   SessionImpl::FindFile
   _________________________________________________________________________ */

bool
SessionImpl::FindFile (/*[in]*/ const char *	lpszFileName,
		       /*[in]*/ FileType	fileType,
		       /*[in]*/ FindFileFlags	flags,
		       /*[out]*/ PathName &	result)
{
  MIKTEX_ASSERT ((flags & FindFileFlags::All) == 0);
  vector<PathName> paths;
  bool found = FindFile(lpszFileName, fileType, flags, paths);
  if (found)
  {
    result = paths[0];
  }
  return (found);
}

/* _________________________________________________________________________

   SessionImpl::MakePkFileName
   _________________________________________________________________________ */

#define DEFAULT_PK_NAME_TEMPLATE "%f.pk"

bool
SessionImpl::MakePkFileName (/*[out]*/ PathName &	pkFileName,
			     /*[in]*/ const char *	lpszFontName,
			     /*[in]*/ int		dpi)
{
  string nameTemplate;

  if (! GetSessionValue(MIKTEX_REGKEY_CORE,
			MIKTEX_REGVAL_PK_FN_TEMPLATE,
			nameTemplate,
			DEFAULT_PK_NAME_TEMPLATE))
    {
      UNEXPECTED_CONDITION ("SessionImpl::MakePkFileName ");
    }

  string str;

  str.reserve (BufferSizes::MaxPath);

  for (const char * p = nameTemplate.c_str(); *p != 0; ++ p)
    {
      if (p[0] == '%')
	{
	  ++ p;
	  if (*p == 0)
	    {
	      FATAL_MIKTEX_ERROR ("SessionImpl::MakePkFileName",
				  T_("Invalid file name template."),
				  nameTemplate.c_str());
	    }
	  switch (*p)
	    {
	    case '%':
	      str += '%';
	      break;
	    case 'd':
	      str += NUMTOSTR(dpi);
	      break;
	    case 'f':
	      str += lpszFontName;
	      break;
	    default:
	      FATAL_MIKTEX_ERROR ("SessionImpl::MakePkFileName",
				  T_("Invalid file name template."),
				  nameTemplate.c_str());
	    }
	}
      else
	{
	  str += *p;
	}
    }

  pkFileName = str.c_str();

  return (true);
}

/* _________________________________________________________________________

   SessionImpl::FindPkFile
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
#  define DEFAULT_PK_SEARCH_PATH ".;%R\\fonts\\pk\\%m//dpi%d"
#else
#  define DEFAULT_PK_SEARCH_PATH ".:%R/fonts/pk/%m//dpi%d"
#endif

bool
SessionImpl::FindPkFile (/*[in]*/ const char *	lpszFontName,
			 /*[in]*/ const char *	lpszMode,
			 /*[in]*/ int		dpi,
			 /*[out]*/ PathName &	result)
{
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszMode);

  PathName pkFileName;

  if (! MakePkFileName (pkFileName, lpszFontName, dpi))
    {
      return (false);
    }

  string searchPathTemplate;

  if (! GetSessionValue(MIKTEX_REGKEY_CORE,
			"PKPath",
			searchPathTemplate,
			DEFAULT_PK_SEARCH_PATH))
    {
      UNEXPECTED_CONDITION ("SessionImpl::FindPkFile");
    }

  string searchPath;

  for (const char * q = searchPathTemplate.c_str(); *q != 0; ++ q)
    {
      if (*q == '%')
	{
	  ++ q;
	  if (*q == 0)
	    {
	      FATAL_MIKTEX_ERROR ("SessionImpl::FindPkFile",
				  T_("Invalid search path template."),
				  searchPathTemplate.c_str());
	    }
	  switch (*q)
	    {
	    case 'R':
	      searchPath += '%';
	      searchPath += 'R';
	      break;
	    case '%':
	      searchPath += '%';
	      break;
	    case 'm':
	      if (lpszMode != 0)
		{
		  searchPath += lpszMode;
		}
	      else
		{
		  // <fixme>hardcoded METAFONT mode
		  searchPath += "ljfour";
		  // </fixme>
		}
	      break;
	    case 'd':
	      searchPath += NUMTOSTR(dpi);
	      break;
	    default:
	      FATAL_MIKTEX_ERROR ("SessionImpl::FindPkFile",
				  T_("Invalid search path template."),
				  searchPathTemplate.c_str());
	    }
	}
      else
	{
	  searchPath += *q;
	}
    }

  bool found = FindFile(pkFileName.Get(), searchPath.c_str(), result);

  if (! found
      && (lpszMode == 0 || StringCompare(lpszMode, "modeless", true) != 0))
    {
      // <recursivecall>
      found = FindPkFile(lpszFontName, "modeless", dpi, result);
      // </recursivecall>
    }

  return (found);
}

/* _________________________________________________________________________

   miktex_find_file
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_find_file (/*[in]*/ const char *	lpszFileName,
		  /*[in]*/ const char *	lpszPathList,
		  /*[out]*/ char *	lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING (lpszPathList);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::GetSession()->FindFile(lpszFileName,
					    lpszPathList,
					    temp))
    {
      return (0);
    }
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_find_tfm_file
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_find_tfm_file (/*[in]*/ const char *	lpszFontName,
		      /*[out]*/ char *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::GetSession()->FindFile(lpszFontName,
					    FileType::TFM,
					    temp))
    {
      return (0);
    }
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_find_ttf_file
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_find_ttf_file (/*[in]*/ const char *	lpszFontName,
		      /*[out]*/ char *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::GetSession()->FindFile(lpszFontName,
					    FileType::TTF,
					    temp))
    {
      return (0);
    }
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_find_afm_file
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_find_afm_file (/*[in]*/ const char *	lpszFontName,
		      /*[out]*/ char *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::GetSession()->FindFile(lpszFontName,
					    FileType::AFM,
					    temp))
    {
      return (0);
    }
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_find_enc_file
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_find_enc_file (/*[in]*/ const char *	lpszFontName,
		      /*[out]*/ char *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::GetSession()->FindFile(lpszFontName,
					    FileType::ENC,
					    temp))

    {
      return (0);
    }
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_find_psheader_file
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_find_psheader_file (/*[in]*/ const char *	lpszHeaderName,
			   /*[out]*/ char *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszHeaderName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::GetSession()->FindFile(lpszHeaderName,
					    FileType::PSHEADER,
					    temp))
    {
      return (0);
    }
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_find_input_file
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_find_input_file (/*[in]*/ const char *	lpszApplicationName,
			/*[in]*/ const char *	lpszFileName,
			/*[out]*/ char *	lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING_OR_NIL (lpszApplicationName);
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::GetSession()->FindFile(lpszFileName,
					    FileType::None,
					    temp))
    {
      if (lpszApplicationName == 0)
	{
	  return (0);
	}
      string searchPath = CURRENT_DIRECTORY;
      searchPath += PATH_DELIMITER;
      searchPath += TEXMF_PLACEHOLDER;
      searchPath += MIKTEX_PATH_DIRECTORY_DELIMITER_STRING;
      searchPath += lpszApplicationName;
      searchPath += RECURSION_INDICATOR;
      if (! SessionImpl::GetSession()->FindFile(lpszFileName,
						searchPath.c_str(),
						temp))
	{
	  return (0);
	}
    }
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_find_hbf_file
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_find_hbf_file (/*[in]*/ const char *	lpszFontName,
		      /*[out]*/ char *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::GetSession()->FindFile(lpszFontName,
					    FileType::HBF,
					    temp))
    {
      return (0);
    }
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_find_miktex_executable
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_find_miktex_executable (/*[in]*/ const char *	lpszExeName,
			       /*[out]*/ char *		lpszExePath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszExeName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszExePath);
  PathName temp;
  if (! SessionImpl::GetSession()->FindFile(lpszExeName,
					    FileType::EXE,
					    temp))
    {
      return (0);
    }
  Utils::CopyString (lpszExePath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}
