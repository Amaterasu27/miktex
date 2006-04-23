/* yap.h: main header file for the Yap application		-*- C++ -*-

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

#include "resource.h"

#define YAP_TRACE_FLAGS_LVL_1			\
  MIKTEX_TRACE_ERROR T_(",")			\
  MIKTEX_TRACE_PROCESS T_(",")			\
  MIKTEX_TRACE_YAP

#define YAP_TRACE_FLAGS_LVL_2			\
  YAP_TRACE_FLAGS_LVL_1 T_(",")			\
  MIKTEX_TRACE_DVIFILE T_(",")			\
  MIKTEX_TRACE_DVIPKFONT

#define YAP_TRACE_FLAGS_LVL_3			\
  YAP_TRACE_FLAGS_LVL_2 T_(",")			\
  MIKTEX_TRACE_DVIGC

// Windows 95/98/Me uses a 16-bit world coordinate system
#if defined(MIKTEX_WINDOWS_32)
#  define SIXTEENBITGDI 1
#else
#  define SIXTEENBITGDI 0
#endif

#define USEMYSCROLLVIEW SIXTEENBITGDI
#define WIN95NOCONTVIEW 0

const bool DEFAULT_bRenderGraphicsInBackground = true;

/* _________________________________________________________________________

   Debug-dependant Macros
   _________________________________________________________________________ */

#if ! defined(NDEBUG)

#  define MIKTEX_ASSERT(expr)					\
  static_cast<void>						\
   ((expr)							\
    ? 0								\
    : (FATAL_MIKTEX_ERROR (0, T_("Assertion failed."), #expr),	\
       0))

#else

#  define MIKTEX_ASSERT(expr)

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

#define Q_(x) Quoted(x).c_str()

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  bool needQuotes = (strchr(lpsz, T_(' ')) != 0);
  tstring result;
  if (needQuotes)
    {
      result += T_('"');
    }
  result += lpsz;
  if (needQuotes)
    {
      result += T_('"');
    }
  return (result);
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const tstring & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const PathName & path)
{
  return (Quoted(path.Get()));
}

/* _________________________________________________________________________

   EditorInfo
   _________________________________________________________________________ */

struct EditorInfo
{
public:
  EditorInfo (/*[in]*/ const tstring & name,
	      /*[in]*/ const tstring & program,
	      /*[in]*/ const tstring & arguments)
    : name (name),
      program (program),
      arguments (arguments)
  {
  }
public:
  EditorInfo (/*[in]*/ const MIKTEXCHAR *	lpszName,
	      /*[in]*/ const MIKTEXCHAR *	lpszProgram,
	      /*[in]*/ const MIKTEXCHAR *	lpszArguments)
    : name (lpszName),
      program (lpszProgram),
      arguments (lpszArguments)
  {
  }
public:
  tstring name;
public:
  tstring program;
public:
  tstring arguments;
public:
  tstring
  GetCommandLine ()
    const
  {
    tstring ret = Q_(program);
    ret += T_(' ');
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
#if ! defined(NDEBUG)
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
  SetDisplayMetafontMode (/*[in]*/ const MIKTEXCHAR * lpszMetafontMode);

public:
  bool
  SetPrinterMetafontMode (/*[in]*/ const MIKTEXCHAR * lpszMetafontMode);
  
private:
  void
  RetrieveMetafontModes ();

private:
  void
  DestroyMetafontModes ();
};

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
		  /*[in]*/ const MIKTEXCHAR *	lpszFileName);

private:
  bool
  GotoHyperLabel (/*[in]*/ const MIKTEXCHAR * lpszLabel);

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

   NUMTOSTR
   _________________________________________________________________________ */

class NumberToStringConverter_
{
private:
  enum { BUFSIZE = 30 };

public:
  NumberToStringConverter_ (/*[in]*/ unsigned u)
  {
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%u"), u);
#  else
    n = _stprintf (buffer, T_("%u"), u);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%u"), u);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ time_t t)
  {
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%u"), static_cast<unsigned>(t));
#  else
    n = _stprintf (buffer, T_("%u"), static_cast<unsigned>(t));
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%u"), static_cast<unsigned>(t));
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ unsigned long dw)
  {
    unsigned u = static_cast<unsigned>(dw);
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%u"), u);
#  else
    n = _stprintf (buffer, T_("%u"), u);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%u"), u);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ int i)
  {
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%d"), i);
#  else
    n = _stprintf (buffer, T_("%d"), i);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%d"), i);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ double f)
  {
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%f"), f);
#  else
    n = _stprintf (buffer, T_("%f"), f);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%f"), f);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ bool b)
  {
    unsigned u = (b ? 1 : 0);
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%u"), u);
#  else
    n = _stprintf (buffer, T_("%u"), u);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%u"), u);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  const MIKTEXCHAR *
  Get ()
  {
    return (buffer);
  }

private:
  MIKTEXCHAR buffer[BUFSIZE];
};

#define NUMTOSTR(num) NumberToStringConverter_(num).Get()

/* _________________________________________________________________________

   AutoResource
   _________________________________________________________________________ */

template<class HandleType>
HandleType
InvalidHandleValue ()
{
  return (0);
}

template<class HandleType,
	 class Destroyer>
class AutoResource
{
public:
  AutoResource (/*[in]*/ HandleType handle = InvalidHandleValue<HandleType>())
    : handle (handle)
  {
  }

public:
  ~AutoResource ()
  {
    try
      {
	Reset ();
      }
    catch (const exception &)
      {
      }
  }

public:
  HandleType
  Get ()
  {
    return (handle);
  }

public:
  void
  Detach ()
  {
    handle = InvalidHandleValue<HandleType>();
  }

public:
  void
  Reset ()
  {
    if (handle != InvalidHandleValue<HandleType>())
      {
	HandleType tmp = handle;
	handle = InvalidHandleValue<HandleType>();
	Destroyer() (tmp);
      }
  }

public:
  HandleType *
  operator & ()
  {
    return (&handle);
  }

private:
  HandleType handle;
};

/* _________________________________________________________________________

   AutoResource2
   _________________________________________________________________________ */

template<class HandleType1,
	 class HandleType2,
	 class Destroyer>
class AutoResource2
{
public:
  AutoResource2 (/*[in]*/ HandleType1 handle1 =
		 InvalidHandleValue<HandleType1>(),
		 /*[in]*/ HandleType2 handle2 =
		 InvalidHandleValue<HandleType2>())
    : handle1 (handle1),
      handle2 (handle2)
  {
  }

public:
  ~AutoResource2 ()
  {
    try
      {
	Reset ();
      }
    catch (const exception &)
      {
      }
  }

public:
  HandleType2
  Get ()
  {
    return (handle2);
  }

public:
  void
  Detach ()
  {
    handle1 = InvalidHandleValue<HandleType1>();
    handle2 = InvalidHandleValue<HandleType2>();
  }

public:
  void
  Reset ()
  {
    if (handle2 != InvalidHandleValue<HandleType2>())
      {
	HandleType1 tmp1 = handle1;
	HandleType2 tmp2 = handle2;
	handle1 = InvalidHandleValue<HandleType1>();
	handle2 = InvalidHandleValue<HandleType2>();
	Destroyer() (tmp1, tmp2);
      }
  }

public:
  HandleType2 *
  operator & ()
  {
    return (&handle2);
  }

private:
  HandleType1 handle1;

private:
  HandleType2 handle2;
};

/* _________________________________________________________________________

   AutoHKEY
   _________________________________________________________________________ */

class RegCloseKey_
{
public:
  void
  operator() (/*[in]*/ HKEY hkey)
  {
    long result = RegCloseKey(hkey);
    if (result != ERROR_SUCCESS)
      {
#if 0
	TraceWindowsError (T_("RegCloseKey"),
			   result,
			   0,
			   T_(__FILE__),
			   __LINE__);
#endif
      }
  }
};

typedef AutoResource<HKEY, RegCloseKey_> AutoHKEY;

/* _________________________________________________________________________

   AutoClosePrinter
   _________________________________________________________________________ */

class ClosePrinter_
{
public:
  void
  operator() (/*[in]*/ HANDLE hPrinter)
  {
    ClosePrinter (hPrinter);
  }
};

typedef AutoResource<HANDLE, ClosePrinter_> AutoClosePrinter;

/* _________________________________________________________________________

   AutoCloseHandle
   _________________________________________________________________________ */

class CloseHandle_
{
public:
  void
  operator() (/*[in]*/ HANDLE handle)
  {
    CloseHandle (handle);
  }
};

typedef AutoResource<HANDLE, CloseHandle_> AutoCloseHandle;

/* _________________________________________________________________________

   AutoUnmapViewOfFile
   _________________________________________________________________________ */

class UnmapViewOfFile_
{
public:
  void
  operator() (/*[in]*/ void * p)
  {
    UnmapViewOfFile (p);
  }
};

typedef AutoResource<void *, UnmapViewOfFile_> AutoUnmapViewOfFile;

/* _________________________________________________________________________

   AutoDeleteMetaFile
   _________________________________________________________________________ */

class DeleteMetaFile_
{
public:
  void
  operator() (/*[in]*/ HMETAFILE hMetaFile)
  {
    DeleteMetaFile (hMetaFile);
  }
};

typedef AutoResource<HMETAFILE, DeleteMetaFile_> AutoDeleteMetaFile;

/* _________________________________________________________________________

   AutoMemoryPointer
   _________________________________________________________________________ */

class free_
{
public:
  void
  operator() (/*[in]*/ void * p)
  {
    free (p);
  }
};

typedef AutoResource<void *, free_> AutoMemoryPointer;

/* _________________________________________________________________________

   AutoGlobalFree
   _________________________________________________________________________ */

class GlobalFree_
{
public:
  void
  operator() (/*[in]*/ HGLOBAL hMem)
  {
    GlobalFree (hMem);
  }
};

typedef AutoResource<void *, GlobalFree_> AutoGlobalFree;

/* _________________________________________________________________________

   AutoDeleteObject
   _________________________________________________________________________ */

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

/* _________________________________________________________________________

   AutoDetachMenu
   _________________________________________________________________________ */

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

/* _________________________________________________________________________

   AutoUnlockPage
   _________________________________________________________________________ */

template<class OBJTYPE>
class UnlockObject
{
public:
  void
  operator() (/*[in]*/ OBJTYPE * pObj)
  {
    pObj->Unlock ();
  }
};

typedef AutoResource<DviPage*, UnlockObject<DviPage> > AutoUnlockPage;

/* _________________________________________________________________________

   AutoDdeUninitialize
   _________________________________________________________________________ */

class DdeUninitialize_
{
public:
  void
  operator() (/*[in]*/ unsigned long inst)
  {
    DdeUninitialize (inst);
  }
};

typedef AutoResource<unsigned long, DdeUninitialize_> AutoDdeUninitialize;

/* _________________________________________________________________________

   AutoDdeFreeDataHandle
   _________________________________________________________________________ */

class DdeFreeDataHandle_
{
public:
  void
  operator() (/*[in]*/ HDDEDATA hData)
  {
    DdeFreeDataHandle (hData);
  }
};

typedef AutoResource<HDDEDATA, DdeFreeDataHandle_> AutoDdeFreeDataHandle;

/* _________________________________________________________________________

   AutoDdeDisconnect
   _________________________________________________________________________ */

class DdeDisconnect_
{
public:
  void
  operator() (/*[in]*/ HCONV hConv)
  {
    DdeDisconnect (hConv);
  }
};

typedef AutoResource<HCONV, DdeDisconnect_> AutoDdeDisconnect;

/* _________________________________________________________________________

   AutoDdeFreeStringHandle
   _________________________________________________________________________ */

class DdeFreeStringHandle_
{
public:
  void
  operator() (/*[in]*/ unsigned long	inst,
	      /*[in]*/ HSZ		hsz)
  {
    DdeFreeStringHandle (inst, hsz);
  }
};

typedef
AutoResource2<unsigned long, HSZ, DdeFreeStringHandle_>
AutoDdeFreeStringHandle;

/* _________________________________________________________________________

   AutoRestoreDC
   _________________________________________________________________________ */

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

/* _________________________________________________________________________

   AutoSelectObject
   _________________________________________________________________________ */

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
  TempFile (/*[in]*/ const MIKTEXCHAR * lpszPath)
    : pTempFile_ (new TempFile_ (lpszPath))
  {
  }

public:
  TempFile (/*[in]*/ const PathName & path)
    : pTempFile_ (new TempFile_ (path))
  {
  }

public:
  const MIKTEXCHAR *
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

/* _________________________________________________________________________

   Protos
   _________________________________________________________________________ */

bool
AddCommandPrefixChar (/*[in]*/ MIKTEXCHAR ch);

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
StartEditor (/*[in]*/ const MIKTEXCHAR *,
	     /*[in]*/ int);

void
VYapLog (/*[in]*/ const MIKTEXCHAR *,
	 /*[in]*/ va_list);

void
UpdateAllDviViews (/*[in]*/ bool reread = false);

void
YapLog (/*[in]*/ const MIKTEXCHAR *,
	/*[in]*/		...);

void
TraceError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
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
  MIKTEXCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n)
  {
    output.append (reinterpret_cast<const MIKTEXCHAR *>(pOutput), n);
    return (pProgressDialog
	    ? (! pProgressDialog->HasUserCancelled())
	    : true);
  }

private:
  tstring output;

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
