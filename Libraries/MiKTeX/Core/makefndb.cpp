/* makefndb.cpp: creating the file name database

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

#include "fndbmem.h"

const unsigned char null_byte = 0;

#define FN_MIKTEXIGNORE ".miktexignore"

struct FILENAMEINFO
{
  string	FileName;
  string	Info;
};
  
struct COMPAREFILENAMEINFO
  : public std::binary_function<FILENAMEINFO, FILENAMEINFO, bool>
{
  bool
  operator() (/*[in]*/ const FILENAMEINFO & lhs,
	      /*[in]*/ const FILENAMEINFO & rhs)
    const
  {
    return (PathName::Compare(lhs.FileName.c_str(),
			      rhs.FileName.c_str())
	    < 0);
  }
};

/* _________________________________________________________________________

   FndbManager
   _________________________________________________________________________ */

class FndbManager
{
public:
  enum {
    AlignmentDoubleWord = 4,
    AlignmentDirectory = 4,
  };

public:
  FndbManager ()
    : traceStream (TraceStream::Open("fndb"))
  {
  }

public:
  ~FndbManager ()
  {
    try
      {
	traceStream->Close ();
	traceStream = 0;
      }
    catch (const exception &)
      {
      }
  }

public:
  bool
  Create (/*[in]*/ const char *			lpszFndbPath,
	  /*[in]*/ const char *			lpszRootPath,
	  /*[in]*/ ICreateFndbCallback *	pCallback,
	  /*[in]*/ bool				enableStringPooling,
	  /*[in]*/ bool				storeFileNameInfo);

private:
  void *
  GetMemPointer ();

private:
  FileNameDatabaseHeader::FndbOffset
  GetMemTop ()
    const;

private:
  void
  SetMem (/*[in]*/ FileNameDatabaseHeader::FndbOffset	fndboff,
	  /*[in]*/ const void *				pData,
	  /*[in]*/ size_t				size);

private:
  void
  SetMem (/*[in]*/ FileNameDatabaseHeader::FndbOffset	fndboff,
	  /*[in]*/ unsigned long			size);

private:
  FileNameDatabaseHeader::FndbOffset
  ReserveMem (/*[in]*/ size_t size);

private:
  void
  FastPushBack (/*[in]*/ unsigned char data);

private:
  FileNameDatabaseHeader::FndbOffset
  PushBack (/*[in]*/ unsigned char data);

private:
  FileNameDatabaseHeader::FndbOffset
  PushBack (/*[in]*/ unsigned long data);

private:
  FileNameDatabaseHeader::FndbOffset
  PushBack (/*[in]*/ const void *	pData,
	    /*[in]*/ size_t		size);

private:
  FileNameDatabaseHeader::FndbOffset
  PushBack (/*[in]*/ const char * lpsz);

private:
  void
  AlignMem (/*[in]*/ unsigned long align = 8);

private:
  static void
  GetIgnorableFiles (/*[in]*/ const char *	lpszPath,
		     /*[out]*/ vector<string> &	filesToBeIgnored);

public:
  void
  ReadDirectory (/*[in]*/ const char *			lpszPath,
		 /*[out]*/ vector<string> &		subDirectoryNames,
		 /*[out]*/ vector<FILENAMEINFO> &	fileNames);

private:
  FileNameDatabaseHeader::FndbOffset
  ProcessFolder (/*[in]*/ FileNameDatabaseHeader::FndbOffset	foParent,
		 /*[in]*/ const char *			lpszParentPath,
		 /*[in]*/ const char *			lpszFolderName,
		 /*[in]*/ FileNameDatabaseHeader::FndbOffset	foFolderName);

private:
  vector<unsigned char> byteArray;

private:
  unsigned long deepestLevel;

private:
  unsigned long currentLevel;

private:
  unsigned long numDirectories;

private:
  unsigned long numFiles;

private:
  ICreateFndbCallback * pCallback;

private:
#if defined(HAVE_UNORDERED_MAP)
  typedef tr1::unordered_map<string, FileNameDatabaseHeader::FndbOffset> StringMap;
#else
  typedef map<string, FileNameDatabaseHeader::FndbOffset> StringMap;
#endif

private:
  StringMap stringMap;

private:
  bool enableStringPooling;

private:
  bool storeFileNameInfo;

private:
  TraceStream * traceStream;
};

/* _________________________________________________________________________

   FndbManager::GetMemPointer
   _________________________________________________________________________ */

void *
FndbManager::GetMemPointer ()
{
  return (&byteArray[0]);
}

/* _________________________________________________________________________
     
   FndbManager::GetMemTop
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FndbManager::GetMemTop ()
  const
{
  return (static_cast<FileNameDatabaseHeader::FndbOffset>(byteArray.size()));
}

/* _________________________________________________________________________
     
   FndbManager::SetMem
   _________________________________________________________________________ */

void
FndbManager::SetMem (/*[in]*/ FileNameDatabaseHeader::FndbOffset	fo,
		     /*[in]*/ const void *				pData,
		     /*[in]*/ size_t					size)
{
  MIKTEX_ASSERT (fo + size <= GetMemTop());
  const unsigned char * pbData =
    reinterpret_cast<const unsigned char *>(pData);
  copy (&pbData[0], &pbData[size], byteArray.begin() + fo);
}

/* _________________________________________________________________________
     
   FndbManager::SetMem
   _________________________________________________________________________ */

void
FndbManager::SetMem (/*[in]*/ FileNameDatabaseHeader::FndbOffset	fo,
		     /*[in]*/ unsigned long				data)
{
  SetMem (fo, &data, sizeof(data));
}

/* _________________________________________________________________________
   
   FndbManager::ReserveMem
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FndbManager::ReserveMem (/*[in]*/ size_t	size)
{
  FileNameDatabaseHeader::FndbOffset ret = GetMemTop();
  byteArray.reserve (byteArray.size() + size);
  for (size_t i = 0; i < size; ++ i)
    {
      byteArray.push_back (null_byte);
    }
  return (ret);
}

/* _________________________________________________________________________
     
   FndbManager::FastPushBack
   _________________________________________________________________________ */

void
FndbManager::FastPushBack (/*[in]*/ unsigned char b)
{
  byteArray.push_back (b);
}

/* _________________________________________________________________________
     
   FndbManager::PushBack
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FndbManager::PushBack (/*[in]*/ unsigned char b)
{
  FileNameDatabaseHeader::FndbOffset ret = GetMemTop();
  FastPushBack (b);
  return (ret);
}

/* _________________________________________________________________________
     
   FndbManager::PushBack
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FndbManager::PushBack (/*[in]*/ unsigned long data)
{
  AlignMem (AlignmentDoubleWord);
  FileNameDatabaseHeader::FndbOffset ret = GetMemTop();
  const unsigned char * p = reinterpret_cast<const unsigned char *>(&data);
  FastPushBack (p[0]);
  FastPushBack (p[1]);
  FastPushBack (p[2]);
  FastPushBack (p[3]);
  return (ret);
}

/* _________________________________________________________________________
     
   FndbManager::PushBack
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FndbManager::PushBack (/*[in]*/ const void *	pData,
		       /*[in]*/ size_t		size)
{
  FileNameDatabaseHeader::FndbOffset ret = GetMemTop();
  const unsigned char * pbData =
    reinterpret_cast<const unsigned char *>(pData);
  for (size_t i = 0; i < size; ++ i)
    {
      FastPushBack (pbData[i]);
    }
  return (ret);
}

/* _________________________________________________________________________
   
   FndbManager::PushBack
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FndbManager::PushBack (/*[in]*/ const char * lpsz)
{
  if (enableStringPooling)
    {
      StringMap::const_iterator it = stringMap.find(lpsz);
      if (it != stringMap.end())
	{
	  return (it->second);
	}
    }
  FileNameDatabaseHeader::FndbOffset ret = GetMemTop();
  MIKTEX_ASSERT (lpsz != 0);
  PushBack (lpsz, strlen(lpsz));
  FastPushBack (null_byte);
  if (enableStringPooling)
    {
      stringMap[lpsz] = ret;
    }
  return (ret);
}
  
/* _________________________________________________________________________
   
   FndbManager::AlignMem
   _________________________________________________________________________ */

void
FndbManager::AlignMem (/*[in]*/ unsigned long align)
{
  FileNameDatabaseHeader::FndbOffset foTop = GetMemTop();
  while (((foTop ++) % align) > 0)
    {
      FastPushBack (null_byte);
    }
}

/* _________________________________________________________________________
     
   FndbManager::GetIgnorableFiles
   _________________________________________________________________________ */

void
FndbManager::GetIgnorableFiles (/*[in]*/ const char *	lpszPath,
				/*[out]*/ vector<string> & filesToBeIgnored)
{
  PathName ignoreFile (lpszPath, FN_MIKTEXIGNORE, 0);
  if (! File::Exists(ignoreFile))
    {
      return;
    }
  StreamReader reader (ignoreFile);
  filesToBeIgnored.reserve (10);
  string line;
  while (reader.ReadLine(line))
    {
      filesToBeIgnored.push_back (line);
    }
  sort (filesToBeIgnored.begin(),
	filesToBeIgnored.end(),
	StringComparerIgnoringCase());
}

/* _________________________________________________________________________
   
   FndbManager::ReadDirectory
   _________________________________________________________________________ */

void
FndbManager::ReadDirectory (/*[in]*/ const char *		lpszPath,
			    /*[out]*/ vector<string> &	subDirectoryNames,
			    /*[out]*/ vector<FILENAMEINFO> &	fileNames)
{
  if (! Directory::Exists(lpszPath))
    {
      traceStream->WriteFormattedLine ("core",
				       T_("the directory %s does not exist"),
				       Q_(lpszPath));
      return;
    }
  vector<string> filesToBeIgnored;
  GetIgnorableFiles (lpszPath, filesToBeIgnored);
  auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(lpszPath));
  DirectoryEntry entry;
  while (pLister->GetNext(entry))
    {
      if (binary_search(filesToBeIgnored.begin(),
			filesToBeIgnored.end(),
			entry.name.c_str(),
			StringComparerIgnoringCase()))
	{
	  continue;
	}
      if (entry.isDirectory)
	{
	  subDirectoryNames.push_back (entry.name.c_str());
	}
      else
	{
	  FILENAMEINFO filenameinfo;
	  filenameinfo.FileName = entry.name;
	  fileNames.push_back (filenameinfo);
	}
    }
  pLister->Close ();
}

/* _________________________________________________________________________
     
   FndbManager::ProcessFolder
   _________________________________________________________________________ */
  
FileNameDatabaseHeader::FndbOffset
FndbManager::ProcessFolder
(/*[in]*/ FileNameDatabaseHeader::FndbOffset	foParent,
 /*[in]*/ const char *				lpszParentPath,
 /*[in]*/ const char *				lpszFolderName,
 /*[in]*/ FileNameDatabaseHeader::FndbOffset	foFolderName)
{
  const size_t cReservedEntries = 0;
  
  if (currentLevel > deepestLevel)
    {
      deepestLevel = currentLevel;
    }

  vector<string> subDirectoryNames;
  subDirectoryNames.reserve (40);

  vector<FILENAMEINFO> fileNames;
  fileNames.reserve (100);

  bool done = false;

  PathName path (lpszParentPath, lpszFolderName);

  path.MakeAbsolute ();

  if (pCallback != 0)
    {
      if (! pCallback->OnProgress(currentLevel, path.Get()))
	{
	  throw OperationCancelledException ();
	}
      char * lpszSubDirNames = 0;
      char * lpszFileNames = 0;
      char * lpszFileNameInfos = 0;
      done =
	pCallback->ReadDirectory(path.Get(),
				 &lpszSubDirNames,
				 &lpszFileNames,
				 &lpszFileNameInfos);
      if (done)
	{
	  AutoMemoryPointer xxx (lpszSubDirNames);
	  AutoMemoryPointer xxy (lpszFileNames);
	  AutoMemoryPointer xxz (lpszFileNameInfos);
	  const char * lpsz = lpszSubDirNames;
	  while (*lpsz != 0)
	    {
	      subDirectoryNames.push_back (lpsz);
	      lpsz += strlen(lpsz) + 1;
	    }
	  lpsz = lpszFileNames;
	  const char * lpsz2 = lpszFileNameInfos;
	  while (*lpsz != 0)
	    {
	      FILENAMEINFO filenameinfo;
	      filenameinfo.FileName = lpsz;
	      lpsz += strlen(lpsz) + 1;
	      if (lpsz2 != 0)
		{
		  filenameinfo.Info = lpsz2;
		  lpsz2 += strlen(lpsz2) + 1;
		}
	      fileNames.push_back (filenameinfo);
	    }
	}
    }

  if (! done)
    {
      ReadDirectory (path.Get(), subDirectoryNames, fileNames);
    }

  numDirectories += static_cast<unsigned long>(subDirectoryNames.size());
  numFiles += static_cast<unsigned long>(fileNames.size());

  sort (subDirectoryNames.begin(),
	subDirectoryNames.end(),
	StringComparerIgnoringCase());
  sort (fileNames.begin(), fileNames.end(), COMPAREFILENAMEINFO());
  
  // store all names; build offset table
  vector<FileNameDatabaseHeader::FndbOffset> vecfndboff;
  vecfndboff.reserve ((storeFileNameInfo ? 2 : 1) * fileNames.size()
		      + 2 * subDirectoryNames.size()
		      + cReservedEntries);
  vector<FILENAMEINFO>::iterator it;
  vector<string>::iterator it2;
  for (it = fileNames.begin(); it != fileNames.end(); ++ it)
    {
      vecfndboff.push_back (PushBack((*it).FileName.c_str()));
    }
  for (it2 = subDirectoryNames.begin(); it2 != subDirectoryNames.end(); ++ it2)
    {
      vecfndboff.push_back (PushBack((*it2).c_str()));
    }
  for (it2 = subDirectoryNames.begin(); it2 != subDirectoryNames.end(); ++ it2)
    {
      vecfndboff.push_back (null_byte);
    }
  if (storeFileNameInfo)
    {
      for (it = fileNames.begin(); it != fileNames.end(); ++ it)
	{
	  vecfndboff.push_back (PushBack((*it).Info.c_str()));
	}
    }
  vecfndboff.insert (vecfndboff.end(), cReservedEntries, 0);
  
  // store directory data (excluding offsets)
  FileNameDatabaseDirectory dirdata;
  dirdata.Init ();
  dirdata.foName = foFolderName;
  dirdata.foParent = foParent;
  dirdata.numSubDirs = static_cast<unsigned long>(subDirectoryNames.size());
  dirdata.numFiles = static_cast<unsigned long>(fileNames.size());
  dirdata.capacity = static_cast<unsigned long>(vecfndboff.size());
  AlignMem (AlignmentDirectory);
  FileNameDatabaseHeader::FndbOffset foThis =
    PushBack(&dirdata, offsetof(FileNameDatabaseDirectory, table));

  if (vecfndboff.size() == 0)
    {
      return (foThis);
    }

  // reserve memory for offset table
  FileNameDatabaseHeader::FndbOffset foOffsetTable =
    ReserveMem(vecfndboff.size() * sizeof(FileNameDatabaseHeader::FndbOffset));
  
  // recurse into sub-directories and remember offsets
  PathName pathFolder (lpszParentPath, lpszFolderName, 0);
  size_t i = 0;
  ++ currentLevel;
  for (it2 = subDirectoryNames.begin();
       it2 != subDirectoryNames.end();
       ++ it2, ++ i)
    {
      vecfndboff[dirdata.numFiles + dirdata.numSubDirs + i]
	// <recursivecall>
	= ProcessFolder(foThis,
			pathFolder.Get(),
			it2->c_str(),
			vecfndboff[dirdata.numFiles + i]);
	// </recursivecall>
    }
  -- currentLevel;
  
  // store offset table
  SetMem (foOffsetTable,
	  &vecfndboff[0],
	  vecfndboff.size() * sizeof(FileNameDatabaseHeader::FndbOffset));
  
  return (foThis);
}
  
/* _________________________________________________________________________
     
   FndbManager::Create
   _________________________________________________________________________ */

bool
FndbManager::Create (/*[in]*/ const char *		lpszFndbPath,
		     /*[in]*/ const char *		lpszRootPath,
		     /*[in]*/ ICreateFndbCallback *	pCallback,
		     /*[in]*/ bool			enableStringPooling,
		     /*[in]*/ bool			storeFileNameInfo)
{
  traceStream->WriteFormattedLine ("core",
				   T_("creating fndb file %s..."),
				   Q_(lpszFndbPath));
  unsigned rootIdx = SessionImpl::GetSession()->DeriveTEXMFRoot(lpszRootPath);
  this->enableStringPooling = enableStringPooling;
  this->storeFileNameInfo = storeFileNameInfo;
  byteArray.reserve (2 * 1024 * 1024);
  try
    {
      ReserveMem (sizeof(FileNameDatabaseHeader));
      FileNameDatabaseHeader fndb;
      fndb.Init ();
      if (pCallback == 0 && FileIsOnROMedia(lpszRootPath))
	{
	  fndb.flags |= FileNameDatabaseHeader::FndbFlags::Frozen;
	}
      if (storeFileNameInfo)
	{
	  fndb.flags |= FileNameDatabaseHeader::FndbFlags::FileNameInfo;
	}
      AlignMem ();
      fndb.foPath = PushBack(lpszRootPath);
      numDirectories = 0;
      numFiles = 0;
      deepestLevel = 0;
      currentLevel = 0;
      this->pCallback = pCallback;
#if 0				// <new>this will break prev MiKTeX</new>
      fndb.foTopDir = ProcessFolder(0, lpszRootPath, CURRENT_DIRECTORY, 0);
#else
      fndb.foTopDir =
	ProcessFolder(0,
		      lpszRootPath,
		      CURRENT_DIRECTORY,
		      fndb.foPath);
#endif
      fndb.numDirs = numDirectories;
      fndb.numFiles = numFiles;
      fndb.depth = deepestLevel;
      fndb.timeStamp = static_cast<unsigned long>(time(0)); // <sixtyfourbit/>
      fndb.size = GetMemTop();
      if ((fndb.flags & FileNameDatabaseHeader::FndbFlags::Frozen) == 0)
	{
	  size_t n = ((GetMemTop() + FNDB_EXTRA + 1) / FNDB_GRAN * FNDB_GRAN) - GetMemTop();
	  ReserveMem (n);
	}
      AlignMem (FNDB_PAGESIZE);
      SetMem (0, &fndb, sizeof(fndb));
      // <fixme>
      bool unloaded = false;
      for (size_t i = 0; ! unloaded && i < 100; ++ i)
	{
	  unloaded =
	    SessionImpl::GetSession()->UnloadFilenameDatabaseInternal(rootIdx,
								      true);
	  if (! unloaded)
	    {
	      traceStream->WriteFormattedLine ("core", "Sleep(1)");
	      Thread::Sleep (1);
	    }
	}
      if (! unloaded)
	{
	  TraceError (T_("fndb cannot be unloaded"));
	}
      // </fixme>
      PathName directory = PathName(lpszFndbPath).RemoveFileSpec();
      if (! Directory::Exists(directory))
	{
	  Directory::Create (directory);
	}
      FileStream streamFndb (File::Open(lpszFndbPath,
					FileMode::Create,
					FileAccess::Write,
					false));
      streamFndb.Write (GetMemPointer(), GetMemTop());
      traceStream->WriteFormattedLine ("core",
				       T_("fndb creation completed"));
      time_t now = time(0);
      string nowStr = NUMTOSTR(now);
      if (SessionImpl::GetSession()->IsAdminMode())
      {
	SessionImpl::GetSession()->SetConfigValue (MIKTEX_REGKEY_CORE, MIKTEX_REGVAL_LAST_ADMIN_MAINTENANCE, nowStr.c_str());
      }
      else
      {
	SessionImpl::GetSession()->SetConfigValue (MIKTEX_REGKEY_CORE, MIKTEX_REGVAL_LAST_USER_MAINTENANCE, nowStr.c_str());
      }
      return (true);
    }
  catch (const OperationCancelledException &)
    {
      traceStream->WriteFormattedLine ("core",
				       T_("fndb creation cancelled"));
      return (false);
    }
}

/* _________________________________________________________________________
   
   Fndb::Create
   _________________________________________________________________________ */

bool
Fndb::Create (/*[in]*/ const char *	lpszFndbPath,
	      /*[in]*/ const char *	lpszRootPath,
	      /*[in]*/ ICreateFndbCallback *	pCallback)
{
  MIKTEX_ASSERT_STRING (lpszFndbPath);
  MIKTEX_ASSERT_STRING (lpszRootPath);
  return (Fndb::Create(lpszFndbPath,
		       lpszRootPath,
		       pCallback,
		       false,
		       false));
}

/* _________________________________________________________________________
   
   Fndb::Create
   _________________________________________________________________________ */

bool
Fndb::Create (/*[in]*/ const char *		lpszFndbPath,
	      /*[in]*/ const char *		lpszRootPath,
	      /*[in]*/ ICreateFndbCallback *	pCallback,
	      /*[in]*/ bool			enableStringPooling,
	      /*[in]*/ bool			storeFileNameInfo)
{
  MIKTEX_ASSERT_STRING (lpszFndbPath);
  MIKTEX_ASSERT_STRING (lpszRootPath);

  FndbManager fndbmngr;

  if (! fndbmngr.Create(lpszFndbPath,
			lpszRootPath,
			pCallback,
			enableStringPooling,
			storeFileNameInfo))
    {
      return (false);
    }

#if defined(MIKTEX_WINDOWS)
  ReportMiKTeXEvent (EVENTLOG_INFORMATION_TYPE,
		     MIKTEX_EVENT_FNDB_CREATED,
		     lpszFndbPath,
		     lpszRootPath,
		     0);
#endif

  return (true);
}

/* _________________________________________________________________________
   
   Fndb::Refresh
   _________________________________________________________________________ */

bool
Fndb::Refresh (/*[in]*/ const PathName &	path,
	       /*[in]*/ ICreateFndbCallback *	pCallback)
{
  unsigned root = SessionImpl::GetSession()->DeriveTEXMFRoot(path);

  PathName pathFndbPath =
    SessionImpl::GetSession()->GetFilenameDatabasePathName(root);

  return (Fndb::Create(pathFndbPath.Get(),
		       SessionImpl::GetSession()->GetRootDirectory(root).Get(),
		       pCallback));
}

/* _________________________________________________________________________
   
   Fndb::Refresh
   _________________________________________________________________________ */

bool
Fndb::Refresh (/*[in]*/ ICreateFndbCallback *	pCallback)
{
  unsigned n = SessionImpl::GetSession()->GetNumberOfTEXMFRoots();
  for (unsigned ord = 0; ord < n; ++ ord)
  {
    if ((SessionImpl::GetSession()->IsAdminMode()
	 && ! SessionImpl::GetSession()->IsCommonRootDirectory(ord))
        || (! SessionImpl::GetSession()->IsAdminMode()
	    && SessionImpl::GetSession()->IsCommonRootDirectory(ord)))
    {
      continue;
    }
    PathName rootDirectory =  SessionImpl::GetSession()->GetRootDirectory(ord);
    PathName pathFndbPath =
      SessionImpl::GetSession()->GetFilenameDatabasePathName(ord);
    if (! Fndb::Create(pathFndbPath, rootDirectory, pCallback))
    {
      return (false);
    }
  }
  return (true);
}
