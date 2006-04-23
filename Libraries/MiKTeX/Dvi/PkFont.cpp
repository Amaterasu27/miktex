/* pkfont.cpp:

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX DVI Library.

   The MiKTeX DVI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX DVI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the MiKTeX DVI Library; if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
   USA.  */

/* some tricks are borrowed from dvips, gftopk and other celebrities */

#include "StdAfx.h"
#include "common.h"

/* _________________________________________________________________________

   PkFont::PkFont
   _________________________________________________________________________ */

PkFont::PkFont (/*[in]*/ DviImpl *		pDvi,
		/*[in]*/ int			checkSum,
		/*[in]*/ int			scaledSize,
		/*[in]*/ int			designSize,
		/*[in]*/ const char *		lpszAreaName,
		/*[in]*/ const char *		lpszFontName,
		/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		/*[in]*/ double			tfmConv,
		/*[in]*/ double			conv,
		/*[in]*/ int			mag,
		/*[in]*/ const char *		lpszMetafontMode,
		/*[in]*/ int			baseDpi)

  : DviFont (pDvi,
	     checkSum,
	     scaledSize,
	     designSize,
	     lpszAreaName,
	     lpszFontName,
	     lpszFileName,
	     tfmConv,
	     conv),
    mag (mag),
    metafontMode (lpszMetafontMode),
    baseDpi (baseDpi),
    checkDpi (false),
    log_error (TraceStream::Open(MIKTEX_TRACE_ERROR)),
    log_pkfont (TraceStream::Open(MIKTEX_TRACE_DVIPKFONT))

{
  log_pkfont->WriteFormattedLine
    (T_("libdvi"),
     T_("creating pk font object '%s'"),
     dviInfo.name.c_str());
  for (int i = 0; i < 30; ++ i)
    {
      existSizes[i] = 0;
    }
  existSizes[1] = 99999999;
  dviInfo.isVirtualFont = false;
}

/* _________________________________________________________________________

   PkFont::~PkFont
   _________________________________________________________________________ */

PkFont::~PkFont ()
{
  try
    {
      map<int, PkChar *>::iterator it;
      for (it = pkChars.begin();
	   it != pkChars.end();
	   ++ it)
	{
	  delete it->second;
	  it->second = 0;
	}
      pkChars.clear ();
      if (log_error.get())
	{
	  log_error->Close ();
	  log_error.reset ();
	}
      if (log_pkfont.get())
	{
	  log_pkfont->Close ();
	  log_pkfont.reset ();
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   PkFont::AddSize
   _________________________________________________________________________ */

void
PkFont::AddSize (/*[in]*/ int rhsize)
{
  int hsize = rhsize;
  int * p;
  for (p = existSizes; *p < hsize; ++ p)
    {
      ;
    }
  if (*p == hsize)
    {
      return;
    }
  do
    {
      int t = *p;
      *p++ = hsize;
      hsize = t;
    }
  while (hsize);
}

/* _________________________________________________________________________

   PkFont::AddDpi
   _________________________________________________________________________ */

void
PkFont::AddDpi (/*[in]*/ int hsize)
{
  AddSize (hsize) ;
  AddSize ((hsize * 116161L + 53020L) / 106040L);
  int a = hsize;
  int b = 0 ;
  int c = 1 ;
  for (int i = 0; i < 9; ++ i)
    {
      b = 6 * b + (a % 5) * c;
      a = a + a / 5;
      c = c * 5;
      if (b > c)
	{
	  b -= c;
	  ++ a;
	}
      if (b + b >= c)
	{
	  AddSize (a + 1);
	}
      else
	{
	  AddSize (a);
	}
    }
}

/* _________________________________________________________________________

   PkFont::CheckDpi
   _________________________________________________________________________ */

int
PkFont::CheckDpi (/*[in]*/ int	dpi,
		  /*[in]*/ int	baseDpi)
{
  if (! checkDpi)
    {
      AddDpi (baseDpi);
      checkDpi = true;
    }
  int i;
  for (i = 0; existSizes[i] < dpi; ++ i)
    {
      ;
    }
  int margin = 1 + dpi / 500;
  if (existSizes[i] - dpi <= margin)
    {
      return (existSizes[i]);
    }
  else if (dpi - existSizes[i - 1] <= margin)
    {
      return (existSizes[i - 1]) ;
    }
  else
    {
      return (dpi);
    }
}

/* _________________________________________________________________________

   PkFont::Read
   _________________________________________________________________________ */

const int pk_id = 89;
const int pk_xxx1 = 240;
const int pk_xxx2 = 241;
const int pk_xxx3 = 242;
const int pk_xxx4 = 243;
const int pk_yyy = 244;
const int pk_post = 245;
const int pk_no_op = 246;
const int pk_pre = 247;

void
PkFont::Read ()
{
  if (! pkChars.empty() || dviInfo.notLoadable)
    {
      return;
    }

  log_pkfont->WriteFormattedLine
    (T_("libdvi"),
     T_("going to load pk font %s"),
     dviInfo.name.c_str());

  int dpi =
    static_cast<int>((static_cast<double>(mag)
		      * static_cast<double>(scaledAt)
		      * static_cast<double>(baseDpi))
		     / (static_cast<double>(designSize) * 1000.0)
		     + 0.5);
  dpi = CheckDpi(dpi, baseDpi);

  dviInfo.notLoadable = true;

  PathName fileName;

  bool fontFileExists =
    SessionWrapper(true)->FindPkFile(dviInfo.name.c_str(),
				     metafontMode.c_str(),
				     dpi,
				     fileName);

  if (! fontFileExists)
    {
      if (Make(dviInfo.name, dpi, baseDpi, metafontMode))
	{
	  fontFileExists =
	    SessionWrapper(true)->FindPkFile(dviInfo.name.c_str(),
					     metafontMode.c_str(),
					     dpi,
					     fileName);
	  if (! fontFileExists)
	    {
	      // this shouldn't happen; but it does (#521481)
	    }
	}
      if (! fontFileExists)
	{
	  dviInfo.transcript += T_("\r\n");
	  dviInfo.transcript += T_("Loading 'cmr10' instead.\r\n");
	  log_error->WriteFormattedLine
	    (T_("libdvi"),
	     T_("'%s' not loadable - loading 'cmr10' instead!"),
	     dviInfo.name.c_str());
	  if (! (SessionWrapper(true)->FindPkFile(T_("cmr10"),
						  metafontMode.c_str(),
						  dpi,
						  fileName)
		 || (Make(T_("cmr10"), dpi, baseDpi, metafontMode)
		     && SessionWrapper(true)->FindPkFile(T_("cmr10"),
							 metafontMode.c_str(),
							 dpi,
							 fileName))))
	    {
	      dviInfo.transcript += T_("'cmr10' not loadable either!");
	      log_error->WriteLine
		(T_("libdvi"),
		 T_("'cmr10' not loadable - will display blank chars!"));
	      return;
	    }
	}
    }

  dviInfo.fileName = fileName.ToString();

  log_pkfont->WriteFormattedLine
    (T_("libdvi"),
     T_("opening pk file %s"),
     fileName.Get());

  InputStream inputstream (fileName.Get());
  int b;
  while (inputstream.TryToReadByte(b))
    {
      // process a PK command
      switch (b)
	{
	case pk_xxx1:
	case pk_xxx2:
	case pk_xxx3:
	case pk_xxx4:
	  {
	    int k;
	    int n = b - pk_xxx1;
	    switch (n)
	      {
	      case 0:
		k = inputstream.ReadByte ();
		break;
	      case 1:
		k = inputstream.ReadPair ();
		break;
	      case 2:
		k = inputstream.ReadTrio ();
		break;
	      case 3:
		k = inputstream.ReadSignedQuad ();
		break;
	      default:
		assert (false);
		__assume (false);
	      }
	    inputstream.SkipBytes (k);
	  }
	  break;

	case pk_yyy:
	  inputstream.SkipBytes (4); // ignore value
	  break;

	case pk_post:
	  break;

	case pk_no_op:
	  break;

	case pk_pre:
	  {
	    if (inputstream.ReadByte() != pk_id)
	      {
		FATAL_DVI_ERROR (T_("PkFont::Read"),
				 T_("Bad font file."),
				 dviInfo.name.c_str());
	      }
	    int len = inputstream.ReadByte();
	    char tmp[256];
	    inputstream.Read (tmp, len);
	    tmp[len] = 0;
	    dviInfo.comment = tmp;
	    int my_designSize = inputstream.ReadSignedQuad();
	    int my_checkSum = inputstream.ReadSignedQuad();
	    hppp = inputstream.ReadSignedQuad();
	    vppp = inputstream.ReadSignedQuad();
	    log_pkfont->WriteFormattedLine
	      (T_("libdvi"),
	       T_("comment: %s"),
	       dviInfo.comment.c_str());
	    log_pkfont->WriteFormattedLine
	      (T_("libdvi"),
	       T_("designSize: %d"),
	       my_designSize);
	    log_pkfont->WriteFormattedLine
	      (T_("libdvi"),
	       T_("checkSum: 0%o"),
	       my_checkSum);
	    log_pkfont->WriteFormattedLine
	      (T_("libdvi"),
	       T_("hppp: %d"),
	       hppp);
	    log_pkfont->WriteFormattedLine
	      (T_("libdvi"),
	       T_("vppp: %d"),
	       vppp);

	    if (my_designSize * tfmConv != designSize)
	      {
		log_error->WriteFormattedLine
		  (T_("libdvi"),
		   T_("%s: designSize mismatch"),
		   dviInfo.name.c_str());
	      }
	    if (my_checkSum != checkSum)
	      {
		log_error->WriteFormattedLine
		  (T_("libdvi"),
		   T_("%s: checkSum mismatch"),
		   dviInfo.name.c_str());
	      }
	  }
	  break;

	default:

	  // do a character definition
	  PkChar * pPkChar = new PkChar (this);
	  pPkChar->Read (inputstream, b);
	  pkChars[ pPkChar->GetCharacterCode() ] = pPkChar;
	  break;
	}
    }

  dviInfo.notLoadable = ! fontFileExists;
}

/* _________________________________________________________________________

   PkFont::Make
   _________________________________________________________________________ */

bool
PkFont::Make (/*[in]*/ const string &	name,
	      /*[in]*/ int		dpi,
	      /*[in]*/ int		baseDpi,
	      /*[in]*/ const tstring &	metafontMode)
{
  MIKTEXCHAR szCmdLine[ 500 ];
  dviInfo.transcript += T_("\r\n");
  dviInfo.transcript += T_("Making PK font:\r\n");
  PathName pathMakePk;
  SessionWrapper(true)->MakeMakePkCommandLine(name.c_str(),
					      dpi,
					      baseDpi,
					      metafontMode.c_str(),
					      pathMakePk,
					      szCmdLine,
					      500);
  dviInfo.transcript += szCmdLine;
  dviInfo.transcript += T_("\r\n");
  pDviImpl->Progress (DviNotification::BeginLoadFont,
		      T_("%s..."),
		      dviInfo.name.c_str());
  char buf[4096];
  int exitCode;
  UINT n = 4096;
  bool b =
    (Process::Run(pathMakePk.Get(),
		  szCmdLine,
		  buf,
		  &n,
		  &exitCode)
     && exitCode == 0);
  buf[4096 - 1] = 0;
  if (! b)
    {
      log_error->WriteLine (T_("libdvi"), buf);
    }
  dviInfo.transcript += buf;
  dviInfo.transcript += T_("\r\n");
  return (b);
}

/* _________________________________________________________________________

   PkFont::operator[]
   _________________________________________________________________________ */

PkChar *
PkFont::operator[] (unsigned long idx)
{
  Read ();
  PkChar * pPkChar = pkChars[idx];
  if (pPkChar == 0)
    {
      log_pkfont->WriteFormattedLine
	(T_("libdvi"),
	 T_("%s: nil character at %u"),
	 dviInfo.name.c_str(),
	 idx);
      pPkChar = new PkChar (this);
      pkChars[idx] = pPkChar;
    }
  return (pPkChar);
}

/* _________________________________________________________________________

   PkFont::ReadTFM
   _________________________________________________________________________ */

void
PkFont::ReadTFM ()
{
  if (! pkChars.empty() || dviInfo.notLoadable)
    {
      return;
    }

  log_pkfont->WriteFormattedLine
    (T_("libdvi"),
     T_("going to load TFM file %s"),
     dviInfo.name.c_str());

  PathName fileName;

  bool tfmFileExists =
    SessionWrapper(true)->FindTfmFile(dviInfo.name.c_str(),
				      fileName,
				      false);

  if (! tfmFileExists)
    {
      if (MakeTFM(dviInfo.name))
	{
	  tfmFileExists =
	    SessionWrapper(true)->FindTfmFile(dviInfo.name.c_str(),
					      fileName,
					      false);
	  if (! tfmFileExists)
	    {
	      // this shouldn't happen; but it does (#521481)
	    }
	}
      if (! tfmFileExists)
	{
	  dviInfo.transcript += T_("\r\n");
	  dviInfo.transcript += T_("Loading 'cmr10' instead.\r\n");
	  log_error->WriteFormattedLine
	    (T_("libdvi"),
	     T_("'%s' not loadable - loading 'cmr10' instead!"),
	     dviInfo.name.c_str());
	  if (! (SessionWrapper(true)->FindTfmFile(T_("cmr10"),
						   fileName,
						   false)
		 || (MakeTFM(T_("cmr10"))
		     && SessionWrapper(true)->FindTfmFile(T_("cmr10"),
							  fileName,
							  false))))
	    {
	      dviInfo.transcript += T_("'cmr10' not loadable either!");
	      log_error->WriteLine
		(T_("libdvi"),
		 T_("'cmr10' not loadable - will display blank chars!"));
	      return;
	    }
	}
    }
}

/* _________________________________________________________________________

   PkFont::MakeTFM
   _________________________________________________________________________ */

bool
PkFont::MakeTFM (/*[in]*/ const string &	name)
{
  dviInfo.transcript += T_("\r\n");
  dviInfo.transcript += T_("Making TFM file:\r\n");
  PathName makeTFM;
  if (! SessionWrapper(true)->FindFile(MIKTEX_MAKETFM_EXE,
				       FileType::EXE,
				       makeTFM))
    {
      FATAL_DVI_ERROR (T_("PkFont::MakeTFM"),
		       T_("The MakeTFM utility could not be found."),
		       0);
    }
  MIKTEXCHAR szBasename[BufferSizes::MaxPath];
  PathName::Split (name.c_str(),
		   0, 0,
		   szBasename, BufferSizes::MaxPath,
		   0, 0);
  CommandLineBuilder commandLine;
  commandLine.AppendOption (T_("-v"));
  commandLine.AppendArgument (szBasename);
  dviInfo.transcript += commandLine.Get();
  dviInfo.transcript += T_("\r\n");
  pDviImpl->Progress (DviNotification::BeginLoadFont,
		      T_("%s..."),
		      dviInfo.name.c_str());
  char szBuf[4096];
  size_t size = 4096;
  int exitCode;
  bool done =
    (Process::Run(makeTFM,
		 commandLine.Get(),
		 szBuf,
		 &size,
		 &exitCode)
     && exitCode == 0);
  szBuf[4096 - 1] = 0;
  if (! done)
    {
      log_error->WriteLine (T_("libdvi"), szBuf);
    }
  dviInfo.transcript += szBuf;
  dviInfo.transcript += T_("\r\n");
  return (done);
}

