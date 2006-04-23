/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/colors.h,v 1.3 2005/07/03 20:02:28 csc Exp $
    
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

#ifndef _COLORS_H_
#define _COLORS_H_

#include "pdfcolor.h"

struct color_by_name {
  const char  *key;
  int          colorspace;
  double       values[4];
} colors_by_name[] = {
  {"GreenYellow",    PDF_COLORSPACE_TYPE_DEVICECMYK, {0.15, 0.00, 0.69, 0.00}},
  {"Yellow",         PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.00, 1.00, 0.00}},
  {"Goldenrod",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.10, 0.84, 0.00}},
  {"Dandelion",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.29, 0.84, 0.00}},
  {"Apricot",        PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.32, 0.52, 0.00}},
  {"Peach",          PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.50, 0.70, 0.00}},
  {"Melon",          PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.46, 0.50, 0.00}},
  {"YellowOrange",   PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.42, 1.00, 0.00}},
  {"Orange",         PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.61, 0.87, 0.00}},
  {"BurntOrange",    PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.51, 1.00, 0.00}},
  {"Bittersweet",    PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.75, 1.00, 0.24}},
  {"RedOrange",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.77, 0.87, 0.00}},
  {"Mahogany",       PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.85, 0.87, 0.35}},
  {"Maroon",         PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.87, 0.68, 0.32}},
  {"BrickRed",       PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.89, 0.94, 0.28}},
  {"Red",            PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 1.00, 1.00, 0.00}},
  {"OrangeRed",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 1.00, 0.50, 0.00}},
  {"RubineRed",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 1.00, 0.13, 0.00}},
  {"WildStrawberry", PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.96, 0.39, 0.00}},
  {"Salmon",         PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.53, 0.38, 0.00}},
  {"CarnationPink",  PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.63, 0.00, 0.00}},
  {"Magenta",        PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 1.00, 0.00, 0.00}},
  {"VioletRed",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.81, 0.00, 0.00}},
  {"Rhodamine",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.82, 0.00, 0.00}},
  {"Mulberry",       PDF_COLORSPACE_TYPE_DEVICECMYK, {0.34, 0.90, 0.00, 0.02}},
  {"RedViolet",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.07, 0.90, 0.00, 0.34}},
  {"Fuchsia",        PDF_COLORSPACE_TYPE_DEVICECMYK, {0.47, 0.91, 0.00, 0.08}},
  {"Lavender",       PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.48, 0.00, 0.00}},
  {"Thistle",        PDF_COLORSPACE_TYPE_DEVICECMYK, {0.12, 0.59, 0.00, 0.00}},
  {"Orchid",         PDF_COLORSPACE_TYPE_DEVICECMYK, {0.32, 0.64, 0.00, 0.00}},
  {"DarkOrchid",     PDF_COLORSPACE_TYPE_DEVICECMYK, {0.40, 0.80, 0.20, 0.00}},
  {"Purple",         PDF_COLORSPACE_TYPE_DEVICECMYK, {0.45, 0.86, 0.00, 0.00}},
  {"Plum",           PDF_COLORSPACE_TYPE_DEVICECMYK, {0.50, 1.00, 0.00, 0.00}},
  {"Violet",         PDF_COLORSPACE_TYPE_DEVICECMYK, {0.79, 0.88, 0.00, 0.00}},
  {"RoyalPurple",    PDF_COLORSPACE_TYPE_DEVICECMYK, {0.75, 0.90, 0.00, 0.00}},
  {"BlueViolet",     PDF_COLORSPACE_TYPE_DEVICECMYK, {0.86, 0.91, 0.00, 0.04}},
  {"Periwinkle",     PDF_COLORSPACE_TYPE_DEVICECMYK, {0.57, 0.55, 0.00, 0.00}},
  {"CadetBlue",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.62, 0.57, 0.23, 0.00}},
  {"CornflowerBlue", PDF_COLORSPACE_TYPE_DEVICECMYK, {0.65, 0.13, 0.00, 0.00}},
  {"MidnightBlue",   PDF_COLORSPACE_TYPE_DEVICECMYK, {0.98, 0.13, 0.00, 0.43}},
  {"NavyBlue",       PDF_COLORSPACE_TYPE_DEVICECMYK, {0.94, 0.54, 0.00, 0.00}},
  {"RoyalBlue",      PDF_COLORSPACE_TYPE_DEVICECMYK, {1.00, 0.50, 0.00, 0.00}},
  {"Blue",           PDF_COLORSPACE_TYPE_DEVICECMYK, {1.00, 1.00, 0.00, 0.00}},
  {"Cerulean",       PDF_COLORSPACE_TYPE_DEVICECMYK, {0.94, 0.11, 0.00, 0.00}},
  {"Cyan",           PDF_COLORSPACE_TYPE_DEVICECMYK, {1.00, 0.00, 0.00, 0.00}},
  {"ProcessBlue",    PDF_COLORSPACE_TYPE_DEVICECMYK, {0.96, 0.00, 0.00, 0.00}},
  {"SkyBlue",        PDF_COLORSPACE_TYPE_DEVICECMYK, {0.62, 0.00, 0.12, 0.00}},
  {"Turquoise",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.85, 0.00, 0.20, 0.00}},
  {"TealBlue",       PDF_COLORSPACE_TYPE_DEVICECMYK, {0.86, 0.00, 0.34, 0.02}},
  {"Aquamarine",     PDF_COLORSPACE_TYPE_DEVICECMYK, {0.82, 0.00, 0.30, 0.00}},
  {"BlueGreen",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.85, 0.00, 0.33, 0.00}},
  {"Emerald",        PDF_COLORSPACE_TYPE_DEVICECMYK, {1.00, 0.00, 0.50, 0.00}},
  {"JungleGreen",    PDF_COLORSPACE_TYPE_DEVICECMYK, {0.99, 0.00, 0.52, 0.00}},
  {"SeaGreen",       PDF_COLORSPACE_TYPE_DEVICECMYK, {0.69, 0.00, 0.50, 0.00}},
  {"Green",          PDF_COLORSPACE_TYPE_DEVICECMYK, {1.00, 0.00, 1.00, 0.00}},
  {"ForestGreen",    PDF_COLORSPACE_TYPE_DEVICECMYK, {0.91, 0.00, 0.88, 0.12}},
  {"PineGreen",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.92, 0.00, 0.59, 0.25}},
  {"LimeGreen",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.50, 0.00, 1.00, 0.00}},
  {"YellowGreen",    PDF_COLORSPACE_TYPE_DEVICECMYK, {0.44, 0.00, 0.74, 0.00}},
  {"SpringGreen",    PDF_COLORSPACE_TYPE_DEVICECMYK, {0.26, 0.00, 0.76, 0.00}},
  {"OliveGreen",     PDF_COLORSPACE_TYPE_DEVICECMYK, {0.64, 0.00, 0.95, 0.40}},
  {"RawSienna",      PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.72, 1.00, 0.45}},
  {"Sepia",          PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.83, 1.00, 0.70}},
  {"Brown",          PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.81, 1.00, 0.60}},
  {"Tan",            PDF_COLORSPACE_TYPE_DEVICECMYK, {0.14, 0.42, 0.56, 0.00}},
  {"Gray",           PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.00, 0.00, 0.50}},
  {"Black",          PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.00, 0.00, 1.00}},
  {"White",          PDF_COLORSPACE_TYPE_DEVICECMYK, {0.00, 0.00, 0.00, 0.00}}
};

#define NUM_NAMED_COLORS (sizeof(colors_by_name)/sizeof(colors_by_name[0]))

#endif /* _COLORS_H_ */
