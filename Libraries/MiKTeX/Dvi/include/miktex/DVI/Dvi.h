/* miktex/DVI/Dvi.h: DVI interfaces				-*- C++ -*-

   Copyright (C) 1996-2011 Christian Schenk

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

#include <miktex/Core/Core>
#include <miktex/Graphics/DibChunker>

// DLL import/export switch
#if ! defined(A86DAFD3F286A340B6BD420E71BCD906)
#  if defined(_MSC_VER)
#    define MIKTEXDVIEXPORT __declspec(dllimport)
#  else
#    define MIKTEXDVIEXPORT
#  endif
#endif

// API decoration for exported member functions
#define MIKTEXDVICEEAPI(type) MIKTEXDVIEXPORT type MIKTEXCEECALL

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
  MIKTEXTHISCALL
  DviException ();

public:
  MIKTEXDVIEXPORT
  MIKTEXTHISCALL
  DviException (/*[in]*/ const char *	lpszProgramInvocationName,
		/*[in]*/ const char *	lpszMessage,
		/*[in]*/ const char *	lpszInfo,
		/*[in]*/ const char *	lpszSourceFile,
		/*[in]*/ int		sourceLine);
};

/* _________________________________________________________________________

   DviPageNotFoundException
   _________________________________________________________________________ */

class DviPageNotFoundException : public DviException
{
public:
  MIKTEXDVIEXPORT
  MIKTEXTHISCALL
  DviPageNotFoundException ();

public:
  MIKTEXDVIEXPORT
  MIKTEXTHISCALL
  DviPageNotFoundException
  (/*[in]*/ const char *	lpszProgramInvocationName,
   /*[in]*/ const char *	lpszMessage,
   /*[in]*/ const char *	lpszInfo,
   /*[in]*/ const char *	lpszSourceFile,
   /*[in]*/ int			sourceLine);
};

/* _________________________________________________________________________

   DviFileInUseException
   _________________________________________________________________________ */

class DviFileInUseException : public DviException
{
public:
  MIKTEXDVIEXPORT
  MIKTEXTHISCALL
  DviFileInUseException ();

public:
  MIKTEXDVIEXPORT
  MIKTEXTHISCALL
  DviFileInUseException
  (/*[in]*/ const char *	lpszProgramInvocationName,
   /*[in]*/ const char *	lpszMessage,
   /*[in]*/ const char *	lpszInfo,
   /*[in]*/ const char *	lpszSourceFile,
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
  MIKTEXTHISCALL
  GetLeft (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetRight (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetTop (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetBottom (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  unsigned long
  MIKTEXTHISCALL
  GetBackgroundColor ()
    = 0;

public:
  virtual
  unsigned long
  MIKTEXTHISCALL
  GetForegroundColor ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  GetX ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetY ()
    = 0;

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetXXX ()
    = 0;

public:
  virtual
  DviSpecialType
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  GetColor ()
    = 0;

public:
  virtual
  unsigned
  MIKTEXTHISCALL
  GetWidth ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetStartX ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetStartY ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetEndX ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
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
  const char *
  MIKTEXTHISCALL
  GetFileName ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetLineNum ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  GetPath ()
    = 0;

public:
  virtual
  float
  MIKTEXTHISCALL
  GetShade ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  GetOutlineStyle (/*[out]*/ float & length)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  GetCx ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetCy ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetRx ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetRy ()
    = 0;

public:
  virtual
  float
  MIKTEXTHISCALL
  GetS ()
    = 0;

public:
  virtual
  float
  MIKTEXTHISCALL
  GetE ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
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
  const char *
  MIKTEXTHISCALL
  GetName ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  GetBbox (/*[out]*/ int & llx,
	   /*[out]*/ int & llr,
	   /*[out]*/ int & urx,
	   /*[out]*/ int & ury)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
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
  const char *
  MIKTEXTHISCALL
  GetFileName ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetWidth ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetHeight ()
    = 0;

public:
  virtual
  GraphicsSpecialAction
  MIKTEXTHISCALL
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
  const char *
  MIKTEXTHISCALL
  GetDef ()
    = 0;

public:
  virtual
  const char *
  MIKTEXTHISCALL
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
  const char *
  MIKTEXTHISCALL
  GetString ()
    = 0;

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetFileName ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
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
  const char *
  MIKTEXTHISCALL
  GetFileName ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetHsize ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetVsize ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetHoffset ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetVoffset ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetHscale ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetVscale ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetAngke ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetLlx ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetLly ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetUrx ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetUry ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetRwi ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetRhi ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsClipped ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasHsize ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasVsize ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasHoffset ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasVoffset ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasHscale ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasVscale ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasAngle ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasLlx ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasLly ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasUrx ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasUry ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasRwi ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasRhi ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasClipFlag ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetBoundingBox (/*[out]*/ float & left,
		  /*[out]*/ float & bottom,
		  /*[out]*/ float & right,
		  /*[out]*/ float & top)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetBoundingBox (/*[in]*/ int	shrinkFactor, 
		  /*[out]*/ int & left,
		  /*[out]*/ int & bottom,
		  /*[out]*/ int & right,
		  /*[out]*/ int & top)
    = 0;
};

/* _________________________________________________________________________

   GraphicsInclusion
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
GraphicsInclusion
{
protected:
                                                                                                                                                                                                    virtual
  MIKTEXTHISCALL
  ~GraphicsInclusion ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  AddRef ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Release ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Render (/*[in]*/ HDC hdc)
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

   DviPageMode
   _________________________________________________________________________ */

class DviPageModeEnum
{
public:
  enum EnumType {
    None,
    Pk,
    Dvips
  };
};

typedef MiKTeX::Core::EnumWrapper<DviPageModeEnum> DviPageMode;

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
  MIKTEXTHISCALL
  GetDviBitmap (/*[in]*/ int shrinkFactor,
		/*[in]*/ int idx)
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetNumberOfDviBitmaps (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  DviSpecial *
  MIKTEXTHISCALL
  GetSpecial (/*[in]*/ int idx)
    = 0;

public:
  virtual
  DviRule *
  MIKTEXTHISCALL
  GetRule (/*[in]*/ int idx)
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetReg (/*[in]*/ int idx)
    = 0;

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetName ()
    = 0;

public:
  virtual
  unsigned long
  MIKTEXTHISCALL
  GetBackgroundColor ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Lock ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Unlock ()
    = 0;

public:
  virtual
  HypertexSpecial *
  MIKTEXTHISCALL
  GetNextHyperref (/*[out]*/ int & idx)
    = 0;

public:
  virtual
  MiKTeX::Graphics::DibChunk *
  MIKTEXTHISCALL
  GetDibChunk (/*[in]*/ int shrinkFactor,
	       /*[in]*/ int idx)
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetNumberOfDibChunks (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  DviPageMode
  MIKTEXTHISCALL
  GetDviPageMode ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetNumberOfGraphicsInclusions (/*[in]*/ int shrinkFactor)
    = 0;

public:
  virtual
  GraphicsInclusion *
  MIKTEXTHISCALL
  GetGraphicsInclusion (/*[in]*/ int shrinkFactor,
			/*[in]*/ int idx)
    = 0;

};

/* _________________________________________________________________________

   DviFontInfo
   _________________________________________________________________________ */

struct DviFontInfo
{
  bool		notLoadable;
  bool		isVirtualFont;
  std::string	name;
  std::string	fileName;
  std::string	transcript;
  std::string	comment;
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

   IDviCallback
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
IDviCallback
{
public:
  virtual
  void
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  ~Dvi ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Dispose ()
    = 0;

public:
  static
  MIKTEXDVICEEAPI(Dvi*)
  Create (/*[in]*/ const char *		lpszFileName,
	  /*[in]*/ const char *		lpszMetafontMode,
	  /*[in]*/ int			resolution,
	  /*[in]*/ int			shrinkFactor,
	  /*[in]*/ DviAccess		access,
	  /*[in]*/ IDviCallback *	pCallback);

public:
  static
  MIKTEXDVICEEAPI(Dvi*)
  Create (/*[in]*/ const char *				lpszFileName,
	  /*[in]*/ const char *				lpszMetafontMode,
	  /*[in]*/ int					resolution,
	  /*[in]*/ int					shrinkFactor,
	  /*[in]*/ DviAccess				access,
	  /*[in]*/ DviPageMode				pageMode,
	  /*[in]*/ const MiKTeX::Core::PaperSizeInfo &	defaultPaperSizeInfo,
	  /*[in]*/ bool					landscape,
	  /*[in]*/ IDviCallback *			pCallback);

public:
  virtual
  int
  MIKTEXTHISCALL
  GetNumberOfPages ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetSource (/*[in]*/ const DviPosition &	pos,
	     /*[out]*/ MiKTeX::Core::PathName &	fileName,
	     /*[out]*/ int *			pLineNum = 0)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindSource (/*[in]*/ const char *	lpszFileName,
	      /*[in]*/ int		line,
	      /*[out]*/ DviPosition &	position)
    = 0;

public:
  virtual
  DviPage *
  MIKTEXTHISCALL
  GetPage (/*[in]*/ int pageIdx)
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetMinPageNumber ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetMaxPageNumber ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetMagnification ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetMaxH ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetMaxV ()
    = 0;

public:
  virtual
  PageStatus
  MIKTEXTHISCALL
  GetPageStatus (/*[in]*/ int pageIdx)
    = 0;

public:
  virtual
  DviPage *
  MIKTEXTHISCALL
  GetLoadedPage (/*[in]*/ int pageIdx)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindHyperLabel (/*[in]*/ const char *		lpszLabel,
		  /*[out]*/ DviPosition &	position)
    = 0;

public:
  virtual
  std::string
  MIKTEXTHISCALL
  GetStatusText ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  MakeFonts ()
    = 0;

public:
  virtual
  std::vector<DviFontInfo>
  MIKTEXTHISCALL
  GetFontTable ()
    = 0;

public:
  virtual
  MiKTeX::Core::PaperSizeInfo
  MIKTEXTHISCALL
  GetPaperSizeInfo ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  Landscape ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Lock ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Unlock ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Scan ()
    = 0;
};

/* _________________________________________________________________________

   AutoUnlockPage
   _________________________________________________________________________ */

class UnlockDviPage_
{
public:
  void
  operator() (/*[in]*/ DviPage * p)
  {
    p->Unlock ();
  }
};

typedef MiKTeX::Core::AutoResource<DviPage *, UnlockDviPage_> AutoUnlockPage;

/* _________________________________________________________________________ */

MIKTEX_DVI_END_NAMESPACE;

#undef MIKTEX_DVI_BEGIN_NAMESPACE
#undef MIKTEX_DVI_END_NAMESPACE

#endif
