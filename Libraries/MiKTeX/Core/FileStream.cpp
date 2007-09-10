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

   FileStream::~FileStream
   _________________________________________________________________________ */

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
FileStream::Read (/*[out]*/ void *	pBytes,
		  /*[in]*/ size_t	count)
  const
{
  size_t n = fread(pBytes, 1, count, pFile);
  if (ferror(pFile) != 0)
    {
      FATAL_CRT_ERROR (T_("fread"), 0);
    }
  return (n);
}

/* _________________________________________________________________________

   FileStream::Write
   _________________________________________________________________________ */

void
FileStream::Write (/*[in]*/ const void *	pBytes,
		   /*[in]*/ size_t		count)
  const
{
  if (fwrite(pBytes, 1, count, pFile) != count)
    {
      FATAL_CRT_ERROR (T_("fwrite"), 0);
    }
}

/* _________________________________________________________________________

   FileStream::Seek
   _________________________________________________________________________ */

void
FileStream::Seek (/*[in]*/ long		offset,
		  /*[in]*/ SeekOrigin	seekOrigin)
  const
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
      UNEXPECTED_CONDITION (T_("FileStream::Seek"));
      break;
    }
  if (fseek(pFile, offset, origin) != 0)
    {
      FATAL_CRT_ERROR (T_("fseek"), 0);
    }
}

/* _________________________________________________________________________

   FileStream::GetPosition
   _________________________________________________________________________ */

long
FileStream::GetPosition ()
  const
{
  long pos = ftell(pFile);
  if (pos < 0)
    {
      FATAL_CRT_ERROR (T_("ftell"), 0);
    }
  return (pos);
}

/* _________________________________________________________________________

   FileStream::SetBinary
   _________________________________________________________________________ */

void
FileStream::SetBinary ()
  const
{
#if defined(MIKTEX_WINDOWS)
  if (_setmode(_fileno(pFile), _O_BINARY) < 0)
    {
      FATAL_CRT_ERROR (_T("_setmode"), 0);
    }
#endif
}

/* _________________________________________________________________________

   StreamReader::StreamReader
   _________________________________________________________________________ */

StreamReader::StreamReader (/*[in]*/ const PathName & path)
  : stream (File::Open(path, FileMode::Open, FileAccess::Read))
{
}

/* _________________________________________________________________________

   StreamReader::StreamReader
   _________________________________________________________________________ */

StreamReader::StreamReader (/*[in]*/ const MIKTEXCHAR * lpszPath)
  : stream (File::Open(lpszPath, FileMode::Open, FileAccess::Read))
{
}

/* _________________________________________________________________________

   StreamReader::~StreamReader
   _________________________________________________________________________ */

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
StreamReader::Close ()
{
  stream.Close ();
}

/* _________________________________________________________________________

   StreamReader::ReadLine
   _________________________________________________________________________ */

bool
StreamReader::ReadLine (/*[out]*/ tstring & line)
{
  return (Utils::ReadLine(line, stream.Get(), false));
}

/* _________________________________________________________________________

   StreamWriter::StreamWriter
   _________________________________________________________________________ */

StreamWriter::StreamWriter (/*[in]*/ const PathName & path)
  : stream (File::Open(path, FileMode::Create, FileAccess::Write))
{
}

/* _________________________________________________________________________

   StreamWriter::~StreamWriter
   _________________________________________________________________________ */

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
StreamWriter::Close ()
{
  stream.Close ();
}

/* _________________________________________________________________________

   FPutC
   _________________________________________________________________________ */

inline
MIKTEXCHARINT
FPutC (/*[in]*/ MIKTEXCHARINT	ch,
       /*[in]*/ FILE *		stream)
{
#if defined(_MSC_VER)
  MIKTEXCHARINT chWritten = _fputtc(ch, stream);
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: FPutC()
#else
  int chWritten = fputc(ch, stream);
#endif
  if (chWritten == MIKTEXEOF)
    {
      FATAL_CRT_ERROR (T_("fputc"), 0);
    }
  return (chWritten);
}

/* _________________________________________________________________________

   FPutS
   _________________________________________________________________________ */

inline
void
FPutS (/*[in]*/ const MIKTEXCHAR *	lpsz,
       /*[in]*/ FILE *			stream)
{
#if defined(_MSC_VER)
  int ok = _fputts(lpsz, stream);
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: FPutS()
#else
  int ok = fputs(lpsz, stream);
#endif
  if (ok < 0)
    {
      FATAL_CRT_ERROR (T_("fputs"), 0);
    }
}

/* _________________________________________________________________________

   StreamWriter::Write
   _________________________________________________________________________ */

void
StreamWriter::Write (/*[in]*/ MIKTEXCHAR	ch)
{
  FPutC (ch, stream.Get());
}

/* _________________________________________________________________________

   StreamWriter::Write
   _________________________________________________________________________ */

void
StreamWriter::Write (/*[in]*/ const tstring & line)
{
  FPutS (line.c_str(), stream.Get());
}

/* _________________________________________________________________________

   StreamWriter::WriteLine
   _________________________________________________________________________ */

void
StreamWriter::WriteLine (/*[in]*/ const tstring & line)
{
  Write (line);
  WriteLine ();
}

/* _________________________________________________________________________

   StreamWriter::WriteLine
   _________________________________________________________________________ */

void
StreamWriter::WriteLine ()
{
  FPutC (T_('\n'), stream.Get());
}

/* _________________________________________________________________________

   StreamWriter::WriteFormatted
   _________________________________________________________________________ */

void
StreamWriter::WriteFormatted (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
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
StreamWriter::WriteFormattedLine (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
				  /*[in]*/			...)
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
