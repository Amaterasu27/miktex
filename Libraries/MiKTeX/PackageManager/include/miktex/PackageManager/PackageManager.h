/* miktex/PackageManager/PackageManager.h: package manager API	-*- C++ -*-

   Copyright (C) 2001-2011 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

/// @file mpm.h
/// @brief MiKTeX package manager API.

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(D5760AC70396FF41BBDD7DD560BDBBBC_)
#define D5760AC70396FF41BBDD7DD560BDBBBC_

#if 0 || defined(MIKTEX_STATIC) || defined(_DEBUG)
#  define MIKTEX_EXTENDED_PACKAGEINFO 1
#endif

#include <miktex/Core/Core>

#include <string>
#include <vector>

// DLL import/export switch
#if ! defined(F927BA187CB94546AB9CA9099D989E81)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define MIKTEXMPMEXPORT __declspec(dllimport)
#  else
#    define MIKTEXMPMEXPORT
#  endif
#endif

// API decoration for exported member functions
#define MIKTEXMPMCEEAPI(type) MIKTEXMPMEXPORT type MIKTEXCEECALL

#define MPM_BEGIN_NAMESPACE			\
  namespace MiKTeX {				\
    namespace Packages {

#define MPM_END_NAMESPACE			\
    }						\
  }

/// @namespace MiKTeX::Packages
/// @brief The package manager namespace.
MPM_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   PackageInfo
   _________________________________________________________________________ */

/// Package info struct.
struct PackageInfo
{
  /// Deployment name.
  std::string deploymentName;

  /// Display name.
  std::string displayName;

  /// One-line package description.
  std::string title;

  /// Informal version information.
  std::string version;

  /// Target system.
  std::string targetSystem;

  /// Multi-line package description.
  std::string description;

  /// Name of creator (packer).
  std::string creator;

  /// Accumulated file sizes of the run file list.
  size_t sizeRunFiles;

  /// Accumulated file sizes of the doc file list.
  size_t sizeDocFiles;

  /// Accumulated file sizes of the source file list.
  size_t sizeSourceFiles;

  /// The run file list.
  std::vector<std::string> runFiles;

  /// The doc file list.
  std::vector<std::string> docFiles;

  /// The source file list.
  std::vector<std::string> sourceFiles;

  /// List of required packages.
  // (these should be <set>s; but <set>s cannot be
  // exported from a DLL (see Q172396))
  std::vector<std::string> requiredPackages;

  /// List of dependants.
  std::vector<std::string> requiredBy;

  /// Date/time when the package was created.
  time_t timePackaged;

  /// Date/time when the package was installed.
  time_t timeInstalled;

  /// Size of the archive file.
  size_t archiveFileSize;

  /// MD5 of the package.
  MiKTeX::Core::MD5 digest;

  /// 
  bool isRemovable;

  /// 
  bool isObsolete;

#if MIKTEX_EXTENDED_PACKAGEINFO
  std::string ctanPath;
  std::string licenseType;
  std::string copyrightOwner;
  std::string copyrightYear;
#endif

  /// Initializes a new package info struct.
  PackageInfo ()
    : archiveFileSize (0),
      sizeDocFiles (0),
      sizeRunFiles (0),
      sizeSourceFiles (0),
      timeInstalled (static_cast<time_t>(0)),
      timePackaged (static_cast<time_t>(0)),
      isRemovable (false),
      isObsolete (false)
  {
  }

  /// Copies another package info struct into this package info struct.
  PackageInfo &
  operator= (/*[in]*/ const PackageInfo & rhs)
  {
    archiveFileSize = rhs.archiveFileSize;
    sizeDocFiles = rhs.sizeDocFiles;
    sizeRunFiles = rhs.sizeRunFiles;
    sizeSourceFiles = rhs.sizeSourceFiles;
    creator = rhs.creator;
    description = rhs.description;
    deploymentName = rhs.deploymentName;
    displayName = rhs.displayName;
    title = rhs.title;
    version = rhs.version;
    targetSystem = rhs.targetSystem;
    timeInstalled = rhs.timeInstalled;
    timePackaged = rhs.timePackaged;
    docFiles = rhs.docFiles;
    requiredBy = rhs.requiredBy;
    requiredPackages = rhs.requiredPackages;
    runFiles = rhs.runFiles;
    sourceFiles = rhs.sourceFiles;
    digest = rhs.digest;
    isRemovable = rhs.isRemovable;
    isObsolete = rhs.isObsolete;
#if MIKTEX_EXTENDED_PACKAGEINFO
    ctanPath = rhs.ctanPath;
    licenseType = rhs.licenseType;
    copyrightOwner = rhs.copyrightOwner;
    copyrightYear = rhs.copyrightYear;
#endif
    return (*this);
  }

  /// Copies another package info struct into a new package info struct.
  PackageInfo (/*[in]*/ const PackageInfo & other)
    : archiveFileSize (other.archiveFileSize),
      sizeDocFiles (other.sizeDocFiles),
      sizeRunFiles (other.sizeRunFiles),
      sizeSourceFiles (other.sizeSourceFiles),
      creator (other.creator),
      description (other.description),
      deploymentName (other.deploymentName),
      displayName (other.displayName),
      title (other.title),
      version (other.version),
      targetSystem (other.targetSystem),
      timeInstalled (other.timeInstalled),
      timePackaged (other.timePackaged),
      docFiles (other.docFiles),
      requiredBy (other.requiredBy),
      requiredPackages (other.requiredPackages),
      runFiles (other.runFiles),
      sourceFiles (other.sourceFiles),
      digest (other.digest),
      isRemovable (other.isRemovable),
      isObsolete (other.isObsolete)
#if MIKTEX_EXTENDED_PACKAGEINFO
      ,ctanPath (other.ctanPath),
      licenseType (other.licenseType),
      copyrightOwner (other.copyrightOwner),
      copyrightYear (other.copyrightYear)
#endif
  {
  }

  /// Gets the total number of files in the package.
  /// @return Returns the number of files.
  unsigned long
  GetNumFiles ()
    const
  {
    return (static_cast<unsigned long>(runFiles.size()
				       + docFiles.size()
				       + sourceFiles.size()));
  }

  /// Gets the total size (in bytes) of all files in the package.
  /// @return Returns the package size.
  size_t
  GetSize ()
    const
  {
    return (sizeRunFiles + sizeDocFiles + sizeSourceFiles);
  }

  /// Tests for a pure container package.
  /// @return Returns true, if this is a pure container package.
  bool
  IsPureContainer ()
    const
  {
    return (IsContainer() && GetNumFiles() <= 1);
  }

  /// Tests for a container package.
  bool
  IsContainer ()
    const
  {
    return (requiredPackages.size() > 0);
  }

  /// Checks to see if this package has dependants.
  bool
  IsContained ()
    const
  {
    return (GetRefCount() > 0);
  }

  /// Checks to see whether the package is installed.
  bool
  IsInstalled ()
    const
  {
    return (timeInstalled != static_cast<time_t>(0));
  }

  /// Gets the number of dependants.
  unsigned long
  GetRefCount ()
    const
  {
    return (static_cast<unsigned long>(requiredBy.size()));
  }
};

/* _________________________________________________________________________

   PackageFilter
   _________________________________________________________________________ */

/// Package filter enum class.
class PackageFilterEnum
{
public:
  enum EnumType {
    /// No filtering.
    None = 0,
    /// Top-level package filter.
    Top = 1,
    /// Dependency package filter.
    RequiredBy = 2,
    /// Obsolete packages.
    Obsolete = 4,
  };
};

/// Package filter enum.
typedef MiKTeX::Core::EnumWrapper<PackageFilterEnum> PackageFilter;

/* _________________________________________________________________________

   PackageLevel
   _________________________________________________________________________ */

/// Package level enum class.
class PackageLevelEnum
{
public:
  enum EnumType {
    None      =          0,
    /// Essential package set.
    Essential =         10,
    /// Basic package set.
    Basic     =       1000,
    /// Advanced package set.
    Advanced  =     100000,
    /// Complete package set.
    Complete  = 1000000000,
  };
};

/// Package level enum.
typedef MiKTeX::Core::EnumWrapper<PackageLevelEnum> PackageLevel;

/* _________________________________________________________________________

   RepositoryType
   _________________________________________________________________________ */

class RepositoryTypeEnum
{
public:
  enum EnumType {
    Unknown,
    MiKTeXDirect,
    Local,
    Remote,
    MiKTeXInstallation
  };
};

typedef MiKTeX::Core::EnumWrapper<RepositoryTypeEnum> RepositoryType;
  
/* _________________________________________________________________________

   RepositoryStatus
   _________________________________________________________________________ */

class RepositoryStatusEnum
{
public:
  enum EnumType {
    Unknown,
    Online,
    Offline,
  };
};

typedef MiKTeX::Core::EnumWrapper<RepositoryStatusEnum> RepositoryStatus;
  
/* _________________________________________________________________________

   RepositoryIntegrity
   _________________________________________________________________________ */

class RepositoryIntegrityEnum
{
public:
  enum EnumType {
    Unknown,
    Intact,
    Corrupted,
  };
};

typedef MiKTeX::Core::EnumWrapper<RepositoryIntegrityEnum> RepositoryIntegrity;
  
/* _________________________________________________________________________

   RepositoryInfo
   _________________________________________________________________________ */

/// Package repository info struct.
struct RepositoryInfo
{
  /// Initializes a new RepositoryInfo struct.
  RepositoryInfo ()
    : packageLevel (PackageLevel::None),
      version (0),
      timeDate (static_cast<time_t>(0)),
      status (RepositoryStatus::Unknown),
      integrity (RepositoryIntegrity::Unknown),
      delay (0)
  {
  }
  /// URL of the repository.
  std::string url;
  /// One-line description.
  std::string description;
  /// Country where the repository is resident.
  std::string country;
  /// Town where the repository is resident.
  std::string town;
  /// Package level.
  PackageLevel packageLevel;
  /// Version number.
  unsigned int version;
  /// Time/Date of last update.
  time_t timeDate;
  /// Status of the repository.
  RepositoryStatus status;
  /// Integritz of the repository.
  RepositoryIntegrity integrity;
  /// Delay (in days).
  unsigned delay;
};

/* _________________________________________________________________________

   ProxySettings
   _________________________________________________________________________ */

struct ProxySettings
{
  bool useProxy;
  std::string proxy;
  int port;
  bool authenticationRequired;
  std::string user;
  std::string password;
  ProxySettings ()
    : useProxy (false),
      authenticationRequired (false),
      port (0)
  {
  }
};

/* _________________________________________________________________________

   PackageManager
   _________________________________________________________________________ */

/// The package manager interface.
class
MIKTEXNOVTABLE
PackageManager
{
protected:
  virtual
  MIKTEXTHISCALL
  ~PackageManager ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  AddRef ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Release ()
    = 0;

  /// Creates a new package installer object. The caller is responsible
  /// for deleting the object.
public:
  virtual
  class PackageInstaller *
  MIKTEXTHISCALL
  CreateInstaller ()
    = 0;

  /// Creates a new package iterator object. The caller is responsible
  /// for deleting the object.
public:
  virtual
  class PackageIterator *
  MIKTEXTHISCALL
  CreateIterator ()
    = 0;

  /// Recreates the file name database of the package manager.
public:
  virtual
  void
  MIKTEXTHISCALL
  CreateMpmFndb ()
    = 0;

  /// Gets the reference count of a file.
  /// @param path Path name of the file.
  /// @returns Returns the number of packages this file is a member of.
public:
  virtual
  unsigned long
  MIKTEXTHISCALL
  GetFileRefCount (/*[in]*/ const MiKTeX::Core::PathName &	path)
    = 0;

  /// Gets information about a specified package.
  /// @param lpszDeploymentName Package key.
  /// @param packageInfo Package info struct to be filled..
public:
  virtual
  PackageInfo
  MIKTEXTHISCALL
  GetPackageInfo (/*[in]*/ const std::string & deploymentName)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  LoadDatabase (/*[in]*/ const MiKTeX::Core::PathName & path)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  UnloadDatabase ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  DownloadRepositoryList ()
    = 0;

public:
  virtual
  std::vector<RepositoryInfo>
  MIKTEXTHISCALL
  GetRepositories ()
    = 0;
  
  /// Picks a random package repository.
  /// @return Returns a string object.
public:
  virtual
  std::string
  MIKTEXTHISCALL
  PickRepositoryUrl ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetPackageInfo (/*[in]*/ const std::string &	deploymentName,
		     /*[out]*/ PackageInfo &		packageInfo)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetRepositoryInfo (/*[in]*/ const std::string &	url,
			/*[out]*/ RepositoryInfo &	repositoryInfo)
    = 0;

public:
  virtual
  RepositoryInfo
  MIKTEXTHISCALL
  VerifyPackageRepository (/*[in]*/ const std::string & url)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryVerifyInstalledPackage
  (/*[in]*/ const std::string & deploymentName)
    = 0;

public:
  virtual
  std::string
  MIKTEXTHISCALL
  GetContainerPath (/*[in]*/ const std::string &	deploymentName,
		    /*[in]*/ bool			useDisplayNames)
    = 0;

public:
  static
  MIKTEXMPMCEEAPI(PackageManager*)
  Create ();

public:
  static
  MIKTEXMPMCEEAPI(std::string)
  GetRemotePackageRepository ();

public:
  static
  MIKTEXMPMCEEAPI(bool)
  TryGetRemotePackageRepository (/*[out]*/ std::string & url);

public:
  static
  MIKTEXMPMCEEAPI(MiKTeX::Core::PathName)
  GetLocalPackageRepository ();

public:
  static
  MIKTEXMPMCEEAPI(bool)
  TryGetLocalPackageRepository (/*[out]*/ MiKTeX::Core::PathName & path);

public:
  static
  MIKTEXMPMCEEAPI(MiKTeX::Core::PathName)
  GetMiKTeXDirectRoot ();

public:
  static
  MIKTEXMPMCEEAPI(bool)
  TryGetMiKTeXDirectRoot (/*[out]*/ MiKTeX::Core::PathName & path);

public:
  static
  MIKTEXMPMCEEAPI(RepositoryType)
  GetDefaultPackageRepository (/*[out]*/ std::string & urlOrPath);

public:
  static
  MIKTEXMPMCEEAPI(bool)
  TryGetDefaultPackageRepository (/*[out]*/ RepositoryType & repositoryType,
				  /*[out]*/ std::string & urlOrPath);

public:
  static
  MIKTEXMPMCEEAPI(void)
  SetDefaultPackageRepository
  (/*[in]*/ RepositoryType			repositoryType,
   /*[in]*/ const std::string &	urlOrPath);

public:
  static
  MIKTEXMPMCEEAPI(bool)
  IsLocalPackageRepository (/*[in]*/ const MiKTeX::Core::PathName & path);

public:
  static
  MIKTEXMPMCEEAPI(void)
  SetRemotePackageRepository (/*[in]*/ const std::string & url);

public:
  static
  MIKTEXMPMCEEAPI(void)
  SetLocalPackageRepository (/*[in]*/ const MiKTeX::Core::PathName & path);

public:
  static
  MIKTEXMPMCEEAPI(void)
  SetMiKTeXDirectRoot (/*[in]*/ const MiKTeX::Core::PathName & path);

public:
  static
  MIKTEXMPMCEEAPI(PackageInfo)
  ReadPackageDefinitionFile (/*[in]*/ const MiKTeX::Core::PathName &	path,
			     /*[in]*/ const char * lpszTeXMFPrefix);

public:
  static
  MIKTEXMPMCEEAPI(void)
  WritePackageDefinitionFile (/*[in]*/ const MiKTeX::Core::PathName &	path,
			      /*[in]*/ const PackageInfo &	packageInfo,
			      /*[in]*/ time_t			timePackaged);

  /// Strips the TEXMF prefix from a string.
  /// @param str The string to strip.
  /// @param[out] result The result to be filled with the stripped string.
  /// @return Returns true, if the prefix was stripped.
public:
  static
  MIKTEXMPMCEEAPI(bool)
  StripTeXMFPrefix (/*[in]*/ const std::string &	str,
		    /*[out]*/ std::string &		result);

public:
  static
  MIKTEXMPMCEEAPI(bool)
  TryGetProxy (/*[in]*/ const char *	lpszDestUrl,
	       /*[out]*/ ProxySettings & proxySettings);

public:
  static
  MIKTEXMPMCEEAPI(bool)
  TryGetProxy (/*[out]*/ ProxySettings & proxySettings);

public:
  static
  MIKTEXMPMCEEAPI(ProxySettings)
  GetProxy (/*[in]*/ const char *	lpszDestUrl);

public:
  static
  MIKTEXMPMCEEAPI(ProxySettings)
  GetProxy ();

public:
  static
  MIKTEXMPMCEEAPI(void)
  SetProxy (/*[in]*/ const ProxySettings & proxySettings);
};

/// Smart pointer to a package manager interface.
typedef MiKTeX::Core::SmartPointer<PackageManager> PackageManagerPtr;

/* _________________________________________________________________________

   PackageIterator
   _________________________________________________________________________ */

/// Instances of this class can be used to browse the package database.
class
MIKTEXNOVTABLE
PackageIterator
{
  /// Destructor.
public:
  virtual
  MIKTEXTHISCALL
  ~PackageIterator ()
    = 0;

  /// Releases all resources used by this object.
public:
  virtual
  void
  MIKTEXTHISCALL
  Dispose ()
    = 0;

  /// Adds a browse filter.
  /// @param filter Filter flags to be added.
  /// @param lpsz Optional filter argument.
public:
  virtual
  void
  MIKTEXTHISCALL
  AddFilter (/*[in]*/ unsigned int		filter,
	     /*[in]*/ const char *	lpsz = 0)
    = 0;

  /// Gets the next entry in the package database.
  /// @param[out] The package info struct to be filled.
  /// @return Returns true, if the information could be retrieved.
  /// Returns false, if there are no more entries in the database.
public:
  virtual
  bool
  MIKTEXTHISCALL
  GetNext (/*[out]*/ PackageInfo & packageInfo)
    = 0;
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
  };
};

/// Notification enum.
typedef MiKTeX::Core::EnumWrapper<NotificationEnum> Notification;

/* _________________________________________________________________________

   PackageInstallerCallback
   _________________________________________________________________________ */

/// Callback interface.
class
MIKTEXNOVTABLE
PackageInstallerCallback
{
  /// Reporting. This method is called by the installer if a new message
  /// is available.
  /// @param lpszLine One-line message.
public:
  virtual
  void
  MIKTEXTHISCALL
  ReportLine (/*[in]*/ const char * lpszLine)
    = 0;

  /// Error handling. This method is called by the installer if a
  /// problem was detected which can be remedied by the user.
  /// @param lpszMessage Message to be presented to the user.
  /// @returns Returns true, if the user has fixed the problem.
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnRetryableError (/*[in]*/ const char * lpszMessage)
    = 0;

  /// Progress. This method is called by the installer if new
  /// progress information is available. Also gives the opportunity
  /// to abort all activities.
  /// @param nf Notification code.
  /// @returns Returns true, if the installer shall continue.
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProgress (/*[in]*/ Notification		nf)
    = 0;
};

/* _________________________________________________________________________

   PackageInstaller
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
PackageInstaller
{
  /// Destructor.
public:
  virtual
  MIKTEXTHISCALL
  ~PackageInstaller ()
    = 0;

  /// Releases all resources used by this package installer object.
public:
  virtual
  void
  MIKTEXTHISCALL
  Dispose ()
    = 0;

  /// Sets the package repository for this package installer object.
  /// @param The location of the package repository. Either an URL or
  /// an absolute path name.
public:
  virtual
  void
  MIKTEXTHISCALL
  SetRepository (/*[in]*/ const std::string & repository)
    = 0;

  /// Sets the download directory for this package installer.
public:
  virtual
  void
  MIKTEXTHISCALL
  SetDownloadDirectory (/*[in]*/ const MiKTeX::Core::PathName & directory)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  UpdateDb ()
    = 0;

  /// Starts the package database synchronization thread.
public:
  virtual
  void
  MIKTEXTHISCALL
  UpdateDbAsync ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  FindUpdates ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  FindUpdatesAsync ()
    = 0;

  /// Update info struct.
public:
  struct UpdateInfo
  {
    /// Initializes a new instance of the class.
    UpdateInfo ()
      : timePackaged (static_cast<time_t>(0)),
	action (None)
    {
    }

    /// Package deployment name.
    std::string deploymentName;

    /// Date/Time the package was created.
    time_t timePackaged;

    enum Action {
      None,
      Keep,
      KeepAdmin,
      KeepObsolete,
      Update,
      ForceUpdate, 
      ForceRemove,
      Repair
    };

    Action action;
  };

public:
  virtual
  std::vector<UpdateInfo>
  MIKTEXTHISCALL
  GetUpdates ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  InstallRemove ()
    = 0;

  /// Starts the installer background thread.
public:
  virtual
  void
  MIKTEXTHISCALL
  InstallRemoveAsync ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  WaitForCompletion ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Download ()
    = 0;

  /// Starts the downloader.
public:
  virtual
  void
  MIKTEXTHISCALL
  DownloadAsync ()
    = 0;

  /// Progress info struct.
public:
  struct ProgressInfo
  {
    /// Initializes a new progress info struct.
    ProgressInfo ()
      : cFilesRemoveCompleted (0),
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
  virtual
  ProgressInfo
  MIKTEXTHISCALL
  GetProgressInfo ()
    = 0;

  /// Sets the callback interface.
  /// @param pCallback Pointer to an interface.
public:
  virtual
  void
  MIKTEXTHISCALL
  SetCallback (/*[in]*/ PackageInstallerCallback *	pCallback)
    = 0;

  /// Sets the package lists.
  /// @param toBeInstalled Packages to be installed.
  /// @param toBeRemoved Packages to be removed.
public:
  virtual
  void
  MIKTEXTHISCALL
  SetFileLists
  (/*[in]*/ const std::vector<std::string> & toBeInstalled,
   /*[in]*/ const std::vector<std::string> & toBeRemoved)
    = 0;

  /// Sets the package level.
public:
  virtual
  void
  MIKTEXTHISCALL
  SetPackageLevel (/*[in]*/ PackageLevel packageLevel)
    = 0;

  /// Sets the file lists.
  /// @param toBeInstalled Packages to be installed.
public:
  virtual
  void
  MIKTEXTHISCALL
  SetFileList
  (/*[in]*/ const std::vector<std::string> & toBeInstalled)
    = 0;

  /// Tests if the installer is running.
public:
  virtual
  bool
  MIKTEXTHISCALL
  IsRunning ()
    const
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  RegisterComponents (/*[in]*/ bool doRegister)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetNoPostProcessing (/*[in]*/ bool noPostProcessing)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetNoLocalServer (/*[in]*/ bool noLocalServer)
    = 0;
};

MPM_END_NAMESPACE;

#undef MPM_BEGIN_NAMESPACE
#undef MPM_END_NAMESPACE

#endif
