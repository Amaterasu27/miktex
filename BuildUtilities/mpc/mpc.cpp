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
  const MIKTEXCHAR *
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
	  (MIKTEXTEXT("ArchiveFileTypeEnum::GetFileNameExtension"),
	   MIKTEXTEXT("Unknown archive file type."),
	   0,
	   MIKTEXTEXT(__FILE__),
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
  tstring	deploymentName;	// external package name
  MIKTEXCHAR	level;		// one of: '-', 'S', 'M', 'L', 'T'
  ArchiveFileType archiveFileType;
};

/* _________________________________________________________________________

   FileDigestTable

   Files and their digests.
   _________________________________________________________________________ */

struct PathNameComparer
  : public binary_function<tstring, tstring, bool>
{
  bool
  operator() (/*[in]*/ const tstring & str1,
	      /*[in]*/ const tstring & str2)
    const
  {
    return (PathName::Compare(str1.c_str(), str2.c_str()) < 0);
  }
};

typedef map<tstring, MD5, PathNameComparer> FileDigestTable;
  
/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_HELP = 300,
  OPT_ASSEMBLE_DATABASE,
  OPT_ASSEMBLE_NET_DATABASE,
  OPT_ASSEMBLE_NET_PACKAGES,
  OPT_ASSEMBLE_TDS,
  OPT_DATABASE_DIR,
  OPT_DEFAULT_LEVEL,
  OPT_DEST_DIR,
  OPT_DISASSEMBLE_PACKAGE,
  OPT_INCLUDE_PACKAGE,
  OPT_PACKAGE_LIST,
  OPT_ROOT_DIR,
  OPT_TEXMF_PREFIX,
  OPT_TIME_PACKAGED,
  OPT_TPM_FILE,
  OPT_VERBOSE,
};

/* _________________________________________________________________________

   MpcPackageInfo
   _________________________________________________________________________ */

struct MpcPackageInfo : public PackageInfo
{
  MD5			archiveDigest;
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

   Global Variables
   _________________________________________________________________________ */

// verbosity flag
bool optVerbose;

// value of "TPM:TimePackaged"
time_t timePackaged;

tstring texmfPrefix = "texmf";

// command-line options
const struct option options[] =
{
  "assemble-database", no_argument, 0, OPT_ASSEMBLE_DATABASE,
  "assemble-net-database", no_argument, 0, OPT_ASSEMBLE_NET_DATABASE,
  "assemble-net-packages", no_argument, 0, OPT_ASSEMBLE_NET_PACKAGES,
  "assemble-tds", no_argument, 0, OPT_ASSEMBLE_TDS,
  "database-dir", required_argument, 0, OPT_DATABASE_DIR,
  "default-level", required_argument, 0, OPT_DEFAULT_LEVEL,
  "dest-dir", required_argument, 0, OPT_DEST_DIR,
  "disassemble-package", no_argument, 0, OPT_DISASSEMBLE_PACKAGE,
  "help", no_argument, 0, OPT_HELP,
  "include-package", required_argument, 0, OPT_INCLUDE_PACKAGE,
  "package-list", required_argument, 0, OPT_PACKAGE_LIST,
  "root-dir", required_argument, 0, OPT_ROOT_DIR,
  "texmf-prefix", required_argument, 0, OPT_TEXMF_PREFIX,
  "time-packaged", required_argument, 0, OPT_TIME_PACKAGED,
  "tpm-file", required_argument, 0, OPT_TPM_FILE,
  "verbose", no_argument, 0, OPT_VERBOSE,
  0, no_argument, 0, 0,
};

// accumulated --package-list contents
map<tstring, PackageSpec> packageList;

// packages to be included
set<tstring> toBeIncluded;

// default package level
MIKTEXCHAR defaultLevel = 'T';

// default archive file type
ArchiveFileType defaultArchiveFileType = ArchiveFileType::TarLzma;

#if MIKTEX_SERIES_INT < 207
#  define DB_ARCHIVE_FILE_TYPE ArchiveFileType::TarBzip2
#else
#  define DB_ARCHIVE_FILE_TYPE ArchiveFileType::TarLzma
#endif

/* _________________________________________________________________________

   Help
   _________________________________________________________________________ */

void
Help ()
{
  // <todo/>
}

/* _________________________________________________________________________

   Verbose
   _________________________________________________________________________ */

void
Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	 /*[in]*/			...)
{
  if (optVerbose)
    {
      va_list arglist;
      va_start (arglist, lpszFormat);
      vprintf (lpszFormat, arglist);
      va_end (arglist);
    }
}

/* _________________________________________________________________________

   FatalError
   _________________________________________________________________________ */

void
FatalError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	    /*[in]*/			...)
{
  va_list arglist;
  fprintf (stderr, "fatal error: ");
  va_start (arglist, lpszFormat);
  vfprintf (stderr, lpszFormat, arglist);
  va_end (arglist);
  fputc ('\n', stderr);
  throw (1);
}

/* _________________________________________________________________________

   Warning
   _________________________________________________________________________ */
void
Warning (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	 /*[in]*/			...)
{
  va_list arglist;
  fprintf (stderr, "warning: ");
  va_start (arglist, lpszFormat);
  vfprintf (stderr, lpszFormat, arglist);
  va_end (arglist);
  fputc ('\n', stderr);
}

/* _________________________________________________________________________

   GetTdsDigest
   _________________________________________________________________________ */

MD5
GetTdsDigest (/*[in]*/ const FileDigestTable &	fileDigests)
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

   MD5CopyFile
   _________________________________________________________________________ */

MD5
MD5CopyFile (/*[in]*/ const PathName &	source,
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

   MD5WildCopy
   _________________________________________________________________________ */

void
MD5WildCopy (/*[in]*/ const PathName &		sourceTemplate,
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

      // don't add package definition files to the digest
      if (sourcePath.HasExtension(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX))
	{
	  continue;
	}

      // relativize file name
      const MIKTEXCHAR * lpszRelPath
	= Utils::GetRelativizedPath(destPath.Get(), prefix.Get());

      // remember MD5
      fileDigests[lpszRelPath] = digest;
    }

  pLister->Close ();
  pLister.reset ();

  if (! haveSomething)
    {
      FatalError ("no match for %s", sourceTemplate.Get());
    }
}

/* _________________________________________________________________________

   MD5CopyFiles
   _________________________________________________________________________ */

void
MD5CopyFiles (/*[in]*/ const vector<tstring> &	files,
	      /*[in]*/ const PathName &		sourceDir,
	      /*[in]*/ const MIKTEXCHAR *	lpszSourceSubDir,
	      /*[in]*/ const PathName &		destDir,
	      /*[in]*/ const MIKTEXCHAR *	lpszDestSubDir,
	      /*[in,out]*/ FileDigestTable &	fileDigests)
{
  // path to source root directory
  PathName sourceRootDir (sourceDir, lpszSourceSubDir);

  // path to destination root directory
  PathName destRootDir (destDir, lpszDestSubDir);

  for ( vector<tstring>::const_iterator it = files.begin();
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

   WriteDescriptionFile
   _________________________________________________________________________ */

void
WriteDescriptionFile (/*[in]*/ const tstring &	description,
		  /*[in]*/ const PathName &	directory)
{
  Directory::Create (directory);
  FileStream stream (File::Open(PathName(directory, "Description", 0),
				FileMode::Create,
				FileAccess::Write));
  fputs (description.c_str(), stream.Get());
  stream.Close ();
}

/* _________________________________________________________________________

   InitializePackageDirectory

   Write package.ini and Description.
   _________________________________________________________________________ */

void
InitializePackageDirectory (/*[in]*/ const PackageInfo &	packageInfo,
			    /*[in]*/ const MD5 &		digest,
			    /*[in]*/ const PathName &		packageDir)
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
  for (size_t i = 0; i < packageInfo.requiredPackages.size(); ++i)
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

   CopyPackage
   _________________________________________________________________________ */

void
CopyPackage (/*[in]*/ const MpcPackageInfo &	packageinfo,
	     /*[in]*/ const PathName &		destDir)
{
  Verbose ("copying %s ...\n", packageinfo.deploymentName.c_str());

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
		"RunFiles",
		destDir,
		0,
		fileDigests);
  MD5CopyFiles (packageinfo.docFiles,
		packageinfo.path,
		"DocFiles",
		destDir,
		0,
		fileDigests);
  MD5CopyFiles (packageinfo.sourceFiles,
		packageinfo.path,
		"SourceFiles",
		destDir,
		0,
		fileDigests);

  // check TDS digest
  if (! (GetTdsDigest(fileDigests) == packageinfo.digest))
    {
      FatalError ("checksum mismatch");
    }
}

/* _________________________________________________________________________

   ReadDescriptionFile
   _________________________________________________________________________ */

void
ReadDescriptionFile (/*[in]*/ const MIKTEXCHAR *	lpszPackageDir,
		     /*[out]*/ tstring &		description)
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
  MIKTEXCHARINT ch;
  while ((ch = fgetc(stream.Get())) != EOF)
    {
      description += static_cast<MIKTEXCHAR>(ch);
    }
  stream.Close ();
}

/* _________________________________________________________________________

   InitializePackageInfo
   _________________________________________________________________________ */

void
InitializePackageInfo (/*[in]*/ const MIKTEXCHAR *	lpszPackagePath,
		       /*[out]*/ MpcPackageInfo &	packageinfo)
{
  SmartPointer<Cfg> pcfg (Cfg::Create());

  // read package.ini
  pcfg->Read (PathName(lpszPackagePath, "package.ini", 0));

  // get deployment name (mandatory value)
  if (! pcfg->TryGetValue(0, "externalname", packageinfo.deploymentName))
    {
      FatalError ("bad package.ini (1)");
    }

  // get display name (mandatory value)
  if (! pcfg->TryGetValue(0, "name", packageinfo.displayName))
    {
      FatalError ("bad package.ini (2)");
    }

  // get creator (optional value)
  pcfg->TryGetValue (0, "creator", packageinfo.creator);

  // get title (optional value)
  pcfg->TryGetValue (0, "title", packageinfo.title);

  // get version (optional value)
  pcfg->TryGetValue (0, "version", packageinfo.version);

  // get required packages (optional value)
  tstring strReqList;
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
  tstring str;
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

   GetPackageLevel
   _________________________________________________________________________ */

MIKTEXCHAR
GetPackageLevel (/*[in]*/ const MpcPackageInfo &	packageInfo)
{
  map<string, PackageSpec>::const_iterator
    it = packageList.find(packageInfo.deploymentName);
  if (it == packageList.end())
    {
      // assume "Total MiKTeX", if the package is not listed
      return (defaultLevel);
    }
  return (it->second.level);
}

/* _________________________________________________________________________

   GetPackageArchiveFileType
   _________________________________________________________________________ */

ArchiveFileType
GetPackageArchiveFileType (/*[in]*/ const MpcPackageInfo &	packageInfo)
{
  map<string, PackageSpec>::const_iterator
    it = packageList.find(packageInfo.deploymentName);
  if (it == packageList.end())
    {
      // assume ".tar.lzma", if the package is not listed
      return (defaultArchiveFileType);
    }
  return (it->second.archiveFileType);
}

/* _________________________________________________________________________

   ConsiderP
   _________________________________________________________________________ */

bool
ConsiderP (/*[in]*/ const MpcPackageInfo &	packageInfo)
{
  return (GetPackageLevel(packageInfo) != '-');
}

/* _________________________________________________________________________

   CollectFiles
   _________________________________________________________________________ */

void
CollectFiles (/*[in]*/ const PathName &		rootDir,
	      /*[in]*/ const PathName &		subDir,
	      /*[in,out]*/ vector<tstring> &	files,
	      /*[in,out]*/ size_t &		size)
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
	  CollectFiles (rootDir, relPath, files, size);
	}
      else
	{
	  // store file name
	  files.push_back (relPath.ToString());

	  // add file size to total size
	  size += dirEntry.size;
	}
    }

  lister->Close ();
  lister.reset ();
}

/* _________________________________________________________________________

   CollectSubTree
   _________________________________________________________________________ */

void
CollectSubTree (/*[in]*/ const PathName &	path,
		/*[in]*/ const MIKTEXCHAR *	lpszSubDir,
		/*[out]*/ vector<tstring> &	files,
		/*[out]*/ size_t &		size)
{
  PathName sourceDir (path, lpszSubDir);
  CollectFiles (sourceDir, 0, files, size);
}

/* _________________________________________________________________________

   CollectPackage
   _________________________________________________________________________ */

void
CollectPackage (/*[in,out]*/ MpcPackageInfo &	packageInfo)
{
  // clear file lists
  packageInfo.sizeRunFiles = 0;
  packageInfo.runFiles.clear ();
  packageInfo.sizeDocFiles = 0;
  packageInfo.docFiles.clear ();
  packageInfo.sizeSourceFiles = 0;
  packageInfo.sourceFiles.clear ();

  // collect run-time files
  CollectSubTree (packageInfo.path,
		  "RunFiles",
		  packageInfo.runFiles,
		  packageInfo.sizeRunFiles);

  // collect document files
  CollectSubTree (packageInfo.path,
		  "DocFiles",
		  packageInfo.docFiles,
		  packageInfo.sizeDocFiles);

  // collect source files
  CollectSubTree (packageInfo.path,
		  "SourceFiles",
		  packageInfo.sourceFiles,
		  packageInfo.sizeSourceFiles);
}

/* _________________________________________________________________________

   CollectPackages
   _________________________________________________________________________ */

void
CollectPackages (/*[in]*/ const PathName &			rootDir,
		 /*[in,out]*/ map<tstring, MpcPackageInfo> &	packageTable)
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

      Verbose ("collecting %s...\n", packageInfo.deploymentName.c_str());

      // ignore duplicates
      map<tstring, MpcPackageInfo>::const_iterator it
	= packageTable.find(packageInfo.deploymentName);
      if (it !=  packageTable.end())
	{
	  Warning ("%s already collected", packageInfo.deploymentName.c_str());
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

   AssembleTDS
   _________________________________________________________________________ */

void
AssembleTDS (/*[in]*/ const map<string, MpcPackageInfo> &	packageTable,
	     /*[in]*/ const PathName &				destDir,
	     /*[in,out]*/ Cfg &					cfgDbLight)
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
      tstring level;
      level = GetPackageLevel(it->second);
      cfgDbLight.PutValue (it->second.deploymentName.c_str(),
			   "Level",
			   level.c_str());
      cfgDbLight.PutValue (it->second.deploymentName.c_str(),
			   "MD5",
			   it->second.digest.ToString().c_str());
      cfgDbLight.PutValue (it->second.deploymentName.c_str(),
			   "TimePackaged",
			   NUMTOSTR(time_t,timePackaged));
      if (! it->second.version.empty())
	{
	  cfgDbLight.PutValue (it->second.deploymentName.c_str(),
			       "Version",
			       it->second.version.c_str());
	}
    }
}

/* _________________________________________________________________________

   WritePackageDefinitionFiles
   _________________________________________________________________________ */

void
WritePackageDefinitionFiles
(/*[in]*/ const map<tstring, MpcPackageInfo> &	packageTable,
 /*[in]*/ const PathName &			destDir,
 /*[in]*/ Cfg &					cfgDbLight)
{
  // create package definition directory
  Directory::Create (destDir);

  for (map<tstring, MpcPackageInfo>::const_iterator it = packageTable.begin();
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
      tstring str;
      time_t timePackaged;
      if (cfgDbLight.TryGetValue(it->second.deploymentName.c_str(),
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

   RunArchiver
   _________________________________________________________________________ */

void
RunArchiver (/*[in]*/ ArchiveFileType		archiveFileType,
	     /*[in]*/ const PathName &		archiveFile,
	     /*[in]*/ const MIKTEXCHAR *	lpszFilter)
{
  tstring command;
  switch (archiveFileType.Get())
    {
    case ArchiveFileType::TarBzip2:
      command = "tar -cvjf \"";
      command += archiveFile.Get();
      command += "\" ";
      command += lpszFilter;
      break;
    case ArchiveFileType::TarLzma:
      command = "tar -cvf - ";
      command += lpszFilter;
      command += " | lzma e -si ";
      command += "\"";
      command += archiveFile.Get();
      command += "\"";
      break;
    }
  Verbose ("%s...\n", command.c_str());
  Process::ExecuteSystemCommand (command.c_str());
}

/* _________________________________________________________________________

   WriteNetDatabase
   _________________________________________________________________________ */

void
WriteNetDatabase (/*[in]*/ const map<tstring, MpcPackageInfo> &	packageTable,
		  /*[in]*/ const PathName &			destDir,
		  /*[in,out]*/ Cfg &				cfgDbLight)
{
  // create database directory
  Directory::Create (destDir);

  // change into database directory
  Directory::SetCurrentDirectory (destDir);

  // remove obsolete package records
  MIKTEXCHAR szExternalName[BufferSizes::MaxPackageName];
  vector<tstring> obsoletePackages;
  for (const MIKTEXCHAR * lpszExternalName =
	 cfgDbLight.FirstKey(szExternalName, BufferSizes::MaxPackageName);
       lpszExternalName != 0;
       lpszExternalName =
	 cfgDbLight.NextKey(szExternalName, BufferSizes::MaxPackageName))
    {
      map<tstring, MpcPackageInfo>::const_iterator
	it = packageTable.find(lpszExternalName);
      if (it == packageTable.end() || ! ConsiderP(it->second))
	{
	  obsoletePackages.push_back (lpszExternalName);
	}
    }
  for (vector<tstring>::const_iterator it = obsoletePackages.begin();
       it != obsoletePackages.end();
       ++ it)
    {
      cfgDbLight.DeleteKey (it->c_str());
    }

  // create temporary mpm.ini
  TempFile tempIni (PathName(destDir, MIKTEX_MPM_INI_FILENAME, 0));
  cfgDbLight.Write (tempIni.GetPathName());

  // create miktex-zzdb1.tar.bz2:
  PathName dbPath1 (MIKTEX_MPM_DB_LIGHT_FILE_NAME_NO_SUFFIX);
  dbPath1.AppendExtension (ArchiveFileType::GetFileNameExtension
			   (DB_ARCHIVE_FILE_TYPE));
  RunArchiver (DB_ARCHIVE_FILE_TYPE, dbPath1, MIKTEX_MPM_INI_FILENAME);

  // delete temporary mpm.ini
  tempIni.Delete ();

  // create package definition directory
  TempDirectory tempDir (PathName(destDir, texmfPrefix, 0));
  PathName packageDefinitionDir = tempDir.GetPathName();
  packageDefinitionDir += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
  Directory::Create (packageDefinitionDir);

  // write all package definition files
  WritePackageDefinitionFiles (packageTable, packageDefinitionDir, cfgDbLight);

  // create miktex-zzdb2.tar.bz2:
  PathName dbPath2 (MIKTEX_MPM_DB_FULL_FILE_NAME_NO_SUFFIX);
  dbPath2.AppendExtension (ArchiveFileType::GetFileNameExtension
			   (DB_ARCHIVE_FILE_TYPE));
  RunArchiver (DB_ARCHIVE_FILE_TYPE, dbPath2, texmfPrefix.c_str());

  // delete package definition files
  tempDir.Delete ();
}

/* _________________________________________________________________________

   ExtractFile
   _________________________________________________________________________ */

void
ExtractFile (/*[in]*/ const PathName &	archiveFile,
	     /*[in]*/ ArchiveFileType	archiveFileType,
	     /*[in]*/ const PathName &	toBeExtracted,
	     /*[in]*/ const PathName &	outFile)
{
  tstring command;
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
      command = "tar --force-local --to-stdout -xvjf \"";
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
      command += "\" -so | tar --force-local --to-stdout -xvf - \"";
      command += toBeExtracted.Get();
      command += "\" > \"";
      command += outFile.Get();
      command += "\"";
      break;
    }
  Verbose ("%s...\n", command.c_str());
  Process::ExecuteSystemCommand (command.c_str());
}

/* _________________________________________________________________________

   CompressArchive
   _________________________________________________________________________ */

void
CompressArchive (/*[in]*/ const PathName &	toBeCompressed,
		 /*[in]*/ ArchiveFileType	archiveFileType,
		 /*[in]*/ const PathName &	outFile)
{
  tstring command;
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
    }
  Verbose ("%s...\n", command.c_str());
  Process::ExecuteSystemCommand (command.c_str());
  File::Delete (toBeCompressed);
}

/* _________________________________________________________________________

   CreateArchiveFile
   _________________________________________________________________________ */

ArchiveFileType
CreateArchiveFile (/*[in,out]*/ MpcPackageInfo &	packageInfo,
		   /*[in]*/ const PathName &		destDir)
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

      // create package directory
      Directory::Create (destDir);
      
      // change into package directory, e.g.:
      // /mypackages/a0poster/
      Directory::SetCurrentDirectory (packageInfo.path);
      
      // path to package definition directory, e.g.:
      // /mypackages/a0poster/Runfiles/texmf/tpm/packages/
      PathName packageDefinitionDir (packageInfo.path);
      packageDefinitionDir += "RunFiles";
      packageDefinitionDir += texmfPrefix;
      packageDefinitionDir += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;

      // create package definition directory
      Directory::Create (packageDefinitionDir);
      
      // path to package definition file, e.g.:
      // /mypackages/a0poster/Runfiles/texmf/tpm/packages/a0poster.tpm
      PathName packageDefinitionFile
	(packageDefinitionDir,
	 packageInfo.deploymentName.c_str(),
	 MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
      
      // create the package definition file
      PackageManager::WritePackageDefinitionFile
	(packageDefinitionFile,
	 packageInfo,
	 timePackaged);
      
      tstring command;
      
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
      command = "tar --force-local -cvf ";
      command += tarFile.ToString();
#if defined(MIKTEX_WINDOWS)
      command += " --files-from=nul";
#else
      command += " --files-from=/dev/null";
#endif
      Verbose ("%s...\n", command.c_str());
      Process::ExecuteSystemCommand (command.c_str());
      if (Directory::Exists("RunFiles"))
	{
	  RestoreCurrentDirectory restoreCurrentDirectory ("RunFiles");
	  command = "tar --force-local -rvf ";
	  command += tarFile.ToString();
	  command += " ";
	  command += texmfPrefix;
	  Verbose ("%s...\n", command.c_str());
	  Process::ExecuteSystemCommand (command.c_str());
	}
      if (Directory::Exists("DocFiles"))
	{
	  RestoreCurrentDirectory restoreCurrentDirectory ("DocFiles");
	  command = "tar --force-local -rvf ";
	  command += tarFile.ToString();
	  command += " ";
	  command += texmfPrefix;
	  Verbose ("%s...\n", command.c_str());
	  Process::ExecuteSystemCommand (command.c_str());
	}
      if (Directory::Exists("SourceFiles"))
	{
	  RestoreCurrentDirectory restoreCurrentDirectory ("SourceFiles");
	  command = "tar --force-local -rvf ";
	  command += tarFile.ToString();
	  command += " ";
	  command += texmfPrefix;
	  Verbose ("%s...\n", command.c_str());
	  Process::ExecuteSystemCommand (command.c_str());
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
  packageInfo.archiveDigest = MD5::FromFile(archiveFile.Get());

  if (! reuseExisting)
    {
      packageInfo.timePackaged = timePackaged;

      // touch the new archive file
      File::SetTimes (archiveFile, timePackaged, timePackaged, timePackaged);
    }

  return (archiveFileType);
}

/* _________________________________________________________________________

   LoadDbLight
   _________________________________________________________________________ */

bool
LoadDbLight (/*[in]*/ const PathName &	directory,
	     /*[out]*/ Cfg &		cfg)
{
  // path to the lightweight database file
  PathName pathDbLight (directory,
			MIKTEX_MPM_DB_LIGHT_FILE_NAME_NO_SUFFIX,
			MIKTEX_TARBZIP2_FILE_SUFFIX);
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
	       DB_ARCHIVE_FILE_TYPE,
	       MIKTEX_MPM_INI_FILENAME,
	       MIKTEX_MPM_INI_FILENAME);

  // parse mpm.ini
  cfg.Read (MIKTEX_MPM_INI_FILENAME);
  
  // leave & remove the scratch directory
  scratchDirectory.Leave ();

  return (true);
}

/* _________________________________________________________________________

   AssembleNetPackages
   _________________________________________________________________________ */

void
AssembleNetPackages (/*[out]*/ map<tstring, MpcPackageInfo> &	packageTable,
		     /*[in]*/ const PathName &			destDir,
		     /*[out]*/ Cfg &				cfgDbLight)
{
  for (map<tstring, MpcPackageInfo>::iterator it = packageTable.begin();
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
      tstring level;
      level = GetPackageLevel(it->second);
      cfgDbLight.PutValue (it->second.deploymentName.c_str(),
			     "Level",
			     level.c_str());

      // get digest of already existing package
      tstring str;
      if (cfgDbLight.TryGetValue(it->second.deploymentName.c_str(),
			      "md5",
			      str))
	{
	  // don't remake archive file if there are no changes
	  if (MD5::Parse(str.c_str()) == it->second.digest)
	    {
	      Verbose ("%s hasn't changed => skipping\n",
		       it->second.deploymentName.c_str());
	      continue;
	    }
	}

      // create the archive file
      ArchiveFileType archiveFileType = CreateArchiveFile(it->second, destDir);

      // update database records
      cfgDbLight.PutValue (it->second.deploymentName.c_str(),
			   "MD5",
			   it->second.digest.ToString().c_str());
      cfgDbLight.PutValue (it->second.deploymentName.c_str(), 
			   "TimePackaged",
			   NUMTOSTR(time_t, it->second.timePackaged));
      cfgDbLight.PutValue (it->second.deploymentName.c_str(),
			   "CabSize",
			   NUMTOSTR(size_t, it->second.archiveFileSize));
      cfgDbLight.PutValue (it->second.deploymentName.c_str(),
			   "CabMD5",
			   it->second.archiveDigest.ToString().c_str());
      cfgDbLight.PutValue (it->second.deploymentName.c_str(),
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
	  cfgDbLight.PutValue (it->second.deploymentName.c_str(),
			       "Version",
			       it->second.version.c_str());
	}
    }
}

/* _________________________________________________________________________

   ReadList
   _________________________________________________________________________ */

void
ReadList (/*[in]*/ const PathName &		path,
	  /*[out]*/ map<tstring, PackageSpec> &	mapPackageList)
{
  StreamReader reader (path);
  tstring line;
  while (reader.ReadLine(line))
    {
      if (line.empty())
	{
	  continue;
	}
      MIKTEXCHAR ch = line[0];
      const MIKTEXCHAR * lpsz = line.c_str() + 1;
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
	      FatalError ("Invalid package list file");
	    }
	}
      pkgspec.archiveFileType = archiveFileType;
      mapPackageList[pkgspec.deploymentName] = pkgspec;
    }
  reader.Close ();
}

/* _________________________________________________________________________

   ReadList
   _________________________________________________________________________ */

void
ReadList (/*[in]*/ const PathName &	path,
	  /*[out]*/ set<string> &	setPackageList)
{
  FileStream stream (File::Open(path,
				FileMode::Open,
				FileAccess::Read));
  tstring line;
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

   DisassemblePackage
   _________________________________________________________________________ */

void
DisassemblePackage (/*[in]*/ const PathName &	packageDefinitionFile,
		    /*[in]*/ const PathName &	sourceDir,
		    /*[in]*/ const PathName &	destDir)
{
  // parse the package definition file
  Verbose ("Parsing %s...", packageDefinitionFile.Get());
  PackageInfo packageInfo =
    PackageManager::ReadPackageDefinitionFile(packageDefinitionFile,
					      texmfPrefix.c_str());

  // remove the package definition file from the RunFiles list
  const MIKTEXCHAR * lpszRelPath
    = Utils::GetRelativizedPath(packageDefinitionFile.Get(), sourceDir.Get());
  if (lpszRelPath != 0)
    {
      vector<tstring>::iterator it;
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
  MIKTEXCHAR szExternalName[BufferSizes::MaxPath];
  packageDefinitionFile.GetFileNameWithoutExtension (szExternalName);
  packageInfo.deploymentName = szExternalName;

  Verbose (" %s (%u files)...",
	   packageInfo.deploymentName.c_str(),
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
		"RunFiles",
		fileDigests);
  MD5CopyFiles (packageInfo.docFiles,
		sourceDir,
		0,
		packageDir,
		"DocFiles",
		fileDigests);
  MD5CopyFiles (packageInfo.sourceFiles,
		sourceDir,
		0,
		packageDir,
		"SourceFiles",
		fileDigests);

  // calculate TDS digest
  MD5 tdsDigest = GetTdsDigest(fileDigests);

  // write package.ini and Description, e.g.:
  // /mypackages/a0poster/package.ini
  // /mypackages/a0poster/Description
  InitializePackageDirectory (packageInfo, tdsDigest, packageDir);
  
  // write new package Definition file, e.g.:
  // /mypackages/a0poster/RunFiles/texmf/tpm/packages/a0poster.tpm
  MpcPackageInfo mpcPackageInfo (packageInfo);
  mpcPackageInfo.digest = tdsDigest;
  mpcPackageInfo.path = packageDir;
  CollectPackage (mpcPackageInfo);
  PathName packageDefinitionDir (packageDir);
  packageDefinitionDir += "RunFiles";
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

   Run
   _________________________________________________________________________ */

void
Run (/*[in]*/ int		argc,
     /*[in]*/ MIKTEXCHAR **	argv)
{
  vector<tstring> rootDirectories;

  PathName databaseDir;
  PathName destDir;
  PathName packageDefinitionFile;

  bool optAssembleDatabase = false;
  bool optAssembleNetDatabase = false;
  bool optAssembleNetPackages = false;
  bool optAssembleTDS = false;
  bool optDatabaseDir = false;
  bool optDisassemblePackage = false;
  bool optPackageDefinitionFile = false;
  bool optDestDir = false;

  optVerbose = false;

  timePackaged = time(0);

  int optionIndex;
  int option;
  optind = 0;

  while ((option = getopt_long(argc, argv, "", options, &optionIndex)) != EOF)
    {
      switch (static_cast<Option>(option))
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
	case OPT_DATABASE_DIR:
	  databaseDir = optarg;
	  optDatabaseDir = true;
	  break;
	case OPT_DEFAULT_LEVEL:
	  defaultLevel = optarg[0];
	  break;
	case OPT_DEST_DIR:
	  destDir = optarg;
	  optDestDir = true;
	  break;
	case OPT_DISASSEMBLE_PACKAGE:
	  optDisassemblePackage = true;
	  break;
	case OPT_HELP:
	  Help ();
	  throw (0);
	case OPT_INCLUDE_PACKAGE:
	  if (optarg[0] == '@')
	    {
	      ReadList (&optarg[1], toBeIncluded);
	    }
	  else
	    {
	      toBeIncluded.insert (optarg);
	    }
	  break;
	case OPT_PACKAGE_LIST:
	  ReadList (optarg, packageList);
	  break;
	case OPT_ROOT_DIR:
	  {
	    for (CSVList dir (optarg, PathName::PathNameDelimiter);
		 dir.GetCurrent() != 0;
		 ++ dir)
	      {
		rootDirectories.push_back (dir.GetCurrent());
	      }
	    break;
	  }
	case OPT_TEXMF_PREFIX:
	  texmfPrefix = optarg;
	  break;
	case OPT_TIME_PACKAGED:
	  timePackaged = atoi(optarg);
	  break;
	case OPT_TPM_FILE:
	  packageDefinitionFile = optarg;
	  optPackageDefinitionFile = true;
	  break;
	case OPT_VERBOSE:
	  optVerbose = true;
	  break;
	}
    }

  if (optDisassemblePackage)
    {
      if (! optPackageDefinitionFile)
	{
	  FatalError ("No package definition file has been specified.");
	}
      if (rootDirectories.size() != 1)
	{
	  FatalError ("Exactly one root directory must be specified.");
	}
      if (! optDestDir)
	{
	  FatalError ("No destination directory has been specified.");
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
      map<tstring, MpcPackageInfo> packageTable;
      for (vector<string>::const_iterator it = rootDirectories.begin();
	   it != rootDirectories.end();
	   ++ it)
	{
	  CollectPackages (*it, packageTable);
	}

      // create empty lightweight database
      SmartPointer<Cfg> pcfgDbLight (Cfg::Create());

      if (optAssembleTDS)
	{
	  // create a TeX directory structure
	  if (! optDestDir)
	    {
	      FatalError ("No destination directory has been specified.");
	    }
	  AssembleTDS (packageTable, destDir, *pcfgDbLight);
	}
      else if (optAssembleNetDatabase || optAssembleNetPackages)
	{
	  // load lightweight database
	  LoadDbLight (databaseDir, *pcfgDbLight);
	}

      if (optAssembleNetPackages)
	{
	  if (! optDestDir)
	    {
	      FatalError ("No destination directory has been specified.");
	    }
	  AssembleNetPackages (packageTable, destDir, *pcfgDbLight);
	}

      if (optAssembleDatabase && optDatabaseDir)
	{
	  if (! optDestDir)
	    {
	      FatalError ("No database directory has been specified.");
	    }
	  WritePackageDefinitionFiles (packageTable,
				       databaseDir,
				       *pcfgDbLight);
	}

      if (optAssembleTDS)
	{
	  // write mpm.ini, e.g.:
	  // /miktex/texmf/config/mpm.ini
	  PathName iniFile (destDir);
	  iniFile += texmfPrefix;
	  iniFile += MIKTEX_PATH_MPM_INI;
	  pcfgDbLight->Write (iniFile);
	}

      if (optAssembleNetDatabase && optDatabaseDir)
	{
	  WriteNetDatabase (packageTable, databaseDir, *pcfgDbLight);
	}
    }
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int		argc,
      /*[in]*/ MIKTEXCHAR **	argv)
{
  int result;

  try
    {
      SessionWrapper pSession;
      Session::InitInfo initInfo;
      initInfo.SetProgramInvocationName (argv[0]);
      pSession.CreateSession (initInfo);
      Run (argc, argv);
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
