/* LogFile.cpp:

   Copyright (C) 2000-2006 Christian Schenk

   This file is part of the Remove MiKTeX! Wizard.

   The Remove MiKTeX! Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The Remove MiKTeX! Wizard is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the Remove MiKTeX! Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "LogFile.h"
#include "RemoveFilesPage.h"

/* _________________________________________________________________________

   LogFile::LogFile
   _________________________________________________________________________ */

LogFile::LogFile (/*[in]*/ RemoveFilesPage * pDlg)
  : pDlg (pDlg)
{
}

/* _________________________________________________________________________

   SkipPrefix
   _________________________________________________________________________ */

bool
SkipPrefix (/*[in]*/ const string &	str,
	    /*[in]*/ const char *	lpszPrefix,
	    /*[out]*/ string &		str2)
{
  size_t n = StrLen(lpszPrefix);
  if (str.compare(0, n, lpszPrefix) != 0)
    {
      return (false);
    }
  str2 = str.c_str() + n;
  return (true);
}


/* _________________________________________________________________________

   LogFile::AddFile
   _________________________________________________________________________ */

void
LogFile::AddFile (/*[in]*/ const PathName & path)
{
  string fileName;
  if (SkipPrefix(path.Get(), T_("texmf/"), fileName)
      || SkipPrefix(path.Get(), T_("texmf\\"), fileName)
      || SkipPrefix(path.Get(), T_("./texmf/"), fileName)
      || SkipPrefix(path.Get(), T_(".\\texmf\\"), fileName))
    {
      PathName absPath
	(SessionWrapper(true)->GetSpecialPath(SpecialPath::InstallRoot));
      absPath += fileName;
      files.insert (absPath);
    }
}

/* _________________________________________________________________________

   LogFile::AddPackages
   _________________________________________________________________________ */

void
LogFile::AddPackages ()
{
  // add packages.ini
  AddFile (PathName(T_("texmf"), MIKTEX_PATH_PACKAGES_INI));

  // iterate over all known packages
  PackageInfo pi;
  PackageManagerPtr pManager (PackageManager::Create());
  auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
  while (pIter->GetNext(pi))
    {
      // add .tpm file
      PathName tpmFile (T_("texmf"), MIKTEX_PATH_PACKAGE_DEFINITION_DIR);
      tpmFile += pi.deploymentName;
      tpmFile.AppendExtension (MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
      AddFile (tpmFile);
	       
      // check if the package is installed
      if (! pi.IsInstalled())
	{
	  continue;
	}

      // add files to the file set
      vector<string>::const_iterator it;
      for (it = pi.runFiles.begin();
	   it != pi.runFiles.end();
	   ++it)
	{
	  AddFile (*it);
	}
      for (it = pi.docFiles.begin();
	   it != pi.docFiles.end();
	   ++it)
	{
	  AddFile (*it);
	}
      for (it = pi.sourceFiles.begin();
	   it != pi.sourceFiles.end();
	   ++it)
	{
	  AddFile (*it);
	}
    }
  pIter->Dispose ();
}

/* _________________________________________________________________________

   LogFile::ReadLogFile
   _________________________________________________________________________ */

void
LogFile::ReadLogFile ()
{
  // open the log file
  StreamReader stream (logFileName);

  // read lines
  string line;
  enum Section { None, Files, HKCU, HKLM };
  Section section = None;
  while (stream.ReadLine(line) && ! pDlg->GetCancelFlag())
    {
      size_t cchLine = line.length();
      if (cchLine == 0)
	{
	  continue;
	}
      if (line[cchLine - 1] == '\n')
	{
	  line[--cchLine] = 0;
	}
      if (line == T_("[files]"))
	{
	  section = Files;
	  continue;
	}
      else if (line == T_("[hkcu]"))
	{
	  section = HKCU;
	  continue;
	}
      else if (line == "[hklm]")
	{
	  section = HKLM;
	  continue;
	}
      switch (section)
	{
	case None:
	  break;
	case Files:
	  files.insert (line);
	  break;
	case HKCU:
	case HKLM:
	  {
	    Tokenizer tok (line.c_str(), ";");
	    if (tok.GetCurrent() == 0)
	      {
		continue;
	      }
	    RegValue regvalue;
	    regvalue.hkey = (section == HKCU
			     ? HKEY_CURRENT_USER
			     : HKEY_LOCAL_MACHINE);
	    regvalue.strSubKey = tok.GetCurrent();
	    ++ tok;
	    if (tok.GetCurrent() == 0)
	      {
		continue;
	      }
	    regvalue.strValueName = tok.GetCurrent();
	    regValues.push_back (regvalue);
	    break;
	  }
	}
    }

  stream.Close ();
}

/* _________________________________________________________________________

   LogFile::Process
   _________________________________________________________________________ */

void
LogFile::Process ()
{
  AddPackages ();
  ReadLogFile ();
  if (pDlg->GetCancelFlag())
    {
      return;
    }
  RemoveFiles ();
  if (pDlg->GetCancelFlag())
    {
      return;
    }
  RemoveRegistrySettings ();
  if (pDlg->GetCancelFlag())
    {
      return;
    }
}

/* _________________________________________________________________________

   LogFile::RemoveFiles
   _________________________________________________________________________ */

void
LogFile::RemoveFiles ()
{
  set<PathName> directories;
  size_t n = 0;
  for (set<PathName>::const_iterator it = files.begin();
       it != files.end() && ! pDlg->GetCancelFlag();
       ++ it, ++ n)
    {
      PathName pathCurDir = *it;
      pathCurDir.RemoveFileSpec ();
      directories.insert (pathCurDir);
      if (File::Exists(*it))
	{
	  File::Delete (*it, true);
	}
      pDlg->Progress (*it,
		      static_cast<ULONG>(n),
		      static_cast<ULONG>(files.size()));
    }
  for (set<PathName>::const_iterator it = directories.begin();
       it != directories.end() && ! pDlg->GetCancelFlag();
       ++it)
    {
      if (Directory::Exists(*it))
	{
	  RemoveEmptyDirectoryChain (*it);
	}
    }
}

/* _________________________________________________________________________

   LogFile::RemoveRegistrySettings
   _________________________________________________________________________ */

void
LogFile::RemoveRegistrySettings ()
{
  // <code>sort (regValues.begin(), regValues.end());</code>
  for (vector<RegValue>::const_iterator it = regValues.begin();
       it != regValues.end() && ! pDlg->GetCancelFlag();
       ++ it)
    {
      HKEY hkeySub;
      if (RegOpenKeyEx(it->hkey, it->strSubKey, 0, KEY_ALL_ACCESS, &hkeySub)
	  != ERROR_SUCCESS)
	{
	  continue;
	}
      RegDeleteValue (hkeySub, it->strValueName);
      char szName[BufferSizes::MaxPath];
      DWORD sizeName = BufferSizes::MaxPath;
      FILETIME fileTime;
      bool del =
	(RegEnumKeyEx(hkeySub,
		      0,
		      szName,
		      &sizeName,
		      0,
		      0,
		      0,
		      &fileTime)
	 == ERROR_NO_MORE_ITEMS);
      RegCloseKey (hkeySub);
      if (del)
	{
	  RegDeleteKey (it->hkey, it->strSubKey);
	}
    }
}
