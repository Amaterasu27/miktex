/* CurlWebSession.h:						-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(HAVE_LIBCURL) && ! defined(GUARD_8FCCFB2E6A53AC448DF1F0EDABB41EC0_)
#define GUARD_8FCCFB2E6A53AC448DF1F0EDABB41EC0_

BEGIN_INTERNAL_NAMESPACE;

class
CurlWebSession : public WebSession
{
public:
  CurlWebSession (/*[in]*/ IProgressNotify_ * pIProgressNotify);

public:
  virtual
  ~CurlWebSession ();

public:
  virtual
  WebFile *
  OpenUrl (/*[in]*/ const char * lpszUrl);

public:
  virtual
  void
  Dispose ();

public:
  void
  SendReceive ();

public:
  void
  Perform ();

private:
  void
  ReadInformationals ();

public:
  std::string
  GetCurlErrorString (/*[in]*/ CURLMcode code)
    const
  {
#if LIBCURL_VERSION_NUM >= 0x70c00
    return (curl_multi_strerror(code));
#else
    std::string str =
      T_("The CURL multi interface returned an error code: ");
    str += NUMTOSTR(code);
    return (str);
#endif
  }

private:
  CURL * pCurl;

private:
  CURLM * pCurlm;

private:
  int runningHandles;

private:
  IProgressNotify_ * pIProgressNotify;

public:
  bool
  IsReady ()
    const
  {
    return (runningHandles == 0);
  }

public:
  CURL *
  GetEasyHandle ()
    const
    throw ()
  {
    return (pCurl);
  }

public:
  CURLM *
  GetMultiHandle ()
    const
    throw ()
  {
    return (pCurlm);
  }

public:
  std::string
  GetCurlErrorString (/*[in]*/ CURLcode code)
    const
  {
#if LIBCURL_VERSION_NUM >= 0x70c00
    return (curl_easy_strerror(code));
#else
    std::string str = T_("The cURL easy interface returned an error code: ");
    str += NUMTOSTR(code);
    return (str);
#endif
  }

public:
  template<class T>
  void
  SetOption (/*[in]*/ CURLoption	option,
	     /*[in]*/ T			val)
    const
  {
    CURLcode code = curl_easy_setopt(pCurl, option, val);
    if (code != CURLE_OK)
      {
	FATAL_MPM_ERROR ("CurlWebSession::SetOption",
			 GetCurlErrorString(code).c_str(),
			 NUMTOSTR(option));
      }
  }

private:
  void
  Initialize ();

private:
  static
  int
  ProgressCallback (/*[in]*/ void *		pv,
		    /*[in]*/ double		dltotal,
		    /*[in]*/ double		dlnow,
		    /*[in]*/ double		ultotal,
		    /*[in]*/ double		ulnow);

private:
  static
  int
  DebugCallback (/*[in]*/ CURL *		pCurl,
		 /*[in]*/ curl_infotype		infoType,
		 /*[in]*/ char *		pData,
		 /*[in]*/ size_t		sizeData,
		 /*[in]*/ void *		pv);

private:
  std::string proxyPort;

private:
  std::string userPassword;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_curl;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_mpm;
};

END_INTERNAL_NAMESPACE;

#endif // libCURL
