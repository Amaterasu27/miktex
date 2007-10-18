/* dvi.cpp:

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

/* code is heavily borrowed from DVItype and other celebs */

#include "StdAfx.h"

#include "common.h"

/* _________________________________________________________________________

   DviImpl::PushState
   _________________________________________________________________________ */

void
DviImpl::PushState ()
{
  stateStack.push (currentState);
}

/* _________________________________________________________________________

   DviImpl::PopState
   _________________________________________________________________________ */

void
DviImpl::PopState ()
{
  MIKTEX_ASSERT (! stateStack.empty());
  currentState = stateStack.top();
  stateStack.pop ();
}

/* _________________________________________________________________________

   DviImpl::PixelRound

   Round a DVI unit to the nearest pixel value.
   _________________________________________________________________________ */


int
DviImpl::PixelRound (/*[in]*/ int du)
{
  if (du >= 0)
    {
      return (static_cast<int>(du * conv + 0.5));
    }
  else
    {
      return (static_cast<int>(du * conv - 0.5));
    }
}

/* _________________________________________________________________________

   DviImpl::RulePixels

   Compute the number of pixels in the height or width of a rule.
   _________________________________________________________________________ */

int
DviImpl::RulePixels (/*[in]*/ int x)
{
  int n = static_cast<int>(conv * x);
  if (static_cast<double>(n) < (conv * x))
    {
      return (n + 1);
    }
  else
    {
      return (n);
    }
}

/* _________________________________________________________________________

   DviImpl::DviImpl
   _________________________________________________________________________ */

DviImpl::DviImpl (/*[in]*/ const char *		lpszFileName,
		  /*[in]*/ const char *		lpszMetafontMode,
		  /*[in]*/ int				resolution,
		  /*[in]*/ int				shrinkFactor,
		  /*[in]*/ DviAccess			dviAccess,
		  /*[in]*/ DviPageMode			pageMode,
		  /*[in]*/ const PaperSizeInfo &	paperSizeInfo,
		  /*[in]*/ bool				landscape)

  : currentColor (rgbDefaultColor),
    currentPageIdx (-1),
    direction (1),
    dviAccess (dviAccess),
    dviFileName (lpszFileName),
    fatalError (false),
    fatalMiKTeXError (false),
    hasDviFileChanged (false),
    landscape (landscape),
    lastChecked (0),
    lineWidth (0),
    log_color (TraceStream::Open(MIKTEX_TRACE_DVICOLOR)),
    log_dvifile (TraceStream::Open(MIKTEX_TRACE_DVIFILE)),
    log_dvipage (TraceStream::Open(MIKTEX_TRACE_DVIPAGE)),
    log_error (TraceStream::Open(MIKTEX_TRACE_ERROR)),
    log_gc (TraceStream::Open(MIKTEX_TRACE_DVIGC)),
    log_hypertex (TraceStream::Open(MIKTEX_TRACE_DVIHYPERTEX)),
    log_search (TraceStream::Open(MIKTEX_TRACE_DVISEARCH)),
    metafontMode (lpszMetafontMode),
    pCallback (0),
    pageMode (pageMode),
    paperSizeInfo (paperSizeInfo),
    recursion (0),
    resolution (resolution),
    defaultShrinkFactor (shrinkFactor),
    havePaperSizeSpecial (false),
    haveLandscapeSpecial (false)

{
  fqDviFileName = dviFileName;
  fqDviFileName.MakeAbsolute ();

  dviInfo.lastWriteTime = 0;
  pFontMap = new FontMap;
  hByeByteEvent = CreateEvent(0, TRUE, FALSE, 0);
  if (hByeByteEvent == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CreateEvent"), 0);
    }
  hNewPageEvent = CreateEvent(0, FALSE, FALSE, 0);
  if (hNewPageEvent == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CreateEvent"), 0);
    }
  hScannedEvent = CreateEvent(0, FALSE, FALSE, 0);
  if (hScannedEvent == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CreateEvent"), 0);
    }
  if (dviAccess == DviAccess::Random)
    {
      pGarbageCollectorThread =
	auto_ptr<Thread>(Thread::Start(GarbageCollector, this));
      pPageLoaderThread = auto_ptr<Thread>(Thread::Start(PageLoader, this));
    }
}

/* _________________________________________________________________________

   DviImpl::~DviImpl
   _________________________________________________________________________ */

DviImpl::~DviImpl ()
{
  try
    {
      Dispose ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   DviImpl::Dispose
   _________________________________________________________________________ */

void
MIKTEXDVICALL
DviImpl::Dispose ()
{
  if (hByeByteEvent != 0)
    {
      if (SetEvent(hByeByteEvent) == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("SetEvent"), 0);
	}
    }
  if (pGarbageCollectorThread.get())
    {
      pGarbageCollectorThread->Join ();
      pGarbageCollectorThread.reset ();
    }
  if (pPageLoaderThread.get())
    {
      pPageLoaderThread->Join ();
      pPageLoaderThread.reset ();
    }
  MIKTEX_BEGIN_CRITICAL_SECTION(&criticalSectionMonitor)
    {
      FreeContents ();
      if (pFontMap != 0)
	{
	  delete pFontMap;
	  pFontMap = 0;
	}
      if (hByeByteEvent != 0)
	{
	  CloseHandle (hByeByteEvent);
	  hByeByteEvent = 0;
	}
      if (hNewPageEvent)
	{
	  CloseHandle (hNewPageEvent);
	  hNewPageEvent = 0;
	}
      if (hScannedEvent)
	{
	  CloseHandle (hScannedEvent);
	  hScannedEvent = 0;
	}
    }
  MIKTEX_END_CRITICAL_SECTION();
  if (log_dvifile.get() != 0)
    {
      log_dvifile->Close ();
      log_dvifile.reset ();
    }
  if (log_color.get() != 0)
    {
      log_color->Close ();
      log_color.reset ();
    }
  if (log_dvipage.get() != 0)
    {
      log_dvipage->Close ();
      log_dvipage.reset ();
    }
  if (log_error.get() != 0)
    {
      log_error->Close ();
      log_error.reset ();
    }
  if (log_gc.get() != 0)
    {
      log_gc->Close ();
      log_gc.reset ();
    }
  if (log_hypertex.get() != 0)
    {
      log_hypertex->Close ();
      log_hypertex.reset ();
    }
  if (log_search.get() != 0)
    {
      log_search->Close ();
      log_search.reset ();
    }
}

/* _________________________________________________________________________

   DviImpl::FreeContents
   _________________________________________________________________________ */

void
DviImpl::FreeContents (/*[in]*/ bool keepFonts)
{
  for (vector<DviPageImpl*>::iterator itPagePtr = pages.begin();
       itPagePtr != pages.end();
       ++ itPagePtr)
    {
      delete *itPagePtr;
    }
  pages.clear ();
  if (pFontMap == 0 || keepFonts)
    {
      return;
    }
  for (FontMap::iterator it = pFontMap->begin();
       it != pFontMap->end();
       ++ it)
    {
      delete it->second;
      it->second = 0;
    }
  pFontMap->clear ();
}

/* _________________________________________________________________________

   Dvi codes
   _________________________________________________________________________ */

const int set_char = 0;
const int set1 = 128;
const int set2 = 129;
const int set3 = 130;
const int set4 = 131;
const int set_rule = 132;
const int put1 = 133;
const int put2 = 134;
const int put3 = 135;
const int put4 = 136;
const int put_rule = 137;
const int nop = 138;
const int bop = 139;
const int eop = 140;
const int p_ush = 141;
const int p_op = 142;
const int right1 = 143;
const int right2 = 144;
const int right3 = 145;
const int right4 = 146;
const int w0 = 147;
const int w1 = 148;
const int w2 = 149;
const int w3 = 150;
const int w4 = 151;
const int x0 = 152;
const int x1 = 153;
const int x2 = 154;
const int x3 = 155;
const int x4 = 156;
const int down1 = 157;
const int down2 = 158;
const int down3 = 159;
const int down4 = 160;
const int y_0 = 161;
const int y_1 = 162;
const int y2 = 163;
const int y3 = 164;
const int y4 = 165;
const int z0 = 166;
const int z1 = 167;
const int z2 = 168;
const int z3 = 169;
const int z4 = 170;
const int fnt_num = 171;
const int fnt1 = 235;
const int fnt2 = 236;
const int fnt3 = 237;
const int fnt4 = 238;
const int xxx1 = 239;
const int xxx2 = 240;
const int xxx3 = 241;
const int xxx4 = 242;
const int fnt_def1 = 243;
const int fnt_def2 = 244;
const int fnt_def3 = 245;
const int fnt_def4 = 246;
const int pre = 247;
const int post = 248;
const int post_post = 249;

#define FOUR_CASES(x) x: case x+1: case x+2: case x+3
#define SIX_CASES(x) FOUR_CASES(x): case x+4: case x+5

#define undefined_commands SIX_CASES(250)

/* _________________________________________________________________________

   DviImpl::FirstParam
   _________________________________________________________________________ */

int
DviImpl::FirstParam (/*[in]*/ InputStream &	inputStream,
		     /*[in]*/ int		opCode)
{
  if (opCode >= set_char && opCode <= set_char + 127)
    {
      return (opCode - set_char);
    }

  switch (opCode)
    {

    case set1: case put1: case fnt1: case xxx1: case fnt_def1:

      return (inputStream.ReadByte());

    case set1+1: case put1+1: case fnt1+1: case xxx1+1: case fnt_def1+1:

      return (inputStream.ReadPair());

    case set1+2: case put1+2: case fnt1+2: case xxx1+2: case fnt_def1+2:

      return (inputStream.ReadTrio());

    case right1: case w1: case x1: case down1: case y_1: case z1:

      return (inputStream.ReadSignedByte());

    case right1+1: case w1+1: case x1+1: case down1+1: case y_1+1: case z1+1:

      return (inputStream.ReadSignedPair());

    case right1+2: case w1+2: case x1+2: case down1+2: case y_1+2: case z1+2:

      return (inputStream.ReadSignedTrio());

    case set1+3: case set_rule: case put1+3: case put_rule: case right1+3:
    case w1+3: case x1+3: case down1+3: case y_1+3: case z1+3: case fnt1+3:
    case xxx1+3: case fnt_def1+3:

      return (inputStream.ReadSignedQuad());

    case nop: case bop: case eop: case p_ush: case p_op: case pre: case post:
    case post_post: case undefined_commands:

      return (0);

    case w0:

      return (currentState.w);

    case x0:

      return (currentState.x);

    case y_0:

      return (currentState.y);

    case z0:

      return (currentState.z);
    }

  if (opCode >= fnt_num && opCode <= fnt_num + 63)
    {
      return (opCode - fnt_num);
    }

  UNEXPECTED_CONDITION (T_("DviImpl::FirstParam"));
}

/* _________________________________________________________________________

   DviImpl::Scan
   _________________________________________________________________________ */

const int dvi_id = 2;

void
MIKTEXDVICALL
DviImpl::Scan ()
{
  InputStream inputStream (dviFileName.Get());
  
  // reset this object
  FreeContents (false);
  
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("going to scan %s"),
     Q_(dviFileName));
  
  // prepare reading the Dvi file
  dviInfo.lastWriteTime = File::GetLastWriteTime(dviFileName);
  
  // process the preamble
  if (inputStream.ReadByte() != pre || inputStream.ReadByte() != dvi_id)
    {
      FATAL_DVI_ERROR (T_("DviImpl::Scan"),
		       T_("Not a DVI file."),
		       dviFileName.Get());
    }
  
  // compute the conversion factor
  numerator = inputStream.ReadSignedQuad();
  denominator = inputStream.ReadSignedQuad();
  
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("numerator/denominator: %d/%d"),
     static_cast<int>(numerator),
     static_cast<int>(denominator));
  
  if (numerator <= 0 || denominator <= 0)
    {
      FATAL_DVI_ERROR (T_("DviImpl::Scan"),
		       T_("Invalid DVI file."),
		       dviFileName.Get());
    }
  
  // get desired magnification
  mag = inputStream.ReadSignedQuad();
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("mag: %d"),
     static_cast<int>(mag));
  
  tfmConv = (((25400000.0 / numerator )
	      * (denominator / 473628672))
	     / 16.0);
  
  conv = ((static_cast<double>(numerator)
	   * static_cast<double>(mag)
	   * static_cast<double>(resolution))
	  / (static_cast<double>(denominator)
	     * 254000000.0));
  
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("conv: %g"),
     static_cast<double>(conv));
  
  // read the introductory comment
  int len = inputStream.ReadByte();
  char tmp[256];
  inputStream.Read (tmp, len);
  tmp[len] = 0;
  dviInfo.comment = tmp;
  
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("comment: %s"),
     dviInfo.comment.c_str());
  
  // find the postamble, working back from the end
  int k;
  inputStream.SetReadPosition (0, SeekOrigin::End);
  int filelength = inputStream.GetReadPosition();
  int m = filelength - 1;
  do
    {
      inputStream.SetReadPosition (m, SeekOrigin::Begin);
      k = inputStream.ReadByte();
      -- m;
    }
  while (k == 223);
  if (k != dvi_id)
    {
      FATAL_DVI_ERROR (T_("DviImpl::Scan"),
		       T_("Invalid DVI file."),
		       dviFileName.Get());
    }
  inputStream.SetReadPosition (m - 3, SeekOrigin::Begin);
  int q = inputStream.ReadSignedQuad();
  if (q < 0 || q > m - 33)
    {
      FATAL_DVI_ERROR (T_("DviImpl::Scan"),
		       T_("Invalid DVI file."),
		       dviFileName.Get());
    }
  inputStream.SetReadPosition (q, SeekOrigin::Begin);
  k = inputStream.ReadByte();
  if (k != post)
    {
      FATAL_DVI_ERROR (T_("DviImpl::Scan"),
		       T_("Invalid DVI file."),
		       dviFileName.Get());
    }
  
  // process the postamble
  int firstbackpointer = 	// pointer to last page
    inputStream.ReadSignedQuad();
  /*int postamble_num =*/ inputStream.ReadSignedQuad();
  /*int postamble_den =*/ inputStream.ReadSignedQuad();
  /*int postamble_mag =*/ inputStream.ReadSignedQuad();
  maxV = inputStream.ReadSignedQuad();
  maxH = inputStream.ReadSignedQuad();
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("maxv: %d"),
     static_cast<int>(maxV));
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("maxh: %d"),
     static_cast<int>(maxH));
  int maxs = inputStream.ReadPair();
  dviInfo.nPages = inputStream.ReadPair();
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("maxs: %d"),
     static_cast<int>(maxs));
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("dviInfo.nPages: %d"),
     static_cast<int>(dviInfo.nPages));
  
  // process the font definitions of the postamble
  do
    {
      k = inputStream.ReadByte();
      if (k >= fnt_def1 && k < fnt_def1 + 4)
	{
	  int p = FirstParam(inputStream, k);
	  DefineFont (inputStream, p);
	  k = nop;
	}
    }
  while (k == nop);
  
  if (k != post_post)
    {
      FATAL_DVI_ERROR (T_("DviImpl::Scan"),
		       T_("Invalid DVI file."),
		       dviFileName.Get());
    }
  
  // build the page table
  int backpointer = firstbackpointer;
  minPageNumber = INT_MAX;
  maxPageNumber = INT_MIN;
  pages.reserve (dviInfo.nPages);
  while (backpointer >= 0 && (dviInfo.nPages-- > 0))
    {
      inputStream.SetReadPosition (backpointer, SeekOrigin::Begin);
      if (inputStream.ReadByte() != bop)
	{
	  FATAL_DVI_ERROR (T_("DviImpl::Scan"),
			   T_("Invalid DVI file."),
			   dviFileName.Get());
	}
      int count0 = inputStream.ReadSignedQuad();
      if (count0 < minPageNumber)
	{
	  minPageNumber = count0;
	}
      if (count0 > maxPageNumber)
	{
	  maxPageNumber = count0;
	}
      int count1 = inputStream.ReadSignedQuad();
      int count2 = inputStream.ReadSignedQuad();
      int count3 = inputStream.ReadSignedQuad();
      int count4 = inputStream.ReadSignedQuad();
      int count5 = inputStream.ReadSignedQuad();
      int count6 = inputStream.ReadSignedQuad();
      int count7 = inputStream.ReadSignedQuad();
      int count8 = inputStream.ReadSignedQuad();
      int count9 = inputStream.ReadSignedQuad();
      backpointer = inputStream.ReadSignedQuad();
      DviPageImpl * pPage =
	new DviPageImpl(this,
			dviInfo.nPages,
			pageMode,
			inputStream.GetReadPosition(),
			count0, count1, count2, count3, count4,
			count5, count6, count7, count8, count9);
      pages.push_back (pPage);
    }
  reverse (pages.begin(), pages.end());
  lastChecked = clock();

#if 0
  // load the first DVI page
  if (pages.size() > 0)
    {
      DviPage * pPage = GetLoadedPage(0);
      pPage->Unlock ();
    }
#endif

  // wake up page loader thread
  SetEvent (hScannedEvent);
  
  hasDviFileChanged = false;
}

/* _________________________________________________________________________

   DviImpl::DefineFont
   _________________________________________________________________________ */

void
DviImpl::DefineFont (/*[in]*/ InputStream &	inputStream,
		     /*[in]*/ int		fontNum)
{
  log_dvifile->WriteFormattedLine
    (T_("libdvi"),
     T_("going to define font %d"),
     static_cast<int>(fontNum));

  int checkSum = inputStream.ReadSignedQuad();
  int scaledSize = inputStream.ReadSignedQuad();
  int designSize = inputStream.ReadSignedQuad();
  int areaNameLen = inputStream.ReadByte();
  int fontNameLen = inputStream.ReadByte();

  char areaName[256];
  if (areaNameLen > 0)
    {
      inputStream.Read (areaName, areaNameLen);
    }
  areaName[areaNameLen] = 0;

  char fontName[256];
  inputStream.Read (fontName, fontNameLen);
  fontName[fontNameLen] = 0;

  log_dvifile->WriteFormattedLine
    (T_("libdvi"), T_("areaName: %s"), areaName);
  log_dvifile->WriteFormattedLine
    (T_("libdvi"), T_("fontname: %s"), fontName);
  log_dvifile->WriteFormattedLine (T_("libdvi"), "checkSum: %#o", checkSum);
  log_dvifile->WriteFormattedLine (T_("libdvi"), "scaledSize: %d", scaledSize);
  log_dvifile->WriteFormattedLine (T_("libdvi"), "designSize: %d", designSize);
  
  DviFont * pfont;
  PathName fileName;
  if (SessionWrapper(true)->FindFile(fontName, FileType::VF, fileName)
      || SessionWrapper(true)->FindFile(fontName, FileType::OVF, fileName))
    {
      log_dvifile->WriteFormattedLine
	(T_("libdvi"),
	 T_("found VF file %s"),
	 Q_(fileName));
      pfont =
	new VFont(this,
		  checkSum,
		  scaledSize,
		  designSize,
		  areaName,
		  fontName,
		  fileName.Get(),
		  tfmConv,
		  conv,
		  mag,
		  metafontMode.c_str(),
		  resolution);
    }
  else if (pageMode == DviPageMode::Pk)
    {
      pfont =
	new PkFont(this,
		   checkSum,
		   scaledSize,
		   designSize,
		   areaName,
		   fontName,
		   T_(""),
		   tfmConv,
		   conv,
		   mag,
		   metafontMode.c_str(),
		   resolution);
    }
  else
    {
      pfont =
	new Tfm(this,
		checkSum,
		scaledSize,
		designSize,
		areaName,
		fontName,
		T_(""),
		tfmConv,
		conv);
    }

  (*pFontMap)[fontNum] = pfont;
}

/* _________________________________________________________________________

   DviImpl::GetPageStatus
   _________________________________________________________________________ */

PageStatus
MIKTEXDVICALL
DviImpl::GetPageStatus (/*[in]*/ int	pageIdx)
{
  CheckCondition ();
  MIKTEX_BEGIN_CRITICAL_SECTION (&criticalSectionMonitor)
    {
      MIKTEX_ASSERT (pageIdx >= 0);
      
      if (pageIdx >= GetNumberOfPages())
	{
	  return (PageStatus::Unknown);
	}
      
      DviPageImpl * pPage = pages[pageIdx];
      
      MIKTEX_ASSERT (pPage != 0);

      pPage->Lock ();

      AutoUnlockPage autoUnlockPage (pPage);
      
      if (! pPage->IsFrozen())
	{
	  return (PageStatus::NotLoaded);
	}
      
      if (hasDviFileChanged)
	{
	  return (PageStatus::Changed);
	}
      
      clock_t now = clock();
      
      if (now <= lastChecked + CLOCKS_PER_SEC)
	{
	  return (PageStatus::Loaded);
	}
      
      lastChecked = now;

      if (SessionWrapper(true)->IsFileAlreadyOpen(dviFileName.Get()))
	{
	  return (PageStatus::Loaded);
	}
      
      if (! File::Exists(dviFileName))
	{
	  return (PageStatus::Lost);
	}

      time_t oldTime = dviInfo.lastWriteTime;
      dviInfo.lastWriteTime = File::GetLastWriteTime(dviFileName);
      if (oldTime != dviInfo.lastWriteTime)
	{
	  hasDviFileChanged = true;
	  return (PageStatus::Changed);
	}
      
      return (PageStatus::Loaded);
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   DviImpl::DoPage
   _________________________________________________________________________ */

// we insist that abs(hh-pixelRound(h)) <= max_drift
const int max_drift = 1;

void
DviImpl::DoPage (/*[in]*/ int pageIdx)
{
  currentFontNumber = 0;
  pCurrentFont = 0;
  pCurrentChar = 0;
  currentState = DviState();
  currentFontNumber = 0;

  DviPageImpl & page = *pages[pageIdx];

  page.Lock ();

  AutoUnlockPage autoUnlockPage (&page);

  Progress (DviNotification::BeginLoadPage,
	    T_("loading page #%d..."),
	    pageIdx);

  bool background = 
    (pPageLoaderThread.get() != 0
     && Thread::IsCurrentThread(pPageLoaderThread.get()));

  if (background)
    {
      log_dvipage->WriteFormattedLine
	(T_("libdvi"),
	 T_("doing page #%d (background)"),
	 pageIdx);
    }
  else
    {
      log_dvipage->WriteFormattedLine
	(T_("libdvi"),
	 T_("doing page #%d"),
	 pageIdx);
    }

  try
    {
      if (SessionWrapper(true)->IsFileAlreadyOpen(dviFileName.Get()))
	{
	  log_error->WriteLine
	    (T_("libdvi"),
	     T_("the DVI file is used by another process"));
	  throw DviFileInUseException
	    (0,
	     T_("The DVI file is used by another process."),
	     0,
	     T_(__FILE__),
	     __LINE__);
	}
    
      InputStream inputStream (dviFileName.Get());

      inputStream.SetReadPosition (page.GetReadPosition(), SeekOrigin::Begin);

      pointTable.clear ();

      while (DoNextCommand(inputStream, page))
	{
	  ;
	}

      page.Freeze ();
    }

  catch (const DviFileInUseException &)
    {
      page.Freeze ();
      throw;
    }
}

/* _________________________________________________________________________

   DviImpl::DoNextCommand
   _________________________________________________________________________ */

bool
DviImpl::DoNextCommand (/*[in]*/ InputStream &		inputStream,
			/*[in]*/ DviPageImpl &		page)
{
  MIKTEX_ASSERT (hByeByteEvent != 0);

  DWORD wait = WaitForSingleObject(hByeByteEvent, 0);
  if (wait == WAIT_FAILED)
    {
      FATAL_WINDOWS_ERROR (T_("WaitForSingleObject"), 0);
    }
  if (wait == WAIT_OBJECT_0)
    {
      throw OperationCancelledException
	(0,
	 T_("Operation cancelled."),
	 0,
	 T_(__FILE__),
	 __LINE__);
    }

  int opCode = inputStream.ReadByte();

  int p, q;		// parameters of the current command
  p = FirstParam(inputStream, opCode);
      
  if (opCode <= set4 || opCode >= put1 && opCode <= put4)
    {
      
      // translate a char command

      if (pCurrentFont == 0)
	{
	  FATAL_DVI_ERROR (T_("DviImpl::DoNextCommand"),
			   T_("Invalid DVI file."),
			   0);
	}
      
      VFont * pVFont = dynamic_cast<VFont*>(pCurrentFont);

      if (pVFont != 0)
	{
	  const int maxRecursion = 20;
	  
	  if (recursion >= maxRecursion)
	    {
	      log_error->WriteLine
		(T_("libdvi"),
		 T_("infinite VF recursion?"));
	      FATAL_DVI_ERROR (T_("DviImpl::DoNextCommand"),
			       T_("Invalid DVI file."),
			       0);
	    }

	  VfChar * pVfChar = pVFont->GetCharAt(p);
	  
	  if (pVfChar == 0)
	    {
	      UNEXPECTED_CONDITION (T_("DviImpl::DoNextCommand"));
	    }

	  AutoRestore<FontMap *> autoRestorFontMap (pFontMap);
	  AutoRestore<int> autoRestoreFontNumber (currentFontNumber);
	  AutoRestore<DviFont *> autoRestoreCurrentFont (pCurrentFont);
	  AutoRestore<VfChar *> autoRestoreCurrentVfChar (pCurrentVfChar);

	  pFontMap  = const_cast<FontMap*>(&(pVFont->GetFontMap()));

	  if (pFontMap->size() > 0)
	    {
	      const pair<int, DviFont*> & pair = *(pFontMap->begin());
	      currentFontNumber = pair.first;
	      pCurrentFont = pair.second;
	    }
	  else
	    {
	      currentFontNumber = 0;
	      pCurrentFont = 0;
	    }

	  pCurrentVfChar = pVfChar;

	  PushState ();

	  currentState.w = 0;
	  currentState.x = 0;
	  currentState.y = 0;
	  currentState.z = 0;

	  unsigned long pl;
	  const BYTE * p = pVfChar->GetPacket(pl);
	  
	  InputStream inputStream (p, pl);

	  {
	    AutoRestore<int> autoRestoreRecursion (recursion);
	    ++ recursion;
	    while (! inputStream.IsEndOfStream())
	      {
		DoNextCommand(inputStream, page);
	      }
	  }

	  PopState ();

	  pCurrentChar = pVfChar;

	  goto fin_set;
	}
      else
	{
	  pCurrentFont->Read ();
	  PkFont * pPkFont = dynamic_cast<PkFont*>(pCurrentFont);
	  if (pPkFont != 0)
	    {
	      PkChar * pPkChar = (*pPkFont)[p];
	      pCurrentChar = pPkChar;
	      if (pCurrentChar == 0)
		{
		  UNEXPECTED_CONDITION (T_("DviImpl::DoNextCommand"));
		}
	      DviItem item;
	      item.x = currentState.hh + resolution;
	      item.y = currentState.vv + resolution;
	      item.pPkChar = pPkChar;
	      item.rgbForeground = currentColor;
	      item.rgbBackground = 0x00ffffff;
	      page.AddItem (item);
	      ExpandBoundingBox (item.GetLeftUns(),
				 item.GetBottomUns(),
				 item.GetRightUns(),
				 item.GetTopUns());
	    }
	  else
	    {
	      Tfm * pTfm = dynamic_cast<Tfm*>(pCurrentFont);
	      if (pTfm == 0)
		{
		  UNEXPECTED_CONDITION (T_("DviImpl::DoNextCommand"));
		}
	      pCurrentChar = (*pTfm)[p];
	      if (pCurrentChar == 0)
		{
		  UNEXPECTED_CONDITION (T_("DviImpl::DoNextCommand"));
		}
	      int x = currentState.hh + resolution;
	      int y = currentState.vv + resolution;
	      ExpandBoundingBox
		(x,
		 y,
		 x + pCurrentChar->GetWidth(),
		 y - PixelRound(pCurrentFont->GetScaledAt()) + 1);


	    }

	  goto fin_set;
	}
    }
  else
    {

      // translate a non-char command

      switch (opCode)
	{

	case set_rule:

	  goto fin_rule;

	case put_rule:

	  goto fin_rule;

	case nop:

	  return (true);

	case bop:

	  FATAL_DVI_ERROR (T_("DviImpl::DoNextCommand"),
			   T_("Invalid DVI file."),
			   0);
	  break;

	case eop:

	  if (! stateStack.empty())
	    {
	      FATAL_DVI_ERROR (T_("DviImpl::DoNextCommand"),
			       T_("Invalid DVI file."),
			       0);
	    }
	  if (recursion > 0)
	    {
	      FATAL_DVI_ERROR (T_("DviImpl::DoNextCommand"),
			       T_("Invalid DVI file."),
			       0);
	    }
	  return (false);

	case p_ush:

	  PushState ();
	  return (true);

	case p_op:

	  PopState ();
	  return (true);
	  
	case w0:
	case FOUR_CASES(w1):

	  if (recursion > 0)
	    {
	      p =
		static_cast<int>
		(ScaleFix(p,
			  static_cast<int>(pCurrentVfChar->GetScaledAt()
					   / tfmConv))
		 * tfmConv);
	    }

	  currentState.w = p;

	  goto out_space;

	case x0:
	case FOUR_CASES(x1):

	  if (recursion > 0)
	    {
	      p =
		static_cast<int>
		(ScaleFix(p,
			  static_cast<int>(pCurrentVfChar->GetScaledAt()
					   / tfmConv))
		 * tfmConv);
	    }

	  currentState.x = p;

	  goto out_space;

	case FOUR_CASES(right1):

	  if (recursion > 0)
	    {
	      p =
		static_cast<int>
		(ScaleFix(p,
			  static_cast<int>(pCurrentVfChar->GetScaledAt()
					   / tfmConv))
		 * tfmConv);
	    }

	out_space:
	  if (pCurrentFont != 0
	      && (p >= pCurrentFont->GetInterWordSpacing()
		  || p <= - pCurrentFont->GetBackSpacing()))
	    {
	      currentState.hh = PixelRound(currentState.h + p);
	    }
	  else
	    {
	      currentState.hh += PixelRound(p);
	    }
	  q = p;
	  goto move_right;
	
	default:

	  SpecialCases(inputStream, opCode, p, page);
	  return (true);
	}
    }
  
 fin_set:

  // finish a command that either sets or puts a character, then goto
  // move_right or return

  if (p < 0)
    {
      p = 255 - ((-1 - p) % 256);
    }
  else if (p >= 256)
    {
      p %= 256;		// width computation for oriental fonts
    }

  q = pCurrentChar->GetDviWidth();

  if (opCode >= put1)
    {
      return (true);
    }

  currentState.hh += pCurrentChar->GetWidth();

  goto move_right;
  
 fin_rule:

  // finish a command that either sets or puts a rule, then goto
  // move_right or return

  q = inputStream.ReadSignedQuad();
  if (p > 0 && q > 0)
    {
      if (recursion > 0)
	{
	  p =
	    static_cast<int>
	    (ScaleFix(p,
		      static_cast<int>(pCurrentVfChar->GetScaledAt()
				       / tfmConv))
	     * tfmConv);
	  q =
	    static_cast<int>
	    (ScaleFix(q,
		      static_cast<int>(pCurrentVfChar->GetScaledAt()
				       / tfmConv))
	     * tfmConv);
	}

      if (pageMode == DviPageMode::Pk)
	{
	  page.AddRule (new DviRuleImpl(this,
					currentState.hh + resolution,
					currentState.vv + resolution,
					RulePixels(q),
					RulePixels(p),
					currentColor));
	}
    }

  if (opCode == put_rule)
    {
      return (true);
    }

  currentState.hh = currentState.hh + RulePixels(q);

  goto move_right;
  
 move_right:

  // finish a command that sets h=h+q, then return
  {
    int hhh = PixelRound(currentState.h + q); // h, rounded to the nearest pxl
    if (abs(hhh - currentState.hh) > max_drift)
      {
	if (hhh > currentState.hh)
	  {
	    currentState.hh = hhh - max_drift;
	  }
	else
	  {
	    currentState.hh = hhh + max_drift;
	  }
      }
    currentState.h += q;
    // MIKTEX_ASSERT (currentState.h < maxH);
  }
  return (true);
}

/* _________________________________________________________________________

   DviImpl::SpecialCases
   _________________________________________________________________________ */

void
DviImpl::SpecialCases (/*[in]*/ InputStream &	inputStream,
		       /*[in]*/ int		opCode,
		       /*[in]*/ int		p,
		       /*[in]*/ DviPageImpl &	page)
{
  if (opCode >= fnt_num && opCode <= fnt_num+63)
    {
      goto change_font;
    }
  
  switch (opCode)
    {
    case y_0:
    case FOUR_CASES(y_1):
      if (recursion > 0)
	{
	  p =
	    static_cast<int>
	    (ScaleFix(p,
		      static_cast<int>(pCurrentVfChar->GetScaledAt()
				       / tfmConv))
	     * tfmConv);
	}
      currentState.y = p;
      goto out_vmove;

    case z0:
    case FOUR_CASES(z1):
      if (recursion > 0)
	{
	  p =
	    static_cast<int>
	    (ScaleFix(p,
		      static_cast<int>(pCurrentVfChar->GetScaledAt()
				       / tfmConv))
	     * tfmConv);
	}
      currentState.z = p;
      goto out_vmove;

    case down1: case down2: case down3: case down4:
      if (recursion > 0)
	{
	  p =
	    static_cast<int>
	    (ScaleFix(p,
		      static_cast<int>(pCurrentVfChar->GetScaledAt()
				       / tfmConv))
	     * tfmConv);
	}
    out_vmove:
      if (pCurrentFont != 0 && abs(p) >= pCurrentFont->GetLineSpacing())
	{
	  currentState.vv = PixelRound(currentState.v + p);
	}
      else
	{
	  currentState.vv += PixelRound(p);
	}
      goto move_down;

    case fnt1: case fnt2: case fnt3: case fnt4:
      goto change_font;

    case FOUR_CASES(fnt_def1):
      // skip a font definition
      {
	inputStream.SkipBytes (12);
	int areaNameLen = inputStream.ReadByte();
	int fontNameLen = inputStream.ReadByte();
	inputStream.SkipBytes (areaNameLen + fontNameLen);
      }
      return;

    case FOUR_CASES(xxx1):
      // translate an xxx command and return
      {
	if (p < 0)
	  {
	    FATAL_DVI_ERROR (T_("DviImpl::SpecialCases"),
			     T_("Invalid DVI file."),
			     0);
	  }
	int x = currentState.hh + resolution;
	int y = currentState.vv + resolution;
	DviSpecial * pSpecial = 0;
	if (InterpretSpecial(&page, x, y, inputStream, p, pSpecial)
	    && pSpecial != 0)
	  {
	    page.AddSpecial (pSpecial);
	  }
      }
      return;
      
    case pre:
      FATAL_DVI_ERROR (T_("DviImpl::SpecialCases"),
		       T_("Invalid DVI file."),
		       0);
      
    case post:
    case post_post:
      FATAL_DVI_ERROR (T_("DviImpl::SpecialCases"),
		       T_("Invalid DVI file."),
		       0);

    default:
      FATAL_DVI_ERROR (T_("DviImpl::SpecialCases"),
		       T_("Invalid DVI file."),
		       0);
    }
  
 move_down:
  // finish a command that sets v=v+p, then return
  {
    int vvv =
      PixelRound(currentState.v + p); // v, rounded to the nearest pixel
    if (abs(vvv - currentState.vv) > max_drift)
      {
	if (vvv > currentState.vv)
	  {
	    currentState.vv = vvv - max_drift;
	  }
	else
	  {
	    currentState.vv = vvv + max_drift;
	  }
      }
    currentState.v += p;
    // MIKTEX_ASSERT (currentState.v < maxV);
    return;
  }
  
 change_font:
  // finish a command that changes the current font, then return
  currentFontNumber = p;
  pCurrentFont = (*pFontMap)[p];
  if (pCurrentFont == 0)
    {
      FATAL_DVI_ERROR (T_("DviImpl::SpecialCases"),
		       T_("Invalid DVI file."),
		       0);
    }
}

/* _________________________________________________________________________

   Dvi::Create
   _________________________________________________________________________ */

Dvi *
Dvi::Create (/*[in]*/ const char *	lpszFileName,
	     /*[in]*/ const char *	lpszMetafontMode,
	     /*[in]*/ int		resolution,
	     /*[in]*/ int		shrinkFactor,
	     /*[in]*/ DviAccess		dviAccess,
	     /*[in]*/ IDviCallback *	pCallback)
{
  PaperSizeInfo defaultPaperSizeInfo;
  if (! SessionWrapper(true)->GetPaperSizeInfo(-1, defaultPaperSizeInfo))
    {
      UNEXPECTED_CONDITION (T_("Dvi::Create"));
    }
  return (Create(lpszFileName,
		 lpszMetafontMode,
		 resolution,
		 shrinkFactor,
		 dviAccess,
		 DEFAULT_PAGE_MODE,
		 defaultPaperSizeInfo,
		 false,
		 pCallback));
}

/* _________________________________________________________________________

   Dvi::Create
   _________________________________________________________________________ */

Dvi *
Dvi::Create (/*[in]*/ const char *		lpszFileName,
	     /*[in]*/ const char *		lpszMetafontMode,
	     /*[in]*/ int			resolution,
	     /*[in]*/ int			shrinkFactor,
	     /*[in]*/ DviAccess			dviAccess,
	     /*[in]*/ DviPageMode		pageMode,
	     /*[in]*/ const PaperSizeInfo &	paperSizeInfo,
	     /*[in]*/ bool			landscape,
	     /*[in]*/ IDviCallback *		pCallback)
{
  DviImpl * pDvi =
    new DviImpl(lpszFileName,
		lpszMetafontMode,
		resolution,
		shrinkFactor,
		dviAccess,
		pageMode,
		paperSizeInfo,
		landscape);
  pDvi->pCallback = pCallback;
  return (pDvi);
}

/* _________________________________________________________________________

   Dvi::~Dvi
   _________________________________________________________________________ */

Dvi::~Dvi ()
{
}

/* _________________________________________________________________________

   DviRuleImpl::DviRuleImpl
   _________________________________________________________________________ */


DviRuleImpl::DviRuleImpl (/*[in]*/ DviImpl *		pDviImpl,
			  /*[in]*/ int			x,
			  /*[in]*/ int			y,
			  /*[in]*/ int			width,
			  /*[in]*/ int			height,
			  /*[in]*/ unsigned long	rgb)

  : flags (0),
    height (height),
    pDviImpl (pDviImpl),
    rgb (rgb),
    width (width),
    x (x),
    y (y)

{
}

/* _________________________________________________________________________

   DviRuleImpl::IsBlackboard
   _________________________________________________________________________ */

bool
DviRuleImpl::IsBlackboard ()
{
  return (GetFlag(flblackboard));
}

/* _________________________________________________________________________

   DviRuleImpl::GetLeft
   _________________________________________________________________________ */

int
DviRuleImpl::GetLeft (/*[in]*/ int shrinkFactor)
{
  return (pDviImpl->WidthShrink(shrinkFactor, x));
}

/* _________________________________________________________________________

   DviRuleImpl::GetRight
   _________________________________________________________________________ */

int
DviRuleImpl::GetRight (/*[in]*/ int shrinkFactor)
{
  return (GetLeft(shrinkFactor)
	  + pDviImpl->WidthShrink(shrinkFactor, width)
	  - 1);
}

/* _________________________________________________________________________

   DviRuleImpl::GetTop
   _________________________________________________________________________ */

int
DviRuleImpl::GetTop (/*[in]*/ int shrinkFactor)
{
  return (pDviImpl->WidthShrink(shrinkFactor, y)
	  - pDviImpl->WidthShrink(shrinkFactor, height)
	  + 1);
}

/* _________________________________________________________________________

   DviRuleImpl::GetBottom
   _________________________________________________________________________ */

int
DviRuleImpl::GetBottom (/*[in]*/ int shrinkFactor)
{
  return (GetTop(shrinkFactor)
	  + pDviImpl->WidthShrink(shrinkFactor, height) - 1);
}

/* _________________________________________________________________________

   DviRuleImpl::GetBackgroundColor
   _________________________________________________________________________ */

unsigned long
DviRuleImpl::GetBackgroundColor ()
{
  return (rgb);
}

/* _________________________________________________________________________

   DviRuleImpl::GetForegroundColor
   _________________________________________________________________________ */

unsigned long
DviRuleImpl::GetForegroundColor ()
{
  return (rgb);
}

/* _________________________________________________________________________

   DviImpl::GetNumberOfPages
   _________________________________________________________________________ */

int
MIKTEXDVICALL
DviImpl::GetNumberOfPages ()
{
  CheckCondition ();
  MIKTEX_BEGIN_CRITICAL_SECTION (&criticalSectionMonitor)
    {
      return (static_cast<int>(pages.size()));
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   DviImpl::GetPage
   _________________________________________________________________________ */

DviPage *
MIKTEXDVICALL
DviImpl::GetPage (/*[in]*/ int pageIdx)
{
  CheckCondition ();
  MIKTEX_BEGIN_CRITICAL_SECTION (&criticalSectionMonitor)
    {
      if (pageIdx >= GetNumberOfPages())
	{
	  return (0);
	}
      DviPageImpl * pPage = pages[pageIdx];
      pPage->Lock ();
      try
	{
	  if ((pPageLoaderThread.get() == 0
	       || ! Thread::IsCurrentThread(pPageLoaderThread.get()))
	      && (pGarbageCollectorThread.get() == 0
		  || ! Thread::IsCurrentThread(pGarbageCollectorThread.get()))
	      && currentPageIdx != pageIdx)
	    {
	      log_dvifile->WriteFormattedLine
		(T_("libdvi"),
		 T_("getting page #%d"),
		 static_cast<int>(pageIdx));
	      if (pageIdx < currentPageIdx)
		{
		  direction = -1;
		}
	      else if (pageIdx > currentPageIdx)
		{
		  direction = 1;
		}
	      currentPageIdx = pageIdx;
	      if (! SetEvent(hNewPageEvent))
		{
		  FATAL_WINDOWS_ERROR (T_("SetEvent"), 0);
		}
	    }
	  return (pPage);
	}
      catch (const exception &)
	{
	  try
	    {
	      pPage->Unlock ();
	    }
	  catch (const exception &)
	    {
	    }
	  throw;
	}
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   DviImpl::GetMinPageNumber
   _________________________________________________________________________ */

int
MIKTEXDVICALL
DviImpl::GetMinPageNumber ()
{
  CheckCondition ();
  return (minPageNumber);
}

/* _________________________________________________________________________

   DviImpl::GetMaxPageNumber
   _________________________________________________________________________ */

int
MIKTEXDVICALL
DviImpl::GetMaxPageNumber ()
{
  CheckCondition ();
  return (maxPageNumber);
}

/* _________________________________________________________________________

   DviImpl::GetMagnification
   _________________________________________________________________________ */

int
MIKTEXDVICALL
DviImpl::GetMagnification ()
{
  CheckCondition ();
  return (mag);
}

/* _________________________________________________________________________

   DviImpl::GetMaxH
   _________________________________________________________________________ */

int
MIKTEXDVICALL
DviImpl::GetMaxH ()
{
  CheckCondition ();
  return (maxH);
}

/* _________________________________________________________________________

   DviImpl::GetMaxV
   _________________________________________________________________________ */

int
MIKTEXDVICALL
DviImpl::GetMaxV ()
{
  CheckCondition ();
  return (maxV);
}

/* _________________________________________________________________________

   DviImpl::GetLoadedPage
   _________________________________________________________________________ */

DviPage *
MIKTEXDVICALL
DviImpl::GetLoadedPage (/*[in]*/ int	pageIdx)
{
  CheckCondition ();
  MIKTEX_BEGIN_CRITICAL_SECTION(&criticalSectionMonitor)
    {
      switch (GetPageStatus(pageIdx).Get())
	{
	case PageStatus::Lost:
	case PageStatus::Unknown:
	  return (0);
	case PageStatus::Changed:
	  Scan ();		// fall through
	case PageStatus::NotLoaded:
	  DoPage (pageIdx);	// fall through
	case PageStatus::Loaded:
	  {
	    DviPageImpl * pPage =
	      reinterpret_cast<DviPageImpl*>(GetPage(pageIdx));
	    MIKTEX_ASSERT (pPage != 0);
	    pPage->SetAutoClean (dviAccess == DviAccess::Sequential);
	    return (pPage);
	  }
	default:
	  MIKTEX_ASSERT (false);
	  __assume (false);
	}
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   DviImpl::Progress
   _________________________________________________________________________ */

void
DviImpl::Progress (/*[in]*/ DviNotification	nf,
		   /*[in]*/ const char *	lpszFormat,
		   /*[in]*/			...)
{
  if ((pPageLoaderThread.get() != 0
       && Thread::IsCurrentThread(pPageLoaderThread.get()))
      || (pGarbageCollectorThread.get() != 0
	  && Thread::IsCurrentThread(pGarbageCollectorThread.get())))
    {
      return;
    }

  if (lpszFormat != 0)
    {
      MIKTEX_BEGIN_CRITICAL_SECTION(&criticalSectionMonitor_StatusText)
	{
	  va_list args;
	  va_start (args, lpszFormat);
	  progressStatus = Utils::FormatString(lpszFormat, args);
	}
      MIKTEX_END_CRITICAL_SECTION();
    }

  if (pCallback != 0)
    {
      pCallback->OnProgress (nf);
    }
}

/* _________________________________________________________________________

   DviImpl::GetStatusText
   _________________________________________________________________________ */

string
MIKTEXDVICALL
DviImpl::GetStatusText ()
{
  CheckCondition ();
  MIKTEX_BEGIN_CRITICAL_SECTION(&criticalSectionMonitor_StatusText)
    {
      return (progressStatus);
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   DviImpl::PageLoader
   _________________________________________________________________________ */

const unsigned long sleepDurationLowestPrio		= 1000;	// milliseconds
const unsigned long sleepDurationBelowNormalPrio	=  100;
const unsigned long sleepDurationNormalPrio		=   10;
const unsigned long sleepDurationAboveNormalPrio	=    0;
const unsigned long sleepDurationHighestPrio		=    0;

const unsigned long limitBelowNormalPrio		=   2 * 1024 * 1024;
const unsigned long limitNormalPrio			=  20 * 1024 * 1024;
const unsigned long limitAboveNormalPrio		=  50 * 1024 * 1024;
const unsigned long limitHighestPrio			= 100 * 1024 * 1024;

void
MIKTEXCALLBACK
DviImpl::PageLoader (/*[in]*/ void * p)
{
  DviImpl * This = reinterpret_cast<DviImpl *>(p);

  try
    {
#if 1
      if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST) == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("SetThreadPriority"), 0);
	}
#endif
      
      MIKTEX_ASSERT (This->hByeByteEvent != 0);
      MIKTEX_ASSERT (This->hNewPageEvent != 0);
      MIKTEX_ASSERT (This->hScannedEvent != 0);
      
      HANDLE handles[2];

      handles[0] = This->hByeByteEvent;
      handles[1] = This->hScannedEvent;

      unsigned long wait =
	WaitForMultipleObjects(2,
			       handles,
			       FALSE,
			       INFINITE);

      if (wait == WAIT_FAILED)
	{
	  FATAL_WINDOWS_ERROR (T_("WaitForMultipleObjects"), 0);
	}

      if (wait == WAIT_OBJECT_0)
	{
	  return;
	}

      for (int pageIdx = This->currentPageIdx + 1;
	   ((wait = WaitForSingleObject(This->hByeByteEvent,
					sleepDurationBelowNormalPrio))
	    != WAIT_OBJECT_0);
	   pageIdx += This->direction)
	{
	  if (wait == WAIT_FAILED)
	    {
	      FATAL_WINDOWS_ERROR (T_("WaitForSingleObject"), 0);
	    }
	  DviPage * pPage = 0;
	  AutoUnlockPage autoUnlockPage (0);
	  MIKTEX_BEGIN_CRITICAL_SECTION(&This->criticalSectionMonitor)
	    {
	      wait = WaitForSingleObject(This->hNewPageEvent, 0);
	      if (wait == WAIT_FAILED)
		{
		  FATAL_WINDOWS_ERROR (T_("WaitForSingleObject"), 0);
		}
	      if (wait == WAIT_OBJECT_0)
		{
		  pageIdx = This->currentPageIdx;
		}
	      if (pageIdx < 0 || pageIdx >= This->GetNumberOfPages())
		{
		  pageIdx = This->currentPageIdx;
		  continue;
		}
	      if (This->direction > 0)
		{
		  if (! (pageIdx >= This->currentPageIdx
			 && pageIdx < This->currentPageIdx + 10))
		    {
		      continue;
		    }
		}
	      else
		{
		  MIKTEX_ASSERT (This->direction < 0);
		  if (! (pageIdx <= This->currentPageIdx
			 && pageIdx > This->currentPageIdx - 10))
		    {
		      continue;
		    }
		}
	      DviPageImpl * pPageImpl = This->pages[pageIdx];
	      if (pPageImpl->IsLocked())
		{
		  continue;
		}
	      pPage = This->GetLoadedPage(pageIdx);
	      autoUnlockPage.Attach (pPage);
	    }
	  MIKTEX_END_CRITICAL_SECTION();
	  if (pPage != 0)
	    {
	      pPage->GetNumberOfDviBitmaps (This->defaultShrinkFactor);
	    }
	}
    }

  catch (const MiKTeXException & e)
    {
      This->miktexException = e;
      This->fatalMiKTeXError = true;
    }

  catch (const exception & e)
    {
      This->stdException = e;
      This->fatalError = true;
    }
}

/* _________________________________________________________________________

   DviImpl::GarbageCollector
   _________________________________________________________________________ */

const time_t timeKeepBitmapsLowestPrio			= 120; // seconds
const time_t timeKeepBitmapsBelowNormalPrio		=  60;
const time_t timeKeepBitmapsNormalPrio			=  30;
const time_t timeKeepBitmapsAboveNormalPrio		=  10;
const time_t timeKeepBitmapsHighestPrio			=  5;

void
MIKTEXCALLBACK
DviImpl::GarbageCollector (/*[in]*/ void * p)
{
  DviImpl * This = reinterpret_cast<DviImpl *>(p);
  try
    {
      int priority = THREAD_PRIORITY_LOWEST;
      unsigned long sleepDuration = sleepDurationLowestPrio;
      time_t timeKeepBitmaps = timeKeepBitmapsLowestPrio;
      if (SetThreadPriority(GetCurrentThread(), priority) == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("SetThreadPriority"), 0);
	}
      DWORD wait;
      while ((wait = WaitForSingleObject(This->hByeByteEvent, sleepDuration))
	     != WAIT_OBJECT_0)
	{
	  if (wait == WAIT_FAILED)
	    {
	      FATAL_WINDOWS_ERROR (T_("WaitForSingleObject"), 0);
	    }
	  size_t sizeBiggest = 0;
	  int biggestPageIdx = -1;
	  DviPageImpl * pPage;
	  size_t totalSize = 0;
	  time_t now = time(0);
	  for (int pageIdx = 0;
	       ((wait = WaitForSingleObject(This->hByeByteEvent, 0))
		!= WAIT_OBJECT_0);
	       ++ pageIdx)
	    {
	      if (wait == WAIT_FAILED)
		{
		  FATAL_WINDOWS_ERROR (T_("WaitForSingleObject"), 0);
		}
	      MIKTEX_BEGIN_CRITICAL_SECTION(&This->criticalSectionMonitor)
		{
		  if (pageIdx >= This->GetNumberOfPages())
		    {
		      break;
		    }
		  if (This->direction > 0)
		    {
		      if (pageIdx >= This->currentPageIdx
			  && pageIdx < This->currentPageIdx + 10)
			{
			  continue;
			}
		    }
		  else
		    {
		      MIKTEX_ASSERT (This->direction < 0);
		      if (pageIdx <= This->currentPageIdx
			  && pageIdx > This->currentPageIdx - 10)
			{
			  continue;
			}
		    }
		  DviPageImpl * pPage = This->pages[pageIdx];
		  totalSize += pPage->GetSize();
		  if (! pPage->IsFrozen())
		    {
		      continue;
		    }
		  time_t timeLastVisit = pPage->GetTimeLastVisit();
		  if (timeLastVisit + timeKeepBitmaps < now)
		    {
		      if (pPage->GetSize() > sizeBiggest
			  && ! pPage->IsLocked())
			{
			  sizeBiggest = pPage->GetSize();
			  biggestPageIdx = pageIdx;
			}
		    }
		}
	      MIKTEX_END_CRITICAL_SECTION();
	    }
	  wait = WaitForSingleObject(This->hByeByteEvent, 0);
	  if (wait == WAIT_FAILED)
	    {
	      FATAL_WINDOWS_ERROR (T_("WaitForSingleObject"), 0);
	    }
	  if (wait == WAIT_OBJECT_0)
	    {
	      break;
	    }
	  int newPriority;
	  if (totalSize > limitHighestPrio)
	    {
#if 0
	      newPriority = THREAD_PRIORITY_HIGHEST;
#else
	      newPriority = THREAD_PRIORITY_NORMAL;
#endif
	      sleepDuration = sleepDurationHighestPrio;
	      timeKeepBitmaps = timeKeepBitmapsHighestPrio;
	    }
	  else if (totalSize > limitAboveNormalPrio)
	    {
#if 0
	      newPriority = THREAD_PRIORITY_ABOVE_NORMAL;
#else
	      newPriority = THREAD_PRIORITY_NORMAL;
#endif
	      sleepDuration = sleepDurationAboveNormalPrio;
	      timeKeepBitmaps = timeKeepBitmapsAboveNormalPrio;
	    }
	  else if (totalSize > limitNormalPrio)
	    {
	      newPriority = THREAD_PRIORITY_NORMAL;
	      sleepDuration = sleepDurationBelowNormalPrio;
	      timeKeepBitmaps = timeKeepBitmapsNormalPrio;
	    }
	  else if (totalSize > limitBelowNormalPrio)
	    {
	      newPriority = THREAD_PRIORITY_BELOW_NORMAL;
	      sleepDuration = sleepDurationBelowNormalPrio;
	      timeKeepBitmaps = timeKeepBitmapsBelowNormalPrio;
	    }
	  else
	    {
	      newPriority = THREAD_PRIORITY_LOWEST;
	      sleepDuration = sleepDurationLowestPrio;
	      timeKeepBitmaps = timeKeepBitmapsLowestPrio;
	    }
	  if (newPriority != priority)
	    {
	      priority = newPriority;
	      if (SetThreadPriority(GetCurrentThread(), priority) == 0)
		{
		  FATAL_WINDOWS_ERROR (T_("SetThreadPriority"), 0);
		}
	      This->log_gc->WriteFormattedLine
		(T_("libdvi"),
		 T_("gc priority: %d"),
		 static_cast<int>(priority));
	    }
	  if (biggestPageIdx < 0)
	    {
	      continue;
	    }
	  MIKTEX_BEGIN_CRITICAL_SECTION(&This->criticalSectionMonitor)
	    {
	      if (biggestPageIdx >= This->GetNumberOfPages())
		{
		  continue;
		}
	      pPage = This->pages[biggestPageIdx];
	      if (pPage->IsLocked())
		{
		  continue;
		}
	      pPage->Lock ();
	      AutoUnlockPage autoUnlockPage (pPage);
	      This->log_gc->WriteFormattedLine
		(T_("libdvi"),
		 T_("freeing page #%d (%u bytes)"),
		 biggestPageIdx,
		 pPage->GetSize());
	      pPage->FreeContents (false, false);
	    }
	  MIKTEX_END_CRITICAL_SECTION();
	}
    }

  catch (const MiKTeXException & e)
    {
      This->miktexException = e;
      This->fatalMiKTeXError = true;
    }

  catch (const exception & e)
    {
      This->stdException = e;
      This->fatalError = true;
    }
}

/* _________________________________________________________________________

   DviImpl::MakeFonts
   _________________________________________________________________________ */

bool
DviImpl::MakeFonts (/*[in]*/ const FontMap &	fontMap,
		    /*[in]*/ int		recursion)
{
  bool done = true;
  for (FontMap::const_iterator it = fontMap.begin();
       it != fontMap.end();
       ++ it)
    {
      if (it->second->IsNotLoadable())
	{
	  done = false;
	  continue;
	}
      it->second->Read ();
      if (it->second->IsNotLoadable())
	{
	  done = false;
	  continue;
	}
      VFont * pVFont = dynamic_cast<VFont*>(it->second);
      if (pVFont == 0)
	{
	  continue;
	}
      const int maxRecursion = 20;
      if (recursion >= maxRecursion)
	{
	  log_error->WriteLine
	    (T_("libdvi"),
	     T_("infinite VF recursion?"));
	}
      done = done && MakeFonts(pVFont->GetFontMap(), recursion + 1);
    }
  return (done);
}

/* _________________________________________________________________________

   DviImpl::MakeFonts
   _________________________________________________________________________ */

bool
MIKTEXDVICALL
DviImpl::MakeFonts ()
{
  CheckCondition ();
  MIKTEX_BEGIN_CRITICAL_SECTION(&criticalSectionMonitor)
    {
      if (pFontMap == 0)
	{
	  UNEXPECTED_CONDITION (T_("DviImpl::MakeFonts"));
	}
      return (MakeFonts(*pFontMap, 0));
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   DviImpl::GetFontTable
   _________________________________________________________________________ */

void
DviImpl::GetFontTable (/*[in]*/ const FontMap &		fontMap,
		       /*[in]*/ vector<DviFontInfo> &	vec,
		       /*[in]*/ int			recursion)
{
  for (FontMap::const_iterator it = fontMap.begin();
       it != fontMap.end();
       ++ it)
    {
      DviFontInfo info;
      it->second->GetInfo (info);
      vec.push_back (info);
      VFont * pVFont = dynamic_cast<VFont*>(it->second);
      if (pVFont == 0)
	{
	  continue;
	}
      const int maxRecursion = 20;
      if (recursion >= maxRecursion)
	{
	  log_error->WriteLine (T_("libdvi"), T_("infinite VF recursion?"));
	}
      GetFontTable (pVFont->GetFontMap(), vec, recursion + 1);
    }
}

/* _________________________________________________________________________

   DviImpl::GetFontTable
   _________________________________________________________________________ */

vector<DviFontInfo>
MIKTEXDVICALL
DviImpl::GetFontTable ()
{
  CheckCondition ();
  MIKTEX_BEGIN_CRITICAL_SECTION(&criticalSectionMonitor)
    {
      vector<DviFontInfo> ret;
      MIKTEX_ASSERT (pFontMap != 0);
      if (pFontMap != 0)
	{
	  GetFontTable (*pFontMap, ret, 0);
	}
      return (ret);
    }
  MIKTEX_END_CRITICAL_SECTION();
}

/* _________________________________________________________________________

   DviImpl::CheckCondition
   _________________________________________________________________________ */

void
DviImpl::CheckCondition ()
{
  if (fatalError)
    {
      fatalError = false;
      throw stdException;
    }
  if (fatalMiKTeXError)
    {
      fatalMiKTeXError = false;
      throw miktexException;
    }
}

/* _________________________________________________________________________

   DviImpl::Lock
   _________________________________________________________________________ */

void
DviImpl::Lock ()
{
  criticalSectionMonitor.Enter ();
}

/* _________________________________________________________________________

   DviImpl::Unlock
   _________________________________________________________________________ */

void
DviImpl::Unlock ()
{
  criticalSectionMonitor.Leave ();
}

