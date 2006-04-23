/* CurlWebFile.h:						-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(HAVE_LIBCURL) && ! defined(GUARD_2B15B70CC1DC4C4B87E769DEDACD4092_)
#define GUARD_2B15B70CC1DC4C4B87E769DEDACD4092_

#include "WebFile.h"
#include "CurlWebSession.h"

BEGIN_INTERNAL_NAMESPACE;

class
CurlWebFile : public WebFile
{
public:
  CurlWebFile (/*[in]*/ CurlWebSession *	pSession,
	       /*[in]*/ const MIKTEXCHAR *	lpszUrl,
	       /*[in]*/ IProgressNotify_ *	pIProgressNotify);

public:
  virtual
  ~CurlWebFile ();

public:
  virtual
  size_t
  Read (/*[out]*/ void *	pBuffer,
	/*[in]*/ size_t		n);

public:
  virtual
  void
  Close ();

private:
  static
  size_t
  WriteCallback (/*[in]*/ char *		pData,
		 /*[in]*/ size_t		elemSize,
		 /*[in]*/ size_t		numElements,
		 /*[in]*/ void *		pv);

private:
  static
  int
  ProgressCallback (/*[in]*/ void *		pv,
		    /*[in]*/ double		dltotal,
		    /*[in]*/ double		dlnow,
		    /*[in]*/ double		ultotal,
		    /*[in]*/ double		ulnow);

private:
  void
  TakeData (/*[in]*/ const void *	pData,
	    /*[in]*/ size_t		size);

private:
  void
  Perform ();

private:
  void
  SetOptions ();

private:
  void
  CurlInit ();

private:
  void
  Connect ();

private:
  void
  ReadInformationals ();

private:
  tstring
  GetCurlErrorString (/*[in]*/ CURLMcode code)
    const
  {
#if LIBCURL_VERSION_NUM >= 0x70c00
    return (curl_multi_strerror(code));
#else
    tstring str = T_("The CURL multi interface returned error code ");
    str += NUMTOSTR(code);
    return (str);
#endif
  }

private:
  CURLM * pCurlm;

private:
  bool handleAdded;

private:
  CurlWebSession * pSession;

private:
  tstring url;

private:
  vector<char> buffer;

private:
  int runningHandles;

private:
  IProgressNotify_ * pIProgressNotify;

private:
  auto_ptr<TraceStream> trace_mpm;
};

END_INTERNAL_NAMESPACE;

#endif // libCURL
