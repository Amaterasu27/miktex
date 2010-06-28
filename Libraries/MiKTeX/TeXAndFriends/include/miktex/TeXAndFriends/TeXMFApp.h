/* miktex/TeXAndFriends/TeXMFApp:				-*- C++ -*-

   Copyright (C) 1996-2010 Christian Schenk

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

#if defined(MIKTEX_METAFONT)
#  define MIKTEX_META_COMPILER 1
#  define ENABLE_8BIT_CHARS 1
#  define HAVE_MAIN_MEMORY 1
#  define IMPLEMENT_TCX 1
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

#include <miktex/TeXAndFriends/WebAppInputLine>
#include <miktex/Core/Registry>

namespace texmfapp {
#include <miktex/texmfapp.defaults.h>
}

#pragma warning (push, 1)
#include <memory>
#include <string>
#pragma warning (pop)

#include <csignal>

#if 0
#include <fcntl.h>
#include <io.h>
#endif

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

#if defined(MIKTEX_TEXMF_UNICODE)
typedef wchar_t TEXMFCHAR;
typedef wint_t TEXMFCHARINT;
#else
typedef char TEXMFCHAR;
typedef int TEXMFCHARINT;
#endif

/* _________________________________________________________________________

   GETPARAM
   _________________________________________________________________________ */

#define GETPARAM(param, varname, cfgname, defcfgval)			\
{									\
  if (param < 0)							\
    {									\
      THEDATA(varname) = GetParameter(#cfgname, defcfgval);		\
    }									\
  else									\
    {									\
      THEDATA(varname) = param;						\
    }									\
  if (trace_mem->IsEnabled())						\
    {									\
      trace_mem->WriteFormattedLine					\
	("libtexmf",							\
	 MIKTEXTEXT("Parameter %s: %d"),				\
	 #cfgname,							\
	 static_cast<int>(THEDATA(varname)));				\
      }									\
}

/* _________________________________________________________________________

   GETPARAMCHECK
   _________________________________________________________________________ */

#define GETPARAMCHECK(param, varname, cfgname, defcfgval)		\
{									\
  if (param < 0)							\
    {									\
      THEDATA(varname) = GetParameter(#cfgname, defcfgval);		\
    }									\
  else									\
    {									\
      THEDATA(varname) = param;						\
    }									\
  if (THEDATA(varname) < inf##varname					\
      || THEDATA(varname) > sup##varname)				\
    {									\
      MiKTeX::Core::Session::FatalMiKTeXError				\
	("GETPARAMCHECK",						\
	 MIKTEXTEXT("Bad parameter value."),				\
	 #cfgname,							\
	 __FILE__,							\
	 __LINE__);							\
    }									\
  if (trace_mem->IsEnabled())						\
    {									\
      trace_mem->WriteFormattedLine					\
	("libtexmf",							\
	 MIKTEXTEXT("Parameter %s: %d"),				\
	 #cfgname,							\
	 static_cast<int>(THEDATA(varname)));				\
    }									\
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
	("GetTeXString",
	 MIKTEXTEXT("Bad string size."),
	 0,
	 __FILE__,
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
MIKTEXMFTYPEAPI(TeXMFApp)

  : public WebAppInputLine

{
  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(void)
  Init (/*[in]*/ const char * lpszProgramInvocationName);

  /* _______________________________________________________________________
     
     Finalize
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(void)
  Finalize ();

  /* _______________________________________________________________________
     
     AddOptions
     _______________________________________________________________________ */

protected:
  
  virtual
  MIKTEXMFTHISAPI(void)
  AddOptions ();

  /* _______________________________________________________________________
     
     GetUsage
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(const char *)
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
  MIKTEXMFTHISAPI(void)
  ProcessCommandLineOptions ();
  
  /* _______________________________________________________________________
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(bool)
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const char *	lpszOptArg);

  /* _______________________________________________________________________
     
     TouchJobOutputFile
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(void)
  TouchJobOutputFile (/*[in]*/ FILE * pFile)
    const;

  /* _______________________________________________________________________
     
     GetMemoryDumpFileExtension
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(const char *)
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
  MIKTEXMFTHISAPI(MiKTeX::Core::FileType)
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
  MIKTEXMFTHISAPI(const char *)
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
  MIKTEXMFTHISAPI(const char *)
  GetInitProgramName ()
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
  MIKTEXMFTHISAPI(const char *)
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
  MIKTEXMFTHISAPI(void)
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
  MIKTEXMFTHISAPI(void)
  OnTeXMFStartJob ();

  /* _______________________________________________________________________
     
     OnTeXMFFinishJob
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(void)
  OnTeXMFFinishJob ();

  /* _______________________________________________________________________
     
     CheckMemory
     _______________________________________________________________________ */

#if defined(MIKTEX_DEBUG)
public:
  virtual
  void
  CheckMemory ()
  {
#if defined(THEDATA)
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(buffer));
#  if defined(MIKTEX_TEX_COMPILER)
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(yzmem));
#else
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(mem));
#endif
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(paramstack));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(strpool));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trickbuf));
#  if ! defined(MIKTEX_OMEGA)
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(strstart));
#  endif
#endif
  }
#endif

  /* _______________________________________________________________________
     
     GetParameter
     _______________________________________________________________________ */

protected:
  template<typename ValueType>
  ValueType
  GetParameter (/*[in]*/ const char *		lpszParameterName,
		/*[in]*/ const ValueType &	defaultValue)
    const
  {
    ValueType value = pSession->GetConfigValue(0, lpszParameterName, -1);
    if (value < 0)
      {
	value =
	  pSession->GetConfigValue(GetProgramName(),
				   lpszParameterName,
				   defaultValue);
      }
    return (value);
  }

  /* _______________________________________________________________________
     
     Reallocate
     _______________________________________________________________________ */

public:
  template<typename T>
  T *
  Reallocate (/*[in]*/ const char *	lpszArrayName,
	      /*[in,out]*/ T * &	p,
	      /*[in]*/ size_t		n,
	      /*[in]*/ const char *	lpszFileName,
	      /*[in]*/ int		line)
  {
    size_t amount;
    if (n == 0)
      {
	amount = 0;
      }
    else
      {
	// one extra element becaus Pascal arrays are 1-based
	amount = (n + 1) * sizeof(T);
      }
    if (trace_mem->IsEnabled())
      {
	trace_mem->WriteFormattedLine
	  ("libtexmf",
	   MIKTEXTEXT("\
Reallocate %s: p == %p, elementSize == %u, nElements == %u, bytes == %u"),
	   (lpszArrayName == 0 ? "array" : lpszArrayName),
	   p,
	   static_cast<unsigned>(sizeof(T)),
	   static_cast<unsigned>(n),
	   static_cast<unsigned>(amount));
      }
    p = 
      reinterpret_cast<T*>(MiKTeX::Debug::Realloc(p,
						  amount,
						  lpszFileName,
						  line));
    if (trace_mem->IsEnabled())
      {
	trace_mem->WriteFormattedLine
	  ("libtexmf",
	   MIKTEXTEXT("\
Reallocate: return %p"),
	   p);
      }
    return (p);
  }
  
  /* _______________________________________________________________________
     
     Allocate
     _______________________________________________________________________ */

protected:
  template<typename T>
  T *
  Allocate (/*[in]*/ const char *	lpszArrayName,
	    /*[in,out]*/ T * &		p,
	    /*[in]*/ size_t		n)
  {
    p = 0;
    return (Reallocate(lpszArrayName, p, n, __FILE__, __LINE__));
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
  Free (/*[in]*/ const char *	lpszArrayName,
	/*[in,out]*/ T * &	p)
  {
    return (Reallocate(lpszArrayName, p, 0, __FILE__, __LINE__));
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
    const int supmainmemory = 256000000;
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
    MIKTEX_ASSERT (THEDATA(membot) == 0);
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

    if (IsInitProgram() || AmI("mf"))
      {
	Allocate (THEDATA(strpool), THEDATA(poolsize));
      }

    if (IsInitProgram())
      {
#  if defined(MIKTEX_TEX_COMPILER)
	Allocate ("mem",
		  THEDATA(yzmem),
		  THEDATA(memtop) - THEDATA(membot) + 2);
	MIKTEX_ASSERT (THEDATA(membot) == 0);
	THEDATA(zmem) = THEDATA(yzmem) - THEDATA(membot);
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
#  if defined(MIKTEX_TEX_COMPILER)
    Free (THEDATA(yzmem));
#else
    Free (THEDATA(mem));
#endif
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

#if defined(poolsize) || defined(THEDATA)
  void
  CheckPoolPointer (/*[in]*/ int	poolPtr,
		    /*[in]*/ size_t	len)
    const
  {
#if defined(poolsize)
    const size_t poolSize = poolsize;
#else
    const size_t poolSize = THEDATA(poolsize);
#endif
    if (poolPtr + len >= poolSize)
      {
	MiKTeX::Core::Session::FatalMiKTeXError
	  ("TeXMFApp::CheckPoolPointer",
	   MIKTEXTEXT("String pool overflow."),
	   0,
	   __FILE__,
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

  MIKTEXMFTHISAPI(void)
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

  MIKTEXMFTHISAPI(unsigned long)
  InitializeBuffer (/*[in]*/ unsigned char * pBuffer);

  /* _______________________________________________________________________
     
     InitializeBuffer
     _______________________________________________________________________ */

public:

  MIKTEXMFTHISAPI(unsigned long)
  InitializeBuffer (/*[in]*/ unsigned short * pBuffer);

  /* _______________________________________________________________________
     
     InitializeBuffer
     _______________________________________________________________________ */

public:

  MIKTEXMFTHISAPI(unsigned long)
  InitializeBuffer (/*[in]*/ C4P::C4P_signed32 * pBuffer);

  /* _______________________________________________________________________
     
     InvokeEditor
     _______________________________________________________________________ */

public:

  MIKTEXMFTHISAPI(void)
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

  MIKTEXMFTHISAPI(bool)
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

  MIKTEXMFTHISAPI(bool)
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
			     0,
			     sizeof(*f),
			     renew))
      {
	return (false);
      }
    f.Attach (pfile, true);
    f.PascalFileIO (false);
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
	  ("fwrite",
	   0,
	   __FILE__,
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
    f.PascalFileIO (false);
    if (fread(&e, sizeof(e), n, static_cast<FILE*>(f)) != n)
      {
	MiKTeX::Core::Session::FatalCrtError
	  ("fread",
	   0,
	   __FILE__,
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
	      ("GetTeXString",
	       MIKTEXTEXT("Bad format file."),
	       0,
	       __FILE__,
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
	      ("GetTeXString",
	       MIKTEXTEXT("Bad format file."),
	       0,
	       __FILE__,
	       __LINE__);
	  }
      }
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     ParseFirstLine
     _______________________________________________________________________ */

public:
  static
  MIKTEXMFCEEAPI(bool)
  ParseFirstLine (/*[in]*/ const MiKTeX::Core::PathName &	path,
		  /*[in,out]*/ MiKTeX::Core::Argv &		argv);

  /* _______________________________________________________________________
     
     ParseFirstLine
     _______________________________________________________________________ */

private:

  MIKTEXMFTHISAPI(void)
  ParseFirstLine (/*[in]*/ const MiKTeX::Core::PathName &	fileName);

  /* _______________________________________________________________________
     
     OnKeyboardInterrupt
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  static
  void
  MIKTEXCEECALL
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

protected:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_mem;

private:
  std::string memoryDumpFileName;

private:
  std::string jobName;

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

template<>
inline
std::string
TeXMFApp::GetParameter (/*[in]*/ const char *		lpszParameterName,
			/*[in]*/ const std::string &	defaultValue)
  const
{
  std::string value = pSession->GetConfigValue(0, lpszParameterName, "");
  if (value.length() == 0)
    {
      value =
	pSession->GetConfigValue(GetProgramName(),
				 lpszParameterName,
				 defaultValue.c_str());
    }
  return (value);
}

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// MiKTeX/TeXAndFriends/TeXMFApp
