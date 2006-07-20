/* mo.h:							-*- C++ -*-

   Copyright (C) 2000-2006 Christian Schenk

   This file is part of MiKTeX Options.

   MiKTeX Options is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.
   
   MiKTeX Options is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Options; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#include "resource.h"

class MiKTeXOptionsApplication : public CWinApp
{
public:
  MiKTeXOptionsApplication ();
  
public:
  virtual
  BOOL
  InitInstance ();
  
public:
  virtual
  void
  WinHelp (/*[in]*/ DWORD	data,
	   /*[in]*/ UINT	cmd = HELP_CONTEXT);

public:
  DECLARE_MESSAGE_MAP();
};

BOOL
OnHelpInfo (/*[in]*/ HELPINFO *			pHelpInfo,
	    /*[in]*/ const DWORD *		pHelpIDs,
	    /*[in]*/ const MIKTEXCHAR *		lpszTopicFile);

void
DoWhatsThisMenu (/*[in]*/ CWnd *		pWnd,
		 /*[in]*/ CPoint		point,
		 /*[in]*/ const DWORD *		pHelpIDs,
		 /*[in]*/ const MIKTEXCHAR *	lpszTopicFile);

bool
HasIE3 ();

struct LANGUAGE
{
  LANGUAGE ()
  {
  }
  LANGUAGE (/*[in]*/ const LANGUAGE & rhs)
    : languageName (rhs.languageName),
      fileName (rhs.fileName),
      synonyms (rhs.synonyms),
      active (rhs.active)
  {
  }
  CString languageName;
  CString fileName;
  vector<CString> synonyms;
  bool active;
};

/* _________________________________________________________________________

   AutoResource
   _________________________________________________________________________ */

template<class HandleType>
HandleType
InvalidHandleValue ()
{
  return (0);
}

template<class HandleType,
	 class Destroyer>
class AutoResource
{
public:
  AutoResource (/*[in]*/ HandleType handle = InvalidHandleValue<HandleType>())
    : handle (handle)
  {
  }

public:
  ~AutoResource ()
  {
    try
      {
	Reset ();
      }
    catch (const exception &)
      {
      }
  }

public:
  HandleType
  Get ()
    const
  {
    return (handle);
  }

public:
  void
  Detach ()
  {
    handle = InvalidHandleValue<HandleType>();
  }

public:
  void
  Reset ()
  {
    if (handle != InvalidHandleValue<HandleType>())
      {
	HandleType tmp = handle;
	handle = InvalidHandleValue<HandleType>();
	Destroyer() (tmp);
      }
  }

public:
  HandleType *
  operator & ()
  {
    return (&handle);
  }

private:
  HandleType handle;
};

/* _________________________________________________________________________

   AutoCoTaskMem
   _________________________________________________________________________ */

class CoTaskMemFree_
{
public:
  void
  operator() (/*[in]*/ void * p)
  {
    CoTaskMemFree (p);
  }
};

typedef AutoResource<void *, CoTaskMemFree_> AutoCoTaskMem;

/* _________________________________________________________________________

   Debug-dependant Macros
   _________________________________________________________________________ */

#if ! defined(NDEBUG)

#  define MIKTEX_ASSERT(expr)					\
  static_cast<void>						\
   ((expr)							\
    ? 0								\
    : (FATAL_MIKTEX_ERROR (0, T_("Assertion failed."), #expr),	\
       0))

#else

#  define MIKTEX_ASSERT(expr)

#endif

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

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
                    T_(__FILE__),				\
		    __LINE__)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     T_(__FILE__),				\
			     __LINE__)

#define CRT_ERROR(lpszCrtFunction, lpszInfo)		\
  TraceStream::TraceLastCRTError (lpszCrtFunction,	\
				  lpszInfo,		\
				  T_(__FILE__),		\
				  __LINE__)

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,		\
			  lpszInfo,			\
			  T_(__FILE__),			\
			  __LINE__)

#define FATAL_CRT_ERROR_2(lpszCrtFunction, errorCode, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,			\
			  errorCode,				\
			  lpszInfo,				\
			  T_(__FILE__),				\
			  __LINE__)

#if defined(MIKTEX_WINDOWS)
#  define WINDOWS_ERROR(lpszWindowsFunction, lpszInfo)		\
  TraceStream::TraceLastWin32Error (lpszWindowsFunction,	\
				    lpszInfo,			\
				    T_(__FILE__),		\
				    __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,			\
			      lpszInfo,				\
			      T_(__FILE__),			\
			      __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,				\
			      errorCode,				\
			      lpszInfo,					\
			      T_(__FILE__),				\
			      __LINE__)
#endif

#define CHECK_WINDOWS_ERROR(windowsFunction, lpszInfo)	\
  if (::GetLastError() != ERROR_SUCCESS)		\
    {							\
      FATAL_WINDOWS_ERROR (windowsFunction, lpszInfo);	\
    }
