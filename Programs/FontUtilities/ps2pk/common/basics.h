/* FILE:    basics.h
 * PURPOSE: basic definitions
 * AUTHOR:  Piet Tutelaers
 * VERSION: 1.1 (Februari  1998)
 */


#ifndef NOBASICS

#if defined(MIKTEX_WINDOWS)
#define PATHSEP ';'
#define DIRSEP '\\'
#define DEVSEP ':'
#define ESCAPECHAR '!'
#define RECURSIVE "//"
#define PSRES_NAME	"psres.dpr"
#define RB "rb"
#define WB "wb"
#define ISDIRSEP(ch)	((ch) == '\\' || (ch) == '/')
#define ISPATH(s)	(ISDIRSEP((s)[0])		\
			 || (isalpha((s)[0])		\
			     && (s)[1] == DEVSEP	\
			     && ISDIRSEP((s)[2])))
#define fncmp		miktex_pathcmp
#endif

#if ! defined(MIKTEX_WINDOWS)
#if defined(MSDOS) || defined(WIN32) /* FP added WIN32 */
#  define PATHSEP ';'
#  define DIRSEP '\\'
#  define DEVSEP ':'
#  define ESCAPECHAR '!'
#  define RECURSIVE "!!"
#  define PSRES_NAME	"psres.dpr"
#  define RB "rb"
#  define WB "wb"
#endif
#endif
 
#ifndef PATHSEP
#define PATHSEP ':'
#endif

#ifndef DIRSEP
#define DIRSEP '/'
#endif

#ifndef DEVSEP /* no logical devices on UNIX */
#define DEVSEP 0
#endif

#ifndef ESCAPECHAR
#define ESCAPECHAR '\\'
#endif

#ifndef RECURSIVE
#define RECURSIVE "//"
#endif

/* TeX PS Resource database name */
#ifndef PSRES_NAME
#define PSRES_NAME	"PSres.upr"
#endif

#ifndef RB
#define RB "r"
#endif

#ifndef WB
#define WB "w"
#endif

#if defined(MIKTEX)
#ifndef fncmp
#define fncmp strcmp
#endif
#ifndef ISDIRSEP
#define ISDIRSEP(ch) ((ch) == DIRSEP)
#endif
#ifndef ISPATH
#define ISPATH(s) ISDIRSEP((s)[0])
#endif
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 256
#endif

#ifndef MAXSTRLEN
#define MAXSTRLEN 256
#endif

#define NOBASICS
#endif

#include <stdarg.h>

/* 
 * Set_cleanup(cleanup) installs a cleanup() function which is called
 * before calling exit() in fatal(). Reset_cleanup() resets to NULL
 * funcion.
 */
void set_cleanup(void (*func)());
void reset_cleanup();
void fatal(char *fmt, ...);
void msg(char *fmt, ...);

/* For debugging purposes it is handy to have a fopen() function that
 * shows which files are opened. The verbose my_fopen() can be installed
 * by a user program by assigning it to the pfopen function pointer
 *    pfopen = my_fopen
 */

#include <stdio.h>
extern FILE * (*pfopen)(const char *, const char *);
FILE * my_fopen(const char *, const char *);

/* For debugging purposes it is handy to have a stat() function that
 * shows which files it looks at. The verbose my_stat() can be installed
 * by a user program by assigning it to the pstat function pointer
 *    pstat = my_stat
 */
#include <sys/types.h>  /* struct stat */
#include <sys/stat.h>   /* stat() */

extern int (*pstat)(const char *, struct stat *);
int my_stat(const char *path, struct stat *buf);
