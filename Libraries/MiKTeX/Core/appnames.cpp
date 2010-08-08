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

inline
void
Append (/*[in,out]*/ string & str,
	/*[in]*/ const char * lpszTag)
{
  if (! str.empty())
  {
    str += PATH_DELIMITER;
  }
  str += lpszTag;
}

/* _________________________________________________________________________

   SessionImpl::PushAppName
   _________________________________________________________________________ */

void
SessionImpl::PushAppName (/*[in]*/ const char * lpszName)
{
  MIKTEX_ASSERT_STRING (lpszName);
  MIKTEX_ASSERT (strchr(lpszName, PATH_DELIMITER) == 0);

  string newApplicationNames;

  newApplicationNames.reserve (applicationNames.length() + 40);

  newApplicationNames = lpszName;

  for (CSVList tag (applicationNames.c_str(), PATH_DELIMITER); tag.GetCurrent() != 0; ++ tag)
  {
    // stop at the miktex application tag; this is always the last tag
    if (StringCompare(tag.GetCurrent(), "miktex", true) == 0)
    {
#if defined(MIKTEX_DEBUG)
      ++tag;
      MIKTEX_ASSERT (tag.GetCurrent() == 0);
#endif
      break;
    }
    if (StringCompare(tag.GetCurrent(), lpszName, true) == 0)
    {
      continue;
    }
    Append (newApplicationNames, tag.GetCurrent());
  }

  Append (newApplicationNames, "miktex");

  if (StringCompare(newApplicationNames.c_str(), applicationNames.c_str(), true) == 0)
  {
    return;
  }

  fileTypes.clear ();

  applicationNames = newApplicationNames;

  trace_config->WriteFormattedLine ("core",
				    T_("application tags: %s"),
				    applicationNames.c_str());
}

/* _________________________________________________________________________

   SessionImpl::PushBackAppName
   _________________________________________________________________________ */

void
SessionImpl::PushBackAppName (/*[in]*/ const char * lpszName)
{
  MIKTEX_ASSERT_STRING (lpszName);
  MIKTEX_ASSERT (strchr(lpszName, PATH_DELIMITER) == 0);

  fileTypes.clear ();

  string newApplicationNames;

  for (CSVList tag (applicationNames.c_str(), PATH_DELIMITER); tag.GetCurrent() != 0; ++ tag)
  {
    // stop at the miktex application tag; this is always the last tag
    if (StringCompare(tag.GetCurrent(), "miktex", true) == 0)
    {
#if defined(MIKTEX_DEBUG)
      ++tag;
      MIKTEX_ASSERT (tag.GetCurrent() == 0);
#endif
      break;
    }
    if (StringCompare(tag.GetCurrent(), lpszName, true) == 0)
    {
      continue;
    }
    Append (newApplicationNames, tag.GetCurrent());
  }

  Append (newApplicationNames, lpszName);

  Append (newApplicationNames, "miktex");

  if (StringCompare(newApplicationNames.c_str(), applicationNames.c_str(), true) == 0)
  {
    return;
  }

  applicationNames = newApplicationNames;

  trace_config->WriteFormattedLine ("core",
				    T_("application tags: %s"),
				    applicationNames.c_str());
}
