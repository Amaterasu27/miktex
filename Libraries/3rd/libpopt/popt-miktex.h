/* popt-miktex.h: popt library wrapper			-*- C++ -*-

   Copyright (C) 2001-2005 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this file; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(MIKTEX__65AE8D66_F021_47CC_9D6B_12D518D15170__)
#define MIKTEX__65AE8D66_F021_47CC_9D6B_12D518D15170__

#include <popt.h>

class Cpopt
{
public:
  Cpopt ()
    : m_poptContext (0)
  {
  }

public:
  operator poptContext ()
    const
  {
    return (m_poptContext);
  }

public:
  operator poptContext ()
  {
    return (m_poptContext);
  }

public:
  Cpopt (/*[in]*/ const char *			lpszName,
	 /*[in]*/ int				argc,
	 /*[in]*/ const char **			argv,
	 /*[in]*/ const struct poptOption *	options,
	 /*[in]*/ int				flags = 0)
  {
    Construct (lpszName, argc, argv, options, flags);
  }

public:
  Cpopt (/*[in]*/ int				argc,
	 /*[in]*/ const char **			argv,
	 /*[in]*/ const struct poptOption *	options,
	 /*[in]*/ int				flags = 0)
  {
    Construct (argc, argv, options, flags);
  }

public:
  void
  Dispose ()
  {
    if (m_poptContext != 0)
      {
	poptFreeContext (m_poptContext);
	m_poptContext = 0;
      }
  }

public:
  ~Cpopt ()
  {
    Dispose ();
  }

public:
  void
  Construct (/*[in]*/ const char *		lpszName,
	     /*[in]*/ int			argc,
	     /*[in]*/ const char **		argv,
	     /*[in]*/ const struct poptOption *	options,
	     /*[in]*/ int			flags = 0)
  {
    m_poptContext = poptGetContext(lpszName, argc, argv, options, flags);
  }

public:
  void
  Construct (/*[in]*/ int			argc,
	     /*[in]*/ const char **		argv,
	     /*[in]*/ const struct poptOption *	options,
	     /*[in]*/ int			flags = 0)
  {
    Construct (0, argc, argv, options, flags);
  }
	 
public:
  void
  SetOtherOptionHelp (/*[in]*/ const char *	lpsz)
  {
    poptSetOtherOptionHelp (m_poptContext, lpsz);
  }

public:
  void
  PrintUsage (/*[in]*/ FILE *	pstream = stdout,
	      /*[in]*/ int	flags = 0)
    const
  {
    poptPrintUsage (m_poptContext, pstream, flags);
  }

public:
  void
  PrintHelp (/*[in]*/ FILE *	pstream = stdout,
	     /*[in]*/ int	flags = 0)
    const
  {
    poptPrintHelp (m_poptContext, pstream, flags);
  }

public:
  int
  GetNextOpt ()
  {
    return (poptGetNextOpt(m_poptContext));
  }

public:
  const char *
  GetOptArg ()
  {
    return (poptGetOptArg(m_poptContext));
  }

public:
  const char **
  GetArgs ()
  {
    return (poptGetArgs(m_poptContext));
  }

public:
  int
  GetArgCount ()
  {
    int count = 0;
    for (const char ** leftovers = GetArgs();
	 leftovers != 0 && *leftovers != 0;
	 ++ leftovers)
      {
	++ count;
      }
    return (count);
  }

public:
  const char *
  BadOption (/*[in]*/ int flags)
  {
    return (poptBadOption(m_poptContext, flags));
  }

public:
  static
  const char *
  Strerror (/*[in]*/ int error)
  {
    return (/*[in]*/ poptStrerror(error));
  }

private:
  poptContext m_poptContext;
};

#endif
