/* unxProcess.h:						-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

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

class unxProcess : public Process
{
public:
  virtual
  FILE *
  MIKTEXCALL
  get_StandardInput ();

public:
  virtual
  FILE *
  MIKTEXCALL
  get_StandardOutput ();

public:
  virtual
  FILE *
  MIKTEXCALL
  get_StandardError ();

public:
  virtual
  void
  MIKTEXCALL
  WaitForExit ();

public:
  virtual
  bool
  MIKTEXCALL
  WaitForExit (/*[in]*/ int milliseconds);

public:
  virtual
  int
  MIKTEXCALL
  get_ExitCode ();

public:
  virtual
  void
  MIKTEXCALL
  Close ();
  
private:
  unxProcess (/*[in]*/ const ProcessStartInfo & startinfo);

private:
  virtual
  ~unxProcess ();

private:
  void
  Create ();

private:
  ProcessStartInfo startinfo;

private:
  int status;

private:
  pid_t pid;

private:
  int fdStandardInput;
  int fdStandardOutput;
  int fdStandardError;

private:
  FILE * pFileStandardInput;
  FILE * pFileStandardOutput;
  FILE * pFileStandardError;

private:
  friend class Process;
};
