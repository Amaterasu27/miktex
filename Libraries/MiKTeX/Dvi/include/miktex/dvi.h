/* miktex/dvi.h: DVI interfaces					-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(B371214BB940374EA689C48ED44141CD)
#define B371214BB940374EA689C48ED44141CD

#include <miktex/core.h>
#include <miktex/DibChunker.h>

#if ! defined(A86DAFD3F286A340B6BD420E71BCD906)
#  if defined(_MSC_VER)
#    define MIKTEXDVIEXPORT __declspec(dllimport)
#  else
#    define MIKTEXDVIEXPORT
#  endif
#endif

#if defined(_MSC_VER)
#  define MIKTEXDVICALL __stdcall
#else
#  define MIKTEXDVICALL
#endif

#define MIKTEX_DVI_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace DVI {

#define MIKTEX_DVI_END_NAMESPACE		\
    }						\
  }

MIKTEX_DVI_BEGIN_NAMESPACE;
  
/* _________________________________________________________________________

   DviException
   _________________________________________________________________________ */

class DviException : public MiKTeX::Core::MiKTeXException
{
public:
  MIKTEXDVIEXPORT
  MIKTEXCALL
  DviException ();

public:
  MIKTEXDVIEXPORT
  MIKTEXCALL
  DviException (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
		/*[in]*/ const MIKTEXCHAR *	lpszMessage,
		/*[in]*/ const MIKTEXCHAR *	lpszInfo,
		/*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		/*[in]*/ int			sourceLine);
};

/* _________________________________________________________________________

   DviPageNotFoundException
   _________________________________________________________________________ */

class DviPageNotFoundException : public DviException
{
public:
  MIKTEXDVIEXPORT
  MIKTEXCALL
  DviPageNotFoundException ();

public:
  MIKTEXDVIEXPORT
  MIKTEXCALL
  DviPageNotFoundException
  (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
   /*[in]*/ const MIKTEXCHAR *	lpszMessage,
   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
   /*[in]*/ int			sourceLine);
};

/* _________________________________________________________________________

   DviFileInUseException
   _________________________________________________________________________ */

class DviFileInUseException : public DviException
{
public:
  MIKTEXDVIEXPORT
  MIKTEXCALL
  DviFileInUseException ();

public:
  MIKTEXDVIEXPORT
  MIKTEXCALL
  DviFileInUseException
  (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
   /*[in]*/ const MIKTEXCHAR *	lpszMessage,
   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
   /*[in]*/ int			sourceLine);
};

/* _________________________________________________________________________

   DviRule
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
DviRule
{
public:
  virtual
  int
  MIKTEXDVICALL
  GetLeft (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetRight (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetTop (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetBottom (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  unsigned long
  MIKTEXDVICALL
  GetBackgroundColor ()
    = 0;

public:
  virtual
  unsigned long
  MIKTEXDVICALL
  GetForegroundColor ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  IsBlackboard ()
    = 0;
};

/* _________________________________________________________________________

   DviSpecialType
   _________________________________________________________________________ */

class DviSpecialTypeEnum
{
public:
  enum EnumType {
    Unknown,
    Source,
    Psdef,
    Psfile,
    Ps,
    IncludeGraphics,
    Tpic,
    Hypertex,
    SolidLine,
    LogMessage,
    PaperSize,
    Landscape,
  };
};

typedef MiKTeX::Core::EnumWrapper<DviSpecialTypeEnum> DviSpecialType;

/* _________________________________________________________________________

   DviSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
DviSpecial
{
public:
  virtual
  int
  MIKTEXDVICALL
  GetX ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetY ()
    = 0;

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetXXX ()
    = 0;

public:
  virtual
  DviSpecialType
  MIKTEXDVICALL
  GetType ()
    = 0;
};

/* _________________________________________________________________________

   SolidLineSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
SolidLineSpecial : public DviSpecial
{
public:
  virtual
  unsigned long
  MIKTEXDVICALL
  GetColor ()
    = 0;

public:
  virtual
  unsigned
  MIKTEXDVICALL
  GetWidth ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetStartX ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetStartY ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetEndX ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetEndY ()
    = 0;
};

/* _________________________________________________________________________

   SourceSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
SourceSpecial : public DviSpecial
{
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetFileName ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetLineNum ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetId ()
    = 0;
};

/* _________________________________________________________________________

   TpicSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
TpicSpecial : public DviSpecial
{
public:
  struct point { int x, y; };
  typedef std::vector<point> path;

public:
  virtual
  const path &
  MIKTEXDVICALL
  GetPath ()
    = 0;

public:
  virtual
  float
  MIKTEXDVICALL
  GetShade ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetPenSize ()
    = 0;
};

/* _________________________________________________________________________

   OutlineStyle
   _________________________________________________________________________ */

class OutlineStyleEnum
{
public:
  enum EnumType {
    None,
    Solid,
    Dashes,
    Dots
  };
};

typedef MiKTeX::Core::EnumWrapper<OutlineStyleEnum> OutlineStyle;

/* _________________________________________________________________________

   TpicPolySpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
TpicPolySpecial : public TpicSpecial
{
public:
  virtual
  OutlineStyle
  MIKTEXDVICALL
  GetOutlineStyle (/*[out]*/ float & length)
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  IsSpline ()
    = 0;
};

/* _________________________________________________________________________

   TpicArcSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
TpicArcSpecial : public TpicSpecial
{
public:
  virtual
  int
  MIKTEXDVICALL
  GetCx ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetCy ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetRx ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetRy ()
    = 0;

public:
  virtual
  float
  MIKTEXDVICALL
  GetS ()
    = 0;

public:
  virtual
  float
  MIKTEXDVICALL
  GetE ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasOutline ()
    = 0;
};

/* _________________________________________________________________________

   HypertexSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
HypertexSpecial : public DviSpecial
{
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetName ()
    = 0;

public:
  virtual
  void
  MIKTEXDVICALL
  GetBbox (/*[out]*/ int & llx,
	   /*[out]*/ int & llr,
	   /*[out]*/ int & urx,
	   /*[out]*/ int & ury)
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  IsName ()
    = 0;
};

/* _________________________________________________________________________

   GraphicsSpecialAction
   _________________________________________________________________________ */

class GraphicsSpecialActionEnum
{
public:
  enum EnumType {
    None,
    DviWinIsoScale,
    DviWinAnisoScale,
    DviWinCenter,
    EmGraph
  };
};

typedef
MiKTeX::Core::EnumWrapper<GraphicsSpecialActionEnum> GraphicsSpecialAction;

/* _________________________________________________________________________

   GraphicsSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
GraphicsSpecial : public DviSpecial
{
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetFileName ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetWidth ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetHeight ()
    = 0;

public:
  virtual
  GraphicsSpecialAction
  MIKTEXDVICALL
  GetAction ()
    = 0;
};

/* _________________________________________________________________________

   PsdefSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
PsdefSpecial : public DviSpecial
{
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetDef ()
    = 0;

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetFileName ()
    = 0;
};

/* _________________________________________________________________________

   DvipsSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
DvipsSpecial : public DviSpecial
{
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetString ()
    = 0;

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetFileName ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  GetProtection ()
    = 0;
};

/* _________________________________________________________________________

   PaperSizeSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
PaperSizeSpecial : public DviSpecial
{
public:
  virtual
  MiKTeX::Core::PaperSizeInfo
  MIKTEXDVICALL
  GetPaperSizeInfo ()
    = 0;
};

/* _________________________________________________________________________

   LandscapeSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
LandscapeSpecial : public DviSpecial
{
};

/* _________________________________________________________________________

   PsfileSpecial
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
PsfileSpecial : public DviSpecial
{
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetFileName ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetHsize ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetVsize ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetHoffset ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetVoffset ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetHscale ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetVscale ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetAngke ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetLlx ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetLly ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetUrx ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetUry ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetRwi ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetRhi ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  IsClipped ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasHsize ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasVsize ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasHoffset ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasVoffset ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasHscale ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasVscale ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasAngle ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasLlx ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasLly ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasUrx ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasUry ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasRwi ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasRhi ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  HasClipFlag ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  GetBoundingBox (/*[out]*/ float & left,
		  /*[out]*/ float & bottom,
		  /*[out]*/ float & right,
		  /*[out]*/ float & top)
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  GetBoundingBox (/*[in]*/ int	shrinkFactor, 
		  /*[out]*/ int & left,
		  /*[out]*/ int & bottom,
		  /*[out]*/ int & right,
		  /*[out]*/ int & top)
    = 0;
};

/* _________________________________________________________________________

   DviBitmap
   _________________________________________________________________________ */

struct DviBitmap
{
  const void *		pPixels;
  int			x;
  int			y;
  int			width;
  int			height;
  int			bytesPerLine;
  bool			monochrome;
  unsigned long		foregroundColor;
  unsigned long		backgroundColor;
};

/* _________________________________________________________________________

   DviPage
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
DviPage
{
public:
  virtual
  const DviBitmap &
  MIKTEXDVICALL
  GetDviBitmap (/*[in]*/ int shrinkFactor,
		/*[in]*/ int idx)
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetNumberOfDviBitmaps (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  DviSpecial *
  MIKTEXDVICALL
  GetSpecial (/*[in]*/ int idx)
    = 0;

public:
  virtual
  DviRule *
  MIKTEXDVICALL
  GetRule (/*[in]*/ int idx)
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetReg (/*[in]*/ int idx)
    = 0;

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXDVICALL
  GetName ()
    = 0;

public:
  virtual
  unsigned long
  MIKTEXDVICALL
  GetBackgroundColor ()
    = 0;

public:
  virtual
  void
  MIKTEXDVICALL
  Lock ()
    = 0;

public:
  virtual
  void
  MIKTEXDVICALL
  Unlock ()
    = 0;

public:
  virtual
  HypertexSpecial *
  MIKTEXDVICALL
  GetNextHyperref (/*[out]*/ int & idx)
    = 0;

public:
  virtual
  MiKTeX::Graphics::DibChunk *
  MIKTEXDVICALL
  GetDibChunk (/*[in]*/ int shrinkFactor,
	       /*[in]*/ int idx)
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetNumberOfDibChunks (/*[in]*/ int shrinkFactor)
    = 0;
};

/* _________________________________________________________________________

   DviFontInfo
   _________________________________________________________________________ */

struct DviFontInfo
{
  bool			notLoadable;
  bool			isVirtualFont;
  MiKTeX::Core::tstring	name;
  MiKTeX::Core::tstring	fileName;
  MiKTeX::Core::tstring	transcript;
  MiKTeX::Core::tstring	comment;
};

/* _________________________________________________________________________

   PageStatus
   _________________________________________________________________________ */

class PageStatusEnum
{
public:
  enum EnumType {
    Unknown,
    NotLoaded,
    Lost,
    Changed,
    Loaded
  };
};

typedef MiKTeX::Core::EnumWrapper<PageStatusEnum> PageStatus;

/* _________________________________________________________________________

   DviNotification
   _________________________________________________________________________ */

class DviNotificationEnum
{
public:
  enum EnumType {
    None,
    BeginLoadPage,
    BeginLoadFont,
  };
};

typedef MiKTeX::Core::EnumWrapper<DviNotificationEnum> DviNotification;

/* _________________________________________________________________________

   DviAccess
   _________________________________________________________________________ */

class DviAccessEnum
{
public:
  enum EnumType {
    None,
    Random,
    Sequential,
  };
};

typedef MiKTeX::Core::EnumWrapper<DviAccessEnum> DviAccess;

/* _________________________________________________________________________

   DviPageMode
   _________________________________________________________________________ */

class DviPageModeEnum
{
public:
  enum EnumType {
    None,
    Auto,
    Pk,
    Dvips
  };
};

typedef MiKTeX::Core::EnumWrapper<DviPageModeEnum> DviPageMode;

/* _________________________________________________________________________

   IDviCallback
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
IDviCallback
{
public:
  virtual
  void
  MIKTEXDVICALL
  OnProgress (/*[in]*/ DviNotification		nf)
    = 0;
};

/* _________________________________________________________________________

   DviPosition
   _________________________________________________________________________ */

struct DviPosition
{
public:
  DviPosition ()
    : pageIdx (0),
      x (0),
      y (0)
  {
  }

public:
  DviPosition (/*[in]*/ int	pageIdx,
	       /*[in]*/ int	x,
	       /*[in]*/ int	y)
    : pageIdx (pageIdx),
      x (x),
      y (y)
  {
  }

public:
  int pageIdx;

public:
  int x;

public:
  int y;
};

/* _________________________________________________________________________

   Dvi
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
Dvi
{
public:
  virtual
  MIKTEXDVICALL
  ~Dvi ()
    = 0;

public:
  virtual
  void
  MIKTEXDVICALL
  Dispose ()
    = 0;

public:
  static
  MIKTEXDVIEXPORT
  Dvi *
  MIKTEXDVICALL
  Create (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	  /*[in]*/ const MIKTEXCHAR *	lpszMetafontMode,
	  /*[in]*/ int			resolution,
	  /*[in]*/ int			shrinkFactor,
	  /*[in]*/ DviAccess		access,
	  /*[in]*/ IDviCallback *	pCallback);

public:
  static
  MIKTEXDVIEXPORT
  Dvi *
  MIKTEXDVICALL
  Create (/*[in]*/ const MIKTEXCHAR *		lpszFileName,
	  /*[in]*/ const MIKTEXCHAR *		lpszMetafontMode,
	  /*[in]*/ int				resolution,
	  /*[in]*/ int				shrinkFactor,
	  /*[in]*/ DviAccess			access,
	  /*[in]*/ DviPageMode			pageMode,
	  /*[in]*/ const MiKTeX::Core::PaperSizeInfo &	defaultPaperSizeInfo,
	  /*[in]*/ bool				landscape,
	  /*[in]*/ IDviCallback *		pCallback);

public:
  virtual
  int
  MIKTEXDVICALL
  GetNumberOfPages ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  GetSource (/*[in]*/ const DviPosition &	pos,
	     /*[out]*/ MiKTeX::Core::PathName &	fileName,
	     /*[out]*/ int *			pLineNum = 0)
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  FindSource (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	      /*[in]*/ int			line,
	      /*[out]*/ DviPosition &		position)
    = 0;

public:
  virtual
  DviPage *
  MIKTEXDVICALL
  GetPage (/*[in]*/ int pageIdx)
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetMinPageNumber ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetMaxPageNumber ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetMagnification ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetMaxH ()
    = 0;

public:
  virtual
  int
  MIKTEXDVICALL
  GetMaxV ()
    = 0;

public:
  virtual
  PageStatus
  MIKTEXDVICALL
  GetPageStatus (/*[in]*/ int pageIdx)
    = 0;

public:
  virtual
  DviPage *
  MIKTEXDVICALL
  GetLoadedPage (/*[in]*/ int pageIdx)
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  FindHyperLabel (/*[in]*/ const MIKTEXCHAR *	lpszLabel,
		  /*[out]*/ DviPosition &	position)
    = 0;

public:
  virtual
  MiKTeX::Core::tstring
  MIKTEXDVICALL
  GetStatusText ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  MakeFonts ()
    = 0;

public:
  virtual
  std::vector<DviFontInfo>
  MIKTEXDVICALL
  GetFontTable ()
    = 0;

public:
  virtual
  MiKTeX::Core::PaperSizeInfo
  MIKTEXDVICALL
  GetPaperSizeInfo ()
    = 0;

public:
  virtual
  bool
  MIKTEXDVICALL
  Landscape ()
    = 0;

public:
  virtual
  void
  MIKTEXDVICALL
  Lock ()
    = 0;

public:
  virtual
  void
  MIKTEXDVICALL
  Unlock ()
    = 0;

public:
  virtual
  void
  MIKTEXDVICALL
  Scan ()
    = 0;
};

/* _________________________________________________________________________ */

MIKTEX_DVI_END_NAMESPACE;

#undef MIKTEX_DVI_BEGIN_NAMESPACE
#undef MIKTEX_DVI_END_NAMESPACE

#endif
