/* error.cpp: error handling

   Copyright (C) 1996-2009 Christian Schenk

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
int debugOnStdException = 0;
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
  if (cerr.fail())
    {
      return;
    }
  try
    {
      cerr << "*** " << e.what() << endl;
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
  if (cerr.fail())
    {
      return;
    }
  try
    {
      string programInvocationName (e.GetProgramInvocationName());
      char szName[BufferSizes::MaxPath];
      bool haveName = (programInvocationName.length() > 0);
      if (haveName)
	{
	  PathName path (programInvocationName);
	  path.GetFileName (szName);
	}
      int last = '\n';
      for (const char * lpsz = e.what();
	   *lpsz != 0;
	   ++ lpsz)
	{
	  if (haveName && last == '\n')
	    {
	      cerr << szName << ": ";
	    }
	  cerr << *lpsz;
	  last = *lpsz;
	}
      if (last != '\n')
	{
	  cerr << endl;
	  last = '\n';
	}
      string info = e.GetInfo();
      for (const char * lpsz = info.c_str();
	   *lpsz != 0;
	   ++ lpsz)
	{
	  if (haveName && last == '\n')
	    {
	      cerr << szName << ": ";
	    }
	  if (last == '\n')
	    {
	      cerr << "Data: ";
	    }
	  cerr << *lpsz;
	  last = *lpsz;
	}
      if (last != '\n')
	{
	  cerr << endl;
	  last = '\n';
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
Session::FatalMiKTeXError (/*[in]*/ const char *	lpszMiktexFunction,
			   /*[in]*/ const char *	lpszMessage,
			   /*[in]*/ const char *	lpszInfo,
			   /*[in]*/ const char *	lpszSourceFile,
			   /*[in]*/ int			sourceLine)
{
  lpszSourceFile = GetShortSourceFile(lpszSourceFile);
  string programInvocationName;
  if (SessionImpl::TryGetSession() != 0)
    {
      TraceMiKTeXError (lpszMiktexFunction,
			lpszMessage,
			lpszInfo,
			lpszSourceFile,
			sourceLine);
      programInvocationName =
	SessionImpl::GetSession()->initInfo.GetProgramInvocationName();
    }
#if 1
  string env;
  if (Utils::GetEnvironmentString("MIKTEX_DEBUG_BREAK", env) && env == "1")
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

   MiKTeX::Debug::FatalMiKTeXError
   _________________________________________________________________________ */

void
MIKTEXCEECALL
MiKTeX::Debug::FatalMiKTeXError (/*[in]*/ const char *	lpszMiktexFunction,
				 /*[in]*/ const char *	lpszMessage,
				 /*[in]*/ const char *	lpszInfo,
				 /*[in]*/ const char *	lpszSourceFile,
				 /*[in]*/ int		sourceLine)
{
  Session::FatalMiKTeXError (lpszMiktexFunction,
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
		    /*[out]*/ string &		errorMessage)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400
  const size_t BUFSIZE = 512;
  char buffer[BUFSIZE];
  if (strerror_s(buffer, BUFSIZE, functionResult) != 0)
    {
      return (false);
    }
  errorMessage = buffer;
#  else
  errorMessage = strerror(functionResult);
#endif
  return (true);
}

/* _________________________________________________________________________

   Session::FatalCrtError
   _________________________________________________________________________ */

void
Session::FatalCrtError (/*[in]*/ const char *	lpszCrtFunction,
			/*[in]*/ const char *	lpszInfo,
			/*[in]*/ const char *	lpszSourceFile,
			/*[in]*/ int		sourceLine)
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
Session::FatalCrtError (/*[in]*/ const char *	lpszCrtFunction,
			/*[in]*/ int		errorCode,
			/*[in]*/ const char *	lpszInfo,
			/*[in]*/ const char *	lpszSourceFile,
			/*[in]*/ int		sourceLine)
{
  lpszSourceFile = GetShortSourceFile(lpszSourceFile);
  string errorMessage;
  if (! GetCrtErrorMessage(errorCode, errorMessage))
    { 
      errorMessage = T_("runtime error ");
      errorMessage += NUMTOSTR(errorCode);
    }
  if (lpszInfo != 0)
    {
      errorMessage += ": ";
      errorMessage += lpszInfo;
    }
  string programInvocationName;
  if (SessionImpl::TryGetSession() != 0)
    {
      SessionImpl::GetSession()->trace_error->WriteFormattedLine
	("core",
	 T_("\
%s\n\
Function: %s\n\
Result: %u\n\
Data: %s\n\
Source: %s:%d"),
	 errorMessage.c_str(),
	 lpszCrtFunction,
	 errorCode,
	 (lpszInfo == 0
	  ? "<no data>"
	  : lpszInfo),
	 lpszSourceFile,
	 sourceLine);
      programInvocationName =
	SessionImpl::GetSession()->initInfo.GetProgramInvocationName();
    }
#if 1
  string env;
  if (Utils::GetEnvironmentString("MIKTEX_DEBUG_BREAK", env) && env == "1")
    {
      DEBUG_BREAK ();
    }
#endif
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
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
 /*[in]*/ int			sourceLine)
  : programInvocationName (lpszProgramInvocationName == 0
			   ?  ""
			   : lpszProgramInvocationName),
    message (lpszMessage == 0 || *lpszMessage == 0
	     ? T_("Unknown MiKTeX exception.")
	     : lpszMessage),
    info (lpszInfo == 0 ? "" : lpszInfo),
    sourceFile (lpszSourceFile == 0 ? "" : lpszSourceFile),
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
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
 /*[in]*/ int		sourceLine)
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
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
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
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
 /*[in]*/ int		sourceLine)
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
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
 /*[in]*/ int		sourceLine)
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
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
 /*[in]*/ int		sourceLine)
  : MiKTeXException (lpszProgramInvocationName,
		     lpszMessage,
		     lpszInfo,
		     lpszSourceFile,
		     sourceLine)
{
}
