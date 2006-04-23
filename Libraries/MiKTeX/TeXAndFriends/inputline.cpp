/* inputline.cpp:

   Copyright (C) 1996-2006 Christian Schenk
 
   This file is part of the MiKTeX TeXMF Library.

   The MiKTeX TeXMF Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX TeXMF Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX TeXMF Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________
     
   WebAppInputLine::WebAppInputLine
   _________________________________________________________________________ */

WebAppInputLine::WebAppInputLine ()
  : inputFileType (FileType::PROGRAMTEXTFILE)
{
}

/* _________________________________________________________________________
     
   WebAppInputLine::Init
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
WebAppInputLine::Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
{
  WebApp::Init (lpszProgramInvocationName);
}

/* _________________________________________________________________________
   
   WebAppInputLine::Finalize
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
WebAppInputLine::Finalize ()
{
  auxDirectory = T_("");
  fqNameOfFile = T_("");
  nameOfFile = T_("");
  outputDirectory = T_("");
  WebApp::Finalize ();
}

/* _________________________________________________________________________

   AutoGZ
   _________________________________________________________________________ */

class gzclose_
{
public:
  void
  operator() (/*[in]*/ gzFile gz)
  {
    gzclose (gz);
  }
};

typedef AutoResource<gzFile, gzclose_> AutoGZ;

/* _________________________________________________________________________

   AutoBZ2
   _________________________________________________________________________ */

class BZ2_bzclose_
{
public:
  void
  operator() (/*[in]*/ BZFILE * bz2)
  {
    BZ2_bzclose (bz2);
  }
};

typedef AutoResource<BZFILE *, BZ2_bzclose_> AutoBZ2;

/* _________________________________________________________________________

   GzReaderThread
   _________________________________________________________________________ */

struct CReaderThreadArg
{
  gzFile	gzin;
  BZFILE *	bzin;
  FILE *	fileout;
};

STATICFUNC(unsigned)
__stdcall
GzReaderThread (/*[in]*/ void * pv)
{
  try
    {
      auto_ptr<CReaderThreadArg>
	prta (reinterpret_cast<CReaderThreadArg*>(pv));
      AutoFILE autoClose (prta->fileout);
      assert (prta->gzin != 0 || prta->bzin != 0);
      assert (prta->gzin == 0 || prta->bzin == 0);
      assert (prta->fileout != 0);
      char buf[4096];
      int len;
      if (prta->gzin != 0)
	{
	  AutoGZ autoClose2 (prta->gzin);
	  while (! ferror(prta->fileout)
		 && (len = gzread(prta->gzin, buf, ARRAY_SIZE(buf))) > 0)
	    {
	      fwrite (buf, 1, len, prta->fileout);
	    }
	  if (len < 0)
	    {
	      FATAL_MIKTEX_ERROR (T_("GzReaderThread"),
				  T_("gzread() failed for some reason."),
				  0);
	    }
	}
      else
	{
	  AutoBZ2 autoClose3 (prta->bzin);
	  while (! ferror(prta->fileout)
		 && (len = BZ2_bzread(prta->bzin, buf, ARRAY_SIZE(buf))) > 0)
	    {
	      fwrite (buf, 1, len, prta->fileout);
	    }
	  int bzerr;
	  BZ2_bzerror (prta->bzin, &bzerr);
	  if (bzerr != BZ_OK)
	    {
	      FATAL_MIKTEX_ERROR (T_("GzReaderThread"),
				  T_("BZ2_bzread() failed for some reason."),
				  0);
	    }
	}
      if (ferror(prta->fileout))
	{
	  FATAL_CRT_ERROR (T_("fwrite"), 0);
	}
    }
  catch (const exception &)
    {
    }
  return (0);
}

/* _________________________________________________________________________
   
   OpenGzInputStream
   _________________________________________________________________________ */

STATICFUNC(FILE *)
OpenGzInputStream (/*[in]*/ const PathName & path)
{
  gzFile gzin = 0;
  BZFILE * bzin = 0;
  AutoGZ autoGz;
  AutoBZ2 autoBz2;
  if (path.HasExtension(T_(".gz")))
    {
      gzin = gzopen(path.Get(), T_("rb"));
      if (gzin == 0)
	{
	  FATAL_MIKTEX_ERROR (T_("OpenGzInputStream"),
			      T_("gzopen() failed for some."),
			      path.Get());
	}
      autoGz.Attach (gzin);
    }
  else
    {
      bzin = BZ2_bzopen(path.Get(), T_("rb"));
      if (bzin == 0)
	{
	  FATAL_MIKTEX_ERROR (T_("OpenGzInputStream"),
			      T_("BZ2_bzopen() failed for some."),
			      path.Get());
	}
      autoBz2.Attach (bzin);
    }
  int handles[2];
  if (_pipe(handles, 4096, _O_TEXT) != 0)
    {
      FATAL_CRT_ERROR (T_("_pipe"), 0);
    }
  FILE * pfileIn = _fdopen(handles[0], T_("rb"));
  if (pfileIn == 0)
    {
      int err = errno;
      _close (handles[0]);
      _close (handles[1]);
      FATAL_CRT_ERROR_2 (T_("_fdopen"), err, 0);
    }
  FILE * pfileOut = _fdopen(handles[1], T_("wb"));
  if (pfileOut == 0)
    {
      int err = errno;
      fclose (pfileIn);
      _close (handles[1]);
      FATAL_CRT_ERROR_2 (T_("_fdopen"), err, 0);
    }
  CReaderThreadArg * pthreadarg = new CReaderThreadArg;
  pthreadarg->gzin = gzin;
  pthreadarg->bzin = bzin;
  pthreadarg->fileout = pfileOut;
  unsigned threadID;
  HANDLE hThread =
    reinterpret_cast<HANDLE>
    (_beginthreadex(0, 0, GzReaderThread, pthreadarg, 0, &threadID));
  if (hThread == 0)
    {
      int err = errno;
      fclose (pfileIn);
      fclose (pfileOut);
      FATAL_CRT_ERROR_2 (T_("_beginthreadex"), err, 0);
    }
  CloseHandle (hThread);
  autoGz.Detach ();
  autoBz2.Detach ();
  return (pfileIn);
}

/* _________________________________________________________________________

   WebAppInputLine::UnmangleNameOfFile
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
WebAppInputLine::UnmangleNameOfFile (/*[out]*/ MIKTEXCHAR *	lpszTo,
				     /*[in]*/ const MIKTEXCHAR * lpszFrom)
{
  assert (lpszTo != 0);
  C4PASSERTSTRING (lpszFrom);
  size_t l = StrLen(lpszFrom);
  size_t i;
  for (i = 0; i < l; ++ i)
    {
      if (lpszFrom[i] == T_('*'))
	{
	  lpszTo[i] = T_(' ');
	}
      else if (lpszFrom[i] == T_('?'))
	{
	  lpszTo[i] = T_('~');
	}
      else
	{
	  lpszTo[i] = lpszFrom[i];
	}
    }
  lpszTo[i] = 0;
}

/* _________________________________________________________________________

   WebAppInputLine::OpenOutputFile
   _________________________________________________________________________ */

MIKTEXMFAPI(bool)
WebAppInputLine::OpenOutputFile (/*[in]*/ C4P::FileRoot &	f,
				 /*[in]*/ const MIKTEXCHAR *	lpszPath,
				 /*[in]*/ FileShare		share,
				 /*[in]*/ bool			text)
{
  C4PASSERTSTRING (lpszPath);
  UnmangleNameOfFile (nameOfFile.GetBuffer(), lpszPath);
  bool isAuxFile =
    ! (nameOfFile.HasExtension(T_(".dvi")) // <fixme/>
       || nameOfFile.HasExtension(T_(".pdf")));
  PathName path;
  if (isAuxFile && ! auxDirectory.Empty())
    {
      path.Set (auxDirectory, nameOfFile);
      lpszPath = path.Get();
    }
  else if (outputDirectory[0] != 0)
    {
      path.Set (outputDirectory, nameOfFile);
      lpszPath = path.Get();
    }
  else
    {
      lpszPath = nameOfFile.Get();
    }
  FILE * pfile =
    pSession->TryOpenFile(lpszPath,
			  FileMode::Create,
			  FileAccess::Write,
			  text,
			  share);
  if (pfile == 0)
    {
      return (false);
    }
  f.Attach (pfile, true);
  return (true);
}

/* _________________________________________________________________________
   
   WebAppInputLine::OpenInputFile
   _________________________________________________________________________ */

MIKTEXMFAPI(bool)
WebAppInputLine::OpenInputFile (/*[in]*/ C4P::C4P_text &	f,
				/*[in]*/ const MIKTEXCHAR *	lpszFileName)
{
  C4PASSERTSTRING (lpszFileName);

  UnmangleNameOfFile (nameOfFile.GetBuffer(), lpszFileName);

  if (! pSession->FindFile(nameOfFile, inputFileType, fqNameOfFile))
    {
      return (false);
    }

  bool isCompressedFile =
    (fqNameOfFile.HasExtension(T_(".gz"))
     || fqNameOfFile.HasExtension(T_(".bz2")));
  
  FILE * pfile;
  
  if (isCompressedFile)
    {
      pfile = OpenGzInputStream(fqNameOfFile);
    }
  else
    {
#if 0
      FileShare share = FileShare::Read;
#else
      FileShare share = FileShare::ReadWrite;
#endif
      pfile =
	pSession->TryOpenFile(fqNameOfFile.Get(),
			      FileMode::Open,
			      FileAccess::Read,
			      false,
			      share);
    }

  if (pfile == 0)
    {
      return (false);
    }

  f.Attach (pfile, true);

#ifdef PASCAL_TEXT_IO
  get (f);
#endif
  
  return (true);
}

/* _________________________________________________________________________

   WebAppInputLine::HandleEof
   _________________________________________________________________________ */

#if 0
MIKTEXMFAPI(void)
WebAppInputLine::HandleEof (/*[in]*/ FILE * pfile)
  const
{
  assert (pfile != 0);
#if 1
  while (GetC(pfile) != MIKTEXEOF)
    {
      ;
    }
#else
  fseek (pfile, SEEK_END, 0);
#endif
}
#endif

  /* _______________________________________________________________________
     
     WebAppInputLine::TouchJobOutputFile
     _______________________________________________________________________ */

MIKTEXMFAPI(void)
WebAppInputLine::TouchJobOutputFile (/*[in]*/ FILE *)
  const
{
}
