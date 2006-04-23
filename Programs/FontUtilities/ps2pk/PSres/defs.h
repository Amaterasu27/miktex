/* FILE:    defs.h
 * PURPOSE: platform dependant or common definitions
 * AUTHOR:  Piet Tutelaers
 * VERSION: 1.0 (September 1995)
 */

/* TeX PS Resource database name and extension */
#if defined(MIKTEX)
#define PSRES_NAME	"psres.dpr"
#define PSRES_BACKUP	"psres.dpr~"
#define PSRES_EXT	".dpr"
#define PSRESOURCEPATH_DEFAULT ""
#define NULDEVICE	NUL
#endif
#ifdef MSDOS
#define PSRES_NAME	"psres.dpr"
#define PSRES_BACKUP	"psres.bak"
#define PSRES_EXT	".dpr"
#define PSRESOURCEPATH_DEFAULT ""
#define NULDEVICE	NUL
#endif

/* start defaults for UNIX */
#ifndef PSRES_NAME
#define PSRES_NAME	"PSres.upr"
#endif

#ifndef PSRES_BACKUP
#define PSRES_BACKUP	"PSres.upr~"
#endif

#ifndef PSRES_EXT
#define PSRES_EXT	".upr"
#endif

#ifndef PSRESOURCEPATH_DEFAULT
#define PSRESOURCEPATH_DEFAULT "."
#endif
/* end defaults for UNIX */

#if defined(MIKTEX)
#define PATHSEP ';'
#define DIRSEP '\\'
#define DIRSEPSTR "\\"
#define ESCAPECHAR '^'
#define RB "rb"
#define WB "wb"
#define ISDIRSEP(ch)	((ch) == '\\' || (ch) == '/')
#define ISPATH(s)	(ISDIRSEP((s)[0])		\
			 || (isalpha((s)[0])		\
			     && (s)[1] == ':'		\
			     && ISDIRSEP((s)[2])))
#define fncmp		miktex_pathcmp
#define fnncmp		miktex_pathncmp
#endif

#ifdef MSDOS
#define PATHSEP ';'
#define DIRSEP '\\'
#define ESCAPECHAR '!'
#define RB "rb"
#define WB "wb"
#endif

#ifndef PATHSEP
#define PATHSEP ':'
#endif

#ifndef DIRSEP
#define DIRSEP '/'
#endif

#if defined(MIKTEX)
#ifndef DIRSEPSTR
#define DIRSEPSTR "/"
#endif
#endif

#ifndef ESCAPECHAR
#define ESCAPECHAR '\\'
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

#ifndef NULLDEVICE
#define NULLDEVICE "/dev/null"
#endif
