/* conspawn.cpp: spawning console processes

   Copyright (C) 1998-2006 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with This file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

/* see MSDN Q150956 */

#include <windows.h>
#include <cstdio>
#include <cassert>

int
main (/*[in]*/ int	argc,
      /*[in]*/ char *	argv[])
{
  assert (argc == 2 && argv[1] != 0);
  if (argc < 2 || argv[1] == 0)
    {
      return (1);
    }
  STARTUPINFO startupInfo;
  ZeroMemory (&startupInfo, sizeof(startupInfo));
  startupInfo.cb = sizeof(startupInfo);
  startupInfo.dwFlags = STARTF_USESTDHANDLES;
  startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
  startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
  PROCESS_INFORMATION processInfo;
  ZeroMemory (&processInfo, sizeof(processInfo));
  BOOL done =
    CreateProcess(0, argv[1], 0, 0, TRUE, 0, 0, 0, &startupInfo, &processInfo);
  if (! done)
    {
      return (1);
    }
  WaitForSingleObject (processInfo.hProcess, INFINITE);
  DWORD exitCode;
  GetExitCodeProcess (processInfo.hProcess, &exitCode);
  CloseHandle (processInfo.hProcess);
  CloseHandle (processInfo.hThread);
  return (static_cast<int>(exitCode));
}
