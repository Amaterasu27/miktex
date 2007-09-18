/* log.cpp: tracing

   Copyright (C) 1996-2007 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   TraceStream::~TraceStream
   _________________________________________________________________________ */

TraceStream::~TraceStream ()
{
}

/* _________________________________________________________________________

   TraceStreamInfo
   _________________________________________________________________________ */

struct TraceStreamInfo
{
public:
  TraceStreamInfo ()
    : refCounter (0)
  {
  }
public:
  bool enabled;
public:
  int volatile refCounter;  
};

/* _________________________________________________________________________

   TraceStreamImpl
   _________________________________________________________________________ */

class
TraceStreamImpl : public TraceStream
{
public:
  virtual
  void
  MIKTEXCALL
  Close ();

public:
  virtual
  void
  MIKTEXCALL
  Enable (/*[in]*/ bool		enable);

public:
  virtual
  bool
  MIKTEXCALL
  IsEnabled ();

public:
  virtual
  void
  MIKTEXCALL
  WriteFormattedLine (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
		      /*[in]*/ const MIKTEXCHAR *	lpszFormat,
		      /*[in]*/				...);

public:
  virtual
  void
  MIKTEXCALL
  Write (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
	 /*[in]*/ const MIKTEXCHAR *	lpszText);

public:
  virtual
  void
  MIKTEXCALL
  WriteLine (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
	     /*[in]*/ const MIKTEXCHAR *	lpszText);

public:
  virtual
  void
  MIKTEXCALL
  VTrace (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
	  /*[in]*/ const MIKTEXCHAR *	lpszFormat,
	  /*[in]*/ va_list		arglist);

private:
  TraceStreamImpl (/*[in]*/ const MIKTEXCHAR *	lpszName)
    : Name (lpszName),
      open (true)
  {
    AddRef ();
  }

protected:
  virtual
  ~ TraceStreamImpl ()
  {
    try
      {
	Close ();
      }
    catch (const exception &)
      {
      }
  }

private:
  bool open;

private:
  tstring Name;

private:
  const tstring &
  get_Name ()
    const
  {
    return (Name);
  }

private:
  void
  Logger (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
	  /*[in]*/ const MIKTEXCHAR *	lpszMessage,
	  /*[in]*/ bool			appendNewline);

private:
  void
  FormatV (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
	   /*[in]*/ bool		appendNewline,
	   /*[in]*/ const MIKTEXCHAR *	lpszFormat,
	   /*[in]*/ va_list		arglist);

private:
  MIKTEX_DEFINE_LOCK(this);

private:
  void
  AddRef ()
  {
    MIKTEX_LOCK(this)
      {
	traceStreams[Name].refCounter += 1;
      }
    MIKTEX_UNLOCK();
  }

private:
  void
  Release ()
  {
    MIKTEX_LOCK(this)
      {
	if (traceStreams[Name].refCounter == 0)
	  {
	    UNEXPECTED_CONDITION (T_("TraceStreamImpl::Release"));
	  }
	traceStreams[Name].refCounter -= 1;
      }
    MIKTEX_UNLOCK();
  }

private:
  friend class TraceStream;

private:
#if defined(USE_HASH_MAP)
  typedef hash_map<tstring, TraceStreamInfo> TraceStreamTable;
#else
  typedef map<tstring, TraceStreamInfo> TraceStreamTable;
#endif

private:
  static TraceStreamTable traceStreams;

private:
  static tstring traceFlags;
};

TraceStreamImpl::TraceStreamTable TraceStreamImpl::traceStreams;
tstring TraceStreamImpl::traceFlags;

/* _________________________________________________________________________

   SessionImpl::RegisterLibraryTraceStreams
   _________________________________________________________________________ */

void
SessionImpl::RegisterLibraryTraceStreams ()
{
  trace_access.reset (TraceStream::Open(MIKTEX_TRACE_ACCESS));
  trace_config.reset (TraceStream::Open(MIKTEX_TRACE_CONFIG));
  trace_core.reset (TraceStream::Open(MIKTEX_TRACE_CORE));
  trace_env.reset (TraceStream::Open(MIKTEX_TRACE_ENV));
  trace_error.reset (TraceStream::Open(MIKTEX_TRACE_ERROR));
  trace_files.reset (TraceStream::Open(MIKTEX_TRACE_FILES));
  trace_filesearch.reset (TraceStream::Open(MIKTEX_TRACE_FILESEARCH));
  trace_fndb.reset (TraceStream::Open(MIKTEX_TRACE_FNDB));
  trace_fonts.reset (TraceStream::Open(MIKTEX_TRACE_FONTINFO));
  trace_packages.reset (TraceStream::Open(MIKTEX_TRACE_PACKAGES));
  trace_process.reset (TraceStream::Open(MIKTEX_TRACE_PROCESS));
  trace_tempfile.reset (TraceStream::Open(MIKTEX_TRACE_TEMPFILE));
  trace_time.reset (TraceStream::Open(MIKTEX_TRACE_TIME));
  trace_values.reset (TraceStream::Open(MIKTEX_TRACE_VALUES));
};

/* _________________________________________________________________________

   TraceStreamImpl::Logger
   _________________________________________________________________________ */

void
TraceStreamImpl::Logger (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
			 /*[in]*/ const MIKTEXCHAR *	lpszMessage,
			 /*[in]*/ bool			appendNewline)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszFacility);
  MIKTEX_ASSERT_STRING (lpszMessage);
  tstring str;
  str.reserve (256);
  str += T_('[');
#if defined(MIKTEX_WINDOWS)
  PathName path;
  if (GetModuleFileName(0, path.GetBuffer(), BufferSizes::MaxPath) != 0)
    {
      MIKTEXCHAR szName[BufferSizes::MaxPath];
      path.GetFileNameWithoutExtension (szName);
      str += szName;
    }
#endif
  str += T_('.');
  if (lpszFacility != 0)
    {
      str += lpszFacility;
    }
  str += T_("]: ");
  str += lpszMessage;
  if (appendNewline)
    {
      str += T_('\n');
    }
#if defined(MIKTEX_WINDOWS)
  OutputDebugString (str.c_str());
#else
  if (stderr != 0)
    {
      fputs (str.c_str(), stderr);
      fflush (stderr);
    }
#endif
}

/* _________________________________________________________________________

   TraceStreamImpl::FormatV
   _________________________________________________________________________ */

void
TraceStreamImpl::FormatV (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
			  /*[in]*/ bool			appendNewline,
			  /*[in]*/ const MIKTEXCHAR *	lpszFormat,
			  /*[in]*/ va_list		arglist)
{
  tstring str = Utils::FormatString(lpszFormat, arglist);
  Logger (lpszFacility, str.c_str(), appendNewline);
}

/* _________________________________________________________________________

   TraceStream::SetTraceFlags
   _________________________________________________________________________ */

void
TraceStream::SetTraceFlags (/*[in]*/ const MIKTEXCHAR *	lpszFlags)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszFlags);

  if (lpszFlags == 0)
    {
      lpszFlags = T_("error");
    }

  TraceStreamImpl::traceFlags = lpszFlags;

  for (TraceStreamImpl::TraceStreamTable::iterator
	 it = TraceStreamImpl::traceStreams.begin();
       it != TraceStreamImpl::traceStreams.end();
       ++ it)
    {
      (*it).second.enabled = false;
    }

  for (Tokenizer tok (lpszFlags, T_(",; \n\t")); tok.GetCurrent() != 0; ++ tok)
    {
      tstring name (tok.GetCurrent());
      TraceStreamImpl::traceStreams[name].enabled = true;
    }
}

/* _________________________________________________________________________

   TraceStreamImpl::Enable
   _________________________________________________________________________ */

void
TraceStreamImpl::Enable (/*[in]*/ bool	enable)
{
  traceStreams[Name].enabled = enable;
}

/* _________________________________________________________________________

   TraceError
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
TraceError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	    /*[in]*/			...)
{
  va_list marker;
  va_start (marker, lpszFormat);
  SessionImpl::GetSession()->trace_error->VTrace
    (T_("core"),
     lpszFormat,
     marker);
  va_end (marker);
}

/* _________________________________________________________________________

   TraceStreamImpl::WriteFormattedLine
   _________________________________________________________________________ */

void
TraceStreamImpl::WriteFormattedLine
(/*[in]*/ const MIKTEXCHAR *	lpszFacility,
 /*[in]*/ const MIKTEXCHAR *	lpszFormat,
 /*[in]*/			...)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszFacility);
  MIKTEX_ASSERT_STRING (lpszFormat);
  if (! IsEnabled())
    {
      return;
    }
  va_list marker;
  va_start (marker, lpszFormat);
  FormatV (lpszFacility, true, lpszFormat, marker);
  va_end (marker);
}

/* _________________________________________________________________________

   TraceStreamImpl::WriteLine
   _________________________________________________________________________ */

void
TraceStreamImpl::WriteLine (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
			    /*[in]*/ const MIKTEXCHAR *	lpszText)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszFacility);
  MIKTEX_ASSERT_STRING (lpszText);
  if (! IsEnabled())
    {
      return;
    }
  Logger (lpszFacility, lpszText, true);
}

/* _________________________________________________________________________

   TraceStreamImpl::Write
   _________________________________________________________________________ */

void
TraceStreamImpl::Write (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
			/*[in]*/ const MIKTEXCHAR *	lpszText)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszFacility);
  MIKTEX_ASSERT_STRING (lpszText);
  if (! IsEnabled())
    {
      return;
    }
  Logger (lpszFacility, lpszText, false);
}

/* _________________________________________________________________________

   TraceStreamImpl::VTrace
   _________________________________________________________________________ */

void
TraceStreamImpl::VTrace (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
			 /*[in]*/ const MIKTEXCHAR *	lpszFormat,
			 /*[in]*/ va_list		arglist)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszFacility);
  MIKTEX_ASSERT_STRING (lpszFormat);
  if (! IsEnabled())
    {
      return;
    }
  FormatV (lpszFacility, true, lpszFormat, arglist);
}

/* _________________________________________________________________________

   TraceStream::Open
   _________________________________________________________________________ */

MIKTEXAPI(TraceStream *)
TraceStream::Open (/*[in]*/ const MIKTEXCHAR * lpszName)
{
  MIKTEX_ASSERT_STRING (lpszName);
  TraceStreamImpl * pTraceStream = new TraceStreamImpl (lpszName);
  bool enable =
    Utils::Contains(TraceStreamImpl::traceFlags.c_str(),
		    lpszName,
		    T_(",; \n\t"));
  pTraceStream->Enable (enable);
  return (pTraceStream);
}

/* _________________________________________________________________________

   TraceStreamImpl::Close
   _________________________________________________________________________ */

void
TraceStreamImpl::Close ()
{
  if (open)
    {
      open = false;
      Release ();
    }
}

/* _________________________________________________________________________

   TraceStreamImpl::IsEnabled
   _________________________________________________________________________ */

bool
TraceStreamImpl::IsEnabled ()
{
  return (traceStreams[Name].enabled);
}

/* _________________________________________________________________________

   TraceStream::TraceLastWin32Error
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
void
TraceStream::TraceLastWin32Error
(/*[in]*/ const MIKTEXCHAR *	lpszWindowsFunction,
 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
 /*[in]*/ int			lpszSourceLine)
{
  TraceWindowsError (lpszWindowsFunction,
		     ::GetLastError(),
		     lpszInfo,
		     lpszSourceFile,
		     lpszSourceLine);
}
#endif


/* _________________________________________________________________________

   TraceStream::TraceLastCRTError
   _________________________________________________________________________ */

void
TraceStream::TraceLastCRTError
(/*[in]*/ const MIKTEXCHAR *	lpszCrtFunction,
 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
 /*[in]*/ int			lpszSourceLine)
{
  int lastError = errno;
  tstring errorMessage;
  if (! GetCrtErrorMessage(lastError, errorMessage))
    {
      return;
    }
  SessionImpl::GetSession()->trace_error->WriteFormattedLine
    (T_("core"),
     T_("\
CRT function %s failed for the following reason:\n\
%s\n\
Result: %u\n\
Info: %s\n\
Source: %s\n\
Line: %d"),
     lpszCrtFunction,
     errorMessage.c_str(),
     lastError,
     (lpszInfo == 0
      ? T_("")
      : lpszInfo),
     lpszSourceFile,
     lpszSourceLine);
}

/* _________________________________________________________________________

   TraceMiKTeXError
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
TraceMiKTeXError (/*[in]*/ const MIKTEXCHAR *	lpszMiktexFunction,
		  /*[in]*/ const MIKTEXCHAR *	lpszMessage,
		  /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		  /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		  /*[in]*/ int			lpszSourceLine)
{
  if (SessionImpl::GetSession() != 0
      && SessionImpl::GetSession()->trace_error.get() != 0)
    {
      SessionImpl::GetSession()->trace_error->WriteFormattedLine
	(T_("core"),
	 T_("\
The MiKTeX function %s fails for the following reason:\n\
%s\n\
Info: %s\n\
Source: %s\n\
Line: %d"),
	 (lpszMiktexFunction ? lpszMiktexFunction : T_("Unknown")),
	 lpszMessage,
	 (lpszInfo == 0
	  ? T_("")
	  : lpszInfo),
	 lpszSourceFile,
	 lpszSourceLine);
    }
}

/* _________________________________________________________________________

   SessionImpl::UnregisterLibraryTraceStreams
   _________________________________________________________________________ */

void
SessionImpl::UnregisterLibraryTraceStreams ()
{
  trace_access->Close ();
  trace_config->Close ();
  trace_core->Close ();
  trace_error->Close ();
  trace_env->Close ();
  trace_files->Close ();
  trace_filesearch->Close ();
  trace_fndb->Close ();
  trace_fonts->Close ();
  trace_packages->Close ();
  trace_process->Close ();
  trace_tempfile->Close ();
  trace_time->Close ();
  trace_values->Close ();
}
