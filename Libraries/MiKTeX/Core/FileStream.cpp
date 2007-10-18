/* FileStream.cpp:

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

/* _________________________________________________________________________

   Stream::~Stream
   _________________________________________________________________________ */

MIKTEXCALL
Stream::~Stream ()
{
}

/* _________________________________________________________________________

   FileStream::~FileStream
   _________________________________________________________________________ */

MIKTEXCALL
FileStream::~FileStream ()
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

   FileStream::Attach
   _________________________________________________________________________ */

void
MIKTEXCALL
FileStream::Attach (/*[in]*/ FILE *	pFile)
{
  if (this->pFile != 0)
    {
      Close ();
    }
  this->pFile = pFile;
}

/* _________________________________________________________________________

   FileStream::Close
   _________________________________________________________________________ */

void
MIKTEXCALL
FileStream::Close ()
{
  if (pFile != 0)
    {
      if (pFile != stdin && pFile != stdout && pFile != stderr)
	{
	  fclose (pFile);
	}
      pFile = 0;
    }
}

/* _________________________________________________________________________

   FileStream::Read
   _________________________________________________________________________ */

size_t
MIKTEXCALL
FileStream::Read (/*[out]*/ void *	pBytes,
		  /*[in]*/ size_t	count)
{
  size_t n = fread(pBytes, 1, count, pFile);
  if (ferror(pFile) != 0)
    {
      FATAL_CRT_ERROR ("fread", 0);
    }
  return (n);
}

/* _________________________________________________________________________

   FileStream::Write
   _________________________________________________________________________ */

void
MIKTEXCALL
FileStream::Write (/*[in]*/ const void *	pBytes,
		   /*[in]*/ size_t		count)
{
  if (fwrite(pBytes, 1, count, pFile) != count)
    {
      FATAL_CRT_ERROR ("fwrite", 0);
    }
}

/* _________________________________________________________________________

   FileStream::Seek
   _________________________________________________________________________ */

void
MIKTEXCALL
FileStream::Seek (/*[in]*/ long		offset,
		  /*[in]*/ SeekOrigin	seekOrigin)
{
  int origin;
  switch (seekOrigin.Get())
    {
    case SeekOrigin::Begin:
      origin = SEEK_SET;
      break;
    case SeekOrigin::End:
      origin = SEEK_END;
      break;
    case SeekOrigin::Current:
      origin = SEEK_CUR;
      break;
    default:
      UNEXPECTED_CONDITION ("FileStream::Seek");
      break;
    }
  if (fseek(pFile, offset, origin) != 0)
    {
      FATAL_CRT_ERROR ("fseek", 0);
    }
}

/* _________________________________________________________________________

   FileStream::GetPosition
   _________________________________________________________________________ */

long
MIKTEXCALL
FileStream::GetPosition ()
{
  long pos = ftell(pFile);
  if (pos < 0)
    {
      FATAL_CRT_ERROR ("ftell", 0);
    }
  return (pos);
}

/* _________________________________________________________________________

   FileStream::SetBinary
   _________________________________________________________________________ */

void
MIKTEXCALL
FileStream::SetBinary ()
{
#if defined(MIKTEX_WINDOWS)
  if (_setmode(_fileno(pFile), _O_BINARY) < 0)
    {
      FATAL_CRT_ERROR (_T("_setmode"), 0);
    }
#endif
}

/* _________________________________________________________________________

   BZip2Stream::BZip2Stream
   _________________________________________________________________________ */

MIKTEXCALL
BZip2Stream::BZip2Stream (/*[in]*/ const FileStream &	fileStream,
			  /*[in]*/ bool			reading)
  : reading (reading)
{
  if (reading)
    {
      int bzError;
      pBzFile =
	BZ2_bzReadOpen(&bzError,
		       fileStream.Get(),
		       0,
		       0,
		       0,
		       0);
      if (bzError != BZ_OK)
	{
	  UNEXPECTED_CONDITION ("BZip2Stream::BZip2Stream");
	}
    }
  else
    {
      UNIMPLEMENTED ("BZip2Stream::BZip2Stream");
    }
}

/* _________________________________________________________________________

   BZip2Stream::~BZip2Stream
   _________________________________________________________________________ */

MIKTEXCALL
BZip2Stream::~BZip2Stream ()
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

   BZip2Stream::Close
   _________________________________________________________________________ */

void
MIKTEXCALL
BZip2Stream::Close ()
{
  BZFILE * pBzFile = static_cast<BZFILE*>(this->pBzFile);
  if (pBzFile != 0)
    {
      this->pBzFile = 0;
      int bzError;
      if (reading)
	{
	  BZ2_bzReadClose (&bzError, pBzFile);
	  if (bzError != BZ_OK)
	    {
	      UNEXPECTED_CONDITION ("BZip2Stream::Close");
	    }
	}
      else
	{
	  UNIMPLEMENTED ("BZip2Stream::Close");
	}
    }
}

/* _________________________________________________________________________

   BZip2Stream::Read
   _________________________________________________________________________ */

size_t
MIKTEXCALL
BZip2Stream::Read (/*[out]*/ void *	pBytes,
		   /*[in]*/ size_t	count)
{
  BZFILE * pBzFile = static_cast<BZFILE*>(this->pBzFile);
  int bzError;
  int n = BZ2_bzRead(&bzError, pBzFile, pBytes, static_cast<int>(count));
  if (bzError != BZ_OK && bzError != BZ_STREAM_END)
    {
      UNEXPECTED_CONDITION ("BZip2Stream::Read");
    }
  return (n);
}

/* _________________________________________________________________________

   BZip2Stream::Write
   _________________________________________________________________________ */

void
MIKTEXCALL
BZip2Stream::Write (/*[in]*/ const void *	pBytes,
		    /*[in]*/ size_t		count)
{
  UNUSED_ALWAYS (pBytes);
  UNUSED_ALWAYS (count);
  UNIMPLEMENTED ("BZip2Stream::Write");
}

/* _________________________________________________________________________

   BZip2Stream::Seek
   _________________________________________________________________________ */

void
MIKTEXCALL
BZip2Stream::Seek (/*[in]*/ long	offset,
		   /*[in]*/ SeekOrigin	seekOrigin)
{
  UNUSED_ALWAYS (offset);
  UNUSED_ALWAYS (seekOrigin);
  UNIMPLEMENTED ("BZip2Stream::Seek");
}

/* _________________________________________________________________________

   BZip2Stream::GetPosition
   _________________________________________________________________________ */

long
MIKTEXCALL
BZip2Stream::GetPosition ()
{
  UNIMPLEMENTED ("BZip2Stream::GetPosition");
}

/* _________________________________________________________________________

   StreamReader::StreamReader
   _________________________________________________________________________ */

MIKTEXCALL
StreamReader::StreamReader (/*[in]*/ const PathName & path)
  : stream (File::Open(path, FileMode::Open, FileAccess::Read))
{
}

/* _________________________________________________________________________

   StreamReader::StreamReader
   _________________________________________________________________________ */

MIKTEXCALL
StreamReader::StreamReader (/*[in]*/ const char * lpszPath)
  : stream (File::Open(lpszPath, FileMode::Open, FileAccess::Read))
{
}

/* _________________________________________________________________________

   StreamReader::~StreamReader
   _________________________________________________________________________ */

MIKTEXCALL
StreamReader::~StreamReader ()
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

   StreamReader::Close
   _________________________________________________________________________ */

void
MIKTEXCALL
StreamReader::Close ()
{
  stream.Close ();
}

/* _________________________________________________________________________

   StreamReader::ReadLine
   _________________________________________________________________________ */

bool
MIKTEXCALL
StreamReader::ReadLine (/*[out]*/ string & line)
{
  return (Utils::ReadLine(line, stream.Get(), false));
}

/* _________________________________________________________________________

   StreamWriter::StreamWriter
   _________________________________________________________________________ */

MIKTEXCALL
StreamWriter::StreamWriter (/*[in]*/ const PathName & path)
  : stream (File::Open(path, FileMode::Create, FileAccess::Write))
{
}

/* _________________________________________________________________________

   StreamWriter::~StreamWriter
   _________________________________________________________________________ */

MIKTEXCALL
StreamWriter::~StreamWriter ()
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

   StreamWriter::Close
   _________________________________________________________________________ */

void
MIKTEXCALL
StreamWriter::Close ()
{
  stream.Close ();
}

/* _________________________________________________________________________

   FPutC
   _________________________________________________________________________ */

inline
int
FPutC (/*[in]*/ int	ch,
       /*[in]*/ FILE *	stream)
{
  int chWritten = fputc(ch, stream);
  if (chWritten == EOF)
    {
      FATAL_CRT_ERROR ("fputc", 0);
    }
  return (chWritten);
}

/* _________________________________________________________________________

   FPutS
   _________________________________________________________________________ */

inline
void
FPutS (/*[in]*/ const char *	lpsz,
       /*[in]*/ FILE *		stream)
{
  int ok = fputs(lpsz, stream);
  if (ok < 0)
    {
      FATAL_CRT_ERROR ("fputs", 0);
    }
}

/* _________________________________________________________________________

   StreamWriter::Write
   _________________________________________________________________________ */

void
MIKTEXCALL
StreamWriter::Write (/*[in]*/ char	ch)
{
  FPutC (ch, stream.Get());
}

/* _________________________________________________________________________

   StreamWriter::Write
   _________________________________________________________________________ */

void
StreamWriter::Write (/*[in]*/ const string & line)
{
  FPutS (line.c_str(), stream.Get());
}

/* _________________________________________________________________________

   StreamWriter::WriteLine
   _________________________________________________________________________ */

void
MIKTEXCALL
StreamWriter::WriteLine (/*[in]*/ const string & line)
{
  Write (line);
  WriteLine ();
}

/* _________________________________________________________________________

   StreamWriter::WriteLine
   _________________________________________________________________________ */

void
MIKTEXCALL
StreamWriter::WriteLine ()
{
  FPutC ('\n', stream.Get());
}

/* _________________________________________________________________________

   StreamWriter::WriteFormatted
   _________________________________________________________________________ */

void
MIKTEXCALL
StreamWriter::WriteFormatted (/*[in]*/ const char *	lpszFormat,
			      /*[in]*/			...)
{
  va_list marker;
  va_start (marker, lpszFormat);
  Write (Utils::FormatString(lpszFormat, marker));
  va_end (marker);
}

/* _________________________________________________________________________

   StreamWriter::WriteFormattedLine
   _________________________________________________________________________ */

void
MIKTEXCALL
StreamWriter::WriteFormattedLine (/*[in]*/ const char *	lpszFormat,
				  /*[in]*/		...)
{
  va_list marker;
  va_start (marker, lpszFormat);
  WriteLine (Utils::FormatString(lpszFormat, marker));
  va_end (marker);
}

#if 0
/* _________________________________________________________________________

   StdioFile::~StdioFile
   _________________________________________________________________________ */

MIKTEXCALL
StdioFile::~StdioFile ()
{
  if (pFile != 0)
    {
      if (owningHandle)
	{
	  fclose (pFile);
	}
      pFile = 0;
    }
}
#endif
