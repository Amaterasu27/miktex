/* GLOBAL.H - RSAREF types and constants

/* Copyright (C) RSA Laboratories, a division of RSA Data Security,
     Inc., created 1991. All rights reserved.
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_ 1

/* PROTOTYPES should be set to one if and only if the compiler supports
     function argument prototyping.
   The following makes PROTOTYPES default to 1 if it has not already been
     defined as 0 with C compiler flags.
 */
#ifndef PROTOTYPES
#define PROTOTYPES 1
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

#if defined(MIKTEX)
#  include "config.h"
#  if defined(HAVE_INTTYPES_H)
#    include <inttypes.h>
#  endif
#  if defined(HAVE_STDINT_H)
#    include <stdint.h>
#  endif
#  if defined(_MSC_VER)
typedef unsigned __int32 UINT4;
#  else
typedef uint32_t UINT4;
#  endif
#else
/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;
#endif

#ifndef NULL_PTR
#define NULL_PTR ((POINTER)0)
#endif

#ifndef UNUSED_ARG
#define UNUSED_ARG(x) x = *(&x);
#endif

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
   If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
     returns an empty list.  
 */
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

#endif /* end _GLOBAL_H_ */
