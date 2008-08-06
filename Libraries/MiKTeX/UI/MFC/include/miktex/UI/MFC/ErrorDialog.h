/* MiKTeX/UI/MFC/ErrorDialog:					-*- C++ -*-

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of MiKTeX UI Library.

   MiKTeX UI Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX UI Library; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#include <miktex/Core/Core>
#include <miktex/UI/MFC/Prototypes>

MIKUI_MFC_BEGIN_NAMESPACE;

class
MIKTEXNOVTABLE
ErrorDialog
{
public:
  AFX_EXT_CLASS
  static
  INT_PTR
  MIKTEXCEECALL
  DoModal (/*[in]*/ CWnd *					pParent,
	   /*[in]*/ const MiKTeX::Core::MiKTeXException &	e);

public:
  AFX_EXT_CLASS
  static
  INT_PTR
  MIKTEXCEECALL
  DoModal (/*[in]*/ CWnd *					pParent,
	   /*[in]*/ const std::exception &			e);
};

MIKUI_MFC_END_NAMESPACE;