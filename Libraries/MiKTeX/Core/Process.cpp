/* process.cpp: executing secondary processes

   Copyright (C) 1996-2010 Christian Schenk

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

   Process::~Process
   _________________________________________________________________________ */

Process::~Process ()
{
}

/* _________________________________________________________________________

   Process2::~Process2
   _________________________________________________________________________ */

Process2::~Process2 ()
{
}

/* _________________________________________________________________________

   Process::Start
   _________________________________________________________________________ */

void
Process::Start (/*[in]*/ const char *	lpszFileName,
		/*[in]*/ const char *	lpszArguments,
		/*[in]*/ FILE *		pFileStandardInput,
		/*[out]*/ FILE **	ppFileStandardInput,
		/*[out]*/ FILE **	ppFileStandardOutput,
		/*[out]*/ FILE **	ppFileStandardError,
		/*[in]*/ const char *	lpszWorkingDirectory)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszArguments);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszWorkingDirectory);

  MIKTEX_ASSERT (pFileStandardInput == 0 || ppFileStandardInput == 0);
  
  ProcessStartInfo startinfo;

  startinfo.FileName = lpszFileName;

  if (lpszArguments != 0)
    {
      startinfo.Arguments = lpszArguments;
    }

  startinfo.StandardInput = pFileStandardInput;

  startinfo.RedirectStandardInput =
    (pFileStandardInput == 0 && ppFileStandardInput != 0);
  startinfo.RedirectStandardOutput = (ppFileStandardOutput != 0);
  startinfo.RedirectStandardError = (ppFileStandardError != 0);

  if (lpszWorkingDirectory != 0)
    {
      startinfo.WorkingDirectory = lpszWorkingDirectory;
    }

  auto_ptr<Process> pProcess (Process::Start(startinfo));

  if (ppFileStandardInput != 0)
    {
      *ppFileStandardInput = pProcess->get_StandardInput();
    }

  if (ppFileStandardOutput != 0)
    {
      *ppFileStandardOutput = pProcess->get_StandardOutput();
    }

  if (ppFileStandardError != 0)
    {
      *ppFileStandardError = pProcess->get_StandardError();
    }

  pProcess->Close ();
}

/* _________________________________________________________________________

   Process::Run

   Run a process.  Pass output (stdout & stderr) to caller.
   _________________________________________________________________________ */

bool
Process::Run (/*[in]*/ const PathName &		fileName,
	      /*[in]*/ const char *		lpszArguments,
	      /*[out]*/ IRunProcessCallback *	pCallback,
	      /*[out]*/ int *			pExitCode,
	      /*[in]*/  const char *		lpszWorkingDirectory)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszArguments);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszWorkingDirectory);

  ProcessStartInfo startinfo;

  startinfo.FileName = fileName.Get();

  if (lpszArguments != 0)
    {
      startinfo.Arguments = lpszArguments;
    }

  startinfo.StandardInput = 0;
  startinfo.RedirectStandardInput = false;
  startinfo.RedirectStandardOutput = (pCallback != 0);
  startinfo.RedirectStandardError = false;

  if (lpszWorkingDirectory != 0)
    {
      startinfo.WorkingDirectory = lpszWorkingDirectory;
    }

  auto_ptr<Process> pProcess (Process::Start(startinfo));

  if (pCallback != 0)
    {
      SessionImpl::GetSession()->trace_process->WriteLine
	("core",
	 T_("start reading the pipe"));
      const size_t CHUNK_SIZE = 64;
      char buf[ CHUNK_SIZE ];
      bool cancelled = false;
      FileStream stdoutStream (pProcess->get_StandardOutput());
      size_t total = 0;
      while (! cancelled && feof(stdoutStream.Get()) == 0)
	{
	  size_t n = fread(buf, 1, CHUNK_SIZE, stdoutStream.Get());
	  int err = ferror(stdoutStream.Get());
	  if (err != 0 && err != EPIPE)
	    {
	      FATAL_CRT_ERROR ("fread", 0);
	    }
	  // pass output to caller
	  total += n;
	  cancelled = ! pCallback->OnProcessOutput(buf, n);
	}
      SessionImpl::GetSession()->trace_process->WriteFormattedLine
	("core",
	 T_("read %u bytes from the pipe"),
	 static_cast<unsigned>(total));
    }
  
  // wait for the process to finish
  pProcess->WaitForExit ();
  
  // get the exit code & close process
  int exitCode = pProcess->get_ExitCode();
  pProcess->Close ();
  if (pExitCode != 0)
    {
      *pExitCode = exitCode;
      return (true);
    }
  else if (exitCode == 0)
    {
      return (true);
    }
  else
    {
      TraceError (T_("%s returned %d"),
		  Q_(fileName),
		  static_cast<int>(exitCode));
      return (false);
    }
}

/* _________________________________________________________________________

   ProcessOutputBuffer
   _________________________________________________________________________ */

BEGIN_ANONYMOUS_NAMESPACE;

class ProcessOutputBuffer
  : public IRunProcessCallback
{
public:
  ProcessOutputBuffer (/*[in]*/ void *	pBuffer,
		       /*[in]*/ size_t	capacity)
    : capacity (capacity),
      pBuffer (pBuffer),
      offset (0)
  {
  }
public:
  void * pBuffer;
public:
  size_t capacity;
public:
  size_t offset;
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n)
  {
    n = min(n, capacity - offset);
    if (n > 0)
    {
      memcpy (reinterpret_cast<char*>(pBuffer) + offset, pOutput, n);
      offset += n;
    }
    return (true);
  }
};

END_ANONYMOUS_NAMESPACE;
  
/* _________________________________________________________________________

   Process::Run

   Run a process.  Store output (stdout & stderr) in a buffer.
   _________________________________________________________________________ */

bool
Process::Run (/*[in]*/ const PathName &	fileName,
	      /*[in]*/ const char *	lpszArguments,
	      /*[out]*/ void *		pBuf,
	      /*[in,out]*/ size_t *	pBufSize,
	      /*[in]*/ int *		pExitCode)
{
  MIKTEX_ASSERT (pBufSize != 0);
  MIKTEX_ASSERT_BUFFER (pBuf, *pBufSize);
  ProcessOutputBuffer buffer (pBuf, *pBufSize);
  size_t nMax = *pBufSize;
  bool done =
    Process::Run(fileName,
		 lpszArguments,
		 &buffer,
		 pExitCode,
		 0);
  if (done)
    {
      *pBufSize = static_cast<unsigned int>(buffer.offset);
      if (*pBufSize < nMax)
	{
	  static_cast<char *>(pBuf)[*pBufSize] = 0;
	}
    }
  return (done);
}

/* _________________________________________________________________________

   Process::Run
   _________________________________________________________________________ */

void
Process::Run (/*[in]*/ const PathName &	fileName,
	      /*[in]*/ const char *	lpszArguments)
{
  if (! Process::Run (fileName,
		      lpszArguments,
		      reinterpret_cast<IRunProcessCallback*>(0),
		      0,
		      0))
    {
      FATAL_MIKTEX_ERROR ("Process::Run",
			  T_("The operation failed for some reason."),
			  fileName.Get());
    }
}

/* _________________________________________________________________________

   Process::Run
   _________________________________________________________________________ */

void
Process::Run (/*[in]*/ const PathName &		fileName,
	      /*[in]*/ const char *		lpszArguments,
	      /*[int]*/ IRunProcessCallback *	pCallback)
{
  if (! Run(fileName, lpszArguments, pCallback, 0, 0))
    {
      FATAL_MIKTEX_ERROR ("Process::Run",
			  T_("The operation failed for some reason."),
			  fileName.Get());
    }
}

/* _________________________________________________________________________

   Process::ExecuteSystemCommand
   _________________________________________________________________________ */

bool
Process::ExecuteSystemCommand (/*[in]*/ const char * lpszCommandLine)
{
  return (ExecuteSystemCommand(lpszCommandLine, 0, 0, 0));
}

/* _________________________________________________________________________

   Process::ExecuteSystemCommand
   _________________________________________________________________________ */

bool
Process::ExecuteSystemCommand (/*[in]*/ const char *	lpszCommandLine,
			       /*[out]*/ int *		pExitCode)
{
  return (ExecuteSystemCommand(lpszCommandLine, pExitCode, 0, 0));
}

/* _________________________________________________________________________

   Process2::GetInvokerNames
   _________________________________________________________________________ */

vector<string>
Process2::GetInvokerNames ()
{
  vector<string> result;
  auto_ptr<Process2> pProcess (Process2::GetCurrentProcess());
  auto_ptr<Process2> pParentProcess (pProcess->get_Parent());
  const int maxLevels = 3;
  int level = 0;
  for (int level = 0; pParentProcess.get() != 0 && level < maxLevels; ++ level)
  {
    result.push_back (pParentProcess->get_ProcessName());
    pParentProcess.reset (pParentProcess->get_Parent());
  }
  if (pParentProcess.get() != 0)
  {
    result.push_back ("...");
  }
  reverse (result.begin(), result.end());
  return (result);
}

/* _________________________________________________________________________

   miktex_start_process
   _________________________________________________________________________ */

MIKTEXCEEAPI(void)
miktex_start_process (/*[in]*/ const char *	lpszFileName,
		      /*[in]*/ const char *	lpszArguments,
		      /*[in]*/ FILE *		pFileStandardInput,
		      /*[out]*/ FILE **		ppFileStandardInput,
		      /*[out]*/ FILE **		ppFileStandardOutput,
		      /*[out]*/ FILE **		ppFileStandardError,
		      /*[in]*/ const char *	lpszWorkingDirectory)
{
  C_FUNC_BEGIN ();
  Process::Start (lpszFileName,
		  lpszArguments,
		  pFileStandardInput,
		  ppFileStandardInput,
		  ppFileStandardOutput,
		  ppFileStandardError,
		  lpszWorkingDirectory);
  C_FUNC_END ();
}
