/* SetupService.cpp:

   Copyright (C) 2013 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include <miktex/Core/Core>
#include <miktex/Core/Paths>
#include <miktex/PackageManager/PackageManager>

#include "internal.h"

#include "setup-version.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeX::Setup;
using namespace std;

/* _________________________________________________________________________

   SetupService::~SetupService
   _________________________________________________________________________ */

SetupService::~SetupService()
{
}

/* _________________________________________________________________________

   SetupServiceImpl::SetupServiceImpl
   _________________________________________________________________________ */

SetupServiceImpl::SetupServiceImpl()
  : refCount (0)
{
  options.Task = SetupTask::None;
  options.IsDryRun = false;
  options.IsCommonSetup = false;
  options.IsPortable = false;
  traceStream = auto_ptr<TraceStream>(TraceStream::Open("setup"));
  TraceStream::SetTraceFlags("error,extractor,mpm,process,config,setup");
}

/* _________________________________________________________________________

   SetupServiceImpl::~SetupServiceImpl
   _________________________________________________________________________ */

SetupServiceImpl::~SetupServiceImpl()
{
  try
  {
  }
  catch (const exception &)
  {
  }
}

/* _________________________________________________________________________

   SetupService::Create
   _________________________________________________________________________ */

SetupService * SetupService::Create()
{
  SetupService * pService = new SetupServiceImpl();
  return (pService);
}

/* _________________________________________________________________________

   SetupServiceImpl::AddRef
   _________________________________________________________________________ */

void SetupServiceImpl::AddRef()
{
  ++ refCount;
}

/* _________________________________________________________________________

   SetupServiceImpl::Release
   _________________________________________________________________________ */

void SetupServiceImpl::Release()
{
  -- refCount;
  if (refCount == 0)
  {
    delete this;
  }
}

/* _________________________________________________________________________

   SetupServiceImpl::SetOptions
   _________________________________________________________________________ */

void SetupServiceImpl::SetOptions(const SetupOptions & options)
{
  this->options = options;
}

/* _________________________________________________________________________

   SetupServiceImpl::OpenLog
   _________________________________________________________________________ */

void SetupServiceImpl::OpenLog()
{
  if (logStream.IsOpen())
  {
    return ;
  }

  // make the intermediate log file name
  intermediateLogFile.SetToTempFile();
  
  // open the intermediate log file
  MIKTEX_LOCK(logStream)
  {
    logStream.Attach(File::Open(intermediateLogFile, FileMode::Create, FileAccess::Write));
    logStream.WriteLine();
    logStream.WriteLine();
  }
  MIKTEX_UNLOCK();
}

/* _________________________________________________________________________

   SetupServiceImpl::CloseLog
   _________________________________________________________________________ */

PathName SetupServiceImpl::CloseLog(bool cancel)
{
  // we must have an intermediate log file
  if (! logStream.IsOpen())
  {
    return "";
  }

  // close the intermediate log file
  logStream.Close();

  if (cancel)
  {
    File::Delete(intermediateLogFile);
    return "";
  }

  // determine the final log directory
  PathName pathLogDir;
  if (options.IsDryRun || options.Task == SetupTask::PrepareMiKTeXDirect)
  {
    pathLogDir.SetToTempDirectory();
  }
  else
  {
    if (options.Task == SetupTask::InstallFromCD || options.Task == SetupTask::InstallFromLocalRepository || options.Task == SetupTask::InstallFromRemoteRepository)
    {
      if (Directory::Exists(GetInstallRoot()))
      {
	pathLogDir.Set(GetInstallRoot(), MIKTEX_PATH_MIKTEX_CONFIG_DIR);
      }
      else
      {
	pathLogDir.SetToTempDirectory();
      }
    }
    else if (options.Task == SetupTask::Download)
    {
      if (Directory::Exists(options.LocalPackageRepository))
      {
	pathLogDir = options.LocalPackageRepository;
      }
      else
      {
	pathLogDir.SetToTempDirectory();
      }
    }
    else
    {
      // remove the intermediate log file
      File::Delete(intermediateLogFile);
      return "";
    }
  }

  // create the log directory
  Directory::Create(pathLogDir);

  // make the final log path name
  PathName pathLogFile(pathLogDir);
  time_t t = time(0);
  char dateTime[128];
  strftime(dateTime, 128, "%Y-%m-%d-%H-%M", localtime(&t));
  string fileName;
  if (options.Task == SetupTask::Download)
  {
    fileName = "download";
  }
  else
  {
    fileName = "setup";
  }
  fileName += "-";
  fileName += dateTime;
  pathLogFile += TU_(fileName);
  pathLogFile.SetExtension(".log");

  // install the log file
  // <todo>add the log file to the uninstall script</todo>
  File::Copy(intermediateLogFile, pathLogFile);

  // remove the intermediate log file
  File::Delete(intermediateLogFile);

  return pathLogFile;
}

/* _________________________________________________________________________

   SetupServiceImpl::Log
   _________________________________________________________________________ */

void SetupServiceImpl::Log(const char * lpszFormat, ...)
{
  va_list argList;
  va_start(argList, lpszFormat);
  LogV(lpszFormat, argList);
  va_end(argList);
}

/* _________________________________________________________________________

   SetupServiceImpl::LogV
   _________________________________________________________________________ */

void SetupServiceImpl::LogV(const char * lpszFormat, va_list argList)
{
  MIKTEX_LOCK(logStream);
  string formatted = Utils::FormatString(lpszFormat, argList);
  static string currentLine;
  for (const char * lpsz = formatted.c_str(); *lpsz != 0; ++ lpsz)
  {
    if (lpsz[0] == '\n' || (lpsz[0] == '\r' && lpsz[1] == '\n'))
    {
      traceStream->WriteFormattedLine("setup", "%s", currentLine.c_str());
      if (logStream.IsOpen())
      {
	logStream.WriteLine(currentLine);
      }
      currentLine = "";
      if (lpsz[0] == '\r')
      {
	++ lpsz;
      }
    }
    else
    {
      currentLine += *lpsz;
    }
  }
  MIKTEX_UNLOCK();
}

/* _________________________________________________________________________

   SetupServiceImpl::ULogOpen
   _________________________________________________________________________ */

void
SetupServiceImpl::ULogOpen()
{
  PathName uninstLog(GetULogFileName());
  FileMode mode = (File::Exists(uninstLog) ? FileMode::Append : FileMode::Create);
  uninstStream.Attach(File::Open(uninstLog, mode, FileAccess::Write));;
  section = None;
}

/* _________________________________________________________________________

   SetupServiceImpl::GetULogFileName
   _________________________________________________________________________ */

PathName
SetupServiceImpl::GetULogFileName()
{
  PathName ret;
  if (options.IsDryRun || options.Task == SetupTask::PrepareMiKTeXDirect)
  {
    ret.SetToTempDirectory();
  }
  else
  {
    ret = GetInstallRoot();
    ret += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
  }
  Directory::Create(ret);
  ret += MIKTEX_UNINSTALL_LOG;
  return (ret);
}

/* _________________________________________________________________________

   SetupServiceImpl::ULogClose
   _________________________________________________________________________ */

void
SetupServiceImpl::ULogClose(bool finalize)
{
  if (! uninstStream.IsOpen())
  {
    return;
  }

  try
  {
    if (finalize)
    {
      ULogAddFile(GetULogFileName());
#if defined(MIKTEX_WINDOWS)
      if (! options.IsPortable)
      {
	RegisterUninstaller();
      }
#endif
    }
  }
  catch (const exception &)
  {
    uninstStream.Close();
    throw;
  }

  uninstStream.Close();
}

/* _________________________________________________________________________

   SetupServiceImpl::ULogAddFile
   _________________________________________________________________________ */

void SetupServiceImpl::ULogAddFile (const PathName & path)
{
  if (! uninstStream.IsOpen())
  {
    return;
  }
  if (section != Files)
  {
    uninstStream.WriteLine("[files]");
    section = Files;
  }
  PathName absolutePath(path);
  absolutePath.MakeAbsolute();
#if defined(MIKTEX_WINDOWS)
  absolutePath.ToDos();
#endif
  uninstStream.WriteLine(absolutePath.Get());
}

/* _________________________________________________________________________

   SetupServiceImpl::CreateShellLink
   _________________________________________________________________________ */

wstring & SetupServiceImpl::Expand(const char * lpszSource, wstring & dest)
{
  dest = Utils::UTF8ToWideChar(lpszSource);
  wstring::size_type pos;
  while ((pos = dest.find(L"%MIKTEX_INSTALL%")) != wstring::npos)
  {
    dest = dest.replace(pos, 16, GetInstallRoot().ToWideCharString());
  }
  return dest;
}
