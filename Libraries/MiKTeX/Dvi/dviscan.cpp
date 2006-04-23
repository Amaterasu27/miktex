/* dviscan.cpp: test driver for the DVI interfaces

   Copyright (C) 1996-2006 Christian Schenk

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <miktex/char.h>
#include <miktex/core.h>
#include <miktex/dvi.h>
#include <popt-miktex.h>

using namespace MiKTeX::Core;
using namespace MiKTeX::DVI;
using namespace MiKTeX;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

enum {
 OPT_LOGFILTER = 1
};

static const struct poptOption long_options[] = {
  { T_("logfilter"), 0, POPT_ARG_STRING, 0, OPT_LOGFILTER,
    T_("Comma-separated list of filter options."),
    T_("OPTIONS")
  },
  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR **	argv)

{
  SessionWrapper pSession (true);

  string metafontMode (T_("ljfour"));
  int dpi = 600;

  Cpopt popt (argc, argv, long_options);
  popt.SetOtherOptionHelp (T_("[OPTION...] [DIR...]"));
  int option;
  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{
	case OPT_LOGFILTER:
	  TraceStream::SetTraceFlags (popt.GetOptArg());
	  break;
	}
    }

  if (option < -1)
    {
      fprintf (stderr, "%s: %s\n",
	       popt.BadOption(POPT_BADOPTION_NOALIAS),
	       popt.Strerror(option));
      return (1);
    }

  const MIKTEXCHAR ** leftovers = popt.GetArgs();

  if (leftovers == 0)
    {
      fprintf (stderr, "missing file name argument\n");
      return (1);
    }

  int ret = 0;

  for (; *leftovers != 0; ++ leftovers)
    {
      const MIKTEXCHAR * lpszDviFileName = *leftovers;
      try
	{
	  auto_ptr<Dvi> pDvi (Dvi::Create(lpszDviFileName,
					  metafontMode.c_str(),
					  dpi,
					  5,
					  DviAccess::Sequential,
					  0));
	  for (int i = 0; i < pDvi->GetNumberOfPages(); ++ i)
	    {
	      DviPage * pPage = pDvi->GetLoadedPage(i);
	      if (pPage == 0)
		{
		  break;
		}
	      for (int j = 0; j < pPage->GetNumberOfDviBitmaps(5); ++ j)
		{
		  /*const DviBitmap & bitmap =*/ pPage->GetDviBitmap(5, j);
		}
	      pPage->Unlock ();
	    }
	  pDvi->Dispose ();
	  pDvi.reset ();
	}
      catch (const exception &)
	{
	  ret = 1;
	}
    }

  return (ret);
}
