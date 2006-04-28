/* core.h: MiKTeX core API					-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

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

/**
 * @mainpage MiKTeX API
 *
 * @section intro Introduction
 *
 * This is the documentation of the MiKTeX API.
 */

/**
 * @file core.h
 * @brief The MiKTeX core API.
 */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(A089FEF06254514BA063DED44B70E66F_)
#define A089FEF06254514BA063DED44B70E66F_

#if ! defined(MIKTEX)
#  define MIKTEX 1
#endif

#if defined(_WIN64)
#  define MIKTEX_WINDOWS 1
#  define MIKTEX_WINDOWS_64 1
#elif defined(_WIN32)
#  define MIKTEX_WINDOWS 1
#  define MIKTEX_WINDOWS_32 1
#else
#  define MIKTEX_UNIX 1
#endif

#if ! defined(EAD86981C92C904D808A5E6CEC64B90E)
#  if defined(MIKTEX_DLL) && defined(_MSC_VER)
#    define MIKTEXEXPORT __declspec(dllimport)
#  else
#    define MIKTEXEXPORT
#  endif
#endif

#if defined(_MSC_VER)
#  define MIKTEXCALL __stdcall
#  define MIKTEXCALLBACK __stdcall
#  define MIKTEXDEPRECATED __declspec(deprecated)
#  define MIKTEXINLINE __forceinline
#  define MIKTEXNOVTABLE __declspec(novtable)
#  define MIKTEXNORETURN __declspec(noreturn)
#else
#  define MIKTEXCALL
#  define MIKTEXCALLBACK
#  define MIKTEXDEPRECATED
#  define MIKTEXINLINE inline
#  define MIKTEXNOVTABLE
#  define MIKTEXNORETURN
#endif

#define MIKTEXAPI(type) MIKTEXEXPORT type MIKTEXCALL

#if defined(MIKTEX_WINDOWS)
#  include <windows.h>
#endif

#include <miktex/char.h>
#include <miktex/md5.h>

#if defined(__cplusplus)
#  include <cstdio>
#  include <cstdarg>
#  include <cstdlib>
#  include <ctime>
#  include <string>
#  include <stdexcept>
#  include <vector>
#else
#  include <stdio.h>
#  include <stdlib.h>
#  include <time.h>
#endif

#if defined(HAVE_PTHREAD)
#  include <pthread.h>
#endif

#if defined(_MSC_VER) && ! defined(MIKTEX_DLL)
#  pragma comment (lib, "htmlhelp")
#  pragma comment (lib, "shfolder.lib")
#  pragma comment (lib, "shell32")
#endif

#if defined(__cplusplus)
#  define MIKTEX_BEGIN_EXTERN_C_BLOCK extern "C" {
#  define MIKTEX_END_EXTERN_C_BLOCK }
#else
#  define MIKTEX_BEGIN_EXTERN_C_BLOCK
#  define MIKTEX_END_EXTERN_C_BLOCK
#endif

#define MIKTEX_CORE_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace Core {

#define MIKTEX_CORE_END_NAMESPACE		\
    }						\
  }

MIKTEX_BEGIN_EXTERN_C_BLOCK;

/* _________________________________________________________________________

   C API

   The C API is deprecated and undocumented.  It's sole purpose is to
   support utilities which cannot be compiled with a C++ compiler.
   _________________________________________________________________________ */

MIKTEXAPI(void)
miktex_create_temp_file_name (/*[out]*/ MIKTEXCHAR *	lpszFileName);

MIKTEXAPI(int)
miktex_determine_metafont_mode (/*[in]*/ unsigned	dpi,
				/*[out]*/ MIKTEXCHAR *	lpszMnemonic,
				/*[in]*/ size_t		bufSize);

MIKTEXAPI(int)
miktex_find_afm_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath);

MIKTEXAPI(int)
miktex_find_input_file (/*[in]*/ const MIKTEXCHAR *	lpszApplicationNames,
			/*[in]*/ const MIKTEXCHAR *	lpszFileName,
			/*[out]*/ MIKTEXCHAR *		lpszPath);

MIKTEXAPI(int)
miktex_find_enc_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath);

MIKTEXAPI(int)
miktex_find_file (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		  /*[in]*/ const MIKTEXCHAR *	lpszPathList,
		  /*[out]*/ MIKTEXCHAR *	lpszPath);

MIKTEXAPI(int)
miktex_find_hbf_file (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath);

MIKTEXAPI(int)
miktex_find_miktex_executable (/*[in]*/ const MIKTEXCHAR *	lpszExeName,
			       /*[out]*/ MIKTEXCHAR *		lpszExePath);

MIKTEXAPI(int)
miktex_find_pk_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		     /*[in]*/ const MIKTEXCHAR *	lpszMode,
		     /*[in]*/ int			dpi,
		     /*[out]*/ MIKTEXCHAR *		lpszPath);

MIKTEXAPI(int)
miktex_find_psheader_file (/*[in]*/ const MIKTEXCHAR *	lpszHeaderName,
			   /*[in]*/ MIKTEXCHAR *	lpszPath);

MIKTEXAPI(int)
miktex_find_tfm_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath);

MIKTEXAPI(int)
miktex_find_ttf_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		      /*[out]*/ MIKTEXCHAR *		lpszPath);

MIKTEXAPI(int)
miktex_find_type1_font_file (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
			     /*[out]*/ MIKTEXCHAR *		lpszPath);

MIKTEXAPI(MIKTEXCHAR *)
miktex_get_config_value (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
			 /*[in]*/ const MIKTEXCHAR *	lpszValueName,
			 /*[out]*/ MIKTEXCHAR *		lpszBuf,
			 /*[in]*/ size_t		bufSize,
			 /*[in]*/ const MIKTEXCHAR *	lpszDefaultValue);

MIKTEXAPI(unsigned)
miktex_get_number_of_texmf_roots ();

MIKTEXAPI(void)
miktex_get_gs_exe (/*[out]*/ MIKTEXCHAR * lpszPath);

MIKTEXAPI(int)
miktex_get_miktex_version_string_ex (/*[out]*/ MIKTEXCHAR *	lpszVersion,
				     /*[in]*/ size_t	maxsize);

MIKTEXAPI(MIKTEXCHAR *)
miktex_get_root_directory (/*[in]*/  unsigned		r,
			   /*[out]*/ MIKTEXCHAR *	lpszPath);

MIKTEXAPI(int)
miktex_pathcmp (/*[in]*/ const MIKTEXCHAR *	lpszPath1,
		/*[in]*/ const MIKTEXCHAR *	lpszPath2);

MIKTEXAPI(void)
miktex_remember_temp_file (/*[in]*/ const MIKTEXCHAR *	lpszFileName);

MIKTEXAPI(void)
miktex_replace_string (/*[out]*/ MIKTEXCHAR *		lpszBuf,
		       /*[in,out]*/ size_t *		lpSzeDest,
		       /*[in]*/ const MIKTEXCHAR *	lpszSource,
		       /*[in]*/ const MIKTEXCHAR *	lpszString1,
		       /*[in]*/ const MIKTEXCHAR *	lpszString2);

MIKTEXAPI(void)
miktex_start_process (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		      /*[in]*/ const MIKTEXCHAR *	lpszArguments,
		      /*[in]*/ FILE *			pFileStandardInput,
		      /*[out]*/ FILE **			ppFileStandardInput,
		      /*[out]*/ FILE **			ppFileStandardOutput,
		      /*[out]*/ FILE **			ppFileStandardError,
		      /*[in]*/ const MIKTEXCHAR *	lpszWorkingDirectory);

#if defined(MIKTEX_DLL)
MIKTEXAPI(void)
miktex_uncompress_file (/*[in]*/ const MIKTEXCHAR *	lpszPathIn,
			/*[out]*/ MIKTEXCHAR *		lpszPathOut);
#endif

#if defined(MIKTEX_2_4_COMPAT)
#  define miktex_find_app_input_file(progname, name, result) \
  miktex_find_input_file(progname, name, result)
#  define miktex_initialize()
#  define miktex_uninitialize()
#  define miktex_create_temp_file_2(tex, lpszFileName) \
  miktex_create_temp_file_name(lpszFileName)
#  define miktex_get_acrobat_font_dir(lpszPath) 0
#endif

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK;
#endif

/* C API ends here */

/* _________________________________________________________________________ */

#if defined(__cplusplus)

#include <cassert>

/// @namespace MiKTeX::Core
/// @brief The MiKTeX core namespace.
MIKTEX_CORE_BEGIN_NAMESPACE;

class PathName;

/* _________________________________________________________________________

   tstring
   _________________________________________________________________________ */

/// A generic string type derived from @c basic_string.
typedef std::basic_string<MIKTEXCHAR> tstring;

/* _________________________________________________________________________
   
   FormatInfo
   _________________________________________________________________________ */

/// Information about a TeX format.
struct FormatInfo
{
  /// Name of the format.
  tstring name;
  /// One-line description.
  tstring description;
  /// The compiler (engine) which processes the format.
  tstring compiler;
  /// The name of the input file.
  tstring inputFile;
  /// The name of the output file.
  tstring outputFile;
  /// The name of another format which has to be pre-loaded.
  tstring preloaded;
  /// Exclusion flag. If set, the format will be ignored by initexmf.
  bool exclude;
  /// Custom flag. Set, if this format was defined by the user.
  bool custom;

  FormatInfo ()
    : exclude (false),
      custom (true)
  {
  }
};

/* _________________________________________________________________________

   FontMapEntry
   _________________________________________________________________________ */

/// Information about an entry in a font map file.
struct FontMapEntry
{
  /// Name of the TeX font.
  tstring texName;
  /// Name of the PostScript font.
  tstring psName;
  /// Special instructions to be embedded into the PostScript document.
  tstring specialInstructions;
  /// Name of the encoding vector file.
  tstring encFile;
  /// Name of the font file.
  tstring fontFile;
  /// List of header files.
  tstring headerList;
};

inline
bool
operator< (/*[in]*/ const FontMapEntry & lhs,
	   /*[in]*/ const FontMapEntry & rhs)
{
  return (lhs.texName < rhs.texName);
}

/* _________________________________________________________________________
   
   MIKTEXMFMODE
   _________________________________________________________________________ */

struct MIKTEXMFMODE
{
  MIKTEXCHAR szMnemonic[32];
  MIKTEXCHAR szDescription[128];
  int iHorzRes;
  int iVertRes;
};

/* _________________________________________________________________________

   EnumWrapper
   _________________________________________________________________________ */

/// A template class which encapsulates enums.
/// The template class derives from @c Enum,
/// which must provide the public enum definition.
/// This following example will demonstrate the basic usage:
/// @verbatim
/// class ColorEnum {
/// public:
///   enum EnumType {
///     Red,
///     Green,
///     Blue
///   };
/// };
/// 
/// typedef EnumWrapper<ColorEnum> Colors;
/// 
/// int main ()
/// {
///   Color color = Color::Red;
/// };
/// @endverbatim
template<class Enum>
class EnumWrapper : public Enum
{
  /// The encapsulated enum type.
public:
  typedef typename Enum::EnumType EnumType;

private:
  EnumType value;

  /// Initializes a new instance of the class.
  /// @param value The enum value.
public:
  EnumWrapper (/*[in]*/ EnumType value)
    : value (value)
  {
  }

  /// Gets the encapsulated enum value.
public:
  EnumType
  Get ()
    const
  {
    return (value);
  }

  /// Compares the encapsulated enum value to another value.
  /// @param value The other enum value.
  /// @return Returns true, if both values compare equal.
public:
  bool
  operator== (/*[in]*/ EnumType value)
    const
  {
    return (this->value == value);
  }

  /// Compares the ancapsulated enum value to another value.
  /// @param value The other enum value.
  /// @return Returns true, if this values is less than the other.
public:
  bool
  operator< (/*[in]*/ const EnumWrapper & other)
    const
  {
    return (this->value < other.value);
  }

  /// Compares the encapsulated enum value to another value.
  /// @param value The other enum value.
  /// @return Returns true, if both values compare unequal.
public:
  bool
  operator!= (/*[in]*/ EnumType value)
    const
  {
    return (this->value != value);
  }

public:
  EnumType
  operator| (/*[in]*/ EnumType value)
    const
  {
    return (static_cast<EnumType>(this->value | value));
  }

  /// Obtains the bitwise AND between the
  /// encapsulated enum value and another enum value.
  /// @param value The other enum value.
  /// @return Returns the bitwise AND.
public:
  EnumType
  operator& (/*[in]*/ EnumType value)
    const
  {
    return (static_cast<EnumType>(this->value & value));
  }

  /// Sets the encapsulated enum value equal to the bitwise OR of
  /// this enum value and another enum value.
  /// @param value The other enum value.
  /// @return Returns a reference to this enum value.
public:
  EnumWrapper &
  operator|= (/*[in]*/ EnumType value)
  {
    this->value = static_cast<EnumType>(this->value | value);
    return (*this);
  }

  /// Sets the encapsulated enum value equal to the bitwise AND of
  /// this enum value and another enum value.
  /// @param value The other enum value.
  /// @return Returns a reference to this enum value.
public:
  EnumWrapper &
  operator&= (/*[in]*/ EnumType value)
  {
    this->value = static_cast<EnumType>(this->value & value);
    return (*this);
  }

public:
  EnumType
  operator~ ()
    const
  {
    return (static_cast<EnumType>(~ value));
  }
};

/* _________________________________________________________________________

   MiKTeXReleaseState
   _________________________________________________________________________ */

/// Release states enum class.
class MiKTeXReleaseStateEnum
{
public:
  enum EnumType {
    /// Officially released.
    Released = 0,
    /// Release candidate.
    ReleaseCandidate = 1,
    /// Field test release.
    Beta = 2,
    /// Snapshot (CVS) release.
    Snapshot = 4
  };
};

/// MiKTeX release states enum.
typedef EnumWrapper<MiKTeXReleaseStateEnum> MiKTeXReleaseState;

/* _________________________________________________________________________

   VersionNumber
   _________________________________________________________________________ */

/// An instance of this struct represents a version number.
struct VersionNumber
{
  /// Initializes an instance of the struct.
  VersionNumber ()
    : n1 (0),
      n2 (0),
      n3 (0),
      n4 (0)
  {
  }

  /// Initializes an instance of the struct.
  /// @param n1 The first part of the version number.
  /// @param n2 The second part of the version number.
  /// @param n3 The third part of the version number.
  /// @param n4 The fourth part of the version number.
  VersionNumber (/*[in]*/ unsigned n1,
		 /*[in]*/ unsigned n2,
		 /*[in]*/ unsigned n3,
		 /*[in]*/ unsigned n4)
    : n1 (n1),
      n2 (n2),
      n3 (n3),
      n4 (n4)
  {
  }

  VersionNumber (/*[in]*/ long	highWord,
		 /*[in]*/ long	lowWord)
    : n1 ((static_cast<unsigned long>(highWord) >> 16) & 0xffff),
      n2 (static_cast<unsigned long>(highWord) & 0xffff),
      n3 ((static_cast<unsigned long>(lowWord) >> 16) & 0xffff),
      n4 (static_cast<unsigned long>(lowWord) & 0xffff)
  {
  }

  /// Initializes an instance of the struct.
  /// @param lpszVersion The version number as a string.
  VersionNumber (/*[in]*/ const MIKTEXCHAR * lpszVersion)
  {
    *this = Parse(lpszVersion);
  }

  /// The first part of the version number. Usually the major version.
  unsigned n1;

  /// The second part of the version number. Usually the minor version.
  unsigned n2;

  /// The third part of the version number. Usually the j2000 number.
  unsigned n3;

  /// The fourth part of the version nimber. Usually 0.
  unsigned n4;

  /// Combines the first two parts of the version number.
  /// @return Returns a 32-bit integer.
  long
  GetHighWord ()
    const
  {
    unsigned long w = (n2 & 0xffff);
    w |= static_cast<unsigned long>(n1 & 0xffff) << 16;
    return (static_cast<long>(w));
  }

  /// Combines the last two parts of the version number.
  /// @return Returns a 32-bit integer.
  long
  GetLowWord ()
    const
  {
    unsigned long w = (n4 & 0xffff);
    w |= static_cast<unsigned long>(n3 & 0xffff) << 16;
    return (static_cast<long>(w));
  }

  /// Compares this version number to another.
  /// @param other The other version number.
  /// @return Returns -1, if this version number is less than the other.
  /// Returns 0, if both version numbers compare equal.
  /// Otherwise returns 1.
  int
  CompareTo (/*[in]*/ const VersionNumber & other)
    const
  {
    long cmp1 = GetHighWord() - other.GetHighWord();
    long cmp2 = GetLowWord() - other.GetLowWord();
    if (cmp1 < 0 || (cmp1 == 0 && cmp2 < 0))
      {
	return (-1);
      }
    else if (cmp1 > 0 || (cmp1 == 0 && cmp2 > 0))
      {
	return (1);
      }
    else
      {
	return (0);
      }
  }

  /// Compares this version number to another.
  /// @param other The other version number (as a string).
  /// @return Returns -1, if this version number is less than the other.
  /// Returns 0, if both version numbers compare equal.
  /// Otherwise returns 1.
  int
  CompareTo (/*[in]*/ const MIKTEXCHAR * lpszOther)
    const
  {
    return (CompareTo(Parse(lpszOther)));
  }

  /// Converts this version number into a string object.
  /// @return Returns the string object.
  MIKTEXEXPORT
  tstring
  MIKTEXCALL
  ToString ()
    const;

  /// Parses the string representation of a version number.
  /// @param lpsz The string to parse.
  /// @param[out] The version number object to fill.
  /// @returns Returns true if the string could be parsed.
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  TryParse (/*[in]*/ const MIKTEXCHAR * lpsz,
	    /*[out]*/ VersionNumber &	versionNumber);

  /// Parses the string representation of a version number.
  /// @param lpsz The string to parse.
  /// @returns Returns the parsed version number.
  static
  MIKTEXEXPORT
  VersionNumber
  MIKTEXCALL
  Parse (/*[in]*/ const MIKTEXCHAR * lpsz);
};

/// Compares two version numbers.
/// @param v1 The first version number.
/// @param v2 The second version number.
/// @return Returns true, if both version numbers compare equal.
inline
bool
operator== (/*[in]*/ const VersionNumber & v1,
	    /*[in]*/ const VersionNumber & v2)
{
  return (v1.CompareTo(v2) == 0);
}

/// Compares two version numbers.
/// @param v1 The first version number.
/// @param v2 The second version number.
/// @return Returns true, if the first version number is less than the
/// other.
inline
bool
operator< (/*[in]*/ const VersionNumber & v1,
	   /*[in]*/ const VersionNumber & v2)
{
  return (v1.CompareTo(v2) < 0);
}

/* _________________________________________________________________________

   FileMode
   _________________________________________________________________________ */

/// File mode enum class.
class FileModeEnum
{
public:
  enum EnumType {
    /// Open a file for append access; that is, writing at the end of
    /// file only. If the file already exists, its initial contents
    /// are unchanged and output to the stream is appended to the end
    /// of the file. Otherwise, a new, empty file is created.
    Append,
    /// The file will be create, if it doesn't already exist.
    Create,
    /// Open an existing file.
    Open,
  };
};

/// File mode enum.
typedef EnumWrapper<FileModeEnum> FileMode;

/* _________________________________________________________________________

   FileAccess
   _________________________________________________________________________ */

/// File access enum class.
class FileAccessEnum
{
public:
  enum EnumType {
    None = 0,
    /// Read access.
    Read = 1,
    /// Write access.
    Write = 2,
    /// Read/Write access.
    ReadWrite = Read | Write,
  };
};

/// File access enum.
typedef EnumWrapper<FileAccessEnum> FileAccess;

/* _________________________________________________________________________

   FileShare
   _________________________________________________________________________ */

/// File share enum class.
class FileShareEnum
{
public:
  enum EnumType {
    /// No other stream can be opened on the file.
    None = 0,
    /// Other streams can be opened for reading only.
    Read = 1,
    /// Other streams can be opened for writing only.
    Write = 2,
    /// Other streams can be opened for reading and writing.
    ReadWrite = Read | Write,
  };
};

/// File share enum.
typedef EnumWrapper<FileShareEnum> FileShare;
  
/* _________________________________________________________________________

   BufferSizes
   _________________________________________________________________________ */

/// Hardcoded buffer sizes.
class BufferSizes
{
public:
  enum {
    /// Maximum length of a path name (including the terminating null-byte).
    MaxPath = 260,
    /// Maximum length of a package key (including the terminating
    /// null-byte).
    MaxPackageName = 256,
    /// Maximum length of a configuration key (including the
    /// terminating null-byte).
    MaxCfgName = 256,
  };
};

/* _________________________________________________________________________

   ConvertPathNameFlags
   _________________________________________________________________________ */

/// Path name conversion flags enum class.
class ConvertPathNameFlagsEnum
{
public:
  enum EnumType {
    None = 0,
    /// Replaces backslashes with slashes.
    ToUnix = 1,
    /// Replaces slashes with backslashes.
    ToDos = 2,
    /// Replaces capital letters with their small letter counterpart.
    MakeLower = 4,
    /// Replaces small letters with their capital letter counterpart.
    MakeUpper = 8,
#if defined(MIKTEX_WINDOWS)
    /// Removes blanks from the path name.
    RemoveBlanks = 16,
#endif
    /// Makes the path name relative to the current directory.
    MakeRelative = 32,
    /// Makes the path name absolute.
    MakeAbsolute = 64,
    /// Resolves symbolic links in the path name.
    ResolveSymbolicLinks = 128,
  };
};

/// Path name conversion flags enum.
typedef EnumWrapper<ConvertPathNameFlagsEnum> ConvertPathNameFlags;

/* _________________________________________________________________________

   TriState
   _________________________________________________________________________ */

/// Tri-state enum class.
class TriStateEnum
{
public:
  enum EnumType {
    /// False (no, off, disabled).
    False,
    /// True (yes, on, enabled).
    True,
    /// Undetermined.
    Undetermined,
  };
};

/// Tri-state enum.
typedef EnumWrapper<TriStateEnum> TriState;

/* _________________________________________________________________________

   MiKTeXException
   _________________________________________________________________________ */

/// The MiKTeX exception base class.
class MiKTeXException : public std::exception
{
  /// Initializes a new MiKTeX exception.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  MiKTeXException ();

  /// Initializes a new MiKTeXException object.
  /// @param lpszProgramInvocationName Name of the program where the
  /// exception was thrown.
  /// @param lpszMessage The message to be presented to the user.
  /// @param lpszInfo Additional info (e.g., a file name).
  /// @param lpszSourceFile The source filew where the exception was thrown.
  /// @param sourceLine The location within the source file where the
  /// exception was thrown.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  MiKTeXException (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
		   /*[in]*/ const MIKTEXCHAR *	lpszMessage,
		   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		   /*[in]*/ int			sourceLine);

  /// Gets the exception description.
  /// @return A null-terminated string.
public:
  virtual
  const char *
  what ()
    const
    throw ()
  {
#if defined(MIKTEX_UNICODE)
#  error Unimplemented: MiKTeXException::what()
#else
    return (message.c_str());
#endif
  }

  /// Destructor.
public:
  virtual
  MIKTEXCALL
  ~MiKTeXException ()
  throw ()
  {
  }

  /// Gets additional information.
  /// @return Returns a string object.
public:
  tstring
  GetInfo ()
    const
    throw ()
  {
    return (info);
  }

  /// Gets the name of the invoked program.
  /// @return Returns a string object.
public:
  tstring
  GetProgramInvocationName ()
    const
    throw ()
  {
    return (programInvocationName);
  }

  /// Gets the name of the source code file.
  /// @return Returns a string object.
public:
  tstring
  GetSourceFile ()
    const
    throw ()
  {
    return (sourceFile);
  }

  /// Gets the location (line number) in the source code file.
  /// @return Returns the line number.
public:
  int
  GetSourceLine ()
    const
    throw ()
  {
    return (sourceLine);
  }

private:
  tstring info;

private:
  tstring message;

private:
  tstring programInvocationName;

private:
  tstring sourceFile;

private:
  int sourceLine;
};

/* _________________________________________________________________________

   OperationCancelledException
   _________________________________________________________________________ */

class OperationCancelledException : public MiKTeXException
{
public:
  MIKTEXEXPORT
  MIKTEXCALL
  OperationCancelledException ();

public:
  MIKTEXEXPORT
  MIKTEXCALL
  OperationCancelledException
  (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName,
   /*[in]*/ const MIKTEXCHAR *	lpszMessage,
   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
   /*[in]*/ int			sourceLine);
};

/* _________________________________________________________________________

   IOException
   _________________________________________________________________________ */

/// Instances of this class represent I/O exceptions.
class IOException : public MiKTeXException
{
  /// Initializes a new IOException object.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  IOException ();

  /// Initializes a new IOException object.
  /// @param lpszProgramInvocationName Name of the program where the
  /// exception was thrown.
  /// @param lpszMessage The message to be presented to the user.
  /// @param lpszInfo Additional info (e.g., a file name).
  /// @param lpszSourceFile The source filew where the exception was thrown.
  /// @param sourceLine The location within the source file where the
  /// exception was thrown.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  IOException (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
	       /*[in]*/ const MIKTEXCHAR *	lpszMessage,
	       /*[in]*/ const MIKTEXCHAR *	lpszInfo,
	       /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
	       /*[in]*/ int			sourceLine);
};

/* _________________________________________________________________________

   BrokenPipeException
   _________________________________________________________________________ */

/// File not found exception class.
/// An instance of this class is thrown when a file could not be found.
class BrokenPipeException : public IOException
{
  /// Initializes a new BrokenPipeException object.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  BrokenPipeException ();

  /// Initializes a new BrokenPipeException object.
  /// @param lpszProgramInvocationName Name of the program where the
  /// exception was thrown.
  /// @param lpszMessage The message to be presented to the user.
  /// @param lpszInfo Additional info (e.g., a file name).
  /// @param lpszSourceFile The source filew where the exception was thrown.
  /// @param sourceLine The location within the source file where the
  /// exception was thrown.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  BrokenPipeException (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName,
		       /*[in]*/ const MIKTEXCHAR *	lpszMessage,
		       /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		       /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		       /*[in]*/ int			sourceLine);
};

/* _________________________________________________________________________

   FileNotFoundException
   _________________________________________________________________________ */

/// File not found exception class.
/// An instance of this class is thrown when a file could not be found.
class FileNotFoundException : public IOException
{
  /// Initializes a new FileNotFoundException object.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  FileNotFoundException ();

  /// Initializes a new FileNotFoundException object.
  /// @param lpszProgramInvocationName Name of the program where the
  /// exception was thrown.
  /// @param lpszMessage The message to be presented to the user.
  /// @param lpszInfo Additional info (e.g., a file name).
  /// @param lpszSourceFile The source filew where the exception was thrown.
  /// @param sourceLine The location within the source file where the
  /// exception was thrown.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  FileNotFoundException (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName,
			 /*[in]*/ const MIKTEXCHAR *	lpszMessage,
			 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
			 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
			 /*[in]*/ int			sourceLine);
};

/* _________________________________________________________________________

   UnauthorizedAccessException
   _________________________________________________________________________ */

/// Unauthorized access exception class.
/// An instance of this class is thrown when the operating system denies
/// access.
class UnauthorizedAccessException : public MiKTeXException
{
  /// Initializes a new UnauthorizedAccessException object.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  UnauthorizedAccessException ();

  /// Initializes a new UnauthorizedAccessException object.
  /// @param lpszProgramInvocationName Name of the program where the
  /// exception was thrown.
  /// @param lpszMessage The message to be presented to the user.
  /// @param lpszInfo Additional info (e.g., a file name).
  /// @param lpszSourceFile The source filew where the exception was thrown.
  /// @param sourceLine The location within the source file where the
  /// exception was thrown.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  UnauthorizedAccessException
  (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
   /*[in]*/ const MIKTEXCHAR *	lpszMessage,
   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
   /*[in]*/ int			sourceLine);
};

/* _________________________________________________________________________

   SharingViolationException
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
/// Sharing violation exception class.
class SharingViolationException : public IOException
{
  /// Initializes a new SharingViolationException object.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  SharingViolationException ();

  /// Initializes a new SharingViolationException object.
  /// @param lpszProgramInvocationName Name of the program where the
  /// exception was thrown.
  /// @param lpszMessage The message to be presented to the user.
  /// @param lpszInfo Additional info (e.g., a file name).
  /// @param lpszSourceFile The source filew where the exception was thrown.
  /// @param sourceLine The location within the source file where the
  /// exception was thrown.
public:
  MIKTEXEXPORT
  MIKTEXCALL
  SharingViolationException
  (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
   /*[in]*/ const MIKTEXCHAR *	lpszMessage,
   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
   /*[in]*/ int			sourceLine);
};
#endif

/* _________________________________________________________________________

   C++ Inline Functions
   _________________________________________________________________________ */

MIKTEXINLINE
size_t
StrLen (/*[in]*/ const MIKTEXCHAR * lpsz)
{
#if defined(MIKTEX_UNICODE)
  return (wcslen(lpsz));
#else
  return (strlen(lpsz));
#endif
}

MIKTEXINLINE
MIKTEXCHAR *
StrDup (/*[in]*/ const MIKTEXCHAR * lpsz)
{
#if defined(_MSC_VER)
  return (_tcsdup(lpsz));
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: StrDup()
#else
  return (strdup(lpsz));
#endif
}

MIKTEXINLINE
int
StringCompare (/*[in]*/ const MIKTEXCHAR *	lpsz1,
	       /*[in]*/ const MIKTEXCHAR *	lpsz2,
	       /*[in]*/ bool			ignoreCase)
{
#if defined(_MSC_VER)
  return (ignoreCase
	  ? _tcsicmp(lpsz1, lpsz2)
	  : _tcscmp(lpsz1, lpsz2));
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: StringCompare()
#else
  return (ignoreCase
	  ? strcasecmp(lpsz1, lpsz2)
	  : strcmp(lpsz1, lpsz2));
#endif
}

MIKTEXINLINE
int
StringCompare (/*[in]*/ const MIKTEXCHAR *	lpsz1,
	       /*[in]*/ const MIKTEXCHAR *	lpsz2)
{
  return (StringCompare(lpsz1, lpsz2, false));
}

/* _________________________________________________________________________

   SmartPointer
   _________________________________________________________________________ */

/// Simple smart pointer class.
template<class T>
class SmartPointer
{
public:
  SmartPointer (/*[in]*/ T * pObject = 0)
    : pObject (pObject)
  {
    if (pObject != 0)
      {
	pObject->AddRef ();
      }
  }

public:
  SmartPointer (/*[in]*/ const SmartPointer<T> & other)
    : pObject (other.pObject)
  {
    if (pObject != 0)
      {
	pObject->AddRef ();
      }
  }

public:
  ~SmartPointer ()
  {
    try
      {
	if (pObject != 0)
	  {
	    pObject->Release ();
	  }
      }
    catch (const std::exception *)
      {
      }
  }

public:
  /// Creates a new instance of the pointee class.  The reference
  /// count of the pointee is incremented.
  void
  Create ()
  {
    pObject = T::Create();
    pObject->AddRef ();
  }

public:
  T *
  Get ()
    const
  {
    return (pObject);
  }

  /// Returns the pointee.
public:
  T *
  operator-> ()
    const
  {
    return (pObject);
  }

public:
  T &
  operator* ()
    const
  {
    return (*pObject);
  }

public:
  SmartPointer<T> &
  operator= (/*[in]*/ T * pOther)
  {
    if (pObject != pOther)
      {
	T * pOld = pObject;
	pObject = pOther;
	if (pObject != 0)
	  {
	    pObject->AddRef();
	  }
	if (pOld != 0)
	  {
	    pOld->Release ();
	  }
      }
    return (*this);
  }

public:
  SmartPointer<T> &
  operator= (/*[in]*/ const SmartPointer<T> & other)
  {
    return (operator=(other.pObject));
  }

  /// Decrements the reference count of the pointee.
public:
  void
  Release ()
  {
    operator= (reinterpret_cast<T*>(0));
  }

private:
  T * pObject;
};

/* _________________________________________________________________________

   PaperSizeInfo
   _________________________________________________________________________ */

/// Paper size info.
class
PaperSizeInfo
{
public:
  static
  MIKTEXEXPORT
  PaperSizeInfo
  MIKTEXCALL
  Parse (/*[in]*/ const MIKTEXCHAR *	lpsz);

  /// Paper name (e.g., "A4")
public:
  tstring name;

public:
  tstring dvipsName;

  /// Paper width (in 72nds of an inch).
public:
  int width;

  /// Paper height (in 72nds of an inch).
public:
  int height;
};

/* _________________________________________________________________________

   Utils
   _________________________________________________________________________ */

/// MiKTeX utility class.
class
MIKTEXNOVTABLE
Utils
{
public:
  /// Concatenates a string to the end of another string.
  /// @param lpszBuf The null-terminated destination string buffer.
  /// @param bufSize Size (in characters) of the destination
  /// string buffer.
  /// @param lpszSource The null-terminated string to be appended.
  /// @return Returns the length (in characters) of the result.
  static
  MIKTEXEXPORT
  size_t
  MIKTEXCALL
  AppendString (/*[in,out]*/ MIKTEXCHAR *	lpszBuf,
		/*[in]*/ size_t			bufSize,
		/*[in]*/ const MIKTEXCHAR *	lpszSource);

  /// Replaces all occurences of a sub-string within a string.
  /// @param[out] lpszBuf The destination string buffer.
  /// @param bufSize The size (in characters) of the destination buffer.
  /// @param lpszSource The string to be searched.
  /// @param lpszString1 The sub-string to be replaced.
  /// @param lpszString2 The replacement sub-string.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  ReplaceString (/*[out]*/ MIKTEXCHAR *		lpszBuf,
		 /*[in,out]*/ size_t &		bufSize,
		 /*[in]*/ const MIKTEXCHAR *	lpszSource,
		 /*[in]*/ const MIKTEXCHAR *	lpszString1,
		 /*[in]*/ const MIKTEXCHAR *	lpszString2);

  /// Copies a string.
  /// @param[out] lpszBuf The destination string buffer.
  /// @param bufSize Size (in characters) of the destination string buffer.
  /// @param lpszSource The null-terminated string to be copied.
  /// @return Returns the length (in characters) of the result.
public:
  static
  MIKTEXEXPORT
  size_t
  MIKTEXCALL
  CopyString (/*[out]*/ MIKTEXCHAR *		lpszBuf,
	      /*[in]*/ size_t			bufSize,
	      /*[in]*/ const MIKTEXCHAR *	lpszSource);

  /// Gets the value of an environment variable.
  /// @param lpszName The name of the environment variable.
  /// @param[out] str The string object to be filled with the value.
  /// @return Returns true if the environment variable exists.
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  GetEnvironmentString (/*[in]*/ const MIKTEXCHAR *	lpszName,
			/*[out]*/ tstring &		str);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  GetEnvironmentString (/*[in]*/ const MIKTEXCHAR *	lpszName,
			/*[out]*/ PathName &		path);

  /// Gets the value of an environment variable.
  /// @param lpszName The name of the environment variable.
  /// @param[out] lpszOut The destination string buffer.
  /// @param sizeOut The size (in characters) of the destination string
  /// buffer.
  /// @return Returns true if the environment variable exists.
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  GetEnvironmentString (/*[in]*/ const MIKTEXCHAR *	lpszName,
			/*[out]*/ MIKTEXCHAR *		lpszOut,
			/*[in]*/ size_t			sizeOut);

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  GetDefPrinter (/*[out]*/ MIKTEXCHAR *		lpszPrinterName,
		 /*[in,out]*/ size_t *		pBufferSize);
#endif

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Contains (/*[in]*/ const MIKTEXCHAR *	lpszList,
	    /*[in]*/ const MIKTEXCHAR *	lpszElement,
	    /*[in]*/ const MIKTEXCHAR *	lpszDelim);

public:
  static
  MIKTEXEXPORT
  tstring
  MIKTEXCALL
  GetExeName ();

  /// Gets the MiKTeX version information as a string.
public:
  static
  MIKTEXEXPORT
  tstring
  MIKTEXCALL
  GetMiKTeXVersionString ();

  /// Gets the MiKTeX banner string.
  /// The banner string includes the MiKTeX version number.
  /// @return Returns a string object.
public:
  static
  MIKTEXEXPORT
  tstring
  MIKTEXCALL
  GetMiKTeXBannerString ();

public:
  static
  MIKTEXEXPORT
  tstring
  MIKTEXCALL
  MakeProgramVersionString (/*[in]*/ const MIKTEXCHAR *	lpszProgramName,
			    /*[in]*/ const VersionNumber &	versionNumber);

public:
  static
  MIKTEXEXPORT
  tstring
  MIKTEXCALL
  GetOSVersionString ();

#if defined(MIKTEX_DLL)
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  UncompressFile (/*[in]*/ const MIKTEXCHAR *	lpszPathIn,
		  /*[out]*/ PathName &		pathOut);
#endif

public:
  static
  MIKTEXEXPORT
  const MIKTEXCHAR *
  MIKTEXCALL
  GetRelativizedPath (/*[in]*/ const MIKTEXCHAR * lpszPath,
		      /*[in]*/ const MIKTEXCHAR * lpszRoot);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  GetUncRootFromPath (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		      /*[out]*/ PathName &		uncRoot);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  IsAbsolutePath (/*[in]*/ const MIKTEXCHAR *	lpszPath);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  IsParentDirectoryOf (/*[in]*/ const MIKTEXCHAR * lpszParentDir,
		       /*[in]*/ const MIKTEXCHAR * lpszFileName);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  MakeTeXPathName (/*[in,out]*/ PathName &	path);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetEnvironmentString (/*[in]*/ const MIKTEXCHAR *	lpszValueName,
			/*[in]*/ const MIKTEXCHAR *	lpszValue);

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  RemoveBlanksFromPathName (/*[in,out]*/ PathName &	path);
#endif

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  PrintException (/*[in]*/ const std::exception &	e);

  /// Prints a MiKTeXException to the standard error stream.
  /// @param e Execption to print.  
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  PrintException (/*[in]*/ const MiKTeXException &	e);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  ReadUntilDelim (/*[out]*/ tstring &		str,
		  /*[in]*/ MIKTEXCHARINT	delim,
		  /*[in]*/ FILE *		stream);

  /// Creates a formatted string object.
  /// @param lpszFormat The format of the string (printf() syntax).
  /// @param arglist Argument list.
  /// @return Returns a string object.
public:
  static
  MIKTEXEXPORT
  tstring
  MIKTEXCALL
  FormatString (/*[in]*/ const MIKTEXCHAR * lpszFormat,
		/*[in]*/ va_list	    arglist);

public:
  static
  MIKTEXEXPORT
  tstring
  MIKTEXCALL
  Hexify (/*[in]*/ const void *	pv,
	  /*[in]*/ size_t	nBytes,
	  /*[in]*/ bool		lowerCase);

public:
  static
  MIKTEXEXPORT
  tstring
  MIKTEXCALL
  Hexify (/*[in]*/ const void *	pv,
	  /*[in]*/ size_t	nBytes);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  ParseDvipsMapLine (/*[in]*/ const tstring &	line,
		     /*[out]*/ FontMapEntry &	fontMapEntry);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  IsMiKTeXDirectRoot (/*[in]*/ const PathName &	root);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  RegisterMiKTeXUser ();

public:
#if defined(MIKTEX_WINDOWS)
  static
  MIKTEXEXPORT
  PathName
  MIKTEXCALL
  GetFolderPath (/*[in]*/ int	nFolder,
		 /*[in]*/ int	nFallbackFolder,
		 /*[in]*/ bool	getCurrentPath);
#endif
};

/* _________________________________________________________________________

   MD5
   _________________________________________________________________________ */

/// Instances of this struct represent MD5 values.
struct MD5
{
public:
  /// Initializes a new MD5 value.
  MD5 ()
  {
    memset (bits, 0, nBytes);
  }

  /// Copies another MD5 object into a new MD5 object.
public:
  MD5 (/*[in]*/ const MD5 & other)
  {
    memcpy (bits, other.bits, nBytes);
  }

  /// Calculates the MD5 value of a file.
  /// @param lpszPath The path name of the file.
  /// @return Returns the MD5 of the file.
public:
  static
  MIKTEXEXPORT
  MD5
  MIKTEXCALL
  FromFile (/*[in]*/ const MIKTEXCHAR * lpszPath);

  /// Parses the string represention of an MD5.
  /// @param lpszHexString The string representation (32 hex characters).
  /// @return Returns the MD5.
public:
  static
  MIKTEXEXPORT
  MD5
  MIKTEXCALL
  Parse (/*[in]*/ const MIKTEXCHAR * lpszHexString);

  /// Converts this MD5 into a string object.
  /// @return Returns a string object.
public:
  tstring
  ToString ()
    const
  {
    return (Utils::Hexify(bits, nBytes));
  }

private:
  enum { nBytes = 16 };

private:
  typedef unsigned char BITS[nBytes];

public:
  const BITS &
  GetBits ()
    const
  {
    return (bits);
  }

  /// Sets the bits of this MD5 value.
  /// @param bits A vector with 16 bytes.
  /// @return Returns a reference to this MD5 object.
public:
  MD5 &
  operator= (/*[in]*/ const std::vector<unsigned char> & bits)
  {
    if (bits.size() != nBytes)
      {
	throw MiKTeXException (0,
			       MIKTEXTEXT("Invalid argument."),
			       reinterpret_cast<const MIKTEXCHAR *>(0),
			       __FILE__,
			       __LINE__);
      }
    memcpy (this->bits, &bits[0], nBytes);
    return (*this);
  }

public:
  BITS &
  GetBuffer ()
  {
    return (bits);
  }

private:
  BITS bits;
};

/// Compares two MD5 objects.
/// @param left The first MD5 object.
/// @param right The second MD5 object.
/// @return Returns true if both MD5 objects compare equal.
inline
bool
operator== (/*[in]*/ const MD5 & left,
	    /*[in]*/ const MD5 & right)
{
  return (memcmp(left.GetBits(), right.GetBits(), sizeof(left)) == 0);
}

/* _________________________________________________________________________

   MD5Builder
   _________________________________________________________________________ */

/// Instances of this struct can be used to calculate MD5 values.
struct MD5Builder
{
  /// Initializes a new MD5Builder object.
public:
  MD5Builder ()
  {
    Init ();
  }

  /// Resets the state of this MD5Builder object.
public:
  void
  Init ()
  {
    MD5Init (&ctx);
  }

  /// Updates the state of this MD5Builder object.
  /// @param ptr Bytes to be contributed to the MD5 value.
  /// @param size Size of the byte buffer.
public:
  void
  Update (/*[in]*/ const void *	ptr,
	  /*[in]*/ size_t	size)
  {
    MD5Update (&ctx, ptr, static_cast<unsigned int>(size));
  }

  /// Calculates the final MD5 value.
  /// @return Returns the MD5 object.
public:
  MD5
  Final ()
  {
    MD5Final (md5.GetBuffer(), &ctx);
    return (md5);
  }

  /// Gets the calculated MD5 value.
  /// @return Returns the MD5 object.
public:
  MD5
  GetMD5 ()
    const
  {
    return (md5);
  }

private:
  MD5_CTX ctx;

private:
  MD5 md5;
};

/* _________________________________________________________________________

   CharArray
   _________________________________________________________________________ */

/// Instances of this class store character strings of a fixed length.
template<int SIZE>
class CharArray
{
  /// Initializes a new instance of the class.
public:
  CharArray ()
  {
    buffer[0] = 0;
  }

  /// Copies another CharArray object into a new CharArray object.
public:
  CharArray (/*[in]*/ const CharArray<SIZE> & other)
  {
    Utils::CopyString (buffer, SIZE, other.buffer);
  }

  /// Copies a character string into a new CharArray object.
  /// @param lpsz A null-terminated character string.
public:
  CharArray (/*[in]*/ const MIKTEXCHAR * lpsz)
  {
    if (lpsz == 0)
      {
	buffer[0] = 0;
      }
    else
      {
	Utils::CopyString (buffer, SIZE, lpsz);
      }
  }

  /// Sets this CharArray object equal to another CharArray object.
  /// @param rhs The other CharArray object.
  /// @return Returns a reference to this object.
public:
  CharArray<SIZE> &
  operator= (/*[in]*/ const CharArray<SIZE> & rhs)
  {
    if (this != &rhs)
      {
	Utils::CopyString (buffer, SIZE, rhs.buffer);
      }
    return (*this);
  }

public:
  /// Copies a null-terminated character string into this CharArray
  /// object.
  /// @param lpszRhs A null-terminated character string.
  /// @return Returns a reference to this object.
  CharArray<SIZE> &
  operator= (/*[in]*/ const MIKTEXCHAR * lpszRhs)
  {
    if (buffer != lpszRhs)
      {
	Utils::CopyString (buffer, SIZE, lpszRhs);
      }
    return (*this);
  }

  /// Copies the contents of a string object into this CharArray object.
  /// @param rhs The string object to be copied.
  /// @return Returns a reference to this object.
public:
  CharArray<SIZE> &
  operator= (/*[in]*/ const tstring & rhs)
  {
    Utils::CopyString (buffer, SIZE, rhs.c_str());
    return (*this);
  }

  /// Returns a const reference to the character at the specified index.
  /// @param idx Index into the char array.
  /// @return Returns a const reference to the character.
public:
  const MIKTEXCHAR &
  operator[] (/*[in]*/ size_t idx)
    const
  {
    assert (idx < SIZE);
    return (buffer[idx]);
  }

public:
  /// Returns a reference to the character at the specified index.
  /// @param idx Index into the char array.
  /// @return Returns a reference to the character.
  MIKTEXCHAR &
  operator[] (/*[in]*/ size_t idx)
  {
    assert (idx < SIZE);
    return (buffer[idx]);
  }

  /// Gets a pointer to the character sequence.
  /// @return Returns a pointer to the first character in the sequence.
public:
  const MIKTEXCHAR *
  Get ()
    const
  {
    return (buffer);
  }

public:
  MIKTEXCHAR *
  GetBuffer ()
  {
    return (buffer);
  }

  /// Gets the size of this CharArray object.
  /// @return Returns the size.
public:
  size_t
  GetSize ()
    const
  {
    return (SIZE);
  }

  /// Gets the length of the character sequence.
  /// @return Returns the length (in characters).
public:
  size_t
  GetLength ()
    const
  {
    return (StrLen(buffer));
  }

  /// Tests if the character sequence is empty.
  /// @return Returns true if the character sequence is empty.
public:
  bool
  Empty ()
    const
  {
    return (buffer[0] == 0);
  }

  /// Converts this CharArray object into a string object.
  /// @return Returns a string object.
public:
  tstring
  ToString ()
    const
  {
    return (tstring(buffer));
  }

protected:
  MIKTEXCHAR buffer[SIZE];
};

/// Compares two CharArray objects.
/// @param lhs The first CharArray object.
/// @param lhs The second CharArray object.
/// @return Returns true if both CharArray objects compare equal.
template<int SIZE>
bool
operator== (/*[in]*/ const CharArray<SIZE> & lhs,
	    /*[in]*/ const CharArray<SIZE> & rhs)
{
  return (StringCompare(lhs, rhs) == 0);
}

/* _________________________________________________________________________

   PathName
   _________________________________________________________________________ */

/// Instances of this class can be used to store path names.
class PathName : public CharArray<BufferSizes::MaxPath>
{
public:
  enum {
    DosDirectoryDelimiter = MIKTEXTEXT('\\'),
    DosPathNameDelimiter = MIKTEXTEXT(';'),
    UnixDirectoryDelimiter = MIKTEXTEXT('/'),
    UnixPathNameDelimiter = MIKTEXTEXT(':'),
#if defined(MIKTEX_WINDOWS)
    AltDirectoryDelimiter = UnixDirectoryDelimiter,
    DirectoryDelimiter = DosDirectoryDelimiter,
    PathNameDelimiter = DosPathNameDelimiter,
    VolumeDelimiter = MIKTEXTEXT(':'),
#elif defined(MIKTEX_UNIX)
    DirectoryDelimiter = UnixDirectoryDelimiter,
    PathNameDelimiter = UnixPathNameDelimiter,
#else
#  error Unimplemented: PathName::DirectoryDelimiter
#endif
  };

#if defined(MIKTEX_WINDOWS)
public:
  static
  bool
  IsVolumeDelimiter (/*[in]*/ MIKTEXCHARINT ch)
  {
    return (ch == VolumeDelimiter);
  }
#endif

public:
  /// Tests if a character is a directory delimiter.
  /// @param ch The character to test.
  /// @return Returns true if the character is a directory delimiter.
  static
  bool
  IsDirectoryDelimiter (/*[in]*/ MIKTEXCHARINT ch)
  {
    if (ch == DirectoryDelimiter)
      {
	return (true);
      }
#if defined(MIKTEX_WINDOWS)
    return (ch == AltDirectoryDelimiter);
#else
    return (false);
#endif
  }

  /// Initializes a new PathName object.
public:
  PathName ()
  {
  }

  /// Copies another PathName object into a new PathName object.
  /// @param rhs The other PathName object.
public:
  PathName (/*[in]*/ const PathName & rhs)
    : CharArray<BufferSizes::MaxPath> (rhs)
  {
  }

  /// Copies a character string into a new PathName object.
  /// @param rhs Null-terminated character string.
public:
  PathName (/*[in]*/ const MIKTEXCHAR * rhs)
    : CharArray<BufferSizes::MaxPath> (rhs)
  {
  }

  /// Copies a string object into a new PathName object.
  /// @param rhs String object.
public:
  PathName (/*[in]*/ const tstring & rhs)
    : CharArray<BufferSizes::MaxPath> (rhs.c_str())
  {
  }

#if defined(MIKTEX_WINDOWS)
public:
  PathName (/*[in]*/ const MIKTEXCHAR * lpszDrive,
	    /*[in]*/ const MIKTEXCHAR * lpszAbsPath,
	    /*[in]*/ const MIKTEXCHAR * lpszRelPath,
	    /*[in]*/ const MIKTEXCHAR * lpszExtension)
  {
    Set (lpszDrive, lpszAbsPath, lpszRelPath, lpszExtension);
  }
#endif

  /// Combines path name components into a new PathName object.
  /// @param lpszAbsPath The first component (absolute directory path).
  /// @param lpszRelPath The second component (relative file name path).
public:
  PathName (/*[in]*/ const MIKTEXCHAR * lpszAbsPath,
	    /*[in]*/ const MIKTEXCHAR * lpszRelPath)
  {
    Set (lpszAbsPath, lpszRelPath);
  }

  /// Combines path name components into a new PathName object.
  /// @param lpszAbsPath The first component (absolute directory path).
  /// @param lpszRelPath The second component (relative file name path).
  /// @param lpszExtension The third component (file name extension).
public:
  PathName (/*[in]*/ const MIKTEXCHAR * lpszAbsPath,
	    /*[in]*/ const MIKTEXCHAR * lpszRelPath,
	    /*[in]*/ const MIKTEXCHAR * lpszExtension)
  {
    Set (lpszAbsPath, lpszRelPath, lpszExtension);
  }

  /// Combines path name components into a new PathName object.
  /// @param absPath The first component (absolute directory path).
  /// @param relPath The second component (relative file name path).
public:
  PathName (/*[in]*/ const PathName &	absPath,
	    /*[in]*/ const PathName &	relPath)
  {
    Set (absPath, relPath);
  }

  /// Combines path name components into a new PathName object.
  /// @param absPath The first component (absolute directory path).
  /// @param relPath The second component (relative file name path).
  /// @param extension File name extension.
public:
  PathName (/*[in]*/ const PathName &	absPath,
	    /*[in]*/ const PathName &	relPath,
	    /*[in]*/ const PathName &	extension)
  {
    Set (absPath, relPath, extension);
  }

public:
  PathName &
  operator= (/*[in]*/ const MIKTEXCHAR * lpszRhs)
  {
    CharArray<BufferSizes::MaxPath>::operator= (lpszRhs);
    return (*this);
  }

public:
  PathName &
  operator= (/*[in]*/ const tstring & rhs)
  {
    CharArray<BufferSizes::MaxPath>::operator= (rhs);
    return (*this);
  }

  /// Calculates the hash value of this PathName object.
  /// @return Returns the hash value.
public:
  MIKTEXEXPORT
  size_t
  MIKTEXCALL
  GetHash ()
    const;

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Combine (/*[out]*/ MIKTEXCHAR *	lpszPath,
	   /*[in]*/ size_t		sizePath,
	   /*[in]*/ const MIKTEXCHAR *	lpszDrive,
	   /*[in]*/ const MIKTEXCHAR *	lpszAbsPath,
	   /*[in]*/ const MIKTEXCHAR *	lpszRelPath,
	   /*[in]*/ const MIKTEXCHAR *	lpszExtension);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Split (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	 /*[out]*/ MIKTEXCHAR *		lpszDrive,
	 /*[in]*/ size_t		sizeDrive,
	 /*[out]*/ MIKTEXCHAR *		lpszDirectory,
	 /*[in]*/ size_t		sizeDirectory,
	 /*[out]*/ MIKTEXCHAR *		lpszName,
	 /*[in]*/ size_t		sizeName,
	 /*[out]*/ MIKTEXCHAR *		lpszExtension,
	 /*[in]*/ size_t		sizeExtension);
#endif

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Split (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	 /*[out]*/ MIKTEXCHAR *		lpszDirectory,
	 /*[in]*/ size_t		sizeDirectory,
	 /*[out]*/ MIKTEXCHAR *		lpszName,
	 /*[in]*/ size_t		sizeName,
	 /*[out]*/ MIKTEXCHAR *		lpszExtension,
	 /*[in]*/ size_t		sizeExtension);

public:
  MIKTEXCHAR *
  GetFileName (/*[out]*/ MIKTEXCHAR * lpszFileName)
    const
  {
    MIKTEXCHAR szExtension[BufferSizes::MaxPath];
    Split (buffer,
	   0, 0,
	   lpszFileName, BufferSizes::MaxPath,
	   szExtension, BufferSizes::MaxPath);
    Utils::AppendString (lpszFileName, BufferSizes::MaxPath, szExtension);
    return (lpszFileName);
  }

public:
  MIKTEXCHAR *
  GetFileNameWithoutExtension (/*[out]*/ MIKTEXCHAR * lpszFileName)
    const
  {
    Split (buffer,
	   0, 0,
	   lpszFileName, BufferSizes::MaxPath,
	   0, 0);
    return (lpszFileName);
  }

public:
  PathName &
  GetFileNameWithoutExtension (/*[out]*/ PathName & fileName)
    const
  {
    Split (buffer,
	   0, 0,
	   fileName.buffer, BufferSizes::MaxPath,
	   0, 0);
    return (fileName);
  }

public:
  PathName
  GetFileNameWithoutExtension ()
    const
  {
    PathName result;
    GetFileNameWithoutExtension (result);
    return (result);
  }

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Combine (/*[out]*/ MIKTEXCHAR *	lpszPath,
	   /*[in]*/ size_t		sizePath,
	   /*[in]*/ const MIKTEXCHAR *	lpszAbsPath,
	   /*[in]*/ const MIKTEXCHAR *	lpszRelPath,
	   /*[in]*/ const MIKTEXCHAR *	lpszExtension);

#if defined(MIKTEX_WINDOWS)
public:
  PathName &
  Set (/*[in]*/ const MIKTEXCHAR * lpszDrive,
       /*[in]*/ const MIKTEXCHAR * lpszAbsPath,
       /*[in]*/ const MIKTEXCHAR * lpszRelPath, 
       /*[in]*/ const MIKTEXCHAR * lpszExtension)
  {
    Combine (buffer,
	     GetSize(),
	     lpszDrive,
	     lpszAbsPath,
	     lpszRelPath,
	     lpszExtension);
    return (*this);
  }
#endif

  /// Sets this PathName object equal to the combination three path name
  /// components.
  /// @param lpszAbsPath The first component (absolute directory path).
  /// @param lpszRelPath The second component (relative file name path).
  /// @return Returns a reference to this object.
public:
  PathName &
  Set (/*[in]*/ const MIKTEXCHAR * lpszAbsPath,
       /*[in]*/ const MIKTEXCHAR * lpszRelPath)
  {
    Combine (buffer,
	     GetSize(),
	     lpszAbsPath,
	     lpszRelPath,
	     0);
    return (*this);
  }

  /// Sets this PathName object equal to the combination three path name
  /// components.
  /// @param lpszAbsPath The first component (absolute directory path).
  /// @param lpszRelPath The second component (relative file name path).
  /// @param lpszExtension The third component (file name extension).
  /// @return Returns a reference to this object.
public:
  PathName &
  Set (/*[in]*/ const MIKTEXCHAR * lpszAbsPath,
       /*[in]*/ const MIKTEXCHAR * lpszRelPath, 
       /*[in]*/ const MIKTEXCHAR * lpszExtension)
  {
    Combine (buffer,
	     GetSize(),
	     lpszAbsPath,
	     lpszRelPath,
	     lpszExtension);
    return (*this);
  }

  /// Sets this PathName object equal to the combination three path name
  /// components.
  /// @param absPath The first component (absolute directory path).
  /// @param relPath The second component (relative file name path).
  /// @param extension The third component (file name extension).
  /// @return Returns a reference to this object.
public:
  PathName &
  Set (/*[in]*/ const PathName &	absPath,
       /*[in]*/ const PathName &	relPath)
  {
    Combine (buffer,
	     GetSize(),
	     absPath.Get(),
	     relPath.Get(),
	     0);
    return (*this);
  }

  /// Sets this PathName object equal to the combination three path name
  /// components.
  /// @param absPath The first component (absolute directory path).
  /// @param relPath The second component (relative file name path).
  /// @param extension The third component (file name extension).
  /// @return Returns a reference to this object.
public:
  PathName &
  Set (/*[in]*/ const PathName &	absPath,
       /*[in]*/ const PathName &	relPath, 
       /*[in]*/ const PathName &	extension)
  {
    Combine (buffer,
	     GetSize(),
	     absPath.Get(),
	     relPath.Get(),
	     extension.Get());
    return (*this);
  }

  /// Removes the file name component from this path name.
  /// @return Returns a reference to this object.
public:
  PathName &
  RemoveFileSpec ()
  {
    return (CutOffLastComponent());
  }

  /// Removes the directory component from this path name.
  /// @return Returns a reference to this object.
public:
  PathName &
  RemoveDirectorySpec ()
  {
    MIKTEXCHAR fileName[BufferSizes::MaxPath];
    GetFileName (fileName);
    Utils::CopyString (buffer, BufferSizes::MaxPath, fileName);
    return (*this);
  }

  /// Sets this PathName object equal to the current (working) directory.
  /// @return Returns a reference to this object.
public:
  MIKTEXEXPORT
  PathName &
  MIKTEXCALL
  SetToCurrentDirectory ();

  /// Sets this PathName object equal to the temporary directory.
  /// @return Returns a reference to this object.
public:
  MIKTEXEXPORT
  PathName &
  MIKTEXCALL
  SetToTempDirectory ();

  /// Sets this PathName object equal to the name of a temporary file.
  /// @return Returns a reference to this object.
public:
  MIKTEXEXPORT
  PathName &
  MIKTEXCALL
  SetToTempFile ();

public:
  MIKTEXEXPORT
  PathName &
  MIKTEXCALL
  Convert (/*[in]*/ ConvertPathNameFlags	flags);

  /// Replaces backslashes with normal slashes.
  /// @return Returns a reference to this object.
public:
  PathName &
  ToUnix ()
  {
    return (Convert(ConvertPathNameFlags::ToUnix));
  }

  /// Replaces normal slashes with backslashes.
  /// @return Returns a reference to this object.
public:
  PathName &
  ToDos ()
  {
    return (Convert(ConvertPathNameFlags::ToDos));
  }

  /// Normalizes this path name.
  /// @return Returns a reference to this object.
public:
  PathName &
  Normalize ()
  {
#if defined(MIKTEX_WINDOWS)
    ConvertPathNameFlags flags (ConvertPathNameFlags::ToUnix);
    flags |= ConvertPathNameFlags::MakeLower;
    return (Convert(flags));
#else
    // already normalized
    return (*this);
#endif
  }

  /// Resolves symbolic links in this path name.
  /// @return Returns a reference to this object.
public:
  PathName &
  ResolveSymbolicLinks ()
  {
    return (Convert(ConvertPathNameFlags::ResolveSymbolicLinks));
  }

  /// Converts this path name into an absolute path name.
  /// @return Returns a reference to this object.
public:
  PathName &
  MakeAbsolute ()
  {
    return (Convert(ConvertPathNameFlags::MakeAbsolute));
  }

  /// Checks to see whether this path name has the specified extension.
  /// @param lpszExtension File name extension.
  /// @return Returns true, if this path name has the specified extension.
public:
  bool
  HasExtension (/*[in]*/ const MIKTEXCHAR * lpszExtension)
    const
  {
    const MIKTEXCHAR * lpszExt = GetExtension();
    return (lpszExt != 0
	    && (PathName::Compare(lpszExt, lpszExtension) == 0));
  }

  /// Gets the file name extension.
  /// @param[out] lpszExtension The string buffer to be filled with the
  /// file name extension.
  /// @return Returns the string buffer.
public:
  MIKTEXCHAR *
  GetExtension (/*[out]*/ MIKTEXCHAR *	lpszExtension)
    const
  {
    Split (buffer,
	   0, 0,
	   0, 0,
	   lpszExtension, BufferSizes::MaxPath);
    return (lpszExtension);
  }

  /// Gets the file name extension.
  /// @return Returns the file name extension. Returns 0, if the path name
  /// has no file name extension.
public:
  MIKTEXEXPORT
  const MIKTEXCHAR *
  MIKTEXCALL
  GetExtension ()
    const;

  /// Sets the file name extension.
  /// @param lpszExtension The file name extension to set.
  /// @param override Indicates whether an existing file name extension
  /// shall be overridden.
  /// @return Returns a reference to this object.
public:
  MIKTEXEXPORT
  PathName &
  MIKTEXCALL
  SetExtension (/*[in]*/ const MIKTEXCHAR *	lpszExtension,
		/*[in]*/ bool			override);

  /// Sets the file name extension.
  /// @param lpszExtension The file name extension to set. Can be 0,
  /// if the extension is to be removed.
  /// @return Returns a reference to this object.
public:
  PathName &
  SetExtension (/*[in]*/ const MIKTEXCHAR * lpszExtension)
  {
    return (SetExtension(lpszExtension, true));
  }

  /// Checks to see whether this path name ends with a directory delimiter.
  /// @return Returns true if the last character is a directory delimiter.
public:
  bool
  EndsWithDirectoryDelimiter ()
    const
  {
    size_t l = GetLength();
    return (l > 0 && IsDirectoryDelimiter(buffer[l - 1]));
  }

  /// Appends a character string to this path name.
  /// @param lpsz The null-terminated character string to add.
  /// @param appendDirectoryDelimiter Indicates whether a directory delimiter
  /// shall be appended before the string.
  /// @return Returns a reference to this object.
public:
  PathName &
  Append (/*[in]*/ const MIKTEXCHAR *	lpsz,
	  /*[in]*/ bool			appendDirectoryDelimiter)
  {
    if (appendDirectoryDelimiter && buffer[0] != 0)
      {
	AppendDirectoryDelimiter ();
      }
    Utils::AppendString (buffer, BufferSizes::MaxPath, lpsz);
    return (*this);
  }

  /// Appends a path name component to this path name.
  /// @param lpszComponent The null-terminated component to add.
  /// @return Returns a reference to this object.
public:
  PathName &
  AppendComponent (/*[in]*/ const MIKTEXCHAR * lpszComponent)
  {
    return (Append(lpszComponent, true));
  }

  /// Appends a path name component to this path name.
  /// @param lpszComponent The null-terminated component to add.
  /// @return Returns a reference to this object.
public:
  PathName &
  operator+= (/*[in]*/ const MIKTEXCHAR * lpszComponent)
  {
    return (AppendComponent(lpszComponent));
  }

  /// Appends a path name component to this path name.
  /// @param component The component to be appended.
  /// @return Returns a reference to this object.
public:
  PathName &
  operator+= (/*[in]*/ const PathName & component)
  {
    return (AppendComponent(component.Get()));
  }

  /// Appends a path name component to this path name.
  /// @param component The component to be appended.
  /// @return Returns a reference to this object.
public:
  PathName &
  operator+= (/*[in]*/ const tstring & component)
  {
    return (AppendComponent(component.c_str()));
  }

  /// Cuts off the last component from the path name.
  /// @return Returns a reference to this object.
public:
  MIKTEXEXPORT
  PathName &
  MIKTEXCALL
  CutOffLastComponent ();

  /// Makes sure that this path name ends with a directory delimiter.
  /// @return Returns a reference to this object.
public:
  MIKTEXEXPORT
  PathName &
  MIKTEXCALL
  AppendDirectoryDelimiter ();

#if defined(MIKTEX_WINDOWS)
public:
  MIKTEXEXPORT
  PathName &
  MIKTEXCALL
  AppendAltDirectoryDelimiter ();
#endif

#if defined(MIKTEX_WINDOWS)
public:
  void
  RemoveBlanks ()
  {
    Utils::RemoveBlanksFromPathName (*this);
  }
#endif

public:
  /// Compares the prefixes of two path names.
  /// @param lpszPath1 The first path name.
  /// @param lpszPath1 The second path name.
  /// @param count Number of characters to compare.
  /// @return Returns -1, if the first prefix compares lesser than the second.
  /// Returns 0, if both prefixes compare equal.
  /// Returns 1, if the first prefix compares greater than the second.
  static
  MIKTEXEXPORT
  int
  MIKTEXCALL
  Compare (/*[in]*/ const MIKTEXCHAR *	lpszPath1,
	   /*[in]*/ const MIKTEXCHAR *	lpszPath2,
	   /*[in]*/ size_t		count);

public:
  static
  int
  Compare (/*[in]*/ const PathName &	path1,
	   /*[in]*/ const PathName &	path2,
	   /*[in]*/ size_t		count)
  {
    return (Compare(path1.Get(), path2.Get(), count));
  }

  /// Compares two path names.
  /// @param lpszPath1 The first path name.
  /// @param lpszPath1 The second path name.
  /// @return Returns -1, if the first path name compares
  /// lesser than the second.
  /// Returns 0, if both path names compare equal.
  /// Returns 1, if the first path name compares greater than the second.
public:
  static
  MIKTEXEXPORT
  int
  MIKTEXCALL
  Compare (/*[in]*/ const MIKTEXCHAR *	lpszPath1,
	   /*[in]*/ const MIKTEXCHAR *	lpszPath2);

  /// Compares two path names.
  /// @param path1 The first path name.
  /// @param path1 The second path name.
  /// @return Returns -1, if the first path name compares
  /// lesser than the second.
  /// Returns 0, if both path names compare equal.
  /// Returns 1, if the first path name compares greater than the second.
public:
  static
  int
  Compare (/*[in]*/ const PathName & path1,
	   /*[in]*/ const PathName & path2)
  {
    return (Compare(path1.Get(), path2.Get()));
  }

  /// Matches a path name against a wildcard pattern.
  /// @param lpszPattern The wildcard pattern.
  /// @param lpszPath The path name to test.
  /// @return Returns true, if the pattern matches.
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Match (/*[in]*/ const MIKTEXCHAR *	lpszPattern,
	 /*[in]*/ const MIKTEXCHAR *	lpszPath);

public:
  static
  bool
  Match (/*[in]*/ const MIKTEXCHAR *	lpszPattern,
	 /*[in]*/ const PathName &	path)
  {
    return (Match(lpszPattern, path.Get()));
  }
};

MIKTEXINLINE
bool
operator< (/*[in]*/ const PathName & lhs,
	   /*[in]*/ const PathName & rhs)
{
  return (PathName::Compare(lhs, rhs) < 0);
}

  /// Compares two path names.
  /// @param lhs The first path name.
  /// @param rhs The second path name.
  /// @return Returns true, if both path names compare equal.
MIKTEXINLINE
bool
operator== (/*[in]*/ const PathName & lhs,
	    /*[in]*/ const PathName & rhs)
{
  return (PathName::Compare(lhs, rhs) == 0);
}

/// Compares two path names.
/// @param lhs The first path name.
/// @param rhs The second path name.
/// @return Returns true, if both path names compare unequal.
MIKTEXINLINE
bool
operator!= (/*[in]*/ const PathName & lhs,
	    /*[in]*/ const PathName & rhs)
{
  return (PathName::Compare(lhs, rhs) != 0);
}

/* _________________________________________________________________________

   CommandLineBuilder
   _________________________________________________________________________ */

class OptionConventionEnum
{
public:
  enum EnumType {
    None,
    GNU,
    Xt,
    DOS
  };
};

typedef EnumWrapper<OptionConventionEnum> OptionConvention;

class
CommandLineBuilder
{
public:
  MIKTEXEXPORT
  MIKTEXCALL
  CommandLineBuilder ();

public:
  MIKTEXEXPORT
  MIKTEXCALL
  CommandLineBuilder (/*[in]*/ const CommandLineBuilder & other);

public:
  MIKTEXEXPORT
  MIKTEXCALL
  ~CommandLineBuilder ();

public:
  MIKTEXEXPORT
  MIKTEXCALL
  CommandLineBuilder (/*[in]*/ const MIKTEXCHAR *	lpszArgument);

public:
  MIKTEXEXPORT
  MIKTEXCALL
  CommandLineBuilder (/*[in]*/ const MIKTEXCHAR *	lpszArgument1,
		      /*[in]*/ const MIKTEXCHAR *	lpszArgument2);

public:
  MIKTEXEXPORT
  MIKTEXCALL
  CommandLineBuilder (/*[in]*/ const MIKTEXCHAR *	lpszArgument1,
		      /*[in]*/ const MIKTEXCHAR *	lpszArgument2,
		      /*[in]*/ const MIKTEXCHAR *	lpszArgument3);

public:
  MIKTEXEXPORT
  CommandLineBuilder &
  MIKTEXCALL
  operator= (/*[in]*/ const CommandLineBuilder & othe);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetOptionConvention (/*[in]*/ OptionConvention optionConvention);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Clear ();

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  AppendArgument (/*[in]*/ const MIKTEXCHAR * lpszArgument);

public:
  void
  AppendArgument (/*[in]*/ const tstring & argument)
  {
    AppendArgument (argument.c_str());
  }

public:
  void
  AppendArgument (/*[in]*/ const PathName & argument)
  {
    AppendArgument (argument.Get());
  }

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  AppendUnquoted (/*[in]*/ const MIKTEXCHAR * lpszText);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  AppendArguments (/*[in]*/ int			argc,
		   /*[in]*/ const MIKTEXCHAR **	argv);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  AppendArguments (/*[in]*/ const std::vector<tstring>	argv);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  AppendOption (/*[in]*/ const MIKTEXCHAR * lpszOption,
		/*[in]*/ const MIKTEXCHAR * lpszValue);

public:
  void
  AppendOption (/*[in]*/ const MIKTEXCHAR *	lpszOption,
		/*[in]*/ const PathName &	value)
  {
    AppendOption (lpszOption, value.Get());
  }

public:
  void
  AppendOption (/*[in]*/ const MIKTEXCHAR *	lpszOption,
		/*[in]*/ const tstring &	value)
  {
    AppendOption (lpszOption, value.c_str());
  }

public:
  void
  AppendOption (/*[in]*/ const MIKTEXCHAR * lpszOption)
  {
    AppendOption (lpszOption, 0);
  }
  
public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  AppendStdoutRedirection (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			   /*[in]*/ bool		append);

public:
  void
  MIKTEXCALL
  AppendStdoutRedirection (/*[in]*/ const PathName &	path,
			   /*[in]*/ bool		append)
  {
    AppendStdoutRedirection (path.Get(), append);
  }

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  AppendStdinRedirection (/*[in]*/ const MIKTEXCHAR *	lpszPath);

public:
  void
  MIKTEXCALL
  AppendStdinRedirection (/*[in]*/ const PathName &	path)
  {
    AppendStdinRedirection (path.Get());
  }

public:
  void
  AppendStdoutRedirection (/*[in]*/ const MIKTEXCHAR * lpszPath)
  {
    AppendStdoutRedirection (lpszPath, false);
  }

public:
  void
  AppendStdoutRedirection (/*[in]*/ const PathName & path)
  {
    AppendStdoutRedirection (path.Get());
  }

public:
  MIKTEXEXPORT
  const MIKTEXCHAR *
  MIKTEXCALL
  Get ();

private:
  void * p;
};

/* _________________________________________________________________________

   RegisterRootDirectoriesFlags
   _________________________________________________________________________ */

class RegisterRootDirectoriesFlagsEnum
{
public:
  enum EnumType {
#if defined(MIKTEX_WINDOWS)
    Registry = 0,
#endif
    ConfigFile = 1,
    Temporary = 2
  };
};

typedef
EnumWrapper<RegisterRootDirectoriesFlagsEnum> RegisterRootDirectoriesFlags;

/* _________________________________________________________________________

   PolicyFlags
   _________________________________________________________________________ */

class PolicyFlagsEnum
{
public:
  enum EnumType {
    None = 0,
    DataRootHighestPriority = 1
  };
};

typedef EnumWrapper<PolicyFlagsEnum> PolicyFlags;

/* _________________________________________________________________________

   StartupConfig
   _________________________________________________________________________ */

struct StartupConfig
{
public:
  tstring roots;

public:
  PathName userDataRoot;

public:
  PathName userConfigRoot;

public:
  PathName commonDataRoot;

public:
  PathName commonConfigRoot;

public:
  PathName installRoot;
};

/* _________________________________________________________________________

   C++ Constants
   _________________________________________________________________________ */

const unsigned INVALID_ROOT_INDEX = static_cast<unsigned>(-1);

// TEXMF root paths shouldn't be longer than this
const size_t MAX_TEXMF_ROOT_PATH = 100;

// relative TEXMF paths shouldn't be longer than this
const size_t MAX_TEXMF_REL_PATH = 150;

// The virtual TEXMF root MPM_ROOT_PATH is assigned to the MiKTeX
// package manager.  We make sure that MPM_ROOT_PATH is a valid path
// name.  On the other hand, it must not interfere with an existing
// file system.
#if defined(MIKTEX_WINDOWS)
// An UNC path with an impossible share name suits our needs: `['
// isn't a valid character in a share name (KB236388)
const MIKTEXCHAR * const MPM_ROOT_PATH = MIKTEXTEXT("\\\\MiKTeX\\[MPM]");
const size_t MPM_ROOT_PATH_LEN_ = 14;
#else
const MIKTEXCHAR * const MPM_ROOT_PATH = MIKTEXTEXT("//MiKTeX/[MPM]");
const size_t MPM_ROOT_PATH_LEN_ = 14;
#endif

#if ! defined(NDEBUG)
#define MPM_ROOT_PATH_LEN					\
  static_cast<size_t>(assert(MiKTeX::Core::StrLen(MPM_ROOT_PATH)	\
		             == MPM_ROOT_PATH_LEN_),		\
                      MPM_ROOT_PATH_LEN_)
#else  // not debug
const size_t MPM_ROOT_PATH_LEN = MPM_ROOT_PATH_LEN_;
#endif // not debug

/* _________________________________________________________________________

   SpecialPath
   _________________________________________________________________________ */

/// Special path enum class
class SpecialPathEnum
{
public:
  enum EnumType {
    /// The installation root directory.
    InstallRoot,

    /// The common data root directory.
    CommonDataRoot,

    /// The data root directory of the user.
    UserDataRoot,

    CommonConfigRoot,
    UserConfigRoot,

    /// The prefered data root directory. For a shared MiKTeX setup
    /// this is equivalent to the common data directory. For a private
    /// MiKTeX setup this is equivalent to the user data directory.
    DataRoot,

    ConfigRoot,

    /// The bin directory.
    BinDirectory,
  };
};

/// Special path enum.
typedef EnumWrapper<SpecialPathEnum> SpecialPath;

/* _________________________________________________________________________

   Callback Interfaces
   _________________________________________________________________________ */

/// Find file callback interface
class
MIKTEXNOVTABLE
IFindFileCallback
{
  /// Request to install package. Called by the FindFile machinery.
  /// @param lpszPackageName Package to be installed.
  /// @return Returns true, if the package has been installed.
public:
  virtual
  bool
  MIKTEXCALL
  InstallPackage (/*[in]*/ const MIKTEXCHAR *	lpszPackageName,
		  /*[in]*/ const MIKTEXCHAR *	lpszTrigger)
    = 0;
};

/// Run process callback interface.
class
MIKTEXNOVTABLE
IRunProcessCallback
{
  /// Output function. Called by the Process object if new output text
  /// is available.
  /// @param pOutput Output text bytes.
  /// @param n Number of output text bytes.
  /// @return Returns true, of the Process object shall continue.
public:
  virtual
  bool
  MIKTEXCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n)
    = 0;

#if 0
  /// Error function. Called by the Process object if new error text
  /// is available.
  /// @param pError Error text bytes.
  /// @param n Number of error text bytes.
  /// @return Returns true, of the Process object shall continue.
public:
  virtual
  bool
  MIKTEXCALL
  OnProcessError (/*[in]*/ const void *	pError,
		  /*[in]*/ size_t	n)
    = 0;
#endif
};

class
MIKTEXNOVTABLE
ICreateFndbCallback
{
public:
  virtual
  bool
  MIKTEXCALL
  ReadDirectory (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		 /*[out]*/ MIKTEXCHAR * *	ppSubDirNames,
		 /*[out]*/ MIKTEXCHAR * *	ppFileNames,
		 /*[out]*/ MIKTEXCHAR * *	ppFileNameInfos)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  OnProgress (/*[in]*/ unsigned			level,
	      /*[in]*/ const MIKTEXCHAR *	lpszDirectory)
    = 0;
};

class
MIKTEXNOVTABLE
IEnumerateFndbCallback
{
public:
  virtual
  bool
  MIKTEXCALL
  OnFndbItem (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	      /*[in]*/ const MIKTEXCHAR *	lpszName,
	      /*[in]*/ const MIKTEXCHAR *	lpszInfo,
	      /*[in]*/ bool			isDirectory)
    = 0;
};

/* _________________________________________________________________________

   FileType
   _________________________________________________________________________ */

/// File type enum class.
class FileTypeEnum
{
public:
  enum EnumType {
    None = 0,
    /// AFM files (*.afm).
    AFM,
    /// METAFONT base files (*.base).
    BASE,
    /// BibTeX database files (*.bib).
    BIB,
    /// BibTeX style files (*.bst).
    BST,
    CWEB,
    DVI,
    DVIPSCONFIG,
    ENC,
    /// Executable files (*.exe).
    EXE,
    /// TeX format files (*.fmt).
    FMT,
    HBF,
    GRAPHICS,
    GF,
    IST,
    MAP,
    /// MetaPost memory files (*.mem).
    MEM,
    MF,
    MFPOOL,
    MFT,
    MISCFONT,
    MP,
    MPPOOL,
    OCP,
    OFM,
    OPL,
    PROGRAMBINFILE,
    PROGRAMTEXTFILE,
    OTP,
    OVF,
    OVP,
    PERLSCRIPT,
    PK,
    PSHEADER,
    TCX,
    /// TeX input files (*.tex).
    TEX,
    /// TeX pool files (*.pool).
    TEXPOOL,
    /// TeX documentation files (*.dvi;*.pdf).
    TEXSYSDOC,
    /// TeX font metric files (*.tfm).
    TFM,
    TTF,
    TYPE1,
    VF,
    WEB,
    WEB2C,
    SCRIPT,
  };
};

/// File type enum.
typedef EnumWrapper<FileTypeEnum> FileType;

/* _________________________________________________________________________

   Cfg
   _________________________________________________________________________ */

/// Configuration settings.
class
MIKTEXNOVTABLE
Cfg
{
public:
  virtual
  void
  MIKTEXCALL
  AddRef ()
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  Release ()
    = 0;

protected:
  virtual
  MIKTEXCALL
  ~Cfg ()
    = 0;

public:
  /// Creates a new Cfg object. The caller is responsible for deleting
  /// the object.
  static
  MIKTEXEXPORT
  Cfg *
  MIKTEXCALL
  Create ();

  /// Tests whether the Cfg object is empty.
public:
  virtual
  bool
  MIKTEXCALL
  Empty ()
    = 0;

  /// Calculates the MD5 value of this Cfg object.
public:
  virtual
  MD5
  MIKTEXCALL
  GetDigest ()
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  TryGetOriginalDigest (/*[out]*/ MD5 & originalDigest)
    = 0;

  /// Gets a configuration value.
public:
  virtual
  tstring
  MIKTEXCALL
  GetValue (/*[in]*/ const MIKTEXCHAR *	lpszKey,
	    /*[in]*/ const MIKTEXCHAR *	lpszValueName)
    = 0;

  /// Gets a configuration value.
public:
  virtual
  bool
  MIKTEXCALL
  TryGetValue (/*[in]*/ const MIKTEXCHAR *	lpszKey,
	       /*[in]*/ const MIKTEXCHAR *	lpszValueName,
	       /*[out]*/ MIKTEXCHAR *		lpszValue,
	       /*[in,out]*/ size_t &		bufSize)
    = 0;
  
  /// Gets a configuration value.
public:
  virtual
  bool
  MIKTEXCALL
  TryGetValue (/*[in]*/ const MIKTEXCHAR *	lpszKey,
	       /*[in]*/ const MIKTEXCHAR *	lpszValueName,
	       /*[out]*/ tstring &		value)
    = 0;

  /// Gets a configuration value.
public:
  virtual
  bool
  MIKTEXCALL
  TryGetValue (/*[in]*/ const MIKTEXCHAR *	lpszKey,
	       /*[in]*/ const MIKTEXCHAR *	lpszValueName,
	       /*[out]*/ PathName &	path)
    = 0;

  /// Marks this Cfg object as modified.
public:
  virtual
  void
  MIKTEXCALL
  SetModified (/*[in]*/ bool b)
    = 0;

  /// Tests whether this Cfg object has been modified.
public:
  virtual
  bool
  MIKTEXCALL
  IsModified ()
    = 0;

  /// Sets a configuration value.
public:
  virtual
  void
  MIKTEXCALL
  PutValue (/*[in]*/ const MIKTEXCHAR *	lpszKey,
	    /*[in]*/ const MIKTEXCHAR *	lpszValueName,
	    /*[in]*/ const MIKTEXCHAR *	lpszValue)
    = 0;
    
  /// Reads configuration values from a file.
public:
  virtual
  void
  MIKTEXCALL
  Read (/*[in]*/ const PathName &	path)
    = 0;

  /// Write configuration settings into a file.
public:
  virtual
  void
  MIKTEXCALL
  Write (/*[in]*/ const PathName & 	path)
    = 0;

  /// Starts a key iteration.
public:
  virtual
  MIKTEXCHAR *
  MIKTEXCALL
  FirstKey (/*[out]*/ MIKTEXCHAR *	lpsz,
	    /*[in]*/ size_t		bufSize)
    = 0;

  /// Continues a key iteration.
public:
  virtual
  MIKTEXCHAR *
  MIKTEXCALL
  NextKey (/*[out]*/ MIKTEXCHAR *	lpsz,
	   /*[in]*/ size_t	bufSize)
    = 0;

  /// Deletes a key.
public:
  virtual
  void
  MIKTEXCALL
  DeleteKey (/*[in]*/ const MIKTEXCHAR * lpszKey)
    = 0;
};				// class Cfg

/* _________________________________________________________________________

   FileAttributes
   _________________________________________________________________________ */

/// File attributes enum class.
class FileAttributesEnum
{
public:
  enum EnumType {
    Normal = 0,
    /// File is a directory.
    Directory = 1,
    /// File is read-only.
    ReadOnly = 2,
#if defined(MIKTEX_WINDOWS)
    /// File is hidden.
    Hidden = 4,
#endif
  };
};

/// File attributes enum.
typedef EnumWrapper<FileAttributesEnum> FileAttributes;

/* _________________________________________________________________________

   Directory
   _________________________________________________________________________ */

/// Directory class.
class
MIKTEXNOVTABLE
Directory
{
  /// Creates a new directory.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Create (/*[in]*/ const PathName &	path);

  /// Gets the current (working) directory.
public:
  static
  MIKTEXEXPORT
  PathName
  MIKTEXCALL
  GetCurrentDirectory ();

  /// Sets the current (working) directory.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetCurrentDirectory (/*[in]*/ const PathName &	path);

  /// Deletes a directory.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Delete (/*[in]*/ const PathName &	path);

  /// Deletes a directory (recursively).
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Delete (/*[in]*/ const PathName &	path,
	  /*[in]*/ bool			recursive);

  /// Tests if a directory exists.
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Exists (/*[in]*/ const PathName &	path);
};

/* _________________________________________________________________________

   SeekOrigin
   _________________________________________________________________________ */

class SeekOriginEnum
{
public:
  enum EnumType {
    Begin,
    Current,
    End,
  };
};

typedef EnumWrapper<SeekOriginEnum> SeekOrigin;

/* _________________________________________________________________________

   StdioFile
   _________________________________________________________________________ */

#if 0
class
StdioFile
{
public:
  StdioFile (/*[in]*/ FILE *	pFile)
    : pFile (pFile),
      owningHandle (true),
      refCount (1)
  {
  }

public:
  MIKTEXEXPORT
  MIKTEXCALL
  ~StdioFile ();

public:
  FILE *
  Get ()
    const
  {
    return (pFile);
  }

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  AddRef ();

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Release ();

private:
  FILE * pFile;

private:
  mutable int refCount;

private:
  bool owningHandle;
};
#endif

/* _________________________________________________________________________

   FileStream
   _________________________________________________________________________ */

/// FileStream class. Encapsulates a stdio file stream pointer (FILE*).
class
FileStream
{
public:
  FileStream ()
    : pFile (0)
  {
  }

public:
  FileStream (/*[in]*/ FILE * pFile)
    : pFile (pFile)
  {
  }

public:
  MIKTEXEXPORT
  MIKTEXCALL
  ~FileStream ();

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Attach (/*[in]*/ FILE * pFile);

  /// Closes the encapsulated stream (excetp for stdin/stdout/stderr).
public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Close ();

public:
  MIKTEXEXPORT
  size_t
  MIKTEXCALL
  Read (/*[out]*/ void *	pBytes,
	/*[in]*/ size_t		count)
    const;

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Write (/*[in]*/ const void *		pBytes,
	 /*[in]*/ size_t		count)
    const;

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Seek (/*[in]*/ long		offset,
	/*[in]*/ SeekOrigin	origin)
    const;

public:
  MIKTEXEXPORT
  long
  MIKTEXCALL
  GetPosition ()
    const;

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetBinary ()
    const;

public:
  FILE *
  Get ()
    const
  {
    return (pFile);
  }

public:
  FILE *
  Detach ()
  {
    FILE * ret = pFile;
    pFile = 0;
    return (ret);
  }
  
private:
  FILE * pFile;
};

/* _________________________________________________________________________

   StreamReader
   _________________________________________________________________________ */

class
StreamReader
{
public:
  MIKTEXEXPORT
  MIKTEXCALL
  StreamReader (/*[in]*/ const PathName & path);

public:
  MIKTEXEXPORT
  MIKTEXCALL
  StreamReader (/*[in]*/ const MIKTEXCHAR * lpszPath);

public:
  StreamReader (/*[in]*/ bool readStdin)
    : stream (readStdin ? stdin : 0)
  {
  }

public:
  MIKTEXEXPORT
  MIKTEXCALL
  ~StreamReader ();

public:
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  ReadLine (/*[out]*/ tstring & line);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Close ();

private:
  FileStream stream;
};
  
/* _________________________________________________________________________

   StreamWriter
   _________________________________________________________________________ */

class
StreamWriter
{
public:
  StreamWriter ()
  {
  }

public:
  MIKTEXEXPORT
  MIKTEXCALL
  StreamWriter (/*[in]*/ const PathName & path);

public:
  StreamWriter (/*[in]*/ FILE * pFile)
    : stream (pFile)
  {
  }

public:
  void
  Attach (/*[in]*/ FILE * pFile)
  {
    stream.Attach (pFile);
  }

public:
  bool
  IsOpen ()
    const
  {
    return (stream.Get() != 0);
  }

public:
  MIKTEXEXPORT
  MIKTEXCALL
  ~StreamWriter ();

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Write (/*[in]*/ MIKTEXCHAR ch);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Write (/*[in]*/ const tstring & line);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  WriteLine (/*[in]*/ const tstring & line);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  WriteLine ();

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  WriteFormatted (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		  /*[in]*/			...);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  WriteFormattedLine (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		      /*[in]*/				...);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Close ();

private:
  FileStream stream;
};
  
/* _________________________________________________________________________

   File
   _________________________________________________________________________ */

/// File class.
class
MIKTEXNOVTABLE
File
{
  /// Deletes a file.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Delete (/*[in]*/ const PathName &	path);
  
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Delete (/*[in]*/ const PathName &	path,
	  /*[in]*/ bool			tryHarder);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Delete (/*[in]*/ const PathName &	path,
	  /*[in]*/ bool			tryHarder,
	  /*[in]*/ bool			updateFndb);

  /// Tests if a file exists.
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Exists (/*[in]*/ const PathName &	path);
    
public:
  static
  MIKTEXEXPORT
  FileAttributes
  MIKTEXCALL
  GetAttributes (/*[in]*/ const PathName &	path);

public:
  static
  MIKTEXEXPORT
  unsigned long
  MIKTEXCALL
  GetNativeAttributes (/*[in]*/ const PathName &	path);

  /// Gets the size of a file.
public:
  static
  MIKTEXEXPORT
  size_t
  MIKTEXCALL
  GetSize (/*[in]*/ const PathName & path);

  /// Renames a file.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Move (/*[in]*/ const PathName &	source,
	/*[in]*/ const PathName &	dest);

  /// Copies a file.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Copy (/*[in]*/ const PathName &	source,
	/*[in]*/ const PathName &	dest);

  /// Copies a file (preserving attributes).
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Copy (/*[in]*/ const PathName &	source,
	/*[in]*/ const PathName &	dest,
	/*[in]*/ bool			preserveAttributes);

  /// Opens a text stream on a file.
public:
  static
  MIKTEXEXPORT
  FILE *
  MIKTEXCALL
  Open (/*[in]*/ const PathName &	path,
	/*[in]*/ FileMode		mode,
	/*[in]*/ FileAccess		access);

  /// Opens a stream on a file.
public:
  static
  MIKTEXEXPORT
  FILE *
  MIKTEXCALL
  Open (/*[in]*/ const PathName &	path,
	/*[in]*/ FileMode		mode,
	/*[in]*/ FileAccess		access,
	/*[in]*/ bool			isTextFile);

  /// Opens a stream on a file.
public:
  static
  MIKTEXEXPORT
  FILE *
  MIKTEXCALL
  Open (/*[in]*/ const PathName &	path,
	/*[in]*/ FileMode		mode,
	/*[in]*/ FileAccess		access,
	/*[in]*/ bool			isTextFile,
	/*[in]*/ FileShare		share);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  PushAside (/*[in,out]*/ PathName &		path,
	     /*[in]*/ const MIKTEXCHAR *	lpszExt,
	     /*[in]*/ bool			updateFndb);

  /// Sets file attributes.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetAttributes (/*[in]*/ const PathName &		path,
		 /*[in]*/ FileAttributes		attributes);

  /// Sets native file attributes.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetNativeAttributes (/*[in]*/ const PathName &	path,
		       /*[in]*/ unsigned long		nativeAttributes);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetTimes (/*[in]*/ FILE *			stream,
	    /*[in]*/ time_t			creationTime,
	    /*[in]*/ time_t			lastAccessTime,
	    /*[in]*/ time_t			lastWriteTime);

  /// Set file time-stamps.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetTimes (/*[in]*/ const PathName &		path,
	    /*[in]*/ time_t			creationTime,
	    /*[in]*/ time_t			lastAccessTime,
	    /*[in]*/ time_t			lastWriteTime);

  /// Gets file time-stamps.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  GetTimes (/*[in]*/ const PathName &		path,
	    /*[out]*/ time_t &			creationTime,
	    /*[out]*/ time_t &			lastAccessTime,
	    /*[out]*/ time_t &			lastWriteTime);

public:
  static
  time_t
  GetLastWriteTime (/*[in]*/ const PathName &		path)
  {
    time_t creationTime;
    time_t lastAccessTime;
    time_t lastWriteTime;
    GetTimes (path, creationTime, lastAccessTime, lastWriteTime);
    return (lastWriteTime);
  }

  /// Reads a file into memory.
  /// @param path File to read.
  /// @return Returns the read bytes.
public:
  static
  MIKTEXEXPORT
  std::vector<unsigned char>
  MIKTEXCALL
  ReadAllBytes (/*[in]*/ const PathName &		path);
};

/* _________________________________________________________________________

   Fndb
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
Fndb
{
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Add (/*[in]*/ const MIKTEXCHAR *	lpszPath);

public:
  static
  void
  Add (/*[in]*/ const PathName &	path)
  {
    Add (path.Get());
  }

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Add (/*[in]*/ const MIKTEXCHAR *	lpszPath,
       /*[in]*/ const MIKTEXCHAR *	lpszFileNameInfo);

public:
  static
  void
  Add (/*[in]*/ const PathName &	path,
       /*[in]*/ const MIKTEXCHAR *	lpszFileNameInfo)
  {
    Add (path.Get(), lpszFileNameInfo);
  }

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Create (/*[in]*/ const MIKTEXCHAR *		lpszFndbPath,
	  /*[in]*/ const MIKTEXCHAR *		lpszRootPath,
	  /*[in]*/ ICreateFndbCallback *	pCallback);

public:
  static
  bool
  Create (/*[in]*/ const PathName &		fndbPath,
	  /*[in]*/ const PathName &		rootPath,
	  /*[in]*/ ICreateFndbCallback *	pCallback)
  {
    return (Create(fndbPath.Get(), rootPath.Get(), pCallback));
  }

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Create (/*[in]*/ const MIKTEXCHAR *		lpszFndbPath,
	  /*[in]*/ const MIKTEXCHAR *		lpszRootPath,
	  /*[in]*/ ICreateFndbCallback *	pCallback,
	  /*[in]*/ bool				enableStringPooling,
	  /*[in]*/ bool				storeFileNameInfo);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Enumerate (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	     /*[in]*/ IEnumerateFndbCallback *	pCallback);
  
public:
  static
  bool
  Enumerate (/*[in]*/ const PathName &		path,
	     /*[in]*/ IEnumerateFndbCallback *	pCallback)
  {
    return (Enumerate(path.Get(), pCallback));
  }
  
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Refresh (/*[in]*/ const PathName &		path,
	   /*[in]*/ ICreateFndbCallback *	pCallback);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Refresh (/*[in]*/ ICreateFndbCallback *	pCallback);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Remove (/*[in]*/ const MIKTEXCHAR *	lpszPath);

public:
  static
  void
  Remove (/*[in]*/ const PathName & path)
  {
    Remove (path.Get());
  }

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  FileExists (/*[in]*/ const PathName &	path);
};

/* _________________________________________________________________________

   DirectoryEntry
   _________________________________________________________________________ */

/// Contains information about a directory entry.
struct DirectoryEntry
{
  /// Name of the entry.
  tstring name;
  /// Indicates whether the entry is a sub-directory.
  bool isDirectory;
};

/* _________________________________________________________________________

   DirectoryEntry2
   _________________________________________________________________________ */

/// Contains detailed information about a directory entry.
struct DirectoryEntry2 : public DirectoryEntry
{
  /// The size (in bytes) of the file.
  size_t size;
};

/* _________________________________________________________________________

   DirectoryLister
   _________________________________________________________________________ */

/// Instances of this class can be used to get the contents of a directory.
class
MIKTEXNOVTABLE
DirectoryLister
{
  /// Destructor.
public:
  virtual
  MIKTEXCALL
  ~DirectoryLister ()
    = 0;

  /// Closes this DirectoryLister.
public:
  virtual
  void
  MIKTEXCALL
  Close ()
    = 0;

  /// Gets the next directory entry.
  /// @param[out] The directory entry to be filled.
  /// @return Returns true, if an entry could be retrieved. Returns, if
  /// there are no more directory entries.
public:
  virtual
  bool
  MIKTEXCALL
  GetNext (/*[out]*/ DirectoryEntry & direntry)
    = 0;
  
  /// Gets the next directory entry.
  /// @param[out] The directory entry to be filled.
  /// @return Returns true, if an entry could be retrieved. Returns, if
  /// there are no more directory entries.
public:
  virtual
  bool
  MIKTEXCALL
  GetNext (/*[out]*/ DirectoryEntry2 & direntry2)
    = 0;
  
  /// Creates a DirectoryLister object. The caller is responsible for deleting
  /// the object.
  /// @param directory Path to the directory.
  /// @return Returns the DirectoryLister object.
public:
  static
  MIKTEXEXPORT
  DirectoryLister *
  MIKTEXCALL
  Open (/*[in]*/ const PathName & directory);

  /// Creates a DirectoryLister object. The caller is responsible for deleting
  /// the object.
  /// @param directory Path to the directory.
  /// @param lpszPattern A filter pattern (e.g. "*.txt").
  /// @return Returns the DirectoryLister object.
public:
  static
  MIKTEXEXPORT
  DirectoryLister *
  MIKTEXCALL
  Open (/*[in]*/ const PathName &	directory,
	/*[in]*/ const MIKTEXCHAR *	lpszPattern);
};

/* _________________________________________________________________________

   MemoryMappedFile
   _________________________________________________________________________ */

/// Instances of this class provide access to memory-mapped files.
class
MIKTEXNOVTABLE
MemoryMappedFile
{
  /// Destructor.
public:
  virtual
  MIKTEXCALL
  ~MemoryMappedFile ()
    = 0;

  /// Maps a file into memory.
  /// @param lpszPath The file to be mapped.
  /// @param readWrite Indicates whether the file should
  /// be open for reading and writing.
  /// @return Returns a pointer to the block of memory.
public:
  virtual
  void *
  MIKTEXCALL
  Open (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	/*[in]*/ bool			readWrite)
    = 0;

public:
  void *
  Open (/*[in]*/ const PathName &	path,
	/*[in]*/ bool			readWrite)
  {
    return (Open(path.Get(), readWrite));
  }

  /// Closes a file mapping.
public:
  virtual
  void
  MIKTEXCALL
  Close ()
    = 0;

  /// Resizes the memory-mapped file.
  /// @param newSize The new size of the memory-mapped file.
  /// @return Returns a pointer to the block of memory.
public:
  virtual
  void *
  MIKTEXCALL
  Resize (/*[in]*/ size_t newSize)
    = 0;

  /// Gets a pointer to rhe block of memory.
  /// @return Returns a pointer to the block of memory.
public:
  virtual
  void *
  MIKTEXCALL
  GetPtr ()
    = 0;

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXCALL
  GetName ()
    = 0;

  /// Gets the size of the memory-mapped file.
  /// @return Returns the size (in bytes).
public:
  virtual
  size_t
  MIKTEXCALL
  GetSize ()
    = 0;

  /// Flushes the memory-mapped file to disk.
public:
  virtual
  void
  MIKTEXCALL
  Flush ()
    = 0;

  /// Creates a new MemoryMappedFile object. The caller is responsible
  /// for deleting the object.
  /// @return Returns the MemoryMappedFile object.
public:
  static
  MIKTEXEXPORT
  MemoryMappedFile *
  MIKTEXCALL
  Create ();
};

/* _________________________________________________________________________

   Thread
   _________________________________________________________________________ */

/// Thread class.
class
MIKTEXNOVTABLE
Thread
{
  /// Destructor.
public:
  virtual
  MIKTEXCALL
  ~Thread ()
    = 0;

  /// Waits for the thread to terminate.
public:
  virtual
  void
  MIKTEXCALL
  Join ()
    = 0;

  /// Creates a new Thread object and starts a thread.
  /// @param function The function to execute in the new thread.
  /// @param argument The argument to be passed to the function.
  /// @return Returns a pointer to the Thread object. The caller is
  /// responsoble for deleting the object.
public:
  static
  MIKTEXEXPORT
  Thread *
  MIKTEXCALL
  Start (/*[in]*/ void (MIKTEXCALLBACK * function) (void *),
	 /*[in]*/ void * argument);

  /// Suspends the running thread.
  /// @param milliseconds Duration (in milliseconds) the thread
  /// should be suspended.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Sleep (/*[in]*/ int milliseconds);

public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  IsCurrentThread (/*[in]*/ Thread * pThread);
};

/* _________________________________________________________________________

   ProcessStartInfo
   _________________________________________________________________________ */

/// Proces start info struct.
struct ProcessStartInfo
{
  /// Arguments to be passed to the process.
  tstring Arguments;
  /// Path name to be executed.
  tstring FileName;
  /// Input file stream for stdin redirection.
  FILE * StandardInput;
  /// Indicates whether error output shall be redirected.
  bool RedirectStandardError;
  /// Indicates whether input shall be redirected.
  bool RedirectStandardInput;
  /// Indicates whether output shall be redirected.
  bool RedirectStandardOutput;
  /// Working directory for the process.
  tstring WorkingDirectory;

  ProcessStartInfo ()
    : RedirectStandardError (false),
      RedirectStandardInput (false),
      RedirectStandardOutput (false),
      StandardInput (0)
  {
  }

  ProcessStartInfo (/*[in]*/ const MIKTEXCHAR * lpszFileName)
    : FileName (lpszFileName),
      RedirectStandardError (false),
      RedirectStandardInput (false),
      RedirectStandardOutput (false),
      StandardInput (0)
  {
  }

};

/* _________________________________________________________________________

   Process
   _________________________________________________________________________ */

/// Process class.
class
MIKTEXNOVTABLE
Process
{
public:
  virtual
  MIKTEXCALL
  ~Process ()
    = 0;

public:
  virtual
  FILE *
  MIKTEXCALL
  get_StandardInput ()
    = 0;

public:
  /// Gets the redirected output stream.
  /// @return Returns the redirected output stream. Returns 0, if
  /// the output was not redirected.
  virtual
  FILE *
  MIKTEXCALL
  get_StandardOutput ()
    = 0;

public:
  virtual
  FILE *
  MIKTEXCALL
  get_StandardError ()
    = 0;

  /// Waits for the process.
public:
  virtual
  void
  MIKTEXCALL
  WaitForExit ()
    = 0;

  /// Waits for the process.
public:
  virtual
  bool
  MIKTEXCALL
  WaitForExit (/*[in]*/ int milliseconds)
    = 0;

  /// Gets the exit code of the process.
  /// The process must have exited.
public:
  virtual
  int
  MIKTEXCALL
  get_ExitCode ()
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  Close ()
    = 0;

  /// Start the system shell to execute a command.
  /// @param lpszCommandLine Command to be executed.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  StartSystemCommand (/*[in]*/ const MIKTEXCHAR *	lpszCommandLine);

  /// Executes the system shell to execute a command.
  /// @param lpszCommandLine Command to be executed.
  /// @return Returns true, if the command exited successfully.
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  ExecuteSystemCommand (/*[in]*/ const MIKTEXCHAR *	lpszCommandLine);

  /// Executes the system shell to execute a command.
  /// @param lpszCommandLine Command to be executed.
  /// @param pCallback Callback interface.
  /// @param pExitCode To be filled with the exit code of the command.
  /// @param lpszWorkingDirectory Working directory for the command.
  /// @return Returns true, if the process exited successfully, or
  /// if pExitCode isn't null.
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  ExecuteSystemCommand (/*[in]*/ const MIKTEXCHAR *	lpszCommandLine,
			/*[out]*/ int *			pExitCode);

  /// Executes the system shell to execute a command.
  /// @param lpszCommandLine Command to be executed.
  /// @param pExitCode To be filled with the exit code of the command.
  /// @param pCallback Callback interface.
  /// @param lpszWorkingDirectory Working directory for the command.
  /// @return Returns true, if the process exited successfully, or
  /// if pExitCode isn't null.
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  ExecuteSystemCommand (/*[in]*/ const MIKTEXCHAR *	lpszCommandLine,
			/*[out]*/ int *			pExitCode,
			/*[in]*/ IRunProcessCallback *	pCallback,
			/*[in]*/ const MIKTEXCHAR *	lpszWorkingDirectory);

  /// Executes a process.
  /// @param lpszFileName The name of an executable file to run in the process.
  /// @param lpszArguments The command-line arguments to pass when starting
  /// the process.
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Run (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
       /*[in]*/ const MIKTEXCHAR *	lpszArguments);

public:
  static
  void
  Run (/*[in]*/ const PathName &	fileName,
       /*[in]*/ const MIKTEXCHAR *	lpszArguments)
  {
    Run (fileName.Get(), lpszArguments);
  }
  
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Run (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
       /*[in]*/ const MIKTEXCHAR *	lpszArguments,
       /*[int]*/ IRunProcessCallback *	pCallback,
       /*[out]*/ int *			pExitCode,
       /*[in]*/  const MIKTEXCHAR *	lpszWorkingDirectory);

public:
  static
  void
  Run (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
       /*[in]*/ const MIKTEXCHAR *	lpszArguments,
       /*[int]*/ IRunProcessCallback *	pCallback)
  {
    Run (lpszFileName, lpszArguments, pCallback, 0, 0);
  }

public:
  static
  void
  Run (/*[in]*/ const PathName &	fileName,
       /*[in]*/ const MIKTEXCHAR *	lpszArguments,
       /*[int]*/ IRunProcessCallback *	pCallback)
  {
    Run (fileName.Get(), lpszArguments, pCallback, 0, 0);
  }

public:
  static
  bool
  Run (/*[in]*/ const PathName &	fileName,
       /*[in]*/ const MIKTEXCHAR *	lpszArguments,
       /*[int]*/ IRunProcessCallback *	pCallback,
       /*[out]*/ int *			pExitCode,
       /*[in]*/  const MIKTEXCHAR *	lpszWorkingDirectory)
  {
    return (Run(fileName.Get(),
		lpszArguments,
		pCallback,
		pExitCode,
		lpszWorkingDirectory));
  }
  
public:
  static
  MIKTEXEXPORT
  bool
  MIKTEXCALL
  Run (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
       /*[in]*/ const MIKTEXCHAR *	lpszArguments,
       /*[out]*/ void *			pBuf,
       /*[in,out]*/ size_t *		pBufSize,
       /*[out]*/ int *			pExitCode);

public:
  static
  bool
  Run (/*[in]*/ const PathName &	fileName,
       /*[in]*/ const MIKTEXCHAR *	lpszArguments,
       /*[out]*/ void *			pBuf,
       /*[in,out]*/ size_t *		pBufSize,
       /*[out]*/ int *			pExitCode)
  {
    return (Run(fileName.Get(),
		lpszArguments,
		pBuf,
		pBufSize,
		pExitCode));
  }

public:
  static
  MIKTEXEXPORT
  Process *
  MIKTEXCALL
  Start (/*[in]*/ const ProcessStartInfo & startinfo);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Start (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	 /*[in]*/ const MIKTEXCHAR *	lpszArguments,
	 /*[in]*/ FILE *		pFileStandardInput,
	 /*[out]*/ FILE **		ppFileStandardInput,
	 /*[out]*/ FILE **		ppFileStandardOutput,
	 /*[out]*/ FILE **		ppFileStandardError,
	 /*[in]*/ const MIKTEXCHAR *	lpszWorkingDirectory);

public:
  static
  void
  Start (/*[in]*/ const PathName &	fileName,
	 /*[in]*/ const MIKTEXCHAR *	lpszArguments,
	 /*[in]*/ FILE *		pFileStandardInput,
	 /*[out]*/ FILE **		ppFileStandardInput,
	 /*[out]*/ FILE **		ppFileStandardOutput,
	 /*[out]*/ FILE **		ppFileStandardError,
	 /*[in]*/ const MIKTEXCHAR *	lpszWorkingDirectory)
  {
    Start (fileName.Get(),
	   lpszArguments,
	   pFileStandardInput,
	   ppFileStandardInput,
	   ppFileStandardOutput,
	   ppFileStandardError,
	   lpszWorkingDirectory);
  }

public:
  static
  void
  Start (/*[in]*/ const PathName &	fileName)
  {
    Start (fileName.Get(),
	   0,
	   0,
	   0,
	   0,
	   0,
	   0);
  }

public:
  static
  void
  Start (/*[in]*/ const PathName &	fileName,
	 /*[in]*/ const MIKTEXCHAR *	lpszArguments)
  {
    Start (fileName.Get(),
	   lpszArguments,
	   0,
	   0,
	   0,
	   0,
	   0);
  }

};

/* _________________________________________________________________________

   ScratchDirectory
   _________________________________________________________________________ */

class
ScratchDirectory
{
public:
  MIKTEXEXPORT
  MIKTEXCALL
  ScratchDirectory ();

public:
  virtual
  MIKTEXEXPORT
  MIKTEXCALL
  ~ScratchDirectory ();

public:
  static
  MIKTEXEXPORT
  PathName &
  MIKTEXCALL
  Create (/*[out]*/ PathName &	path);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Enter (/*[in]*/ const MIKTEXCHAR *	lpszPrefix);

public:
  void
  Enter ()
  {
    Enter (MIKTEXTEXT("mik"));
  }

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Leave ();

private:
  bool entered;
};

/* _________________________________________________________________________

   TraceStream
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
TraceStream
{
public:
  virtual
  MIKTEXCALL
  ~TraceStream ()
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  Close ()
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  Enable (/*[in]*/ bool		enable)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  IsEnabled ()
    = 0;

public:
  static
  MIKTEXEXPORT
  TraceStream *
  MIKTEXCALL
  Open (/*[in]*/ const MIKTEXCHAR *	lpszName);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetTraceFlags (/*[in]*/ const MIKTEXCHAR *	lpszOptions);

public:
  virtual
  void
  MIKTEXCALL
  Write (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
	 /*[in]*/ const MIKTEXCHAR *	lpszText)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  WriteLine (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
	     /*[in]*/ const MIKTEXCHAR *	lpszText)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  WriteFormattedLine (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
		      /*[in]*/ const MIKTEXCHAR *	lpszFormat,
		      /*[in]*/				...)
    = 0;

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  TraceLastCRTError (/*[in]*/ const MIKTEXCHAR *	lpszCrtFunction,
		     /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		     /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		     /*[in]*/ int			lpszSourceLine);

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  TraceLastWin32Error (/*[in]*/ const MIKTEXCHAR *	lpszWindowsFunction,
		       /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		       /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		       /*[in]*/ int			sourceLine);
#endif

public:
  virtual
  void
  MIKTEXCALL
  VTrace (/*[in]*/ const MIKTEXCHAR *	lpszFacility,
	  /*[in]*/ const MIKTEXCHAR *	lpszFormat,
	  /*[in]*/ va_list		arglist)
    = 0;
};

/* _________________________________________________________________________

   FileTypeInfo
   _________________________________________________________________________ */

struct FileTypeInfo
{
  FileTypeInfo ()
    : fileType (FileType::None)
  {
  }

  FileType fileType;
  tstring fileTypeString;
  tstring fileNameExtensions;
  tstring applicationName;
  tstring searchPath;
  tstring envVarNames;
};

/* _________________________________________________________________________

   FileInfoRecord
   _________________________________________________________________________ */

struct FileInfoRecord
{
  FileInfoRecord ()
    : access (FileAccess::None)
  {
  }
  tstring	fileName;
  tstring	packageName;
  FileAccess	access;
};

/* _________________________________________________________________________

   Session
   _________________________________________________________________________ */

/// The MiKTeX session interface.
class
MIKTEXNOVTABLE
Session
{
public:
  /// Init flags enum class.
  class InitFlagsEnum
  {
  public:
    enum EnumType {
      None,
      /// No config files will be loaded.
      NoConfigFiles = 1,
    };
  };

  /// Init flags enum.
  typedef EnumWrapper<InitFlagsEnum> InitFlags;

  /// Init info struct.
public:
  class InitInfo
  {
    /// Initializes a new init info struct.
  public:
    InitInfo ()
      : flags (InitFlags::None)
    {
    }
    /// Initializes a new init info struct.
    /// @param lpszProgramInvocationName Name of the invoked program.
  public:
    InitInfo (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName)
      : flags (InitFlags::None),
	programInvocationName (lpszProgramInvocationName)
    {
    }
    /// Initializes a new init info struct.
    /// @param lpszProgramInvocationName Name of the invoked program.
    /// @param flags Init flags.
  public:
    InitInfo (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
	      /*[in]*/ unsigned long		flags)
      : flags (flags),
	programInvocationName (lpszProgramInvocationName)
    {
    }
    /// Sets init flags.
  public:
    void
    SetFlags (/*[in]*/ unsigned long	flags)
    {
      this->flags = flags;
    }
    /// Gets init flags.
  public:
    unsigned long
    GetFlags ()
      const
    {
      return (flags);
    }
    /// Sets the name of the invoked program.
  public:
    void
    SetProgramInvocationName
    (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
    {
      programInvocationName = lpszProgramInvocationName;
    }
    /// Gets the name of the invoked program.
  public:
    tstring
    GetProgramInvocationName ()
      const
    {
      return (programInvocationName);
    }
  public:
    void
    SetStartupConfig (/*[in]*/ const StartupConfig & startupConfig)
    {
      this->startupConfig = startupConfig;
    }
  public:
    StartupConfig
    GetStartupConfig ()
      const
    {
      return (startupConfig);
    }
    /// Sets the bin directory.
  public:
    void
    SetBinDirectory (/*[out]*/ const MIKTEXCHAR * lpszBinDirectory)
    {
      binDirectory = lpszBinDirectory;
    }
    /// Gets the bin directory.
  public:
    PathName
    GetBinDirectory ()
      const
    {
      return (binDirectory);
    }
    /// Enables trace streams.
  public:
    void
    SetTraceFlags (/*[out]*/ const MIKTEXCHAR * lpszTraceFlags)
    {
      traceFlags = lpszTraceFlags;
    }
    /// Gets enabled trace streams.
  public:
    tstring
    GetTraceFlags ()
      const
    {
      return (traceFlags);
    }
  private:
    unsigned long flags;
  private:
    tstring programInvocationName;
  private:
    StartupConfig startupConfig;
  private:
    PathName binDirectory;
  private:
    tstring traceFlags;
  };

public:
  static
  MIKTEXEXPORT
  Session *
  MIKTEXCALL
  Get ();

public:
  static
  MIKTEXEXPORT
  Session *
  MIKTEXCALL
  TryGet ();

public:
  static
  MIKTEXEXPORT
  Session *
  MIKTEXCALL
  Get (/*[in]*/ const InitInfo & initInfo);

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Release (/*[in]*/ Session * pSession);

protected:
  virtual
  MIKTEXCALL
  ~Session ()
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  PushAppName (/*[in]*/ const MIKTEXCHAR *	lpszAppName)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  PushBackAppName (/*[in]*/ const MIKTEXCHAR *	lpszAppName)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  AddWorkingDirectory (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		       /*[in]*/ bool			bAtEnd)
    = 0;

  /// Gets a special path.
  /// @param specialPath Specifies special path.
  /// @return Returns a path name.
public:
  virtual
  PathName
  MIKTEXCALL
  GetSpecialPath (/*[in]*/ SpecialPath		specialPath)
    = 0;

  /// Gets the number of root directories.
public:
  virtual
  unsigned
  MIKTEXCALL
  GetNumberOfTEXMFRoots ()
    = 0;

public:
  virtual
  PathName
  MIKTEXCALL
  GetRootDirectory (/*[in]*/ unsigned	r)
    = 0;

  /// Gets the path name of the MPM file name database.
public:
  virtual
  PathName
  MIKTEXCALL
  GetMpmDatabasePathName ()
    = 0;

public:
  virtual
  unsigned
  MIKTEXCALL
  DeriveTEXMFRoot (/*[in]*/ const PathName & path)
    = 0;

  /// Finds a file name database file.
  /// @param r File name database ID.
  /// @param path Path name object buffer to be filled.
public:
  virtual
  bool
  MIKTEXCALL
  FindFilenameDatabase (/*[in]*/ unsigned	r,
			/*[out]*/ PathName &	path)
    = 0;

  /// Gets the path name of a file name database file.
  /// @param r File name database ID.
  /// @param path Path name object buffer to be filled.
public:
  virtual
  PathName
  MIKTEXCALL
  GetFilenameDatabasePathName (/*[in]*/ unsigned	r)
    = 0;

  /// Unloads the file name database.
  /// @return Returns true, if the file name database could be unloaded.
  /// Returns false, if the database is still in use.
public:
  virtual
  bool
  MIKTEXCALL
  UnloadFilenameDatabase ()
    = 0;

  /// Splits a TEXMF path name.
  /// @param lpszPath Path name to split.
  /// @param lpszRoot String buffer to be filled with the root directory.
  /// @param lpszRelativ String buffer to be filled with the relative path.
  /// @return Returns the TEXMF ID.
public:
  virtual
  unsigned
  MIKTEXCALL
  SplitTEXMFPath (/*[in]*/ const PathName &	path,
		  /*[out]*/ PathName &		root,
		  /*[out]*/ PathName &		relative)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  RegisterRootDirectories (/*[in]*/ const tstring &	roots)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  RegisterRootDirectories (/*[in]*/ const StartupConfig &	startupConfig,
			   /*[in]*/ bool			sessionOnly)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  IsMiKTeXDirect ()
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  GetMETAFONTMode (/*[in]*/ unsigned		idx,
		   /*[out]*/ MIKTEXMFMODE *	pMode)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  DetermineMETAFONTMode (/*[in]*/ unsigned		dpi,
			 /*[out]*/ MIKTEXMFMODE *	pMode)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  TryGetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		     /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		     /*[out]*/ tstring &		value)
    = 0;

public:
  virtual
  tstring
  MIKTEXCALL
  GetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ const MIKTEXCHAR *	lpszDefaultValue)
    = 0;

public:
  virtual
  int
  MIKTEXCALL
  GetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ int			defaultValue)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  GetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ bool			defaultValue)
    = 0;

public:
  virtual
  TriState
  MIKTEXCALL
  GetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ TriState		defaultValue)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  SetUserConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		      /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		      /*[in]*/ const MIKTEXCHAR *	lpszValue)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  SetUserConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		      /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		      /*[in]*/ bool			value)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  SetUserConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		      /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		      /*[in]*/ int			value)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  SharedMiKTeXSetup (/*[in]*/ bool shared)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  SharedMiKTeXSetup (/*[in]*/ bool shared,
		     /*[in]*/ bool sessionOnly)
    = 0;

public:
  virtual
  FILE *
  MIKTEXCALL
  OpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile)
    = 0;

public:
  virtual
  FILE *
  MIKTEXCALL
  TryOpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	       /*[in]*/ FileMode		mode,
	       /*[in]*/ FileAccess		access,
	       /*[in]*/ bool			isTextFile)
    = 0;

public:
  virtual
  FILE *
  MIKTEXCALL
  OpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile,
	    /*[in]*/ FileShare		share)
    = 0;

public:
  virtual
  FILE *
  MIKTEXCALL
  TryOpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	       /*[in]*/ FileMode		mode,
	       /*[in]*/ FileAccess		access,
	       /*[in]*/ bool			isTextFile,
	       /*[in]*/ FileShare		share)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  CloseFile (/*[in]*/ FILE *	pFile)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  IsOutputFile (/*[in]*/ const FILE *	pFile)
    = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXCALL
  IsFileAlreadyOpen (/*[in]*/ const MIKTEXCHAR *	lpszFileName)
    = 0;
#endif

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  void
  MIKTEXCALL
  ScheduleFileRemoval (/*[in]*/ const MIKTEXCHAR *	lpszFileName)
    = 0;
#endif

public:
  virtual
  bool
  MIKTEXCALL
  StartFileInfoRecorder (/*[in]*/ bool bRecordPackageNames)
    = 0;

public:
  virtual
  std::vector<FileInfoRecord>
  MIKTEXCALL
  GetFileInfoRecords ()
    = 0;

public:
  virtual
  FileType
  MIKTEXCALL
  DeriveFileType (/*[in]*/ const MIKTEXCHAR * lpszPath)
    = 0;

  /// Finds a file.
  /// @param lpszFileName Name of the file to be found.
  /// @param lpszPathList Search path. See the MiKTeX manual.
  /// @param[out] path The path name to be filled with the result.
public:
  virtual
  bool
  MIKTEXCALL
  FindFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	    /*[in]*/ const MIKTEXCHAR *	lpszPathList,
	    /*[out]*/ PathName &	path)
    = 0;

public:
  bool
  FindFile (/*[in]*/ const PathName &	fileName,
	    /*[in]*/ const MIKTEXCHAR *	lpszPathList,
	    /*[out]*/ PathName &	path)
  {
    return (FindFile(fileName.Get(), lpszPathList, path));
  }

  /// Finds a file.
  /// @param lpszFileName Name of the file to be found.
  /// @param lpszPathList Type of the file.
  /// @param[out] path The path name to be filled with the result.
public:
  virtual
  bool
  MIKTEXCALL
  FindFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	    /*[in]*/ FileType		fileType,
	    /*[out]*/ PathName &	path)
    = 0;

public:
  bool
  FindFile (/*[in]*/ const PathName &	fileName,
	    /*[in]*/ FileType		fileType,
	    /*[out]*/ PathName &	path)
  {
    return (FindFile(fileName.Get(), fileType, path));
  }

public:
  virtual
  bool
  MIKTEXCALL
  FindPkFile (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
	      /*[in]*/ const MIKTEXCHAR *	lpszMode,
	      /*[in]*/ int			dpi,
	      /*[out]*/ PathName &		path)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  FindTfmFile (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
	       /*[out]*/ PathName &		path,
	       /*[in]*/ bool			create)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  SetFindFileCallback (/*[in]*/ IFindFileCallback *	pCallback)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  SplitFontPath (/*[in]*/ const MIKTEXCHAR *	lpszFontPath,
		 /*[out]*/ MIKTEXCHAR *		lpszFontType,
		 /*[out]*/ MIKTEXCHAR *		lpszSupplier,
		 /*[out]*/ MIKTEXCHAR *		lpszTypeface,
		 /*[out]*/ MIKTEXCHAR *		lpszFontName,
		 /*[out]*/ MIKTEXCHAR *		lpszPointSize)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  GetFontInfo (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
	       /*[out]*/ MIKTEXCHAR *		lpszSupplier,
	       /*[out]*/ MIKTEXCHAR *		lpszTypeface,
	       /*[out]*/ double *		lpGenSize)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  GetGhostscript (/*[out]*/ MIKTEXCHAR *	lpszPath,
		  /*[out]*/ unsigned long *	pVersionNumber)
    = 0;

public:
  virtual
  tstring
  MIKTEXCALL
  GetExpandedSearchPath (/*[in]*/ FileType	fileType)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  FindGraphicsRule (/*[in]*/ const MIKTEXCHAR *	lpszFrom,
		    /*[in]*/ const MIKTEXCHAR *	lpszTo,
		    /*[out]*/ MIKTEXCHAR *	lpszRule,
		    /*[in]*/ size_t	bufSize)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  ConvertToBitmapFile (/*[in]*/ const MIKTEXCHAR *	lpszSourceFileName,
		       /*[out]*/ MIKTEXCHAR *		lpszDestFileName,
		       /*[in]*/ IRunProcessCallback *	pCallback)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  EnableFontMaker (/*[in]*/ bool enable)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  GetMakeFontsFlag ()
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  MakeMakePkCommandLine (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
			 /*[in]*/ int			dpi,
			 /*[in]*/ int			baseDpi,
			 /*[in]*/ const MIKTEXCHAR *	lpszMfMode,
			 /*[out]*/ PathName &		fileName,
			 /*[out]*/ MIKTEXCHAR *		lpszArguments,
			 /*[in]*/ size_t		bufSize)
    = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  int
  MIKTEXCALL
  RunBatch (/*[in]*/ int			argc,
	    /*[in]*/ const MIKTEXCHAR **	argv)
    = 0;
#endif

public:
  virtual
  int
  MIKTEXCALL
  RunPerl (/*[in]*/ int			argc,
	   /*[in]*/ const MIKTEXCHAR **	argv)
    = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXCALL
  ShowManualPageAndWait (/*[in]*/ HWND		hWnd,
			 /*[in]*/ unsigned long	topic)
    = 0;
#endif

public:
  virtual
  bool
  MIKTEXCALL
  GetNextFileTypeInfo (/*[in]*/ unsigned	index,
		       /*[out]*/ FileTypeInfo &	fileTypeInfo)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  GetFormatInfo (/*[in]*/ unsigned	index,
		 /*[out]*/ FormatInfo &	formatInfo)
    = 0;

public:
  virtual
  FormatInfo
  MIKTEXCALL
  GetFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszFormatName)
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  TryGetFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszFormatName,
		    /*[out]*/ FormatInfo &	formatInfo)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  DeleteFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszFormatName)
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  SetFormatInfo (/*[in]*/ const FormatInfo &	formatInfo)
    = 0;

public:
  virtual
  PathName
  MIKTEXCALL
  GetMyLocation ()
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  RunningAsAdministrator ()
    = 0;

public:
  virtual
  TriState
  MIKTEXCALL
  IsSharedMiKTeXSetup ()
    = 0;

public:
  virtual
  bool
  MIKTEXCALL
  GetPaperSizeInfo (/*[in]*/ int		idx,
		    /*[out]*/ PaperSizeInfo &	paperSize)
    = 0;

public:
  virtual
  PaperSizeInfo
  MIKTEXCALL
  GetPaperSizeInfo (/*[in]*/ const MIKTEXCHAR *	lpszDvipsName)
    = 0;

public:
  virtual
  PolicyFlags
  MIKTEXCALL
  GetPolicyFlags ()
    = 0;

public:
  virtual
  void
  MIKTEXCALL
  SetDefaultPaperSize (/*[in]*/ const MIKTEXCHAR * lpszDvipsName)
    = 0;

public:
  static
  MIKTEXEXPORT
  void
  MIKTEXNORETURN
  MIKTEXCALL
  FatalCrtError (/*[in]*/ const MIKTEXCHAR *	lpszCrtFunction,
		 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		 /*[in]*/ int			lpszSourceLine);
  
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXNORETURN
  MIKTEXCALL
  FatalCrtError (/*[in]*/ const MIKTEXCHAR *	lpszCrtFunction,
		 /*[in]*/ int			errorCode,
		 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		 /*[in]*/ int			lpszSourceLine);
  
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXNORETURN
  MIKTEXCALL
  FatalMiKTeXError (/*[in]*/ const MIKTEXCHAR *	lpszMiktexFunction,
		    /*[in]*/ const MIKTEXCHAR *	lpszMessage,
		    /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		    /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		    /*[in]*/ int		lpszSourceLine);

#if defined(MIKTEX_WINDOWS)  
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXNORETURN
  MIKTEXCALL
  FatalWindowsError (/*[in]*/ const MIKTEXCHAR *	lpszWindowsFunction,
		     /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		     /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		     /*[in]*/ int			lpszSourceLine);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXEXPORT
  void
  MIKTEXNORETURN
  MIKTEXCALL
  FatalWindowsError (/*[in]*/ const MIKTEXCHAR *	lpszWindowsFunction,
		     /*[in]*/ unsigned long		errorCode,
		     /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		     /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		     /*[in]*/ int			lpszSourceLine);
#endif
};

/* _________________________________________________________________________

   SessionWrapper
   _________________________________________________________________________ */

class
SessionWrapper
{
public:
  SessionWrapper ()
    : pSession (0)
  {
  }

public:
  SessionWrapper (/*[in]*/ bool useExisting)
    : pSession (Session::Get())
  {
    useExisting;
    assert (useExisting);
  }

public:
  SessionWrapper (/*[in]*/ const Session::InitInfo & initInfo)
    : pSession (Session::Get(initInfo))
  {
  }

public:
  ~SessionWrapper ()
  {
    try
      {
	Reset ();
      }
    catch (const std::exception &)
      {
      }
  }

public:
  void
  CreateSession (/*[in]*/ const Session::InitInfo & initInfo)
  {
    pSession = Session::Get(initInfo);
  }

public:
  void
  Reset ()
  {
    if (pSession != 0)
      {
	Session::Release (pSession);
	pSession = 0;
      }
  }

public:
  Session *
  GetSession ()
    const
  {
    return (pSession);
  }

public:
  Session *
  operator-> ()
    const
  {
    return (GetSession());
  }

  // unimplemented
private:
  SessionWrapper (/*[in]*/ const SessionWrapper &);

  // unimplemented
private:
  SessionWrapper &
  operator= (/*[in]*/ const SessionWrapper &);

private:
  Session * pSession;
};

/* _________________________________________________________________________

   CSVList: Comma Separated Value List
   _________________________________________________________________________ */

class CSVList
{
public:
  MIKTEXEXPORT
  MIKTEXCALL
  CSVList ();

public:
  MIKTEXEXPORT
  MIKTEXCALL
  CSVList (/*[in]*/ const MIKTEXCHAR *	lpszValueList,
	   /*[in]*/ MIKTEXCHARINT	separator);

public:
  MIKTEXEXPORT
  MIKTEXCALL
  ~CSVList ();

public:
  MIKTEXEXPORT
  const MIKTEXCHAR *
  MIKTEXCALL
  operator++ ();

public:
  const MIKTEXCHAR *
  GetCurrent ()
    const
  {
    return (lpszCurrent);
  }

private:
  CSVList (/*[in]*/ const CSVList & rhs);

private:
  CSVList &
  operator= (/*[in]*/ const CSVList & rhs);

private:
  MIKTEXCHAR separator;

private:
  MIKTEXCHAR * lpszList;

private:
  MIKTEXCHAR * lpszNext;

private:
  MIKTEXCHAR * lpszCurrent;
};

/* _________________________________________________________________________

   CriticalSectionMonitor
   _________________________________________________________________________ */

class CriticalSectionMonitor
{
public:
  CriticalSectionMonitor ()
    : autoInit (true),
      nLocks (0)
  {
    Initialize_ ();
  }

public:
  CriticalSectionMonitor (/*[in]*/ bool autoInit)
    : autoInit (autoInit),
      nLocks (0)
  {
    if (autoInit)
      {
	Initialize_ ();
      }
  }

public:
  ~CriticalSectionMonitor ()
  {
    try
      {
	if (autoInit)
	  {
	    Delete_ ();
	  }
      }
    catch (const std::exception &)
      {
      }
  }

public:
  void
  Initialize ()
  {
    assert (! autoInit);
    Initialize_ ();
  }

private:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Initialize_ ();

public:
  void
  Delete ()
  {
    assert (! autoInit);
    Delete_ ();
  }
  
private:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Delete_ ();

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Enter ();

public:
  void
  Lock ()
  {
    Enter ();
  }

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  Leave ();

public:
  void
  Unlock ()
  {
    Leave ();
  }

public:
  bool
  IsLocked ()
    const
  {
    return (nLocks > 0);
  }

private:
  CriticalSectionMonitor (/*[in]*/ const CriticalSectionMonitor & rhs);

private:
  CriticalSectionMonitor &
  operator= (/*[in]*/ const CriticalSectionMonitor & rhs);

#if defined(MIKTEX_WINDOWS)
private:
  CRITICAL_SECTION critsec;
#endif

#if defined(HAVE_PTHREAD)
private:
  pthread_mutex_t mutex;
  pthread_mutexattr_t mutexAttr;
#endif

private:
  long volatile nLocks;

private:
  bool autoInit;
};

/* _________________________________________________________________________

   MIKTEX_BEGIN_CRITICAL_SECTION / MIKTEX_END_CRITICAL_SECTION
   _________________________________________________________________________ */

class AutoCriticalSectionMonitor_
{
public:
  AutoCriticalSectionMonitor_ (/*[in]*/ CriticalSectionMonitor * pcritsec)
    : pCriticalSectionMonitor (pcritsec)
  {
    pCriticalSectionMonitor->Enter ();
  }

public:
  ~AutoCriticalSectionMonitor_ ()
  {
    try
      {
	pCriticalSectionMonitor->Leave ();
      }
    catch (const std::exception &)
      {
      }
  }

public:
  CriticalSectionMonitor *
  operator-> ()
    const
  {
    return (pCriticalSectionMonitor);
  }

private:
  CriticalSectionMonitor * pCriticalSectionMonitor;
};

#define MIKTEX_BEGIN_CRITICAL_SECTION(pcritsec)		\
  {							\
    MiKTeX::Core::AutoCriticalSectionMonitor_			\
      miktex_h__criticalSectionMonitor_ (pcritsec);

#define MIKTEX_END_CRITICAL_SECTION()			\
  }

#define MIKTEX_DEFINE_LOCK(name) \
  MiKTeX::Core::CriticalSectionMonitor name##__CriticalSectionMonitor_

#define MIKTEX_LOCK(name)					\
  {								\
    MiKTeX::Core::AutoCriticalSectionMonitor_				\
      auto_critsec_ (&name##__CriticalSectionMonitor_);

#define MIKTEX_UNLOCK()				\
  }

/* _________________________________________________________________________

   Tokenizer
   _________________________________________________________________________ */

class Tokenizer
{
public:
  MIKTEXEXPORT
  MIKTEXCALL
  Tokenizer (/*[in]*/ const MIKTEXCHAR * lpsz,
	     /*[in]*/ const MIKTEXCHAR * lpszDelim);

public:
  MIKTEXEXPORT
  void
  MIKTEXCALL
  SetDelim (const MIKTEXCHAR * lpszDelim);

public:
  const MIKTEXCHAR *
  GetCurrent ()
    const
  {
    return (*lpszCurrent == 0 ? 0 : lpszCurrent);
  }

public:
  MIKTEXEXPORT
  const MIKTEXCHAR *
  MIKTEXCALL
  operator++ ();

public:
  MIKTEXEXPORT
  MIKTEXCALL
  ~Tokenizer ();

private:
  Tokenizer (/*[in]*/ const Tokenizer & rhs);

private:
  Tokenizer &
  operator= (/*[in]*/ const Tokenizer & rhs);

private:
  void
  FindToken ();

private:
  MIKTEXCHAR * lpszOrig;

private:
  const MIKTEXCHAR * lpszCurrent;

private:
  MIKTEXCHAR * lpszNext;

private:
  void * pDelims;
};

/* _________________________________________________________________________

   IsDirectoryDelimiter
   _________________________________________________________________________ */

MIKTEXINLINE
bool
IsDirectoryDelimiter (/*[in]*/ MIKTEXCHARINT ch)
{
  return (PathName::IsDirectoryDelimiter(ch));
}

/* _________________________________________________________________________

   PathNameParser
   _________________________________________________________________________ */

class PathNameParser
{
public:
  MIKTEXEXPORT
  MIKTEXCALL
  PathNameParser (/*[in]*/ const MIKTEXCHAR * lpszPath);

public:
  const MIKTEXCHAR *
  GetCurrent ()
    const
  {
    return (*lpszCurrent == 0 ? 0 : lpszCurrent);
  }

public:
  MIKTEXEXPORT
  const MIKTEXCHAR *
  MIKTEXCALL
  operator++ ();

private:
  PathNameParser (/*[in]*/ const PathNameParser & rhs);

private:
  PathNameParser &
  operator= (/*[in]*/ const PathNameParser & rhs);

private:
  MIKTEXCHAR * lpszCurrent;

private:
  MIKTEXCHAR * lpszNext;

private:
  MIKTEXCHAR buffer[BufferSizes::MaxPath];
};

/* _________________________________________________________________________

   hash_compare_icase
   _________________________________________________________________________ */

class hash_compare_icase
{
#if defined(_MSC_VER)
public:
  enum {
    bucket_size = 4,
    min_buckets = 8
  };
#endif

public:
  // hash algorithm taken from SGI's STL
  size_t
  operator() (/*[in]*/ const tstring & str)
    const
  {
    size_t h = 0;
    for (const MIKTEXCHAR * lpsz = str.c_str(); *lpsz != 0; ++ lpsz)
      {
	MIKTEXCHAR ch = *lpsz;
	assert (static_cast<unsigned>(ch) < 0x80);
	if (ch >= MIKTEXTEXT('a') && ch <= MIKTEXTEXT('z'))
	  {
	    ch = MIKTEXTEXT('A') + (ch - MIKTEXTEXT('a'));
	  }
	h = 5 * h + ch;
      }
    return (h);
  }

public:
  bool
  operator() (/*[in]*/ const tstring & str1,
	      /*[in]*/ const tstring & str2)
    const
  {
    return (StringCompare(str1.c_str(), str2.c_str(), true) < 0);
  }
};

/* _________________________________________________________________________

   hash_compare_path
   _________________________________________________________________________ */

class hash_compare_path
{
#if defined(_MSC_VER)
public:
  enum {
    bucket_size = 4,
    min_buckets = 8
  };
#endif

public:
  // hash algorithm taken from SGI's STL
  size_t
  operator() (/*[in]*/ const tstring & str)
    const
  {
    return (PathName(str).GetHash());
  }
  
public:
  bool
  operator() (/*[in]*/ const tstring & str1,
	      /*[in]*/ const tstring & str2)
    const
  {
    return (PathName::Compare(str1.c_str(), str2.c_str()) < 0);
  }
};

/* _________________________________________________________________________ */

MIKTEX_CORE_END_NAMESPACE;

#endif	// C++

#endif	// miktex.h
