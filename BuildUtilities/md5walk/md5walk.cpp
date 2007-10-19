/* md5walk.cpp: calculate the MD5 of a file tree

   Copyright (C) 2001-2007 Christian Schenk

   This file is part of MD5Walk.

   MD5Walk is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MD5Walk is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MD5Walk; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <MiKTeX/Core/Core>
#include <popt-miktex.h>
#include "md5walk-version.h"

#if defined(MIKTEX_WINDOWS) && defined(_MSC_VER)
#include <io.h>
#include <conio.h>
#include <fcntl.h>
#include <windows.h>
#endif

#if defined(_MSC_VER)
#  pragma warning (push, 1)
#  pragma warning (disable: 4702)
#endif

#if defined(_MSC_VER)
#  include <hash_map>
#endif

#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>

#if defined(_MSC_VER)
  #pragma warning (pop)
#endif

using namespace MiKTeX::Core;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

/* _________________________________________________________________________

   Special Names
   _________________________________________________________________________ */

const char * MD5WALK_FILE = ".nvsbl";
const char * NAME_CHECK_INTEGRITY = T_("chkdata");

/* _________________________________________________________________________

   MakeLong
   _________________________________________________________________________ */

inline
long
MakeLong (/*[in]*/ unsigned a,
	  /*[in]*/ unsigned b)
{
  unsigned long ul = (a & 0xffff);
  ul |= static_cast<unsigned long>(b & 0xffff) << 16;
  return (static_cast<long>(ul));
}

/* _________________________________________________________________________

   MakeWord
   _________________________________________________________________________ */

inline
unsigned
MakeWord (/*[in]*/ unsigned char a,
	  /*[in]*/ unsigned char b)
{
  unsigned u = (a & 0xff);
  u |= static_cast<unsigned>(b & 0xff) << 8;
  return (static_cast<long>(u));
}

/* _________________________________________________________________________

   hash_compare_md5sum
   _________________________________________________________________________ */

class hash_compare_md5sum
{
public:
  enum
    {
      bucket_size = 4,
      min_buckets = 8
    };
  
public:
  size_t
  operator() (/*[in]*/ const MD5 & md5)
    const
  {
    return (MakeLong(MakeWord(md5.GetBits()[0],
			      md5.GetBits()[1]),
		     MakeWord(md5.GetBits()[2],
			      md5.GetBits()[3])));
  }
  
public:
  bool
  operator() (/*[in]*/ const MD5 & md5_1,
	      /*[in]*/ const MD5 & md5_2)
    const
  {
    return (memcmp(md5_1.GetBits(), md5_2.GetBits(), sizeof(md5_1)) < 0);
  }
};

/* _________________________________________________________________________

   TASK
   _________________________________________________________________________ */

enum TASK { None, Check, ComputeDigest, FindDuplicates };

/* _________________________________________________________________________

   Exit Codes
   _________________________________________________________________________ */

const int SUCCESS = 0;
const int FATAL_ERROR = 1;
const int CANNOT_CHECK_INTEGRITY = 2;
const int NOT_OF_INTEGRITY = 3;

/* _________________________________________________________________________

   Global Variables
   _________________________________________________________________________ */

typedef map<string, MD5, hash_compare_icase> FileNameToMD5;
typedef multimap<size_t, string> SizeToFileName;
#if defined(_MSC_VER)
typedef stdext::hash_multimap<MD5, string, hash_compare_md5sum> MD5ToFileName;
#else
typedef multimap<MD5, string, hash_compare_md5sum> MD5ToFileName;
#endif

set<string> ignoreExtensions;
int optVerbose = false;
int optBinary = false;
#if defined(MIKTEX_WINDOWS)
int optPauseWhenFinished = false;
#endif

/* _________________________________________________________________________

   Verbose
   _________________________________________________________________________ */

void
Verbose (/*[in]*/ const char *	lpszFormat,
	 /*[in]*/			...)
{
  if (! optVerbose)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1000,
  OPT_CHECK,
  OPT_COMPUTE_DIGEST,
  OPT_EXCLUDE,
  OPT_FIND_DUPLICATES,
  OPT_VERSION,
};

/* _________________________________________________________________________

   aoption
   _________________________________________________________________________ */

const struct poptOption aoption[] = {
  {
    T_("binary"), 0, POPT_ARG_VAL, &optBinary, true,
    T_("Print binary MD5."), 0
  },
  {
    T_("check"), 0, POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0, OPT_CHECK,
    T_("Check the data integrity."), 0
  },
  {
    T_("compute-digest"), 0, POPT_ARG_NONE, 0, OPT_COMPUTE_DIGEST,
    T_("Compute the MD5."), 0,
  },
  {
    T_("exclude"), 0, POPT_ARG_STRING, 0, OPT_EXCLUDE,
    T_("Files (*.EXT) to be excluded."),
    T_(".EXT")
  },
  {
    T_("find-duplicates"), 0, POPT_ARG_NONE, 0, OPT_FIND_DUPLICATES,
    T_("Find duplicates."), 0
  },
  {
    T_("verbose"), 0, POPT_ARG_VAL, &optVerbose, true,
    T_("Print information about what is being done."), 0
  },
  {
    T_("version"), 0, POPT_ARG_NONE, 0, OPT_VERSION,
    T_("Show version information and exit."), 0
  },
  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   FatalError
   _________________________________________________________________________ */

void
FatalError (/*[in]*/ const char *	lpszFormat,
	    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << Utils::GetExeName() << ": "
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
  throw (FATAL_ERROR);
}

/* _________________________________________________________________________

   DirectoryWalk
   _________________________________________________________________________ */

void
DirectoryWalk (/*[in]*/ TASK			task,
	       /*[in]*/ const PathName &	path,
	       /*[in]*/ const char *	lpszStripPrefix,
	       /*[in,out]*/ FileNameToMD5 &	mapFnToMD5,
	       /*[in,out]*/ SizeToFileName &	mapSizeToFn)
{
  auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(path));
  DirectoryEntry2 dirEntry;
  while (pLister->GetNext(dirEntry))
    {
      PathName path2 (path);
      path2 += dirEntry.name;
      if (dirEntry.isDirectory)
	{
	  DirectoryWalk (task,
			 path2,
			 lpszStripPrefix,
			 mapFnToMD5,
			 mapSizeToFn);
	}
      else
	{
	  const char * lpszExt = path2.GetExtension();
	  if (lpszExt == 0
	      || ignoreExtensions.find(lpszExt) == ignoreExtensions.end())
	    {
	      switch (task)
		{
		case Check:
		case ComputeDigest:
		  {
		    mapFnToMD5[Utils::GetRelativizedPath(path2.Get(),
							 lpszStripPrefix)] =
		      MD5::FromFile(path2.Get());
		    break;
		  }
		case FindDuplicates:
		  {
		    mapSizeToFn.insert (make_pair(dirEntry.size, path2.Get()));
		    break;
		  }
		}   
	    }
	}
    }
  pLister->Close ();
  pLister.reset ();
}

/* _________________________________________________________________________

   PrintMD5
   _________________________________________________________________________ */

void
PrintMD5 (/*[in]*/ const MD5 & md5)
{
  if (optBinary)
    {
#if defined(MIKTEX_WINDOWS) && defined(_MSC_VER)
      _setmode (_fileno(stdout), _O_BINARY);
#endif
      fwrite (md5.GetBits(), sizeof(md5), 1, stdout);
    }
  else
    {
      cout << md5.ToString() << endl;
    }
}

/* _________________________________________________________________________

   PrintDuplicates
   _________________________________________________________________________ */

void
PrintDuplicates (const set<string> & setstr)
{
  if (setstr.size() <= 1)
    {
      return;
    }
  Verbose (T_("found %u identical files (size: %u):\n"),
	   static_cast<unsigned>(setstr.size()),
	   static_cast<unsigned>(File::GetSize(setstr.begin()->c_str())));
  for (set<string>::const_iterator it = setstr.begin();
       it != setstr.end();
       ++ it)
    {
      cout << "  " << it->c_str() << '\n';
    }
  cout << endl;
}

/* _________________________________________________________________________

   FindDuplicateFiles
   _________________________________________________________________________ */

void
FindDuplicateFiles (const set<string> & setstr)
{
  if (setstr.size() <= 1)
    {
      return;
    }
  MD5ToFileName mapMd5sumToFn;
  for (set<string>::const_iterator it = setstr.begin();
       it != setstr.end();
       ++ it)
    {
      mapMd5sumToFn.insert (make_pair(MD5::FromFile(it->c_str()), *it));
    }
  MD5 md5Last;
  set<string> setstrFiles;
  for (MD5ToFileName::const_iterator it2 = mapMd5sumToFn.begin();
       it2 != mapMd5sumToFn.end();
       ++ it2)
    {
      if (it2 != mapMd5sumToFn.begin() && ! (md5Last == it2->first))
	{
	  PrintDuplicates (setstrFiles);
	  setstrFiles.clear ();
	}
      setstrFiles.insert (it2->second);
      md5Last = it2->first;
    }
  PrintDuplicates (setstrFiles);
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

void
Main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
  int option;

  Cpopt popt (argc, argv, aoption);

  popt.SetOtherOptionHelp (T_("[OPTION...] [DIRECTORY...]"));

  TASK task;

  if (PathName::Compare(Utils::GetExeName().c_str(), NAME_CHECK_INTEGRITY)
      == 0)
    {
      task = Check;
      optVerbose = true;
#if defined(MIKTEX_WINDOWS)
      optPauseWhenFinished = true;
#endif
    }
  else
    {
      task = ComputeDigest;
    }

  ignoreExtensions.insert (MD5WALK_FILE);

  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{
	case OPT_CHECK:
	  task = Check;
	  break;
	case OPT_COMPUTE_DIGEST:
	  task = ComputeDigest;
	  break;
	case OPT_EXCLUDE:
	  ignoreExtensions.insert (popt.GetOptArg());
	  break;
	case OPT_FIND_DUPLICATES:
	  task = FindDuplicates;
	  break;
	case OPT_VERSION:
	  cout <<
	    Utils::MakeProgramVersionString(Utils::GetExeName().c_str(),
					    VersionNumber(VER_FILEVERSION))
	       << T_("\n\
Copyright (C) 2005-2007 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	       << endl;
	  return;
	}
    }

  if (option != -1)
    {
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += ": ";
      msg += popt.Strerror(option);
      FatalError ("%s", msg.c_str());
    }
      
  FileNameToMD5 mapFnToMD5;
  SizeToFileName mapSizeToFn;

  vector<string> directories;

  const char ** leftovers = popt.GetArgs();

  if (leftovers == 0)
    {
      if (task == Check)
	{
	  directories.push_back (Directory::GetCurrentDirectory().ToString());
	}
    }
  else
    {
      for (int i = 0; *leftovers != 0; ++ leftovers, ++ i)
	{
	  directories.push_back (*leftovers);
	}
    }

  MD5 md5Good;
  bool haveMD5File = false;

  for (vector<string>::const_iterator it = directories.begin();
       it != directories.end();
       ++ it)
    {
      if (task == Check)
	{
	  Verbose (T_("Checking the data integrity of \"%s\"...\n"),
		   it->c_str());
	}
      DirectoryWalk (task,
		     it->c_str(),
		     it->c_str(),
		     mapFnToMD5,
		     mapSizeToFn);
      if (task == Check && it == directories.begin())
	{
	  PathName md5File (*it, MD5WALK_FILE, 0);
	  if (File::Exists(md5File))
	    {
	      haveMD5File = true;
	      md5Good = File::ReadAllBytes(md5File);
	    }
	}
    }

  switch (task)
    {
    case Check:
    case ComputeDigest:
      {
	MD5Builder md5Builder;
	for (FileNameToMD5::const_iterator it = mapFnToMD5.begin();
	     it != mapFnToMD5.end();
	     ++ it)
	  {
	    md5Builder.Update (it->first.c_str(), it->first.length());
	    md5Builder.Update (it->second.GetBits(), sizeof(MD5));
	  }
	MD5 md5 = md5Builder.Final();
	if (task == Check)
	  {
	    Verbose (T_("Done.\n\
Findings: "));
	    if (! haveMD5File)
	      {
		cerr << T_("The data might have been corrupted.") << endl;
		throw (CANNOT_CHECK_INTEGRITY);
	      }
	    if (md5 == md5Good)
	      {
		Verbose (T_("The data is intact.\n"));
	      }
	    else
	      {
		cerr << T_("The data has been corrupted.") << endl;
		throw (NOT_OF_INTEGRITY);
	      }
	  }
	else
	  {
	    PrintMD5 (md5);
	  }
      }
    case FindDuplicates:
      {
	set<string> setstr;
	size_t lastSize = 0;
	for (SizeToFileName::const_iterator it = mapSizeToFn.begin();
	     it != mapSizeToFn.end();
	     ++ it)
	  {
	    if (it != mapSizeToFn.begin() && it->first != lastSize)
	      {
		FindDuplicateFiles (setstr);
		setstr.clear ();
	      }
	    setstr.insert (it->second);
	    lastSize = it->first;
	  }
	FindDuplicateFiles (setstr);
      }
    }
}

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
  int exitCode;
  try
    {
      SessionWrapper pSession;
      Session::InitInfo initInfo;
      initInfo.SetProgramInvocationName (argv[0]);
      pSession.CreateSession (initInfo);
      Main (argc, argv);
      exitCode = 0;
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      exitCode = FATAL_ERROR;
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
      exitCode = FATAL_ERROR;
    }
  catch (int r)
    {
      exitCode = r;
    }
#if defined(MIKTEX_WINDOWS)
  if (optPauseWhenFinished)
    {
      cout << T_("\nPress any key to continue...") << flush;
      _getch ();
    }
#endif
  return (exitCode);
}
