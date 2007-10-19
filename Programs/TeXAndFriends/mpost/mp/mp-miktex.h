/* mp-miktex.h:							-*- C++ -*-

   Copyright (C) 1998-2007 Christian Schenk

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

#if defined(MIKTEX_TRAPMP)
#  include "trapmpdefs.h"
#define THEDATA(x) TRAPMPDATA.m_##x
#else
#  include "mpdefs.h"
#define THEDATA(x) MPDATA.m_##x
#endif

#if ! defined(C4PEXTERN)
#  define C4PEXTERN extern
#endif

#include "mp.rc"
#include <MiKTeX/TeXAndFriends/MetafontApp>

namespace mpost {
#include "mpost.defaults.h"
}

#if ! defined(MIKTEXHELP_MPOST)
#  include <MiKTeX/Core/Help>
#endif

#if defined(MIKTEX_TRAPMP)
class TRAPMPCLASS
#else
class MPCLASS
#endif

  : public MiKTeX::TeXAndFriends::MetafontApp

{
public:
#if defined(MIKTEX_TRAPMP)
  TRAPMPCLASS ()
#else
  MPCLASS ()
#endif
  {
    inputFileType = MiKTeX::Core::FileType::MP;
  }

public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const char * lpszProgramInvocationName)
  {
    MetafontApp::Init (lpszProgramInvocationName);
    SetProgramInfo (MIKTEXTEXT("mp;mpost"), 0, 0, 0);
#if defined(IMPLEMENT_TCX)
    EnableFeature (MiKTeX::TeXAndFriends::Feature::TCX);
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
	       "N");

    AddOption (MIKTEXTEXT("tex\0Use TEXPROGRAM for text labels."),
	       OPT_TEX_PROGRAM,
	       required_argument,
	       "TEXPROGRAM");

    AddOption ("mem", "undump");

    AddOption ("troff", OPT_UNSUPPORTED, 0, 0, 0, MIKTEXTEXT('T'));
  }

public:
  virtual
  bool
  MIKTEXMFCALL
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const char *	lpszOptArg)
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
    Free (THEDATA(nextstr));
  }

public:
  void
  AllocateMemory ()
  {
    MetafontApp::AllocateMemory ();

    GETPARAMCHECK (m_font_mem_size,
		   fontmemsize,
		   font_mem_size,
		   mpost::mpost::font_mem_size());

    Allocate ("fontinfo", THEDATA(fontinfo), THEDATA(fontmemsize));

    if (IsInitProgram())
      {
	Allocate ("nextstr", THEDATA(nextstr), THEDATA(maxstrings));
      }
  }

public:
  virtual
  const char *
  MIKTEXMFCALL
  GetMemoryDumpFileExtension ()
    const
  {
    return (".mem");
  }

public:
  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetMemoryDumpFileType ()
    const
  {
    return (MiKTeX::Core::FileType::MEM);
  }

public:
  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetInputFileType ()
    const
  {
    return (MiKTeX::Core::FileType::MP);
  }

public:
  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetPoolFileType ()
    const
  {
    return (MiKTeX::Core::FileType::MPPOOL);
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
  const char *
  MIKTEXMFCALL
  GetMemoryDumpFileName ()
    const
  {
    return ("mpost.mem");
  }

public:
  const char *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    return ("mp.pool");
  }

public:
  const char *
  MIKTEXMFCALL
  GetInitProgramName ()
    const
  {
    return (MIKTEXTEXT("inimp;inimpost"));
  }

public:
  const char *
  MIKTEXMFCALL
  GetVirginProgramName ()
    const
  {
    return (MIKTEXTEXT("virmp;virmpost"));
  }

public:
  const char *
  MIKTEXMFCALL
  TheNameOfTheGame ()
    const
  {
    return ("MetaPost");
  }

private:
  std::string texProgram;
  
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
  RunMakeMPX (/*[in]*/ const char *	lpszMpFile,
	      /*[in]*/ const char *	lpszMpxFile)
  {
    MiKTeX::Core::PathName makempx;
    if (! pSession->FindFile("makempx",
			     MiKTeX::Core::FileType::EXE,
			     makempx))
      {
	FatalError (MIKTEXTEXT("The makempx utility could not be found."));
      }
    
    MiKTeX::Core::PathName mpFile (lpszMpFile);
    mpFile.SetExtension (".mp", false);
    
    MiKTeX::Core::CommandLineBuilder arguments;
    
    if (! texProgram.empty())
      {
	arguments.AppendOption (MIKTEXTEXT("--tex="), texProgram);
      }
    
    arguments.AppendArgument (mpFile);
    arguments.AppendArgument (lpszMpxFile);
    
    MiKTeX::Core::Process::Run (makempx, arguments.Get());
  }
};

#if defined(MIKTEX_TRAPMP)
extern TRAPMPCLASS TRAPMPAPP;
#define THEAPP TRAPMPAPP
#else
extern MPCLASS MPAPP;
#define THEAPP MPAPP
#endif

inline
void
miktexrunmakempx (/*[in]*/ const char * lpszSource,
		  /*[in]*/ const char * lpszDest)
{
  THEAPP.RunMakeMPX (lpszSource, lpszDest);
}

inline
bool
miktexopenmemfile (/*[in]*/ wordfile &		f,
		   /*[in]*/ bool		renew = false)
{
  return (THEAPP.OpenMemoryDumpFile(f, renew));
}

inline
bool
miktexopenpstabfile (/*[in]*/ alphafile &		f,
		     /*[in]*/ const char *	lpszFileName)
{
  return (MiKTeX::TeXAndFriends::OpenMAPFile(&f, lpszFileName));
}

inline
bool
miktexopenmetafontfile (/*[in]*/ alphafile &		f,
			/*[in]*/ const char *	lpszFileName)
{
  return (MiKTeX::TeXAndFriends::OpenMETAFONTFile(&f, lpszFileName));
}

#include <MiKTeX/TeXAndFriends/MetafontApp.inliners>

#include <MiKTeX/KPSE/Emulation>

#define fontname THEDATA(fontname)
#define fontpsnamefixed THEDATA(fontpsnamefixed)
#define fontsizes THEDATA(fontsizes)
#define jobname THEDATA(jobname)
#define memident THEDATA(memident)
#define mpfontmap THEDATA(mpfontmap)
#define nameoffile (&(THEDATA(nameoffile)[-1]))
#define nextstr THEDATA(nextstr)
#define poolptr THEDATA(poolptr)
#define poolsize THEDATA(poolsize)
#define psfile THEDATA(psfile)
#define selector THEDATA(selector)
#define strpool THEDATA(strpool)
#define strref THEDATA(strref)
#define strstart THEDATA(strstart)

#include "mplib.h"
