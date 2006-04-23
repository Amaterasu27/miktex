/* findexe.cpp: find an executable

   Copyright (C) 2000-2006 Christian Schenk

   This file is part of FindExe.

   FindExe is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   FindExe is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with FindExe; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <io.h>
#include <sys/stat.h>

/* _________________________________________________________________________

   class CSVList
   _________________________________________________________________________ */

class CSVList
{
public:
  inline CSVList () : myvaluelist(0), nextvalue(0), value(0) {}

public:
  CSVList (const char *, int = ',');

public:
  ~CSVList ();

public:
  inline operator const char * () const { return (value); }

public:
  const char * operator ++ ();

private:
  char sepchar;

private:
  char * myvaluelist;

private:
  char * nextvalue;

private:
  char * value;
};

/* _________________________________________________________________________

   CSVList::CSVList
   _________________________________________________________________________ */

CSVList::CSVList (const char *	valuelist,
		  int		sep)
{
  if (valuelist == 0 || *valuelist == 0)
    {
      myvaluelist = nextvalue = value = 0;
    }
  else
    {
      sepchar = sep;
      myvaluelist = strdup(valuelist);
      value = myvaluelist;
      nextvalue = strchr(myvaluelist, sepchar);
      if (nextvalue)
	{
	  *nextvalue++ = 0;
	}
    }
}

/* _________________________________________________________________________

   CSVList::~CVSList
   _________________________________________________________________________ */

CSVList::~CSVList ()
{
  if (myvaluelist)
    {
      free (myvaluelist);
      myvaluelist = 0;
    }
}

/* _________________________________________________________________________

   CSVList::operator ++
   _________________________________________________________________________ */

const char *
CSVList::operator ++ ()

{
  if (nextvalue && *nextvalue)
    {
      value = nextvalue;
      nextvalue = strchr(nextvalue, sepchar);
      if (nextvalue)
	{
	  *nextvalue++ = 0;
	}
    }
  else
    {
      value = 0;
    }
  return (static_cast<const char *>(*this));
}

/* _________________________________________________________________________

   Help
   _________________________________________________________________________ */

void
Help ()
{
  puts ("Usage: findexe PROGRAM");
}

/* _________________________________________________________________________

   FindExe
   _________________________________________________________________________ */

size_t
FindExe (const char * lpszProgramName)
{
  const char * lpszPath = getenv("PATH");
  if (lpszPath == 0)
    {
      return (0);
    }
  CSVList path (lpszPath, ';');
  size_t found = 0;
  while (path)
    {
      char szExePath[_MAX_PATH];
      _makepath (szExePath, 0, path, lpszProgramName, 0);
      struct _stat stat;
      if (_stat(szExePath, &stat) == 0)
	{
	  printf ("\n%s\n", szExePath);
	  printf ("           Size: %d\n", stat.st_size);
	  printf ("  Time modified: %s", ctime(&stat.st_mtime));
	  found++;
	}
      ++ path;
    }
  return (found);
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (int	argc,
      char **	argv)
{
  if (argc != 2)
    {
      Help ();
      exit (1);
    }
  return (FindExe(argv[1]) > 0 ? 0 : 1);
}
