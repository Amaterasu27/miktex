/* miktex/Setup/SetupService.h:				-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(FAA6EA3649584988994D2B7032C040AB)
#define FAA6EA3649584988994D2B7032C040AB

#include <miktex/Core/Core>

#include <string>
#include <vector>

// DLL import/export switch
#if ! defined(C09CDC45E4B649EDA745DAF436D18309)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define MIKTEXSETUPEXPORT __declspec(dllimport)
#  else
#    define MIKTEXSETUPEXPORT
#  endif
#endif

// API decoration for exported member functions
#define MIKTEXSETUPCEEAPI(type) MIKTEXSETUPEXPORT type MIKTEXCEECALL

#define SETUP_BEGIN_NAMESPACE			\
  namespace MiKTeX {				\
    namespace Setup {

#define SETUP_END_NAMESPACE			\
    }						\
  }

SETUP_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   SetupTask
   _________________________________________________________________________ */

class SetupTaskEnum
{
public:
  enum EnumType {
    None,
    Download,
    InstallFromCD,
    InstallFromLocalRepository,
    InstallFromRemoteRepository, // <todo/>
    PrepareMiKTeXDirect,
  };
};

typedef MiKTeX::Core::EnumWrapper<SetupTaskEnum> SetupTask;

/* _________________________________________________________________________

   SetupOptions
   _________________________________________________________________________ */

struct SetupOptions
{
public:
  SetupOptions() :
    Task(SetupTask::None), IsDryRun(false), IsCommonSetup(false),
    IsPortable(false)
  {
  }

public:
  SetupTask Task;

public:
  std::string Banner;

public:
  std::string Version;

public:
  bool IsDryRun;

public:
  bool IsCommonSetup;

public:
  bool IsPortable;

public:
  MiKTeX::Core::StartupConfig Config;

public:
  MiKTeX::Core::PathName LocalPackageRepository;

#if defined(MIKTEX_WINDOWS)
public:
  MiKTeX::Core::PathName FolderName;
#endif
};

/* _________________________________________________________________________

   SetupService
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
SetupService
{
protected:
  virtual MIKTEXTHISCALL ~SetupService() = 0;

public:
  virtual void MIKTEXTHISCALL AddRef() = 0;

public:
  virtual void MIKTEXTHISCALL Release() = 0;

public:
  virtual SetupOptions MIKTEXTHISCALL GetOptions() = 0;

public:
  virtual SetupOptions MIKTEXTHISCALL SetOptions(const SetupOptions & options) = 0;

public:
  virtual void MIKTEXTHISCALL OpenLog() = 0;

public:
  virtual MiKTeX::Core::PathName MIKTEXTHISCALL CloseLog(bool cancel) = 0;

public:
  virtual void MIKTEXCEECALL Log(const char * lpszFormat, ...) = 0;

public:
  virtual void MIKTEXTHISCALL LogV(const char * lpszFormat, va_list argList) = 0;

public:
  virtual void MIKTEXTHISCALL ULogOpen() = 0;

public:
  virtual void MIKTEXTHISCALL ULogClose(bool finalize) = 0;

public:
  virtual MiKTeX::Core::PathName MIKTEXTHISCALL GetULogFileName() = 0;

public:
  virtual void ULogAddFile(const MiKTeX::Core::PathName & path) = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual void ULogAddRegValue (HKEY hkey, const char * lpszSubKey, const char * lpszValueName) = 0;
#endif

#if defined(MIKTEX_WINDOWS)
public:
  virtual void MIKTEXTHISCALL CreateProgramIcons() = 0;
#endif

public:
  static MIKTEXSETUPCEEAPI(SetupService*) Create();
};

typedef MiKTeX::Core::SmartPointer<SetupService> SetupServicePtr;

SETUP_END_NAMESPACE;

#undef SETUP_BEGIN_NAMESPACE
#undef SETUP_END_NAMESPACE

#endif
