/* CabExtractor.cpp:

   Copyright (C) 2001-2007 Christian Schenk

   This file is part of MiKTeX Extractor.

   MiKTeX Extractor is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.
   
   MiKTeX Extractor is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Extractor; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"

#include "internal.h"

#include "CabExtractor.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Extractor;
using namespace std;

/* _________________________________________________________________________

   CabExtractor::Open
   _________________________________________________________________________ */

struct mspack_file *
CabExtractor::Open (/*[in]*/ struct mspack_system *	self,
		    /*[in]*/ char *		lpszFileName,
		    /*[in]*/ int			mode)
{
  UNUSED_ALWAYS (self);
  try
    {
      MyFile * pMyFile = new MyFile;
      pMyFile->fileName = lpszFileName;
      FileMode fileMode (FileMode::Open);
      FileAccess fileAccess (FileAccess::Read);
      switch (mode)
	{
	case MSPACK_SYS_OPEN_READ:
	  fileMode = FileMode::Open;
	  fileAccess = FileAccess::Read;
	  break;
	case MSPACK_SYS_OPEN_WRITE:
	  fileMode = FileMode::Create;
	  fileAccess = FileAccess::Write;
	  break;
	case MSPACK_SYS_OPEN_UPDATE:
	  fileMode = FileMode::Open;
	  fileAccess = FileAccess::ReadWrite;
	  break;
	case MSPACK_SYS_OPEN_APPEND:
	  fileMode = FileMode::Append;
	  fileAccess = FileAccess::Write;
	  break;
	default:
	  delete pMyFile;
	  FATAL_EXTRACTOR_ERROR (T_("CabExtractor::Open"),
				 T_("Problem with mspack interface."),
				 NUMTOSTR(mode));
	}
      try
	{
	  pMyFile->pFile
	    = File::Open(lpszFileName, fileMode, fileAccess, false);
	}
      catch (const exception &)
	{
	  delete pMyFile;
	  throw;
	}
      return (reinterpret_cast<mspack_file*>(pMyFile));
    }
  catch (const exception &)
    {
      return (0);
    }
}

/* _________________________________________________________________________

   CabExtractor::Close
   _________________________________________________________________________ */

void
CabExtractor::Close (/*[in]*/ struct mspack_file *	pFile)
{
  MyFile * pMyFile = reinterpret_cast<MyFile*>(pFile);
  try
    {
      fclose (pMyFile->pFile);
      delete pMyFile;
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   CabExtractor::Read
   _________________________________________________________________________ */

int
CabExtractor::Read (/*[in]*/ struct mspack_file *	pFile,
		    /*[out]*/ void *			pBuffer,
		    /*[in]*/ int			numBytes)
{
  MyFile * pMyFile = reinterpret_cast<MyFile*>(pFile);
  try
    {
      if (feof(pMyFile->pFile))
	{
	  return (0);
	}
      size_t n = fread(pBuffer, 1, numBytes, pMyFile->pFile);
      if (ferror(pMyFile->pFile) != 0)
	{
	  FATAL_CRT_ERROR (T_("fread"), pMyFile->fileName.c_str());
	}
      return (static_cast<int>(n));
    }
  catch (const exception &)
    {
      return (-1);
    }
}

/* _________________________________________________________________________

   CabExtractor::Write
   _________________________________________________________________________ */

int
CabExtractor::Write (/*[in]*/ struct mspack_file *	pFile,
		     /*[in]*/ void *			pBuffer,
		     /*[in]*/ int			numBytes)
{
  MyFile * pMyFile = reinterpret_cast<MyFile*>(pFile);
  try
    {
      size_t n = fwrite(pBuffer, 1, numBytes, pMyFile->pFile);
      if (ferror(pMyFile->pFile) != 0)
	{
	  FATAL_CRT_ERROR (T_("fwrite"), pMyFile->fileName.c_str());
	}
      return (static_cast<int>(n));
    }
  catch (const exception &)
    {
      return (-1);
    }
}

/* _________________________________________________________________________

   CabExtractor::Seek
   _________________________________________________________________________ */

int
CabExtractor::Seek (/*[in]*/ struct mspack_file *	pFile,
		    /*[in]*/ off_t			offset,
		    /*[in]*/ int			mode)
{
  MyFile * pMyFile = reinterpret_cast<MyFile*>(pFile);
  try
    {
      int origin;
      switch (mode)
	{
	case MSPACK_SYS_SEEK_START:
	  origin = SEEK_SET;
	  break;
	case MSPACK_SYS_SEEK_CUR:
	  origin = SEEK_CUR;
	  break;
	case MSPACK_SYS_SEEK_END:
	  origin = SEEK_END;
	  break;
	default:
	  FATAL_EXTRACTOR_ERROR (T_("CabExtractor::Seek"),
				 T_("Problem with mspack interface."),
				 NUMTOSTR(mode));
	}
      if (fseek(pMyFile->pFile, offset, origin) != 0)
	{
	  FATAL_CRT_ERROR (T_("fseek"), pMyFile->fileName.c_str());
	}
      return (0);
    }
  catch (const exception &)
    {
      return (-1);
    }
}

/* _________________________________________________________________________

   CabExtractor::Tell
   _________________________________________________________________________ */

off_t
CabExtractor::Tell (/*[in]*/ struct mspack_file * pFile)
{
  MyFile * pMyFile = reinterpret_cast<MyFile*>(pFile);
  try
    {
      long position = ftell(pMyFile->pFile);
      if (position < 0)
	{
	  FATAL_CRT_ERROR (T_("ftell"), pMyFile->fileName.c_str());
	}
      return (position);
    }
  catch (const exception &)
    {
      return (-1);
    }
}

/* _________________________________________________________________________

   CabExtractor::Message
   _________________________________________________________________________ */

void
CabExtractor::Message (/*[in]*/ struct mspack_file *	pFile,
		       /*[in]*/ char *		lpszFormat,
		       /*[in]*/				...)
{
  UNUSED_ALWAYS (pFile);
  UNUSED_ALWAYS (lpszFormat);
}

/* _________________________________________________________________________

   CabExtractor::Alloc
   _________________________________________________________________________ */

void *
CabExtractor::Alloc (/*[in]*/ struct mspack_system *	self,
		     /*[in]*/ size_t			numBytes)
{
  UNUSED_ALWAYS (self);
  try
    {
      void * ptr = malloc(numBytes);
      if (ptr == 0)
	{
	  OUT_OF_MEMORY (T_("CabExtractor::Alloc"));
	}
      return (ptr);
    }
  catch (const exception &)
    {
      return (0);
    }
}

/* _________________________________________________________________________

   CabExtractor::Free
   _________________________________________________________________________ */

void
CabExtractor::Free (/*[in]*/ void *	ptr)
{
  free (ptr);
}

/* _________________________________________________________________________

   CabExtractor::Copy
   _________________________________________________________________________ */

void
CabExtractor::Copy (/*[in]*/ void *	pSource,
		    /*[in]*/ void *	pDest,
		    /*[in]*/ size_t	numBytes)
{
  memcpy (pDest, pSource, numBytes);
}


/* _________________________________________________________________________

   CabExtractor::CabExtractor
   _________________________________________________________________________ */

CabExtractor::CabExtractor ()
  : traceStream (TraceStream::Open(MIKTEX_TRACE_EXTRACTOR))
{
  mspackSystem.open = Open;
  mspackSystem.close = Close;
  mspackSystem.read = Read;
  mspackSystem.write = Write;
  mspackSystem.seek = Seek;
  mspackSystem.tell = Tell;
  mspackSystem.message = Message;
  mspackSystem.alloc = Alloc;
  mspackSystem.free = Free;
  mspackSystem.copy = Copy;
  mspackSystem.null_ptr = 0;
  pDecompressor = mspack_create_cab_decompressor(&mspackSystem);
  if (pDecompressor == 0)
    {
      FATAL_EXTRACTOR_ERROR (T_("CabExtractor::CabExtractor"),
			     T_("Could not create cabinet decompressor."),
			     0);
    }
}

/* _________________________________________________________________________

   CabExtractor::~CabExtractor
   _________________________________________________________________________ */

EXTRACTORCALL
CabExtractor::~CabExtractor ()
{
  try
    {
      Dispose ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   CabExtractor::Dispose
   _________________________________________________________________________ */

void
EXTRACTORCALL
CabExtractor::Dispose ()
{
  if (pDecompressor != 0)
    {
      mspack_destroy_cab_decompressor (pDecompressor);
      pDecompressor = 0;
    }
  if (traceStream.get() != 0)
    {
      traceStream->Close ();
      traceStream.reset ();
    }
}

/* _________________________________________________________________________

   SetAttributes
   _________________________________________________________________________ */

static
void
SetAttributes (/*[in]*/ const PathName &	path,
	       /*[in]*/ int			cabattr)
{
  unsigned long nativeAttributes;
#if defined(MIKTEX_WINDOWS)
  nativeAttributes = 0;
  if ((cabattr & MSCAB_ATTRIB_RDONLY) != 0)
    {
      nativeAttributes |= FILE_ATTRIBUTE_READONLY;
    }
  if ((cabattr & MSCAB_ATTRIB_HIDDEN) != 0)
    {
      nativeAttributes |= FILE_ATTRIBUTE_HIDDEN;
    }
  if ((cabattr & MSCAB_ATTRIB_SYSTEM) != 0)
    {
      nativeAttributes |= FILE_ATTRIBUTE_SYSTEM;
    }
  if ((cabattr & MSCAB_ATTRIB_ARCH) != 0)
    {
      nativeAttributes |= FILE_ATTRIBUTE_ARCHIVE;
    }
  if (nativeAttributes == 0)
    {
      return;
    }
#else
  const unsigned long NORMAL =
    (0
     | S_IRUSR | S_IWUSR
     | S_IRGRP
     | S_IROTH);
  nativeAttributes = NORMAL;
  if ((cabattr & MSCAB_ATTRIB_RDONLY) != 0)
    {
      nativeAttributes &= ~ (S_IWUSR | S_IWGRP | S_IWOTH);
    }
  if ((cabattr & MSCAB_ATTRIB_EXEC) != 0)
    {
      nativeAttributes |= S_IXUSR;
    }
  if (nativeAttributes == NORMAL)
    {
      return;
    }
#endif
  File::SetNativeAttributes (path, nativeAttributes);
}

/* _________________________________________________________________________

   CabExtractor::Extract
   _________________________________________________________________________ */

void
EXTRACTORCALL
CabExtractor::Extract (/*[in]*/ const PathName &	cabinetPath,
		       /*[in]*/ const PathName &	destDir,
		       /*[in]*/ bool			makeDirectories,
		       /*[in]*/ IExtractCallback *	pCallback,
		       /*[in]*/ const char *	lpszPrefix)
{
  traceStream->WriteFormattedLine (T_("libextractor"),
				   T_("extracting %s to %s (%s directories)"),
				   Q_(cabinetPath),
				   Q_(destDir),
				   (makeDirectories
				    ? T_("make")
				    : T_("don't make")));
  
  mscabd_cabinet * pCabinet = 0;

  try
    {
      pCabinet =
	pDecompressor->open(pDecompressor,
			    const_cast<char *>(cabinetPath.Get()));

      if (pCabinet == 0)
	{
	  FATAL_EXTRACTOR_ERROR (T_("CabExtractor::Extract"),
			   T_("The cabinet could not be opened."),
			   cabinetPath.Get());
	}

      size_t prefixLen = (lpszPrefix == 0 ? 0 : StrLen(lpszPrefix));

      unsigned fileCount = 0;

      for (mscabd_file * pCabFile = pCabinet->files;
	   pCabFile != 0;
	   pCabFile = pCabFile->next)
	{
	  PathName dest (pCabFile->filename);

#if defined(MIKTEX_UNIX)
	  dest.ToUnix ();
#endif

	  // skip directory prefix
	  if (lpszPrefix != 0
	      && PathName::Compare(lpszPrefix, dest, prefixLen) == 0)
	    {
	      PathName tmp (dest);
	      dest = tmp.Get() + prefixLen;
	    }

	  // make the destination path name
	  PathName path (destDir);
	  if (! makeDirectories)
	    {
	      dest.RemoveDirectorySpec ();
	    }
	  path += dest;

	  // notify the client
	  if (pCallback != 0)
	    {
	      pCallback->OnBeginFileExtraction (path.Get(),
						pCabFile->length);
	    }

	  // create the destination directory
	  Directory::Create (PathName(path).RemoveFileSpec());

	  // remove the existing file
	  if (File::Exists(path))
	    {
	      File::Delete (path, true);
	    }

	  // extract the file
	  int r =
	    pDecompressor->extract(pDecompressor,
				   pCabFile,
				   path.GetBuffer());
	  if (r != MSPACK_ERR_OK)
	    {
	      FATAL_EXTRACTOR_ERROR (T_("CabExtractor::Extract"),
			       T_("The file could not be extracted."),
			       pCabFile->filename);
	    }

	  fileCount += 1;

	  // set time when the file was created
	  struct tm tm;
	  tm.tm_sec = pCabFile->time_s;
	  tm.tm_min = pCabFile->time_m;
	  tm.tm_hour = pCabFile->time_h;
	  tm.tm_mday = pCabFile->date_d;
	  tm.tm_mon = pCabFile->date_m - 1;
	  tm.tm_year = pCabFile->date_y - 1900;
	  tm.tm_isdst = 0;
	  time_t time = mktime(&tm);
	  if (time == static_cast<time_t>(-1))
	    {
	      FATAL_CRT_ERROR (T_("mktime"), 0);
	    }
	  File::SetTimes (path, time, time, time);

	  // set file attributes
	  SetAttributes (path, pCabFile->attribs);

	  // notify the client
	  if (pCallback != 0)
	    {
	      pCallback->OnEndFileExtraction (0, pCabFile->length);
	    }
	}
      
      traceStream->WriteFormattedLine (T_("libextractor"),
				       T_("extracted %u file(s)"),
				       fileCount);
      
      pDecompressor->close (pDecompressor, pCabinet);
      pCabinet = 0;

    }
  catch (const exception &)
    {
      if (pCabinet != 0)
	{
	  pDecompressor->close (pDecompressor, pCabinet);
	}
      throw;
    }
}

/* _________________________________________________________________________

   CabExtractor::Extract
   _________________________________________________________________________ */

void
EXTRACTORCALL
CabExtractor::Extract (/*[in]*/ Stream *		pStream,
		       /*[in]*/ const PathName &	destDir,
		       /*[in]*/ bool			makeDirectories,
		       /*[in]*/ IExtractCallback *	pCallback,
		       /*[in]*/ const char *	lpszPrefix)
{
  UNUSED_ALWAYS (pStream);
  UNUSED_ALWAYS (destDir);
  UNUSED_ALWAYS (makeDirectories);
  UNUSED_ALWAYS (pCallback);
  UNUSED_ALWAYS (lpszPrefix);
  UNIMPLEMENTED (T_("CabExtractor::Extract"));
}
