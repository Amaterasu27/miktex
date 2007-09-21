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

   Tokenizer::Tokenizer
   _________________________________________________________________________ */

Tokenizer::Tokenizer (/*[in]*/ const char *	lpsz,
		      /*[in]*/ const char *	lpszDelim)
  : CharBuffer(lpsz)
{
  lpszNext = GetBuffer();
  lpszCurrent = 0;
  pDelims = 0;
  SetDelim (lpszDelim);
  FindToken ();
}

/* _________________________________________________________________________

   Tokenizer::SetDelim
   _________________________________________________________________________ */

void
Tokenizer::SetDelim (/*[in]*/ const char * lpszDelim)
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
      psetDelim->set (static_cast<unsigned char>(*lpszDelim));
    }
}

/* _________________________________________________________________________

   Tokenizer::operator++
   _________________________________________________________________________ */

const char *
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
  MIKTEX_ASSERT (lpszNext != 0);
  lpszCurrent = lpszNext;
  bitset<256> * pDelims = reinterpret_cast<bitset<256>*>(this->pDelims);
  MIKTEX_ASSERT (pDelims != 0);
  while ((*pDelims)[static_cast<unsigned char>(*lpszCurrent)]
	 && *lpszCurrent != 0)
    {
      ++ lpszCurrent;
    }
  for (lpszNext = const_cast<char*>(lpszCurrent);
       *lpszNext != 0;
       ++ lpszNext)
    {
      if ((*pDelims)[static_cast<unsigned char>(*lpszNext)])
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
Utils::CopyString (/*[out]*/ char *		lpszBuf,
		   /*[in]*/ size_t		bufSize,
		   /*[in]*/ const char *	lpszSource)
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
      BUF_TOO_SMALL (T_("Utils::CopyString"));
    }

  return (length);
}

/* _________________________________________________________________________

   Utils::CopyString
   _________________________________________________________________________ */

size_t
Utils::CopyString (/*[out]*/ char *		lpszBuf,
		   /*[in]*/ size_t		bufSize,
		   /*[in]*/ const wchar_t *	lpszSource)
{
  MIKTEX_ASSERT_CHAR_BUFFER (lpszBuf, bufSize);
  MIKTEX_ASSERT_STRING (lpszSource);

#if defined(MIKTEX_WINDOWS)
  int n = WideCharToMultiByte
    (CP_ACP,
     WC_NO_BEST_FIT_CHARS,
     lpszSource,
     -1,
     lpszBuf,
     bufSize,
     0,
     FALSE);
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR (T_("WideCharToMultiByte"), 0);
    }
  if (n < 0)
    {
      UNEXPECTED_CONDITION (T_("Utils::CopyString"));
    }
  return (static_cast<size_t>(n));
#else
  size_t length;

  for (length = 0; length < bufSize; ++ length)
    {
      if (*lpszSource > 255)
	{
	  INVALID_ARGUMENT (T_("Utils::CopyString"));
	}
      if ((lpszBuf[length] = lpszSource[length]) == 0)
	{
	  break;
	}
    }

  if (length == bufSize)
    {
      BUF_TOO_SMALL (T_("Utils::CopyString"));
    }

  return (length);
#endif
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
  MD5Builder md5Builder;
  md5Builder.Init ();
  if (File::GetSize(lpszPath) > 0)
    {
      auto_ptr<MemoryMappedFile> pFile (MemoryMappedFile::Create());
      const void * ptr = pFile->Open(lpszPath, false);
      md5Builder.Update (ptr, pFile->GetSize());
    }
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
  CharBuffer<MIKTEXCHAR> autoBuffer;
  int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
  n = _vsctprintf(lpszFormat, arglist);
  if (n < 0)
  {
    FATAL_CRT_ERROR (T_("_vsctprintf"), 0);
  }
  autoBuffer.Reserve (n + 1);
  n = _vstprintf_s(
    autoBuffer.GetBuffer(),
    autoBuffer.GetCapacity(),
    lpszFormat,
    arglist);
  if (n < 0)
  {
    FATAL_CRT_ERROR (T_("_vsntprintf_s"), 0);
  }
  else if (static_cast<size_t>(n) >= autoBuffer.GetCapacity())
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
		    autoBuffer.GetCapacity(),
		    lpszFormat,
		    arglist);
    if (n < 0)
    {
      FATAL_CRT_ERROR (T_("_vsntprintf"), 0);
    }
    else if (static_cast<size_t>(n) < autoBuffer.GetCapacity())
    {
      done = true;
    }
    else if (autoBuffer.GetCapacity() < MAXSIZE)
    {
      autoBuffer.Reserve (autoBuffer.GetCapacity() + 1000);
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
	      autoBuffer.GetCapacity(),
	      lpszFormat,
	      arglist);
  if (n < 0)
  {
    FATAL_CRT_ERROR (T_("vsnprintf"), 0);
  }
  else if (static_cast<size_t>(n) >= autoBuffer.GetCapacity())
  {
    autoBuffer.Reserve (n + 1);
    n =
      vsnprintf(autoBuffer.GetBuffer(),
		autoBuffer.GetCapacity(),
		lpszFormat,
		arglist);
    if (n < 0)
    {
      FATAL_CRT_ERROR (T_("vsnprintf"), 0);
    }
    else if (static_cast<size_t>(n) >= autoBuffer.GetCapacity())
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

   Argv::Argv
   _________________________________________________________________________ */

MIKTEXCALL
Argv::Argv ()
{
  argv.push_back (0);
}


/* _________________________________________________________________________

   Argv::~Argv
   _________________________________________________________________________ */

MIKTEXEXPORT
MIKTEXCALL
Argv::~Argv ()
{
  for (vector<MIKTEXCHAR*>::const_iterator it = argv.begin();
       it != argv.end() && *it != 0;
       ++ it)
    {
      free (*it);
    }
}

/* _________________________________________________________________________

   Argv::Build
   _________________________________________________________________________ */

void
MIKTEXCALL
Argv::Build (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	     /*[in]*/ const MIKTEXCHAR *	lpszArguments)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszArguments);
  MIKTEX_ASSERT (argv.size() == 1);
  argv.clear ();
  argv.push_back (StrDup(lpszFileName));
  argv.push_back (0);
  if (lpszArguments != 0)
    {
      Append (lpszArguments);
    }
}


/* _________________________________________________________________________

   NumberString::Init
   _________________________________________________________________________ */

void
NumberString::Init (/*[in]*/ long long	num,
		    /*[in]*/ bool	hex)
{
  int n;
  const MIKTEXCHAR * lpszFormat = (hex ? T_("%llx") : T_("%lld"));
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
  n = _stprintf_s (buffer, BUFSIZE, lpszFormat, num);
#  else
  n = _stprintf (buffer, lpszFormat, num);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberString::NumberString()
#else
  n = sprintf (buffer, lpszFormat, num);
#endif
  if (n < 0)
    {
      FATAL_CRT_ERROR (T_("sprintf"), 0);
    }
}

/* _________________________________________________________________________

   NumberString::Init
   _________________________________________________________________________ */

void
NumberString::Init (/*[in]*/ double	num)
{
  int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
  n = _stprintf_s (buffer, BUFSIZE, T_("%f"), num);
#  else
  n = _stprintf (buffer, T_("%f"), num);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
  n = sprintf (buffer, T_("%f"), num);
#endif
  if (n < 0)
    {
      FATAL_CRT_ERROR (T_("sprintf"), 0);
    }
}

/* _________________________________________________________________________

   Utils::IsAscii
   _________________________________________________________________________ */

bool
MIKTEXCALL 
Utils::IsAscii (/*[in]*/ const char * lpsz)
{
  MIKTEX_ASSERT_STRING (lpsz);
  for (; *lpsz != 0; ++ lpsz)
    {
      if (! isascii(*lpsz))
	{
	  return (false);
	}
    }
  return (true);
}

/* _________________________________________________________________________

   Utils::IsAscii
   _________________________________________________________________________ */

bool
MIKTEXCALL 
Utils::IsAscii (/*[in]*/ const wchar_t * lpsz)
{
  MIKTEX_ASSERT_STRING (lpsz);
  for (; *lpsz != 0; ++ lpsz)
    {
      if (! iswascii(*lpsz))
	{
	  return (false);
	}
    }
  return (true);
}

/* _________________________________________________________________________

   Utils::IsUTF8
   _________________________________________________________________________ */

bool
MIKTEXCALL 
Utils::IsUTF8 (/*[in]*/ const char * lpsz)
{
  MIKTEX_ASSERT_STRING (lpsz);
  for (; lpsz[0] != 0; ++ lpsz)
    {
      if (((lpsz[0] & 0xe0) == static_cast<char>(0xc0))
	  && ((lpsz[1] & 0xc0) == static_cast<char>(0x80)))
	{
	  return (true);
	}
      else if (((lpsz[0] & 0xf0) == static_cast<char>(0xe0))
	       && ((lpsz[1] & 0xc0) == static_cast<char>(0x80))
	       && ((lpsz[2] & 0xc0) == static_cast<char>(0x80)))
	
	{
	  return (true);
	}
      else if (((lpsz[0] & 0xf8) == static_cast<char>(0xf0))
	       && ((lpsz[1] & 0xc0) == static_cast<char>(0x80))
	       && ((lpsz[2] & 0xc0) == static_cast<char>(0x80))
	       && ((lpsz[3] & 0xc0) == static_cast<char>(0x80)))
	
	{
	  return (true);
	}
      else if (lpsz[0] >= static_cast<char>(128))
	{
	  return (false);
	}
    }
  return (false);
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
