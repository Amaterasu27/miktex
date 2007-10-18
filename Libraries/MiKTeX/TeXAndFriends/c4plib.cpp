/* c4plib.cpp: C4P runtime routines

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX TeXMF Library.

   The MiKTeX TeXMF Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX TeXMF Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX TeXMF Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#define C4P_BEGIN_NAMESPACE namespace C4P {
#define C4P_END_NAMESPACE }

C4P_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   DiscardLine
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
DiscardLine (/*[in]*/ C4P_text & textfile)
{
  MIKTEX_API_BEGIN ("DiscardLine");
  textfile.AssertValid ();
  // <fixme>macintosh</fixme>
  while (! feof(textfile) && GetChar(textfile) != T_('\n'))
    {
      ;
    }
  MIKTEX_API_END ("DiscardLine");
}

/* _________________________________________________________________________

   GetChar
   _________________________________________________________________________ */

MIKTEXMFAPI(char)
GetChar (/*[in]*/ C4P_text & textfile)
{
  MIKTEX_API_BEGIN ("GetChar");
  textfile.AssertValid ();
  char ch = *textfile;
  int ch2 = GetC(textfile);
  if (ch2 != EOF)
    {
      *textfile = static_cast<char>(ch2);
    }
  return (ch);
  MIKTEX_API_END ("GetChar");
}

/* _________________________________________________________________________

   GetInteger
   _________________________________________________________________________ */

MIKTEXMFAPI(C4P_integer)
GetInteger (/*[in]*/ C4P_text & textfile)
{
  MIKTEX_API_BEGIN ("GetInteger");
  textfile.AssertValid ();
  int ch = GetChar(textfile);
  int sign = (ch == T_('-') ? -1 : 1);
  C4P_integer result = 0;
  if (ch == T_('+') || ch == T_('-'))
    {
      ch = GetChar(textfile);
    }
  while (isdigit(ch))
    {
      result *= 10;
      result += (ch - T_('0'));
      ch = GetChar(textfile);
    }
  return (result * sign);
  MIKTEX_API_END ("GetInteger");
}

/* _________________________________________________________________________

   GetReal
   _________________________________________________________________________ */

MIKTEXMFAPI(C4P_real)
GetReal (/*[in]*/ C4P_text & /*textfile*/)
{
  MIKTEX_API_BEGIN ("GetReal");
  UNIMPLEMENTED (T_("GetReal"));
  MIKTEX_API_END ("GetReal");
}

/* _________________________________________________________________________

   FileRoot::open
   _________________________________________________________________________ */

MIKTEXMFAPI(bool)
FileRoot::Open (/*[in]*/ LPCTSTR	lpszPath,
		/*[in]*/ FileMode	mode,
		/*[in]*/ FileAccess	access,
		/*[in]*/ FileShare	share,
		/*[in]*/ bool		text,
		/*[in]*/ bool		mustExist)
{
  MIKTEX_API_BEGIN ("FileRoot::open");
  MIKTEX_ASSERT_STRING (lpszPath);
  FILE * pFile;
  if (mustExist)
    {
      pFile =
	SessionWrapper(true)->OpenFile(lpszPath,
				       mode,
				       access,
				       text,
				       share);
    }
  else
    {
      pFile =
	SessionWrapper(true)->TryOpenFile(lpszPath,
					  mode,
					  access,
					  text,
					  share);
      if (pFile == 0)
	{
	  return (false);
	}
    }
  Attach (pFile, true);
  return (true);
  MIKTEX_API_END ("FileRoot::open");
}

/* _________________________________________________________________________

   Round
   _________________________________________________________________________ */

MIKTEXMFAPI(C4P_integer)
Round (/*[in]*/ double r)
{
  MIKTEX_API_BEGIN ("Round");
  if (r > INT_MAX)
    {
      return (INT_MAX);
    }
  else if (r < - INT_MAX)
    {
      return (- INT_MAX);
    }
  else if (r >= 0.0)
    {
      return (static_cast<C4P_integer>(r + 0.5));
    }
  else
    {
      return (static_cast<C4P_integer>(r - 0.5));
    }
  MIKTEX_API_END ("Round");
}

C4P_END_NAMESPACE;
