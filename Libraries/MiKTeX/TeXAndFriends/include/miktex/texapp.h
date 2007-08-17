/* texapp.h: TeX, pdfTeX, e-TeX, Omega				-*- C++ -*-

   Copyright (C) 1996-2007 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(GUARD_87C7C6104A0DDE499613CA76FF1B8B2F_)
#define GUARD_87C7C6104A0DDE499613CA76FF1B8B2F_

#include <miktex/texmfapp.h>

namespace texapp {
#include <miktex/texapp.defaults.h>
}

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   SourceSpecials
   _________________________________________________________________________ */

class SourceSpecialsEnum
{
public:
  enum EnumType {
    Auto,
    CarriageReturn,
    Display,
    HorizontalBox,
    Math,
    Paragraph,
    ParagraphEnd,
    VerticalBox,
  };
};

typedef MiKTeX::Core::EnumWrapper<SourceSpecialsEnum> SourceSpecials;

/* _________________________________________________________________________

   TeXApp
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
TeXApp

  : public TeXMFApp

{
public:
  MIKTEXMFEXPORT
  MIKTEXMFCALL
  TeXApp ();

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(void)
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName);

  /* _______________________________________________________________________
     
     Finalize
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  Finalize ();

  /* _______________________________________________________________________
     
     AddOptions
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(void)
  AddOptions ();
  
  /* _______________________________________________________________________
     
     GetInputFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetInputFileType ()
    const
  {
    return (MiKTeX::Core::FileType::TEX);
  }

  /* _______________________________________________________________________
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(bool)
  ProcessOption (/*[in]*/ int			c,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg);

  /* _______________________________________________________________________
     
     GetFormatIdent
     _______________________________________________________________________ */

public:

  virtual
  int
  MIKTEXMFCALL
  GetFormatIdent ()
    const
  {
#if defined(THEDATA)
    return (THEDATA(formatident));
#else
    MIKTEX_ASSERT (false);
    return (0);
#endif
  }

  /* _______________________________________________________________________
     
     GetMemoryDumpFileExtension
     _______________________________________________________________________ */

public:

  virtual
  const char *
  MIKTEXMFCALL
  GetMemoryDumpFileExtension ()
    const
  {
    return (".fmt");
  }

  /* _______________________________________________________________________
     
     GetMemoryDumpFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetMemoryDumpFileType ()
    const
  {
    return (MiKTeX::Core::FileType::FMT);
  }

  /* _______________________________________________________________________
     
     GetPoolFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetPoolFileType ()
    const
  {
    return (MiKTeX::Core::FileType::TEXPOOL);
  }

  /* _______________________________________________________________________
     
     OnTeXMFStartJob
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  OnTeXMFStartJob ();

  /* _______________________________________________________________________
     
     AllocateMemory
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  AllocateMemory ()
  {
    GETPARAM (param_mem_bot, membot, mem_bot, 0);

    if (THEDATA(membot) < 0 || THEDATA(membot) > 1)
      {
	MiKTeX::Core::Session::FatalMiKTeXError
	  (MIKTEXTEXT("TeXApp::AllocateMemory"),
	   MIKTEXTEXT("mem_bot must be 0 or 1."),
	   0,
	   MIKTEXTEXT(__FILE__),
	   __LINE__);
      }

    TeXMFApp::AllocateMemory ();

    GETPARAMCHECK (param_max_in_open,
		   maxinopen,
		   max_in_open,
		   texapp::texapp::max_in_open());
    GETPARAMCHECK (param_nest_size,
		   nestsize,
		   nest_size,
		   texapp::texapp::nest_size());
    GETPARAMCHECK (param_save_size,
		   savesize,
		   save_size,
		   texapp::texapp::save_size());
    GETPARAM (param_trie_size,
	      triesize,
	      trie_size,
	      texapp::texapp::trie_size());
   
    Allocate (THEDATA(sourcefilenamestack), THEDATA(maxinopen));
    Allocate (THEDATA(linestack), THEDATA(maxinopen));
    Allocate (THEDATA(fullsourcefilenamestack), THEDATA(maxinopen));
    Allocate (THEDATA(inputfile), THEDATA(maxinopen));
    Allocate (THEDATA(nest), THEDATA(nestsize) + 1);
    Allocate (THEDATA(savestack), THEDATA(savesize) + 1);
    Allocate (THEDATA(triehash), THEDATA(triesize) + 1);
    Allocate (THEDATA(triel), THEDATA(triesize) + 1);
    Allocate (THEDATA(trieo), THEDATA(triesize) + 1);
    Allocate (THEDATA(trier), THEDATA(triesize) + 1);
    Allocate (THEDATA(trietaken), THEDATA(triesize));

    Allocate (THEDATA(nameoffile),
	      MiKTeX::Core::BufferSizes::MaxPath + 1);

#if ! defined(MIKTEX_OMEGA)
    GETPARAMCHECK (param_hyph_size,
		   hyphsize,
		   hyph_size,
		   texapp::texapp::hyph_size());
    GETPARAM (param_font_max,
	      fontmax,
	      font_max,
	      texapp::texapp::font_max());
    GETPARAMCHECK (param_font_mem_size,
		   fontmemsize,
		   font_mem_size,
		   texapp::texapp::font_mem_size());

    Allocate (THEDATA(trietrl), THEDATA(triesize));
    Allocate (THEDATA(trietro), THEDATA(triesize));
    Allocate (THEDATA(trietrc), THEDATA(triesize));

    Allocate (THEDATA(hyphword), THEDATA(hyphsize) + 1);
    Allocate (THEDATA(hyphlist), THEDATA(hyphsize) + 1);
    Allocate (THEDATA(hyphlink), THEDATA(hyphsize) + 1);

    Allocate (THEDATA(bcharlabel), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(charbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(depthbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(extenbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontarea), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontbc), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontbchar), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontcheck), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontdsize), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontec), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontfalsebchar), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontglue), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontname), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontparams), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontsize), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(fontused), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(heightbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(hyphenchar), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(italicbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(kernbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(ligkernbase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(parambase), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(skewchar), THEDATA(fontmax) + 1 - constfontbase);
    Allocate (THEDATA(triec), THEDATA(triesize) + 1);
    Allocate (THEDATA(widthbase), THEDATA(fontmax) + 1 - constfontbase);

    if (IsInitProgram() || ! AmITeXCompiler() || AmI(MIKTEXTEXT("omega")))
      {
	Allocate (THEDATA(fontinfo), THEDATA(fontmemsize) + 1);
      }
#endif // not Omega

#if defined(MIKTEX_OMEGA)
    GETPARAM (param_trie_op_size,
	      trieopsize,
	      trie_op_size,
	      omega::omega::trie_op_size());

    Allocate (THEDATA(hyfdistance), THEDATA(trieopsize));
    Allocate (THEDATA(hyfnext), THEDATA(trieopsize));
    Allocate (THEDATA(hyfnum), THEDATA(trieopsize));
    Allocate (THEDATA(trie), THEDATA(triesize) + 1);
    Allocate (THEDATA(trieophash), 2 * THEDATA(trieopsize) + 1);
    Allocate (THEDATA(trieoplang), THEDATA(trieopsize));
    Allocate (THEDATA(trieopval), THEDATA(trieopsize));
#endif // Omega

  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     FreeMemory
     _______________________________________________________________________ */

public:
  
#if defined(THEDATA)
  void
  FreeMemory ()
  {
    TeXMFApp::FreeMemory ();

    Free (THEDATA(linestack));
    Free (THEDATA(inputstack));
    Free (THEDATA(inputfile));
    Free (THEDATA(fullsourcefilenamestack));
    Free (THEDATA(sourcefilenamestack));
    Free (THEDATA(nest));
    Free (THEDATA(savestack));
    Free (THEDATA(triec));
    Free (THEDATA(triehash));
    Free (THEDATA(triel));
    Free (THEDATA(trieo));
    Free (THEDATA(trier));
    Free (THEDATA(trietaken));

    Free (THEDATA(nameoffile));

#if ! defined(MIKTEX_OMEGA)
    Free (THEDATA(hyphword));
    Free (THEDATA(hyphlist));
    Free (THEDATA(hyphlink));

    Free (THEDATA(trietrl));
    Free (THEDATA(trietro));
    Free (THEDATA(trietrc));

    Free (THEDATA(bcharlabel));
    Free (THEDATA(charbase));
    Free (THEDATA(depthbase));
    Free (THEDATA(extenbase));
    Free (THEDATA(fontarea));
    Free (THEDATA(fontbc));
    Free (THEDATA(fontbchar));
    Free (THEDATA(fontcheck));
    Free (THEDATA(fontdsize));
    Free (THEDATA(fontec));
    Free (THEDATA(fontfalsebchar));
    Free (THEDATA(fontglue));
    Free (THEDATA(fontinfo));
    Free (THEDATA(fontname));
    Free (THEDATA(fontparams));
    Free (THEDATA(fontsize));
    Free (THEDATA(fontused));
    Free (THEDATA(heightbase));
    Free (THEDATA(hyphenchar));
    Free (THEDATA(italicbase));
    Free (THEDATA(kernbase));
    Free (THEDATA(ligkernbase));
    Free (THEDATA(parambase));
    Free (THEDATA(skewchar));
    Free (THEDATA(widthbase));
#endif // not Omega

#if defined(MIKTEX_OMEGA)
    Free (THEDATA(hyfdistance));
    Free (THEDATA(hyfnext));
    Free (THEDATA(hyfnum));
    Free (THEDATA(trie));

    Free (THEDATA(trieophash));
    Free (THEDATA(trieoplang));
    Free (THEDATA(trieopval));
#endif
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     MLTeXP
     _______________________________________________________________________ */

public:

  bool
  MLTeXP ()
    const
  {
    return (enableMLTeX);
  }

  /* _______________________________________________________________________
     
     EncTeXP
     _______________________________________________________________________ */

public:

  bool
  EncTeXP ()
    const
  {
    return (enableEncTeX);
  }

  /* _______________________________________________________________________
     
     Write18P
     _______________________________________________________________________ */

public:

  bool
  Write18P ()
    const
  {
    return (enableWrite18);
  }

  /* _______________________________________________________________________
     
     IsSourceSpecialOn
     _______________________________________________________________________ */

public:

  bool
  IsSourceSpecialOn (/*[in]*/ SourceSpecials s)
    const
  {
    return (sourceSpecials[s.Get()]);
  }

  /* _______________________________________________________________________
     
     IsNewSource
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  bool
  IsNewSource (/*[in]*/ int	sourceFileName,
	       /*[in]*/ int	line)
    const
  {
    TEXMFCHAR szFileName[MiKTeX::Core::BufferSizes::MaxPath];
    GetTeXString (szFileName,
		  sourceFileName,
		  MiKTeX::Core::BufferSizes::MaxPath);
    return (lastSourceFilename != szFileName || lastLineNum != line);
  }
#endif
  
  /* _______________________________________________________________________
     
     MakeSrcSpecial
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  int
  MakeSrcSpecial (/*[in]*/ int	sourceFileName,
		  /*[in]*/ int	line)
  {
    poolpointer oldpoolptr = THEDATA(poolptr);
    TEXMFCHAR szFileName[MiKTeX::Core::BufferSizes::MaxPath];
    GetTeXString (szFileName,
		  sourceFileName,
		  MiKTeX::Core::BufferSizes::MaxPath);
    MiKTeX::Core::PathName fileName = UnmangleNameOfFile(szFileName);
    const size_t BUFSIZE = MiKTeX::Core::BufferSizes::MaxPath + 100;
    char szBuf[BUFSIZE];
#if _MSC_VER >= 1400
    sprintf_s (szBuf,
	       BUFSIZE,
	       "src:%d%s%s",
	       line,
	       (isdigit(fileName[0]) ? " " : ""),
	       fileName.Get());
#else
    sprintf (szBuf,
	     "src:%d%s%s",
	     line,
	     (isdigit(fileName[0]) ? " " : ""),
	     fileName.Get());
#endif
    size_t len = MiKTeX::Core::StrLen(szBuf);
    CheckPoolPointer (THEDATA(poolptr), len);
    char * lpsz = szBuf;
    while (*lpsz != 0)
      {
	THEDATA(strpool)[THEDATA(poolptr)++] = *lpsz++;
      }
    return (oldpoolptr);
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     RememberSourceInfo
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  RememberSourceInfo (/*[in]*/ int	sourceFileName,
		      /*[in]*/ int	line)
  {
    TEXMFCHAR szFileName[MiKTeX::Core::BufferSizes::MaxPath];
    GetTeXString (szFileName,
		  sourceFileName,
		  MiKTeX::Core::BufferSizes::MaxPath);
    lastSourceFilename = szFileName;
    lastLineNum = line;
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     Write18
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(bool)
  Write18 (/*[in]*/ const char *	lpszCommand,
	   /*[out]*/ int &		exitCode)
    const;

  /* _______________________________________________________________________
     
     Write18
     _______________________________________________________________________ */

public:

  MIKTEXMFAPI(bool)
  Write18 (/*[in]*/ const wchar_t *	lpszCommand,
	   /*[out]*/ int &		exitCode)
    const;

  /* _______________________________________________________________________
     
     Variables
     _______________________________________________________________________ */

private:
  bool enableWrite18;

private:
  bool enableMLTeX;

private:
  bool enableEncTeX;
  
private:
  int lastLineNum;

private:
  MiKTeX::Core::PathName lastSourceFilename;

private:
  std::bitset<32> sourceSpecials;

private:
  int param_hyph_size;
  
private:
  int param_font_max;
  
private:
  int param_font_mem_size;
  
private:
  int param_max_in_open;
  
private:
  int param_mem_bot;
  
private:
  int param_nest_size;
  
private:
  int param_save_size;

private:
  int param_trie_op_size;
  
private:
  int param_trie_size;

private:
  int optBase;
};

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// texapp.h
