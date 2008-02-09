#ifndef _SYSTEM_H_

#define _SYSTEM_H_

#if defined(MIKTEX)
#include <miktex/Core/Core>
#include <gnu-miktex.h>
#include <miktex/KPSE/Emulation>
#endif
#include <kpathsea/c-auto.h>
#include <kpathsea/kpathsea.h>

#ifdef WIN32
#  undef ERROR
#  undef NO_ERROR
#  undef RGB
#  undef CMYK
#  undef SETLINEJOIN
#  undef SETMITERLIMIT
#  pragma warning(disable : 4101 4018)
#else
#  define __cdecl
#  define CDECL
#endif /* WIN32 */

#endif /* _SYSTEM_H_ */



