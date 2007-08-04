/* miktex/c4plib.h: Pascalish run-time support			-*- C++ -*-

   Copyright (C) 1996-2007 Christian Schenk

   This file is part of the MiKTeX TeXMF Library.

   The MiKTeX TeXMF Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX TeXMF Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX TeXMF Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(D8041ED6B5CDA942BA2AEEA8E29FD1D9)
#define D8041ED6B5CDA942BA2AEEA8E29FD1D9

#include <cstdio>

#if defined(_WIN32) || defined(_WIN64)
#  include <windows.h>
#endif

#include <miktex/core.h>

#if ! defined(C1F0C63F01D5114A90DDF8FC10FF410B)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define C4PEXPORT __declspec(dllimport)
#  else
#    define C4PEXPORT
#  endif
#endif

#if defined(_MSC_VER)
#  define C4PCALL __stdcall
#else
#  define C4PCALL
#endif

#define C4PAPI(type) C4PEXPORT type C4PCALL
#define C4PDATA(type) C4PEXPORT type

#define C4P_BEGIN_NAMESPACE namespace C4P {
#define C4P_END_NAMESPACE }

C4P_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   Integral Types
   _________________________________________________________________________ */

#if defined(_MSC_VER)
typedef signed __int8 C4P_signed8;
typedef unsigned __int8 C4P_unsigned8;
typedef signed __int16 C4P_signed16;
typedef unsigned __int16 C4P_unsigned16;
typedef signed __int32 C4P_signed32;
typedef unsigned __int32 C4P_unsigned32;
#else
#  error Unimplemented: C4P_signed8
#endif

typedef int C4P_integer;

/* _________________________________________________________________________

   Boolean Type
   _________________________________________________________________________ */

typedef bool C4P_boolean;	// assert (sizeof(bool) == 1)

/* _________________________________________________________________________

   FileRoot
   _________________________________________________________________________ */

struct FileRoot
{
protected:
  FILE * pFile;
  
protected:
  unsigned flags;

public:
  void
  AssertValid ()
    const
  {
    MIKTEX_ASSERT (pFile != 0);
  }

public:
  C4PAPI(bool)
  Open (/*[in]*/ const MIKTEXCHAR *		lpszName,
	/*[in]*/ MiKTeX::Core::FileMode		mode,
	/*[in]*/ MiKTeX::Core::FileAccess	access,
	/*[in]*/ MiKTeX::Core::FileShare	share,
	/*[in]*/ bool				text,
	/*[in]*/ bool				mustExist);

public:
  void
  Close ()
  {
    AssertValid ();
    FILE * pFile = this->pFile;
    this->pFile = 0;
    MiKTeX::Core::SessionWrapper(true)->CloseFile (pFile);
  }

public:
  void
  Attach (/*[in]*/ FILE *	pFile,
	  /*[in]*/ bool		takeOwnership)
  {
    takeOwnership;
    flags = 0;
    this->pFile = pFile;
  }

public:
  operator FILE * ()
  {
    return (pFile);
  }

public:
  FILE * &
  fileref ()
  {
    flags = 0;
    return (pFile);
  }

public:
  FILE *
  operator-> ()
  {
    return (pFile);
  }
};

/* _________________________________________________________________________

   BufferedFile
   _________________________________________________________________________ */

template<class T>
struct BufferedFile
  : public FileRoot
{
public:
  typedef T ElementType;

protected:
  enum { Buffered = 0x00010000 };

protected:
  ElementType currentElement;

public:
  const ElementType &
  bufref ()
    const
  {
    MIKTEX_ASSERT ((flags & Buffered) != 0)
    return (currentElement);
  }
  
public:
  ElementType &
  bufref ()
  {
    flags |= Buffered;
    return (currentElement);
  }

public:
  const ElementType &
  operator* ()
    const
  {
    return (bufref());
  }

public:
  ElementType &
  operator* ()
  {
    return (bufref());
  }

public:
  bool
  Eof ()
  {
    if (feof(pFile) != 0)
      {
	return (true);
      }

    if ((flags & Buffered) != 0)
      {
	return (false);
      }

    int lookAhead = getc(pFile);

    if (lookAhead == EOF)
      {
	if (ferror(pFile) != 0)
	  {
	    MiKTeX::Core::Session::FatalCrtError
	      (MIKTEXTEXT("getc"), 0, MIKTEXTEXT(__FILE__), __LINE__);
	  }
	return (true);
      }
    
    if (ungetc(lookAhead, pFile) != lookAhead)
      {
	MiKTeX::Core::Session::FatalCrtError
	  (MIKTEXTEXT("ungetc"), 0, MIKTEXTEXT(__FILE__), __LINE__);
      }

    return (false);
  }

public:
  void
  Read (/*[out]*/ ElementType *	pBuf,
	/*[in]*/ size_t		n)
  {
    AssertValid ();
    MIKTEX_ASSERT_BUFFER (pBuf, n);
    if (feof(*this) != 0)
      {
	MiKTeX::Core::Session::FatalMiKTeXError
	  (MIKTEXTEXT("BufferedFile::read"),
	   MIKTEXTEXT("Read operation failed."),
	   0,
	   MIKTEXTEXT(__FILE__),
	   __LINE__);
      }
    if (fread(pBuf, sizeof(ElementType), n, *this) != n)
      {
	MiKTeX::Core::Session::FatalMiKTeXError
	  (MIKTEXTEXT("BufferedFile::read"),
	   MIKTEXTEXT("Read operation failed."),
	   0,
	   MIKTEXTEXT(__FILE__),
	   __LINE__);
      }
    if (ferror(*this) != 0)
      {
	MiKTeX::Core::Session::FatalCrtError
	  (MIKTEXTEXT("fread"), 0, MIKTEXTEXT(__FILE__), __LINE__);
      }
    currentElement = pBuf[n - 1];
  }

public:
  void
  Read ()
  {
    AssertValid ();
    flags |= Buffered;
    if (feof(*this) != 0)
      {
	MiKTeX::Core::Session::FatalMiKTeXError
	  (MIKTEXTEXT("BufferedFile::read"),
	   MIKTEXTEXT("Read operation failed."),
	   0,
	   MIKTEXTEXT(__FILE__),
	   __LINE__);
      }
    fread (&currentElement, sizeof(ElementType), 1, *this);
    if (ferror(*this) != 0)
      {
	MiKTeX::Core::Session::FatalCrtError
	  (MIKTEXTEXT("fread"), 0, MIKTEXTEXT(__FILE__), __LINE__);
      }
  }

public:
  void
  Reset ()
  {
    AssertValid ();
    rewind (*this);
    Read ();
  }

public:
  void
  Rewrite ()
  {
    AssertValid ();
    rewind (*this);
  }

public:
  void
  Write ()
  {
    AssertValid ();
    MIKTEX_ASSERT ((flags & Buffered) != 0);
    if (fwrite(&currentElement, sizeof(ElementType), 1, *this) != 1
	|| ferror(*this) != 0)
      {
	MiKTeX::Core::Session::FatalCrtError
	  (MIKTEXTEXT("fwrite"), 0, MIKTEXTEXT(__FILE__), __LINE__);
      }
  }

public:
  void
  Seek (/*[in]*/ long	offset,
	/*[in]*/ int	origin)
  {
    AssertValid ();
    if (fseek(*this, offset, origin) != 0)
      {
	MiKTeX::Core::Session::FatalCrtError
	  (MIKTEXTEXT("fseek"), 0, MIKTEXTEXT(__FILE__), __LINE__);
      }
    if ((flags & Buffered) != 0)
      {
	Read ();
      }
  }
};

#define C4P_FILE_STRUCT(Bt) C4P::BufferedFile<Bt>

typedef C4P_FILE_STRUCT(MIKTEXCHAR) C4P_text;

/* _________________________________________________________________________

   Floating Point Type
   _________________________________________________________________________ */

#if defined(C4P_REAL_IS_DOUBLE)
typedef double C4P_real;
#else
typedef float C4P_real;
#endif

typedef double C4P_longreal;

C4P_END_NAMESPACE;

#include <cctype>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <csignal>

C4P_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   Command Line Arguments
   _________________________________________________________________________ */

C4PAPI(/*[in]*/ int)
GetArgC ();

C4PAPI(/*[in]*/ const MIKTEXCHAR * *)
GetArgV ();

C4PAPI(const MIKTEXCHAR *)
GetCmdLine ();

C4PAPI(MIKTEXCHAR *)
GetProgName (/*[out]*/ MIKTEXCHAR * lpsz);

C4PAPI(C4P_text *)
GetStdFilePtr (/*[in]*/ unsigned idx);

#define c4pargc C4P::GetArgC()
#define c4pargv C4P::GetArgV()

#define c4ppline C4P::GetCmdLine()
#define c4pplen strlen(C4P::GetCmdLine())

#define input (*(C4P::GetStdFilePtr(0)))
#define output (*(C4P::GetStdFilePtr(1)))
#define c4perroroutput (*(C4P::GetStdFilePtr(2)))

/* _________________________________________________________________________

   Program
   _________________________________________________________________________ */

class
Program
{
public:
  C4PEXPORT
  C4PCALL
  Program (/*[in]*/ const MIKTEXCHAR *	lpszName,
	   /*[in]*/ int			argc,
	   /*[in]*/ const MIKTEXCHAR **	argv);

public:
  ~Program ()
  {
    try
      {
	if (running)
	  {
	    Finish ();
	  }
      }
    catch (const std::exception &)
      {
      }
  }

public:
  C4PEXPORT
  void
  C4PCALL
  Finish ();

private:
  bool running;
};

#define C4P_BEGIN_PROGRAM(lpszName, argc, argv)			\
  {								\
    int c4p_retcode = 0;					\
    try								\
      {								\
        C4P::Program c4p_program (lpszName, argc, argv);

#define C4P_END_PROGRAM()			\
        c4p_program.Finish ();			\
      }						\
    catch (int retcode)				\
      {						\
        c4p_retcode = retcode;			\
      }						\
    return (c4p_retcode);			\
}

/* _________________________________________________________________________

   Non-local goto
   _________________________________________________________________________ */

#define c4pbegintryblock(n)			\
  try						\
    {

#define c4pendtryblock(n)			\
      ;						\
    }						\
  catch (const C4P::Exception##n &)		\
    {						\
    }

#define c4pthrow(n) throw C4P::Exception##n ()

/* _________________________________________________________________________

   for Loops
   _________________________________________________________________________ */

#define C4P_FOR_BEGIN(var, startval, relop, lastval)	\
{							\
  int c4p_temp = (lastval);				\
  if ((var = (startval)) relop c4p_temp)		\
    {							\
      do						\
	{

#define C4P_FOR_END(var, op)                       	\
	}						\
      while (var op != c4p_temp);                     	\
    }							\
}

/* _________________________________________________________________________

   Constants
   _________________________________________________________________________ */

const C4P_integer maxint = INT_MAX;

/* _________________________________________________________________________

   File I/O
   _________________________________________________________________________ */

C4PAPI(void)
DiscardLine (/*[in]*/ C4P_text & textfile);

C4PAPI(MIKTEXCHAR)
GetChar (/*[in]*/ C4P_text & textfile);

C4PAPI(int)
GetInteger (/*[in]*/ C4P_text & textfile);

C4PAPI(C4P_real)
GetReal (/*[in]*/ C4P_text & textfile);

#define C4P_READ_BEGIN() {
#define C4P_READLN_BEGIN() C4P_READ_BEGIN ()

template<class Ft>
inline
void
c4p_read_f (/*[in]*/ Ft &,
	    /*[in]*/ Ft &)
{
}

template<class Vt, class Ft>
inline
void
c4p_read_v (/*[in]*/ Vt & v,
	    /*[in]*/ Ft & f)
{
  f.AssertValid ();
  v = *f;
  get (f);
}

template<class Vt, class Ft>
inline
void
c4p_read_c (/*[in]*/ Vt & v,
	    /*[in]*/ Ft & f)
{
  f.AssertValid ();
  v = GetChar(f);
}

template<class Vt, class Ft>
inline
void
c4p_read_i (/*[in]*/ Vt & v,
	    /*[in]*/ Ft & f)
{
  f.AssertValid ();
  v = GetInteger(f);
}

template<class Vt, class Ft>
inline
void
c4p_read_r (/*[in]*/ Vt & v,
	    /*[in]*/ Ft & f)
{
  f.AssertValid ();
  v = GetReal(f);
}

#define C4P_READ_END(f) }
#define C4P_READLN_END(f) C4P::DiscardLine (f); }

inline
void
c4p_readln ()
{
  input.AssertValid ();
  DiscardLine (input);
}

#define C4P_WRITE_BEGIN() {
#define C4P_WRITELN_BEGIN() C4P_WRITE_BEGIN ()

template<class Ft>
inline
void
c4p_write_f (/*[in]*/ Ft &,
	     /*[in]*/ Ft &)
{
}

template<class Vt, class Ft>
inline
void
c4p_write_v (/*[in]*/ Vt v,
	     /*[in]*/ Ft & f)
{
  f.AssertValid ();
  *f = v;
  put (f);
}

template<class Vt, class Ft>
inline
void
c4p_write_c (/*[in]*/ Vt v,
	     /*[in]*/ Ft & f)
{
  f.AssertValid ();
#if defined(_MSC_VER)
  MIKTEXCHARINT ch = _puttc(v, f);
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: c4p_write_c()
#else
  MIKTEXCHARINT ch = putc(v, f);
#endif
  if (ch == MIKTEXEOF)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fputc"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

template<class Vt, class Ft>
inline
void
c4p_write_c1 (/*[in]*/ Vt v,
	      /*[in]*/ Ft & f)
{
  return (c4p_write_c(v, f));
}

template<class Vt, class Ft>
inline
void
c4p_write_i (/*[in]*/ Vt v,
	     /*[in]*/ Ft & f)
{
  f.AssertValid ();
  if (_ftprintf(f, MIKTEXTEXT("%ld"), static_cast<long>(v)) < 0)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fprintf"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

template<class Vt, class Ft>
inline
void
c4p_write_i1 (/*[in]*/ Vt	v,
	      /*[in]*/ int	w1,
	      /*[in]*/ Ft &	f)
{
  f.AssertValid ();
  if (_ftprintf(f,
		MIKTEXTEXT("%*ld"),
		static_cast<int>(w1),
		static_cast<long>(v))
      < 0)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fprintf"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }

}

template<class Vt, class Ft>
inline
void
c4p_write_i2 (/*[in]*/ Vt v,
	      /*[in]*/ int w1,
	      /*[in]*/ int w2,
	      /*[in]*/ Ft & f)
{
  f.AssertValid ();
  if (_ftprintf(f, MIKTEXTEXT("%*.*ld"),
		static_cast<int>(w1),
		static_cast<int>(w2),
		static_cast<long>(v))
      < 0)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fprintf"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

template<class Vt, class Ft>
inline
void
c4p_write_r (/*[in]*/ Vt	v,
	     /*[in]*/ Ft &	f)
{
  f.AssertValid ();
  if (_ftprintf(f, MIKTEXTEXT("%f"), static_cast<float>(v)) < 0)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fprintf"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

template<class Vt, class Ft>
inline
void
c4p_write_r2 (/*[in]*/ Vt	v,
	      /*[in]*/ int	w1,
	      /*[in]*/ int	w2,
	      /*[in]*/ Ft &	f)
{
  f.AssertValid ();
  if (_ftprintf(f,
		MIKTEXTEXT("%*.*f"),
		static_cast<int>(w1),
		static_cast<int>(w2),
		static_cast<float>(v))
      < 0)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fprintf"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

template<class Vt, class Ft>
inline
void
c4p_write_s (/*[in]*/ Vt	v,
	     /*[in]*/ Ft &	f)
{
  f.AssertValid ();
  if (fputs(v, f) == EOF)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fputs"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

#define C4P_WRITE_END(f) }
#define C4P_WRITELN_END(f) _puttc (MIKTEXTEXT('\n'), f); }

inline
void
c4p_writeln ()
{
  output.AssertValid ();
  if (_puttc(MIKTEXTEXT('\n'), output) == MIKTEXEOF)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fputc"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

template<class T>
inline
void
c4pmget (/*[in]*/ T &				f, 
	 /*[out]*/ typename T::ElementType *	buf,
	 /*[in]*/ size_t			n)
{
  f.AssertValid ();
  f.Read (buf, n);
}

template<class T>
inline
MIKTEXCHAR
c4pgetc (/*[in]*/ T & f)
{
  f.AssertValid ();
  return (GetChar(f));
}

template<class T>
inline
void
c4p_puttc (/*[in]*/ T & f)
{
  f.AssertValid ();
  if (_puttc (*f, f) == MIKTEXEOF)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fputc"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

template<class T>
inline
void
c4p_break (/*[in]*/ T & f)
{
  f.AssertValid ();
  if (fflush(f) == EOF)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("flush"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

template<class Ft>
inline
void
c4pfseek (/*[in]*/ Ft &	f,
	  /*[in]*/ int	offs,
	  /*[in]*/ int	orig)
{
  f.AssertValid ();
  f.Seek (offs, orig);
}

template<class Ft>
inline
long
c4pftell (/*[in]*/ Ft & f)
{
  f.AssertValid ();
  long n = ftell(f);
  if (n < 0)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("ftell"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
  return (n);
}

template<class Ft>
inline
void
c4pbufwrite (/*[in]*/ Ft &		f,
	     /*[in]*/ const void *	buf,
	     /*[in]*/ size_t		buf_size)
{
  f.AssertValid ();
  //MIKTEX_ASSERT_BUFFER (buf, buf_size);
  if (fwrite(buf, buf_size, 1, f) != 1)
    {
      MiKTeX::Core::Session::FatalCrtError
	(MIKTEXTEXT("fwrite"), 0, MIKTEXTEXT(__FILE__), __LINE__);
    }
}

/* _________________________________________________________________________

   Memory Functions
   _________________________________________________________________________ */

#define c4parrcpy(dst, src) c4pmemcpy(dst, sizeof(dst), src, sizeof(dst))

inline
void
c4pmemcpy (/*[out]*/ void *		pdst,
	   /*[in]*/ size_t		size,
	   /*[in]*/ const void *	psrc,
	   /*[in]*/ size_t		count)
{
  MIKTEX_ASSERT (size >= count);
  MIKTEX_ASSERT_BUFFER (pdst, size);
  //MIKTEX_ASSERT_BUFFER (psrc, count);
#if defined(_MSC_VER) && (_MSC_VER > 1400)
  memcpy_s (pdst, size, psrc, count);
#else
  size;
  memcpy (pdst, psrc, count);
#endif
}

inline
void
c4pstrcpy (/*[out]*/ MIKTEXCHAR *	pdst,
	   /*[in]*/ size_t		size,
	   /*[in]*/ const MIKTEXCHAR *	psrc)
{
  MiKTeX::Core::Utils::CopyString (pdst, size, psrc);
}

inline
size_t
c4pstrlen (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  MIKTEX_ASSERT_STRING (lpsz);
  return (MiKTeX::Core::StrLen(lpsz));
}

/* _________________________________________________________________________

   File Functions
   _________________________________________________________________________ */

template<class T>
inline
bool
c4pfopen (/*[in]*/ T &			f,
	  /*[in]*/ const MIKTEXCHAR *	lpszName,
	  /*[in]*/ const MIKTEXCHAR *	lpszMode,
	  /*[in]*/ bool			mustExist)
{
  MIKTEX_ASSERT_STRING (lpszName);
  MIKTEX_ASSERT_STRING (lpszMode);
  MIKTEX_ASSERT
    (MiKTeX::Core::StringCompare(lpszMode, MIKTEXTEXT("r")) == 0
     || MiKTeX::Core::StringCompare(lpszMode, MIKTEXTEXT("rb")) == 0
     || MiKTeX::Core::StringCompare(lpszMode, MIKTEXTEXT("w")) == 0
     || MiKTeX::Core::StringCompare(lpszMode, MIKTEXTEXT("wb")) == 0);
  bool reading = (lpszMode[0] == MIKTEXTEXT('r'));
  bool text = (lpszMode[1] == 0);
  return (f.Open(lpszName,
		 (reading
		  ? MiKTeX::Core::FileMode::Open
		  : MiKTeX::Core::FileMode::Create),
		 (reading
		  ? MiKTeX::Core::FileAccess::Read
		  : MiKTeX::Core::FileAccess::Write),
		 (reading
		  ? MiKTeX::Core::FileShare::Read
		  : MiKTeX::Core::FileShare::None),
		 text,
		 mustExist));
}

template<class T>
inline
bool
c4ptryfopen (/*[in]*/ T &			f,
	     /*[in]*/ const MIKTEXCHAR *	lpszName,
	     /*[in]*/ const MIKTEXCHAR *	lpszMode)
{
  return (c4pfopen(f, lpszName, lpszMode, false));
}

template<class T>
inline
void
c4pfclose (/*[in]*/ T & f)
{
  f.close ();
}

/* _________________________________________________________________________

   Program Control
   _________________________________________________________________________ */

inline
void
c4pexit (/*[in]*/ int n)
{
  throw (n);
}

#define c4preturn() goto C4P_LABEL_PROC_EXIT
#define c4pbreakloop() break

template<class T>
T *
c4pptr (/*[in]*/ T & obj)
{
  return (&obj);
};

template<class T>
inline
void
c4pincr (/*[in]*/ T & lvalue)
{
  ++ lvalue;
}

template<class T>
inline
void
c4pdecr(/*[in]*/ T & lvalue)
{
  -- lvalue;
}

const int c4pcur = SEEK_CUR;
const int c4pend = SEEK_END;
const int c4pset = SEEK_SET;

const MIKTEXCHAR * const c4prmode = MIKTEXTEXT("r");
const MIKTEXCHAR * const c4pwmode = MIKTEXTEXT("w");
const MIKTEXCHAR * const c4prbmode = MIKTEXTEXT("rb");
const MIKTEXCHAR * const c4pwbmode = MIKTEXTEXT("wb");

C4PAPI(unsigned)
GetYear ();

C4PAPI(unsigned)
GetMonth ();

C4PAPI(unsigned)
GetDay ();

C4PAPI(unsigned)
GetHour ();

C4PAPI(unsigned)
GetMinute ();

C4PAPI(unsigned)
GetSecond ();

#define c4pyear C4P::GetYear()
#define c4pmonth C4P::GetMonth()
#define c4pday C4P::GetDay()
#define c4phour C4P::GetHour()
#define c4pminute C4P::GetMinute()
#define c4psecond C4P::GetSecond()

#define C4P_PROC_ENTRY(handle)
#define C4P_PROC_EXIT(handle) C4P_LABEL_PROC_EXIT:

C4PAPI(void)
SetStartUpTime (/*[in]*/ time_t time);

C4PAPI(time_t)
GetStartUpTime ();

C4PAPI(int)
MakeCommandLine (/*[in]*/ int			argc,
		 /*[in]*/ const MIKTEXCHAR **	argv);

C4PAPI(C4P_integer)
Round (double r);

/* _________________________________________________________________________

   Pascal Functions
   _________________________________________________________________________ */

inline
double
arctan (/*[in]*/ double x)
{
  return (atan(x));
}

template<class T>
inline
int
chr (/*[in]*/ T i)
{
  return (i);
}

template<class T>
inline
C4P_boolean
eof (/*[in]*/ T & f)
{
#if 0
  return (feof(f) != 0);
#else
  return (f.Eof());
#endif
}

template<class T>
inline
C4P_boolean
eoln (/*[in]*/ T & f)
{
  return (*f == MIKTEXTEXT('\n'));
};

template<class T>
inline
void
get (/*[in]*/ T & f)
{
  f.Read ();
}

inline
double
ln (/*[in]*/ double x)
{
  return (log(x));
}

template<class T>
inline
C4P_boolean
odd (/*[in]*/ T i)
{
  return ((i & 1) != 0 ? true : false);
}

template<class T>
inline
int
ord (/*[in]*/ T x)
{
  return (x);
}

template<class T>
inline
T
pred (/*[in]*/ T x)
{
  return (x - 1);
}

template<class T>
inline
void
put (/*[in]*/ T & f)
{
  f.Write ();
}

template<class T>
inline
void
reset (/*[in]*/ T & f)
{
  f.Reset ();
}

template<class T>
inline
void
rewrite (/*[in]*/ T & f)
{
  f.Rewrite ();
}

inline
int
round (/*[in]*/ double r)
{
  return (C4P::Round(r));
}

inline
double
sqr (/*[in]*/ double x)
{
  return (sqrt(x));
}

template<class T>
inline
T
succ (/*[in]*/ T x)
{
  return (x + 1);
}

inline
int
trunc (/*[in]*/ double x)
{
  return (static_cast<int>(x));
}

C4P_END_NAMESPACE;

#undef C4P_BEGIN_NAMESPACE
#undef C4P_END_NAMESPACE

#endif /* miktex/c4plib.h */
