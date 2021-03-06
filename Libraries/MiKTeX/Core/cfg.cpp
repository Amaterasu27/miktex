/* cfg.cpp: configuration files

   Copyright (C) 1996-2012 Christian Schenk

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

const char COMMENT_CHAR = ';';
const char * const COMMENT_CHAR_STR = ";";

const char * const EMSA_ = "EMSA1(SHA-256)";
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
  string documentation;

public:
  bool commentedOut;

public:
  string name;

public:
  string value;

private:
  enum { INITIAL_VALUE_SIZE = 256 };

public:
  CfgValue ()
    : commentedOut (false)
  {
    value.reserve (INITIAL_VALUE_SIZE);
  }

public:
  CfgValue (/*[in]*/ const CfgValue & other)
    : name (other.name),
      value (other.value),
      documentation (other.documentation),
      commentedOut (other.commentedOut)
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

#if defined(HAVE_UNORDERED_MAP)
typedef tr1::unordered_map<string,
			   CfgValue,
			   hash_icase,
			   equal_icase> ValueMap;
#else
typedef map<string,
	    CfgValue,
	    less_icase> ValueMap;
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

static const char * const knownSearchPathValues[] = {
  "path",
  "extensions",
  MIKTEX_REGVAL_COMMON_ROOTS,
  MIKTEX_REGVAL_USER_ROOTS,
};

bool
IsSearchPathValue (/*[in]*/ const char * lpszValueName)
{
  for (int idx = 0;
       idx != sizeof(knownSearchPathValues) / sizeof(knownSearchPathValues[0]);
       ++ idx)
    {
      if (StringCompare(lpszValueName, knownSearchPathValues[idx], true) == 0)
	{
	  return (true);
	}
    }
  return (false);
}

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
		  writer.WriteFormatted ("%c%c ", COMMENT_CHAR, COMMENT_CHAR);
		}
	      writer.Write (*it2);
	      start = (*it2 == '\n');
	    }
	  if (! start)
	    {
	      writer.WriteLine ();
	    }
	}
      if (IsSearchPathValue(it->second.name.c_str())
	  && (it->second.value.find_first_of(PATH_DELIMITER) != string::npos))
	{
	  writer.WriteFormattedLine ("%s%s=",
				     (it->second.commentedOut
				      ? COMMENT_CHAR_STR
				      : ""),
				     it->second.name.c_str());
	  for (CSVList root (it->second.value.c_str(), PATH_DELIMITER);
	       root.GetCurrent() != 0;
	       ++ root)
	    {
	      writer.WriteFormattedLine ("%s%s;=%s",
					 (it->second.commentedOut
					  ? COMMENT_CHAR_STR
					  : ""),
					 it->second.name.c_str(),
					 root.GetCurrent());
	    }
	}
      else
	{
	  writer.WriteFormattedLine ("%s%s=%s",
				     (it->second.commentedOut
				      ? COMMENT_CHAR_STR
				      : ""),
				     it->second.name.c_str(),
				     it->second.value.c_str());
	}
    }
}

/* _________________________________________________________________________

   CfgImpl
   _________________________________________________________________________ */

#if defined(HAVE_UNORDERED_MAP)
typedef unordered_map<string,
		      CfgKey,
		      hash_icase,
		      equal_icase> KeyMap;
#else
typedef map<string,
	    CfgKey,
	    less_icase> KeyMap;
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
  Read (/*[in]*/ const PathName &	path)
  {
    Read (path, false);
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  Write (/*[in]*/ const PathName &	path)
  {
    Write (path, T_("DO NOT EDIT THIS FILE!"));
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  Write (/*[in]*/ const PathName & 	path,
	 /*[in]*/ const char *		lpszHeader)
  {
    Write (path, lpszHeader, 0);
  }

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

public:
  virtual
  void
  MIKTEXTHISCALL
  DeleteValue (/*[in]*/ const char *	lpszKey,
	       /*[in]*/ const char * lpszValueName);

public:
  virtual
  void
  MIKTEXTHISCALL
  Read (/*[in]*/ const PathName &	path,
	/*[in]*/ bool			mustBeSigned);

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsSigned ()
  {
    return (signature.size() > 0);
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  Write (/*[in]*/ const PathName & 	path,
	 /*[in]*/ const char *		lpszHeader,
	 /*[in]*/ IPrivateKeyProvider * pPrivateKeyProvider);

private:
  void
  Read (/*[in]*/ const PathName &	path,
	/*[in]*/ const char *		lpszDefaultKey,
	/*[in]*/ int			level,
	/*[in]*/ bool			mustBeSigned);

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
  void
  Walk (/*[in,out]*/ Botan::Pipe & pipe);

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
  Botan::SecureVector<Botan::byte> signature;

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

   CfgImpl::Walk
   _________________________________________________________________________ */

void
CfgImpl::Walk (/*[in,out]*/ Botan::Pipe & pipe)
{
  vector<CfgKey> keys;
  keys.reserve (keyMap.size());
  for (KeyMap::const_iterator it = keyMap.begin(); it != keyMap.end(); ++ it)
  {
    keys.push_back (it->second);
  }
  sort (keys.begin(), keys.end());
  for (vector<CfgKey>::const_iterator it = keys.begin(); it != keys.end(); ++ it)
  {
    char szKeyName[BufferSizes::MaxCfgName];
    MakeLower (szKeyName, BufferSizes::MaxCfgName, it->name.c_str());
    pipe.write ("[");
    pipe.write (szKeyName);
    pipe.write ("]\n");
    vector<CfgValue> values;
    values.reserve (it->valueMap.size());
    for (ValueMap::const_iterator it2 = it->valueMap.begin(); it2 != it->valueMap.end(); ++ it2)
    {
      values.push_back (it2->second);
    }
    sort (values.begin(), values.end());
    for (vector<CfgValue>::const_iterator it2 = values.begin(); it2 != values.end(); ++ it2)
    {
      char szValueName[BufferSizes::MaxCfgName];
      MakeLower (szValueName, BufferSizes::MaxCfgName, it2->name.c_str());
      pipe.write (szValueName);
      pipe.write ("=");
      pipe.write (it2->value);
      pipe.write ("\n");
    }
  }
}

/* _________________________________________________________________________

   CfgImpl::GetDigest
   _________________________________________________________________________ */

MD5
CfgImpl::GetDigest ()
{
  Botan::Pipe pipe (new Botan::Hash_Filter("MD5"));
  pipe.start_msg ();
  Walk (pipe);
  pipe.end_msg ();
  Botan::SecureVector<Botan::byte> md5 = pipe.read_all(0);
  MiKTeX::Core::MD5 result;
  MIKTEX_ASSERT (sizeof(result) == 16);
  MIKTEX_ASSERT (md5.size() == sizeof(result));
  memcpy (result.GetBuffer(), static_cast<const Botan::byte *>(md5), md5.size());
  return (result);
}

/* _________________________________________________________________________

   CfgImpl::TryGetOriginalDigest

   Obsolete.
   _________________________________________________________________________ */

bool
CfgImpl::TryGetOriginalDigest (/*[out]*/ MD5 & originalDigest)
{
  UNIMPLEMENTED ("CfgImpl::TryGetOriginalDigest");
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

  if (bufSize == 0)
    {
      MIKTEX_ASSERT (lpszValue == 0);
      bufSize = pcfgvalue->value.length() + 1;
      return (true);
    }

  MIKTEX_ASSERT_CHAR_BUFFER (lpszValue, bufSize);

  bufSize = pcfgvalue->value.length() + 1;

  Utils::CopyString (lpszValue,
		     bufSize,
		     pcfgvalue->value.c_str());

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

  pair<KeyMap::iterator, bool> pair1 = keyMap.insert(make_pair(cfgkey.name, cfgkey));

  KeyMap::iterator itstrkey = pair1.first;
  MIKTEX_ASSERT (itstrkey != keyMap.end());

  CfgValue cfgvalue;
  cfgvalue.name = lpszValueName;
  cfgvalue.value = lpszValue;
  cfgvalue.documentation = (lpszDocumentation == 0 ? "" : lpszDocumentation);
  cfgvalue.commentedOut = commentedOut;

  pair<ValueMap::iterator, bool> pair2 =
	  (itstrkey->second.valueMap.insert(make_pair(lpszValueName, cfgvalue)));

  if (! pair2.second)
    {
      // modify existing value
      ValueMap::iterator itstrval = pair2.first;
      if (lpszDocumentation != 0 && itstrval->second.documentation.empty())
	{
	  itstrval->second.documentation = lpszDocumentation;
	}
      itstrval->second.commentedOut = commentedOut;
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
CfgImpl::Read (/*[in]*/ const PathName &	path,
	       /*[in]*/ bool			mustBeSigned)
{
  char szDefaultKey[BufferSizes::MaxCfgName];

  GetBaseNameSansExt (path.Get(), szDefaultKey, BufferSizes::MaxCfgName);

  Read (path, szDefaultKey, 0, mustBeSigned);

  this->path = path;
}

/* _________________________________________________________________________

   CfgImpl::Read
   _________________________________________________________________________ */

void
CfgImpl::Read (/*[in]*/ const PathName &	path,
	       /*[in]*/ const char *		lpszDefaultKey,
	       /*[in]*/ int			level,
	       /*[in]*/ bool			mustBeSigned)
{
  MIKTEX_ASSERT (! (level > 0 && mustBeSigned));

  traceStream->WriteFormattedLine
    ("core",
     T_("parsing: %s..."),
     path.Get());

  if (mustBeSigned)
  {
    traceStream->WriteFormattedLine ("core", T_("signature required..."));
  }

  bool wasEmpty = Empty();

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
    else if (line[0] == '!')
    {
      documentation = "";
      Tokenizer tok (line.c_str() + 1, " \t");
      const char * lpsz = tok.GetCurrent();
      if (lpsz == 0)
      {
	FATAL_CFG_ERROR ("CfgImpl::Read", T_("invalid cfg directive"));
      }
      if (StringCompare(lpsz, "include") == 0)
      {
	++ tok;
	lpsz = tok.GetCurrent();
	if (lpsz == 0)
	{
	  FATAL_CFG_ERROR ("CfgImpl::Read", T_("missing file name argument"));
	}
	PathName path2 (path);
	path2.MakeAbsolute ();
	path2.RemoveFileSpec ();
	path2 += lpsz;
	Read (path2, szKeyName, level + 1, false);
      }
      else
      {
	FATAL_CFG_ERROR ("CfgImpl::Read", T_("unknown cfg directive"));
      }
    }
    else if (line[0] == '[')
    {
      documentation = "";
      Tokenizer tok (line.c_str() + 1, "]");
      const char * lpsz = tok.GetCurrent();
      if (lpsz == 0)
      {
	FATAL_CFG_ERROR ("CfgImpl::Read", T_("incomplete secion name"));
      }
      Utils::CopyString (szKeyName, BufferSizes::MaxCfgName, lpsz);
    }
    else if (line.length() >= 3 && line[0] == COMMENT_CHAR && line[1] == COMMENT_CHAR && line[2] == ' ')
    {
      if (! documentation.empty())
      {
	documentation += '\n';
      }
      documentation += &line[3];
    }
    else if ((line.length() >= 2 && line[0] == COMMENT_CHAR && (IsAlNum(line[1]) || line[1] == '.'))
      || IsAlNum(line[0]) || line[0] == '.')
    {
      string valueName;
      string value;
      PutMode putMode;
      if (! ParseValueDefinition((line[0] == COMMENT_CHAR
	? &line[1]
      : &line[0]),
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
	line[0] == COMMENT_CHAR);
    }
    else if (line.length() >= 4 && line[0] == COMMENT_CHAR && line[1] == COMMENT_CHAR && line[2] == COMMENT_CHAR && line[3] == COMMENT_CHAR)
    {
      documentation = "";
      Tokenizer tok (line.c_str() + 4, " \t");
      const char * lpsz = tok.GetCurrent();
      if (lpsz != 0)
      {
	if (StringCompare(lpsz, "signature/miktex:") == 0)
	{
	  ++ tok;
	  lpsz = tok.GetCurrent();
	  if (lpsz != 0 && wasEmpty && level == 0)
	  {
	    Botan::Pipe pipe(new Botan::Base64_Decoder(Botan::Decoder_Checking::FULL_CHECK));
	    pipe.process_msg (lpsz);
	    signature =  pipe.read_all();
	  }
	}
      }
    }
  }

  reader.Close ();

  if (mustBeSigned && signature.size() == 0)
  {
    FATAL_CFG_ERROR ("CfgImpl::Read", T_("the configuration file is not signed"));
  }

  if (wasEmpty && level == 0 && signature.size() > 0)
  {
    auto_ptr<Botan::Public_Key> pPublicKey (LoadPublicKey());
    Botan::RSA_PublicKey * pRsaKey = dynamic_cast<Botan::RSA_PublicKey*>(pPublicKey.get());
    if (pRsaKey == 0)
    {
      UNEXPECTED_CONDITION ("CfgImpl::Read");
    }
    Botan::Pipe pipe (new Botan::PK_Verifier_Filter(
      Botan::get_pk_verifier(*pRsaKey, EMSA_),
      signature));
    pipe.start_msg ();
    Walk (pipe);
    pipe.end_msg();
    Botan::byte ok;
    if (pipe.read_byte(ok) != 1)
    {
      UNEXPECTED_CONDITION ("CfgImpl::Read");
    }
    MIKTEX_ASSERT (ok == 1 || ok == 0);
    if (ok != 1)
    {
      FATAL_CFG_ERROR ("CfgImpl::Read", T_("the file has been tampered with"));
    }
  }
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
  MIKTEX_ASSERT (! line.empty() && (IsAlNum(line[0]) || line[0] == '.'));

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

class BotanUI : public Botan::User_Interface
{
public:
  BotanUI (/*[in]*/ IPrivateKeyProvider * pPrivateKeyProvider)
    : pPrivateKeyProvider (pPrivateKeyProvider)
  {
  }
public:
  virtual string get_passphrase (/*[in]*/ const string &  what,
				 /*[in]*/ const string &  source,
				 /*[out]*/ UI_Result &	  result)
					 const
  {
    string passphrase;
    result = (pPrivateKeyProvider->GetPassphrase(passphrase) ? UI_Result::OK : UI_Result::CANCEL_ACTION);
    return (passphrase);
  }
private:
  IPrivateKeyProvider * pPrivateKeyProvider;
};

void
CfgImpl::Write (/*[in]*/ const PathName &	path,
		/*[in]*/ const char *		lpszHeader,
		/*[in]*/ IPrivateKeyProvider *	pPrivateKeyProvider)
{
  time_t t = time(0);
  StreamWriter writer (path);
  if (lpszHeader != 0)
  {
    writer.WriteFormattedLine ("%c%c%c %s",
      COMMENT_CHAR, COMMENT_CHAR, COMMENT_CHAR,
      lpszHeader);
    writer.WriteLine ();
  }
  WriteKeys (writer);
  if (pPrivateKeyProvider != 0)
  {
    auto_ptr<Botan::PKCS8_PrivateKey> privateKey;
    Botan::AutoSeeded_RNG rng;
    BotanUI ui (pPrivateKeyProvider);
    auto_ptr<Botan::Pipe> pPipe;
    privateKey.reset (Botan::PKCS8::load_key(
      pPrivateKeyProvider->GetPrivateKeyFile().ToString(),
      rng,
      ui));
    Botan::RSA_PrivateKey * pRsaKey =
      dynamic_cast<Botan::RSA_PrivateKey *>(privateKey.get());
    if (pRsaKey == 0)
    {
      UNEXPECTED_CONDITION ("CfgImpl::Write");
    }
    pPipe.reset (new Botan::Pipe(
      new Botan::PK_Signer_Filter(new Botan::PK_Signer(*pRsaKey, Botan::get_emsa(EMSA_)), rng),
      new Botan::Base64_Encoder()));
    pPipe->start_msg ();
    Walk (*pPipe);
    pPipe->end_msg ();
    writer.WriteLine ();
    writer.WriteFormattedLine (
      T_("%c%c%c%c This configuration file is signed by a MiKTeX maintainer. The signature follows."),
      COMMENT_CHAR, COMMENT_CHAR, COMMENT_CHAR, COMMENT_CHAR);
    writer.WriteFormattedLine (
      T_("%c%c%c%c-----BEGIN MIKTEX SIGNATURE-----"),
      COMMENT_CHAR, COMMENT_CHAR, COMMENT_CHAR, COMMENT_CHAR);
    writer.WriteFormattedLine ("%c%c%c%c signature/miktex: %s",
      COMMENT_CHAR, COMMENT_CHAR, COMMENT_CHAR, COMMENT_CHAR,
      pPipe->read_all_as_string().c_str());
    writer.WriteFormattedLine (
      T_("%c%c%c%c-----END MIKTEX SIGNATURE-----"),
      COMMENT_CHAR, COMMENT_CHAR, COMMENT_CHAR, COMMENT_CHAR);
  }
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
  MIKTEX_ASSERT_STRING_OR_NIL (lpszKey);
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

   CfgImpl::DeleteValue (experimental)
   _________________________________________________________________________ */

void
CfgImpl::DeleteValue (/*[in]*/ const char *	lpszKey,
		      /*[in]*/ const char *	lpszValueName)
{
  MIKTEX_ASSERT_STRING (lpszKey);
  MIKTEX_ASSERT_STRING (lpszValueName);
  KeyMap::iterator it = keyMap.find(lpszKey);
  if (it == keyMap.end())
  {
    INVALID_ARGUMENT ("CfgImpl::DeleteValue", lpszKey);
  }
  ValueMap::iterator it2 = it->second.valueMap.find (lpszValueName);
  if (it2 == it->second.valueMap.end())
  {
    INVALID_ARGUMENT ("CfgImpl::DeleteValue", lpszValueName);
  }
  it->second.valueMap.erase (it2);
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
