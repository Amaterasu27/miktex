/* gftodvi-miktex.h:					-*- C++ -*-

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

#define THEDATA(x) GFTODVIDATA.m_##x

#include "gftodvi.rc"
#include <miktex/webapp.h>

#define OPT_OVERFLOW_LABEL_OFFSET 1000
#define OPT_TITLEFONT 1001
#define OPT_LABELFONT 1002
#define OPT_GRAYFONT 1003
#define OPT_SLANTFONT 1004
#define OPT_LOGOFONT 1005

#if ! defined(MIKTEXHELP_GFTODVI)
#  include <miktex/help.h>
#endif

using namespace MiKTeX::TeXAndFriends;
using namespace MiKTeX::Core;

class GFTODVICLASS

  : public WebApp

{
public:
  virtual
  void
  MIKTEXMFCALL
  AddOptions ()
  {
    AddOption (MIKTEXTEXT("overflow-label-offset\0\
Specifies the distance from the right edge of the character bounding \
box at which the overflow equations (if any) are typeset. \
The value is given in TeX points."),
	       OPT_OVERFLOW_LABEL_OFFSET,
	       required_argument,
	       MIKTEXTEXT("REAL"));
    AddOption (MIKTEXTEXT("title-font\0Font for titles."),
	       OPT_TITLEFONT,
	       required_argument,
	       MIKTEXTEXT("FONT"));
    AddOption (MIKTEXTEXT("label-font\0Font for labels."),
	       OPT_LABELFONT,
	       required_argument,
	       MIKTEXTEXT("FONT"));
    AddOption (MIKTEXTEXT("gray-font\0Font for pixels."),
	       OPT_GRAYFONT,
	       required_argument,
	       MIKTEXTEXT("FONT"));
    AddOption (MIKTEXTEXT("slant-font\0Font for slants."),
	       OPT_SLANTFONT,
	       required_argument,
	       MIKTEXTEXT("FONT"));
    AddOption (MIKTEXTEXT("logo-font\0Font for METAFONT logo."),
	       OPT_LOGOFONT,
	       required_argument,
	       MIKTEXTEXT("FONT"));
    WebApp::AddOptions ();
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetUsage ()
    const
  {
    return (MIKTEXTEXT("[OPTION...] [INPUTFILE]"));
  }

public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
  {
    WebApp::Init (lpszProgramInvocationName);
    THEDATA(overlabeloffset) = 10000000;
    titleFont = MIKTEXTEXT("cmr8");
    labelFont = MIKTEXTEXT("cmtt10");
    grayFont = MIKTEXTEXT("gray");
    slantFont = MIKTEXTEXT("");
    logoFont = MIKTEXTEXT("logo8");
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
      case OPT_OVERFLOW_LABEL_OFFSET:
	THEDATA(overlabeloffset) = C4P::Round(_tstof(lpszOptArg) * 65536.0);
	break;
      case OPT_TITLEFONT:
	titleFont = lpszOptArg;
	break;
      case OPT_LABELFONT:
	labelFont = lpszOptArg;
	break;
      case OPT_GRAYFONT:
	grayFont = lpszOptArg;
	break;
      case OPT_SLANTFONT:
	slantFont = lpszOptArg;
	break;
      case OPT_LOGOFONT:
	logoFont = lpszOptArg;
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
    int argc = C4P::GetArgC();
    const MIKTEXCHAR ** argv = C4P::GetArgV();
    if (argc > 1)
      {
	inputFile = argv[1];
      }
  }

public:
  bool
  GetInputFileName ()
  {
    if (inputFile.length() == 0
	|| inputFile.length() > terminallinelength)
      {
	return (false);
      }
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    strcpy_s (reinterpret_cast<char*>(THEDATA(buffer)),
	      terminallinelength,
	      inputFile.c_str());
#else
    strcpy (reinterpret_cast<char*>(THEDATA(buffer)),
	    inputFile.c_str());
#endif
    THEDATA(linelength) =
      static_cast<C4P_unsigned8>(inputFile.length());
    inputFile = "";
    return (true);
  }

private:
  strnumber
  MakeString (/*[in]*/ const char * lpsz)
  {
    size_t l = strlen(lpsz);
    if (l == 0)
      {
	return (0);
      }
    memcpy (THEDATA(strpool) + THEDATA(poolptr), lpsz, l);
    THEDATA(poolptr) += static_cast<poolpointer>(l);
    THEDATA(strptr) += 1;
    THEDATA(strstart)[THEDATA(strptr)] = THEDATA(poolptr);
    return (THEDATA(strptr) - 1);
  }

public:
  strnumber
  GetTitleFont ()
  {
    return (MakeString(titleFont.c_str()));
  }

public:
  strnumber
  GetLabelFont ()
  {
    return (MakeString(labelFont.c_str()));
  }

public:
  strnumber
  GetGrayFont ()
  {
    return (MakeString(grayFont.c_str()));
  }

public:
  strnumber
  GetSlantFont ()
  {
    return (MakeString(slantFont.c_str()));
  }

public:
  strnumber
  GetLogoFont ()
  {
    return (MakeString(logoFont.c_str()));
  }

private:
  std::string inputFile;

private:
  std::string titleFont;

private:
  std::string labelFont;

private:
  std::string grayFont;

private:
  std::string slantFont;

private:
  std::string logoFont;

public:
  virtual
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_GFTODVI);
  }
};

extern GFTODVICLASS GFTODVIAPP;
#define THEAPP GFTODVIAPP

inline
bool
miktexgetinputfilename ()
{
  return (THEAPP.GetInputFileName());
}

inline
strnumber
miktexgettitlefont ()
{
  return (THEAPP.GetTitleFont());
}

inline
strnumber
miktexgetlabelfont ()
{
  return (THEAPP.GetLabelFont());
}

inline
strnumber
miktexgetgrayfont ()
{
  return (THEAPP.GetGrayFont());
}

inline
strnumber
miktexgetslantfont ()
{
  return (THEAPP.GetSlantFont());
}

inline
strnumber
miktexgetlogofont ()
{
  return (THEAPP.GetLogoFont());
}

#include <miktex/webapp.inl>
