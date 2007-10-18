/* getdelim.cpp: reading input lines

   Copyright (C) 1996-2006 Christian Schenk
   Copyright (C) Free Software Foundation

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
Utils::ReadUntilDelim (/*[out]*/ string &	str,
		       /*[in]*/ int		delim,
		       /*[in]*/ FILE *		stream)
{
  if (delim == '\n')
    {
      // special case
      return (ReadLine(str, stream, true));
    }
  else
    {
      str = "";
      if (feof(stream) != 0)
	{
	  return (false);
	}
      int ch;
      while ((ch = GetC(stream)) != EOF)
	{
	  str += static_cast<char>(ch);
	  if (ch == delim)
	    {
	      return (true);
	    }
	}
      return (ch == EOF ? ! str.empty() : true);
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
Utils::ReadLine (/*[out]*/ string &	str,
		 /*[in]*/ FILE *	stream,
		 /*[in]*/ bool		keepLineEnding)
{
  str = "";
  if (feof(stream) != 0)
    {
      return (false);
    }
  int ch;
  while ((ch = GetC(stream)) != EOF)
    {
      if (ch == '\r')
	{
	  if (keepLineEnding)
	    {
	      str += '\r';
	    }
	  ch = GetC(stream);
	  if (ch == '\n')
	    {
	      if (keepLineEnding)
		{
		  str += '\n';
		}
	    }
	  else if (ch != EOF)
	    {
	      UnGetC (ch, stream);
	    }
	  return (true);
	}
      else if (ch == '\n')
	{
	  if (keepLineEnding)
	    {
	      str += '\n';
	    }
	  return (true);
	}
      else
	{
	  str += static_cast<char>(ch);
	}
    }
  return (ch == EOF ? ! str.empty() : true);
}
