/* output.c: C4p output routines				-*- C++ -*-

   Copyright (C) 1991-2001 Christian Schenk

   This file is part of C4P.

   C4P is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   C4P is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with C4P; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cstdlib>

#include "common.h"

namespace {
  bool halting;
  FILE * file_table[3];
  FILE * cur_file;
  int cur_file_num;
  char out_buf[BUF_SIZE];
  unsigned buf_ptr;
  unsigned chars_in_buf;
  int last_char;
}

#if ! defined (min)
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif

#if ! defined (max)
#define max(a,b)	(((a) > (b)) ? (a) : (b))
#endif

/* _________________________________________________________________________

   flush_buf
   _________________________________________________________________________ */

void
flush_buf ()
{
  if (chars_in_buf == BUF_SIZE)
    {
      fwrite (out_buf + buf_ptr, 1, BUF_SIZE - buf_ptr, cur_file);
    }
  if (buf_ptr > 0)
    {
      fwrite (out_buf, 1, buf_ptr, cur_file);
    }
  buf_ptr = 0;
  chars_in_buf = 0;
}

/* _________________________________________________________________________

   get_buf_mark
   _________________________________________________________________________ */

unsigned
get_buf_mark ()
{
  return (buf_ptr);
}

/* _________________________________________________________________________

   get_buf_text
   _________________________________________________________________________ */

void
get_buf_text (std::string &	str,
	      unsigned		buf_mark,
	      unsigned		count)
{
  buf_mark %= BUF_SIZE;

  if (count == 0)
    {
      if (buf_mark < buf_ptr)
	{
	  count = buf_ptr - buf_mark;
	}
      else
	{
	  count = BUF_SIZE - (buf_mark - buf_ptr);
	}
    }

  unsigned n = min(count, (unsigned) BUF_SIZE - buf_mark);

  str.assign (out_buf + buf_mark, n);
  if (n < count)
    {
      str.append (out_buf, count - n);
    }
}

/* _________________________________________________________________________

   out_buf_over
   _________________________________________________________________________ */

void
out_buf_over (unsigned		buf_mark,
	      const char *	string,
	      unsigned		count)
{
  if (count == 0)
    {
      if (buf_mark < buf_ptr)
	{
	  count = buf_ptr - buf_mark;
	}
      else
	{
	  count = BUF_SIZE - (buf_mark - buf_ptr);
	}
    }

  unsigned n = min(count, (unsigned) BUF_SIZE - buf_mark);

  memcpy (out_buf + buf_mark, string, n);
  if (n < count)
    {
      memcpy (out_buf, string + n, count - n);
    }
}

/* _________________________________________________________________________

   open_file
   _________________________________________________________________________ */

void
open_file (unsigned	filnum,
	   const char *	file_name)
{
  if ((file_table[filnum] = fopen(file_name, "wb")) == 0)
    {
      perror (file_name);
    }
  if (filnum == C_FILE_NUM)
    {
      c_file_line_count = 0;
    }
}

/* _________________________________________________________________________

   close_file
   _________________________________________________________________________ */

void
close_file (unsigned filnum)
{
  flush_buf ();
  fclose (file_table[filnum]);
}

/* _________________________________________________________________________

   reopen_file
   _________________________________________________________________________ */

void
reopen_file (unsigned		filnum,
	     const char *	file_name)
{
  flush_buf ();
  fclose (file_table[filnum]);
  open_file (filnum, file_name);
}

/* _________________________________________________________________________

   put_char
   _________________________________________________________________________ */

void
put_char (int c)
{
  if (chars_in_buf == BUF_SIZE)
    {
      fputc (out_buf[buf_ptr], cur_file);
    }
  else
    {
      ++ chars_in_buf;
    }
  out_buf[buf_ptr] = (char) c;
  buf_ptr = (buf_ptr + 1) % BUF_SIZE;
}


/* _________________________________________________________________________

   out_char
   _________________________________________________________________________ */

void
out_char (int c)
{
  if (last_char == '\n')
    {
      for (unsigned i = 0; i < (curly_brace_level + extra_indent) * 2; ++ i)
	{
	  put_char (' ');
	}
      if (cur_file_num == C_FILE_NUM)
	{
	  ++ c_file_line_count;
	}
    }
  if (c == '\n' && macroizing)
    {
      put_char ('\\');
    }
  put_char (c);
  last_char = c;
}

/* _________________________________________________________________________

   out_stop
   _________________________________________________________________________ */

void
out_stop ()
{
  halting = true;
}

/* _________________________________________________________________________

   out_resume
   _________________________________________________________________________ */

void
out_resume ()
{
  halting = false;
}

/* _________________________________________________________________________

   out_form
   _________________________________________________________________________ */

void
out_form (const char *	fmt,
			...)
{
  if (halting)
    {
      return;
    }
  va_list ap;
  va_start (ap, fmt);
  char buf[4096];
  vsprintf (buf, fmt, ap);
  va_end (ap);
  out_s (buf);
}

/* _________________________________________________________________________

   out_s
   _________________________________________________________________________ */

void
out_s (const char *s)
{
  if (halting)
    {
      return;
    }
  for (; *s; last_char = *s, ++ s)
    {
      out_char (*s);
    }
}

/* _________________________________________________________________________

   redir_file
   _________________________________________________________________________ */

void
redir_file (int filnum)
{
  flush_buf ();
  if (filnum >= 0)
    {
      cur_file = file_table[filnum];
      last_char = '\n';
    }
  cur_file_num = filnum;
}

/* _________________________________________________________________________

   translate_char
   _________________________________________________________________________ */

void
translate_char (int c)
{
  switch (c)
    {
    case '\'':
      out_s ("\\\'");
      break;
    case '\\':
      out_s ("\\\\");
      break;
    default:
      out_char (c);
      break;
    }
}

/* _________________________________________________________________________

   translate_string
   _________________________________________________________________________ */

void
translate_string (const char * s)
{
  while (*s)
    {
      switch (*s)
	{
	case '"':
	  out_s ("\\\"");
	  break;
	case '\\':
	  out_s ("\\\\");
	  break;
	default:
	  out_char (*s);
	  break;
	}
      ++ s;
    }
}

/* _________________________________________________________________________

   out_init
   _________________________________________________________________________ */

void
out_init ()
{
  ;
}
