/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/spc_html.h,v 1.2 2005/07/03 20:02:29 csc Exp $

    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
    the dvipdfmx project team <dvipdfmx@project.ktug.or.kr>
    
    Copyright (C) 1998, 1999 by Mark A. Wicks <mwicks@kettering.edu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

#ifndef _SPC_HTML_H_
#define _SPC_HTML_H_

#include "specials.h"

extern int spc_html_at_begin_page     (void);
extern int spc_html_at_end_page       (void);
extern int spc_html_at_begin_document (void);
extern int spc_html_at_end_document   (void);

extern int spc_html_check_special (const char *buffer, long size);
extern int spc_html_setup_handler (struct spc_handler *handle,
				   struct spc_env *spe, struct spc_arg *args);

#endif /* _SPC_HTML_H_ */
