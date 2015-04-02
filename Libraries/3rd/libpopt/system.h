#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(MIKTEX)

#  if defined(_WIN32) || defined(_WIN64)
#    include "win/config.h"
#    define X_OK 0
#    include <sys/types.h>
#    include <io.h>
#    include <process.h>
#    define getuid() 0
#    define geteuid() 0
#    define alloca _alloca
#  endif /* Windows */

#  if defined(MIKTEX_POPT_SHARED) && defined(_MSC_VER)
#    define MIKTEXPOPTAPI(type) __declspec(dllexport) type __cdecl
#    define MIKTEXPOPTCALLBACK __cdecl
#    if defined(__cplusplus)
#      define MIKTEXPOPTDATA(type) extern "C" __declspec(dllexport) type
#    else
#      define MIKTEXPOPTDATA(type) __declspec(dllexport) type
#    endif
#  elif defined(_MSC_VER)
#    define MIKTEXPOPTAPI(type) type __cdecl
#    define MIKTEXPOPTCALLBACK __cdecl
#    if defined(__cplusplus)
#      define MIKTEXPOPTDATA(type) extern "C" type
#    else
#      define MIKTEXPOPTDATA(type) type
#    endif
#  else
#    define MIKTEXPOPTAPI(type) type
#    define MIKTEXPOPTCALLBACK
#    if defined(MIKTEX_POPT_SHARED) && defined(__CYGWIN__)
#      define MIKTEXPOPTDATA(type) __declspec(dllexport) type
#    else
#      define MIKTEXPOPTDATA(type) extern type
#    endif
#  endif
#endif	/* MiKTeX */

#if defined (__GLIBC__) && defined(__LCLINT__)
/*@-declundef@*/
/*@unchecked@*/
extern __const __int32_t *__ctype_tolower;
/*@unchecked@*/
extern __const __int32_t *__ctype_toupper;
/*@=declundef@*/
#endif

#include <ctype.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>

#if HAVE_MCHECK_H 
#include <mcheck.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef __NeXT
/* access macros are not declared in non posix mode in unistd.h -
 don't try to use posix on NeXTstep 3.3 ! */
#include <libc.h>
#endif

#if defined(__LCLINT__)
/*@-declundef -incondefs -redecl@*/ /* LCL: missing annotation */
/*@only@*/ void * alloca (size_t __size)
	/*@ensures MaxSet(result) == (__size - 1) @*/
	/*@*/;
/*@=declundef =incondefs =redecl@*/
#endif

/* AIX requires this to be the first thing in the file.  */ 
#ifndef __GNUC__
# if HAVE_ALLOCA_H
#  include <alloca.h>
# else
#  ifdef _AIX
#pragma alloca
#  else
#   ifndef alloca /* predefined by HP cc +Olibcalls */
char *alloca ();
#   endif
#  endif
# endif
#elif defined(__GNUC__) && defined(__STRICT_ANSI__)
#define alloca __builtin_alloca
#endif

/*@-redecl -redef@*/
/*@mayexit@*/ /*@only@*/ char * xstrdup (const char *str)
	/*@*/;
/*@=redecl =redef@*/

#if HAVE_MCHECK_H && defined(__GNUC__)
#define	vmefail()	(fprintf(stderr, "virtual memory exhausted.\n"), exit(EXIT_FAILURE), NULL)
#define xstrdup(_str)   (strcpy((malloc(strlen(_str)+1) ? : vmefail()), (_str)))
#else
#define	xstrdup(_str)	strdup(_str)
#endif  /* HAVE_MCHECK_H && defined(__GNUC__) */


#include "popt.h"
