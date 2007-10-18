/* vfont.cpp:

   Copyright (C) 1996-2007 Christian Schenk

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

#include "StdAfx.h"
#include "common.h"

/* _________________________________________________________________________

   VFont::VFont
   _________________________________________________________________________ */

VFont::VFont (/*[in]*/ DviImpl *		pDvi,
	      /*[in]*/ int			checkSum,
	      /*[in]*/ int			scaledSize,
	      /*[in]*/ int			designSize,
	      /*[in]*/ const char *		lpszAreaName,
	      /*[in]*/ const char *		lpszFontName,
	      /*[in]*/ const char *		lpszFileName,
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
    log_error (TraceStream::Open(MIKTEX_TRACE_ERROR)),
    log_vfont (TraceStream::Open(MIKTEX_TRACE_DVIVFONT))

{
  dviInfo.isVirtualFont = true;
}

/* _________________________________________________________________________

   VFont::~VFont
   _________________________________________________________________________ */

VFont::~VFont ()
{
  try
    {
      map<int, VfChar *>::iterator it1;
      for (it1 = characterTable.begin();
	   it1 != characterTable.end();
	   ++ it1)
	{
	  delete it1->second;
	  it1->second = 0;
	}
      characterTable.clear ();      
      FontMap::iterator it2;
      for (it2 = fontMap.begin();
	   it2 != fontMap.end();
	   ++ it2)
	{
	  delete it2->second;
	  it2->second = 0;
	}
      fontMap.clear ();
      if (log_error.get())
	{
	  log_error->Close ();
	  log_error.reset ();
	}
      if (log_vfont.get())
	{
	  log_vfont->Close ();
	  log_vfont.reset ();
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   VFont::Read
   _________________________________________________________________________ */

const short pre		= 247;
const short id_byte	= 202;
const short post	= 248;
const short long_char	= 242;
const short fnt_def1	= 243;
const short fnt_def2	= 244;
const short fnt_def3	= 245;
const short fnt_def4	= 246;

void
VFont::Read ()
{
  if (! characterTable.empty() || dviInfo.notLoadable)
    {
      return;
    }

  dviInfo.notLoadable = true;

  InputStream stream (dviInfo.fileName.c_str());

  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("reading vf file %s"),
     Q_(dviInfo.fileName));

  if (stream.ReadByte() != pre)
    {
      FATAL_DVI_ERROR (T_("VFont::Read"),
		       T_("Not a VF file."),
		       dviInfo.fileName.c_str());
    }

  ReadPreamble (stream);
  ReadFontDefsAndCharPackets (stream);
  ReadPostamble (stream);
  
  dviInfo.notLoadable = false;
}

/* _________________________________________________________________________

   VFont::ReadPreamble
   _________________________________________________________________________ */

void
VFont::ReadPreamble (/*[in]*/ InputStream & inputStream)
{
  if (inputStream.ReadByte() != id_byte)
    {
      FATAL_DVI_ERROR (T_("VFont::ReadPreamble"),
		       T_("Bad VF file."),
		       dviInfo.fileName.c_str());
    }

  unsigned long commentSize = inputStream.ReadByte();

  char tmp[256];
  inputStream.Read (tmp, commentSize);
  tmp[commentSize] = 0;
  dviInfo.comment = tmp;

  int my_checkSum = inputStream.ReadSignedQuad();
  int my_designSize = inputStream.ReadSignedQuad();

  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("comment: %s"),
     dviInfo.comment.c_str());
  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("checkSum: 0%o"),
     my_checkSum);
  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("designSize: %d"),
     my_designSize);

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

/* _________________________________________________________________________

   VFont::ReadFontDefsAndCharPackets
   _________________________________________________________________________ */

void
VFont::ReadFontDefsAndCharPackets (/*[in]*/ InputStream & inputStream)
{
  short tempByte;
  do
    {
      tempByte = inputStream.ReadByte();
      if (tempByte != post)
	{
	  if (tempByte > long_char)
	    {
	      ReadFontDef (inputStream, tempByte);
	    }
	  else
	    {
	      ReadCharPacket (inputStream, tempByte);
	    }
	}
    }
  while (tempByte != post);
}

/* _________________________________________________________________________

   VFont::ReadFontDef
   _________________________________________________________________________ */

void
VFont::ReadFontDef (/*[in]*/ InputStream &	inputStream,
		    /*[in]*/ short		fntDefX)
{
  int fontNum;

  switch (fntDefX)
    {
    case fnt_def1:
      fontNum = inputStream.ReadByte();
      break;
    case fnt_def2:
      fontNum = inputStream.ReadPair();
      break;
    case fnt_def3:
      fontNum = inputStream.ReadTrio();
      break;
    case fnt_def4:
      fontNum = inputStream.ReadSignedQuad();
      break;
    default:
      FATAL_DVI_ERROR (T_("VFont::ReadFontDef"),
		       T_("Bad VF file."),
		       dviInfo.fileName.c_str());
      break;
    }

  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("defining font %d"),
     fontNum);

  int cs = inputStream.ReadSignedQuad();
  int ss = inputStream.ReadSignedQuad();
  int ds = inputStream.ReadSignedQuad();

  int areaNameSize = inputStream.ReadByte();
  int fontNameSize = inputStream.ReadByte();

  char szArea[256];
  inputStream.Read (szArea, areaNameSize);
  szArea[areaNameSize] = 0;

  char szName[256];
  inputStream.Read (szName, fontNameSize);
  szName[fontNameSize] = 0;

  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("areaname: %s"),
     szArea);
  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("fontname: %s"),
     szName);
  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("checkSum: 0%o"),
     cs);
  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("scaledSize: %d"),
     ss);
  log_vfont->WriteFormattedLine
    (T_("libdvi"),
     T_("designSize: %d"),
     ds);
  
  DviFont * pFont;
  PathName fileName;
  if (SessionWrapper(true)->FindFile(szName, FileType::VF, fileName))
    {
      log_vfont->WriteFormattedLine
	(T_("libdvi"),
	 T_("found vf file %s"),
	 Q_(fileName));
      pFont =
	new VFont (pDviImpl,
		   cs,
		   ScaleFix(ss, scaledAt),
		   static_cast<int>(ds * tfmConv),
		   szArea,
		   szName,
		   fileName.Get(),
		   tfmConv,
		   conv,
		   mag,
		   metafontMode.c_str(),
		   baseDpi);
    }
  else if (pDviImpl->GetPageMode() == DviPageMode::Pk)
    {
      pFont =
	new PkFont (pDviImpl,
		    cs,
		    ScaleFix(ss, scaledAt),
		    static_cast<int>(ds * tfmConv),
		    szArea,
		    szName,
		    T_(""),
		    tfmConv,
		    conv,
		    mag,
		    metafontMode.c_str(),
		    baseDpi);
    }
  else
    {
      pFont =
	new Tfm (pDviImpl,
		 cs,
		 ScaleFix(ss, scaledAt),
		 static_cast<int>(ds * tfmConv),
		 szArea,
		 szName,
		 T_(""),
		 tfmConv,
		 conv);
    }

  fontMap[fontNum] = pFont;
}

/* _________________________________________________________________________

   VFont::ReadCharPacket
   _________________________________________________________________________ */

void
VFont::ReadCharPacket (/*[in]*/ InputStream &	inputStream,
		       /*[in]*/ short		size)
{
  VfChar * pVfChar = new VfChar (this);
  pVfChar->Read (inputStream, size, conv);
  characterTable[ pVfChar->GetCharacterCode() ] = pVfChar;
}

/* _________________________________________________________________________

   VFont::ReadPostamble
   _________________________________________________________________________ */

void
VFont::ReadPostamble (/*[in]*/ InputStream & inputStream)

{
  int by;
  while (inputStream.TryToReadByte(by))
    {
      if (by != post)
	{
	  FATAL_DVI_ERROR (T_("VFont::ReadPostamble"),
			   T_("Bad VF file."),
			   dviInfo.fileName.c_str());
	}
    }
}

/* _________________________________________________________________________

   VFont::GetCharAt
   _________________________________________________________________________ */

VfChar *
VFont::GetCharAt (/*[in]*/ int idx)

{
  Read ();
  return (characterTable[idx]);
}
