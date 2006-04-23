/* DviMagnifyingGlass.h:					-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#pragma once

#include "DviDraw.h"

class DviDoc;
class DviView;

class DviMagnifyingGlass
  : public CWnd,
    public DviDraw
{
public:
  typedef enum { Small, Medium, Large } Size;

public:
  DviMagnifyingGlass (/*[in]*/ DviView *	pDviView,
		      /*[in]*/ DviDoc *		pDviDoc,
		      /*[in]*/ Size		size = Small,
		      /*[in]*/ int		shrinkFactor = 1);

protected:
  virtual
  BOOL
  PreCreateWindow (/*[in,out]*/ CREATESTRUCT & cs);

protected:
  virtual
  void
  PostNcDestroy ();
  
public:
  void
  Create (/*[in]*/ const CPoint & pt);

protected:
  afx_msg
  void
  OnPaint ();

protected:
  afx_msg
  BOOL
  OnEraseBkgnd (/*[in]*/ CDC * pDC);
  
protected:
  DECLARE_MESSAGE_MAP();

public:
  void
  MoveGlass (/*[in]*/ const CPoint & pt);

private:
  DviDoc * pDviDoc;

private:
  DviView * pDviView;

private:
  CSize sizeWindow;

private:
  Size size;

private:
  int shrinkFactor;
};
