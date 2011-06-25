/* fnamedb.cpp: file name database

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
     
   FileNameDatabase::FileNameDatabase
   _________________________________________________________________________ */

FileNameDatabase::FileNameDatabase ()
  : isInvariable (false),
    timeStamp (0),
    foEnd (0),
    lockCount (0),
    pHeader (0),
    mmap (MemoryMappedFile::Create()),
    traceStream (TraceStream::Open(MIKTEX_TRACE_FNDB))
{
}

/* _________________________________________________________________________
     
   FileNameDatabase::Finalize
   _________________________________________________________________________ */

void
FileNameDatabase::Finalize ()
{
  if (traceStream.get() != 0)
    {
      traceStream->WriteFormattedLine
	("core",
	 T_("unloading fndb %p"),
	 this);
    }
  if (mmap->GetPtr() != 0)
    {
      mmap->Close ();
    }
  if (traceStream.get() != 0)
    {
      traceStream->Close ();
    }
}

/* _________________________________________________________________________
     
   FileNameDatabase::Release
   _________________________________________________________________________ */

long
FileNameDatabase::Release ()
{
  long n;
#if defined(MIKTEX_WINDOWS)
  n = InterlockedDecrement(&lockCount);
#else
  MIKTEX_LOCK(this)
    {
      lockCount -= 1;
      n = lockCount;
    }
  MIKTEX_UNLOCK();
#endif
  if (n == 0)
    {
      Finalize ();
      delete this;
    }
  return (n);
}

/* _________________________________________________________________________
     
   FileNameDatabase::AddRef
   _________________________________________________________________________ */

long
FileNameDatabase::AddRef ()
{
  long n;
#if defined(MIKTEX_WINDOWS)
  n = InterlockedIncrement(&lockCount);
#else
  MIKTEX_LOCK(this)
    {
      lockCount += 1;
      n = lockCount;
    }
  MIKTEX_UNLOCK();
#endif
  return (n);
}

/* _________________________________________________________________________
     
   FileNameDatabase::GetRefCount
   _________________________________________________________________________ */

long
FileNameDatabase::GetRefCount ()
  const
{
  return (lockCount);
}

/* _________________________________________________________________________
     
   FileNameDatabase::~FileNameDatabase
   _________________________________________________________________________ */

FileNameDatabase::~FileNameDatabase ()
{
  try
    {
      Finalize ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________
     
   FileNameDatabase::HasFileNameInfo
   _________________________________________________________________________ */

bool
FileNameDatabase::HasFileNameInfo ()
  const
{
  return ((pHeader->flags & FileNameDatabaseHeader::FndbFlags::FileNameInfo)
	  != 0);
}

/* _________________________________________________________________________
     
   FileNameDatabase::GetPointer
   _________________________________________________________________________ */

const void *
FileNameDatabase::GetPointer (/*[in]*/ FileNameDatabaseHeader::FndbOffset fo)
  const
{
  MIKTEX_ASSERT (fo < foEnd);
  return (fo == 0
	  ? 0
	  : reinterpret_cast<unsigned char *>(pHeader) + fo);
}

/* _________________________________________________________________________
     
   FileNameDatabase::GetPointer
   _________________________________________________________________________ */

void *
FileNameDatabase::GetPointer (/*[in]*/ FileNameDatabaseHeader::FndbOffset fo)
{
  MIKTEX_ASSERT (fo < foEnd);
  return (fo == 0
	  ? 0
	  : reinterpret_cast<unsigned char *>(pHeader) + fo);
}

/* _________________________________________________________________________
     
   FileNameDatabase::GetOffset
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FileNameDatabase::GetOffset (/*[in]*/ const void * p)
  const
{
  ptrdiff_t d
    = (reinterpret_cast<const unsigned char *>(p)
       - reinterpret_cast<const unsigned char *>(pHeader));
  MIKTEX_ASSERT (d >= 0);
  FileNameDatabaseHeader::FndbOffset fo
    = static_cast<FileNameDatabaseHeader::FndbOffset>(d);
  MIKTEX_ASSERT (fo < foEnd);
  return (fo);
}

/* _________________________________________________________________________
     
   FileNameDatabase::GetDirectoryAt
   _________________________________________________________________________ */

const FileNameDatabaseDirectory *
FileNameDatabase::GetDirectoryAt
(/*[in]*/ FileNameDatabaseHeader::FndbOffset fo)
  const
{
  if (fo == 0)
    {
      return (0);
    }
  MIKTEX_ASSERT (fo >= sizeof(FileNameDatabaseHeader) && fo < foEnd);
  return (reinterpret_cast<const FileNameDatabaseDirectory*>
	  (GetPointer(fo)));
}
  
/* _________________________________________________________________________
     
   FileNameDatabase::GetDirectoryAt
   _________________________________________________________________________ */

FileNameDatabaseDirectory *
FileNameDatabase::GetDirectoryAt
(/*[in]*/ FileNameDatabaseHeader::FndbOffset fo)
{
  if (fo == 0)
    {
      return (0);
    }
  MIKTEX_ASSERT (fo >= sizeof(FileNameDatabaseHeader) && fo < foEnd);
  return (reinterpret_cast<FileNameDatabaseDirectory*>(GetPointer(fo)));
}

/* _________________________________________________________________________
     
   FileNameDatabase::GetTopDirectory2
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FileNameDatabase::GetTopDirectory2 ()
  const
{
  return (pHeader->foTopDir);
}

/* _________________________________________________________________________
     
   FileNameDatabase::GetTopDirectory
   _________________________________________________________________________ */

const FileNameDatabaseDirectory *
FileNameDatabase::GetTopDirectory ()
  const
{
  return (GetDirectoryAt(GetTopDirectory2()));
}

/* _________________________________________________________________________
     
   FileNameDatabase::GetTopDirectory
   _________________________________________________________________________ */

FileNameDatabaseDirectory *
FileNameDatabase::GetTopDirectory ()
{
  return (GetDirectoryAt(GetTopDirectory2()));
}

/* _________________________________________________________________________
     
   FileNameDatabase::GetString
   _________________________________________________________________________ */

const char *
FileNameDatabase::GetString (/*[in]*/ FileNameDatabaseHeader::FndbOffset fo)
  const
{
  MIKTEX_ASSERT (fo >= sizeof(FileNameDatabaseHeader) && fo < foEnd);
  MIKTEX_ASSERT_STRING (reinterpret_cast<const char *>(GetPointer(fo)));
  return (reinterpret_cast<const char *>(GetPointer(fo)));
}

/* _________________________________________________________________________
     
   FileNameDatabase::IsDirty
   _________________________________________________________________________ */

bool
FileNameDatabase::IsDirty ()
  const
{
  MIKTEX_ASSERT (pHeader != 0);
  return (pHeader->timeStamp != timeStamp);
}

/* _________________________________________________________________________
     
   FileNameDatabase::IsInvariable
   _________________________________________________________________________ */

bool
FileNameDatabase::IsInvariable ()
  const
{
  return (isInvariable);
}

/* _________________________________________________________________________
     
   FileNameDatabase::OpenFileNameDatabase
   _________________________________________________________________________ */

void
FileNameDatabase::OpenFileNameDatabase (/*[in]*/ const char *	lpszFndbPath,
					/*[in]*/ bool		readWrite)
{
#if defined(MIKTEX_WINDOWS)
  // check file attributes
  FileAttributes attributes = File::GetAttributes(lpszFndbPath);
  if ((attributes & FileAttributes::ReadOnly) != 0)
    {
      traceStream->WriteFormattedLine
	("core",
	 T_("file name database file is readonly"),
	 0);
      readWrite = false;
    }
#endif
  
  mmap->Open (lpszFndbPath, readWrite);

  if (mmap->GetSize() < sizeof(*pHeader))
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::OpenFileNameDatabase",
			  T_("Not a file name database file (wrong size)."),
			  lpszFndbPath);
    }
  
  pHeader = reinterpret_cast<FileNameDatabaseHeader*>(mmap->GetPtr());

  foEnd = static_cast<FileNameDatabaseHeader::FndbOffset>(mmap->GetSize());
  
  // check signature
  if (pHeader->signature != FileNameDatabaseHeader::Signature)
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::OpenFileNameDatabase",
		  T_("Not a file name database file (wrong signature)."),
			  lpszFndbPath);
    }
  
  // check version number
  if (pHeader->version != FileNameDatabaseHeader::Version)
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::OpenFileNameDatabase",
			  T_("Unknown file name database file."),
			  lpszFndbPath);
    }
  
  if (! readWrite
      || (pHeader->flags & FileNameDatabaseHeader::FndbFlags::Frozen) != 0)
    {
      isInvariable = true;
      readWrite = false;
    }
  
  if (! isInvariable)
    {
      // grow file-mapping object if necessary
      if (pHeader->size + 131072 > foEnd)
	{
	  size_t newSize =
	    ((pHeader->size + FNDB_EXTRA + 1) / FNDB_GRAN) * FNDB_GRAN;
	  traceStream->WriteFormattedLine
	    ("core",
	     T_("enlarging fndb file %s (%u -> %u)..."),
	     Q_(lpszFndbPath),
	     static_cast<unsigned>(foEnd),
	     static_cast<unsigned>(newSize));
	  pHeader =
	    reinterpret_cast<FileNameDatabaseHeader*>(mmap->Resize(newSize));
#if defined(MIKTEX_WINDOWS)
	  ReportMiKTeXEvent (EVENTLOG_INFORMATION_TYPE,
			     MIKTEX_EVENT_FNDB_ENLARGED,
			     mmap->GetName(),
			     NUMTOSTR(foEnd),
			     NUMTOSTR(static_cast<unsigned>(newSize)),
			     0);
#endif
	  foEnd = static_cast<FileNameDatabaseHeader::FndbOffset>(newSize);
	}
    }
  
  timeStamp = pHeader->timeStamp;
}

/* _________________________________________________________________________
     
   FileNameDatabase::Initialize
   _________________________________________________________________________ */

void
FileNameDatabase::Initialize (/*[in]*/ const char *	lpszFndbPath,
			      /*[in]*/ const char *	lpszRoot,
			      /*[in]*/ bool		readWrite)
{
  rootDirectory = lpszRoot;
  isInvariable = ! readWrite;

  OpenFileNameDatabase (lpszFndbPath, readWrite);

  lockCount = 1;
  
  if ((pHeader->flags & FileNameDatabaseHeader::FndbFlags::Frozen) != 0)
    {
      isInvariable = true;
    }

  ReadFileNames ();
}

/* _________________________________________________________________________
     
   FileNameDatabase::FindSubDirectory2
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FileNameDatabase::FindSubDirectory2
(/*[in]*/ const FileNameDatabaseDirectory *	pDir,
 /*[in]*/ const char *				lpszRelPath)
  const
{
  MIKTEX_ASSERT (pDir != 0);
  FileNameDatabaseHeader::FndbOffset fo = 0;
  for (PathNameParser dirName (lpszRelPath);
       dirName.GetCurrent() != 0;
       ++ dirName)
    {
      if (PathName::Compare(dirName.GetCurrent(), CURRENT_DIRECTORY) == 0)
	{
	  fo = GetOffset(pDir);
	  continue;
	}
      bool matching = false;
      U32 subidx = 0;
      while (! matching && pDir != 0)
	{
	  U32 lo = 0;
	  U32 hi = pDir->numSubDirs;
	  while (! matching && hi > lo)
	    {
	      subidx = lo + (hi - lo) / 2;
	      FileNameDatabaseHeader::FndbOffset foSubDirName =
		pDir->GetSubDirName(subidx);
	      const char * lpszSubdirName = GetString(foSubDirName);
	      int cmp =
		PathName::Compare(lpszSubdirName, dirName.GetCurrent());
	      if (cmp > 0)
		{
		  hi = subidx;
		}
	      else if (cmp < 0)
		{
		  lo = subidx + 1;
		}
	      else
		{
		  matching = true;
		}
	    }
	  if (! matching)
	    {
	      pDir = GetDirectoryAt(pDir->foExtension);
	    }
	}
      if (! matching)
	{
	  return (0);
	}
      fo = pDir->GetSubDir(subidx);
      pDir = GetDirectoryAt(fo);
    }
  return (fo);
}

/* _________________________________________________________________________
     
   FileNameDatabase::FindSubDirectory
   _________________________________________________________________________ */

const FileNameDatabaseDirectory *
FileNameDatabase::FindSubDirectory
(/*[in]*/ const FileNameDatabaseDirectory *	pDir,
 /*[in]*/ const char *				lpszRelPath)
  const
{
  FileNameDatabaseHeader::FndbOffset fo = FindSubDirectory2(pDir, lpszRelPath);
  return (GetDirectoryAt(fo));
}

/* _________________________________________________________________________

   FileNameDatabase::SearchFileName
   _________________________________________________________________________ */

const FileNameDatabaseDirectory *
FileNameDatabase::SearchFileName
(/*[in]*/ const FileNameDatabaseDirectory *	pDir,
 /*[in]*/ const char *				lpszFileName,
 /*[out]*/ U32 &				index)
  const
{
  for ( ; pDir != 0; pDir = GetDirectoryAt(pDir->foExtension))
    {
      U32 lo = 0;
      U32 hi = pDir->numFiles;
      while (hi > lo)
	{
	  index = lo + (hi - lo) / 2;
	  const char * lpszCandidate =
	    GetString(pDir->GetFileName(index));
	  int cmp = PathName::Compare(lpszCandidate, lpszFileName);
	  if (cmp > 0)
	    {
	      hi = index;
	    }
	  else if (cmp < 0)
	    {
	      lo = index + 1;
	    }
	  else			// cmp == 0
	    {
	      return (pDir);
	    }
	}
    }
  return (0);
}
  
/* _________________________________________________________________________

   FileNameDatabase::MakePathName
   _________________________________________________________________________ */

void
FileNameDatabase::MakePathName
(/*[in]*/ const FileNameDatabaseDirectory *	pDir,
 /*[out]*/ PathName &				path)
  const
{
  if (pDir == 0 || pDir->foParent == 0)
    {
      return;
    }
  // <recursivecall>
  MakePathName (GetDirectoryAt(pDir->foParent), path);
  // </recursivecall>
  path += GetString(pDir->foName);
}

/* _________________________________________________________________________
     
   FileNameDatabase::ExtendDirectory
   _________________________________________________________________________ */

#define ROUND2(n, pow2) ((n + pow2 - 1) & ~(pow2 - 1))

FileNameDatabaseDirectory *
FileNameDatabase::ExtendDirectory (/*[out]*/ FileNameDatabaseDirectory * pDir)
{
  U32 neededSlots = 10;		// make room for 10 files
  if (HasFileNameInfo())
    {
      neededSlots *= 2;
    }
  neededSlots += 2;		// and 1 sub-directory
  U32 neededBytes = offsetof(FileNameDatabaseDirectory, table);
  neededBytes += neededSlots * sizeof(FileNameDatabaseHeader::FndbOffset);
  FileNameDatabaseHeader::FndbOffset foExtension = ROUND2(pHeader->size, 16);
  if (foExtension + neededBytes > foEnd)
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::ExtendDirectory",
			  T_("File name database capacity exceeded."),
			  0);
    }
  pHeader->size = foExtension + neededBytes;
  FileNameDatabaseDirectory * pDirExt
    = reinterpret_cast<FileNameDatabaseDirectory *>(GetPointer(foExtension));
  pDirExt->Init ();
  pDirExt->foName = pDir->foName;
  pDirExt->foParent = pDir->foParent;
  pDirExt->numFiles = 0;
  pDirExt->numSubDirs = 0;
  pDirExt->foExtension = 0;
  pDirExt->capacity = neededSlots;
  memset (pDirExt->table,
	  0,
	  sizeof(FileNameDatabaseHeader::FndbOffset) * neededSlots);
  pDir->foExtension = foExtension;
  return (pDirExt);
}

/* _________________________________________________________________________

   FileNameDatabase::CreateString
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FileNameDatabase::CreateString (/*[in]*/ const char * lpszName)
{
  FileNameDatabaseHeader::FndbOffset foName;
  size_t neededBytes = strlen(lpszName) + 1;
  foName = ROUND2(pHeader->size, 2);
  if (foName + neededBytes > foEnd)
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::CreateString",
			  T_("File name database capacity exceeded."),
			  0);
    }
  memcpy (GetPointer(foName), lpszName, neededBytes);
  pHeader->size = foName + static_cast<U32>(neededBytes);
  return (foName);
}

/* _________________________________________________________________________
     
   FileNameDatabase::FindLowerBound
   _________________________________________________________________________ */

U32
FileNameDatabase::FindLowerBound
(/*[in]*/ const FileNameDatabaseHeader::FndbOffset *	pBegin,
 /*[in]*/ U32						count,
 /*[in]*/ const char *					lpszName,
 /*[out]*/ bool &					isDuplicate)
  const
{
  const FileNameDatabaseHeader::FndbOffset * pEnd = &pBegin[count];
  const FileNameDatabaseHeader::FndbOffset * iter = pBegin;
  while (iter != pEnd && PathName::Compare(lpszName, GetString(*iter)) > 0)
    {
      ++ iter;
    }
  isDuplicate
    = (iter != pEnd && PathName::Compare(lpszName, GetString(*iter)) == 0);
  return (static_cast<U32>(iter - pBegin));
}

/* _________________________________________________________________________

   FileNameDatabase::InsertFileName
   _________________________________________________________________________ */

void
FileNameDatabase::InsertFileName
(/*[in]*/ FileNameDatabaseDirectory *			pDir,
 /*[in]*/ FileNameDatabaseHeader::FndbOffset		foFileName,
 /*[in]*/ FileNameDatabaseHeader::FndbOffset		foFileNameInfo)
{
  MIKTEX_ASSERT (pDir->capacity
	  >= (pDir->SizeOfTable(HasFileNameInfo())
	      + (HasFileNameInfo() ? 2 : 1)));
  bool isDuplicate;
  U32 idx =
    FindLowerBound(&pDir->table[0],
		   pDir->numFiles,
		   GetString(foFileName),
		   isDuplicate);
  if (isDuplicate)
    {
      return;
    }
  pDir->TableInsert (idx, foFileName);
  if (HasFileNameInfo())
    {
      pDir->TableInsert (pDir->numFiles + 1 + 2 * pDir->numSubDirs + idx,
			 foFileNameInfo);
    }
  pDir->numFiles += 1;
  pHeader->numFiles += 1;
  pHeader->timeStamp = static_cast<U32>(time(0)); // <sixtyfourbit/>
}

/* _________________________________________________________________________
     
   FileNameDatabase::InsertDirectory
   _________________________________________________________________________ */

void
FileNameDatabase::InsertDirectory
(/*[out]*/ FileNameDatabaseDirectory *		pDir,
 /*[in]*/  const FileNameDatabaseDirectory *	pDirSub)
{
  MIKTEX_ASSERT (pDir->capacity >= pDir->SizeOfTable(HasFileNameInfo()) + 2);
  bool isDuplicate;
  U32 idx =
    FindLowerBound(&pDir->table[pDir->numFiles],
		   pDir->numSubDirs,
		   GetString(pDirSub->foName),
		   isDuplicate);
  if (isDuplicate)
    {
      return;
    }
  pDir->TableInsert (pDir->numFiles + idx, pDirSub->foName);
  pDir->TableInsert (pDir->numFiles + pDir->numSubDirs + 1 + idx,
		     GetOffset(pDirSub));
  pDir->numSubDirs += 1;
  pHeader->numDirs += 1;
}

/* _________________________________________________________________________

   FileNameDatabase::CreateDirectory
   _________________________________________________________________________ */

FileNameDatabaseDirectory *
FileNameDatabase::CreateDirectory
(/*[out]*/ FileNameDatabaseDirectory *	pDir,
 /*[in]*/ const char *			lpszName)
{
  while (pDir->capacity < pDir->SizeOfTable(HasFileNameInfo()) + 2)
    {
      if (pDir->foExtension != 0)
	{
	  pDir = GetDirectoryAt(pDir->foExtension);
	}
      else
	{
	  pDir = ExtendDirectory(pDir);
	}
      MIKTEX_ASSERT (pDir != 0);
    }

  FileNameDatabaseHeader::FndbOffset foName = CreateString(lpszName);

  MIKTEX_ASSERT (foName != 0);

  U32 neededSlots = 10;		// make room for 10 files

  if (HasFileNameInfo())
    {
      neededSlots *= 2;
    }

  neededSlots += 2;		// and 1 sub-directory

  U32 neededBytes = offsetof(FileNameDatabaseDirectory, table);
  neededBytes += neededSlots * sizeof(FileNameDatabaseHeader::FndbOffset);
  if (pHeader->size + neededBytes > foEnd)
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::CreateDirectory",
			  T_("File name database capacity exceeded."),
			  0);
    }
  FileNameDatabaseHeader::FndbOffset foSub = pHeader->size;
  pHeader->size += neededBytes;
  FileNameDatabaseDirectory * pDirSub
    = reinterpret_cast<FileNameDatabaseDirectory *>(GetPointer(foSub));
  pDirSub->Init ();
  pDirSub->foName = foName;
  pDirSub->foParent = GetOffset(pDir);
  pDirSub->numFiles = 0;
  pDirSub->numSubDirs = 0;
  pDirSub->foExtension = 0;
  pDirSub->capacity = neededSlots;
  memset (pDirSub->table,
	  0,
	  sizeof(FileNameDatabaseHeader::FndbOffset) * neededSlots);
  InsertDirectory (pDir, pDirSub);

  return (pDirSub);
}

/* _________________________________________________________________________

   FileNameDatabase::CreateDirectoryPath
   _________________________________________________________________________ */

FileNameDatabaseDirectory *
FileNameDatabase::CreateDirectoryPath
(/*[in]*/ FileNameDatabaseDirectory *	pDir,
 /*[in]*/ const char *		lpszRelPath)
{
  bool create = false;
  U32 level = 0;
  for (PathNameParser dirName (lpszRelPath);
       dirName.GetCurrent() != 0;
       ++ dirName)
    {
      const FileNameDatabaseDirectory * pDirSub = 0;
      if (! create)
	{
	  pDirSub = FindSubDirectory(pDir, dirName.GetCurrent());
	  if (pDirSub == 0)
	    {
	      create = true;
	    }
	}
      if (create)
	{
	  pDirSub = CreateDirectory(pDir, dirName.GetCurrent());
	  if (pDirSub == 0)
	    {
	      UNEXPECTED_CONDITION
		("FileNameDatabase::CreateDirectoryPath");
	    }
	}
      pDir = const_cast<FileNameDatabaseDirectory*>(pDirSub);
      ++ level;
    }
  if (level > pHeader->depth)
    {
      MIKTEX_ASSERT (create);
      pHeader->depth = level;
    }
  return (pDir);
}

/* _________________________________________________________________________

   FileNameDatabase::RemoveFileName
   _________________________________________________________________________ */

void
FileNameDatabase::RemoveFileName (/*[in]*/ FileNameDatabaseDirectory *	pDir,
				  /*[in]*/ const char *	lpszFileName)
{
  U32 index;

  pDir =
    const_cast<FileNameDatabaseDirectory*>(SearchFileName(pDir,
							  lpszFileName,
							  index));

  if (pDir == 0)
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::RemoveFileName",
			  T_("File name not found in file name database."),
			  lpszFileName);
    }

  pDir->TableRemove (index);

  if (HasFileNameInfo())
    {
      pDir->TableRemove (pDir->numFiles - 1
			 + 2 * pDir->numSubDirs
			 + index);
    }

  pDir->numFiles -= 1;
  pHeader->numFiles -= 1;
  pHeader->timeStamp = static_cast<U32>(time(0)); // <sixtyfourbit/>
}

/* _________________________________________________________________________
     
   FileNameDatabase::Flush
   _________________________________________________________________________ */

void
FileNameDatabase::Flush ()
{
  traceStream->WriteFormattedLine
    ("core",
     T_("flushing file name database"));
  mmap->Flush ();
}

/* _________________________________________________________________________

   Match
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
Match (/*[in]*/ const char * lpszPathPattern,
       /*[in]*/ const char * lpszPath)
{
  int lastch = 0;
  for (; *lpszPathPattern != 0 && *lpszPath != 0; ++ lpszPathPattern, ++ lpszPath)
  {
    if (CompareFileNameChars(*lpszPathPattern, *lpszPath) == 0)
    {
      lastch = *lpszPath;
      continue;
    }
    MIKTEX_ASSERT (RECURSION_INDICATOR_LENGTH == 2);
    MIKTEX_ASSERT (IsDirectoryDelimiter(RECURSION_INDICATOR[0]));
    MIKTEX_ASSERT (IsDirectoryDelimiter(RECURSION_INDICATOR[1]));
    if (*lpszPathPattern == RECURSION_INDICATOR[1] && IsDirectoryDelimiter(lastch))
    {
      for (; IsDirectoryDelimiter(*lpszPathPattern); ++ lpszPathPattern) {};
      if (*lpszPathPattern == 0)
      {
	return (true);
      }
      for (; *lpszPath != 0; ++ lpszPath)
      {
	if (IsDirectoryDelimiter(lastch))
	{
	  // <recursivecall/>
	  if (Match(lpszPathPattern, lpszPath))
	  {
	    return (true);
	  }
	  // </recursivecall>
	}
	lastch = *lpszPath;
      }
    }
    return (false);
  }
  return ((*lpszPathPattern == 0 || strcmp(lpszPathPattern, RECURSION_INDICATOR) == 0) && *lpszPath == 0);
}

/* _________________________________________________________________________

   FileNameDatabase::Search
   _________________________________________________________________________ */

bool
FileNameDatabase::Search (/*[in]*/ const char *		lpszFileName,
			  /*[in]*/ const char *		lpszPathPattern,
			  /*[in]*/ bool			firstMatchOnly,
			  /*[out]*/ PathNameArray &	result,
			  /*[out]*/ vector<string> &	fileNameInfo)
{
  traceStream->WriteFormattedLine
    ("core",
     T_("fndb search: rootDirectory=%s, filename=%s, pathpattern=%s"),
     Q_(rootDirectory),
     Q_(lpszFileName),
     Q_(lpszPathPattern));

  MIKTEX_ASSERT (result.size() == 0);
  MIKTEX_ASSERT (fileNameInfo.size() == 0);
  MIKTEX_ASSERT (! Utils::IsAbsolutePath(lpszFileName));
  MIKTEX_ASSERT (! IsExplicitlyRelativePath(lpszFileName));

  char szDir[BufferSizes::MaxPath];

  PathName scratch1;

  PathName::Split (lpszFileName, szDir, BufferSizes::MaxPath, 0, 0, 0, 0);

  if (szDir[0] != 0)
  {
    size_t l = strlen(szDir);
    if (IsDirectoryDelimiter(szDir[l - 1]))
    {
      szDir[l - 1] = 0;
      -- l;
    }
    scratch1 = lpszPathPattern;
    scratch1 += szDir;
    lpszPathPattern = scratch1.Get();
    lpszFileName += l + 1;
  }

  // check to see whether we have this file name
  pair<FileNameHashTable::const_iterator, FileNameHashTable::const_iterator> range =
    fileNames.equal_range(lpszFileName);

  if (range.first == range.second)
  {
    return (false);
  }

  // path pattern must be relative to root directory
  if (Utils::IsAbsolutePath(lpszPathPattern))
  {
    const char * lpsz =
      Utils::GetRelativizedPath(lpszPathPattern, rootDirectory.Get());
    if (lpsz == 0)
    {
      FATAL_MIKTEX_ERROR ("fndb_search",
	T_("Path pattern is not covered by file name database."),
	lpszPathPattern);
    }
    lpszPathPattern = lpsz;
  }

  for (FileNameHashTable::const_iterator it = range.first; it != range.second; ++ it)
  {
    PathName relPath;
    MakePathName (it->second, relPath);
    if (Match(lpszPathPattern, relPath.Get()))
    {
      PathName path;
      path = rootDirectory;
      path += relPath;
      path += lpszFileName;
      result.push_back (path);
      if (HasFileNameInfo())
      {
	U32 idx;
	const FileNameDatabaseDirectory * pDir = SearchFileName(it->second, lpszFileName, idx);
	if (pDir == 0)
	{
	  UNEXPECTED_CONDITION ("FileNameDatabase::Search");
	}
	fileNameInfo.push_back (GetString(pDir->GetFileNameInfo(idx)));
	traceStream->WriteFormattedLine ("core",
	  T_("found: %s (%s)"),
	  Q_(path),
	  GetString(pDir->GetFileNameInfo(idx)));
      }
      else
      {
	fileNameInfo.push_back ("");
	traceStream->WriteFormattedLine ("core",
	  T_("found: %s"),
	  Q_(path));
      }
      if (firstMatchOnly)
      {
	break;
      }
    }
  }

  return (result.size() > 0);
}

/* _________________________________________________________________________

   FileNameDatabase::Create

   Create a file name database. Open the root directory.
   _________________________________________________________________________ */

FileNameDatabase *
FileNameDatabase::Create (/*[in]*/ const char *	lpszFndbPath,
			  /*[in]*/ const char *	lpszRoot,
			  /*[in]*/ bool		readOnly)
{
  MIKTEX_ASSERT_STRING (lpszFndbPath);
  MIKTEX_ASSERT_STRING (lpszRoot);

  FileNameDatabase * pfndb = new FileNameDatabase;

  try
    {
      pfndb->Initialize (lpszFndbPath, lpszRoot, ! readOnly);
      return (pfndb);
    }
  catch (const exception &)
    {
      delete pfndb;
      throw;
    }
}

/* _________________________________________________________________________

   FileNameDatabase::AddFile
   _________________________________________________________________________ */

void
FileNameDatabase::AddFile (/*[in]*/ const char *	lpszPath,
			   /*[in]*/ const char *	lpszFileNameInfo)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  traceStream->WriteFormattedLine ("core",
				   T_("adding %s to the file name database"),
				   Q_(lpszPath));

  // make sure we can add files
  if (IsInvariable())
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::AddFile",
			  T_("File name database is invariable."),
			  0);
    }

  // make sure that the path is relative to the texmf root directory
  if (Utils::IsAbsolutePath(lpszPath))
    {
      const char * lpsz =
	Utils::GetRelativizedPath(lpszPath, rootDirectory.Get());
      if (lpsz == 0)
	{
	  FATAL_MIKTEX_ERROR ("FileNameDatabase::AddFile",
			      (T_("File name is not covered by file name")
			       T_(" database.")),
			      lpszPath);
	}
      lpszPath = lpsz;
    }

  // make a working copy of the path; separate file name from directory name
  PathName pathDirectory (lpszPath);
  pathDirectory.RemoveFileSpec ();
  PathName pathFile (lpszPath);
  pathFile.RemoveDirectorySpec ();
  
  // get (possibly create) the parent directory
  FileNameDatabaseDirectory * pDir;
  if (pathDirectory.GetLength() > 0)
    {
      pDir = CreateDirectoryPath(GetTopDirectory(), pathDirectory.Get());
    }
  else
    {
      // no sub-directory, i.e. create file in top directory
      pDir = GetTopDirectory();
    }
  
  if (pDir == 0)
    {
      UNEXPECTED_CONDITION ("FileNameDatabase::AddFile");
    }

  // extend the directory, if necessary
  while (pDir->capacity
	 < (pDir->SizeOfTable(HasFileNameInfo())
	    + (HasFileNameInfo() ? 2 : 1)))
    {
      if (pDir->foExtension != 0)
	{
	  // get next extension
	  pDir = GetDirectoryAt(pDir->foExtension);
	}
      else
	{
	  // create an extension
	  pDir = ExtendDirectory(pDir);
	  if (pDir == 0)
	    {
	      UNEXPECTED_CONDITION ("FileNameDatabase::AddFile");
	    }
	}
    }

  // create a new table entry
  InsertFileName (pDir,
		  CreateString(pathFile.Get()),
		  (lpszFileNameInfo
		   ? CreateString(lpszFileNameInfo)
		   : 0));

  // add the name to the hash table
  fileNames.insert
    (pair<string, const FileNameDatabaseDirectory *>(pathFile.Get(), pDir));
}

/* _________________________________________________________________________

   FileNameDatabase::Enumerate
   _________________________________________________________________________ */

bool
FileNameDatabase::Enumerate (/*[in]*/ const char *	lpszPath,
			     /*[in]*/ IEnumerateFndbCallback *	pCallback)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszPath);

  if (lpszPath != 0 && Utils::IsAbsolutePath(lpszPath))
    {
      if (PathName::Compare(lpszPath, rootDirectory.Get()) == 0)
	{
	  lpszPath = 0;
	}
      else
	{
	  const char * lpsz =
	    Utils::GetRelativizedPath(lpszPath, rootDirectory.Get());
	  if (lpsz == 0)
	    {
	      FATAL_MIKTEX_ERROR ("FileNameDatabase::Enumerate",
				  (T_("Path is not covered by file name")
				   T_(" database.")),
				  lpszPath);
	    }
	  lpszPath = lpsz;
	}
    }

  const FileNameDatabaseDirectory * pDir
    = (lpszPath == 0 || *lpszPath == 0
       ? GetTopDirectory()
       : FindSubDirectory(GetTopDirectory(), lpszPath));

  if (pDir == 0)
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::Enumerate",
			  T_("Directory not found in file name database."),
			  lpszPath);
    }
  
  PathName path (rootDirectory, lpszPath);

  for (const FileNameDatabaseDirectory * pDirIter = pDir;
       pDirIter != 0;
       pDirIter = GetDirectoryAt(pDirIter->foExtension))
    {
      for (U32 i = 0; i < pDirIter->numSubDirs; ++ i)
	{
	  if (! pCallback->OnFndbItem(path.Get(),
				      GetString(pDirIter->GetSubDirName(i)),
				      0,
				      true))
	    {
	      return (false);
	    }
	}
      
    }

  for (const FileNameDatabaseDirectory * pDirIter = pDir;
       pDirIter != 0;
       pDirIter = GetDirectoryAt(pDirIter->foExtension))
    {
      for (U32 i = 0; i < pDirIter->numFiles; ++ i)
	{
	  const char * lpszFileNameInfo = 0;
	  if (HasFileNameInfo())
	    {
	      FileNameDatabaseHeader::FndbOffset fo =
		pDirIter->GetFileNameInfo(i);
	      if (fo != 0)
		{
		  lpszFileNameInfo = GetString(fo);
		}
	    }
	  if (! pCallback->OnFndbItem(path.Get(),
				      GetString(pDirIter->GetFileName(i)),
				      lpszFileNameInfo,
				      false))
	    {
	      return (false);
	    }
	}
    }

  return (true);
}

/* _________________________________________________________________________

   FileNameDatabase::TryGetParent
   _________________________________________________________________________ */

FileNameDatabaseDirectory *
FileNameDatabase::TryGetParent (/*[in]*/ const char *	lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  // make sure that the path is relative to the texmf root directory
  if (Utils::IsAbsolutePath(lpszPath))
    {
      const char * lpsz =
	Utils::GetRelativizedPath(lpszPath, rootDirectory.Get());
      if (lpsz == 0)
	{
	  FATAL_MIKTEX_ERROR ("FileNameDatabase::TryGetParent",
			      (T_("The path name is not covered by the")
			       T_(" file name database.")),
			      lpszPath);
	}
      lpszPath = lpsz;
    }

  // make a working copy; separate file name from directory name
  PathName pathDirectory (lpszPath);
  pathDirectory.RemoveFileSpec ();

  // get the parent directory
  FileNameDatabaseDirectory * pDir;
  if (pathDirectory.GetLength() > 0)
    {
      pDir =
	const_cast<FileNameDatabaseDirectory*>
	(FindSubDirectory(GetTopDirectory(), pathDirectory.Get()));
    }
  else
    {
      pDir = GetTopDirectory();
    }

  return (pDir);
}

/* _________________________________________________________________________

   FileNameDatabase::RemoveFile
   _________________________________________________________________________ */

void
FileNameDatabase::RemoveFile (/*[in]*/ const char *	lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  traceStream->WriteFormattedLine
    ("core",
     T_("removing %s from the file name database"),
     Q_(lpszPath));

  if (IsInvariable())
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::RemoveFile",
			  T_("The file name database is invariable."),
			  0);
    }

  PathName pathFile (lpszPath);
  pathFile.RemoveDirectorySpec ();

  FileNameDatabaseDirectory * pDir = TryGetParent(lpszPath);

  if (pDir == 0)
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::RemoveFile",
			  (T_("The path could not be found")
			   T_(" in the file name database.")),
			  lpszPath);
    }

  // remove the file name
  RemoveFileName (pDir, pathFile.Get());

  // also from the hash table
  pair<FileNameHashTable::const_iterator, FileNameHashTable::const_iterator> range =
    fileNames.equal_range(pathFile.Get());
  if (range.first == range.second)
  {
    FATAL_MIKTEX_ERROR ("FileNameDatabase::RemoveFile",
			T_("The file name could not be found in the hash table."),
			  lpszPath);
  }
  bool removed = false;
  for (FileNameHashTable::const_iterator it = range.first; it != range.second; ++ it)
  {
    if (it->second == pDir)
    {
      fileNames.erase (it);
      removed = true;
      break;
    }
  }
  if (! removed)
  {
    FATAL_MIKTEX_ERROR ("FileNameDatabase::RemoveFile",
			T_("The file name could not be removed from the hash table."),
			  lpszPath);
  }
}

/* _________________________________________________________________________

   FileNameDatabase::FileExists
   _________________________________________________________________________ */

bool
FileNameDatabase::FileExists (/*[in]*/ const PathName &	path)
{
  const FileNameDatabaseDirectory * pDir = TryGetParent(path.Get());
  if (pDir != 0)
    {
      PathName fileName (path);
      fileName.RemoveDirectorySpec ();
      U32 index;
      pDir = SearchFileName(pDir, fileName.Get(), index);
    }
  return (pDir != 0);
}

#if 1 // experimental

/* _________________________________________________________________________

   FileNameDatabase::ReadFileNames
   _________________________________________________________________________ */

void
FileNameDatabase::ReadFileNames ()
{
  fileNames.clear ();
  fileNames.rehash (pHeader->numFiles);
  AutoTraceTime att ("fndb read files", rootDirectory.Get());
  ReadFileNames (GetTopDirectory());
}

/* _________________________________________________________________________

   FileNameDatabase::ReadFileNames
   _________________________________________________________________________ */

void
FileNameDatabase::ReadFileNames (/*[in]*/ const FileNameDatabaseDirectory * pDir)
{
  for (const FileNameDatabaseDirectory * pDirIter = pDir;
       pDirIter != 0;
       pDirIter = GetDirectoryAt(pDirIter->foExtension))
  {
    for (U32 i = 0; i < pDirIter->numSubDirs; ++ i)
    {
      ReadFileNames (GetDirectoryAt(pDirIter->GetSubDir(i)));
    }
    for (U32 i = 0; i < pDirIter->numFiles; ++ i)
    {
      fileNames.insert (pair<string, const FileNameDatabaseDirectory *>(
	GetString(pDirIter->GetFileName(i)),
	pDir));
    }
  }
}

/* _________________________________________________________________________

   FndbDirectoryLister
   _________________________________________________________________________ */

class FndbDirectoryLister : public DirectoryLister
{
public:
  virtual
  void
  MIKTEXTHISCALL
  Close ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetNext (/*[out]*/ DirectoryEntry & direntry);

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetNext (/*[out]*/ DirectoryEntry2 & direntry2);

public:
  FndbDirectoryLister (/*[in]*/ const PathName &	directory);

public:
  virtual
  MIKTEXTHISCALL
  ~FndbDirectoryLister ();

private:
  PathName directory;

private:
  string pattern;

private:
  HANDLE handle;

private:
  friend class DirectoryLister;
};

/* _________________________________________________________________________

   FileNameDatabase::OpenDirectory
   _________________________________________________________________________ */

DirectoryLister *
FileNameDatabase::OpenDirectory (/*[in]*/ const char *	lpszPath)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszPath);

  if (lpszPath != 0 && Utils::IsAbsolutePath(lpszPath))
    {
      if (PathName::Compare(lpszPath, rootDirectory.Get()) == 0)
	{
	  lpszPath = 0;
	}
      else
	{
	  const char * lpsz =
	    Utils::GetRelativizedPath(lpszPath, rootDirectory.Get());
	  if (lpsz == 0)
	    {
	      FATAL_MIKTEX_ERROR ("FileNameDatabase::Enumerate",
				  (T_("Path is not covered by file name")
				   T_(" database.")),
				  lpszPath);
	    }
	  lpszPath = lpsz;
	}
    }

  const FileNameDatabaseDirectory * pDir
    = (lpszPath == 0 || *lpszPath == 0
       ? GetTopDirectory()
       : FindSubDirectory(GetTopDirectory(), lpszPath));

  if (pDir == 0)
    {
      FATAL_MIKTEX_ERROR ("FileNameDatabase::Enumerate",
			  T_("Directory not found in file name database."),
			  lpszPath);
    }
  
  PathName path (rootDirectory, lpszPath);

#if 0
  for (const FileNameDatabaseDirectory * pDirIter = pDir;
       pDirIter != 0;
       pDirIter = GetDirectoryAt(pDirIter->foExtension))
    {
      for (U32 i = 0; i < pDirIter->numSubDirs; ++ i)
	{
	  if (! pCallback->OnFndbItem(path.Get(),
				      GetString(pDirIter->GetSubDirName(i)),
				      0,
				      true))
	    {
	      return (false);
	    }
	}
      
    }

  for (const FileNameDatabaseDirectory * pDirIter = pDir;
       pDirIter != 0;
       pDirIter = GetDirectoryAt(pDirIter->foExtension))
    {
      for (U32 i = 0; i < pDirIter->numFiles; ++ i)
	{
	  const char * lpszFileNameInfo = 0;
	  if (HasFileNameInfo())
	    {
	      FileNameDatabaseHeader::FndbOffset fo =
		pDirIter->GetFileNameInfo(i);
	      if (fo != 0)
		{
		  lpszFileNameInfo = GetString(fo);
		}
	    }
	  if (! pCallback->OnFndbItem(path.Get(),
				      GetString(pDirIter->GetFileName(i)),
				      lpszFileNameInfo,
				      false))
	    {
	      return (false);
	    }
	}
    }
#endif


  return (0);
}

#endif // experimental
