/* DviDoc.h:							-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

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

#pragma once

class DviDoc
  : public CDocument,
    public IDviCallback
{
protected:
  DECLARE_DYNCREATE (DviDoc);

protected:
  DviDoc ();

public:
  virtual
  ~DviDoc ();

public:
  virtual
  void
  MIKTEXDVICALL
  OnProgress (/*[in]*/ DviNotification	nf);

public:
  int
  GetPageCount ()
    const;

public:
  int
  GetRowCount ()
    const;

public:
  int
  DeterminePageIdx (/*[in]*/ int	rowIdx,
		    /*[in]*/ bool	onRight)
    const;

public:
  int
  GetRowIdx (/*[in]*/ int pageIdx)
    const;

public:
  bool
  IsOnLeft (/*[in]*/ int pageIdx)
    const;

public:
  bool
  IsOnRight (/*[in]*/ int pageIdx)
    const;

public:
  bool
  IsFlipSide (/*[in]*/ int pageIdx)
    const;

public:
  void
  SetGraphicsDone (/*[in]*/ int pageIdx);

public:
  bool
  GraphicsDone (/*[in]*/ int pageIdx)
    const;

public:
  void
  RememberGraphicsInclusion (/*[in]*/ int			pageIdx,
			     /*[in]*/ const GraphicsInclusion &	grinc);

public:
  bool
  GetGraphicsInclusion (/*[in]*/ int			pageIdx,
			/*[in]*/ size_t			idx,
			/*[out]*/ GraphicsInclusion &	graphicsInclusion);

public:
  void
  ForgetGraphicsInclusions ();

public:
  int
  GetMagnification ()
    const;

public:
  int
  GetDisplayShrinkFactor ()
    const;

public:
  DviPageMode
  GetDviPageMode ()
    const
  {
    return (dviPageMode);
  }

public:
  DviPageMode
  SetDviPageMode (/*[in]*/ const DviPageMode & dviPageMode);

public:
  PageStatus
  GetPageStatus (/*[in]*/ int pageIdx);

public:
  bool
  IsPrintContext ()
    const;

public:
  int
  FindPage (/*[in]*/ int iPageNum)
    const;

public:
  void
  BeginDviPrinting (/*[in]*/ const CDC * pPrinterDC);

public:
  void
  EndDviPrinting ();

public:
  bool
  FindHyperLabel (/*[in]*/ const char *	lpszFileName,
		  /*[out]*/ DviPosition &	position);
  
public:
  bool
  FindSrcSpecial (/*[in]*/ const char *		lpszFileName,
		  /*[in]*/ int			line,
		  /*[out]*/ DviPosition &	position);

public:
  int
  GetDisplayResolution ()
    const;

public:
  CString
  GetDocDir ();

public:
  DviPage *
  GetLoadedPage (/*[in]*/ int pageIdx);

public:
  int
  GetMaxPageNum ()
    const;

public:
  unsigned long
  GetMetafontMode ()
    const;

public:
  int
  GetMinPageNum ()
    const;

public:
  const char *
  GetPageName (/*[in]*/ unsigned pageIdx);

public:
  int
  GetPageNum (/*[in]*/ int pageIdx)
    const;

public:
  CSize
  GetPaperSize ()
    const;

public:
  PaperSizeInfo
  GetPaperSizeInfo ()
    const
  {
    MIKTEX_ASSERT (pDvi != 0);
    return (pDvi->GetPaperSizeInfo());
  }

public:
  int
  GetPrinterResolution ()
    const;

public:
  bool
  GetSource (/*[in]*/ const DviPosition &	position,
	     /*[out]*/ PathName &		fileName,
	     /*[out]*/ int *			pLineNum);

public:
  bool
  IsPortrait ()
    const;

public:
  void
  Reread ();

private:
  void
  CreateDocument (/*[in]*/ const char * lpszPathName);

public:
  void
  MakeFonts ();

public:
  int
  SetDisplayShrinkFactor (/*[in]*/ int shrinkFactor);

public:
  void
  Shrink (/*[in]*/ int d = 1);

public:
  void
  Lock ()
  {
    MIKTEX_ASSERT (pDvi != 0);
    pDvi->Lock ();
  }

public:
  void
  Unlock ()
  {
    MIKTEX_ASSERT (pDvi != 0);
    pDvi->Unlock ();
  }

public:
  void
  Unshrink ();

public:
  static
  DviDoc *
  GetActiveDocument ();

public:
  virtual
  BOOL
  OnOpenDocument (/*[in]*/ const char * lpszPathName);

protected:
  virtual
  void
  OnIdle ();

public:
  int
  GetShrinkFactor ()
    const;

public:
  int
  GetResolution ()
    const;

private:
  int
  pt2pxl (/*[in]*/ int pt)
    const;

public:
  vector<DviFontInfo>
  GetFontTable ();

public:
  enum DviFileStatus {
    DVIFILE_NOT_LOADED,
    DVIFILE_LOADED,
    DVIFILE_MODIFIED,
    DVIFILE_LOST
  };

public:
  DviFileStatus
  GetDviFileStatus ();

private:
  bool isPrintContext;

private:
  Dvi * pDvi;

private:
  Dvi * pDviSave;

private:
  int displayShrinkFactor;

private:
  DviPageMode dviPageMode;

private:
  string dvipsPaperName;

private:
  bool landscape;

private:
  map<int, vector<GraphicsInclusion> > graphicsInclusions;

private:
  map<int, bool> graphicsDone;

private:
  time_t modificationTime;

private:
  DviFileStatus fileStatus;

private:
  bool makingFonts;

private:
  ProgressDialog * pProgressDialog;

private:
  SessionWrapper pSession;

protected:
  DECLARE_MESSAGE_MAP();
};

/* _________________________________________________________________________

   DviDoc::GetPageCount
   _________________________________________________________________________ */

inline
int
DviDoc::GetPageCount ()
  const
{
  if (fileStatus == DVIFILE_NOT_LOADED || fileStatus == DVIFILE_LOST)
    {
      return (0);
    }
  MIKTEX_ASSERT (pDvi != 0);
  return (pDvi->GetNumberOfPages());
}

/* _________________________________________________________________________
     
   DviDoc::GetRowCount
   _________________________________________________________________________ */

inline
int
DviDoc::GetRowCount ()
  const
{
  return ((GetPageCount() + 2) / 2);
}

/* _________________________________________________________________________

   DviDoc::DeterminePageIdx
   _________________________________________________________________________ */

inline
int
DviDoc::DeterminePageIdx (/*[in]*/ int	rowIdx,
			  /*[in]*/ bool	onRight)
  const
{
  MIKTEX_ASSERT (pDvi != 0);
  MIKTEX_ASSERT (rowIdx >= 0);
  MIKTEX_ASSERT (rowIdx < GetRowCount());
  return (onRight && 2 * rowIdx < GetPageCount()
	  ? 2 * rowIdx
	  : (rowIdx == 0
	     ? 0
	     : 2 * rowIdx - 1));
}

/* _________________________________________________________________________

   DviDoc::GetRowIdx
   _________________________________________________________________________ */

inline
int
DviDoc::GetRowIdx (/*[in]*/ int pageIdx)
  const
{
  MIKTEX_ASSERT (pDvi != 0);
  MIKTEX_ASSERT (pageIdx >= 0);
  MIKTEX_ASSERT (pageIdx < GetPageCount());
  return ((pageIdx + 1) / 2);
}

/* _________________________________________________________________________

   DviDoc::IsOnLeft
   _________________________________________________________________________ */

inline
bool
DviDoc::IsOnLeft (/*[in]*/ int pageIdx)
  const
{
  return (IsFlipSide(pageIdx));
}

/* _________________________________________________________________________

   DviDoc::IsOnRight
   _________________________________________________________________________ */

inline
bool
DviDoc::IsOnRight (/*[in]*/ int pageIdx)
  const
{
  return (! IsOnLeft(pageIdx));
}

/* _________________________________________________________________________

   DviDoc::IsFlipSide
   _________________________________________________________________________ */

inline
bool
DviDoc::IsFlipSide (/*[in]*/ int pageIdx)
  const
{
  return (pageIdx % 2 != 0);
}

/* _________________________________________________________________________

   DviDoc::SetGraphicsDone
   _________________________________________________________________________ */

inline
void
DviDoc::SetGraphicsDone (/*[in]*/ int pageIdx)
{
  graphicsDone[pageIdx] = true;
}

/* _________________________________________________________________________

   DviDoc::GraphicsDone
   _________________________________________________________________________ */

inline
bool
DviDoc::GraphicsDone (/*[in]*/ int pageIdx)
  const
{
  map<int, bool>::const_iterator it = graphicsDone.find(pageIdx);
  if (it == graphicsDone.end())
    {
      return (false);
    }
  return (it->second);
}

/* _________________________________________________________________________

   DviDoc::RememberGraphicsInclusion
   _________________________________________________________________________ */

inline
void
DviDoc::RememberGraphicsInclusion
(/*[in]*/ int				pageIdx,
 /*[in]*/ const GraphicsInclusion &	graphicsInclusion)
{
  graphicsInclusions[pageIdx].push_back (graphicsInclusion);
}

/* _________________________________________________________________________

   DviDoc::GetGraphicsInclusion
   _________________________________________________________________________ */

inline
bool
DviDoc::GetGraphicsInclusion (/*[in]*/ int			pageIdx,
			      /*[in]*/ size_t			idx,
			      /*[out]*/ GraphicsInclusion & graphicsInclusion)
{
#if 0
  if (static_cast<size_t>(pageIdx) >= graphicsInclusions.size())
    {
      UNEXPECTED_CONDITION (T_("DviDoc::GetGraphicsInclusion"));
    }
#endif
  vector<GraphicsInclusion> & vec = graphicsInclusions[pageIdx];
  if (idx == vec.size())
    {
      return (false);
    }
  if (idx > vec.size())
    {
      UNEXPECTED_CONDITION (T_("DviDoc::GetGraphicsInclusion"));
    }
  graphicsInclusion = vec[idx];
  return (true);
}

/* _________________________________________________________________________

   DviDoc::GetFontTable
   _________________________________________________________________________ */

inline
vector<DviFontInfo>
DviDoc::GetFontTable ()
{
  MIKTEX_ASSERT (pDvi != 0);
  return (pDvi->GetFontTable());
}

/* _________________________________________________________________________
     
   DviDoc::GetShrinkFactor
   _________________________________________________________________________ */

inline
int
DviDoc::GetShrinkFactor ()
  const
{
  return (isPrintContext
	  ? 1
	  : GetDisplayShrinkFactor());
}

/* _________________________________________________________________________

   DviDoc::GetResolution
   _________________________________________________________________________ */

inline
int
DviDoc::GetResolution ()
  const
{
  return (isPrintContext
	  ? GetPrinterResolution()
	  : GetDisplayResolution());
}

/* _________________________________________________________________________

   DviDoc::pt2pxl
   _________________________________________________________________________ */

inline
int
DviDoc::pt2pxl (/*[in]*/ int pt)
  const
{
  MIKTEX_ASSERT (GetShrinkFactor() != 0);
  return (static_cast<int>(((GetResolution() * pt) / 72.0)
			   / GetShrinkFactor()));
}

/* _________________________________________________________________________

   DviDoc::IsPrintContext
   _________________________________________________________________________ */

inline bool
DviDoc::IsPrintContext () const
{
  return (isPrintContext);
}

/* _________________________________________________________________________

   DviDoc::GetDisplayShrinkFactor
   _________________________________________________________________________ */

inline int
DviDoc::GetDisplayShrinkFactor () const
{
  return (displayShrinkFactor);
}

/* _________________________________________________________________________

   DviDoc::GetPageStatus
   _________________________________________________________________________ */

inline
PageStatus
DviDoc::GetPageStatus (/*[in]*/ int pageIdx)
{
  MIKTEX_ASSERT (pDvi != 0);
  MIKTEX_ASSERT (pageIdx >= 0 && pageIdx < GetPageCount());
  return (pDvi->GetPageStatus(pageIdx));
}

/* _________________________________________________________________________

   DviDoc::GetMagnification
   _________________________________________________________________________ */

inline
int
DviDoc::GetMagnification ()
  const
{
  MIKTEX_ASSERT (pDvi != 0);
  return (pDvi->GetMagnification());
}

/* _________________________________________________________________________

   AutoUnlockDviDoc
   _________________________________________________________________________ */

class UnlockDviDoc_
{
public:
  void
  operator() (/*[in]*/ DviDoc * p)
  {
    p->Unlock ();
  }
};

typedef MiKTeX::Core::AutoResource<DviDoc *, UnlockDviDoc_> AutoUnlockDviDoc;

/* _________________________________________________________________________

   AutoRestoreDisplayShrinkFactor
   _________________________________________________________________________ */

class SetDisplayShrinkFactor_
{
public:
  void
  operator() (/*[in]*/ DviDoc *	pDviDoc,
	      /*[in]*/ int	shrinkFactor)
  {
    pDviDoc->SetDisplayShrinkFactor (shrinkFactor);
  }
};

typedef AutoResource2<DviDoc *, int, SetDisplayShrinkFactor_>
AutoRestoreDisplayShrinkFactor;
