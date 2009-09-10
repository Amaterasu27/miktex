/* CurlWebSession.cpp:

   Copyright (C) 2001-2009 Christian Schenk

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

#include "CurlWebSession.h"
#include "CurlWebFile.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

const long DEFAULT_CONNECTION_TIMEOUT_SECONDS = 30;
const long DEFAULT_FTP_RESPONSE_TIMEOUT_SECONDS = 30;

#define ALLOW_REDIRECTS 1
#define DEFAULT_MAX_REDIRECTS 20

/* _________________________________________________________________________

   CurlWebSession::CurlWebSession
   _________________________________________________________________________ */

CurlWebSession::CurlWebSession (/*[in]*/ IProgressNotify_ * pIProgressNotify)
  : pCurl (0),
    pCurlm (0),
    pIProgressNotify (pIProgressNotify),
    runningHandles (-1),
    trace_mpm (TraceStream::Open(MIKTEX_TRACE_MPM)),
    trace_curl (TraceStream::Open(MIKTEX_TRACE_CURL))
{
}

/* _________________________________________________________________________

   CurlWebSession::Initialize
   _________________________________________________________________________ */

void
CurlWebSession::Initialize ()
{
  trace_curl->WriteFormattedLine
    ("libmpm",
     T_("initializing cURL library version %s"),
     LIBCURL_VERSION);

  pCurlm = curl_multi_init();

  if (pCurlm == 0)
    {
      FATAL_MPM_ERROR
	("CurlWebSession::Initialize",
	 T_("The cURL multi interface could not be initialized."),
	 0);
    }

  pCurl = curl_easy_init();

  if (pCurl == 0)
    {
      FATAL_MPM_ERROR
	("CurlWebSession::Initialize",
	 T_("The cURL easy interface could not be initialized."),
	 0);
    }

  SetOption (CURLOPT_USERAGENT, MPM_AGENT);

  string ftpMode =
    SessionWrapper(true)->GetConfigValue(0,
					 MIKTEX_REGVAL_FTP_MODE,
					 "default");

  if (ftpMode == "default")
    {
    }
  else if (ftpMode == "port")
    {
      SetOption (CURLOPT_FTPPORT, "-");
    }
  else if (ftpMode == "pasv")
    {
      SetOption (CURLOPT_FTP_USE_EPSV, static_cast<long>(false));
    }
  else if (ftpMode == "epsv")
    {
      SetOption (CURLOPT_FTP_USE_EPSV, static_cast<long>(true));
    }
  else
    {
      FATAL_MPM_ERROR ("CurlWebSession::Initialize",
		       T_("Invalid MiKTeX configuration."),
		       MIKTEX_REGVAL_FTP_MODE);
    }

  SetOption (CURLOPT_PROGRESSDATA, reinterpret_cast<void*>(this));
  curl_progress_callback progressCallback = ProgressCallback;
  SetOption (CURLOPT_PROGRESSFUNCTION, progressCallback);

  if (trace_curl->IsEnabled())
    {
      SetOption (CURLOPT_VERBOSE, static_cast<long>(true));
      curl_debug_callback debugCallback = DebugCallback;
      SetOption (CURLOPT_DEBUGFUNCTION, debugCallback);
      SetOption (CURLOPT_DEBUGDATA, reinterpret_cast<void*>(this));
    }
  else
    {
      SetOption (CURLOPT_VERBOSE, static_cast<long>(false));
    }

  SetOption (CURLOPT_CONNECTTIMEOUT, DEFAULT_CONNECTION_TIMEOUT_SECONDS);

#if LIBCURL_VERSION_NUM >= 0x70a08
  SetOption (CURLOPT_FTP_RESPONSE_TIMEOUT,
	     DEFAULT_FTP_RESPONSE_TIMEOUT_SECONDS);
#endif

  // SF 2855025
#if ALLOW_REDIRECTS
  int maxRedirects =
    SessionWrapper(true)->GetConfigValue(0,
					 MIKTEX_REGVAL_MAX_REDIRECTS,
					 DEFAULT_MAX_REDIRECTS);
  SetOption (CURLOPT_FOLLOWLOCATION, static_cast<long>(true));
  SetOption (CURLOPT_MAXREDIRS, static_cast<long>(maxRedirects));
#endif

  SetOption (CURLOPT_NOSIGNAL, static_cast<long>(true));

  ProxySettings proxySettings;

  bool haveProxySettings = PackageManager::TryGetProxy(proxySettings);

  if (haveProxySettings && proxySettings.useProxy)
    {
      proxyPort = proxySettings.proxy;
      proxyPort += ":";
      proxyPort += NUMTOSTR(proxySettings.port);
      SetOption (CURLOPT_PROXY, proxyPort.c_str());
      if (proxySettings.authenticationRequired)
	{
	  if (proxySettings.user.find(':') != string::npos)
	    {
	      FATAL_MPM_ERROR
		("CurlWebSession::Initialize",
		 T_("Unsupported proxy user name."),
		 0);
	    }
	  if (proxySettings.password.find(':') != string::npos)
	    {
	      FATAL_MPM_ERROR
		("CurlWebSession::Initialize",
		 T_("Unsupported proxy password."),
		 0);
	    }
	  userPassword = proxySettings.user;
	  userPassword += ':';
	  userPassword += proxySettings.password;
	  SetOption (CURLOPT_PROXYUSERPWD, userPassword.c_str());
	}
    }
}

/* _________________________________________________________________________

   CurlWebSession::~CurlWebSession
   _________________________________________________________________________ */

CurlWebSession::~CurlWebSession ()
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

   CurlWebSession::OpenUrl
   _________________________________________________________________________ */

WebFile *
CurlWebSession::OpenUrl (/*[in]*/ const char * lpszUrl)
{
  runningHandles = -1;
  if (pCurl == 0)
    {
      Initialize ();
    }
  trace_mpm->WriteFormattedLine ("libmpm",
				 T_("going to download %s"),
				 Q_(lpszUrl));
  return (new CurlWebFile (this, lpszUrl));
}

/* _________________________________________________________________________

   CurlWebSession::Dispose
   _________________________________________________________________________ */

void
CurlWebSession::Dispose ()
{
  if (pCurl != 0)
    {
      trace_curl->WriteLine ("libmpm", T_("releasing cURL easy handle"));
      curl_easy_cleanup (pCurl);
      pCurl = 0;
    }
  if (pCurlm != 0)
  {
    trace_curl->WriteLine ("libmpm", T_("releasing cURL multi handle"));
    CURLMcode code = curl_multi_cleanup(pCurlm);
    pCurlm = 0;
    if (code != CURLM_OK)
    {
      FATAL_MPM_ERROR (
	"CurlWebSession::Dispose",
	GetCurlErrorString(code).c_str(),
	0);
    }
  }
  runningHandles = -1;
}

/* _________________________________________________________________________

   CurlWebSession::Connect
   _________________________________________________________________________ */

void
CurlWebSession::Connect ()
{
  CURLMcode code;
  do
  {
    code = curl_multi_perform(pCurlm, &runningHandles);
    if (code != CURLM_OK && code != CURLM_CALL_MULTI_PERFORM)
    {
      FATAL_MPM_ERROR ("CurlWebSession::Connect",
	GetCurlErrorString(code).c_str(),
	0);
    }
  }
  while (code == CURLM_CALL_MULTI_PERFORM);
  if (runningHandles == 0)
  {
    ReadInformationals ();
  }
}

/* _________________________________________________________________________

   CurlWebSession::Perform
   _________________________________________________________________________ */

void
CurlWebSession::Perform ()
{
  Connect ();

  if (runningHandles == 0)
  {
    return;
  }

  int oldRunningHandles = runningHandles;

  if (runningHandles > 0)
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

    if (code != CURLM_OK)
    {
      FATAL_MPM_ERROR ("CurlWebSession::Perform",
	GetCurlErrorString(code).c_str(),
	0);
    }

    long timeout;

#if LIBCURL_VERSION_NUM >= 0x70f04 && 0
    code = curl_multi_timeout(pCurlm, &timeout);

    if (code != CURLM_OK)
    {
      FATAL_MPM_ERROR ("CurlWebSession::Perform",
	GetCurlErrorString(code).c_str(),
	0);
    }
#else
    timeout = 100;
#endif

    if (timeout < 0)
    {
      timeout = 100;
    }

    if (maxfd < 0)
    {
      Thread::Sleep (timeout);
    }
    else
    {
      struct timeval tv;
      tv.tv_sec = timeout / 1000;
      tv.tv_usec = (timeout % 1000) * 1000;

      int n = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &tv);

      if (n < 0)
      {
	FATAL_MPM_ERROR ("CurlWebSession::Perform",
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
	    FATAL_MPM_ERROR ("CurlWebSession::Perform",
	      GetCurlErrorString(code).c_str(),
	      0);
	  }
	}
	while (code == CURLM_CALL_MULTI_PERFORM);
      }
    }
  }

  if (oldRunningHandles >= 0 && runningHandles != oldRunningHandles)
  {
    ReadInformationals ();
  }
}

/* _________________________________________________________________________

   CurlWebSession::ReadInformationals
   _________________________________________________________________________ */

void
CurlWebSession::ReadInformationals ()
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
	     GetCurlErrorString(pCurlMsg->data.result).c_str(),
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
	     GetCurlErrorString(r).c_str(),
	     0);
	}
      trace_mpm->WriteFormattedLine ("libmpm",
				     T_("response code: %ld"),
				     responseCode);
      char * lpszEffectiveUrl = 0;
      r = curl_easy_getinfo(pCurlMsg->easy_handle,
			    CURLINFO_EFFECTIVE_URL,
			    &lpszEffectiveUrl);
      if (r != CURLE_OK)
      {
	FATAL_MPM_ERROR
	  ("CurlWebFile::ReadInformationals",
	  GetCurlErrorString(r).c_str(),
	  0);
      }
      if (lpszEffectiveUrl != 0)
      {
	trace_mpm->WriteFormattedLine (
	  "libmpm",
	  T_("effective URL: %s"),
	  lpszEffectiveUrl);
      }
      if (responseCode >= 300 && responseCode <= 399)
      {
#if ALLOW_REDIRECTS
	UNEXPECTED_CONDITION ("CurlWebSession::ReadInformationals");
#else
	string msg = T_("The server returned status code ");
	msg += NUMTOSTR(responseCode);
	msg += T_(", but redirection is not supported. You must choose ");
	msg += T_("another package repository.");
	FATAL_MPM_ERROR
	  ("CurlWebFile::ReadInformationals",
	  msg.c_str(),
	  0);
#endif
      }
      else if (responseCode >= 400)
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

/* _________________________________________________________________________

   CurlWebSession::ProgressCallback
   _________________________________________________________________________ */

int
CurlWebSession::ProgressCallback (/*[in]*/ void *		pv,
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
      CurlWebFile * This = reinterpret_cast<CurlWebSession*>(pv);
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

   CurlWebSession::DebugCallback
   _________________________________________________________________________ */

int
CurlWebSession::DebugCallback (/*[in]*/ CURL *		pCurl,
			       /*[in]*/ curl_infotype	infoType,
			       /*[in]*/ char *		pData,
			       /*[in]*/ size_t		sizeData,
			       /*[in]*/ void *		pv)
{
  UNUSED_ALWAYS (pCurl);
  try
    {
      CurlWebSession * This = reinterpret_cast<CurlWebSession*>(pv);
      if (infoType == CURLINFO_TEXT)
	{
	  MIKTEX_ASSERT (pData != 0);
	  string text (pData, sizeData);
	  This->trace_curl->Write ("libmpm", text.c_str());
	}
    }
  catch (const exception &)
    {
    }
  return (0);
}

#endif // libCURL
