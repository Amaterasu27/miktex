/* tempfile.cpp: managing temporary files

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

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   miktex_remember_temp_file
   _________________________________________________________________________ */

MIKTEXAPI(void)
miktex_remember_temp_file (/*[in]*/ const MIKTEXCHAR * lpszFileName)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_STRING (lpszFileName);
  UNUSED (lpszFileName);
  UNIMPLEMENTED (T_("miktex_remember_temp_file"));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_create_temp_file_name
   _________________________________________________________________________ */

MIKTEXAPI(void)
miktex_create_temp_file_name (/*[out]*/ MIKTEXCHAR *	lpszFileName)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_PATH_BUFFER (lpszFileName);
  Utils::CopyString (lpszFileName,
		     BufferSizes::MaxPath,
		     PathName().SetToTempFile().Get());
  C_FUNC_END ();
}
