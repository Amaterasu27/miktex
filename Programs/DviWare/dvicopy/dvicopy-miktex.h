/* dvicopy-miktex.h:						-*- C++ -*-

   Copyright (C) 1996-2005 Christian Schenk

   This file is part of DVIcopy.

   DVIcopy is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   DVIcopy is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with DVIcopy; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#if ! defined(_MSC_VER)
#  pragma once
#endif

#define THEDATA(x) DVICOPYDATA.m_##x

#include "dvicopy.rc"
#include "dvicopydefs.h"
#include <miktex/webapp.h>

#if ! defined(MIKTEXHELP_DVICOPY)
#  include <MiKTeX/Core/Help>
#endif

using namespace MiKTeX::TeXAndFriends;
using namespace MiKTeX::Core;

class DVICOPYCLASS

  : public WebApp

{
#define OPT_MAG 1000
#define OPT_SELECT 1001
#define OPT_MAX_PAGES 1002
#define OPT_PAGE_START 1003

public:
  virtual
  void
  MIKTEXMFCALL
  AddOptions ()
  {
    WebApp::AddOptions ();
    AddOption (MIKTEXTEXT("mag\0Override existing magnification with MAG."),
	       OPT_MAG,  required_argument, MIKTEXTEXT("MAG"));
    AddOption (MIKTEXTEXT("magnification"), MIKTEXTEXT("mag"));
    AddOption (MIKTEXTEXT("select\0\
Select a page range (start page and page count), \
for example `2 12' or `5.*.-2 4'. This option can be \
used up to 10 times.  This option cannot be used together \
with -max-pages or -page-start."),
	       OPT_SELECT,  required_argument, MIKTEXTEXT("SEL"));
    AddOption (MIKTEXTEXT("max-pages\0Process N pages; default one million. ")
	       MIKTEXTEXT("This option cannot be used together with -select."),
	       OPT_MAX_PAGES, required_argument, MIKTEXTEXT("N"));
    AddOption (MIKTEXTEXT("page-start\0Start at PAGE-SPEC, for example ")
	       MIKTEXTEXT("`2' or `5.*.-2'. This option cannot be used")
	       MIKTEXTEXT(" together with -select."),
	       OPT_PAGE_START, required_argument, MIKTEXTEXT("PAGE-SPEC"));
  }

public:
  void
  SetupOptions ()
  {
    if (maxPages.length() > 0 || pageStart.length() > 0)
      {
	if (selections.size() > 0)
	  {
	    FatalError (MIKTEXTEXT("\
-select was specified together with -max-pages and/or -page-start."));
	  }
	if (maxPages.length() == 0)
	  {
	    maxPages = MIKTEXTEXT("1000000");
	  }
	if (pageStart.length() == 0)
	  {
	    pageStart = MIKTEXTEXT("*");
	  }
	tstring str = pageStart;
	str += MIKTEXTEXT(" ");
	str += maxPages;
	selections.push_back (str);
      }
	
    if (selections.size() > maxselections)
      {
	FatalError (MIKTEXTEXT("\
Too mant page selections."));
      }
	
    for (std::vector<tstring>::const_iterator it = selections.begin();
	 it != selections.end();
	 ++ it)
      {
	if (it->length() + 8 >= sizeof(THEDATA(options)[0]))
	  {
	    FatalError (MIKTEXTEXT("\
Invalid page selection."));
	  }
#if defined(_MSC_VER) && _MSC_VER >= 1400
	_stprintf_s (THEDATA(options)[THEDATA(nopt)++],
		     (sizeof(THEDATA(options)[0])
		      / sizeof(THEDATA(options)[0][0])),
		     MIKTEXTEXT("select %s"),
		     it->c_str());
#else
	_stprintf (THEDATA(options)[THEDATA(nopt)++],
		   MIKTEXTEXT("select %s"),
		   it->c_str());
#endif
      }

    if (mag.length() > 0)
      {
	if (mag.length() + 5 >= sizeof(THEDATA(options)[0]))
	  {
	    FatalError (MIKTEXTEXT("\
Invalid magnification."));
	  }
#if defined(_MSC_VER) && _MSC_VER >= 1400
	_stprintf_s (THEDATA(options)[THEDATA(nopt)++],
		     (sizeof(THEDATA(options)[0])
		      / sizeof(THEDATA(options)[0][0])),
		     MIKTEXTEXT("mag %s"),
		     mag.c_str());
#else
	_stprintf (THEDATA(options)[THEDATA(nopt)++],
		   MIKTEXTEXT("mag %s"),
		   mag.c_str());
#endif
      }
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetUsage ()
    const
  {
    return (MIKTEXTEXT("[OPTION...] INDVI OUTDVI"));
  }

public:
  virtual
  bool
  MIKTEXMFCALL
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg)
  {
    bool done = true;
    switch (opt)
      {
      case OPT_SELECT:
	selections.push_back (lpszOptArg);
	break;
      case OPT_MAG:
	if (mag.length() > 0)
	  {
	    FatalError (MIKTEXTEXT("\
-mag was specified more than once."));
	  }
	mag = lpszOptArg;
	break;
      case OPT_MAX_PAGES:
	if (maxPages.length() > 0)
	  {
	    FatalError (MIKTEXTEXT("\
-max-pages was specified more than once."));
	  }
	maxPages = lpszOptArg;
	break;
      case OPT_PAGE_START:
	if (pageStart.length() > 0)
	  {
	    FatalError (MIKTEXTEXT("\
-page-start was specified more than once."));
	  }
	pageStart = lpszOptArg;
	break;
      default:
	done = WebApp::ProcessOption(opt, lpszOptArg);
	break;
      }
    return (done);
  }

private:
  std::vector<tstring> selections;

private:
  tstring mag;

private:
  tstring pageStart;

private:
  tstring maxPages;

public:
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_DVICOPY);
  }
};

extern DVICOPYCLASS DVICOPYAPP;
#define THEAPP DVICOPYAPP

inline
void
setupoptions ()
{
  THEAPP.SetupOptions ();
}

#include <miktex/webapp.inl>
