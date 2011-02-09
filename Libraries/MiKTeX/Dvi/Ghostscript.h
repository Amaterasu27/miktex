/* Ghostscript.h:					-*- C++ -*-

   Copyright (C) 1996-2011 Christian Schenk

   This file is part of the MiKTeX DVI Library.

   The MiKTeX DVI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX DVI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the MiKTeX DVI Library; if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
   USA.  */

#pragma once

#include "PostScript.h"

class CGhostscript
  : public CPostScript,
    public IDibChunkerCallback
{
public:
  CGhostscript ();

public:
  virtual
  ~CGhostscript ();

public:
  virtual
  void
  Execute (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  virtual
  void
  Finalize ();

private:
  void
  Start ();

public:
  virtual
  void
  Write (/*[in]*/ const void *	p,
	 /*[in]*/ unsigned	n);

public:
  virtual
  size_t
  MIKTEXTHISCALL
  Read (/*[out]*/ void *	pBuf,
	/*[in]*/ size_t		size);

public:
  virtual
  void
  MIKTEXTHISCALL
  OnNewChunk (/*[in]*/ DibChunk * pChunk);

private:
  static void MIKTEXCALLBACK Chunker (/*[in]*/ void * pParam);

private:
  static void MIKTEXCALLBACK StderrReader (/*[in]*/ void * pParam);

private:
  auto_ptr<Thread> pChunkerThread;

private:
  auto_ptr<Thread> pStderrReaderThread;

private:
  FileStream gsIn;

private:
  FileStream gsOut;

private:
  FileStream gsErr;

private:
  auto_ptr<Process> m_pGhostscript;

private:
  bool m_bGhostscriptRunning;

private:
  string m_strStderr;
};
