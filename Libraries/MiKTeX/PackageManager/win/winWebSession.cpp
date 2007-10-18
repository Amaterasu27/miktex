/* winWebSession.cpp:

   Copyright (C) 2001-2006 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#pragma once

#include "StdAfx.h"

#include "internal.h"

#include "win/winWebFile.h"
#include "win/winWebSession.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

BEGIN_INTERNAL_NAMESPACE;

/* _________________________________________________________________________

   winWebSession::winWebSession ()
   _________________________________________________________________________ */

winWebSession::winWebSession ()
  : hInternet (0)
{
}

/* _________________________________________________________________________

   winWebSession::~winWebSession ()
   _________________________________________________________________________ */

winWebSession::~winWebSession ()
{
  try
    {
      Dispose ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   winWebSession::OpenUrl
   _________________________________________________________________________ */

WebFile *
winWebSession::OpenUrl (/*[in]*/ const char *		lpszUrl,
			/*[in]*/ IProgressNotify_ *	pIProgressNotify)
{
  UNUSED_ALWAYS (pIProgressNotify);
  if (hInternet == 0)
    {
      // check to see if computer is connected to the Internet
      if (InternetAttemptConnect(0) != ERROR_SUCCESS)
	{
	  FATAL_WINDOWS_ERROR ("InternetAttemptConnect", 0);
	}
      
      hInternet
	= InternetOpen(MPM_AGENT,
		       INTERNET_OPEN_TYPE_PRECONFIG,
		       0,
		       0,
		       INTERNET_FLAG_KEEP_CONNECTION);
      if (hInternet == 0)
	{
	  string error;
	  GetLastErrorMessage (error);
	  FATAL_MPM_ERROR ("winWebSession::winWebSession",
			   error.c_str(),
			   0);
	}
    }
  return (new winWebFile (this, lpszUrl));
}

/* _________________________________________________________________________

   winWebSession::Dispose
   _________________________________________________________________________ */

void
winWebSession::Dispose ()
{
  if (this->hInternet != 0)
    {
      HINTERNET hInternet = this->hInternet;
      this->hInternet = 0;
      if (! InternetCloseHandle(hInternet))
	{
	  string error;
	  GetLastErrorMessage (error);
	  FATAL_MPM_ERROR ("winWebSession::Dispose", error.c_str(), 0);
	}
    }
}

/* _________________________________________________________________________

   winWebSession::IsGlobalOffline
   _________________________________________________________________________ */

bool
winWebSession::IsGlobalOffline ()
{
  unsigned long state = 0;
  unsigned long size = sizeof(unsigned int);
  return (InternetQueryOption(0,
			      INTERNET_OPTION_CONNECTED_STATE,
			      &state,
			      &size)
	  && (state & INTERNET_STATE_DISCONNECTED_BY_USER) != 0);
}

/* _________________________________________________________________________

   winWebSession::GetLastErrorMessage
   _________________________________________________________________________ */

void
winWebSession::GetLastErrorMessage (/*[out]*/ string &	message)
{
  unsigned int lastError = ::GetLastError();
  void * pMsgBuf = 0;
  if (lastError >= INTERNET_ERROR_BASE && lastError <= INTERNET_ERROR_LAST)
    {
      if (lastError == ERROR_INTERNET_EXTENDED_ERROR)
	{
	  unsigned long inetError;
	  unsigned long length;
	  if (! InternetGetLastResponseInfoA(&inetError, 0, &length)
	      && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	    {
	      length += 1;
	      pMsgBuf = LocalAlloc(LMEM_FIXED, length);
	      if (pMsgBuf != 0)
		{
		  if (! InternetGetLastResponseInfoA(&inetError,
				    reinterpret_cast<char*>(pMsgBuf),
						    &length))
		    {
		      LocalFree (pMsgBuf);
		      pMsgBuf = 0;
		    }
		}
	    }
	}
      else
	{
	  FormatMessageA ((FORMAT_MESSAGE_ALLOCATE_BUFFER
			   | FORMAT_MESSAGE_IGNORE_INSERTS
			   | FORMAT_MESSAGE_FROM_HMODULE),
			  GetModuleHandleA("wininet.dll"),
			  lastError,
			  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			  reinterpret_cast<char*>(&pMsgBuf),
			  0,
			  0);
	}
    }
  else
    {
      FormatMessageA ((FORMAT_MESSAGE_ALLOCATE_BUFFER
		       | FORMAT_MESSAGE_IGNORE_INSERTS
		       | FORMAT_MESSAGE_FROM_SYSTEM),
		      0,
		      lastError,
		      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		      reinterpret_cast<char*>(&pMsgBuf),
		      0,
		      0);
    }
  if (pMsgBuf == 0)
    {
      message = T_("Unexpected error condition.");
    }
  else
    {
      message = reinterpret_cast<const char*>(pMsgBuf);
      LocalFree (pMsgBuf);
    }
}

END_INTERNAL_NAMESPACE;
