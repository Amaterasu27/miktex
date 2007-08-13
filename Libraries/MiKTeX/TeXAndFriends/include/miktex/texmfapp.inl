/* texmfapp.inl: TeX/MF common inliners			  	-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(GUARD_2DC13B85F68F954EA2F85204AD7808F6_)
#define GUARD_2DC13B85F68F954EA2F85204AD7808F6_

#if ! defined(THEAPP)
#  error THEAPP not defined
#endif

#if ! defined(THEDATA)
#  error THEDATA not defined
#endif

#include <miktex/inputline.inl>

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

/* _________________________________________________________________________
     
   miktexcstyleerrormessagesp
   _________________________________________________________________________ */

inline
bool
miktexcstyleerrormessagesp ()
{
  return (THEAPP.CStyleErrorMessagesP());
}

/* _________________________________________________________________________
     
   miktexgetdefaultdumpfilename
   _________________________________________________________________________ */

inline
void
miktexgetdefaultdumpfilename (/*[in]*/ MIKTEXCHAR * lpszPath)
{
  THEAPP.GetDefaultMemoryDumpFileName (lpszPath);
}

/* _________________________________________________________________________
     
   miktexgetinteraction
   _________________________________________________________________________ */

inline
int
miktexgetinteraction ()
{
  return (THEAPP.GetInteraction());
}

/* _________________________________________________________________________
     
   miktexgetjobname
   _________________________________________________________________________ */

inline
int
miktexgetjobname ()
{
  return (THEAPP.GetJobName());
}

/* _________________________________________________________________________
     
   miktexgetpoolfilename
   _________________________________________________________________________ */

inline
void
miktexgetpoolfilename (/*[in]*/ MIKTEXCHAR * lpszPath)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  _tcscpy_s (lpszPath, _MAX_PATH, THEAPP.GetPoolFileName());
#else
  _tcscpy (lpszPath, THEAPP.GetPoolFileName());
#endif
}

/* _________________________________________________________________________
     
   miktexgetstringat
   _________________________________________________________________________ */

#if ! (defined(MIKTEX_OMEGA) || defined(MIKTEX_EOMEGA))
inline
const MIKTEXCHAR *
miktexgetstringat (/*[in]*/ int idx)
{
  return (const_cast<const MIKTEXCHAR *>
	  (reinterpret_cast<MIKTEXCHAR *>
	   (&(THEDATA(strpool)[idx]))));
}
#endif

/* _________________________________________________________________________
     
   miktexhaltonerrorp
   _________________________________________________________________________ */

inline
bool
miktexhaltonerrorp ()
{
  return (THEAPP.HaltOnErrorP()); 
}

/* _________________________________________________________________________
     
   miktexinitializebuffer
   _________________________________________________________________________ */

inline
void
miktexinitializebuffer ()
{
#if defined(MIKTEX_OMEGA) || defined(MIKTEX_EOMEGA)
  THEDATA(last) = THEAPP.InitializeBufferW(THEDATA(buffer));
#else
  THEDATA(last) = THEAPP.InitializeBufferA(THEDATA(buffer));
#endif
}

/* _________________________________________________________________________
     
   miktexinvokeeditor
   _________________________________________________________________________ */

inline
void
miktexinvokeeditor (/*[in]*/ int		editFileName,
		    /*[in]*/ int		editFileNameLength,
		    /*[in]*/ int		editLineNumber,
		    /*[in]*/ int		transcriptFileName,
		    /*[in]*/ int		transcriptFileNameLength)
{
  THEAPP.InvokeEditor (editFileName,
		       editFileNameLength,
		       editLineNumber,
		       transcriptFileName,
		       transcriptFileNameLength);
}

/* _________________________________________________________________________
     
   miktexinvokeeditor
   _________________________________________________________________________ */

inline
void
miktexinvokeeditor (/*[in]*/ int		editFileName,
		    /*[in]*/ int		editFileNameLength,
		    /*[in]*/ int		editLineNumber)
{
  THEAPP.InvokeEditor (editFileName,
		       editFileNameLength,
		       editLineNumber,
		       0,
		       0);
}

/* _________________________________________________________________________
     
   miktexisinitprogram
   _________________________________________________________________________ */

inline
bool
miktexisinitprogram ()
{
  return (THEAPP.IsInitProgram());
}

/* _________________________________________________________________________
     
   miktexmakefullnamestring
   _________________________________________________________________________ */

inline
int
miktexmakefullnamestring ()
{
  return (THEAPP.MakeFullNameString());
}

/* _________________________________________________________________________
     
   miktexontexmffinishjob
   _________________________________________________________________________ */

inline
void
miktexontexmffinishjob ()
{
  THEAPP.OnTeXMFFinishJob ();
}

/* _________________________________________________________________________
     
   miktexontexmfinitialize
   _________________________________________________________________________ */

inline
void
miktexontexmfinitialize ()
{
  THEAPP.OnTeXMFInitialize ();
}

/* _________________________________________________________________________
     
   miktexontexmfstartjob
   _________________________________________________________________________ */

inline
void
miktexontexmfstartjob ()
{
  THEAPP.OnTeXMFStartJob ();
}

/* _________________________________________________________________________
     
   miktexopenpoolfile
   _________________________________________________________________________ */

template<class T>
inline
bool
miktexopenpoolfile (/*[in]*/ T & f)
{
  return (THEAPP.OpenPoolFile(f));
}

/* _________________________________________________________________________
     
   miktexreallocate
   _________________________________________________________________________ */

template<typename T>
inline
T *
miktexreallocate (/*[in,out]*/ T * &	p,
		  /*[in]*/ size_t	n)
{
  return (THEAPP.Reallocate(p, n));
}

/* _________________________________________________________________________
     
   miktexdump
   _________________________________________________________________________ */

template<typename FILE_,
	 typename ELETYPE_>
inline
void
miktexdump (/*[in]*/ FILE_ &		f,
	    /*[in]*/ const ELETYPE_ &	e,
	    /*[in]*/ size_t		n)
{
  THEAPP.Dump (f, e, n);
}

/* _________________________________________________________________________
     
   miktexdump
   _________________________________________________________________________ */

template<typename FILE_,
	 typename ELETYPE_>
inline
void
miktexdump (/*[in]*/ FILE_ &		f,
	    /*[in]*/ const ELETYPE_ &	e)
{
  THEAPP.Dump (f, e);
}

/* _________________________________________________________________________
     
   miktexundump
   _________________________________________________________________________ */

template<typename FILE_,
	 typename ELETYPE_>
inline
void
miktexundump (/*[in]*/ FILE_ &		f,
	      /*[out]*/ ELETYPE_ &	e,
	      /*[in]*/ size_t		n)
{
  THEAPP.Undump (f, e, n);
}

/* _________________________________________________________________________
     
   miktexundump
   _________________________________________________________________________ */

template<typename FILE_,
	 typename ELETYPE_>
inline
void
miktexundump (/*[in]*/ FILE_ &		f,
	      /*[out]*/ ELETYPE_ &	e)
{
  THEAPP.Undump (f, e);
}

/* _________________________________________________________________________
     
   miktexundump
   _________________________________________________________________________ */

template<typename FILE_,
	 typename LOWTYPE_,
	 typename HIGHTYPE_,
	 typename ELETYPE_>
inline
void
miktexundump (/*[in]*/ FILE_ &		f,
	      /*[in]*/ LOWTYPE_ 	low,
	      /*[in]*/ HIGHTYPE_ 	high,
	      /*[out]*/ ELETYPE_ &	e,
	      /*[in]*/ size_t		n)
{
  THEAPP.Undump (f,
		 static_cast<ELETYPE_>(low),
		 static_cast<ELETYPE_>(high),
		 e,
		 n);
}

/* _________________________________________________________________________
     
   miktexundump
   _________________________________________________________________________ */

template<typename FILE_,
	 typename HIGHTYPE_,
	 typename ELETYPE_>
inline
void
miktexundump (/*[in]*/ FILE_ &		f,
	      /*[in]*/ HIGHTYPE_ 	high,
	      /*[out]*/ ELETYPE_ &	e,
	      /*[in]*/ size_t		n)
{
  THEAPP.Undump (f, static_cast<ELETYPE_>(high), e, n);
}

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// texmfapp.inl
