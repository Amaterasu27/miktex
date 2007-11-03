/* dviscan.cpp: test driver for the DVI interfaces

   Copyright (C) 1996-2007 Christian Schenk

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

#include <MiKTeX/App/Application>
#include <MiKTeX/Core/Text>
#include <MiKTeX/Core/Core>
#include <MiKTeX/DVI/Dvi>
#include <popt-miktex.h>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX::DVI;
using namespace MiKTeX;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

enum {
  OPT_AAA = 1234,
  OPT_TRACE,
  OPT_PAGE_MODE
};

static const struct poptOption long_options[] = {
  {
    "page-mode", 0, POPT_ARG_STRING, 0, OPT_PAGE_MODE,
    "Sets the DVI page mode.",
    "PAGEMODE"
  },
  {
    "trace", 0, POPT_ARG_STRING, 0, OPT_TRACE,
    "Turn on tracing.",
    "TRACESTREAMS"
  },
  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   DviScanner
   _________________________________________________________________________ */

class DviScanner
  : public Application
{
public:
  DviScanner ()
    : pageMode (DviPageMode::Auto)
  {
  }

public:
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv);

private:
  DviPageMode pageMode;
};

/* _________________________________________________________________________

   DviScanner::Run
   _________________________________________________________________________ */

void
DviScanner::Run (/*[in]*/ int		argc,
		 /*[in]*/ const char **	argv)
{
  Session::InitInfo initInfo (argv[0]);

  string metafontMode ("ljfour");
  int dpi = 600;

  Cpopt popt (argc, argv, long_options);

  popt.SetOtherOptionHelp (T_("[OPTION...] DVIFILE..."));

  int option;

  while ((option = popt.GetNextOpt()) >= 0)
    {
      const char * lpszOptArg = popt.GetOptArg();
      switch (option)
	{
	case OPT_PAGE_MODE:
	  if (StringCompare(lpszOptArg, "pk") == 0)
	    {
	      pageMode = DviPageMode::Pk;
	    }
	  else if (StringCompare(lpszOptArg, "dvips") == 0)
	    {
	      pageMode = DviPageMode::Dvips;
	    }
	  else if (StringCompare(lpszOptArg, "auto") == 0)
	    {
	      pageMode = DviPageMode::Auto;
	    }
	  else
	    {
	      throw T_("invalid page mode");
	    }
	  break;
	case OPT_TRACE:
	  initInfo.SetTraceFlags (lpszOptArg);
	  break;
	}
    }

  if (option < -1)
    {
      fprintf (stderr, "%s: %s\n",
	       popt.BadOption(POPT_BADOPTION_NOALIAS),
	       popt.Strerror(option));
      throw (1);
    }

  const char ** leftovers = popt.GetArgs();

  if (leftovers == 0)
    {
      fprintf (stderr, "missing file name argument\n");
      throw (1);
    }

  Init (initInfo);

  for (; *leftovers != 0; ++ leftovers)
    {
      const char * lpszDviFileName = *leftovers;
      auto_ptr<Dvi> pDvi
	(Dvi::Create(lpszDviFileName,
		     metafontMode.c_str(),
		     dpi,
		     5,
		     DviAccess::Sequential,
		     pageMode,
		     pSession->GetPaperSizeInfo("A4size"),
		     false,
		     0));
      pDvi->Scan ();
      for (int i = 0; i < pDvi->GetNumberOfPages(); ++ i)
	{
	  DviPage * pPage = pDvi->GetLoadedPage(i);
	  if (pPage == 0)
	    {
	      break;
	    }
	  for (int j = 0; j < pPage->GetNumberOfDviBitmaps(5); ++ j)
	    {
	      pPage->GetDviBitmap (5, j);
	    }
	  pPage->Unlock ();
	}
      pDvi->Dispose ();
      pDvi.reset ();
    }

  Finalize ();
}


/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)

{
  try
    {
      DviScanner app;
      app.Run (argc, argv);
      return (0);
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (const char *	lpszMessage)
    {
      fprintf (stderr, T_("fatal error: %s\n"), lpszMessage);
      return (1);
    }
  catch (int retCode)
    {
      return (retCode);
    }
}
