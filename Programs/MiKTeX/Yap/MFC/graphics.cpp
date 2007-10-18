/* graphics.cpp: graphics specials

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include "StdAfx.h"

#include "yap.h"

#include "Dib.h"
#include "DviView.h"
#include "DviDoc.h"

/* _________________________________________________________________________

   DviView::FindGraphicsFile
   _________________________________________________________________________ */

bool
DviView::FindGraphicsFile (/*[in]*/ const char *	lpszFileName,
			   /*[out]*/ PathName &		result)
{
  if (Utils::IsAbsolutePath(lpszFileName))
    {
      result = lpszFileName;
      return (File::Exists(result));
    }
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  MIKTEX_ASSERT (pDoc->GetPathName().GetLength() != 0);
  result = pDoc->GetDocDir();
  result += lpszFileName;
  if (File::Exists(result))
    {
      return (true);
    }
  if (! SessionWrapper(true)->FindFile(lpszFileName,
				       FileType::GRAPHICS,
				       result))
    {
      return (false);
    }
  return (File::Exists(result));
}

/* _________________________________________________________________________

   GetGraphicsType
   _________________________________________________________________________ */

GraphicsInclusion::Type
GetGraphicsType (/*[in]*/ const PathName & fileName)
{
  if (fileName.HasExtension(T_(".bmp")))
    {
      return (GraphicsInclusion::BMP);
    }
  else if (fileName.HasExtension(T_(".emf")))
    {
      return (GraphicsInclusion::EMF);
    }
  else if (fileName.HasExtension(T_(".wmf")))
    {
      return (GraphicsInclusion::WMF);
    }
  else if (SessionWrapper(true)->FindGraphicsRule(fileName.GetExtension(),
						  T_(".bmp"),
						  0,
						  0))
    {
      return (GraphicsInclusion::Other);
    }
  else
    {
      return (GraphicsInclusion::Unknown);
    }
}

/* _________________________________________________________________________

   DviView::LoadEnhMetaFile

   Borrowed from the mfedit sample.
   _________________________________________________________________________ */

HENHMETAFILE
DviView::LoadEnhMetaFile (/*[in]*/ const char *	lpszFileName)
{
  HENHMETAFILE hEmf = 0;

  const DWORD META32_SIGNATURE = 0x464D4520;      // ' EMF'
  const DWORD ALDUS_ID = 0x9AC6CDD7;
  const size_t APMSIZE = 22;

  HANDLE hFile =
    CreateFile(lpszFileName,
	       GENERIC_READ,
	       FILE_SHARE_READ,
	       0,
	       OPEN_EXISTING,
	       FILE_ATTRIBUTE_READONLY,
	       0);

  if (hFile == INVALID_HANDLE_VALUE)
    {
      FATAL_WINDOWS_ERROR (T_("CreateFile"), lpszFileName);
    }

  AutoHANDLE autoCloseFile (hFile);

  HANDLE hMapFile =
    CreateFileMapping(hFile,
		      0,
		      PAGE_READONLY,
		      0,
		      0,
		      "YAPEMF");

  if (hMapFile == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CreateFileMapping"), lpszFileName);
    }

  AutoHANDLE autoCloseFileMapping (hMapFile);

  void * pMapFile = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

  if (pMapFile == 0)
    {
      FATAL_WINDOWS_ERROR (T_("MapViewOfFile"), lpszFileName);
    }

  AutoUnmapViewOfFile autoUnmap (pMapFile);

  LPENHMETAHEADER pEmh = reinterpret_cast<LPENHMETAHEADER>(pMapFile);

  if (pEmh->dSignature == META32_SIGNATURE)
    {
      hEmf = GetEnhMetaFile(lpszFileName);
      if (hEmf == 0)
	{
	  FATAL_MIKTEX_ERROR (T_(""),
			      T_("The metafile could not be loaded."),
			      lpszFileName);
	}
      return (hEmf);
    }
    
  if (*reinterpret_cast<LPDWORD>(pEmh) == ALDUS_ID)
    {
      DWORD size =
	*reinterpret_cast<LPDWORD>(reinterpret_cast<PBYTE>(pMapFile)
				   + APMSIZE
				   + 6);
      // <fixme>use device context</fixme>
      hEmf =
	SetWinMetaFileBits(size * 2,
			   reinterpret_cast<PBYTE>(pMapFile) + APMSIZE,
			   0,
			   0);
      if (hEmf == 0)
	{
	  FATAL_MIKTEX_ERROR (T_(""),
			      T_("The metafile could not be loaded."),
			      lpszFileName);
	}
      return (hEmf);
    }

  HMETAFILE hMf = GetMetaFile(lpszFileName);

  if (hMf == 0)
    {
      FATAL_MIKTEX_ERROR (T_(""),
			  T_("The metafile could not be loaded."),
			  lpszFileName);
    }

  AutoDeleteMetaFile autoDeleteMetaFile (hMf);
    
  UINT size = GetMetaFileBitsEx(hMf, 0, 0);

  if (size == 0)
    {
      FATAL_MIKTEX_ERROR (T_(""),
			  T_("The metafile could not be loaded."),
			  lpszFileName);
    }
    
  void * pvData = malloc(size);

  if (pvData == 0)
    {
      OUT_OF_MEMORY (T_("DviView::LoadEnhMetaFile"));
    }

  AutoMemoryPointer autoFree (pvData);
      
  size = GetMetaFileBitsEx(hMf, size, pvData);

  if (size == 0)
    {
      FATAL_MIKTEX_ERROR (T_(""),
			  T_("The metafile could not be loaded."),
			  lpszFileName);
    }
    
  hEmf =
    SetWinMetaFileBits
    (size,
     const_cast<const PBYTE>(reinterpret_cast<PBYTE>(pvData)),
     0,
     0);

  if (hEmf == 0)
    {
      FATAL_MIKTEX_ERROR (T_(""),
			  T_("The metafile could not be loaded."),
			  lpszFileName);
    }

  return (hEmf);
}

/* _________________________________________________________________________

   DviView::CreateTempBitmapFile
   _________________________________________________________________________ */

TempFile
DviView::CreateTempBitmapFile (/*[in]*/ const PathName &	origFileName)
{
#if 0 // <todo/>
  StatusBarMessage ("Loading %s...", strOrigFileName.c_str());
#endif

  PathName tempFileName;

  StdoutReader reader (0);

  if (! SessionWrapper(true)->ConvertToBitmapFile(origFileName.Get(),
						  tempFileName.GetBuffer(),
						  &reader))
    {
      FATAL_MIKTEX_ERROR (T_("DviView::CreateTempBitmapFile"),
			  T_("Could not make bitmap file."),
			  origFileName.Get());
    }

  return (tempFileName);
}

/* _________________________________________________________________________

   DviView::IncludeGraphics
   _________________________________________________________________________ */

void
DviView::IncludeGraphics (/*[in]*/ int			pageIdx,
			  /*[in]*/ GraphicsSpecial *	pGraphicsSpecial)
{
  // find the graphics file
  PathName fileName;
  if (! FindGraphicsFile(pGraphicsSpecial->GetFileName(), fileName))
    {
      FATAL_MIKTEX_ERROR (T_("DviView::IncludeGraphics"),
			  T_("The graphics file could not be found."),
			  pGraphicsSpecial->GetFileName());
    }

  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);

  GraphicsInclusion
    graphicsInclusion (pGraphicsSpecial->GetX(),
		       pGraphicsSpecial->GetY(),
		       pGraphicsSpecial->GetWidth(),
		       pGraphicsSpecial->GetHeight());

  switch (GetGraphicsType(fileName))
    {
    case GraphicsInclusion::BMP:
      graphicsInclusion.SetDib (new Dib (fileName));
      break;
    case GraphicsInclusion::EMF:
    case GraphicsInclusion::WMF:
      graphicsInclusion.SetEnhancedMetafile (LoadEnhMetaFile(fileName.Get()));
      break;
    case GraphicsInclusion::Other:
      {
	graphicsInclusion.SetDib (new Dib (CreateTempBitmapFile(fileName)));
	break;
      }
    case GraphicsInclusion::Unknown:
      FATAL_MIKTEX_ERROR (T_("DviView::IncludeGraphics"),
			  T_("Unknown graphics inclusion type."),
			  fileName.Get());
    }

  if (graphicsInclusion.GetCx() <= 0 && graphicsInclusion.GetCy() <= 0)
    {
      if (graphicsInclusion.GetDib() != 0)
	{
	  LONG cx, cy;
	  graphicsInclusion.GetDib()->GetSize(&cx, &cy);
	  graphicsInclusion.SetCx (static_cast<int>(cx));
	  graphicsInclusion.SetCy (static_cast<int>(cy));
	}
    }

  pDoc->RememberGraphicsInclusion (pageIdx, graphicsInclusion);
}

/* _________________________________________________________________________

   DviView::RenderGraphicsInclusions
   _________________________________________________________________________ */

void
DviView::RenderGraphicsInclusions (/*[in]*/ CDC *	pDC,
				   /*[in]*/ int		pageIdx)
{
  DviDoc * pDoc = GetDocument();
  ASSERT_VALID (pDoc);
  GraphicsInclusion graphicsInclusion;
  for (size_t idx = 0;
       pDoc->GetGraphicsInclusion(pageIdx, idx, graphicsInclusion);
       ++ idx)
    {
      CPoint origin;
      origin.x = PixelShrink(graphicsInclusion.GetX());
      origin.y = PixelShrink(graphicsInclusion.GetY());
      CSize size;
      size.cx = PixelShrink(graphicsInclusion.GetCx());
      size.cy = PixelShrink(graphicsInclusion.GetCy());
#if 0
      todo ();
      if (graphicsInclusion.hEmf)
	{
	  int logpixelsx = pDC->GetDeviceCaps(LOGPIXELSX);
	  int logpixelsy = pDC->GetDeviceCaps(LOGPIXELSY);
	  CSize grsize;
	  ENHMETAHEADER header;
	  GetEnhMetaFileHeader (graphicsInclusion.hEmf,
				sizeof(header),
				&header);
	  int width = header.rclFrame.right;
	  width = static_cast<int>(((static_cast<float>(width) * logpixelsx)
				    / 2540));
	  int height = header.rclFrame.bottom;
	  height = static_cast<int>(((static_cast<float>(height) * logpixelsy)
				     / 2540));
	  grsize.cx = PixelShrink(width);
	  grsize.cy = PixelShrink(height);
	  origin.x += (size.cx - grsize.cx) / 2;
	  origin.y += (size.cy - grsize.cy) / 2;
	  size = grsize;
	}
#endif
      CRect bbox (origin.x, origin.y, origin.x + size.cx, origin.y + size.cy);
      switch (graphicsInclusion.GetType())
	{
	case GraphicsInclusion::BMP:
	  MIKTEX_ASSERT (graphicsInclusion.GetDib() != 0);
	  graphicsInclusion.GetDib()->Draw (pDC,
					    origin.x,
					    origin.y,
					    size.cx,
					    size.cy);
	  break;
	case GraphicsInclusion::EMF:
	case GraphicsInclusion::WMF:
	  if (! pDC->PlayMetaFile(graphicsInclusion.GetEnhancedMetafile(),
				  bbox))
	    {
	      FATAL_MIKTEX_ERROR (T_("DviView::RenderGraphicsInclusions"),
				  T_("The metafile could not be rendered."),
				  0);
	    }
	  break;
	default:
	  MIKTEX_ASSERT (false);
	  __assume (false);
	  break;
	}
    }
}
