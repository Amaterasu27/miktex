/* misc.cpp: common DVI stuff

   Copyright (C) 1996-2011 Christian Schenk

   This file is part of the MiKTeX DVI Library.

   The MiKTeX DVI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX DVI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the MiKTeX DVI Library; if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
   USA.  */

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   InputStream::InputStream
   _________________________________________________________________________ */

InputStream::InputStream (/*[in]*/ const char * lpszFileName)
  : nBytes (0),
    pBytes (0),
    fileName (lpszFileName),
    idx (0),
    stream (File::Open(lpszFileName,
		       FileMode::Open,
		       FileAccess::Read,
		       false))
{
}

/* _________________________________________________________________________

   InputStream::InputStream
   _________________________________________________________________________ */

InputStream::InputStream (/*[in]*/ const BYTE *	pBytes,
			  /*[in]*/ size_t	nBytes)
{
  this->pBytes = new char[ nBytes ];
  this->nBytes = nBytes;
  memcpy (this->pBytes, pBytes, nBytes);
  this->idx = 0;
}

/* _________________________________________________________________________

   InputStream::~InputStream
   _________________________________________________________________________ */

InputStream::~InputStream ()
{
  try
    {
      if (pBytes != 0)
	{
	  delete [] pBytes;
	  nBytes = 0;
	  pBytes = 0;
	  idx = 0;
	}
      else
	{
	  stream.Close ();
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   InputStream::Read
   _________________________________________________________________________ */

bool
InputStream::Read (/*[out]*/ void *	pBytes,
		   /*[in]*/ size_t	nBytes,
		   /*[in]*/ bool	allowEof)
{
  if (this->pBytes != 0)
    {
      if (IsEndOfStream())
	{
	  if (! allowEof)
	    {
	      FATAL_DVI_ERROR ("InputStream::Read",
			       T_("Unexpected end of stream."),
			       fileName.Get());
	    }
	  else
	    {
	      return (false);
	    }
	}
      else if (idx + nBytes > this->nBytes)
	{
	  UNEXPECTED_CONDITION ("InputStream::Read");
	}
      else
	{
	  memcpy (pBytes, this->pBytes + idx, nBytes);
	  idx += nBytes;
	  return (true);
	}
    }
  else
    {
      if (stream.Read(pBytes, nBytes) != nBytes)
	{
	  if (! allowEof)
	    {
	      FATAL_DVI_ERROR ("InputStream::Read",
			       T_("Unexpected end of file."),
			       fileName.Get());
	    }
	  return (false);
	}
      else
	{
	  return (true);
	}
    }
}

/* _________________________________________________________________________

   InputStream::SkipBytes
   _________________________________________________________________________ */

void
InputStream::SkipBytes (/*[in]*/ long count)
{
  if (pBytes != 0)
    {
      UNEXPECTED_CONDITION ("InputStream::SkipBytes");
    }
  else
    {
      stream.Seek (count, SeekOrigin::Current);
    }
}

/* _________________________________________________________________________

   InputStream::SetReadPosition
   _________________________________________________________________________ */

void
InputStream::SetReadPosition (/*[in]*/ long		offset,
			      /*[in]*/ SeekOrigin	origin)
{
  if (pBytes != 0)
    {
      UNEXPECTED_CONDITION ("InputStream::SkipBytes");
    }
  else
    {
      stream.Seek (offset, origin);
    }
}

/* _________________________________________________________________________

   InputStream::GetReadPosition
   _________________________________________________________________________ */

long
InputStream::GetReadPosition ()
{
  if (pBytes != 0)
    {
      UNEXPECTED_CONDITION ("InputStream::SkipBytes");
    }
  else
    {
      return (stream.GetPosition());
    }
}

/* _________________________________________________________________________

   InputStream::TryToReadByte
   _________________________________________________________________________ */

bool
InputStream::TryToReadByte (/*[out]*/ int & by)
{
  if (! Read(reinterpret_cast<char*>(buffer), 1, true))
    {
      return (false);
    }
  else
    {
      by = (buffer[0] & 0xff);
      return (true);
    }
}

/* _________________________________________________________________________

   DviException::DviException
   _________________________________________________________________________ */

DviException::DviException ()
  : MiKTeXException (0,
		     T_("DVI exception."),
		     0,
		     0,
		     0)
{
}

/* _________________________________________________________________________

   DviException::DviException
   _________________________________________________________________________ */

DviException::DviException
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
 /*[in]*/ int		sourceLine)
  : MiKTeXException (lpszProgramInvocationName,
		     lpszMessage,
		     lpszInfo,
		     lpszSourceFile,
		     sourceLine)
{
}

/* _________________________________________________________________________

   DviFileInUseException::DviFileInUseException
   _________________________________________________________________________ */

DviFileInUseException::DviFileInUseException ()
{
}

/* _________________________________________________________________________

   DviFileInUseException::DviFileInUseException
   _________________________________________________________________________ */

DviFileInUseException::DviFileInUseException
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
 /*[in]*/ int		sourceLine)
  : DviException (lpszProgramInvocationName,
		  lpszMessage,
		  lpszInfo,
		  lpszSourceFile,
		  sourceLine)
{
}

/* _________________________________________________________________________

   DviPageNotFoundException::DviPageNotFoundException
   _________________________________________________________________________ */

DviPageNotFoundException::DviPageNotFoundException ()
{
}

/* _________________________________________________________________________

   DviPageNotFoundException::DviPageNotFoundException
   _________________________________________________________________________ */

DviPageNotFoundException::DviPageNotFoundException
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
 /*[in]*/ int			sourceLine)
  : DviException (lpszProgramInvocationName,
		  lpszMessage,
		  lpszInfo,
		  lpszSourceFile,
		  sourceLine)
{
}

/* _________________________________________________________________________

   ScaleFix
   _________________________________________________________________________ */

int
ScaleFix (/*[in]*/ int		tfm,
	  /*[in]*/ int		z)
{
  int alpha, beta;	// quantities used in the scaling computation
  
  // replace z by z' and compute alpha/beta
  alpha = 16;
  while (z >= 040000000)
    {
      z /= 2;
      alpha += alpha;
    }
  beta = 256 / alpha;
  alpha *= z;
  
  // Convert the width value
  int b0 = (tfm >> 24) & 0xff;
  int b1 = (tfm >> 16) & 0xff;
  int b2 = (tfm >> 8) & 0xff;
  int b3 = tfm & 0xff;
  
  MIKTEX_ASSERT (beta != 0);

  tfm = (((((b3 * z) / 0400) + (b2 * z)) / 0400) + (b1 * z)) / beta;

  if (b0 == 255)
    {
      tfm -= alpha;
    }

  return (tfm);
}

/* _________________________________________________________________________

   FatalDviError
   _________________________________________________________________________ */

void
MIKTEXNORETURN
FatalDviError (/*[in]*/ const char *	lpszMiktexFunction,
	       /*[in]*/ const char *	lpszMessage,
	       /*[in]*/ const char *	lpszInfo,
	       /*[in]*/ const char *	lpszSourceFile,
	       /*[in]*/ int		sourceLine)
{
  UNUSED_ALWAYS (lpszMiktexFunction);
  throw DviException (0,
		      lpszMessage,
		      lpszInfo,
		      lpszSourceFile,
		      sourceLine);
}

/* _________________________________________________________________________

   GraphicsInclusion::~GraphicsInclusion
   _________________________________________________________________________ */

GraphicsInclusion::~GraphicsInclusion ()
{
}

/* _________________________________________________________________________

   GraphicsInclusionImpl::LoadEnhMetaFile

   Borrowed from the mfedit sample.
   _________________________________________________________________________ */

HENHMETAFILE
GraphicsInclusionImpl::LoadEnhMetaFile (/*[in]*/ const PathName & fileName)
{
  HENHMETAFILE hEmf = 0;

  const DWORD META32_SIGNATURE = 0x464D4520;      // ' EMF'
  const DWORD ALDUS_ID = 0x9AC6CDD7;
  const size_t APMSIZE = 22;

  HANDLE hFile = CreateFile(fileName.Get(),
    GENERIC_READ,
    FILE_SHARE_READ,
    0,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_READONLY,
    0);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    FATAL_WINDOWS_ERROR ("CreateFile", fileName.Get());
  }

  AutoHANDLE autoCloseFile (hFile);

  HANDLE hMapFile = CreateFileMapping(hFile,
    0,
    PAGE_READONLY,
    0,
    0,
    "MIKEMF");

  if (hMapFile == 0)
  {
    FATAL_WINDOWS_ERROR ("CreateFileMapping", fileName.Get());
  }

  AutoHANDLE autoCloseFileMapping (hMapFile);

  void * pMapFile = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

  if (pMapFile == 0)
  {
    FATAL_WINDOWS_ERROR ("MapViewOfFile", fileName.Get());
  }

  AutoUnmapViewOfFile autoUnmap (pMapFile);

  LPENHMETAHEADER pEmh = reinterpret_cast<LPENHMETAHEADER>(pMapFile);

  if (pEmh->dSignature == META32_SIGNATURE)
  {
    hEmf = GetEnhMetaFile(fileName.Get());
    if (hEmf == 0)
    {
      FATAL_MIKTEX_ERROR ("GraphicsInclusionImpl::LoadEnhMetaFile",
	T_("The metafile could not be loaded."),
	fileName.Get());
    }
    return (hEmf);
  }
    
  if (*reinterpret_cast<LPDWORD>(pEmh) == ALDUS_ID)
  {
    DWORD size = *reinterpret_cast<LPDWORD>(reinterpret_cast<PBYTE>(pMapFile)
      + APMSIZE
      + 6);
    // <fixme>use device context</fixme>
    hEmf = SetWinMetaFileBits(size * 2,
      reinterpret_cast<PBYTE>(pMapFile) + APMSIZE,
      0,
      0);
    if (hEmf == 0)
    {
      FATAL_MIKTEX_ERROR ("GraphicsInclusionImpl::LoadEnhMetaFile",
	T_("The metafile could not be loaded."),
	fileName.Get());
    }
    return (hEmf);
  }

  HMETAFILE hMf = GetMetaFile(fileName.Get());

  if (hMf == 0)
  {
    FATAL_MIKTEX_ERROR ("GraphicsInclusionImpl::LoadEnhMetaFile",
      T_("The metafile could not be loaded."),
      fileName.Get());
  }

  AutoDeleteMetaFile autoDeleteMetaFile (hMf);
    
  UINT size = GetMetaFileBitsEx(hMf, 0, 0);

  if (size == 0)
  {
    FATAL_MIKTEX_ERROR ("GraphicsInclusionImpl::LoadEnhMetaFile",
      T_("The metafile could not be loaded."),
      fileName.Get());
  }
    
  void * pvData = malloc(size);

  if (pvData == 0)
  {
    OUT_OF_MEMORY ("GraphicsInclusionImpl::LoadEnhMetaFile");
  }

  AutoMemoryPointer autoFree (pvData);
      
  size = GetMetaFileBitsEx(hMf, size, pvData);

  if (size == 0)
  {
    FATAL_MIKTEX_ERROR ("GraphicsInclusionImpl::LoadEnhMetaFile",
      T_("The metafile could not be loaded."),
      fileName.Get());
  }
    
  hEmf =
    SetWinMetaFileBits
    (size,
     const_cast<const PBYTE>(reinterpret_cast<PBYTE>(pvData)),
     0,
     0);

  if (hEmf == 0)
  {
    FATAL_MIKTEX_ERROR ("GraphicsInclusionImpl::LoadEnhMetaFile",
      T_("The metafile could not be loaded."),
      fileName.Get());
  }

  return (hEmf);
}

/* _________________________________________________________________________

   TempFile::~TempFile
   _________________________________________________________________________ */

TempFile::~TempFile ()
{
}

/* _________________________________________________________________________

   TempFile::Create
   _________________________________________________________________________ */

TempFile *
TempFile::Create ()
{
  return (new TempFileImpl());
}

/* _________________________________________________________________________

   TempFile::Create
   _________________________________________________________________________ */

TempFile *
TempFile::Create (/*[in]*/ const PathName & path)
{
  return (new TempFileImpl(path));
}
