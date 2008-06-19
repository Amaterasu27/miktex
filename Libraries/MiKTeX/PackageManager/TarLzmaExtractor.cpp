/* TarLzmaExtractor.cpp:

   Copyright (C) 2001-2008 Christian Schenk

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

#include <CPP/Common/MyWindows.h>
#include <CPP/Common/MyInitGuid.h>
#include <CPP/7zip/ICoder.h>

#include <miktex/lzma.h>

#include "TarLzmaExtractor.h"

const size_t BLOCKSIZE = 512;

/* _________________________________________________________________________

   LzmaStreamHelper::LzmaStreamHelper
   _________________________________________________________________________ */

class LzmaStreamHelper

  : public ISequentialInStream,
    public ISequentialOutStream

{
public:
  LzmaStreamHelper (/*[in]*/ FILE * pLzmaFile)
    : pBuffer (0),
      offset (0),
      sizeBuffer (0),
      refCount (0),
      ready (false),
      successful (false),
      pLzmaFile (pLzmaFile)
  {
  }

public:
  virtual
  ~LzmaStreamHelper ()
  {
    if (pBuffer != 0)
      {
	free (pBuffer);
      }
  }

public:
  virtual
  HRESULT
  STDMETHODCALLTYPE
  QueryInterface (/*[in]*/ REFGUID 	iid,
		  /*[out]*/ void **	ppObject)
  {
    if (iid == IID_IUnknown)
      {
	*ppObject =
	  static_cast<IUnknown*>(static_cast<ISequentialInStream*>(this));
      }
    else if (iid == IID_ISequentialInStream)
      {
	*ppObject = static_cast<ISequentialInStream*>(this);
      }
    else if (iid == IID_ISequentialOutStream)
      {
	*ppObject = static_cast<ISequentialOutStream*>(this);
      }
    else
      {
	*ppObject = 0;
	return (E_NOINTERFACE);
      }
    AddRef ();
    return (S_OK);
  }

public:
  virtual
  ULONG
  STDMETHODCALLTYPE
  AddRef ()
  {
    MIKTEX_LOCK(LzmaStreamHelper)
      {
	++ refCount;
	return (refCount);
      }
    MIKTEX_UNLOCK();
  }

public:
  virtual
  ULONG
  STDMETHODCALLTYPE
  Release ()
  {
    ULONG refCount;
    MIKTEX_LOCK(LzmaStreamHelper)
      {
	-- this->refCount;
	refCount = this->refCount;
      }
    MIKTEX_UNLOCK();
    if (refCount == 0)
      {
	delete this;
      }
    return (refCount);
  }

public:
  virtual
  HRESULT
  STDMETHODCALLTYPE
  Read (/*[in]*/ void *		pData,
	/*[in[*/ UInt32		size,
	/*[out]*/ UInt32 *	pProcessedSize)
  {
    try
      {
	size_t n = fread(pData, 1, size, pLzmaFile);
	if (ferror(pLzmaFile) != 0)
	  {
	    FATAL_CRT_ERROR (T_("fread"), 0);
	  }
	*pProcessedSize = static_cast<UInt32>(n);
	return (S_OK);
      }
    catch (const exception &)
      {
	return (E_FAIL);
      }
  }

public:
  virtual
  HRESULT
  STDMETHODCALLTYPE
  Write (/*[in]*/ const void *	pData,
	 /*[in]*/ UInt32	size,
	 /*[out]*/ UInt32 *	pProcessedSize)
  {
    MIKTEX_LOCK(LzmaStreamHelper)
      {
	size_t newOffset = offset + size;
	if (newOffset > sizeBuffer)
	  {
	    sizeBuffer = newOffset + 4096;
	    pBuffer =
	      static_cast<unsigned char *>(realloc(pBuffer, sizeBuffer));
	  }
	memcpy (pBuffer + offset, pData, size);
	offset = newOffset;
	*pProcessedSize = size;
	return (S_OK);
      }
    MIKTEX_UNLOCK();
  }

public:
  size_t
  GetData (/*[in]*/ void *	pData,
	   /*[in]*/ size_t	size)
  {
    size_t read = 0;
    bool done = false;
    do
      {
	MIKTEX_LOCK(LzmaStreamHelper)
	  {
	    if (ready)
	      {
		if (! successful)
		  {
		    throw threadMiKTeXException;
		  }
		if (size > offset)
		  {
		    size = offset;
		  }
	      }
	    if (size <= offset)
	      {
		memcpy (pData, pBuffer, size);
		memmove (pBuffer, pBuffer + size, offset - size);
		offset -= size;
		read = size;
		done = true;
	      }
	  }
	MIKTEX_UNLOCK();
	if (! done)
	  {
	    Thread::Sleep (10);
	  }
      }
    while (! done);
    return (read);
  }

private:
  void
  DoUncompress ()
  {
    ICompressCoder * pCompressCoder =
      static_cast<ICompressCoder*>(MiKTeX::LZMA::CreateDecoder());
    ICompressSetDecoderProperties2 * pSetProp = 0;
    try
      {
	if (pCompressCoder->QueryInterface(IID_ICompressSetDecoderProperties2,
					   reinterpret_cast<void**>(&pSetProp))
	    != S_OK)
	  {
	    pSetProp = 0;
	    UNEXPECTED_CONDITION (T_("LzmaStreamHelper::Uncompress"));
	  }
	unsigned char props[5];
	UInt32 n;
	if (Read(props, 5, &n) != S_OK || n != 5)
	  {
	    FATAL_MIKTEX_ERROR (T_("LzmaStreamHelper::Uncompress"),
				T_("Unexpected end of file."),
				0);
	  }
	if (pSetProp->SetDecoderProperties2(props, 5) != S_OK)
	  {
	    FATAL_MIKTEX_ERROR (T_("LzmaStreamHelper::Uncompress"),
				T_("LZMA decoder error."),
				0);
	  }
	UInt64 fileSize = 0;
	for (int i = 0; i < 8; ++ i)
	  {
	    unsigned char b;
	    if (Read(&b, 1, &n) != S_OK || n != 1)
	      {
		FATAL_MIKTEX_ERROR (T_("LzmaStreamHelper::Uncompress"),
				    T_("Unexpected end of file."),
				    0);
	      }
	    fileSize |= (static_cast<UInt64>(b) << (8 * i));
	  }
	if (pCompressCoder->Code(this, this, 0, &fileSize, 0) != S_OK)
	  {
	    FATAL_MIKTEX_ERROR (T_("LzmaStreamHelper::Uncompress"),
				T_("LZMA decoder error."),
				0);
	  }
      }
    catch (const exception &)
      {
	if (pSetProp != 0)
	  {
	    pSetProp->Release ();
	  }
	if (pCompressCoder != 0)
	  {
	    pCompressCoder->Release ();
	  }
	throw;
      }
    if (pSetProp != 0)
      {
	pSetProp->Release ();
      }
    if (pCompressCoder != 0)
      {
	pCompressCoder->Release ();
      }
  }

private:
  static
  void
  MIKTEXCALLBACK
  UncompressThread (/*[in]*/ void * pv)
  {
    LzmaStreamHelper * This = static_cast<LzmaStreamHelper*>(pv);
    try
      {
	This->DoUncompress ();
	This->ready = true;
	This->successful = true;
      }
    catch (const MiKTeXException & e)
      {
	This->ready = true;
	This->successful = false;
	This->threadMiKTeXException = e;
      }
  }

public:
  void
  Start ()
  {
    ready = false;
    successful = false;
    offset = 0;
    pThread =
      auto_ptr<Thread>(Thread::Start(LzmaStreamHelper::UncompressThread,
				     this));
  }

public:
  bool
  IsReady ()
  {
    return (ready);
  }

public:
  bool
  IsSuccessful ()
  {
    return (successful);
  }

private:
  MIKTEX_DEFINE_LOCK(LzmaStreamHelper);

private:
  FILE * pLzmaFile;

private:
  unsigned char * volatile pBuffer;

private:
  volatile size_t sizeBuffer;

private:
  volatile size_t offset;

private:
  volatile bool successful;

private:
  volatile bool ready;

private:
  volatile ULONG refCount;

private:
  auto_ptr<Thread> pThread;

private:
  MiKTeXException threadMiKTeXException;
};

/* _________________________________________________________________________

   TarLzmaExtractor::TarLzmaExtractor
   _________________________________________________________________________ */

TarLzmaExtractor::TarLzmaExtractor ()
  : traceStream (TraceStream::Open(MIKTEX_TRACE_MPM)),
    haveLongName (false)
{
}

/* _________________________________________________________________________

   TarLzmaExtractor::~TarLzmaExtractor
   _________________________________________________________________________ */

TarLzmaExtractor::~TarLzmaExtractor ()
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

   TarLzmaExtractor::Dispose
   _________________________________________________________________________ */

void
TarLzmaExtractor::Dispose ()
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
      FATAL_MPM_ERROR (T_("TarLzmaExtractor::Extract"),
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
    char namez[sizeof(name) + 1];
    memcpy (namez, name, sizeof(name));
    namez[sizeof(name)] = 0;
    PathName ret;
    if (IsUSTAR())
      {
	ret = prefix;
      }
    ret += namez;
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

    LongName = 'L'
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
Skip (/*[in]*/ LzmaStreamHelper *	pLzmaStream,
      /*[in]*/ size_t			bytes)
{
  char buffer[sizeof(Header)];
  if (bytes > sizeof(Header))
    {
      UNEXPECTED_CONDITION (T_("Skip"));
    }
  if (pLzmaStream->GetData(buffer, bytes) != bytes)
    {
      FATAL_MPM_ERROR (T_("TarLzmaExtractor::Extract"),
		       T_("Invalid package archive file."),
		       0);
    }
}

/* _________________________________________________________________________

   TarLzmaExtractor::Extract
   _________________________________________________________________________ */

void
TarLzmaExtractor::Extract (/*[in]*/ const PathName &	tarlzmaPath,
			    /*[in]*/ const PathName &	destDir,
			    /*[in]*/ bool		makeDirectories,
			    /*[in]*/ IExtractCallback *	pCallback,
			    /*[in]*/ const MIKTEXCHAR *	lpszPrefix)
{
  traceStream->WriteFormattedLine (T_("libmpm"),
				   T_("extracting %s to %s (%s directories)"),
				   Q_(tarlzmaPath),
				   Q_(destDir),
				   (makeDirectories
				    ? T_("make")
				    : T_("don't make")));

  FileStream file (File::Open(tarlzmaPath,
			      FileMode::Open,
			      FileAccess::Read,
			      false));

  LzmaStreamHelper * pLzmaStream = new LzmaStreamHelper(file.Get());
  pLzmaStream->AddRef ();
  
  try
    {
      pLzmaStream->Start ();

      size_t len;
      Header header;
      size_t prefixLen = (lpszPrefix == 0 ? 0 : StrLen(lpszPrefix));
      unsigned fileCount = 0;
      
      bool checkHeader = true;
      
      while ((len = pLzmaStream->GetData(&header, sizeof(header))) > 0)
	{
	  // read next header
	  if (len != sizeof(header))
	    {
	      FATAL_MPM_ERROR
		(T_("TarLzmaExtractor::Extract"),
		 T_("Invalid package archive file."),
		 tarlzmaPath.Get());
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
		    (T_("TarLzmaExtractor::Extract"),
		     T_("Invalid package archive file."),
		     tarlzmaPath.Get());
		}
#if defined(NDEBUG)
	      checkHeader = false;
#endif
	    }
	  
	  PathName dest = header.GetFileName();
	  size_t size = header.GetFileSize();
	  
	  if (! header.IsNormalFile())
	    {
	      if (header.GetType() == Header::LongName)
		{
		  if (size >= BLOCKSIZE)
		    {
		      UNEXPECTED_CONDITION ("TarExtractor::Extract");
		    }
		  char longNameData[BLOCKSIZE];
		  if (pLzmaStream->GetData(longNameData, BLOCKSIZE)
		      != BLOCKSIZE)
		    {
		      FATAL_MPM_ERROR (T_("TarLzma2Extractor::Extract"),
				       T_("Invalid package archive file."),
				       0);
		    }
		  longNameData[size] = 0;
		  longName = longNameData;
		  haveLongName = true;
		}
	      else
		{
		  Skip (pLzmaStream,
			(((size + sizeof(Header) - 1) / sizeof(Header))
			 * sizeof(Header)));
		}
	      continue;
	    }

	  if (haveLongName)
	    {
	      dest = longName;
	      haveLongName = false;
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
	      if (pLzmaStream->GetData(buffer, n) != n)
		{
		  FATAL_MPM_ERROR (T_("TarLzmaExtractor::Extract"),
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
	      Skip (pLzmaStream, sizeof(Header) - bytesRead % sizeof(Header));
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
      pLzmaStream->Release ();
      throw;
    }

  pLzmaStream->Release ();
}
