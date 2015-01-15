/* inputline.cpp:

   Copyright (C) 1996-2011 Christian Schenk
 
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

void
WebAppInputLine::Init (/*[in]*/ const char * lpszProgramInvocationName)
{
  WebApp::Init (lpszProgramInvocationName);
  enablePipes = false;
}

/* _________________________________________________________________________
   
   WebAppInputLine::Finalize
   _________________________________________________________________________ */

void
WebAppInputLine::Finalize ()
{
  auxDirectory = "";
  fqNameOfFile = "";
  lastInputFileName = "";
  outputDirectory = "";
  WebApp::Finalize ();
}

/* _________________________________________________________________________
   
   WebAppInputLine::AddOptions
   _________________________________________________________________________ */

enum {
  OPT_DISABLE_PIPES,
  OPT_ENABLE_PIPES,
};

void
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

bool
WebAppInputLine::ProcessOption (/*[in]*/ int		opt,
				/*[in]*/ const char *	lpszOptArg)
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
  if (len >= ret.GetCapacity())
    {
      INVALID_ARGUMENT ("WebAppInputLine::MangleNameOfFile", lpszFrom);
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
  if (len >= ret.GetCapacity())
    {
      INVALID_ARGUMENT ("UnmangleNameOfFile_", 0/*lpszFrom*/);
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

PathName
WebAppInputLine::UnmangleNameOfFile (/*[in]*/ const char * lpszFrom)
{
  return (UnmangleNameOfFile_(lpszFrom));
}

/* _________________________________________________________________________

   WebAppInputLine::UnmangleNameOfFile
   _________________________________________________________________________ */

PathName
WebAppInputLine::UnmangleNameOfFile (/*[in]*/ const wchar_t * lpszFrom)
{
  return (UnmangleNameOfFile_(lpszFrom));
}

/* _________________________________________________________________________

   IsOutputFile   
   _________________________________________________________________________ */

static
bool
IsOutputFile (/*[in]*/ const PathName & path)
{
  PathName path_ (path);
  if (path_.HasExtension(".gz"))
  {
    path_.SetExtension (0);
  }
  return (path_.HasExtension(".dvi")
	  || path_.HasExtension(".pdf")
	  || path_.HasExtension(".synctex"));
}

/* _________________________________________________________________________

   WebAppInputLine::AllowFileName
   _________________________________________________________________________ */

bool
WebAppInputLine::AllowFileName (/*[in]*/ const char *		lpszPath,
				/*[in]*/ bool			forInput)
{
  bool allow;
  if (forInput)
    {
      static Core::TriState allowInput = TriState::Undetermined;
      if (allowInput == TriState::Undetermined)
	{
	  allow =
	    pSession->GetConfigValue(0,
				     "AllowUnsafeInputFiles",
				     true);
	  allowInput = (allow ? TriState::True : TriState::False);
	}
      else
	{
	  allow = (allowInput == TriState::True ? true : false);
	}
    }
  else
    {
      static Core::TriState allowOutput = TriState::Undetermined;
      if (allowOutput == TriState::Undetermined)
	{
	  allow =
	    pSession->GetConfigValue(0,
				     "AllowUnsafeOutputFiles",
				     false);
	  allowOutput = (allow ? TriState::True : TriState::False);
	}
      else
	{
	  allow = (allowOutput == TriState::True ? true : false);
	}
    }
  if (allow)
    {
      return (true);
    }
  return (Core::Utils::IsSafeFileName(lpszPath, forInput));
}

/* _________________________________________________________________________

   WebAppInputLine::OpenOutputFile
   _________________________________________________________________________ */

bool
WebAppInputLine::OpenOutputFile (/*[in]*/ C4P::FileRoot &	f,
				 /*[in]*/ const char *		lpszPath,
				 /*[in]*/ FileShare		share,
				 /*[in]*/ bool			text,
				 /*[out]*/ PathName &		outPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);
#if defined(MIKTEX_WINDOWS)
  string utf8Path;
  if (! Utils::IsUTF8(lpszPath))
  {
    utf8Path = Utils::AnsiToUTF8(lpszPath);
    lpszPath = utf8Path.c_str();
  }
#endif
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
      bool isAuxFile = ! IsOutputFile(unmangled);
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
      if (pfile != 0)
      {
	outPath = lpszPath;
      }
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

bool
WebAppInputLine::OpenInputFile (/*[out]*/ FILE * *		ppFile,
				/*[in]*/ const char *		lpszFileName)
{
  MIKTEX_ASSERT_STRING (lpszFileName);

#if defined(MIKTEX_WINDOWS)
  string utf8FileName;
  if (! Utils::IsUTF8(lpszFileName))
  {
    utf8FileName = Utils::AnsiToUTF8(lpszFileName);
    lpszFileName = utf8FileName.c_str();
  }
#endif

  if (enablePipes && lpszFileName[0] == '|')
    {
      *ppFile =
	pSession->OpenFile(lpszFileName + 1,
			   FileMode::Command,
			   FileAccess::Read,
			   false);
      fqNameOfFile.Clear();
    }
  else
    {
      if (! pSession->FindFile(UnmangleNameOfFile(lpszFileName).Get(),
			       GetInputFileType(),
			       fqNameOfFile))
	{
	  return (false);
	}

#if 1 // 2015-01-15
      PathName cwd;
      cwd.SetToCurrentDirectory ();
      const char * lpszRel = Utils::GetRelativizedPath(fqNameOfFile.Get(), cwd.Get());
      if (lpszRel != 0)
      {
	PathName temp (lpszRel);
	fqNameOfFile = temp;
      }
#endif

      try
	{
	  if (fqNameOfFile.HasExtension(".gz"))
	    {
	      CommandLineBuilder cmd ("zcat");
	      cmd.AppendArgument (fqNameOfFile);
	      *ppFile =
		pSession->OpenFile(cmd.Get(),
				   FileMode::Command,
				   FileAccess::Read,
				   false);
	    }
	  else if (fqNameOfFile.HasExtension(".bz2"))
	    {
	      CommandLineBuilder cmd ("bzcat");
	      cmd.AppendArgument (fqNameOfFile);
	      *ppFile =
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
	      *ppFile =
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

  if (*ppFile == 0)
    {
      return (false);
    }

  lastInputFileName = lpszFileName;
  
  return (true);
}

/* _________________________________________________________________________
   
   WebAppInputLine::OpenInputFile
   _________________________________________________________________________ */

bool
WebAppInputLine::OpenInputFile (/*[out]*/ C4P::FileRoot &	f,
				/*[in]*/ const char *		lpszFileName)
{
  FILE * pFile = 0;

  if (!OpenInputFile(&pFile, lpszFileName))
    {
      return (false);
    }

  f.Attach (pFile, true);

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
void
WebAppInputLine::HandleEof (/*[in]*/ FILE * pfile)
  const
{
  MIKTEX_ASSERT (pfile != 0);
#if 1
  while (GetC(pfile) != EOF)
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

void
WebAppInputLine::TouchJobOutputFile (/*[in]*/ FILE *)
  const
{
}
