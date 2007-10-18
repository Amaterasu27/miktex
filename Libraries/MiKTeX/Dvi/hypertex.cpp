/* hypertex.cpp: html specials

   Copyright (C) 1996-2007 Christian Schenk

   This file is part of the MiKTeX DVI Library.

   The MiKTeX DVI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX DVI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the MiKTeX DVI Library; if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
   USA.  */

#include "StdAfx.h"

#include "common.h"

HyperTeXSpecialImpl::State HyperTeXSpecialImpl::state;

/* _________________________________________________________________________

   DviImpl::ProcessHtmlSpecial
   _________________________________________________________________________ */

DviSpecial *
DviImpl::ProcessHtmlSpecial (/*[in]*/ DviPageImpl *	ppage,
			     /*[in]*/ int		x,
			     /*[in]*/ int		y,
			     /*[in]*/ const char *	lpszSpecial)
{
  MIKTEX_ASSERT (strncmp(lpszSpecial, "html:", 5) == 0);
  const char * lpsz = lpszSpecial + 5;
  while (isspace(*lpsz))
    {
      ++ lpsz;
    }
  if (*lpsz++ != '<')
    {
      log_error->WriteFormattedLine
	(T_("libdvi"),
	 T_("bad html special: %s"),
	 lpszSpecial);
      return (0);
    }
  if (*lpsz == '/' && tolower(lpsz[1]) == 'a' && lpsz[2] == '>')
    {
      return (new DviSpecialObject<HyperTeXSpecialImpl> (ppage,
							  x,
							  y,
							  lpszSpecial));
    }
  else if (_strnicmp(lpsz, "img", 3) == 0
	   && (lpsz[3] == ' ' || lpsz[3] == '\t'))
    {
      log_error->WriteLine (T_("libdvi"), T_("img not yet supported"));
      return (0);
    }
  else if ((_strnicmp(lpsz, "base", 4) == 0
	    && (lpsz[4] == ' ' || lpsz[4] == '\t'))
	   || tolower(*lpsz) == 'a' && (lpsz[1] == ' ' || lpsz[1] == '\t'))
    {
      bool isBaseUrl = (tolower(*lpsz) == 'b');
      if (isBaseUrl)
	{
	  lpsz += 4;
	}
      else
	{
	  ++ lpsz;
	}
      while (isspace(*lpsz))
	{
	  ++ lpsz;
	}
      HyperTeXSpecialImpl::state.isName = false;
      HyperTeXSpecialImpl::state.isHref = false;
      if (_strnicmp(lpsz, "name", 4) == 0)
	{
	  lpsz += 4;
	  HyperTeXSpecialImpl::state.isName = true;
	}
      else if (_strnicmp(lpsz, "href", 4) == 0)
	{
	  lpsz += 4;
	  HyperTeXSpecialImpl::state.isHref = true;
	}
      if (isBaseUrl && ! HyperTeXSpecialImpl::state.isHref)
	{
	  log_error->WriteFormattedLine
	    (T_("libdvi"),
	     T_("bad html special: %s"),
	     lpszSpecial);
	  return (0);
	}
      if (HyperTeXSpecialImpl::state.isName
	  || HyperTeXSpecialImpl::state.isHref)
	{
	  while (isspace(*lpsz))
	    {
	      ++ lpsz;
	    }
	  if (*lpsz != '=')
	    {
	      log_error->WriteFormattedLine
		(T_("libdvi"),
		 T_("bad html special: %s"),
		 lpszSpecial);
	      return (0);
	    }
	  ++ lpsz;
	  while (isspace(*lpsz))
	    {
	      ++ lpsz;
	    }
	  if (*lpsz != '"')
	    {
	      log_error->WriteFormattedLine
		(T_("libdvi"),
		 T_("bad html special: %s"),
		 lpszSpecial);
	      return (0);
	    }
	  ++ lpsz;
	  string str = "";
	  while (*lpsz && *lpsz != '"')
	    {
	      str += *lpsz++;
	    }
	  if (*lpsz != '"')
	    {
	      log_error->WriteFormattedLine
		(T_("libdvi"),
		 T_("bad html special: %s"),
		 lpszSpecial);
	      return (0);
	    }
	  if (isBaseUrl)
	    {
	      HyperTeXSpecialImpl::state.baseUrl = str;
	    }
	  else
	    {
	      HyperTeXSpecialImpl::state.nameOrHref = "";
	      if (str[0] != '#'
		  && ! HyperTeXSpecialImpl::state.baseUrl.empty())
		{
		  HyperTeXSpecialImpl::state.nameOrHref =
		    HyperTeXSpecialImpl::state.baseUrl;
		}
	      HyperTeXSpecialImpl::state.nameOrHref += str;
	      HyperTeXSpecialImpl::state.llx =
		HyperTeXSpecialImpl::state.urx =
		x;
	      HyperTeXSpecialImpl::state.lly =
		HyperTeXSpecialImpl::state.ury =
		y;
	    }
	}
    }
  return (0);
}

/* _________________________________________________________________________

   HyperTeXSpecialImpl::Parse
   _________________________________________________________________________ */

DviSpecialType
HyperTeXSpecialImpl::Parse ()
{
  name = state.nameOrHref;
  llx = state.llx;
  lly = state.lly;
  urx = state.urx;
  ury = state.ury;
  isName = state.isName;
  if (state.isName)
    {
      log_hypertex->WriteFormattedLine
	(T_("libdvi"),
	 T_("new hypertex target \"%s\" (%d, %d, %d, %d)"),
	 name.c_str(),
	 state.llx,
	 state.lly,
	 state.urx,
	 state.ury);
    }
  else
    {
      log_hypertex->WriteFormattedLine
	(T_("libdvi"),
	 T_("new hypertex reference \"%s\" (%d, %d, %d, %d)"),
	 name.c_str(), state.llx, state.lly, state.urx, state.ury);
    }
  return (DviSpecialType::Hypertex);
}
/* _________________________________________________________________________

   ExpandBoundingBox
   _________________________________________________________________________ */

void
ExpandBoundingBox (/*[in]*/ int llx,
		   /*[in]*/ int lly,
		   /*[in]*/ int urx,
		   /*[in]*/ int ury)
{
  if (llx < HyperTeXSpecialImpl::state.llx)
    {
      HyperTeXSpecialImpl::state.llx = llx;
    }
  if (urx > HyperTeXSpecialImpl::state.urx)
    {
      HyperTeXSpecialImpl::state.urx = urx;
    }
  if (lly > HyperTeXSpecialImpl::state.lly)
    {
      HyperTeXSpecialImpl::state.lly = lly;
    }
  if (ury < HyperTeXSpecialImpl::state.ury)
    {
      HyperTeXSpecialImpl::state.ury = ury;
    }
}

/* _________________________________________________________________________

   DviImpl::FindHyperLabel
   _________________________________________________________________________ */

bool
MIKTEXDVICALL
DviImpl::FindHyperLabel (/*[in]*/ const char *		lpszLabel,
			 /*[out]*/ DviPosition &	position)
{
  CheckCondition ();
  MIKTEX_BEGIN_CRITICAL_SECTION (&criticalSectionMonitor)
    {
      log_hypertex->WriteFormattedLine
	(T_("libdvi"),
	 T_("searching hyperlabel %s"),
	 lpszLabel);
      
      for (int p = 0; p < GetNumberOfPages(); ++ p)
	{
	  DviPageImpl * pPage;
	  try
	    {
	      pPage = reinterpret_cast<DviPageImpl*>(GetLoadedPage(p));
	    }
	  catch (const OperationCancelledException &)
	    {
	      return (false);
	    }
	  if (pPage == 0)
	    {
	      throw DviPageNotFoundException
		(0,
		 T_("The DVI page could not be found."),
		 0,
		 T_(__FILE__),
		 __LINE__);
	    }
	  AutoUnlockPage autoUnlockPage (pPage);
	  HypertexSpecial * pHyperSpecial;
	  for (int j = -1;
	       ((pHyperSpecial = pPage->GetNextSpecial<HypertexSpecial>(j))
		!= 0);
	       )
	    {
	      if (pHyperSpecial->IsName()
		  && strcmp(lpszLabel, pHyperSpecial->GetName()) == 0)
		{
		  position.pageIdx = p;
		  position.x = pHyperSpecial->GetX();
		  position.y = pHyperSpecial->GetY();
		  log_hypertex->WriteFormattedLine
		    (T_("libdvi"),
		     T_("found %s on page %d at %d,%d"),
		     lpszLabel,
		     p,
		     position.x,
		     position.y);
		  return (true);
		}
	    }
	}
      
      log_search->WriteLine (T_("libdvi"), "search failed");

      return (false);
    }
  MIKTEX_END_CRITICAL_SECTION();
}
