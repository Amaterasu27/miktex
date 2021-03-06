/* miktex/TeXAndFriends/TeXApp.h:				-*- C++ -*-

   Copyright (C) 1996-2010 Christian Schenk

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

#include <miktex/TeXAndFriends/TeXMFApp>

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
MIKTEXMFTYPEAPI(TeXApp)

  : public TeXMFApp

{
public:
  MIKTEXMFEXPORT
  MIKTEXTHISCALL
  TeXApp ();

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(void)
  Init (/*[in]*/ const char * lpszProgramInvocationName);

  /* _______________________________________________________________________
     
     Finalize
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(void)
  Finalize ();

  /* _______________________________________________________________________
     
     AddOptions
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(void)
  AddOptions ();
  
  /* _______________________________________________________________________
     
     GetInputFileType
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(MiKTeX::Core::FileType)
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
  MIKTEXMFTHISAPI(bool)
  ProcessOption (/*[in]*/ int		c,
		 /*[in]*/ const char *	lpszOptArg);

  /* _______________________________________________________________________
     
     GetFormatIdent
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(int)
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
  MIKTEXMFTHISAPI(const char *)
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
  MIKTEXMFTHISAPI(MiKTeX::Core::FileType)
  GetMemoryDumpFileType ()
    const
  {
    return (MiKTeX::Core::FileType::FMT);
  }

  /* _______________________________________________________________________
     
     OnTeXMFStartJob
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(void)
  OnTeXMFStartJob ();

  /* _______________________________________________________________________
     
     CheckMemory
     _______________________________________________________________________ */

#if defined(MIKTEX_DEBUG)
public:
  virtual
  void
  CheckMemory ()
  {
#if defined(THEDATA)
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(linestack));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(inputstack));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(inputfile));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fullsourcefilenamestack));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(sourcefilenamestack));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(nest));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(savestack));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(triec));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(triehash));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(triel));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trieo));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trier));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trietaken));

    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(nameoffile));

#if ! defined(MIKTEX_OMEGA)
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(hyphword));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(hyphlist));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(hyphlink));

    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trietrl));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trietro));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trietrc));

    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(bcharlabel));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(charbase));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(depthbase));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(extenbase));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontarea));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontbc));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontbchar));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontcheck));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontdsize));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontec));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontfalsebchar));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontglue));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontinfo));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontname));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontparams));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontsize));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(fontused));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(heightbase));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(hyphenchar));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(italicbase));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(kernbase));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(ligkernbase));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(parambase));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(skewchar));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(widthbase));
#endif // not Omega

#if defined(MIKTEX_OMEGA)
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(hyfdistance));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(hyfnext));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(hyfnum));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trie));

    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trieophash));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trieoplang));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(trieopval));
#endif
#endif // THEDATA

    TeXMFApp::CheckMemory ();
  }
#endif

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
	  ("TeXApp::AllocateMemory",
	   MIKTEXTEXT("mem_bot must be 0 or 1."),
	   0,
	   __FILE__,
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

#if ! defined(MIKTEX_OMEGA)
    GETPARAM(param_hash_extra,
	     hashextra,
	     hash_extra,
	     texapp::texapp::hash_extra());
#endif

    Allocate ("sourcefilenamestack",
	      THEDATA(sourcefilenamestack),
	      THEDATA(maxinopen));
    Allocate ("linestack",
	      THEDATA(linestack),
	      THEDATA(maxinopen));
    Allocate ("fullsourcefilenamestack",
	      THEDATA(fullsourcefilenamestack),
	      THEDATA(maxinopen));
    Allocate ("inputfile",
	      THEDATA(inputfile),
	      THEDATA(maxinopen));
    Allocate ("nest",
	      THEDATA(nest),
	      THEDATA(nestsize) + 1);
    Allocate ("savestack",
	      THEDATA(savestack),
	      THEDATA(savesize) + 1);
    Allocate ("triehash",
	      THEDATA(triehash),
	      THEDATA(triesize) + 1);
    Allocate ("triel",
	      THEDATA(triel),
	      THEDATA(triesize) + 1);
    Allocate ("trieo",
	      THEDATA(trieo),
	      THEDATA(triesize) + 1);
    Allocate ("trier",
	      THEDATA(trier),
	      THEDATA(triesize) + 1);
    Allocate ("trietaken",
	      THEDATA(trietaken),
	      THEDATA(triesize));

    Allocate ("nameoffile",
	      THEDATA(nameoffile),
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

    Allocate ("trietrl", THEDATA(trietrl), THEDATA(triesize));
    Allocate ("trietro", THEDATA(trietro), THEDATA(triesize));
    Allocate ("trietrc", THEDATA(trietrc), THEDATA(triesize));

    Allocate ("hyphword", THEDATA(hyphword), THEDATA(hyphsize));
    Allocate ("hyphlist", THEDATA(hyphlist), THEDATA(hyphsize));
    Allocate ("hyphlink", THEDATA(hyphlink), THEDATA(hyphsize));

    Allocate ("bcharlabel",
	      THEDATA(bcharlabel),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("charbase",
	      THEDATA(charbase),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("depthbase",
	      THEDATA(depthbase),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("extenbase",
	      THEDATA(extenbase),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontarea",
	      THEDATA(fontarea),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontbc",
	      THEDATA(fontbc),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontbchar",
	      THEDATA(fontbchar),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontcheck",
	      THEDATA(fontcheck),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontdsize",
	      THEDATA(fontdsize),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontec",
	      THEDATA(fontec),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontfalsebchar",
	      THEDATA(fontfalsebchar),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontglue",
	      THEDATA(fontglue),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontname",
	      THEDATA(fontname),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontparams",
	      THEDATA(fontparams),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontsize",
	      THEDATA(fontsize),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("fontused",
	      THEDATA(fontused),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("heightbase",
	      THEDATA(heightbase),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("hyphenchar",
	      THEDATA(hyphenchar),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("italicbase",
	      THEDATA(italicbase),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("kernbase",
	      THEDATA(kernbase),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("ligkernbase",
	      THEDATA(ligkernbase),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("parambase",
	      THEDATA(parambase),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("skewchar",
	      THEDATA(skewchar),
	      THEDATA(fontmax) - constfontbase);
    Allocate ("triec",
	      THEDATA(triec),
	      THEDATA(triesize));
    Allocate ("widthbase",
	      THEDATA(widthbase),
	      THEDATA(fontmax) - constfontbase);

    if (IsInitProgram())
    {
      // memory allocated in tex-miktex-hash.ch
      THEDATA(yhash) = 0;
      THEDATA(zeqtb) = 0;
    }

    if (IsInitProgram() || ! AmITeXCompiler() || AmI("omega"))
      {
	Allocate ("fontinfo",
		  THEDATA(fontinfo),
		  THEDATA(fontmemsize));
      }
#endif // not Omega

#if defined(MIKTEX_OMEGA)
    GETPARAM (param_trie_op_size,
	      trieopsize,
	      trie_op_size,
	      omega::omega::trie_op_size());

    Allocate ("hyfdistance", THEDATA(hyfdistance), THEDATA(trieopsize));
    Allocate ("hyfnext", THEDATA(hyfnext), THEDATA(trieopsize));
    Allocate ("hyfnum", THEDATA(hyfnum), THEDATA(trieopsize));
    Allocate ("trie", THEDATA(trie), THEDATA(triesize));
    Allocate ("trieophash", THEDATA(trieophash), 2 * THEDATA(trieopsize));
    Allocate ("trieoplang", THEDATA(trieoplang), THEDATA(trieopsize));
    Allocate ("trieopval", THEDATA(trieopval), THEDATA(trieopsize));
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

    Free ("linestack", THEDATA(linestack));
    Free ("inputstack", THEDATA(inputstack));
    Free ("inputfile", THEDATA(inputfile));
    Free ("fullsourcefilenamestack", THEDATA(fullsourcefilenamestack));
    Free ("sourcefilenamestack", THEDATA(sourcefilenamestack));
    Free ("nest", THEDATA(nest));
    Free ("savestack", THEDATA(savestack));
    Free ("triec", THEDATA(triec));
    Free ("triehash", THEDATA(triehash));
    Free ("triel", THEDATA(triel));
    Free ("trieo", THEDATA(trieo));
    Free ("trier", THEDATA(trier));
    Free ("trietaken", THEDATA(trietaken));

    Free ("nameoffile", THEDATA(nameoffile));

#if ! defined(MIKTEX_OMEGA)
    Free ("hyphword", THEDATA(hyphword));
    Free ("hyphlist", THEDATA(hyphlist));
    Free ("hyphlink", THEDATA(hyphlink));

    Free ("trietrl", THEDATA(trietrl));
    Free ("trietro", THEDATA(trietro));
    Free ("trietrc", THEDATA(trietrc));

    Free ("bcharlabel", THEDATA(bcharlabel));
    Free ("charbase", THEDATA(charbase));
    Free ("depthbase", THEDATA(depthbase));
    Free ("extenbase", THEDATA(extenbase));
    Free ("fontarea", THEDATA(fontarea));
    Free ("fontbc", THEDATA(fontbc));
    Free ("fontbchar", THEDATA(fontbchar));
    Free ("fontcheck", THEDATA(fontcheck));
    Free ("fontdsize", THEDATA(fontdsize));
    Free ("fontec", THEDATA(fontec));
    Free ("fontfalsebchar", THEDATA(fontfalsebchar));
    Free ("fontglue", THEDATA(fontglue));
    Free ("fontinfo", THEDATA(fontinfo));
    Free ("fontname", THEDATA(fontname));
    Free ("fontparams", THEDATA(fontparams));
    Free ("fontsize", THEDATA(fontsize));
    Free ("fontused", THEDATA(fontused));
    Free ("heightbase", THEDATA(heightbase));
    Free ("hyphenchar", THEDATA(hyphenchar));
    Free ("italicbase", THEDATA(italicbase));
    Free ("kernbase", THEDATA(kernbase));
    Free ("ligkernbase", THEDATA(ligkernbase));
    Free ("parambase", THEDATA(parambase));
    Free ("skewchar", THEDATA(skewchar));
    Free ("widthbase", THEDATA(widthbase));
#endif // not Omega

#if defined(MIKTEX_OMEGA)
    Free ("hyfdistance", THEDATA(hyfdistance));
    Free ("hyfnext", THEDATA(hyfnext));
    Free ("hyfnum", THEDATA(hyfnum));
    Free ("trie", THEDATA(trie));

    Free ("trieophash", THEDATA(trieophash));
    Free ("trieoplang", THEDATA(trieoplang));
    Free ("trieopval", THEDATA(trieopval));
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
     
     MLTeXP
     _______________________________________________________________________ */

public:

  int
  GetSynchronizationOptions ()
    const
  {
    return (synchronizationOptions);
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
     
     GetWrite18Mode
     _______________________________________________________________________ */

public:
    class Write18ModeEnum
    {
    public:
      enum EnumType {
	Enabled = 't',
	Disabled = 'f',
	PartiallyEnabled = 'p',
	Query = 'q',
      };
    };

  typedef MiKTeX::Core::EnumWrapper<Write18ModeEnum> Write18Mode;

public:

  Write18Mode
  GetWrite18Mode ()
    const
  {
    return (write18Mode);
  }

  /* _______________________________________________________________________
     
     Write18P
     _______________________________________________________________________ */

public:

  bool
  Write18P ()
    const
  {
    return (write18Mode == Write18Mode::Enabled
      || write18Mode == Write18Mode::PartiallyEnabled
      || write18Mode == Write18Mode::Query);
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

  class Write18ResultEnum
  {
  public:
    enum EnumType {
      QuotationError = -1,
      Disallowed = 0,
      Executed = 1,
      ExecutedAllowed = 2
    };
  };

  typedef MiKTeX::Core::EnumWrapper<Write18ResultEnum> Write18Result;

  MIKTEXMFTHISAPI(Write18Result)
  Write18 (/*[in]*/ const char *	lpszCommand,
	   /*[out]*/ int &		exitCode)
    const;

  /* _______________________________________________________________________
     
     Write18
     _______________________________________________________________________ */

public:

  MIKTEXMFTHISAPI(Write18Result)
  Write18 (/*[in]*/ const wchar_t *	lpszCommand,
	   /*[out]*/ int &		exitCode)
    const;

  /* _______________________________________________________________________
     
     Variables
     _______________________________________________________________________ */

private:
  Write18Mode write18Mode;

private:
  bool enableMLTeX;

private:
  bool enableEncTeX;
  
private:
  int synchronizationOptions;

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
  int param_hash_extra;

private:
  int optBase;
};

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// texapp.h
