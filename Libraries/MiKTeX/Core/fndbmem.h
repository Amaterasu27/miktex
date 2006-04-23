/* fndbmem.h: fndb file format					-*- C++ -*-

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

#if ! defined(MIKTEX__A0FEBF8A_9A7A_419B_B123_0D6A7C07C5FA__)
#define MIKTEX__A0FEBF8A_9A7A_419B_B123_0D6A7C07C5FA__

BEGIN_INTERNAL_NAMESPACE;

/* _________________________________________________________________________

   FileNameDatabaseHeader
   _________________________________________________________________________ */

struct FileNameDatabaseHeader
{
  static const U32 Signature = 0x42444e46; // 'FNDB' (the x86 way)
  static const U32 Version = 4;

  class FndbFlags
  {
  public:
    enum {
      Frozen = 1,		// database cannot be modified
      FileNameInfo = 2,		// extra file name info
    };
  };

  // file offset
  typedef U32 FndbOffset;

  // signature of fndb file
  U32 signature;

  // format version number
  U32 version;

  // flag word (bits see above)
  U32 flags;

  // pointer to path name
  FndbOffset foPath;

  // pointer to root directory
  FndbOffset foTopDir;

  // number of directories
  U32 numDirs;

  // number of files
  U32 numFiles;

  // time of last refresh
  U32 timeStamp;

  // max directory depth
  U32 depth;

  // size (in bytes) of fndb; includes header size
  U32 size;

  void
  Init ()
  {
    MIKTEX_ASSERT (sizeof(*this) % 8 == 0);
    signature = Signature;
    version = Version;
    flags = 0;
    size = sizeof(*this);
  }
};

/* _________________________________________________________________________

   FileNameDatabaseDirectory
   _________________________________________________________________________ */

struct FileNameDatabaseDirectory
{
  // pointer to directory name
  FileNameDatabaseHeader::FndbOffset foName;

  // pointer to parent directory
  FileNameDatabaseHeader::FndbOffset foParent;

  // number of files in this directory
  U32 numFiles;

  // number of sub-directories
  U32 numSubDirs;

  // pointer to directory extension
  FileNameDatabaseHeader::FndbOffset foExtension;

  // capacity of pointer table
  U32 capacity;

  // table of pointers to
  //     numFiles file names
  //   + numSubDirs sub directory names
  //   + numSubDirs sub directories
  //   + numFiles file infos (if FNDB_FLAG_FILE_NAME_INFO is set)
  FileNameDatabaseHeader::FndbOffset table[1];

  FileNameDatabaseHeader::FndbOffset
  GetFileName (/*[in]*/ U32 idx)
    const
  {
    MIKTEX_ASSERT (idx < numFiles);
    return (table[idx]);
  }

  FileNameDatabaseHeader::FndbOffset
  GetSubDirName (/*[in]*/ U32 idx)
    const
  {
    MIKTEX_ASSERT (idx < numSubDirs);
    return (table[numFiles + idx]);
  }

  FileNameDatabaseHeader::FndbOffset
  GetSubDir (/*[in]*/ U32 idx)
    const
  {
    MIKTEX_ASSERT (idx < numSubDirs);
    return (table[numFiles + numSubDirs + idx]);
  }

  FileNameDatabaseHeader::FndbOffset
  GetFileNameInfo (/*[in]*/ U32 idx)
    const
  {
    MIKTEX_ASSERT (idx < numFiles);
    return (table[numFiles + (2 * numSubDirs) + idx]);
  }

  U32
  SizeOfTable (/*[in]*/ bool hasFileNameInfo)
    const
  {
    U32 size = numFiles + 2 * numSubDirs;
    if (hasFileNameInfo)
      {
	size += numFiles;
      }
    return (size);
  }

  void
  TableInsert (/*[in]*/ size_t					idx,
	       /*[in]*/ FileNameDatabaseHeader::FndbOffset	fo)
  {
    MIKTEX_ASSERT (idx < capacity);
    memmove (&table[idx + 1],
	     &table[idx],
	     ((capacity - idx - 1)
	      * sizeof(FileNameDatabaseHeader::FndbOffset)));
    table[idx] = fo;
  }

  void
  TableRemove (/*[in]*/ size_t	idx)
  {
    MIKTEX_ASSERT (idx < capacity);
    memmove (&table[idx],
	     &table[idx + 1],
	     ((capacity - idx - 1)
	      * sizeof(FileNameDatabaseHeader::FndbOffset)));
  }

  void
  Init ()
  {
    foExtension = 0;
  }
};

END_INTERNAL_NAMESPACE;

#endif
