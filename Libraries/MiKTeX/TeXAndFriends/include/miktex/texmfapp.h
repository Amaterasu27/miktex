/* miktex/texmfapp.h: TeX/MF common implementation		-*- C++ -*-

   Copyright (C) 1996-2007 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(GUARD_02DB7BCAFC0F214F8FDED7403B6A014C_)
#define GUARD_02DB7BCAFC0F214F8FDED7403B6A014C_

#if defined(MIKTEX_TEX) || defined(MIKTEX_TRIPTEX)
#  define MIKTEX_TEX_COMPILER 1
#endif

#if defined(MIKTEX_PDFTEX)
#  define MIKTEX_TEX_COMPILER 1
#endif

#if defined(MIKTEX_XETEX)
#  define MIKTEX_TEX_COMPILER 1
#  define MIKTEX_TEXMF_UNICODE 1
#endif

#if defined(MIKTEX_OMEGA)
#  define MIKTEX_TEX_COMPILER 1
#  define MIKTEX_TEXMF_UNICODE 1
#endif

#if defined(MIKTEX_METAFONT) || defined(MIKTEX_METAPOST)
#  define MIKTEX_META_COMPILER 1
#  define ENABLE_8BIT_CHARS 1
#  define HAVE_MAIN_MEMORY 1
#  define IMPLEMENT_TCX 1
#  if defined(MIKTEX_METAPOST)
#    define HAVE_POOL_FREE 1
#    define HAVE_EXTRA_MEM_TOP 1
#    define HAVE_MAIN_MEMORY 1
#  endif
#endif

#if defined(MIKTEX_TEX_COMPILER)
#    define HAVE_EXTRA_MEM_BOT 1
#    define HAVE_EXTRA_MEM_TOP 1
#    define HAVE_MAIN_MEMORY 1
#    define HAVE_POOL_FREE 1
#    define HAVE_STRINGS_FREE 1
#  if ! (defined(MIKTEX_XETEX) || defined(MIKTEX_OMEGA))
#    define IMPLEMENT_TCX 1
#  endif
#  if ! defined(MIKTEX_OMEGA)
#    define ENABLE_8BIT_CHARS 1
#  endif
#endif

#if defined(MIKTEX_BIBTEX)
#  define IMPLEMENT_TCX 1
#endif

#include <miktex/inputline.h>
#include <miktex/reg.h>

namespace texmfapp {
#include <miktex/texmfapp.defaults.h>
}

#pragma warning (push, 1)
#include <string>
#pragma warning (pop)

#include <csignal>

#include <fcntl.h>
#include <io.h>

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

#if defined(MIKTEX_TEXMF_UNICODE)
typedef wchar_t TEXMFCHAR;
typedef unsigned wchar_t TEXMFUCHAR;
typedef wint_t TEXMFCHARINT;
#else
typedef char TEXMFCHAR;
typedef unsigned char TEXMFUCHAR;
typedef int TEXMFCHARINT;
#endif

/* _________________________________________________________________________

   GETPARAM
   _________________________________________________________________________ */

#define GETPARAM(param, varname, cfgname, defcfgval)		\
{								\
  if (param < 0)						\
    {								\
      THEDATA(varname) =					\
        pSession->GetConfigValue(0,				\
                                 MIKTEXTEXT(#cfgname),		\
                                 defcfgval);			\
    }								\
  else								\
    {								\
      THEDATA(varname) = param;					\
    }								\
}

/* _________________________________________________________________________

   GETPARAMCHECK
   _________________________________________________________________________ */

#define GETPARAMCHECK(param, varname, cfgname, defcfgval)	\
{								\
  if (param < 0)						\
    {								\
      THEDATA(varname) =					\
        pSession->GetConfigValue(0,				\
                                 MIKTEXTEXT(#cfgname),		\
                                 defcfgval);			\
    }								\
  else								\
    {								\
      THEDATA(varname) = param;					\
    }								\
  if (THEDATA(varname) < inf##varname				\
      || THEDATA(varname) > sup##varname)			\
    {								\
      MiKTeX::Core::Session::FatalMiKTeXError			\
	(MIKTEXTEXT("GETPARAMCHECK"),				\
	 MIKTEXTEXT("Bad parameter value."),			\
	 MIKTEXTEXT(#cfgname),					\
	 MIKTEXTEXT(__FILE__),					\
	 __LINE__);						\
    }								\
}

/* _________________________________________________________________________
   
   GetTeXString
   _________________________________________________________________________ */

#if defined(THEDATA)
inline
TEXMFCHAR *
GetTeXString (/*[out]*/ TEXMFCHAR *	lpsz,
	      /*[out]*/ size_t		size,
	      /*[in]*/ int		stringStart,
	      /*[in]*/ int		stringLength)
{
  MIKTEX_ASSERT (sizeof(THEDATA(strpool)[0]) == sizeof(lpsz[0]));
  if (stringLength < 0 || stringLength >= size)
    {
      MiKTeX::Core::Session::FatalMiKTeXError
	(MIKTEXTEXT("GetTeXString"),
	 MIKTEXTEXT("Bad string size."),
	 0,
	 MIKTEXTEXT(__FILE__),
	 __LINE__);
    }
  for (int idx = 0; idx < stringLength; ++ idx)
    {
      lpsz[idx] = THEDATA(strpool)[stringStart + idx];
    }
  lpsz[stringLength] = 0;
  return (lpsz);
}
#endif // THEDATA

/* _________________________________________________________________________
   
   GetTeXString
   _________________________________________________________________________ */

#if defined(THEDATA) && defined(MIKTEX_TEXMF_UNICODE)
inline
char *
GetTeXString (/*[out]*/ char *	lpsz,
	      /*[out]*/ size_t	size,
	      /*[in]*/ int	stringStart,
	      /*[in]*/ int	stringLength)
{
  MiKTeX::Core::CharBuffer<wchar_t, 200> buf (stringLength + 1);
  GetTeXString (buf.GetBuffer(), buf.GetCapacity(), stringStart, stringLength);
  MiKTeX::Core::Utils::CopyString (lpsz, size, buf.Get());
  return (lpsz);
}
#endif

/* _________________________________________________________________________
   
   GetTeXStringStart
   _________________________________________________________________________ */

#if defined(THEDATA)
inline
int
GetTeXStringStart (/*[in]*/ int stringNumber)
{
#if defined(MIKTEX_OMEGA) || defined(MIKTEX_XETEX)
  MIKTEX_ASSERT (stringNumber >= 65536);
  stringNumber -= 65536;
#endif
  MIKTEX_ASSERT (stringNumber >= 0 && stringNumber < THEDATA(strptr));
#if defined(MIKTEX_OMEGA)
  int stringStart = THEDATA(strstartar)[stringNumber];
#else
  int stringStart = THEDATA(strstart)[stringNumber];
#endif
  return (stringStart);
}
#endif // THEDATA

/* _________________________________________________________________________
   
   GetTeXStringLength
   _________________________________________________________________________ */

#if defined(THEDATA)
inline
int
GetTeXStringLength (/*[in]*/ int stringNumber)
{
#if defined(MIKTEX_OMEGA) || defined(MIKTEX_XETEX)
  MIKTEX_ASSERT (stringNumber >= 65536);
  stringNumber -= 65536;
#endif
  MIKTEX_ASSERT (stringNumber >= 0 && stringNumber < THEDATA(strptr));
#if defined(MIKTEX_OMEGA)
  int stringLength =
    (THEDATA(strstartar)[stringNumber + 1]
     - THEDATA(strstartar)[stringNumber]);
#else
  int stringLength =
    (THEDATA(strstart)[stringNumber + 1] - THEDATA(strstart)[stringNumber]);
#endif
  return (stringLength);
}
#endif // THEDATA

/* _________________________________________________________________________
   
   GetTeXString
   _________________________________________________________________________ */

#if defined(THEDATA)
template<typename CharType>
inline
CharType *
GetTeXString (/*[out]*/ CharType *	lpsz,
	      /*[in]*/ int		stringNumber,
	      /*[in]*/ size_t		size = 0xffff)
{
  int stringStart = GetTeXStringStart(stringNumber);
  int stringLength = GetTeXStringLength(stringNumber);
  return (GetTeXString(lpsz, size, stringStart, stringLength));
}
#endif // THEDATA

/* _________________________________________________________________________

   TeXMFApp
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
TeXMFApp

  : public WebAppInputLine

{
  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName);

  /* _______________________________________________________________________
     
     Finalize
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  Finalize ();

  /* _______________________________________________________________________
     
     AddOptions
     _______________________________________________________________________ */

protected:
  
  virtual
  MIKTEXMFAPI(void)
  AddOptions ();

  /* _______________________________________________________________________
     
     GetUsage
     _______________________________________________________________________ */

protected:

  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetUsage ()
    const
  {
    return (MIKTEXTEXT("[OPTION...] [COMMAND...]"));
  }

  /* _______________________________________________________________________
     
     ProcessCommandLineOptions
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  ProcessCommandLineOptions ();
  
  /* _______________________________________________________________________
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(bool)
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg);

  /* _______________________________________________________________________
     
     TouchJobOutputFile
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  TouchJobOutputFile (/*[in]*/ FILE * pFile)
    const;

  /* _______________________________________________________________________
     
     GetMemoryDumpFileExtension
     _______________________________________________________________________ */

public:

  virtual
  const char *
  MIKTEXMFCALL
  GetMemoryDumpFileExtension ()
    const
  {
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     GetMemoryDumpFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetMemoryDumpFileType ()
    const
  {
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (MiKTeX::Core::FileType::None);
  }

  /* _______________________________________________________________________
     
     GetMemoryDumpFileName
     _______________________________________________________________________ */

protected:

  virtual
  const char *
  MIKTEXMFCALL
  GetMemoryDumpFileName ()
    const
  {
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     GetInitProgramName
     _______________________________________________________________________ */

protected:

  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetInitProgramName ()
    const
  {
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     GetPoolFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetPoolFileType ()
    const
  {
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (MiKTeX::Core::FileType::None);
  }

  /* _______________________________________________________________________
     
     GetPoolFileName
     _______________________________________________________________________ */

public:

  virtual
  const char *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     GetVirginProgramName
     _______________________________________________________________________ */

protected:

  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetVirginProgramName ()
    const
  {
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     OnTeXMFInitialize
     _______________________________________________________________________ */

public:

  virtual
  void
  MIKTEXMFCALL
  OnTeXMFInitialize ()
    const
  {
#if defined(THEDATA)
    signal (SIGINT, OnKeybordInterrupt);
#endif
  }
  
  /* _______________________________________________________________________
     
     OnTeXMFStartJob
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  OnTeXMFStartJob ();

  /* _______________________________________________________________________
     
     OnTeXMFFinishJob
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  OnTeXMFFinishJob ();

  /* _______________________________________________________________________
     
     Reallocate
     _______________________________________________________________________ */

public:
  template<typename T>
  T *
  Reallocate (/*[in]*/ const char *	lpszTableName,
	      /*[in,out]*/ T * &	p,
	      /*[in]*/ size_t		n)
  {
    size_t amount = (n + 1) * sizeof(T);
    if (trace_mem->IsEnabled())
      {
	trace_mem->WriteFormattedLine
	  (MIKTEXTEXT("libtexmf"),
	   MIKTEXTEXT("\
Reallocate %s: p == %p, elementSize == %u, nElements == %u, bytes == %u"),
	   (lpszTableName == 0 ? "table" : lpszTableName),
	   p,
	   static_cast<unsigned>(sizeof(T)),
	   static_cast<unsigned>(n),
	   static_cast<unsigned>(amount));
      }
    p = reinterpret_cast<T *>(realloc(p, amount));
    if (p == 0 && amount > 0)
      {
	FatalError (MIKTEXTEXT("\
Virtual memory exhausted while trying to reserve %u bytes for table %s."),
		    static_cast<unsigned>(amount),
		    (lpszTableName == 0 ? "???" : lpszTableName));
      }
    if (trace_mem->IsEnabled())
      {
	trace_mem->WriteFormattedLine
	  (MIKTEXTEXT("libtexmf"),
	   MIKTEXTEXT("\
Reallocate: return %p"),
	   p);
      }
    return (p);
  }
  
  /* _______________________________________________________________________
     
     Reallocate
     _______________________________________________________________________ */

public:
  template<typename T>
  T *
  Reallocate (/*[in,out]*/ T * &	p,
	      /*[in]*/ size_t		n)
  {
    return (Reallocate(0, p, n));
  }
  
  /* _______________________________________________________________________
     
     Allocate
     _______________________________________________________________________ */

protected:
  template<typename T>
  T *
  Allocate (/*[in]*/ const char *	lpszTableName,
	    /*[in,out]*/ T * &		p,
	    /*[in]*/ size_t		n)
  {
    p = 0;
    return (Reallocate(lpszTableName, p, n));
  }

  /* _______________________________________________________________________
     
     Allocate
     _______________________________________________________________________ */

protected:
  template<typename T>
  T *
  Allocate (/*[in,out]*/ T * &	p,
	    /*[in]*/ size_t	n)
  {
    return (Allocate(0, p, n));
  }

  /* _______________________________________________________________________
     
     Free
     _______________________________________________________________________ */

protected:
  template<typename T>
  T *
  Free (/*[in]*/ const char *	lpszTableName,
	/*[in,out]*/ T * &	p)
  {
    return (Reallocate(lpszTableName, p, 0));
  }
  
  /* _______________________________________________________________________
     
     Free
     _______________________________________________________________________ */

protected:
  template<typename T>
  T *
  Free (/*[in,out]*/ T * &	p)
  {
    return (Free(0, p));
  }
  
  /* _______________________________________________________________________
     
     AllocateMemory
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  AllocateMemory ()
  {
    GETPARAMCHECK (param_buf_size,
		   bufsize,
		   buf_size,
		   texmfapp::texmfapp::buf_size());
    GETPARAMCHECK (param_error_line,
		   errorline,
		   error_line,
		   texmfapp::texmfapp::error_line());
#if defined(HAVE_EXTRA_MEM_BOT)
    GETPARAM (param_extra_mem_bot,
	      extramembot,
	      extra_mem_bot,
	      texmfapp::texmfapp::extra_mem_bot());
#endif
#if defined(HAVE_EXTRA_MEM_TOP)
    GETPARAM (param_extra_mem_top,
	      extramemtop,
	      extra_mem_top,
	      texmfapp::texmfapp::extra_mem_top());
#endif
    GETPARAMCHECK (param_half_error_line,
		   halferrorline,
		   half_error_line,
		   texmfapp::texmfapp::half_error_line());
#if defined(HAVE_MAIN_MEMORY)
#  if ! defined(infmainmemory)
    const int infmainmemory = 3000;
    const int supmainmemory = 32000000;
#  endif
    GETPARAMCHECK (param_main_memory,
		   mainmemory,
		   main_memory,
		   texmfapp::texmfapp::main_memory());
#endif
    GETPARAMCHECK (param_max_print_line,
		   maxprintline,
		   max_print_line,
		   texmfapp::texmfapp::max_print_line());
    GETPARAMCHECK (param_max_strings,
		   maxstrings,
		   max_strings,
		   texmfapp::texmfapp::max_strings());
#if ! defined(infparamsize)
    const int infparamsize = 60;
    const int supparamsize = 600000;
#endif
    GETPARAMCHECK (param_param_size,
		   paramsize,
		   param_size,
		   texmfapp::texmfapp::param_size());
#if defined(HAVE_POOL_FREE)
    GETPARAMCHECK (param_pool_free,
		   poolfree,
		   pool_free,
		   texmfapp::texmfapp::pool_free());
#endif
    GETPARAMCHECK (param_pool_size,
		   poolsize,
		   pool_size,
		   texmfapp::texmfapp::pool_size());
    GETPARAMCHECK (param_stack_size,
		   stacksize,
		   stack_size,
		   texmfapp::texmfapp::stack_size());
#if defined(HAVE_STRINGS_FREE)
    GETPARAMCHECK (param_strings_free,
		   stringsfree,
		   strings_free,
		   texmfapp::texmfapp::strings_free());
#endif
    GETPARAMCHECK (param_string_vacancies,
		   stringvacancies,
		   string_vacancies,
		   texmfapp::texmfapp::string_vacancies());
    
    THEDATA(maxstrings) += 0x100;

#if defined(HAVE_EXTRA_MEM_BOT)
    if (IsInitProgram())
      {
	THEDATA(extramembot) = 0;
      }
    if (THEDATA(extramembot) > supmainmemory)
      {
	THEDATA(extramembot) = supmainmemory;
      }
#endif

#if defined(HAVE_EXTRA_MEM_TOP)
    if (IsInitProgram())
      {
	THEDATA(extramemtop) = 0;
      }
    if (THEDATA(extramemtop) > supmainmemory)
      {
	THEDATA(extramemtop) = supmainmemory;
      }
#endif

#if defined(MIKTEX_TEX_COMPILER)
    THEDATA(memtop) = THEDATA(membot) + THEDATA(mainmemory) - 1;
    THEDATA(memmin) = THEDATA(membot);
    THEDATA(memmax) = THEDATA(memtop);
#elif defined(MIKTEX_META_COMPILER)
    THEDATA(memtop) = 0/*memmin*/ + THEDATA(mainmemory) - 1;
    THEDATA(memmax) = THEDATA(memtop);
#endif

    Allocate ("buffer", THEDATA(buffer), THEDATA(bufsize));
    Allocate ("inputstack", THEDATA(inputstack), THEDATA(stacksize));
    Allocate ("paramstack", THEDATA(paramstack), THEDATA(paramsize));
    Allocate ("trickbuf", THEDATA(trickbuf), THEDATA(errorline));

    if (IsInitProgram() || AmI(MIKTEXTEXT("mf")))
      {
	Allocate (THEDATA(strpool), THEDATA(poolsize));
      }

    if (IsInitProgram())
      {
#  if defined(MIKTEX_TEX_COMPILER)
	Allocate ("mem",
		  THEDATA(yzmem),
		  THEDATA(memtop) - THEDATA(membot) + 2);
	THEDATA(zmem) = THEDATA(yzmem);
	THEDATA(mem) = THEDATA(zmem);
#  else
	Allocate ("mem", THEDATA(mem), THEDATA(memtop) - 0/*memmin*/ + 2);
#  endif
      }

#if ! defined(MIKTEX_OMEGA)
    Allocate ("strstart", THEDATA(strstart), THEDATA(maxstrings));
#endif
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     FreeMemory
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  FreeMemory ()
  {
    Free (THEDATA(buffer));
    Free (THEDATA(mem));
    Free (THEDATA(paramstack));
    Free (THEDATA(strpool));
    Free (THEDATA(trickbuf));
#if ! defined(MIKTEX_OMEGA)
    Free (THEDATA(strstart));
#endif
  }
#endif // THEDATA

  /* _______________________________________________________________________

     CheckPoolPointer
     _______________________________________________________________________ */

protected:

#if defined(THEDATA)
  void
  CheckPoolPointer (/*[in]*/ int	poolptr,
		    /*[in]*/ size_t	len)
    const
  {
#if ! defined(poolsize)
    const size_t poolsize = THEDATA(poolsize);
#endif
    if (poolptr + len >= poolsize)
      {
	MiKTeX::Core::Session::FatalMiKTeXError
	  (MIKTEXTEXT("TeXMFApp::CheckPoolPointer"),
	   MIKTEXTEXT("String pool overflow."),
	   0,
	   MIKTEXTEXT(__FILE__),
	   __LINE__);
      }
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     CStyleErrorMessagesP
     _______________________________________________________________________ */

public:

  bool
  CStyleErrorMessagesP ()
    const
  {
    return (showFileLineErrorMessages);
  }

  /* _______________________________________________________________________
     
     GetDefaultMemoryDumpFileName
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(void)
  GetDefaultMemoryDumpFileName (/*[out]*/ char * lpszPath)
    const;

  /* _______________________________________________________________________
     
     GetInteraction
     _______________________________________________________________________ */

public:

  int
  GetInteraction ()
    const
  {
    return (interactionMode);
  }

  /* _______________________________________________________________________
     
     GetJobName
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  int
  GetJobName ()
  {				
    if (jobName.empty())
      {
	char szName[MiKTeX::Core::BufferSizes::MaxPath];
	MiKTeX::Core::PathName::Split
	  (lastInputFileName.Get(),
	   0, 0,
	   szName, MiKTeX::Core::BufferSizes::MaxPath,
	   0, 0);
	jobName = szName;
      }
    // <fixme>conserve strpool space</fixme>
    return (MakeTeXString(jobName.c_str()));
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     HaltOnErrorP
     _______________________________________________________________________ */

public:

  bool
  HaltOnErrorP ()
    const
  {
    return (haltOnError);
  }

  /* _______________________________________________________________________
     
     InitializeBuffer
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(unsigned long)
  InitializeBuffer (/*[in]*/ unsigned char * pBuffer);

  /* _______________________________________________________________________
     
     InitializeBuffer
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(unsigned long)
  InitializeBuffer (/*[in]*/ unsigned short * pBuffer);

  /* _______________________________________________________________________
     
     InvokeEditor
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(void)
  InvokeEditor (/*[in]*/ const MiKTeX::Core::PathName &	editFileName,
		/*[in]*/ int				editLineNumber,
		/*[in]*/ const MiKTeX::Core::PathName &	transcriptFileName)
    const;

  /* _______________________________________________________________________
     
     InvokeEditor
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  InvokeEditor (/*[in]*/ int		editFileName_,
		/*[in]*/ int		editFileNameLength,
		/*[in]*/ int		editLineNumber,
		/*[in]*/ int		transcriptFileName_,
		/*[in]*/ int		transcriptFileNameLength)
    const
  {
    TEXMFCHAR szEditFileName[Core::BufferSizes::MaxPath];
    GetTeXString (szEditFileName,
		  Core::BufferSizes::MaxPath,
		  editFileName_,
		  editFileNameLength);
    TEXMFCHAR szTranscriptFileName[Core::BufferSizes::MaxPath];
    if (transcriptFileName_ != 0)
      {
	GetTeXString (szTranscriptFileName,
		      Core::BufferSizes::MaxPath,
		      transcriptFileName_,
		      transcriptFileNameLength);
      }
    else
      {
	szTranscriptFileName[0] = 0;
      }
    InvokeEditor (szEditFileName, editLineNumber, szTranscriptFileName);
  }
#endif

  /* _______________________________________________________________________
     
     IsInitProgram
     _______________________________________________________________________ */

public:

  bool
  IsInitProgram ()
    const
  {
    return (isInitProgram);
  }

  /* _______________________________________________________________________
     
     IsVirgin
     _______________________________________________________________________ */

protected:

  MIKTEXMFAPI(bool)
  IsVirgin ()
    const;

  /* _______________________________________________________________________
     
     MakeFullNameString
     _______________________________________________________________________ */
  
public:

#if defined(THEDATA)
  int
  MakeFullNameString ()
  {
    return (MakeTeXString(GetFQNameOfFile()));
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     MakeTeXString
     _______________________________________________________________________ */
  
public:

#if defined(THEDATA)
  template<typename CharType>
  int
  MakeTeXString (/*[in]*/ const CharType * lpsz)
  {
    MIKTEX_ASSERT_STRING (lpsz);
    size_t len = MiKTeX::Core::StrLen(lpsz);
    CheckPoolPointer (THEDATA(poolptr), len);
    while (len-- > 0)
      {
	THEDATA(strpool)[THEDATA(poolptr)++] = *lpsz++;
      }
    return (makestring());
  }
#endif // THEDATA
  
  /* _______________________________________________________________________
     
     MakeTeXString
     _______________________________________________________________________ */
  
public:

#if defined(THEDATA) && defined(MIKTEX_XETEX)
  template<>
  int
  MakeTeXString (/*[in]*/ const unsigned char * lpszUtf8)
  {
    std::wstring str =
      MiKTeX::Core::Utils::UTF8ToWideChar
      (reinterpret_cast<const char*>(lpszUtf8));
    return (MakeTeXString(str.c_str()));
  }
#endif // THEDATA
  
  /* _______________________________________________________________________

     OpenMemoryDumpFile
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(bool)
  OpenMemoryDumpFile (/*[in]*/ const MiKTeX::Core::PathName &	fileName,
		      /*[in]*/ FILE **				ppFile,
		      /*[in]*/ void *				pBuf,
		      /*[in]*/ size_t				size,
		      /*[in]*/ bool				renew)
    const;

  /* _______________________________________________________________________

     OpenMemoryDumpFile
     _______________________________________________________________________ */

public:

#if defined(THEDATA)  
  template<class T>
  bool
  OpenMemoryDumpFile (/*[in]*/ T &	f,
		      /*[in]*/ bool	renew = false)
    const
  {
    FILE * pfile;
    if (! OpenMemoryDumpFile(GetNameOfFile(),
			     &pfile,
			     &f.bufref(),
			     sizeof(*f),
			     renew))
      {
	return (false);
      }
    f.Attach (pfile, true);
    return (true);
  }
#endif // THEDATA

  /* _______________________________________________________________________

     Dump
     _______________________________________________________________________ */

public:

#if defined(THEDATA)  
  template<typename FILE_,
	   typename ELETYPE_>
  void
  Dump (/*[in]*/ FILE_ &		f,
	/*[in]*/ const ELETYPE_ &	e,
	/*[in]*/ size_t			n)
  {
    if (fwrite(&e, sizeof(e), n, static_cast<FILE*>(f)) != n)
      {
	MiKTeX::Core::Session::FatalCrtError
	  (MIKTEXTEXT("fwrite"),
	   0,
	   MIKTEXTEXT(__FILE__),
	   __LINE__);
      }
  }
#endif // THEDATA

  /* _______________________________________________________________________

     Dump
     _______________________________________________________________________ */

public:

#if defined(THEDATA)  
  template<typename FILE_,
	   typename ELETYPE_>
  void
  Dump (/*[in]*/ FILE_ &		f,
	/*[in]*/ const ELETYPE_ &	e)
  {
    Dump (f, e, 1);
  }
#endif // THEDATA

  /* _______________________________________________________________________

     Undump
     _______________________________________________________________________ */

public:

#if defined(THEDATA)  
  template<typename FILE_,
	   typename ELETYPE_>
  void
  Undump (/*[in]*/ FILE_ &		f,
	  /*[out]*/ ELETYPE_ &		e,
	  /*[in]*/ size_t		n)
  {
    if (fread(&e, sizeof(e), n, static_cast<FILE*>(f)) != n)
      {
	MiKTeX::Core::Session::FatalCrtError
	  (MIKTEXTEXT("fread"),
	   0,
	   MIKTEXTEXT(__FILE__),
	   __LINE__);
      }
  }
#endif // THEDATA

  /* _______________________________________________________________________

     Undump
     _______________________________________________________________________ */

public:

#if defined(THEDATA)  
  template<typename FILE_,
	   typename ELETYPE_>
  void
  Undump (/*[in]*/ FILE_ &		f,
	  /*[in]*/ ELETYPE_ &		e)
  {
    Undump (f, e, 1);
  }
#endif // THEDATA

  /* _______________________________________________________________________

     Undump
     _______________________________________________________________________ */

public:

#if defined(THEDATA)  
  template<typename FILE_,
	   typename ELETYPE_>
  void
  Undump (/*[in]*/ FILE_ &	f,
	  /*[in]*/ ELETYPE_	low,
	  /*[in]*/ ELETYPE_	high,
	  /*[out]*/ ELETYPE_ &	e,
	  /*[in]*/ size_t	n)
  {
    Undump (f, e, n);
    for (size_t idx = 0; idx < n; ++ idx)
      {
	if ((&e)[idx] < low || (&e)[idx] > high)
	  {
	    MiKTeX::Core::Session::FatalMiKTeXError
	      (MIKTEXTEXT("GetTeXString"),
	       MIKTEXTEXT("Bad format file."),
	       0,
	       MIKTEXTEXT(__FILE__),
	       __LINE__);
	  }
      }
  }
#endif // THEDATA

  /* _______________________________________________________________________

     Undump
     _______________________________________________________________________ */

public:

#if defined(THEDATA)  
  template<typename FILE_,
	   typename ELETYPE_>
  void
  Undump (/*[in]*/ FILE_ &	f,
	  /*[in]*/ ELETYPE_	high,
	  /*[out]*/ ELETYPE_ &	e,
	  /*[in]*/ size_t	n)
  {
    Undump (f, e, n);
    for (size_t idx = 0; idx < n; ++ idx)
      {
	if ((&e)[idx] > high)
	  {
	    MiKTeX::Core::Session::FatalMiKTeXError
	      (MIKTEXTEXT("GetTeXString"),
	       MIKTEXTEXT("Bad format file."),
	       0,
	       MIKTEXTEXT(__FILE__),
	       __LINE__);
	  }
      }
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     OpenPoolFile
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(bool)
  OpenPoolFile (/*[in]*/ void *				p,
		/*[in]*/ const MiKTeX::Core::PathName &	fileName)
    const;

  /* _______________________________________________________________________
     
     OpenPoolFile
     _______________________________________________________________________ */

#if defined(THEDATA)
  template<class T>
  bool
  OpenPoolFile (/*[in]*/ T & f)
    const
  {
    return (OpenPoolFile(&f, GetNameOfFile()));
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     ParseFirstLine
     _______________________________________________________________________ */

public:
  static
  MIKTEXMFAPI(bool)
  ParseFirstLine (/*[in]*/ const MiKTeX::Core::PathName &	path,
		  /*[in,out]*/ MiKTeX::Core::Argv &		argv);

  /* _______________________________________________________________________
     
     ParseFirstLine
     _______________________________________________________________________ */

private:

  MIKTEXMFAPI(void)
  ParseFirstLine (/*[in]*/ const MiKTeX::Core::PathName &	fileName);

  /* _______________________________________________________________________
     
     OnKeyboardInterrupt
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  static
  void
  __cdecl
  OnKeybordInterrupt (/*[in]*/ int)
  {
    signal (SIGINT, SIG_IGN);
    THEDATA(interrupt) = 1;
    signal (SIGINT, OnKeybordInterrupt);
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     SetTeX
     _______________________________________________________________________ */

protected:

  void
  SetTeX ()
  {
    isTeXProgram = true;
  }

  /* _______________________________________________________________________
     
     AmITeXCompiler
     _______________________________________________________________________ */

protected:

  bool
  AmITeXCompiler()
    const
  {
    return (isTeXProgram);
  }

  /* _______________________________________________________________________
     
     Variables
     _______________________________________________________________________ */

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_time;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_mem;

private:
  MiKTeX::Core::tstring memoryDumpFileName;

private:
  MiKTeX::Core::tstring jobName;

private:
  clock_t clockStart;

private:
  bool timeStatistics;

private:
  bool parseFirstLine;

private:
  bool showFileLineErrorMessages;

private:
  bool haltOnError;

private:
  bool isInitProgram;

private:
  bool setJobTime;

private:
  bool recordFileNames;

private:
  bool disableExtensions;

private:
  bool isTeXProgram;

private:
  int interactionMode;

private:
  int param_buf_size;

private:
  int param_error_line;

private:
  int param_extra_mem_bot;

private:
  int param_extra_mem_top;

private:
  int param_half_error_line;

private:
  int param_main_memory;

private:
  int param_max_print_line;

private:
  int param_max_strings;

private:
  int param_param_size;

private:
  int param_pool_free;

private:
  int param_pool_size;

private:
  int param_stack_size;

private:
  int param_strings_free;

private:
  int param_string_vacancies;

private:
  int optBase;
};

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// miktex/texmfapp.h
