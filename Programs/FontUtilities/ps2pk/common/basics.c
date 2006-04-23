/* FILE:    basics.c
 * PURPOSE: basic functions
 * AUTHOR:  Piet Tutelaers
 * VERSION: 1.1 (Februari  1998)
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>   /* stat() */

/* provide a pointer to a cleanup() function in fatal() */
static void (*cleanup)() = NULL;

void set_cleanup(void (*func)()) {
   cleanup = func;
}

void reset_cleanup() {
   cleanup = NULL;
}

/* Give up ... */
void fatal(char *fmt, ...)
{  va_list args;

   if (cleanup != NULL) cleanup();
   va_start(args, fmt);
   vfprintf(stderr, fmt, args);
   va_end(args);
   exit(1);
}

/* Give a message ... */
void msg(char *fmt, ...)
{  va_list args;

   va_start(args, fmt);
   vfprintf(stderr, fmt, args);
   fflush(stderr);
   va_end(args);
}

/* default FILE opener */
#ifdef WIN32 /* FP added WIN32 */
FILE* (__cdecl * pfopen) (const char *, const char *) = fopen;
#else
FILE * (*pfopen)(const char *, const char *) = fopen;
#endif

/* A verbose fopen() function */
FILE *my_fopen(const char *path, const char *mode)
{
   FILE *F ;
   msg("Opening <%s> ", path) ;
   F = fopen(path, mode) ;
   if (F == 0) msg("failed\n") ;
   else msg("succeeded\n") ;
   return F ;
}

/* default stat-ter */
#ifdef WIN32 /* FP added WIN32 */
int (__cdecl * pstat)(const char *, struct stat *) = stat;
#else
int (*pstat)(const char *, struct stat *) = stat;
#endif

/* verbose stat function */
int my_stat(const char *path, struct stat *buf)
{
   int s;
   msg("stat <%s> ", path);
   s = stat(path, buf) ;
   if (s == 0) msg("succeeded\n");
   else msg("failed\n");
   return s ;
}
