/* FileStreamStream.cpp:

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

#include <CPP/Common/MyWindows.h>
#include <CPP/Common/MyInitGuid.h>
#include <CPP/7zip/ICoder.h>

#include <miktex/lzma.h>

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
	*pProcessedSize = n;
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

   LzmaStream::LzmaStream
   _________________________________________________________________________ */

MIKTEXCALL
LzmaStream::LzmaStream (/*[in]*/ const FileStream &	fileStream,
			/*[in]*/ bool			reading)
  : reading (reading)
  
{
  if (! reading)
    {
      UNIMPLEMENTED (T_("LzmaStream::LzmaStream"));
    }
  LzmaStreamHelper * pHelper = new LzmaStreamHelper (fileStream.Get());
  pHelper->AddRef ();
  p = pHelper;
  pHelper->Start ();
}

/* _________________________________________________________________________

   LzmaStream::~LzmaStream
   _________________________________________________________________________ */

MIKTEXCALL
LzmaStream::~LzmaStream ()
{
  try
    {
      Close ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   LzmaStream::Close
   _________________________________________________________________________ */

void
MIKTEXCALL
LzmaStream::Close ()
{
  if (p != 0)
    {
      LzmaStreamHelper * pHelper = static_cast<LzmaStreamHelper*>(p);
      p = 0;
      pHelper->Release ();
    }
}

/* _________________________________________________________________________

   LzmaStream::Read
   _________________________________________________________________________ */

size_t
MIKTEXCALL
LzmaStream::Read (/*[out]*/ void *	pBytes,
		  /*[in]*/ size_t	count)
{
  LzmaStreamHelper * pHelper = static_cast<LzmaStreamHelper*>(p);
  return (pHelper->GetData(pBytes, count));
}

/* _________________________________________________________________________

   LzmaStream::Write
   _________________________________________________________________________ */

void
MIKTEXCALL
LzmaStream::Write (/*[in]*/ const void *	pBytes,
		   /*[in]*/ size_t		count)
{
  UNUSED_ALWAYS (pBytes);
  UNUSED_ALWAYS (count);
  UNIMPLEMENTED (T_("LzmaStream::Write"));
}

/* _________________________________________________________________________

   LzmaStream::Seek
   _________________________________________________________________________ */

void
MIKTEXCALL
LzmaStream::Seek (/*[in]*/ long		offset,
		  /*[in]*/ SeekOrigin	seekOrigin)
{
  UNUSED_ALWAYS (offset);
  UNUSED_ALWAYS (seekOrigin);
  UNIMPLEMENTED (T_("LzmaStream::Seek"));
}

/* _________________________________________________________________________

   LzmaStream::GetPosition
   _________________________________________________________________________ */

long
MIKTEXCALL
LzmaStream::GetPosition ()
{
  UNIMPLEMENTED (T_("LzmaStream::GetPosition"));
}