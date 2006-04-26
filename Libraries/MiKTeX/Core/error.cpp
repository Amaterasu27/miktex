/* error.cpp: error handling

   Copyright (C) 1996-2006 Christian Schenk

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

   MiKTeX::Debug::OnThrowStdException
   _________________________________________________________________________ */

#if defined(_MSC_VER)
BEGIN_INTERNAL_NAMESPACE;
#if defined(NDEBUG)
int debugOnStdException = 0;
#else
int debugOnStdException = 1;
#endif
END_INTERNAL_NAMESPACE;

void
__declspec(dllexport)
__cdecl
MiKTeX::Debug::OnThrowStdException ()
{
  if (debugOnStdException == 1)
    {
      DebugBreak ();
    }
}
#endif

/* _________________________________________________________________________

   PrintException
   _________________________________________________________________________ */

void
Utils::PrintException (/*[in]*/ const exception &	e)
{
  if (tcerr.fail())
    {
      return;
    }
  try
    {
      tcerr << T_("*** ") << e.what() << endl;
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   PrintException
   _________________________________________________________________________ */

void
Utils::PrintException (/*[in]*/ const MiKTeXException &	e)
{
  if (tcerr.fail())
    {
      return;
    }
  try
    {
      tstring programInvocationName (e.GetProgramInvocationName());
      MIKTEXCHAR szName[BufferSizes::MaxPath];
      bool haveName = (programInvocationName.length() > 0);
      if (haveName)
	{
	  PathName path (programInvocationName);
	  path.GetFileName (szName);
	}
      MIKTEXCHARINT last = T_('\n');
      for (const MIKTEXCHAR * lpsz = e.what();
	   *lpsz != 0;
	   ++ lpsz)
	{
	  if (haveName && last == T_('\n'))
	    {
	      tcerr << szName << T_(": ");
	    }
	  tcerr << *lpsz;
	  last = *lpsz;
	}
      if (last != T_('\n'))
	{
	  tcerr << endl;
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   Session::FatalMiKTeXError
   _________________________________________________________________________ */

void
MIKTEXNORETURN
MIKTEXCALL
Session::FatalMiKTeXError (/*[in]*/ const MIKTEXCHAR *	lpszMiktexFunction,
			   /*[in]*/ const MIKTEXCHAR *	lpszMessage,
			   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
			   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
			   /*[in]*/ int			sourceLine)
{
  tstring programInvocationName;
  if (SessionImpl::theSession != 0)
    {
      TraceMiKTeXError (lpszMiktexFunction,
			lpszMessage,
			lpszInfo,
			lpszSourceFile,
			sourceLine);
      programInvocationName =
	SessionImpl::theSession->initInfo.GetProgramInvocationName();
    }
#if 1
  tstring env;
  if (Utils::GetEnvironmentString(T_("MIKTEX_DEBUG_BREAK"), env)
      && env == T_("1"))
    {
      DEBUG_BREAK ();
    }
#endif
  throw MiKTeXException (programInvocationName.c_str(),
			 lpszMessage,
			 lpszInfo,
			 lpszSourceFile,
			 sourceLine);
}

/* _________________________________________________________________________

   GetCrtErrorMessage
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
GetCrtErrorMessage (/*[in]*/ int		functionResult,
		    /*[out]*/ tstring &		errorMessage)
{
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
  const size_t BUFSIZE = 512;
  MIKTEXCHAR buffer[BUFSIZE];
  if (_tcserror_s(buffer, BUFSIZE, functionResult) != 0)
    {
      return (false);
    }
  errorMessage = buffer;
#  else
  errorMessage = _tcserror(functionResult);
#  endif
#else
  errorMessage = strerror(functionResult);
#endif
  return (true);
}

/* _________________________________________________________________________

   Session::FatalCrtError
   _________________________________________________________________________ */

void
MIKTEXNORETURN
MIKTEXCALL
Session::FatalCrtError (/*[in]*/ const MIKTEXCHAR *	lpszCrtFunction,
			/*[in]*/ const MIKTEXCHAR *	lpszInfo,
			/*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
			/*[in]*/ int			sourceLine)
{
  FatalCrtError (lpszCrtFunction,
		 errno,
		 lpszInfo,
		 lpszSourceFile,
		 sourceLine);
}

/* _________________________________________________________________________

   Session::FatalCrtError
   _________________________________________________________________________ */

void
MIKTEXNORETURN
MIKTEXCALL
Session::FatalCrtError (/*[in]*/ const MIKTEXCHAR *	lpszCrtFunction,
			/*[in]*/ int			errorCode,
			/*[in]*/ const MIKTEXCHAR *	lpszInfo,
			/*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
			/*[in]*/ int			sourceLine)
{
  tstring errorMessage;
  if (! GetCrtErrorMessage(errorCode, errorMessage))
    { 
      errorMessage = T_("runtime error ");
      errorMessage += NUMTOSTR(errorCode);
    }
  if (lpszInfo != 0)
    {
      errorMessage += T_(": ");
      errorMessage += lpszInfo;
    }
  tstring programInvocationName;
  if (SessionImpl::theSession != 0)
    {
      TraceStream::TraceLastCRTError (lpszCrtFunction,
				      lpszInfo,
				      lpszSourceFile,
				      sourceLine);
      programInvocationName =
	SessionImpl::theSession->initInfo.GetProgramInvocationName();
    }
  switch (errorCode)
    {
    case EACCES:
      throw UnauthorizedAccessException (programInvocationName.c_str(),
					 errorMessage.c_str(),
					 lpszInfo,
					 lpszSourceFile,
					 sourceLine);
    case ENOENT:
      throw FileNotFoundException (programInvocationName.c_str(),
				   errorMessage.c_str(),
				   lpszInfo,
				   lpszSourceFile,
				   sourceLine);
    case EPIPE:
      throw BrokenPipeException (programInvocationName.c_str(),
				 errorMessage.c_str(),
				 lpszInfo,
				 lpszSourceFile,
				 sourceLine);
    default:
      throw MiKTeXException (programInvocationName.c_str(),
			     errorMessage.c_str(),
			     lpszInfo,
			     lpszSourceFile,
			     sourceLine);
    }
}

/* _________________________________________________________________________

   MiKTeXException::MiKTeXException
   _________________________________________________________________________ */

MiKTeXException::MiKTeXException ()
  : message (T_("Unknown MiKTeX exception.")),
    sourceLine (0)
{
}

/* _________________________________________________________________________

   MiKTeXException::MiKTeXException
   _________________________________________________________________________ */

MiKTeXException::MiKTeXException
(/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
 /*[in]*/ const MIKTEXCHAR *	lpszMessage,
 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
 /*[in]*/ int			sourceLine)
  : programInvocationName (lpszProgramInvocationName == 0
			   ?  T_("")
			   : lpszProgramInvocationName),
    message (lpszMessage == 0 || *lpszMessage == 0
	     ? T_("Unknown MiKTeX exception.")
	     : lpszMessage),
    info (lpszInfo == 0 ? T_("") : lpszInfo),
    sourceFile (lpszSourceFile == 0 ? T_("") : lpszSourceFile),
    sourceLine (sourceLine)
{
}

/* _________________________________________________________________________

   OperationCancelledException::OperationCancelledException
   _________________________________________________________________________ */

OperationCancelledException::OperationCancelledException ()
  : MiKTeXException (0,
		     T_("Operation cancelled."),
		     0,
		     0,
		     0)
{
}

/* _________________________________________________________________________

   OperationCancelledException::OperationCancelledException
   _________________________________________________________________________ */

OperationCancelledException::OperationCancelledException
(/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
 /*[in]*/ const MIKTEXCHAR *	lpszMessage,
 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
 /*[in]*/ int			sourceLine)
  : MiKTeXException (lpszProgramInvocationName,
		     lpszMessage,
		     lpszInfo,
		     lpszSourceFile,
		     sourceLine)
{
}

/* _________________________________________________________________________

   IOException::IOException
   _________________________________________________________________________ */

IOException::IOException ()
{
}

/* _________________________________________________________________________

   IOException::IOException
   _________________________________________________________________________ */

IOException::IOException
(/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
 /*[in]*/ const MIKTEXCHAR *	lpszMessage,
 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
 /*[in]*/ int			sourceLine)
  : MiKTeXException (lpszProgramInvocationName,
		     lpszMessage,
		     lpszInfo,
		     lpszSourceFile,
		     sourceLine)
{
}

/* _________________________________________________________________________

   FileNotFoundException::FileNotFoundException
   _________________________________________________________________________ */

FileNotFoundException::FileNotFoundException ()
{
}

/* _________________________________________________________________________

   FileNotFoundException::FileNotFoundException
   _________________________________________________________________________ */

FileNotFoundException::FileNotFoundException
(/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
 /*[in]*/ const MIKTEXCHAR *	lpszMessage,
 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
 /*[in]*/ int			sourceLine)
  : IOException (lpszProgramInvocationName,
		 lpszMessage,
		 lpszInfo,
		 lpszSourceFile,
		 sourceLine)
{
}

/* _________________________________________________________________________

   BrokenPipeException::BrokenPipeException
   _________________________________________________________________________ */

BrokenPipeException::BrokenPipeException ()
{
}

/* _________________________________________________________________________

   BrokenPipeException::BrokenPipeException
   _________________________________________________________________________ */

BrokenPipeException::BrokenPipeException
(/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
 /*[in]*/ const MIKTEXCHAR *	lpszMessage,
 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
 /*[in]*/ int			sourceLine)
  : IOException (lpszProgramInvocationName,
		 lpszMessage,
		 lpszInfo,
		 lpszSourceFile,
		 sourceLine)
{
}

/* _________________________________________________________________________

   UnauthorizedAccessException::UnauthorizedAccessException
   _________________________________________________________________________ */

UnauthorizedAccessException::UnauthorizedAccessException ()
{
}

/* _________________________________________________________________________

   UnauthorizedAccessException::UnauthorizedAccessException
   _________________________________________________________________________ */

UnauthorizedAccessException::UnauthorizedAccessException
(/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
 /*[in]*/ const MIKTEXCHAR *	lpszMessage,
 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
 /*[in]*/ int			sourceLine)
  : MiKTeXException (lpszProgramInvocationName,
		     lpszMessage,
		     lpszInfo,
		     lpszSourceFile,
		     sourceLine)
{
}
