/* Uri.cpp:

   Copyright (C) 2008 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

inline
string
ToString (/*[in]*/ const UriTextRangeA & textRange)
{
  string ret;
  if (textRange.first == 0)
    {
      return (ret);
    }
  ret.assign (textRange.first, textRange.afterLast - textRange.first);
  return (ret);
}

/* _________________________________________________________________________

   Data
   _________________________________________________________________________ */

struct Data
{
  UriParserStateA state;
  UriUriA uri;

  ~Data ()
  {
    uriFreeUriMembersA (&uri);
  }
};

#define pData reinterpret_cast<Data*>(this->p)

/* _________________________________________________________________________

   Uri::Uri
   _________________________________________________________________________ */

Uri::Uri ()
  : p (0)
{
}

/* _________________________________________________________________________

   Uri::Uri
   _________________________________________________________________________ */

Uri::Uri (/*[in]*/ const Uri & other)
  : p (0)
{
  this->operator= (other);
}

/* _________________________________________________________________________

   Uri::Uri
   _________________________________________________________________________ */

Uri::Uri (/*[in]*/ const char * lpszUri)
  : p (new Data)
{
  pData->state.uri = &pData->uri;
  int result = uriParseUriA(&pData->state, lpszUri);
  if (result == URI_ERROR_SYNTAX)
    {
      string uri = "http://";
      uri += lpszUri;
      result = uriParseUriA(&pData->state, uri.c_str());
    }
  if (result != URI_SUCCESS)
    {
      delete pData;
      p = 0;
      FATAL_MIKTEX_ERROR ("Uri::Uri", T_("Bad URI."), lpszUri);
    }
}

/* _________________________________________________________________________

   Uri::~Uri
   _________________________________________________________________________ */

Uri::~Uri ()
{
  try
    {
      if (pData != 0)
	{
	  delete pData;
	  p = 0;
	}
    }
  catch (const exception &)
    {
    }
  p = 0;
}

/* _________________________________________________________________________

   Uri::operator=
   _________________________________________________________________________ */

Uri &
Uri::operator= (/*[in]*/ const Uri & other)
{
  // todo
  return (*this);
}

/* _________________________________________________________________________

   Uri::GetScheme
   _________________________________________________________________________ */

string
Uri::GetScheme ()
{
  return (ToString(pData->uri.scheme));
}

/* _________________________________________________________________________

   Uri::GetHost
   _________________________________________________________________________ */

string
Uri::GetHost ()
{
  return (ToString(pData->uri.hostText));
}

/* _________________________________________________________________________

   Uri::GetPort
   _________________________________________________________________________ */

int
Uri::GetPort ()
{
  string portText = ToString(pData->uri.portText);
  if (! portText.empty())
    {
      return (atoi(portText.c_str()));
    }
  string scheme = GetScheme();
  if (scheme == "http")
    {
      return (80);
    }
  else if (scheme == "ftp")
    {
      return (23);
    }
  return (-1);
}

/* _________________________________________________________________________

   Uri::GetUserInfo
   _________________________________________________________________________ */

string
Uri::GetUserInfo ()
{
  return (ToString(pData->uri.userInfo));
}

