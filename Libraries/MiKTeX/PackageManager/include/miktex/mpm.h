/* mpm.h: MiKTeX package manager API				-*- C++ -*-

   Copyright (C) 2001-2006 Christian Schenk

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

#if ! defined(F927BA187CB94546AB9CA9099D989E81)
#  if defined(MPM_DLL) && defined(_MSC_VER)
#    define MPMEXPORT __declspec(dllimport)
#  else
#    define MPMEXPORT
#  endif
#endif

#if defined(_MSC_VER)
#  define MPMCALL __stdcall
#else
#  define MPMCALL
#endif

#define MPMAPI(type) MPMEXPORT type MPMCALL

#include <miktex/core.h>

#include <string>
#include <vector>

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
  MiKTeX::Core::tstring deploymentName;

  /// Display name.
  MiKTeX::Core::tstring displayName;

  /// One-line package description.
  MiKTeX::Core::tstring title;

  /// Informal version information.
  MiKTeX::Core::tstring version;

  /// Multi-line package description.
  MiKTeX::Core::tstring description;

  /// Name of creator (packer).
  MiKTeX::Core::tstring creator;

  /// Accumulated file sizes of the run file list.
  size_t sizeRunFiles;

  /// Accumulated file sizes of the doc file list.
  size_t sizeDocFiles;

  /// Accumulated file sizes of the source file list.
  size_t sizeSourceFiles;

  /// The run file list.
  std::vector<MiKTeX::Core::tstring> runFiles;

  /// The doc file list.
  std::vector<MiKTeX::Core::tstring> docFiles;

  /// The source file list.
  std::vector<MiKTeX::Core::tstring> sourceFiles;

  /// List of required packages.
  // (these should be <set>s; but <set>s cannot be
  // exported from a DLL (see Q172396))
  std::vector<MiKTeX::Core::tstring> requiredPackages;

  /// List of dependants.
  std::vector<MiKTeX::Core::tstring> requiredBy;

  /// Date/time when the package was created.
  time_t timePackaged;

  /// Date/time when the package was installed.
  time_t timeInstalled;

  /// Size of the archive file.
  size_t archiveFileSize;

  /// MD5 of the package.
  MiKTeX::Core::MD5 digest;

  /// Initializes a new package info struct.
  PackageInfo ()
    : archiveFileSize (0),
      sizeDocFiles (0),
      sizeRunFiles (0),
      sizeSourceFiles (0),
      timeInstalled (static_cast<time_t>(0)),
      timePackaged (static_cast<time_t>(0))
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
    timeInstalled = rhs.timeInstalled;
    timePackaged = rhs.timePackaged;
    docFiles = rhs.docFiles;
    requiredBy = rhs.requiredBy;
    requiredPackages = rhs.requiredPackages;
    runFiles = rhs.runFiles;
    sourceFiles = rhs.sourceFiles;
    digest = rhs.digest;
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
      timeInstalled (other.timeInstalled),
      timePackaged (other.timePackaged),
      docFiles (other.docFiles),
      requiredBy (other.requiredBy),
      requiredPackages (other.requiredPackages),
      runFiles (other.runFiles),
      sourceFiles (other.sourceFiles),
      digest (other.digest)
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
    Remote
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
  MiKTeX::Core::tstring url;
  /// One-line description.
  MiKTeX::Core::tstring description;
  /// Country where the repository is resident.
  MiKTeX::Core::tstring country;
  /// Town where the repository is resident.
  MiKTeX::Core::tstring town;
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
  MiKTeX::Core::tstring proxy;
  int port;
  bool authenticationRequired;
  MiKTeX::Core::tstring user;
  MiKTeX::Core::tstring password;
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
  MPMCALL
  ~PackageManager ()
    = 0;

public:
  virtual
  void
  MPMCALL
  AddRef ()
    = 0;

public:
  virtual
  void
  MPMCALL
  Release ()
    = 0;

  /// Creates a new package installer object. The caller is responsible
  /// for deleting the object.
public:
  virtual
  class PackageInstaller *
  MPMCALL
  CreateInstaller ()
    = 0;

  /// Creates a new package iterator object. The caller is responsible
  /// for deleting the object.
public:
  virtual
  class PackageIterator *
  MPMCALL
  CreateIterator ()
    = 0;

  /// Recreates the file name database of the package manager.
public:
  virtual
  void
  MPMCALL
  CreateMpmFndb ()
    = 0;

  /// Gets the reference count of a file.
  /// @param path Path name of the file.
  /// @returns Returns the number of packages this file is a member of.
public:
  virtual
  unsigned long
  MPMCALL
  GetFileRefCount (/*[in]*/ const MiKTeX::Core::PathName &	path)
    = 0;

  /// Gets information about a specified package.
  /// @param lpszDeploymentName Package key.
  /// @param packageInfo Package info struct to be filled..
public:
  virtual
  PackageInfo
  MPMCALL
  GetPackageInfo (/*[in]*/ const MiKTeX::Core::tstring & deploymentName)
    = 0;

public:
  virtual
  void
  MPMCALL
  LoadDatabase (/*[in]*/ const MiKTeX::Core::PathName & path)
    = 0;

public:
  virtual
  void
  MPMCALL
  UnloadDatabase ()
    = 0;

public:
  virtual
  void
  MPMCALL
  DownloadRepositoryList ()
    = 0;

public:
  virtual
  std::vector<RepositoryInfo>
  MPMCALL
  GetRepositories ()
    = 0;
  
  /// Picks a random package repository.
  /// @return Returns a string object.
public:
  virtual
  MiKTeX::Core::tstring
  MPMCALL
  PickRepositoryUrl ()
    = 0;

public:
  virtual
  bool
  MPMCALL
  TryGetPackageInfo (/*[in]*/ const MiKTeX::Core::tstring &	deploymentName,
		     /*[out]*/ PackageInfo &			packageInfo)
    = 0;

public:
  static
  MPMEXPORT
  PackageManager *
  MPMCALL
  Create ();

public:
  static
  MPMEXPORT
  MiKTeX::Core::tstring
  MPMCALL
  GetRemotePackageRepository ();

public:
  static
  MPMEXPORT
  bool
  MPMCALL
  TryGetRemotePackageRepository (/*[out]*/ MiKTeX::Core::tstring & url);

public:
  static
  MPMEXPORT
  MiKTeX::Core::PathName
  MPMCALL
  GetLocalPackageRepository ();

public:
  static
  MPMEXPORT
  bool
  MPMCALL
  TryGetLocalPackageRepository (/*[out]*/ MiKTeX::Core::PathName & path);

public:
  static
  MPMEXPORT
  MiKTeX::Core::PathName
  MPMCALL
  GetMiKTeXDirectRoot ();

public:
  static
  MPMEXPORT
  bool
  MPMCALL
  TryGetMiKTeXDirectRoot (/*[out]*/ MiKTeX::Core::PathName & path);

public:
  static
  MPMEXPORT
  RepositoryType
  MPMCALL
  GetDefaultPackageRepository (/*[out]*/ MiKTeX::Core::tstring & urlOrPath);

public:
  static
  MPMEXPORT
  bool
  MPMCALL
  TryGetDefaultPackageRepository (/*[out]*/ RepositoryType & repositoryType,
				  /*[out]*/ MiKTeX::Core::tstring & urlOrPath);

public:
  static
  MPMEXPORT
  void
  MPMCALL
  SetDefaultPackageRepository
  (/*[in]*/ RepositoryType			repositoryType,
   /*[in]*/ const MiKTeX::Core::tstring &	urlOrPath);

public:
  static
  MPMEXPORT
  bool
  MPMCALL
  IsLocalPackageRepository (/*[in]*/ const MiKTeX::Core::PathName & path);

public:
  static
  MPMEXPORT
  void
  MPMCALL
  SetRemotePackageRepository (/*[in]*/ const MiKTeX::Core::tstring & url);

public:
  static
  MPMEXPORT
  void
  MPMCALL
  SetLocalPackageRepository (/*[in]*/ const MiKTeX::Core::PathName & path);

public:
  static
  MPMEXPORT
  void
  MPMCALL
  SetMiKTeXDirectRoot (/*[in]*/ const MiKTeX::Core::PathName & path);

public:
  static
  MPMEXPORT
  PackageInfo
  MPMCALL
  ReadPackageDefinitionFile (/*[in]*/ const MiKTeX::Core::PathName &	path,
			     /*[in]*/ const MIKTEXCHAR * lpszTeXMFPrefix);

public:
  static
  MPMEXPORT
  void
  MPMCALL
  WritePackageDefinitionFile (/*[in]*/ const MiKTeX::Core::PathName &	path,
			      /*[in]*/ const PackageInfo &	packageInfo,
			      /*[in]*/ time_t			timePackaged);

  /// Strips the TEXMF prefix from a string.
  /// @param str The string to strip.
  /// @param[out] result The result to be filled with the stripped string.
  /// @return Returns true, if the prefix was stripped.
public:
  static
  MPMEXPORT
  bool
  MPMCALL
  StripTeXMFPrefix (/*[in]*/ const MiKTeX::Core::tstring &	str,
		    /*[out]*/ MiKTeX::Core::tstring &		result);

public:
  static
  MPMEXPORT
  bool
  MPMCALL
  TryGetProxy (/*[out]*/ ProxySettings & proxySettings);

public:
  static
  MPMEXPORT
  ProxySettings
  MPMCALL
  GetProxy ();

public:
  static
  MPMEXPORT
  void
  MPMCALL
  SetProxy (/*[in]*/ const ProxySettings & proxySettings);
};

/// Smart pointer to a package manager interface.
typedef MiKTeX::Core::SmartPointer<PackageManager> PackageManagerPtr;

/* _________________________________________________________________________

   PackageManager2
   _________________________________________________________________________ */

/// The package manager v2 interface.
class
MIKTEXNOVTABLE
PackageManager2 : public PackageManager
{
protected:
  virtual
  MPMCALL
  ~PackageManager2 ()
    = 0;

public:
  virtual
  bool
  MPMCALL
  TryGetRepositoryInfo (/*[in]*/ const MiKTeX::Core::tstring &	url,
			/*[out]*/ RepositoryInfo &		repositoryInfo)
    = 0;

public:
  virtual
  RepositoryInfo
  MPMCALL
  VerifyPackageRepository (/*[in]*/ const MiKTeX::Core::tstring & url)
    = 0;

public:
  virtual
  bool
  MPMCALL
  TryVerifyInstalledPackage
  (/*[in]*/ const MiKTeX::Core::tstring & deploymentName)
    = 0;

public:
  static
  MPMEXPORT
  PackageManager2 *
  MPMCALL
  Create ();
};

/// Smart pointer to a package manager v2 interface.
typedef MiKTeX::Core::SmartPointer<PackageManager2> PackageManager2Ptr;

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
  MPMCALL
  ~PackageIterator ()
    = 0;

  /// Releases all resources used by this object.
public:
  virtual
  void
  MPMCALL
  Dispose ()
    = 0;

  /// Adds a browse filter.
  /// @param filter Filter flags to be added.
  /// @param lpsz Optional filter argument.
public:
  virtual
  void
  MPMCALL
  AddFilter (/*[in]*/ unsigned int		filter,
	     /*[in]*/ const MIKTEXCHAR *	lpsz = 0)
    = 0;

  /// Gets the next entry in the package database.
  /// @param[out] The package info struct to be filled.
  /// @return Returns true, if the information could be retrieved.
  /// Returns false, if there are no more entries in the database.
public:
  virtual
  bool
  MPMCALL
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

   IPackageInstallerCallback
   _________________________________________________________________________ */

/// Callback interface.
class
MIKTEXNOVTABLE
IPackageInstallerCallback
{
  /// Reporting. This method is called by the installer if a new message
  /// is available.
  /// @param lpszLine One-line message.
public:
  virtual
  void
  MPMCALL
  ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine)
    = 0;

  /// Error handling. This method is called by the installer if a
  /// problem was detected which can be remedied by the user.
  /// @param lpszMessage Message to be presented to the user.
  /// @returns Returns true, if the user has fixed the problem.
public:
  virtual
  bool
  MPMCALL
  OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage)
    = 0;

  /// Progress. This method is called by the installer if new
  /// progress information is available. Also gives the opportunity
  /// to abort all activities.
  /// @param nf Notification code.
  /// @returns Returns true, if the installer shall continue.
public:
  virtual
  bool
  MPMCALL
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
  MPMCALL
  ~PackageInstaller ()
    = 0;

  /// Releases all resources used by this package installer object.
public:
  virtual
  void
  MPMCALL
  Dispose ()
    = 0;

  /// Sets the package repository for this package installer object.
  /// @param The location of the package repository. Either an URL or
  /// an absolute path name.
public:
  virtual
  void
  MPMCALL
  SetRepository (/*[in]*/ const MiKTeX::Core::tstring & repository)
    = 0;

  /// Sets the installation directory for this package installer.
public:
  virtual
  void
  MPMCALL
  SetDestination (/*[in]*/ const MiKTeX::Core::PathName & destinationDirectory)
    = 0;

public:
  virtual
  void
  MPMCALL
  UpdateDb ()
    = 0;

  /// Starts the package database synchronization thread.
public:
  virtual
  void
  MPMCALL
  UpdateDbAsync ()
    = 0;

public:
  virtual
  void
  MPMCALL
  FindUpdates ()
    = 0;

public:
  virtual
  void
  MPMCALL
  FindUpdatesAsync ()
    = 0;

  /// Update info struct.
public:
  struct UpdateInfo
  {
    /// Initializes a new instance of the class.
    UpdateInfo ()
      : timePackaged (static_cast<time_t>(0))
    {
    }
    /// Package deployment name.
    MiKTeX::Core::tstring deploymentName;
    /// Date/Time the package was created.
    time_t timePackaged;

    bool
    IsBroken ()
      const
    {
      return (timePackaged == static_cast<time_t>(-1));
    }
  };

public:
  virtual
  std::vector<UpdateInfo>
  MPMCALL
  GetUpdates ()
    = 0;

public:
  virtual
  void
  MPMCALL
  InstallRemove ()
    = 0;

  /// Starts the installer background thread.
public:
  virtual
  void
  MPMCALL
  InstallRemoveAsync ()
    = 0;

public:
  virtual
  void
  MPMCALL
  WaitForCompletion ()
    = 0;

public:
  virtual
  void
  MPMCALL
  Download ()
    = 0;

  /// Starts the downloader.
public:
  virtual
  void
  MPMCALL
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
    MiKTeX::Core::tstring deploymentName;

    /// Display name of package.
    MiKTeX::Core::tstring displayName;

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
  MPMCALL
  GetProgressInfo ()
    = 0;

  /// Sets the callback interface.
  /// @param pCallback Pointer to an interface.
public:
  virtual
  void
  MPMCALL
  SetCallback (/*[in]*/ IPackageInstallerCallback *	pCallback)
    = 0;

  /// Sets the package lists.
  /// @param toBeInstalled Packages to be installed.
  /// @param toBeRemoved Packages to be removed.
public:
  virtual
  void
  MPMCALL
  SetFileLists
  (/*[in]*/ const std::vector<MiKTeX::Core::tstring> & toBeInstalled,
   /*[in]*/ const std::vector<MiKTeX::Core::tstring> & toBeRemoved)
    = 0;

  /// Sets the package level.
public:
  virtual
  void
  MPMCALL
  SetPackageLevel (/*[in]*/ PackageLevel packageLevel)
    = 0;

  /// Sets the file lists.
  /// @param toBeInstalled Packages to be installed.
public:
  virtual
  void
  MPMCALL
  SetFileList
  (/*[in]*/ const std::vector<MiKTeX::Core::tstring> & toBeInstalled)
    = 0;

  /// Tests if the installer is running.
public:
  virtual
  bool
  MPMCALL
  IsRunning ()
    const
    = 0;
};

MPM_END_NAMESPACE;

#undef MPM_BEGIN_NAMESPACE
#undef MPM_END_NAMESPACE

#endif
