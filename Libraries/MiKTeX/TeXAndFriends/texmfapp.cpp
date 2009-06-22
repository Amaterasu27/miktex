/* texmfapp.cpp:

   Copyright (C) 1996-2009 Christian Schenk
 
   This file is part of the MiKTeX TeXMF Library.

   The MiKTeX TeXMF Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX TeXMF Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX TeXMF Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   TraceExecutionTime
   _________________________________________________________________________ */

STATICFUNC(void)
TraceExecutionTime (/*[in]*/ TraceStream *	trace_time,
		    /*[in]*/ clock_t		clockStart)
{
  clock_t clockSinceStart = clock() - clockStart;
  trace_time->WriteFormattedLine ("libtexmf",
				  T_("gross execution time: %u ms"),
				  static_cast<unsigned>(clockSinceStart));
  fprintf (stderr,
	   T_("gross execution time: %u ms\n"),
	   static_cast<unsigned>(clockSinceStart));
#if defined(MIKTEX_WINDOWS)
  if (! IsWindowsNT())
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      return;
#else
      UNSUPPORTED_PLATFORM ();
#endif
    }
  HINSTANCE hinstKernel;
  hinstKernel = LoadLibraryA("kernel32.dll");
  if (hinstKernel == 0)
    {
      return;
    }
  FARPROC pfGetProcessTimes;
  pfGetProcessTimes = GetProcAddress(hinstKernel, "GetProcessTimes");
  if (pfGetProcessTimes == 0)
    {
      return;
    }
  FILETIME ftCreate, ftExit, ftKernel, ftUser;
  if (! GetProcessTimes(GetCurrentProcess(), &ftCreate, &ftExit,
			&ftKernel, &ftUser))
    {
      return;
    }
  LARGE_INTEGER tUser64;
  LARGE_INTEGER tKernel64;
  DWORD tUser, tKernel;
  tUser64.LowPart = ftUser.dwLowDateTime;
  tUser64.HighPart = ftUser.dwHighDateTime;
  tKernel64.LowPart = ftKernel.dwLowDateTime;
  tKernel64.HighPart = ftKernel.dwHighDateTime;
  tUser = static_cast<DWORD>(tUser64.QuadPart / 10000);
  tKernel = static_cast<DWORD>(tKernel64.QuadPart / 10000);
  trace_time->WriteFormattedLine
    ("libtexmf",
     T_("user mode: %u ms, kernel mode: %u ms, total: %u"),
     static_cast<unsigned>(tUser),
     static_cast<unsigned>(tKernel),
     static_cast<unsigned>(tUser + tKernel));
  fprintf (stderr,
	   T_("user mode: %u ms, kernel mode: %u ms, total: %u\n"),
	   static_cast<unsigned>(tUser),
	   static_cast<unsigned>(tKernel),
	   static_cast<unsigned>(tUser + tKernel));
#endif // MIKTEX_WINDOWS
}

/* _________________________________________________________________________
   
   TeXMFApp::Init
   _________________________________________________________________________ */

void
TeXMFApp::Init (/*[in]*/ const char * lpszProgramInvocationName)
{
  WebAppInputLine::Init (lpszProgramInvocationName);

  trace_time = auto_ptr<TraceStream> (TraceStream::Open(MIKTEX_TRACE_TIME));
  trace_mem = auto_ptr<TraceStream> (TraceStream::Open(MIKTEX_TRACE_MEM));

  clockStart = clock();
  disableExtensions = false;
  haltOnError = false;
  interactionMode = -1;
  isInitProgram = false;
  isTeXProgram = false;
  param_buf_size = -1;
  param_error_line = -1;
  param_extra_mem_bot = -1;
  param_extra_mem_top = -1;
  param_half_error_line = -1;
  param_max_print_line = -1;
  param_max_strings = -1;
  param_main_memory = -1;
  param_param_size = -1;
  param_pool_free = -1;
  param_pool_size = -1;
  param_stack_size = -1;
  param_strings_free = -1;
  param_string_vacancies = -1;
  parseFirstLine = false;
  recordFileNames = false;
  setJobTime = false;
  showFileLineErrorMessages = false;
  timeStatistics = false;
}

/* _________________________________________________________________________
   
   TeXMFApp::Finalize
   _________________________________________________________________________ */

void
TeXMFApp::Finalize ()
{
  if (trace_time.get() != 0)
    {
      trace_time->Close ();
      trace_time.reset ();
    }
  if (trace_mem.get() != 0)
    {
      trace_mem->Close ();
      trace_mem.reset ();
    }
  memoryDumpFileName = "";
  jobName = "";
  WebAppInputLine::Finalize ();
}

/* _________________________________________________________________________

   TeXMFApp::OnTeXMFStartJob
   _________________________________________________________________________ */

void
TeXMFApp::OnTeXMFStartJob ()
{
  MIKTEX_ASSERT_STRING (TheNameOfTheGame());
  string appName;
  for (const char * lpsz = TheNameOfTheGame(); *lpsz != 0; ++ lpsz)
    {
      if (*lpsz != '-')		// pdf-e-tex => pdfetex
	{
	  appName += *lpsz;
	}
    }
  pSession->PushBackAppName (appName.c_str());
  parseFirstLine =
    pSession->GetConfigValue(0, MIKTEX_REGVAL_PARSE_FIRST_LINE, isTeXProgram);
  showFileLineErrorMessages =
    pSession->GetConfigValue(0, MIKTEX_REGVAL_C_STYLE_ERRORS, false);
  EnablePipes (pSession->GetConfigValue(0,
					MIKTEX_REGVAL_ENABLE_PIPES,
					false));
  clockStart = clock();
}

/* _________________________________________________________________________

   TeXMFApp::OnTeXMFFinishJob
   _________________________________________________________________________ */

void
TeXMFApp::OnTeXMFFinishJob ()
{
  if (recordFileNames)
    {
      StreamWriter writer (PathName(0, jobName.c_str(), "fls"));
      PathName cwd;
      cwd.SetToCurrentDirectory ();
      writer.WriteFormattedLine ("PWD %s", cwd.ToUnix().Get());
      vector<FileInfoRecord> fileInfoRecords = pSession->GetFileInfoRecords();
      for (vector<FileInfoRecord>::const_iterator it = fileInfoRecords.begin();
	   it != fileInfoRecords.end();
	   ++ it)
	{
	  writer.WriteFormattedLine ("%s %s",
				     (it->access == FileAccess::Read
				      ? "INPUT"
				      : "OUTPUT"),
				     PathName(it->fileName).ToUnix().Get());
	}
      writer.Close ();
    }
  if (timeStatistics && trace_time->IsEnabled())
    {
      TraceExecutionTime (trace_time.get(), clockStart);
    }
}

/* _________________________________________________________________________
   
   TeXMFApp::AddOptions
   _________________________________________________________________________ */

enum {
  OPT_AUX_DIRECTORY,
  OPT_BUF_SIZE,
  OPT_C_STYLE_ERRORS,
  OPT_DISABLE_8BIT_CHARS,
  OPT_DONT_PARSE_FIRST_LINE,
  OPT_ENABLE_8BIT_CHARS,
  OPT_ERROR_LINE,
  OPT_EXTRA_MEM_BOT,
  OPT_EXTRA_MEM_TOP,
  OPT_HALF_ERROR_LINE,
  OPT_HALT_ON_ERROR,
  OPT_INITIALIZE,
  OPT_INTERACTION,
  OPT_JOB_NAME,
  OPT_JOB_TIME,
  OPT_MAIN_MEMORY,
  OPT_MAX_PRINT_LINE,
  OPT_MAX_STRINGS,
  OPT_NO_C_STYLE_ERRORS,
  OPT_OUTPUT_DIRECTORY,
  OPT_PARAM_SIZE,
  OPT_PARSE_FIRST_LINE,
  OPT_POOL_FREE,
  OPT_POOL_SIZE,
  OPT_QUIET,
  OPT_RECORDER,
  OPT_STACK_SIZE,
  OPT_STRICT,
  OPT_STRINGS_FREE,
  OPT_STRING_VACANCIES,
  OPT_TCX,
  OPT_TIME_STATISTICS,
  OPT_UNDUMP,
};

void
TeXMFApp::AddOptions ()
{
  WebAppInputLine::AddOptions ();

  bool invokedAsInitProgram = false;
  if (Utils::Contains(GetInitProgramName(), Utils::GetExeName().c_str()))
    {
      invokedAsInitProgram = true;
    }

  optBase = static_cast<int>(GetOptions().size());

  if (IsFeatureEnabled(Feature::EightBitChars))
    {
      AddOption (T_("enable-8bit-chars\0\
Make all characters printable by default."),
		 FIRST_OPTION_VAL + optBase + OPT_ENABLE_8BIT_CHARS);
      AddOption (T_("disable-8bit-chars\0\
Make only 7-bit characters printable by."),
		 FIRST_OPTION_VAL + optBase + OPT_DISABLE_8BIT_CHARS);
    }

  AddOption (T_("aux-directory\0\
Use DIR as the directory to write auxiliary files to."),
	     FIRST_OPTION_VAL + optBase + OPT_AUX_DIRECTORY,
	     required_argument,
	     "DIR");

  AddOption (T_("buf-size\0\
Set buf_size to N."),
	     FIRST_OPTION_VAL + optBase + OPT_BUF_SIZE,
	     required_argument,
	     "N");

  AddOption (T_("c-style-errors\0\
Enable file:line:error style messages."),
	     FIRST_OPTION_VAL + optBase + OPT_C_STYLE_ERRORS);

  AddOption (T_("dont-parse-first-line\0\
Do not parse the first line of the input line to look for a dump name and/or\
 extra command-line options."),
	     FIRST_OPTION_VAL + optBase + OPT_DONT_PARSE_FIRST_LINE);

  AddOption (T_("error-line\0\
Set error_line to N."),
	     FIRST_OPTION_VAL + optBase + OPT_ERROR_LINE,
	     required_argument,
	     "N");

  if (isTeXProgram)
    {
      AddOption (T_("extra-mem-bot\0\
Set extra_mem_bot to N."),
		 FIRST_OPTION_VAL + optBase + OPT_EXTRA_MEM_BOT,
		 required_argument,
		 "N");
    }

  if (isTeXProgram || AmI("mpost"))
    {
      AddOption (T_("extra-mem-top\0\
Set extra_mem_top to N."),
		 FIRST_OPTION_VAL + optBase + OPT_EXTRA_MEM_TOP,
		 required_argument,
		 "N");
    }

  AddOption (T_("half-error-line\0\
Set half_error_line to N."),
	     FIRST_OPTION_VAL + optBase + OPT_HALF_ERROR_LINE,
	     required_argument,
	     "N");

  AddOption (T_("halt-on-error\0\
Stop after the first error."),
	     FIRST_OPTION_VAL + optBase + OPT_HALT_ON_ERROR);

  if (! invokedAsInitProgram)
    {
      AddOption (T_("initialize\0\
Be the INI variant of the program."),
		 FIRST_OPTION_VAL + optBase + OPT_INITIALIZE);
    }

  AddOption (T_("interaction\0\
Set the interaction mode; MODE must be one of:\
 batchmode, nonstopmode, scrollmode, errorstopmode."),
	     FIRST_OPTION_VAL + optBase + OPT_INTERACTION,
	     required_argument,
	     "MODE");

  AddOption (T_("job-name\0\
Set the job name and hence the name(s) of the output file(s)."),
	     FIRST_OPTION_VAL + optBase + OPT_JOB_NAME,
	     required_argument,
	     "NAME");

  AddOption (T_("job-time\0\
Set the job time.  Take FILE's timestamp as the reference."),
	     FIRST_OPTION_VAL + optBase + OPT_JOB_TIME,
	     required_argument,
	     "FILE");

  AddOption (T_("main-memory\0\
Set main_memory to N."),
	     FIRST_OPTION_VAL + optBase + OPT_MAIN_MEMORY,
	     required_argument,
	     "N");

  AddOption (T_("max-print-line\0\
Set max_print_line to N."),
	     FIRST_OPTION_VAL + optBase + OPT_MAX_PRINT_LINE,
	     required_argument,
	     "N");

  AddOption (T_("max-strings\0\
Set max_strings to N."),
	     FIRST_OPTION_VAL + optBase + OPT_MAX_STRINGS,
	     required_argument,
	     "N");

  AddOption (T_("no-c-style-errors\0\
Disable file:line:error style messages."),
	     FIRST_OPTION_VAL + optBase + OPT_NO_C_STYLE_ERRORS);

  AddOption (T_("output-directory\0\
Use DIR as the directory to write output files to."),
	     FIRST_OPTION_VAL + optBase + OPT_OUTPUT_DIRECTORY,
	     required_argument,
	     "DIR");

  AddOption (T_("param-size\0\
Set param_size to N."),
	     FIRST_OPTION_VAL + optBase + OPT_PARAM_SIZE,
	     required_argument,
	     "N");

  AddOption (T_("parse-first-line\0\
Parse the first line of the input line to look for a dump name and/or\
 extra command-line options."),
	     FIRST_OPTION_VAL + optBase + OPT_PARSE_FIRST_LINE,
	     no_argument);

  if (isTeXProgram)
    {
      AddOption (T_("pool-free\0\
Set pool_free to N."),
		 FIRST_OPTION_VAL + optBase + OPT_POOL_FREE,
		 required_argument,
		 "N");
    }

  AddOption (T_("pool-size\0\
Set pool_size to N."),
	     FIRST_OPTION_VAL + optBase + OPT_POOL_SIZE,
	     required_argument,
	     "N");

  AddOption (T_("quiet\0\
Suppress all output (except errors)."),
	     FIRST_OPTION_VAL + optBase + OPT_QUIET);

  AddOption (T_("recorder\0\
Turn on the file name recorder to leave a trace of the files\
 opened for input and output in a file with extension .fls."),
	     FIRST_OPTION_VAL + optBase + OPT_RECORDER);

  AddOption (T_("stack-size\0\
Set stack_size to N."),
	     FIRST_OPTION_VAL + optBase + OPT_STACK_SIZE,
	     required_argument,
	     "N");

  AddOption (T_("strict\0\
Disable MiKTeX extensions."),
	     FIRST_OPTION_VAL + optBase + OPT_STRICT,
	     no_argument | POPT_ARGFLAG_DOC_HIDDEN);

  if (isTeXProgram)
    {
      AddOption (T_("strings-free\0\
Set strings_free to N."),
		 FIRST_OPTION_VAL + optBase + OPT_STRINGS_FREE,
		 required_argument,
		 "N");
    }

  AddOption (T_("string-vacancies\0\
Set string_vacancies to N."),
	     FIRST_OPTION_VAL + optBase + OPT_STRING_VACANCIES,
	     required_argument,
	     "N");

  AddOption (T_("time-statistics\0\
Show processing time statistics."),
	     FIRST_OPTION_VAL + optBase + OPT_TIME_STATISTICS);

  AddOption (T_("undump\0\
Use NAME instead of program name when loading internal tables."),
	     FIRST_OPTION_VAL + optBase + OPT_UNDUMP,
	     required_argument,
	     "NAME");

  if (IsFeatureEnabled(Feature::TCX))
    {
      AddOption (T_("tcx\0\
Use the TCXNAME translation table to set the mapping of input\
characters and re-mapping of output characters."),
		 FIRST_OPTION_VAL + optBase + OPT_TCX,
		 required_argument,
		 "TCXNAME");
    }

  // old option names
  if (! invokedAsInitProgram)
    {
      AddOption ("ini", "initialize");
    }
  AddOption ("silent", "quiet");
  if (IsFeatureEnabled(Feature::TCX))
    {
      AddOption ("translate-file", "tcx");
    }

  // supported Web2C options
  if (IsFeatureEnabled(Feature::EightBitChars))
    {
      AddOption ("8bit", "enable-8bit-chars");
    }
  AddOption ("file-line-error", "c-style-errors");
  AddOption ("file-line-error-style", "c-style-errors");
  AddOption ("jobname", "job-name");
  AddOption ("no-file-line-error", "no-c-style-errors");
  AddOption ("no-parse-first-line", "dont-parse-first-line");
  AddOption ("progname", "alias");

  // unsupported Web2C options
  AddOption ("default-translate-file", OPT_UNSUPPORTED, required_argument);
  AddOption ("maketex\0", OPT_UNSUPPORTED, required_argument);
  AddOption ("mktex\0", OPT_UNSUPPORTED, required_argument);
  AddOption ("no-maketex\0", OPT_UNSUPPORTED, required_argument);
  AddOption ("no-mktex\0", OPT_UNSUPPORTED, required_argument);
}

/* _________________________________________________________________________

   TeXMFApp::ProcessOption
   _________________________________________________________________________ */

bool
TeXMFApp::ProcessOption (/*[in]*/ int			opt,
			 /*[in]*/ const char *	lpszOptArg)
{
  bool done = true;

  switch (opt - FIRST_OPTION_VAL - optBase)
    {

    case OPT_ENABLE_8BIT_CHARS:
      Enable8BitChars (true);
      break;

    case OPT_DISABLE_8BIT_CHARS:
      Enable8BitChars (false);
      break;

    case OPT_AUX_DIRECTORY:
      auxDirectory = lpszOptArg;
      auxDirectory.MakeAbsolute ();
      if (! Directory::Exists(auxDirectory))
	{
	  FATAL_MIKTEX_ERROR ("TeXMFApp::ProcessOption",
			      T_("The specified directory does not exist."),
			      auxDirectory.Get());
	}
      pSession->AddInputDirectory (auxDirectory.Get(), true);
      break;

    case OPT_BUF_SIZE:
      param_buf_size = atoi(lpszOptArg);
      break;

    case OPT_C_STYLE_ERRORS:
      showFileLineErrorMessages = true;
      break;

    case OPT_DONT_PARSE_FIRST_LINE:
      parseFirstLine = false;
      break;

    case OPT_ERROR_LINE:
      param_error_line = atoi(lpszOptArg);
      break;

    case OPT_EXTRA_MEM_BOT:
      param_extra_mem_bot = atoi(lpszOptArg);
      break;

    case OPT_EXTRA_MEM_TOP:
      param_extra_mem_top = atoi(lpszOptArg);
      break;

    case OPT_HALF_ERROR_LINE:
      param_half_error_line = atoi(lpszOptArg);
      break;

    case OPT_HALT_ON_ERROR:
      haltOnError = true;
      break;

    case OPT_INITIALIZE:
      isInitProgram = true;
      break;

    case OPT_INTERACTION:
      if (StringCompare(lpszOptArg, "batchmode") == 0)
	{
	  interactionMode = 0;
	}
      else if (StringCompare(lpszOptArg, "nonstopmode") == 0)
	{
	  interactionMode = 1;
	}
      else if (StringCompare(lpszOptArg, "scrollmode") == 0)
	{
	  interactionMode = 2;
	}
      else if (StringCompare(lpszOptArg, "errorstopmode") == 0)
	{
	  interactionMode = 3;
	}
      else
	{
	  FATAL_MIKTEX_ERROR ("TeXMFApp::ProcessOption",
			      T_("Invalid option argument."),
			      lpszOptArg);
	}
      break;

    case OPT_JOB_NAME:
      jobName = MangleNameOfFile(lpszOptArg).Get();
      break;

    case OPT_JOB_TIME:
      {
	time_t jobTime;
	if (isdigit(lpszOptArg[0]))
	  {
	    jobTime = atoi(lpszOptArg);
	  }
	else
	  {
	    time_t creationTime;
	    time_t lastAccessTime;
	    time_t lastWriteTime;
	    File::GetTimes (lpszOptArg,
			    creationTime,
			    lastAccessTime,
			    lastWriteTime);
	    jobTime = lastWriteTime;
	  }
	SetStartUpTime (jobTime);
	setJobTime = true;
      }
      break;

    case OPT_MAIN_MEMORY:
      param_main_memory = atoi(lpszOptArg);
      break;

    case OPT_MAX_PRINT_LINE:
      param_max_print_line = atoi(lpszOptArg);
      break;

    case OPT_MAX_STRINGS:
      param_max_strings = atoi(lpszOptArg);
      break;

    case OPT_TIME_STATISTICS:
      timeStatistics = true;
      break;

    case OPT_NO_C_STYLE_ERRORS:
      showFileLineErrorMessages = false;
      break;

    case OPT_OUTPUT_DIRECTORY:
      outputDirectory = lpszOptArg;
      outputDirectory.MakeAbsolute ();
      if (! Directory::Exists(outputDirectory))
	{
	  FATAL_MIKTEX_ERROR ("TeXMFApp::ProcessOption",
			      T_("The specified directory does not exist."),
			      outputDirectory.Get());
	}
      if (auxDirectory[0] == 0)
	{
	  auxDirectory = outputDirectory;
	}
      pSession->AddInputDirectory (outputDirectory.Get(), true);
      break;

    case OPT_PARAM_SIZE:
      param_param_size = atoi(lpszOptArg);
      break;

    case OPT_PARSE_FIRST_LINE:
      parseFirstLine = true;
      break;

    case OPT_POOL_FREE:
      param_pool_free = atoi(lpszOptArg);
      break;

    case OPT_POOL_SIZE:
      param_pool_size = atoi(lpszOptArg);
      break;

    case OPT_QUIET:
      SetQuietFlag (true);
      break;

    case OPT_RECORDER:
      pSession->StartFileInfoRecorder (false);
      recordFileNames = true;
      break;

    case OPT_STACK_SIZE:
      param_stack_size = atoi(lpszOptArg);
      break;

    case OPT_STRICT:
      disableExtensions = true;
      pSession->EnableFontMaker (false);
      break;

    case OPT_STRING_VACANCIES:
      param_string_vacancies = atoi(lpszOptArg);
      break;

    case OPT_TCX:
      SetTcxFileName (lpszOptArg);
      break;

    case OPT_UNDUMP:
      memoryDumpFileName = lpszOptArg;
      break;

    default:
      done = WebAppInputLine::ProcessOption(opt, lpszOptArg);
      break;
    }

  return (done);
}

/* _________________________________________________________________________

   TeXMFApp::ParseFirstLine
   _________________________________________________________________________ */

bool
TeXMFApp::ParseFirstLine (/*[in]*/ const PathName &		path,
			  /*[in,out]*/ MiKTeX::Core::Argv &	argv)
{
  StreamReader reader (path);

  string firstLine;

  if (! reader.ReadLine(firstLine))
    {
      return (false);
    }

  reader.Close ();

  if (! (firstLine.substr(0, 2) == "%&"))
    {
      return (false);
    }

  argv.Build ("foo", firstLine.c_str() + 2);

  return (argv.GetArgc() > 1);
}

/* _________________________________________________________________________

   TeXMFApp::ParseFirstLine
   _________________________________________________________________________ */

void
TeXMFApp::ParseFirstLine (/*[in]*/ const PathName &	fileName)
{
  PathName path;

  if (! pSession->FindFile(fileName, GetInputFileType(), path))
    {
      return;
    }

  Argv argv;

  if (! ParseFirstLine(path, argv))
    {
      return;
    }

  int optidx;

  if (argv.GetArgc() > 1 && argv[1][0] != '-')
    {
      optidx = 2;
      if (memoryDumpFileName.empty())
	{
	  string memoryDumpFileName = argv[1];
	  PathName fileName (memoryDumpFileName);
	  if (fileName.GetExtension() == 0)
	    {
	      fileName.SetExtension (GetMemoryDumpFileExtension());
	    }
	  PathName path;
	  if (pSession->FindFile(fileName, GetMemoryDumpFileType(), path))
	    {
	      this->memoryDumpFileName = memoryDumpFileName;
	    }
	}
    }
  else
    {
      optidx = 1;
    }

  int opt;

  if (optidx < argv.GetArgc())
    {
      for (Cpopt popt (argv.GetArgc() - optidx,
		       const_cast<const char**>(argv.GetArgv()) + optidx,
		       &(GetOptions())[0],
		       POPT_CONTEXT_KEEP_FIRST);
	   ((opt = popt.GetNextOpt()) >= 0);
	   )
	{
	  ProcessOption (opt, popt.GetOptArg());
	}
    }
}

/* _________________________________________________________________________

   TeXMFApp::OpenMemoryDumpFile
   _________________________________________________________________________ */

bool
TeXMFApp::OpenMemoryDumpFile (/*[in]*/ const PathName &	fileName_,
			      /*[out]*/ FILE **		ppFile,
			      /*[in]*/ void *		pBuf,
			      /*[in]*/ size_t		size,
			      /*[in]*/ bool		renew)
  const
{
  MIKTEX_ASSERT (ppFile);

  if (pBuf != 0)
    {
      MIKTEX_ASSERT_BUFFER (pBuf, size);
    }

  PathName fileName (fileName_);

  if (fileName.GetExtension() == 0)
    {
      fileName.SetExtension (GetMemoryDumpFileExtension());
    }

  char szProgName[BufferSizes::MaxPath];
  GetProgName (szProgName);
#if 0
  PathName::Convert (szProgName, szProgName, ConvertPathNameFlags::MakeLower);
#endif

  PathName path;

  char szDumpName[BufferSizes::MaxPath];
  fileName.GetFileNameWithoutExtension (szDumpName);
#if 0
  PathName::Convert (szDumpName,
		     szDumpName,
		     ConvertPathNameFlags::MakeLower);
#endif

  bool haveIt = false;

  if (! renew)
    {
      haveIt = pSession->FindFile(fileName, GetMemoryDumpFileType(), path);
      if (! haveIt)
	{
	  renew = true;
	}
      else if (pSession->GetConfigValue(MIKTEX_REGKEY_TEXMF,
					MIKTEX_REGVAL_RENEW_FORMATS_ON_UPDATE,
					true))
	{
	  PathName pathPackagesIni
	    (pSession->GetSpecialPath(SpecialPath::InstallRoot),
	     MIKTEX_PATH_PACKAGES_INI,
	     0);
	  if (File::Exists(pathPackagesIni))
	    {
	      renew = (File::GetLastWriteTime(pathPackagesIni)
		       > File::GetLastWriteTime(path));
	    }
	}
    }
  
  if (renew)
    {
      PathName exe;
      if (! pSession->FindFile(MIKTEX_INITEXMF_EXE, FileType::EXE, exe))
	{
	  FATAL_MIKTEX_ERROR ("TeXMFApp::OpenMemoryDumpFile",
			      (T_("\
The MiKTeX configuration utility could not be found.")),
			      0);
	}
      CommandLineBuilder arguments;
      arguments.AppendOption ("--dump-by-name=", szDumpName);
      if (isTeXProgram)
	{
	  arguments.AppendOption ("--engine=",
				  GetProgramName());
	}
      int exitCode;
      if (! (Process::Run(exe, arguments.Get(), 0, &exitCode, 0)
	     && exitCode == 0))
	{
	  return (false);
	}
    }

  if (! haveIt)
    {
      haveIt = pSession->FindFile(fileName, GetMemoryDumpFileType(), path);
    }

  if (! haveIt)
    {
      FATAL_MIKTEX_ERROR ("TeXMFApp::OpenMemoryDumpFile",
			  T_("The memory dump file could not be found."),
			  fileName.Get());
    }

  FileStream stream (pSession->OpenFile(path.Get(),
					FileMode::Open,
					FileAccess::Read,
					false));

  if (pBuf != 0)
    {
      if (stream.Read(pBuf, size) != size)
	{
	  FATAL_MIKTEX_ERROR ("TeXMFApp::OpenMemoryDumpFile",
			      T_("Premature end of memory dump file."),
			      path.Get());
	}
    }

  pSession->PushAppName (szDumpName);

  *ppFile = stream.Detach();

  return (true);
}

/* _________________________________________________________________________

   TeXMFApp::ProcessCommandLineOptions
   _________________________________________________________________________ */

void
TeXMFApp::ProcessCommandLineOptions ()
{
  if (Utils::Contains(GetInitProgramName(), Utils::GetExeName().c_str()))
    {
      isInitProgram = true;
    }

  WebAppInputLine::ProcessCommandLineOptions ();

  if (GetQuietFlag())
    {
      showFileLineErrorMessages = true;
      interactionMode = 0;	// batch_mode
    }

  if (showFileLineErrorMessages && interactionMode < 0)
    {
      interactionMode = 2;	// scrollmode
    }

  if (parseFirstLine
      && GetArgC() > 1
      && GetArgV()[1][0] != '&'
      && GetArgV()[1][0] != '*' // <fixme/>
      && GetArgV()[1][0] != '\\'
      && (memoryDumpFileName.empty() || GetTcxFileName().Empty()))
    {
      ParseFirstLine (GetArgV()[1]);
    }
}

/* _________________________________________________________________________

   TeXMFApp::IsVirgin
   _________________________________________________________________________ */

bool
TeXMFApp::IsVirgin () const
{
  string exeName = Utils::GetExeName();
  if (Utils::Contains(GetProgramName(), exeName.c_str())
      || Utils::Contains(GetVirginProgramName(), exeName.c_str()))
  {
    return (true);
  }
#if 1
  size_t prefixLen = strlen(MIKTEX_PREFIX);
  if (exeName.compare(0, prefixLen, MIKTEX_PREFIX) == 0)
  {
    exeName = exeName.substr(prefixLen);
    if (Utils::Contains(GetProgramName(), exeName.c_str())
        || Utils::Contains(GetVirginProgramName(), exeName.c_str()))
    {
      return (true);
    }
  }
#endif
  return (false);
}

/* _________________________________________________________________________

   TeXMFApp::GetDefaultMemoryDumpFileName
   _________________________________________________________________________ */

void
TeXMFApp::GetDefaultMemoryDumpFileName (/*[out]*/ char * lpszPath)
  const
{
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  PathName name;
  if (memoryDumpFileName.length() > 0)
    {
      name = memoryDumpFileName;
    }
  else if (IsVirgin())
    {
      name = GetMemoryDumpFileName();
    }
  else
    {
      name = Utils::GetExeName();
    }
  name.SetExtension (GetMemoryDumpFileExtension());
  Utils::CopyString (lpszPath,
		     BufferSizes::MaxPath,
		     name.Get());
}

/* _________________________________________________________________________

   IsFileNameArgument
   _________________________________________________________________________ */

bool
IsFileNameArgument (/*[in]*/ const char * lpszArg)
{
  for (size_t l = 0; lpszArg[l] != 0; ++ l)
    {
      if (l >= BufferSizes::MaxPath)
	{
	  return (false);
	}
      char ch = lpszArg[l];
      if (ch == '<'
	  || ch == '>'
	  || ch == '"'
	  || ch == '|'
	  || ch == '*'
	  || ch == '?')
	{
	  return (false);
	}
    }
  return (true);
}

/* _________________________________________________________________________

   InitializeBuffer_
   _________________________________________________________________________ */

template<typename CharType>
int
InitializeBuffer_ (/*[in,out]*/ CharType *	pBuffer,
		   /*[in]*/ FileType		inputFileType,
		   /*[in]*/ bool		isTeXProgram)
{
  int fileNameArgIdx = -1;
  PathName fileName;
  bool mangleFileName = isTeXProgram;

  if (mangleFileName)
    {
      /* test command-line for one of:
	 (a) tex FILENAME
	 (b) tex &FORMAT FILENAME
	 (c) initex FILENAME \dump
	 (d) initex &FORMAT FILENAME \dump
      */
      PathName path;
      if (c4pargc == 2
	  && IsFileNameArgument(c4pargv[1])
	  && SessionWrapper(true)->FindFile(c4pargv[1], inputFileType, path))
	{
	  fileNameArgIdx = 1;
	}
      else if (c4pargc == 3
	       && c4pargv[1][0] == '&'
	       && IsFileNameArgument(c4pargv[2])
	       && SessionWrapper(true)->FindFile(c4pargv[2],
						 inputFileType,
						 path))
	{
	  fileNameArgIdx = 2;
	}
      else if (c4pargc == 3
	       && strcmp(c4pargv[2], "\\dump") == 0
	       && IsFileNameArgument(c4pargv[1])
	       && SessionWrapper(true)->FindFile(c4pargv[1],
						 inputFileType,
						 path))
	{
	  fileNameArgIdx = 1;
	}
      else if (c4pargc == 4
	       && c4pargv[1][0] == '&'
	       && strcmp(c4pargv[3], "\\dump") == 0
	       && IsFileNameArgument(c4pargv[2])
	       && SessionWrapper(true)->FindFile(c4pargv[2],
						 inputFileType,
						 path))
	{
	  fileNameArgIdx = 2;
	}
      if (fileNameArgIdx >= 0)
	{
	  fileName =
	    WebAppInputLine::MangleNameOfFile(c4pargv[fileNameArgIdx]);
	}
    }

  // first = 1;
  unsigned last = 1;
  for (int idx = 1; idx < c4pargc; ++ idx)
    {
      if (idx > 1)
	{
	  pBuffer[last++] = ' ';
	}
      const char * lpszOptArg;
      if (idx == fileNameArgIdx)
	{
	  lpszOptArg = fileName.Get();
	}
      else
	{
	  lpszOptArg = c4pargv[idx];
	}
      size_t len = StrLen(lpszOptArg);
      for (size_t j = 0; j < len; ++ j)
	{
	  pBuffer[last++] = lpszOptArg[j];
	}
    }

  // clear the command-line
  MakeCommandLine (0, 0);

  return (last);
}

/* _________________________________________________________________________

   TeXMFApp::InitializeBuffer
   _________________________________________________________________________ */

unsigned long
TeXMFApp::InitializeBuffer (/*[in,out]*/ unsigned char * pBuffer)
{
  MIKTEX_ASSERT (pBuffer != 0);
  return (InitializeBuffer_<unsigned char>(pBuffer,
					   GetInputFileType(),
					   isTeXProgram));
}

/* _________________________________________________________________________

   TeXMFApp::InitializeBuffer
   _________________________________________________________________________ */

unsigned long
TeXMFApp::InitializeBuffer (/*[in,out]*/ unsigned short * pBuffer)
{
  MIKTEX_ASSERT (pBuffer != 0);
  return (InitializeBuffer_<unsigned short>(pBuffer,
					    GetInputFileType(),
					    isTeXProgram));
}

/* _________________________________________________________________________

   TeXMFApp::InitializeBuffer
   _________________________________________________________________________ */

unsigned long
TeXMFApp::InitializeBuffer (/*[in,out]*/ C4P_signed32 * pBuffer)
{
  MIKTEX_ASSERT (pBuffer != 0);
  return (InitializeBuffer_<C4P_signed32>(pBuffer,
					  GetInputFileType(),
					  isTeXProgram));
}

/* _________________________________________________________________________

   TeXMFApp::InvokeEditor
   _________________________________________________________________________ */

void
TeXMFApp::InvokeEditor (/*[in]*/ const PathName &	editFileName,
			/*[in]*/ int			editLineNumber,
			/*[in]*/ const PathName &	transcriptFileName)
  const
{
  string commandLine;
  commandLine.reserve (256);

  // <fixme>We use a non-standard section name. Instead, we should
  // read the Registry directly.
  string defaultEditor =
    pSession->GetConfigValue(MIKTEX_REGKEY_YAP_SETTINGS,
			     MIKTEX_REGVAL_EDITOR,
			     "notepad.exe \"%f\"");
  // </fixme>

  string templ =
    pSession->GetConfigValue(0,
			     MIKTEX_REGVAL_EDITOR,
			     defaultEditor.c_str());

  const char * lpszCommandLineTemplate = templ.c_str();

  while (*lpszCommandLineTemplate != 0)
    {
      if (lpszCommandLineTemplate[0] == '%'
	  && lpszCommandLineTemplate[1] != 0)
	{
	  switch (lpszCommandLineTemplate[1])
	    {
	    default:
	      break;
	    case '%':
	      commandLine += '%';
	      break;
	    case 'f':
	      {
		PathName unmangled = UnmangleNameOfFile(editFileName.Get());
		PathName path;
		if (pSession->FindFile(unmangled,
				       GetInputFileType(),
				       path))
		  {
		    commandLine += path.Get();		    
		  }
		else
		  {
		    commandLine += unmangled.Get();
		  }
		break;
	      }
	    case 'h':
	      /* <todo/> */
	      break;
	    case 't':
	      commandLine += transcriptFileName.Get();
	      break;
	    case 'l':
	      commandLine += NUMTOSTR(editLineNumber);
	      break;
	    case 'm':
	      /* <todo/> */
	      break;
	    }
	  lpszCommandLineTemplate += 2;
	}
      else
	{
	  commandLine += *lpszCommandLineTemplate;
	  ++ lpszCommandLineTemplate;
	}
    }

  Process::StartSystemCommand (commandLine.c_str());
}

/* _________________________________________________________________________

   TeXMFApp::TouchJobOutputFile
   _________________________________________________________________________ */

void
TeXMFApp::TouchJobOutputFile (/*[in]*/ FILE * pfile) const
{
  MIKTEX_ASSERT (pfile != 0);
  if (setJobTime && pSession->IsOutputFile(pfile))
    {
      time_t time = GetStartUpTime();
      File::SetTimes (pfile, time, time, time);
    }
}

/* _________________________________________________________________________

   TeXMFApp::OpenPoolFile
   _________________________________________________________________________ */

bool
TeXMFApp::OpenPoolFile (/*[in]*/ void *			p,
			/*[in]*/ const PathName &	fileName_)
  const
{
  MIKTEX_ASSERT_BUFFER (p, sizeof(C4P_text));
  PathName fileName (fileName_);
  if (fileName.GetExtension() == 0)
    {
      fileName.SetExtension (".pool");
    }
  PathName path;
  if (! pSession->FindFile(fileName, GetPoolFileType(), path))
    {
      return (false);
    }
  FILE * pfile =
    pSession->OpenFile(path.Get(), FileMode::Open, FileAccess::Read, true);
  reinterpret_cast<C4P_text*>(p)->Attach (pfile, true);
  get (*reinterpret_cast<C4P_text*>(p));
  return (true);
}
