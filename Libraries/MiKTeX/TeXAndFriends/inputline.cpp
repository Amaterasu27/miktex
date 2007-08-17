/* inputline.cpp:

   Copyright (C) 1996-2007 Christian Schenk
 
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
  enablePipes = false;
}

/* _________________________________________________________________________
   
   WebAppInputLine::Finalize
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
WebAppInputLine::Finalize ()
{
  auxDirectory = T_("");
  fqNameOfFile = T_("");
  lastInputFileName = T_("");
  outputDirectory = T_("");
  WebApp::Finalize ();
}

/* _________________________________________________________________________
   
   WebAppInputLine::AddOptions
   _________________________________________________________________________ */

enum {
  OPT_DISABLE_PIPES,
  OPT_ENABLE_PIPES,
};

MIKTEXMFAPI(void)
WebAppInputLine::AddOptions ()
{
  WebApp::AddOptions ();

  optBase = static_cast<int>(GetOptions().size());

  AddOption (T_("enable-pipes\0\
Enable input (output) from (to) processes."),
	     FIRST_OPTION_VAL + optBase + OPT_ENABLE_PIPES);

  AddOption (T_("disable-pipes\0\
Disable input (output) from (to) processes."),
	     FIRST_OPTION_VAL + optBase + OPT_DISABLE_PIPES);
}

/* _________________________________________________________________________

   WebAppInputLine::ProcessOption
   _________________________________________________________________________ */

MIKTEXMFAPI(bool)
WebAppInputLine::ProcessOption (/*[in]*/ int			opt,
				/*[in]*/ const MIKTEXCHAR *	lpszOptArg)
{
  bool done = true;

  switch (opt - FIRST_OPTION_VAL - optBase)
    {

    case OPT_DISABLE_PIPES:
      enablePipes = false;
      break;

    case OPT_ENABLE_PIPES:
      enablePipes = true;
      break;

    default:
      done = WebApp::ProcessOption(opt, lpszOptArg);
      break;
    }

  return (done);
}

/* _________________________________________________________________________

   WebAppInputLine::MangleNameOfFile
   _________________________________________________________________________ */

PathName
WebAppInputLine::MangleNameOfFile (/*[in]*/ const char * lpszFrom)
{
  PathName ret;
  char * lpszTo = ret.GetBuffer();
  MIKTEX_ASSERT_STRING (lpszFrom);
  size_t len = StrLen(lpszFrom);
  if (len >= ret.GetSize())
    {
      INVALID_ARGUMENT (T_("WebAppInputLine::MangleNameOfFile"), lpszFrom);
    }
  size_t idx;
  for (idx = 0; idx < len; ++ idx)
    {
      if (lpszFrom[idx] == ' ')
	{
	  lpszTo[idx] = '*';
	}
      else if (lpszFrom[idx] == '~')
	{
	  lpszTo[idx] = '?';
	}
      else if (lpszFrom[idx] == '\\')
	{
	  lpszTo[idx] = '/';
	}
      else
	{
	  lpszTo[idx] = lpszFrom[idx];
	}
    }
  lpszTo[idx] = 0;
  return (ret);
}

/* _________________________________________________________________________

   UnmangleNameOfFile_
   _________________________________________________________________________ */

template<typename CharType>
static
PathName
UnmangleNameOfFile_ (/*[in]*/ const CharType * lpszFrom)
{
  PathName ret;
  char * lpszTo = ret.GetBuffer();
  MIKTEX_ASSERT_STRING (lpszFrom);
  size_t len = StrLen(lpszFrom);
  if (len >= ret.GetSize())
    {
      INVALID_ARGUMENT (T_("UnmangleNameOfFile_"), 0/*lpszFrom*/);
    }
  size_t idx;
  for (idx = 0; idx < len; ++ idx)
    {
      if (lpszFrom[idx] == '*')
	{
	  lpszTo[idx] = ' ';
	}
      else if (lpszFrom[idx] == '?')
	{
	  lpszTo[idx] = '~';
	}
      else
	{
	  lpszTo[idx] = lpszFrom[idx];
	}
    }
  lpszTo[idx] = 0;
  return (ret);
}

/* _________________________________________________________________________

   WebAppInputLine::UnmangleNameOfFile
   _________________________________________________________________________ */

MIKTEXMFAPI(PathName)
WebAppInputLine::UnmangleNameOfFile (/*[in]*/ const char * lpszFrom)
{
  return (UnmangleNameOfFile_(lpszFrom));
}

/* _________________________________________________________________________

   WebAppInputLine::UnmangleNameOfFile
   _________________________________________________________________________ */

MIKTEXMFAPI(PathName)
WebAppInputLine::UnmangleNameOfFile (/*[in]*/ const wchar_t * lpszFrom)
{
  return (UnmangleNameOfFile_(lpszFrom));
}

/* _________________________________________________________________________

   WebAppInputLine::OpenOutputFile
   _________________________________________________________________________ */

MIKTEXMFAPI(bool)
WebAppInputLine::OpenOutputFile (/*[in]*/ C4P::FileRoot &	f,
				 /*[in]*/ const char *		lpszPath,
				 /*[in]*/ FileShare		share,
				 /*[in]*/ bool			text)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  FILE * pfile = 0;
  if (enablePipes && lpszPath[0] == '|')
    {
      pfile =
	pSession->OpenFile(lpszPath + 1,
			   FileMode::Command,
			   FileAccess::Write,
			   false);
    }
  else
    {
      PathName unmangled = UnmangleNameOfFile(lpszPath);
      bool isAuxFile =
	! (unmangled.HasExtension(".dvi") // <fixme/>
	   || unmangled.HasExtension(".pdf"));
      PathName path;
      if (isAuxFile && ! auxDirectory.Empty())
	{
	  path.Set (auxDirectory, unmangled);
	  lpszPath = path.Get();
	}
      else if (outputDirectory[0] != 0)
	{
	  path.Set (outputDirectory, unmangled);
	  lpszPath = path.Get();
	}
      else
	{
	  lpszPath = unmangled.Get();
	}
      pfile =
	pSession->TryOpenFile(lpszPath,
			      FileMode::Create,
			      FileAccess::Write,
			      text,
			      share);
    }
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
WebAppInputLine::OpenInputFile (/*[in]*/ C4P::FileRoot &	f,
				/*[in]*/ const char *		lpszFileName)
{
  MIKTEX_ASSERT_STRING (lpszFileName);

  FILE * pfile = 0;

  if (enablePipes && lpszFileName[0] == '|')
    {
      pfile =
	pSession->OpenFile(lpszFileName + 1,
			   FileMode::Command,
			   FileAccess::Read,
			   false);
    }
  else
    {
      if (! pSession->FindFile(UnmangleNameOfFile(lpszFileName),
			       GetInputFileType(),
			       fqNameOfFile))
	{
	  return (false);
	}
      
      try
	{
	  if (fqNameOfFile.HasExtension(".gz"))
	    {
	      CommandLineBuilder cmd ("zcat");
	      cmd.AppendArgument (fqNameOfFile);
	      pfile =
		pSession->OpenFile(cmd.Get(),
				   FileMode::Command,
				   FileAccess::Read,
				   false);
	    }
	  else if (fqNameOfFile.HasExtension(".bz2"))
	    {
	      CommandLineBuilder cmd ("bzcat");
	      cmd.AppendArgument (fqNameOfFile);
	      pfile =
		pSession->OpenFile(cmd.Get(),
				   FileMode::Command,
				   FileAccess::Read,
				   false);
	    }
	  else
	    {
#if 0
	      FileShare share = FileShare::Read;
#else
	      FileShare share = FileShare::ReadWrite;
#endif
	      pfile =
		pSession->OpenFile(fqNameOfFile.Get(),
				   FileMode::Open,
				   FileAccess::Read,
				   false,
				   share);
	    }
	}
#if defined(MIKTEX_WINDOWS)
      catch (const SharingViolationException &)
	{
	}
#endif
      catch (const UnauthorizedAccessException &)
	{
	}
      catch (const FileNotFoundException &)
	{
	}
    }

  if (pfile == 0)
    {
      return (false);
    }

  f.Attach (pfile, true);

#ifdef PASCAL_TEXT_IO
  not_implemented ();
  get (f);
#endif

  lastInputFileName = lpszFileName;
  
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
  MIKTEX_ASSERT (pfile != 0);
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

/* _________________________________________________________________________
     
   WebAppInputLine::TouchJobOutputFile
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
WebAppInputLine::TouchJobOutputFile (/*[in]*/ FILE *)
  const
{
}
