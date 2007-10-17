/* miktex/webapp.h:					       	-*- C++ -*-

   Copyright (C) 1996-2007 Christian Schenk

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

#if ! defined(MIKTEX__79ED517D_1F1C_42F4_B632_4CECD7C51331__)
#define MIKTEX__79ED517D_1F1C_42F4_B632_4CECD7C51331__

#include <miktex/miktex-texmf.h>

#include <cstdio>

#include <algorithm>
#include <bitset>
#include <hash_set>
#include <memory>
#include <string>
#include <vector>

#include <MiKTeX/C4P/C4P>
#include <popt-miktex.h>

#include <MiKTeX/App/Application>

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

namespace C4P {
  class Exception31l {};	// aux_done
  class Exception32l {};	// bst_done
  class Exception9932l {};	// no_bst_file
  class Exception9998l {};	// end_of_TEX
  class Exception9999l {};	// final_end
}

MIKTEXMF_BEGIN_NAMESPACE;

/* _________________________________________________________________________
     
   Feature
   _________________________________________________________________________ */

class FeatureEnum
{
public:
  enum EnumType {
    EightBitChars,
    TCX
  };
};

typedef MiKTeX::Core::EnumWrapper<FeatureEnum> Feature;

/* _________________________________________________________________________

   miktexopentfmfile
   _________________________________________________________________________ */

template<class FileType>
inline
bool
miktexopentfmfile (/*[out]*/ FileType &		f,
		   /*[in]*/ const char *	lpszFileName)
{
  return (OpenTFMFile(&f, lpszFileName));
}

/* _________________________________________________________________________

   miktexopenvffile
   _________________________________________________________________________ */

template<class FileType>
inline
bool
miktexopenvffile (/*[out]*/ FileType &		f,
		  /*[in]*/ const char *		lpszFileName)
{
  return (OpenVFFile(&f, lpszFileName));
}

/* _________________________________________________________________________

   miktexopenxfmfile
   _________________________________________________________________________ */

template<class FileType>
inline
bool
miktexopenxfmfile (/*[out]*/ FileType &		f,
		   /*[in]*/ const char *	lpszFileName)
{
  return (OpenXFMFile(&f, lpszFileName));
}

/* _________________________________________________________________________

   miktexopenxvffile
   _________________________________________________________________________ */

template<class FileType>
inline
bool
miktexopenxvffile (/*[out]*/ FileType &		f,
		   /*[in]*/ const char *	lpszFileName)
{
  return (OpenXVFFile(&f, lpszFileName));
}

/* _________________________________________________________________________

   miktexprintmiktexbanner
   _________________________________________________________________________ */

template<class FileType>
inline
void
miktexprintmiktexbanner (/*[in]*/ FileType & f)
{
  fprintf (f, " (%s)", MiKTeX::Core::Utils::GetMiKTeXBannerString().c_str());
}

/* _________________________________________________________________________

   WebApp
   _________________________________________________________________________ */

class WebApp

  : public MiKTeX::App::Application

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
     
     GetInputFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetInputFileType ()
    const
  {
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (MiKTeX::Core::FileType::None);
  }

  /* _______________________________________________________________________
     
     GetHelpId
     _______________________________________________________________________ */

public:

  virtual
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
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
    // must be implemented in sub-classes
    MIKTEX_ASSERT (false);
    return (0);
  }

  /* _______________________________________________________________________
     
     ProcessCommandLineOptions
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  ProcessCommandLineOptions ();

  /* _______________________________________________________________________
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(bool)
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg);
	    

  /* _______________________________________________________________________
     
     TheNameOfTheGame
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(const MIKTEXCHAR *)
  TheNameOfTheGame ()
    const;

  /* _______________________________________________________________________
     
     FatalError
     _______________________________________________________________________ */

public:

  MIKTEXNORETURN
  MIKTEXMFAPI(void)
  FatalError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	      /*[in]*/				...);

  /* _______________________________________________________________________
     
     AddOption
     _______________________________________________________________________ */
  
protected:

  enum {
    no_argument = POPT_ARG_NONE,
    required_argument = POPT_ARG_STRING,
    optional_argument = POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL,
  };

  enum {
    OPT_UNSUPPORTED = INT_MAX - 100,
    OPT_NOOP,
  };

  MIKTEXMFAPI(void)
  AddOption (/*[in]*/ const MIKTEXCHAR *	lpszNameAndHelp,
	     /*[in]*/ int			opt,
	     /*[in]*/ int			argInfo = POPT_ARG_NONE,
	     /*[in]*/ const MIKTEXCHAR *	lpszArgDescription = 0,
	     /*[in]*/ void *			pArg = 0,
	     /*[in]*/ MIKTEXCHAR		shortName = 0);

  /* _______________________________________________________________________
     
     AddOption
     _______________________________________________________________________ */
  
protected:

  MIKTEXMFAPI(void)
  AddOption (/*[in]*/ const MIKTEXCHAR *	lpszAliasName,
	     /*[in]*/ const MIKTEXCHAR *	lpszName);

  /* _______________________________________________________________________
     
     BadUsage
     _______________________________________________________________________ */

protected:

  MIKTEXMFAPI(void)
  BadUsage ()
    const;
  
  /* _______________________________________________________________________
     
     IsFeatureEnabled
     _______________________________________________________________________ */

public:

  bool
  IsFeatureEnabled (/*[in]*/ Feature f)
    const
  {
    return (features[f.Get()]);
  }

  /* _______________________________________________________________________
     
     GetProgramName
     _______________________________________________________________________ */

public:

  const MIKTEXCHAR *
  GetProgramName ()
    const
  {
    return (programName.c_str());
  }

  /* _______________________________________________________________________
     
     AmI
     _______________________________________________________________________ */

protected:
  bool
  AmI (/*[in]*/ const MIKTEXCHAR * lpszName)
    const
  {
    return (MiKTeX::Core::Utils::Contains(GetProgramName(), lpszName));
  }

  /* _______________________________________________________________________
     
     Enable8BitCharsP
     _______________________________________________________________________ */

public:

  bool
  Enable8BitCharsP ()
    const
  {
    return (enable8BitChars);
  }

  /* _______________________________________________________________________
     
     GetTcxFileName
     _______________________________________________________________________ */

public:

  MiKTeX::Core::PathName
  GetTcxFileName ()
    const
  {
    return (tcxFileName);
  }

  /* _______________________________________________________________________
     
     InitializeCharTables
     _______________________________________________________________________ */

public:

#if defined(THEDATA) && defined(IMPLEMENT_TCX)
  void
  InitializeCharTables ()
  {
    unsigned long flags = 0;
    if (tcxFileName.GetLength() > 0)
      {
	flags |= ICT_TCX;
      }
    if (Enable8BitCharsP())
      {
	flags |= ICT_8BIT;
      }
#if defined(MIKTEX_TEX_COMPILER) || defined(MIKTEX_META_COMPILER)
    MiKTeX::TeXAndFriends::InitializeCharTables (flags,
						 tcxFileName.Get(),
						 THEDATA(xchr),
						 THEDATA(xord),
						 THEDATA(xprn));
#else
    MiKTeX::TeXAndFriends::InitializeCharTables (flags,
						 tcxFileName.Get(),
						 THEDATA(xchr),
						 THEDATA(xord),
						 0);
#endif
  }
#endif

  /* _______________________________________________________________________
     
     EnableFeature
     _______________________________________________________________________ */

public:

  void
  EnableFeature (/*[in]*/ Feature f)
  {
    features[f.Get()] = true;
  }

  /* _______________________________________________________________________
     
     SetProgramInfo
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(void)
  SetProgramInfo (/*[in]*/ const MIKTEXCHAR * lpszProgramName,
		  /*[in]*/ const MIKTEXCHAR * lpszVersion,
		  /*[in]*/ const MIKTEXCHAR * lpszCopyright,
		  /*[in]*/ const MIKTEXCHAR * lpszTrademarks);

  /* _______________________________________________________________________
     
     Enable8BitChars
     _______________________________________________________________________ */

protected:

  void
  Enable8BitChars (/*[in]*/ bool enable8BitChars)
  {
    this->enable8BitChars = enable8BitChars;
  }

  /* _______________________________________________________________________
     
     SetTcxFileName
     _______________________________________________________________________ */

protected:

  void
  SetTcxFileName (/*[in]*/ const MIKTEXCHAR * lpszTcxFileName)
  {
    tcxFileName = lpszTcxFileName;
  }

  /* _______________________________________________________________________
     
     ShowHelp
     _______________________________________________________________________ */

protected:

  MIKTEXMFAPI(void)
  ShowHelp (/*[in]*/ bool usageOnly = false)
    const;

  /* _______________________________________________________________________
     
     ShowProgramVersion
     _______________________________________________________________________ */
  
protected:

  MIKTEXMFAPI(void)
  ShowProgramVersion ()
    const;

  /* _______________________________________________________________________
     
     GetOptions
     _______________________________________________________________________ */

protected:

  const std::vector<poptOption> &
  GetOptions ()
    const
  {
    return (options);
  }

  /* _______________________________________________________________________
     
     Variables
     _______________________________________________________________________ */

private:
  std::bitset<32> features;

private:
  Cpopt popt;
  
private:
  MiKTeX::Core::tstring copyright;
  
private:
  MiKTeX::Core::PathName packageListFileName;
  
private:
  MiKTeX::Core::tstring programName;
  
private:
  MiKTeX::Core::PathName tcxFileName;

private:
  bool enable8BitChars;
  
private:
  MiKTeX::Core::tstring trademarks;
  
private:
  MiKTeX::Core::tstring version;
  
private:
  std::vector<poptOption> options;

private:
  MiKTeX::Core::tstring theNameOfTheGame;

private:
  int optBase;
};

/* _________________________________________________________________________

   MIKTEX_DEFINE_WEBAPP
   _________________________________________________________________________ */

#if defined(VER_FILEVERSION_STR)
#  if defined(VER_LEGALTRADEMARKS_STR)
#    define SET_PROGRAM_INFO__423C8217_4CFC_41B7_9F89_EA3C4F729FD1(app)	\
      app.SetProgramInfo (app.TheNameOfTheGame(),			\
			  VER_FILEVERSION_STR,				\
			  VER_LEGALCOPYRIGHT_STR,			\
			  VER_LEGALTRADEMARKS_STR)
#  else
#    define SET_PROGRAM_INFO__423C8217_4CFC_41B7_9F89_EA3C4F729FD1(app)	\
      app.SetProgramInfo (app.TheNameOfTheGame(),			\
			  VER_FILEVERSION_STR,				\
			  VER_LEGALCOPYRIGHT_STR,			\
			  "")
#  endif
#else
#  define SET_PROGRAM_INFO__423C8217_4CFC_41B7_9F89_EA3C4F729FD1(app)
#endif

#define MIKTEX_DEFINE_WEBAPP(dllentry, appclass, app, program, data)	\
appclass app;								\
extern "C"								\
__declspec(dllexport)							\
int									\
MIKTEXMFCALL								\
dllentry (/*[in]*/ int			argc,				\
          /*[in]*/ const char **	argv)				\
{									\
  SET_PROGRAM_INFO__423C8217_4CFC_41B7_9F89_EA3C4F729FD1 (app);		\
  ZeroMemory (&data, sizeof(data));					\
  try									\
    {									\
      app.Init (argv[0]);						\
      int exitCode = program(argc, argv);				\
      app.Finalize ();							\
      return (exitCode);						\
    }									\
  catch (const MiKTeX::Core::MiKTeXException & e)			\
    {									\
      MiKTeX::Core::Utils::PrintException (e);				\
      return (1);							\
    }									\
  catch (const std::exception & e)					\
    {									\
      MiKTeX::Core::Utils::PrintException (e);				\
      return (1);							\
    }									\
}

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#pragma warning (disable: 4244)
#pragma warning (disable: 4245)
#pragma warning (disable: 4127)
#pragma warning (disable: 4611)
#pragma warning (disable: 4701)
#pragma warning (disable: 4101)

#endif
