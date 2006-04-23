/* util.cpp: generic utilities

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

/* _________________________________________________________________________

   CSVList::CSVList
   _________________________________________________________________________ */

CSVList::CSVList ()
  : lpszList(0),
    lpszNext(0),
    lpszCurrent(0)
{
}

/* _________________________________________________________________________

   CSVList::CSVList
   _________________________________________________________________________ */

CSVList::CSVList (/*[in]*/ const MIKTEXCHAR *	lpszValueList,
		  /*[in]*/ MIKTEXCHARINT	separator)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszValueList);

  if (lpszValueList == 0 || *lpszValueList == 0)
    {
      lpszList = lpszNext = lpszCurrent = 0;
    }
  else
    {
      this->separator = static_cast<MIKTEXCHAR>(separator);
      lpszList = StrDup(lpszValueList);
      if (lpszList == 0)
	{
	  OUT_OF_MEMORY (T_("MiKTeX::Core::CSVList::CSVList"));
	}
      lpszCurrent = lpszList;
      lpszNext = StrChr(lpszList, separator);
      if (lpszNext != 0)
	{
	  *lpszNext++ = 0;
	}
    }
}

/* _________________________________________________________________________

   CSVList::~CSVList
   _________________________________________________________________________ */

CSVList::~CSVList ()
{
  if (lpszList != 0)
    {
      free (lpszList);
      lpszList = 0;
    }
}

/* _________________________________________________________________________

   CSVList::operator++
   _________________________________________________________________________ */

const MIKTEXCHAR *
CSVList::operator++ ()
{
  if (lpszNext != 0 && *lpszNext != 0)
    {
      lpszCurrent = lpszNext;
      lpszNext = StrChr(lpszNext, separator);
      if (lpszNext != 0)
	{
	  *lpszNext++ = 0;
	}
    }
  else
    {
      lpszCurrent = 0;
    }
  return (GetCurrent());
}

/* _________________________________________________________________________

   CSVList2::CSVList2
   _________________________________________________________________________ */

CSVList2::CSVList2 ()
  : lpszNext (0),
    lpszCurrent (0)
{
}

/* _________________________________________________________________________

   CSVList2::CSVList2
   _________________________________________________________________________ */

CSVList2::CSVList2 (/*[in]*/ const MIKTEXCHAR *	lpszValueList,
		    /*[in]*/ MIKTEXCHARINT	separator)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszValueList);

  if (lpszValueList == 0 || *lpszValueList == 0)
    {
      lpszNext = lpszCurrent = 0;
    }
  else
    {
      this->separator = static_cast<MIKTEXCHAR>(separator);
      CopyString2 (buffer, BUFSIZE, lpszValueList, BUFSIZE - 1);
      lpszCurrent = buffer;
      lpszNext = StrChr(buffer, separator);
      if (lpszNext != 0)
	{
	  *lpszNext++ = 0;
	}
    }
}

/* _________________________________________________________________________

   CSVList2::operator++
   _________________________________________________________________________ */

const MIKTEXCHAR *
CSVList2::operator++ ()
{
  if (lpszNext != 0 && *lpszNext != 0)
    {
      lpszCurrent = lpszNext;
      lpszNext = StrChr(lpszNext, separator);
      if (lpszNext != 0)
	{
	  *lpszNext++ = 0;
	}
    }
  else
    {
      lpszCurrent = 0;
    }
  return (GetCurrent());
}

/* _________________________________________________________________________

   Tokenizer::Tokenizer
   _________________________________________________________________________ */

Tokenizer::Tokenizer (/*[in]*/ const MIKTEXCHAR *	lpsz,
		      /*[in]*/ const MIKTEXCHAR *	lpszDelim)
{
  MIKTEX_ASSERT_STRING (lpsz);
  lpszOrig = StrDup(lpsz);
  if (lpszOrig == 0)
    {
      OUT_OF_MEMORY (T_("Tokenizer::Tokenizer"));
    }
  lpszNext = lpszOrig;
  lpszCurrent = 0;
  pDelims = 0;
  SetDelim (lpszDelim);
  FindToken ();
}

/* _________________________________________________________________________

   Tokenizer::SetDelim
   _________________________________________________________________________ */

void
Tokenizer::SetDelim (/*[in]*/ const MIKTEXCHAR * lpszDelim)
{
  bitset<256> * psetDelim = reinterpret_cast<bitset<256>*>(pDelims);
  if (psetDelim == 0)
    {
      psetDelim = new bitset<256>;
      pDelims = psetDelim;
    }
  psetDelim->reset ();
  MIKTEX_ASSERT_STRING (lpszDelim);
  for ( ; *lpszDelim != 0; ++ lpszDelim)
    {
      psetDelim->set (static_cast<MIKTEXUCHAR>(*lpszDelim));
    }
}

/* _________________________________________________________________________

   Tokenizer::operator++
   _________________________________________________________________________ */

const MIKTEXCHAR *
Tokenizer::operator++ ()
{
  FindToken ();
  return (GetCurrent());
}

/* _________________________________________________________________________

   Tokenizer::~Tokenizer
   _________________________________________________________________________ */

Tokenizer::~Tokenizer ()
{
  try
    {
      MIKTEX_ASSERT_STRING (lpszOrig);
      free (lpszOrig);
      lpszOrig = 0;
      if (pDelims != 0)
	{
	  bitset<256> * pDelims =
	    reinterpret_cast<bitset<256>*>(this->pDelims);
	  this->pDelims = 0;
	  delete pDelims;
	}
      lpszCurrent = 0;
      lpszNext = 0;
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   Tokenizer::FindToken
   _________________________________________________________________________ */

void
Tokenizer::FindToken ()
{
  lpszCurrent = lpszNext;
  bitset<256> * pDelims = reinterpret_cast<bitset<256>*>(this->pDelims);
  MIKTEX_ASSERT (pDelims != 0);
  while ((*pDelims)[static_cast<MIKTEXUCHAR>(*lpszCurrent)]
	 && *lpszCurrent != 0)
    {
      ++ lpszCurrent;
    }
  for (lpszNext = const_cast<MIKTEXCHAR *>(lpszCurrent);
       *lpszNext != 0;
       ++ lpszNext)
    {
      if ((*pDelims)[static_cast<MIKTEXUCHAR>(*lpszNext)])
	{
	  *lpszNext = 0;
	  ++ lpszNext;
	  break;
	}
    }
}

/* _________________________________________________________________________

   Utils::Contains
   _________________________________________________________________________ */

bool
Utils::Contains (/*[in]*/ const MIKTEXCHAR *	lpszList,
		 /*[in]*/ const MIKTEXCHAR *	lpszElement,
		 /*[in]*/ const MIKTEXCHAR *	lpszDelims)
{
  for (Tokenizer tok (lpszList, lpszDelims); tok.GetCurrent() != 0; ++ tok)
    {
      if (StringCompare(tok.GetCurrent(), lpszElement, true) == 0)
	{
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   Utils::AppendString
   _________________________________________________________________________ */

size_t
Utils::AppendString (/*[in,out]*/ MIKTEXCHAR *		lpszBuf,
		     /*[in]*/ size_t			bufSize,
		     /*[in]*/ const MIKTEXCHAR *	lpszSource)
{
  MIKTEX_ASSERT_STRING (lpszBuf);
  MIKTEX_ASSERT_CHAR_BUFFER (lpszBuf, bufSize);
  MIKTEX_ASSERT_STRING (lpszSource);

  size_t length;

  for (length = 0; length < bufSize && lpszBuf[length] != 0; ++ length)
    {
      ;
    }
  
  if (length == bufSize)
    {
      INVALID_ARGUMENT (T_("MiKTeX::Core::Utils::AppendString"), lpszBuf);
    }

  length += CopyString(&lpszBuf[length], bufSize - length, lpszSource);

  return (length);
}

/* _________________________________________________________________________

   Utils::CopyString
   _________________________________________________________________________ */

size_t
Utils::CopyString (/*[out]*/ MIKTEXCHAR *	lpszBuf,
		   /*[in]*/ size_t		bufSize,
		   /*[in]*/ const MIKTEXCHAR *	lpszSource)
{
  MIKTEX_ASSERT_CHAR_BUFFER (lpszBuf, bufSize);
  MIKTEX_ASSERT_STRING (lpszSource);

  size_t length;

  for (length = 0; length < bufSize; ++ length)
    {
      if ((lpszBuf[length] = lpszSource[length]) == 0)
	{
	  break;
	}
    }

  if (length == bufSize)
    {
      TraceError (T_("Utils::CopyString() is going to throw an exception"));
      TraceError (T_("  bufSize=%u"), static_cast<unsigned>(bufSize));
      TraceError (T_("  lpszSource=%.*s..."),
		  static_cast<int>(bufSize),
		  lpszSource);
      BUF_TOO_SMALL (T_("Utils::CopyString"));
    }

  return (length);
}

/* _________________________________________________________________________

   CopyString2
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
CopyString2 (/*[out]*/ MIKTEXCHAR *		lpszBuf,
	     /*[in]*/ size_t			bufSize,
	     /*[in]*/ const MIKTEXCHAR *	lpszSource,
	     /*[in]*/ size_t			count)
{
  MIKTEX_ASSERT_CHAR_BUFFER (lpszBuf, bufSize);
  MIKTEX_ASSERT_STRING (lpszSource);

  *lpszBuf = 0;

  if (count == 0)
    {
      return;
    }

  MIKTEXCHAR * lpsz = lpszBuf;

  while (bufSize > 0
	 && count > 0
	 && (*lpsz++ = *lpszSource++) != 0)
    {
      -- bufSize;
      -- count;
    }

  if (bufSize == 0)
    {
      TraceError (T_("CopyString2() is going to throw an exception"));
      TraceError (T_("  bufSize=%u"), static_cast<unsigned>(bufSize));
      TraceError (T_("  count=%u"), static_cast<unsigned>(count));
      TraceError (T_("  lpszSource=%.*s..."),
		  static_cast<int>(bufSize),
		  lpszSource);
      BUF_TOO_SMALL (T_("CopyString2"));
    }

  if (count == 0)
    {
      *lpsz = 0;
    }
}

/* _________________________________________________________________________

   Utils::ReplaceString
   _________________________________________________________________________ */

void
Utils::ReplaceString (/*[out]*/ MIKTEXCHAR *		lpszBuf,
		      /*[in,out]*/ size_t &		bufSize,
		      /*[in]*/ const MIKTEXCHAR *	lpszSource,
		      /*[in]*/ const MIKTEXCHAR *	lpszString1,
		      /*[in]*/ const MIKTEXCHAR *	lpszString2)
{
  MIKTEX_ASSERT_STRING (lpszSource);
  MIKTEX_ASSERT_STRING (lpszString1);
  MIKTEX_ASSERT_STRING (lpszString2);
  if (bufSize == 0)
    {
      MIKTEX_ASSERT (lpszBuf == 0);
    }
  else
    {
      MIKTEX_ASSERT_CHAR_BUFFER (lpszBuf, bufSize);
    }
  while (*lpszSource != 0)
    {
      size_t n;
      for (n = 0;
	   (lpszSource[n] !=0
	    && lpszString1[n] != 0
	    && lpszSource[n] == lpszString1[n]);
	   ++ n)
	{
	  ;
	}
      if (n == 0)
	{
	  while (*lpszSource != 0 && *lpszSource != lpszString1[0])
	    {
	      if (lpszBuf == 0)
		{
		  bufSize += 1;
		}
	      else
		{
		  *lpszBuf = *lpszSource;
		  ++ lpszBuf;
		  bufSize -= 1;
		  if (bufSize == 0)
		    {
		      BUF_TOO_SMALL (T_("MiKTeX::Core::Utils::ReplaceString"));
		    }
		}
	      ++ lpszSource;
	    }
	}
      else if (lpszString1[n] == 0)
	{
	  lpszSource += n;
	  n = StrLen(lpszString2);
	  if (lpszBuf == 0)
	    {
	      bufSize += n;
	    }
	  else
	    {
	      Utils::CopyString (lpszBuf, bufSize, lpszString2);
	      lpszBuf += n;
	      bufSize -= n;
	    }
	}
      else
	{
	  if (lpszBuf == 0)
	    {
	      bufSize += n;
	    }
	  else
	    {
	      CopyString2 (lpszBuf, bufSize, lpszSource, n);
	      lpszBuf += n;
	      bufSize -= n;
	    }
	  lpszSource += n;
	}
    }
  if (lpszBuf == 0)
    {
      ++ bufSize;
    }
  else
    {
      *lpszBuf = 0;
    }
}

/* _________________________________________________________________________

   MD5::FromFile
   _________________________________________________________________________ */

MD5
MIKTEXCALL
MD5::FromFile (/*[in]*/ const MIKTEXCHAR *	lpszPath)
{
  auto_ptr<MemoryMappedFile> pFile (MemoryMappedFile::Create());
  MD5Builder md5Builder;
  md5Builder.Init ();
  const void * ptr = pFile->Open(lpszPath, false);
  md5Builder.Update (ptr, pFile->GetSize());
  md5Builder.Final ();
  return (md5Builder.GetMD5());
}

/* _________________________________________________________________________

   Unhex
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(int)
Unhex (/*[in]*/ MIKTEXCHAR x)
{
  if (x >= T_('0') && x <= T_('9'))
    {
      return (x - T_('0'));
    }
  else if (x >= T_('A') && x <= T_('F'))
    {
      return (x - T_('A') + 10);
    }
  else if (x >= T_('a') && x <= T_('f'))
    {
      return (x - T_('a') + 10);
    }
  else
    {
      FATAL_MIKTEX_ERROR (T_("Unhex"),
			  T_("Hex digit expected."),
			  0);
    }
}

/* _________________________________________________________________________

   MD5::Parse
   _________________________________________________________________________ */

MD5
MIKTEXCALL
MD5::Parse (/*[in]*/ const MIKTEXCHAR *	lpszHexString)
{
  size_t l = StrLen(lpszHexString);
  if (l != 32)
    {
      FATAL_MIKTEX_ERROR (T_("MD5::Parse"), T_("Invalid MD5."), lpszHexString);
    }
  MD5 md5;
  for (size_t i = 0; i < 16; ++i)
    {
      int x1 = Unhex(lpszHexString[i * 2]);
      int x2 = Unhex(lpszHexString[i * 2 + 1]);
      md5.bits[i] = static_cast<unsigned char>(x1 << 4);
      md5.bits[i] |= x2;
    }
  return (md5);
}

/* _________________________________________________________________________

   Utils::FormatString
   _________________________________________________________________________ */

tstring
MIKTEXCALL
Utils::FormatString (/*[in]*/ const MIKTEXCHAR *  lpszFormat,
		     /*[in]*/ va_list		  arglist)
{
  AutoBuffer autoBuffer;
  int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
  n = _vsctprintf(lpszFormat, arglist);
  if (n < 0)
  {
    FATAL_CRT_ERROR (T_("_vsctprintf"), 0);
  }
  autoBuffer.Resize (n + 1);
  n = _vstprintf_s(
    autoBuffer.GetBuffer(),
    autoBuffer.GetSize(),
    lpszFormat,
    arglist);
  if (n < 0)
  {
    FATAL_CRT_ERROR (T_("_vsntprintf_s"), 0);
  }
  else if (static_cast<size_t>(n) >= autoBuffer.GetSize())
  {
    FATAL_MIKTEX_ERROR (
      T_("Utils::FormatString"),
      T_("Invalid format string or argument list."),
      0);
  }
#  else
  const size_t MAXSIZE = 10000;
  bool done = false;
  do
  {
    n = _vsntprintf(autoBuffer.GetBuffer(),
		    autoBuffer.GetSize(),
		    lpszFormat,
		    arglist);
    if (n < 0)
    {
      FATAL_CRT_ERROR (T_("_vsntprintf"), 0);
    }
    else if (static_cast<size_t>(n) < autoBuffer.GetSize())
    {
      done = true;
    }
    else if (autoBuffer.GetSize() < MAXSIZE)
    {
      autoBuffer.Resize (autoBuffer.GetSize() + 1000);
    }
    else
    {
      FATAL_MIKTEX_ERROR (
	T_("Utils::FormatString"),
	T_("Invalid format string or argument list."),
	0);
    }
  }
  while (! done);
#  endif
#else
  n =
    vsnprintf(autoBuffer.GetBuffer(),
	      autoBuffer.GetSize(),
	      lpszFormat,
	      arglist);
  if (n < 0)
  {
    FATAL_CRT_ERROR (T_("vsnprintf"), 0);
  }
  else if (static_cast<size_t>(n) >= autoBuffer.GetSize())
  {
    autoBuffer.Resize (n + 1);
    n =
      vsnprintf(autoBuffer.GetBuffer(),
		autoBuffer.GetSize(),
		lpszFormat,
		arglist);
    if (n < 0)
    {
      FATAL_CRT_ERROR (T_("vsnprintf"), 0);
    }
    else if (static_cast<size_t>(n) >= autoBuffer.GetSize())
    {
      FATAL_MIKTEX_ERROR (
	T_("Utils::FormatString"),
	T_("Invalid format string or argument list."),
	0);
    }
  }
#endif
  return (autoBuffer.GetBuffer());
}

/* _________________________________________________________________________

   PathNameParser::PathNameParser
   _________________________________________________________________________ */

PathNameParser::PathNameParser (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  Utils::CopyString (buffer, BufferSizes::MaxPath, lpszPath);
  lpszNext = buffer;
  lpszCurrent = 0;
  ++ (*this);
}

/* _________________________________________________________________________

   PathNameParser::operator++
   _________________________________________________________________________ */

const MIKTEXCHAR *
PathNameParser::operator++ ()
{
  lpszCurrent = lpszNext;

  MIKTEXCHAR * lpsz;

  if (lpszCurrent == buffer && IsDirectoryDelimiter(buffer[0]))
    {
      if (IsDirectoryDelimiter(buffer[1]))
	{
	  lpsz = &buffer[2];
	}
      else
	{
	  lpsz = &buffer[1];
	}
    }
  else
    {
      // skip extra directory delimiters
      for (;
	   PathName::IsDirectoryDelimiter(*lpszCurrent);
	   ++ lpszCurrent)
	{
	  ;
	}
      lpsz = lpszCurrent;
    }

  // cut out the next component
  for (lpszNext = lpsz; *lpszNext != 0; ++ lpszNext)
    {
      if (PathName::IsDirectoryDelimiter(*lpszNext))
	{
	  *lpszNext = 0;
	  ++ lpszNext;
	  break;
	}
    }

  return (GetCurrent());
}

/* _________________________________________________________________________

   Hexify
   _________________________________________________________________________ */

MIKTEXAPI(tstring)
Utils::Hexify (/*[in]*/ const void *	pv,
	       /*[in]*/ size_t		nBytes,
	       /*[in]*/ bool		lowerCase)
{
  tstring ret;
#define TOHEX(x) ((x) < 10 ? T_('0') + (x) : (x) - 10 + T_('A'))
#define tohex(x) ((x) < 10 ? T_('0') + (x) : (x) - 10 + T_('a'))
  for (size_t i = 0; i < nBytes; ++ i)
    {
      unsigned char XX = reinterpret_cast<const unsigned char *>(pv)[i];
      ret += (lowerCase ? tohex(XX >> 4) : TOHEX(XX >> 4));
      ret += (lowerCase ? tohex(XX & 15) : TOHEX(XX & 15));
    }
#undef tohex
#undef TOHEX
  return (ret);
}

/* _________________________________________________________________________

   Hexify
   _________________________________________________________________________ */

MIKTEXAPI(tstring)
Utils::Hexify (/*[in]*/ const void *	pv,
	       /*[in]*/ size_t		nBytes)
{
  return (Hexify(pv, nBytes, true));
}

/* _________________________________________________________________________

   miktex_replace_string
   _________________________________________________________________________ */

MIKTEXAPI(void)
miktex_replace_string (/*[out]*/ MIKTEXCHAR *		lpszBuf,
		       /*[in]*/ size_t *		lpSizeDest,
		       /*[in]*/ const MIKTEXCHAR *	lpszSource,
		       /*[in]*/ const MIKTEXCHAR *	lpszString1,
		       /*[in]*/ const MIKTEXCHAR *	lpszString2)
{
  C_FUNC_BEGIN ();

  MIKTEX_ASSERT (lpSizeDest != 0);

  Utils::ReplaceString (lpszBuf,
			*lpSizeDest,
			lpszSource,
			lpszString1,
			lpszString2);

  C_FUNC_END ();
}
