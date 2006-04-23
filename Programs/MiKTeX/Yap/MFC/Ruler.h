/* Ruler.h:							-*- C++ -*-

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

class DviDoc;
class DviView;

class Ruler : public CWnd
{
protected:
  DECLARE_MESSAGE_MAP();

public:
  enum Kind { Horizontal, Vertical };

public:
  Ruler (/*[in]*/ CWnd *	pParent,
	 /*[in]*/ const CSize &	size,
	 /*[in]*/ Units		unit,
	 /*[in]*/ double	pixelsPerInch,
	 /*[in]*/ Kind		kind);
  
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
  
public:
  void
  MoveRuler (/*[in]*/ const CPoint & pt);

private:
  CWnd * pParent;

private:
  CSize sizeWindow;

private:
  Units unit;

private:
  double pixelsPerUnit;

private:
  Kind kind;
};
