/* files.cpp: file system operations

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

const size_t PIPE_SIZE = 4096;

/* _________________________________________________________________________

   DirectoryLister::~DirectoryLister
   _________________________________________________________________________ */

DirectoryLister::~DirectoryLister ()
{
}

/* _________________________________________________________________________

   SessionImpl::RecordFileInfo
   _________________________________________________________________________ */

void
SessionImpl::RecordFileInfo (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			     /*[in]*/ FileAccess		access)
{
  if (! (recordingFileNames
	 || recordingPackageNames
	 || packageHistoryFile.length() > 0))
    {
      return;
    }
  fileInfoRecords.reserve (50);
  FileInfoRecord fir;
  fir.fileName = lpszPath;
  fir.access = access;
  if (recordingPackageNames || packageHistoryFile.length() > 0)
    {
      PathName pathRelPath;
      if (IsTEXMFFile(lpszPath, pathRelPath.GetBuffer(), 0))
	{
	  FileNameDatabase * pFndb = GetFileNameDatabase(GetMpmRoot());
	  if (pFndb != 0)
	    {
	      AutoFndbRelease autoRelease (pFndb);
	      PathName path;
	      MIKTEXCHAR szPackageName[BufferSizes::MaxPackageName];
	      if (pFndb->Search(pathRelPath.Get(),
				MPM_ROOT_PATH,
				path,
				szPackageName,
				BufferSizes::MaxPackageName))
		{
		  fir.packageName = szPackageName;
		}
	    }
	}
    }
  fileInfoRecords.push_back (fir);
}

/* _________________________________________________________________________

   SessionImpl::TryOpenFile

   _________________________________________________________________________ */

FILE *
SessionImpl::TryOpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			  /*[in]*/ FileMode		mode,
			  /*[in]*/ FileAccess		access,
			  /*[in]*/ bool			text)
{
  try
    {
      return (OpenFile(lpszPath, mode, access, text, FileShare::Read));
    }
#if defined(MIKTEX_WINDOWS)
  catch (const SharingViolationException &)
    {
      return (0);
    }
#endif
  catch (const UnauthorizedAccessException &)
    {
      return (0);
    }
  catch (const FileNotFoundException &)
    {
      return (0);
    }
}

/* _________________________________________________________________________

   SessionImpl::OpenFile
   _________________________________________________________________________ */

FILE *
SessionImpl::OpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		       /*[in]*/ FileMode		mode,
		       /*[in]*/ FileAccess		access,
		       /*[in]*/ bool			text)
{
  return (OpenFile(lpszPath, mode, access, text, FileShare::Read));
}

/* _________________________________________________________________________

   SessionImpl::TryOpenFile
   _________________________________________________________________________ */

FILE *
SessionImpl::TryOpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			  /*[in]*/ FileMode		mode,
			  /*[in]*/ FileAccess		access,
			  /*[in]*/ bool			text,
			  /*[in]*/ FileShare		share)
{
  try
    {
      return (OpenFile(lpszPath, mode, access, text, share));
    }
#if defined(MIKTEX_WINDOWS)
  catch (const SharingViolationException &)
    {
      return (0);
    }
#endif
  catch (const UnauthorizedAccessException &)
    {
      return (0);
    }
  catch (const FileNotFoundException &)
    {
      return (0);
    }
}

/* _________________________________________________________________________

   SessionImpl::OpenFile

   Open a file.
   _________________________________________________________________________ */

FILE *
SessionImpl::OpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		       /*[in]*/ FileMode		mode,
		       /*[in]*/ FileAccess		access,
		       /*[in]*/ bool			text,
		       /*[in]*/ FileShare		share)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  trace_open->WriteFormattedLine
    (T_("core"),
     T_("OpenFile(\"%s\", %d, 0x%x, %d, %d)"),
     lpszPath,
     static_cast<int>(mode.Get()),
     static_cast<int>(access.Get()),
     static_cast<int>(text),
     static_cast<int>(share.Get()));
  
  FILE * pFile = File::Open(lpszPath, mode, access, text, share);

  try
    {
      OpenFileInfo info;
      info.pFile = pFile;
      info.fileName = lpszPath;
      info.access = access;
      openFilesMap.insert (pair<FILE *, OpenFileInfo>(pFile, info));
      if (setvbuf(pFile, 0, _IOFBF, 1024 * 4) != 0)
	{
	  TraceError (T_("setvbuf() failed for some reason"));
	}
      RecordFileInfo (lpszPath, access);
      trace_open->WriteFormattedLine (T_("core"), T_("  => %p"), pFile);
      return (pFile);
    }
  catch (const exception &)
    {
      fclose (pFile);
      throw;
    }
}

/* _________________________________________________________________________

   Close
   _________________________________________________________________________ */

int
Close (/*[in]*/ int fd)
{
#if defined(_MSC_VER)
  return (_close(fd));
#else
  return (close(fd));
#endif
}

/* _________________________________________________________________________

   Pipe
   _________________________________________________________________________ */

void
Pipe (/*[out]*/ int	filedes[2],
      /*[in]*/ size_t	pipeSize)
{
  int p;
#if defined(_MSC_VER)
  p = _pipe(filedes, static_cast<unsigned>(pipeSize), _O_BINARY);
#else
  UNUSED_ALWAYS (pipeSize);
  p = pipe(filedes);
#endif
  if (p != 0)
    {
      FATAL_CRT_ERROR (T_("pipe"), 0);
    }
}

/* _________________________________________________________________________

   Pipe
   _________________________________________________________________________ */

void
Pipe (/*[out]*/ FILE *	pFiles[2],
      /*[in]*/ size_t	pipeSize)
{
  int handles[2];
  Pipe (handles, pipeSize);
  pFiles[0] = 0;
  pFiles[1] = 0;
  try
    {
      pFiles[0] = FdOpen(handles[0], T_("rb"));
      pFiles[1] = FdOpen(handles[1], T_("wb"));
    }
  catch (const exception &)
    {
      if (pFiles[0] != 0)
	{
	  fclose (pFiles[0]);
	}
      else
	{
	  Close (handles[0]);
	}
      if (pFiles[1] != 0)
	{
	  fclose (pFiles[1]);
	}
      else
	{
	  Close (handles[1]);
	}
      throw;
    }
}

/* _________________________________________________________________________

   AutoGZ
   _________________________________________________________________________ */

class gzclose_
{
public:
  void
  operator() (/*[in]*/ gzFile gz)
  {
    gzclose (gz);
  }
};

typedef AutoResource<gzFile, gzclose_> AutoGZ;

/* _________________________________________________________________________

   GZipReaderThread
   _________________________________________________________________________ */

struct GZipReaderThreadArg
{
  gzFile	gzin;
  FILE *	fileout;
};

MIKTEXSTATICFUNC(void)
MIKTEXCALLBACK
GZipReaderThread (/*[in]*/ void * pv)
{
  try
    {
      auto_ptr<GZipReaderThreadArg>
	pArg (reinterpret_cast<GZipReaderThreadArg*>(pv));
      MIKTEX_ASSERT (pArg->gzin != 0);
      MIKTEX_ASSERT (pArg->fileout != 0);
      AutoGZ autoCloseGZip2Stream (pArg->gzin);
      FileStream autoCloseOutput (pArg->fileout);
      char buf[PIPE_SIZE];
      int len = 0;
      while (! ferror(pArg->fileout)
	     && (len = gzread(pArg->gzin, buf, ARRAY_SIZE(buf))) > 0)
	{
	  fwrite (buf, 1, len, pArg->fileout);
	}
      if (len < 0)
	{
	  FATAL_MIKTEX_ERROR (T_("GZipReaderThread"),
			      T_("gzread() failed for some reason."),
			      0);
	}
      if (ferror(pArg->fileout))
	{
	  FATAL_CRT_ERROR (T_("fwrite"), 0);
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   SessionImpl::OpenGZipFile
   _________________________________________________________________________ */

FILE *
MIKTEXCALL
SessionImpl::OpenGZipFile (/*[in]*/ const PathName & path)
{
  AutoGZ autoGz (gzopen(path.Get(), T_("rb")));
  if (autoGz.Get() == 0)
    {
      FATAL_MIKTEX_ERROR (T_("OpenGZipFile"),
			  T_("gzopen() failed for some."),
			  path.Get());
    }
  FILE * pFiles[2];
  Pipe (pFiles, PIPE_SIZE);
  try
    {
      auto_ptr<GZipReaderThreadArg> pArg (new GZipReaderThreadArg);
      pArg->gzin = autoGz.Get();
      pArg->fileout = pFiles[1];
      auto_ptr<Thread> pThread (Thread::Start(GZipReaderThread, pArg.get()));
      pArg.release ();
      autoGz.Detach ();
      return (pFiles[0]);
    }
  catch (const exception &)
    {
      if (pFiles[0] != 0)
	{
	  fclose (pFiles[0]);
	}
      if (pFiles[1] != 0)
	{
	  fclose (pFiles[1]);
	}
      throw;
    }
}

/* _________________________________________________________________________

   AutoBZ2
   _________________________________________________________________________ */

class BZ2_bzclose_
{
public:
  void
  operator() (/*[in]*/ BZFILE * bz2)
  {
    BZ2_bzclose (bz2);
  }
};

typedef AutoResource<BZFILE *, BZ2_bzclose_> AutoBZ2;

/* _________________________________________________________________________

   BZip2ReaderThread
   _________________________________________________________________________ */

struct BZip2ReaderThreadArg
{
  BZFILE *	bzin;
  FILE *	fileout;
};

MIKTEXSTATICFUNC(void)
MIKTEXCALLBACK
BZip2ReaderThread (/*[in]*/ void * pv)
{
  try
    {
      auto_ptr<BZip2ReaderThreadArg>
	pArg (reinterpret_cast<BZip2ReaderThreadArg*>(pv));
      MIKTEX_ASSERT (pArg->bzin != 0);
      MIKTEX_ASSERT (pArg->fileout != 0);
      AutoBZ2 autoCloseBZip2Stream (pArg->bzin);
      FileStream autoCloseOutput (pArg->fileout);
      char buf[PIPE_SIZE];
      int len;
      while (! ferror(pArg->fileout)
	     && (len = BZ2_bzread(pArg->bzin, buf, ARRAY_SIZE(buf))) > 0)
	{
	  fwrite (buf, 1, len, pArg->fileout);
	}
      int bzerr;
      BZ2_bzerror (pArg->bzin, &bzerr);
      if (bzerr != BZ_OK)
	{
	  FATAL_MIKTEX_ERROR (T_("BZip2ReaderThread"),
			      T_("BZ2_bzread() failed for some reason."),
			      0);
	}
      if (ferror(pArg->fileout))
	{
	  FATAL_CRT_ERROR (T_("fwrite"), 0);
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   SessionImpl::OpenBZip2File
   _________________________________________________________________________ */

FILE *
MIKTEXCALL
SessionImpl::OpenBZip2File (/*[in]*/ const PathName & path)
{
  AutoBZ2 autoBz2 (BZ2_bzopen(path.Get(), T_("rb")));
  if (autoBz2.Get() == 0)
    {
      FATAL_MIKTEX_ERROR (T_("OpenBZip2File"),
			  T_("BZ2_bzopen() failed for some."),
			  path.Get());
    }
  FILE * pFiles[2];
  Pipe (pFiles, PIPE_SIZE);
  try
    {
      auto_ptr<BZip2ReaderThreadArg> pArg (new BZip2ReaderThreadArg);
      pArg->bzin = autoBz2.Get();
      pArg->fileout = pFiles[1];
      auto_ptr<Thread> pThread (Thread::Start(BZip2ReaderThread, pArg.get()));
      pArg.release ();
      autoBz2.Detach ();
      return (pFiles[0]);
    }
  catch (const exception &)
    {
      if (pFiles[0] != 0)
	{
	  fclose (pFiles[0]);
	}
      if (pFiles[1] != 0)
	{
	  fclose (pFiles[1]);
	}
      throw;
    }
}

/* _________________________________________________________________________

   SessionImpl::CloseFile
   _________________________________________________________________________ */

void
SessionImpl::CloseFile (/*[in]*/ FILE * pFile)
{
  MIKTEX_ASSERT_BUFFER (pFile, sizeof(*pFile));
  trace_open->WriteFormattedLine (T_("core"), T_("CloseFile(%p)"), pFile);
  map<const FILE *, OpenFileInfo>::iterator it = openFilesMap.find(pFile);
  if (it != openFilesMap.end())
    {
      openFilesMap.erase (it);
    }
  if (fclose(pFile) != 0)
    {
      CRT_ERROR (T_("fclose"), 0);
    }
}

/* _________________________________________________________________________

   SessionImpl::IsOutputFile
   _________________________________________________________________________ */

bool
SessionImpl::IsOutputFile (/*[in]*/ const FILE *	pFile)
{
  MIKTEX_ASSERT (pFile != 0);
  map<const FILE *, OpenFileInfo>::const_iterator
    it = openFilesMap.find(pFile);
  if (it == openFilesMap.end())
    {
      return (false);
    }
  return (it->second.access == FileAccess::Write);
}

/* _________________________________________________________________________

   Directory::Create
   _________________________________________________________________________ */

void
Directory::Create (/*[in]*/ const PathName & path)
{
  CreateDirectoryPath (path.Get());
}

/* _________________________________________________________________________

   File::PushAside
   _________________________________________________________________________ */

void
File::PushAside (/*[in,out]*/ PathName &	path,
		 /*[in]*/ const MIKTEXCHAR *	lpszExt,
		 /*[in]*/ bool			updateFndb)
{
  MIKTEX_ASSERT_STRING (lpszExt);
  tstring newFile = path.ToString();
  newFile += lpszExt;
  if (File::Exists(newFile.c_str()))
    {
      File::Delete (newFile.c_str(), true);
    }
  SessionImpl::theSession->trace_files->WriteFormattedLine
    (T_("core"),
     T_("renaming \"%s\" to \"%s\""),
     path.Get(),
     newFile.c_str());
  File::Move (path, newFile.c_str());
  if (updateFndb
      && SessionImpl::theSession->IsTEXMFFile(path.Get(), 0, 0)
      && Fndb::FileExists(path))
    {
      Fndb::Remove (path);
    }
  path = newFile.c_str();
}

/* _________________________________________________________________________

   File::Delete
   _________________________________________________________________________ */

void
File::Delete (/*[in]*/ const PathName &	path,
	      /*[in]*/ bool		tryHarder)
{
  File::Delete (path, tryHarder, false);
}

/* _________________________________________________________________________

   File::Delete
   _________________________________________________________________________ */

void
File::Delete (/*[in]*/ const PathName &		path,
	      /*[in]*/ bool			tryHarder,
	      /*[in]*/ bool			updateFndb)
{
  if (updateFndb
      && SessionImpl::theSession->IsTEXMFFile(path.Get(), 0, 0)
      && Fndb::FileExists(path))
    {
      Fndb::Remove (path);
    }

  FileAttributes attributes = File::GetAttributes(path);

  bool done;

  try
    {
      if ((attributes & FileAttributes::ReadOnly) != 0)
	{
	  attributes &= ~ FileAttributes(FileAttributes::ReadOnly);
	  File::SetAttributes (path, attributes);
	}
      File::Delete (path);
      done = true;
    }

  catch (const UnauthorizedAccessException &)
    {
#if defined(MIKTEX_WINDOWS)
      if (! tryHarder)
	{
	  throw;
	}
      done = false;
#else
      throw;
#endif
    }

#if defined(MIKTEX_WINDOWS)
  if (! done)
    {
      // move the file out of the way
      PathName absPath (path);
      if (! Utils::IsAbsolutePath(path.Get()))
	{
	  absPath.MakeAbsolute ();
	}
      PathName dir (absPath);
      dir.RemoveFileSpec ();
      MIKTEXCHAR szTemp[BufferSizes::MaxPath];
      if (GetTempFileName(dir.Get(), T_("mik"), 0, szTemp) == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("GetTempFileName"), dir.Get());
	}
      File::Delete (szTemp);
      File::Move (absPath, szTemp);
      SessionImpl::theSession->ScheduleFileRemoval (szTemp);
    }
#endif
}

/* _________________________________________________________________________

   File::Copy
   _________________________________________________________________________ */

void
File::Copy (/*[in]*/ const PathName &	source,
	    /*[in]*/ const PathName &	dest)
{
  Copy (source, dest, true);
}

/* _________________________________________________________________________

   SessionImpl::StartFileInfoRecorder
   _________________________________________________________________________ */

bool
SessionImpl::StartFileInfoRecorder (/*[in]*/ bool recordPackageNames)
{
  recordingFileNames = true;
  recordingPackageNames = recordPackageNames;
  return (true);
}

/* _________________________________________________________________________

   SessionImpl::GetFileInfoRecords
   _________________________________________________________________________ */

vector<FileInfoRecord>
SessionImpl::GetFileInfoRecords ()
{
  return (fileInfoRecords);
}

/* _________________________________________________________________________

   SessionImpl::CheckOpenFiles
   _________________________________________________________________________ */

void
SessionImpl::CheckOpenFiles ()
{
  for (map<const FILE *, OpenFileInfo>::const_iterator it
	 = openFilesMap.begin();
       it != openFilesMap.end();
       ++ it)
    {
      TraceError (T_("still open: %s"), Q_(it->second.fileName));
    }
}

/* _________________________________________________________________________

   SessionImpl::WritePackageHistory
   _________________________________________________________________________ */

void
SessionImpl::WritePackageHistory ()
{
  if (packageHistoryFile.length() == 0)
    {
      return;
    }
  StreamWriter writer (File::Open(packageHistoryFile,
				  FileMode::Append,
				  FileAccess::Write));
  for (vector<FileInfoRecord>::const_iterator
	 it = fileInfoRecords.begin();
       it != fileInfoRecords.end();
       ++ it)
    {
      if (it->packageName.length() > 0)
	{
	  writer.WriteLine (it->packageName);
	}
    }
  writer.Close ();
}

/* _________________________________________________________________________

   Directory::Delete
   _________________________________________________________________________ */

void
Directory::Delete (/*[in]*/ const PathName &	path,
		   /*[in]*/ bool		recursive)
{
  if (recursive)
    {
      PathNameArray vecFiles;
      vecFiles.reserve (10);
      
      PathNameArray vecDirectories;
      vecDirectories.reserve (10);
      
      auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(path));
      DirectoryEntry entry;
      while (pLister->GetNext(entry))
	{
	  if (entry.isDirectory)
	    {
	      vecDirectories.push_back (PathName(path.Get(),
						 entry.name.c_str(),
						 0));
	    }
	  else
	    {
	      vecFiles.push_back (PathName(path.Get(), entry.name.c_str(), 0));
	    }
	}
      pLister->Close ();
      
      PathNameArray::const_iterator it;
      
      // remove files
      for (it = vecFiles.begin(); it != vecFiles.end(); ++ it)
	{
	  File::Delete (*it, true);
	}
      
      // remove directories recursively
      for (it = vecDirectories.begin(); it != vecDirectories.end(); ++ it)
	{
	  // <recursivecall>
	  Delete (*it, true);
	  // </recursivecall>
	}
    }

  // remove this directory
  Directory::Delete (path);
}

/* _________________________________________________________________________

   File::ReadAllBytes
   _________________________________________________________________________ */

vector<unsigned char>
File::ReadAllBytes (/*[in]*/ const PathName &		path)
{
  size_t size = GetSize(path);
  vector<unsigned char> arr;
  arr.reserve (size);
  FileStream stream (Open(path,
			  FileMode::Open,
			  FileAccess::Read,
			  false));
  stream.Read (&arr[0], size);
  return (arr);
}

/* _________________________________________________________________________

   File::Open
   _________________________________________________________________________ */

FILE *
File::Open (/*[in]*/ const PathName &	path,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access)
{
  return (Open(path, mode, access, true, FileShare::Read));
}

/* _________________________________________________________________________

   File::Open
   _________________________________________________________________________ */

FILE *
File::Open (/*[in]*/ const PathName &	path,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile)
{
  return (Open(path, mode, access, isTextFile, FileShare::Read));
}
