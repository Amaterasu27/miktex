/* BitmapPrinter.h:						-*- C++ -*-

   Copyright (C) 2003-2006 Christian Schenk

   This file is part of MTPrint.

   MTPrint is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MTPrint is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MTPrint; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#pragma once

#include "Printer.h"

class BitmapPrinter
  : public Printer,
    public IDibChunkerCallback
{
public:
  BitmapPrinter (/*[in]*/ const Printer::PRINTINFO &	printinfo,
		 /*[in]*/ bool				pintNothing = false);

public:
  virtual
  size_t
  DIBCHUNKERCALL
  Read (/*[out]*/ void *	pBuf,
	/*[in]*/ size_t		size);

public:
  virtual
  void
  DIBCHUNKERCALL
  OnNewChunk (/*[in]*/ DibChunk * pChunk);

public:
  void
  Print (/*[in]*/ FILE *	pfileDibStream);

private:
  void
  PrintChunk (/*[in]*/ DibChunk * pChunk);

private:
  int offsetX;

private:
  int offsetY;

private:
  FileStream stream;
};
