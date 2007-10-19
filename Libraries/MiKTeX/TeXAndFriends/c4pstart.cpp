/* c4pstart.cpp: C4P statup code

   Copyright (C) 1996-2007 Christian Schenk

   This file is part of the MiKTeX TeXMF Library.

   The MiKTeX TeXMF Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX TeXMF Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX TeXMF Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

namespace {
  time_t startUpTime = static_cast<time_t>(-1);
  struct tm startUpTimeStruct;
  C4P_text standardTextFiles[3];
  int argCount;
  vector<const char *> argumentVector;
  string commandLine;
  string programName;
}

/* _________________________________________________________________________

   MakeCommandLine
   _________________________________________________________________________ */

MIKTEXMFAPI(int)
C4P::MakeCommandLine (/*[in]*/ int		argc,
		      /*[in]*/ const char **	argv)
{
  MIKTEX_API_BEGIN ("C4P::MakeCommandLine");
  argCount = 0;
  argumentVector.clear ();
  argumentVector.push_back (strdup(Utils::GetExeName().c_str()));
  ++ argCount;
  commandLine = "";
  for (int i = 0; i < argc; ++i, ++ argCount)
    {
      argumentVector.push_back (strdup(argv[i]));
      commandLine += ' ';
      MIKTEX_ASSERT_STRING (argv[i]);
      commandLine += argv[i];
    }
  return (0);
  MIKTEX_API_END ("C4P::MakeCommandLine");
}

/* _________________________________________________________________________

   SetStartUpTime
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
C4P::SetStartUpTime (/*[in]*/ time_t time)
{
  MIKTEX_API_BEGIN ("C4P::SetStartUpTime");
  startUpTime = time;
#if defined(_MSC_VER) && (_MSC_VER) >= 1400
  if (_localtime64_s(&startUpTimeStruct, &startUpTime) != 0)
    {
      FATAL_CRT_ERROR ("_localtime_s", 0);
    }
#else
  struct tm * p = localtime(&startUpTime);
  if (p == 0)
    {
      FATAL_CRT_ERROR ("localtime", 0);
    }
  startUpTimeStruct = *p;
#endif
  MIKTEX_API_END ("C4P::SetStartUpTime");
}

/* _________________________________________________________________________

   GetStartUpTime
   _________________________________________________________________________ */

MIKTEXMFAPI(time_t)
C4P::GetStartUpTime ()
{
  MIKTEX_API_BEGIN ("C4P::GetStartUpTime");
  return (startUpTime);
  MIKTEX_API_END ("C4P::GetStartUpTime");
}

/* _________________________________________________________________________

   Program::Program
   _________________________________________________________________________ */

C4PEXPORT
C4PCALL
C4P::Program::Program (/*[in]*/ const char *	lpszProgramName,
		       /*[in]*/ int			argc,
		       /*[in]*/ const char **	argv)
{
  MIKTEX_API_BEGIN ("C4P::StartUp");
  MIKTEX_ASSERT_STRING (lpszProgramName);
  programName = lpszProgramName;
  if (startUpTime == static_cast<time_t>(-1))
    {
      SetStartUpTime (time(0));
    }
  MakeCommandLine (argc - 1, argv + 1);
  standardTextFiles[0].Attach (stdin, false);
  standardTextFiles[1].Attach (stdout, false);
  standardTextFiles[2].Attach (stderr, false);
  *input = '\n';
  *output = '\0';
  *c4perroroutput = '\0';
  running = true;
  MIKTEX_API_END ("C4P::StartUp");
}

/* _________________________________________________________________________

   Program::Finish
   _________________________________________________________________________ */

C4PAPI(void)
C4P::Program::Finish ()
{
  for (vector<const char *>::iterator it = argumentVector.begin();
       it != argumentVector.end();
       ++it)
    {
      free (const_cast<char *>(*it));
    }
  argumentVector.clear ();
  commandLine = "";
  programName = "";
}

/* _________________________________________________________________________

   GetYear
   _________________________________________________________________________ */

MIKTEXMFAPI(unsigned)
C4P::GetYear ()
{
  MIKTEX_API_BEGIN ("C4P::GetYear");
  return (startUpTimeStruct.tm_year + 1900);
  MIKTEX_API_END ("C4P::GetYear");
}

/* _________________________________________________________________________

   GetMonth
   _________________________________________________________________________ */

MIKTEXMFAPI(unsigned)
C4P::GetMonth ()
{
  MIKTEX_API_BEGIN ("C4P::GetMonth");
  return (startUpTimeStruct.tm_mon + 1);
  MIKTEX_API_END ("C4P::GetMonth");
}

/* _________________________________________________________________________

   GetDay
   _________________________________________________________________________ */

MIKTEXMFAPI(unsigned)
C4P::GetDay ()
{
  MIKTEX_API_BEGIN ("C4P::GetDay");
  return (startUpTimeStruct.tm_mday);
  MIKTEX_API_END ("C4P::GetDay");
}

/* _________________________________________________________________________

   GetHour
   _________________________________________________________________________ */

MIKTEXMFAPI(unsigned)
C4P::GetHour ()
{
  MIKTEX_API_BEGIN ("C4P::GetHour");
  return (startUpTimeStruct.tm_hour);
  MIKTEX_API_END ("C4P::GetHour");
}

/* _________________________________________________________________________

   GetMinute
   _________________________________________________________________________ */

MIKTEXMFAPI(unsigned)
C4P::GetMinute ()
{
  MIKTEX_API_BEGIN ("C4P::GetMinute");
  return (startUpTimeStruct.tm_min);
  MIKTEX_API_END ("C4P::GetMinute");
}

/* _________________________________________________________________________

   GetSecond
   _________________________________________________________________________ */

MIKTEXMFAPI(unsigned)
C4P::GetSecond ()
{
  MIKTEX_API_BEGIN ("C4P::GetSecond");
  return (startUpTimeStruct.tm_sec);
  MIKTEX_API_END ("C4P::GetSecond");
}

/* _________________________________________________________________________

   GetStdFilePtr
   _________________________________________________________________________ */

MIKTEXMFAPI(C4P::C4P_text *)
C4P::GetStdFilePtr (/*[in]*/ unsigned idx)
{
  MIKTEX_API_BEGIN ("C4P::GetStdFilePtr");
  MIKTEX_ASSERT
    (idx < sizeof(standardTextFiles) / sizeof(standardTextFiles[0]));
  return (&standardTextFiles[idx]);
  MIKTEX_API_END ("C4P::GetStdFilePtr");
}

/* _________________________________________________________________________

   GetArgC
   _________________________________________________________________________ */

MIKTEXMFAPI(int)
C4P::GetArgC ()
{
  MIKTEX_API_BEGIN ("C4P::GetArgC");
  return (argCount);
  MIKTEX_API_END ("C4P::GetArgC");
}

/* _________________________________________________________________________

   GetArgV
   _________________________________________________________________________ */

MIKTEXMFAPI(const char **)
C4P::GetArgV ()
{
  MIKTEX_API_BEGIN ("C4P::GetArgV");
  return (&argumentVector[0]);
  MIKTEX_API_END ("C4P::GetArgV");
}

/* _________________________________________________________________________

   GetCmdLine
   _________________________________________________________________________ */

MIKTEXMFAPI(const char *)
C4P::GetCmdLine ()
{
  MIKTEX_API_BEGIN ("C4P::GetCmdLine");
  return (commandLine.c_str());
  MIKTEX_API_END ("C4P::GetCmdLine");
}

/* _________________________________________________________________________

   GetProgName
   _________________________________________________________________________ */

MIKTEXMFAPI(char *)
C4P::GetProgName (/*[in]*/ char * lpszProgName)
{
  MIKTEX_API_BEGIN ("C4P::GetProgName");
  Utils::CopyString (lpszProgName, BufferSizes::MaxPath, programName.c_str());
  return (lpszProgName);
  MIKTEX_API_END ("C4P::GetProgName");
}
