/* TpmParser.h:							-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(GUARD_E1E6167DD8414A4DBF3537EBE0C1DC2E_)
#define GUARD_E1E6167DD8414A4DBF3537EBE0C1DC2E_

BEGIN_INTERNAL_NAMESPACE;

class TpmParser
{
public:
  TpmParser ();
  
public:
  ~TpmParser ();
  
public:
  void
  Parse (/*[in]*/ const MiKTeX::Core::PathName &	path);

public:
  void
  Parse (/*[in]*/ const MiKTeX::Core::PathName &	path,
	 /*[in]*/ const MIKTEXCHAR *	lpszTeXMFPrefix);
  
  // get the result
public:
  const PackageInfo &
  GetPackageInfo ()
    const
  {
    return (packageInfo);
  }
  
private:
  static
  void
  OnStartElement (/*[in]*/ void *		pv,
		  /*[in]*/ const XML_Char *	lpszName,
		  /*[in]*/ const XML_Char **	aAttr);

private:
  static
  void
  OnEndElement (/*[in]*/ void *			pv,
		/*[in]*/ const XML_Char *	lpszName);

private:
  static
  void
  OnCharacterData (/*[in]*/ void *		pv,
		   /*[in]*/ const XML_Char *	lpsz,
		   /*[in]*/ int			len);
  
private:
  void
  GetFiles (/*[in]*/ const XML_Char *			lpszFiles,
	    /*[out]*/ std::vector<std::string> &	files);
  
  // the result
private:
  PackageInfo packageInfo;
  
  // character buffer
private:
  MiKTeX::Core::CharBuffer<XML_Char, 8192> charBuffer;
  
  // element stack
private:
  std::stack<std::string> elementStack;
  
private:
  void * pParser;

private:
  std::string texMFPrefix;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> traceError;
};

END_INTERNAL_NAMESPACE;

#endif
