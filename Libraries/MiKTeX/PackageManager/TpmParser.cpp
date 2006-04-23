/* TpmParser.cpp:

   Copyright (C) 2001-2006 Christian Schenk

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

/* _________________________________________________________________________

   TpmParser::TpmParser
   _________________________________________________________________________ */

TpmParser::TpmParser ()
  : pCurrent (0),
    pParser (0),
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
TpmParser::GetFiles (/*[in]*/ const tstring &		str,
		     /*[out]*/ vector<tstring> &	files)
{
  for (Tokenizer tok (str.c_str(), T_(";\n\r \t"));
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

const MIKTEXCHAR *
GetAttributeValue (/*[in]*/ const MIKTEXCHAR **	aAttr,
		   /*[in]*/ const MIKTEXCHAR *	lpszKey)
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
			   /*[in]*/ const MIKTEXCHAR *		lpszName,
			   /*[in]*/ const MIKTEXCHAR **		aAttr)
{
  MIKTEX_ASSERT (pv != 0);
  TpmParser * This = reinterpret_cast<TpmParser*>(pv);
  try
    {
      if (StrCmp(lpszName, T_("TPM:Creator")) == 0)
	{
	  This->packageInfo.creator.reserve (15);
	  This->pCurrent = & This->packageInfo.creator;
	}
      else if (StrCmp(lpszName, T_("TPM:Description")) == 0)
	{
	  This->packageInfo.description.reserve (1024);
	  This->pCurrent = & This->packageInfo.description;
	}
      else if (StrCmp(lpszName, T_("TPM:Name")) == 0)
	{
	  This->packageInfo.displayName.reserve (15);
	  This->pCurrent = & This->packageInfo.displayName;
	}
      else if (StrCmp(lpszName, T_("TPM:Title")) == 0)
	{
	  
	  This->packageInfo.title.reserve (100);
	  This->pCurrent = & This->packageInfo.title;
	}
      else if (StrCmp(lpszName, T_("TPM:Version")) == 0)
	{
	  This->packageInfo.version.reserve (100);
	  This->pCurrent = & This->packageInfo.version;
	}
      else if (StrCmp(lpszName, T_("TPM:Package")) == 0
	       || StrCmp(lpszName, T_("TPM:Collection")) == 0)
	{
	  const MIKTEXCHAR * lpszPackageName;
	  if (! This->elementStack.empty()
	      && This->elementStack.top() == T_("TPM:Requires")
	      && aAttr != 0
	      && (lpszPackageName = GetAttributeValue(aAttr, T_("name"))) != 0)
	    {
	      vector<tstring>::const_iterator
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
      else if (StrCmp(lpszName, T_("TPM:RunFiles")) == 0
	       || StrCmp(lpszName, T_("TPM:DocFiles")) == 0
	       || StrCmp(lpszName, T_("TPM:SourceFiles")) == 0)
	{
	  This->tempString = T_("");
	  This->pCurrent = & This->tempString;
	  int size = 0;
	  const MIKTEXCHAR * lpszSize;
	  if (aAttr != 0
	      && (lpszSize = GetAttributeValue(aAttr, T_("size"))) != 0)
	    {
	      size = AToI(lpszSize);
	    }
	  if (StrCmp(lpszName, T_("TPM:RunFiles")) == 0)
	    {
	      This->tempString.reserve (20000);
	      This->packageInfo.sizeRunFiles = size;
	    }
	  else if (StrCmp(lpszName, T_("TPM:DocFiles")) == 0)
	    {
	      This->tempString.reserve (10000);
	      This->packageInfo.sizeDocFiles = size;
	    }
	  else
	    {
	      MIKTEX_ASSERT (StrCmp(lpszName, T_("TPM:SourceFiles")) == 0);
	      This->tempString.reserve (10000);
	      This->packageInfo.sizeSourceFiles = size;
	    }
	}
      else
	{
	  This->tempString = T_("");
	  This->tempString.reserve (1000);
	  This->pCurrent = & This->tempString;
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
			 /*[in]*/ const MIKTEXCHAR *	lpszName)
{
  MIKTEX_ASSERT (pv != 0);
  TpmParser * This = reinterpret_cast<TpmParser*>(pv);
  try
    {
      MIKTEX_ASSERT (! This->elementStack.empty());
      MIKTEX_ASSERT (This->elementStack.top() == lpszName);
      This->elementStack.pop ();
      if (StrCmp(lpszName, T_("TPM:MD5")) == 0)
	{
	  This->packageInfo.digest = MD5::Parse(This->pCurrent->c_str());
	}
      else if (StrCmp(lpszName, T_("TPM:TimePackaged")) == 0)
	{
	  This->packageInfo.timePackaged = AToI(This->pCurrent->c_str());
	}
      else if (StrCmp(lpszName, T_("TPM:RunFiles")) == 0)
	{
	  This->packageInfo.runFiles.reserve (1000);
	  This->GetFiles (*This->pCurrent, This->packageInfo.runFiles);
	}
      else if (StrCmp(lpszName, T_("TPM:DocFiles")) == 0)
	{
	  This->packageInfo.docFiles.reserve (100);
	  This->GetFiles (*This->pCurrent, This->packageInfo.docFiles);
	}
      else if (StrCmp(lpszName, T_("TPM:SourceFiles")) == 0)
	{
	  This->packageInfo.sourceFiles.reserve (100);
	  This->GetFiles (*This->pCurrent,
			  This->packageInfo.sourceFiles);
	}
      This->pCurrent = 0;
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
      if (This->pCurrent != 0)
	{
	  This->pCurrent->append (lpsz, len);
	}
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
  texMFPrefix = (lpszTeXMFPrefix == 0 ? T_("") : lpszTeXMFPrefix);

  packageInfo = PackageInfo ();

  pCurrent = 0;

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

  if (! packageInfo.title.empty()
      && packageInfo.title[packageInfo.title.length() - 1] == T_('.'))
    {
      packageInfo.title.erase (packageInfo.title.length() - 1, 1);
    }

  if (StringCompare(packageInfo.title.c_str(), T_("no caption"), true) == 0
      || (StringCompare(packageInfo.title.c_str(),
			T_("no description available"),
			true)
	  == 0))
    {
      packageInfo.title = T_("");
    }

  MIKTEXCHARINT lastChar = MIKTEXEOF;
  tstring description;
  for (tstring::const_iterator it = packageInfo.description.begin();
       it != packageInfo.description.end();
       lastChar = *(it++))
    {
      if (lastChar != MIKTEXEOF
	  && isspace(*it, locale())
	  && isspace(static_cast<MIKTEXCHAR>(lastChar), locale()))
	{
	  continue;
	}
      if (*it == T_('\r') || *it == T_('\n'))
	{
	  description += T_(' ');
	}
      else
	{
	  description += *it;
	}
    }
  packageInfo.description = description;
}
