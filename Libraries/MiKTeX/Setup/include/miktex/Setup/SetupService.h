/* miktex/Setup/SetupService.h:				-*- C++ -*-

   Copyright (C) 2013-2014 Christian Schenk

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
#include <miktex/PackageManager/PackageManager>

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
    IsPortable(false), PackageLevel(MiKTeX::Packages::PackageLevel::None),
    PaperSize("A4"),
#if defined(MIKTEX_WINDOWS)
    IsRegistryEnabled(false),
#endif
    IsRegisterPathEnabled(false),
    IsInstallOnTheFlyEnabled(MiKTeX::Core::TriState::Undetermined),
    IsPrefabricated(false)
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
  bool IsRegisterPathEnabled;

public:
  bool IsPrefabricated;

#if defined(MIKTEX_WINDOWS)
public:
  bool IsRegistryEnabled;
#endif

public:
  MiKTeX::Core::TriState IsInstallOnTheFlyEnabled;

public:
  MiKTeX::Core::StartupConfig Config;

public:
  MiKTeX::Core::PathName MiKTeXDirectRoot;

public:
  std::string RemotePackageRepository;

public:
  MiKTeX::Packages::PackageLevel PackageLevel;

public:
  MiKTeX::Core::PathName LocalPackageRepository;

#if defined(MIKTEX_WINDOWS)
public:
  MiKTeX::Core::PathName FolderName;
#endif

public:
  std::string PaperSize;
};

/* _________________________________________________________________________

   Notification
   _________________________________________________________________________ */

/// Notification enum class.
class NotificationEnum
{
public:
  enum EnumType {
    None = 0,
    /// Package download is about to start.
    DownloadPackageStart,
    /// Package download has finished.
    DownloadPackageEnd,
    /// File installation is about to start.
    InstallFileStart,
    /// File installation has finished.
    InstallFileEnd,
    /// Package installation is about to start.
    InstallPackageStart,
    /// Package installation has finished.
    InstallPackageEnd,
    /// File removal is about to start.
    RemoveFileStart,
    /// File removal has finished.
    RemoveFileEnd,
    /// Package removal is about to start.
    RemovePackageStart,
    /// Package removal has finished.
    RemovePackageEnd,

    ConfigureBegin,
    ConfigureEnd,
  };
};

/// Notification enum.
typedef MiKTeX::Core::EnumWrapper<NotificationEnum> Notification;

/* _________________________________________________________________________

   SetupServiceCallback
   _________________________________________________________________________ */

/// Callback interface.
class MIKTEXNOVTABLE SetupServiceCallback
{
  /// Reporting. This method is called by the installer if a new message
  /// is available.
  /// @param lpszLine One-line message.
public:
  virtual void MIKTEXTHISCALL ReportLine(/*[in]*/ const char * lpszLine) = 0;

  /// Error handling. This method is called by the installer if a
  /// problem was detected which can be remedied by the user.
  /// @param lpszMessage Message to be presented to the user.
  /// @returns Returns true, if the user has fixed the problem.
public:
  virtual bool MIKTEXTHISCALL OnRetryableError(/*[in]*/ const char * lpszMessage) = 0;

  /// Progress. This method is called by the installer if new
  /// progress information is available. Also gives the opportunity
  /// to abort all activities.
  /// @param nf Notification code.
  /// @returns Returns true, if the installer shall continue.
public:
  virtual bool MIKTEXTHISCALL OnProgress(/*[in]*/ Notification nf) = 0;

  /// Output function. Called by the Process object if new output text
  /// is available.
  /// @param pOutput Output text bytes.
  /// @param n Number of output text bytes.
  /// @return Returns true, of the Process object shall continue.
public:
  virtual bool MIKTEXTHISCALL OnProcessOutput(const void * pOutput, size_t n) = 0;
};

/* _________________________________________________________________________

   SetupService
   _________________________________________________________________________ */

class MIKTEXNOVTABLE SetupService
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

  /// Progress info struct.
public:
  struct ProgressInfo
  {
    /// Initializes a new progress info struct.
    ProgressInfo () :
      cFilesRemoveCompleted (0),
      cFilesRemoveTotal (0),
      cPackagesRemoveCompleted (0),
      cPackagesRemoveTotal (0),
      cbPackageDownloadCompleted (0),
      cbPackageDownloadTotal (0),
      cbDownloadCompleted (0),
      cbDownloadTotal (0),
      cFilesPackageInstallCompleted (0),
      cFilesPackageInstallTotal (0),
      cFilesInstallCompleted (0),
      cFilesInstallTotal (0),
      cPackagesInstallCompleted (0),
      cPackagesInstallTotal (0),
      cbPackageInstallCompleted (0),
      cbPackageInstallTotal (0),
      cbInstallCompleted (0),
      cbInstallTotal (0),
      bytesPerSecond (0),
      timeRemaining (0),
      ready (false),
      numErrors (0),
      cancelled (false)
    {
    }

    /// Deployment name of package.
    std::string deploymentName;

    /// Display name of package.
    std::string displayName;

    /// Path name of current file.
    MiKTeX::Core::PathName fileName;

    /// Number of removed files.
    unsigned long cFilesRemoveCompleted;

    /// Number of files to be removed.
    unsigned long cFilesRemoveTotal;

    /// Number of removed packages.
    unsigned long cPackagesRemoveCompleted;

    /// Number of packages to be removed.
    unsigned long cPackagesRemoveTotal;

    /// Number of received bytes (current package).
    size_t cbPackageDownloadCompleted;

    /// Number of bytes to be received (current package).
    size_t cbPackageDownloadTotal;

    /// Number of bytes received.
    size_t cbDownloadCompleted;
    
    /// Number of bytes to be received.
    size_t cbDownloadTotal;

    /// Number of installed files (current package).
    size_t cFilesPackageInstallCompleted;

    /// Number of files to be installed (current package).
    unsigned long cFilesPackageInstallTotal;

    /// Number of installed files.
    unsigned long cFilesInstallCompleted;

    /// Number of files to be installed.
    unsigned long cFilesInstallTotal;

    /// Number of installed packages
    unsigned long cPackagesInstallCompleted;

    /// Number of packages to be installed.
    unsigned long cPackagesInstallTotal;

    /// Number of written bytes (current package).
    size_t cbPackageInstallCompleted;

    /// Number of bytes to be written (current package).
    size_t cbPackageInstallTotal;

    /// Number of bytes written.
    size_t cbInstallCompleted;

    /// Number of bytes to be written.
    size_t cbInstallTotal;

    /// Current transfer speed.
    unsigned long bytesPerSecond;

    /// estimated time of arrival (millisecs remaining).
    unsigned long timeRemaining;
    
    /// Ready flag.
    bool ready;

    /// Number of errors.
    unsigned numErrors;

    bool cancelled;
  };

  /// Gets progress information.
public:
  virtual ProgressInfo MIKTEXTHISCALL GetProgressInfo() = 0;

  /// Sets the callback interface.
  /// @param pCallback Pointer to an interface.
public:
  virtual void MIKTEXTHISCALL SetCallback(/*[in]*/ SetupServiceCallback * pCallback) = 0;

public:
  virtual void MIKTEXTHISCALL Run() = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual void MIKTEXTHISCALL CreateProgramIcons() = 0;
#endif

public:
  static MIKTEXSETUPCEEAPI(SetupService*) Create();

public:
  static MIKTEXSETUPCEEAPI(MiKTeX::Packages::PackageLevel) TestLocalRepository(const MiKTeX::Core::PathName & pathRepository, MiKTeX::Packages::PackageLevel requestedPackageLevel);

public:
  static MIKTEXSETUPCEEAPI(MiKTeX::Core::PathName) GetDefaultLocalRepository();

public:
  static MIKTEXSETUPCEEAPI(MiKTeX::Packages::PackageLevel) SearchLocalRepository(/*[out]*/ MiKTeX::Core::PathName & localRepository, MiKTeX::Packages::PackageLevel requestedPackageLevel, /*[out]*/ bool & prefabricated);

public:

};

typedef MiKTeX::Core::SmartPointer<SetupService> SetupServicePtr;

SETUP_END_NAMESPACE;

#undef SETUP_BEGIN_NAMESPACE
#undef SETUP_END_NAMESPACE

#endif
