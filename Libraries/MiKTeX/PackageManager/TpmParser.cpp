/* TpmParser.cpp:

   Copyright (C) 2001-2007 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#include "TpmParser.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

#if defined(XML_UNICODE)
#  define X_(x) L##x
#else
#  define X_(x) x
#endif

/* _________________________________________________________________________

   TpmParser::TpmParser
   _________________________________________________________________________ */

TpmParser::TpmParser ()
  : pParser (0),
    traceError (TraceStream::Open(MIKTEX_TRACE_ERROR))
{
}

/* _________________________________________________________________________

   TpmParser::~TpmParser
   _________________________________________________________________________ */

TpmParser::~TpmParser ()
{
  try
    {
      if (pParser != 0)
	{
	  XML_ParserFree (reinterpret_cast<XML_Parser>(pParser));
	  pParser = 0;
	}
      traceError->Close ();
      traceError.reset ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   TpmParser::GetFiles
   _________________________________________________________________________ */

void
TpmParser::GetFiles (/*[in]*/ const XML_Char *		lpszFiles,
		     /*[out]*/ vector<string> &		files)
{
  MIKTEX_ASSERT (Utils::IsAscii(lpszFiles));
  for (Tokenizer tok (lpszFiles, X_(";\n\r \t"));
       tok.GetCurrent() != 0;
       ++ tok)
    {
      PathName path (tok.GetCurrent());
#if defined(MIKTEX_UNIX)
      path.ToUnix ();
#endif
      if (texMFPrefix.length() == 0
	  || (PathName::Compare(texMFPrefix, path, texMFPrefix.length()) == 0))
	{
	  files.push_back (path.ToString());
	}
    }
}

/* _________________________________________________________________________

   GetAttributeValue
   _________________________________________________________________________ */

const XML_Char *
GetAttributeValue (/*[in]*/ const XML_Char **	aAttr,
		   /*[in]*/ const XML_Char *	lpszKey)
{
  for (size_t i = 0; aAttr[i] != 0; i += 2)
    {
      if (StrCmp(aAttr[i], lpszKey) == 0)
	{
	  return (aAttr[i + 1]);
	}
    }
  return (0);
}

/* _________________________________________________________________________

   TpmParser::OnStartElement
   _________________________________________________________________________ */

void
TpmParser::OnStartElement (/*[in]*/ void *			pv,
			   /*[in]*/ const XML_Char *		lpszName,
			   /*[in]*/ const XML_Char **		aAttr)
{
  MIKTEX_ASSERT (pv != 0);
  TpmParser * This = reinterpret_cast<TpmParser*>(pv);
  try
    {
      This->charBuffer.Clear ();
      if (StrCmp(lpszName, X_("TPM:Package")) == 0
	  || StrCmp(lpszName, X_("TPM:Collection")) == 0)
	{
	  const XML_Char * lpszPackageName;
	  if (! This->elementStack.empty()
	      && This->elementStack.top() == X_("TPM:Requires")
	      && aAttr != 0
	      && (lpszPackageName = GetAttributeValue(aAttr, X_("name"))) != 0)
	    {
	      MIKTEX_ASSERT (Utils::IsAscii(lpszPackageName));
	      vector<string>::const_iterator
		it = find(This->packageInfo.requiredPackages.begin(),
			  This->packageInfo.requiredPackages.end(),
			  lpszPackageName);
	      if (it == This->packageInfo.requiredPackages.end())
		{
		  This->packageInfo.requiredPackages.push_back
		    (lpszPackageName);
		}
	    }
	}
      else if (StrCmp(lpszName, X_("TPM:RunFiles")) == 0
	       || StrCmp(lpszName, X_("TPM:DocFiles")) == 0
	       || StrCmp(lpszName, X_("TPM:SourceFiles")) == 0)
	{
	  int size = 0;
	  const XML_Char * lpszSize;
	  if (aAttr != 0
	      && (lpszSize = GetAttributeValue(aAttr, X_("size"))) != 0)
	    {
	      MIKTEX_ASSERT (Utils::IsAscii(lpszSize));
	      size = AToI(lpszSize);
	    }
	  if (StrCmp(lpszName, X_("TPM:RunFiles")) == 0)
	    {
	      This->packageInfo.sizeRunFiles = size;
	    }
	  else if (StrCmp(lpszName, X_("TPM:DocFiles")) == 0)
	    {
	      This->packageInfo.sizeDocFiles = size;
	    }
	  else if (StrCmp(lpszName, X_("TPM:DocFiles")) == 0)
	    {
	      This->packageInfo.sizeSourceFiles = size;
	    }
	  else
	    {
	      UNEXPECTED_CONDITION (T_("TpmParser::OnStartElement"));
	    }
	}
      This->elementStack.push (lpszName);
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   TpmParser::OnEndElement
   _________________________________________________________________________ */

void
TpmParser::OnEndElement (/*[in]*/ void *		pv,
			 /*[in]*/ const XML_Char *	lpszName)
{
  MIKTEX_ASSERT (pv != 0);
  TpmParser * This = reinterpret_cast<TpmParser*>(pv);
  try
    {
      MIKTEX_ASSERT (! This->elementStack.empty());
      MIKTEX_ASSERT (This->elementStack.top() == lpszName);
      This->elementStack.pop ();
#if defined(MIKTEX_WINDOWS)
      This->charBuffer.Ansify ();
#endif
      if (StrCmp(lpszName, X_("TPM:Creator")) == 0)
	{
	  This->packageInfo.creator = This->charBuffer.Get();
	}
      else if (StrCmp(lpszName, X_("TPM:Description")) == 0)
	{
	  size_t len = This->charBuffer.GetLength();
	  This->packageInfo.description.reserve (len);
	  XML_Char lastChar = static_cast<XML_Char>(-1);
	  for (size_t idx = 0;
	       idx < len;
	       lastChar = This->charBuffer[idx++])
	    {
	      XML_Char ch = This->charBuffer[idx];
	      if (idx > 0
		  && isspace(ch, locale())
		  && isspace(lastChar, locale()))
		{
		  continue;
		}
	      if (ch == X_('\r') || ch == X_('\n'))
		{
		  This->packageInfo.description += X_(' ');
		}
	      else
		{
		  This->packageInfo.description += ch;
		}
	    }
	}
      else if (StrCmp(lpszName, X_("TPM:Name")) == 0)
	{
	  This->packageInfo.displayName = This->charBuffer.Get();
	}
      else if (StrCmp(lpszName, X_("TPM:Title")) == 0)
	{
	  size_t len = This->charBuffer.GetLength();
	  if (len > 0 && This->charBuffer[len - 1] == X_('.'))
	    {
	      This->charBuffer[len - 1] = 0;
	      -- len;
	    }
	  if ((StringCompare(This->charBuffer.Get(),
			     X_("no caption"),
			     true)
	       == 0)
	      || (StringCompare(This->charBuffer.Get(),
				X_("no description available"),
				true)
		  == 0))
	    {
	      This->charBuffer.Clear ();
	      len = 0;
	    }
	  This->packageInfo.title = This->charBuffer.Get();
	}
      else if (StrCmp(lpszName, X_("TPM:Version")) == 0)
	{
	  This->packageInfo.version = This->charBuffer.Get();
	}
      else if (StrCmp(lpszName, X_("TPM:MD5")) == 0)
	{
	  This->packageInfo.digest = MD5::Parse(This->charBuffer.Get());
	}
      else if (StrCmp(lpszName, X_("TPM:TimePackaged")) == 0)
	{
	  This->packageInfo.timePackaged = AToI(This->charBuffer.Get());
	}
      else if (StrCmp(lpszName, X_("TPM:RunFiles")) == 0)
	{
	  This->packageInfo.runFiles.reserve (1000);
	  This->GetFiles (This->charBuffer.Get(), This->packageInfo.runFiles);
	}
      else if (StrCmp(lpszName, X_("TPM:DocFiles")) == 0)
	{
	  This->packageInfo.docFiles.reserve (100);
	  This->GetFiles (This->charBuffer.Get(), This->packageInfo.docFiles);
	}
      else if (StrCmp(lpszName, X_("TPM:SourceFiles")) == 0)
	{
	  This->packageInfo.sourceFiles.reserve (100);
	  This->GetFiles (This->charBuffer.Get(),
			  This->packageInfo.sourceFiles);
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   TpmParser::OnCharacterData
   _________________________________________________________________________ */

void
TpmParser::OnCharacterData (/*[in]*/ void *		pv,
			    /*[in]*/ const XML_Char *	lpsz,
			    /*[in]*/ int		len)
{
  MIKTEX_ASSERT (pv != 0);
  TpmParser * This = reinterpret_cast<TpmParser*>(pv);
  try
    {
      This->charBuffer.Append (lpsz, len);
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   TpmParser::Parse
   _________________________________________________________________________ */

void
TpmParser::Parse (/*[in]*/ const PathName & path)
{
  Parse (path, TEXMF_PREFIX_DIRECTORY);
}

/* _________________________________________________________________________

   TpmParser::Parse
   _________________________________________________________________________ */

void
TpmParser::Parse (/*[in]*/ const PathName &	path,
		  /*[in]*/ const MIKTEXCHAR *	lpszTeXMFPrefix)
{
  texMFPrefix = (lpszTeXMFPrefix == 0 ? X_("") : lpszTeXMFPrefix);

  packageInfo = PackageInfo ();

  size_t bytesToRead = File::GetSize(path);

  FileStream stream (File::Open(path,
				FileMode::Open,
				FileAccess::Read,
				false));

  if (pParser == 0)
    {
      pParser = reinterpret_cast<void*>(XML_ParserCreate(0));
      if (pParser == 0)
	{
	  FATAL_MPM_ERROR (T_("TpmParser::Parse"),
			   T_("The TPM parser could not be created."),
			   path.Get());
	}
    }
  else
    {
      XML_ParserReset (reinterpret_cast<XML_Parser>(pParser), 0);
    }
 
  XML_Parser p = reinterpret_cast<XML_Parser>(pParser);
  
  XML_SetElementHandler (p, OnStartElement, OnEndElement);
  XML_SetCharacterDataHandler (p, OnCharacterData);
  XML_SetUserData (p, reinterpret_cast<void*>(this));
  
  char buf[1024 * 4];
  size_t n;
  while ((n = stream.Read(buf, sizeof(buf))) > 0)
    {
      bytesToRead -= n;
      XML_Status st =
	XML_Parse(p, buf, static_cast<int>(n), (bytesToRead == 0));
      if (st == XML_STATUS_ERROR)
	{
	  traceError->WriteFormattedLine
	    (T_("libmpm"),
	     T_("parse error in %s at line/column %d/%d:"),
	     Q_(path),
	     static_cast<int>(XML_GetCurrentLineNumber(p)),
	     static_cast<int>(XML_GetCurrentColumnNumber(p)));
	  traceError->WriteLine
	    (T_("libmpm"),
	     XML_ErrorString(XML_GetErrorCode(p)));
	  FATAL_MPM_ERROR (T_("TpmParser::Parse"),
			   T_("The TPM file could not be parsed."),
			   path.Get());
	}
    }

  stream.Close ();
}
