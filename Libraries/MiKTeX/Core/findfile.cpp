/* findfile.cpp: finding files

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
SessionImpl::CheckCandidate (/*[in,out]*/ PathName &		path,
			     /*[in]*/ const MIKTEXCHAR *	lpszFileInfo)
{
  bool found = false;
  if (IsMpmFile(path.Get()))
    {
      PathName trigger (Utils::GetRelativizedPath(path.Get(), MPM_ROOT_PATH));
      // install the package
      if (lpszFileInfo != 0
	  && pInstallPackageCallback != 0
	  && pInstallPackageCallback->InstallPackage(lpszFileInfo,
						     trigger.Get()))
	{
	  PathName temp = GetSpecialPath(SpecialPath::InstallRoot);
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

   Do a recursive file search using the file system API.
   
   CURDIR is the path to the current directory,
   e.g. "\\server\texmf\fonts\pk\ljfour".

   SUBDIR is a sub-directory we are looking for, if any.

   SEARCHSPEC is a search specification, e.g. "dpi300\cmr10.pk".
   _________________________________________________________________________ */

bool
SessionImpl::SearchFileSystem (/*[in]*/ const MIKTEXCHAR *	lpszCurDir,
			       /*[in]*/ const MIKTEXCHAR *	lpszSubDir,
			       /*[in]*/ const MIKTEXCHAR *	lpszSearchSpec,
			       /*[out]*/ PathName &		result)
{
  MIKTEX_ASSERT (lpszCurDir != 0);
  MIKTEX_ASSERT (*lpszCurDir != 0);
  MIKTEX_ASSERT (lpszSearchSpec != 0);
  MIKTEX_ASSERT (*lpszSearchSpec != 0);
  MIKTEX_ASSERT (lpszSubDir == 0 || *lpszSubDir != 0);

  // if we have a sub directory...
  if (lpszSubDir != 0 && *lpszSubDir != 0)
    {
      // ...then make a new sub-directory path by appending the
      // sub-directory name to the current directory name
      PathName pathCurDirNew (lpszCurDir);
      pathCurDirNew += lpszSubDir;

      // if the new sub-directory exists, then do a recursive search
      // here
      if (Directory::Exists(pathCurDirNew))
	{
	  // <recursivecall>
	  if (SearchFileSystem(pathCurDirNew.Get(), 0, lpszSearchSpec, result))
	    {
	      return (true);
	    }
	  // </recursivecall>
	}

      // do a recursive search in each sub-directory of the current
      // sub-directory
      auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(lpszCurDir));
      DirectoryEntry entry;
      while (pLister->GetNext(entry))
	{
	  if (entry.isDirectory)
	    {
	      // append sub directory name to current directory name
	      pathCurDirNew = lpszCurDir;
	      pathCurDirNew += entry.name.c_str();
		  
	      // and do a recursive search
	      // <recursivecall>
	      if (SearchFileSystem(pathCurDirNew.Get(),
			     lpszSubDir,
			     lpszSearchSpec,
			     result))
		{
		  pLister->Close ();
		  return (true);
		}
	      // </recursivecall>
	    }
	}
      pLister->Close ();
      return (false);
    }
  else
    {
      // no sub-directory is given
      const MIKTEXCHAR * lpszRecInd;
      lpszRecInd = StrStr(lpszSearchSpec, RECURSION_INDICATOR);
      if (lpszRecInd != 0)
	{
	  // decompose the SEARCHSPEC into a sub-directory and a
	  // smaller search spec
	  ptrdiff_t cch =  lpszRecInd - lpszSearchSpec;

	  // do a recursive search with the new sub-directory and
	  // search spec
	  // <recursivecall>
	  return (SearchFileSystem(lpszCurDir,
			     STRDUP(lpszSearchSpec, cch).Get(),
			     lpszRecInd + RECURSION_INDICATOR_LENGTH,
			     result));
	  // <recursivecall/>
	}
      else
	{
	  // the search spec is rather simple: it is a relative
	  // filename path

	  // (I) append the path to the current directory; return, if
	  // the resulting file name is accessible
	  PathName temp (lpszCurDir);
	  temp += lpszSearchSpec;
	  trace_filesearch->WriteFormattedLine (T_("core"),
						T_("trying \"%s\"..."),
						temp.Get());
	  if (CheckCandidate(temp, 0))
	    {
	      result = temp;
	      return (true);
	    }

	  // (II) do a recursive search in each subdirectory of the
	  // current directory
	  auto_ptr<DirectoryLister>
	    pLister (DirectoryLister::Open(lpszCurDir));
	  DirectoryEntry entry;
	  while (pLister->GetNext(entry))
	    {
	      if (entry.isDirectory)
		{
		  PathName temp (lpszCurDir);
		  temp += entry.name.c_str();
		  // <recursivecall>
		  if (SearchFileSystem(temp.Get(), 0, lpszSearchSpec, result))
		    {
		      pLister->Close ();
		      return (true);
		    }
		  // </recursivecall>
		}
	      
	    }
	  pLister->Close ();
	  return (false);
	}
    }
}

/* _________________________________________________________________________

   SessionImpl::SearchFileSystem
   _________________________________________________________________________ */

bool
SessionImpl::SearchFileSystem (/*[in]*/ const MIKTEXCHAR *	lpszRelPath,
			       /*[in]*/ const MIKTEXCHAR *	lpszDirPath,
			       /*[out]*/ PathName &		result)
{
  if ((PathName::Compare(MPM_ROOT_PATH,
			 lpszDirPath,
			 static_cast<unsigned long>(MPM_ROOT_PATH_LEN))
       == 0)
      && (lpszDirPath[MPM_ROOT_PATH_LEN] == 0
	  || IsDirectoryDelimiter(lpszDirPath[MPM_ROOT_PATH_LEN])))
    {
      return (false);
    }

  trace_filesearch->WriteFormattedLine
    (T_("core"),
     T_("slow file search: %s, %s"),
     lpszRelPath,
     lpszDirPath);

  // make a search spec: "DIRPATH\RELPATH"
  tstring str;
  str.reserve (StrLen(lpszDirPath) + StrLen(lpszRelPath) + 2);
  str = lpszDirPath;
  AppendDirectoryDelimiter (str);
  str += lpszRelPath;

  // if search spec doesn't contain "//"...
  tstring::size_type pos = str.find(RECURSION_INDICATOR);
  if (pos == tstring::npos || pos == 0)
    {
      // ...then try to access the file
      result = str;
      return (CheckCandidate(result, 0));
    }
  else
    {
      // otherwise, do a recursive search
      // pos points to the "//" sub-string
      PathName pathCurDir;
      CopyString2 (pathCurDir.GetBuffer(),
		   pathCurDir.GetSize(),
		   str.c_str(),
		   pos);
      if (Directory::Exists(pathCurDir))
	{
	  str.erase (0, pos + RECURSION_INDICATOR_LENGTH);
	  return (SearchFileSystem(pathCurDir.Get(), 0, str.c_str(), result));
	}
      else
	{
	  return (false);
	}
    }
}

/* _________________________________________________________________________

   SessionImpl::SlowFindFile
   _________________________________________________________________________ */

bool
SessionImpl::SlowFindFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
			   /*[in]*/ const MIKTEXCHAR *	lpszPathList,
			   /*[out]*/ PathName &		result)
{
  // if an absolute filename is given, then don't look out any further
  if (Utils::IsAbsolutePath(lpszFileName))
    {
      result = lpszFileName;
      return (CheckCandidate(result, 0));
    }

  // if an explicitly relative path name is given, then don't look out
  // any further
  if (IsExplicitlyRelativePath(lpszFileName))
    {
      PathName pathWD;
      for (unsigned i = 0; GetWorkingDirectory(i, pathWD.GetBuffer()); ++ i)
	{
	  result = pathWD;
	  result += lpszFileName;
	  result.MakeAbsolute ();
	  if (CheckCandidate(result, 0))
	    {
	      return (true);
	    }
	}
      return (false);
    }

  PathNameArray vec = DisassembleSearchPath(lpszPathList);

  // search along vector
  bool found = false;
  for (PathNameArray::const_iterator it = vec.begin();
       ! found && it != vec.end();
       ++ it)
    {
      found = SearchFileSystem(lpszFileName, it->Get(), result);
    }

  return (found);
}

/* _________________________________________________________________________

   SessionImpl::FindFileAlongVec
   _________________________________________________________________________ */

bool
SessionImpl::FindFileAlongVec (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
			       /*[in]*/ const PathNameArray &	vec,
			       /*[out]*/ PathName &		result)
     
{
  // if a fully qualified path name is given, then don't look out any
  // further
  if (Utils::IsAbsolutePath(lpszFileName))
    {
      result = lpszFileName;
      return (CheckCandidate(result, 0));
    }

  // if an explicitly relative path name is given, then don't look out
  // any further
  if (IsExplicitlyRelativePath(lpszFileName))
    {
      PathName pathWD;
      for (unsigned i = 0; GetWorkingDirectory(i, pathWD.GetBuffer()); ++ i)
	{
	  result = pathWD;
	  result += lpszFileName;
	  result.MakeAbsolute ();
	  if (CheckCandidate(result, 0))
	    {
	      return (true);
	    }
	}
      return (false);
    }

  // get the current directory
  PathName pathCWD;
  pathCWD.SetToCurrentDirectory ();

  // search along vector
  bool found = false;
  for (PathNameArray::const_iterator it = vec.begin();
       ! found && it != vec.end();
       ++ it)
    {
      if (PathName::Compare(*it, pathCWD) == 0)
	{
	  // special case: path is the current working directory
	  MIKTEX_ASSERT (! Utils::IsAbsolutePath(lpszFileName));
	  result = lpszFileName;
	  if (CheckCandidate(result, 0))
	    {
	      return (true);
	    }
	}

      PathName pathWD;
      for (unsigned i = 1; GetWorkingDirectory(i, pathWD.GetBuffer()); ++ i)
	{
	  if (PathName::Compare(*it, pathWD) == 0)
	    {
	      // special case: path is one of the include directories
	      MIKTEX_ASSERT (! Utils::IsAbsolutePath(lpszFileName));
	      result = pathWD;
	      result += lpszFileName;
	      if (CheckCandidate(result, 0))
		{
		  return (true);
		}
	    }
	}

      FileNameDatabase * pFndb = GetFileNameDatabase(it->Get());
      if (pFndb != 0)
	{
	  // search fndb
	  AutoFndbRelease autoRelease (pFndb);
	  PathName pathFileNameInfo;
	  found =
	    pFndb->Search(lpszFileName,
			  it->Get(),
			  result,
			  pathFileNameInfo.GetBuffer(),
			  pathFileNameInfo.GetSize());
	  if (found)
	    {
	      found = CheckCandidate(result, pathFileNameInfo.Get());
	    }
	}
      else
	{
	  // search disk
	  found = SearchFileSystem(lpszFileName, it->Get(), result);
	}
    }

  return (found);
}

/* _________________________________________________________________________

   SessionImpl::FindFile
   _________________________________________________________________________ */

bool
SessionImpl::FindFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		       /*[in]*/ const MIKTEXCHAR *	lpszPathList,
		       /*[out]*/ PathName &		result)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING (lpszPathList);

  PathNameArray vec = DisassembleSearchPath(lpszPathList);

  return (FindFileAlongVec(lpszFileName, vec, result));
}

/* _________________________________________________________________________

   SessionImpl::FindFile
   _________________________________________________________________________ */

bool
SessionImpl::FindFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		       /*[in]*/ FileType		fileType,
		       /*[out]*/ PathName &		result)
{
  if (fileType == FileType::None)
    {
      fileType = DeriveFileType(lpszFileName);
      if (fileType == FileType::None)
	{
	  return (false);
	}
    }

  const FileTypeInfo * pFileTypeInfo = GetFileTypeInfo(fileType);

  PathNameArray vec = ConstructSearchVector(fileType);

  if (FindFileAlongVec(lpszFileName, vec, result))
    {
      return (true);
    }

  if (GetFileNameExtension(lpszFileName) == 0)
    {
      PathName fileName (lpszFileName);
      for (CSVList2 ext (pFileTypeInfo->fileNameExtensions.c_str(),
			 PATH_DELIMITER);
	   ext.GetCurrent() != 0;
	   ++ ext)
	{
	  fileName.SetExtension (ext.GetCurrent());
	  if (FindFileAlongVec(fileName.Get(), vec, result))
	    {
	      return (true);
	    }
	}
    }

  return (false);
}

/* _________________________________________________________________________

   SessionImpl::MakePkFileName
   _________________________________________________________________________ */

#define DEFAULT_PK_NAME_TEMPLATE T_("%f.pk")

bool
SessionImpl::MakePkFileName (/*[out]*/ PathName &		pkFileName,
			     /*[in]*/ const MIKTEXCHAR *	lpszFontName,
			     /*[in]*/ int			dpi)
{
  tstring nameTemplate;

  if (! GetSessionValue(MIKTEX_REGKEY_CORE,
			MIKTEX_REGVAL_PK_FN_TEMPLATE,
			nameTemplate,
			DEFAULT_PK_NAME_TEMPLATE))
    {
      UNEXPECTED_CONDITION (T_("SessionImpl::MakePkFileName "));
    }

  tstring str;

  str.reserve (BufferSizes::MaxPath);

  for (const MIKTEXCHAR * p = nameTemplate.c_str(); *p != 0; ++ p)
    {
      if (p[0] == T_('%'))
	{
	  ++ p;
	  if (*p == 0)
	    {
	      FATAL_MIKTEX_ERROR (T_("SessionImpl::MakePkFileName"),
				  T_("Invalid file name template."),
				  nameTemplate.c_str());
	    }
	  switch (*p)
	    {
	    case T_('%'):
	      str += T_('%');
	      break;
	    case T_('d'):
	      str += NUMTOSTR(dpi);
	      break;
	    case T_('f'):
	      str += lpszFontName;
	      break;
	    default:
	      FATAL_MIKTEX_ERROR (T_("SessionImpl::MakePkFileName"),
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
#  define DEFAULT_PK_SEARCH_PATH T_(".;%R\\fonts\\pk\\%m//dpi%d")
#else
#  define DEFAULT_PK_SEARCH_PATH T_(".:%R/fonts/pk/%m//dpi%d")
#endif

bool
SessionImpl::FindPkFile (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
			 /*[in]*/ const MIKTEXCHAR *	lpszMode,
			 /*[in]*/ int			dpi,
			 /*[out]*/ PathName &		result)
{
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszMode);

  PathName pkFileName;

  if (! MakePkFileName (pkFileName, lpszFontName, dpi))
    {
      return (false);
    }

  tstring searchPathTemplate;

  if (! GetSessionValue(MIKTEX_REGKEY_CORE,
			T_("PKPath"),
			searchPathTemplate,
			DEFAULT_PK_SEARCH_PATH))
    {
      UNEXPECTED_CONDITION (T_("SessionImpl::FindPkFile"));
    }

  tstring searchPath;

  for (const MIKTEXCHAR * q = searchPathTemplate.c_str(); *q != 0; ++ q )
    {
      if (*q == T_('%'))
	{
	  ++ q;
	  if (*q == 0)
	    {
	      FATAL_MIKTEX_ERROR (T_("SessionImpl::FindPkFile"),
				  T_("Invalid search path template."),
				  searchPathTemplate.c_str());
	    }
	  switch (*q)
	    {
	    case T_('R'):
	      searchPath += T_('%');
	      searchPath += T_('R');
	      break;
	    case T_('%'):
	      searchPath += T_('%');
	      break;
	    case T_('m'):
	      if (lpszMode != 0)
		{
		  searchPath += lpszMode;
		}
	      else
		{
		  // <fixme>hardcoded METAFONT mode
		  searchPath += T_("ljfour");
		  // </fixme>
		}
	      break;
	    case T_('d'):
	      searchPath += NUMTOSTR(dpi);
	      break;
	    default:
	      FATAL_MIKTEX_ERROR (T_("SessionImpl::FindPkFile"),
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
      && (lpszMode == 0 || StringCompare(lpszMode, T_("modeless"), true) != 0))
    {
      // <recursivecall>
      found = FindPkFile(lpszFontName, T_("modeless"), dpi, result);
      // </recursivecall>
    }

  return (found);
}

/* _________________________________________________________________________

   SessionImpl::FindTfmFile
   _________________________________________________________________________ */

bool
SessionImpl::FindTfmFile (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
			  /*[out]*/ PathName &		path,
			  /*[in]*/ bool			create)
{
  MIKTEX_ASSERT_STRING (lpszFontName);
  bool exists = SessionImpl::FindFile(lpszFontName, FileType::TFM, path);
  if (exists || ! create)
    {
      return (exists);
    }
  PathName makeTFM;
  if (! SessionImpl::FindFile(MIKTEX_MAKETFM_EXE, FileType::EXE, makeTFM))
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::FindTfmFile"),
			  T_("The MakeTFM utility could not be found."),
			  0);
    }
  MIKTEXCHAR szBasename[BufferSizes::MaxPath];
  PathName::Split (lpszFontName,
		   0, 0,
		   szBasename, BufferSizes::MaxPath,
		   0, 0);
  CommandLineBuilder commandLine;
  commandLine.AppendOption (T_("-v"));
  commandLine.AppendArgument (szBasename);
  char szBuf[4096];
  size_t size = 4096;
  int exitCode;
  if (! Process::Run(makeTFM,
		     commandLine.Get(),
		     szBuf,
		     &size,
		     &exitCode))
    {
      return (false);
    }
  if (exitCode != 0)
    {
      TraceError (T_("MakeTFM failed; output follows"));
      TraceError (T_("%.*s"), static_cast<int>(size), szBuf);
      return (false);
    }
  if (! SessionImpl::FindFile(lpszFontName, FileType::TFM, path))
    {
      FATAL_MIKTEX_ERROR
	(T_("SessionImpl::FindTfmFile"),
	 T_("MakeTFM succeeded but the TFM file could not be found."),
	 szBasename);
    }
  return (true);
}

/* _________________________________________________________________________

   miktex_find_file
   _________________________________________________________________________ */


MIKTEXAPI(int)
miktex_find_file (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		  /*[in]*/ const MIKTEXCHAR *	lpszPathList,
		  /*[out]*/ MIKTEXCHAR *	lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING (lpszPathList);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszFileName,
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

   miktex_find_pk_file
   _________________________________________________________________________ */

MIKTEXAPI(int)
miktex_find_pk_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		     /*[in]*/ const MIKTEXCHAR *	lpszMode,
		     /*[in]*/ int			dpi,
		     /*[out]*/ MIKTEXCHAR *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszMode);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindPkFile(lpszFontName,
					    lpszMode,
					    dpi,
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

MIKTEXAPI(int)
miktex_find_tfm_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszFontName,
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

MIKTEXAPI(int)
miktex_find_ttf_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszFontName,
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

MIKTEXAPI(int)
miktex_find_afm_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszFontName,
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

MIKTEXAPI(int)
miktex_find_enc_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszFontName,
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

MIKTEXAPI(int)
miktex_find_psheader_file (/*[in]*/ const MIKTEXCHAR *	lpszHeaderName,
			   /*[out]*/ MIKTEXCHAR *	lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszHeaderName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszHeaderName,
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

MIKTEXAPI(int)
miktex_find_input_file (/*[in]*/ const MIKTEXCHAR *	lpszApplicationName,
			/*[in]*/ const MIKTEXCHAR *	lpszFileName,
			/*[out]*/ MIKTEXCHAR *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING_OR_NIL (lpszApplicationName);
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszFileName,
					  FileType::None,
					  temp))
    {
      if (lpszApplicationName == 0)
	{
	  return (0);
	}
      tstring searchPath = CURRENT_DIRECTORY;
      searchPath += PATH_DELIMITER;
      searchPath += TEXMF_PLACEHOLDER;
      searchPath += MIKTEX_PATH_DIRECTORY_DELIMITER_STRING;
      searchPath += lpszApplicationName;
      searchPath += RECURSION_INDICATOR;
      if (! SessionImpl::theSession->FindFile(lpszFileName,
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

MIKTEXAPI(int)
miktex_find_hbf_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszFontName,
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

   miktex_find_type1_font_file
   _________________________________________________________________________ */

MIKTEXAPI(int)
miktex_find_type1_font_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
			     /*[out]*/ MIKTEXCHAR *		lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszFontName,
					  FileType::TYPE1,
					  temp))
    {
      return (0);
    }
  Utils::CopyString (lpszPath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END();
}

/* _________________________________________________________________________

   miktex_find_miktex_executable
   _________________________________________________________________________ */

MIKTEXAPI(int)
miktex_find_miktex_executable (/*[in]*/ const MIKTEXCHAR *	lpszExeName,
			       /*[out]*/ MIKTEXCHAR *		lpszExePath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszExeName);
  MIKTEX_ASSERT_PATH_BUFFER (lpszExePath);
  PathName temp;
  if (! SessionImpl::theSession->FindFile(lpszExeName,
					  FileType::EXE,
					  temp))
    {
      return (0);
    }
  Utils::CopyString (lpszExePath, BufferSizes::MaxPath, temp.Get());
  return (1);
  C_FUNC_END ();
}
