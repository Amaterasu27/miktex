/* util.cpp: generic utilities

   Copyright (C) 1996-2010 Christian Schenk

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
  : Base(lpsz)
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
Utils::Contains (/*[in]*/ const char *	lpszList,
		 /*[in]*/ const char *	lpszElement,
		 /*[in]*/ const char *	lpszDelims,
		 /*[in]*/ bool		ignoreCase)
{
  for (Tokenizer tok (lpszList, lpszDelims); tok.GetCurrent() != 0; ++ tok)
    {
      if (StringCompare(tok.GetCurrent(), lpszElement, ignoreCase) == 0)
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
Utils::AppendString (/*[in,out]*/ char *	lpszBuf,
		     /*[in]*/ size_t		bufSize,
		     /*[in]*/ const char *	lpszSource)
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
      INVALID_ARGUMENT ("MiKTeX::Core::Utils::AppendString", lpszBuf);
    }

  length += CopyString(&lpszBuf[length], bufSize - length, lpszSource);

  return (length);
}

/* _________________________________________________________________________

   GenericCopyString
   _________________________________________________________________________ */

template<typename CharType>
size_t
GenericCopyString (/*[out]*/ CharType *		lpszBuf,
		   /*[in]*/ size_t		bufSize,
		   /*[in]*/ const CharType *	lpszSource)
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
      BUF_TOO_SMALL ("Utils::CopyString");
    }

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
  return (GenericCopyString(lpszBuf, bufSize, lpszSource));
}

/* _________________________________________________________________________

   Utils::CopyString
   _________________________________________________________________________ */

size_t
Utils::CopyString (/*[out]*/ wchar_t *		lpszBuf,
		   /*[in]*/ size_t		bufSize,
		   /*[in]*/ const wchar_t *	lpszSource)
{
  return (GenericCopyString(lpszBuf, bufSize, lpszSource));
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
      FATAL_WINDOWS_ERROR ("WideCharToMultiByte", 0);
    }
  if (n < 0)
    {
      UNEXPECTED_CONDITION ("Utils::CopyString");
    }
  return (static_cast<size_t>(n));
#else
  size_t length;

  for (length = 0; length < bufSize; ++ length)
    {
      if (*lpszSource > 255)
	{
	  INVALID_ARGUMENT ("Utils::CopyString", 0);
	}
      if ((lpszBuf[length] = lpszSource[length]) == 0)
	{
	  break;
	}
    }

  if (length == bufSize)
    {
      BUF_TOO_SMALL ("Utils::CopyString");
    }

  return (length);
#endif
}

/* _________________________________________________________________________

   CopyString2
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
CopyString2 (/*[out]*/ char *		lpszBuf,
	     /*[in]*/ size_t		bufSize,
	     /*[in]*/ const char *	lpszSource,
	     /*[in]*/ size_t		count)
{
  MIKTEX_ASSERT_CHAR_BUFFER (lpszBuf, bufSize);
  MIKTEX_ASSERT_STRING (lpszSource);

  *lpszBuf = 0;

  if (count == 0)
    {
      return;
    }

  char * lpsz = lpszBuf;

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
      TraceError ("  bufSize=%u", static_cast<unsigned>(bufSize));
      TraceError ("  count=%u", static_cast<unsigned>(count));
      TraceError ("  lpszSource=%.*s...",
		  static_cast<int>(bufSize),
		  lpszSource);
      BUF_TOO_SMALL ("CopyString2");
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
Utils::ReplaceString (/*[out]*/ char *		lpszBuf,
		      /*[in,out]*/ size_t &	bufSize,
		      /*[in]*/ const char *	lpszSource,
		      /*[in]*/ const char *	lpszString1,
		      /*[in]*/ const char *	lpszString2)
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
		      BUF_TOO_SMALL ("MiKTeX::Core::Utils::ReplaceString");
		    }
		}
	      ++ lpszSource;
	    }
	}
      else if (lpszString1[n] == 0)
	{
	  lpszSource += n;
	  n = strlen(lpszString2);
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
MD5::FromFile (/*[in]*/ const char *	lpszPath)
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
Unhex (/*[in]*/ char x)
{
  if (x >= '0' && x <= '9')
    {
      return (x - '0');
    }
  else if (x >= 'A' && x <= 'F')
    {
      return (x - 'A' + 10);
    }
  else if (x >= 'a' && x <= 'f')
    {
      return (x - 'a' + 10);
    }
  else
    {
      FATAL_MIKTEX_ERROR ("Unhex",
			  T_("Hex digit expected."),
			  0);
    }
}

/* _________________________________________________________________________

   MD5::Parse
   _________________________________________________________________________ */

MD5
MD5::Parse (/*[in]*/ const char *	lpszHexString)
{
  size_t l = strlen(lpszHexString);
  if (l != 32)
    {
      FATAL_MIKTEX_ERROR ("MD5::Parse", T_("Invalid MD5."), lpszHexString);
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

string
Utils::FormatString (/*[in]*/ const char *  lpszFormat,
		     /*[in]*/ va_list	  arglist)
{
  CharBuffer<char> autoBuffer;
  int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
  n = _vscprintf(lpszFormat, arglist);
  if (n < 0)
  {
    FATAL_CRT_ERROR ("_vscprintf", 0);
  }
  autoBuffer.Reserve (n + 1);
  n = vsprintf_s(
    autoBuffer.GetBuffer(),
    autoBuffer.GetCapacity(),
    lpszFormat,
    arglist);
  if (n < 0)
  {
    FATAL_CRT_ERROR ("vsprintf_s", 0);
  }
  else if (static_cast<size_t>(n) >= autoBuffer.GetCapacity())
  {
    FATAL_MIKTEX_ERROR (
      "Utils::FormatString",
      T_("Invalid format string or argument list."),
      0);
  }
#  else
  const size_t MAXSIZE = 10000;
  bool done = false;
  do
  {
    n = _vsnprintf(autoBuffer.GetBuffer(),
		   autoBuffer.GetCapacity(),
		   lpszFormat,
		   arglist);
    if (n < 0)
    {
      FATAL_CRT_ERROR ("_vsnprintf", 0);
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
	"Utils::FormatString",
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
    FATAL_CRT_ERROR ("vsnprintf", 0);
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
      FATAL_CRT_ERROR ("vsnprintf", 0);
    }
    else if (static_cast<size_t>(n) >= autoBuffer.GetCapacity())
    {
      FATAL_MIKTEX_ERROR (
	"Utils::FormatString",
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

PathNameParser::PathNameParser (/*[in]*/ const char * lpszPath)
{
  Utils::CopyString (buffer, BufferSizes::MaxPath, lpszPath);
  lpszNext = buffer;
  lpszCurrent = 0;
  ++ (*this);
}

/* _________________________________________________________________________

   PathNameParser::operator++
   _________________________________________________________________________ */

const char *
PathNameParser::operator++ ()
{
  lpszCurrent = lpszNext;

  char * lpsz;

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
#if defined(MIKTEX_WINDOWS)
  else if (lpszCurrent == buffer
	   && IsDriveLetter(buffer[0])
	   && buffer[1] == ':'
	   && IsDirectoryDelimiter(buffer[2]))
    {
      lpsz = &buffer[3];
    }
#endif
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

string
Utils::Hexify (/*[in]*/ const void *	pv,
	       /*[in]*/ size_t		nBytes,
	       /*[in]*/ bool		lowerCase)
{
  string ret;
#define TOHEX(x) ((x) < 10 ? '0' + (x) : (x) - 10 + 'A')
#define tohex(x) ((x) < 10 ? '0' + (x) : (x) - 10 + 'a')
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

string
Utils::Hexify (/*[in]*/ const void *	pv,
	       /*[in]*/ size_t		nBytes)
{
  return (Hexify(pv, nBytes, true));
}


/* _________________________________________________________________________

   Argv::Argv
   _________________________________________________________________________ */

Argv::Argv ()
{
  argv.push_back (0);
}


/* _________________________________________________________________________

   Argv::~Argv
   _________________________________________________________________________ */

Argv::~Argv ()
{
  try
    {
      for (vector<char*>::iterator it = argv.begin();
	   it != argv.end() && *it != 0;
	   ++ it)
	{
	  MIKTEX_FREE (*it);
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   Argv::Build
   _________________________________________________________________________ */

void
Argv::Build (/*[in]*/ const char *	lpszFileName,
	     /*[in]*/ const char *	lpszArguments)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszArguments);
  MIKTEX_ASSERT (argv.size() == 1);
  argv.clear ();
  argv.push_back (MIKTEX_STRDUP(lpszFileName));
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
  const char * lpszFormat = (hex ? "%llx" : "%lld");
#if defined(_MSC_VER) && _MSC_VER >= 1400
  n = _stprintf_s (buffer, BUFSIZE, lpszFormat, num);
#else
  n = sprintf (buffer, lpszFormat, num);
#endif
  if (n < 0)
    {
      FATAL_CRT_ERROR ("sprintf", 0);
    }
}

/* _________________________________________________________________________

   NumberString::Init
   _________________________________________________________________________ */

void
NumberString::Init (/*[in]*/ double	num)
{
  int n;
#if defined(_MSC_VER) && _MSC_VER >= 1400
  n = _stprintf_s (buffer, BUFSIZE, "%f", num);
#else
  n = sprintf (buffer, "%f", num);
#endif
  if (n < 0)
    {
      FATAL_CRT_ERROR ("sprintf", 0);
    }
}

/* _________________________________________________________________________

   Utils::IsAscii
   _________________________________________________________________________ */

bool
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
Utils::IsAscii (/*[in]*/ const wchar_t * lpsz)
{
  MIKTEX_ASSERT_STRING (lpsz);
  for (; *lpsz != 0; ++ lpsz)
    {
#if defined(_MSC_VER)
      if (! iswascii(*lpsz))
	{
	  return (false);
	}
#else
      UNIMPLEMENTED ("Utils::IsAscii");
#endif
    }
  return (true);
}

/* _________________________________________________________________________

   Utils::IsUTF8
   _________________________________________________________________________ */

bool
Utils::IsUTF8 (/*[in]*/ const char *	lpsz,
	       /*[in]*/ bool		allowPureAscii)
{
  MIKTEX_ASSERT_STRING (lpsz);
  const unsigned char * lpsz2 = reinterpret_cast<const unsigned char *>(lpsz);
  for (; lpsz2[0] != 0; ++ lpsz2)
    {
      if (((lpsz2[0] & 0xe0) == 0xc0)
	  && ((lpsz2[1] & 0xc0) == 0x80))
	{
	  return (true);
	}
      else if (((lpsz2[0] & 0xf0) == 0xe0)
	       && ((lpsz2[1] & 0xc0) == 0x80)
	       && ((lpsz2[2] & 0xc0) == 0x80))
	
	{
	  return (true);
	}
      else if (((lpsz2[0] & 0xf8) == 0xf0)
	       && ((lpsz2[1] & 0xc0) == 0x80)
	       && ((lpsz2[2] & 0xc0) == 0x80)
	       && ((lpsz2[3] & 0xc0) == 0x80))
	{
	  return (true);
	}
      else if (lpsz2[0] >= 128)
	{
	  return (false);
	}
    }
  return (allowPureAscii);
}

/* _________________________________________________________________________

   miktex_replace_string
   _________________________________________________________________________ */

MIKTEXCEEAPI(void)
miktex_replace_string (/*[out]*/ char *		lpszBuf,
		       /*[in]*/ size_t *	lpSizeDest,
		       /*[in]*/ const char *	lpszSource,
		       /*[in]*/ const char *	lpszString1,
		       /*[in]*/ const char *	lpszString2)
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

/* _________________________________________________________________________

   MiKTeX::Debug::Malloc
   _________________________________________________________________________ */

void *
MIKTEXCEECALL
MiKTeX::Debug::Malloc (/*[in]*/ size_t		size,
		       /*[in]*/ const char *	lpszFileName,
		       /*[in]*/ int		line)
{
#if defined(MIKTEX_DEBUG)
  Utils::CheckHeap ();
#endif
#if defined(_MSC_VER) && defined(_DEBUG)
  void * ptr = _malloc_dbg(size, _NORMAL_BLOCK, lpszFileName, line);
#else
  void * ptr = malloc(size);
#endif
  if (ptr == 0)
    {
      OUT_OF_MEMORY ("malloc");
    }
  return (ptr);
}

/* _________________________________________________________________________

   miktex_core_malloc
   _________________________________________________________________________ */

MIKTEXCEEAPI(void *)
miktex_core_malloc (/*[in]*/ size_t		size,
		    /*[in]*/ const char *	lpszFileName,
		    /*[in]*/ int		line)
{
  C_FUNC_BEGIN ();

  return (MiKTeX::Debug::Malloc(size, lpszFileName, line));

  C_FUNC_END ();
}

/* _________________________________________________________________________

   MiKTeX::Debug::Calloc
   _________________________________________________________________________ */

void *
MIKTEXCEECALL
MiKTeX::Debug::Calloc (/*[in]*/ size_t		num,
		       /*[in]*/ size_t		size,
		       /*[in]*/ const char *	lpszFileName,
		       /*[in]*/ int		line)
{
#if defined(MIKTEX_DEBUG)
  Utils::CheckHeap ();
#endif
#if defined(_MSC_VER) && defined(_DEBUG)
  void * ptr = _calloc_dbg(num, size, _NORMAL_BLOCK, lpszFileName, line);
#else
  void * ptr = calloc(num, size);
#endif
  if (ptr == 0)
    {
      OUT_OF_MEMORY ("calloc");
    }
  return (ptr);
}

/* _________________________________________________________________________

   miktex_core_calloc
   _________________________________________________________________________ */

MIKTEXCEEAPI(void *)
miktex_core_malloc (/*[in]*/ size_t		num,
		    /*[in]*/ size_t		size,
		    /*[in]*/ const char *	lpszFileName,
		    /*[in]*/ int		line)
{
  C_FUNC_BEGIN ();

  return (MiKTeX::Debug::Calloc(num, size, lpszFileName, line));

  C_FUNC_END ();
}

/* _________________________________________________________________________

   MiKTeX::Debug::Realloc
   _________________________________________________________________________ */

void *
MIKTEXCEECALL
MiKTeX::Debug::Realloc (/*[in]*/ void *		ptr,
			/*[in]*/ size_t		size,
			/*[in]*/ const char *	lpszFileName,
			/*[in]*/ int		line)
{
#if defined(MIKTEX_DEBUG)
  Utils::CheckHeap ();
#endif
#if defined(_MSC_VER) && defined(_DEBUG)
  ptr = _realloc_dbg(ptr, size, _NORMAL_BLOCK, lpszFileName, line);
#else
  ptr = realloc(ptr, size);
#endif
  if (ptr == 0 && size > 0)
    {
      OUT_OF_MEMORY ("realloc");
    }
  return (ptr);
}

/* _________________________________________________________________________

   miktex_core_realloc
   _________________________________________________________________________ */

MIKTEXCEEAPI(void *)
miktex_core_realloc (/*[in]*/ void *		ptr,
		     /*[in]*/ size_t		size,
		     /*[in]*/ const char *	lpszFileName,
		     /*[in]*/ int		line)
{
  C_FUNC_BEGIN();

  return (MiKTeX::Debug::Realloc(ptr, size, lpszFileName, line));

  C_FUNC_END();
}

/* _________________________________________________________________________

   MiKTeX::Debug::StrDup
   _________________________________________________________________________ */

char *
MIKTEXCEECALL
MiKTeX::Debug::StrDup (/*[in]*/ const char *	lpsz,
		       /*[in]*/ const char *	lpszFileName,
		       /*[in]*/ int		line)
{
  size_t len = StrLen(lpsz);
  char * lpsz2 =
    reinterpret_cast<char *>(Malloc(sizeof(*lpsz) * (len + 1),
				    lpszFileName,
				    line));
  Utils::CopyString (lpsz2, len + 1, lpsz);
  return (lpsz2);
}

/* _________________________________________________________________________

   miktex_core_strdup
   _________________________________________________________________________ */

MIKTEXCEEAPI(char *)
miktex_core_strdup (/*[in]*/ const char *	lpsz,
		    /*[in]*/ const char *	lpszFileName,
		    /*[in]*/ int		line)
{
  C_FUNC_BEGIN ();

  return (MiKTeX::Debug::StrDup(lpsz, lpszFileName, line));

  C_FUNC_END ();
}

/* _________________________________________________________________________

   MiKTeX::Debug::Free
   _________________________________________________________________________ */

void
MIKTEXCEECALL
MiKTeX::Debug::Free (/*[in]*/ void * 		ptr,
		     /*[in]*/ const char *	lpszFileName,
		     /*[in]*/ int		line)
{
  int oldErrno = errno;
#if defined(_MSC_VER) && defined(_DEBUG)
  if (ptr != 0 && ! _CrtIsValidHeapPointer(ptr))
    {
      INVALID_ARGUMENT ("Debug::Free", 0);
    }
#endif
#if defined(MIKTEX_DEBUG)
  Utils::CheckHeap ();
#endif
#if defined(_MSC_VER) && defined(_DEBUG)
  _free_dbg (ptr, _NORMAL_BLOCK);
#else
  free (ptr);
#endif
  if (errno != 0 && errno != oldErrno)
    {
      FATAL_CRT_ERROR ("free", 0);
    }
}

/* _________________________________________________________________________

   miktex_core_free
   _________________________________________________________________________ */

MIKTEXCEEAPI(void)
miktex_core_free (/*[in]*/ void * 	ptr,
		  /*[in]*/ const char *	lpszFileName,
		  /*[in]*/ int		line)
{
  C_FUNC_BEGIN();

  MiKTeX::Debug::Free (ptr, lpszFileName, line);

  C_FUNC_END();
}

/* _________________________________________________________________________

   GetShortSourceFile
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(const char *)
GetShortSourceFile (/*[in]*/ const char * lpszSourceFile)
{
  const char * lpszShortSourceFile = 0;
  if (Utils::IsAbsolutePath(lpszSourceFile))
    {
      lpszShortSourceFile = 
	Utils::GetRelativizedPath(lpszSourceFile, MIKTEX_SOURCE_DIR);
      if (lpszShortSourceFile == 0)
	{
	  lpszShortSourceFile =
	    Utils::GetRelativizedPath (lpszSourceFile, MIKTEX_BINARY_DIR);	    
	}
    }
  if (lpszShortSourceFile == 0)
    {
      lpszShortSourceFile = lpszSourceFile;
    }
  return (lpszShortSourceFile);
}

