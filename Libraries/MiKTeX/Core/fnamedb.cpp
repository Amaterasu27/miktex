/* fnamedb2.cpp: file name database

   Copyright (C) 1996-2007 Christian Schenk

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

   ChopPath

   Return the file name part of a path name.  Zero-terminate the
   directory part.
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(MIKTEXCHAR *)
ChopPath (/*[in,out]*/ PathName & path)
{
  if (path.Empty())
    {
      return (&path[0]);
    }
  MIKTEXCHAR * lpsz =  &path[0] + path.GetLength() - 1;
  while (! IsDirectoryDelimiter(*lpsz) && lpsz != &path[0])
    {
      -- lpsz;
    }
  if (IsDirectoryDelimiter(*lpsz))
    {
      *lpsz = 0;
      return (lpsz + 1);
    }
  else
    {
      return (&path[0]);
    }
}

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
	(T_("core"),
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

const MIKTEXCHAR *
FileNameDatabase::GetString (/*[in]*/ FileNameDatabaseHeader::FndbOffset fo)
  const
{
  MIKTEX_ASSERT (fo >= sizeof(FileNameDatabaseHeader) && fo < foEnd);
  MIKTEX_ASSERT_STRING (reinterpret_cast<const MIKTEXCHAR *>(GetPointer(fo)));
  return (reinterpret_cast<const MIKTEXCHAR *>(GetPointer(fo)));
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
FileNameDatabase::OpenFileNameDatabase
(/*[in]*/ const MIKTEXCHAR *	lpszFndbPath,
 /*[in]*/ bool			readWrite)
{
#if defined(MIKTEX_WINDOWS)
  // check file attributes
  FileAttributes attributes = File::GetAttributes(lpszFndbPath);
  if ((attributes & FileAttributes::ReadOnly) != 0)
    {
      traceStream->WriteFormattedLine
	(T_("core"),
	 T_("file name database file is readonly"),
	 0);
      readWrite = false;
    }
#endif
  
  mmap->Open (lpszFndbPath, readWrite);

  if (mmap->GetSize() < sizeof(*pHeader))
    {
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::OpenFileNameDatabase"),
			  T_("Not a file name database file (wrong size)."),
			  lpszFndbPath);
    }
  
  pHeader = reinterpret_cast<FileNameDatabaseHeader*>(mmap->GetPtr());

  foEnd = static_cast<FileNameDatabaseHeader::FndbOffset>(mmap->GetSize());
  
  // check signature
  if (pHeader->signature != FileNameDatabaseHeader::Signature)
    {
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::OpenFileNameDatabase"),
		  T_("Not a file name database file (wrong signature)."),
			  lpszFndbPath);
    }
  
  // check version number
  if (pHeader->version != FileNameDatabaseHeader::Version)
    {
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::OpenFileNameDatabase"),
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
	  const size_t GRAN = (1024 * 1024);
	  const size_t EXTRA = 2 * GRAN;
	  size_t newSize =
	    ((pHeader->size + EXTRA + 1) / GRAN) * GRAN;
	  traceStream->WriteFormattedLine
	    (T_("core"),
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
FileNameDatabase::Initialize (/*[in]*/ const MIKTEXCHAR *	lpszFndbPath,
			      /*[in]*/ const MIKTEXCHAR *	lpszRoot,
			      /*[in]*/ bool			readWrite)
{
  rootDirectory = lpszRoot;
  isInvariable = ! readWrite;

  OpenFileNameDatabase (lpszFndbPath, readWrite);

  lockCount = 1;
  
  if ((pHeader->flags & FileNameDatabaseHeader::FndbFlags::Frozen) != 0)
    {
      isInvariable = true;
    }
}

/* _________________________________________________________________________
     
   FileNameDatabase::FindSubDirectory2
   _________________________________________________________________________ */

FileNameDatabaseHeader::FndbOffset
FileNameDatabase::FindSubDirectory2
(/*[in]*/ const FileNameDatabaseDirectory *	pDir,
 /*[in]*/ const MIKTEXCHAR *			lpszRelPath)
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
	      const MIKTEXCHAR * lpszSubdirName = GetString(foSubDirName);
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
 /*[in]*/ const MIKTEXCHAR *			lpszRelPath)
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
 /*[in]*/ const MIKTEXCHAR *			lpszFileName,
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
	  const MIKTEXCHAR * lpszCandidate =
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

   FileNameDatabase::SearchInDirectory
   _________________________________________________________________________ */

bool
FileNameDatabase::SearchInDirectory
(/*[in]*/ const FileNameDatabaseDirectory *		pDir,
 /*[in,out]*/ PathName &				path,
 /*[out]*/ FileNameDatabaseHeader::FndbOffset &		foFileNameInfo)
  const
{
  MIKTEXCHAR * lpszDirectory = &path[0];
  MIKTEXCHAR * lpszFileName = ChopPath(path);
  if (lpszFileName != lpszDirectory)
    {
      pDir = FindSubDirectory(pDir, lpszDirectory);
    }
  U32 index;
  pDir = SearchFileName(pDir, lpszFileName, index);
  if (pDir == 0)
    {
      return (false);
    }
  if (! HasFileNameInfo())
    {
      foFileNameInfo = 0;
    }
  else
    {
      foFileNameInfo = pDir->GetFileNameInfo(index);
    }
  return (true);
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
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::ExtendDirectory"),
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
FileNameDatabase::CreateString (/*[in]*/ const MIKTEXCHAR * lpszName)
{
  FileNameDatabaseHeader::FndbOffset foName;
  size_t neededBytes = StrLen(lpszName) + 1;
  foName = ROUND2(pHeader->size, 2);
  if (foName + neededBytes > foEnd)
    {
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::CreateString"),
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
 /*[in]*/ const MIKTEXCHAR *				lpszName,
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
 /*[in]*/ const MIKTEXCHAR *		lpszName)
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
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::CreateDirectory"),
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
 /*[in]*/ const MIKTEXCHAR *		lpszRelPath)
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
		(T_("FileNameDatabase::CreateDirectoryPath"));
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
				  /*[in]*/ const MIKTEXCHAR *	lpszFileName)
{
  U32 index;

  pDir =
    const_cast<FileNameDatabaseDirectory*>(SearchFileName(pDir,
							  lpszFileName,
							  index));

  if (pDir == 0)
    {
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::RemoveFileName"),
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
    (T_("core"),
     T_("flushing file name database"));
  mmap->Flush ();
}

/* _________________________________________________________________________

   Search
   _________________________________________________________________________ */

bool
FileNameDatabase::Search (/*[in]*/ FileNameDatabaseHeader::FndbOffset	foDir,
			  /*[in]*/ const MIKTEXCHAR *	lpszPath,
			  /*[out]*/ PathName &		result,
			  /*[out]*/ MIKTEXCHAR *	lpszFileNameInfo,
			  /*[in]*/ size_t		sizeFileNameInfo)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  if (foDir == 0)
    {
      return (false);
    }

  FileNameDatabaseHeader::FndbOffset foFileNameInfo;

  PathName temp (lpszPath);
  if (! SearchInDirectory(GetDirectoryAt(foDir), temp, foFileNameInfo))
    {
      return (false);
    }

  PathName dirRel;
  MakePathName (GetDirectoryAt(foDir), dirRel);

  result = rootDirectory;
  result += dirRel.Get();
  result += lpszPath;

  if (lpszFileNameInfo != 0)
    {
      if (foFileNameInfo == 0)
	{
	  *lpszFileNameInfo = 0;
	}
      else
	{
	  CopyString2 (lpszFileNameInfo,
		       sizeFileNameInfo,
		       GetString(foFileNameInfo),
		       sizeFileNameInfo);
	}
    }

  return (true);
}

/* _________________________________________________________________________

   FileNameDatabase::RecursiveSearch
   _________________________________________________________________________ */

bool
FileNameDatabase::RecursiveSearch
(/*[in]*/ FileNameDatabaseHeader::FndbOffset	foDir,
 /*[in]*/ const MIKTEXCHAR *			lpszSubDir,
 /*[in]*/ const MIKTEXCHAR *			lpszSearchSpec,
 /*[out]*/ PathName &				result,
 /*[out]*/ MIKTEXCHAR *				lpszFileNameInfo,
 /*[in]*/ size_t				sizeFileNameInfo)
{
  if (lpszSubDir != 0)
    {
      FileNameDatabaseHeader::FndbOffset foSubDir =
	FindSubDirectory2(GetDirectoryAt(foDir), lpszSubDir);
      if (foSubDir != 0)
	{
	  // <recursivecall>
	  if (RecursiveSearch(foSubDir,
			      0,
			      lpszSearchSpec,
			      result,
			      lpszFileNameInfo,
			      sizeFileNameInfo))
	    {
	      return (true);
	    }
	  // </recursivecall>
	}
      for (FileNameDatabaseDirectory * pDir = GetDirectoryAt(foDir);
	   pDir != 0;
	   pDir = GetDirectoryAt(pDir->foExtension))
	{
	  for (U32 subidx = 0; subidx < pDir->numSubDirs; ++ subidx)
	    {
	      // <recursivecall>
	      if (RecursiveSearch(pDir->GetSubDir(subidx),
				  lpszSubDir,
				  lpszSearchSpec,
				  result,
				  lpszFileNameInfo,
				  sizeFileNameInfo))
		{
		  return (true);
		}
	      // </recursivecall>
	    }
	}
    }
  else
    {
      const MIKTEXCHAR * lpszRecInd =
	StrStr(lpszSearchSpec, RECURSION_INDICATOR);

      if (lpszRecInd != 0)
	{
	  ptrdiff_t len = lpszRecInd - lpszSearchSpec;
	  STRDUP subdir (lpszSearchSpec, len);
	  // <recursivecall>
	  return (RecursiveSearch(foDir,
				  subdir.Get(),
				  (lpszRecInd
				   + RECURSION_INDICATOR_LENGTH),
				  result,
				  lpszFileNameInfo,
				  sizeFileNameInfo));
	  // </recursivecall>
	}
      else
	{
	  if (Search(foDir,
		     lpszSearchSpec,
		     result,
		     lpszFileNameInfo,
		     sizeFileNameInfo))
	    {
	      return (true);
	    }
	  else
	    {
	      for (FileNameDatabaseDirectory * pDir
		     = GetDirectoryAt(foDir);
		   pDir != 0;
		   pDir = GetDirectoryAt(pDir->foExtension))
		{
		  for (U32 subidx = 0;
		       subidx < pDir->numSubDirs;
		       ++ subidx)
		    {
		      // <recursivecall>
		      if (RecursiveSearch(pDir->GetSubDir(subidx),
					  0,
					  lpszSearchSpec,
					  result,
					  lpszFileNameInfo,
					  sizeFileNameInfo))
			{
			  return (true);
			}
		      // </recursivecall>
		    }
		}
	    }
	}
    }
  return (false);
}

/* _________________________________________________________________________

   FileNameDatabase::Search
   _________________________________________________________________________ */

bool
FileNameDatabase::Search (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
			  /*[in]*/ const MIKTEXCHAR *	lpszDirPath,
			  /*[out]*/ PathName &		result,
			  /*[out]*/ MIKTEXCHAR *	lpszFileNameInfo,
			  /*[in]*/ size_t		sizeFileNameInfo)

{
  CharBuffer<char, BufferSizes::MaxPath + 10> searchSpec;
  searchSpec = lpszDirPath;
  AppendDirectoryDelimiter (searchSpec.GetBuffer(), searchSpec.GetSize());
  searchSpec += lpszFileName;

  traceStream->WriteFormattedLine
    (T_("core"),
     T_("fndb search: rootDirectory=\"%s\", searchspec==\"%s\""),
     rootDirectory.Get(),
     searchSpec.Get());

  if (StrLen(searchSpec.Get()) > BufferSizes::MaxPath)
    {
      return (false);
    }

  if (Utils::IsAbsolutePath(searchSpec.Get()))
    {
      const MIKTEXCHAR * lpsz =
	Utils::GetRelativizedPath(searchSpec.Get(),
				  rootDirectory.Get());
      if (lpsz == 0)
	{
	  FATAL_MIKTEX_ERROR (T_("fndb_search"),
			      (T_("File name is not covered by file name")
			       T_(" database.")),
			      searchSpec.Get());
	}
      searchSpec = lpsz;
    }

  bool found = false;
  const char * lpszRecInd = StrStr(searchSpec.Get(), RECURSION_INDICATOR);
  if (lpszRecInd == 0)
    {
      found =
	Search(GetTopDirectory2(),
	       searchSpec.Get(),
	       result,
	       lpszFileNameInfo,
	       sizeFileNameInfo);
    }
  else
    {
      STRDUP subdir (searchSpec.Get(), lpszRecInd - searchSpec.Get());
      FileNameDatabaseHeader::FndbOffset foSubDir =
	FindSubDirectory2(GetDirectoryAt(GetTopDirectory2()), subdir.Get());
      if (foSubDir != 0)
	{
	  found =
	    RecursiveSearch(foSubDir,
			    0,
			    (lpszRecInd + RECURSION_INDICATOR_LENGTH),
			    result,
			    lpszFileNameInfo,
			    sizeFileNameInfo);
	}
    }

  if (found)
    {
      if (lpszFileNameInfo != 0 && *lpszFileNameInfo != 0)
	{
	  traceStream->WriteFormattedLine
	    (T_("core"),
	     T_("found: %s (%s)"),
	     result.Get(),
	     lpszFileNameInfo);
	}
      else
	{
	  traceStream->WriteFormattedLine
	    (T_("core"),
	     T_("found: %s"),
	     result.Get());
	}
    }

  return (found);
}

/* _________________________________________________________________________

   FileNameDatabase::Create

   Create a file name database. Open the root directory.
   _________________________________________________________________________ */

FileNameDatabase *
FileNameDatabase::Create (/*[in]*/ const MIKTEXCHAR *	lpszFndbPath,
			  /*[in]*/ const MIKTEXCHAR *	lpszRoot,
			  /*[in]*/ bool			readOnly)
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
FileNameDatabase::AddFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			   /*[in]*/ const MIKTEXCHAR *	lpszFileNameInfo)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  traceStream->WriteFormattedLine (T_("core"),
				   T_("adding %s to the file name database"),
				   Q_(lpszPath));

  // make sure we can add files
  if (IsInvariable())
    {
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::AddFile"),
			  T_("File name database is invariable."),
			  0);
    }

  // make sure that the path is relative to the texmf root directory
  if (Utils::IsAbsolutePath(lpszPath))
    {
      const MIKTEXCHAR * lpsz =
	Utils::GetRelativizedPath(lpszPath, rootDirectory.Get());
      if (lpsz == 0)
	{
	  FATAL_MIKTEX_ERROR (T_("FileNameDatabase::AddFile"),
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
      UNEXPECTED_CONDITION (T_("FileNameDatabase::AddFile"));
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
	      UNEXPECTED_CONDITION (T_("FileNameDatabase::AddFile"));
	    }
	}
    }

  // create a new table entry
  InsertFileName (pDir,
		  CreateString(pathFile.Get()),
		  (lpszFileNameInfo
		   ? CreateString(lpszFileNameInfo)
		   : 0));
}

/* _________________________________________________________________________

   FileNameDatabase::Enumerate
   _________________________________________________________________________ */

bool
FileNameDatabase::Enumerate (/*[in]*/ const MIKTEXCHAR *	lpszPath,
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
	  const MIKTEXCHAR * lpsz =
	    Utils::GetRelativizedPath(lpszPath, rootDirectory.Get());
	  if (lpsz == 0)
	    {
	      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::Enumerate"),
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
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::Enumerate"),
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
	  const MIKTEXCHAR * lpszFileNameInfo = 0;
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
FileNameDatabase::TryGetParent (/*[in]*/ const MIKTEXCHAR *	lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  // make sure that the path is relative to the texmf root directory
  if (Utils::IsAbsolutePath(lpszPath))
    {
      const MIKTEXCHAR * lpsz =
	Utils::GetRelativizedPath(lpszPath, rootDirectory.Get());
      if (lpsz == 0)
	{
	  FATAL_MIKTEX_ERROR (T_("FileNameDatabase::TryGetParent"),
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
FileNameDatabase::RemoveFile (/*[in]*/ const MIKTEXCHAR *	lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  traceStream->WriteFormattedLine
    (T_("core"),
     T_("removing %s from the file name database"),
     Q_(lpszPath));

  if (IsInvariable())
    {
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::RemoveFile"),
			  T_("The file name database is invariable."),
			  0);
    }

  PathName pathFile (lpszPath);
  pathFile.RemoveDirectorySpec ();

  FileNameDatabaseDirectory * pDir = TryGetParent(lpszPath);

  if (pDir == 0)
    {
      FATAL_MIKTEX_ERROR (T_("FileNameDatabase::RemoveFile"),
			  (T_("The path could not be found")
			   T_(" in the file name database.")),
			  lpszPath);
    }

  // remove the file name
  RemoveFileName (pDir, pathFile.Get());
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
