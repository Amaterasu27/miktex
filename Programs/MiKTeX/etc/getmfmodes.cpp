/* getmfmodes.cpp: getting METAFONT modes

   Copyright (C) 2000-2006 Christian Schenk

   This file is part of GetMFModes.

   GetMFModes is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   GetMFModes is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with GetMFModes; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include <cstdlib>
#include <cstdio>

#include <miktex/core.h>

using namespace MiKTeX::Core;

#define MAX_MODES 20000

namespace {
  MIKTEXMFMODE rgMode[ MAX_MODES ];
  size_t cMode;
}

/* _________________________________________________________________________

   get_mode_names
   _________________________________________________________________________ */

int
get_mode_names ()

{

  PathName path;
  if (! SessionWrapper(true)->FindFile("modes.mf", FileType::MF, path))
    {
      fprintf (stderr, "Cannot find modes.mf\n");
      return (-1);
    }

  FILE * pfile = OpenFile(path.Get(), "r");
  if (pfile == 0)
    {
      fprintf (stderr, "Cannot open %s\n", path.Get());
      return (-1);
    }

  bool bModeDef = false;
  char szLine[2048];

  while (fgets(szLine, sizeof(szLine), pfile) && cMode < MAX_MODES)
    {
      if (bModeDef)
	{
	  if (strncmp(szLine, "enddef;", 7) == 0)
	    {
	      bModeDef = false;
	    }
	}
      else if (strncmp(szLine, "mode_def", 8) == 0)
	{
	  char * lpsz = szLine + 8;
	  while (! isalpha(*lpsz) && *lpsz != '\n')
	    {
	      ++ lpsz;
	    }
	  if (! isalpha(*lpsz))
	    {
	      continue;
	    }
	  char * lpszModeName = lpsz;
	  while (isalpha(*lpsz))
	    {
	      ++ lpsz;
	    }
	  if (*lpsz == 0)
	    {
	      continue;
	    }
	  *lpsz++ = 0;
	  if (strcmp(lpszModeName, "help") == 0)
	    {
	      continue;
	    }
	  lpsz = strstr(lpsz, "%\\[");
	  if (lpsz == 0)
	    {
	      continue;
	    }
	  lpsz += 3;
	  while (isspace(*lpsz))
	    {
	      *lpsz++;
	    }
	  if (lpsz == 0)
	    {
	      continue;
	    }
	  char * lpszPrinterName = lpsz;
	  while (*lpsz && *lpsz != '\n')
	    {
	      ++ lpsz;
	    }
	  *lpsz = 0;
	  bModeDef = true;
	  if (strcmp(lpszModeName, "help") != 0)
	    {
	      strcpy (rgMode[cMode].szMnemonic, lpszModeName);
	      strcpy (rgMode[cMode].szDescription, lpszPrinterName);
	      ++ cMode;
	    }
	}
    }

  return (0);
}

/* _________________________________________________________________________

   get_mode_params
   _________________________________________________________________________ */

int
get_mode_params ()

{
  char szMfExe[_MAX_PATH];
  if (! SessionWrapper(true)->FindFile("mf.exe", FileType::EXE, szMfExe))
    {
      fprintf (stderr, "mf.exe not found\n");
      return (-1);
    }

  for (size_t i = 0; i < cMode; ++i)
    {
      FILE * pfileTranscript;
      Utils::RemoveBlanksFromPathName (szMfExe);
      char szCommandLine[_MAX_PATH + 200];
      sprintf (szCommandLine,
	       "%s \\mode:=%s; mode_setup; message decimal pixels_per_inch; \
message decimal aspect_ratio; bye; > transcript",
	       szMfExe, rgMode[i].szMnemonic);
      system (szCommandLine);
      pfileTranscript = fopen("transcript", "r");
      char szLine[200];
      fgets (szLine, 200, pfileTranscript);
      fgets (szLine, 200, pfileTranscript);
      rgMode[i].iHorzRes = atoi(szLine);
      fgets (szLine, 200, pfileTranscript);
      rgMode[i].iVertRes = rgMode[i].iHorzRes * atoi(szLine);
    }

  return (0);
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (int	argc,
      char *	argv)

{
  MiKTeX::Core::SessionWrapper pSession (Session::InitInfo(argv[0]));

  size_t i;

  if (get_mode_names() == 0 && get_mode_params() == 0)
    {
      for (i = 0; i < cMode; ++i)
	{
	  printf ("%s\n%s\n%d\n%d\n",
		  rgMode[i].szMnemonic, rgMode[i].szDescription,
		  rgMode[i].iHorzRes, rgMode[i].iVertRes);
	}
      return (0);
    }
  else
    {
      return (1);
    }
}
