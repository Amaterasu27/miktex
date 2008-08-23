/* miktex/TeXAndFriends/TeXMFApp.inl:				-*- C++ -*-

   Copyright (C) 1996-2008 Christian Schenk

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

#include <miktex/TeXAndFriends/WebAppInputLine.inl>

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
miktexgetdefaultdumpfilename (/*[out]*/ char * lpszPath)
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
miktexgetpoolfilename (/*[out]*/ char * lpszPath)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  strcpy_s (lpszPath,
	    MiKTeX::Core::BufferSizes::MaxPath,
	    THEAPP.GetPoolFileName());
#else
  strcpy (lpszPath, THEAPP.GetPoolFileName());
#endif
}

/* _________________________________________________________________________
     
   miktexgetstringat
   _________________________________________________________________________ */

inline
const TEXMFCHAR *
miktexgetstringat (/*[in]*/ int idx)
{
  MIKTEX_ASSERT (sizeof(TEXMFCHAR) == sizeof(THEDATA(strpool)[idx]));
  return (reinterpret_cast<TEXMFCHAR *>(&(THEDATA(strpool)[idx])));
}

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
  THEDATA(last) = THEAPP.InitializeBuffer(THEDATA(buffer));
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

template<class FileType>
inline
bool
miktexopenpoolfile (/*[in]*/ FileType & f)
{
  return (THEAPP.OpenPoolFile(f));
}

/* _________________________________________________________________________
     
   miktexreallocate
   _________________________________________________________________________ */

#define miktexreallocate(p, n) \
  THEAPP.Reallocate(#p, p, n, __FILE__, __LINE__)

/* _________________________________________________________________________
     
   miktexdump
   _________________________________________________________________________ */

template<typename FileType,
	 typename EleType>
inline
void
miktexdump (/*[in]*/ FileType &		f,
	    /*[in]*/ const EleType &	e,
	    /*[in]*/ size_t		n)
{
  THEAPP.Dump (f, e, n);
}

/* _________________________________________________________________________
     
   miktexdump
   _________________________________________________________________________ */

template<typename FileType,
	 typename EleType>
inline
void
miktexdump (/*[in]*/ FileType &		f,
	    /*[in]*/ const EleType &	e)
{
  THEAPP.Dump (f, e);
}

/* _________________________________________________________________________
     
   miktexdumpint
   _________________________________________________________________________ */

template<typename FileType>
inline
void
miktexdumpint (/*[in]*/ FileType &	f,
	       /*[in]*/ int		val)
{
  miktexdump (f, val);
}

/* _________________________________________________________________________
     
   miktexundump
   _________________________________________________________________________ */

template<typename FileType,
	 typename EleType>
inline
void
miktexundump (/*[in]*/ FileType &	f,
	      /*[out]*/ EleType &	e,
	      /*[in]*/ size_t		n)
{
  THEAPP.Undump (f, e, n);
}

/* _________________________________________________________________________
     
   miktexundump
   _________________________________________________________________________ */

template<typename FileType,
	 typename EleType>
inline
void
miktexundump (/*[in]*/ FileType &	f,
	      /*[out]*/ EleType &	e)
{
  THEAPP.Undump (f, e);
}

/* _________________________________________________________________________
     
   miktexundump
   _________________________________________________________________________ */

template<typename FileType,
	 typename LowType,
	 typename HighType,
	 typename EleType>
inline
void
miktexundump (/*[in]*/ FileType &	f,
	      /*[in]*/ LowType		low,
	      /*[in]*/ HighType 	high,
	      /*[out]*/ EleType &	e,
	      /*[in]*/ size_t		n)
{
  THEAPP.Undump (f,
		 static_cast<EleType>(low),
		 static_cast<EleType>(high),
		 e,
		 n);
}

/* _________________________________________________________________________
     
   miktexundump
   _________________________________________________________________________ */

template<typename FileType,
	 typename HighType,
	 typename EleType>
inline
void
miktexundump (/*[in]*/ FileType &	f,
	      /*[in]*/ HighType 	high,
	      /*[out]*/ EleType &	e,
	      /*[in]*/ size_t		n)
{
  THEAPP.Undump (f, static_cast<EleType>(high), e, n);
}

/* _________________________________________________________________________
     
   miktexundumpint
   _________________________________________________________________________ */

template<typename FileType>
inline
void
miktexundumpint (/*[in]*/ FileType &	f,
		 /*[out]*/ int &	val)
{
  miktexundump (f, val);
}

/* _________________________________________________________________________
     
   miktexcheckmemoryifdebug
   _________________________________________________________________________ */

inline
void
miktexcheckmemoryifdebug ()
{
#if defined(_DEBUG)
  THEAPP.CheckMemory ();
#endif
}

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// texmfapp.inl
