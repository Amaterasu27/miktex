/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/pdfdraw.h,v 1.2 2005/07/03 20:02:28 csc Exp $
    
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

#ifndef _PDF_DRAW_H_
#define _PDF_DRAW_H_

#include "pdfdev.h"

#define  PDF_DASH_SIZE_MAX  16
#define  PDF_GSAVE_MAX      256

extern void  pdf_dev_init_gstates  (void);
extern void  pdf_dev_clean_gstates (void);

#define pdf_copymatrix(m,n) do {\
  (m)->a = (n)->a; (m)->b = (n)->b;\
  (m)->c = (n)->c; (m)->d = (n)->d;\
  (m)->e = (n)->e; (m)->f = (n)->f;\
} while (0)

#define pdf_setmatrix(m,p,q,r,s,t,u) do {\
  (m)->a = (p); (m)->b = (q);\
  (m)->c = (r); (m)->d = (s);\
  (m)->e = (t); (m)->f = (u);\
} while (0)

/* m -> m x n */
#define pdf_concatmatrix(m,n) do {\
  double _tmp_a, _tmp_b, _tmp_c, _tmp_d; \
  _tmp_a = (m)->a; _tmp_b = (m)->b; \
  _tmp_c = (m)->c; _tmp_d = (m)->d; \
  (m)->a  = _tmp_a * ((n)->a) + _tmp_b * ((n)->c); \
  (m)->b  = _tmp_a * ((n)->b) + _tmp_b * ((n)->d); \
  (m)->c  = _tmp_c * ((n)->a) + _tmp_d * ((n)->c); \
  (m)->d  = _tmp_c * ((n)->b) + _tmp_d * ((n)->d); \
  (m)->e += _tmp_a * ((n)->e) + _tmp_c * ((n)->f); \
  (m)->f += _tmp_b * ((n)->e) + _tmp_d * ((n)->f); \
} while (0)

extern void   pdf_invertmatrix      (pdf_tmatrix *M);

extern int    pdf_dev_currentmatrix (pdf_tmatrix *M);
extern int    pdf_dev_currentpoint  (pdf_coord *cp);

extern int    pdf_dev_setlinewidth  (double width);
extern int    pdf_dev_setmiterlimit (double mlimit);
extern int    pdf_dev_setlinecap    (int style);
extern int    pdf_dev_setlinejoin   (int style);
extern int    pdf_dev_setdash       (int count, double *pattern, double offset);

extern int    pdf_dev_moveto        (double x,  double y);
extern int    pdf_dev_rmoveto       (double x,  double y);
extern int    pdf_dev_closepath     (void);

extern int    pdf_dev_gsave         (void);
extern int    pdf_dev_grestore      (void);

extern int    pdf_dev_lineto        (double x0, double y0);
extern int    pdf_dev_rlineto       (double x0, double y0);
extern int    pdf_dev_curveto       (double x0, double y0,
				     double x1, double y1,
				     double x2, double y2);
extern int    pdf_dev_rcurveto      (double x0, double y0,
				     double x1, double y1,
				     double x2, double y2);
extern int    pdf_dev_arc           (double c_x , double c_y,
				     double r_x , double r_y,
				     double ang0, double ang1);
extern int    pdf_dev_arcn          (double c_x , double c_y,
				     double r_x , double r_y,
				     double ang0, double ang1);
extern int    pdf_dev_bspline       (double x0, double y0,
				     double x1, double y1,
				     double x2, double y2);

#define PDF_FILL_RULE_NONZERO 0
#define PDF_FILL_RULE_EVENODD 1

extern int    pdf_dev_newpath       (void);

extern int    pdf_dev_clip          (void);
extern int    pdf_dev_eoclip        (void);

extern int    pdf_dev_flushpath     (char p_op, int rule);
#define pdf_dev_fill()       pdf_dev_flushpath('f', PDF_FILL_RULE_NONZERO)
#define pdf_dev_stroke()     pdf_dev_flushpath('S', PDF_FILL_RULE_NONZERO)
#define pdf_dev_fillstroke() pdf_dev_flushpath('B', PDF_FILL_RULE_NONZERO)

extern int    pdf_dev_concat        (const pdf_tmatrix *M);

extern void   pdf_dev_dtransform    (pdf_coord *p, const pdf_tmatrix *M);
extern void   pdf_dev_idtransform   (pdf_coord *p, const pdf_tmatrix *M);
extern void   pdf_dev_transform     (pdf_coord *p, const pdf_tmatrix *M);
extern void   pdf_dev_itransform    (pdf_coord *p, const pdf_tmatrix *M);

extern void   pdf_make_tmatrix      (pdf_tmatrix *M,
				     double xscale, double yscale,
				     double rotate,
				     double x_user, double y_user);

extern int    pdf_dev_gsave         (void);
extern int    pdf_dev_grestore      (void);

/* The depth here is the depth of q/Q nesting.
 * We must remember current depth of nesting when starting a page or xform,
 * and must recover until that depth at the end of page/xform.
 */
extern int    pdf_dev_current_depth (void);
extern void   pdf_dev_grestore_to   (int depth);
#define pdf_dev_grestoreall() pdf_dev_grestore_to(0);

#endif /* _PDF_DRAW_H_ */
