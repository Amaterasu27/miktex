/* screen.h: METAFONT online displays

   Copyright (C) 1998-2005 Christian Schenk
   Copyright (C) 1998 Wolfgang Kleinschmidt

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

#if defined(_MSC_VER)
#  pragma once
#endif

bool
miktexinitscreen (/*[in]*/ int	w,
		  /*[in]*/ int	h);

void
miktexblankrectangle (/*[in]*/ screencol	left_col,
		      /*[in]*/ screencol	right_col,
		      /*[in]*/ screenrow	top_row,
		      /*[in]*/ screenrow	bot_row);

void
miktexupdatescreen ();

void
miktexpaintrow (/*[in]*/ screenrow	r,
		/*[in]*/ pixelcolor	b,
		/*[in]*/ transspec 	a,
		/*[in]*/ screencol	n);
