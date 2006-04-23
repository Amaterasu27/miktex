/* vfchar.cpp:

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

#include "StdAfx.h"

#include "common.h"

class DviFont;

/* _________________________________________________________________________

   VfChar::VfChar
   _________________________________________________________________________ */

VfChar::VfChar (/*[in]*/ DviFont * pFont)
  : DviChar (pFont),
    pPacket (0),
    packetSize (0),
    log_vfchar (TraceStream::Open(MIKTEX_TRACE_DVIVFCHAR))
{
}

/* _________________________________________________________________________

   VfChar::~VfChar
   _________________________________________________________________________ */

VfChar::~VfChar ()
{
  try
    {
      if (pPacket != 0)
	{
	  if (packetSize > sizeof(smallPacket))
	    {
	      delete [] pPacket;
	    }
	  pPacket = 0;
	  packetSize = 0;
	}
      if (log_vfchar.get() != 0)
	{
	  log_vfchar->Close ();
	  log_vfchar.reset ();
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   VfChar::Read
   _________________________________________________________________________ */

const short long_char = 242;

void
VfChar::Read (/*[in]*/ InputStream &	inputstream,
	      /*[in]*/ int		size,
	      /*[in]*/ double		conv)
{
  if (size == long_char)
    {
      packetSize = inputstream.ReadSignedQuad();
      charCode = inputstream.ReadSignedQuad();
      tfm = inputstream.ReadSignedQuad();
    }
  else
    {
      packetSize = size;
      charCode = inputstream.ReadByte();
      tfm = inputstream.ReadTrio();
    }

  log_vfchar->WriteLine
    (T_("libdvi"),
     T_("going to read vf character packet"));
  log_vfchar->WriteFormattedLine
    (T_("libdvi"), T_("pl: %d"),
     packetSize);
  log_vfchar->WriteFormattedLine
    (T_("libdvi"),
     T_("cc: %d"),
     charCode);
  log_vfchar->WriteFormattedLine
    (T_("libdvi"),
     T_("tfm: %d"),
     tfm);

  tfm = ScaleFix(tfm, pDviFont->GetScaledAt());
  cx = static_cast<int>(tfm * conv + 0.5);

  log_vfchar->WriteFormattedLine
    (T_("libdvi"),
     T_("dx: %d"),
     cx);

  if (packetSize <= sizeof(smallPacket))
    {
      pPacket = smallPacket;
    }
  else
    {
      pPacket = new BYTE[packetSize];
    }

  inputstream.Read (pPacket, packetSize);
}
