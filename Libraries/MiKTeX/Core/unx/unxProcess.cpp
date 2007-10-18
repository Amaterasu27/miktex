/* unxProcess.cpp:

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

#include "unx/unxProcess.h"

const int filenoStdin = 0;
const int filenoStdout = 1;
const int filenoStderr = 2;

#if defined(HAVE_FORK)
#  define FORK fork
#else
#  error this system does not have a working fork() function
#endif

#if defined(HAVE_VFORK)
#  define VFORK vfork
#else
#  define VFORK fork
#endif

/* _________________________________________________________________________

   Dup
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(int)
Dup (/*[in]*/ int fd)
{
  int dupfd = dup(fd);
  if (dupfd < 0)
    {
      FATAL_CRT_ERROR ("dup", 0);
    }
  return (dupfd);
}

/* _________________________________________________________________________

   Dup2
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
Dup2 (/*[in]*/ int fd,
	   /*[in]*/ int fd2)
{
  if (dup2(fd, fd2) < 0)
    {
      FATAL_CRT_ERROR ("dup2", 0);
    }
}

/* _________________________________________________________________________

   Close_
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
Close_ (/*[in]*/ int fd)
{
  if (close(fd) < 0)
    {
      FATAL_CRT_ERROR ("close", 0);
    }
}

/* _________________________________________________________________________

   Pipe
   _________________________________________________________________________ */

struct Pipe
{
public:
  Pipe ()
  {
    twofd[0] = -1;
    twofd[1] = -1;
  }

public:
  ~Pipe ()
  {
    try
      {
	Dispose ();
      }
    catch (const exception &)
      {
      }
  }

public:
  void
  Create ()
  {
    if (pipe(twofd) < 0)
      {
	FATAL_CRT_ERROR ("pipe", 0);
      }
  }

public:
  int
  GetReadEnd ()
    const
  {
    return (twofd[0]);
  }

public:
  int
  GetWriteEnd ()
    const
  {
    return (twofd[1]);
  }

public:
  int
  StealReadEnd ()
  {
    int fd = twofd[0];
    twofd[0] = -1;
    return (fd);
  }

public:
  int
  StealWriteEnd ()
  {
    int fd = twofd[1];
    twofd[1] = -1;
    return (fd);
  }

public:
  void
  CloseReadEnd ()
  {
    int fd = twofd[0];
    twofd[0] = -1;
    Close_ (fd);
  }

public:
  void
  CloseWriteEnd ()
  {
    int fd = twofd[1];
    twofd[1] = -1;
    Close_ (fd);
  }

public:
  void
  Dispose ()
  {
    if (twofd[0] >= 0)
      {
	CloseReadEnd ();
      }
    if (twofd[1] >= 0)
      {
	CloseWriteEnd ();
      }
  }

private:
  int twofd[2];
};

/* _________________________________________________________________________

   Argv::Append

   "borrowed" from the popt library
   _________________________________________________________________________ */

void
MIKTEXCALL
Argv::Append (/*[in]*/ const char *	lpszArguments)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszArguments);
  MIKTEX_ASSERT (argv.size() > 0);

  argv.pop_back ();

  if (argv.size() == 0)
    {
      argv.push_back (StrDup("foo"));
    }

  string arg;
  char quote = 0;
  for (const char * lpsz = lpszArguments; *lpsz != 0; ++ lpsz)
    {
      if (*lpsz == quote)
	{
	  quote = 0;
	}
      else if (quote != 0)
	{
	  if (*lpsz == '\\')
	    {
	      ++ lpsz;
	      if (*lpsz == 0)
		{
		  FATAL_MIKTEX_ERROR ("Argv::Build",
				      T_("Invalid command-line."),
				      lpszArguments);
		}
	      if (*lpsz != quote)
		{
		  arg += '\\';
		}
	    }
	  arg += *lpsz;
	}
      else if (IsSpace(*lpsz))
	{
	  if (arg.length() > 0)
	    {
	      argv.push_back (StrDup(arg.c_str()));
	      arg = "";
	    }
	}
      else
	{
	  switch (*lpsz)
	    {
	    case '"':
	    case '\'':
	      quote = *lpsz;
	      break;
	    case '\\':
	      ++ lpsz;
	      if (*lpsz == 0)
		  {
		    FATAL_MIKTEX_ERROR ("Argv::Build",
					T_("Invalid command-line."),
					lpszArguments);
		  }
	      arg += *lpsz;
	      break;
	    default:
	      arg += *lpsz;
	      break;
	    }
	}
    }
  if (! arg.empty())
    {
      argv.push_back (StrDup(arg.c_str()));
    }
  argv.push_back (0);
}

/* _________________________________________________________________________

   Process::Start
   _________________________________________________________________________ */

Process *
Process::Start (/*[in]*/ const ProcessStartInfo & startinfo)
{
  return (new unxProcess (startinfo));
}

/* _________________________________________________________________________

   unxProcess::Create
   _________________________________________________________________________ */

#if defined(NDEBUG)
#  define TRACEREDIR 0
#else
#  define TRACEREDIR 1
#endif

void
unxProcess::Create ()
{
  if (startinfo.FileName.length() == 0)
    {
      INVALID_ARGUMENT ("unxProcess::Create", 0);
    }

  Argv argv;
  argv.Build (startinfo.FileName.c_str(), startinfo.Arguments.c_str());

  Pipe pipeStdout;
  Pipe pipeStderr;
  Pipe pipeStdin;

  int fdChildStdin = -1;
  int fdChildStderr = -1;

  // create stdout pipe
  if (startinfo.RedirectStandardOutput)
    {
      pipeStdout.Create ();
#if 1
      if (! startinfo.RedirectStandardError)
	{
	  fdChildStderr = Dup(pipeStdout.GetWriteEnd());
	}
#endif
    }

  // create stderr pipe
  if (startinfo.RedirectStandardError)
    {
      pipeStderr.Create ();
    }

  // use file descriptor or create stdin pipe
  if (startinfo.StandardInput != 0)
    {
      // caller feeds stdin via a stdio stream
      fdChildStdin = fileno(startinfo.StandardInput);
      if (fdChildStdin < 0)
	{
	  FATAL_CRT_ERROR ("fileno", 0);
	}
    }
  else if (startinfo.RedirectStandardInput)
    {
      pipeStdin.Create ();
    }

  // fork
  SessionImpl::theSession->trace_process->WriteFormattedLine
    ("core",
     T_("forking..."));
  if (pipeStdout.GetReadEnd() >=0
      || pipeStderr.GetReadEnd() >= 0
      || pipeStdin.GetReadEnd() >= 0
      || fdChildStdin >= 0
      || fdChildStderr >= 0)
    {
      pid = FORK();
    }
  else
    {
      pid = VFORK();
    }
  if (pid < 0)
    {
      FATAL_CRT_ERROR ("fork", 0);
    }

  if (pid == 0)
    {
      try
	{
	  // I'm a child
	  if (pipeStdout.GetWriteEnd() >= 0)
	    {
	      Dup2 (pipeStdout.GetWriteEnd(), filenoStdout);
	    }
	  if (pipeStderr.GetWriteEnd() >= 0)
	    {
	      Dup2 (pipeStderr.GetWriteEnd(), filenoStderr);
	    }
	  else if (fdChildStderr >= 0)
	    {
	      Dup2 (fdChildStderr, filenoStderr);
	      ::Close_ (fdChildStderr);
	    }
	  if (pipeStdin.GetReadEnd() >= 0)
	    {
	      Dup2 (pipeStdin.GetReadEnd(), filenoStdin);
	    }
	  else if (fdChildStdin >= 0)
	    {
	      Dup2 (fdChildStdin, filenoStdin);
	      ::Close_ (fdChildStdin);
	    }
	  pipeStdout.Dispose ();
	  pipeStderr.Dispose ();
	  pipeStdin.Dispose ();
	  SessionImpl::GetSession()->SetEnvironmentVariables ();
	  execv (startinfo.FileName.c_str(),
		 const_cast<char * const *>(argv.GetArgv()));
	  perror ("execv failed");
	}
      catch (const exception &)
	{
	}
      _exit (127);
    }

  MIKTEX_ASSERT (pid > 0);

  if (startinfo.RedirectStandardOutput)
    {
      fdStandardOutput = pipeStdout.StealReadEnd();
    }

  if (startinfo.RedirectStandardError)
    {
      fdStandardError = pipeStderr.StealReadEnd();
    }

  if (startinfo.RedirectStandardInput)
    {
      fdStandardInput = pipeStdin.StealWriteEnd();
    }

  if (fdChildStderr >= 0)
    {
      ::Close_ (fdChildStderr);
    }

  pipeStdout.Dispose ();
  pipeStderr.Dispose ();
  pipeStdin.Dispose ();
}

/* _________________________________________________________________________

   unxProcess::unxProcess
   _________________________________________________________________________ */

unxProcess::unxProcess (/*[in]*/ const ProcessStartInfo & startinfo)
  : startinfo (startinfo),
    fdStandardInput (-1),
    fdStandardOutput (-1),
    fdStandardError (-1),
    pFileStandardInput (0),
    pFileStandardOutput (0),
    pFileStandardError (0),
    pid (-1)
{
  Create ();
}

/* _________________________________________________________________________

   unxProcess::~unxProcess
   _________________________________________________________________________ */

unxProcess::~unxProcess ()
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

   WaitThread
   _________________________________________________________________________ */

namespace {
  struct Data
  {
    Data (/*[in]*/ pid_t pid)
      : pid (pid)
    {
    };
    pid_t	pid;
  };
}

MIKTEXSTATICFUNC(void)
WaitThread (/*[in]*/ void * p)
{
  try
    {
      Data * pData = reinterpret_cast<Data*>(p);
      pid_t pid = pData->pid;
      free (pData);
      while (true)
	{
	  int status;
	  pid_t pid2 = waitpid(pid, &status, WNOHANG);
	  if (pid2 < 0)
	    {
	      FATAL_CRT_ERROR ("waitpid", 0);
	    }
	  if (pid2 > 0)
	    {
	      return;
	    }
	  Thread::Sleep (100);
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   unxProcess::Close
   _________________________________________________________________________ */

void
unxProcess::Close ()
{
  if (fdStandardOutput >= 0)
    {
      close (fdStandardOutput);
      fdStandardOutput = -1;
    }
  if (fdStandardError >= 0)
    {
      close (fdStandardError);
      fdStandardError = -1;
    }
  if (fdStandardInput >= 0)
    {
      close (fdStandardInput);
      fdStandardInput = -1;
    }
  if (! WaitForExit(0))
    {
      pid_t pid = this->pid;
      this->pid = -1;
      SessionImpl::theSession->trace_process->WriteFormattedLine ("core",
	     T_("starting wait thread"));
      Thread * pThread = Thread::Start(WaitThread, new Data (pid));
      delete pThread;
    }
}

/* _________________________________________________________________________

   unxProcess::get_StandardInput
   _________________________________________________________________________ */

FILE *
unxProcess::get_StandardInput ()
{
  if (pFileStandardInput != 0)
    {
      return (pFileStandardInput);
    }
  if (fdStandardInput < 0)
    {
      return (0);
    }
  pFileStandardInput = FdOpen(fdStandardInput, "wb");
  fdStandardInput = -1;
  return (pFileStandardInput);
}

/* _________________________________________________________________________

   unxProcess::get_StandardOutput
   _________________________________________________________________________ */

FILE *
unxProcess::get_StandardOutput ()
{
  if (pFileStandardOutput != 0)
    {
      return (pFileStandardOutput);
    }
  if (fdStandardOutput < 0)
    {
      return (0);
    }
  pFileStandardOutput = FdOpen(fdStandardOutput, "rb");
  fdStandardOutput = -1;
  return (pFileStandardOutput);
}

/* _________________________________________________________________________

   unxProcess::get_StandardError
   _________________________________________________________________________ */

FILE *
unxProcess::get_StandardError ()
{
  if (pFileStandardError != 0)
    {
      return (pFileStandardError);
    }
  if (fdStandardError < 0)
    {
      return (0);
    }
  pFileStandardError = FdOpen(fdStandardError, "rb");
  fdStandardError = -1;
  return (pFileStandardError);
}

/* _________________________________________________________________________

   unxProcess::WaitForExit
   _________________________________________________________________________ */

void
unxProcess::WaitForExit ()
{
  if (this->pid > 0)
    {
      SessionImpl::theSession->trace_process->WriteFormattedLine ("core",
	     T_("waiting for process %d"),
	     static_cast<int>(this->pid));
      pid_t pid = this->pid;
      this->pid = -1;
      if (waitpid(pid, &status, 0) <= 0)
	{
	  FATAL_CRT_ERROR ("waitpid", 0);
	}
      SessionImpl::theSession->trace_process->WriteFormattedLine ("core",
	     T_("process %d exited with status %d"),
	     static_cast<int>(pid),
	     status);
    }
}

/* _________________________________________________________________________

   unxProcess::WaitForExit
   _________________________________________________________________________ */

bool
unxProcess::WaitForExit (/*[in]*/ int milliseconds)
{
  if (this->pid <= 0)
    {
      return (true);
    }
  do
    {
      pid_t pid = waitpid(this->pid, &status, WNOHANG);
      if (pid == this->pid)
	{
	  this->pid = -1;
	  return (true);
	}
      else if (pid < 0)
	{
	  this->pid = -1;
	  FATAL_CRT_ERROR ("waitpid", 0);
	}
      MIKTEX_ASSERT (pid == 0);
      Thread::Sleep (1);
      -- milliseconds;
    }
  while (milliseconds > 0);
  return (false);
}

/* _________________________________________________________________________

   unxProcess::get_ExitCode
   _________________________________________________________________________ */

int
unxProcess::get_ExitCode ()
{
  if (WIFEXITED(status) != 0)
    {
      return (WEXITSTATUS(status));
    }
  else
    {
      return (-1);
    }
}

/* _________________________________________________________________________

   FindSystemShell
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(PathName)
FindSystemShell ()
{
  PathName ret;
  const char * lpszPathList;
#if defined(HAVE_CONFSTR)
  size_t n = confstr(_CS_PATH, 0, 0);
  if (n == 0)
    {
      FATAL_CRT_ERROR ("confstr", 0);
    }
  CharBuffer pathList (n);
  n = confstr(_CS_PATH, pathList.GetBuffer(), n);
  if (n == 0)
    {
      FATAL_CRT_ERROR ("confstr", 0);
    }
  if (n > pathList.GetCapacity())
    {
      UNEXPECTED_CONDITION ("Process::ExecuteSystemCommand");
    }
  lpszPathList = pathList.GetBuffer();
#else
  lpszPathList = "/bin";
#endif
  if (! SessionImpl::theSession->FindFile("sh", lpszPathList, ret))
    {
      FATAL_MIKTEX_ERROR ("Process:ExecuteSystemCommand",
			  T_("The command processor could not be found."),
			  lpszPathList);
    }
  return (ret);
}

/* _________________________________________________________________________

   Process::StartSystemCommand
   _________________________________________________________________________ */

MIKTEXAPI(void)
Process::StartSystemCommand (/*[in]*/ const char * lpszCommandLine)
{
  string arguments = "-c '";
  arguments += lpszCommandLine;
  arguments += '\'';
  Process::Start (FindSystemShell(), arguments.c_str());
}

/* _________________________________________________________________________

   Process::ExecuteSystemCommand

   Start the command processor (usually /bin/sh) with a command line.
   Pass output (stdout & stderr) to caller.

   Suppose command-line is: tifftopnm "%i" | ppmtobmp -windows > "%o"

   Then we start as follows:

     /bin/sh -c 'tifftopnm "%i" | ppmtobmp -windows > "%o"'
   _________________________________________________________________________ */

MIKTEXAPI(bool)
Process::ExecuteSystemCommand (/*[in]*/ const char * lpszCommandLine,
			       /*[out]*/ int *			pExitCode,
			       /*[in]*/ IRunProcessCallback *	pCallback,
			       /*[in]*/ const char *	lpszDirectory)
{
  string arguments = "-c '";
  arguments += lpszCommandLine;
  arguments += '\'';
  return (Process::Run(FindSystemShell(),
		       arguments.c_str(),
		       pCallback,
		       pExitCode,
		       lpszDirectory));
}
