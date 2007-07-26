/* texapp.cpp:

   Copyright (C) 1996-2007 Christian Schenk
 
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

#define EXPERT_SRC_SPECIALS 0

/* _________________________________________________________________________

   TeXApp::TeXApp
   _________________________________________________________________________ */

TeXApp::TeXApp ()
{
}

/* _________________________________________________________________________

   TeXApp::Init
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
TeXApp::Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
{
  TeXMFApp::Init (lpszProgramInvocationName);

  SetTeX ();

  enableWrite18 = false;
  enableMLTeX = false;
  lastLineNum = -1;
  param_font_max = -1;
  param_font_mem_size = -1;
  param_hyph_size = -1;
  param_max_in_open = -1;
  param_mem_bot = -1;
  param_nest_size = -1;
  param_save_size = -1;
  param_trie_op_size = -1;
  param_trie_size = -1;
}

/* _________________________________________________________________________

   TeXApp::OnTeXMFStartJob
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
TeXApp::OnTeXMFStartJob ()
{
  TeXMFApp::OnTeXMFStartJob ();
  enableWrite18 =
    pSession->GetConfigValue(0,
			     MIKTEX_REGVAL_ENABLE_WRITE18,
			     false);
}

/* _________________________________________________________________________

   TeXApp::Finalize
   _________________________________________________________________________ */

MIKTEXMFAPI(void)
TeXApp::Finalize ()
{
  lastSourceFilename = T_("");
  sourceSpecials.reset ();
  TeXMFApp::Finalize ();
}

/* _________________________________________________________________________

   TeXApp::AddOptions
   _________________________________________________________________________ */

enum {
  OPT_DISABLE_WRITE18,
  OPT_ENABLE_WRITE18,
  OPT_FONT_MAX,
  OPT_FONT_MEM_SIZE,
  OPT_MAX_IN_OPEN,
  OPT_MEM_BOT,
  OPT_MLTEX,
  OPT_NEST_SIZE,
  OPT_SAVE_SIZE,
  OPT_SRC_SPECIALS,
  OPT_TRIE_OP_SIZE,
  OPT_TRIE_SIZE,
};

MIKTEXMFAPI(void)
TeXApp::AddOptions ()
{
  TeXMFApp::AddOptions ();

  optBase = static_cast<int>(GetOptions().size());

  AddOption (T_("disable-write18\0\
Disable the \\write18{COMMAND} construct."),
	     FIRST_OPTION_VAL + optBase + OPT_DISABLE_WRITE18);

  AddOption (T_("enable-write18\0\
Enable the \\write18{COMMAND} construct."),
	     FIRST_OPTION_VAL + optBase + OPT_ENABLE_WRITE18);

  AddOption (T_("max-in-open\0\
Set max_in_open to N."),
	     FIRST_OPTION_VAL + optBase + OPT_MAX_IN_OPEN,
	     required_argument,
	     T_("N"));

  AddOption (T_("mem-bot\0\
Set mem_bot to N."),
	     FIRST_OPTION_VAL + optBase + OPT_MEM_BOT,
	     required_argument,
	     T_("N"));

  AddOption (T_("mltex\0\
Enable MLTeX extensions such as \\charsubdef."),
	     FIRST_OPTION_VAL + optBase + OPT_MLTEX);

  AddOption (T_("nest-size\0\
Set nest_size to N."),
	     FIRST_OPTION_VAL + optBase + OPT_NEST_SIZE,
	     required_argument,
	     T_("N"));

  AddOption (T_("save-size\0\
Set save_size to N."),
	     FIRST_OPTION_VAL + optBase + OPT_SAVE_SIZE,
	     required_argument, T_("N"));

  AddOption (T_("trie-size\0\
Set trie_size to N."),
	     FIRST_OPTION_VAL + optBase + OPT_TRIE_SIZE,
	     required_argument,
	     T_("N"));

  if (StringCompare(GetProgramName(), T_("omega"), true) != 0)
    {
      AddOption (T_("font-max\0\
Set font_max to N."),
		 FIRST_OPTION_VAL + optBase + OPT_FONT_MAX,
		 required_argument,
		 T_("N"));
      AddOption (T_("font-mem-size\0\
Set font_mem_size to N."),
		 FIRST_OPTION_VAL + optBase + OPT_FONT_MEM_SIZE,
		 required_argument,
		 T_("N"));
    }
  else
    {
      AddOption (T_("trie-op-size\0\
Set trie_op_size to N."),
		 FIRST_OPTION_VAL + optBase + OPT_TRIE_OP_SIZE,
		 required_argument,
		 T_("N"));
    }

#if EXPERT_SRC_SPECIALS
  AddOption ((T_("src-specials\0\
Insert source specials in certain places of the DVI file.\
  WHERE is a comma-separated value list of:\
 cr display hbox math par parend vbox.")),
	     FIRST_OPTION_VAL + optBase + OPT_SRC_SPECIALS,
	     optional_argument,
	     T_("WHERE"));
#else
  AddOption ((T_("src-specials\0\
Insert source specials in certain places of the DVI file.")),
	     FIRST_OPTION_VAL + optBase + OPT_SRC_SPECIALS);
#endif

  // obsolete options
  AddOption (T_("try-gz\0"), OPT_UNSUPPORTED);

  // old option names
  AddOption (T_("src"), T_("src-specials"));

  // supported Web2C options
  AddOption (T_("fmt"), T_("undump"));
  AddOption (T_("no-shell-escape"), T_("disable-write18"));
  AddOption (T_("shell-escape"), T_("enable-write18"));

  // unsupported Web2C options
  AddOption (T_("enc"), OPT_UNSUPPORTED);
  AddOption (T_("ipc"), OPT_UNSUPPORTED);
  AddOption (T_("ipc-start"), OPT_UNSUPPORTED);
  AddOption (T_("output-comment"),
	     OPT_UNSUPPORTED,
	     required_argument,
	     T_("comment"));
}

/* _________________________________________________________________________
     
   TeXApp::ProcessOption
   _________________________________________________________________________ */

MIKTEXMFAPI(bool)
TeXApp::ProcessOption (/*[in]*/ int			optchar,
		       /*[in]*/ const MIKTEXCHAR *	lpszArg)
{
  bool done = true;
  switch (optchar - FIRST_OPTION_VAL - optBase)
    {
    case OPT_DISABLE_WRITE18:
      enableWrite18 = false;
      break;
    case OPT_ENABLE_WRITE18:
      enableWrite18 = true;
      break;
    case OPT_FONT_MAX:
      param_font_max = _ttoi(lpszArg);
      break;
    case OPT_FONT_MEM_SIZE:
      param_font_mem_size = _ttoi(lpszArg);
      break;
    case OPT_MAX_IN_OPEN:
      param_max_in_open = _ttoi(lpszArg);
      break;
    case OPT_MEM_BOT:
      param_mem_bot = _ttoi(lpszArg);
      break;
    case OPT_MLTEX:
      enableMLTeX = true;
      break;
    case OPT_NEST_SIZE:
      param_nest_size = _ttoi(lpszArg);
      break;
    case OPT_SAVE_SIZE:
      param_save_size = _ttoi(lpszArg);
      break;
    case OPT_SRC_SPECIALS:
#if EXPERT_SRC_SPECIALS
      sourceSpecials[SourceSpecials::Auto] = true;
      sourceSpecials[SourceSpecials::Paragraph] = true;
#else
      if (lpszArg == 0)
	{
	  sourceSpecials[SourceSpecials::Auto] = true;
	  sourceSpecials[SourceSpecials::Paragraph] = true;
	}
      else
	{
	  Tokenizer tok (lpszArg, T_(", "));
	  while (tok.GetCurrent() != 0)
	    {
	      if (StringCompare(tok.GetCurrent(), T_("everypar")) == 0
		  || StringCompare(tok.GetCurrent(), T_("par")) == 0)
		{
		  sourceSpecials[SourceSpecials::Auto] = true;
		  sourceSpecials[SourceSpecials::Paragraph] = true;
		}
	      else if (StringCompare(tok.GetCurrent(), T_("everyparend")) == 0
		       || StringCompare(tok.GetCurrent(), T_("parend")) == 0)
		{
		  sourceSpecials[SourceSpecials::ParagraphEnd] = true;
		}
	      else if (StringCompare(tok.GetCurrent(), T_("everycr")) == 0
		       || StringCompare(tok.GetCurrent(), T_("cr")) == 0)
		{
		  sourceSpecials[SourceSpecials::CarriageReturn] = true;
		}
	      else if (StringCompare(tok.GetCurrent(), T_("everymath")) == 0
		       || StringCompare(tok.GetCurrent(), T_("math")) == 0)
		{
		  sourceSpecials[SourceSpecials::Math] = true;
		}
	      else if (StringCompare(tok.GetCurrent(), T_("everyhbox")) == 0
		       || StringCompare(tok.GetCurrent(), T_("hbox")) == 0)
		{
		  sourceSpecials[SourceSpecials::HorizontalBox] = true;
		}
	      else if (StringCompare(tok.GetCurrent(), T_("everyvbox")) == 0
		       || StringCompare(tok.GetCurrent(), T_("vbox")) == 0)
		{
		  sourceSpecials[SourceSpecials::VerticalBox] = true;
		}
	      else if (StringCompare(tok.GetCurrent(), T_("everydisplay")) == 0
		       || StringCompare(tok.GetCurrent(), T_("display")) == 0)
		{
		  sourceSpecials[SourceSpecials::Display] = true;
		}
	      else
		{
		  FATAL_MIKTEX_ERROR (T_(""),
				      T_("Unknown source special."),
				      tok.GetCurrent());
		}
	      ++ tok;
	    }
	}
#endif // EXPERT_SRC_SPECIALS
      break;
    case OPT_TRIE_SIZE:
      param_trie_size = _ttoi(lpszArg);
      break;
    case OPT_TRIE_OP_SIZE:
      param_trie_op_size = _ttoi(lpszArg);
      break;
    default:
      done = TeXMFApp::ProcessOption(optchar, lpszArg);
      break;
    }
  return (done);
}

/* _________________________________________________________________________

   TeXApp::Write18
   _________________________________________________________________________ */

MIKTEXMFAPI(bool)
TeXApp::Write18 (/*[in]*/ const MIKTEXCHAR *	lpszCommand,
		 /*[out]*/ int &		exitCode)
  const
{
  MIKTEX_ASSERT_STRING (lpszCommand);
  if (! enableWrite18)
    {
      return (false);
    }
  Process::ExecuteSystemCommand (lpszCommand, &exitCode);
  return (true);
}
