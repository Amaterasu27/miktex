/* CommandLine.cpp: command-line builder

   Copyright (C) 1996-2007 Christian Schenk

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
  string str;
  string optionIndicator;
  string valueIndicator;
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

CommandLineBuilder::CommandLineBuilder (/*[in]*/ const char * lpszArg)
  : p (new Data)
{
  AppendArgument (lpszArg);
}

/* _________________________________________________________________________

   CommandLineBuilder::CommandLineBuilder
   _________________________________________________________________________ */

CommandLineBuilder::CommandLineBuilder (/*[in]*/ const char * lpszArg1,
					/*[in]*/ const char * lpszArg2)
  : p (new Data)
{
  AppendArgument (lpszArg1);
  AppendArgument (lpszArg2);
}

/* _________________________________________________________________________

   CommandLineBuilder::CommandLineBuilder
   _________________________________________________________________________ */

CommandLineBuilder::CommandLineBuilder (/*[in]*/ const char * lpszArg1,
					/*[in]*/ const char * lpszArg2,
					/*[in]*/ const char * lpszArg3)
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
      pData->optionIndicator = "";
      pData->valueIndicator = "";
      break;
    case OptionConvention::Xt:
      pData->optionIndicator = "-";
      pData->valueIndicator = "=";
      break;
    case OptionConvention::GNU:
      pData->optionIndicator = "--";
      pData->valueIndicator = "=";
      break;
    case OptionConvention::DOS:
      pData->optionIndicator = "/";
      pData->valueIndicator = ":";
      break;
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::Clear
   _________________________________________________________________________ */

void
CommandLineBuilder::Clear ()
{
  pData->str = "";
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendUnquoted
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendUnquoted (/*[in]*/ const char * lpszText)
{
  if (! pData->str.empty())
    {
      pData->str += ' ';
    }
  pData->str += lpszText;
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendArgument
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendArgument (/*[in]*/ const char * lpszArgument)
{
  if (! pData->str.empty())
    {
      pData->str += ' ';
    }
  bool needsQuoting = (*lpszArgument == 0 || StrChr(lpszArgument, ' ') != 0);
  if (needsQuoting)
    {
      pData->str += '"';
    }
  pData->str += lpszArgument;
  if (needsQuoting)
    {
      pData->str += '"';
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendArguments
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendArguments (/*[in]*/ int			argc,
				     /*[in]*/ const char * const *	argv)
{
  for (int idx = 0; idx < argc; ++ idx)
    {
      AppendArgument (argv[idx]);
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendArguments
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendArguments (/*[in]*/ const vector<string> &	argv)
{
  for (vector<string>::const_iterator it = argv.begin();
       it != argv.end();
       ++ it)
    {
      AppendArgument (it->c_str());
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendArguments
   _________________________________________________________________________ */

void
MIKTEXCALL
CommandLineBuilder::AppendArguments (/*[in]*/ const Argv & argv)
{
  if (argv.GetArgc() > 0)
    {
      AppendArguments (argv.GetArgc() - 1, argv.GetArgv() + 1);
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendOption
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendOption (/*[in]*/ const char * lpszOption,
				  /*[in]*/ const char * lpszValue)
{
  if (! pData->str.empty())
    {
      pData->str += ' ';
    }
  pData->str += pData->optionIndicator;
  pData->str += lpszOption;
  if (lpszValue != 0)
    {
      pData->str += pData->valueIndicator;
      bool needsQuoting =
	(*lpszValue == 0 || StrChr(lpszValue, ' ') != 0);
      if (needsQuoting)
	{
	  pData->str += '"';
	}
      pData->str += lpszValue;
      if (needsQuoting)
	{
	  pData->str += '"';
	}
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendStdoutRedirection
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendStdoutRedirection (/*[in]*/ const char * lpszPath,
					     /*[in]*/ bool	append)
{
  pData->str += '>';
  if (append)
    {
      pData->str += '>';
    }
#if defined(MIKTEX_WINDOWS)
  PathName dosish (lpszPath);
  lpszPath = dosish.ToDos().Get();
#endif
  bool needsQuoting = (StrChr(lpszPath, ' ') != 0);
  if (needsQuoting)
    {
      pData->str += '"';
    }
  pData->str += lpszPath;
  if (needsQuoting)
    {
      pData->str += '"';
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::AppendStdinRedirection
   _________________________________________________________________________ */

void
CommandLineBuilder::AppendStdinRedirection (/*[in]*/ const char * lpszPath)
{
  pData->str += '<';
#if defined(MIKTEX_WINDOWS)
  PathName dosish (lpszPath);
  lpszPath = dosish.ToDos().Get();
#endif
  bool needsQuoting = (StrChr(lpszPath, ' ') != 0);
  if (needsQuoting)
    {
      pData->str += '"';
    }
  pData->str += lpszPath;
  if (needsQuoting)
    {
      pData->str += '"';
    }
}

/* _________________________________________________________________________

   CommandLineBuilder::Get
   _________________________________________________________________________ */

const char *
CommandLineBuilder::Get ()
{
  return (pData->str.c_str());
}
