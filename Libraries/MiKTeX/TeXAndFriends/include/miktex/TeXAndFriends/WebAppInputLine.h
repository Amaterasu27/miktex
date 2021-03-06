/* miktex/TeXAndFriends/WebAppInputLine.h:			-*- C++ -*-

   Copyright (C) 1996-2011 Christian Schenk

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

#if ! defined(GUARD_68E0A68C189D54439CC37B0BE0B55F36_)
#define GUARD_68E0A68C189D54439CC37B0BE0B55F36_

#include <miktex/TeXAndFriends/WebApp>

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   WebAppInputLine
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
MIKTEXMFTYPEAPI(WebAppInputLine)

  : public WebApp

{
public:
  MIKTEXMFEXPORT
  MIKTEXTHISCALL
  WebAppInputLine ();

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
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(bool)
  ProcessOption (/*[in]*/ int		opt,
		 /*[in]*/ const char *	lpszOptArg);

  /* _______________________________________________________________________
     
     GetFormatIdent
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(int)
  GetFormatIdent ()
    const
  {
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     TouchJobOutputFile
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(void)
  TouchJobOutputFile (/*[in]*/ FILE *)
    const;

  /* _______________________________________________________________________
     
     BufferSizeExceeded
     _______________________________________________________________________ */

private:

#ifdef THEDATA
  void
  BufferSizeExceeded ()
  {
#if defined(MIKTEX_BIBTEX)
    fputs ("Buffer size exceeded!", stdout);
    c4pthrow(9998l);
#else
    if (GetFormatIdent() == 0)
      {
	fputs ("Buffer size exceeded!", THEDATA(termout));
	throw (new C4P::Exception9999l);
      }
    else
      {
	THEDATA(curinput).locfield = THEDATA(first);
	THEDATA(curinput).limitfield = THEDATA(last) - 1;
#if defined(bufsize)
	overflow (256, bufsize);
#else
	overflow (256, THEDATA(bufsize));
#endif
      }
#endif // ifndef MIKTEX_BIBTEX
  }
#endif // ifdef THEDATA
  
  /* _______________________________________________________________________
     
     CloseFile
     _______________________________________________________________________ */

public:

  template<class FileType>
  void
  CloseFile (/*[in]*/ FileType & f)
  {
    f.AssertValid ();
    TouchJobOutputFile (f);
    pSession->CloseFile (f);
  }

  /* _______________________________________________________________________
     
     GetCharacter
     _______________________________________________________________________ */

private:

  int
  GetCharacter (/*[in]*/ FILE * pFile)
    const
  {
    MIKTEX_ASSERT (pFile != 0);
    int ch = getc(pFile);
    if (ch == EOF)
      {
	if (ferror(pFile) != 0)
	  {
	    MiKTeX::Core::Session::FatalCrtError ("getc",
						  0,
						  __FILE__,
						  __LINE__);
	  }
      }
#if 0
    const int e_o_f = 0x1a; // ^Z
    if (ch == e_o_f)
      {
	ch = EOF;		// -1
	HandleEof (pFile);
      }
#endif
    return (ch);
  }

  /* _______________________________________________________________________
     
     GetFQNameOfFile
     _______________________________________________________________________ */

public:

  const char *
  GetFQNameOfFile ()
    const
  {
    return (fqNameOfFile.Get());
  }

  /* _______________________________________________________________________
     
     GetNameOfFile
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  MiKTeX::Core::PathName
  GetNameOfFile ()
    const
  {
#if defined(MIKTEX_XETEX)
    MIKTEX_ASSERT (sizeof(THEDATA(nameoffile)[0]) == sizeof(char));
    const char * lpsz = reinterpret_cast<const char *>(THEDATA(nameoffile));
    return (MiKTeX::Core::Utils::UTF8ToWideChar(lpsz));
#else
    return (THEDATA(nameoffile));
#endif
  }
#endif

  /* _______________________________________________________________________
     
     SetNameOfFile
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  SetNameOfFile (/*[in]*/ const MiKTeX::Core::PathName & fileName)
  {
#if defined(MIKTEX_XETEX)
    MiKTeX::Core::Utils::CopyString (
      reinterpret_cast<char *>(THEDATA(nameoffile)), MiKTeX::Core::BufferSizes::MaxPath + 1, fileName.Get());
    THEDATA(namelength) = fileName.GetLength();
#else
    MiKTeX::Core::Utils::CopyString (
      THEDATA(nameoffile), MiKTeX::Core::BufferSizes::MaxPath + 1, fileName.Get());
    THEDATA(namelength) = fileName.GetLength();
#endif
  }
#endif

  /* _______________________________________________________________________
     
     GetOutputDirectory
     _______________________________________________________________________ */

public:

  MiKTeX::Core::PathName
  GetOutputDirectory ()
    const
  {
    return (outputDirectory);
  }

  /* _______________________________________________________________________
     
     HandleEof
     _______________________________________________________________________ */

private:

#if 0  
  MIKTEXMFTHISAPI(void)
  HandleEof (/*[in]*/ FILE * pFile)
    const;
#endif

  /* _______________________________________________________________________
     
     InputLine
     _______________________________________________________________________ */

public:

#if defined(THEDATA) && ! defined(MIKTEX_XETEX)
  bool
  InputLine (/*[in]*/ C4P::C4P_text &	f,
	     /*[in]*/ C4P::C4P_boolean	bypassEndOfLine)
  {
    f.AssertValid ();

#if defined(PASCAL_TEXT_IO)
    not_implemented ();
    if (bypassEndOfLine && feof(f) == 0)
      {
	MIKTEX_ASSERT ((*f)() == '\n');
	c4pgetc (f);
      }
#endif

#if defined(MIKTEX_BIBTEX)
    const unsigned long first = 0;
#else
    const unsigned long first = THEDATA(first);
#ifndef bufsize
    const unsigned long bufsize = THEDATA(bufsize);
#endif
#endif
    
    THEDATA(last) = first;
    
    if (feof(f) != 0)
      {
	return (false);
      }
    
    int ch;

#if defined(PASCAL_TEXT_IO)
    not_implemented ();
    ch = (*f)();
#else
    ch = GetCharacter(f);
    if (ch == EOF)
      {
	return (false);
      }
    if (ch == '\r')
      {
	ch = GetCharacter(f);
	if (ch == EOF)
	  {
	    return (false);
	  }
	if (ch != '\n')
	  {
	    ungetc (ch, f);
	    ch = '\n';
	  }
      }
#endif // not Pascal Text I/O
    
    if (ch == '\n')
      {
	return (true);
      }

#if defined(MIKTEX_OMEGA)
    THEDATA(buffer)[ THEDATA(last)++ ] = ch;
#else
    THEDATA(buffer)[ THEDATA(last)++ ] = THEDATA(xord)[ch & 0xff];
#endif
	
    while ((ch = GetCharacter(f)) != EOF && THEDATA(last) < bufsize)
      {
	if (ch == '\r')
	  {
	    ch = GetCharacter(f);
	    if (ch == EOF)
	      {
		break;
	      }
	    if (ch != '\n')
	      {
		ungetc(ch, f);
		ch = '\n';
	      }
	  }
	if (ch == '\n')
	  {
	    break;
	  }
#if defined(MIKTEX_OMEGA)
	THEDATA(buffer)[ THEDATA(last)++ ] = ch;
#else
	THEDATA(buffer)[ THEDATA(last)++ ] = THEDATA(xord)[ch & 0xff];
#endif
      }
    
    if (ch != '\n' && ch != EOF)
      {
	BufferSizeExceeded ();
      }
    
#if ! defined(MIKTEX_BIBTEX)
    if (THEDATA(last) >= THEDATA(maxbufstack))
      {
	THEDATA(maxbufstack) = THEDATA(last) + 1;
	if (THEDATA(maxbufstack) >= bufsize)
	  {
	    BufferSizeExceeded ();
	  }
      }
#endif // ifndef MIKTEX_BIBTEX
    
    while (THEDATA(last) > first
	   && (THEDATA(buffer)[THEDATA(last) - 1] == ' '
	       || THEDATA(buffer)[THEDATA(last) - 1] == '\t'
	       || THEDATA(buffer)[THEDATA(last) - 1] == '\r'))
      {
	THEDATA(last)--;
      }
    
#if defined(PASCAL_TEXT_IO)
    not_implemented ();
    f() = '\n';
#endif
    
    return (true);
  }

#endif // THEDATA && ! MIKTEX_XETEX

  /* _______________________________________________________________________
     
     OpenInputFile
     _______________________________________________________________________ */

public:

  MIKTEXMFTHISAPI(bool)
  OpenInputFile (/*[out]*/ FILE * *		ppFile,
		 /*[in]*/ const char *		lpszFileName);

  /* _______________________________________________________________________
     
     OpenInputFile
     _______________________________________________________________________ */

public:

  MIKTEXMFTHISAPI(bool)
  OpenInputFile (/*[out]*/ C4P::FileRoot &	f,
		 /*[in]*/ const char *		lpszFileName);

  /* _______________________________________________________________________
     
     OpenOutputFile
     _______________________________________________________________________ */

public:

  MIKTEXMFTHISAPI(bool)
  OpenOutputFile (/*[in]*/ C4P::FileRoot &		f,
		  /*[in]*/ const char *			lpszPath,
		  /*[in]*/ MiKTeX::Core::FileShare	share,
		  /*[in]*/ bool				text,
		  /*[out]*/ MiKTeX::Core::PathName &	outPath);

  /* _______________________________________________________________________
     
     AllowFileName
     _______________________________________________________________________ */

public:

  MIKTEXMFTHISAPI(bool)
  AllowFileName (/*[in]*/ const char *			lpszPath,
		 /*[in]*/ bool				forInput);

  /* _______________________________________________________________________
     
     EnablePipes
     _______________________________________________________________________ */

protected:

  void
  EnablePipes (/*[in]*/ bool f)
  {
    enablePipes = f;
  }

  /* _______________________________________________________________________
     
     MangleNameOfFile
     _______________________________________________________________________ */

public:

  static
  MIKTEXMFCEEAPI(MiKTeX::Core::PathName)
  MangleNameOfFile (/*[in]*/ const char *	lpszFileName);

  /* _______________________________________________________________________
     
     UnmangleNameOfFile
     _______________________________________________________________________ */

public:

  static
  MIKTEXMFCEEAPI(MiKTeX::Core::PathName)
  UnmangleNameOfFile (/*[in]*/ const char *	lpszFileName);

  /* _______________________________________________________________________
     
     UnmangleNameOfFile
     _______________________________________________________________________ */

public:

  static
  MIKTEXMFCEEAPI(MiKTeX::Core::PathName)
  UnmangleNameOfFile (/*[in]*/ const wchar_t *	lpszFileName);

  /* _______________________________________________________________________
     
     Variables
     _______________________________________________________________________ */

protected:
  MiKTeX::Core::PathName auxDirectory;
  
private:
  MiKTeX::Core::PathName fqNameOfFile;

protected:
  MiKTeX::Core::PathName lastInputFileName;
  
protected:
  MiKTeX::Core::PathName outputDirectory;

protected:
  MiKTeX::Core::FileType inputFileType;

protected:
  bool enablePipes;

private:
  int optBase;
};

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif
