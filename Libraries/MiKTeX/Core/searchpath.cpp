/* searchpath.cpp: managing search paths

   Copyright (C) 1996-2007 Christian Schenk

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

   SessionImpl::PushBackPath
   _________________________________________________________________________ */

void
SessionImpl::PushBackPath (/*[in,out]*/ PathNameArray &	vec,
			   /*[in]*/ const PathName &	path)
{
  // expand root directories
  if (path[0] == '%' && (path[1] == 'R' || path[1] == 'r'))
    {
      for (unsigned idx = 0; idx < GetNumberOfTEXMFRoots(); ++ idx)
	{
	  PathName path2 = GetRootDirectory(idx);
	  path2.Append (path.Get() + 2, false);
	  // <recursivecall>
	  PushBackPath (vec, path2);
	  // </recursivecall>
	}
      if (path[1] == 'R')
	{
	  PathName path2 = MPM_ROOT_PATH;
	  path2.Append (path.Get() + 2, false);
	  // <recursivecall>
	  PushBackPath (vec, path2);
	  // </recursivecall>
	}
      return;
    }

  // expand '~'
  if (path[0] == '~' && (path[1] == 0 || IsDirectoryDelimiter(path[1])))
    {
      PathName pathFQ = GetHomeDirectory();
      if (! Utils::IsAbsolutePath(pathFQ.Get()))
	{
	  TraceError (T_("cannot expand ~: %s is not fully qualified"),
		      Q_(pathFQ));
	  return;
	}
      if (path[1] != 0 && IsDirectoryDelimiter(path[1]) && path[2] != 0)
	{
	  pathFQ += &path[2];
	}
      if (find(vec.begin(), vec.end(), pathFQ) == vec.end())
	{
	  vec.push_back (pathFQ);
	}
      return;
    }
  
  // fully qualified path?
  if (Utils::IsAbsolutePath(path.Get()))
    {
      if (find(vec.begin(), vec.end(), path) == vec.end())
	{
	  vec.push_back (path);
	}
      return;
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
      if (PathName::Compare(path, CURRENT_DIRECTORY) != 0)
	{
	  pathFQ += path.Get();
	}
      if (find(vec.begin(), vec.end(), pathFQ) == vec.end())
	{
	  vec.push_back (pathFQ);
	}
    }
}

/* _________________________________________________________________________

   SessionImpl::SplitSearchPath
   _________________________________________________________________________ */

void
SessionImpl::SplitSearchPath (/*[in,out]*/ PathNameArray &	vec,
			      /*[in]*/ const char *	lpszSearchPath)
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

MIKTEXSTATICFUNC(string)
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
SessionImpl::TraceSearchVector (/*[in]*/ const char *	lpszKey,
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
  InternalFileTypeInfo * pfti = GetFileTypeInfo(fileType);

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
