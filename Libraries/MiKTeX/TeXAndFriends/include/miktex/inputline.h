/* miktex/inputline.h: inputln() implementation for TeX & Friends -*- C++ -*-

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

#if ! defined(GUARD_68E0A68C189D54439CC37B0BE0B55F36_)
#define GUARD_68E0A68C189D54439CC37B0BE0B55F36_

#include <miktex/webapp.h>

#include <cassert>

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
WebAppInputLine
  : public WebApp
{
public:
  MIKTEXMFEXPORT
  MIKTEXMFCALL
  WebAppInputLine ();

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
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(bool)
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg);

  /* _______________________________________________________________________
     
     GetFormatIdent
     _______________________________________________________________________ */

public:

  virtual
  int
  MIKTEXMFCALL
  GetFormatIdent ()
    const
  {
    // must be implemented in sub-classes
    assert (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     TouchJobOutputFile
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(void)
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
    _fputts (MIKTEXTEXT("Buffer size exceeded!"), stdout);
    c4pthrow(9998l);
#else
    if (GetFormatIdent() == 0)
      {
	_fputts (MIKTEXTEXT("Buffer size exceeded!"), THEDATA(termout));
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

  template<class T>
  void
  CloseFile (/*[in]*/ T & f)
  {
    f.AssertValid ();
    TouchJobOutputFile (f);
    pSession->CloseFile (f);
  }

  /* _______________________________________________________________________
     
     GetCharacter
     _______________________________________________________________________ */

private:

  MIKTEXCHARINT
  GetCharacter (/*[in]*/ FILE * pFile)
    const
  {
    assert (pFile != 0);
    MIKTEXCHARINT ch = _gettc(pFile);
    if (ch == MIKTEXEOF)
      {
	if (ferror(pFile) != 0)
	  {
	    MiKTeX::Core::Session::FatalCrtError (MIKTEXTEXT("getc"),
						  0,
						  MIKTEXTEXT(__FILE__),
						  __LINE__);
	  }
      }
#if 0
    const MIKTEXCHARINT e_o_f = 0x1a; // ^Z
    if (ch == e_o_f)
      {
	ch = MIKTEXEOF;		// -1
	HandleEof (pFile);
      }
#endif
    return (ch);
  }

  /* _______________________________________________________________________
     
     GetFQNameOfFile
     _______________________________________________________________________ */

protected:

  const MIKTEXCHAR *
  GetFQNameOfFile ()
    const
  {
    return (fqNameOfFile.Get());
  }

  /* _______________________________________________________________________
     
     HandleEof
     _______________________________________________________________________ */

private:

#if 0  
  MIKTEXMFAPI(void)
  HandleEof (/*[in]*/ FILE * pFile)
    const;
#endif

  /* _______________________________________________________________________
     
     InputLine
     _______________________________________________________________________ */

public:

#ifdef THEDATA
  bool
  InputLine (/*[in]*/ C4P::C4P_text &	f,
	     /*[in]*/ C4P::C4P_boolean	bypassEndOfLine)
  {
    f.AssertValid ();

#if defined(PASCAL_TEXT_IO)
    if (bypassEndOfLine && feof(f) == 0)
      {
	assert ((*f)() == MIKTEXTEXT('\n'));
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
#endif // ifndef MIKTEX_BIBTEX
    
    THEDATA(last) = first;
    
    if (feof(f) != 0)
      {
	return (false);
      }
    
    int ch;

#if defined(PASCAL_TEXT_IO)
    ch = (*f)();
#else
    ch = GetCharacter(f);
    if (ch == EOF)
      {
	return (false);
      }
    if (ch == MIKTEXTEXT('\r'))
      {
	ch = GetCharacter(f);
	if (ch == EOF)
	  {
	    return (false);
	  }
	if (ch != MIKTEXTEXT('\n'))
	  {
	    ungetc (ch, f);
	    ch = MIKTEXTEXT('\n');
	  }
      }
#endif // not Pascal Text I/O
    
    if (ch == MIKTEXTEXT('\n'))
      {
	return (true);
      }

#if defined(MIKTEX_OMEGA) || defined(MIKTEX_EOMEGA)
    THEDATA(buffer)[ THEDATA(last)++ ] = ch;
#else
    THEDATA(buffer)[ THEDATA(last)++ ] = THEDATA(xord)[ch & 0xff];
#endif
	
    while ((ch = GetCharacter(f)) != EOF && THEDATA(last) < bufsize)
      {
	if (ch == MIKTEXTEXT('\r'))
	  {
	    ch = GetCharacter(f);
	    if (ch == EOF)
	      {
		break;
	      }
	    if (ch != MIKTEXTEXT('\n'))
	      {
		ungetc(ch, f);
		ch = MIKTEXTEXT('\n');
	      }
	  }
	if (ch == MIKTEXTEXT('\n'))
	  {
	    break;
	  }
#if defined(MIKTEX_OMEGA) || defined(MIKTEX_EOMEGA)
	THEDATA(buffer)[ THEDATA(last)++ ] = ch;
#else
	THEDATA(buffer)[ THEDATA(last)++ ] = THEDATA(xord)[ch & 0xff];
#endif
      }
    
    if (ch != MIKTEXTEXT('\n') && ch != EOF)
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
	   && (THEDATA(buffer)[THEDATA(last) - 1] == MIKTEXTEXT(' ')
	       || THEDATA(buffer)[THEDATA(last) - 1] == MIKTEXTEXT('\t')
	       || THEDATA(buffer)[THEDATA(last) - 1] == MIKTEXTEXT('\r')))
      {
	THEDATA(last)--;
      }
    
#if defined(PASCAL_TEXT_IO)
    f() = MIKTEXTEXT('\n');
#endif
    
    return (true);
  }

#endif // ifdef THEDATA

  /* _______________________________________________________________________
     
     OpenInputFile
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(bool)
  OpenInputFile (/*[in]*/ C4P::C4P_text & f,
		 /*[in]*/ const MIKTEXCHAR * lpszFileName);

  /* _______________________________________________________________________
     
     OpenOutputFile
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(bool)
  OpenOutputFile (/*[in]*/ C4P::FileRoot &	f,
		  /*[in]*/ const MIKTEXCHAR *	lpszPath,
		  /*[in]*/ MiKTeX::Core::FileShare	share,
		  /*[in]*/ bool			text);

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
  MIKTEXMFAPI(MiKTeX::Core::PathName)
  MangleNameOfFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName);

  /* _______________________________________________________________________
     
     UnmangleNameOfFile
     _______________________________________________________________________ */

public:

  static
  MIKTEXMFAPI(MiKTeX::Core::PathName)
  UnmangleNameOfFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName);

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

#endif	// miktex/inputline.h
