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

const long DEFAULT_CONNECTION_TIMEOUT_SECONDS = 30;
const long DEFAULT_FTP_RESPONSE_TIMEOUT_SECONDS = 30;
const long DEFAULT_TIMEOUT_SECONDS = 60 * 60;

/* _________________________________________________________________________

   CurlWebSession::CurlWebSession
   _________________________________________________________________________ */

CurlWebSession::CurlWebSession ()
  : pCurl (0),
    haveProxySettings (false),
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
    (T_("libmpm"),
     T_("initializing cURL library version %s..."),
     LIBCURL_VERSION);

  pCurl = curl_easy_init();

  if (pCurl == 0)
    {
      FATAL_MPM_ERROR
	(T_("CurlWebSession::Initialize"),
	 T_("The cURL easy interface could not be initialized."),
	 0);
    }

  SetOption (CURLOPT_USERAGENT, MPM_AGENT);

  tstring ftpMode =
    SessionWrapper(true)->GetConfigValue(0,
					 MIKTEX_REGVAL_FTP_MODE,
					 T_("pasv"));

  if (ftpMode == T_("port"))
    {
      SetOption (CURLOPT_FTPPORT, T_("-"));
    }
  else if (ftpMode == T_("pasv"))
    {
      SetOption (CURLOPT_FTP_USE_EPSV, static_cast<long>(false));
    }
  else if (ftpMode == T_("epsv"))
    {
      SetOption (CURLOPT_FTP_USE_EPSV, static_cast<long>(true));
    }
  else
    {
      FATAL_MPM_ERROR (T_("CurlWebSession::Initialize"),
		       T_("Invalid FTP mode configured."),
		       ftpMode.c_str());
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

  if (! proxySettings.proxy.empty())
    {
      SetOption (CURLOPT_PROXY, proxySettings.proxy.c_str());
    }

  if (! (proxySettings.user.empty() && proxySettings.password.empty()))
    {
      if (proxySettings.user.find(T_(':')) != tstring::npos)
	{
	  FATAL_MPM_ERROR
	    (T_("CurlWebSession::Initialize"),
	     T_("Unsupported proxy user name (colons are not supported)."),
	     0);
	}
      if (proxySettings.password.find(T_(':')) != tstring::npos)
	{
	  FATAL_MPM_ERROR
	    (T_("CurlWebSession::Initialize"),
	     T_("Unsupported proxy password."),
	     0);
	}
      userPassword = proxySettings.user;
      userPassword += T_(':');
      userPassword += proxySettings.password;
      SetOption (CURLOPT_PROXYUSERPWD, userPassword.c_str());
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
CurlWebSession::OpenUrl (/*[in]*/ const MIKTEXCHAR *	lpszUrl,
			 /*[in]*/ IProgressNotify_ *	pIProgressNotify)
{
  if (pCurl == 0)
    {
      Initialize ();
    }
  trace_curl->WriteFormattedLine (T_("libmpm"),
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
      trace_curl->WriteLine (T_("libmpm"), T_("releasing cURL easy handle"));
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
	  tstring text (pData, sizeData);
	  This->trace_curl->Write (T_("curl"), text.c_str());
	}
    }
  catch (const exception &)
    {
    }
  return (0);
}

/* _________________________________________________________________________

   CurlWebSession::SetProxyServer
   _________________________________________________________________________ */

void
CurlWebSession::SetProxyServer (/*[in]*/ const ProxySettings & proxySettings)
{
  haveProxySettings = true;
  this->proxySettings = proxySettings;
}

#endif // libCURL
