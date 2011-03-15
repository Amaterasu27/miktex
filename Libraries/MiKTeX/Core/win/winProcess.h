/* winProcess.h:						-*- C++ -*-

   Copyright (C) 1996-2011 Christian Schenk

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

class winProcess : public Process2
{
public:
  virtual
  FILE *
  MIKTEXTHISCALL
  get_StandardInput ();

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  get_StandardOutput ();

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  get_StandardError ();

public:
  virtual
  void
  MIKTEXTHISCALL
  WaitForExit ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  WaitForExit (/*[in]*/ int milliseconds);

public:
  virtual
  int
  MIKTEXTHISCALL
  get_ExitCode ();

public:
  virtual
  void
  MIKTEXTHISCALL
  Close ();

public:
  virtual
  Process2 *
  MIKTEXTHISCALL
  get_Parent ();

public:
  virtual
  std::string
  MIKTEXTHISCALL
  get_ProcessName ();

private:
  winProcess ();

private:
  winProcess (/*[in]*/ const ProcessStartInfo & startinfo);

private:
  virtual
  ~winProcess ();

private:
  void
  Create ();

private:
  ProcessStartInfo startinfo;

private:
  PROCESS_INFORMATION processInformation;

private:
  HANDLE standardInput;
  HANDLE standardOutput;
  HANDLE standardError;

private:
  FILE * pFileStandardInput;
  FILE * pFileStandardOutput;
  FILE * pFileStandardError;

private:
  bool processStarted;

private:
  PROCESSENTRY32W processEntry;

private:
  static
  PROCESSENTRY32W
  GetProcessEntry (/*[in]*/ DWORD processId);

private:
  static
  bool
  TryGetProcessEntry (/*[in]*/ DWORD processId,
		      /*[out]*/ PROCESSENTRY32W & processEntry);

private:
  friend class Process;
  friend class Process2;
};
