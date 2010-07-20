/* fnamedb.h: file name database				-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(MIKTEX__BA15DC03_8D45_4985_9111_D4B075360D81__)
#define MIKTEX__BA15DC03_8D45_4985_9111_D4B075360D81__

#if 1 // experimental
#define USE_HASH_TABLE 1
#endif

#include "fndbmem.h"

BEGIN_INTERNAL_NAMESPACE;

/* _________________________________________________________________________

   FileNameDatabase
   _________________________________________________________________________ */

class FileNameDatabase
{
public:
  static
  FileNameDatabase *
  Create (/*[in]*/ const char *	lpszFndbPath,
	  /*[in]*/ const char *	lpszRoot,
	  /*[in]*/ bool		readOnly);

public:
  long
  Release ();

public:
  bool
  Search (/*[in]*/ const char *	lpszFileName,
	  /*[in]*/ const char *	lpszPathPattern,
	  /*[out]*/ PathName &	result,
	  /*[out]*/ char *	lpszFileNameInfo,
	  /*[in]*/ size_t	sizeFileNameInfo);

public:
  void
  AddFile (/*[in]*/ const char *	lpszPath,
	   /*[in]*/ const char *	lpszFileNameInfo);

public:
  long
  AddRef ();
  
public:
  long
  GetRefCount ()
    const;

public:  
  void
  RemoveFile (/*[in]*/ const char *	lpszPath);

public:
  bool
  IsInvariable ()
    const;

public:
  bool
  Enumerate (/*[in]*/ const char *		lpszPath,
	     /*[in]*/ IEnumerateFndbCallback *	pCallback);

public:
  bool
  FileExists (/*[in]*/ const PathName &	path);

#if USE_HASH_TABLE
public:
  void
  ReadFileNames ();

private:
  void
  ReadFileNames (/*[in]*/ const FileNameDatabaseDirectory * pDir);
#endif

#if 1 // experimental
public:
  Core::DirectoryLister *
  OpenDirectory (/*[in]*/ const char * lpszPath);
#endif

private:
  FileNameDatabase ();

  // no copy construction
private:
  FileNameDatabase (/*[in]*/ const FileNameDatabase & rhs);

private:
  ~FileNameDatabase ();

private:
  void
  Finalize ();

private:
  FileNameDatabaseDirectory *
  CreateDirectory (/*[out]*/ FileNameDatabaseDirectory *	pDir,
		   /*[in]*/ const char *			lpszName);

private:
  FileNameDatabaseDirectory *
  CreateDirectoryPath (/*[in]*/ FileNameDatabaseDirectory *	pDir,
		       /*[in]*/ const char *			lpszRelPath);

private:
  FileNameDatabaseHeader::FndbOffset
  CreateString (/*[in]*/ const char * lpszName);

private:
  FileNameDatabaseDirectory *
  ExtendDirectory (/*[out]*/ FileNameDatabaseDirectory * pDir);

private:
  U32
  FindLowerBound (/*[in]*/ const FileNameDatabaseHeader::FndbOffset *	pBegin,
		  /*[in]*/ U32			count,
		  /*[in]*/ const char *		lpszName,
		  /*[out]*/ bool &		isDuplicate)
    const;

private:
  const FileNameDatabaseDirectory *
  FindSubDirectory (/*[in]*/ const FileNameDatabaseDirectory *	pDir,
		    /*[in]*/ const char *			lpszRelPath)
    const;

private:
  FileNameDatabaseHeader::FndbOffset
  FindSubDirectory2 (/*[in]*/ const FileNameDatabaseDirectory *	pDir,
		     /*[in]*/ const char *			lpszRelPath)
    const;

private:
  FileNameDatabaseDirectory *
  TryGetParent (/*[in]*/ const char *	lpszPath);
  
private:
  void
  Flush ();

private:
  const FileNameDatabaseDirectory *
  GetDirectoryAt (/*[in]*/ FileNameDatabaseHeader::FndbOffset fndboff)
    const;

private:
  FileNameDatabaseDirectory *
  GetDirectoryAt (/*[in]*/ FileNameDatabaseHeader::FndbOffset fndboff);

private:
  U32
  GetHeaderFlags ()
    const
  {
    return (pHeader->flags);
  }
  
private:
  FileNameDatabaseHeader::FndbOffset
  GetOffset (/*[in]*/ const void * p)
    const;

private:
  const void *
  GetPointer (/*[in]*/ FileNameDatabaseHeader::FndbOffset fndboff)
    const;

private:
  void *
  GetPointer (/*[in]*/ FileNameDatabaseHeader::FndbOffset fndboff);
  
private:
  const char *
  GetString (/*[in]*/ FileNameDatabaseHeader::FndbOffset fndboff)
    const;

private:
  const FileNameDatabaseDirectory *
  GetTopDirectory ()
    const;

private:
  FileNameDatabaseDirectory *
  GetTopDirectory ();

private:
  FileNameDatabaseHeader::FndbOffset
  GetTopDirectory2 ()
    const;

private:
  bool
  HasFileNameInfo ()
    const;

private:
  void
  Initialize (/*[in]*/ const char *		lpszFndbPath,
	      /*[in]*/ const char *		lpszRoot,
	      /*[in]*/ bool			readWrite = false);

private:
  void
  InsertDirectory (/*[out]*/ FileNameDatabaseDirectory *	pDir,
		   /*[in]*/  const FileNameDatabaseDirectory *	pDirSub);

private:
  void
  InsertFileName (/*[in]*/ FileNameDatabaseDirectory *		pDir,
		  /*[in]*/ FileNameDatabaseHeader::FndbOffset foFileName,
		  /*[in]*/ FileNameDatabaseHeader::FndbOffset foFileNameInfo);

private:
  bool
  IsDirty ()
    const;

private:
  void
  MakePathName (/*[in]*/ const FileNameDatabaseDirectory *	pDir,
		/*[out]*/ PathName &				path)
    const;

private:
  void
  OpenFileNameDatabase (/*[in]*/ const char *	lpszFndbPath,
			/*[in]*/ bool			readWrite = false);

private:
  void
  RemoveFileName (/*[in]*/ FileNameDatabaseDirectory *	pDir,
		  /*[in]*/ const char *			lpszFileName);

private:
  const FileNameDatabaseDirectory *
  SearchFileName (/*[in]*/ const FileNameDatabaseDirectory * 	pDir,
		  /*[in]*/ const char *				lpszFileName,
		  /*[out]*/ U32 &				index)
    const;

#if ! USE_HASH_TABLE
private:
  bool
  SearchInDirectory
  (/*[in]*/ const FileNameDatabaseDirectory *		pDir,
   /*[out]*/ PathName &					path,
   /*[out]*/ FileNameDatabaseHeader::FndbOffset &	foFileNameInfo)
    const;

private:
  bool
  RecursiveSearch (/*[in]*/ FileNameDatabaseHeader::FndbOffset		foDir,
		   /*[in]*/ const char *	lpszSubDir,
		   /*[in]*/ const char *	lpszSearchSpec,
		   /*[out]*/ PathName &		result,
		   /*[out]*/ char *		lpszFileNameInfo,
		   /*[in]*/ size_t		sizeFileNameInfo);

private:
  bool
  Search (/*[in]*/ FileNameDatabaseHeader::FndbOffset			foDir,
	  /*[in]*/ const char *			lpszPath,
	  /*[out]*/ PathName &			result,
	  /*[out]*/ char *			lpszFileNameInfo,
	  /*[in]*/ size_t			sizeFileNameInfo);
#endif

  // true, if the FNDB is read-only
private:			// <fixme/>
  bool isInvariable;

  // last modification time
private:
  U32 timeStamp;

  // size (in bytes) of the FNDB file
private:
  FileNameDatabaseHeader::FndbOffset foEnd;

private:
  auto_ptr<MemoryMappedFile> mmap;

  // lock count
private:
  long volatile lockCount;

  // pointer to the FNDB header
private:
  FileNameDatabaseHeader * pHeader;

  // file-system path to root directory
private:
  PathName rootDirectory;

#if USE_HASH_TABLE
private:
  typedef hash_multimap<string, const FileNameDatabaseDirectory *, hash_compare_icase> FileNameHashTable;
  FileNameHashTable fileNames;
#endif

private:
  auto_ptr<TraceStream> traceStream;

private:
  MIKTEX_DEFINE_LOCK(this);

};				// FileNameDatabase

/* _________________________________________________________________________

   AutoFndbRelease
   _________________________________________________________________________ */

class FndbRelease_
{
public:
  void
  operator() (/*[in]*/ FileNameDatabase * pFndb)
  {
    pFndb->Release ();
  }
};

typedef AutoResource<FileNameDatabase *, FndbRelease_> AutoFndbRelease;

END_INTERNAL_NAMESPACE;

#endif
