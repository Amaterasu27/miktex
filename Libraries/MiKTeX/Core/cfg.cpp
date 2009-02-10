/* cfg.cpp: configuration files

   Copyright (C) 1996-2009 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#define FATAL_CFG_ERROR(function, message)			\
  FATAL_MIKTEX_ERROR						\
    (function,							\
     T_("Damaged configuration file."),				\
     (string(Q_(currentFile)) + ":" + NUMTOSTR(lineno) + ": "	\
      + message + ".").c_str())

/* _________________________________________________________________________

   GetBaseNameSansExt
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
GetBaseNameSansExt (/*[in]*/ const char *	lpszFileName,
		    /*[out]*/ char *		lpszBaseName,
		    /*[in]*/ size_t		bufSize)
{
  PathName::Split (lpszFileName,
		   0, 0,
		   lpszBaseName, bufSize,
		   0, 0);
}

/* _________________________________________________________________________

   Trim
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(string &)
Trim (/*[in,out]*/ string & str)
{
  const char * whitespace = " \t\r\n";
  size_t pos = str.find_last_not_of(whitespace);
  if (pos != string::npos)
    {
      str.erase (pos + 1);
    }
  pos = str.find_first_not_of(whitespace);
  if (pos == string::npos)
    {
      str.erase ();
    }
  else
    {
      str.erase (0, pos);
    }
  return (str);
}

/* _________________________________________________________________________

   CfgValue
   _________________________________________________________________________ */

struct CfgValue
{
public:
  bool expanded;

public:
  string documentation;

public:
  bool commentedOut;

public:
  string expandedValue;

public:
  string name;

public:
  string value;

private:
  enum { INITIAL_VALUE_SIZE = 256 };

public:
  CfgValue ()
    : expanded (false),
      commentedOut (false)
  {
    value.reserve (INITIAL_VALUE_SIZE);
  }

public:
  CfgValue (/*[in]*/ const CfgValue & other)
    : expanded (other.expanded),
      expandedValue (other.expandedValue),
      name (other.name),
      value (other.value)
  {
  }

public:
  ~CfgValue ()
  {
  }
};

inline
bool
operator< (/*[in]*/ const CfgValue & lhs,
	   /*[in]*/ const CfgValue & rhs)
{
  return (StringCompare(lhs.name.c_str(),
			rhs.name.c_str(),
			true)
	  < 0);
}

/* _________________________________________________________________________

   CfgKey
   _________________________________________________________________________ */

#if defined(USE_HASH_MAP)
typedef hash_map<string,
		 CfgValue,
		 hash_compare_icase> ValueMap;
#else
typedef map<string,
	    CfgValue,
	    hash_compare_icase> ValueMap;
#endif

struct CfgKey
{
public:
  string name;

public:
  ValueMap valueMap;

public:
  CfgKey ()
  {
  }

public:
  CfgKey (/*[in]*/ const CfgKey & other)
    : name (other.name),
      valueMap (other.valueMap)
  {
  }

public:
  ~CfgKey ()
  {
  }

public:
  CfgValue *
  FindValue (/*[in]*/ const char * lpszValueName);

public:
  void
  WriteValues (/*[in]*/ StreamWriter &		writer);
};

inline
bool
operator< (/*[in]*/ const CfgKey & lhs,
	   /*[in]*/ const CfgKey & rhs)
{
  return (StringCompare(lhs.name.c_str(),
			rhs.name.c_str(),
			true)
	  < 0);
}

/* _________________________________________________________________________

   CfgKey::FindValue
   _________________________________________________________________________ */

CfgValue *
CfgKey::FindValue (/*[in]*/ const char * lpszValueName)
{
  ValueMap::iterator it = valueMap.find(lpszValueName);
  if (it == valueMap.end())
    {
      return (0);
    }
  return (&it->second);
}

/* _________________________________________________________________________

   CfgKey::WriteValues
   _________________________________________________________________________ */

void
CfgKey::WriteValues (/*[in]*/ StreamWriter &		writer)
{
  bool noKey = false;
  for (ValueMap::iterator it = valueMap.begin();
       it != valueMap.end();
       ++ it)
    {
      if (! noKey)
	{
	  writer.WriteLine ();
	  writer.WriteFormattedLine ("[%s]", name.c_str());
	  noKey = true;
	}
      if (! it->second.documentation.empty())
	{
	  writer.WriteLine ();
	  bool start = true;
	  for (string::const_iterator it2 = it->second.documentation.begin();
	       it2 != it->second.documentation.end();
	       ++ it2)
	    {
	      if (start)
		{
		  writer.Write (";; ");
		}
	      writer.Write (*it2);
	      start = (*it2 == '\n');
	    }
	  if (! start)
	    {
	      writer.WriteLine ();
	    }
	}
      writer.WriteFormattedLine ("%s%s=%s",
				 (it->second.commentedOut
				  ? ";"
				  : ""),
				 it->second.name.c_str(),
				 it->second.value.c_str());
    }
}

/* _________________________________________________________________________

   CfgImpl
   _________________________________________________________________________ */

#if defined(USE_HASH_MAP)
typedef hash_map<string,
		 CfgKey,
		 hash_compare_icase> KeyMap;
#else
typedef map<string,
	    CfgKey,
	    hash_compare_icase> KeyMap;
#endif

class CfgImpl : public Cfg
{
public:
  virtual
  void
  MIKTEXTHISCALL
  AddRef ();

public:
  virtual
  void
  MIKTEXTHISCALL
  Release ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  Empty ();

public:
  virtual
  MD5
  MIKTEXTHISCALL
  GetDigest ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetOriginalDigest (/*[out]*/ MD5 & originalDigest);

public:
  virtual
  string
  MIKTEXTHISCALL
  GetValue (/*[in]*/ const char *	lpszKey,
	    /*[in]*/ const char *	lpszValueName);

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetValue (/*[in]*/ const char *	lpszKey,
	       /*[in]*/ const char *	lpszValueName,
	       /*[out]*/ char *		lpszValue,
	       /*[in,out]*/ size_t &	bufSize);
  
public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetValue (/*[in]*/ const char *	lpszKey,
	       /*[in]*/ const char *	lpszValueName,
	       /*[out]*/ string &	value);

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetValue (/*[in]*/ const char *	lpszKey,
	       /*[in]*/ const char *	lpszValueName,
	       /*[out]*/ PathName &	path);

public:
  virtual
  void
  MIKTEXTHISCALL
  SetModified (/*[in]*/ bool b);

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsModified ();

public:
  virtual
  void
  MIKTEXTHISCALL
  PutValue (/*[in]*/ const char *	lpszKey,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const char *	lpszValue);
  
public:
  virtual
  void
  MIKTEXTHISCALL
  PutValue (/*[in]*/ const char *	lpszKey,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const char *	lpszValue,
	    /*[in]*/ const char *	lpszDocumentation,
	    /*[in]*/ bool		commentedOut);
  
public:
  virtual
  void
  MIKTEXTHISCALL
  Read (/*[in]*/ const PathName &	path);

public:
  virtual
  void
  MIKTEXTHISCALL
  Write (/*[in]*/ const PathName &	path);

public:
  virtual
  char *
  MIKTEXTHISCALL
  FirstKey (/*[out]*/ char *	lpsz,
	    /*[in]*/ size_t	bufSize);

public:
  virtual
  char *
  MIKTEXTHISCALL
  NextKey (/*[out]*/ char *	lpsz,
	   /*[in]*/ size_t	bufSize);

public:
  virtual
  void
  MIKTEXTHISCALL
  DeleteKey (/*[in]*/ const char * lpszKey);

public:
  virtual
  char *
  MIKTEXTHISCALL
  FirstValue (/*[in]*/ const char *	lpszKey,
	      /*[out]*/ char *		lpszValueName,
	      /*[in]*/ size_t		valueNameSize);

public:
  virtual
  char *
  MIKTEXTHISCALL
  NextValue (/*[out]*/ char *	lpszValueName,
	     /*[in]*/ size_t	valueNameSize);

private:
  void
  Read (/*[in]*/ const PathName &	path,
	/*[in]*/ const char *		lpszDefaultKey,
	/*[in]*/ int			level);

private:
  enum PutMode {
    None,
    Append,
    SearchPathAppend
  };

private:
  bool
  ParseValueDefinition (/*[in]*/ const string &	line,
			/*[out]*/ string &	valueName,
			/*[out]*/ string &	value,
			/*[out]*/ PutMode &	putMode);

private:
  PathName path;
  
private:
  KeyMap keyMap;

private:
  KeyMap::const_iterator iter;

private:
  ValueMap::const_iterator iter2;
  
private:
  ValueMap::const_iterator iter3;
  
private:
  bool tracking;
  
private:
  MD5 snapshotDigest;

private:
  MD5 originalDigest;

private:
  bool haveOriginalDigest;

private:
  auto_ptr<TraceStream> traceStream;
  
private:
  auto_ptr<TraceStream> traceError;
  
private:
  CfgImpl ();

private:
  CfgImpl (/*[in]*/ const CfgImpl &)
  {
    UNEXPECTED_CONDITION ("CfgImpl::CfgImpl");
  };
  
private:
  virtual
  ~CfgImpl ();

private:
  char *
  GetDefaultKey (/*[in]*/ char * lpszDefaultKey)
    const;

private:
  CfgKey *
  FindKey (/*[in]*/ const char * lpszKey);

private:
  const CfgKey *
  FindKey (/*[in]*/ const char * lpszKey)
    const;

private:
  void
  WriteKeys (/*[in]*/ StreamWriter & writer);

private:
  void
  Expand (/*[in]*/ const char *	lpszKeyName,
	  /*[in]*/ const char *	lpszIn,
	  /*[out]*/ string &	expansion);

private:
  void
  ExpandMacro (/*[in]*/ const char *	lpszKeyName,
	       /*[in]*/ const char *	lpszMacroName,
	       /*[out]*/ string &	expansion);

private:
  void
  PutValue (/*[in]*/ const char *	lpszKey,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const char *	lpszValue,
	    /*[in]*/ PutMode		putMode,
	    /*[in]*/ const char *	lpszDocumentation,
	    /*[in]*/ bool		commentedOut);

private:
  int lineno;

private:
  PathName currentFile;

private:
  int refCount;

private:
  friend class Cfg;
};

/* _________________________________________________________________________

   Cfg::~Cfg
   _________________________________________________________________________ */

Cfg::~Cfg ()
{
}

/* _________________________________________________________________________

   CfgImpl::CfgImpl
   _________________________________________________________________________ */

CfgImpl::CfgImpl ()
  : tracking (false),
    traceStream (TraceStream::Open(MIKTEX_TRACE_CONFIG)),
    traceError (TraceStream::Open(MIKTEX_TRACE_ERROR)),
    refCount (0)
{
}
  
/* _________________________________________________________________________

   CfgImpl::~CfgImpl
   _________________________________________________________________________ */

CfgImpl::~CfgImpl ()
{
  try
    {
      traceError->Close ();
      traceStream->Close ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   CfgImpl::AddRef
   _________________________________________________________________________ */

void
CfgImpl::AddRef ()
{
  ++ refCount;
}

/* _________________________________________________________________________

   CfgImpl::Release
   _________________________________________________________________________ */

void
CfgImpl::Release ()
{
  -- refCount;
  if (refCount == 0)
    {
      delete this;
    }
}

/* _________________________________________________________________________
   
   CfgImpl::GetDefaultKey
   _________________________________________________________________________ */

char *
CfgImpl::GetDefaultKey (/*[out]*/ char * lpszDefaultKey)
  const
{
  if (path.Empty())
    {
      return (0);
    }
  GetBaseNameSansExt (path.Get(), lpszDefaultKey, BufferSizes::MaxCfgName);
  return (lpszDefaultKey);
}

/* _________________________________________________________________________

   CfgImpl::FindKey
   _________________________________________________________________________ */

CfgKey *
CfgImpl::FindKey (/*[in]*/ const char * lpszKey)
{
  char szDefaultKey[BufferSizes::MaxCfgName];
  if (lpszKey == 0)
    {
      lpszKey = GetDefaultKey(szDefaultKey);
      if (lpszKey == 0)
	{
	  return (0);
	}
    }
  KeyMap::iterator it = keyMap.find(lpszKey);
  if (it == keyMap.end())
    {
      return (0);
    }
  return (&it->second);
}

/* _________________________________________________________________________

   CfgImpl::FindKey
   _________________________________________________________________________ */

const CfgKey *
CfgImpl::FindKey (/*[in]*/ const char * lpszKey)
  const
{
  char szDefaultKey[BufferSizes::MaxCfgName];
  if (lpszKey == 0)
    {
      lpszKey = GetDefaultKey(szDefaultKey);
      if (lpszKey == 0)
	{
	  return (0);
	}
    }
  KeyMap::const_iterator it = keyMap.find(lpszKey);
  if (it == keyMap.end())
    {
      return (0);
    }
  return (&it->second);
}

/* _________________________________________________________________________
   
   CfgImpl::WriteKeys
   _________________________________________________________________________ */

void
CfgImpl::WriteKeys (/*[in]*/ StreamWriter & writer)
{
  for (KeyMap::iterator it = keyMap.begin();
       it != keyMap.end();
       ++ it)
    {
      it->second.WriteValues (writer);
    }
  if (tracking)
    {
      snapshotDigest = GetDigest();
    }
}

/* _________________________________________________________________________

   CfgImpl::DeleteKey
   _________________________________________________________________________ */

void
CfgImpl::DeleteKey (/*[in]*/ const char * lpszKey)
{
  MIKTEX_ASSERT_STRING (lpszKey);
  KeyMap::iterator it = keyMap.find(lpszKey);
  if (it == keyMap.end())
    {
      INVALID_ARGUMENT ("CfgImpl::DeleteKey", lpszKey);
    }
  keyMap.erase (it);
}

/* _________________________________________________________________________

   CfgImpl::GetDigest
   _________________________________________________________________________ */

MD5
CfgImpl::GetDigest ()
{
  MD5Builder md5Builder;
  md5Builder.Init ();
  vector<CfgKey> keys;
  keys.reserve (keyMap.size());
  for (KeyMap::const_iterator it = keyMap.begin();
       it != keyMap.end();
       ++ it)
    {
      keys.push_back (it->second);
    }
  sort (keys.begin(), keys.end());
  for (vector<CfgKey>::const_iterator it = keys.begin();
       it != keys.end();
       ++ it)
    {
      char szKeyName[BufferSizes::MaxCfgName];
      MakeLower (szKeyName,
		 BufferSizes::MaxCfgName,
		 it->name.c_str());
      md5Builder.Update (szKeyName, strlen(szKeyName));
      vector<CfgValue> values;
      values.reserve (it->valueMap.size());
      for (ValueMap::const_iterator it2 =
	     it->valueMap.begin();
	   it2 != it->valueMap.end();
	   ++ it2)
	{
	  values.push_back (it2->second);
	}
      sort (values.begin(), values.end());
      for (vector<CfgValue>::const_iterator it2 = values.begin();
	   it2 != values.end();
	   ++ it2)
	{
	  char szValueName[BufferSizes::MaxCfgName];
	  MakeLower (szValueName,
		     BufferSizes::MaxCfgName,
		     it2->name.c_str());
	  md5Builder.Update (szValueName, strlen(szValueName));
	  md5Builder.Update (it2->value.c_str(), it2->value.length());
	}
    }
  return (md5Builder.Final());
}

/* _________________________________________________________________________

   CfgImpl::TryGetOriginalDigest
   _________________________________________________________________________ */

bool
CfgImpl::TryGetOriginalDigest (/*[out]*/ MD5 & originalDigest)
{
  if (haveOriginalDigest)
    {
      originalDigest = this->originalDigest;
    }
  return (haveOriginalDigest);
}

/* _________________________________________________________________________

   Cfg::Create
   _________________________________________________________________________ */

Cfg *
Cfg::Create ()
{
  return (new CfgImpl);
}

/* _________________________________________________________________________

   CfgImpl::ExpandMacro

   <fixme>This should be a callback function.</fixme>
   _________________________________________________________________________ */

void
CfgImpl::ExpandMacro (/*[in]*/ const char *	lpszKeyName,
		      /*[in]*/ const char *	lpszMacroName,
		      /*[out]*/ string &	expansion)
{
  UNUSED_ALWAYS (lpszKeyName);
  if (StringCompare(lpszMacroName, CFG_MACRO_NAME_BINDIR, true) == 0)
    {
      expansion =
	SessionImpl::GetSession()
	->GetSpecialPath(SpecialPath::BinDirectory).ToString();
    }
#if defined(MIKTEX_WINDOWS)
  else if (StringCompare(lpszMacroName, CFG_MACRO_NAME_WINDIR, true) == 0)
    {
      PathName path;
      if (GetWindowsDirectory(path.GetBuffer(),
			      static_cast<UINT>(path.GetCapacity()) == 0))
	{
	  FATAL_WINDOWS_ERROR ("GetWindowsDirectory", 0);
	}
      expansion = path.ToString();
    }
#endif
  else if (StringCompare(lpszMacroName,
			 CFG_MACRO_NAME_LOCALFONTDIRS, true)
	   == 0)
    {
      expansion = SessionImpl::GetSession()->GetLocalFontDirectories();
    }
  else if (StringCompare(lpszMacroName, CFG_MACRO_NAME_PSFONTDIRS, true) == 0)
    {
      string psFontDirs;
      if (SessionImpl::GetSession()->GetPsFontDirs(psFontDirs))
	{
	  expansion = psFontDirs;
	}
      else
	{
	  expansion = "";
	}
    }
  else if (StringCompare(lpszMacroName, CFG_MACRO_NAME_TTFDIRS, true) == 0)
    {
      string ttfDirs;
      if (SessionImpl::GetSession()->GetTTFDirs(ttfDirs))
	{
	  expansion = ttfDirs;
	}
      else
	{
	  expansion = "";
	}
    }
  else if (StringCompare(lpszMacroName, CFG_MACRO_NAME_OTFDIRS, true) == 0)
    {
      string otfDirs;
      if (SessionImpl::GetSession()->GetOTFDirs(otfDirs))
	{
	  expansion = otfDirs;
	}
      else
	{
	  expansion = "";
	}
    }
  else
    {
      FATAL_CFG_ERROR ("CfgImpl::ExpandMacro", T_("unknown macro name"));
    }
}

/* _________________________________________________________________________

   CfgImpl::Expand
   _________________________________________________________________________ */

void
CfgImpl::Expand (/*[in]*/ const char *	lpszKeyName,
		 /*[in]*/ const char *	lpszIn,
		 /*[out]*/ string &	expansion)
{
  const char * lpsz = lpszIn;
  string macroName;
  expansion.reserve (strlen(lpsz));
  for (; *lpsz != 0; ++ lpsz)
    {
      if (lpsz[0] == '$')
	{
	  if (lpsz[1] == '$')
	    {
	      lpsz += 2;
	      expansion += '$';
	    }
	  else if (lpsz[1] == '(')
	    {
	      macroName = "";
	      for (lpsz += 2; *lpsz != 0 && *lpsz != ')'; ++ lpsz)
		{
		  macroName += *lpsz;
		}
	      if (*lpsz != ')')
		{
		  FATAL_CFG_ERROR ("CfgImpl::Expand", T_("missing `)'"));
		}
	      if (macroName.empty())
		{
		  FATAL_CFG_ERROR ("CfgImpl::Expand",
				   T_("missing macro name"));
		}
	      string expandedMacro;
	      ExpandMacro (lpszKeyName,
			   macroName.c_str(),
			   expandedMacro);
	      expansion += expandedMacro;
	    }
	  else
	    {
	      FATAL_CFG_ERROR ("CfgImpl::Expand",
			       T_("invalid control sequence"));
	    }
	}
      else
	{
	  expansion += *lpsz;
	}
    }
}

/* _________________________________________________________________________

   CfgImpl::GetValue
   _________________________________________________________________________ */

string
CfgImpl::GetValue (/*[in]*/ const char *	lpszKey,
		   /*[in]*/ const char *	lpszValueName)
{
  size_t bufSize = 0;

  if (! TryGetValue(lpszKey,
		    lpszValueName,
		    0,
		    bufSize))
    {
      FATAL_MIKTEX_ERROR ("CfgImpl::GetValue",
			  T_("The configuration value does not exist."),
			  lpszValueName);
    }

  CharBuffer<char> buffer (bufSize);

  if (! TryGetValue(lpszKey,
		    lpszValueName,
		    buffer.GetBuffer(),
		    bufSize))
    {
      UNEXPECTED_CONDITION ("CfgImpl::GetValue");
    }

  return (buffer.Get());
}

/* _________________________________________________________________________

   CfgImpl::TryGetValue
   _________________________________________________________________________ */

bool
CfgImpl::TryGetValue (/*[in]*/ const char *	lpszKey,
		      /*[in]*/ const char *	lpszValueName,
		      /*[out]*/ char *		lpszValue,
		      /*[in,out]*/ size_t &	bufSize)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszKey);
  MIKTEX_ASSERT_STRING (lpszValueName);

  CfgKey * pcfgkey = FindKey(lpszKey);

  if (pcfgkey == 0)
    {
      return (false);
    }

  CfgValue * pcfgvalue = pcfgkey->FindValue(lpszValueName);

  if (pcfgvalue == 0 || pcfgvalue->commentedOut)
    {
      return (false);
    }

  if (! pcfgvalue->expanded)
    {
      Expand (lpszKey,
	      pcfgvalue->value.c_str(),
	      pcfgvalue->expandedValue);
      pcfgvalue->expanded = true;
    }
  
  if (bufSize == 0)
    {
      MIKTEX_ASSERT (lpszValue == 0);
      bufSize = pcfgvalue->expandedValue.length() + 1;
      return (true);
    }

  MIKTEX_ASSERT_CHAR_BUFFER (lpszValue, bufSize);

  bufSize = pcfgvalue->expandedValue.length() + 1;

  Utils::CopyString (lpszValue,
		     bufSize,
		     pcfgvalue->expandedValue.c_str());

  return (true);
}

/* _________________________________________________________________________

   CfgImpl::TryGetValue
   _________________________________________________________________________ */

bool
CfgImpl::TryGetValue (/*[in]*/ const char *	lpszKey,
		      /*[in]*/ const char *	lpszValueName,
		      /*[out]*/ string &	value)
{
  size_t bufSize = 0;

  if (! TryGetValue(lpszKey,
		    lpszValueName,
		    0,
		    bufSize))
    {
      return (false);
    }

  CharBuffer<char> buffer (bufSize);

  if (! TryGetValue(lpszKey,
		    lpszValueName,
		    buffer.GetBuffer(),
		    bufSize))
    {
      UNEXPECTED_CONDITION ("CfgImpl::TryGetValue");
    }

  value = buffer.Get();

  return (true);
}

/* _________________________________________________________________________

   CfgImpl::TryGetValue
   _________________________________________________________________________ */

bool
CfgImpl::TryGetValue (/*[in]*/ const char *	lpszKey,
		      /*[in]*/ const char *	lpszValueName,
		      /*[out]*/ PathName &	path)
{
  size_t size = path.GetCapacity();
  return (TryGetValue(lpszKey, lpszValueName, path.GetBuffer(), size));
}

/* _________________________________________________________________________

   CfgImpl::PutValue
   _________________________________________________________________________ */

void
CfgImpl::PutValue (/*[in]*/ const char *	lpszKey,
		   /*[in]*/ const char *	lpszValueName,
		   /*[in]*/ const char *	lpszValue,
		   /*[in]*/ CfgImpl::PutMode	putMode,
		   /*[in]*/ const char *	lpszDocumentation,
		   /*[in]*/ bool		commentedOut)
{
  char szDefaultKey[BufferSizes::MaxCfgName];

  if (lpszKey == 0)
    {
      lpszKey = GetDefaultKey(szDefaultKey);
      if (lpszKey == 0)
	{
	  FATAL_CFG_ERROR ("CfgImpl::PutValue",
			   T_("missing section definition"));
	}
    }

  CfgKey cfgkey;
  cfgkey.name = lpszKey;

  pair<KeyMap::iterator, bool> pair1
    = keyMap.insert(make_pair<string, CfgKey>(lpszKey, cfgkey));

  KeyMap::iterator itstrkey = pair1.first;
  MIKTEX_ASSERT (itstrkey != keyMap.end());

  CfgValue cfgvalue;
  cfgvalue.name = lpszValueName;
  cfgvalue.value = lpszValue;
  cfgvalue.documentation = (lpszDocumentation == 0 ? "" : lpszDocumentation);
  cfgvalue.commentedOut = commentedOut;

  pair<ValueMap::iterator, bool> pair2
    = (itstrkey->second.valueMap
       .insert(make_pair<string, CfgValue>(lpszValueName, cfgvalue)));

  if (! pair2.second)
    {
      ValueMap::iterator itstrval = pair2.first;
      itstrval->second.expanded = false;
      itstrval->second.expandedValue = "";
      if (putMode == Append)
	{
	  itstrval->second.value += lpszValue;
	}
      else if (putMode == SearchPathAppend)
	{
	  if (! itstrval->second.value.empty())
	    {
	      itstrval->second.value += PATH_DELIMITER;
	    }
	  itstrval->second.value += lpszValue;
	}
      else
	{
	  itstrval->second.value = lpszValue;
	}
    }
}

/* _________________________________________________________________________

   CfgImpl::PutValue
   _________________________________________________________________________ */

void
CfgImpl::PutValue (/*[in]*/ const char *	lpszKey,
		   /*[in]*/ const char *	lpszValueName,
		   /*[in]*/ const char *	lpszValue)
{
  return (PutValue(lpszKey, lpszValueName, lpszValue, None, 0, false));
}

/* _________________________________________________________________________

   CfgImpl::PutValue
   _________________________________________________________________________ */

void
CfgImpl::PutValue (/*[in]*/ const char *	lpszKey,
		   /*[in]*/ const char *	lpszValueName,
		   /*[in]*/ const char *	lpszValue,
		   /*[in]*/ const char *	lpszDocumentation,
		   /*[in]*/ bool		commentedOut)
{
  return (PutValue(lpszKey,
		   lpszValueName,
		   lpszValue,
		   None,
		   lpszDocumentation,
		   commentedOut));
}

/* _________________________________________________________________________

   CfgImpl::Read
   _________________________________________________________________________ */

void
CfgImpl::Read (/*[in]*/ const PathName &	path)
{
  char szDefaultKey[BufferSizes::MaxCfgName];

  GetBaseNameSansExt (path.Get(), szDefaultKey, BufferSizes::MaxCfgName);

  Read (path, szDefaultKey, 0);

  this->path = path;
}

/* _________________________________________________________________________

   CfgImpl::Read
   _________________________________________________________________________ */

void
CfgImpl::Read (/*[in]*/ const PathName &	path,
	       /*[in]*/ const char *		lpszDefaultKey,
	       /*[in]*/ int			level)
{
  UNUSED_ALWAYS (level);

  traceStream->WriteFormattedLine
    ("core",
     T_("parsing: %s..."),
     path.Get());

  bool wasEmpty = Empty();

  if (level == 0)
    {
      haveOriginalDigest = false;
    }

  StreamReader reader (path);

  string line;
  line.reserve (128);

  AutoRestore<int> autoRestore1 (lineno);
  AutoRestore<PathName> autoRestore (currentFile);

  char szKeyName[BufferSizes::MaxCfgName];
  Utils::CopyString (szKeyName, BufferSizes::MaxCfgName, lpszDefaultKey);

  lineno = 0;
  currentFile = path;

  string documentation;

  while (reader.ReadLine(line))
    {
      ++ lineno;
      if (line.empty())
	{
	  documentation = "";
	}
      else if (line[0] == '@')
	{
	  documentation = "";
	  if (wasEmpty && level == 0)
	    {
	      originalDigest = MD5::Parse(line.c_str() + 1);
	      haveOriginalDigest = true;
	    }
	}
      else if (line[0] == '!')
	{
	  documentation = "";
	  Tokenizer tok (line.c_str() + 1, " \t");
	  const char * lpsz = tok.GetCurrent();
	  if (lpsz == 0)
	    {
	      FATAL_CFG_ERROR ("CfgImpl::Read",
			       T_("invalid cfg directive"));
	    }
	  if (StringCompare(lpsz, "include") == 0)
	    {
	      ++ tok;
	      lpsz = tok.GetCurrent();
	      if (lpsz == 0)
		{
		  FATAL_CFG_ERROR ("CfgImpl::Read",
				   T_("missing file name argument"));
		}
	      PathName path2 (path);
	      path2.MakeAbsolute ();
	      path2.RemoveFileSpec ();
	      path2 += lpsz;
	      Read (path2, szKeyName, level + 1);
	    }
	  else
	    {
	      FATAL_CFG_ERROR ("CfgImpl::Read",
			       T_("unknown cfg directive"));
	    }
	}
      else if (line[0] == '[')
	{
	  documentation = "";
	  Tokenizer tok (line.c_str() + 1, "]");
	  const char * lpsz = tok.GetCurrent();
	  if (lpsz == 0)
	    {
	      FATAL_CFG_ERROR ("CfgImpl::Read",
			       T_("incomplete secion name"));
	    }
	  Utils::CopyString (szKeyName, BufferSizes::MaxCfgName, lpsz);
	}
      else if (line[0] == ';' && line[1] == ';' && line[2] == ' ')
	{
	  if (! documentation.empty())
	    {
	      documentation += '\n';
	    }
	  documentation += &line[3];
	}
      else if ((line[0] == ';' && (IsAlNum(line[1]) || line[1] == '.'))
	       || IsAlNum(line[0]) || line[0] == '.')
	{
	  string valueName;
	  string value;
	  PutMode putMode;
	  if (! ParseValueDefinition(line[0] == ';' ? &line[1] : &line[0],
				     valueName,
				     value,
				     putMode))
	    {
	      FATAL_CFG_ERROR ("CfgImpl::Read",
			       T_("invalid value definition"));
	    }
	  PutValue (szKeyName,
		    valueName.c_str(),
		    value.c_str(),
		    putMode,
		    documentation.c_str(),
		    line[0] == ';');
	}
    }

  reader.Close ();
}

/* _________________________________________________________________________

   CfgImpl::ParseValueDefinition

   Parse a value definition, i.e.

   valuename=value
   _________________________________________________________________________ */

bool
CfgImpl::ParseValueDefinition (/*[in]*/ const string &		line,
			       /*[out]*/ string &		valueName,
			       /*[out]*/ string &		value,
			       /*[out]*/ CfgImpl::PutMode &	putMode)
{
  MIKTEX_ASSERT (IsAlNum(line[0]) || line[0] == '.');

  size_t posEqual = line.find('=');

  putMode = None;

  if (posEqual == string::npos)
    {
      return (false);
    }

  value = line.substr(posEqual + 1);
  Trim (value);

  if (line[posEqual - 1] == '+')
    {
      putMode = Append;
      posEqual -= 1;
    }
  else if (line[posEqual - 1] == ';')
    {
      putMode = SearchPathAppend;
      posEqual -= 1;
    }

  valueName = line.substr(0, posEqual);
  Trim (valueName);

  return (true);
}

/* _________________________________________________________________________

   CfgImpl::Write
   _________________________________________________________________________ */

void
CfgImpl::Write (/*[in]*/ const PathName &	path)
{
  time_t t = time(0);
  StreamWriter writer (path);
  writer.WriteLine (T_(";;; DO NOT EDIT THIS FILE!"));
  writer.WriteLine ();
  WriteKeys (writer);
#if 0
  // this breaks Config::IniFiles
  writer.WriteLine ();
  writer.WriteFormattedLine ("@%s", GetDigest().ToString().c_str());
#endif
  writer.Close ();
  File::SetTimes (path, t, t, t);
}

/* _________________________________________________________________________

   CfgImpl::FirstKey
   _________________________________________________________________________ */

char *
CfgImpl::FirstKey (/*[out]*/ char *	lpsz,
		   /*[in]*/ size_t	bufSize)
{
  MIKTEX_ASSERT_CHAR_BUFFER (lpsz, bufSize);

  iter = keyMap.begin();

  if (iter == keyMap.end())
    {
      return (0);
    }

  Utils::CopyString (lpsz, bufSize, iter->first.c_str());

  return (lpsz);
}

/* _________________________________________________________________________

   CfgImpl::NextKey
   _________________________________________________________________________ */

char *
CfgImpl::NextKey (/*[out]*/ char *	lpsz,
		  /*[in]*/ size_t	bufSize)
{
  MIKTEX_ASSERT_CHAR_BUFFER (lpsz, bufSize);

  if (iter == keyMap.end())
    {
      INVALID_ARGUMENT ("CfgImpl::NextKey", 0);
    }

  ++ iter;

  if (iter == keyMap.end())
    {
      return (0);
    }

  Utils::CopyString (lpsz, bufSize, iter->first.c_str());

  return (lpsz);
}

/* _________________________________________________________________________

   CfgImpl::FirstValue
   _________________________________________________________________________ */

char *
CfgImpl::FirstValue (/*[in]*/ const char *	lpszKey,
		     /*[out]*/ char *		lpszValueName,
		     /*[in]*/ size_t		valueNameSize)
{
  MIKTEX_ASSERT_STRING (lpszKey);
  MIKTEX_ASSERT_CHAR_BUFFER (lpszValueName, valueNameSize);

  CfgKey * pCfgKey = FindKey(lpszKey);

  if (pCfgKey == 0)
    {
      UNEXPECTED_CONDITION ("CfgImpl::FirstValue");
    }

  iter2 = pCfgKey->valueMap.begin();
  iter3 = pCfgKey->valueMap.end();

  if (iter2 == iter3)
    {
      return (0);
    }

  Utils::CopyString (lpszValueName, valueNameSize, iter2->first.c_str());

  return (lpszValueName);
}

/* _________________________________________________________________________

   CfgImpl::NextValue
   _________________________________________________________________________ */

char *
CfgImpl::NextValue (/*[out]*/ char *	lpszValueName,
		    /*[in]*/ size_t	valueNameSize)
{
  MIKTEX_ASSERT_CHAR_BUFFER (lpszValueName, valueNameSize);

  if (iter2 == iter3)
    {
      INVALID_ARGUMENT ("CfgImpl::NextValue", 0);
    }

  ++ iter2;

  if (iter2 == iter3)
    {
      return (0);
    }

  Utils::CopyString (lpszValueName, valueNameSize, iter2->first.c_str());

  return (lpszValueName);
}

/* _________________________________________________________________________

   CfgImpl::SetModified
   _________________________________________________________________________ */

void
CfgImpl::SetModified (/*[in]*/ bool b)
{
  tracking = true;
  snapshotDigest = GetDigest();
  if (b)
    {
      (snapshotDigest.GetBuffer())[0] = ~ (snapshotDigest.GetBuffer())[0];
    }
}

/* _________________________________________________________________________

   CfgImpl::IsModified
   _________________________________________________________________________ */

bool
CfgImpl::IsModified ()
{
  MIKTEX_ASSERT (tracking);
  MD5 digest = GetDigest();
  return (! (digest == snapshotDigest));
}
  
/* _________________________________________________________________________

   CfgImpl::Empty
   _________________________________________________________________________ */

bool
CfgImpl::Empty ()
{
  return (keyMap.size() == 0);
}
