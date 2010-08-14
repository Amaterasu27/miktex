/* searchpath.cpp: managing search paths

   Copyright (C) 1996-2010 Christian Schenk

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

/* _________________________________________________________________________

   SessionImpl::ExpandRootDirectories
   _________________________________________________________________________ */

void
SessionImpl::ExpandRootDirectories (/*[in]*/ const char * lpszToBeExpanded,
                                    /*[in,out*/ PathNameArray & paths)
{
  if (lpszToBeExpanded[0] == '%' && (lpszToBeExpanded[1] == 'R' || lpszToBeExpanded[1] == 'r'))
  {
    const char * lpszSuffix = lpszToBeExpanded + 2;
    if (IsDirectoryDelimiter(*lpszSuffix))
    {
      ++ lpszSuffix;
    }
    for (unsigned idx = 0; idx < GetNumberOfTEXMFRoots(); ++ idx)
    {
      PathName path2 = GetRootDirectory(idx);
      path2.AppendDirectoryDelimiter ();
      path2.Append (lpszSuffix, false);
      paths.push_back (path2);
    }
    if (lpszToBeExpanded[1] == 'R')
    {
      PathName path2 = MPM_ROOT_PATH;
      path2.AppendDirectoryDelimiter ();
      path2.Append (lpszSuffix, false);
      paths.push_back (path2);
    }
  }
  else
  {
    paths.push_back (lpszToBeExpanded);
  }
}

/* _________________________________________________________________________

   SessionImpl::ExpandRootDirectories
   _________________________________________________________________________ */

PathNameArray
SessionImpl::ExpandRootDirectories (/*[in]*/ const char * lpszToBeExpanded)
{
  PathNameArray result;
  for (CSVList path (lpszToBeExpanded, PATH_DELIMITER);
       path.GetCurrent() != 0;
       ++ path)
  {
    ExpandRootDirectories (path.GetCurrent(), result);
  }
  return (result);
}

/* _________________________________________________________________________

   SessionImpl::PushBackPath
   _________________________________________________________________________ */

void
SessionImpl::PushBackPath (/*[in,out]*/ PathNameArray &	vec,
			   /*[in]*/ const PathName &	path)
{
  PathNameArray paths = ExpandBraces(path.Get());

  for (PathNameArray::const_iterator it = paths.begin(); it != paths.end(); ++ it)
  {
    // expand '~'
    if ((*it)[0] == '~' && ((*it)[1] == 0 || IsDirectoryDelimiter((*it)[1])))
    {
      PathName pathFQ = GetHomeDirectory();
      if (! Utils::IsAbsolutePath(pathFQ.Get()))
      {
	TraceError (T_("cannot expand ~: %s is not fully qualified"),
	  Q_(pathFQ));
	continue;
      }
      if ((*it)[1] != 0 && IsDirectoryDelimiter((*it)[1]) && (*it)[2] != 0)
      {
	pathFQ += &(*it)[2];
      }
      if (find(vec.begin(), vec.end(), pathFQ) == vec.end())
      {
	vec.push_back (pathFQ);
      }
      continue;
    }

    // fully qualified path?
    if (Utils::IsAbsolutePath((*it).Get()))
    {
      if (find(vec.begin(), vec.end(), (*it)) == vec.end())
      {
	vec.push_back ((*it));
      }
      continue;
    }

    // it is a relative path
    PathName pathFQ;
    for (unsigned idx = 0; GetWorkingDirectory(idx, pathFQ); ++ idx)
    {
      if (! Utils::IsAbsolutePath(pathFQ.Get()))
      {
	TraceError (T_("%s is not fully qualified"), Q_(pathFQ));
	continue;
      }
      if (PathName::Compare((*it), CURRENT_DIRECTORY) != 0)
      {
	pathFQ += (*it).Get();
      }
      if (find(vec.begin(), vec.end(), pathFQ) == vec.end())
      {
	vec.push_back (pathFQ);
      }
    }
  }
}

/* _________________________________________________________________________

   SessionImpl::SplitSearchPath
   _________________________________________________________________________ */

void
SessionImpl::SplitSearchPath (/*[in,out]*/ PathNameArray &	vec,
			      /*[in]*/ const char *		lpszSearchPath)
{
  for (CSVList path (lpszSearchPath, PATH_DELIMITER);
       path.GetCurrent() != 0;
       ++ path)
    {
      PushBackPath (vec, path.GetCurrent());
    }
}

/* _________________________________________________________________________

   SessionImpl::SplitSearchPath
   _________________________________________________________________________ */

PathNameArray
SessionImpl::SplitSearchPath (/*[in]*/ const char *	lpszSearchPath)
{
  PathNameArray vec;
  SplitSearchPath (vec, lpszSearchPath);
  return (vec);
}

/* _________________________________________________________________________

   MakeSearchPath
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(string)
MakeSearchPath (/*[in]*/ const PathNameArray &	vec)
{
  string searchPath;
  for (PathNameArray::const_iterator it = vec.begin();
       it != vec.end();
       ++ it)
    {
      if (! searchPath.empty())
	{
	  searchPath += PATH_DELIMITER;
	}
      searchPath += it->Get();
    }
  return (searchPath);
}

/* _________________________________________________________________________

   SessionImpl::AppendToSearchPath
   _________________________________________________________________________ */

void
SessionImpl::AppendToSearchPath (/*[in,out]*/ string &		searchPath,
				 /*[in]*/ const string &	searchPath2)
{
  PathNameArray vec = SplitSearchPath(searchPath.c_str());

  for (CSVList path (searchPath2.c_str(), PATH_DELIMITER);
       path.GetCurrent() != 0;
       ++ path)
    {
      PushBackPath (vec, path.GetCurrent());
    }

  searchPath = MakeSearchPath(vec);
}

/* _________________________________________________________________________

   SessionImpl::TraceSearchVector
   _________________________________________________________________________ */

void
SessionImpl::TraceSearchVector (/*[in]*/ const char *		lpszKey,
				/*[in]*/ const PathNameArray &	vec)
{
  if (! trace_filesearch->IsEnabled())
    {
      return;
    }
  trace_filesearch->WriteFormattedLine
    ("core",
     T_("search vector %s:"),
     lpszKey);
  unsigned nr = 0;
  for (PathNameArray::const_iterator it = vec.begin();
       it != vec.end();
       ++ it, ++ nr)
    {
      trace_filesearch->WriteFormattedLine
	("core",
	 T_("  %2u: %s"),
	 nr,
	 it->Get());
    }
}

/* _________________________________________________________________________

   SessionImpl::ConstructSearchVector
   _________________________________________________________________________ */

PathNameArray
SessionImpl::ConstructSearchVector (/*[in]*/ FileType fileType)
{
  InternalFileTypeInfo * pfti = GetInternalFileTypeInfo(fileType);

  if (pfti->searchVec.size() == 0)
    {
      pfti->searchVec = SplitSearchPath(pfti->searchPath.c_str());
      
      // environment string
      for (CSVList env (pfti->envVarNames.c_str(), PATH_DELIMITER);
	   env.GetCurrent() != 0;
	   ++ env)
	{
	  string searchPath;
	  if (Utils::GetEnvironmentString(env.GetCurrent(), searchPath))
	    {
	      SplitSearchPath (pfti->searchVec, searchPath.c_str());
	    }
	}
      
      TraceSearchVector (pfti->fileTypeString.c_str(), pfti->searchVec);
    }

  return (pfti->searchVec);
}

/* _________________________________________________________________________

   SessionImpl::GetExpandedSearchPath
   _________________________________________________________________________ */

string
SessionImpl::GetExpandedSearchPath (/*[in]*/ FileType	fileType)
{
  MIKTEX_ASSERT (fileType != FileType::None);
  return (MakeSearchPath(ConstructSearchVector(fileType)));
}

/* _________________________________________________________________________

   SessionImpl::DirectoryWalk
   _________________________________________________________________________ */

void
SessionImpl::DirectoryWalk (/*[in]*/ const PathName &	  directory,
			    /*[in]*/ const PathName &	  pathPattern,
			    /*[in,out]*/ PathNameArray &  paths)
{
  if (pathPattern.Empty())
  {
    paths.push_back (directory);
  }
  else
  {
    ExpandPathPattern (directory, pathPattern, paths);
  }
  auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(directory, 0, DirectoryLister::Options::DirectoriesOnly));
  DirectoryEntry entry;
  PathNameArray subdirs;
  while (pLister->GetNext(entry))
  {
    MIKTEX_ASSERT (entry.isDirectory);
    PathName subdir (directory);
    subdir += entry.name;
    subdirs.push_back (subdir);
  }
  pLister->Close ();
  for (PathNameArray::const_iterator it = subdirs.begin(); it != subdirs.end(); ++ it)
  {
    if (! pathPattern.Empty())
    {
      ExpandPathPattern (*it, pathPattern, paths);
    }
    DirectoryWalk (*it, pathPattern, paths);
  }
}

/* _________________________________________________________________________

   SessionImpl::ExpandPathPattern

   Collect all directories matching a path pattern relative to a root
   directory.
   _________________________________________________________________________ */

void
SessionImpl::ExpandPathPattern (/*[in]*/ const PathName &     rootDirectory,
				/*[in]*/ const PathName &     pathPattern,
				/*[in,out]*/ PathNameArray &  paths)
{
  MIKTEX_ASSERT (! pathPattern.Empty());
  const char * lpszRecursionIndicator = strstr(pathPattern.Get(), RECURSION_INDICATOR);
  if (lpszRecursionIndicator == 0)
  {
    // no recursion; check to see whether the path pattern specifies an
    // existing sub directory
    PathName directory (rootDirectory);
    directory += pathPattern;
    if (Directory::Exists(directory))
    {
      paths.push_back (directory);
    }
  }
  else
  {
    // recursion; decompose the path pattern into two parts:
    // (1) sub directory (2) smaller (possibly empty) path pattern
    string subDir (pathPattern.Get(), lpszRecursionIndicator - pathPattern.Get());
    const char * lpszSmallerPathPattern = lpszRecursionIndicator + RECURSION_INDICATOR_LENGTH;
    for (; IsDirectoryDelimiter(*lpszSmallerPathPattern); ++ lpszSmallerPathPattern) {};
    PathName directory (rootDirectory);
    directory += subDir;
    // check to see whether the sub directory exists
    if (Directory::Exists(directory))
    {
      DirectoryWalk (directory, lpszSmallerPathPattern, paths);
    }
  }
}

/* _________________________________________________________________________

   SessionImpl::ExpandPathPatterns
   _________________________________________________________________________ */

PathNameArray
SessionImpl::ExpandPathPatterns (/*[in]*/ const char * lpszToBeExpanded)
{
  PathNameArray pathPatterns = SplitSearchPath(lpszToBeExpanded);
  PathNameArray paths;
  for (PathNameArray::const_iterator it = pathPatterns.begin();
       it != pathPatterns.end();
       ++ it)
  {
    SearchPathDictionary::const_iterator it2 =
      expandedPathPatterns.find(it->Get());
    if (it2 == expandedPathPatterns.end())
    {
      PathNameArray paths2;
      ExpandPathPattern ("", *it, paths2);
      expandedPathPatterns[it->Get()] = paths2;
      paths.insert (paths.end(), paths2.begin(), paths2.end());
    }
    else
    {
      paths.insert (paths.end(), it2->second.begin(), it2->second.end());
    }
  }
  return (paths);
}

/* _________________________________________________________________________

   SessionImpl::ExpandBraces
   _________________________________________________________________________ */

inline
void
Combine (/*[in,out]*/ PathNameArray &	  paths,
         /*[in]*/ const PathNameArray &	  toBeAppended)
{
  if (toBeAppended.empty())
  {
    return;
  }
  if (paths.empty())
  {
    paths = toBeAppended;
    return;
  }
  PathNameArray result;
  result.reserve (paths.size() * toBeAppended.size());
  for (PathNameArray::const_iterator it = paths.begin();
    it != paths.end();
    ++ it)
  {
    for (PathNameArray::const_iterator it2 = toBeAppended.begin();
      it2 != toBeAppended.end();
      ++ it2)
    {
      PathName path (*it);
      path.Append (it2->Get(), false);
      result.push_back (path);
    }
  }
  paths = result;
}

inline
void
Combine (/*[in,out]*/ PathNameArray &	  paths,
         /*[in]*/ const string &	  path)
{
  PathNameArray toBeAppended;
  toBeAppended.push_back (path);
  Combine (paths, toBeAppended);
}

inline
PathNameArray
ExpandBracesHelper (/*[in]*/ const char * & lpszToBeExpanded)
{
  MIKTEX_ASSERT (*lpszToBeExpanded == '{');
  ++ lpszToBeExpanded;
  string str;
  PathNameArray result;
  PathNameArray subtotal;
  for (; *lpszToBeExpanded != 0 && *lpszToBeExpanded != '}'; ++ lpszToBeExpanded)
  {
    if (*lpszToBeExpanded == '{')
    {
      Combine (subtotal, str);
      str = "";
      Combine (subtotal, ExpandBracesHelper(lpszToBeExpanded));
      if (*lpszToBeExpanded != '}')
      {
	// todo
      }
    }
    else if (*lpszToBeExpanded == ',')
    {
      Combine (subtotal, str);
      str = "";
      result.insert (result.end(), subtotal.begin(), subtotal.end());
      subtotal.clear ();
    }
    else
    {
      str += *lpszToBeExpanded;
    }
  }
  Combine (subtotal, str);
  result.insert (result.end(), subtotal.begin(), subtotal.end());
  return (result);
}

// ab{cd,ef}gh{ij,kl}mn =>
// abcdghijmn
// abcdghklmn
// abefghijmn
// abefghklmn

void
SessionImpl::ExpandBraces (/*[in]*/ const char * lpszToBeExpanded,
			   /*[in,out]*/ PathNameArray & paths)
{
  string str;
  PathNameArray result;
  for (; *lpszToBeExpanded != 0; ++ lpszToBeExpanded)
  {
    if (*lpszToBeExpanded == '{')
    {
      Combine (result, str);
      str = "";
      Combine (result, ExpandBracesHelper(lpszToBeExpanded));
      if (*lpszToBeExpanded != '}')
      {
	// todo
      }
    }
    else
    {
      str += *lpszToBeExpanded;
    }
  }
  Combine (result, str);
  paths.insert (paths.end(), result.begin(), result.end());
}

/* _________________________________________________________________________

   SessionImpl::ExpandBraces
   _________________________________________________________________________ */

PathNameArray
SessionImpl::ExpandBraces (/*[in]*/ const char * lpszToBeExpanded)
{
  PathNameArray paths = ExpandRootDirectories(lpszToBeExpanded);
  PathNameArray result;
  for (PathNameArray::const_iterator it = paths.begin(); it != paths.end(); ++ it)
  {
    ExpandBraces (it->Get(), result);
  }
  return (result);
}
