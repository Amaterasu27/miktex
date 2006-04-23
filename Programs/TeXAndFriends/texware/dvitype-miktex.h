/* dvitype-miktex.h:						-*- C++ -*-

   Copyright (C) 1991-2005 Christian Schenk

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

#define THEDATA(x) DVITYPEDATA.m_##x

#include "dvitype.rc"
#include <miktex/webapp.h>

#define OPT_OUT_MODE 1000
#define OPT_START_THERE 1001
#define OPT_MAX_PAGES 1002
#define OPT_RESOLUTION 1003
#define OPT_NEW_MAG 1004

using namespace MiKTeX::TeXAndFriends;
using namespace MiKTeX::Core;

class DVITYPECLASS

  : public WebApp

{
public:
  virtual
  void
  MIKTEXMFCALL
  AddOptions ()
  {
    AddOption (MIKTEXTEXT("out-mode\0Set output mode."),
	       OPT_OUT_MODE, required_argument,
	       MIKTEXTEXT("MODE"));
    AddOption (MIKTEXTEXT("start-there\0Set starting page."),
	       OPT_START_THERE, required_argument,
	       MIKTEXTEXT("PAGESPEC"));
    AddOption (MIKTEXTEXT("max-pages\0Set maximum number of pages."),
	       OPT_MAX_PAGES, required_argument,
	       MIKTEXTEXT("N"));
    AddOption (MIKTEXTEXT("resolution\0Set desired resolution."),
	       OPT_RESOLUTION, required_argument,
	       MIKTEXTEXT("NUM/DEN"));
    AddOption (MIKTEXTEXT("new-mag\0Set new magnification."),
	       OPT_NEW_MAG,
	       required_argument,
	       MIKTEXTEXT("MAG"));
    WebApp::AddOptions ();
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetUsage ()
    const
  {
    return (MIKTEXTEXT("[OPTION...] INPUTFILE"));
  }

public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
  {
    WebApp::Init (lpszProgramInvocationName);
    THEDATA(outmode) = 4;
    THEDATA(startvals) = 0;
    THEDATA(startthere)[0] = false;
    THEDATA(maxpages) = 1000000;
    THEDATA(resolution) = 300.0;
    THEDATA(newmag) = 0;
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
      case OPT_OUT_MODE:
	if (! (lpszOptArg[1] == 0
	       && lpszOptArg[0] >= MIKTEXTEXT('0')
	       && lpszOptArg[1] <= MIKTEXTEXT('4')))
	  {
	    BadUsage ();
	  }
	THEDATA(outmode) = _ttoi(lpszOptArg);
	break;
      case OPT_START_THERE:
	{
	  const MIKTEXCHAR * lpsz = lpszOptArg;
	  size_t k = 0;
	  do 
	    {
	      if (*lpsz == MIKTEXTEXT('*'))
		{
		  THEDATA(startthere)[k] = false;
		  ++ lpsz;
		}
	      else if (! (_istdigit(*lpsz)
			  || (*lpsz == MIKTEXTEXT('-') && _istdigit(lpsz[1]))))
		{
		  BadUsage ();
		}
	      else
		{
		  THEDATA(startthere)[k] = true;
		  LPTSTR lpsz2 = 0;
		  THEDATA(startcount)[k] = _tcstol(lpsz, &lpsz2, 10);
		  lpsz = const_cast<const MIKTEXCHAR *>(lpsz2);
		}
	      if (k < 9 && *lpsz == '.')
		{
		  ++ k;
		  ++ lpsz;
		}
	      else if (*lpsz == 0)
		{
		  THEDATA(startvals) = static_cast<C4P_signed8>(k);
		}
	      else
		{
		  BadUsage ();
		}
	    }
	  while (THEDATA(startvals) != static_cast<C4P_signed8>(k));
	}
	break;
      case OPT_MAX_PAGES:
	if (! isdigit(lpszOptArg[0]))
	  {
	    BadUsage ();
	  }
	THEDATA(maxpages) = atoi(lpszOptArg);
	break;
      case OPT_RESOLUTION:
	{
	  int num, den;
#if _MSC_VER < 1400
#define _stscanf_s _stscanf
#endif
	  if (_stscanf_s(lpszOptArg, MIKTEXTEXT("%d/%d"), &num, &den) != 2
	      || (num < 0))
	    {
	      BadUsage ();
	    }
#if _MSC_VER < 1400
#undef _stscanf_s
#endif
	  THEDATA(resolution) = static_cast<float>(num) / den;
	}
	break;
      case OPT_NEW_MAG:
	if (! isdigit(*lpszOptArg))
	  {
	    BadUsage ();
	  }
	THEDATA(newmag) = _ttoi(lpszOptArg);
	break;
      default:
	done = WebApp::ProcessOption(opt, lpszOptArg);
	break;
      }
    return (done);
  }

public:
  virtual
  void
  MIKTEXMFCALL
  ProcessCommandLineOptions ()
  {
    WebApp::ProcessCommandLineOptions ();
    if (C4P::GetArgC() != 2)
      {
	BadUsage ();
      }
  }
};

extern DVITYPECLASS DVITYPEAPP;
#define THEAPP DVITYPEAPP

#include <miktex/webapp.inl>
