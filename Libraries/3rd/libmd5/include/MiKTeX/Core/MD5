/* MD5.H - header file for MD5C.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD5 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD5 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.  
                                                                    
   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.  
                                                                    
   These notices must be retained in any copies of any part of this
   documentation and/or software.  
 */

#ifndef _MD5_H_
#define _MD5_H_ 1

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(MIKTEX)

// get wchar_t
#if defined(__cplusplus)
#  include <cstddef>
#else
#  include <stddef.h>
#endif

#if defined(HAVE_INTTYPES_H)
#   include <inttypes.h>
#endif

#if defined(HAVE_STDINT_H)
#  include <stdint.h>
#endif

#if ! defined(MIKTEXMD5API)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define MIKTEXMD5API(type) __declspec(dllimport) type __stdcall
#  elif defined(_MSC_VER)
#    define MIKTEXMD5API(type) type __stdcall
#  else
#    define MIKTEXMD5API(type) type
#  endif
#endif

#if defined(__cplusplus)
namespace MiKTeX {
  namespace Core {
#endif

#endif	/* MiKTeX */

/* MD5 context. */
typedef struct {
#if defined(MIKTEX)
#  if defined(_MSC_VER)
  unsigned __int32 state[4];
  unsigned __int32 count[2];
#  else
  uint32_t state[4];
  uint32_t count[2];
#endif
#else
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
#endif
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

#if defined(MIKTEX)
 MIKTEXMD5API(void) MD5Init (/*[out]*/ MD5_CTX *);
 MIKTEXMD5API(void) MD5Update (/*[out]*/ MD5_CTX *,
			       /*[in]*/ const void *,
			       /*[in]*/ unsigned int);
 MIKTEXMD5API(void) MD5Final (/*[out]*/ unsigned char [16],
			      /*[in]*/ MD5_CTX *);
#else
void MD5Init PROTO_LIST ((MD5_CTX *));
void MD5Update PROTO_LIST
  ((MD5_CTX *, unsigned char *, unsigned int));
void MD5Final PROTO_LIST ((unsigned char [16], MD5_CTX *));
#endif

#if defined(MIKTEX) && defined(__cplusplus)
  } /* MD5 */
} /* namespace MiKTeX */
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif
