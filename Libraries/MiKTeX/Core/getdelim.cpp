/* getdelim.cpp: reading input lines

   Copyright (C) Free Software Foundation
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

// derived from getdelim() (Copyright (C) FSF)

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   Utils::ReadUntilDelim
   _________________________________________________________________________ */

bool
MIKTEXCALL
Utils::ReadUntilDelim (/*[out]*/ tstring &	str,
		       /*[in]*/ MIKTEXCHARINT	delim,
		       /*[in]*/ FILE *		stream)
{
  if (delim == T_('\n'))
    {
      // special case
      return (ReadLine(str, stream, true));
    }
  else
    {
      str = T_("");
      if (feof(stream) != 0)
	{
	  return (false);
	}
      MIKTEXCHARINT ch;
      while ((ch = GetC(stream)) != MIKTEXEOF)
	{
	  str += static_cast<MIKTEXCHAR>(ch);
	  if (ch == delim)
	    {
	      return (true);
	    }
	}
      return (ch == MIKTEXEOF ? ! str.empty() : true);
    }
}

/* _________________________________________________________________________

   Utils::ReadLine

   stuff\r      (Mac)
   stuff\r\n    (DOS)
   stuff\n      (Unx)
   _________________________________________________________________________ */

bool
MIKTEXCALL
Utils::ReadLine (/*[out]*/ tstring &	str,
		 /*[in]*/ FILE *	stream,
		 /*[in]*/ bool		keepLineEnding)
{
  str = T_("");
  if (feof(stream) != 0)
    {
      return (false);
    }
  MIKTEXCHARINT ch;
  while ((ch = GetC(stream)) != MIKTEXEOF)
    {
      if (ch == T_('\r'))
	{
	  if (keepLineEnding)
	    {
	      str += T_('\r');
	    }
	  ch = GetC(stream);
	  if (ch == T_('\n'))
	    {
	      if (keepLineEnding)
		{
		  str += T_('\n');
		}
	    }
	  else if (ch != MIKTEXEOF)
	    {
	      UnGetC (ch, stream);
	    }
	  return (true);
	}
      else if (ch == T_('\n'))
	{
	  if (keepLineEnding)
	    {
	      str += T_('\n');
	    }
	  return (true);
	}
      else
	{
	  str += static_cast<MIKTEXCHAR>(ch);
	}
    }
  return (ch == MIKTEXEOF ? ! str.empty() : true);
}
