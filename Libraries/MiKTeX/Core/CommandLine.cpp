/* CommandLine.cpp: command-line builder

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

   Data
   _________________________________________________________________________ */

struct Data
{
  tstring str;
  tstring optionIndicator;
  tstring valueIndicator;
};

#define pData reinterpret_cast<Data*>(this->p)

/* _________________________________________________________________________

   CommandLineBuilder::CommandLineBuilder
   _________________________________________________________________________ */

CommandLineBuilder::CommandLineBuilder ()
  : p (new Data)
{
  SetOptionConvention (OptionConvention::None);
}

/* _________________________________________________________________________

   CommandLineBuilder::CommandLineBuilder
   _________________________________________________________________________ */

CommandLineBuilder::CommandLineBuilder
(/*[in]*/ const CommandLineBuilder & other)
  : p (new Data)
{
  this->operator= (other);
}

/* _________________________________________________________________________

   CommandLineBuilder::CommandLineBuilder
   _________________________________________________________________________ */

CommandLineBuilder::CommandLineBuilder (/*[in]*/ const MIKTEXCHAR * lpszArg)
  : p (new Data)
{
  AppendArgument (lpszArg);
}

/* _________________________________________________________________________

   CommandLineBuilder::CommandLineBuilder
   _________________________________________________________________________ */

CommandLineBuilder::CommandLineBuilder (/*[in]*/ const MIKTEXCHAR * lpszArg1,
					/*[in]*/ const MIKTEXCHAR * lpszArg2)
  : p (new Data)
{
  AppendArgument (lpszArg1);
  AppendArgument (lpszArg2);
}

/* _________________________________________________________________________

   CommandLineBuilder::CommandLineBuilder
   _________________________________________________________________________ */

CommandLineBuilder::CommandLineBuilder (/*[in]*/ const MIKTEXCHAR * lpszArg1,
					/*[in]*/ const MIKTEXCHAR * lpszArg2,
					/*[in]*/ const MIKTEXCHAR * lpszArg3)
  : p (new Data)
{
  AppendArgument (lpszArg1);
  AppendArgument (lpszArg2);
  AppendArgument (lpszArg3);
}

/* _________________________________________________________________________

   CommandLineBuilder::~CommandLineBuilder
   _________________________________________________________________________ */

CommandLineBuilder::~CommandLineBuilder ()
{
  try
    {
      delete pData;
    }
  catch (const exception &)
    {
    }
  p = 0;
}

/* _________________________________________________________________________

   CommandLineBuilder::operator=
   _________________________________________________________________________ */

CommandLineBuilder &
CommandLineBuilder::operator= (/*[in]*/ const CommandLineBuilder & other)
{
  *pData = *reinterpret_cast<const Data *>(other.p);
  return (*this);
}

/* _________________________________________________________________________

   CommandLineBuilder::SetOptionConvention
   _________________________________________________________________________ */

void
CommandLineBuilder::SetOptionConvention
(/*[in]*/ OptionConvention optionConvention)
{
  switch (optionConvention.Get())
    {
    case OptionConvention::None:
      pData->optionIndicator = T_("");
      pData->valueIndicator = T_("");
      break;
    case OptionConvention::Xt:
      pData->optionIndicator = T_("-");
      pData->valueIndicator = T_("=");
      break;
    case OptionConvention::GNU:
      pData->optionIndicator = T_("--");
      pData->valueIndicator = T_("=");
      break;
    case OptionConvention::DOS:
      pData->optionIndicator = T_("/");
      pData->valueIndicator = T_(":");
      break;
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::Clear
   _________________________________________________________________________ */

void
CommandLineBuilder::Clear ()
{
  pData->str = T_("");
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendUnquoted
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendUnquoted (/*[in]*/ const MIKTEXCHAR * lpszText)
{
  if (pData->str.length() > 0)
    {
      pData->str += T_(' ');
    }
  pData->str += lpszText;
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendArgument
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendArgument (/*[in]*/ const MIKTEXCHAR * lpszArgument)
{
  if (! pData->str.empty())
    {
      pData->str += T_(' ');
    }
  bool needsQuoting =
    (*lpszArgument == 0 || StrChr(lpszArgument, T_(' ')) != 0);
  if (needsQuoting)
    {
      pData->str += T_('"');
    }
  pData->str += lpszArgument;
  if (needsQuoting)
    {
      pData->str += T_('"');
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendArguments
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendArguments (/*[in]*/ int			argc,
				     /*[in]*/ const MIKTEXCHAR * const * argv)
{
  for (int i = 0; i < argc; ++ i)
    {
      AppendArgument (argv[i]);
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendArguments
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendArguments (/*[in]*/ const vector<tstring>	argv)
{
  for (vector<tstring>::const_iterator it = argv.begin();
       it != argv.end();
       ++ it)
    {
      AppendArgument (it->c_str());
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendOption
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendOption (/*[in]*/ const MIKTEXCHAR * lpszOption,
				  /*[in]*/ const MIKTEXCHAR * lpszValue)
{
  if (! pData->str.empty())
    {
      pData->str += T_(' ');
    }
  pData->str += pData->optionIndicator;
  pData->str += lpszOption;
  if (lpszValue != 0)
    {
      pData->str += pData->valueIndicator;
      bool needsQuoting =
	(*lpszValue == 0 || StrChr(lpszValue, T_(' ')) != 0);
      if (needsQuoting)
	{
	  pData->str += T_('"');
	}
      pData->str += lpszValue;
      if (needsQuoting)
	{
	  pData->str += T_('"');
	}
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendStdoutRedirection
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendStdoutRedirection
(/*[in]*/ const MIKTEXCHAR *	lpszPath,
 /*[in]*/ bool			append)
{
  pData->str += T_('>');
  if (append)
    {
      pData->str += T_('>');
    }
#if defined(MIKTEX_WINDOWS)
  PathName dosish (lpszPath);
  lpszPath = dosish.ToDos().Get();
#endif
  bool needsQuoting = (StrChr(lpszPath, T_(' ')) != 0);
  if (needsQuoting)
    {
      pData->str += T_('"');
    }
  pData->str += lpszPath;
  if (needsQuoting)
    {
      pData->str += T_('"');
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendStdinRedirection
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendStdinRedirection
(/*[in]*/ const MIKTEXCHAR *	lpszPath)
{
  pData->str += T_('<');
#if defined(MIKTEX_WINDOWS)
  PathName dosish (lpszPath);
  lpszPath = dosish.ToDos().Get();
#endif
  bool needsQuoting = (StrChr(lpszPath, T_(' ')) != 0);
  if (needsQuoting)
    {
      pData->str += T_('"');
    }
  pData->str += lpszPath;
  if (needsQuoting)
    {
      pData->str += T_('"');
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::Get
   _________________________________________________________________________ */

const MIKTEXCHAR *
CommandLineBuilder::Get ()
{
  return (pData->str.c_str());
}
