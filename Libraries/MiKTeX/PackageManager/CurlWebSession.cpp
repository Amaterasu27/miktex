/* CurlWebSession.cpp:

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

#include "CurlWebSession.h"
#include "CurlWebFile.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

const long DEFAULT_CONNECTION_TIMEOUT_SECONDS = 30;
const long DEFAULT_FTP_RESPONSE_TIMEOUT_SECONDS = 30;
const long DEFAULT_TIMEOUT_SECONDS = 60 * 60;

/* _________________________________________________________________________

   CurlWebSession::CurlWebSession
   _________________________________________________________________________ */

CurlWebSession::CurlWebSession ()
  : pCurl (0),
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
					 "pasv");

  if (ftpMode == "port")
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
#if 0
  SetOption (CURLOPT_TIMEOUT, DEFAULT_TIMEOUT_SECONDS);
#endif

#if LIBCURL_VERSION_NUM >= 0x70a08
  SetOption (CURLOPT_FTP_RESPONSE_TIMEOUT,
	     DEFAULT_FTP_RESPONSE_TIMEOUT_SECONDS);
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
CurlWebSession::OpenUrl (/*[in]*/ const char *		lpszUrl,
			 /*[in]*/ IProgressNotify_ *	pIProgressNotify)
{
  if (pCurl == 0)
    {
      Initialize ();
    }
  trace_curl->WriteFormattedLine ("libmpm",
				  T_("going to download %s"),
				  Q_(lpszUrl));
  return (new CurlWebFile (this, lpszUrl, pIProgressNotify));
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
	  This->trace_curl->Write ("curl", text.c_str());
	}
    }
  catch (const exception &)
    {
    }
  return (0);
}

#endif // libCURL
