/* FILE:    filenames.c
 * PURPOSE: some handy functions for working with TeXfiles
 * AUTHOR:  Piet Tutelaers (internet: rcpt@urc.tue.nl)
 * VERSION: 1.6 (Februari 1998)
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>	/* strchr(), strncpy() ... */
#include "basics.h"	/* basic definitions and fatal() */

/* for those systems that don't provide an ANSI strchr() */
#if defined(MIKTEX)
#define HAVE_STRCHR
#endif
#ifndef HAVE_STRCHR
static char *strchr(char *name, char c)
{
   while (*name != c && *name != '\0') name++; 
   if (*name == '\0') return NULL;
   else return name;
}
#endif

/* comparing names (system dependant) */
int equal(char *s, char *t)
{
#  ifndef UNIX
   while (tolower(*s) == tolower(*t)) {
      if (*s == '\0') break;
      s++; t++;
   }
#  else
   while (*s == *t) {
      if (*s == '\0' || *t == '\0')  break;
      s++; t++;
   }
#  endif
   if (*s == '\0' && *t == '\0') return 1;
   else return 0;
}

/*
 * Determine the extension
 */
char *extension(char *str) {
   char *p, *base;
   for (p = base = str; *p;)
#if defined(MIKTEX)
      if (ISDIRSEP(*p)) base = ++p; else ++p;
#else
      if (*p++ == DIRSEP) base = p;
#endif
   for (p = base ; *p; p++)
      if (*p == '.') break;
   return p;
}

/*
 * Construct filename by replacing the current extension (if available)
 * with supplied one.
 */
char *newname(char *name, char *ext)
{  
   char *e, *nn; int len1, len2;

   e = extension(name);
   if (equal(ext, e)) return name;
   len1 = strlen(name) - strlen(e);
   len2 = len1 + strlen(ext) + 1;
   nn = (char *) malloc(len2);
   if (nn == NULL) fatal("Out of memory\n");
   strncpy(nn, name, len1);
   strcpy(nn+len1, ext);
   return nn;
}

/*
 * Derived from BSD basename. We call it baseName to avoid clashes with
 * existing basename()'s. Consider everything before a DEVSEP as 
 * a devicename (eg. c:\win95\command.com or amigaTeX:pk).
 */
char *baseName(char *str, char *suffix){   
   char *p, *t; int len = 0;
   char *base;

   for (p = base = str; *p; p++) {
#if defined(MIKTEX)
      if (ISDIRSEP(*p) || *p == DEVSEP) { 
#else
      if (*p == DIRSEP || *p == DEVSEP) { 
#endif
         base = p+1; len = 0;
      }
      else len++;
   }
   if (suffix != NULL) {
      for (t = suffix; *t; ++t);
      do {
        len--; t--; p--;
        if (*t != *p) break;
        if (t == suffix) { char *bn;
            if (len == 0) return NULL;
            bn = malloc(len+1);
            if (bn == NULL) fatal("Out of memory\n");
            strncpy(bn, base, len);
            *(bn+len) = '\0'; /* RA */
            return bn;
         }
      } while (p > base);
   }
   return base;
}

/*
 * Return true if name can be the name of a PostScript resource
 * (no extension and no absolute pathname).
 */
int ps_resource(char *name) {
   if (strchr(name, '.')) return 0 ;
#if defined(MIKTEX)
   if (strchr(name, '/')) return 0 ;
   if (strchr(name, '\\')) return 0 ;
#else
   if (strchr(name, DIRSEP)) return 0 ;
#endif
   if (DEVSEP != 0 && strchr(name, DEVSEP)) return 0 ;
   return 1;
}
