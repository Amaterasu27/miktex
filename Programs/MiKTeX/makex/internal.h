/* internal.h:							-*- C++ -*-

   Copyright (C) 1998-2009 Christian Schenk

   This file is part of the MiKTeX Maker Library.

   The MiKTeX Maker Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Maker Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Maker Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(_MSC_VER)
#  pragma warning (disable: 4786)
#endif

#include "makex-version.h"

#include <iomanip>
#include <iostream>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <string>

#include <malloc.h>

#include <getopt.h>
#include <miktex/App/Application>
#include <miktex/Core/Core>
#include <miktex/Core/Debug>
#include <miktex/Core/Paths>
#include <miktex/Core/Registry>

#if defined(_MSC_VER)
#  define MKTEXAPI(func) extern "C" __declspec(dllexport) int __cdecl func
#else
#  define MKTEXAPI(func) extern "C"  int func
#endif

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

#define OUT__ (stdoutStderr ? tcerr : tcout)

#include "makex.h"

using namespace MiKTeX::Core;
using namespace std;

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)
#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

/* _________________________________________________________________________

   Debug-dependant Macros
   _________________________________________________________________________ */

#if ! defined(UNUSED)
#  if ! defined(NDEBUG)
#    define UNUSED(x)
#  else
#    define UNUSED(x) static_cast<void>(x)
#  endif
#endif

#if ! defined(UNUSED_ALWAYS)
#  define UNUSED_ALWAYS(x) static_cast<void>(x)
#endif

/* _________________________________________________________________________

   Error Macros
   _________________________________________________________________________ */

#define BUF_TOO_SMALL(function)						\
  FATAL_MIKTEX_ERROR (function,						\
                      T_("Not enough room in an internal buffer."),	\
                      0)

#define INVALID_ARGUMENT(function, param)				\
  FATAL_MIKTEX_ERROR (function, T_("Invalid argument."), param)

#define OUT_OF_MEMORY(function)					\
  FATAL_MIKTEX_ERROR(function, T_("Virtual memory exhausted."), 0)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define UNIMPLEMENTED(function)						\
  FATAL_MIKTEX_ERROR (function, T_("Function not implemented."), 0)

#define MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  TraceMiKTeXError (miktexFunction,				\
                    traceMessage,				\
                    lpszInfo,					\
                    __FILE__,				\
		    __LINE__)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     __FILE__,				\
			     __LINE__)

#define CRT_ERROR(lpszCrtFunction, lpszInfo)		\
  TraceStream::TraceLastCRTError (lpszCrtFunction,	\
				  lpszInfo,		\
				  __FILE__,		\
				  __LINE__)

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,		\
			  lpszInfo,			\
			  __FILE__,			\
			  __LINE__)

#define FATAL_CRT_ERROR_2(lpszCrtFunction, errorCode, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,			\
			  errorCode,				\
			  lpszInfo,				\
			  __FILE__,				\
			  __LINE__)

#if defined(MIKTEX_WINDOWS)
#  define WINDOWS_ERROR(lpszWindowsFunction, lpszInfo)		\
  TraceStream::TraceLastWin32Error (lpszWindowsFunction,	\
				    lpszInfo,			\
				    __FILE__,		\
				    __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,			\
			      lpszInfo,				\
			      __FILE__,			\
			      __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,				\
			      errorCode,				\
			      lpszInfo,					\
			      __FILE__,				\
			      __LINE__)
#endif

#define FATAL_MPM_ERROR(miktexFunction, traceMessage, lpszInfo) \
  FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)

/* _________________________________________________________________________

   HasPrefix
   _________________________________________________________________________ */

inline
bool
HasPrefix (/*[in]*/ const char * lpsz1,
	   /*[in]*/ const char * lpsz2)
{
  return (PathName::Compare(lpsz1, lpsz2, strlen(lpsz2)) == 0);
}

/* _________________________________________________________________________

   MakeUtility
   _________________________________________________________________________ */

class MakeUtility
  : public MiKTeX::App::Application
{
public:
  MakeUtility ()
    : quiet (false),
      verbose (false),
      debug (false),
      printOnly (false),
      stdoutStderr (true),
      adminMode (false)
  {
  }

public:
  virtual
  ~MakeUtility ()
  {
  }

public:
  virtual
  MIKTEXAPPTHISAPI(void)
  Init (/*[in]*/ const MiKTeX::Core::Session::InitInfo & initInfo);

public:
  virtual
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv)
    = 0;
      
protected:
  virtual
  void
  Usage ()
    = 0;

protected:
  virtual
  void
  CreateDestinationDirectory ()
    = 0;

protected:
  virtual
  void
  HandleOption (/*[in]*/ int			ch,
		/*[in]*/ const char *	lpszOptArg,
		/*[in,out]*/ bool &		handled)
    = 0;

protected:
  void
  GetOptions (/*[in]*/ int			argc,
	      /*[in]*/ const char **	argv,
	      /*[in]*/ const struct option *	pLongOptions,
	      /*[out]*/ int &			index);

protected:
  bool
  RunProcess (/*[in]*/ const char *	lpszExeName,
	      /*[in]*/ const char *	lpszArguments);
  
protected:
  bool
  RunMETAFONT (/*[in]*/ const char *	lpszName,
	       /*[in]*/ const char *	lpszMode,
	       /*[in]*/ const char *	lpszMag);

protected:
  void
  Install (/*[in]*/ const PathName & source,
	   /*[in]*/ const PathName & dest);

protected:
  void
  CreateDirectory (/*[in]*/ const string &	templ,
		   /*[out]*/ PathName &		path);

protected:
  MIKTEXNORETURN
  void
  FatalError (/*[in]*/ const char *	lpszFormat,
	      /*[in]*/				...)
    const;

protected:
  void
  ShowVersion ();

protected:
  void
  Verbose (/*[in]*/ const char * lpszFormat,
	   /*[in]*/			...);

protected:
  void
  Message (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

protected:
  void
  PrintOnly (/*[in]*/ const char *	lpszFormat,
	     /*[in]*/				...);

protected:
  bool quiet;

protected:
  bool verbose;

protected:
  bool debug;

protected:
  bool printOnly;

protected:
  bool stdoutStderr;

protected:
  PathName destinationDirectory;

protected:
  string name;

protected:
  bool adminMode;
};

/* _________________________________________________________________________

   Option Table Implementation
   _________________________________________________________________________ */

#define COMMON_OPTIONS							\
    T_("admin"),	no_argument,		0,	'A',	\
    T_("debug"),	no_argument,		0,	'd',	\
    T_("help"),		no_argument,		0,	'h',	\
    T_("print-only"),	no_argument,		0,	'n',	\
    T_("quiet"),	no_argument,		0,	'q',	\
    T_("verbose"),	no_argument,		0,	'v',	\
    T_("version"),	no_argument,		0,	'V'

#define BEGIN_OPTION_MAP(cls)				\
void							\
HandleOption (int	ch,		\
		   const char *	lpszOptArg,	\
		   bool &		bHandled)	\
{							\
  UNUSED_ALWAYS(lpszOptArg);				\
  switch (ch)						\
    {

#define OPTION_ENTRY_TRUE(ch, var)		\
    case ch:					\
      var = true;				\
      break;

#define OPTION_ENTRY_SET(ch, var)		\
    case ch:					\
      var = lpszOptArg;				\
      break;

#define OPTION_ENTRY_STRING(ch, var, size)		\
    case ch:						\
      Utils::CopyString (var, size, lpszOptArg);	\
      break;

#define OPTION_ENTRY(ch, action)		\
    case ch:					\
      action;					\
      break;

#define END_OPTION_MAP()			\
    default:					\
      bHandled = false;				\
      break;					\
    }						\
}
