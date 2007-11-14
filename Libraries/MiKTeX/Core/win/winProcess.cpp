/* winProcess.cpp:

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

#include "win/winProcess.h"

/* _________________________________________________________________________

   Process::Start
   _________________________________________________________________________ */

Process *
Process::Start (/*[in]*/ const ProcessStartInfo & startinfo)
{
  return (new winProcess (startinfo));
}

/* _________________________________________________________________________

   winProcess::Create
   _________________________________________________________________________ */

#if defined(NDEBUG)
#  define TRACEREDIR 0
#else
#  define TRACEREDIR 1
#endif

#define MERGE_STDOUT_STDERR 1

void
winProcess::Create ()
{
  if (startinfo.FileName.empty())
    {
      INVALID_ARGUMENT ("winProcess::Create", 0);
    }

  PathName fileName;

  if (Utils::IsAbsolutePath(startinfo.FileName.c_str()))
    {
      fileName = startinfo.FileName;
    }
  else
    {
      char * lpszFilePart = 0;
      if (SearchPathA(0,
		      startinfo.FileName.c_str(),
		      0,
		      static_cast<DWORD>(fileName.GetCapacity()),
		      fileName.GetBuffer(),
		      &lpszFilePart)
	  == 0)
	{
	  INVALID_ARGUMENT ("winProcess::Create",
			    startinfo.FileName.c_str());
	}
    }

  // standard security attributes for pipes
  SECURITY_ATTRIBUTES const SAPIPE = {
    sizeof(SECURITY_ATTRIBUTES),
    0,				// default security descriptor
    TRUE,			// make the handle inheritable
  };

  const unsigned long PIPE_BUF_SIZE = 0; // use default size

  HANDLE hCurrentProcess = GetCurrentProcess();

  // inheritable child handles
  HANDLE hChildStdin = INVALID_HANDLE_VALUE;
  HANDLE hChildStdout = INVALID_HANDLE_VALUE;
  HANDLE hChildStderr = INVALID_HANDLE_VALUE;

  try
    {
      // redirect stdout (and possibly stderr)
      if (startinfo.StandardOutput != 0)
	{
#if TRACEREDIR
	  SessionImpl::GetSession()->trace_process->WriteFormattedLine
	    ("core",
	     T_("redirecting stdout to a stream"));
#endif
	  int fd = _fileno(startinfo.StandardOutput);
	  if (fd < 0)
	    {
	      FATAL_CRT_ERROR ("_fileno", 0);
	    }
	  HANDLE hStdout = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
	  if (hStdout == INVALID_HANDLE_VALUE)
	    {
	      FATAL_CRT_ERROR ("_get_osfhandle", 0);
	    }
	  if (! DuplicateHandle(hCurrentProcess,
				hStdout,
				hCurrentProcess,
				&hChildStdout,
				0,
				TRUE,
				DUPLICATE_SAME_ACCESS))
	    {
	      FATAL_WINDOWS_ERROR ("DuplicateHandle", 0);
	    }
	}
      else if (startinfo.RedirectStandardOutput)
	{
#if TRACEREDIR
	  SessionImpl::GetSession()->trace_process->WriteFormattedLine
	    ("core",
	     T_("redirecting stdout to a pipe"));
#endif
	  // create stdout pipe
	  AutoHANDLE hStdoutRd;
	  if (! CreatePipe(&hStdoutRd,
			   &hChildStdout,
			   const_cast<LPSECURITY_ATTRIBUTES>(&SAPIPE),
			   PIPE_BUF_SIZE))
	    {
	      FATAL_WINDOWS_ERROR ("CreatePipe", 0);
	    }
	  // duplicate the read end of the pipe
	  if (! DuplicateHandle(hCurrentProcess,
				hStdoutRd.Get(),
				hCurrentProcess,
				&standardOutput,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS))
	    {
	      FATAL_WINDOWS_ERROR ("DuplicateHandle", 0);
	    }
	}
      
      // redirect stderr
      if (startinfo.StandardError != 0)
	{
#if TRACEREDIR
	  SessionImpl::GetSession()->trace_process->WriteFormattedLine
	    ("core",
	     T_("redirecting stderr to a stream"));
#endif
	  int fd = _fileno(startinfo.StandardError);
	  if (fd < 0)
	    {
	      FATAL_CRT_ERROR ("_fileno", 0);
	    }
	  HANDLE hStderr = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
	  if (hStderr == INVALID_HANDLE_VALUE)
	    {
	      FATAL_CRT_ERROR ("_get_osfhandle", 0);
	    }
	  if (! DuplicateHandle(hCurrentProcess,
				hStderr,
				hCurrentProcess,
				&hChildStderr,
				0,
				TRUE,
				DUPLICATE_SAME_ACCESS))
	    {
	      FATAL_WINDOWS_ERROR ("DuplicateHandle", 0);
	    }
	}
      else if (startinfo.RedirectStandardError)
	{
#if TRACEREDIR
	  SessionImpl::GetSession()->trace_process->WriteFormattedLine
	    ("core",
	     T_("redirecting stderr to a pipe"));
#endif
	  // create child stderr pipe
	  AutoHANDLE hStderrRd;
	  if (! CreatePipe(&hStderrRd,
			   &hChildStderr,
			   const_cast<LPSECURITY_ATTRIBUTES>(&SAPIPE),
			   PIPE_BUF_SIZE))
	    {
	      FATAL_WINDOWS_ERROR ("CreatePipe", 0);
	    }
	  // duplicate the read end of the pipe
	  if (! DuplicateHandle(hCurrentProcess,
				hStderrRd.Get(),
				hCurrentProcess,
				&standardError,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS))
	    {
	      FATAL_WINDOWS_ERROR ("DuplicateHandle", 0);
	    }
	}
#if MERGE_STDOUT_STDERR
      else if (hChildStdout != INVALID_HANDLE_VALUE)
	{
#if TRACEREDIR
	  SessionImpl::GetSession()->trace_process->WriteFormattedLine
	    ("core",
	     T_("make child stderr = child stdout"));
#endif
	  // make child stderr = child stdout
	  if (! DuplicateHandle(hCurrentProcess,
				hChildStdout,
				hCurrentProcess,
				&hChildStderr,
				0,
				TRUE,
				DUPLICATE_SAME_ACCESS))
	    {
	      FATAL_WINDOWS_ERROR ("DuplicateHandle", 0);
	    }
	}
#endif

      // redirect stdin
      if (startinfo.StandardInput != 0)
	{
#if TRACEREDIR
	  SessionImpl::GetSession()->trace_process->WriteFormattedLine
	    ("core",
	     T_("redirecting stdin to a stream"));
#endif
	  int fd = _fileno(startinfo.StandardInput);
	  if (fd < 0)
	    {
	      FATAL_CRT_ERROR ("_fileno", 0);
	    }
	  HANDLE hStdin = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
	  if (hStdin == INVALID_HANDLE_VALUE)
	    {
	      FATAL_CRT_ERROR ("_get_osfhandle", 0);
	    }
	  if (! DuplicateHandle(hCurrentProcess,
				hStdin,
				hCurrentProcess,
				&hChildStdin,
				0,
				TRUE,
				DUPLICATE_SAME_ACCESS))
	    {
	      FATAL_WINDOWS_ERROR ("DuplicateHandle", 0);
	    }
	}
      else if (startinfo.RedirectStandardInput)
	{
#if TRACEREDIR
	  SessionImpl::GetSession()->trace_process->WriteFormattedLine
	    ("core",
	     T_("redirecting stdin to a pipe"));
#endif
	  // create child stdin pipe
	  AutoHANDLE hStdinWr;
	  if (! CreatePipe(&hChildStdin,
			   &hStdinWr,
			   const_cast<LPSECURITY_ATTRIBUTES>(&SAPIPE),
			   PIPE_BUF_SIZE))
	    {
	      FATAL_WINDOWS_ERROR ("CreatePipe", 0);
	    }
	  // duplicate the write end of the pipe
	  if (! DuplicateHandle(hCurrentProcess,
				hStdinWr.Get(),
				hCurrentProcess,
				&standardInput,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS))
	    {
	      FATAL_WINDOWS_ERROR ("DuplicateHandle", 0);
	    }
	}

      // set child handles
      STARTUPINFOA siStartInfo;
      ZeroMemory (&siStartInfo, sizeof(siStartInfo));
      siStartInfo.cb = sizeof(siStartInfo);
      siStartInfo.dwFlags = STARTF_USESTDHANDLES;
      siStartInfo.hStdInput =
	(hChildStdin != INVALID_HANDLE_VALUE
	 ? hChildStdin
	 : GetStdHandle(STD_INPUT_HANDLE));
      siStartInfo.hStdOutput =
	(hChildStdout != INVALID_HANDLE_VALUE
	 ? hChildStdout
	 : GetStdHandle(STD_OUTPUT_HANDLE));
      siStartInfo.hStdError =
	(hChildStderr != INVALID_HANDLE_VALUE
	 ? hChildStderr
	 : GetStdHandle(STD_ERROR_HANDLE));

      // don't open a window if both stdout & stderr are redirected
      if (hChildStdout != INVALID_HANDLE_VALUE
	  && hChildStderr != INVALID_HANDLE_VALUE)
	{
	  siStartInfo.dwFlags |= STARTF_USESHOWWINDOW;
	  siStartInfo.wShowWindow = 0;
	}

      // build command-line
      string commandLine;
      commandLine = '"';
      commandLine += startinfo.FileName;
      commandLine += '"';
      if (! startinfo.Arguments.empty())
	{
	  commandLine += ' ';
	  commandLine += startinfo.Arguments;
	}

      // set environment variables
      SessionImpl::GetSession()->SetEnvironmentVariables ();
      
      // start child process
      SessionImpl::GetSession()->trace_process->WriteFormattedLine
	("core",
	 T_("start process: %s"),
	 commandLine.c_str());
      if (! CreateProcessA(fileName.Get(),
			   STRDUP(commandLine.c_str()).GetBuffer(),
			   0,	// lpProcessAttributes
			   0,	// lpThreadAttributes
			   TRUE,	// bInheritHandles
			   0,	// dwCreationFlags
			   0,	// lpEnvironment
			   (startinfo.WorkingDirectory == ""
			    ? 0
			    : startinfo.WorkingDirectory.c_str()),
			   &siStartInfo,
			   &processInformation))
	{
	  FATAL_WINDOWS_ERROR ("CreateProcess",
			       startinfo.FileName.c_str());
	}
      processStarted = true;
    }

  catch (const exception &)
    {
      if (hChildStdin != INVALID_HANDLE_VALUE)
	{
	  CloseHandle (hChildStdin);
	}
      if (hChildStdout != INVALID_HANDLE_VALUE)
	{
	  CloseHandle (hChildStdout);
	}
      if (hChildStderr != INVALID_HANDLE_VALUE)
	{
	  CloseHandle (hChildStderr);
	}
      if (standardInput != 0)
	{
	  CloseHandle (standardInput);
	}
      if (standardOutput != 0)
	{
	  CloseHandle (standardOutput);
	}
      if (standardError != 0)
	{
	  CloseHandle (standardError);
	}
      throw;
    }

  if (hChildStdin != INVALID_HANDLE_VALUE)
    {
      CloseHandle (hChildStdin);
    }

  if (hChildStdout != INVALID_HANDLE_VALUE)
    {
      CloseHandle (hChildStdout);
    }

  if (hChildStderr != INVALID_HANDLE_VALUE)
    {
      CloseHandle (hChildStderr);
    }
}


/* _________________________________________________________________________

   winProcess::winProcess
   _________________________________________________________________________ */

winProcess::winProcess (/*[in]*/ const ProcessStartInfo & startinfo)
  : startinfo (startinfo),
    standardInput (INVALID_HANDLE_VALUE),
    standardOutput (INVALID_HANDLE_VALUE),
    standardError (INVALID_HANDLE_VALUE),
    pFileStandardInput (0),
    pFileStandardOutput (0),
    pFileStandardError (0),
    processStarted (false)
{
  Create ();
}

/* _________________________________________________________________________

   winProcess::~winProcess
   _________________________________________________________________________ */

winProcess::~winProcess ()
{
  try
    {
      Close ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   winProcess::Close
   _________________________________________________________________________ */

void
winProcess::Close ()
{
  if (standardInput != INVALID_HANDLE_VALUE)
    {
      CloseHandle (standardInput);
      standardInput = INVALID_HANDLE_VALUE;
    }
  if (standardOutput != INVALID_HANDLE_VALUE)
    {
      CloseHandle (standardOutput);
      standardOutput = INVALID_HANDLE_VALUE;
    }
  if (standardError != INVALID_HANDLE_VALUE)
    {
      CloseHandle (standardError);
      standardError = INVALID_HANDLE_VALUE;
    }
  if (processStarted)
    {
      processStarted = false;
      CloseHandle (processInformation.hProcess);
      CloseHandle (processInformation.hThread);
    }
}

/* _________________________________________________________________________

   winProcess::get_StandardInput
   _________________________________________________________________________ */

FILE *
winProcess::get_StandardInput ()
{
  if (pFileStandardInput != 0)
    {
      return (pFileStandardInput);
    }
  if (standardInput == INVALID_HANDLE_VALUE)
    {
      return (0);
    }
  int fd =
    _open_osfhandle(reinterpret_cast<intptr_t>(standardInput),
		    O_WRONLY | O_BINARY);
  if (fd < 0)
    {
      FATAL_CRT_ERROR ("_open_osfhandle", 0);
    }
  pFileStandardInput = FdOpen(fd, "wb");
  standardInput = INVALID_HANDLE_VALUE;
  return (pFileStandardInput);
}

/* _________________________________________________________________________

   winProcess::get_StandardOutput
   _________________________________________________________________________ */

FILE *
winProcess::get_StandardOutput ()
{
  if (pFileStandardOutput != 0)
    {
      return (pFileStandardOutput);
    }
  if (standardOutput == INVALID_HANDLE_VALUE)
    {
      return (0);
    }
  int fd =
    _open_osfhandle(reinterpret_cast<intptr_t>(standardOutput),
		    O_RDONLY | O_BINARY);
  if (fd < 0)
    {
      FATAL_CRT_ERROR ("_open_osfhandle", 0);
    }
  pFileStandardOutput = FdOpen(fd, "rb");
  standardOutput = INVALID_HANDLE_VALUE;
  return (pFileStandardOutput);
}

/* _________________________________________________________________________

   winProcess::get_StandardError
   _________________________________________________________________________ */

FILE *
winProcess::get_StandardError ()
{
  if (pFileStandardError != 0)
    {
      return (pFileStandardError);
    }
  if (standardError == INVALID_HANDLE_VALUE)
    {
      return (0);
    }
  int fd =
    _open_osfhandle(reinterpret_cast<intptr_t>(standardError),
		    O_RDONLY | O_BINARY);
  if (fd < 0)
    {
      FATAL_CRT_ERROR ("_open_osfhandle", 0);
    }
  pFileStandardError = FdOpen(fd, "rb");
  standardError = INVALID_HANDLE_VALUE;
  return (pFileStandardError);
}

/* _________________________________________________________________________

   winProcess::WaitForExit
   _________________________________________________________________________ */

void
winProcess::WaitForExit ()
{
  WaitForSingleObject (processInformation.hProcess, INFINITE);
}

/* _________________________________________________________________________

   winProcess::WaitForExit
   _________________________________________________________________________ */

bool
winProcess::WaitForExit (/*[in]*/ int milliseconds)
{
  return (WaitForSingleObject(processInformation.hProcess,
			      static_cast<DWORD>(milliseconds))
	  == WAIT_OBJECT_0);
}

/* _________________________________________________________________________

   winProcess::get_ExitCode
   _________________________________________________________________________ */

int
winProcess::get_ExitCode ()
{
  DWORD exitCode;
  if (! GetExitCodeProcess(processInformation.hProcess, &exitCode))
    {
      FATAL_WINDOWS_ERROR ("GetExitCodeProcess", 0);
    }
  return (static_cast<int>(exitCode));
}

/* _________________________________________________________________________

   FindSystemShell
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(PathName)
FindSystemShell ()
{
  MIKTEXPERMANENTVAR(char) szCmd[BufferSizes::MaxPath] = { 0 };

  // locate the command interpreter
  if (szCmd[0] == 0)
    {
      string path;
      if (Utils::GetEnvironmentString("COMSPEC", path))
	{
	  if (! Utils::IsAbsolutePath(path.c_str()))
	    {
	      char * lpsz = 0;
	      if (! SearchPath(0,
			       path.c_str(),
			       0,
			       ARRAY_SIZE(szCmd),
			       szCmd,
			       &lpsz))
		{
		  szCmd[0] = 0;
		}
	    }
	  else if (File::Exists(path))
	    {
	      Utils::CopyString (szCmd, ARRAY_SIZE(szCmd), path.c_str());
	    }
	}
      if (szCmd[0] == 0)
	{
	  const char * lpszShell;
	  if (IsWindowsNT())
	    {
	      lpszShell = "cmd.exe";
	    }
	  else
	    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
	      lpszShell = "command.com";
#else
	      UNSUPPORTED_PLATFORM ();
#endif
	    }
	  char * lpsz = 0;
	  if (SearchPath(0,
			 lpszShell,
			 0,
			 ARRAY_SIZE(szCmd),
			 szCmd,
			 &lpsz)
	      == 0)
	    {
	      FATAL_MIKTEX_ERROR ("Process::ExecuteSystemCommand",
				  T_("COMSPEC not defined"),
				  0);
	    }
	}
    }

  return (szCmd);
}

/* _________________________________________________________________________

   Wrap
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(PathName)
Wrap (/*[in,out]*/ string &	arguments)
{
  PathName systemShell = FindSystemShell();

  bool wrapped;

  string wrappedArguments;

  if (IsWindowsNT())
    {
      wrapped = false;
    }
  else
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      // start a wrapper, if we are running on W9x
      PathName conspawn;
      if (! SessionImpl::GetSession()->FindFile("conspawn",
						FileType::EXE,
						conspawn))
	{
	  FATAL_MIKTEX_ERROR ("Process::ExecuteSystemCommand",
			      T_("cannot find conspawn.exe"),
			      0);
	}
      wrappedArguments = '\"';
      wrappedArguments += systemShell.Get();
      wrappedArguments += ' ';
      systemShell = conspawn;
      wrapped = true;
#else
      UNSUPPORTED_PLATFORM ();
#endif
    }

  wrappedArguments += "/c ";

  if (wrapped)
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      for (const char * lpsz = arguments.c_str(); *lpsz != 0; ++ lpsz)
	{
	  if (*lpsz == '"')
	    {
	      wrappedArguments += '\\';
	    }
	  wrappedArguments += *lpsz;
	}
      wrappedArguments += '"';
#else
      MIKTEX_ASSERT (! wrapped);
#endif
    }
  else
    {
      wrappedArguments += arguments;
    }

  arguments = wrappedArguments;

  return (systemShell);
}

/* _________________________________________________________________________

   Process::ExecuteSystemCommand

   Start cmd.exe with a command line.  Pass output (stdout & stderr)
   to caller.

   Suppose command-line is: tifftopnm "%i" | ppmtobmp -windows > "%o"

   Then we start as follows on Windows 95:

     conspawn "command /c tifftopnm \"%i\" | ppmtobmp -windows > \"%o\""

   conspawn.exe runs

     command /c tifftopnm "%i" | ppmtobmp -windows > "%o"
   _________________________________________________________________________ */

bool
Process::ExecuteSystemCommand (/*[in]*/ const char *	lpszCommandLine,
			       /*[out]*/ int *			pExitCode,
			       /*[in]*/ IRunProcessCallback *	pCallback,
			       /*[in]*/ const char *	lpszDirectory)
{
  string arguments (lpszCommandLine);
  PathName systemShell = Wrap(arguments);
  return (Process::Run(systemShell,
		       arguments.c_str(),
		       pCallback,
		       pExitCode,
		       lpszDirectory));
}

/* _________________________________________________________________________

   Process::StartSystemCommand
   _________________________________________________________________________ */

void
Process::StartSystemCommand (/*[in]*/ const char * lpszCommandLine)
{
  string arguments (lpszCommandLine);
  PathName systemShell = Wrap(arguments);
  Process::Start (systemShell, arguments.c_str());
}
