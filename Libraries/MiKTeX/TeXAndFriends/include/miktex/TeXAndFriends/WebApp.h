/* miktex/TeXAndFriends/WebApp.h:			       	-*- C++ -*-

   Copyright (C) 1996-2009 Christian Schenk

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

#include <miktex/TeXAndFriends/Prototypes>

#include <cstdio>

#include <algorithm>
#include <bitset>
#include <memory>
#include <string>
#include <vector>

#include <miktex/C4P/C4P>
#include <popt-miktex.h>

#include <miktex/App/Application>

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
     
     GetInputFileType
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(MiKTeX::Core::FileType)
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
  MIKTEXMFTHISAPI(unsigned long)
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
  MIKTEXMFTHISAPI(const char *)
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
  MIKTEXMFTHISAPI(void)
  ProcessCommandLineOptions ();

  /* _______________________________________________________________________
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(bool)
  ProcessOption (/*[in]*/ int		opt,
		 /*[in]*/ const char *	lpszOptArg);
	    

  /* _______________________________________________________________________
     
     TheNameOfTheGame
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(const char *)
  TheNameOfTheGame ()
    const;

  /* _______________________________________________________________________
     
     FatalError
     _______________________________________________________________________ */

public:

  MIKTEXNORETURN
  MIKTEXMFCEEAPI(void)
  FatalError (/*[in]*/ const char *	lpszFormat,
	      /*[in]*/			...);

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

  MIKTEXMFTHISAPI(void)
  AddOption (/*[in]*/ const char *	lpszNameAndHelp,
	     /*[in]*/ int		opt,
	     /*[in]*/ int		argInfo = POPT_ARG_NONE,
	     /*[in]*/ const char *	lpszArgDescription = 0,
	     /*[in]*/ void *		pArg = 0,
	     /*[in]*/ char		shortName = 0);

  /* _______________________________________________________________________
     
     AddOption
     _______________________________________________________________________ */
  
protected:

  MIKTEXMFTHISAPI(void)
  AddOption (/*[in]*/ const char *	lpszAliasName,
	     /*[in]*/ const char *	lpszName);

  /* _______________________________________________________________________
     
     BadUsage
     _______________________________________________________________________ */

protected:

  MIKTEXMFTHISAPI(void)
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

  const char *
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
  AmI (/*[in]*/ const char * lpszName)
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

  MIKTEXMFTHISAPI(void)
  SetProgramInfo (/*[in]*/ const char * lpszProgramName,
		  /*[in]*/ const char * lpszVersion,
		  /*[in]*/ const char * lpszCopyright,
		  /*[in]*/ const char * lpszTrademarks);

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
  SetTcxFileName (/*[in]*/ const char * lpszTcxFileName)
  {
    tcxFileName = lpszTcxFileName;
  }

  /* _______________________________________________________________________
     
     ShowHelp
     _______________________________________________________________________ */

protected:

  MIKTEXMFTHISAPI(void)
  ShowHelp (/*[in]*/ bool usageOnly = false)
    const;

  /* _______________________________________________________________________
     
     ShowProgramVersion
     _______________________________________________________________________ */
  
protected:

  MIKTEXMFTHISAPI(void)
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
  std::string copyright;
  
private:
  MiKTeX::Core::PathName packageListFileName;
  
private:
  std::string programName;
  
private:
  MiKTeX::Core::PathName tcxFileName;

private:
  bool enable8BitChars;
  
private:
  std::string trademarks;
  
private:
  std::string version;
  
private:
  std::vector<poptOption> options;

private:
  std::string theNameOfTheGame;

private:
  int optBase;
};

/* _________________________________________________________________________

   MIKTEX_DEFINE_WEBAPP
   _________________________________________________________________________ */

#if defined(MIKTEX_COMPONENT_VERSION_STR)
#  if defined(MIKTEX_COMP_TM_STR)
#    define SET_PROGRAM_INFO__423C8217_4CFC_41B7_9F89_EA3C4F729FD1(app)	\
      app.SetProgramInfo (app.TheNameOfTheGame(),			\
			  MIKTEX_COMPONENT_VERSION_STR,			\
			  MIKTEX_COMP_COPYRIGHT_STR,		\
			  MIKTEX_COMP_TM_STR)
#  else
#    define SET_PROGRAM_INFO__423C8217_4CFC_41B7_9F89_EA3C4F729FD1(app)	\
      app.SetProgramInfo (app.TheNameOfTheGame(),			\
			  MIKTEX_COMPONENT_VERSION_STR,			\
			  MIKTEX_COMP_COPYRIGHT_STR,		\
			  "")
#  endif
#else
#  define SET_PROGRAM_INFO__423C8217_4CFC_41B7_9F89_EA3C4F729FD1(app)
#endif

#if defined(_MSC_VER)
#  define MIKTEX_WEBAPP_EXPORT __declspec(dllexport)
#else
#  define MIKTEX_WEBAPP_EXPORT
#endif

#if USE_C4P_DATA_STRUCT

#define MIKTEX_DEFINE_WEBAPP(dllentry, appclass, app, program, data)	\
appclass app;								\
extern "C"								\
MIKTEX_WEBAPP_EXPORT							\
int									\
MIKTEXCEECALL								\
dllentry (/*[in]*/ int			argc,				\
          /*[in]*/ const char **	argv)				\
{									\
  SET_PROGRAM_INFO__423C8217_4CFC_41B7_9F89_EA3C4F729FD1 (app);		\
  memset (&data, 0, sizeof(data));					\
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

#else

#define MIKTEX_DEFINE_WEBAPP(dllentry, appclass, app, program)		\
appclass app;								\
extern "C"								\
MIKTEX_WEBAPP_EXPORT							\
int									\
MIKTEXCEECALL								\
dllentry (/*[in]*/ int			argc,				\
          /*[in]*/ const char **	argv)				\
{									\
  SET_PROGRAM_INFO__423C8217_4CFC_41B7_9F89_EA3C4F729FD1 (app);		\
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

#endif // USE_C4P_DATA_STRUCT

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
