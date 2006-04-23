/* appnames.cpp: managing application names

   Copyright (C) 1996-2006 Christian Schenk

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

/* _________________________________________________________________________

   SessionImpl::PushAppName
   _________________________________________________________________________ */

void
SessionImpl::PushAppName (/*[in]*/ const MIKTEXCHAR * lpszName)
{
  MIKTEX_ASSERT_STRING (lpszName);

  fileTypes.clear ();

  applicationNames.reserve (100);

  tstring newApplicationNames;

  newApplicationNames.reserve (applicationNames.length() + 40);

  newApplicationNames = lpszName;

  if (! applicationNames.empty())
    {
      newApplicationNames += PATH_DELIMITER;
      newApplicationNames += applicationNames;
    }

  applicationNames = newApplicationNames;
}

/* _________________________________________________________________________

   SessionImpl::PushBackAppName
   _________________________________________________________________________ */

void
SessionImpl::PushBackAppName (/*[in]*/ const MIKTEXCHAR * lpszName)
{
  MIKTEX_ASSERT_STRING (lpszName);

  fileTypes.clear ();

  applicationNames.reserve (100);

  if (! applicationNames.empty())
    {
      applicationNames += PATH_DELIMITER;
    }

  applicationNames += lpszName;
}
