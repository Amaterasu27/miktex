/* mpc.cpp: creating MiKTeX packages

   Copyright (C) 2001-2007 Christian Schenk

   This file is part of MPC.

   MPC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MPC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MPC; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include "StdAfx.h"
#include "internal.h"

const char * const TheNameOfTheGame = T_("MiKTeX Package Composer");

#define PROGNAME "mpc"

/* _________________________________________________________________________

   ArchiveFileType
   _________________________________________________________________________ */

class ArchiveFileTypeEnum
{
public:
  enum EnumType {
    None = 0,
    MSCab = 1,
    TarBzip2 = 2,
    Zip = 3,
    Tar = 4,
    TarLzma = 5,
  };

public:
  static
  const char *
  GetFileNameExtension (/*[in]*/ EnumType archiveFileType)
  {
    switch (archiveFileType)
      {
      case MSCab:
	return (MIKTEX_CABINET_FILE_SUFFIX);
      case TarBzip2:
	return (MIKTEX_TARBZIP2_FILE_SUFFIX);
      case TarLzma:
	return (MIKTEX_TARLZMA_FILE_SUFFIX);
      case Zip:
	return (MIKTEX_ZIP_FILE_SUFFIX);
      case Tar:
	return (MIKTEX_TAR_FILE_SUFFIX);
      default:
	MiKTeX::Core::Session::FatalMiKTeXError
	  ("ArchiveFileTypeEnum::GetFileNameExtension",
	   T_("Unknown archive file type."),
	   0,
	   __FILE__,
	   __LINE__);
      }
  }
};

typedef EnumWrapper<ArchiveFileTypeEnum> ArchiveFileType;

/* _________________________________________________________________________

   RestoreCurrentDirectory
   _________________________________________________________________________ */

class RestoreCurrentDirectory
{
public:
  RestoreCurrentDirectory (/*[in]*/ const PathName & path)
  {
    oldPath.SetToCurrentDirectory ();
    Directory::SetCurrentDirectory (path);
  }
public:
  ~RestoreCurrentDirectory ()
  {
    try
      {
	Directory::SetCurrentDirectory (oldPath);
      }
    catch (const exception &)
      {
      }
  }
private:
  PathName oldPath;
};

/* _________________________________________________________________________

   PackageSpec
   _________________________________________________________________________ */

struct PackageSpec
{
  PackageSpec ()
    : level (0),
      archiveFileType (ArchiveFileType::None)
  {
  }
  string	deploymentName;	// external package name
  char		level;		// one of: '-', 'S', 'M', 'L', 'T'
  ArchiveFileType archiveFileType;
};

/* _________________________________________________________________________

   FileDigestTable

   Files and their digests.
   _________________________________________________________________________ */

struct PathNameComparer
  : public binary_function<string, string, bool>
{
  bool
  operator() (/*[in]*/ const string & str1,
	      /*[in]*/ const string & str2)
    const
  {
    return (PathName::Compare(str1.c_str(), str2.c_str()) < 0);
  }
};

typedef map<string, MD5, PathNameComparer> FileDigestTable;
  
/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 300,
  OPT_ASSEMBLE_DATABASE,
  OPT_ASSEMBLE_NET_DATABASE,
  OPT_ASSEMBLE_NET_PACKAGES,
  OPT_ASSEMBLE_TDS,
  OPT_DEFAULT_LEVEL,
  OPT_DEST_DIR,
  OPT_DISASSEMBLE_PACKAGE,
  OPT_INCLUDE_PACKAGE,
  OPT_MIKTEX_SERIES,
  OPT_PACKAGE_LIST,
  OPT_REPOSITORY,
  OPT_ROOT_DIR,
  OPT_TEXMF_PREFIX,
  OPT_TIME_PACKAGED,
  OPT_TPM_DIR,
  OPT_TPM_FILE,
  OPT_VERBOSE,
  OPT_VERSION,
};

/* _________________________________________________________________________

   MpcPackageInfo
   _________________________________________________________________________ */

struct MpcPackageInfo : public PackageInfo
{
  MD5			archiveFileDigest;
  PathName		path;
  MpcPackageInfo ()
  {
  }
  MpcPackageInfo (/*[in]*/ const PackageInfo & packageInfo)
    : PackageInfo (packageInfo)
  {
  }
};

/* _________________________________________________________________________

   PackageComposer
   _________________________________________________________________________ */

class PackageComposer
  : public IRunProcessCallback

{
public:
  PackageComposer ()
    : optVerbose (false),
      timePackaged (static_cast<time_t>(-1)),
      texmfPrefix ("texmf"),
      defaultLevel ('T'),
      miktexSeries (MIKTEX_SERIES_STR),
      defaultArchiveFileType (ArchiveFileType::TarLzma)
  {
  }

public:
  void
  Init (/*[in]*/ const char * argv0);

public:
  void
  Finalize ();

public:
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv);

protected:
  ArchiveFileType
  GetDbArchiveFileType ();

protected:
  PathName
  GetDbFileName (/*[in]*/ int			id,
		 /*[in]*/ const VersionNumber &	versionNumber);

protected:
  PathName
  GetDbLightFileName ();

protected:
  PathName
  GetDbHeavyFileName ();

protected:
  void
  Verbose (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);
    
protected:
  void
  FatalError (/*[in]*/ const char *	lpszFormat,
	      /*[in]*/			...);

protected:
  void
  Warning (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

protected:
  MD5
  GetTdsDigest (/*[in]*/ const FileDigestTable &	fileDigests);

protected:
  MD5
  MD5CopyFile (/*[in]*/ const PathName &	source,
	       /*[in]*/ const PathName &	dest);
  
protected:
  void
  MD5WildCopy (/*[in]*/ const PathName &	sourceTemplate,
	       /*[in]*/ const PathName &	destDir,
	       /*[in]*/ const PathName &	prefix,
	       /*[in,out]*/ FileDigestTable &	fileDigests);

protected:
  void
  MD5CopyFiles (/*[in]*/ const vector<string> &	files,
		/*[in]*/ const PathName &	sourceDir,
		/*[in]*/ const char *		lpszSourceSubDir,
		/*[in]*/ const PathName &	destDir,
		/*[in]*/ const char *		lpszDestSubDir,
		/*[in,out]*/ FileDigestTable &	fileDigests);
  
protected:
  void
  WriteDescriptionFile (/*[in]*/ const string &	description,
			/*[in]*/ const PathName &	directory);

protected:
  void
  InitializePackageDirectory (/*[in]*/ const PackageInfo &	packageInfo,
			      /*[in]*/ const MD5 &		digest,
			      /*[in]*/ const PathName &		packageDir);

protected:
  void
  CopyPackage (/*[in]*/ const MpcPackageInfo &	packageinfo,
	       /*[in]*/ const PathName &		destDir);

protected:
  void
  ReadDescriptionFile (/*[in]*/ const char *	lpszPackageDir,
		       /*[out]*/ string &	description);
  
protected:
  void
  InitializePackageInfo (/*[in]*/ const char *		lpszPackagePath,
			 /*[out]*/ MpcPackageInfo &	packageinfo);

protected:
  char
  GetPackageLevel (/*[in]*/ const MpcPackageInfo &	packageInfo);

protected:
  ArchiveFileType
  GetPackageArchiveFileType (/*[in]*/ const MpcPackageInfo &	packageInfo);

protected:
  bool
  ConsiderP (/*[in]*/ const MpcPackageInfo &	packageInfo);

protected:
  bool
  IsInTeXMFDirectory (/*[in]*/ const PathName &	relPath,
		      /*[in]*/ const char * lpszDirectory);

protected:
  void
  CollectFiles (/*[in]*/ const PathName &	rootDir,
		/*[in]*/ const PathName &	subDir,
		/*[out]*/ vector<string> &	runFiles,
		/*[out]*/ size_t &		sizeRunFiles,
		/*[out]*/ vector<string> &	docFiles,
		/*[out]*/ size_t &		sizeDocFiles,
		/*[out]*/ vector<string> &	sourceFiles,
		/*[out]*/ size_t &	sizeSourceFiles);

protected:
  void
  CollectSubTree (/*[in]*/ const PathName &	path,
		  /*[in]*/ const char *		lpszSubDir,
		  /*[out]*/ vector<string> &	runFiles,
		  /*[out]*/ size_t &		sizeRunFiles,
		  /*[out]*/ vector<string> &	docFiles,
		  /*[out]*/ size_t &		sizeDocFiles,
		  /*[out]*/ vector<string> &	sourceFiles,
		  /*[out]*/ size_t &	sizeSourceFiles);

protected:
  void
  CollectPackage (/*[in,out]*/ MpcPackageInfo &	packageInfo);

protected:
  void
  CollectPackages (/*[in]*/ const PathName &			rootDir,
		   /*[in,out]*/ map<string, MpcPackageInfo> &	packageTable);

protected:
  void
  AssembleTDS (/*[in]*/ const map<string, MpcPackageInfo> &	packageTable,
	       /*[in]*/ const PathName &			destDir,
	       /*[in,out]*/ Cfg &				dbLight);

protected:
  void
  WritePackageDefinitionFiles
  (/*[in]*/ const map<string, MpcPackageInfo> &	packageTable,
   /*[in]*/ const PathName &			destDir,
   /*[in]*/ Cfg &				dbLight);

protected:
  void
  ExecuteSystemCommand (/*[in]*/ const char * lpszCommand);

protected:
  void
  RunArchiver (/*[in]*/ ArchiveFileType		archiveFileType,
	       /*[in]*/ const PathName &	archiveFile,
	       /*[in]*/ const char *		lpszFilter);

protected:
  void
  WriteNetDatabase (/*[in]*/ const map<string, MpcPackageInfo> & packageTable,
		    /*[in]*/ const PathName &			destDir,
		    /*[in,out]*/ Cfg &				dbLight);

protected:
  void
  ExtractFile (/*[in]*/ const PathName &	archiveFile,
	       /*[in]*/ ArchiveFileType		archiveFileType,
	       /*[in]*/ const PathName &	toBeExtracted,
	       /*[in]*/ const PathName &	outFile);

protected:
  void
  CompressArchive (/*[in]*/ const PathName &	toBeCompressed,
		   /*[in]*/ ArchiveFileType	archiveFileType,
		   /*[in]*/ const PathName &	outFile);

protected:
  ArchiveFileType
  CreateArchiveFile (/*[in,out]*/ MpcPackageInfo &	packageInfo,
		     /*[in]*/ const PathName &		destDir);

protected:
  bool
  LoadDbLight (/*[in]*/ const PathName &	directory,
	       /*[out]*/ Cfg &			cfg);

protected:
  void
  AssembleNetPackages (/*[out]*/ map<string, MpcPackageInfo> &	packageTable,
		       /*[in]*/ const PathName &		destDir,
		       /*[out]*/ Cfg &				dbLight);

protected:
  void
  ReadList (/*[in]*/ const PathName &			path,
	    /*[out]*/ map<string, PackageSpec> &	mapPackageList);

protected:
  void
  ReadList (/*[in]*/ const PathName &	path,
	    /*[out]*/ set<string> &	setPackageList);

protected:
  void
  DisassemblePackage (/*[in]*/ const PathName &	packageDefinitionFile,
		      /*[in]*/ const PathName &	sourceDir,
		      /*[in]*/ const PathName &	destDir);

private:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n);

private:
  SessionWrapper pSession;

private:
// verbosity flag
  bool optVerbose;

private:
  // value of "TPM:TimePackaged"
  time_t timePackaged;

private:
  string texmfPrefix;

private:
  // accumulated --package-list contents
  map<string, PackageSpec> packageList;

private:
  // accumulated --include-package contents
  set<string> toBeIncluded;
  
private:
  // default package level
  char defaultLevel;

private:
  // default MiKTeX series
  VersionNumber miktexSeries;

private:  
  // default archive file type
  ArchiveFileType defaultArchiveFileType;

private:
  // command-line options
  static const struct poptOption options[];
};

/* _________________________________________________________________________

   PackageComposer::options
   _________________________________________________________________________ */

const struct poptOption PackageComposer::options[] = {
  {
    "assemble-database", 0,
    POPT_ARG_NONE, 0,
    OPT_ASSEMBLE_DATABASE,
    T_("Write package definition files."),
    0
  },

  {
    "assemble-net-database", 0,
    POPT_ARG_NONE, 0,
    OPT_ASSEMBLE_NET_DATABASE,
    T_("Write package database."),
    0
  },

  {
    "assemble-net-packages", 0,
    POPT_ARG_NONE, 0,
    OPT_ASSEMBLE_NET_PACKAGES,
    T_("Create package archive files."),
    0
  },

  {
    "assemble-tds", 0,
    POPT_ARG_NONE, 0,
    OPT_ASSEMBLE_TDS,
    T_("Create a TDS hierarchy."),
    0
  },

  {
    "default-level", 0,
    POPT_ARG_STRING, 0,
    OPT_DEFAULT_LEVEL,
    T_("Specify the default package level (one of: S, M, L, T)."),
    T_("LEVEL")
  },

  {
    "dest-dir", 0,
    POPT_ARG_STRING, 0,
    OPT_DEST_DIR,
    T_("Specify the destination directory."),
    T_("DIR")
  },

  {
    "disassemble-package", 0,
    POPT_ARG_NONE, 0,
    OPT_DISASSEMBLE_PACKAGE,
    T_("Disassemble a package."),
    0
  },


  {
    "include-package", 0,
    POPT_ARG_STRING, 0,
    OPT_INCLUDE_PACKAGE,
    T_("Add a package to the inclusion list."),
    T_("PACKAGE")
  },

  {
    "miktex-series", 0,
    POPT_ARG_STRING, 0,
    OPT_MIKTEX_SERIES,
    T_("Specify the MiKTeX series (one of: 2.5, 2.6, 2.7)."),
    T_("SERIES")
  },

  {
    "package-list", 0,
    POPT_ARG_STRING, 0,
    OPT_PACKAGE_LIST,
    T_("Specify the package list file."),
    T_("FILE")
  },

  {
    "repository", 0,
    POPT_ARG_STRING, 0,
    OPT_REPOSITORY,
    T_("Specify the location of the local package repository."),
    T_("DIR")
  },

  {
    "root-dir", 0,
    POPT_ARG_STRING, 0,
    OPT_ROOT_DIR,
    T_("Specify the root directory."),
    T_("DIR")
  },

  {
    "texmf-prefix", 0,
    POPT_ARG_STRING, 0,
    OPT_TEXMF_PREFIX,
    T_("Specify the TEXMF prefix."),
    T_("PREFIX")
  },

  {
    "time-packaged", 0,
    POPT_ARG_STRING, 0,
    OPT_TIME_PACKAGED,
    T_("Specify the package time-stamp (in seconds since 1970)."),
    T_("TIME")
  },

  {
    "tpm-dir", 0,
    POPT_ARG_STRING, 0,
    OPT_TPM_DIR,
    T_("Specify the destination directory for package definition files."),
    T_("DIR")
  },

  {
    "tpm-file", 0,
    POPT_ARG_STRING, 0,
    OPT_TPM_FILE,
    T_("Specify the name of a package definition file."),
    T_("FILE")
  },

  {
    "verbose", 0,
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Turn on verbose mode."),
    0
  },

  {
    T_("version"), 0,
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   PackageComposer::Init
   _________________________________________________________________________ */

void
PackageComposer::Init (/*[in]*/ const char * argv0)
{
  Session::InitInfo initInfo;
  initInfo.SetProgramInvocationName (argv0);
  pSession.CreateSession (initInfo);
}

/* _________________________________________________________________________

   PackageComposer::Finalize
   _________________________________________________________________________ */

void
PackageComposer::Finalize ()
{
  pSession.Reset ();
}

/* _________________________________________________________________________

   PackageComposer::GetDbArchiveFileType
   _________________________________________________________________________ */

ArchiveFileType
PackageComposer::GetDbArchiveFileType ()
{
  if (miktexSeries < "2.7")
    {
      return (ArchiveFileType::TarBzip2);
    }
  else
    {
      return (ArchiveFileType::TarLzma);
    }
}

/* _________________________________________________________________________

   PackageComposer::GetDbFileName
   _________________________________________________________________________ */

PathName
PackageComposer::GetDbFileName (/*[in]*/ int			id,
				/*[in]*/ const VersionNumber &	versionNumber)
{
  PathName ret ("miktex-zzdb");
  ret.Append (NUMTOSTR(id), false);
  ret.Append ("-", false);
  ret.Append (versionNumber.ToString().c_str(), false);
  ret.AppendExtension (ArchiveFileType::GetFileNameExtension
		       (GetDbArchiveFileType().Get()));
  return (ret);
}

/* _________________________________________________________________________

   PackageComposer::GetDbLightFileName
   _________________________________________________________________________ */

PathName
PackageComposer::GetDbLightFileName ()
{
  return (GetDbFileName(1, miktexSeries));
}

/* _________________________________________________________________________

   PackageComposer::GetDbHeavyFileName
   _________________________________________________________________________ */

PathName
PackageComposer::GetDbHeavyFileName ()
{
  return (GetDbFileName(2, miktexSeries));
}

/* _________________________________________________________________________

   PackageComposer::Verbose
   _________________________________________________________________________ */

void
PackageComposer::Verbose (/*[in]*/ const char *	lpszFormat,
			  /*[in]*/		...)
{
  if (optVerbose)
    {
      va_list arglist;
      va_start (arglist, lpszFormat);
      cout << Utils::FormatString(lpszFormat, arglist) << endl;
      va_end (arglist);
    }
}

/* _________________________________________________________________________

   PackageComposer::FatalError
   _________________________________________________________________________ */

void
PackageComposer::FatalError (/*[in]*/ const char *	lpszFormat,
			     /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << PROGNAME << ": "
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   PackageComposer::Warning
   _________________________________________________________________________ */

void
PackageComposer::Warning (/*[in]*/ const char *	lpszFormat,
			  /*[in]*/		...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << PROGNAME << T_(": warning: ")
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
}

/* _________________________________________________________________________

   PackageComposer::GetTdsDigest
   _________________________________________________________________________ */

MD5
PackageComposer::GetTdsDigest (/*[in]*/ const FileDigestTable &	fileDigests)
{
  MD5Builder md5Builder;
  for (FileDigestTable::const_iterator it = fileDigests.begin();
       it != fileDigests.end();
       ++ it)
    {
      PathName path (it->first);
      // we must dosify the path name for backward compatibility
      path.ToDos ();
      md5Builder.Update (path.Get(), path.GetLength());
      md5Builder.Update (it->second.GetBits(), sizeof(MD5));
    }
  return (md5Builder.Final());
}

/* _________________________________________________________________________

   PackageComposer::MD5CopyFile
   _________________________________________________________________________ */

MD5
PackageComposer::MD5CopyFile (/*[in]*/ const PathName &	source,
			      /*[in]*/ const PathName &	dest)
{
  FileStream fromStream (File::Open(source,
				    FileMode::Open,
				    FileAccess::Read,
				    false));
  FileStream toStream (File::Open(dest,
				  FileMode::Create,
				  FileAccess::Write,
				  false));
  char buf[4096];
  size_t n;
  MD5Builder md5Builder;
  while ((n = fromStream.Read(buf, 4096)) > 0)
    {
      toStream.Write (buf, n);
      md5Builder.Update (buf, n);
    }
  toStream.Close ();
  fromStream.Close ();
  time_t creationTime;
  time_t lastAccessTime;
  time_t lastWriteTime;
  File::GetTimes (source, creationTime, lastAccessTime, lastWriteTime);
  File::SetTimes (dest, creationTime, lastAccessTime, lastWriteTime);
  return (md5Builder.Final());
}

/* _________________________________________________________________________

   PackageComposer::MD5WildCopy
   _________________________________________________________________________ */

void
PackageComposer::MD5WildCopy (/*[in]*/ const PathName &		sourceTemplate,
			      /*[in]*/ const PathName &		destDir,
			      /*[in]*/ const PathName &		prefix,
			      /*[in,out]*/ FileDigestTable &	fileDigests)
{
  PathName sourceDir (sourceTemplate);
  sourceDir.RemoveFileSpec ();

  if (! Directory::Exists(sourceDir))
    {
      return;
    }

  PathName pattern (sourceTemplate);
  pattern.RemoveDirectorySpec ();

  // create destination directory
  Directory::Create (destDir);

  DirectoryEntry direntry;

  auto_ptr<DirectoryLister>
    pLister (DirectoryLister::Open(sourceDir, pattern.Get()));

  bool haveSomething = false;

  while (pLister->GetNext(direntry))
    {
      haveSomething = true;

      // don't recurse
      if (direntry.isDirectory)
	{
	  continue;
	}
      
      // make name of source file
      PathName sourcePath (sourceDir, direntry.name.c_str(), 0);

      // make name of destination file
      PathName destPath (destDir, direntry.name.c_str(), 0);
      
      // copy file and calculate its digest
      MD5 digest = MD5CopyFile(sourcePath, destPath);

      // package definition files do not contribute to the TDS digest
      if (sourcePath.HasExtension(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX))
	{
	  continue;
	}

      // relativize file name
      const char * lpszRelPath
	= Utils::GetRelativizedPath(destPath.Get(), prefix.Get());

      // remember digest
      fileDigests[lpszRelPath] = digest;
    }

  pLister->Close ();
  pLister.reset ();

  if (! haveSomething)
    {
      FatalError (T_("No match for %s"), Q_(sourceTemplate.Get()));
    }
}

/* _________________________________________________________________________

   PackageComposer::MD5CopyFiles
   _________________________________________________________________________ */

void
PackageComposer::MD5CopyFiles (/*[in]*/ const vector<string> &	files,
			       /*[in]*/ const PathName &	sourceDir,
			       /*[in]*/ const char *	lpszSourceSubDir,
			       /*[in]*/ const PathName &	destDir,
			       /*[in]*/ const char *		lpszDestSubDir,
			       /*[in,out]*/ FileDigestTable &	fileDigests)
{
  // path to source root directory
  PathName sourceRootDir (sourceDir, lpszSourceSubDir);

  // path to destination root directory
  PathName destRootDir (destDir, lpszDestSubDir);

  for (vector<string>::const_iterator it = files.begin();
       it != files.end();
       ++ it)
    {
      // source template
      PathName sourceTemplate (sourceRootDir, *it);

      // path to destination directory
      PathName destDir (destRootDir, *it);
      destDir.RemoveFileSpec ();

      // copy file
      MD5WildCopy (sourceTemplate, destDir, destRootDir, fileDigests);
    }
}

/* _________________________________________________________________________

   PackageComposer::WriteDescriptionFile
   _________________________________________________________________________ */

void
PackageComposer::WriteDescriptionFile (/*[in]*/ const string &	description,
				       /*[in]*/ const PathName & directory)
{
  Directory::Create (directory);
  FileStream stream (File::Open(PathName(directory, "Description", 0),
				FileMode::Create,
				FileAccess::Write));
  fputs (description.c_str(), stream.Get());
  stream.Close ();
}

/* _________________________________________________________________________

   PackageComposer::InitializePackageDirectory

   Write package.ini and Description.
   _________________________________________________________________________ */

void
PackageComposer::InitializePackageDirectory
(/*[in]*/ const PackageInfo &	packageInfo,
 /*[in]*/ const MD5 &		digest,
 /*[in]*/ const PathName &	packageDir)
{
  // create package directory
  Directory::Create (packageDir);

  // open package.ini for writing
  FileStream stream (File::Open(PathName(packageDir, "package.ini", 0),
				FileMode::Create,
				FileAccess::Write));

  // write values
  fprintf (stream.Get(), "externalname=%s\n",
	   packageInfo.deploymentName.c_str());
  fprintf (stream.Get(), "name=%s\n", packageInfo.displayName.c_str());
  fprintf (stream.Get(), "creator=%s\n", packageInfo.creator.c_str());
  fprintf (stream.Get(), "title=%s\n", packageInfo.title.c_str());
  fprintf (stream.Get(), "version=%s\n", packageInfo.version.c_str());
  fprintf (stream.Get(), "md5=%s\n", digest.ToString().c_str());
  for (size_t i = 0; i < packageInfo.requiredPackages.size(); ++ i)
    {
      if (i > 0)
	{
	  fputc (';', stream.Get());
	}
      fputs (packageInfo.requiredPackages[i].c_str(), stream.Get());
    }

  stream.Close ();

  // write Description
  if (! packageInfo.description.empty())
    {
      WriteDescriptionFile (packageInfo.description, packageDir);
    }
}

/* _________________________________________________________________________

   PackageComposer::CopyPackage
   _________________________________________________________________________ */

void
PackageComposer::CopyPackage (/*[in]*/ const MpcPackageInfo &	packageinfo,
			      /*[in]*/ const PathName &		destDir)
{
  Verbose (T_("Copying %s ..."), Q_(packageinfo.deploymentName.c_str()));

  // path to package definition directory, e.g.:
  // /miktex/texmf/tpm/packages/
  PathName packageDefinitionDirectory
    (destDir,
     PathName(texmfPrefix, MIKTEX_PATH_PACKAGE_DEFINITION_DIR, 0),
     0);
  
  // create package definition directory
  Directory::Create (packageDefinitionDirectory);
  
  // create the package definition file...
  PackageManager::WritePackageDefinitionFile
    (PathName(packageDefinitionDirectory,
	      packageinfo.deploymentName.c_str(),
	      MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX),
     packageinfo,
     timePackaged);

  // copy files and calculate digests
  FileDigestTable fileDigests;
  MD5CopyFiles (packageinfo.runFiles,
		packageinfo.path,
		"Files",
		destDir,
		0,
		fileDigests);
  MD5CopyFiles (packageinfo.docFiles,
		packageinfo.path,
		"Files",
		destDir,
		0,
		fileDigests);
  MD5CopyFiles (packageinfo.sourceFiles,
		packageinfo.path,
		"Files",
		destDir,
		0,
		fileDigests);

  // check TDS digest
  if (! (GetTdsDigest(fileDigests) == packageinfo.digest))
    {
      FatalError (T_("Bad TDS digest (%s)."),
		  packageinfo.deploymentName.c_str());
    }
}

/* _________________________________________________________________________

   PackageComposer::ReadDescriptionFile
   _________________________________________________________________________ */

void
PackageComposer::ReadDescriptionFile (/*[in]*/ const char *	lpszPackageDir,
				      /*[out]*/ string &	description)
{
  PathName descriptionFileName (lpszPackageDir, "Description");
  if (! File::Exists(descriptionFileName))
    {
      description = "";
      return;
    }
  FileStream stream (File::Open(descriptionFileName,
				FileMode::Open,
				FileAccess::Read));
  int ch;
  while ((ch = fgetc(stream.Get())) != EOF)
    {
      description += static_cast<char>(ch);
    }
  stream.Close ();
}

/* _________________________________________________________________________

   PackageComposer::InitializePackageInfo
   _________________________________________________________________________ */

void
PackageComposer::InitializePackageInfo (/*[in]*/ const char * lpszPackagePath,
					/*[out]*/ MpcPackageInfo & packageinfo)
{
  SmartPointer<Cfg> pcfg (Cfg::Create());

  // read package.ini
  pcfg->Read (PathName(lpszPackagePath, "package.ini", 0));

  // get deployment name (mandatory value)
  if (! pcfg->TryGetValue(0, "externalname", packageinfo.deploymentName))
    {
      FatalError (T_("Invalid package information file (externalname)."));
    }

  // get display name (mandatory value)
  if (! pcfg->TryGetValue(0, "name", packageinfo.displayName))
    {
      FatalError (T_("Invalid package information file (name)."));
    }

  // get creator (optional value)
  pcfg->TryGetValue (0, "creator", packageinfo.creator);

  // get title (optional value)
  pcfg->TryGetValue (0, "title", packageinfo.title);

  // get version (optional value)
  pcfg->TryGetValue (0, "version", packageinfo.version);

  // get required packages (optional value)
  string strReqList;
  if (pcfg->TryGetValue(0, "requires", strReqList))
    {
      for (CSVList tok (strReqList.c_str(), ';');
	   tok.GetCurrent() != 0;
	   ++ tok)
	{
	  packageinfo.requiredPackages.push_back (tok.GetCurrent());
	}
    }

  // get TDS digest (optional value)
  string str;
  if (pcfg->TryGetValue(0, "md5", str))
    {
      packageinfo.digest = MD5::Parse(str.c_str());
    }

  // read extra description file
  ReadDescriptionFile (lpszPackagePath, packageinfo.description);

  // remember the package directory
  packageinfo.path = lpszPackagePath;
}

/* _________________________________________________________________________

   PackageComposer::GetPackageLevel
   _________________________________________________________________________ */

char
PackageComposer::GetPackageLevel (/*[in]*/ const MpcPackageInfo & packageInfo)
{
  map<string, PackageSpec>::const_iterator
    it = packageList.find(packageInfo.deploymentName);
  if (it == packageList.end())
    {
      // assume default level, if the package is not listed
      return (defaultLevel);
    }
  return (it->second.level);
}

/* _________________________________________________________________________

   PackageComposer::GetPackageArchiveFileType
   _________________________________________________________________________ */

ArchiveFileType
PackageComposer::GetPackageArchiveFileType
(/*[in]*/ const MpcPackageInfo &	packageInfo)
{
  map<string, PackageSpec>::const_iterator
    it = packageList.find(packageInfo.deploymentName);
  if (it == packageList.end())
    {
      // assume default archive file type, if the package is not listed
      return (defaultArchiveFileType);
    }
  return (it->second.archiveFileType);
}

/* _________________________________________________________________________

   PackageComposer::ConsiderP
   _________________________________________________________________________ */

bool
PackageComposer::ConsiderP (/*[in]*/ const MpcPackageInfo &	packageInfo)
{
  return (GetPackageLevel(packageInfo) != '-');
}

/* _________________________________________________________________________

   PackageComposer::IsInTeXMFDirectory
   _________________________________________________________________________ */

bool
PackageComposer::IsInTeXMFDirectory (/*[in]*/ const PathName &	relPath,
				     /*[in]*/ const char * lpszDirectory)
{
  PathName texmfDirectory (texmfPrefix);
  texmfDirectory += lpszDirectory;
  return (PathName::Compare(texmfDirectory,
			    relPath,
			    texmfDirectory.GetLength())
	  == 0);
}

/* _________________________________________________________________________

   PackageComposer::CollectFiles
   _________________________________________________________________________ */

void
PackageComposer::CollectFiles (/*[in]*/ const PathName &	rootDir,
			       /*[in]*/ const PathName &	subDir,
			       /*[out]*/ vector<string> &	runFiles,
			       /*[out]*/ size_t &		sizeRunFiles,
			       /*[out]*/ vector<string> &	docFiles,
			       /*[out]*/ size_t &		sizeDocFiles,
			       /*[out]*/ vector<string> &	sourceFiles,
			       /*[out]*/ size_t &	sizeSourceFiles)
{
  // directory to be inspected, e.g.:
  // /mypackages/a0poster/RunFiles/texmf/tex/latex/a0poster/
  PathName directory (rootDir, subDir);

  if (! Directory::Exists(directory))
    {
      return;
    }

  auto_ptr<DirectoryLister> lister (DirectoryLister::Open(directory));

  DirectoryEntry2 dirEntry;

  while (lister->GetNext(dirEntry))
    {
      // path relative to root directory
      PathName relPath (subDir);
      relPath += dirEntry.name;

      // check to see if it is a sub-directory
      if (dirEntry.isDirectory)
	{
	  // recurse into sub-directory
	  CollectFiles (rootDir,
			relPath,
			runFiles, sizeRunFiles,
			docFiles, sizeDocFiles,
			sourceFiles, sizeSourceFiles);
	}
      else
	{
	  // store file name
	  // add file size to total size
	  if (IsInTeXMFDirectory(relPath, "doc"))
	    {
	      docFiles.push_back (relPath.ToString());
	      sizeDocFiles += dirEntry.size;
	    }
	  else if (IsInTeXMFDirectory(relPath, "source"))
	    {
	      sourceFiles.push_back (relPath.ToString());
	      sizeSourceFiles += dirEntry.size;
	    }
	  else
	    {
	      runFiles.push_back (relPath.ToString());
	      sizeRunFiles += dirEntry.size;
	    }
	}
    }

  lister->Close ();
  lister.reset ();
}

/* _________________________________________________________________________

   PackageComposer::CollectSubTree
   _________________________________________________________________________ */

void
PackageComposer::CollectSubTree (/*[in]*/ const PathName &	path,
				 /*[in]*/ const char *		lpszSubDir,
				 /*[out]*/ vector<string> &	runFiles,
				 /*[out]*/ size_t &		sizeRunFiles,
				 /*[out]*/ vector<string> &	docFiles,
				 /*[out]*/ size_t &		sizeDocFiles,
				 /*[out]*/ vector<string> &	sourceFiles,
				 /*[out]*/ size_t &	sizeSourceFiles)
{
  PathName sourceDir (path, lpszSubDir);
  CollectFiles (sourceDir,
		0,
		runFiles, sizeRunFiles,
		docFiles, sizeDocFiles,
		sourceFiles, sizeSourceFiles);
}

/* _________________________________________________________________________

   PackageComposer::CollectPackage
   _________________________________________________________________________ */

void
PackageComposer::CollectPackage (/*[in,out]*/ MpcPackageInfo &	packageInfo)
{
  // clear file lists
  packageInfo.sizeRunFiles = 0;
  packageInfo.runFiles.clear ();
  packageInfo.sizeDocFiles = 0;
  packageInfo.docFiles.clear ();
  packageInfo.sizeSourceFiles = 0;
  packageInfo.sourceFiles.clear ();

  // collect files
  CollectSubTree (packageInfo.path,
		  "Files",
		  packageInfo.runFiles,
		  packageInfo.sizeRunFiles,
		  packageInfo.docFiles,
		  packageInfo.sizeDocFiles,
		  packageInfo.sourceFiles,
		  packageInfo.sizeSourceFiles);
}

/* _________________________________________________________________________

   PackageComposer::CollectPackages
   _________________________________________________________________________ */

void
PackageComposer::CollectPackages
(/*[in]*/ const PathName &			rootDir,
 /*[in,out]*/ map<string, MpcPackageInfo> &	packageTable)
{
  if (! Directory::Exists(rootDir))
    {
      return;
    }

  auto_ptr<DirectoryLister> lister (DirectoryLister::Open(rootDir));

  DirectoryEntry dirEntry;

  while (lister->GetNext(dirEntry))
    { 
      if (! dirEntry.isDirectory)
	{
	  continue;
	}

      // path to package directory
      PathName packagePath (rootDir, dirEntry.name);

      // check to see if package.ini exists
      if (! File::Exists(PathName(packagePath, "package.ini")))
	{
	  continue;
	}

      // read package.ini and Description
      MpcPackageInfo packageInfo;
      InitializePackageInfo (packagePath.Get(), packageInfo);

      if (! ConsiderP(packageInfo))
	{
	  continue;
	}

      Verbose (T_("Collecting %s..."),
	       Q_(packageInfo.deploymentName.c_str()));

      // ignore duplicates
      map<string, MpcPackageInfo>::const_iterator it
	= packageTable.find(packageInfo.deploymentName);
      if (it !=  packageTable.end())
	{
	  Warning (T_("%s already collected."),
		   Q_(packageInfo.deploymentName.c_str()));
	  continue;
	}
      
      // collect package
      CollectPackage (packageInfo);

      // store package
      packageTable[packageInfo.deploymentName] = packageInfo;
    }

  lister->Close ();
  lister.reset ();
}

/* _________________________________________________________________________

   PackageComposer::AssembleTDS
   _________________________________________________________________________ */

void
PackageComposer::AssembleTDS
(/*[in]*/ const map<string, MpcPackageInfo> &	packageTable,
 /*[in]*/ const PathName &			destDir,
 /*[in,out]*/ Cfg &				dbLight)
{
  for (map<string, MpcPackageInfo>::const_iterator it = packageTable.begin();
       it != packageTable.end();
       ++ it)
    {
      if (! ConsiderP(it->second))
	{
	  continue;
	}

      // assemble package
      CopyPackage (it->second, destDir);

      // update database records
      string level;
      level = GetPackageLevel(it->second);
      dbLight.PutValue (it->second.deploymentName.c_str(),
			"Level",
			level.c_str());
      dbLight.PutValue (it->second.deploymentName.c_str(),
			"MD5",
			it->second.digest.ToString().c_str());
      dbLight.PutValue (it->second.deploymentName.c_str(),
			"TimePackaged",
			NUMTOSTR(timePackaged));
      if (! it->second.version.empty())
	{
	  dbLight.PutValue (it->second.deploymentName.c_str(),
			    "Version",
			    it->second.version.c_str());
	}
    }
}

/* _________________________________________________________________________

   PackageComposer::WritePackageDefinitionFiles
   _________________________________________________________________________ */

void
PackageComposer::WritePackageDefinitionFiles
(/*[in]*/ const map<string, MpcPackageInfo> &	packageTable,
 /*[in]*/ const PathName &			destDir,
 /*[in]*/ Cfg &					dbLight)
{
  // create package definition directory
  Directory::Create (destDir);

  for (map<string, MpcPackageInfo>::const_iterator it = packageTable.begin();
       it != packageTable.end();
       ++ it)
    {
      // ignore unwanted packages
      if (! ConsiderP(it->second))
	{
	  continue;
	}

      // path to package definition file
      PathName packageDefinitionFile
	(destDir,
	 it->second.deploymentName,
	 MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);

      // remove existing package definition file
      if (File::Exists(packageDefinitionFile))
	{
	  File::Delete (packageDefinitionFile);
	}

      // write the package definition file
      string str;
      time_t timePackaged;
      if (dbLight.TryGetValue(it->second.deploymentName.c_str(),
			      "TimePackaged",
			      str))
	{
	  timePackaged = atoi(str.c_str());
	}
      else
	{
	  timePackaged = 0;
	}
      PackageManager::WritePackageDefinitionFile (packageDefinitionFile,
						  it->second,
						  timePackaged);
    }
}

/* _________________________________________________________________________

   PackageComposer::OnProcessOutput
   _________________________________________________________________________ */

bool
PackageComposer::OnProcessOutput (/*[in]*/ const void *	pOutput,
				  /*[in]*/ size_t	n)
{
  return (true);
}

/* _________________________________________________________________________

   PackageComposer::ExecuteSystemCommand
   _________________________________________________________________________ */

void
PackageComposer::ExecuteSystemCommand (/*[in]*/ const char * lpszCommand)
{
  int exitCode = 0;
  if (! Process::ExecuteSystemCommand(lpszCommand, &exitCode, this, 0)
      || exitCode != 0)
    {
      FatalError (T_("A system command failed for some reason."));
    }
}

/* _________________________________________________________________________

   PackageComposer::RunArchiver
   _________________________________________________________________________ */

void
PackageComposer::RunArchiver (/*[in]*/ ArchiveFileType	archiveFileType,
			      /*[in]*/ const PathName &	archiveFile,
			      /*[in]*/ const char *	lpszFilter)
{
  string command;
  switch (archiveFileType.Get())
    {
    case ArchiveFileType::TarBzip2:
      command = "tar -cjf \"";
      command += archiveFile.Get();
      command += "\" ";
      command += lpszFilter;
      break;
    case ArchiveFileType::TarLzma:
      command = "tar -cf - ";
      command += lpszFilter;
      command += " | lzma e -si ";
      command += "\"";
      command += archiveFile.Get();
      command += "\"";
      break;
    default:
      FatalError (T_("Unsupported archive file type."));
    }
  ExecuteSystemCommand (command.c_str());
}

/* _________________________________________________________________________

   PackageComposer::WriteNetDatabase
   _________________________________________________________________________ */

void
PackageComposer::WriteNetDatabase
(/*[in]*/ const map<string, MpcPackageInfo> &	packageTable,
 /*[in]*/ const PathName &			destDir,
 /*[in,out]*/ Cfg &				dbLight)
{
  // create database directory
  Directory::Create (destDir);

  // change into database directory
  Directory::SetCurrentDirectory (destDir);

  // find obsolete packages
  char szDeploymentName[BufferSizes::MaxPackageName];
  vector<string> obsoletePackages;
  for (const char * lpszDeploymentName =
	 dbLight.FirstKey(szDeploymentName, BufferSizes::MaxPackageName);
       lpszDeploymentName != 0;
       lpszDeploymentName =
	 dbLight.NextKey(szDeploymentName, BufferSizes::MaxPackageName))
    {
      map<string, MpcPackageInfo>::const_iterator
	it = packageTable.find(lpszDeploymentName);
      if (it == packageTable.end() || ! ConsiderP(it->second))
	{
	  obsoletePackages.push_back (lpszDeploymentName);
	}
    }

  // remove obsolete package records
  for (vector<string>::const_iterator it = obsoletePackages.begin();
       it != obsoletePackages.end();
       ++ it)
    {
      dbLight.DeleteKey (it->c_str());
    }

  // create temporary mpm.ini
  TempFile tempIni (PathName(destDir, MIKTEX_MPM_INI_FILENAME, 0));
  dbLight.Write (tempIni.GetPathName());

  // create light-weight database:
  PathName dbPath1 = GetDbLightFileName();
  RunArchiver (GetDbArchiveFileType(), dbPath1, MIKTEX_MPM_INI_FILENAME);

  // delete temporary mpm.ini
  tempIni.Delete ();

  // create package definition directory
  TempDirectory tempDir (PathName(destDir, texmfPrefix, 0));
  PathName packageDefinitionDir = tempDir.GetPathName();
  packageDefinitionDir += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
  Directory::Create (packageDefinitionDir);

  // write all package definition files
  WritePackageDefinitionFiles (packageTable, packageDefinitionDir, dbLight);

  // create heavy-weight database:
  PathName dbPath2 = GetDbHeavyFileName();
  RunArchiver (GetDbArchiveFileType(), dbPath2, texmfPrefix.c_str());

  // delete package definition files
  tempDir.Delete ();
}

/* _________________________________________________________________________

   PackageComposer::ExtractFile
   _________________________________________________________________________ */

void
PackageComposer::ExtractFile (/*[in]*/ const PathName &	archiveFile,
			      /*[in]*/ ArchiveFileType	archiveFileType,
			      /*[in]*/ const PathName &	toBeExtracted,
			      /*[in]*/ const PathName &	outFile)
{
  string command;
  switch (archiveFileType.Get())
    {
    case ArchiveFileType::MSCab:
      command = "cabextract --filter \"";
      command += toBeExtracted.Get();
      command += "\" --pipe \"";
      command += archiveFile.Get();
      command += "\" > \"";
      command += outFile.Get();
      command += "\"";
      break;
    case ArchiveFileType::TarBzip2:
      command = "tar --force-local --to-stdout -xjf \"";
      command += archiveFile.Get();
      command += "\" \"";
      command += toBeExtracted.Get();
      command += "\" > \"";
      command += outFile.Get();
      command += "\"";
      break;
    case ArchiveFileType::TarLzma:
      command = "lzma d \"";
      command += archiveFile.Get();
      command += "\" -so | tar --force-local --to-stdout -xf - \"";
      command += toBeExtracted.Get();
      command += "\" > \"";
      command += outFile.Get();
      command += "\"";
      break;
    default:
      FatalError (T_("Unsupported archive file type."));
    }
  ExecuteSystemCommand (command.c_str());
}

/* _________________________________________________________________________

   PackageComposer::CompressArchive
   _________________________________________________________________________ */

void
PackageComposer::CompressArchive (/*[in]*/ const PathName &	toBeCompressed,
				  /*[in]*/ ArchiveFileType archiveFileType,
				  /*[in]*/ const PathName &	outFile)
{
  string command;
  switch (archiveFileType.Get())
    {
    case ArchiveFileType::TarBzip2:
      command = "bzip2 --keep --compress --stdout \"";
      command += toBeCompressed.Get();
      command += "\" > \"";
      command += outFile.Get();
      command += "\"";
      break;
    case ArchiveFileType::TarLzma:
      command = "lzma e \"";
      command += toBeCompressed.Get();
      command += "\" \"";
      command += outFile.Get();
      command += "\"";
      break;
    default:
      FatalError (T_("Unsupported archive file type."));
    }
  ExecuteSystemCommand (command.c_str());
  File::Delete (toBeCompressed);
}

/* _________________________________________________________________________

   PackageComposer::CreateArchiveFile
   _________________________________________________________________________ */

ArchiveFileType
PackageComposer::CreateArchiveFile (/*[in,out]*/ MpcPackageInfo & packageInfo,
				    /*[in]*/ const PathName &	destDir)
{
  PathName archiveFile;

  bool reuseExisting = false;

  ArchiveFileType archiveFileType (ArchiveFileType::None);

  // check to see whether a cabinet file exists
  PathName archiveFile2;
  archiveFile2.Set (destDir,
		    packageInfo.deploymentName.c_str(),
		    MIKTEX_CABINET_FILE_SUFFIX);
  if (File::Exists(archiveFile2))
    {
      archiveFile = archiveFile2;
      archiveFileType = ArchiveFileType::MSCab;
    }

  // check to see whether a .tar.bz2 file exists
  archiveFile2.Set (destDir,
		    packageInfo.deploymentName.c_str(),
		    MIKTEX_TARBZIP2_FILE_SUFFIX);
  if (File::Exists(archiveFile2))
    {
      archiveFile = archiveFile2;
      archiveFileType = ArchiveFileType::TarBzip2;
    }

  // check to see whether a .tar.lzma file exists
  archiveFile2.Set (destDir,
		    packageInfo.deploymentName.c_str(),
		    MIKTEX_TARLZMA_FILE_SUFFIX);
  if (File::Exists(archiveFile2))
    {
      archiveFile = archiveFile2;
      archiveFileType = ArchiveFileType::TarLzma;
    }

  if (archiveFileType != ArchiveFileType::None)
    {
      Verbose (T_("Checking %s..."), archiveFile.Get());

      // extract the package definition file
      PathName filter (texmfPrefix);
      filter += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
      filter += packageInfo.deploymentName.c_str();
      filter.AppendExtension (MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
#if defined(MIKTEX_WINDOWS)
      filter.ToUnix ();
#endif
      TempFile packageDefinitionFile;
      ExtractFile (archiveFile,
		   archiveFileType,
		   filter, 
		   packageDefinitionFile.GetPathName());

      // read the package definition file
      PackageInfo existingPackageInfo =
	PackageManager::ReadPackageDefinitionFile
	(packageDefinitionFile.GetPathName(),
	 texmfPrefix.c_str());
      
      // check to see whether we can keep the existing file
      if (packageInfo.digest == existingPackageInfo.digest)
	{
	  reuseExisting = true;
	  packageInfo.timePackaged = existingPackageInfo.timePackaged;
	}
      else
	{
	  archiveFileType = ArchiveFileType::None;
	}
    }

  if (! reuseExisting)
    {
      archiveFileType = GetPackageArchiveFileType(packageInfo);

      PathName packageArchiveFile (packageInfo.deploymentName.c_str());
      packageArchiveFile.AppendExtension
	(ArchiveFileType::GetFileNameExtension(archiveFileType.Get()));

      Verbose (T_("Creating %s..."), packageArchiveFile.Get());

      // create destination directory
      Directory::Create (destDir);
      
      // change into package directory, e.g.:
      // /mypackages/a0poster/
      Directory::SetCurrentDirectory (packageInfo.path);
      
      // path to package definition directory, e.g.:
      // /mypackages/a0poster/Files/texmf/tpm/packages/
      PathName packageDefinitionDir (packageInfo.path);
      packageDefinitionDir += "Files";
      packageDefinitionDir += texmfPrefix;
      packageDefinitionDir += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;

      // create package definition directory
      Directory::Create (packageDefinitionDir);
      
      // path to package definition file, e.g.:
      // /mypackages/a0poster/Files/texmf/tpm/packages/a0poster.tpm
      PathName packageDefinitionFile
	(packageDefinitionDir,
	 packageInfo.deploymentName.c_str(),
	 MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
      
      // create the package definition file
      PackageManager::WritePackageDefinitionFile
	(packageDefinitionFile,
	 packageInfo,
	 timePackaged);
      
      string command;
      
      // path to .tar file
      PathName tarFile (destDir,
			packageInfo.deploymentName.c_str(),
			MIKTEX_TAR_FILE_SUFFIX);
      
      // path to compressed .tar file
      archiveFile.Set
	(destDir,
	 packageInfo.deploymentName.c_str(),
	 ArchiveFileType::GetFileNameExtension(archiveFileType.Get()));
      
#if defined(MIKTEX_WINDOWS)
      tarFile.ToUnix ();
      archiveFile.ToUnix ();
#endif
      
      // create the .tar file
      command = "tar --force-local -cf ";
      command += tarFile.ToString();
#if defined(MIKTEX_WINDOWS)
      command += " --files-from=nul";
#else
      command += " --files-from=/dev/null";
#endif
      ExecuteSystemCommand (command.c_str());
      if (Directory::Exists("Files"))
	{
	  RestoreCurrentDirectory restoreCurrentDirectory ("Files");
	  command = "tar --force-local -rf ";
	  command += tarFile.ToString();
	  command += " ";
	  command += texmfPrefix;
	  ExecuteSystemCommand (command.c_str());
	}

      // compress the tar file
      {
	RestoreCurrentDirectory restoreCurrentDirectory (destDir);
	if (File::Exists(archiveFile))
	  {
	    File::Delete (archiveFile);
	  }
	CompressArchive (tarFile, archiveFileType, archiveFile);
      }
    }

  // get size of archive file
  packageInfo.archiveFileSize = File::GetSize(archiveFile);

  // get MD5 of archive file
  packageInfo.archiveFileDigest = MD5::FromFile(archiveFile.Get());

  if (! reuseExisting)
    {
      packageInfo.timePackaged = timePackaged;

      // touch the new archive file
      File::SetTimes (archiveFile, timePackaged, timePackaged, timePackaged);
    }

  return (archiveFileType);
}

/* _________________________________________________________________________

   PackageComposer::LoadDbLight
   _________________________________________________________________________ */

bool
PackageComposer::LoadDbLight (/*[in]*/ const PathName &	directory,
			      /*[out]*/ Cfg &		cfg)
{
  // path to the light-weight database file
  PathName pathDbLight = GetDbLightFileName();
#if defined(MIKTEX_WINDOWS)
  pathDbLight.ToUnix ();
#endif

  // check to see if the database file exists
  if (! File::Exists(pathDbLight))
    {
      return (false);
    }

  // create a scratch directory
  ScratchDirectory scratchDirectory;

  // change into scratch directory
  scratchDirectory.Enter ();

  // extract mpm.ini:
  ExtractFile (pathDbLight,
	       GetDbArchiveFileType(),
	       MIKTEX_MPM_INI_FILENAME,
	       MIKTEX_MPM_INI_FILENAME);

  // parse mpm.ini
  cfg.Read (MIKTEX_MPM_INI_FILENAME);
  
  // leave & remove the scratch directory
  scratchDirectory.Leave ();

  return (true);
}

/* _________________________________________________________________________

   PackageComposer::AssembleNetPackages
   _________________________________________________________________________ */

void
PackageComposer::AssembleNetPackages
(/*[out]*/ map<string, MpcPackageInfo> &	packageTable,
 /*[in]*/ const PathName &			destDir,
 /*[out]*/ Cfg &				dbLight)
{
  for (map<string, MpcPackageInfo>::iterator it = packageTable.begin();
       it != packageTable.end();
       ++ it)
    {
      // ignore unwanted packages
      if (! ConsiderP(it->second))
	{
	  continue;
	}

      // check include list
      if (toBeIncluded.size() > 0
	  && toBeIncluded.find(it->first) == toBeIncluded.end())
	{
	  continue;
	}
      
      // ignore pure container packages
      if ((it->second.docFiles.size()
	   + it->second.sourceFiles.size())
	  == 0)
	{
	  size_t n = it->second.runFiles.size();
	  if (n == 0)
	    {
	      continue;
	    }
	  else if (n == 1)
	    {
	      PathName onlyFile (it->second.runFiles[0]);
	      if (onlyFile.HasExtension(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX))
		{
		  continue;
		}
	    }
	}

      // update level field
      string level;
      level = GetPackageLevel(it->second);
      dbLight.PutValue (it->second.deploymentName.c_str(),
			"Level",
			level.c_str());

      // get TDS digest of already existing package
      string str;
      if (dbLight.TryGetValue(it->second.deploymentName.c_str(), "md5", str))
	{
	  // don't remake archive file if there are no changes
	  if (MD5::Parse(str.c_str()) == it->second.digest)
	    {
	      Verbose (T_("%s hasn't changed => skipping"),
		       Q_(it->second.deploymentName.c_str()));
	      continue;
	    }
	}

      // create the archive file
      ArchiveFileType archiveFileType = CreateArchiveFile(it->second, destDir);

      // update database records
      dbLight.PutValue (it->second.deploymentName.c_str(),
			"MD5",
			it->second.digest.ToString().c_str());
      dbLight.PutValue (it->second.deploymentName.c_str(), 
			"TimePackaged",
			NUMTOSTR(it->second.timePackaged));
      dbLight.PutValue (it->second.deploymentName.c_str(),
			"CabSize",
			NUMTOSTR(it->second.archiveFileSize));
      dbLight.PutValue (it->second.deploymentName.c_str(),
			"CabMD5",
			it->second.archiveFileDigest.ToString().c_str());
      dbLight.PutValue (it->second.deploymentName.c_str(),
			"Type",
			(archiveFileType == ArchiveFileType::MSCab
			 ? "MSCab"
			 : (archiveFileType == ArchiveFileType::TarBzip2
			    ? "TarBzip2"
			    : (archiveFileType == ArchiveFileType::TarLzma
			       ? "TarLzma"
			       : "unknown"))));
			   
      if (! it->second.version.empty())
	{
	  dbLight.PutValue (it->second.deploymentName.c_str(),
			    "Version",
			    it->second.version.c_str());
	}
    }
}

/* _________________________________________________________________________

   PackageComposer::ReadList
   _________________________________________________________________________ */

void
PackageComposer::ReadList (/*[in]*/ const PathName &		path,
			   /*[out]*/ map<string, PackageSpec> &	mapPackageList)
{
  StreamReader reader (path);
  string line;
  while (reader.ReadLine(line))
    {
      if (line.empty())
	{
	  continue;
	}
      char ch = line[0];
      const char * lpsz = line.c_str() + 1;
      while (*lpsz != 0 && (*lpsz == ' ' || *lpsz == '\t'))
	{
	  ++ lpsz;
	}
      if (*lpsz == 0)
	{
	  continue;
	}
      if (ch == '@')
	{
	  ReadList (lpsz, mapPackageList);
	  continue;
	}
      if (ch != 'S' && ch != 'M' && ch != 'L' && ch != 'T' && ch != '-')
	{
	  continue;
	}
      Tokenizer tok (lpsz, ";");
      PackageSpec pkgspec;
      pkgspec.deploymentName = tok.GetCurrent();
      pkgspec.level = ch;
      ArchiveFileType archiveFileType = defaultArchiveFileType;
      ++ tok;
      if (tok.GetCurrent() != 0)
	{
	  if (strcmp(tok.GetCurrent(), "MSCab") == 0)
	    {
	      archiveFileType = ArchiveFileType::MSCab;
	    }
	  else if (strcmp(tok.GetCurrent(), "TarBzip2") == 0)
	    {
	      archiveFileType = ArchiveFileType::TarBzip2;
	    }
	  else if (strcmp(tok.GetCurrent(), "TarLzma") == 0)
	    {
	      archiveFileType = ArchiveFileType::TarLzma;
	    }
	  else
	    {
	      FatalError ("Invalid package list file.");
	    }
	}
      pkgspec.archiveFileType = archiveFileType;
      mapPackageList[pkgspec.deploymentName] = pkgspec;
    }
  reader.Close ();
}

/* _________________________________________________________________________

   PackageComposer::ReadList
   _________________________________________________________________________ */

void
PackageComposer::ReadList (/*[in]*/ const PathName &	path,
			   /*[out]*/ set<string> &	setPackageList)
{
  FileStream stream (File::Open(path,
				FileMode::Open,
				FileAccess::Read));
  string line;
  while (Utils::ReadUntilDelim(line, '\n', stream.Get()))
    {
      size_t l = line.length();
      if (l == 0)
	{
	  continue;
	}
      if (line[l - 1] == '\n')
	{
	  -- l;
	  line.erase (l);
	}
      if (l == 0)
	{
	  continue;
	}
      setPackageList.insert (line);
    }
  stream.Close ();
}

/* _________________________________________________________________________

   PackageComposer::DisassemblePackage
   _________________________________________________________________________ */

void
PackageComposer::DisassemblePackage
(/*[in]*/ const PathName &	packageDefinitionFile,
 /*[in]*/ const PathName &	sourceDir,
 /*[in]*/ const PathName &	destDir)
{
  // parse the package definition file
  Verbose (T_("Parsing %s..."), Q_(packageDefinitionFile.Get()));
  PackageInfo packageInfo =
    PackageManager::ReadPackageDefinitionFile(packageDefinitionFile,
					      texmfPrefix.c_str());

  // remove the package definition file from the RunFiles list
  const char * lpszRelPath
    = Utils::GetRelativizedPath(packageDefinitionFile.Get(), sourceDir.Get());
  if (lpszRelPath != 0)
    {
      vector<string>::iterator it;
      for (it = packageInfo.runFiles.begin();
	   it != packageInfo.runFiles.end();
	   ++ it)
	{
	  if (PathName::Compare(it->c_str(), lpszRelPath) == 0)
	    {
	      break;
	    }
	}
      if (it != packageInfo.runFiles.end())
	{
	  packageInfo.runFiles.erase (it);
	}
    }

  // determine the external package name, e.g.:
  // a0poster
  char szDeploymentName[BufferSizes::MaxPath];
  packageDefinitionFile.GetFileNameWithoutExtension (szDeploymentName);
  packageInfo.deploymentName = szDeploymentName;

  Verbose (" %s (%u files)...",
	   Q_(packageInfo.deploymentName.c_str()),
	   static_cast<unsigned>(packageInfo.GetNumFiles()));

  // determnine package directory, e.g.:
  // /mypackages/a0poster/
  PathName packageDir (destDir);
  packageDir += packageInfo.deploymentName;
  
  // copy files and calculate checksums; the package definition file
  // has been removed from the RunFiles list
  FileDigestTable fileDigests;
  MD5CopyFiles (packageInfo.runFiles,
		sourceDir,
		0,
		packageDir,
		"Files",
		fileDigests);
  MD5CopyFiles (packageInfo.docFiles,
		sourceDir,
		0,
		packageDir,
		"Files",
		fileDigests);
  MD5CopyFiles (packageInfo.sourceFiles,
		sourceDir,
		0,
		packageDir,
		"Files",
		fileDigests);

  // calculate TDS digest
  MD5 tdsDigest = GetTdsDigest(fileDigests);

  // write package.ini and Description, e.g.:
  // /mypackages/a0poster/package.ini
  // /mypackages/a0poster/Description
  InitializePackageDirectory (packageInfo, tdsDigest, packageDir);
  
  // write new package Definition file, e.g.:
  // /mypackages/a0poster/Files/texmf/tpm/packages/a0poster.tpm
  MpcPackageInfo mpcPackageInfo (packageInfo);
  mpcPackageInfo.digest = tdsDigest;
  mpcPackageInfo.path = packageDir;
  CollectPackage (mpcPackageInfo);
  PathName packageDefinitionDir (packageDir);
  packageDefinitionDir += "Files";
  packageDefinitionDir += texmfPrefix;
  packageDefinitionDir += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
  Directory::Create (packageDefinitionDir);
  PackageManager::WritePackageDefinitionFile
    (PathName(packageDefinitionDir,
	      packageInfo.deploymentName.c_str(),
	      MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX),
     mpcPackageInfo,
     0);
}

/* _________________________________________________________________________

   PackageComposer::Run
   _________________________________________________________________________ */

void
PackageComposer::Run (/*[in]*/ int		argc,
		      /*[in]*/ const char **	argv)
{
  vector<string> rootDirectories;

  PathName destDir;
  PathName packageDefinitionFile;
  PathName repository;
  PathName tpmDir;

  bool optAssembleDatabase = false;
  bool optAssembleNetDatabase = false;
  bool optAssembleNetPackages = false;
  bool optAssembleTDS = false;
  bool optDestDir = false;
  bool optDisassemblePackage = false;
  bool optPackageDefinitionFile = false;
  bool optRepository = false;
  bool optTpmDir = false;

  bool optVersion = false;

  optVerbose = false;

  timePackaged = time(0);

  Cpopt popt (argc, argv, options);

  int option;

  while ((option = popt.GetNextOpt()) >= 0)
    {
      const char * lpszOptArg = popt.GetOptArg();
      switch (option)
	{
	case OPT_ASSEMBLE_DATABASE:
	  optAssembleDatabase = true;
	  break;
	case OPT_ASSEMBLE_NET_DATABASE:
	  optAssembleNetDatabase = true;
	  break;
	case OPT_ASSEMBLE_NET_PACKAGES:
	  optAssembleNetPackages = true;
	  break;
	case OPT_ASSEMBLE_TDS:
	  optAssembleTDS = true;
	  break;
	case OPT_DEFAULT_LEVEL:
	  defaultLevel = lpszOptArg[0];
	  break;
	case OPT_DEST_DIR:
	  destDir = lpszOptArg;
	  optDestDir = true;
	  break;
	case OPT_DISASSEMBLE_PACKAGE:
	  optDisassemblePackage = true;
	  break;
	case OPT_INCLUDE_PACKAGE:
	  if (lpszOptArg[0] == '@')
	    {
	      ReadList (&lpszOptArg[1], toBeIncluded);
	    }
	  else
	    {
	      toBeIncluded.insert (lpszOptArg);
	    }
	  break;
	case OPT_MIKTEX_SERIES:
	  miktexSeries = lpszOptArg;
	  if (miktexSeries.CompareTo(MIKTEX_SERIES_STR) > 0)
	    {
	      FatalError (T_("Unsupported MiKTeX series."));
	    }
	  break;
	case OPT_PACKAGE_LIST:
	  ReadList (lpszOptArg, packageList);
	  break;
	case OPT_REPOSITORY:
	  repository = lpszOptArg;
	  optRepository = true;
	  break;
	case OPT_ROOT_DIR:
	  {
	    for (CSVList dir (lpszOptArg, PathName::PathNameDelimiter);
		 dir.GetCurrent() != 0;
		 ++ dir)
	      {
		rootDirectories.push_back (dir.GetCurrent());
	      }
	    break;
	  }
	case OPT_TEXMF_PREFIX:
	  texmfPrefix = lpszOptArg;
	  break;
	case OPT_TIME_PACKAGED:
	  timePackaged = atoi(lpszOptArg);
	  break;
	case OPT_TPM_DIR:
	  tpmDir = lpszOptArg;
	  optTpmDir = true;
	  break;
	case OPT_TPM_FILE:
	  packageDefinitionFile = lpszOptArg;
	  optPackageDefinitionFile = true;
	  break;
	case OPT_VERBOSE:
	  optVerbose = true;
	  break;
	case OPT_VERSION:
	  optVersion = true;
	  break;
	}
    }

  if (optVersion)
    {
      cout << Utils::MakeProgramVersionString(TheNameOfTheGame,
					      VER_FILEVERSION_STR)
	   << T_("\n\
Copyright (C) 1996-2007 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	   << endl;
      return;
    }

  if (optDisassemblePackage)
    {
      if (! optPackageDefinitionFile)
	{
	  FatalError (T_("No package definition file has been specified."));
	}
      if (rootDirectories.size() != 1)
	{
	  FatalError (T_("Exactly one root directory must be specified."));
	}
      if (! optDestDir)
	{
	  FatalError (T_("No destination directory has been specified."));
	}
      DisassemblePackage (packageDefinitionFile,
			  rootDirectories[0],
			  destDir);
    }

  if (optAssembleDatabase
      || optAssembleNetDatabase
      || optAssembleNetPackages
      || optAssembleTDS)
    {
      // collect the packages
      map<string, MpcPackageInfo> packageTable;
      for (vector<string>::const_iterator it = rootDirectories.begin();
	   it != rootDirectories.end();
	   ++ it)
	{
	  CollectPackages (*it, packageTable);
	}

      // create clean light-weight database
      SmartPointer<Cfg> pDbLight (Cfg::Create());

      if (optAssembleTDS)
	{
	  // create a TeX directory structure
	  if (! optDestDir)
	    {
	      FatalError (T_("No destination directory has been specified."));
	    }
	  AssembleTDS (packageTable, destDir, *pDbLight);
	}
      else if (optAssembleNetDatabase || optAssembleNetPackages)
	{
	  // load light-weight database
	  LoadDbLight (repository, *pDbLight);
	}

      if (optAssembleNetPackages)
	{
	  if (! optDestDir)
	    {
	      FatalError (T_("No destination directory has been specified."));
	    }
	  AssembleNetPackages (packageTable, destDir, *pDbLight);
	}

      if (optAssembleDatabase && optTpmDir)
	{
	  if (! optDestDir)
	    {
	      FatalError (T_("No database directory has been specified."));
	    }
	  WritePackageDefinitionFiles (packageTable,
				       tpmDir,
				       *pDbLight);
	}

      if (optAssembleTDS)
	{
	  // write mpm.ini, e.g.:
	  // /miktex/texmf/config/mpm.ini
	  PathName iniFile (destDir);
	  iniFile += texmfPrefix;
	  iniFile += MIKTEX_PATH_MPM_INI;
	  pDbLight->Write (iniFile);
	}

      if (optAssembleNetDatabase && optRepository)
	{
	  WriteNetDatabase (packageTable, repository, *pDbLight);
	}
    }
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
  int result;

  try
    {
      PackageComposer app;
      app.Init (argv[0]);
      app.Run (argc, argv);
      app.Finalize ();
      result = 0;
    }

  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      result = 1;
    }

  catch (const exception & e)
    {
      Utils::PrintException (e);
      result = 1;
    }

  catch (int i)
    {
      result = i;
    }

  return (result);
}
