/* TarBzip2Extractor.cpp:

   Copyright (C) 2001-2006 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#include "TarBzip2Extractor.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

/* _________________________________________________________________________

   TarBzip2Extractor::TarBzip2Extractor
   _________________________________________________________________________ */

TarBzip2Extractor::TarBzip2Extractor ()
  : traceStream (TraceStream::Open(MIKTEX_TRACE_MPM))
{
}

/* _________________________________________________________________________

   TarBzip2Extractor::~TarBzip2Extractor
   _________________________________________________________________________ */

TarBzip2Extractor::~TarBzip2Extractor ()
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

   TarBzip2Extractor::Dispose
   _________________________________________________________________________ */

void
TarBzip2Extractor::Dispose ()
{
}

/* _________________________________________________________________________

   GetOctal
   _________________________________________________________________________ */

static
int
GetOctal (/*[int]*/ const char *	lpszField)
{
#if defined(_MSC_VER)
#  define SSCANF sscanf_s
#else
#  define SSCANF sscanf
#endif

  int ret;

  if (SSCANF(lpszField, "%o", &ret) != 1)
    {
      FATAL_MPM_ERROR (T_("TarBzip2Extractor::Extract"),
		       T_("Invalid octal field."),
		       0);
    }

  return (ret);

#undef SSCANF
}

/* _________________________________________________________________________

   Header
   _________________________________________________________________________ */

struct Header
{
public:
  Header ()
  {
    MIKTEX_ASSERT (sizeof(*this) == 512);
  }

public:
  bool
  Check ()
    const
  {
    unsigned myHeaderCheckSum = 0;
    size_t i;
    for (i = 0; i < offsetof(Header, chksum); ++ i)
      {
	myHeaderCheckSum += reinterpret_cast<const unsigned char *>(this)[i];
      }
    myHeaderCheckSum += ' ' * sizeof(chksum);
    i += sizeof(chksum);
    for (; i < sizeof(*this); ++ i)
      {
	myHeaderCheckSum += reinterpret_cast<const unsigned char *>(this)[i];
      }
    return (myHeaderCheckSum == GetHeaderCheckSum());
  }

public:
  bool
  IsEndOfArchive ()
    const
  {
    for (size_t i = 0; i < sizeof(*this); ++ i)
      {
	if (reinterpret_cast<const unsigned char *>(this)[i] != 0)
	  {
	    return (false);
	  }
      }
    return (true);
  }

private:
  char name[100];

public:
  PathName
  GetFileName ()
    const
  {
    PathName ret;
    if (IsUSTAR())
      {
	ret = prefix;
      }
    ret += name;
    return (ret);
  }

private:
  char mode[8];

public:
  unsigned long
  GetFileMode ()
    const
  {
    return (GetOctal(mode));
  }

private:
  char uid[8];

private:
  char gid[8];

private:
  char size[12];

public:
  size_t
  GetFileSize ()
    const
  {
    return (GetOctal(size));
  }

private:
  char mtime[12];

public:
  time_t
  GetLastModificationTime ()
    const
  {
    return (GetOctal(mtime));
  }

private:
  char chksum[8];

public:
  unsigned long
  GetHeaderCheckSum ()
    const
  {
    return (GetOctal(chksum));
  }

public:
  enum Type {
    RegularFile = '0',
    AlternateRegularFile = '\0',
    Link = '1',
    SymbolicLink = '2',
    CharacterSpecial = '3',
    BlockSpecial = '4',
    Directory = '5',
    FIFOSpecial = '6',
    Reserved = '7',
  };

private:
  char typeflag[1];

public:
  Type
  GetType ()
    const
  {
    return (static_cast<Type>(typeflag[0]));
  }

public:
  bool
  IsNormalFile ()
    const
  {
    return (GetType() == RegularFile || GetType() == AlternateRegularFile);
  }

private:
  char linkname[100];

private:
  char magic[6];

public:
  bool
  IsUSTAR ()
    const
  {
    return (memcmp(magic, "ustar", 5) == 0
	    && (magic[5] == 0 || magic[5] == ' '));
  }

private:
  char version[2];

private:
  char uname[32];

private:
  char gname[32];

private:
  char devmajor[8];

private:
  char devminor[8];

private:
  char prefix[155];

private:
  char reservered[12];
};

/* _________________________________________________________________________

   Skip
   _________________________________________________________________________ */

void
Skip (/*[in]*/ BZFILE *		bzfileIn,
      /*[in]*/ size_t		bytes)
{
  char buffer[sizeof(Header)];
  if (bytes > sizeof(Header))
    {
      UNEXPECTED_CONDITION (T_("Skip"));
    }
  if (BZ2_bzread(bzfileIn, buffer, static_cast<int>(bytes))
      != static_cast<int>(bytes))
    {
      FATAL_MPM_ERROR (T_("TarBzip2Extractor::Extract"),
		       T_("Invalid package archive file."),
		       0);
    }
}

/* _________________________________________________________________________

   TarBzip2Extractor::Extract
   _________________________________________________________________________ */

void
TarBzip2Extractor::Extract (/*[in]*/ const PathName &	tarbz2Path,
			    /*[in]*/ const PathName &	destDir,
			    /*[in]*/ bool		makeDirectories,
			    /*[in]*/ IExtractCallback *	pCallback,
			    /*[in]*/ const MIKTEXCHAR *	lpszPrefix)
{
  traceStream->WriteFormattedLine (T_("libmpm"),
				   T_("extracting %s to %s (%s directories)"),
				   Q_(tarbz2Path),
				   Q_(destDir),
				   (makeDirectories
				    ? T_("make")
				    : T_("don't make")));

  BZFILE * bzfileIn = BZ2_bzopen(tarbz2Path.Get(), T_("rb"));

  if (bzfileIn == 0)
    {
      FATAL_MPM_ERROR (T_("TarBzip2Extractor::Extract"),
		       T_("The package archive file could not be opened."),
		       tarbz2Path.Get());
    }

  try
    {
      size_t len;
      Header header;
      size_t prefixLen = (lpszPrefix == 0 ? 0 : StrLen(lpszPrefix));
      unsigned fileCount = 0;

      bool checkHeader = true;

      while ((len = BZ2_bzread(bzfileIn, &header, sizeof(header))) > 0)
	{
	  // read next header
	  if (len != sizeof(header))
	    {
	      FATAL_MPM_ERROR
		(T_("TarBzip2Extractor::Extract"),
		 T_("Invalid package archive file."),
		 tarbz2Path.Get());
	    }

	  if (header.IsEndOfArchive())
	    {
	      break;
	    }

	  if (checkHeader)
	    {
	      if (! header.Check())
		{
		  FATAL_MPM_ERROR
		    (T_("TarBzip2Extractor::Extract"),
		     T_("Invalid package archive file."),
		     tarbz2Path.Get());
		}
#if defined(NDEBUG)
	      checkHeader = false;
#endif
	    }

	  PathName dest = header.GetFileName();
	  size_t size = header.GetFileSize();

	  if (! header.IsNormalFile())
	    {
	      if (! (header.GetType() == Header::Link
		     || header.GetType() == Header::SymbolicLink
		     || header.GetType() == Header::Directory
		     || header.GetType() == Header::CharacterSpecial
		     || header.GetType() == Header::BlockSpecial
		     || header.GetType() == Header::FIFOSpecial))
		{
		  Skip (bzfileIn,
			(size + sizeof(Header) - 1) / sizeof(Header));
		}
	      continue;
	    }

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
	      pCallback->OnBeginFileExtraction (path.Get(), size);
	    }

	  // create the destination directory
	  Directory::Create (PathName(path).RemoveFileSpec());

	  // remove the existing file
	  if (File::Exists(path))
	    {
	      File::Delete (path, true);
	    }

	  // extract the file
	  FileStream stream (File::Open(path,
					FileMode::Create,
					FileAccess::Write,
					false));
	  const size_t BUFSIZE = 4096;
	  char buffer[BUFSIZE];
	  size_t bytesRead = 0;
	  while (bytesRead < size)
	    {
	      size_t remaining = size - bytesRead;
	      size_t n = (remaining > BUFSIZE ? BUFSIZE : remaining);
	      if (BZ2_bzread(bzfileIn, buffer, static_cast<int>(n))
		  != static_cast<int>(n))
		{
		  FATAL_MPM_ERROR (T_("TarBzip2Extractor::Extract"),
				   T_("Invalid package archive file."),
				   0);
		}
	      stream.Write (buffer, n);
	      bytesRead += n;
	    }
	  stream.Close ();

	  // skip extra bytes
	  if (bytesRead % sizeof(Header) > 0)
	    {
	      Skip (bzfileIn, sizeof(Header) - bytesRead % sizeof(Header));
	    }

	  fileCount += 1;
	  
	  // set time when the file was created
	  time_t time = header.GetLastModificationTime();
	  File::SetTimes (path, time, time, time);

#if 0
	  // set file attributes
	  File::SetAttributes (path, todo);
#endif

	  // notify the client
	  if (pCallback != 0)
	    {
	      pCallback->OnEndFileExtraction (0, size);
	    }
	}
      
      traceStream->WriteFormattedLine (T_("libmpm"),
				       T_("extracted %u file(s)"),
				       fileCount);
    }
  catch (const exception &)
    {
      BZ2_bzclose (bzfileIn);
      throw;
    }

  BZ2_bzclose (bzfileIn);
}
