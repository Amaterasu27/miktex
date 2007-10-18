/* common.h: internal DVI definitions				-*- C++ -*-

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

#if defined(_MSC_VER) && defined(_WIN32)
#  define MIKTEXDVIEXPORT __declspec(dllexport)
#else
#  define MIKTEXDVIEXPORT
#endif

#define A86DAFD3F286A340B6BD420E71BCD906
#include "MiKTeX/DVI/Dvi"

using namespace MiKTeX::Core;
using namespace MiKTeX::Graphics;
using namespace MiKTeX::DVI;
using namespace MiKTeX;
using namespace std;

class DviFont;
class DviImpl;

#if 1
#  define DEFAULT_PAGE_MODE DviPageMode::Pk
#else
#  define DEFAULT_PAGE_MODE DviPageMode::Auto
#endif

/* _________________________________________________________________________

   Debug-dependant Macros
   _________________________________________________________________________ */

#if ! defined(UNUSED)
#  if ! defined(NDEBUG)
#    define UNUSED(x)
#  else
#    define UNUSED(x) static_cast<void>(x)
#  endif
#endif

#if ! defined(UNUSED_ALWAYS)
#  define UNUSED_ALWAYS(x) static_cast<void>(x)
#endif

/* _________________________________________________________________________

   Error Macros
   _________________________________________________________________________ */

#define BUF_TOO_SMALL(function)						\
  FATAL_MIKTEX_ERROR (function,						\
                      T_("Not enough room in an internal buffer."),	\
                      0)

#define INVALID_ARGUMENT(function, param)				\
  FATAL_MIKTEX_ERROR (function, T_("Invalid argument."), param)

#define OUT_OF_MEMORY(function)					\
  FATAL_MIKTEX_ERROR(function, T_("Virtual memory exhausted."), 0)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define UNIMPLEMENTED(function)						\
  FATAL_MIKTEX_ERROR (function, T_("Function not implemented."), 0)

#define MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  TraceMiKTeXError (miktexFunction,				\
                    traceMessage,				\
                    lpszInfo,					\
                    T_(__FILE__),				\
		    __LINE__)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     T_(__FILE__),				\
			     __LINE__)

#define CRT_ERROR(lpszCrtFunction, lpszInfo)		\
  TraceStream::TraceLastCRTError (lpszCrtFunction,	\
				  lpszInfo,		\
				  T_(__FILE__),		\
				  __LINE__)

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,		\
			  lpszInfo,			\
			  T_(__FILE__),			\
			  __LINE__)

#define FATAL_CRT_ERROR_2(lpszCrtFunction, errorCode, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,			\
			  errorCode,				\
			  lpszInfo,				\
			  T_(__FILE__),				\
			  __LINE__)

#if defined(MIKTEX_WINDOWS)
#  define WINDOWS_ERROR(lpszWindowsFunction, lpszInfo)		\
  TraceStream::TraceLastWin32Error (lpszWindowsFunction,	\
				    lpszInfo,			\
				    T_(__FILE__),		\
				    __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,			\
			      lpszInfo,				\
			      T_(__FILE__),			\
			      __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,				\
			      errorCode,				\
			      lpszInfo,					\
			      T_(__FILE__),				\
			      __LINE__)
#endif

#define FATAL_DVI_ERROR(miktexFunction, traceMessage, lpszInfo) \
  FatalDviError (miktexFunction,				\
		 traceMessage,					\
		 lpszInfo,					\
		 T_(__FILE__),					\
		 __LINE__)

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

#define STATICFUNC(type) static type
#define INTERNALFUNC(type) type

#define DVITRACE DviTrace

#if ! defined(UNUSED)
#  if defined(_DEBUG)
#    define UNUSED(x)
#  else
#    define UNUSED(x) x
#  endif
#endif

/* _________________________________________________________________________

   FontMap
   _________________________________________________________________________ */

typedef map<int, DviFont *> FontMap;

/* _________________________________________________________________________

   DviPoint
   _________________________________________________________________________ */

struct DviPoint
{
public:
  DviPoint (/*[in]*/ int x = 0,
	    /*[in]*/ int y = 0)
    : x (x),
      y (y)
  {
  }
public:
  int x;
public:
  int y;
};

/* _________________________________________________________________________

   MAPNUMTOPOINT
   _________________________________________________________________________ */

typedef map<int, DviPoint> MAPNUMTOPOINT;

/* _________________________________________________________________________

   Includes
   _________________________________________________________________________ */

#include "DviChar.h"
#include "DviFont.h"
#include "PkChar.h"
#include "PkFont.h"
#include "Tfm.h"
#include "VFont.h"
#include "VfChar.h"

/* _________________________________________________________________________

   Prototypes
   _________________________________________________________________________ */

void
MIKTEXNORETURN
FatalDviError (/*[in]*/ const char *	lpszMiktexFunction,
	       /*[in]*/ const char *	lpszMessage,
	       /*[in]*/ const char *	lpszInfo,
	       /*[in]*/ const char *	lpszSourceFile,
	       /*[in]*/ int			sourceLine);

int
ScaleFix (/*[in]*/ int tfm,
	  /*[in]*/ int z);

void
ExpandBoundingBox (/*[in]*/ int llx,
		   /*[in]*/ int lly,
		   /*[in]*/ int urx,
		   /*[in]*/ int ury);

int
CalculateWidth (/*[in]*/ float			width,
		/*[in]*/ const char *	unit,
		/*[in]*/ int			resolution);

/* _________________________________________________________________________

   CmykColor
   _________________________________________________________________________ */

struct CmykColor
{
  double cyan;
  double magenta;
  double yellow;
  double black;
  operator
  unsigned long ();
};

/* _________________________________________________________________________

   RgbColor
   _________________________________________________________________________ */

struct RgbColor
{
  double red;
  double blue;
  double green;
  operator
  unsigned long ();
};

const unsigned long rgbDefaultColor = 0;	// black

/* _________________________________________________________________________

   HsbColor
   _________________________________________________________________________ */

struct HsbColor
{
  double hue;
  double saturation;
  double brightness;
  operator
  unsigned long ();
};

/* _________________________________________________________________________

   AutoRestore
   _________________________________________________________________________ */

template<class VALTYPE>
class AutoRestore
{
public:
  AutoRestore (/*[in]*/ VALTYPE & val)
    : oldVal (val),
      pVal (&val)
  {
  }

public:
  ~AutoRestore ()
  {
    *pVal = oldVal;
  }

private:
  VALTYPE oldVal;

private:
  VALTYPE * pVal;
};

/* _________________________________________________________________________

   DviItem
   _________________________________________________________________________ */

struct DviItem
{
public:
  int x;

public:
  int y;

public:
  unsigned long rgbForeground;

public:
  unsigned long rgbBackground;

public:
  PkChar * pPkChar;

public:
  inline
  int
  GetWidthShr (/*[in]*/ int shrinkFactor);

public:
  inline
  int
  GetWidthUns ();

public:
  inline
  int
  GetHeightShr (/*[in]*/ int shrinkFactor);

public:
  inline
  int
  GetHeightUns ();

public:
  inline
  int
  GetLeftShr (/*[in]*/ int shrinkFactor);

public:
  inline
  int
  GetLeftUns ();

public:
  int
  GetRightShr (/*[in]*/ int shrinkFactor)
  {
    return (GetLeftShr(shrinkFactor) + GetWidthShr(shrinkFactor) - 1);
  }

public:
  int
  GetRightUns ()
  {
    return (GetLeftUns() + GetWidthUns() - 1);
  }

public:
  inline
  int
  GetTopShr (/*[in]*/ int shrinkFactor);

public:
  inline
  int
  GetTopUns ();

public:
  int
  GetBottomShr (/*[in]*/ int shrinkFactor)
  {
    return (GetTopShr(shrinkFactor) + GetHeightShr(shrinkFactor) - 1);
  }

public:
  int
  GetBottomUns ()
  {
    return (GetTopUns() + GetHeightUns() - 1);
  }
};

/* _________________________________________________________________________

   InputStream
   _________________________________________________________________________ */

class InputStream
{
public:
  InputStream (/*[in]*/ const char * lpszFileName);

public:
  InputStream (/*[in]*/ const BYTE *	pBytes,
	       /*[in]*/ size_t		nBytes);

public:
  ~InputStream ();

public:
  bool
  Read (/*[out]*/ void *	pBytes,
	/*[in]*/ size_t		nBytes,
	/*[in]*/ bool		allowEof = false);
  
public:
  short
  ReadByte ()
  {
    Read (buffer, 1);
    return (static_cast<short>(buffer[0] & 0xff));
  }

public:
  bool
  TryToReadByte (/*[out]*/ int & by);

public:
  short
  ReadSignedByte ()
  {
    Read (buffer, 1);
    return (buffer[0]);
  }

public:
  long
  ReadPair ()
  {
    Read (buffer, 2);
    return (MakePair(buffer));
  }

public:
  long
  ReadSignedPair ()
  {
    Read (buffer, 2);
    return (MakeSignedPair(buffer));
  }

public:
  long
  ReadTrio ()
  {
    Read (buffer, 3);
    return (MakeTrio(buffer));
  }

public:
  long
  ReadSignedTrio ()
  {
    Read (buffer, 3);
    return (MakeSignedTrio(buffer));
  }

public:
  long
  ReadSignedQuad ()
  {
    Read (buffer, 4);
    return (MakeSignedQuad(buffer));
  }

public:
  void
  SkipBytes (/*[in]*/ long count);

public:
  void
  SetReadPosition (/*[in]*/ long	offset,
		   /*[in]*/ SeekOrigin	origin);
  
public:
  long
  GetReadPosition ();

public:
  bool
  IsEndOfStream ()
  {
    if (pBytes != 0)
      {
	return (idx == nBytes);
      }
    else
      {
	UNEXPECTED_CONDITION (T_("InputStream::IsEndOfStream"));
      }
  }

protected:
  short
  MakeByte (/*[in]*/ const char * ptr)
  {
    return (static_cast<short>(*ptr & 0xff));
  }

protected:
  short
  MakeSignedByte (/*[in]*/ const char * ptr)
  {
    return (*ptr);
  }

protected:
  long
  MakePair (/*[in]*/ const char * ptr)
  {
    return ((MakeByte(ptr) << 8 | MakeByte(ptr + 1)) & 0xffff);
  }

protected:
  long
  MakeSignedPair (/*[in]*/ const char * ptr)
  {
    return (MakeSignedByte(ptr) << 8 | MakeByte(ptr + 1));
  }

protected:
  long
  MakeTrio (/*[in]*/ const char * ptr)
  {
    return (((static_cast<long>(MakeByte(ptr)) << 16)
	     | MakePair(ptr + 1)) & 0x00ffffff);
  }

protected:
  long
  MakeSignedTrio (/*[in]*/ const char * ptr)
  {
    return ((static_cast<long>(MakeSignedByte(ptr)) << 16)
	    | MakePair(ptr + 1));
  }

protected:
  long
  MakeSignedQuad (/*[in]*/ const char * ptr)
  {
    return (MakeSignedPair(ptr) << 16 | MakePair(ptr + 2));
  }

protected:
  char buffer[4];

protected:
  PathName fileName;

protected:
  FileStream stream;

protected:
  char * pBytes;

protected:
  size_t nBytes;

protected:
  size_t idx;
};

/* _________________________________________________________________________

   DviRuleImpl
   _________________________________________________________________________ */

class DviRuleImpl : public DviRule
{
public:

public:
  virtual
  int
  MIKTEXDVICALL
  GetLeft (/*[in]*/ int shrinkFactor);

public:
  virtual
  int
  MIKTEXDVICALL
  GetRight (/*[in]*/ int shrinkFactor);

public:
  virtual
  int
  MIKTEXDVICALL
  GetTop (/*[in]*/ int shrinkFactor);

public:
  virtual
  int
  MIKTEXDVICALL
  GetBottom (/*[in]*/ int shrinkFactor);

public:
  virtual
  unsigned long
  MIKTEXDVICALL
  GetBackgroundColor ();

public:
  virtual
  unsigned long
  MIKTEXDVICALL
  GetForegroundColor ();

public:
  virtual
  bool
  MIKTEXDVICALL
  IsBlackboard ();

public:
  DviRuleImpl (/*[in]*/ DviImpl *	pDviImpl,
	       /*[in]*/ int		x,
	       /*[in]*/ int		y,
	       /*[in]*/ int		width,
	       /*[in]*/ int		height,
	       /*[in]*/ unsigned long	rgb);
  
public:
  enum Flag { flblackboard = 1 };

public:
  void
  SetFlag (/*[in]*/ Flag fl)
  {
    flags |= static_cast<short>(fl);
  }

public:
  void
  ClearFlag (/*[in]*/ Flag fl)
  {
    flags &= ~static_cast<short>(fl);
  }

public:
  bool
  GetFlag (/*[in]*/ Flag fl)
  {
    return (static_cast<short>(fl) & flags ? true : false);
  }

public:
  int
  GetLeftUns ()
  {
    return (x);
  }

public:
  int
  GetRightUns ()
  {
    return (x + width - 1);
  }

public:
  int
  GetTopUns ()
  {
    return (y - height + 1);
  }

public:
  int
  GetBottomUns ()
  {
    return (y);
  }

private:
  short flags;		// <fixme/>

private:
  int x;

private:
  int y;

private:
  int width;

private:
  int height;

private:
  unsigned long rgb;

private:
  DviImpl * pDviImpl;
};

/* _________________________________________________________________________

   DviPageImpl
   _________________________________________________________________________ */

class DviPageImpl
  : public DviPage,
    public IDibChunkerCallback
{
public:
  virtual
  const DviBitmap &
  MIKTEXDVICALL
  GetDviBitmap (/*[in]*/ int shrinkFactor,
		/*[in]*/ int idx);

public:
  virtual
  int
  MIKTEXDVICALL
  GetNumberOfDviBitmaps (/*[in]*/ int shrinkFactor);

public:
  virtual
  DviSpecial *
  MIKTEXDVICALL
  GetSpecial (/*[in]*/ int idx);

public:
  virtual
  DviRule *
  MIKTEXDVICALL
  GetRule (/*[in]*/ int idx);

public:
  virtual
  int
  MIKTEXDVICALL
  GetReg (/*[in]*/ int idx);

public:
  virtual
  const char *
  MIKTEXDVICALL
  GetName ();

public:
  virtual
  unsigned long
  MIKTEXDVICALL
  GetBackgroundColor ();

public:
  virtual
  void
  MIKTEXDVICALL
  Lock();

public:
  virtual
  void
  MIKTEXDVICALL
  Unlock();

public:
  virtual
  HypertexSpecial *
  MIKTEXDVICALL
  GetNextHyperref (/*[in,out]*/ int & idx);

public:
  virtual
  DibChunk *
  MIKTEXDVICALL
  GetDibChunk (/*[in]*/ int shrinkFactor,
	       /*[in]*/ int idx);

public:
  virtual
  int
  MIKTEXDVICALL
  GetNumberOfDibChunks (/*[in]*/ int shrinkFactor);

public:
  DviImpl *
  GetDviObject ();

public:
  void
  Freeze (/*[in]*/ bool force = false);

public:
  void
  FreeContents (/*[in]*/ bool keepSpecials = false,
		/*[in]*/ bool keepItems = false);

public:
  bool
  IsFrozen ()
  {
    return (frozen);
  }

public:
  bool
  IsLocked ()
  {
    return (criticalSectionMonitor.IsLocked());
  }

public:
  size_t
  GetSize ()
  {
    return (size);
  }

public:
  static
  size_t
  GetTotalSize ()
  {
    return (totalSize);
  }

public:
  time_t
  GetTimeLastVisit ()
  {
    return (lastVisited);
  }

public:
  template<class T>
  T *
  GetNextSpecial (/*[in]*/ int & idx)
  {
    MIKTEX_ASSERT (IsLocked());
    MIKTEX_ASSERT (idx >= -1 && idx < static_cast<int>(dviSpecials.size()));
    for (int j = idx + 1; j < static_cast<int>(dviSpecials.size()); ++ j)
      {
	T * pSpecial =
	  dynamic_cast<T *>(dviSpecials[j]);
	if (pSpecial != 0)
	  {
	    idx = j;
	    return (pSpecial);
	  }
      }
    return (0);
  }

private:
  DviPageImpl (/*[in]*/ DviImpl *	pDvi,
	       /*[in]*/ int		pageIdx,
	       /*[in]*/ DviPageMode	pageMode,
	       /*[in]*/ long		readPosition,
	       /*[in]*/ int		c0 = 0,
	       /*[in]*/ int		c1 = 0,
	       /*[in]*/ int		c2 = 0,
	       /*[in]*/ int		c3 = 0,
	       /*[in]*/ int		c4 = 0,
	       /*[in]*/ int		c5 = 0,
	       /*[in]*/ int		c6 = 0,
	       /*[in]*/ int		c7 = 0,
	       /*[in]*/ int		c8 = 0,
	       /*[in]*/ int		c9 = 0);

private:
  void
  SetAutoClean (/*[in]*/ bool enabled = true)
  {
    autoClean = enabled;
  }
  
private:
  virtual
  ~DviPageImpl ();

private:
  void
  AddItem (/*[in]*/ const DviItem & item);

private:
  void
  AddSpecial (/*[in]*/ DviSpecial * pSpecial);

private:
  void
  AddRule (/*[in]*/ DviRuleImpl * pRule);

private:
  void
  MakeShrinkedRaster (/*[in]*/ int shrinkFactor);

private:
  void
  MakeDviBitmaps (/*[in]*/ int shrinkFactor);

private:
  void
  ProcessBand (/*[in]*/ int			shrinkFactor,
	       /*[in]*/ vector<DviItem *> &	vecDviItemPtr);

private:
  void
  MakeDviBitmap (/*[in]*/ int				shrinkFactor,
		 /*[in]*/ DviBitmap &			bitmap,
		 /*[in]*/ vector<DviItem *>::iterator	ititemptrBegin,
		 /*[in]*/ vector<DviItem *>::iterator	ititemptrEnd);

private:
  void
  CheckRules ();

private:
  void
  DestroyDviBitmaps ();

private:
  void
  DestroyDibChunks ();

private:
  void
  MakeDibChunks (/*[in]*/ int shrinkFactor);

private:
  Process *
  StartDvips ();

private:
  static
  void
  MIKTEXCALLBACK
  DvipsTranscriptReader (/*[in]*/ void * p);

private:
  static
  void
  MIKTEXCALLBACK
  GhostscriptTranscriptReader (/*[in]*/ void * p);

public:
  virtual
  size_t
  DIBCHUNKERCALL
  Read (/*[out]*/ void *	pBuf,
	/*[in]*/ size_t		size);

public:
  virtual
  void
  DIBCHUNKERCALL
  OnNewChunk (/*[in]*/ DibChunk * pChunk);

private:
  Process *
  StartGhostscript (/*[in]*/ int shrinkFactor);

private:
  int
  GetReadPosition ()
  {
    return (readPosition);
  }

private:
  inline
  int
  PixelShrink (/*[in]*/ int shrinkFactor,
	       /*[in]*/ int pxl); // <fixme/>

private:
  inline
  int
  WidthShrink (/*[in]*/ int shrinkFactor,
	       /*[in]*/ int pxl); // <fixme/>

private:
  auto_ptr<TraceStream> tracePage;

private:
  auto_ptr<TraceStream> traceBitmap;

private:
  unsigned long backgroundColor;

  // position within DVI file
private:
  long readPosition;

  // page numbers
private:
  int counts[10];

  // page index
private:
  int pageIdx;

  // item vector
private:
  vector<DviItem> dviItems;

  // special vector
private:
  vector<DviSpecial *> dviSpecials;

  // rule vector
private:
  vector<DviRuleImpl *> dviRules;

private:
  map<int, bool> haveShrinkedRaster;

private:
  time_t lastVisited;

private:
  string pageName;

private:
  bool frozen;

private:
  map<int, vector<DviBitmap> > shrinkedDviBitmaps;

private:
  map<int, vector<SmartPointer<DibChunk> > > shrinkedDibChunks;

private:
  int dibShrinkFactor;

private:
  string dvipsTranscript;

private:
  string gsTranscript;

private:
  DviImpl * pDviImpl;

private:
  CriticalSectionMonitor criticalSectionMonitor;

private:
  size_t size;

private:
  bool autoClean;

private:
  long nLocks;

private:
  DviPageMode pageMode;

private:
  FileStream dvipsOut;

private:
  FileStream dvipsErr;

private:
  FileStream gsOut;

private:
  FileStream gsErr;

private:
  static size_t totalSize;

private:
  friend DviImpl;	// <fixme/>
};

/* _________________________________________________________________________

   DviInfo
   _________________________________________________________________________ */

struct DviInfo
{
  string		comment;
  long			nPages;
  time_t		lastWriteTime;
};

/* _________________________________________________________________________

   DviImpl
   _________________________________________________________________________ */

class DviImpl : public Dvi
{
public:
  virtual
  int
  MIKTEXDVICALL
  GetNumberOfPages ();

public:
  virtual
  bool
  MIKTEXDVICALL
  GetSource (/*[in]*/ const DviPosition &	pos,
	     /*[out]*/ PathName &		fileName,
	     /*[out]*/ int *			pLineNum);

public:
  virtual
  bool
  MIKTEXDVICALL
  FindSource (/*[in]*/ const char *	lpszFileName,
	      /*[in]*/ int			line,
	      /*[out]*/ DviPosition &		position);

public:
  virtual
  DviPage *
  MIKTEXDVICALL
  GetPage (int pageidx);

public:
  virtual
  int
  MIKTEXDVICALL
  GetMinPageNumber ();

public:
  virtual
  int
  MIKTEXDVICALL
  GetMaxPageNumber ();

public:
  virtual
  int
  MIKTEXDVICALL
  GetMagnification ();

public:
  virtual
  int
  MIKTEXDVICALL
  GetMaxH ();

public:
  virtual
  int
  MIKTEXDVICALL
  GetMaxV ();

public:
  virtual
  PageStatus
  MIKTEXDVICALL
  GetPageStatus (/*[in]*/ int pageidx);

public:
  virtual
  DviPage *
  MIKTEXDVICALL
  GetLoadedPage (/*[in]*/ int pageno);

public:
  virtual
  bool
  MIKTEXDVICALL
  FindHyperLabel (/*[in]*/ const char *		lpszLabel,
		  /*[out]*/ DviPosition &	position);
  
public:
  virtual
  string
  MIKTEXDVICALL
  GetStatusText ();

public:
  virtual
  bool
  MIKTEXDVICALL
  MakeFonts ();

public:
  virtual
  vector<DviFontInfo>
  MIKTEXDVICALL
  GetFontTable ();

public:
  virtual
  PaperSizeInfo
  MIKTEXDVICALL
  GetPaperSizeInfo ()
  {
    return (paperSizeInfo);
  }

public:
  virtual
  bool
  MIKTEXDVICALL
  Landscape ()
  {
    return (landscape);
  }

public:
  virtual
  void
  MIKTEXDVICALL
  Lock();

public:
  virtual
  void
  MIKTEXDVICALL
  Unlock();

public:
  virtual
  void
  MIKTEXDVICALL
  Scan ();

private:
  DviImpl (/*[in]*/ const char *		lpszFileName,
	   /*[in]*/ const char *		lpszMetafontMode,
	   /*[in]*/ int				resolution,
	   /*[in]*/ int				shrinkFactor,
	   /*[in]*/ DviAccess			access,
	   /*[in]*/ DviPageMode			pageMode,
	   /*[in]*/ const PaperSizeInfo &	paperSizeInfo,
	   /*[in]*/ bool			landscape);

private:
  virtual
  MIKTEXDVICALL
  ~DviImpl ();

public:
  virtual
  void
  MIKTEXCALL
  Dispose ();

private:
  friend class Dvi;		// <fixme/>

private:
  void
  CheckCondition ();
  
public:
  void
  Progress
  (/*[in]*/ DviNotification		nf,
   /*[in]*/ const char *		lpszFormat,
   /*[in]*/				...);

public:
  int
  PixelShrink (/*[in]*/ int shrinkFactor,
	       /*[in]*/ int pxl)
  {
    return (pxl / shrinkFactor);
  }

public:
  int
  WidthShrink (/*[in]*/ int shrinkFactor,
	       /*[in]*/ int pxl)
  {
    return (PixelShrink(shrinkFactor, pxl + shrinkFactor - 1));
  }

public:
  int
  GetBitsPerPixel (/*[in]*/ int shrinkFactor)
  {
    return (shrinkFactor == 1 ? 1 : 4);
  }

public:
  int
  GetBytesPerLine (/*[in]*/ int shrinkFactor,
		   /*[in]*/ int width)
  {
    return ((((width * GetBitsPerPixel(shrinkFactor)) + 31) / 32) * 4);
  }

public:
  int
  GetPixelsPerByte (/*[in]*/ int shrinkFactor)
  {
    return (8 / GetBitsPerPixel(shrinkFactor));
  }

public:
  int
  GetResolution ()
  {
    return (resolution);
  }

public:
  bool
  HaveLandscapeSpecial ()
  {
    return (haveLandscapeSpecial);
  }

public:
  bool
  HavePaperSizeSpecial ()
  {
    return (havePaperSizeSpecial);
  }

public:
  string
  GetMetafontMode ()
  {
    return (metafontMode);
  }

public:
  PathName
  GetDviFileName ()
  {
    return (dviFileName);
  }

public:
  bool
  IsLocked ()
  {
    return (criticalSectionMonitor.IsLocked());
  }

public:
  MAPNUMTOPOINT &
  GetPoints ()
  {
    return (pointTable);
  }

public:
  unsigned
  GetLineWidth ()
  {
    return (lineWidth);
  }

public:
  unsigned long
  GetCurrentColor ()
  {
    return (currentColor);
  }

public:
  DviPageMode
  GetPageMode ()
  {
    return (pageMode);
  }

private:
  bool
  InterpretSpecial (/*[in]*/ DviPageImpl *	pPage,
		    /*[in]*/ int		x,
		    /*[in]*/ int		y,
		    /*[in]*/ InputStream &	inputstream,
		    /*[in]*/ DWORD		p,
		    /*[out]*/ DviSpecial * &	pSpecial);

private:
  bool
  SetCurrentColor (/*[in]*/ const char * lpszColor);

private:
  bool
  ParseColorSpec (/*[in]*/ const char *	lpsz,
		  /*[in]*/ unsigned long &	rgb);

private:
  void
  PushColor (/*[in]*/ unsigned long rgb);

private:
  void
  PopColor ();

private:
  void
  ResetCurrentColor ();

private:
  int
  FirstParam (/*[in]*/ InputStream &	inputstream,
	      /*[in]*/ int		opcode); // <fixme/>

private:
  int
  PixelRound (/*[in]*/ int du); // <fixme/>

private:
  void
  DefineFont (/*[in]*/ InputStream &	inputstream,
	      /*[in]*/ int		fontnum);

private:
  void
  DoPage (/*[in]*/ int pageidx);

private:
  bool
  DoNextCommand (/*[in]*/ InputStream &	inputstream,
		 /*[in]*/ DviPageImpl &	page);

private:
  void
  SpecialCases (/*[in]*/ InputStream &	inputstream,
		/*[in]*/ int		opcode,
		/*[in]*/ int		p,
		/*[in]*/ DviPageImpl &	page); // <fixme/>

private:
  int
  RulePixels (/*[in]*/ int x);	// <fixme/>

private:
  void
  FreeContents (/*[in]*/ bool keepFonts = false);

private:
  void
  PushState ();

private:
  void
  PopState ();

private:
  void
  GetFontTable (/*[in]*/ const FontMap &	mapnumtofontptr,
		/*[out]*/ vector<DviFontInfo> &		vec,
		/*[in]*/ int				recursion);

private:
  bool
  MakeFonts (/*[in]*/ const FontMap &	mapnumtofontptr,
	     /*[in]*/ int			recursion);

private:
  double
  GetConv ()
  {
    return (conv);
  }

private:
  DviSpecial *
  ProcessHtmlSpecial (/*[in]*/ DviPageImpl *	ppage,
		      /*[in]*/ int		x,
		      /*[in]*/ int		y,
		      /*[in]*/ const char *	lpszSpecial);

private:
  float
  PatternToShadeLevel (/*[in]*/ const char * lpszTexture);

private:  
  static
  void
  MIKTEXCALLBACK
  GarbageCollector (/*[in]*/ void * p);

private:
  static
  void
  MIKTEXCALLBACK
  PageLoader (/*[in]*/ void * p);

private:
  HANDLE hByeByteEvent;

private:
  HANDLE hNewPageEvent;

private:
  HANDLE hScannedEvent;

private:
  int currentPageIdx;

private:
  int direction;

private:
  DviPageMode pageMode;

private:
  DviAccess dviAccess;

private:
  PaperSizeInfo paperSizeInfo;

private:
  bool havePaperSizeSpecial;

private:
  bool haveLandscapeSpecial;

private:
  bool landscape;

private:
  auto_ptr<Thread> pGarbageCollectorThread;

private:
  auto_ptr<Thread> pPageLoaderThread;

  // resolution in dots per inch
private:
  int resolution;

  // converts DVI units to pixels
private:
  double conv;

  // converts TFM units to DVI units
private:
  double tfmConv;

  // current font object
private:
  DviFont * pCurrentFont;

  // current char object
private:
  class DviChar * pCurrentChar;

  // current VfChar object
private:
  class VfChar * pCurrentVfChar;
  
  // stated conversion ratio
private:
  int numerator, denominator;

  // magnification factor times 1000
private:
  int mag;

  // the value of abs(v) should probably not exceed this
private:
  int maxV;

  // the value of abs(h) should probably not exceed this
private:
  int maxH;
  
private:
  string metafontMode;

private:
  vector<class DviPageImpl *> pages;

private:
  FontMap * pFontMap;

private:
  int currentFontNumber;

private:
  int recursion;

private:
  int minPageNumber;

private:
  int maxPageNumber;

private:
  DviInfo dviInfo;

private:
  PathName dviFileName;

private:
  PathName fqDviFileName;

private:
  int defaultShrinkFactor;

private:
  struct DviState
  {
  public:
    DviState ()
      : h (0),
	v (0),
	w (0),
	x (0),
	y (0),
	z (0),
	hh (0),
	vv (0)
    {
    }
  public:
    int h, v, w, x, y, z, hh, vv;
  };

  // current state
private:
  DviState currentState;

private:
  stack<DviState> stateStack;

private:
  stack<unsigned long> colorStack;

private:
  unsigned long currentColor;

  // last time the Dvi file was checked
private:
  clock_t lastChecked;

private:
  bool hasDviFileChanged;

private:
  MAPNUMTOPOINT pointTable;

private:
  unsigned lineWidth;

private:
  string progressStatus;

private:
  IDviCallback * pCallback;

private:
  CriticalSectionMonitor criticalSectionMonitor_StatusText;

private:
  CriticalSectionMonitor criticalSectionMonitor;

private:
  MiKTeXException miktexException;

private:
  exception stdException;

private:
  bool fatalMiKTeXError;

private:
  bool fatalError;

private:
  auto_ptr<TraceStream> log_color;

private:
  auto_ptr<TraceStream> log_dvifile;

private:
  auto_ptr<TraceStream> log_dvipage;

private:
  auto_ptr<TraceStream> log_error;

private:
  auto_ptr<TraceStream> log_gc;

private:
  auto_ptr<TraceStream> log_hypertex;

private:
  auto_ptr<TraceStream> log_search;
};

/* _________________________________________________________________________

   SpecialRoot
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
SpecialRoot
{
public:
  SpecialRoot ()
    : specialType (DviSpecialType::Unknown),
      log_error (TraceStream::Open(MIKTEX_TRACE_ERROR))
  {
  }

public:
  virtual
  ~SpecialRoot ()
  {
    try
      {
	if (log_error.get() != 0)
	  {
	    log_error->Close ();
	    log_error.reset ();
	  }
      }
    catch (const exception &)
      {
      }
  }

protected:
  DviPageImpl *
  GetPage ()
  {
    return (pDviPageImpl);
  }

protected:
  DviPageImpl * pDviPageImpl;

protected:
  int x, y;

protected:
  string specialString;

protected:
  DviSpecialType specialType;

protected:
  auto_ptr<TraceStream> log_error;
};

/* _________________________________________________________________________

   DviSpecialObject
   _________________________________________________________________________ */

template<class T>
class DviSpecialObject
  : public T
{
public:
  virtual
  ~DviSpecialObject ()
  {
  }

public:
  virtual
  int
  MIKTEXDVICALL
  GetX ()
  {
    return (x);
  }

public:
  virtual
  int
  MIKTEXDVICALL
  GetY ()
  {
    return (y);
  }

public:
  virtual
  const char *
  MIKTEXDVICALL
  GetXXX ()
  {
    return (specialString.c_str());
  }

public:
  virtual
  DviSpecialType
  MIKTEXDVICALL
  GetType ()
  {
    return (specialType);
  }

public:
  DviSpecialObject (/*[in]*/ DviPageImpl *		ppage,
		    /*[in]*/ int			x,
		    /*[in]*/ int			y,
		    /*[in]*/ const char *		lpszSpecial)
  {
    pDviPageImpl = ppage;
    this->x = x;
    this->y = y;
    if (lpszSpecial != 0)
      {
	specialString = lpszSpecial;
      }
    specialType = Parse();
  }
};

/* _________________________________________________________________________

   SolidLineSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
SolidLineSpecialImpl
  : public SolidLineSpecial,
    public SpecialRoot
{
public:
  unsigned long
  MIKTEXDVICALL
  GetColor ()
  {
    return (color);
  }

public:
  unsigned
  MIKTEXDVICALL
  GetWidth ()
  {
    return (width);
  }

public:
  int
  MIKTEXDVICALL
  GetStartX ()
  {
    return (xStart);
  }

public:
  int
  MIKTEXDVICALL
  GetStartY ()
  {
    return (yStart);
  }

public:
  int
  MIKTEXDVICALL
  GetEndX ()
  {
    return (xEnd);
  }

public:
  int
  MIKTEXDVICALL
  GetEndY ()
  {
    return (yEnd);
  }

protected:
  DviSpecialType
  Parse ();

protected:
  unsigned long color;

protected:
  unsigned width;

protected:
  int xStart;

protected:
  int yStart;

protected:
  int xEnd;

protected:
  int yEnd;
};

/* _________________________________________________________________________

   SourceSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
SourceSpecialImpl
  : public SourceSpecial,
    public SpecialRoot
{
public:
  const char *
  MIKTEXDVICALL
  GetFileName ()
  {
    return (fileName.c_str());
  }

public:
  int
  MIKTEXDVICALL
  GetLineNum ()
  {
    return (lineNum);
  }

public:
  int
  MIKTEXDVICALL
  GetId ()
  {
    return (id);
  }

protected:
  DviSpecialType
  Parse ();

protected:
  int id;

protected:
  string fileName;

protected:
  int lineNum;
};

/* _________________________________________________________________________

   TpicContext
   _________________________________________________________________________ */

struct TpicContext
{
public:
  TpicContext ()
  {
    Reset ();
  }

public:
  void
  Reset ()
  {
    tpicPath.clear ();
    shade = 0.5;
    penSize = 5;
  }

public:
  TpicSpecial::path tpicPath;

public:
  float shade;

public:
  int penSize;
};

/* _________________________________________________________________________

   TpicSpecialRoot
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
TpicSpecialRoot : public SpecialRoot
{
public:
  static TpicContext TpicSpecialRoot::tpicContext;
};

/* _________________________________________________________________________

   TpicSpecialObject
   _________________________________________________________________________ */

template<class T>
class
MIKTEXNOVTABLE
TpicSpecialObject : public T
{
public:
  const TpicSpecial::path &
  MIKTEXDVICALL
  GetPath ()
  {
    return (tpicPath);
  }

public:
  float
  MIKTEXDVICALL
  GetShade ()
  {
    return (shade);
  }

public:
  int
  MIKTEXDVICALL
  GetPenSize ()
  {
    return (penSize);
  }

protected:
  TpicSpecialObject ()
  {
    tpicPath = TpicSpecialRoot::tpicContext.tpicPath;
    shade = TpicSpecialRoot::tpicContext.shade;
    penSize = TpicSpecialRoot::tpicContext.penSize;
    TpicSpecialRoot::tpicContext.Reset ();
  }

protected:
  TpicSpecial::path tpicPath;

protected:
  float shade;

protected:
  int penSize;
};

/* _________________________________________________________________________

   TpicPolySpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
TpicPolySpecialImpl
  : public TpicPolySpecial,
    public TpicSpecialRoot
{
public:
  TpicPolySpecialImpl ()
    : m_outline (OutlineStyle::None)
  {
  }

public:
  OutlineStyle
  MIKTEXDVICALL
  GetOutlineStyle (/*[out]*/ float & length)
  {
    length = m_length;
    return (m_outline);
  }
  
public:
  bool
  MIKTEXDVICALL
  IsSpline ()
  {
    return (m_bSpline);
  }

protected:
  DviSpecialType
  Parse ();

protected:
  OutlineStyle m_outline;

protected:
  float m_length;

protected:
  bool m_bSpline;
};

/* _________________________________________________________________________

   TpicArcSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
TpicArcSpecialImpl
  : public TpicArcSpecial,
    public TpicSpecialRoot
{
public:
  int
  MIKTEXDVICALL
  GetCx ()
  {
    return (cx);
  }

public:
  int
  MIKTEXDVICALL
  GetCy ()
  {
    return (cy);
  }

public:
  int
  MIKTEXDVICALL
  GetRx ()
  {
    return (m_rx);
  }

public:
  int
  MIKTEXDVICALL
  GetRy ()
  {
    return (m_ry);
  }

public:
  float
  MIKTEXDVICALL
  GetS ()
  {
    return (m_s);
  }

public:
  float
  MIKTEXDVICALL
  GetE ()
  {
    return (m_e);
  }

public:
  bool
  MIKTEXDVICALL
  HasOutline ()
  {
    return (m_bOutline);
  }

protected:
  DviSpecialType
  Parse ();

protected:
  int cx, cy, m_rx, m_ry;

protected:
  float m_s, m_e;

protected:
  bool m_bOutline;
};

/* _________________________________________________________________________

   HyperTeXSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
HyperTeXSpecialImpl
  : public HypertexSpecial,
    public SpecialRoot
{
public:
  HyperTeXSpecialImpl ()
    : log_hypertex (TraceStream::Open(MIKTEX_TRACE_DVIHYPERTEX))
  {
  }

public:
  virtual
  ~HyperTeXSpecialImpl ()
  {
    try
      {
	if (log_hypertex.get() != 0)
	  {
	    log_hypertex->Close ();
	    log_hypertex.reset ();
	  }
      }
    catch (const exception &)
      {
      }
  }

public:
  const char *
  MIKTEXDVICALL
  GetName ()
  {
    return (name.c_str());
  }

public:
  void
  MIKTEXDVICALL
  GetBbox (/*[out]*/ int & llx,
	   /*[out]*/ int & lly,
	   /*[out]*/ int & urx,
	   /*[out]*/ int & ury)
  {
    llx = this->llx;
    lly = this->lly;
    urx = this->urx;
    ury = this->ury;
  }

public:
  bool
  MIKTEXDVICALL
  IsName ()
  {
    return (isName);
  }

protected:
  DviSpecialType
  Parse ();

protected:
  string name;

protected:
  int llx, lly, urx, ury;

protected:
  bool isName;

protected:
  auto_ptr<TraceStream> log_hypertex;

public:
  static struct State
  {
    string nameOrHref;
    string baseUrl;
    int llx, lly, urx, ury;
    bool isName;
    bool isHref;
  } state;
};

/* _________________________________________________________________________

   GraphicsSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
GraphicsSpecialImpl
  : public GraphicsSpecial,
    public SpecialRoot
{
public:
  GraphicsSpecialImpl ()
    : graphicsAction (GraphicsSpecialAction::None)
  {
  }

public:
  const char *
  MIKTEXDVICALL
  GetFileName ()
  {
    return (fileName.c_str());
  }

public:
  int
  MIKTEXDVICALL
  GetWidth ()
  {
    return (width);
  }

public:
  int
  MIKTEXDVICALL
  GetHeight ()
  {
    return (height);
  }

public:
  GraphicsSpecialAction
  MIKTEXDVICALL
  GetAction ()
  {
    return (graphicsAction);
  }

protected:
  DviSpecialType
  Parse ();

protected:
  string fileName;

protected:
  int width;

protected:
  int height;

protected:
  GraphicsSpecialAction graphicsAction;
};

/* _________________________________________________________________________

   PsdefSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
PsdefSpecialImpl
  : public PsdefSpecial,
    public SpecialRoot
{
public:
  const char *
  MIKTEXDVICALL
  GetDef ()
  {
    return (isFileName ? 0 : str.c_str());
  }

public:
  const char *
  MIKTEXDVICALL
  GetFileName ()
  {
    return (isFileName ? str.c_str() : 0);
  }

protected:
  DviSpecialType
  Parse ();

protected:
  string str;

protected:
  bool isFileName;
};

/* _________________________________________________________________________

   DvipsSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
DvipsSpecialImpl
  : public DvipsSpecial,
    public SpecialRoot
{
public:
  const char *
  MIKTEXDVICALL
  GetString ()
  {
    return (isFileName ? 0 : str.c_str());
  }

public:
  const char *
  MIKTEXDVICALL
  GetFileName ()
  {
    return (isFileName ? str.c_str() : 0);
  }

public:
  bool
  MIKTEXDVICALL
  GetProtection ()
  {
    return (protection);
  }

protected:
  DviSpecialType
  Parse ();

protected:
  string str;

protected:
  bool protection;

protected:
  bool isFileName;
};

/* _________________________________________________________________________

   PaperSizeSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
PaperSizeSpecialImpl
  : public PaperSizeSpecial,
    public SpecialRoot
{
public:
  PaperSizeInfo
  MIKTEXDVICALL
  GetPaperSizeInfo ()
  {
    return (paperSizeInfo);
  }

protected:
  DviSpecialType
  Parse ();

protected:
  PaperSizeInfo paperSizeInfo;
};

/* _________________________________________________________________________

   LandscapeSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
LandscapeSpecialImpl
  : public LandscapeSpecial,
    public SpecialRoot
{
protected:
  DviSpecialType
  Parse ()
  {
    return (DviSpecialType::Landscape);
  }
};

/* _________________________________________________________________________

   PsfileSpecialImpl
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
PsfileSpecialImpl
  : public PsfileSpecial,
    public SpecialRoot
{
public:
  const char *
  MIKTEXDVICALL
  GetFileName ()
  {
    return (fileName.c_str());
  }

public:
  int
  MIKTEXDVICALL
  GetHsize ()
  {
    return (hSize);
  }

public:
  int
  MIKTEXDVICALL
  GetVsize ()
  {
    return (vSize);
  }

public:
  int
  MIKTEXDVICALL
  GetHoffset ()
  {
    return (hOffset);
  }

public:
  int
  MIKTEXDVICALL
  GetVoffset ()
  {
    return (vOffset);
  }

public:
  int
  MIKTEXDVICALL
  GetHscale ()
  {
    return (hScale);
  }

public:
  int
  MIKTEXDVICALL
  GetVscale ()
  {
    return (vScale);
  }

public:
  int
  MIKTEXDVICALL
  GetAngke ()
  {
    return (angle);
  }

public:
  int
  MIKTEXDVICALL
  GetLlx ()
  {
    return (llx);
  }

public:
  int
  MIKTEXDVICALL
  GetLly ()
  {
    return (lly);
  }

public:
  int
  MIKTEXDVICALL
  GetUrx ()
  {
    return (urx);
  }

public:
  int
  MIKTEXDVICALL
  GetUry ()
  {
    return (ury);
  }

public:
  int
  MIKTEXDVICALL
  GetRwi ()
  {
    return (rwi);
  }

public:
  int
  MIKTEXDVICALL
  GetRhi ()
  {
    return (rhi);
  }

public:
  bool
  MIKTEXDVICALL
  IsClipped ()
  {
    return (isClipped);
  }

public:
  bool
  MIKTEXDVICALL
  HasHsize ()
  {
    return (hasHSize);
  }

public:
  bool
  MIKTEXDVICALL
  HasVsize ()
  {
    return (hasVSize);
  }

public:
  bool
  MIKTEXDVICALL
  HasHoffset ()
  {
    return (hasHOffset);
  }

public:
  bool
  MIKTEXDVICALL
  HasVoffset ()
  {
    return (hasVOffset);
  }

public:
  bool
  MIKTEXDVICALL
  HasHscale ()
  {
    return (hasHSale);
  }

public:
  bool
  MIKTEXDVICALL
  HasVscale ()
  {
    return (hasVScale);
  }

public:
  bool
  MIKTEXDVICALL
  HasAngle ()
  {
    return (hasAngle);
  }

public:
  bool
  MIKTEXDVICALL
  HasLlx ()
  {
    return (hasLlx);
  }

public:
  bool
  MIKTEXDVICALL
  HasLly ()
  {
    return (hasLLy);
  }

public:
  bool
  MIKTEXDVICALL
  HasUrx ()
  {
    return (hasUrx);
  }

public:
  bool
  MIKTEXDVICALL
  HasUry ()
  {
    return (hasUry);
  }

public:
  bool
  MIKTEXDVICALL
  HasRwi ()
  {
    return (hasRwi);
  }

public:
  bool
  MIKTEXDVICALL
  HasRhi ()
  {
    return (hasRhi);
  }

public:
  bool
  MIKTEXDVICALL
  HasClipFlag ()
  {
    return (hasClipFlag);
  }

public:
  bool
  MIKTEXDVICALL
  GetBoundingBox (/*[out]*/ float &	left,
		  /*[out]*/ float &	bottom,
		  /*[out]*/ float &	right,
		  /*[out]*/ float &	top);

public:
  bool
  MIKTEXDVICALL
  GetBoundingBox (/*[in]*/ int		shrinkFactor, 
		  /*[out]*/ int &	left,
		  /*[out]*/ int &	bottom,
		  /*[out]*/ int &	right,
		  /*[out]*/ int &	top);

protected:
  DviSpecialType
  Parse ();

protected:
  string fileName;

protected:
  int hSize, vSize;

protected:
  int hOffset, vOffset;

protected:
  int hScale, vScale;

protected:
  int angle;

protected:
  int llx, lly, urx, ury;

protected:
  int rwi, rhi;

protected:
  bool isClipped;

protected:
  bool hasHSize	: 1;

protected:
  bool hasVSize	: 1;

protected:
  bool hasHOffset	: 1;

protected:
  bool hasVOffset	: 1;

protected:
  bool hasHSale	: 1;

protected:
  bool hasVScale	: 1;

protected:
  bool hasAngle	: 1;

protected:
  bool hasLlx	: 1;

protected:
  bool hasLLy	: 1;

protected:
  bool hasUrx	: 1;

protected:
  bool hasUry	: 1;

protected:
  bool hasRwi	: 1;

protected:
  bool hasRhi	: 1;

protected:
  bool hasClipFlag	: 1;
};

#include "inliners.h"
