/* internal.h:							-*- C++ -*-

   Copyright (C) 2002-2007 Christian Schenk

   This file is part of MkFntMap.

   MkFntMap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MkFntMap is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MkFntMap; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <set>
#include <string>

#include <MiKTeX/Core/Core>

using namespace MiKTeX::Core;

#define UNUSED_ALWAYS(x) (x)

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)
#define Q_(x) Quoted(x).c_str()
#define BOOLSTR(b) ((b) ? T_("true") : T_("false"))

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
std::string
Quoted (/*[in]*/ const char * lpsz)
{
  bool needQuotes = (strchr(lpsz, T_(' ')) != 0);
  std::string result;
  if (needQuotes)
    {
      result += T_('"');
    }
  result += lpsz;
  if (needQuotes)
    {
      result += T_('"');
    }
  return (result);
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
std::string
Quoted (/*[in]*/ const std::string & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
std::string
Quoted (/*[in]*/ const PathName & path)
{
  return (Quoted(path.Get()));
}
