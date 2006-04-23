/* fnamedb.h: file name database				-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(MIKTEX__BA15DC03_8D45_4985_9111_D4B075360D81__)
#define MIKTEX__BA15DC03_8D45_4985_9111_D4B075360D81__

#include "fndbmem.h"

#define FNDB_USE_CACHE 1
#define FNDB_REMEMBER_RESULTS 0

BEGIN_INTERNAL_NAMESPACE;

/* _________________________________________________________________________

   FileNameDatabase
   _________________________________________________________________________ */

class FileNameDatabase
{
public:
  static
  FileNameDatabase *
  Create (/*[in]*/ const MIKTEXCHAR *	lpszFndbPath,
	  /*[in]*/ const MIKTEXCHAR *	lpszRoot,
	  /*[in]*/ bool			readOnly);

public:
  long
  Release ();

public:
  bool
  Search (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	  /*[in]*/ const MIKTEXCHAR *	lpszDirPath,
	  /*[out]*/ PathName &		result,
	  /*[out]*/ MIKTEXCHAR *	lpszFileNameInfo,
	  /*[in]*/ size_t		sizeFileNameInfo);

public:
  void
  AddFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	   /*[in]*/ const MIKTEXCHAR *	lpszFileNameInfo);

public:
  long
  AddRef ();
  
public:
  long
  GetRefCount ()
    const;

public:  
  void
  RemoveFile (/*[in]*/ const MIKTEXCHAR *	lpszPath);

public:
  bool
  IsInvariable ()
    const;

public:
  bool
  Enumerate (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	     /*[in]*/ IEnumerateFndbCallback *	pCallback);

public:
  bool
  FileExists (/*[in]*/ const PathName &	path);

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

#if FNDB_USE_CACHE
private:
  bool
  CheckCandidate (/*[in]*/ const MIKTEXCHAR *	lpszSearchSpec,
		  /*[out]*/ bool &		exists,
		  /*[out]*/ PathName &		result);
#endif

private:
  FileNameDatabaseDirectory *
  CreateDirectory (/*[out]*/ FileNameDatabaseDirectory *	pDir,
		   /*[in]*/ const MIKTEXCHAR *			lpszName);

private:
  FileNameDatabaseDirectory *
  CreateDirectoryPath (/*[in]*/ FileNameDatabaseDirectory *	pDir,
		       /*[in]*/ const MIKTEXCHAR *		lpszRelPath);

private:
  FileNameDatabaseHeader::FndbOffset
  CreateString (/*[in]*/ const MIKTEXCHAR * lpszName);

private:
  FileNameDatabaseDirectory *
  ExtendDirectory (/*[out]*/ FileNameDatabaseDirectory * pDir);

private:
  U32
  FindLowerBound (/*[in]*/ const FileNameDatabaseHeader::FndbOffset *	pBegin,
		  /*[in]*/ U32			count,
		  /*[in]*/ const MIKTEXCHAR *	lpszName,
		  /*[out]*/ bool &		isDuplicate)
    const;

private:
  const FileNameDatabaseDirectory *
  FindSubDirectory (/*[in]*/ const FileNameDatabaseDirectory *	pDir,
		    /*[in]*/ const MIKTEXCHAR *			lpszRelPath)
    const;

private:
  FileNameDatabaseHeader::FndbOffset
  FindSubDirectory2 (/*[in]*/ const FileNameDatabaseDirectory *	pDir,
		     /*[in]*/ const MIKTEXCHAR *		lpszRelPath)
    const;

private:
  FileNameDatabaseDirectory *
  TryGetParent (/*[in]*/ const MIKTEXCHAR *	lpszPath);
  
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
  const MIKTEXCHAR *
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
  Initialize (/*[in]*/ const MIKTEXCHAR *	lpszFndbPath,
	      /*[in]*/ const MIKTEXCHAR *	lpszRoot,
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
  OpenFileNameDatabase (/*[in]*/ const MIKTEXCHAR *	lpszFndbPath,
			/*[in]*/ bool			readWrite = false);

private:
  void
  Remember (/*[in]*/ const MIKTEXCHAR * lpszSearchSpec,
	    /*[in]*/ const MIKTEXCHAR * lpszResult);

private:
  void
  RemoveFileName (/*[in]*/ FileNameDatabaseDirectory *	pDir,
		  /*[in]*/ const MIKTEXCHAR *		lpszFileName);

private:
  const FileNameDatabaseDirectory *
  SearchFileName (/*[in]*/ const FileNameDatabaseDirectory * 	pDir,
		  /*[in]*/ const MIKTEXCHAR *			lpszFileName,
		  /*[out]*/ U32 &				index)
    const;

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
		   /*[in]*/ const MIKTEXCHAR *	lpszSubDir,
		   /*[in]*/ const MIKTEXCHAR *	lpszSearchSpec,
		   /*[out]*/ PathName &		result,
		   /*[out]*/ MIKTEXCHAR *	lpszFileNameInfo,
		   /*[in]*/ size_t		sizeFileNameInfo);

private:
  bool
  Search (/*[in]*/ FileNameDatabaseHeader::FndbOffset			foDir,
	  /*[in]*/ const MIKTEXCHAR *		lpszPath,
	  /*[out]*/ PathName &			result,
	  /*[out]*/ MIKTEXCHAR *		lpszFileNameInfo,
	  /*[in]*/ size_t			sizeFileNameInfo);



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

  // cache
#if FNDB_USE_CACHE
private:
#  if FNDB_REMEMBER_RESULTS
#    if defined(USE_HASH_MAP)
  typedef hash_map<tstring, tstring, hash_compare_icase> CACHE;
#    else
  typedef map<tstring, tstring, hash_compare_icase> CACHE;
#    endif
#  else
#    if defined(USE_HASH_SET)
  typedef hash_set<tstring, hash_compare_icase> CACHE;
#    else
  typedef set<tstring, hash_compare_icase> CACHE;
#    endif
#  endif
  CACHE cache;
#endif

#if 0
private:
  hash_map<tstring, FileNameDatabaseHeader::FndbOffset> stringTable;
#endif

  // file-system path to root directory
private:			// <fixme/>
  tstring rootDirectory;

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
