/* Remove.h:							-*- C++ -*-

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of the Remove MiKTeX! Wizard.

   The Remove MiKTeX! Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The Remove MiKTeX! Wizard is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the Remove MiKTeX! Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#pragma once

#include "resource.h"

/* _________________________________________________________________________

   RemoveWizardApp
   _________________________________________________________________________ */

class RemoveWizardApp : public CWinApp
{
protected:
  DECLARE_MESSAGE_MAP();

public:
  RemoveWizardApp ();

protected:
  virtual
  BOOL
  InitInstance ();
  
};

/* _________________________________________________________________________

   Error Macros
   _________________________________________________________________________ */

#define INVALID_ARGUMENT(function, param)				\
  FATAL_MIKTEX_ERROR (function, T_("Invalid argument."), param)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     T_(__FILE__),				\
			     __LINE__)

#define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,		\
			      lpszInfo,			\
			      T_(__FILE__),		\
			      __LINE__)

#define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,				\
			      errorCode,				\
			      lpszInfo,					\
			      T_(__FILE__),				\
			      __LINE__)

#define CHECK_WINDOWS_ERROR(windowsFunction, lpszInfo)	\
  if (::GetLastError() != ERROR_SUCCESS)		\
    {							\
      FATAL_WINDOWS_ERROR (windowsFunction, lpszInfo);	\
    }

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

/* _________________________________________________________________________

   AutoBuffer
   _________________________________________________________________________ */

class AutoBuffer
{
protected:
  enum { BUFSIZE = 512 };

protected:
  MIKTEXCHAR smallBuffer[BUFSIZE];

protected:
  MIKTEXCHAR * buffer;

protected:
  size_t n;

public:
  AutoBuffer ()
    : buffer (smallBuffer),
      n (BUFSIZE)
  {
  }

public:
  AutoBuffer (/*[in]*/ size_t n)
    : n (n)
  {
    if (n <= BUFSIZE)
      {
	buffer = smallBuffer;
      }
    else
      {
	buffer = new MIKTEXCHAR[n];
      }
  }

public:
  ~AutoBuffer ()
  {
    try
      {
	if (buffer != smallBuffer)
	  {
	    delete [] buffer;
	  }
	buffer = 0;
	n = 0;
      }
    catch (const exception &)
      {
      }
  }

public:
  void
  Resize (/*[in]*/ size_t newSize)
  {
    if (newSize > BUFSIZE && newSize > n)
      {
	char * newBuffer = new MIKTEXCHAR[newSize];
	memcpy (newBuffer, buffer, n);
	if (buffer != smallBuffer)
	  {
	    delete [] buffer;
	  }
	buffer = newBuffer;
	n = newSize;
      }
  }

public:
  MIKTEXCHAR *
  GetBuffer ()
    const
  {
    return (buffer);
  }

public:
  const MIKTEXCHAR *
  Get ()
    const
  {
    return (buffer);
  }

public:
  size_t
  GetSize ()
    const
  {
    return (n);
  }
};

/* _________________________________________________________________________

   IsWindowsNT
   _________________________________________________________________________ */

inline
bool
IsWindowsNT ()
{
  return (GetVersion() < 0x80000000 ? true : false);
}

/* _________________________________________________________________________

   STR_BYT_SIZ
   _________________________________________________________________________ */

#define STR_BYT_SIZ(lpsz) ((StrLen(lpsz) + 1) * sizeof(MIKTEXCHAR))

/* _________________________________________________________________________

   Global Variables
   _________________________________________________________________________ */

extern PathName logFileName;

/* _________________________________________________________________________

   Protos
   _________________________________________________________________________ */

void
ReportError (/*[in]*/ const MiKTeXException & e);

void
ReportError (/*[in]*/ const exception & e);

void
RemoveEmptyDirectoryChain (/*[in]*/ const PathName &	directory);

vector<PathName>
GetRoots ();
