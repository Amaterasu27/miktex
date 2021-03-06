/* inipool.cpp: WEB pool to C (based on makecpool)

   Copyright (C) 2007, 2008 Taco Hoekwater.
   Copyright (C) 2008-2009 Christian Schenk

   You may freely use, modify and/or distribute this file. */

#include <cstdio>
#include <cstring>
#include <cstdlib>

int
main (int	argc,
      char *	argv[])
{
  if (argc != 3)
    {
      fprintf (stderr,
	       "\
%s: need exactly two arguments (pool name and C/C++ header name).\n",
	       argv[0]);
      exit(EXIT_FAILURE);
    }
  char * filename = argv[1];
  char *headername = argv[2];
  FILE * fh = fopen(filename, "r");
  if (fh == 0)
    {
      fprintf(stderr, "%s: can't open %s for reading.\n", argv[0], filename);
      exit(EXIT_FAILURE);
    }
  bool is_metafont = false;
  bool is_metapost = false;
  bool is_luatex = false;
  if (strstr(filename, "luatex.pool") != 0)
    {
      is_luatex = true;
    }
  else if (strstr(filename, "mp.pool") != 0)
    {
      is_metapost = true;
    }
  else if (strstr(filename, "mf.pool") != 0)
    {
      is_metafont = true;
    }
  printf ("\
/*\n\
 * This file is auto-generated by inipool.\n\
 *   %s %s %s\n\
 */\n\
\n\
#include <string.h>\n\
#include \"%s\"\n\
\n\
#undef strpool\n\
#undef poolptr\n\
#undef strref\n\
\n\
static const char * poolfilearr[] = {\n",
	  argv[0],
	  filename,
	  headername,
	  headername);
  char data[1024];
  while (fgets(data, 1024, fh))
    {
      int len = strlen(data);
      if (data[len - 1] == '\n')
	{
	  data[len - 1] = 0;
	  len --;
	}
      if (data[0] == '*')
	{
	  break;
	}
      // skip first bytes
      int o = 0;
      if (data[0] >= '0'
	  && data[0] <= '9'
	  && data[1] >= '0'
	  && data[1] <= '9')
	{
	  o = 2;
	}
      printf ("  \"");
      for (int i = o; i < len; i ++)
	{
	  if (data[i] == '"' || data[i] == '\\')
	    {
	      putchar ('\\');
	    }
	  if (data[i] == '?')
	    {
	      printf("\" \"");	// suppress trigraphs
	    }
	  putchar (data[i]);
	}
      printf ("\",\n");
    }
  fclose(fh);
  printf ("\
  0 };\n\
int miktexloadpoolstrings (int poolSize)\n\
{\n\
  const char * lpsz;\n\
  strnumber g = 0;\n\
  int size = 0;\n\
  for (int idx = 0; (lpsz = poolfilearr[idx]) != 0; ++ idx)\n\
    {\n\
      int len = strlen(lpsz);\n\
      size += len;\n\
      if (size >= poolSize)\n\
        {\n\
          return (0);\n\
        }\n");
  if (is_luatex)
    {
      printf ("\
      while (len-- > 0)\n\
        {\n\
          THEDATA(str_pool)[THEDATA(pool_ptr)++] = *lpsz++;\n\
        }\n\
      g = make_string();\n");
    }
  else
    {
      printf ("\
      while (len-- > 0)\n\
        {\n\
          THEDATA(strpool)[THEDATA(poolptr)++] = *lpsz++;\n\
        }\n\
      g = makestring();\n");
    }
  if (is_metapost || is_metafont)
    {
      printf ("\
      THEDATA(strref)[g] = 127;\n");
    }
  printf ("\
    }\n\
  return (g);\n\
}\n");
  return (EXIT_SUCCESS);
}
