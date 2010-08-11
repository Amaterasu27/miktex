/* findfile.cpp: finding files

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
			       /*[out]*/ PathNameArray &	result,
			       /*[in]*/ bool			firstMatchOnly)
{
  MIKTEX_ASSERT (result.empty());

  if ((PathName::Compare(MPM_ROOT_PATH,
			 lpszDirectoryPattern,
			 static_cast<unsigned long>(MPM_ROOT_PATH_LEN))
       == 0)
      && (lpszDirectoryPattern[MPM_ROOT_PATH_LEN] == 0
	  || IsDirectoryDelimiter(lpszDirectoryPattern[MPM_ROOT_PATH_LEN])))
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

   SessionImpl::FindFile
   _________________________________________________________________________ */

bool
SessionImpl::FindFile (/*[in]*/ const char *		lpszFileName,
		       /*[in]*/ const PathNameArray &	directoryPatterns,
		       /*[out]*/ vector<PathName> &	result,
		       /*[in]*/ bool			firstMatchOnly)
     
{
  MIKTEX_ASSERT (result.size() == 0);

  AutoTraceTime att ("find file", lpszFileName);

  // if a fully qualified path name is given, then don't look out any
  // further
  if (Utils::IsAbsolutePath(lpszFileName))
  {
    PathName path (lpszFileName);
    bool found = CheckCandidate(path, 0);
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
    PathName pathWD;
    bool found = false;
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

  bool found = false;
  for (PathNameArray::const_iterator it = directoryPatterns.begin();
       ! (found && firstMatchOnly) && it != directoryPatterns.end();
       ++ it)
  {
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
      // search disk
      vector<PathName> paths;
      if (SearchFileSystem(lpszFileName, it->Get(), paths, firstMatchOnly))
      {
	found = true;
	result.insert (result.end(), paths.begin(), paths.end());
      }
    }
  }

  return (found);
}

/* _________________________________________________________________________

   SessionImpl::FindFile
   _________________________________________________________________________ */

bool
SessionImpl::FindFile (/*[in]*/ const char *	  lpszFileName,
		       /*[in]*/ const char *	  lpszPathList,
		       /*[out]*/ PathNameArray &  result)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING (lpszPathList);

  return (FindFile(
    lpszFileName,
    SplitSearchPath(lpszPathList),
    result,
    FALSE));
}

/* _________________________________________________________________________

   SessionImpl::FindFile
   _________________________________________________________________________ */

bool
SessionImpl::FindFile (/*[in]*/ const char *	lpszFileName,
		       /*[in]*/ const char *	lpszPathList,
		       /*[out]*/ PathName &	result)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING (lpszPathList);

  PathNameArray vec = SplitSearchPath(lpszPathList);

  PathNameArray paths;

  bool found = FindFile(lpszFileName, vec, paths, true);

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
SessionImpl::FindFile (/*[in]*/ const char *		lpszFileName,
		       /*[in]*/ FileType		fileType,
		       /*[in]*/ FindFileFlags		flags,
		       /*[out]*/ vector<PathName> &	result,
		       /*[in]*/ bool			firstMatchOnly)
{
  MIKTEX_ASSERT (result.empty());

  // try to derive the file type
  if (fileType == FileType::None)
  {
    fileType = DeriveFileType(lpszFileName);
    if (fileType == FileType::None)
    {
      return (false);
    }
  }

  if ((flags & FindFileFlags::Renew) != 0)
  {
    if (! TryCreateFile(lpszFileName, fileType))
    {
      return (false);
    }
  }

  // construct the search vector
  PathNameArray vec = ConstructSearchVector(fileType);

  // get the file type information
  const FileTypeInfo * pFileTypeInfo = GetInternalFileTypeInfo(fileType);
  MIKTEX_ASSERT (pFileTypeInfo != 0);

  // check to see whether we have a registered file name extension
  const char * lpszExtension = GetFileNameExtension(lpszFileName);
  bool hasRegisteredExtension = false;
  if (lpszExtension != 0)
  {
    for (CSVList ext (pFileTypeInfo->fileNameExtensions.c_str(),
		      PATH_DELIMITER);
	 ext.GetCurrent() != 0;
	 ++ ext)
    {
      if (PathName::Compare(lpszExtension, ext.GetCurrent()) == 0)
      {
	hasRegisteredExtension = true;
	break;
      }
    }
  }      

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
      if (FindFile(fileName.Get(), vec, result, firstMatchOnly) && firstMatchOnly)
      {
	return (true);
      }
    }
  }

  FindFile (lpszFileName, vec, result, firstMatchOnly);

  if ((flags & FindFileFlags::Create) != 0)
  {
    if (result.empty())
    {
      if (TryCreateFile(lpszFileName, fileType))
      {
	FindFile (lpszFileName, vec, result, firstMatchOnly);
      }
    }
    else if ((fileType == FileType::BASE || fileType == FileType::FMT || fileType == FileType::MEM)
	     && GetConfigValue(MIKTEX_REGKEY_TEXMF,
			       MIKTEX_REGVAL_RENEW_FORMATS_ON_UPDATE,
			       true))
    {
      PathName pathPackagesIni (
	GetSpecialPath(SpecialPath::InstallRoot),
	MIKTEX_PATH_PACKAGES_INI,
	0);
      if (File::Exists(pathPackagesIni)
	  && File::GetLastWriteTime(pathPackagesIni) > File::GetLastWriteTime(result[0]))
      {
	if (TryCreateFile(lpszFileName, fileType))
	{
	  result.clear ();
	  FindFile (lpszFileName, vec, result, firstMatchOnly);
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
		       /*[in]*/ FileType	  fileType,
		       /*[in]*/ FindFileFlags	flags,
		       /*[out]*/ PathNameArray &  result)
{
  return (FindFile(lpszFileName, fileType, flags, result, false));
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
  vector<PathName> paths;
  bool found = FindFile(lpszFileName, fileType, flags, paths, true);
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

   SessionImpl::FindTfmFile
   _________________________________________________________________________ */

bool
SessionImpl::FindTfmFile (/*[in]*/ const char *	lpszFontName,
			  /*[out]*/ PathName &	path,
			  /*[in]*/ bool		create)
{
  MIKTEX_ASSERT_STRING (lpszFontName);
  return (FindFile(
    lpszFontName,
    FileType::TFM,
    (create ? FindFileFlags::Create : FindFileFlags::None),
    path));
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
