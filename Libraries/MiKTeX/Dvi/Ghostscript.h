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

class CGhostscript : public CPostScript
{
public:
  CGhostscript ();

public:
  virtual ~CGhostscript ();

public:
  virtual bool Execute (/*[in]*/ LPCTSTR lpszFormat,
			/*[in]*/ ...);

public:
  static CString GetGhostscriptDirectory ();

public:
  static CString GetGhostscriptInclude ();

private:
  virtual bool Finalize ();

private:
  bool Start ();

public:
  virtual
  bool
  Write (/*[in]*/ const void *	p,
	 /*[in]*/ unsigned	n);

private:
  static bool __stdcall
  OnNewChunk (/*[in]*/ LPVOID				pv,
	      /*[in]*/ const CDibChunker::CHUNK *	pChunk);

private:
  static UINT Chunker (/*[in]*/ LPVOID pParam);

private:
  static UINT StderrReader (/*[in]*/ LPVOID pParam);

private:
  HANDLE m_hChunker;

private:
  HANDLE m_hStderrReader;

private:
  FILE * m_pStdin;

private:
  FILE * m_pStdout;

private:
  FILE * m_pStderr;

private:
  auto_ptr<Process> m_pGhostscript;

private:
  bool m_bGhostscriptRunning;

private:
  CString m_strStderr;
};
