/* texmflib.cpp: TeX'n'Friends helpers

   Copyright (C) 1996-2010 Christian Schenk
 
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

typedef C4P_FILE_STRUCT(unsigned char) bytefile;
typedef C4P_text alphafile;

/* _________________________________________________________________________

   OpenFontFile
   _________________________________________________________________________ */

STATICFUNC(bool)
OpenFontFile (/*[in,out]*/ bytefile *	pByteFile,
	      /*[in]*/ const char *	lpszFontName,
	      /*[in]*/ FileType		filetype,
	      /*[in]*/ const char *	lpszMakeFontCommand)
{
  PathName pathFont;
  if (! SessionWrapper(true)->FindFile(lpszFontName,
				       filetype,
				       pathFont))
    {
      if (lpszMakeFontCommand == 0
	  || ! SessionWrapper(true)->GetMakeFontsFlag())
	{
	  return (false);
	}
      PathName exe;
      if (! SessionWrapper(true)->FindFile(lpszMakeFontCommand,
					   FileType::EXE,
					   exe))
	{
	  FATAL_MIKTEX_ERROR ("OpenFontFile",
			      T_("\
The font creation utility could not be found."),
			      lpszMakeFontCommand);
	}
      char szBaseName[BufferSizes::MaxPath];
      PathName::Split (lpszFontName,
		       0, 0,
		       szBaseName, BufferSizes::MaxPath,
		       0, 0);
      string arguments;
      arguments = " -v \"";
      arguments += szBaseName;
      arguments += "\"";
      int exitCode;
      if (! (Process::Run(exe, arguments.c_str(), 0, &exitCode, 0)
	     && exitCode == 0))
	{
	  return (false);
	}
      if (! SessionWrapper(true)->FindFile(lpszFontName,
					   filetype,
					   pathFont))
	{
	  FATAL_MIKTEX_ERROR ("OpenFontFile",
			      T_("The font file could not be found."),
			      lpszFontName);
	}
    }
  pByteFile->Attach (SessionWrapper(true)->OpenFile(pathFont.Get(),
						    FileMode::Open,
						    FileAccess::Read,
						    false),
		     true);
  pByteFile->Read ();
  return (true);
}

/* _________________________________________________________________________

   OpenTFMFile
   _________________________________________________________________________ */

bool
MIKTEXCEECALL
MiKTeX::TeXAndFriends::OpenTFMFile (/*[in]*/ void  *		p,
				    /*[in]*/ const char *	lpszFontName)
{
  MIKTEX_API_BEGIN ("OpenTFMFile");
  MIKTEX_ASSERT_BUFFER (p, sizeof(bytefile));
  MIKTEX_ASSERT_STRING (lpszFontName);
  return (OpenFontFile(reinterpret_cast<bytefile*>(p),
		       lpszFontName,
		       FileType::TFM,
		       MIKTEX_MAKETFM_EXE));
  MIKTEX_API_END ("OpenTFMFile");
}

/* _________________________________________________________________________

   OpenXFMFile
   _________________________________________________________________________ */

bool
MIKTEXCEECALL
MiKTeX::TeXAndFriends::OpenXFMFile (/*[in]*/ void *		p,
				    /*[in]*/ const char *	lpszFontName)
{
  MIKTEX_API_BEGIN ("OpenXFMFile");
  MIKTEX_ASSERT_BUFFER (p, sizeof(bytefile));
  MIKTEX_ASSERT_STRING (lpszFontName);
  return (OpenFontFile(reinterpret_cast<bytefile*>(p),
		       lpszFontName,
		       FileType::OFM,
		       MIKTEX_MAKETFM_EXE)); // <fixme>???</fixme>
  MIKTEX_API_END ("OpenXFMFile");
}

/* _________________________________________________________________________

   OpenVFFile
   _________________________________________________________________________ */

bool
MIKTEXCEECALL
MiKTeX::TeXAndFriends::OpenVFFile (/*[in]*/ void *		p,
				   /*[in]*/ const char *	lpszFontName)
{
  MIKTEX_API_BEGIN ("OpenVFFile");
  MIKTEX_ASSERT_BUFFER (p, sizeof(bytefile));
  MIKTEX_ASSERT_STRING (lpszFontName);
  return (OpenFontFile(reinterpret_cast<bytefile*>(p),
		       lpszFontName,
		       FileType::VF,
		       0));
  MIKTEX_API_END ("OpenVFFile");
}

/* _________________________________________________________________________

   OpenXVFFile
   _________________________________________________________________________ */

int
MIKTEXCEECALL
MiKTeX::TeXAndFriends::OpenXVFFile (/*[in]*/ void *		p,
				    /*[in]*/ const char *	lpszFontName)
{
  MIKTEX_API_BEGIN ("OpenXVFFile");
  MIKTEX_ASSERT_BUFFER (p, sizeof(bytefile));
  MIKTEX_ASSERT_STRING (lpszFontName);
  return (OpenFontFile(reinterpret_cast<bytefile*>(p),
		       lpszFontName,
		       FileType::VF,
		       0)
	  || OpenFontFile(reinterpret_cast<bytefile*>(p),
			  lpszFontName,
			  FileType::OVF,
			  0));
  MIKTEX_API_END ("OpenXVFFile");
}

/* _________________________________________________________________________

   ProcessTCXFile
   _________________________________________________________________________ */

STATICFUNC(bool)
ProcessTCXFile (/*[in]*/ const char *		lpszFileName,
		/*[in,out]*/ unsigned char *	pChr,
		/*[in,out]*/ unsigned char *	pOrd,
		/*[in,out]*/ unsigned char *	pPrn)
{
  PathName tcxPath;

  if (! SessionWrapper(true)->FindFile(lpszFileName, FileType::TCX, tcxPath))
    {
      return (false);
    }

  StreamReader reader (tcxPath);

  string line;
  int lineNumber = 0;

  while (reader.ReadLine(line))
    {
      ++ lineNumber;

      const char * start;
      char * end;

      if (line.empty() || line[0] == '%')
	{
	  continue;
	}

      start = line.c_str();

      if (start == 0)
	{
	  FATAL_MIKTEX_ERROR ("ProcessTCXFile",
			      T_("Invalid tcx file."),
			      tcxPath.Get());
	}

      // get xord index (src)
      long xordidx = strtol(start, &end, 0);
      if (start == end)
	{
	  FATAL_MIKTEX_ERROR ("ProcessTCXFile",
			      T_("Invalid tcx file."),
			      tcxPath.Get());
	}
      else if (xordidx < 0 || xordidx > 255)
	{
	  FATAL_MIKTEX_ERROR ("ProcessTCXFile",
			      T_("Invalid tcx file."),
			      tcxPath.Get());
	}

      // make the char printable by default
      long printable = 1;

      // get optional xchr index (dest)
      start = end;
      long xchridx = strtol(start, &end, 0);
      if (start == end)
	{
	  // xchridx not specified; make it the same as xordidx
	  xchridx = xordidx;
	}
      else if (xchridx < 0 || xchridx > 255)
	{
	  FATAL_MIKTEX_ERROR ("ProcessTCXFile",
			      T_("Invalid tcx file."),
			      tcxPath.Get());
	}
      else
	{
	  // get optional printable flag
	  start = end;
	  printable = strtol(start, &end, 0);
	  if (start == end)
	    {
	      // not specified; by default printable
	      printable = 1;
	    }
	  else if (printable < 0 || printable > 1)
	    {
	      FATAL_MIKTEX_ERROR ("ProcessTCXFile",
				  T_("Invalid tcx file."),
				  tcxPath.Get());
	    }
	}

      // don't allow the 7bit ASCII set to become unprintable.
      if (printable == 0 && xordidx >= ' ' && xordidx <= '~')
	{
	  printable = 1;
	}

      // update the char tables
      pOrd[xordidx] = static_cast<unsigned char>(xchridx);
      pChr[xchridx] = static_cast<unsigned char>(xordidx);
      if (pPrn != 0)
	{
	  pPrn[xchridx] = static_cast<unsigned char>(printable);
	}
    }

  reader.Close ();

  return (true);
}

/* _________________________________________________________________________

   InitializeCharTables
   _________________________________________________________________________ */

bool
MIKTEXCEECALL
MiKTeX::TeXAndFriends::InitializeCharTables
(/*[in]*/ unsigned long	flags,
 /*[in]*/ const char *	lpszFileName,
 /*[in]*/ void *	pChr,
 /*[in]*/ void *	pOrd,
 /*[in]*/ void *	pPrn)
{
  MIKTEX_API_BEGIN ("InitializeCharTables");
  MIKTEX_ASSERT_BUFFER (pChr, 256);
  MIKTEX_ASSERT_BUFFER (pOrd, 256);
  MIKTEX_ASSERT_BUFFER_OR_NIL (pPrn, 256);
  unsigned char * pxchr = reinterpret_cast<unsigned char*>(pChr);
  unsigned char * pxord = reinterpret_cast<unsigned char*>(pOrd);
  unsigned char * pxprn = reinterpret_cast<unsigned char*>(pPrn);
  for (unsigned idx = 0; idx < 256; ++ idx)
    {
      pxchr[idx] = static_cast<unsigned char>(idx);
      pxord[idx] = static_cast<unsigned char>(idx);
      if (pxprn != 0)
	{
	  if ((flags & ICT_TCX) != 0)
	    {
	      pxprn[idx] = 1;
	    }
	  else
	    {
	      if (idx >= ' ' && idx <= '~')
		{
		  pxprn[idx] = 1;
		}
	      else
		{
		  pxprn[idx] = 0;
		}
	    }
	}
    }
  if ((flags & ICT_TCX) != 0)
    {
      ProcessTCXFile (lpszFileName, pxchr, pxord, pxprn);
    }
  return (true);
  MIKTEX_API_END ("InitializeCharTables");
}

/* _________________________________________________________________________

   OpenAlphaFile
   _________________________________________________________________________ */

STATICFUNC(bool)
OpenAlphaFile (/*[in]*/ void *		p,
	       /*[in]*/ const char *	lpszFileName,
	       /*[in]*/ FileType	fileType,
	       /*[in]*/ const char *	lpszExtension)
{
  MIKTEX_ASSERT (p != 0);
  MIKTEX_ASSERT_BUFFER (p, sizeof(alphafile));
  MIKTEX_ASSERT_STRING (lpszFileName);
  PathName fileName (lpszFileName);
  if (fileName.GetExtension() == 0 && lpszExtension != 0)
    {
      fileName.SetExtension (lpszExtension);
    }
  PathName path;
  if (! SessionWrapper(true)->FindFile(fileName.Get(), fileType, path))
    {
      return (false);
    }
  FILE * pfile =
    SessionWrapper(true)->TryOpenFile(path.Get(),
				      FileMode::Open,
				      FileAccess::Read,
				      false);
  if (pfile == 0)
    {
      return (false);
    }
  reinterpret_cast<alphafile*>(p)->Attach (pfile, true);
  get (*reinterpret_cast<alphafile*>(p));
  return (true);
}

/* _________________________________________________________________________

   OpenMAPFile
   _________________________________________________________________________ */

bool
MIKTEXCEECALL
MiKTeX::TeXAndFriends::OpenMAPFile (/*[in]*/ void *		p,
				    /*[in]*/ const char *	lpszFileName)
{
  MIKTEX_API_BEGIN ("OpenMAPFile");
  return (OpenAlphaFile(p, lpszFileName, FileType::MAP, ".map"));
  MIKTEX_API_END ("OpenMAPFile");
}

/* _________________________________________________________________________

   OpenMETAFONTFile
   _________________________________________________________________________ */

bool
MIKTEXCEECALL
MiKTeX::TeXAndFriends::OpenMETAFONTFile (/*[in]*/ void *	p,
					 /*[in]*/ const char *	lpszFileName)
{
  MIKTEX_API_BEGIN ("OpenMETAFONTFile");
  return (OpenAlphaFile(p, lpszFileName, FileType::MF, ".mf"));
  MIKTEX_API_END ("OpenMETAFONTFile");
}
