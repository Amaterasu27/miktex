/* chunkdib.cpp: test driver for the DibChunker interfaces

   Copyright (C) 2002-2006 Christian Schenk

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include <memory>

#include <cstdio>
#include <cstdlib>

#include <miktex/app.h>
#include <miktex/core.h>
#include <miktex/DibChunker.h>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX::Graphics;
using namespace MiKTeX;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

/* _________________________________________________________________________

   ChunkDib
   _________________________________________________________________________ */

class ChunkDib
  : public Application,
    public IDibChunkerCallback
{
public:
  ChunkDib ()
    : nChunks (0),
      nBitmaps (0)
  {
  }

public:
  virtual
  size_t
  DIBCHUNKERCALL
  Read (/*[out]*/ void *	pBuf,
	/*[in]*/ size_t		size);

public:
  virtual
  void
  DIBCHUNKERCALL
  OnNewChunk (/*[in]*/ DibChunk * pChunk);

public:
  void
  Run (/*[in]*/ int			argc,
       /*[in]*/ const MIKTEXCHAR **	argv);

private:
  FileStream stream;

private:
  unsigned long nChunks;

private:
  unsigned long nBitmaps;
};

/* _________________________________________________________________________

   ChunkDib::Read
   _________________________________________________________________________ */

size_t
DIBCHUNKERCALL
ChunkDib::Read (/*[out]*/ void *	pBuf,
		/*[in]*/ size_t		size)
{
  return (stream.Read(pBuf, size));
}

/* _________________________________________________________________________

   ChunkDib::OnNewChunk
   _________________________________________________________________________ */

void
DIBCHUNKERCALL
ChunkDib::OnNewChunk (/*[in]*/ DibChunk * pChunk)
{
  nChunks += 1;
  PathName fileName;
  _stprintf_s (fileName.GetBuffer(),
	       fileName.GetSize(),
	       T_("chunk-%u-%u.bmp"),
	       nBitmaps,
	       nChunks);
  FileStream bitmapFile (File::Open(fileName,
				    FileMode::Create,
				    FileAccess::Write,
				    false));
  BITMAPINFOHEADER bitmapInfoHeader = pChunk->GetBitmapInfoHeader();
  unsigned long nBytesPerLine =
    ((((bitmapInfoHeader.biWidth
	* bitmapInfoHeader.biBitCount)
       + 31)
      & ~31)
     >> 3);
  unsigned nColors =
    (bitmapInfoHeader.biBitCount == 24
     ? 0
     : 1 << bitmapInfoHeader.biBitCount);
  BITMAPFILEHEADER header;
  header.bfType = 0x4d42;
  header.bfSize =
    (sizeof(header)
     + sizeof(BITMAPINFOHEADER)
     + nColors * sizeof(RGBQUAD)
     + nBytesPerLine * bitmapInfoHeader.biHeight);
  header.bfReserved1 = 0;
  header.bfReserved2 = 0;
  header.bfOffBits =
    (sizeof(header)
     + sizeof(BITMAPINFOHEADER)
     + nColors * sizeof(RGBQUAD));
  bitmapFile.Write (&header, sizeof(header));
  bitmapFile.Write (&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));
  bitmapFile.Write (pChunk->GetColors(), nColors * sizeof(RGBQUAD));
  bitmapFile.Write (pChunk->GetBits(),
		    nBytesPerLine * bitmapInfoHeader.biHeight);
  bitmapFile.Close ();
  printf (T_("chunk %s written\n"), fileName.Get());
}

/* _________________________________________________________________________

   ChunkDib::Run
   _________________________________________________________________________ */

void
ChunkDib::Run (/*[in]*/ int			argc,
	       /*[in]*/ const MIKTEXCHAR **	argv)
{
  unsigned long chunkSize;
  auto_ptr<DibChunker> pChunker (DibChunker::Create());
  if (argc > 1)
    {
      chunkSize = static_cast<unsigned long>(File::GetSize(argv[1])) / 5;
      stream.Attach (File::Open(argv[1],
				FileMode::Open,
				FileAccess::Read,
				false));
    }
  else
    {
      chunkSize = 2 * 1024 * 1024;
      stream.Attach (stdin);
    }
  nBitmaps = 0;
  do
    {
      ++ nBitmaps;
      nChunks = 0;
    }
  while (pChunker->Process(DibChunker::Default,
			   chunkSize,
			   this));
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR **	argv)
{
  try
    {
      ChunkDib app;
      app.Init (argv[0]);
      app.Run (argc, argv);
      app.Finalize ();
      return (0);
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (const MIKTEXCHAR *	lpszMessage)
    {
      fprintf (stderr, "fatal error: %s\n", lpszMessage);
      return (1);
    }
}
