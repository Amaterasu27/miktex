/* CurlWebFile.cpp:

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

#include "CurlWebFile.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

const int READ_TIMEOUT_SECONDS = 40;

/* _________________________________________________________________________

   CurlWebFile::CurlWebFile
   _________________________________________________________________________ */

CurlWebFile::CurlWebFile (/*[in]*/ CurlWebSession *	pSession,
			  /*[in]*/ const char *		lpszUrl)
  : initialized (false),
    pSession (pSession),
    trace_mpm (TraceStream::Open(MIKTEX_TRACE_MPM)),
    url (lpszUrl)
{
  try
  {
    Initialize ();
    // connect to the server
    pSession->SendReceive ();
  }
  catch (const exception &)
  {
    if (initialized)
    {
      curl_multi_remove_handle (
	pSession->GetMultiHandle(),
	pSession->GetEasyHandle());
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

   CurlWebFile::Initialize
   _________________________________________________________________________ */

void
CurlWebFile::Initialize ()
{
  pSession->SetOption (CURLOPT_URL, url.c_str());
  pSession->SetOption (CURLOPT_WRITEDATA, reinterpret_cast<void*>(this));
  curl_write_callback writeCallback = WriteCallback;
  pSession->SetOption (CURLOPT_WRITEFUNCTION, writeCallback);
  CURLMcode code = curl_multi_add_handle(
    pSession->GetMultiHandle(),
    pSession->GetEasyHandle());
  if (code != CURLM_OK && code != CURLM_CALL_MULTI_PERFORM)
  {
    FATAL_MPM_ERROR ("CurlWebFile::Initialize",
      pSession->GetCurlErrorString(code).c_str(),
      0);
  }
  initialized = true;
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

   CurlWebFile::Read
   _________________________________________________________________________ */

size_t
CurlWebFile::Read (/*[out]*/ void *	pBuffer,
		   /*[in]*/ size_t	n)
{
  size_t oldBufferSize = buffer.size();
  clock_t now = clock();
  clock_t due = now + READ_TIMEOUT_SECONDS * CLOCKS_PER_SEC;
  do
  {
    pSession->Perform ();
  }
  while (buffer.size() < n && ! pSession->IsReady() && clock() < due);
  if (buffer.size() == oldBufferSize && ! pSession->IsReady())
  {
    FATAL_MPM_ERROR (
      "CurlWebFile::Read",
      T_("A timeout was reached while receiving data from the server."),
      0);
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
  if (initialized)
  {
    trace_mpm->WriteLine ("libmpm", T_("closing Web file"));
    initialized = false;
    CURLMcode code = curl_multi_remove_handle(
      pSession->GetMultiHandle(),
      pSession->GetEasyHandle());
    if (code != CURLM_OK)
    {
      FATAL_MPM_ERROR ("CurlWebFile::Close",
	pSession->GetCurlErrorString(code).c_str(),
	0);
    }
  }
  buffer.clear ();
}

#endif // libCURL
