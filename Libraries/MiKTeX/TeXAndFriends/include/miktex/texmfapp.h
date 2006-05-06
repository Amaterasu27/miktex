/* miktex/texmfapp.h: TeX/MF common implementation		-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

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

#if defined(MIKTEX_TEX)
#  define MIKTEX_TEX_COMPILER 1
#endif

#if defined(MIKTEX_PDFTEX)
#  define MIKTEX_TEX_COMPILER 1
#endif

#if defined(MIKTEX_PDFETEX)
#  define MIKTEX_TEX_COMPILER 1
#endif

#if defined(MIKTEX_OMEGA)
#  define MIKTEX_TEX_COMPILER 1
#endif

#if defined(MIKTEX_ETEX)
#  define MIKTEX_TEX_COMPILER 1
#endif

#if defined(MIKTEX_METAFONT) || defined(MIKTEX_METAPOST)
#  define MIKTEX_META_COMPILER 1
#endif

#if defined(MIKTEX_TEX_COMPILER)
#  if ! defined(MIKTEX_OMEGA)
#    define IMPLEMENT_TCX 1
#  endif
#endif

#if defined(MIKTEX_META_COMPILER) || defined(MIKTEX_BIBTEX)
#  define IMPLEMENT_TCX 1
#endif

#include <miktex/inputline.h>
#include <miktex/reg.h>

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

/* _________________________________________________________________________

   GETPARAM
   _________________________________________________________________________ */

#define GETPARAM(option, varname, cfgname, defcfgval)		\
{								\
  if (option < 0)						\
    {								\
      THEDATA(varname) =					\
        pSession->GetConfigValue(MIKTEX_REGKEY_WEB,		\
                                 MIKTEXTEXT(#cfgname),		\
                                 defcfgval);			\
    }								\
  else								\
    {								\
      THEDATA(varname) = option;				\
    }								\
}

/* _________________________________________________________________________
   
   GetTeXString
   _________________________________________________________________________ */

#if defined(THEDATA)
inline
MIKTEXCHAR *
GetTeXString (/*[out]*/ MIKTEXCHAR *	lpsz,
	      /*[in]*/ int		stringNumber,
	      /*[in]*/ size_t		size = 0xffff)
{
  assert (sizeof(THEDATA(strpool)[0]) == sizeof(lpsz[0]));
#if defined(MIKTEX_OMEGA)
  int len =
    (THEDATA(strstartar)[stringNumber + 1]
     - THEDATA(strstartar)[stringNumber]);
  const MIKTEXCHAR * lpszStart =
    reinterpret_cast<const MIKTEXCHAR *>
    (&THEDATA(strpool)[THEDATA(strstartar)[stringNumber]]);
#else
  int len =
    (THEDATA(strstart)[stringNumber + 1] - THEDATA(strstart)[stringNumber]);
  const MIKTEXCHAR * lpszStart =
    reinterpret_cast<const MIKTEXCHAR *>
    (&THEDATA(strpool)[THEDATA(strstart)[stringNumber]]);
#endif
  if (len < 0 || static_cast<size_t>(len) >= size)
    {
      MiKTeX::Core::Session::FatalMiKTeXError
	(MIKTEXTEXT("GetTeXString"),
	 MIKTEXTEXT("Bad string size."),
	 0,
	 MIKTEXTEXT(__FILE__),
	 __LINE__);
    }
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  _tcsncpy_s (lpsz, size, lpszStart, static_cast<size_t>(len));
#else
  _tcsncpy (lpsz, lpszStart, static_cast<size_t>(len));
  lpsz[len] = 0;
#endif
  return (lpsz);
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
     
     GetDumpExtension
     _______________________________________________________________________ */

public:

  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetDumpExtension ()
    const
  {
    // must be implemented in sub-classes
    assert (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     GetDumpFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetDumpFileType ()
    const
  {
    // must be implemented in sub-classes
    assert (false);
    return (MiKTeX::Core::FileType::None);
  }

  /* _______________________________________________________________________
     
     GetDumpFileName
     _______________________________________________________________________ */

protected:

  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetDumpFileName ()
    const
  {
    // must be implemented in sub-classes
    assert (false);
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
    assert (false);
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
    assert (false);
    return (MiKTeX::Core::FileType::None);
  }

  /* _______________________________________________________________________
     
     GetPoolFileName
     _______________________________________________________________________ */

public:

  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    // must be implemented in sub-classes
    assert (false);
    return (0);
  }

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
    assert (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(bool)
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg);

  /* _______________________________________________________________________
     
     ProcessCommandLineOptions
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  ProcessCommandLineOptions ();
  
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
  Reallocate (/*[in,out]*/ T * &	p,
	      /*[in]*/ size_t		n)
  {
    size_t amount = n * sizeof(T);
    p = reinterpret_cast<T *>(realloc(p, amount));
    if (p == 0 && amount > 0)
      {
	FatalError (MIKTEXTEXT("Virtual memory exhausted."));
      }
    return (p);
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
    p = 0;
    return (Reallocate(p, n));
  }

  /* _______________________________________________________________________
     
     Free
     _______________________________________________________________________ */

protected:
  template<typename T>
  T *
  Free (/*[in,out]*/ T * &	p)
  {
    return (Reallocate(p, 0));
  }
  
  /* _______________________________________________________________________
     
     AllocateMemory
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  AllocateMemory ()
  {
    GETPARAM (m_buf_size, bufsize, buf_size, 200000);
    GETPARAM (m_error_line, errorline, error_line, 79);
    GETPARAM (m_half_error_line, halferrorline, half_error_line, 50);
    GETPARAM (m_max_print_line, maxprintline, max_print_line, 79);
    GETPARAM (m_max_strings, maxstrings, max_strings, 100000);
    GETPARAM (m_mem_max, memmax, mem_max, 2000000);
    GETPARAM (m_mem_min, memmin, mem_min, 0);
    GETPARAM (m_mem_top, memtop, mem_top, 1048576);
    GETPARAM (m_param_size, paramsize, param_size, 5000);
    GETPARAM (m_pool_size, poolsize, pool_size, 1250000);
    GETPARAM (m_stack_size, stacksize, stack_size, 1500);
    GETPARAM (m_string_vacancies, stringvacancies, string_vacancies, 400000);

#if ! defined(MIKTEX_OMEGA)
    THEDATA(maxstrings) += 0x100;
#endif

    if (IsInitProgram())
      {				// <fixme/>
	THEDATA(memmax) = THEDATA(memtop);
#if defined(MIKTEX_META_COMPILER)
	THEDATA(memmin) = 0;
#else
	THEDATA(memmin) = THEDATA(membot);
#endif
      }

    Allocate (THEDATA(buffer), THEDATA(bufsize) + 1);
    Allocate (THEDATA(inputstack), THEDATA(stacksize) + 1);
    Allocate (THEDATA(paramstack), THEDATA(paramsize) + 1);
#if defined(MIKTEX_METAPOST)
    Allocate (THEDATA(strpool), THEDATA(poolsize) + 1);
#else
    Allocate (THEDATA(strpool), THEDATA(poolsize) + 1);
#endif
    Allocate (THEDATA(trickbuf), THEDATA(errorline) + 1);

#if 0
    // <fixme/>
    Allocate (THEDATA(mem), THEDATA(memmax) - THEDATA(memmin) + 1);
#else
    Allocate (THEDATA(mem), THEDATA(memmax) + 1);
#endif

#if ! defined(MIKTEX_OMEGA)
    Allocate (THEDATA(strstart), THEDATA(maxstrings) + 1);
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
     
     GetDefaultDumpFileName
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(void)
  GetDefaultDumpFileName (/*[out]*/ MIKTEXCHAR * lpszPath)
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
    if (jobName.length() == 0)
      {
	MIKTEXCHAR szName[MiKTeX::Core::BufferSizes::MaxPath];
	MiKTeX::Core::PathName::Split (GetFQNameOfFile(),
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
     
     InitializeBufferA
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(unsigned long)
  InitializeBufferA (/*[in]*/ unsigned char * pBuffer);

  /* _______________________________________________________________________
     
     InitializeBufferW
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(unsigned long)
  InitializeBufferW (/*[in]*/ unsigned __int16 * pBuffer);

  /* _______________________________________________________________________
     
     InvokeEditorIfNecessary
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(void)
  InvokeEditorIfNecessary ()
    const;

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
     
     IsPrintable
     _______________________________________________________________________ */

public:

  bool
  IsPrintable (/*[in]*/ unsigned ch)
    const
  {
#if defined(MIKTEX_TEX_COMPILER)
    if (! disableExtensions)
      {
	return (true);
      }
#endif
    return (((ch >= MIKTEXTEXT(' ') && ch <= MIKTEXTEXT('~'))));
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
  
protected:

#if defined(THEDATA)
  int
  MakeTeXString (/*[in]*/ const MIKTEXCHAR * lpsz)
  {
    assert (lpsz != 0);
    size_t len = _tcslen(lpsz);
    CheckPoolPointer (THEDATA(poolptr), len);
    while (len-- > 0)
      {
	THEDATA(strpool)[THEDATA(poolptr)++] = *lpsz++;
      }
    return (makestring());
  }
#endif // THEDATA
  
  /* _______________________________________________________________________

     OpenDumpFile
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(bool)
  OpenDumpFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		/*[in]*/ FILE **		ppFile,
		/*[in]*/ void *			pBuf,
		/*[in]*/ size_t			size,
		/*[in]*/ bool			renew)
    const;

  /* _______________________________________________________________________

     OpenDumpFile
     _______________________________________________________________________ */

public:

#if defined(THEDATA)  
  template<class T>
  bool
  OpenDumpFile (/*[in]*/ T &	f,
		/*[in]*/ bool	renew = false)
    const
  {
    FILE * pfile;
    if (! OpenDumpFile(THEDATA(nameoffile),
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
     
     OpenPoolFile
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(bool)
  OpenPoolFile (/*[in]*/ void *			p,
		/*[in]*/ const MIKTEXCHAR *	lpszName)
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
    return (OpenPoolFile(&f, THEDATA(nameoffile)));
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     ParseFirstLine
     _______________________________________________________________________ */

public:
  static
  MIKTEXMFAPI(bool)
  ParseFirstLine (/*[in]*/ const MIKTEXCHAR *		lpszPath,
		  /*[out*/ int &			argc,
		  /*[out]*/ const MIKTEXCHAR ** &	argv);

  /* _______________________________________________________________________
     
     ParseFirstLine
     _______________________________________________________________________ */

private:

  MIKTEXMFAPI(void)
  ParseFirstLine (/*[in]*/ const MIKTEXCHAR *	lpszFileName);

  /* _______________________________________________________________________
     
     RememberEditInfo
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  RememberEditInfo (/*[in]*/ int	fileName,
		    /*[in]*/ int	lineNum)
  {
    GetTeXString (editFileName.GetBuffer(), fileName, editFileName.GetSize());
    GetTeXString (transcriptFileName.GetBuffer(),
		  THEDATA(logname),
		  transcriptFileName.GetSize());
    editLineNum = lineNum;
  }
#endif // THEDATA

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
     
     TouchJobOutputFile
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(void)
  TouchJobOutputFile (/*[in]*/ FILE * pFile)
    const;

  /* _______________________________________________________________________
     
     Variables
     _______________________________________________________________________ */

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_time;

private:
  MiKTeX::Core::tstring dumpName;

private:
  MiKTeX::Core::tstring jobName;

private:
  MiKTeX::Core::PathName editFileName;

private:
  MiKTeX::Core::PathName transcriptFileName;

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
  int editLineNum;

private:
  int interactionMode;

private:
  int m_buf_size;

private:
  int m_error_line;

private:
  int m_half_error_line;

private:
  int m_max_print_line;

private:
  int m_max_strings;

private:
  int m_mem_max;

private:
  int m_mem_min;

private:
  int m_mem_top;

private:
  int m_param_size;

private:
  int m_pool_size;

private:
  int m_stack_size;

private:
  int m_string_vacancies;
};

/* _________________________________________________________________________
     
   PrintFileName
   _________________________________________________________________________ */

#if defined(THEDATA)
inline
void
PrintFileNameHelper (/*[in]*/ const MIKTEXCHAR * lpszBuf)
{
  size_t l = MiKTeX::Core::StrLen(lpszBuf);
  for (size_t i = 0; i < l; ++ i)
    {
      MIKTEXCHAR ch = lpszBuf[i];
#if defined(MIKTEX_WINDOWS)
      if (ch == MiKTeX::Core::PathName::AltDirectoryDelimiter)
	{
	  ch = MiKTeX::Core::PathName::DirectoryDelimiter;
	}
#endif
      printchar (ch);
    }
}

inline
void
PrintFileName (/*[in]*/ strnumber	area,
	       /*[in]*/ strnumber	name,
	       /*[in]*/ strnumber	extension)
{
  bool mustQuote = false;
  MIKTEXCHAR szArea[MiKTeX::Core::BufferSizes::MaxPath];
  if (area != 0)
    {
      GetTeXString (szArea, area, MiKTeX::Core::BufferSizes::MaxPath);
      mustQuote = mustQuote || _tcschr(szArea, MIKTEXTEXT(' '));
    }
  MIKTEXCHAR szName[MiKTeX::Core::BufferSizes::MaxPath];
  if (name != 0)
    {
      GetTeXString (szName, name, MiKTeX::Core::BufferSizes::MaxPath);
      mustQuote = mustQuote || _tcschr(szName, MIKTEXTEXT(' '));
    }
  MIKTEXCHAR szExtension[MiKTeX::Core::BufferSizes::MaxPath];
  if (extension)
    {
      GetTeXString (szExtension, extension, MiKTeX::Core::BufferSizes::MaxPath);
      mustQuote = mustQuote || _tcschr(szExtension, MIKTEXTEXT(' '));
    }
  if (mustQuote)
    {
      printchar (MIKTEXTEXT('"'));
    }
  if (area != 0)
    {
      PrintFileNameHelper (szArea);
    }
  if (name != 0)
    {
      PrintFileNameHelper (szName);
    }
  if (extension != 0)
    {
      PrintFileNameHelper (szExtension);
    }
  if (mustQuote)
    {
      printchar (MIKTEXTEXT('"'));
    }
}

inline
void
miktexprintfilename (/*[in]*/ strnumber a,
		     /*[in]*/ strnumber n,
		     /*[in]*/ strnumber e)
{
  PrintFileName (a, n, e);
}
#endif // THEDATA

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// miktex/texmfapp.h
