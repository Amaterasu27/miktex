/* CurlWebFile.h:						-*- C++ -*-

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
	       /*[in]*/ const char *		lpszUrl);

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
  void
  TakeData (/*[in]*/ const void *	pData,
	    /*[in]*/ size_t		size);

private:
  void
  Initialize ();

private:
  bool initialized;

private:
  CurlWebSession * pSession;

private:
  std::string url;

private:
  std::vector<char> buffer;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_mpm;
};

END_INTERNAL_NAMESPACE;

#endif // libCURL
