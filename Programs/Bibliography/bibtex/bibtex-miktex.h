/* bibtex-miktex.h:						-*- C++ -*-

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

#include <miktex/TeXAndFriends/config.h>

#if ! defined(THEDATA)
#  define THEDATA(x) C4P_VAR(x)
#endif

#define IMPLEMENT_TCX 1

#include "bibtex.rc"

#include "bibtexdefs.h"

#include <miktex/Core/Registry>
#include <miktex/TeXAndFriends/WebAppInputLine>

using namespace MiKTeX::TeXAndFriends;
using namespace MiKTeX::Core;

#if ! defined(MIKTEXHELP_BIBTEX)
#  include <MiKTeX/Core/Help>
#endif

class BIBTEXCLASS

  : public WebAppInputLine

{
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
  
private:
  template<typename T>
  T *
  Allocate (/*[in,out]*/ T * &	p,
	    /*[in]*/ size_t	n)
  {
    p = 0;
    return (Reallocate(p, n));
  }

private:
  template<typename T>
  T *
  Free (/*[in,out]*/ T * &	p)
  {
    return (Reallocate(p, 0));
  }
  
public:
  virtual
  void
  MIKTEXTHISCALL
  Init (/*[in]*/ const char * lpszProgramInvocationName)
  {
    WebAppInputLine::Init (lpszProgramInvocationName);
#if defined(IMPLEMENT_TCX)
    EnableFeature (Feature::TCX);
#endif
    THEDATA(mincrossrefs) =
      pSession->GetConfigValue(MIKTEX_REGKEY_BIBTEX,
			       "min_crossrefs",
			       2);
    THEDATA(maxbibfiles) = 20;
    THEDATA(maxentints) = 3000;
    THEDATA(maxentstrs) = 3000;
    THEDATA(maxfields) = 100000; //5000;
    THEDATA(poolsize) = 65000;
    THEDATA(wizfnspace) = 3000;
    Allocate (THEDATA(bibfile), THEDATA(maxbibfiles));
    Allocate (THEDATA(biblist), THEDATA(maxbibfiles));
    Allocate (THEDATA(entryints), THEDATA(maxentints));
    Allocate (THEDATA(entrystrs), THEDATA(maxentstrs));
    Allocate (THEDATA(fieldinfo), THEDATA(maxfields));
    Allocate (THEDATA(spreamble), THEDATA(maxbibfiles));
    Allocate (THEDATA(strpool), THEDATA(poolsize));
    Allocate (THEDATA(wizfunctions), THEDATA(wizfnspace));
  }
  
public:
  virtual
  void
  MIKTEXTHISCALL
  Finalize ()
  {
    Free (THEDATA(bibfile));
    Free (THEDATA(biblist));
    Free (THEDATA(entryints));
    Free (THEDATA(entrystrs));
    Free (THEDATA(fieldinfo));
    Free (THEDATA(spreamble));
    Free (THEDATA(strpool));
    Free (THEDATA(wizfunctions));
    WebAppInputLine::Finalize ();
  }

#define OPT_MIN_CROSSREFS 1000
#define OPT_QUIET 1001

public:
  virtual
  void
  MIKTEXTHISCALL
  AddOptions ()
  {
    WebAppInputLine::AddOptions ();
    AddOption (MIKTEXTEXT("min-crossrefs\0\
Include item after N cross-refs; default 2."),
	       OPT_MIN_CROSSREFS,  POPT_ARG_STRING, "N");
    AddOption (MIKTEXTEXT("quiet\0Suppress all output (except errors)."),
	       OPT_QUIET, POPT_ARG_NONE);
    AddOption ("silent", "quiet");
    AddOption ("terse", "quiet");
  }
  
public:
  virtual
  MiKTeX::Core::FileType
  MIKTEXTHISCALL
  GetInputFileType ()
    const
  {
    return (MiKTeX::Core::FileType::BIB);
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetUsage ()
    const
  {
    return (MIKTEXTEXT("[OPTION...] AUXFILE"));
  }

public:
  virtual
  bool
  MIKTEXTHISCALL
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const char *	lpszOptArg)
  {
    bool done = true;
    switch (opt)
      {
      case OPT_MIN_CROSSREFS:
	THEDATA(mincrossrefs) = atoi(lpszOptArg);
	break;
      case OPT_QUIET:
	SetQuietFlag (true);
	break;
      default:
	done = WebAppInputLine::ProcessOption(opt, lpszOptArg);
	break;
      }
    return (done);
  }
  
public:
  const char *
  MIKTEXTHISCALL
  TheNameOfTheGame ()
    const
  {
    return ("BibTeX");
  }

public:
  unsigned long
  MIKTEXTHISCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_BIBTEX);
  }

public:
  template<class T>
  bool
  OpenBstFile (/*[in]*/ T & f)
    const
  {
    const char * lpszFileName = THEDATA(nameoffile);
    MIKTEX_ASSERT_STRING (lpszFileName);
    PathName bstFileName (lpszFileName);
    if (bstFileName.GetExtension() == 0)
      {
	bstFileName.SetExtension (MIKTEXTEXT(".bst"));
      }
    PathName path;
    if (! pSession->FindFile(bstFileName.Get(), FileType::BST, path))
      {
	return (false);
      }
    FILE * pfile =
      pSession->OpenFile(path.Get(), FileMode::Open, FileAccess::Read, true);
    f.Attach (pfile, true);
#ifdef PASCAL_TEXT_IO
    get (f);
#endif
    return (true);
  }
};

extern BIBTEXCLASS BIBTEXAPP;
#define THEAPP BIBTEXAPP

template<class T>
inline
void
miktexbibtexrealloc (/*[in]*/ const char *	lpszVar,
		     /*[in]*/ T * &			p,
		     /*[in]*/ size_t			n)
{
  p = THEAPP.Reallocate (p, n + 1);
}

template<class T>
inline
bool
miktexopenbstfile (/*[in]*/ T & f)
{
  return (THEAPP.OpenBstFile(f));
}

inline
bool
miktexhasextension (/*[in]*/ const char * lpszFileName,
		    /*[in]*/ const char * lpszExtension)
{
  return (PathName(lpszFileName).HasExtension(lpszExtension));
}

#include <miktex/TeXAndFriends/WebAppInputLine.inl>
