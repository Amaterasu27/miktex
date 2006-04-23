/* mp-miktex.h:							-*- C++ -*-

   Copyright (C) 1998-2005 Christian Schenk

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

#define THEDATA(x) METAPOSTDATA.m_##x

#if defined(TRAPMP)
#  include "trapmpdefs.h"
#else
#  include "mpdefs.h"
#endif

#include "mp.rc"
#include <miktex/mfapp.h>

#if ! defined(MIKTEXHELP_MPOST)
#  include <miktex/help.h>
#endif

using namespace MiKTeX::Core;
using namespace MiKTeX::TeXAndFriends;
using namespace std;

class METAPOSTCLASS

  : public MetafontApp

{
public:
  METAPOSTCLASS ()
  {
    inputFileType = FileType::MP;
  }

public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
  {
    MetafontApp::Init (lpszProgramInvocationName);
    SetProgramInfo (MIKTEXTEXT("mp;mpost"), 0, 0, 0);
#if defined(IMPLEMENT_TCX)
    EnableFeature (Feature::TCX);
#endif
    m_font_mem_size = -1;
  }

  enum {
    OPT_FONT_MEM_SIZE = 10000,
    OPT_TEX_PROGRAM,
  };

public:
  virtual
  void
  MIKTEXMFCALL
  AddOptions ()
  {
    MetafontApp::AddOptions ();

    AddOption (MIKTEXTEXT("font-mem-size\0Set font_mem_size to N."),
	       OPT_FONT_MEM_SIZE,
	       required_argument,
	       MIKTEXTEXT("N"));

    AddOption (MIKTEXTEXT("tex\0Use TEXPROGRAM for text labels."),
	       OPT_TEX_PROGRAM,
	       required_argument,
	       MIKTEXTEXT("TEXPROGRAM"));

    AddOption (MIKTEXTEXT("mem"), MIKTEXTEXT("undump"));

    AddOption (MIKTEXTEXT("troff"), OPT_UNSUPPORTED, 0, 0, 0, MIKTEXTEXT('T'));
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
      case OPT_FONT_MEM_SIZE:
	m_font_mem_size = atoi(lpszOptArg);
	break;
      case OPT_TEX_PROGRAM:
	texProgram = lpszOptArg;
	break;
      default:
	done = MetafontApp::ProcessOption(opt, lpszOptArg);
	break;
      }
    return (done);
  }

public:
  void
  FreeMemory ()
  {
    MetafontApp::FreeMemory ();
    Free (THEDATA(fontinfo));
  }

public:
  void
  AllocateMemory ()
  {
    MetafontApp::AllocateMemory ();

    GETPARAM (m_font_mem_size, fontmemsize, font_mem_size, 200000);

    Allocate (THEDATA(fontinfo), THEDATA(fontmemsize) + 1);
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetDumpExtension ()
    const
  {
    return (MIKTEXTEXT(".mem"));
  }

public:
  virtual
  FileType
  MIKTEXMFCALL
  GetDumpFileType ()
    const
  {
    return (FileType::MEM);
  }

public:
  virtual
  FileType
  MIKTEXMFCALL
  GetInputFileType ()
    const
  {
    return (FileType::MP);
  }

public:
  virtual
  FileType
  MIKTEXMFCALL
  GetPoolFileType ()
    const
  {
    return (FileType::MPPOOL);
  }

public:
  virtual
  int
  MIKTEXMFCALL
  GetFormatIdent ()
    const
  {
    return (THEDATA(memident));
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetDumpFileName ()
    const
  {
    return (MIKTEXTEXT("mpost.mem"));
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    return (MIKTEXTEXT("mp.pool"));
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetInitProgramName ()
    const
  {
    return (MIKTEXTEXT("inimp;inimpost"));
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetVirginProgramName ()
    const
  {
    return (MIKTEXTEXT("virmp;virmpost"));
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  TheNameOfTheGame ()
    const
  {
    return (MIKTEXTEXT("MetaPost"));
  }

public:
  const
  tstring &
  GetTeXProgram ()
    const
  {
    return (texProgram);
  }

private:
  tstring texProgram;
  
private:
  int m_font_mem_size;

public:
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_MPOST);
  }

public:
  void
  RunMakeMPX (/*[in]*/ const MIKTEXCHAR *	lpszMpFile,
	      /*[in]*/ const MIKTEXCHAR *	lpszMpxFile)
  {
    PathName makempx;
    if (! pSession->FindFile(MIKTEXTEXT("makempx"), FileType::EXE, makempx))
      {
	FatalError (MIKTEXTEXT("The makempx utility could not be found."));
      }
    
    PathName mpFile (lpszMpFile);
    mpFile.SetExtension (MIKTEXTEXT(".mp"));
    
    CommandLineBuilder arguments;
    
    if (texProgram.length() > 0)
      {
	arguments.AppendOption (MIKTEXTEXT("--tex="), texProgram);
      }
    
    arguments.AppendArgument (mpFile);
    arguments.AppendArgument (lpszMpxFile);
    
    Process::Run (makempx, arguments.Get());
  }
};

extern METAPOSTCLASS METAPOSTAPP;
#define THEAPP METAPOSTAPP

inline
void
miktexrunmakempx (/*[in]*/ const MIKTEXCHAR * lpszSource,
		  /*[in]*/ const MIKTEXCHAR * lpszDest)
{
  THEAPP.RunMakeMPX (lpszSource, lpszDest);
}

inline
bool
miktexopenmemfile (/*[in]*/ wordfile &		f,
		   /*[in]*/ bool		renew = false)
{
  return (THEAPP.OpenDumpFile(f, renew));
}

inline
bool
miktexopenpstabfile (/*[in]*/ alphafile &		f,
		     /*[in]*/ const MIKTEXCHAR *	lpszFileName)
{
  return (OpenMAPFile(&f, lpszFileName));
}

#include <miktex/mfapp.inl>
