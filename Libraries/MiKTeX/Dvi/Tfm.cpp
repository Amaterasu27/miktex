/* Tfm.cpp:

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

/* some tricks are borrowed from Dvips, TFtoPL  and other celebrities */

#include "StdAfx.h"
#include "common.h"

/* _________________________________________________________________________

   Tfm::Tfm
   _________________________________________________________________________ */

Tfm::Tfm (/*[in]*/ DviImpl *		pDvi,
	  /*[in]*/ int			checkSum,
	  /*[in]*/ int			scaledSize,
	  /*[in]*/ int			designSize,
	  /*[in]*/ const char *		lpszAreaName,
	  /*[in]*/ const char *		lpszFontName,
	  /*[in]*/ const char *	lpszFileName,
	  /*[in]*/ double		tfmConv,
	  /*[in]*/ double		conv)

  : DviFont (pDvi,
	     checkSum,
	     scaledSize,
	     designSize,
	     lpszAreaName,
	     lpszFontName,
	     lpszFileName,
	     tfmConv,
	     conv),
    log_error (TraceStream::Open(MIKTEX_TRACE_ERROR)),
    log_tfm (TraceStream::Open(MIKTEX_TRACE_DVITFM))

{
  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("creating TFM object '%s'"),
     dviInfo.name.c_str());
  dviInfo.isVirtualFont = false;
}

/* _________________________________________________________________________

   Tfm::~Tfm
   _________________________________________________________________________ */

Tfm::~Tfm ()
{
  try
    {
      map<int, DviChar *>::iterator it;
      for (it = dviChars.begin();
	   it != dviChars.end();
	   ++ it)
	{
	  delete it->second;
	  it->second = 0;
	}
      dviChars.clear ();
      if (log_error.get())
	{
	  log_error->Close ();
	  log_error.reset ();
	}
      if (log_tfm.get())
	{
	  log_tfm->Close ();
	  log_tfm.reset ();
	}
    }
  catch (const exception &)
    {
    }
}
/* _________________________________________________________________________

   Tfm::operator[]
   _________________________________________________________________________ */

DviChar *
Tfm::operator[] (/*[in]*/ unsigned long idx)
{
  Read ();
  DviChar * pDviChar = dviChars[idx];
  if (pDviChar == 0)
    {
      log_tfm->WriteFormattedLine
	(T_("libdvi"),
	 T_("%s: nil character at %u"),
	 dviInfo.name.c_str(),
	 idx);
      pDviChar = new DviChar (this);
      dviChars[idx] = pDviChar;
    }
  return (pDviChar);
}

/* _________________________________________________________________________

   Tfm::Read
   _________________________________________________________________________ */

void
Tfm::Read ()
{
  if (! dviChars.empty() || dviInfo.notLoadable)
    {
      return;
    }

  log_tfm->WriteFormattedLine
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
      if (Make(dviInfo.name))
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
		 || (Make(T_("cmr10"))
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

  dviInfo.fileName = fileName.ToString();

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("opening TFM file %s"),
     fileName.Get());

  InputStream inputStream (fileName.Get());

  long lf = inputStream.ReadSignedPair();

  if (lf == 0)
    {
      FATAL_DVI_ERROR (T_("Tfm::Read"),
		       T_("Invalid TFM file."),
		       0);
    }

  long lh = inputStream.ReadSignedPair();
  long bc = inputStream.ReadSignedPair();
  long ec = inputStream.ReadSignedPair();
  long nw = inputStream.ReadSignedPair();
  long nh = inputStream.ReadSignedPair();
  long nd = inputStream.ReadSignedPair();
  long ni = inputStream.ReadSignedPair();
  long nl = inputStream.ReadSignedPair();
  long nk = inputStream.ReadSignedPair();
  long ne = inputStream.ReadSignedPair();
  long np = inputStream.ReadSignedPair();

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("header size: %ld"),
     lh);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("smallest character code: %ld"),
     bc);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("largest character code: %ld"),
     ec);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("width table size: %ld"),
     nw);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("height table size: %ld"),
     nh);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("depth table size: %ld"),
     nd);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("italic correction table size: %ld"),
     ni);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("lig/kern table size: %ld"),
     nl);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("kern table size: %ld"),
     nk);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("extensible character table size: %ld"),
     ne);

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("font parameter size: %ld"),
     np);

  int my_checkSum = inputStream.ReadSignedQuad();

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("checkSum: 0%lo"),
     my_checkSum);
  
  int my_designSize = inputStream.ReadSignedQuad();

  log_tfm->WriteFormattedLine
    (T_("libdvi"),
     T_("designSize: %ld"),
     my_designSize);

  if (my_checkSum != checkSum)
    {
      log_error->WriteFormattedLine
	(T_("libdvi"),
	 T_("%s: checkSum mismatch"),
	 dviInfo.name.c_str());
    }

  if (my_designSize * tfmConv != designSize)
    {
      log_error->WriteFormattedLine
	(T_("libdvi"),
	 T_("%s: designSize mismatch"),
	 dviInfo.name.c_str());
    }

  inputStream.SkipBytes ((lh - 2) * 4);

  struct TfmIndex
  {
    int widthIndex;
    int heightIndex;
    int depthIndex;
  };

  vector<TfmIndex> infoTable (ec);

  for (int charCode = bc; charCode < ec; ++ charCode)
    {
      DviChar * pDviChar = new DviChar (this);
      dviChars[charCode] = pDviChar;
      pDviChar->SetCharacterCode (charCode);
      TfmIndex tfmIndex;
      tfmIndex.widthIndex = inputStream.ReadSignedByte();
      int heightDepth = inputStream.ReadSignedByte();
      tfmIndex.heightIndex = ((heightDepth >> 4) & 15);
      tfmIndex.depthIndex = (heightDepth & 15);
      inputStream.SkipBytes (2);
      infoTable[charCode] = tfmIndex;
    }

  vector<int> widths (nw);

  for (int idx = 0; idx < nw; ++ idx)
    {
      widths[idx] = inputStream.ReadSignedQuad();
    }

  vector<int> heights (nh);

  for (int idx = 0; idx < nh; ++ idx)
    {
      heights[idx] = inputStream.ReadSignedQuad();
    }

  vector<int> depths (nd);

  for (int idx = 0; idx < nd; ++ idx)
    {
      depths[idx] = inputStream.ReadSignedQuad();
    }

  // inputStream.Close ();

  for (int charCode = bc; charCode < ec; ++ charCode)
    {
      int tfmWidth =
	ScaleFix(widths[infoTable[charCode].widthIndex], GetScaledAt());
      dviChars[charCode]->SetDviWidth (tfmWidth);
      int pixelWidth;
      if (tfmWidth >= 0)
	{
	  pixelWidth = static_cast<int>(conv * tfmWidth + 0.5);
	}
      else
	{
	  pixelWidth = - static_cast<int>(conv * -tfmWidth + 0.5);
	}
      dviChars[charCode]->SetWidth (pixelWidth);
    }
}

/* _________________________________________________________________________

   Tfm::Make
   _________________________________________________________________________ */

bool
Tfm::Make (/*[in]*/ const string &	name)
{
  dviInfo.transcript += T_("\r\n");
  dviInfo.transcript += T_("Making TFM file:\r\n");
  PathName makeTFM;
  if (! SessionWrapper(true)->FindFile(MIKTEX_MAKETFM_EXE,
				       FileType::EXE,
				       makeTFM))
    {
      FATAL_DVI_ERROR (T_("Tfm::Make"),
		       T_("The MakeTFM utility could not be found."),
		       0);
    }
  char szBasename[BufferSizes::MaxPath];
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

