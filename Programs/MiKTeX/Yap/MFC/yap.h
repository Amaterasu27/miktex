/* yap.h: main header file for the Yap application		-*- C++ -*-

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

#pragma once

#include "resource.h"

#define YAP_TRACE_FLAGS_LVL_1			\
  MIKTEX_TRACE_ERROR ","			\
  MIKTEX_TRACE_PROCESS ","			\
  MIKTEX_TRACE_YAP

#define YAP_TRACE_FLAGS_LVL_2			\
  YAP_TRACE_FLAGS_LVL_1 ","			\
  MIKTEX_TRACE_DVIFILE ","			\
  MIKTEX_TRACE_DVIPKFONT

#define YAP_TRACE_FLAGS_LVL_3			\
  YAP_TRACE_FLAGS_LVL_2 ","			\
  MIKTEX_TRACE_DVIGC

// Windows 95/98/Me uses a 16-bit world coordinate system
#if defined(MIKTEX_WINDOWS_32) && defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
#  define SIXTEENBITGDI 1
#else
#  define SIXTEENBITGDI 0
#endif

#define USEMYSCROLLVIEW SIXTEENBITGDI
#define WIN95NOCONTVIEW 0

const bool DEFAULT_bRenderGraphicsInBackground = true;

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
                    __FILE__,				\
		    __LINE__)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     __FILE__,				\
			     __LINE__)

#define CRT_ERROR(lpszCrtFunction, lpszInfo)		\
  TraceStream::TraceLastCRTError (lpszCrtFunction,	\
				  lpszInfo,		\
				  __FILE__,		\
				  __LINE__)

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,		\
			  lpszInfo,			\
			  __FILE__,			\
			  __LINE__)

#define FATAL_CRT_ERROR_2(lpszCrtFunction, errorCode, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,			\
			  errorCode,				\
			  lpszInfo,				\
			  __FILE__,				\
			  __LINE__)

#if defined(MIKTEX_WINDOWS)
#  define WINDOWS_ERROR(lpszWindowsFunction, lpszInfo)		\
  TraceStream::TraceLastWin32Error (lpszWindowsFunction,	\
				    lpszInfo,			\
				    __FILE__,		\
				    __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,			\
			      lpszInfo,				\
			      __FILE__,			\
			      __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,				\
			      errorCode,				\
			      lpszInfo,					\
			      __FILE__,				\
			      __LINE__)
#endif

#define FATAL_DVI_ERROR(miktexFunction, traceMessage, lpszInfo) \
  FatalDviError (miktexFunction,				\
		 traceMessage,					\
		 lpszInfo,					\
		 __FILE__,					\
		 __LINE__)

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

/* _________________________________________________________________________

   EditorInfo
   _________________________________________________________________________ */

struct EditorInfo
{
public:
  EditorInfo (/*[in]*/ const string & name,
	      /*[in]*/ const string & program,
	      /*[in]*/ const string & arguments)
    : name (name),
      program (program),
      arguments (arguments)
  {
  }
public:
  EditorInfo (/*[in]*/ const char *	lpszName,
	      /*[in]*/ const char *	lpszProgram,
	      /*[in]*/ const char *	lpszArguments)
    : name (lpszName),
      program (lpszProgram),
      arguments (lpszArguments)
  {
  }
public:
  string name;
public:
  string program;
public:
  string arguments;
public:
  string
  GetCommandLine ()
    const
  {
    string ret = Q_(program);
    ret += ' ';
    ret += arguments;
    return (ret);
  }
};

/* _________________________________________________________________________

   Units
   _________________________________________________________________________ */

class UnitsEnum
{
public:
  enum EnumType {
    BigPoints,
    Centimeters,
    Inches,
    Millimeters,
    None,
    Picas,
  };
};

typedef EnumWrapper<UnitsEnum> Units;

/* _________________________________________________________________________

   YapConfig
   _________________________________________________________________________ */

class YapConfig
{
public:
  CString ghostscriptDevice;

public:
  bool maintainHorizontalPosition;

public:
  bool maintainVerticalPosition;

public:
  DWORD displayMetafontMode;

public:
  DWORD printerMetafontMode;

public:
  CString inverseSearchCommandLine;

public:
  int pageXShift;

public:
  int pageYShift;

public:
  int displayShrinkFactor;

public:
  bool showSourceSpecials;

public:
  CString dvipsExtraOptions;

public:
  int lastTool;

public:
  int magGlassOpacity;

public:
  bool magGlassHidesCursor;

public:
  int magGlassSmallWidth;

public:
  int magGlassSmallHeight;

public:
  int magGlassSmallShrinkFactor;

public:
  int magGlassMediumWidth;

public:
  int magGlassMediumHeight;

public:
  int magGlassMediumShrinkFactor;

public:
  int magGlassLargeWidth;

public:
  int magGlassLargeHeight;

public:
  int magGlassLargeShrinkFactor;

public:
  int enableShellCommands;

public:
  enum {
    SEC_DISABLE_COMMANDS = 0,
    SEC_ASK_USER = 1,
    SEC_SECURE_COMMANDS = 2,
    SEC_ENABLE_ALL_COMMANDS = 3,
  };

public:
  CString secureCommands;

public:
  bool checkFileTypeAssociations;

public:
#if defined(MIKTEX_DEBUG)
  bool showBoundingBoxes;
#endif

public:
  bool renderGraphicsInBackground;

public:
  double gamma;

public:
  CString gammaValues;

public:
  bool doublePage;

public:
  bool continuousView;

public:
  bool win95;

public:
  DviPageMode dviPageMode;

public:
  Units unit;

public:
  YapConfig ();

public:
  ~YapConfig ();

public:
  void
  Load ();

public:
  void
  Save ();

public:
  bool
  SetDisplayMetafontMode (/*[in]*/ const char * lpszMetafontMode);

public:
  bool
  SetPrinterMetafontMode (/*[in]*/ const char * lpszMetafontMode);
  
private:
  void
  RetrieveMetafontModes ();

private:
  void
  DestroyMetafontModes ();

private:
  template<typename ValueType>
  void
  PutValue (/*[in]*/ const char *	lpszKeyName,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const ValueType &	value)
  {
    pCfg->PutValue (lpszKeyName, lpszValueName, NUMTOSTR(value));
  }

private:
  template<>
  void
  PutValue (/*[in]*/ const char *	lpszKeyName,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const bool &	value)
  {
    pCfg->PutValue (lpszKeyName, lpszValueName, value ? "1" : "0");
  }

private:
  template<>
  void
  PutValue (/*[in]*/ const char *	lpszKeyName,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const CString &	value)
  {
    pCfg->PutValue (lpszKeyName, lpszValueName, value);
  }

private:
  template<typename ValueType>
  void
  UpdateValue (/*[in]*/ const char *		lpszKeyName,
	       /*[in]*/ const char *		lpszValueName,
	       /*[in]*/ const ValueType &	value,
	       /*[in]*/ const ValueType &	defaultValue)
  {
    if (value == defaultValue)
    {
      string s;
      if (pCfg->TryGetValue(lpszKeyName, lpszValueName, s))
      {
	pCfg->DeleteValue (lpszKeyName, lpszValueName);
      }
    }
    else
    {
      PutValue (lpszKeyName, lpszValueName, value);
    }
  }

private:
  template<typename ValueType>
  ValueType
  GetValue (/*[in]*/ const char *	lpszKeyName,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const ValueType &	defaultValue)
  {
    string value;
    if (pCfg->TryGetValue(lpszKeyName, lpszValueName, value))
    {
      return (static_cast<ValueType>(atoi(value.c_str())));
    }
    else
    {
      return (defaultValue);
    }
  }

private:
  template<>
  bool
  GetValue (/*[in]*/ const char *	lpszKeyName,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const bool &	defaultValue)
  {
    string value;
    if (pCfg->TryGetValue(lpszKeyName, lpszValueName, value))
    {
      return (atoi(value.c_str()) == 0 ? false : true);
    }
    else
    {
      return (defaultValue);
    }
  }

private:
  template<>
  double
  GetValue (/*[in]*/ const char *	lpszKeyName,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const double &	defaultValue)
  {
    string value;
    if (pCfg->TryGetValue(lpszKeyName, lpszValueName, value))
    {
      return (atof(value.c_str()));
    }
    else
    {
      return (defaultValue);
    }
  }

private:
  template<>
  CString
  GetValue (/*[in]*/ const char *	lpszKeyName,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const CString &	defaultValue)
  {
    string value;
    if (pCfg->TryGetValue(lpszKeyName, lpszValueName, value))
    {
      return (value.c_str());
    }
    else
    {
      return (defaultValue);
    }
  }


private:
  SmartPointer<Cfg> pCfg;
};

/* _________________________________________________________________________

   AutoDeleteObject
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
template<class OBJTYPE>
class DeleteObject_
{
public:
  void
  operator() (/*[in]*/ OBJTYPE * pObj)
  {
    pObj->DeleteObject ();
  }
};

typedef AutoResource<CGdiObject*, DeleteObject_<CGdiObject> > AutoDeleteObject;
#endif

/* _________________________________________________________________________

   AutoDetachMenu
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
template<class OBJTYPE>
class Detach_
{
public:
  void
  operator() (/*[in]*/ OBJTYPE * pObj)
  {
    pObj->Detach ();
  }
};

typedef AutoResource<CMenu*, Detach_<CMenu> > AutoDetachMenu;
#endif

/* _________________________________________________________________________

   AutoRestoreDC
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class RestoreDC_
{
public:
  void
  operator() (/*[in]*/ CDC *	pDC,
	      /*[in]*/ int	savedDC)
  {
    pDC->RestoreDC (savedDC);
  }
};

typedef AutoResource2<CDC *, int, RestoreDC_> AutoRestoreDC;
#endif

/* _________________________________________________________________________

   AutoSelectObject
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class SelectObject_
{
public:
  void
  operator() (/*[in]*/ CDC *		pDC,
	      /*[in]*/ CGdiObject *	pObj)
  {
    pDC->SelectObject (pObj);
  }
};

typedef AutoResource2<CDC *, CGdiObject *, SelectObject_> AutoSelectObject;
#endif

/* _________________________________________________________________________

   YapCommandLineInfo
   _________________________________________________________________________ */

class YapCommandLineInfo : public CCommandLineInfo
{
public:
  YapCommandLineInfo ()
    : sourceLineNum (-1),
      singleInstance (false)
  {
  }

public:
  CString sourceFile;

public:
  CString hyperLabel;

public:
  int sourceLineNum;

public:
  bool singleInstance;

public:
  CString traceFlags;
};

/* _________________________________________________________________________

   YapApplication
   _________________________________________________________________________ */

class YapApplication : public CWinApp
{
protected:
  DECLARE_MESSAGE_MAP();

public:
  YapApplication();

public:
  virtual
  BOOL
  InitInstance ();

public:
  virtual
  int
  ExitInstance ();

public:
  virtual
  BOOL
  OnDDECommand (/*[in]*/ LPTSTR lpszCommand);

public:
  CDocument *
  OpenDocumentFile (/*[in]*/ LPCTSTR lpszFileName);
  
protected:
  afx_msg
  void
  OnAppAbout ();

protected:
  afx_msg
  void
  OnViewTrace ();

protected:
  afx_msg
  void
  OnUpdateViewTrace (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnRegisterMiKTeX ();

private:
  void
  RegisterWindowClass ();

private:
  bool
  ActivateFirstInstance (/*[in]*/ const YapCommandLineInfo & cmdInfo);

private:
  bool
  FindSrcSpecial (/*[in]*/ int			lineNum,
		  /*[in]*/ const char *	lpszFileName);

private:
  bool
  GotoHyperLabel (/*[in]*/ const char * lpszLabel);

protected:
  bool tracing;

public:
  CString traceFlags;

private:
  SessionWrapper pSession;

public:
  auto_ptr<TraceStream> trace_yap;

public:
  auto_ptr<TraceStream> trace_error;
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

#if DVI_DONT_RENDER_GRAPHICS_SPECIALS
/* _________________________________________________________________________

   ReferenceCountedObjectRoot
   _________________________________________________________________________ */

class ReferenceCountedObjectRoot
{
public:
  ReferenceCountedObjectRoot ()
    : ref (0)
  {
  }

public:
  virtual
  ~ReferenceCountedObjectRoot ()
    = 0
  {
  }

public:
  void
  AddRef ()
  {
    ++ ref;
  }

public:
  void
  Release ()
  {
    -- ref;
    if (ref == 0)
      {
	delete this;
      }
  }

private:
  int ref;
};
#endif

#if DVI_DONT_RENDER_GRAPHICS_SPECIALS
/* _________________________________________________________________________

   TempFile
   _________________________________________________________________________ */

class TempFile
{
private:
  struct TempFile_ : public ReferenceCountedObjectRoot
  {
  public:
    TempFile_ ()
    {
    }

  public:
    TempFile_ (/*[in]*/ const PathName & path)
      : path (path)
    {
    }

  public:
    virtual
    ~TempFile_ ()
    {
      try
	{
	  Delete ();
	}
      catch (const exception &)
	{
	}
    }

  public:
    PathName path;

  public:
    void
    Create ()
    {
      Delete ();
      path.SetToTempFile ();
    }

  public:
    void
    Delete ()
    {
      if (! path.Empty())
	{
	  PathName tmp (path);
	  path = _T("");
	  File::Delete (tmp);
	}
    }
  };

public:
  TempFile ()
    : pTempFile_ (new TempFile_)
  {
  }

public:
  TempFile (/*[in]*/ const char * lpszPath)
    : pTempFile_ (new TempFile_ (lpszPath))
  {
  }

public:
  TempFile (/*[in]*/ const PathName & path)
    : pTempFile_ (new TempFile_ (path))
  {
  }

public:
  const char *
  Get ()
    const
  {
    return (pTempFile_->path.Get());
  }

public:
  void
  Create ()
  {
    pTempFile_->Create ();
  }

public:
  void
  Delete ()
  {
    pTempFile_->Delete ();
  }

private:
  SmartPointer<TempFile_> pTempFile_;
};
#endif

#if DVI_DONT_RENDER_GRAPHICS_SPECIALS

/* _________________________________________________________________________

   GraphicsInclusion
   _________________________________________________________________________ */

#include "Dib.h"

class GraphicsInclusion
{
private:
  struct Value : public ReferenceCountedObjectRoot
  {
    Value ()
      : pDib (0),
	hEmf (0)
    {
    }
    void
    Dispose ()
    {
      if (pDib != 0)
	{
	  delete pDib;
	  pDib = 0;
	}
      if (hEmf != 0)
	{
	  if (DeleteEnhMetaFile(hEmf) == 0)
	    {
	      FATAL_WINDOWS_ERROR (T_("DeleteEnhMetaFile"), 0);
	    }
	  hEmf = 0;
	}
    }
    virtual
    ~Value ()
    {
      try
	{
	  Dispose ();
	}
      catch (const exception &)
	{
	}
    }
    Dib * pDib;
    HENHMETAFILE hEmf;
  };

private:
  SmartPointer<Value> pValue;

public:  
  enum Type { Unknown, BMP, EMF, WMF, Other };

public:
  GraphicsInclusion ()
    : x (-1),
      y (-1),
      cx (-1),
      cy (-1),
      type (Unknown),
      pValue (new Value)
  {
  }

public:
  GraphicsInclusion (/*[in]*/ int	x,
		     /*[in]*/ int	y,
		     /*[in]*/ int	cx,
		     /*[in]*/ int	cy)
    : x (x),
      y (y),
      cx (cx),
      cy (cy),
      type (Unknown),
      pValue (new Value)
  {
  }

public:
  GraphicsInclusion (/*[in]*/ int	x,
		     /*[in]*/ int	y,
		     /*[in]*/ int	cx,
		     /*[in]*/ int	cy,
		     /*[in]*/ Dib *	pDib)
    : x (x),
      y (y),
      cx (cx),
      cy (cy),
      pValue (new Value)
  {
    type = BMP;
    pValue->pDib = pDib;
  }

public:
  GraphicsInclusion (/*[in]*/ int		x,
		     /*[in]*/ int		y,
		     /*[in]*/ int		cx,
		     /*[in]*/ int		cy,
		     /*[in]*/ HENHMETAFILE	hEmf)
    : x (x),
      y (y),
      cx (cx),
      cy (cy),
      pValue (new Value)
  {
    type = EMF;
    pValue->hEmf = hEmf;
  }

public:
  int
  GetX ()
    const
  {
    return (x);
  }

public:
  int
  GetY ()
    const
  {
    return (y);
  }

public:
  int
  GetCx ()
    const
  {
    return (cx);
  }

public:
  void
  SetCx (/*[in]*/ int cx)
  {
    this->cx = cx;
  }

public:
  int
  GetCy ()
    const
  {
    return (cy);
  }

public:
  void
  SetCy (/*[in]*/ int cy)
  {
    this->cy = cy;
  }

public:
  Type
  GetType ()
    const
  {
    return (type);
  }

public:
  Dib *
  GetDib ()
    const
  {
    MIKTEX_ASSERT (type == BMP);
    return (pValue->pDib);
  }

public:
  void
  SetDib (/*[in]*/ Dib * pDib)
  {
    pValue->Dispose ();
    type = BMP;
    pValue->pDib = pDib;
  }

public:
  HENHMETAFILE
  GetEnhancedMetafile ()
    const
  {
    MIKTEX_ASSERT (type == EMF);
    return (pValue->hEmf);
  }

public:
  void
  SetEnhancedMetafile (/*in]*/ HENHMETAFILE hEmf)
  {
    pValue->Dispose ();
    type = EMF;
    pValue->hEmf = hEmf;
  }

private:
  Type type;

private:
  int x, y, cx, cy;
};

#endif

/* _________________________________________________________________________

   Protos
   _________________________________________________________________________ */

bool
AddCommandPrefixChar (/*[in]*/ char ch);

bool
AllowShellCommand (/*[in]*/ const CString & command);

void DrawTransparentBitmap (/*[in]*/ HDC,
			    /*[in]*/ HBITMAP,
			    /*[in]*/ short,
			    /*[in]*/ short,
			    /*[in]*/ COLORREF);

CString
GetCommandPrefix (/*[in]*/ bool clear);

DWORD
HLStoRGB (/*[in]*/ WORD,
	  /*[in]*/ WORD,
	  /*[in]*/ WORD);

void
MakeTransparent (/*[in]*/ HWND		hwnd,
		 /*[in]*/ COLORREF	transparencyColor,
		 /*[in]*/ unsigned char	opacity);

void
RGBtoHLS (/*[in]*/ DWORD,
	  /*[out]*/ WORD &,
	  /*[out]*/ WORD &,
	  /*[out]*/ WORD &);

void
StartEditor (/*[in]*/ const char *	lpszFileName,
	     /*[in]*/ const char *	lpszDocDir,
	     /*[in]*/ int			line);

void
VYapLog (/*[in]*/ const char *,
	 /*[in]*/ va_list);

void
UpdateAllDviViews (/*[in]*/ bool reread = false);

void
YapLog (/*[in]*/ const char *,
	/*[in]*/		...);

void
TraceError (/*[in]*/ const char *	lpszFormat,
	    /*[in]*/			...);

/* _________________________________________________________________________

   StdoutReader
   _________________________________________________________________________ */

class
StdoutReader
  : public IRunProcessCallback
{
public:
  StdoutReader (/*[in]*/ ProgressDialog * pProgressDialog)
    : pProgressDialog (pProgressDialog)
  {
  }

public:
  ~StdoutReader ()
  {
    try
      {
	YapLog (_T("%s"), output.c_str());
      }
    catch (const exception &)
      {
      }
  }

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n)
  {
    output.append (reinterpret_cast<const char *>(pOutput), n);
    return (pProgressDialog
	    ? (! pProgressDialog->HasUserCancelled())
	    : true);
  }

private:
  string output;

private:
  ProgressDialog * pProgressDialog;
};

/* _________________________________________________________________________

   Internal Messages
   _________________________________________________________________________ */

enum {
  WM_DVIPROGRESS = WM_APP + 1,
  WM_MAKEFONTS
};

/* _________________________________________________________________________

   Global Variables
   _________________________________________________________________________ */

extern auto_ptr<YapConfig> g_pYapConfig;
