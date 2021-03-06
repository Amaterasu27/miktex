/* source.cpp: source specials

   Copyright (C) 1996-2011 Christian Schenk
   
   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include "StdAfx.h"

#include "yap.h"

#include "Dvidoc.h"
#include "DviView.h"
#include "SourceSpecialsDialog.h"

/* _________________________________________________________________________

   DviView::CloseSourceSpecialsDialog
   _________________________________________________________________________ */

void
DviView::CloseSourceSpecialsDialog ()
{
  pSourceSpecialDialog = 0;
}

/* _________________________________________________________________________

   DviView::GotoSrcSpecial
   _________________________________________________________________________ */

bool
DviView::GotoSrcSpecial (/*[in]*/ int			line,
			 /*[in]*/ const char *		lpszFileName)
{
  ClearSearchPosition ();

  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);

  DviPosition position;

  if (! pDoc->FindSrcSpecial(lpszFileName, line, position))
    {
      return (false);
    }

  searchPosition.x = position.x;
  searchPosition.y = position.y;

  pageSearchPosition = position.pageIdx;

  if (curPageIdx != position.pageIdx)
    {
      ChangePage (position.pageIdx);
    }

  position.x = PixelShrink(position.x);
  position.x += GetPagePositionX(position.pageIdx);

  position.y = PixelShrink(position.y);
  position.y += GetPagePositionY(position.pageIdx);

  if (! IsPointInView(CPoint(position.x, position.y)))
    {
      CenterOnPoint (CPoint(position.x, position.y));
    }

  Invalidate ();

  return (true);
}

/* _________________________________________________________________________

   DviView::OnPageEditor
   _________________________________________________________________________ */

void
DviView::OnPageEditor () 
{
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      PathName fileName;
      int line;
      if (GetSource(fileName, line))
	{
	  StartEditor (fileName.Get(), pDoc->GetDocDir().Get(), line);
	}
      else
	{
	  FATAL_MIKTEX_ERROR ("DviView::OnPageEditor",
			      T_("The source file could not be opened because \
the page contains no source links."),
			      0);
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   DviView::OnUpdatePageEditor
   _________________________________________________________________________ */

void
DviView::OnUpdatePageEditor (/*[in]*/ CCmdUI * pCmdUI)
{
  BOOL enable = FALSE;
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  enable = TRUE;
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  pCmdUI->Enable (enable);
}

/* _________________________________________________________________________

   DviView::OnToolsSourcespecials
   _________________________________________________________________________ */

void
DviView::OnToolsSourcespecials ()
{
  try
    {
      if (pSourceSpecialDialog == 0)
	{
	  pSourceSpecialDialog =
	    new SourceSpecialsDialog (this, GetDocument());
	  pSourceSpecialDialog->Create (IDD_SOURCE_SPECIALS, this);
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   DviView::OnUpdateToolsSourcespecials
   _________________________________________________________________________ */

void
DviView::OnUpdateToolsSourcespecials (/*[in]*/ CCmdUI * pCmdUI)
{
  BOOL enable = FALSE;
  try
    {
      DviDoc * pDoc = GetDocument();
      ASSERT_VALID (pDoc);
      if (pDoc->GetDviFileStatus() == DviDoc::DVIFILE_LOADED)
	{
	  enable = TRUE;
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  pCmdUI->Enable (enable);
}

/* _________________________________________________________________________

   DviView::GetSource
   _________________________________________________________________________ */

bool
DviView::GetSource (/*[out]*/ PathName &	fileName,
		    /*[out]*/ int &		line)
{
  int x, y;
  int pageIdx;
  if (! GetPageUnderCursor(pageIdx, x, y))
    {
      return (false);
    }
  MIKTEX_ASSERT (pageIdx >= 0);
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  if (pDoc->GetPageStatus(pageIdx) != PageStatus::Loaded)
    {
      return (false);
    }
  return (pDoc->GetSource(DviPosition(pageIdx, x, y), fileName, &line));
}

/* _________________________________________________________________________

   DviDoc::GetSource
   _________________________________________________________________________ */

bool
DviDoc::GetSource (/*[in]*/ const DviPosition &	position,
		   /*[out]*/ PathName &		fileName,
		   /*[out]*/ int *		pLine)
{
  return (pDvi->GetSource(position, fileName, pLine));
}

/* _________________________________________________________________________

   DviDoc::FindSrcSpecial
   _________________________________________________________________________ */

bool
DviDoc::FindSrcSpecial (/*[in]*/ const char *	lpszFileName,
			/*[in]*/ int			line,
			/*[in]*/ DviPosition &		position)
{
  return (pDvi->FindSource(lpszFileName, line, position));
}
