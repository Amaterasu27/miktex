/* CurlWebFile.cpp:

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

#include "StdAfx.h"

#if defined(HAVE_LIBCURL)

#include "internal.h"

#include "CurlWebFile.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

/* _________________________________________________________________________

   CurlWebFile::CurlWebFile
   _________________________________________________________________________ */

CurlWebFile::CurlWebFile (/*[in]*/ CurlWebSession *	pSession,
			  /*[in]*/ const char *		lpszUrl,
			  /*[in]*/ IProgressNotify_ *	pIProgressNotify)
  : handleAdded (false),
    pCurlm (0),
    pIProgressNotify (pIProgressNotify),
    pSession (pSession),
    runningHandles (0),
    trace_mpm (TraceStream::Open(MIKTEX_TRACE_MPM)),
    url (lpszUrl)
{
  try
    {
      SetOptions ();
      CurlInit();
      Connect ();
    }
  catch (const exception &)
    {
      if (handleAdded)
	{
	  curl_multi_remove_handle (pCurlm, pSession->GetHandle());
	}
      if (pCurlm != 0)
	{
	  curl_multi_cleanup (pCurlm);
	}
      throw;
    }
}

/* _________________________________________________________________________

   CurlWebFile::~CurlWebFile
   _________________________________________________________________________ */

CurlWebFile::~CurlWebFile ()
{
  try
    {
      Close ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   CurlWebFile::SetOptions
   _________________________________________________________________________ */

void
CurlWebFile::SetOptions ()
{
  pSession->SetOption (CURLOPT_URL, url.c_str());
  pSession->SetOption (CURLOPT_WRITEDATA, reinterpret_cast<void*>(this));
  curl_write_callback writeCallback = WriteCallback;
  pSession->SetOption (CURLOPT_WRITEFUNCTION, writeCallback);
  pSession->SetOption (CURLOPT_PROGRESSDATA, reinterpret_cast<void*>(this));
  curl_progress_callback progressCallback = ProgressCallback;
  pSession->SetOption (CURLOPT_PROGRESSFUNCTION, progressCallback);
}

/* _________________________________________________________________________

   CurlWebFile::CurlInit
   _________________________________________________________________________ */

void
CurlWebFile::CurlInit ()
{
  pCurlm = curl_multi_init();
  if (pCurlm == 0)
    {
      FATAL_MPM_ERROR
	("CurlWebFile::CurlInit",
	 T_("The cURL multi interface could not be initialized."),
	 0);
    }
  CURLMcode code = curl_multi_add_handle(pCurlm, pSession->GetHandle());
  if (code != CURLM_OK && code != CURLM_CALL_MULTI_PERFORM)
    {
      FATAL_MPM_ERROR ("CurlWebFile::CurlInit",
		       GetCurlErrorString(code).c_str(),
		       0);
    }
  handleAdded = true;
}

/* _________________________________________________________________________

   CurlWebFile::Connect
   _________________________________________________________________________ */

void
CurlWebFile::Connect ()
{
  CURLMcode code;
  do
    {
      code = curl_multi_perform(pCurlm, &runningHandles);
      if (code != CURLM_OK && code != CURLM_CALL_MULTI_PERFORM)
	{
	  FATAL_MPM_ERROR ("CurlWebFile::Connect",
			   GetCurlErrorString(code).c_str(),
			   0);
	}
      if (pIProgressNotify != 0)
	{
	  pIProgressNotify->OnProgress ();
	}
    }
  while (code == CURLM_CALL_MULTI_PERFORM);
  if (runningHandles == 0)
    {
      ReadInformationals ();
    }
}

/* _________________________________________________________________________

   CurlWebFile::ProgressCallback
   _________________________________________________________________________ */

int
CurlWebFile::ProgressCallback (/*[in]*/ void *		pv,
			       /*[in]*/ double		dltotal,
			       /*[in]*/ double		dlnow,
			       /*[in]*/ double		ultotal,
			       /*[in]*/ double		ulnow)
{
  UNUSED_ALWAYS (dltotal);
  UNUSED_ALWAYS (dlnow);
  UNUSED_ALWAYS (ultotal);
  UNUSED_ALWAYS (ulnow);
#if 1
  UNUSED_ALWAYS (pv);
  return (0);
#else
  try
    {
      CurlWebFile * This = reinterpret_cast<CurlWebFile*>(pv);
      if (This->pIProgressNotify != 0)
	{
	  This->pIProgressNotify->OnProgress ();
	}
      return (0);
    }
  catch (const exception &)
    {
      return (-1);
    }
#endif
}

/* _________________________________________________________________________

   CurlWebFile::WriteCallback
   _________________________________________________________________________ */

size_t
CurlWebFile::WriteCallback (/*[in]*/ char *		pData,
			    /*[in]*/ size_t		elemSize,
			    /*[in]*/ size_t		numElements,
			    /*[in]*/ void *		pv)
{
  try
    {
      CurlWebFile * This = reinterpret_cast<CurlWebFile*>(pv);
      This->TakeData (pData, elemSize * numElements);
      return (elemSize * numElements);
    }
  catch (const exception &)
    {
      return (0);
    }
}

/* _________________________________________________________________________

   CurlWebFile::TakeData
   _________________________________________________________________________ */

void
CurlWebFile::TakeData (/*[in]*/ const void *	pData,
		       /*[in]*/ size_t		size)
{
  const char * beg = reinterpret_cast<const char*>(pData);
  buffer.insert (buffer.end(), beg, beg + size);
}

/* _________________________________________________________________________

   CurlWebFile::Perform
   _________________________________________________________________________ */

void
CurlWebFile::Perform ()
{
  while (buffer.size() == 0 && runningHandles > 0)
    {
      fd_set fdread;
      fd_set fdwrite;
      fd_set fdexcep;
      
      FD_ZERO (&fdread);
      FD_ZERO (&fdwrite);
      FD_ZERO (&fdexcep);
      
      int maxfd;

      CURLMcode code
	= curl_multi_fdset(pCurlm, &fdread, &fdwrite, &fdexcep, &maxfd);
	  
      if (code != CURLM_OK && code != CURLM_CALL_MULTI_PERFORM)
	{
	  FATAL_MPM_ERROR ("CurlWebFile::Perform",
			   GetCurlErrorString(code).c_str(),
			   0);
	}
      
      struct timeval timeout;
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      
      int n = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);

      if (n < 0)
	{
	  FATAL_MPM_ERROR ("CurlWebFile::Perform",
			   T_("select() failed for some reason."),
			   NUMTOSTR(n));
	}
      
      if (n > 0)
	{
	  do
	    {
	      code = curl_multi_perform(pCurlm, &runningHandles);
	      if (code != CURLM_OK && code != CURLM_CALL_MULTI_PERFORM)
		{
		  FATAL_MPM_ERROR ("CurlWebFile::Perform",
				   GetCurlErrorString(code).c_str(),
				   0);
		}
	    }
	  while (code == CURLM_CALL_MULTI_PERFORM);
	}
    }

  if (runningHandles == 0)
    {
      ReadInformationals ();
    }
}


/* _________________________________________________________________________

   CurlWebFile::Read
   _________________________________________________________________________ */

size_t
CurlWebFile::Read (/*[out]*/ void *	pBuffer,
		   /*[in]*/ size_t	n)
{
  if (runningHandles > 0)
    {
      Perform ();
    }
  n = min(n, buffer.size());
  if (n > 0)
    {
      memcpy (pBuffer, &this->buffer[0], n);
      buffer.erase (buffer.begin(), buffer.begin() + n);
    }
  return (n);
}

/* _________________________________________________________________________

   CurlWebFile::Close
   _________________________________________________________________________ */

void
CurlWebFile::Close ()
{
  if (pCurlm != 0)
    {
      trace_mpm->WriteLine ("libmpm", T_("closing Web file"));
      CURLMcode r1 = curl_multi_remove_handle(pCurlm, pSession->GetHandle());
      CURLMcode r2 = curl_multi_cleanup(pCurlm);
      pCurlm = 0;
      if (r1 != CURLM_OK)
	{
	  FATAL_MPM_ERROR ("CurlWebFile::Close",
			   GetCurlErrorString(r1).c_str(),
			   0);
	}
      if (r2 != CURLM_OK)
	{
	  FATAL_MPM_ERROR ("CurlWebFile::Close",
			   GetCurlErrorString(r2).c_str(),
			   0);
	}
    }
  buffer.clear ();
}

/* _________________________________________________________________________

   CurlWebFile::ReadInformationals
   _________________________________________________________________________ */

void
CurlWebFile::ReadInformationals ()
{
  CURLMsg * pCurlMsg;
  int remaining;
  while ((pCurlMsg = curl_multi_info_read(pCurlm, &remaining)) != 0)
    {
      if (pCurlMsg->msg != CURLMSG_DONE)
	{
	  FATAL_MPM_ERROR ("CurlWebFile::ReadInformationals",
			   T_("Unexpected cURL message."),
			   NUMTOSTR(pCurlMsg->msg));
	}
      if (pCurlMsg->data.result != CURLE_OK)
	{
	  FATAL_MPM_ERROR
	    ("CurlWebFile::ReadInformationals",
	     pSession->GetCurlErrorString(pCurlMsg->data.result).c_str(),
	     0);
	}
      long responseCode;
      CURLcode r;
#if LIBCURL_VERSION_NUM >= 0x70a08
      r = curl_easy_getinfo(pCurlMsg->easy_handle,
			    CURLINFO_RESPONSE_CODE,
			    &responseCode);
#else
      r = curl_easy_getinfo(pCurlMsg->easy_handle,
			    CURLINFO_HTTP_CODE,
			    &responseCode);
#endif
      if (r != CURLE_OK)
	{
	  FATAL_MPM_ERROR
	    ("CurlWebFile::ReadInformationals",
	     pSession->GetCurlErrorString(r).c_str(),
	     0);
	}
      trace_mpm->WriteFormattedLine ("libmpm",
				     T_("response code: %ld"),
				     responseCode);
      if (responseCode >= 400)
	{
	  string msg = T_("Error response from server: ");
	  msg += NUMTOSTR(responseCode);
	  FATAL_MPM_ERROR
	    ("CurlWebFile::ReadInformationals",
	     msg.c_str(),
	     0);
	}
    }
}

#endif // libCURL
