/* src.cpp: src specials

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

#include "internal.h"

/* _________________________________________________________________________

   MyPathNameCompare
   _________________________________________________________________________ */

STATICFUNC(int)
MyPathNameCompare (/*[in]*/ const PathName & path1,
		   /*[in]*/ const PathName & path2)
{
  int ret = PathName::Compare(path1, path2);

  if (ret == 0)
    {
      return (0);
    }

  const char * lpszExtension1 = path1.GetExtension();
  const char * lpszExtension2 = path2.GetExtension();

  if ((lpszExtension1 == 0 && lpszExtension2 == 0)
      || (lpszExtension1 != 0 && lpszExtension2 != 0))
    {
      return (ret);
    }

  PathName path11 (path1);
  PathName path22 (path2);

  if (lpszExtension1 == 0)
    {
      path11.AppendExtension (lpszExtension2);
    }
  else
    {
      path22.AppendExtension (lpszExtension1);
    }

  return (PathName::Compare(path11, path22));
}

/* _________________________________________________________________________

   DviImpl::FindSource
   _________________________________________________________________________ */

bool
DviImpl::FindSource (/*[in]*/ const char *	lpszFileName,
		     /*[in]*/ int		line,
		     /*[out]*/ DviPosition &	position)
{
  CheckCondition ();

  MIKTEX_BEGIN_CRITICAL_SECTION (&criticalSectionMonitor)
    {
      trace_search->WriteFormattedLine
	("libdvi",
	 T_("searching src special %d %s"),
	 line,
	 lpszFileName);
      
      SourceSpecial * pSourceSpecial1Best = 0;
      SourceSpecial * pSourceSpecial2Best = 0;

      int pageIdx1 = -1;
      int pageIdx2 = -1;

      // get fully qualified path to the directory location of the
      // document
      PathName documentLocation (fqDviFileName);
      documentLocation.RemoveFileSpec ();

      // file name relative to the location of the DVI document
      const char * lpszRelFileName;

      // absolute file name
      PathName fqFileName;

      if (Utils::IsAbsolutePath(lpszFileName))
	{
	  lpszRelFileName =
	    Utils::GetRelativizedPath(lpszFileName, documentLocation.Get());
	  fqFileName = lpszFileName;
	  fqFileName.MakeAbsolute ();
	}
      else
	{
	  lpszRelFileName = lpszFileName;
	  fqFileName = documentLocation;
	  fqFileName += lpszFileName;
	  fqFileName.MakeAbsolute ();
	}

      //
      // scan the document
      //

      for (int pageIdx = 0; pageIdx < GetNumberOfPages(); ++ pageIdx)
	{
	  DviPageImpl * pPage;

	  try
	    {
	      pPage = reinterpret_cast<DviPageImpl*>(GetLoadedPage(pageIdx));
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
		 __FILE__,
		 __LINE__);
	    }

	  AutoUnlockPage autoUnlockPage (pPage);

	  SourceSpecial * pSourceSpecial1 = 0;
	  SourceSpecial * pSourceSpecial2 = 0;
	  
	  SourceSpecial * pSourceSpecial;
	  
	  for (int j = -1;
	       (pSourceSpecial = pPage->GetNextSpecial<SourceSpecial>(j)) != 0;
	       )
	    {
	      const char * lpszName = pSourceSpecial->GetFileName();

	      // try exact match
	      bool nameMatch =
		(MyPathNameCompare(lpszName, lpszFileName) == 0);

	      // try fully qualified file names
	      if (! nameMatch)
		{
		  PathName fqName;
		  if (Utils::IsAbsolutePath(lpszName))
		    {
		      fqName = lpszName;
		      fqName.MakeAbsolute ();
		    }
		  else
		    {
		      fqName = documentLocation;
		      fqName += lpszName;
		      fqName.MakeAbsolute ();
		    }
		  nameMatch = (MyPathNameCompare(fqName, fqFileName) == 0);
		}

	      // try relative file names
	      if (! nameMatch && lpszRelFileName != 0)
		{
		  const char * lpszRelName;
		  if (! Utils::IsAbsolutePath(lpszName))
		    {
		      lpszRelName = lpszName;
		    }
		  else
		    {
		      lpszRelName =
			Utils::GetRelativizedPath(lpszName,
						  documentLocation.Get());
		    }
		  nameMatch =
		    (lpszRelName != 0
		     && MyPathNameCompare(lpszRelName, lpszRelFileName) == 0);
		}

	      if (! nameMatch)
		{
		  continue;
		}

	      if (line >= pSourceSpecial->GetLineNum())
		{
		  if (pSourceSpecial1 == 0
		      || (pSourceSpecial->GetLineNum()
			  > pSourceSpecial1->GetLineNum()))
		    {
		      pSourceSpecial1 = pSourceSpecial;
		    }
		}

	      if (line <= pSourceSpecial->GetLineNum())
		{
		  if (pSourceSpecial2 == 0
		      || (pSourceSpecial->GetLineNum()
			  < pSourceSpecial2->GetLineNum()))
		    {
		      pSourceSpecial2 = pSourceSpecial;
		    }
		}
	    }
	  
	  if (pSourceSpecial1 != 0 && pSourceSpecial2 != 0)
	    {
	      pSourceSpecial1Best = pSourceSpecial1;
	      pageIdx1 = pageIdx;
	      pSourceSpecial2Best = pSourceSpecial2;
	      pageIdx2 = pageIdx;
	      break;
	    }
	  else if (pSourceSpecial1 != 0
		   && (pSourceSpecial1Best == 0
		       || (abs(pSourceSpecial1Best->GetLineNum() - line)
			   > abs(pSourceSpecial1->GetLineNum() - line))))
	    {
	      pSourceSpecial1Best = pSourceSpecial1;
	      pageIdx1 = pageIdx;
	      pSourceSpecial2Best = 0;
	      pageIdx2 = -1;
	    }
	  else if (pSourceSpecial2 != 0
		   && (pSourceSpecial2Best == 0
		       || (abs(pSourceSpecial2Best->GetLineNum() - line)
			   > abs(pSourceSpecial2->GetLineNum() - line))))
	    {
	      pSourceSpecial2Best = pSourceSpecial2;
	      pageIdx2 = pageIdx;
	      pSourceSpecial1Best = 0;
	      pageIdx1 = -1;
	    }
	}

      //
      // evaluate the results
      //

      if (pSourceSpecial1Best == 0 && pSourceSpecial2Best == 0)
	{
	  trace_search->WriteLine ("libdvi", T_("search failed"));
	  return (false);
	}
      
      if (pSourceSpecial1Best == 0)
	{
	  trace_search->WriteFormattedLine
	    ("libdvi",
	     T_("found src2 on page #%d"),
	     pageIdx2);
	  trace_search->WriteFormattedLine
	    ("libdvi",
	     "   src2 = [%d (%d,%d)]",
	     pSourceSpecial2Best->GetLineNum(),
	     pSourceSpecial2Best->GetX(),
	     pSourceSpecial2Best->GetY());
	  position.pageIdx = pageIdx2;
	  position.x = pSourceSpecial2Best->GetX();
	  position.y = pSourceSpecial2Best->GetY();
	}
      else if (pSourceSpecial2Best == 0)
	{
	  trace_search->WriteFormattedLine
	    ("libdvi",
	     T_("found src1 on page #%d"),
	     pageIdx1);
	  trace_search->WriteFormattedLine
	    ("libdvi",
	     "   src1 = [%d (%d,%d)]",
	     pSourceSpecial1Best->GetLineNum(),
	     pSourceSpecial1Best->GetX(),
	     pSourceSpecial1Best->GetY());
	  position.pageIdx = pageIdx1;
	  position.x = pSourceSpecial1Best->GetX();
	  position.y = pSourceSpecial1Best->GetY();
	}
      else
	{
	  position.pageIdx = pageIdx1;
	  trace_search->WriteFormattedLine
	    ("libdvi",
	     T_("found src region on page #%d"),
	     position.pageIdx);
	  trace_search->WriteFormattedLine
	    ("libdvi",
	     "   src1 = [%d (%d,%d)]",
	     pSourceSpecial1Best->GetLineNum(),
	     pSourceSpecial1Best->GetX(),
	     pSourceSpecial1Best->GetY());
	  trace_search->WriteFormattedLine
	    ("libdvi",
	     "   src2 = [%d (%d,%d)]",
	     pSourceSpecial2Best->GetLineNum(),
	     pSourceSpecial2Best->GetX(),
	     pSourceSpecial2Best->GetY());
	  position.x = pSourceSpecial1Best->GetX ();
	  if (pSourceSpecial2Best->GetLineNum()
	      == pSourceSpecial1Best->GetLineNum())
	    {
	      position.y = pSourceSpecial1Best->GetY();
	    }
	  else
	    {
	      position.y = (pSourceSpecial1Best->GetY()
		   + ((line - pSourceSpecial1Best->GetLineNum())
		      * (pSourceSpecial2Best->GetY()
			 - pSourceSpecial1Best->GetY())
		      / (pSourceSpecial2Best->GetLineNum()
			 - pSourceSpecial1Best->GetLineNum())));
	    }
	  trace_search->WriteFormattedLine
	    ("libdvi",
	     "   interpolated (x,y) = (%d,%d)",
	     position.x,
	     position.y);
	}

      return (true);
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   DviImpl::GetSource
   _________________________________________________________________________ */

#define OFFSET(x, y) ((y) * PixelRound(maxH) + (x))

bool
DviImpl::GetSource (/*[in]*/ const DviPosition &	pos,
		    /*[out]*/ PathName &		fileName,
		    /*[out]*/ int *			pLineNum)
{
  CheckCondition ();
  MIKTEX_BEGIN_CRITICAL_SECTION (&criticalSectionMonitor)
    {
      if (pos.pageIdx >= GetNumberOfPages())
	{
	  return (false);
	}
      
      DviPageImpl * pPage =
	reinterpret_cast<DviPageImpl*>(GetLoadedPage(pos.pageIdx));
      
      if (pPage == 0)
	{
	  throw DviPageNotFoundException
	    (0,
	     T_("The DVI page could not be found."),
	     0,
	     __FILE__,
	     __LINE__);
	}

      AutoUnlockPage autoUnlockPage (pPage);
      
      SourceSpecial * pSourceSpecial1 = 0;
      SourceSpecial * pSourceSpecial2 = 0;
      
      int cursorOffset = OFFSET(pos.x, pos.y);

      int src1Offset = 0;
      int src2Offset = 0;
      
      SourceSpecial * pSourceSpecial;
      
      // find two adjacent src specials
      for (int j = -1;
	   (pSourceSpecial = pPage->GetNextSpecial<SourceSpecial>(j)) != 0;
	   )
	{
	  int srcOffset =
	    OFFSET(pSourceSpecial->GetX(), pSourceSpecial->GetY()); 
	  if (cursorOffset > srcOffset)
	    {
	      if (pSourceSpecial1 == 0 || srcOffset > src1Offset)
		{
		  pSourceSpecial1 = pSourceSpecial;
		  src1Offset = srcOffset;
		}
	    }
	  else if (cursorOffset < srcOffset)
	    {
	      if (pSourceSpecial2 == 0 || srcOffset < src2Offset)
		{
		  pSourceSpecial2 = pSourceSpecial;
		  src2Offset = srcOffset;
		}
	    }
	}
      
      if (pSourceSpecial1 == 0 && pSourceSpecial2 == 0)
	{
	  return (false);
	}
      
      int lineDelta = 0;
      
      if (pSourceSpecial1 == 0)
	{
	  pSourceSpecial = pSourceSpecial2;
	}
      else if (pSourceSpecial2 == 0)
	{
	  pSourceSpecial = pSourceSpecial1;
	}
      else if (MyPathNameCompare(pSourceSpecial1->GetFileName(),
				 pSourceSpecial2->GetFileName())
	       != 0)
	{
	  if (cursorOffset - src1Offset < src2Offset - cursorOffset)
	    {
	      pSourceSpecial = pSourceSpecial1;
	    }
	  else
	    {
	      pSourceSpecial = pSourceSpecial2;
	    }
	}
      else
	{
	  int d = src2Offset - src1Offset;
	  if (d != 0)
	    {
	      int d1 = cursorOffset - src1Offset;
	      int linesbetween
		= (pSourceSpecial2->GetLineNum()
		   - pSourceSpecial1->GetLineNum()
		   + 1);
	      lineDelta = (d1 * linesbetween) / d;
	    }
	   pSourceSpecial = pSourceSpecial1;
	}
      fileName = pSourceSpecial->GetFileName();
      if (pLineNum != 0)
	{
	  *pLineNum = pSourceSpecial->GetLineNum() + lineDelta;
	}
      
      return (true);
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   SourceSpecialImpl::Parse
   _________________________________________________________________________ */

DviSpecialType
SourceSpecialImpl::Parse ()

{
  static LONG volatile nextid = 1;
  id = InterlockedIncrement(&nextid);
  const char * lpsz = GetXXX() + 4; // src:
  while (*lpsz == ' ')
    {
      ++ lpsz;
    }
  if (*lpsz != 0 && isdigit(*lpsz))
    {
      char * lpszFileName;
      lineNum = strtol(lpsz, &lpszFileName, 10);
      if (lpszFileName != 0)
	{
	  while (*lpszFileName == ' ')
	    {
	      ++ lpszFileName;
	    }
	  if (*lpszFileName != 0)
	    {
	      fileName = lpszFileName;
	    }
	}
    }
  return (DviSpecialType::Source);
}
