/* miktex/Core/core.h: MiKTeX core API				-*- C++ -*-

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

#if ! defined(A089FEF06254514BA063DED44B70E66F)
#define A089FEF06254514BA063DED44B70E66F

#include <miktex/Core/First>
#include <miktex/Core/Definitions>
#include <miktex/Core/config.h>

#if defined(MIKTEX_WINDOWS)
#  include <windows.h>
#endif

#include <miktex/Core/Text>
#include <miktex/Core/MD5>

#if defined(__cplusplus)
#  include <cstdarg>
#  include <cstdio>
#  include <cstdlib>
#  include <cstring>
#  include <ctime>
#  include <stdexcept>
#  include <string>
#  include <vector>
#else
#  include <stdio.h>
#  include <stdlib.h>
#  include <time.h>
#endif

#if defined(HAVE_PTHREAD)
#  include <pthread.h>
#endif

#if defined(_MSC_VER) && defined(MIKTEX_STATIC)
#  pragma comment (lib, "htmlhelp")
#  pragma comment (lib, "shfolder")
#  pragma comment (lib, "shell32")
#endif

// DLL import/export switch
#if ! defined(EAD86981C92C904D808A5E6CEC64B90E)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define MIKTEXCOREEXPORT __declspec(dllimport)
#  elif ! defined(MIKTEX_STATIC) && __GNUC__ >= 4
#    define MIKTEXCOREEXPORT __attribute__((visibility("default")))
#  else
#    define MIKTEXCOREEXPORT
#  endif
#endif

// API decoration for exported member functions
#define MIKTEXCORETHISAPI(type) MIKTEXCOREEXPORT type MIKTEXTHISCALL

// API decoration for exported functions
#define MIKTEXCORECEEAPI(type) MIKTEXCOREEXPORT type MIKTEXCEECALL

// API decoration for exported types
#if __GNUC__ >= 4
#  define MIKTEXCORETYPEAPI(type) MIKTEXCOREEXPORT type
#else
#  define MIKTEXCORETYPEAPI(type) type
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

   Support utilities which cannot be compiled with a C++ compiler.
   _________________________________________________________________________ */

#define MIKTEXCEEAPI(type) MIKTEXCOREEXPORT type MIKTEXCEECALL

#if defined(MIKTEX_WINDOWS)
MIKTEXCEEAPI(char*)
miktex_ansi_to_utf8 (/*[in]*/ const char *	lpszAnsi,
		     /*[in]*/ size_t		sizeUtf8,
		     /*[out]*/ char *		lpszUtf8);
#endif

MIKTEXCEEAPI(void)
miktex_create_temp_file_name (/*[out]*/ char *	lpszFileName);

MIKTEXCEEAPI(int)
miktex_determine_metafont_mode (/*[in]*/ unsigned	dpi,
				/*[out]*/ char *	lpszMnemonic,
				/*[in]*/ size_t		bufSize);

MIKTEXCEEAPI(int)
miktex_find_afm_file (/*[in]*/ const char *	lpszFontName,
		      /*[out]*/ char *		lpszPath);

MIKTEXCEEAPI(int)
miktex_find_input_file (/*[in]*/ const char *	lpszApplicationNames,
			/*[in]*/ const char *	lpszFileName,
			/*[out]*/ char *	lpszPath);

MIKTEXCEEAPI(int)
miktex_find_enc_file (/*[in]*/ const char *	lpszFontName,
		      /*[out]*/ char *		lpszPath);

MIKTEXCEEAPI(int)
miktex_find_file (/*[in]*/ const char *	lpszFileName,
		  /*[in]*/ const char *	lpszPathList,
		  /*[out]*/ char *	lpszPath);

MIKTEXCEEAPI(int)
miktex_find_hbf_file (/*[in]*/ const char *	lpszFileName,
		      /*[out]*/ char *		lpszPath);

MIKTEXCEEAPI(int)
miktex_find_miktex_executable (/*[in]*/ const char *	lpszExeName,
			       /*[out]*/ char *		lpszExePath);

MIKTEXCEEAPI(int)
miktex_find_psheader_file (/*[in]*/ const char *	lpszHeaderName,
			   /*[in]*/ char *		lpszPath);

MIKTEXCEEAPI(int)
miktex_find_tfm_file (/*[in]*/ const char *	lpszFontName,
		      /*[out]*/ char *		lpszPath);

MIKTEXCEEAPI(int)
miktex_find_ttf_file (/*[in]*/ const char *	lpszFontName,
		      /*[out]*/ char *		lpszPath);

MIKTEXCEEAPI(char *)
miktex_get_config_value (/*[in]*/ const char *	lpszSectionName,
			 /*[in]*/ const char *	lpszValueName,
			 /*[out]*/ char *	lpszBuf,
			 /*[in]*/ size_t	bufSize,
			 /*[in]*/ const char *	lpszDefaultValue);

MIKTEXCEEAPI(unsigned)
miktex_get_number_of_texmf_roots ();

MIKTEXCEEAPI(void)
miktex_get_gs_exe (/*[out]*/ char * lpszPath);

MIKTEXCEEAPI(int)
miktex_get_miktex_version_string_ex (/*[out]*/ char *	lpszVersion,
				     /*[in]*/ size_t	maxsize);

MIKTEXCEEAPI(char *)
miktex_get_root_directory (/*[in]*/  unsigned	r,
			   /*[out]*/ char *	lpszPath);

MIKTEXCEEAPI(int)
miktex_pathcmp (/*[in]*/ const char *	lpszPath1,
		/*[in]*/ const char *	lpszPath2);

MIKTEXCEEAPI(void)
miktex_remember_temp_file (/*[in]*/ const char *	lpszFileName);

MIKTEXCEEAPI(void)
miktex_replace_string (/*[out]*/ char *		lpszBuf,
		       /*[in,out]*/ size_t *	lpSzeDest,
		       /*[in]*/ const char *	lpszSource,
		       /*[in]*/ const char *	lpszString1,
		       /*[in]*/ const char *	lpszString2);

MIKTEXCEEAPI(void)
miktex_start_process (/*[in]*/ const char *	lpszFileName,
		      /*[in]*/ const char *	lpszArguments,
		      /*[in]*/ FILE *		pFileStandardInput,
		      /*[out]*/ FILE **		ppFileStandardInput,
		      /*[out]*/ FILE **		ppFileStandardOutput,
		      /*[out]*/ FILE **		ppFileStandardError,
		      /*[in]*/ const char *	lpszWorkingDirectory);

MIKTEXCEEAPI(void)
miktex_uncompress_file (/*[in]*/ const char *	lpszPathIn,
			/*[out]*/ char *	lpszPathOut);

#if defined(MIKTEX_WINDOWS)
MIKTEXCEEAPI(char*)
miktex_utf8_to_ansi (/*[in]*/ const char *	lpszUtf8,
		     /*[in]*/ size_t		sizeAnsi,
		     /*[out]*/ char *		lpszAnsi);
#endif

MIKTEXCEEAPI(wchar_t*)
miktex_utf8_to_wide_char (/*[in]*/ const char *	lpszUtf8,
			  /*[in]*/ size_t	sizeWideChar,
			  /*[out]*/ wchar_t *	lpszWideChar);

MIKTEXCEEAPI(char*)
miktex_wide_char_to_utf8 (/*[in]*/ const wchar_t *  lpszWideChar,
			  /*[in]*/ size_t	    sizeUtf8,
			  /*[out]*/ char *	    lpszUtf8);

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

#include <miktex/Core/Debug>

/// @namespace MiKTeX::Core
/// @brief The MiKTeX core namespace.
MIKTEX_CORE_BEGIN_NAMESPACE;

class PathName;

/* _________________________________________________________________________

   IsWindowsNT
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
inline
bool
IsWindowsNT ()
{
  DWORD windowsVersion = GetVersion();
#if defined(_MSC_VER) && ! defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
  MIKTEX_ASSERT (windowsVersion < 0x80000000);
  MIKTEX_ASSUME (windowsVersion < 0x80000000);
#endif
  return (windowsVersion < 0x80000000);
}
#endif

/* _________________________________________________________________________

   IsWindowsVista
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
inline
bool
IsWindowsVista ()
{
  return ((GetVersion() & 0xff) >= 6);
}
#endif

/* _________________________________________________________________________

   StructChecker
   _________________________________________________________________________ */

template<class Derived>
struct StructChecker
{
private:
  size_t sizeOfStruct;
protected:
  StructChecker ()
    : sizeOfStruct (sizeof(Derived))
  {
  }
public:
  size_t
  GetSizeOfStruct ()
    const
  {
    return (sizeOfStruct);
  }
};

/* _________________________________________________________________________
   
   FormatInfo
   _________________________________________________________________________ */

/// Information about a TeX format.
struct FormatInfo
{
  /// The access key.
  std::string key;
  /// Name of the format.
  std::string name;
  /// One-line description.
  std::string description;
  /// The compiler (engine) which processes the format.
  std::string compiler;
  /// The name of the input file.
  std::string inputFile;
  /// The name of the output file.
  std::string outputFile;
  /// The name of another format which has to be pre-loaded.
  std::string preloaded;
  /// Exclusion flag. If set, the format will be ignored by initexmf.
  bool exclude;
  /// Custom flag. Set, if this format was defined by the user.
  bool custom;

  std::string arguments;

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
  std::string texName;
  /// Name of the PostScript font.
  std::string psName;
  /// Special instructions to be embedded into the PostScript document.
  std::string specialInstructions;
  /// Name of the encoding vector file.
  std::string encFile;
  /// Name of the font file.
  std::string fontFile;
  /// List of header files.
  std::string headerList;
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
  char szMnemonic[32];
  char szDescription[128];
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

  /// Compares the encapsulated enum value to another value.
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
  /// @return Returns true, if this values is greater than the other.
public:
  bool
  operator> (/*[in]*/ const EnumWrapper & other)
    const
  {
    return (this->value > other.value);
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
  VersionNumber (/*[in]*/ const char * lpszVersion)
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
  CompareTo (/*[in]*/ const char * lpszOther)
    const
  {
    return (CompareTo(Parse(lpszOther)));
  }

  /// Converts this version number into a string object.
  /// @return Returns the string object.
  MIKTEXCORETHISAPI(std::string)
  ToString ()
    const;

  /// Parses the string representation of a version number.
  /// @param lpsz The string to parse.
  /// @param[out] The version number object to fill.
  /// @returns Returns true if the string could be parsed.
  static
  MIKTEXCORECEEAPI(bool)
  TryParse (/*[in]*/ const char * lpsz,
	    /*[out]*/ VersionNumber &	versionNumber);

  /// Parses the string representation of a version number.
  /// @param lpsz The string to parse.
  /// @returns Returns the parsed version number.
  static
  MIKTEXCORECEEAPI(VersionNumber)
  Parse (/*[in]*/ const char * lpsz);
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
    /// The file will be created, if it doesn't already exist.
    Create,
    /// Open an existing file.
    Open,
    /// Execute a command and create a pipe.
    Command,
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
    /// Resolve symbolic links.
    Canonicalize = 128,
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  MiKTeXException (/*[in]*/ const char *	lpszProgramInvocationName,
		   /*[in]*/ const char *	lpszMessage,
		   /*[in]*/ const char *	lpszInfo,
		   /*[in]*/ const char *	lpszSourceFile,
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
    return (message.c_str());
  }

  /// Destructor.
public:
  virtual
  MIKTEXTHISCALL
  ~MiKTeXException ()
    throw ()
  {
  }

  /// Gets additional information.
  /// @return Returns a string object.
public:
  std::string
  GetInfo ()
    const
    throw ()
  {
    return (info);
  }

  /// Gets the name of the invoked program.
  /// @return Returns a string object.
public:
  std::string
  GetProgramInvocationName ()
    const
    throw ()
  {
    return (programInvocationName);
  }

  /// Gets the name of the source code file.
  /// @return Returns a string object.
public:
  std::string
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
  std::string info;

private:
  std::string message;

private:
  std::string programInvocationName;

private:
  std::string sourceFile;

private:
  int sourceLine;
};

/* _________________________________________________________________________

   OperationCancelledException
   _________________________________________________________________________ */

class OperationCancelledException : public MiKTeXException
{
public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  OperationCancelledException ();

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  OperationCancelledException
  (/*[in]*/ const char * lpszProgramInvocationName,
   /*[in]*/ const char *	lpszMessage,
   /*[in]*/ const char *	lpszInfo,
   /*[in]*/ const char *	lpszSourceFile,
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  IOException (/*[in]*/ const char *	lpszProgramInvocationName,
	       /*[in]*/ const char *	lpszMessage,
	       /*[in]*/ const char *	lpszInfo,
	       /*[in]*/ const char *	lpszSourceFile,
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  BrokenPipeException (/*[in]*/ const char *	lpszProgramInvocationName,
		       /*[in]*/ const char *	lpszMessage,
		       /*[in]*/ const char *	lpszInfo,
		       /*[in]*/ const char *	lpszSourceFile,
		       /*[in]*/ int		sourceLine);
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  FileNotFoundException (/*[in]*/ const char *	lpszProgramInvocationName,
			 /*[in]*/ const char *	lpszMessage,
			 /*[in]*/ const char *	lpszInfo,
			 /*[in]*/ const char *	lpszSourceFile,
			 /*[in]*/ int		sourceLine);
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  UnauthorizedAccessException
  (/*[in]*/ const char *	lpszProgramInvocationName,
   /*[in]*/ const char *	lpszMessage,
   /*[in]*/ const char *	lpszInfo,
   /*[in]*/ const char *	lpszSourceFile,
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  SharingViolationException
  (/*[in]*/ const char *	lpszProgramInvocationName,
   /*[in]*/ const char *	lpszMessage,
   /*[in]*/ const char *	lpszInfo,
   /*[in]*/ const char *	lpszSourceFile,
   /*[in]*/ int			sourceLine);
};
#endif

/* _________________________________________________________________________

   C++ Inline Functions
   _________________________________________________________________________ */

inline
const char *
StrChr (/*[in]*/ const char *	lpsz,
	/*[in]*/ int		ch)
{
  return (strchr(lpsz, ch));
}

inline
const wchar_t *
StrChr (/*[in]*/ const wchar_t *	lpsz,
	/*[in]*/ wint_t			ch)
{
  return (wcschr(lpsz, ch));
}

inline
size_t
StrLen (/*[in]*/ const char * lpsz)
{
  return (strlen(lpsz));
}

inline
size_t
StrLen (/*[in]*/ const wchar_t * lpsz)
{
  return (wcslen(lpsz));
}

inline
int
StringCompare (/*[in]*/ const char *	lpsz1,
	       /*[in]*/ const char *	lpsz2,
	       /*[in]*/ bool		ignoreCase)
{
#if defined(_MSC_VER)
  return (ignoreCase
	  ? _stricmp(lpsz1, lpsz2)
	  : _stricmp(lpsz1, lpsz2));
#else
  return (ignoreCase
	  ? strcasecmp(lpsz1, lpsz2)
	  : strcmp(lpsz1, lpsz2));
#endif
}

inline
int
StringCompare (/*[in]*/ const char *	lpsz1,
	       /*[in]*/ const char *	lpsz2)
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
  MIKTEXCORECEEAPI(PaperSizeInfo)
  Parse (/*[in]*/ const char *	lpsz);

  /// Paper name (e.g., "A4")
public:
  std::string name;

public:
  std::string dvipsName;

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
  MIKTEXCORECEEAPI(size_t)
  AppendString (/*[in,out]*/ char *	lpszBuf,
		/*[in]*/ size_t			bufSize,
		/*[in]*/ const char *	lpszSource);

  /// Replaces all occurences of a sub-string within a string.
  /// @param[out] lpszBuf The destination string buffer.
  /// @param bufSize The size (in characters) of the destination buffer.
  /// @param lpszSource The string to be searched.
  /// @param lpszString1 The sub-string to be replaced.
  /// @param lpszString2 The replacement sub-string.
public:
  static
  MIKTEXCORECEEAPI(void)
  ReplaceString (/*[out]*/ char *	lpszBuf,
		 /*[in,out]*/ size_t &	bufSize,
		 /*[in]*/ const char *	lpszSource,
		 /*[in]*/ const char *	lpszString1,
		 /*[in]*/ const char *	lpszString2);

  /// Copies a string.
  /// @param[out] lpszBuf The destination string buffer.
  /// @param bufSize Size (in characters) of the destination string buffer.
  /// @param lpszSource The null-terminated string to be copied.
  /// @return Returns the length (in characters) of the result.
public:
  static
  MIKTEXCORECEEAPI(size_t)
  CopyString (/*[out]*/ char *		lpszBuf,
	      /*[in]*/ size_t		bufSize,
	      /*[in]*/ const char *	lpszSource);

  /// Copies a wide string into another wide string.
  /// @param[out] lpszBuf The destination string buffer.
  /// @param bufSize Size (in characters) of the destination string buffer.
  /// @param lpszSource The null-terminated string to be copied.
  /// @return Returns the length (in characters) of the result.
public:
  static
  MIKTEXCORECEEAPI(size_t)
  CopyString (/*[out]*/ wchar_t *	lpszBuf,
	      /*[in]*/ size_t		bufSize,
	      /*[in]*/ const wchar_t *	lpszSource);

  /// Copies a wide string into a single-byte string.
  /// @param[out] lpszBuf The destination string buffer.
  /// @param bufSize Size (in characters) of the destination string buffer.
  /// @param lpszSource The null-terminated string to be copied.
  /// @return Returns the length (in characters) of the result.
public:
  static
  MIKTEXCORECEEAPI(size_t)
  CopyString (/*[out]*/ char *		lpszBuf,
	      /*[in]*/ size_t		bufSize,
	      /*[in]*/ const wchar_t *	lpszSource);

  /// Gets the value of an environment variable.
  /// @param lpszName The name of the environment variable.
  /// @param[out] str The string object to be filled with the value.
  /// @return Returns true if the environment variable exists.
public:
  static
  MIKTEXCORECEEAPI(bool)
  GetEnvironmentString (/*[in]*/ const char *		lpszName,
			/*[out]*/ std::string &		str);

public:
  static
  MIKTEXCORECEEAPI(bool)
  GetEnvironmentString (/*[in]*/ const char *	lpszName,
			/*[out]*/ PathName &	path);

  /// Gets the value of an environment variable.
  /// @param lpszName The name of the environment variable.
  /// @param[out] lpszOut The destination string buffer.
  /// @param sizeOut The size (in characters) of the destination string
  /// buffer.
  /// @return Returns true if the environment variable exists.
public:
  static
  MIKTEXCORECEEAPI(bool)
  GetEnvironmentString (/*[in]*/ const char *	lpszName,
			/*[out]*/ char *	lpszOut,
			/*[in]*/ size_t		sizeOut);

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(bool)
  GetDefPrinter (/*[out]*/ char *		lpszPrinterName,
		 /*[in,out]*/ size_t *		pBufferSize);
#endif

public:
  static
  MIKTEXCORECEEAPI(bool)
  Contains (/*[in]*/ const char *	lpszList,
	    /*[in]*/ const char *	lpszElement,
	    /*[in]*/ const char *	lpszDelim,
	    /*[in]*/ bool		ignoreCase);

public:
  static
  bool
  Contains (/*[in]*/ const char *	lpszList,
	    /*[in]*/ const char *	lpszElement,
	    /*[in]*/ const char *	lpszDelim)
  {
    return (Contains(lpszList, lpszElement, lpszDelim, true));
  }

public:
  static
  bool
  Contains (/*[in]*/ const char *	lpszList,
	    /*[in]*/ const char *	lpszElement)
  {
    return (Contains(lpszList, lpszElement, ",;:"));
  }

public:
  static
  MIKTEXCORECEEAPI(std::string)
  GetExeName ();

  /// Gets the MiKTeX version information as a string.
public:
  static
  MIKTEXCORECEEAPI(std::string)
  GetMiKTeXVersionString ();

  /// Gets the MiKTeX banner string.
  /// The banner string includes the MiKTeX version number.
  /// @return Returns a string object.
public:
  static
  MIKTEXCORECEEAPI(std::string)
  GetMiKTeXBannerString ();

public:
  static
  MIKTEXCORECEEAPI(std::string)
  MakeProgramVersionString (/*[in]*/ const char *	lpszProgramName,
			    /*[in]*/ const VersionNumber &	versionNumber);

public:
  static
  MIKTEXCORECEEAPI(std::string)
  GetOSVersionString ();

public:
  static
  MIKTEXCORECEEAPI(void)
  UncompressFile (/*[in]*/ const char *	lpszPathIn,
		  /*[out]*/ PathName &	pathOut);

public:
  static
  MIKTEXCORECEEAPI(const char *)
  GetRelativizedPath (/*[in]*/ const char * lpszPath,
		      /*[in]*/ const char * lpszRoot);

public:
  static
  MIKTEXCORECEEAPI(bool)
  GetUncRootFromPath (/*[in]*/ const char *	lpszPath,
		      /*[out]*/ PathName &	uncRoot);

public:
  static
  MIKTEXCORECEEAPI(bool)
  IsAbsolutePath (/*[in]*/ const char *	lpszPath);

public:
  static
  MIKTEXCORECEEAPI(bool)
  IsParentDirectoryOf (/*[in]*/ const char * lpszParentDir,
		       /*[in]*/ const char * lpszFileName);

public:
  static
  MIKTEXCORECEEAPI(void)
  MakeTeXPathName (/*[in,out]*/ PathName &	path);

public:
  static
  MIKTEXCORECEEAPI(void)
  SetEnvironmentString (/*[in]*/ const char *	lpszValueName,
			/*[in]*/ const char *	lpszValue);

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(void)
  RemoveBlanksFromPathName (/*[in,out]*/ PathName &	path);
#endif

public:
  static
  MIKTEXCORECEEAPI(void)
  PrintException (/*[in]*/ const std::exception &	e);

  /// Prints a MiKTeXException to the standard error stream.
  /// @param e Execption to print.  
public:
  static
  MIKTEXCORECEEAPI(void)
  PrintException (/*[in]*/ const MiKTeXException &	e);

public:
  static
  MIKTEXCORECEEAPI(bool)
  ReadUntilDelim (/*[out]*/ std::string &	str,
		  /*[in]*/ int			delim,
		  /*[in]*/ FILE *		stream);

public:
  static
  MIKTEXCORECEEAPI(bool)
  ReadLine (/*[out]*/ std::string &	str,
	    /*[in]*/ FILE *		stream,
	    /*[in]*/ bool		keepLineEnding);

  /// Creates a formatted string object.
  /// @param lpszFormat The format of the string (printf() syntax).
  /// @param arglist Argument list.
  /// @return Returns a string object.
public:
  static
  MIKTEXCORECEEAPI(std::string)
  FormatString (/*[in]*/ const char *	lpszFormat,
		/*[in]*/ va_list	arglist);

public:
  static
  MIKTEXCORECEEAPI(std::string)
  Hexify (/*[in]*/ const void *	pv,
	  /*[in]*/ size_t	nBytes,
	  /*[in]*/ bool		lowerCase);

public:
  static
  MIKTEXCORECEEAPI(std::string)
  Hexify (/*[in]*/ const void *	pv,
	  /*[in]*/ size_t	nBytes);

public:
  static
  MIKTEXCORECEEAPI(bool)
  ParseDvipsMapLine (/*[in]*/ const std::string &	line,
		     /*[out]*/ FontMapEntry &		fontMapEntry);

public:
  static
  MIKTEXCORECEEAPI(bool)
  IsMiKTeXDirectRoot (/*[in]*/ const PathName &	root);

public:
  static
  MIKTEXCORECEEAPI(void)
  RegisterMiKTeXUser ();

public:
  static
  MIKTEXCORECEEAPI(void)
  ShowWebPage (/*[in]*/ const char * lpszUrl);

public:
#if defined(MIKTEX_WINDOWS)
  static
  MIKTEXCORECEEAPI(PathName)
  GetFolderPath (/*[in]*/ int	nFolder,
		 /*[in]*/ int	nFallbackFolder,
		 /*[in]*/ bool	getCurrentPath);
#endif

public:
#if defined(MIKTEX_WINDOWS)
  static
  MIKTEXCORECEEAPI(std::wstring)
  UTF8ToWideChar (/*[in]*/ const char * lpszUtf8);
#endif

public:
#if defined(MIKTEX_WINDOWS)
  static
  MIKTEXCORECEEAPI(std::string)
  WideCharToUTF8 (/*[in]*/ const wchar_t * lpszWideChar);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(std::string)
  WideCharToAnsi (/*[in]*/ const wchar_t * lpszWideChar);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(std::wstring)
  AnsiToWideChar (/*[in]*/ const char * lpszAnsi);
#endif

public:
  static
  MIKTEXCORECEEAPI(void)
  CheckHeap ();

public:
  static
  MIKTEXCORECEEAPI(void)
  CanonicalizePathName (/*[in,out]*/ PathName & path);

public:
  static
  MIKTEXCORECEEAPI(bool)
  GetPathNamePrefix (/*[in]*/ const PathName &	path_,
		     /*[in]*/ const PathName &	suffix_,
		     /*[out]*/ PathName &	prefix);

#if defined(MIKTEX_WINDOWS)
public:
  static
  std::string
  UTF8ToAnsi (/*[in]*/ const char * lpszUtf8)
  {
    return (WideCharToAnsi(UTF8ToWideChar(lpszUtf8).c_str()));
  }
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  std::string
  AnsiToUTF8 (/*[in]*/ const char * lpszAnsi)
  {
    return (WideCharToAnsi(AnsiToWideChar(lpszAnsi).c_str()));
  }
#endif

public:
  static
  MIKTEXCORECEEAPI(bool)
  IsAscii (/*[in]*/ const char * lpsz);

public:
  static
  MIKTEXCORECEEAPI(bool)
  IsAscii (/*[in]*/ const wchar_t * lpsz);

public:
  static
  MIKTEXCORECEEAPI(bool)
  IsUTF8 (/*[in]*/ const char * lpsz,
	  /*[in]*/ bool		allowPureAscii);

public:
  static
  bool
  IsUTF8 (/*[in]*/ const char * lpsz)
  {
    return (IsUTF8(lpsz, true));
  }

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(bool)
  CheckPath (/*[in]*/ bool repair);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(void)
  RegisterShellFileAssoc (/*[in]*/ const char * lpszExtension,
			  /*[in]*/ const char * lpszProgId,
			  /*[in]*/ bool		takeOwnership);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(void)
  UnregisterShellFileAssoc (/*[in]*/ const char * lpszExtension,
			    /*[in]*/ const char * lpszProgId);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(void)
  RegisterShellFileType (/*[in]*/ const char * lpszProgId,
			 /*[in]*/ const char * lpszUserFriendlyName,
			 /*[in]*/ const char * lpszIconPath);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(void)
  UnregisterShellFileType (/*[in]*/ const char * lpszProgId);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(void)
  RegisterShellVerb (/*[in]*/ const char * lpszProgId,
		     /*[in]*/ const char * lpszVerb,
		     /*[in]*/ const char * lpszCommand,
		     /*[in]*/ const char * lpszDdeExec);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(std::string)
  MakeProgId (/*[in]*/ const char * lpszComponent);
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
  MIKTEXCORECEEAPI(MD5)
  FromFile (/*[in]*/ const char * lpszPath);

  /// Parses the string represention of an MD5.
  /// @param lpszHexString The string representation (32 hex characters).
  /// @return Returns the MD5.
public:
  static
  MIKTEXCORECEEAPI(MD5)
  Parse (/*[in]*/ const char * lpszHexString);

  /// Converts this MD5 into a string object.
  /// @return Returns a string object.
public:
  std::string
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
			       reinterpret_cast<const char *>(0),
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

   CharBuffer
   _________________________________________________________________________ */

/// Instances of this class store characters.
template<typename CharType, int BUFSIZE=512>
class CharBuffer
{
private:
  CharType smallBuffer[BUFSIZE];

private:
  CharType * buffer;

private:
  size_t capacity;

public:
  CharBuffer ()
    : buffer (smallBuffer),
      capacity (BUFSIZE)
  {
    Reset ();
  }

  /// Copies another CharBuffer object into a new CharBuffer object.
public:
  CharBuffer (/*[in]*/ const CharBuffer & other)
    : buffer (smallBuffer),
      capacity (BUFSIZE)
  {
    Reset ();
    Set (other);
  }

public:
  CharBuffer (/*[in]*/ const char * lpsz)
    : buffer (smallBuffer),
      capacity (BUFSIZE)
  {
    MIKTEX_ASSERT_STRING_OR_NIL (lpsz);
    Set (lpsz);
  }

public:
  CharBuffer (/*[in]*/ const wchar_t * lpsz)
    : buffer (smallBuffer),
      capacity (BUFSIZE)
  {
    MIKTEX_ASSERT_STRING_OR_NIL (lpsz);
    Set (lpsz);
  }

public:
  CharBuffer (/*[in]*/ const std::basic_string<char> & other)
    : buffer (smallBuffer),
      capacity (BUFSIZE)
  {
    Set (other);
  }

public:
  CharBuffer (/*[in]*/ const std::basic_string<wchar_t> & other)
    : buffer (smallBuffer),
      capacity (BUFSIZE)
  {
    Set (other);
  }

public:
  CharBuffer (/*[in]*/ size_t n)
    : buffer (smallBuffer),
      capacity (BUFSIZE)
  {
    Reset ();
    Reserve (n);
  }

public:
  ~CharBuffer ()
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
  Set (/*[in]*/ const CharBuffer & rhs)
  {
    if (this != &rhs)
      {
	Reserve (rhs.capacity);
	memcpy (this->buffer, rhs.buffer, rhs.capacity * sizeof(CharType));
      }
  }

public:
  template<typename OtherCharType>
  void
  Set (/*[in]*/ const OtherCharType * lpsz)
  {
    MIKTEX_ASSERT_STRING_OR_NIL (lpsz);
    if (lpsz == 0)
      {
	Reset ();
      }
    else
      {
	Reserve (StrLen(lpsz) + 1);
	Utils::CopyString (buffer, GetCapacity(), lpsz);
      }
  }

public:
  template<typename OtherCharType>
  void
  Set (/*[in]*/ const std::basic_string<OtherCharType> & rhs)
  {
    Set (rhs.c_str());
  }

public:
  void
  Append (/*[in]*/ const CharType * lpsz)
  {
    MIKTEX_ASSERT_STRING (lpsz);
    Reserve (GetLength() + StrLen(lpsz) + 1);
    Utils::AppendString (buffer, GetCapacity(), lpsz);
  }

public:
  void
  Append (/*[in]*/ const CharType *	lpsz,
	  /*[in]*/ size_t		len)
  {
    size_t idx = GetLength();
    Reserve (idx + len + 1);
    for (; len > 0; -- len, ++ idx, ++ lpsz)
      {
	buffer[idx] = *lpsz;
      }
    buffer[idx] = 0;
  }

public:
  void
  Append (/*[in]*/ const CharType ch)
  {
    size_t len = GetLength();
    Reserve (len + 2);
    buffer[len] = ch;
    buffer[len + 1] = 0;
  }

public:
  void
  Clear ()
  {
    buffer[0] = 0;
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

public:
  void
  Reset ()
  {
    if (buffer != smallBuffer)
      {
	delete [] buffer;
	buffer = smallBuffer;
	capacity = BUFSIZE;
      }
    Clear ();
  }

public:
  void
  Reserve (/*[in]*/ size_t newSize)
  {
    if (newSize > BUFSIZE && newSize > capacity)
      {
	CharType * newBuffer = new CharType[newSize];
	memcpy (newBuffer, buffer, capacity * sizeof(CharType));
	if (buffer != smallBuffer)
	  {
	    delete [] buffer;
	  }
	buffer = newBuffer;
	capacity = newSize;
      }
  }

public:
  CharType *
  GetBuffer ()
    const
  {
    return (buffer);
  }

public:
  const CharType *
  Get ()
    const
  {
    return (buffer);
  }

public:
  size_t
  GetLength ()
    const
  {
    size_t idx;
    for (idx = 0; idx < capacity && buffer[idx] != 0; ++ idx)
      {
	;
      }
    MIKTEX_ASSERT (idx < capacity);
    return (idx);
  }

public:
  size_t
  GetCapacity ()
    const
  {
    return (capacity);
  }

  /// Converts this CharBuffer object into a string object.
  /// @return Returns a string object.
public:
  std::basic_string<CharType>
  ToString ()
    const
  {
    return (std::basic_string<CharType>(buffer));
  }

#if defined(MIKTEX_WINDOWS)
public:
  void
  Ansify ()
  {
    if (Utils::IsUTF8(Get(), false))
      {
	Set (Utils::UTF8ToAnsi(Get()));
      }
  }
#endif

public:
  const CharType &
  operator[] (/*[in]*/ size_t idx)
    const
  {
    MIKTEX_ASSERT (idx < GetCapacity());
    return (buffer[idx]);
  }

public:
  CharType &
  operator[] (/*[in]*/ size_t idx)
  {
    MIKTEX_ASSERT (idx < GetCapacity());
    return (buffer[idx]);
  }

public:
  CharBuffer &
  operator= (/*[in]*/ const CharBuffer & rhs)
  {
    Set (rhs);
    return (*this);
  }

public:
  template<typename OtherCharType>
  CharBuffer &
  operator= (/*[in]*/ const OtherCharType * lpszRhs)
  {
    Set (lpszRhs);
    return (*this);
  }

public:
  template<typename OtherCharType>
  CharBuffer &
  operator= (/*[in]*/ const std::basic_string<OtherCharType> & rhs)
  {
    Set (rhs);
    return (*this);
  }

public:
  CharBuffer &
  operator+= (/*[in]*/ const CharType * lpszRhs)
  {
    Append (lpszRhs);
    return (*this);
  }

public:
  CharBuffer &
  operator+= (/*[in]*/ CharType ch)
  {
    Append (ch);
    return (*this);
  }
};

/* _________________________________________________________________________

   PathName
   _________________________________________________________________________ */

/// Instances of this class can be used to store path names.
class PathName : public CharBuffer<char, BufferSizes::MaxPath>
{
protected:
  typedef CharBuffer<char, BufferSizes::MaxPath> Base;
public:
  enum {
    DosDirectoryDelimiter = '\\',
    DosPathNameDelimiter = ';',
    UnixDirectoryDelimiter = '/',
    UnixPathNameDelimiter = ':',
#if defined(MIKTEX_WINDOWS)
    AltDirectoryDelimiter = UnixDirectoryDelimiter,
    DirectoryDelimiter = DosDirectoryDelimiter,
    PathNameDelimiter = DosPathNameDelimiter,
    VolumeDelimiter = ':',
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
  IsVolumeDelimiter (/*[in]*/ int ch)
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
  IsDirectoryDelimiter (/*[in]*/ int ch)
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
    : CharBuffer<char, BufferSizes::MaxPath> (rhs)
  {
  }

public:
  PathName (/*[in]*/ int zero)
    : CharBuffer<char, BufferSizes::MaxPath> (zero)
  {
  }

  /// Copies a character string into a new PathName object.
  /// @param rhs Null-terminated character string.
public:
  PathName (/*[in]*/ const char * rhs)
    : CharBuffer<char, BufferSizes::MaxPath> (rhs)
  {
  }

  /// Copies a wide character string into a new PathName object.
  /// @param rhs Null-terminated character string.
public:
  PathName (/*[in]*/ const wchar_t * rhs)
    : CharBuffer<char, BufferSizes::MaxPath> (rhs)
  {
  }

  /// Copies a string object into a new PathName object.
  /// @param rhs String object.
public:
  PathName (/*[in]*/ const std::basic_string<char> & rhs)
    : CharBuffer<char, BufferSizes::MaxPath> (rhs.c_str())
  {
  }

  /// Copies a string object into a new PathName object.
  /// @param rhs String object.
public:
  PathName (/*[in]*/ const std::basic_string<wchar_t> & rhs)
    : CharBuffer<char, BufferSizes::MaxPath> (rhs.c_str())
  {
  }

#if defined(MIKTEX_WINDOWS)
public:
  PathName (/*[in]*/ const char * lpszDrive,
	    /*[in]*/ const char * lpszAbsPath,
	    /*[in]*/ const char * lpszRelPath,
	    /*[in]*/ const char * lpszExtension)
  {
    Set (lpszDrive, lpszAbsPath, lpszRelPath, lpszExtension);
  }
#endif

  /// Combines path name components into a new PathName object.
  /// @param lpszAbsPath The first component (absolute directory path).
  /// @param lpszRelPath The second component (relative file name path).
public:
  PathName (/*[in]*/ const char * lpszAbsPath,
	    /*[in]*/ const char * lpszRelPath)
  {
    Set (lpszAbsPath, lpszRelPath);
  }

  /// Combines path name components into a new PathName object.
  /// @param lpszAbsPath The first component (absolute directory path).
  /// @param lpszRelPath The second component (relative file name path).
  /// @param lpszExtension The third component (file name extension).
public:
  PathName (/*[in]*/ const char * lpszAbsPath,
	    /*[in]*/ const char * lpszRelPath,
	    /*[in]*/ const char * lpszExtension)
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
  operator= (/*[in]*/ const char * lpszRhs)
  {
    Base::operator= (lpszRhs);
    return (*this);
  }

public:
  PathName &
  operator= (/*[in]*/ const std::basic_string<char> & rhs)
  {
    Base::operator= (rhs);
    return (*this);
  }

  /// Calculates the hash value of this PathName object.
  /// @return Returns the hash value.
public:
  MIKTEXCORETHISAPI(size_t)
  GetHash ()
    const;

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(void)
  Combine (/*[out]*/ char *		lpszPath,
	   /*[in]*/ size_t		sizePath,
	   /*[in]*/ const char *	lpszDrive,
	   /*[in]*/ const char *	lpszAbsPath,
	   /*[in]*/ const char *	lpszRelPath,
	   /*[in]*/ const char *	lpszExtension);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(void)
  Split (/*[in]*/ const char *	lpszPath,
	 /*[out]*/ char *	lpszDrive,
	 /*[in]*/ size_t	sizeDrive,
	 /*[out]*/ char *	lpszDirectory,
	 /*[in]*/ size_t	sizeDirectory,
	 /*[out]*/ char *	lpszName,
	 /*[in]*/ size_t	sizeName,
	 /*[out]*/ char *	lpszExtension,
	 /*[in]*/ size_t	sizeExtension);
#endif

public:
  static
  MIKTEXCORECEEAPI(void)
  Split (/*[in]*/ const char *	lpszPath,
	 /*[out]*/ char *	lpszDirectory,
	 /*[in]*/ size_t	sizeDirectory,
	 /*[out]*/ char *	lpszName,
	 /*[in]*/ size_t	sizeName,
	 /*[out]*/ char *	lpszExtension,
	 /*[in]*/ size_t	sizeExtension);

public:
  char *
  GetFileName (/*[out]*/ char * lpszFileName)
    const
  {
    char szExtension[BufferSizes::MaxPath];
    Split (Get(),
	   0, 0,
	   lpszFileName, BufferSizes::MaxPath,
	   szExtension, BufferSizes::MaxPath);
    Utils::AppendString (lpszFileName, BufferSizes::MaxPath, szExtension);
    return (lpszFileName);
  }

public:
  char *
  GetFileNameWithoutExtension (/*[out]*/ char * lpszFileName)
    const
  {
    Split (Get(),
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
    Split (Get(),
	   0, 0,
	   fileName.GetBuffer(), fileName.GetCapacity(),
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
  MIKTEXCORECEEAPI(void)
  Combine (/*[out]*/ char *		lpszPath,
	   /*[in]*/ size_t		sizePath,
	   /*[in]*/ const char *	lpszAbsPath,
	   /*[in]*/ const char *	lpszRelPath,
	   /*[in]*/ const char *	lpszExtension);

#if defined(MIKTEX_WINDOWS)
public:
  PathName &
  Set (/*[in]*/ const char * lpszDrive,
       /*[in]*/ const char * lpszAbsPath,
       /*[in]*/ const char * lpszRelPath, 
       /*[in]*/ const char * lpszExtension)
  {
    Combine (GetBuffer(),
	     GetCapacity(),
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
  Set (/*[in]*/ const char * lpszAbsPath,
       /*[in]*/ const char * lpszRelPath)
  {
    Combine (GetBuffer(),
	     GetCapacity(),
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
  Set (/*[in]*/ const char * lpszAbsPath,
       /*[in]*/ const char * lpszRelPath, 
       /*[in]*/ const char * lpszExtension)
  {
    Combine (GetBuffer(),
	     GetCapacity(),
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
    Combine (GetBuffer(),
	     GetCapacity(),
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
    Combine (GetBuffer(),
	     GetCapacity(),
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
    char fileName[BufferSizes::MaxPath];
    GetFileName (fileName);
    Base::Set (fileName);
    return (*this);
  }

  /// Sets this PathName object equal to the current (working) directory.
  /// @return Returns a reference to this object.
public:
  MIKTEXCORETHISAPI(PathName &)
  SetToCurrentDirectory ();

  /// Sets this PathName object equal to the temporary directory.
  /// @return Returns a reference to this object.
public:
  MIKTEXCORETHISAPI(PathName &)
  SetToTempDirectory ();

  /// Sets this PathName object equal to the name of a temporary file.
  /// @return Returns a reference to this object.
public:
  MIKTEXCORETHISAPI(PathName &)
  SetToTempFile ();

public:
  MIKTEXCORETHISAPI(PathName &)
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

public:
  PathName &
  Canonicalize ()
  {
    Convert (ConvertPathNameFlags::Canonicalize);
    return (*this);
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
  HasExtension (/*[in]*/ const char * lpszExtension)
    const
  {
    const char * lpszExt = GetExtension();
    return (lpszExt != 0
	    && (PathName::Compare(lpszExt, lpszExtension) == 0));
  }

  /// Gets the file name extension.
  /// @param[out] lpszExtension The string buffer to be filled with the
  /// file name extension.
  /// @return Returns the string buffer.
public:
  char *
  GetExtension (/*[out]*/ char *	lpszExtension)
    const
  {
    Split (Get(),
	   0, 0,
	   0, 0,
	   lpszExtension, BufferSizes::MaxPath);
    return (lpszExtension);
  }

  /// Gets the file name extension.
  /// @return Returns the file name extension. Returns 0, if the path name
  /// has no file name extension.
public:
  MIKTEXCORETHISAPI(const char *)
  GetExtension ()
    const;

  /// Sets the file name extension.
  /// @param lpszExtension The file name extension to set.
  /// @param override Indicates whether an existing file name extension
  /// shall be overridden.
  /// @return Returns a reference to this object.
public:
  MIKTEXCORETHISAPI(PathName &)
  SetExtension (/*[in]*/ const char *	lpszExtension,
		/*[in]*/ bool		override);

  /// Sets the file name extension.
  /// @param lpszExtension The file name extension to set. Can be 0,
  /// if the extension is to be removed.
  /// @return Returns a reference to this object.
public:
  PathName &
  SetExtension (/*[in]*/ const char * lpszExtension)
  {
    return (SetExtension(lpszExtension, true));
  }

public:
  PathName &
  AppendExtension (/*[in]*/ const char * lpszExtension)
  {
    Base::Append (lpszExtension);
    return (*this);
  }

public:
  PathName &
  RemoveExtensions ()
  {
    while (GetExtension())
      {
	SetExtension (0);
      }
  }

  /// Checks to see whether this path name ends with a directory delimiter.
  /// @return Returns true if the last character is a directory delimiter.
public:
  bool
  EndsWithDirectoryDelimiter ()
    const
  {
    size_t l = GetLength();
    return (l > 0
	    && (IsDirectoryDelimiter(Base::operator[](l - 1))));
  }

  /// Appends a character string to this path name.
  /// @param lpsz The null-terminated character string to add.
  /// @param appendDirectoryDelimiter Indicates whether a directory delimiter
  /// shall be appended before the string.
  /// @return Returns a reference to this object.
public:
  PathName &
  Append (/*[in]*/ const char *	lpsz,
	  /*[in]*/ bool		appendDirectoryDelimiter)
  {
    if (appendDirectoryDelimiter && ! Empty())
      {
	AppendDirectoryDelimiter ();
      }
    Base::Append (lpsz);
    return (*this);
  }

  /// Appends a path name component to this path name.
  /// @param lpszComponent The null-terminated component to add.
  /// @return Returns a reference to this object.
public:
  PathName &
  AppendComponent (/*[in]*/ const char * lpszComponent)
  {
    return (Append(lpszComponent, true));
  }

  /// Appends a path name component to this path name.
  /// @param lpszComponent The null-terminated component to add.
  /// @return Returns a reference to this object.
public:
  PathName &
  operator+= (/*[in]*/ const char * lpszComponent)
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
  operator+= (/*[in]*/ const std::basic_string<char> & component)
  {
    return (AppendComponent(component.c_str()));
  }

  /// Cuts off the last component from the path name.
  /// @return Returns a reference to this object.
public:
  MIKTEXCORETHISAPI(PathName &)
  CutOffLastComponent (/*[in]*/ bool allowSelfCutting);

  /// Cuts off the last component from the path name.
  /// @return Returns a reference to this object.
public:
  PathName &
  CutOffLastComponent ()
  {
    return (CutOffLastComponent(false));
  }

  /// Makes sure that this path name ends with a directory delimiter.
  /// @return Returns a reference to this object.
public:
  MIKTEXCORETHISAPI(PathName &)
  AppendDirectoryDelimiter ();

#if defined(MIKTEX_WINDOWS)
public:
  MIKTEXCORETHISAPI(PathName &)
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
  bool
  IsExplicitlyRelative ()
    const
  {
    if ((*this)[0] == '.')
    {
      return (IsDirectoryDelimiter((*this)[1])
	      || ((*this)[1] == '.'
	          && IsDirectoryDelimiter((*this)[2])));
    }
    else
    {
      return (false);
    }
  }

public:
  /// Compares the prefixes of two path names.
  /// @param lpszPath1 The first path name.
  /// @param lpszPath1 The second path name.
  /// @param count Number of characters to compare.
  /// @return Returns -1, if the first prefix compares lesser than the second.
  /// Returns 0, if both prefixes compare equal.
  /// Returns 1, if the first prefix compares greater than the second.
  static
  MIKTEXCORECEEAPI(int)
  Compare (/*[in]*/ const char *	lpszPath1,
	   /*[in]*/ const char *	lpszPath2,
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
  MIKTEXCORECEEAPI(int)
  Compare (/*[in]*/ const char *	lpszPath1,
	   /*[in]*/ const char *	lpszPath2);

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
  MIKTEXCORECEEAPI(bool)
  Match (/*[in]*/ const char *	lpszPattern,
	 /*[in]*/ const char *	lpszPath);

public:
  static
  bool
  Match (/*[in]*/ const char *		lpszPattern,
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

   Argv
   _________________________________________________________________________ */

class Argv
{
public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  Argv ();

public:
  virtual
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~Argv ();

public:
  MIKTEXCORETHISAPI(void)
  Build (/*[in]*/ const char *	lpszFileName,
	 /*[in]*/ const char *	lpszArguments);

public:
  MIKTEXCORETHISAPI(void)
  Append (/*[in]*/ const char *	lpszArguments);

public:
  const char * const *
  GetArgv ()
    const
  {
    return (&argv[0]);
  }

public:
  int
  GetArgc ()
    const
  {
    MIKTEX_ASSERT (argv.size() > 0);
    return (static_cast<int>(argv.size() - 1));
  }

public:
  const char *
  operator[] (/*[in]*/ size_t idx)
    const
  {
    MIKTEX_ASSERT (idx < argv.size());
    return (argv[idx]);
  }

private:
  std::vector<char *> argv;
};

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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  CommandLineBuilder ();

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  CommandLineBuilder (/*[in]*/ const CommandLineBuilder & other);

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~CommandLineBuilder ();

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  CommandLineBuilder (/*[in]*/ const char *	lpszArgument);

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  CommandLineBuilder (/*[in]*/ const char *	lpszArgument1,
		      /*[in]*/ const char *	lpszArgument2);

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  CommandLineBuilder (/*[in]*/ const char *	lpszArgument1,
		      /*[in]*/ const char *	lpszArgument2,
		      /*[in]*/ const char *	lpszArgument3);

public:
  MIKTEXCORETHISAPI(CommandLineBuilder &)
  operator= (/*[in]*/ const CommandLineBuilder & other);

public:
  MIKTEXCORETHISAPI(void)
  SetOptionConvention (/*[in]*/ OptionConvention optionConvention);

public:
  MIKTEXCORETHISAPI(void)
  Clear ();

public:
  MIKTEXCORETHISAPI(void)
  AppendArgument (/*[in]*/ const char * lpszArgument);

public:
  void
  AppendArgument (/*[in]*/ const std::string & argument)
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
  MIKTEXCORETHISAPI(void)
  AppendUnquoted (/*[in]*/ const char * lpszText);

public:
  MIKTEXCORETHISAPI(void)
  AppendArguments (/*[in]*/ int				argc,
		   /*[in]*/ const char * const *	argv);

public:
  MIKTEXCORETHISAPI(void)
  AppendArguments (/*[in]*/ const std::vector<std::string> & argv);

public:
  MIKTEXCORETHISAPI(void)
  AppendArguments (/*[in]*/ const Argv & argv);

public:
  MIKTEXCORETHISAPI(void)
  AppendOption (/*[in]*/ const char * lpszOption,
		/*[in]*/ const char * lpszValue);

public:
  void
  AppendOption (/*[in]*/ const char *		lpszOption,
		/*[in]*/ const PathName &	value)
  {
    AppendOption (lpszOption, value.Get());
  }

public:
  void
  AppendOption (/*[in]*/ const char *		lpszOption,
		/*[in]*/ const std::string &	value)
  {
    AppendOption (lpszOption, value.c_str());
  }

public:
  void
  AppendOption (/*[in]*/ const char * lpszOption)
  {
    AppendOption (lpszOption, 0);
  }
  
public:
  MIKTEXCORETHISAPI(void)
  AppendStdoutRedirection (/*[in]*/ const char *	lpszPath,
			   /*[in]*/ bool		append);

public:
  void
  AppendStdoutRedirection (/*[in]*/ const PathName &	path,
			   /*[in]*/ bool		append)
  {
    AppendStdoutRedirection (path.Get(), append);
  }

public:
  MIKTEXCORETHISAPI(void)
  AppendStdinRedirection (/*[in]*/ const char *	lpszPath);

public:
  void
  AppendStdinRedirection (/*[in]*/ const PathName &	path)
  {
    AppendStdinRedirection (path.Get());
  }

public:
  void
  AppendStdoutRedirection (/*[in]*/ const char * lpszPath)
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
  MIKTEXCORETHISAPI(const char *)
  Get ();

private:
  void * p;
};

/* _________________________________________________________________________

   Uri
   _________________________________________________________________________ */

class
Uri
{
public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  Uri ();

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  Uri (/*[in]*/ const Uri & other);

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~Uri ();

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  Uri (/*[in]*/ const char *	lpszUri);

public:
  MIKTEXCORETHISAPI(Uri &)
  operator= (/*[in]*/ const Uri & other);

public:
  MIKTEXCORETHISAPI(std::string)
  GetHost ();

public:
  MIKTEXCORETHISAPI(int)
  GetPort ();

public:
  MIKTEXCORETHISAPI(std::string)
  GetUserInfo ();

public:
  MIKTEXCORETHISAPI(std::string)
  GetScheme ();

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

/* _________________________________________________________________________

   CommandLineBuilder
   _________________________________________________________________________ */

class MiKTeXConfigurationEnum
{
public:
  enum EnumType {
    None,
    Regular,
    Direct,
    Portable,
  };
};

typedef EnumWrapper<MiKTeXConfigurationEnum> MiKTeXConfiguration;

struct StartupConfig
{
public:
  StartupConfig ()
    : config (MiKTeXConfiguration::None)
  {
  }

public:
  std::string userRoots;

public:
  PathName userInstallRoot;

public:
  PathName userDataRoot;

public:
  PathName userConfigRoot;

public:
  std::string commonRoots;

public:
  PathName commonDataRoot;

public:
  PathName commonConfigRoot;

public:
  PathName commonInstallRoot;

public:
  MiKTeXConfiguration config;
};

/* _________________________________________________________________________

   C++ Constants
   _________________________________________________________________________ */

const unsigned INVALID_ROOT_INDEX = static_cast<unsigned>(-1);

// TEXMF root paths shouldn't be longer than this
const size_t MAX_TEXMF_ROOT_PATH = 100;

// relative TEXMF paths shouldn't be longer than this
const size_t MAX_TEXMF_REL_PATH = 150;

/* _________________________________________________________________________

   SpecialPath
   _________________________________________________________________________ */

/// Special path enum class
class SpecialPathEnum
{
public:
  enum EnumType {
    /// The common installation root directory.
    CommonInstallRoot,

    // The installation directory of the user.
    UserInstallRoot,

    /// The common data root directory.
    CommonDataRoot,

    /// The data root directory of the user.
    UserDataRoot,

    CommonConfigRoot,
    UserConfigRoot,
    InstallRoot,

    /// The prefered data root directory. For a common MiKTeX setup
    /// this is equivalent to the common data directory. For a private
    /// MiKTeX setup this is equivalent to the user data directory.
    DataRoot,

    ConfigRoot,

    /// The bin directory.
    BinDirectory,

    /// The internal bin directory.
    InternalBinDirectory,

    /// The portable root directory.
    PortableRoot,

    /// The portable mount directory.
    PortableMount,
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
  /// @param lpszTrigger File name that triggered the auto-installer.
  /// @param[out] installRoot Installation root directory.
  /// @return Returns true, if the package has been installed.
public:
  virtual
  bool
  MIKTEXTHISCALL
  InstallPackage (/*[in]*/ const char *	lpszPackageName,
		  /*[in]*/ const char *	lpszTrigger,
		  /*[out]*/ PathName &	installRoot)
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
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  ReadDirectory (/*[in]*/ const char *	lpszPath,
		 /*[out]*/ char * *	ppSubDirNames,
		 /*[out]*/ char * *	ppFileNames,
		 /*[out]*/ char * *	ppFileNameInfos)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProgress (/*[in]*/ unsigned		level,
	      /*[in]*/ const char *	lpszDirectory)
    = 0;
};

class
MIKTEXNOVTABLE
IEnumerateFndbCallback
{
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnFndbItem (/*[in]*/ const char *	lpszPath,
	      /*[in]*/ const char *	lpszName,
	      /*[in]*/ const char *	lpszInfo,
	      /*[in]*/ bool		isDirectory)
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
#if defined(MIKTEX_WINDOWS)
    WindowsCommandScriptFile,
#endif
    OTF,
    SFD,
    CMAP
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
  MIKTEXTHISCALL
  AddRef ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Release ()
    = 0;

protected:
  virtual
  MIKTEXTHISCALL
  ~Cfg ()
    = 0;

public:
  /// Creates a new Cfg object. The caller is responsible for deleting
  /// the object.
  static
  MIKTEXCORECEEAPI(Cfg *)
  Create ();

  /// Tests whether the Cfg object is empty.
public:
  virtual
  bool
  MIKTEXTHISCALL
  Empty ()
    = 0;

  /// Calculates the MD5 value of this Cfg object.
public:
  virtual
  MD5
  MIKTEXTHISCALL
  GetDigest ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetOriginalDigest (/*[out]*/ MD5 & originalDigest)
    = 0;

  /// Gets a configuration value.
public:
  virtual
  std::string
  MIKTEXTHISCALL
  GetValue (/*[in]*/ const char *	lpszKey,
	    /*[in]*/ const char *	lpszValueName)
    = 0;

  /// Gets a configuration value.
public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetValue (/*[in]*/ const char *	lpszKey,
	       /*[in]*/ const char *	lpszValueName,
	       /*[out]*/ char *		lpszValue,
	       /*[in,out]*/ size_t &	bufSize)
    = 0;
  
  /// Gets a configuration value.
public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetValue (/*[in]*/ const char *	lpszKey,
	       /*[in]*/ const char *	lpszValueName,
	       /*[out]*/ std::string &	value)
    = 0;

  /// Gets a configuration value.
public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetValue (/*[in]*/ const char *	lpszKey,
	       /*[in]*/ const char *	lpszValueName,
	       /*[out]*/ PathName &	path)
    = 0;

  /// Marks this Cfg object as modified.
public:
  virtual
  void
  MIKTEXTHISCALL
  SetModified (/*[in]*/ bool b)
    = 0;

  /// Tests whether this Cfg object has been modified.
public:
  virtual
  bool
  MIKTEXTHISCALL
  IsModified ()
    = 0;

  /// Sets a configuration value.
public:
  virtual
  void
  MIKTEXTHISCALL
  PutValue (/*[in]*/ const char *	lpszKey,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const char *	lpszValue)
    = 0;
    
  /// Sets a configuration value.
public:
  virtual
  void
  MIKTEXTHISCALL
  PutValue (/*[in]*/ const char *	lpszKey,
	    /*[in]*/ const char *	lpszValueName,
	    /*[in]*/ const char *	lpszValue,
	    /*[in]*/ const char *	lpszDocumentation,
	    /*[in]*/ bool		commentedOut)
    = 0;
    
  /// Reads configuration values from a file.
public:
  virtual
  void
  MIKTEXTHISCALL
  Read (/*[in]*/ const PathName &	path)
    = 0;

  /// Write configuration settings into a file.
public:
  virtual
  void
  MIKTEXTHISCALL
  Write (/*[in]*/ const PathName & 	path)
    = 0;

  /// Write configuration settings into a file.
public:
  virtual
  void
  MIKTEXTHISCALL
  Write (/*[in]*/ const PathName & 	path,
	 /*[in]*/ const char *		lpszHeader)
    = 0;

  /// Starts a key iteration.
public:
  virtual
  char *
  MIKTEXTHISCALL
  FirstKey (/*[out]*/ char *	lpsz,
	    /*[in]*/ size_t		bufSize)
    = 0;

  /// Continues a key iteration.
public:
  virtual
  char *
  MIKTEXTHISCALL
  NextKey (/*[out]*/ char *	lpsz,
	   /*[in]*/ size_t	bufSize)
    = 0;

  /// Deletes a key.
public:
  virtual
  void
  MIKTEXTHISCALL
  DeleteKey (/*[in]*/ const char * lpszKey)
    = 0;

  /// Starts a value iteration.
public:
  virtual
  char *
  MIKTEXTHISCALL
  FirstValue (/*[in]*/ const char *	lpszKey,
	      /*[out]*/ char *		lpszValueName,
	      /*[in]*/ size_t		valueNameSize)
    = 0;

  /// Continues a value iteration.
public:
  virtual
  char *
  MIKTEXTHISCALL
  NextValue (/*[out]*/ char *	lpszValueName,
	     /*[in]*/ size_t		valueNameSize)
    = 0;

  /// Deletes a value (experimental).
public:
  virtual
  void
  MIKTEXTHISCALL
  DeleteValue (/*[in]*/ const char *	lpszKey,
	       /*[in]*/ const char * lpszValueName)
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
  MIKTEXCORECEEAPI(void)
  Create (/*[in]*/ const PathName &	path);

  /// Gets the current (working) directory.
public:
  static
  MIKTEXCORECEEAPI(PathName)
  GetCurrentDirectory ();

  /// Sets the current (working) directory.
public:
  static
  MIKTEXCORECEEAPI(void)
  SetCurrentDirectory (/*[in]*/ const PathName &	path);

  /// Deletes a directory.
public:
  static
  MIKTEXCORECEEAPI(void)
  Delete (/*[in]*/ const PathName &	path);

  /// Deletes a directory (recursively).
public:
  static
  MIKTEXCORECEEAPI(void)
  Delete (/*[in]*/ const PathName &	path,
	  /*[in]*/ bool			recursive);

  /// Tests if a directory exists.
public:
  static
  MIKTEXCORECEEAPI(bool)
  Exists (/*[in]*/ const PathName &	path);

  /// Set directory time-stamps.
public:
  static
  MIKTEXCORECEEAPI(void)
  SetTimes (/*[in]*/ const PathName &		path,
	    /*[in]*/ time_t			creationTime,
	    /*[in]*/ time_t			lastAccessTime,
	    /*[in]*/ time_t			lastWriteTime);
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~StdioFile ();

public:
  FILE *
  Get ()
    const
  {
    return (pFile);
  }

public:
  MIKTEXCORETHISAPI(void)
  AddRef ();

public:
  MIKTEXCORETHISAPI(void)
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

   Stream
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
Stream
{
public:
  virtual
  MIKTEXTHISCALL
  ~Stream ()
    = 0;

public:
  virtual
  size_t
  MIKTEXTHISCALL
  Read (/*[out]*/ void *	pBytes,
	/*[in]*/ size_t		count)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Write (/*[in]*/ const void *		pBytes,
	 /*[in]*/ size_t		count)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Seek (/*[in]*/ long		offset,
	/*[in]*/ SeekOrigin	origin)
    = 0;

public:
  virtual
  long
  MIKTEXTHISCALL
  GetPosition ()
    = 0;
};

/* _________________________________________________________________________

   FileStream
   _________________________________________________________________________ */

/// FileStream class. Encapsulates a stdio file stream pointer (FILE*).
class
MIKTEXCORETYPEAPI(FileStream)
  : public Stream
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
  virtual
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~FileStream ();

public:
  MIKTEXCORETHISAPI(void)
  Attach (/*[in]*/ FILE * pFile);

  /// Closes the encapsulated stream (except for stdin/stdout/stderr).
public:
  MIKTEXCORETHISAPI(void)
  Close ();

public:
  virtual
  MIKTEXCORETHISAPI(size_t)
  Read (/*[out]*/ void *	pBytes,
	/*[in]*/ size_t		count);

public:
  virtual
  MIKTEXCORETHISAPI(void)
  Write (/*[in]*/ const void *		pBytes,
	 /*[in]*/ size_t		count);

public:
  virtual
  MIKTEXCORETHISAPI(void)
  Seek (/*[in]*/ long		offset,
	/*[in]*/ SeekOrigin	origin);

public:
  virtual
  MIKTEXCORETHISAPI(long)
  GetPosition ();

public:
  MIKTEXCORETHISAPI(void)
  SetBinary ();

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

   BZip2Stream
   _________________________________________________________________________ */

class
BZip2Stream : public Stream
{
public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  BZip2Stream (/*[in]*/ const FileStream &	fileStream,
	       /*[in]*/ bool			reading);

public:
  virtual
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~BZip2Stream ();

public:
  virtual
  MIKTEXCORETHISAPI(size_t)
  Read (/*[out]*/ void *	pBytes,
	/*[in]*/ size_t		count);

public:
  virtual
  MIKTEXCORETHISAPI(void)
  Write (/*[in]*/ const void *		pBytes,
	 /*[in]*/ size_t		count);

public:
  virtual
  void
  MIKTEXTHISCALL
  Seek (/*[in]*/ long		offset,
	/*[in]*/ SeekOrigin	origin);

public:
  virtual
  MIKTEXCORETHISAPI(long)
  GetPosition ();

public:
  MIKTEXCORETHISAPI(void)
  Close ();

private:
  bool reading;

private:
  void * pBzFile;
};

/* _________________________________________________________________________

   LzmaStream
   _________________________________________________________________________ */

class
LzmaStream : public Stream
{
public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  LzmaStream (/*[in]*/ const FileStream &	fileStream,
	      /*[in]*/ bool			reading);

public:
  virtual
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~LzmaStream ();

public:
  virtual
  MIKTEXCORETHISAPI(size_t)
  Read (/*[out]*/ void *	pBytes,
	/*[in]*/ size_t		count);

public:
  virtual
  MIKTEXCORETHISAPI(void)
  Write (/*[in]*/ const void *		pBytes,
	 /*[in]*/ size_t		count);

public:
  virtual
  void
  MIKTEXTHISCALL
  Seek (/*[in]*/ long		offset,
	/*[in]*/ SeekOrigin	origin);

public:
  virtual
  MIKTEXCORETHISAPI(long)
  GetPosition ();

public:
  MIKTEXCORETHISAPI(void)
  Close ();

private:
  bool reading;

private:
  void * p;
};

/* _________________________________________________________________________

   StreamReader
   _________________________________________________________________________ */

class
StreamReader
{
public:
  StreamReader ()
  {
  }

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  StreamReader (/*[in]*/ const PathName & path);

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  StreamReader (/*[in]*/ const char * lpszPath);

public:
  StreamReader (/*[in]*/ bool readStdin)
    : stream (readStdin ? stdin : 0)
  {
  }

public:
  void
  Attach (/*[in]*/ FILE * pFile)
  {
    stream.Attach (pFile);
  }

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~StreamReader ();

public:
  MIKTEXCORETHISAPI(bool)
  ReadLine (/*[out]*/ std::string & line);

public:
  MIKTEXCORETHISAPI(void)
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~StreamWriter ();

public:
  MIKTEXCORETHISAPI(void)
  Write (/*[in]*/ char ch);

public:
  MIKTEXCORETHISAPI(void)
  Write (/*[in]*/ const std::string & line);

public:
  MIKTEXCORETHISAPI(void)
  WriteLine (/*[in]*/ const std::string & line);

public:
  MIKTEXCORETHISAPI(void)
  WriteLine ();

public:
  MIKTEXCOREEXPORT
  void
  MIKTEXCEECALL
  WriteFormatted (/*[in]*/ const char *	lpszFormat,
		  /*[in]*/		...);

public:
  MIKTEXCOREEXPORT
  void
  MIKTEXCEECALL
  WriteFormattedLine (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...);

public:
  MIKTEXCORETHISAPI(void)
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
  MIKTEXCORECEEAPI(void)
  Delete (/*[in]*/ const PathName &	path);
  
public:
  static
  MIKTEXCORECEEAPI(void)
  Delete (/*[in]*/ const PathName &	path,
	  /*[in]*/ bool			tryHarder);

public:
  static
  MIKTEXCORECEEAPI(void)
  Delete (/*[in]*/ const PathName &	path,
	  /*[in]*/ bool			tryHarder,
	  /*[in]*/ bool			updateFndb);

  /// Tests if a file exists.
public:
  static
  MIKTEXCORECEEAPI(bool)
  Exists (/*[in]*/ const PathName &	path);
    
public:
  static
  MIKTEXCORECEEAPI(FileAttributes)
  GetAttributes (/*[in]*/ const PathName &	path);

public:
  static
  MIKTEXCORECEEAPI(unsigned long)
  GetNativeAttributes (/*[in]*/ const PathName &	path);

  /// Gets the size of a file.
public:
  static
  MIKTEXCORECEEAPI(size_t)
  GetSize (/*[in]*/ const PathName & path);

  /// Renames a file.
public:
  static
  MIKTEXCORECEEAPI(void)
  Move (/*[in]*/ const PathName &	source,
	/*[in]*/ const PathName &	dest);

  /// Copies a file.
public:
  static
  MIKTEXCORECEEAPI(void)
  Copy (/*[in]*/ const PathName &	source,
	/*[in]*/ const PathName &	dest);

  /// Copies a file (preserving attributes).
public:
  static
  MIKTEXCORECEEAPI(void)
  Copy (/*[in]*/ const PathName &	source,
	/*[in]*/ const PathName &	dest,
	/*[in]*/ bool			preserveAttributes);

  /// Opens a text stream on a file.
public:
  static
  MIKTEXCORECEEAPI(FILE *)
  Open (/*[in]*/ const PathName &	path,
	/*[in]*/ FileMode		mode,
	/*[in]*/ FileAccess		access);

  /// Opens a stream on a file.
public:
  static
  MIKTEXCORECEEAPI(FILE *)
  Open (/*[in]*/ const PathName &	path,
	/*[in]*/ FileMode		mode,
	/*[in]*/ FileAccess		access,
	/*[in]*/ bool			isTextFile);

  /// Opens a stream on a file.
public:
  static
  MIKTEXCORECEEAPI(FILE *)
  Open (/*[in]*/ const PathName &	path,
	/*[in]*/ FileMode		mode,
	/*[in]*/ FileAccess		access,
	/*[in]*/ bool			isTextFile,
	/*[in]*/ FileShare		share);

  /// Sets file attributes.
public:
  static
  MIKTEXCORECEEAPI(void)
  SetAttributes (/*[in]*/ const PathName &		path,
		 /*[in]*/ FileAttributes		attributes);

  /// Sets native file attributes.
public:
  static
  MIKTEXCORECEEAPI(void)
  SetNativeAttributes (/*[in]*/ const PathName &	path,
		       /*[in]*/ unsigned long		nativeAttributes);

public:
  static
  MIKTEXCORECEEAPI(void)
  SetTimes (/*[in]*/ int			fd,
	    /*[in]*/ time_t			creationTime,
	    /*[in]*/ time_t			lastAccessTime,
	    /*[in]*/ time_t			lastWriteTime);

public:
  static
  MIKTEXCORECEEAPI(void)
  SetTimes (/*[in]*/ FILE *			stream,
	    /*[in]*/ time_t			creationTime,
	    /*[in]*/ time_t			lastAccessTime,
	    /*[in]*/ time_t			lastWriteTime);

  /// Set file time-stamps.
public:
  static
  MIKTEXCORECEEAPI(void)
  SetTimes (/*[in]*/ const PathName &		path,
	    /*[in]*/ time_t			creationTime,
	    /*[in]*/ time_t			lastAccessTime,
	    /*[in]*/ time_t			lastWriteTime);

  /// Gets file time-stamps.
public:
  static
  MIKTEXCORECEEAPI(void)
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
  MIKTEXCORECEEAPI(std::vector<unsigned char>)
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
  MIKTEXCORECEEAPI(void)
  Add (/*[in]*/ const char *	lpszPath);

public:
  static
  void
  Add (/*[in]*/ const PathName &	path)
  {
    Add (path.Get());
  }

public:
  static
  MIKTEXCORECEEAPI(void)
  Add (/*[in]*/ const char *	lpszPath,
       /*[in]*/ const char *	lpszFileNameInfo);

public:
  static
  void
  Add (/*[in]*/ const PathName &	path,
       /*[in]*/ const char *		lpszFileNameInfo)
  {
    Add (path.Get(), lpszFileNameInfo);
  }

public:
  static
  MIKTEXCORECEEAPI(bool)
  Create (/*[in]*/ const char *			lpszFndbPath,
	  /*[in]*/ const char *			lpszRootPath,
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
  MIKTEXCORECEEAPI(bool)
  Create (/*[in]*/ const char *			lpszFndbPath,
	  /*[in]*/ const char *			lpszRootPath,
	  /*[in]*/ ICreateFndbCallback *	pCallback,
	  /*[in]*/ bool				enableStringPooling,
	  /*[in]*/ bool				storeFileNameInfo);

public:
  static
  MIKTEXCORECEEAPI(bool)
  Enumerate (/*[in]*/ const char *		lpszPath,
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
  MIKTEXCORECEEAPI(bool)
  Refresh (/*[in]*/ const PathName &		path,
	   /*[in]*/ ICreateFndbCallback *	pCallback);

public:
  static
  MIKTEXCORECEEAPI(bool)
  Refresh (/*[in]*/ ICreateFndbCallback *	pCallback);

public:
  static
  MIKTEXCORECEEAPI(void)
  Remove (/*[in]*/ const char *	lpszPath);

public:
  static
  void
  Remove (/*[in]*/ const PathName & path)
  {
    Remove (path.Get());
  }

public:
  static
  MIKTEXCORECEEAPI(bool)
  FileExists (/*[in]*/ const PathName &	path);
};

/* _________________________________________________________________________

   DirectoryEntry
   _________________________________________________________________________ */

/// Contains information about a directory entry.
struct DirectoryEntry
{
  /// Name of the entry.
  std::string name;
#if defined(MIKTEX_WINDOWS)
  /// Native (Unicode) name of the entry.
  std::wstring wname;
#endif
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
  MIKTEXTHISCALL
  ~DirectoryLister ()
    = 0;

  /// Closes this DirectoryLister.
public:
  virtual
  void
  MIKTEXTHISCALL
  Close ()
    = 0;

  /// Gets the next directory entry.
  /// @param[out] The directory entry to be filled.
  /// @return Returns true, if an entry could be retrieved. Returns, if
  /// there are no more directory entries.
public:
  virtual
  bool
  MIKTEXTHISCALL
  GetNext (/*[out]*/ DirectoryEntry & direntry)
    = 0;
  
  /// Gets the next directory entry.
  /// @param[out] The directory entry to be filled.
  /// @return Returns true, if an entry could be retrieved. Returns, if
  /// there are no more directory entries.
public:
  virtual
  bool
  MIKTEXTHISCALL
  GetNext (/*[out]*/ DirectoryEntry2 & direntry2)
    = 0;
  
  /// Creates a DirectoryLister object. The caller is responsible for deleting
  /// the object.
  /// @param directory Path to the directory.
  /// @return Returns the DirectoryLister object.
public:
  static
  MIKTEXCORECEEAPI(DirectoryLister *)
  Open (/*[in]*/ const PathName & directory);

  /// Creates a DirectoryLister object. The caller is responsible for deleting
  /// the object.
  /// @param directory Path to the directory.
  /// @param lpszPattern A filter pattern (e.g. "*.txt").
  /// @return Returns the DirectoryLister object.
public:
  static
  MIKTEXCORECEEAPI(DirectoryLister *)
  Open (/*[in]*/ const PathName &	directory,
	/*[in]*/ const char *		lpszPattern);
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
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  Open (/*[in]*/ const char *	lpszPath,
	/*[in]*/ bool		readWrite)
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
  MIKTEXTHISCALL
  Close ()
    = 0;

  /// Resizes the memory-mapped file.
  /// @param newSize The new size of the memory-mapped file.
  /// @return Returns a pointer to the block of memory.
public:
  virtual
  void *
  MIKTEXTHISCALL
  Resize (/*[in]*/ size_t newSize)
    = 0;

  /// Gets a pointer to rhe block of memory.
  /// @return Returns a pointer to the block of memory.
public:
  virtual
  void *
  MIKTEXTHISCALL
  GetPtr ()
    = 0;

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetName ()
    = 0;

  /// Gets the size of the memory-mapped file.
  /// @return Returns the size (in bytes).
public:
  virtual
  size_t
  MIKTEXTHISCALL
  GetSize ()
    = 0;

  /// Flushes the memory-mapped file to disk.
public:
  virtual
  void
  MIKTEXTHISCALL
  Flush ()
    = 0;

  /// Creates a new MemoryMappedFile object. The caller is responsible
  /// for deleting the object.
  /// @return Returns the MemoryMappedFile object.
public:
  static
  MIKTEXCORECEEAPI(MemoryMappedFile *)
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
  MIKTEXTHISCALL
  ~Thread ()
    = 0;

  /// Waits for the thread to terminate.
public:
  virtual
  void
  MIKTEXTHISCALL
  Join ()
    = 0;

  /// Creates a new Thread object and starts a thread.
  /// @param function The function to execute in the new thread.
  /// @param argument The argument to be passed to the function.
  /// @return Returns a pointer to the Thread object. The caller is
  /// responsoble for deleting the object.
public:
  static
  MIKTEXCORECEEAPI(Thread *)
  Start (/*[in]*/ void (MIKTEXCALLBACK * function) (void *),
	 /*[in]*/ void * argument);

  /// Suspends the running thread.
  /// @param milliseconds Duration (in milliseconds) the thread
  /// should be suspended.
public:
  static
  MIKTEXCORECEEAPI(void)
  Sleep (/*[in]*/ int milliseconds);

public:
  static
  MIKTEXCORECEEAPI(bool)
  IsCurrentThread (/*[in]*/ Thread * pThread);
};

/* _________________________________________________________________________

   ProcessStartInfo
   _________________________________________________________________________ */

/// Proces start info struct.
struct ProcessStartInfo
{
  /// Arguments to be passed to the process.
  std::string Arguments;

  /// Path name to be executed.
  std::string FileName;

#if defined(MIKTEX_WINDOWS)
  /// Output file stream for stderr redirection.
  FILE * StandardError;
#endif

  /// Input file stream for stdin redirection.
  FILE * StandardInput;

#if defined(MIKTEX_WINDOWS)
  /// Output file stream for stdout redirection.
  FILE * StandardOutput;
#endif

  /// Indicates whether error output shall be written to a pipe.
  bool RedirectStandardError;

  /// Indicates whether input shall be read from a pipe.
  bool RedirectStandardInput;

  /// Indicates whether output shall be written to a pipe.
  bool RedirectStandardOutput;

  /// Working directory for the process.
  std::string WorkingDirectory;

  ProcessStartInfo ()
    : RedirectStandardError (false),
      RedirectStandardInput (false),
      RedirectStandardOutput (false),
#if defined(MIKTEX_WINDOWS)
      StandardError (0),
      StandardOutput (0),
#endif
      StandardInput (0)
  {
  }

  ProcessStartInfo (/*[in]*/ const char * lpszFileName)
    : FileName (lpszFileName),
      RedirectStandardError (false),
      RedirectStandardInput (false),
      RedirectStandardOutput (false),
#if defined(MIKTEX_WINDOWS)
      StandardError (0),
      StandardOutput (0),
#endif
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
  MIKTEXTHISCALL
  ~Process ()
    = 0;

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  get_StandardInput ()
    = 0;

public:
  /// Gets the redirected output stream.
  /// @return Returns the redirected output stream. Returns 0, if
  /// the output was not redirected.
  virtual
  FILE *
  MIKTEXTHISCALL
  get_StandardOutput ()
    = 0;

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  get_StandardError ()
    = 0;

  /// Waits for the process.
public:
  virtual
  void
  MIKTEXTHISCALL
  WaitForExit ()
    = 0;

  /// Waits for the process.
public:
  virtual
  bool
  MIKTEXTHISCALL
  WaitForExit (/*[in]*/ int milliseconds)
    = 0;

  /// Gets the exit code of the process.
  /// The process must have exited.
public:
  virtual
  int
  MIKTEXTHISCALL
  get_ExitCode ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Close ()
    = 0;

  /// Start the system shell to execute a command.
  /// @param lpszCommandLine Command to be executed.
public:
  static
  MIKTEXCORECEEAPI(void)
  StartSystemCommand (/*[in]*/ const char *	lpszCommandLine);

  /// Executes the system shell to execute a command.
  /// @param lpszCommandLine Command to be executed.
  /// @return Returns true, if the command exited successfully.
public:
  static
  MIKTEXCORECEEAPI(bool)
  ExecuteSystemCommand (/*[in]*/ const char *	lpszCommandLine);

  /// Executes the system shell to execute a command.
  /// @param lpszCommandLine Command to be executed.
  /// @param pCallback Callback interface.
  /// @param pExitCode To be filled with the exit code of the command.
  /// @param lpszWorkingDirectory Working directory for the command.
  /// @return Returns true, if the process exited successfully, or
  /// if pExitCode isn't null.
public:
  static
  MIKTEXCORECEEAPI(bool)
  ExecuteSystemCommand (/*[in]*/ const char *	lpszCommandLine,
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
  MIKTEXCORECEEAPI(bool)
  ExecuteSystemCommand (/*[in]*/ const char *		lpszCommandLine,
			/*[out]*/ int *			pExitCode,
			/*[in]*/ IRunProcessCallback *	pCallback,
			/*[in]*/ const char *		lpszWorkingDirectory);

  /// Executes a process.
  /// @param lpszFileName The name of an executable file to run in the process.
  /// @param lpszArguments The command-line arguments to pass when starting
  /// the process.
public:
  static
  MIKTEXCORECEEAPI(void)
  Run (/*[in]*/ const PathName &	fileName,
       /*[in]*/ const char *		lpszArguments);

public:
  static
  void
  Run (/*[in]*/ const PathName &	fileName)
  {
    Run (fileName, 0);
  }
  
public:
  static
  MIKTEXCORECEEAPI(void)
  Run (/*[in]*/ const PathName &	fileName,
       /*[in]*/ const char *		lpszArguments,
       /*[int]*/ IRunProcessCallback *	pCallback);

public:
  static
  MIKTEXCORECEEAPI(bool)
  Run (/*[in]*/ const PathName &	fileName,
       /*[in]*/ const char *		lpszArguments,
       /*[int]*/ IRunProcessCallback *	pCallback,
       /*[out]*/ int *			pExitCode,
       /*[in]*/  const char *		lpszWorkingDirectory);

public:
  static
  MIKTEXCORECEEAPI(bool)
  Run (/*[in]*/ const PathName &	fileName,
       /*[in]*/ const char *		lpszArguments,
       /*[out]*/ void *			pBuf,
       /*[in,out]*/ size_t *		pBufSize,
       /*[out]*/ int *			pExitCode);

public:
  static
  MIKTEXCORECEEAPI(Process *)
  Start (/*[in]*/ const ProcessStartInfo & startinfo);

public:
  static
  MIKTEXCORECEEAPI(void)
  Start (/*[in]*/ const char *	lpszFileName,
	 /*[in]*/ const char *	lpszArguments,
	 /*[in]*/ FILE *	pFileStandardInput,
	 /*[out]*/ FILE **	ppFileStandardInput,
	 /*[out]*/ FILE **	ppFileStandardOutput,
	 /*[out]*/ FILE **	ppFileStandardError,
	 /*[in]*/ const char *	lpszWorkingDirectory);

public:
  static
  void
  Start (/*[in]*/ const PathName &	fileName,
	 /*[in]*/ const char *		lpszArguments,
	 /*[in]*/ FILE *		pFileStandardInput,
	 /*[out]*/ FILE **		ppFileStandardInput,
	 /*[out]*/ FILE **		ppFileStandardOutput,
	 /*[out]*/ FILE **		ppFileStandardError,
	 /*[in]*/ const char *		lpszWorkingDirectory)
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
	 /*[in]*/ const char *		lpszArguments)
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
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ScratchDirectory ();

public:
  virtual
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  ~ScratchDirectory ();

public:
  static
  MIKTEXCORECEEAPI(PathName)
  Create ();

public:
  MIKTEXCORETHISAPI(void)
  Enter (/*[in]*/ const char *	lpszPrefix);

public:
  void
  Enter ()
  {
    Enter ("mik");
  }

public:
  MIKTEXCORETHISAPI(void)
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
  MIKTEXTHISCALL
  ~TraceStream ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Close ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Enable (/*[in]*/ bool		enable)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsEnabled ()
    = 0;

public:
  static
  MIKTEXCORECEEAPI(TraceStream *)
  Open (/*[in]*/ const char *	lpszName);

public:
  static
  MIKTEXCORECEEAPI(void)
  SetTraceFlags (/*[in]*/ const char *	lpszOptions);

public:
  virtual
  void
  MIKTEXTHISCALL
  Write (/*[in]*/ const char *	lpszFacility,
	 /*[in]*/ const char *	lpszText)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  WriteLine (/*[in]*/ const char *	lpszFacility,
	     /*[in]*/ const char *	lpszText)
    = 0;

public:
  virtual
  void
  MIKTEXCEECALL
  WriteFormattedLine (/*[in]*/ const char *	lpszFacility,
		      /*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
    = 0;

public:
  static
  MIKTEXCORECEEAPI(void)
  TraceLastCRTError (/*[in]*/ const char *	lpszCrtFunction,
		     /*[in]*/ const char *	lpszInfo,
		     /*[in]*/ const char *	lpszSourceFile,
		     /*[in]*/ int		lpszSourceLine);

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCORECEEAPI(void)
  TraceLastWin32Error (/*[in]*/ const char *	lpszWindowsFunction,
		       /*[in]*/ const char *	lpszInfo,
		       /*[in]*/ const char *	lpszSourceFile,
		       /*[in]*/ int		sourceLine);
#endif

public:
  virtual
  void
  MIKTEXTHISCALL
  VTrace (/*[in]*/ const char *	lpszFacility,
	  /*[in]*/ const char *	lpszFormat,
	  /*[in]*/ va_list	arglist)
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
  std::string fileTypeString;
  std::string fileNameExtensions;
  std::string applicationName;
  std::string searchPath;
  std::string envVarNames;
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
  std::string	fileName;
  std::string	packageName;
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
#if defined(MIKTEX_WINDOWS)
      /// Initialize the COM library.
      InitializeCOM = 2,
#endif
      /// start in administration mode
      AdminMode = 4,
    };
  };

  /// Init flags enum.
  typedef EnumWrapper<InitFlagsEnum> InitFlags;

  /// Init info struct.
public:
  class InitInfo
    : public StructChecker<InitInfo>
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
    InitInfo (/*[in]*/ const char *	lpszProgramInvocationName)
      : flags (InitFlags::None),
	programInvocationName (lpszProgramInvocationName)
    {
    }
    /// Initializes a new init info struct.
    /// @param lpszProgramInvocationName Name of the invoked program.
    /// @param flags Init flags.
  public:
    InitInfo (/*[in]*/ const char *	lpszProgramInvocationName,
	      /*[in]*/ unsigned long	flags)
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
    /// Adds init flags.
  public:
    void
    AddFlags (/*[in]*/ unsigned long	flags)
    {
      this->flags |= flags;
    }
    /// Removes init flags.
  public:
    void
    RemoveFlags (/*[in]*/ unsigned long	flags)
    {
      this->flags &= ~ flags;
    }
    /// Sets the name of the invoked program.
  public:
    void
    SetProgramInvocationName
    (/*[in]*/ const char * lpszProgramInvocationName)
    {
      programInvocationName = lpszProgramInvocationName;
    }
    /// Gets the name of the invoked program.
  public:
    std::string
    GetProgramInvocationName ()
      const
    {
      return (programInvocationName);
    }
    /// Sets the name of the game.
  public:
    void
    SetTheNameOfTheGame
    (/*[in]*/ const char * lpszTheNameOfTheGame)
    {
      theNameOfTheGame = lpszTheNameOfTheGame;
    }
    /// Gets the name of the game.
  public:
    std::string
    GetTheNameOfTheGame ()
      const
    {
      return (theNameOfTheGame);
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
    /// Enables trace streams.
  public:
    void
    SetTraceFlags (/*[out]*/ const char * lpszTraceFlags)
    {
      traceFlags = lpszTraceFlags;
    }
    /// Gets enabled trace streams.
  public:
    std::string
    GetTraceFlags ()
      const
    {
      return (traceFlags);
    }
  private:
    unsigned long flags;
  private:
    std::string programInvocationName;
  private:
    std::string theNameOfTheGame;
  private:
    StartupConfig startupConfig;
  private:
    std::string traceFlags;
  };

public:
  static
  MIKTEXCORECEEAPI(Session *)
  Get ();

public:
  static
  MIKTEXCORECEEAPI(Session *)
  TryGet ();

public:
  static
  MIKTEXCORECEEAPI(Session *)
  Get (/*[in]*/ const InitInfo & initInfo);

public:
  static
  MIKTEXCORECEEAPI(void)
  Release (/*[in]*/ Session * pSession);

protected:
  virtual
  MIKTEXTHISCALL
  ~Session ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  PushAppName (/*[in]*/ const char *	lpszAppName)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  PushBackAppName (/*[in]*/ const char *	lpszAppName)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  AddInputDirectory (/*[in]*/ const char *	lpszPath,
		     /*[in]*/ bool		atEnd)
    = 0;

  /// Gets a special path.
  /// @param specialPath Specifies special path.
  /// @return Returns a path name.
public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetSpecialPath (/*[in]*/ SpecialPath		specialPath)
    = 0;

  /// Gets the number of root directories.
public:
  virtual
  unsigned
  MIKTEXTHISCALL
  GetNumberOfTEXMFRoots ()
    = 0;

public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetRootDirectory (/*[in]*/ unsigned	r)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsCommonRootDirectory (/*[in]*/ unsigned r)
    = 0;

  /// Gets the path name of the virtual MPM TEXMF root.
public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetMpmRootPath ()
    = 0;

  /// Gets the path name of the MPM file name database.
public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetMpmDatabasePathName ()
    = 0;

public:
  virtual
  unsigned
  MIKTEXTHISCALL
  TryDeriveTEXMFRoot (/*[in]*/ const PathName & path)
    = 0;

public:
  virtual
  unsigned
  MIKTEXTHISCALL
  DeriveTEXMFRoot (/*[in]*/ const PathName & path)
    = 0;

  /// Finds a file name database file.
  /// @param r File name database ID.
  /// @param path Path name object buffer to be filled.
public:
  virtual
  bool
  MIKTEXTHISCALL
  FindFilenameDatabase (/*[in]*/ unsigned	r,
			/*[out]*/ PathName &	path)
    = 0;

  /// Gets the path name of a file name database file.
  /// @param r File name database ID.
  /// @param path Path name object buffer to be filled.
public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetFilenameDatabasePathName (/*[in]*/ unsigned	r)
    = 0;

  /// Unloads the file name database.
  /// @return Returns true, if the file name database could be unloaded.
  /// Returns false, if the database is still in use.
public:
  virtual
  bool
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  SplitTEXMFPath (/*[in]*/ const PathName &	path,
		  /*[out]*/ PathName &		root,
		  /*[out]*/ PathName &		relative)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  RegisterRootDirectories (/*[in]*/ const std::string &	roots)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  RegisterRootDirectories (/*[in]*/ const StartupConfig &	startupConfig,
			   /*[in]*/ bool			sessionOnly)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsMiKTeXDirect ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsMiKTeXPortable ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetMETAFONTMode (/*[in]*/ unsigned		idx,
		   /*[out]*/ MIKTEXMFMODE *	pMode)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  DetermineMETAFONTMode (/*[in]*/ unsigned		dpi,
			 /*[out]*/ MIKTEXMFMODE *	pMode)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetConfigValue (/*[in]*/ const char *	lpszSectionName,
		     /*[in]*/ const char *	lpszValueName,
		     /*[out]*/ std::string &	value)
    = 0;

public:
  virtual
  std::string
  MIKTEXTHISCALL
  GetConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ const char *	lpszDefaultValue)
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ int		defaultValue)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ bool		defaultValue)
    = 0;

public:
  virtual
  TriState
  MIKTEXTHISCALL
  GetConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ TriState	defaultValue)
    = 0;

public:
  virtual
  char
  MIKTEXTHISCALL
  GetConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ char		defaultValue)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetUserConfigValue (/*[in]*/ const char *	lpszSectionName,
		      /*[in]*/ const char *	lpszValueName,
		      /*[in]*/ const char *	lpszValue)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetUserConfigValue (/*[in]*/ const char *	lpszSectionName,
		      /*[in]*/ const char *	lpszValueName,
		      /*[in]*/ bool			value)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetUserConfigValue (/*[in]*/ const char *	lpszSectionName,
		      /*[in]*/ const char *	lpszValueName,
		      /*[in]*/ int			value)
    = 0;

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  OpenFile (/*[in]*/ const char *	lpszPath,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile)
    = 0;

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  TryOpenFile (/*[in]*/ const char *	lpszPath,
	       /*[in]*/ FileMode	mode,
	       /*[in]*/ FileAccess	access,
	       /*[in]*/ bool		isTextFile)
    = 0;

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  OpenFile (/*[in]*/ const char *	lpszPath,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile,
	    /*[in]*/ FileShare		share)
    = 0;

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  TryOpenFile (/*[in]*/ const char *	lpszPath,
	       /*[in]*/ FileMode	mode,
	       /*[in]*/ FileAccess	access,
	       /*[in]*/ bool		isTextFile,
	       /*[in]*/ FileShare	share)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  CloseFile (/*[in]*/ FILE *	pFile)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsOutputFile (/*[in]*/ const FILE *	pFile)
    = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXTHISCALL
  IsFileAlreadyOpen (/*[in]*/ const char *	lpszFileName)
    = 0;
#endif

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  void
  MIKTEXTHISCALL
  ScheduleFileRemoval (/*[in]*/ const char *	lpszFileName)
    = 0;
#endif

public:
  virtual
  bool
  MIKTEXTHISCALL
  StartFileInfoRecorder (/*[in]*/ bool bRecordPackageNames)
    = 0;

public:
  virtual
  std::vector<FileInfoRecord>
  MIKTEXTHISCALL
  GetFileInfoRecords ()
    = 0;

public:
  virtual
  FileType
  MIKTEXTHISCALL
  DeriveFileType (/*[in]*/ const char * lpszPath)
    = 0;

  /// Finds a file.
  /// @param lpszFileName Name of the file to be found.
  /// @param lpszPathList Search path. See the MiKTeX manual.
  /// @param[out] path The path name to be filled with the result.
public:
  virtual
  bool
  MIKTEXTHISCALL
  FindFile (/*[in]*/ const char *	lpszFileName,
	    /*[in]*/ const char *	lpszPathList,
	    /*[out]*/ PathName &	path)
    = 0;

public:
  bool
  FindFile (/*[in]*/ const PathName &	fileName,
	    /*[in]*/ const char *	lpszPathList,
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
  MIKTEXTHISCALL
  FindFile (/*[in]*/ const char *	lpszFileName,
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
  MIKTEXTHISCALL
  FindPkFile (/*[in]*/ const char *	lpszFontName,
	      /*[in]*/ const char *	lpszMode,
	      /*[in]*/ int		dpi,
	      /*[out]*/ PathName &	path)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindTfmFile (/*[in]*/ const char *	lpszFontName,
	       /*[out]*/ PathName &	path,
	       /*[in]*/ bool		create)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetFindFileCallback (/*[in]*/ IFindFileCallback *	pCallback)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SplitFontPath (/*[in]*/ const char *	lpszFontPath,
		 /*[out]*/ char *	lpszFontType,
		 /*[out]*/ char *	lpszSupplier,
		 /*[out]*/ char *	lpszTypeface,
		 /*[out]*/ char *	lpszFontName,
		 /*[out]*/ char *	lpszPointSize)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetFontInfo (/*[in]*/ const char *	lpszFontName,
	       /*[out]*/ char *		lpszSupplier,
	       /*[out]*/ char *		lpszTypeface,
	       /*[out]*/ double *	lpGenSize)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  GetGhostscript (/*[out]*/ char *		lpszPath,
		  /*[out]*/ unsigned long *	pVersionNumber)
    = 0;

public:
  virtual
  std::string
  MIKTEXTHISCALL
  GetExpandedSearchPath (/*[in]*/ FileType	fileType)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindGraphicsRule (/*[in]*/ const char *	lpszFrom,
		    /*[in]*/ const char *	lpszTo,
		    /*[out]*/ char *		lpszRule,
		    /*[in]*/ size_t		bufSize)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  ConvertToBitmapFile (/*[in]*/ const char *		lpszSourceFileName,
		       /*[out]*/ char *			lpszDestFileName,
		       /*[in]*/ IRunProcessCallback *	pCallback)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  EnableFontMaker (/*[in]*/ bool enable)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetMakeFontsFlag ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  MakeMakePkCommandLine (/*[in]*/ const char *	lpszFontName,
			 /*[in]*/ int		dpi,
			 /*[in]*/ int		baseDpi,
			 /*[in]*/ const char *	lpszMfMode,
			 /*[out]*/ PathName &	fileName,
			 /*[out]*/ char *	lpszArguments,
			 /*[in]*/ size_t	bufSize)
    = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  int
  MIKTEXTHISCALL
  RunBatch (/*[in]*/ int		argc,
	    /*[in]*/ const char **	argv)
    = 0;
#endif

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  int
  MIKTEXTHISCALL
  RunBatch (/*[in]*/ const char *	lpszName,
	    /*[in]*/ const char *	lpszArguments)
    = 0;
#endif

public:
  virtual
  int
  MIKTEXTHISCALL
  RunPerl (/*[in]*/ int			argc,
	   /*[in]*/ const char **	argv)
    = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXTHISCALL
  ShowManualPageAndWait (/*[in]*/ HWND		hWnd,
			 /*[in]*/ unsigned long	topic)
    = 0;
#endif

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetNextFileTypeInfo (/*[in]*/ unsigned	index,
		       /*[out]*/ FileTypeInfo &	fileTypeInfo)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetFormatInfo (/*[in]*/ unsigned	index,
		 /*[out]*/ FormatInfo &	formatInfo)
    = 0;

public:
  virtual
  FormatInfo
  MIKTEXTHISCALL
  GetFormatInfo (/*[in]*/ const char *	lpszKey)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetFormatInfo (/*[in]*/ const char *	lpszKey,
		    /*[out]*/ FormatInfo &	formatInfo)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  DeleteFormatInfo (/*[in]*/ const char *	lpszKey)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetFormatInfo (/*[in]*/ const FormatInfo &	formatInfo)
    = 0;

public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetMyLocation (/*[in]*/ bool canonicalized)
    = 0;

public:
  PathName
  GetMyLocation ()
  {
    return (GetMyLocation(false));
  }

public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetMyPrefix ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  RunningAsAdministrator ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetAdminMode (/*[in]*/ bool adminMode)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsAdminMode ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetPaperSizeInfo (/*[in]*/ int		idx,
		    /*[out]*/ PaperSizeInfo &	paperSize)
    = 0;

public:
  virtual
  PaperSizeInfo
  MIKTEXTHISCALL
  GetPaperSizeInfo (/*[in]*/ const char *	lpszDvipsName)
    = 0;

public:
  virtual
  PolicyFlags
  MIKTEXTHISCALL
  GetPolicyFlags ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetDefaultPaperSize (/*[in]*/ const char * lpszDvipsName)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryCreateFromTemplate (/*[in]*/ const PathName & path)
    = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXTHISCALL
  RunningAsPowerUser ()
    = 0;
#endif

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsUserAnAdministrator ()
    = 0;

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXTHISCALL
  IsUserAPowerUser ()
    = 0;
#endif

public:
  virtual
  void
  MIKTEXTHISCALL
  ConfigureFile (/*[in]*/ const PathName & pathIn,
		 /*[in]*/ const PathName & pathOut)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  ConfigureFile (/*[in]*/ const PathName & pathRel)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  SetTheNameOfTheGame (/*[in]*/ const char * lpszTheNameOfTheGame)
    = 0;

public:
  virtual
  std::string
  MIKTEXTHISCALL
  GetLocalFontDirectories ()
    = 0;

public:
  static
  MIKTEXCOREEXPORT
  MIKTEXNORETURN
  void
  MIKTEXCEECALL
  FatalCrtError (/*[in]*/ const char *	lpszCrtFunction,
		 /*[in]*/ const char *	lpszInfo,
		 /*[in]*/ const char *	lpszSourceFile,
		 /*[in]*/ int		lpszSourceLine);
  
public:
  static
  MIKTEXCOREEXPORT
  MIKTEXNORETURN
  void
  MIKTEXCEECALL
  FatalCrtError (/*[in]*/ const char *	lpszCrtFunction,
		 /*[in]*/ int		errorCode,
		 /*[in]*/ const char *	lpszInfo,
		 /*[in]*/ const char *	lpszSourceFile,
		 /*[in]*/ int		lpszSourceLine);
  
public:
  static
  MIKTEXCOREEXPORT
  MIKTEXNORETURN
  void
  MIKTEXCEECALL
  FatalMiKTeXError (/*[in]*/ const char *	lpszMiktexFunction,
		    /*[in]*/ const char *	lpszMessage,
		    /*[in]*/ const char *	lpszInfo,
		    /*[in]*/ const char *	lpszSourceFile,
		    /*[in]*/ int		lpszSourceLine);

#if defined(MIKTEX_WINDOWS)  
public:
  static
  MIKTEXCOREEXPORT
  MIKTEXNORETURN
  void
  MIKTEXCEECALL
  FatalWindowsError (/*[in]*/ const char *	lpszWindowsFunction,
		     /*[in]*/ const char *	lpszInfo,
		     /*[in]*/ const char *	lpszSourceFile,
		     /*[in]*/ int		lpszSourceLine);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  static
  MIKTEXCOREEXPORT
  MIKTEXNORETURN
  void
  MIKTEXCEECALL
  FatalWindowsError (/*[in]*/ const char *	lpszWindowsFunction,
		     /*[in]*/ unsigned long	errorCode,
		     /*[in]*/ const char *	lpszInfo,
		     /*[in]*/ const char *	lpszSourceFile,
		     /*[in]*/ int		lpszSourceLine);
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
    MIKTEX_ASSERT (useExisting);
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

   BasicCsvList
   _________________________________________________________________________ */

template<typename CharType, int BUFSIZE>
class BasicCsvList
  : protected CharBuffer<CharType, BUFSIZE>
{
public:
  BasicCsvList ()
    : lpszNext (0),
      lpszCurrent (0)
  {
  }

public:
  BasicCsvList (/*[in]*/ const CharType *	lpszValueList,
		/*[in]*/ CharType		separator)
    : CharBuffer<CharType, BUFSIZE> (lpszValueList)
  {
    if (lpszValueList == 0 || *lpszValueList == 0)
      {
	lpszNext = lpszCurrent = 0;
      }
    else
      {
	this->separator = separator;
	lpszCurrent = CharBuffer<CharType, BUFSIZE>::GetBuffer();
	lpszNext =
	  const_cast<CharType*>(StrChr(CharBuffer<CharType, BUFSIZE>::Get(),
				       separator));
	if (lpszNext != 0)
	  {
	    *lpszNext++ = 0;
	  }
      }
  }

public:
  const CharType *
  operator++ ()
  {
    if (lpszNext != 0 && *lpszNext != 0)
      {
	lpszCurrent = lpszNext;
	lpszNext = const_cast<CharType*>(StrChr(lpszNext, separator));
	if (lpszNext != 0)
	  {
	    *lpszNext++ = 0;
	  }
      }
    else
      {
	lpszCurrent = 0;
      }
    return (GetCurrent());
  }

public:
  const CharType *
  GetCurrent ()
    const
  {
    return (lpszCurrent);
  }

private:
  BasicCsvList (/*[in]*/ const BasicCsvList & rhs);

private:
  BasicCsvList &
  operator= (/*[in]*/ const BasicCsvList & rhs);

private:
  CharType separator;

private:
  CharType * lpszNext;

private:
  CharType * lpszCurrent;
};

typedef BasicCsvList<char, 512> CSVList;
typedef BasicCsvList<wchar_t, 512> CSVListW;

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
    MIKTEX_ASSERT (! autoInit);
    Initialize_ ();
  }

private:
  MIKTEXCORETHISAPI(void)
  Initialize_ ();

public:
  void
  Delete ()
  {
    MIKTEX_ASSERT (! autoInit);
    Delete_ ();
  }
  
private:
  MIKTEXCORETHISAPI(void)
  Delete_ ();

public:
  MIKTEXCORETHISAPI(void)
  Enter ();

public:
  void
  Lock ()
  {
    Enter ();
  }

public:
  MIKTEXCORETHISAPI(void)
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
    MiKTeX::Core::AutoCriticalSectionMonitor_		\
      miktex_h__criticalSectionMonitor_ (pcritsec);

#define MIKTEX_END_CRITICAL_SECTION()			\
  }

#define MIKTEX_DEFINE_LOCK(name)					\
  MiKTeX::Core::CriticalSectionMonitor name##__CriticalSectionMonitor_

#define MIKTEX_LOCK(name)				\
  {							\
    MiKTeX::Core::AutoCriticalSectionMonitor_		\
      auto_critsec_ (&name##__CriticalSectionMonitor_);

#define MIKTEX_UNLOCK()				\
  }

/* _________________________________________________________________________

   Tokenizer
   _________________________________________________________________________ */

class Tokenizer
  : protected CharBuffer<char, 512>
{
protected:
  typedef CharBuffer<char, 512> Base;

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  Tokenizer (/*[in]*/ const char * lpsz,
	     /*[in]*/ const char * lpszDelim);

public:
  MIKTEXCORETHISAPI(void)
  SetDelim (const char * lpszDelim);

public:
  const char *
  GetCurrent ()
    const
  {
    return (lpszCurrent == 0 || *lpszCurrent == 0 ? 0 : lpszCurrent);
  }

public:
  MIKTEXCORETHISAPI(const char *)
  operator++ ();

public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
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
  const char * lpszCurrent;

private:
  char * lpszNext;

private:
  void * pDelims;
};

/* _________________________________________________________________________

   Quoter
   _________________________________________________________________________ */

template<typename CharType>
class Quoter : public CharBuffer<CharType>
{
public:
  Quoter (/*[in]*/ const CharType * lpsz)
  {
    Quote (lpsz);
  }

public:
  Quoter (/*[in]*/ const std::basic_string<CharType> & str)
  {
    Quote (str.c_str());
  }

public:
  Quoter (/*[in]*/ const PathName & path)
  {
    Quote (path.Get());
  }

private:
  void
  Quote (/*[in]*/ const CharType * lpsz)
  {
    bool needQuotes = (*lpsz == 0 || StrChr(lpsz, ' ') != 0);
    if (needQuotes)
      {
	CharBuffer<CharType>::Append ('"');
      }
    CharBuffer<CharType>::Append (lpsz);
    if (needQuotes)
      {
	CharBuffer<CharType>::Append ('"');
      }
  }
};

/* _________________________________________________________________________

   IsDirectoryDelimiter
   _________________________________________________________________________ */

MIKTEXINLINE
bool
IsDirectoryDelimiter (/*[in]*/ int ch)
{
  return (PathName::IsDirectoryDelimiter(ch));
}

/* _________________________________________________________________________

   PathNameParser
   _________________________________________________________________________ */

class PathNameParser
{
public:
  MIKTEXCOREEXPORT
  MIKTEXTHISCALL
  PathNameParser (/*[in]*/ const char * lpszPath);

public:
  const char *
  GetCurrent ()
    const
  {
    return (*lpszCurrent == 0 ? 0 : lpszCurrent);
  }

public:
  MIKTEXCORETHISAPI(const char *)
  operator++ ();

private:
  PathNameParser (/*[in]*/ const PathNameParser & rhs);

private:
  PathNameParser &
  operator= (/*[in]*/ const PathNameParser & rhs);

private:
  char * lpszCurrent;

private:
  char * lpszNext;

private:
  char buffer[BufferSizes::MaxPath];
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
  operator() (/*[in]*/ const std::string & str)
    const
  {
    size_t h = 0;
    for (const char * lpsz = str.c_str(); *lpsz != 0; ++ lpsz)
      {
	char ch = *lpsz;
	if (ch >= 'a' && ch <= 'z')
	  {
	    ch = 'A' + (ch - 'a');
	  }
	h = 5 * h + ch;
      }
    return (h);
  }

public:
  bool
  operator() (/*[in]*/ const std::string & str1,
	      /*[in]*/ const std::string & str2)
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
  size_t
  operator() (/*[in]*/ const std::string & str)
    const
  {
    return (PathName(str).GetHash());
  }
  
public:
  bool
  operator() (/*[in]*/ const std::string & str1,
	      /*[in]*/ const std::string & str2)
    const
  {
    return (PathName::Compare(str1.c_str(), str2.c_str()) < 0);
  }
};

/* _________________________________________________________________________

   NUMTOSTR
   _________________________________________________________________________ */

class NumberString
{
private:
  enum { BUFSIZE = 30 };

private:
  MIKTEXCORETHISAPI(void)
  Init (/*[in]*/ long long	num,
	/*[in]*/ bool		hex);

private:
  MIKTEXCORETHISAPI(void)
  Init (/*[in]*/ double		num);

public:
  NumberString (/*[in]*/ long long	num,
		/*[in]*/ bool		hex)
  {
    Init (num, hex);
  }

public:
  NumberString (/*[in]*/ double		num,
		/*[in]*/ bool		hex)
  {
    MIKTEX_ASSERT (! hex);
    Init (num);
  }

public:
  NumberString (/*[in]*/ int		num,
		/*[in]*/ bool		hex)
  {
    Init (num, hex);
  }

public:
  NumberString (/*[in]*/ long		num,
		/*[in]*/ bool		hex)
  {
    Init (num, hex);
  }

public:
  NumberString (/*[in]*/ unsigned	num,
		/*[in]*/ bool		hex)
  {
    Init (num, hex);
  }

public:
  NumberString (/*[in]*/ unsigned long	num,
		/*[in]*/ bool		hex)
  {
    Init (num, hex);
  }

public:
  const char *
  GetBuffer ()
    const
  {
    return (buffer);
  }

private:
  char buffer[BUFSIZE];
};

#define NUMTOSTR(num) MiKTeX::Core::NumberString(num, false).GetBuffer()
#define NUMTOHEXSTR(num) MiKTeX::Core::NumberString(num, true).GetBuffer()

/* _________________________________________________________________________

   AutoResource
   _________________________________________________________________________ */

template<class HandleType>
HandleType
InvalidHandleValue ()
{
  return (0);
}

template<class HandleType,
	 class Destroyer>
class AutoResource
{
public:
  AutoResource (/*[in]*/ HandleType handle = InvalidHandleValue<HandleType>())
    : handle (handle)
  {
  }

public:
  ~AutoResource ()
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
  HandleType
  Get ()
    const
  {
    return (handle);
  }

public:
  void
  Attach (/*[in]*/ HandleType handle)
  {
    this->handle = handle;
  }

public:
  HandleType
  Detach ()
  {
    HandleType handle = Get();
    Attach (InvalidHandleValue<HandleType>());
    return (handle);
  }

public:
  void
  Reset (/*[in]*/ HandleType handle)
  {
    if (Get() != InvalidHandleValue<HandleType>())
      {
	Destroyer() (Detach());
      }
    Attach (handle);
  }

public:
  void
  Reset ()
  {
    Reset (InvalidHandleValue<HandleType>());
  }

public:
  HandleType *
  operator & ()
  {
    return (&handle);
  }

private:
  HandleType handle;
};

/* _________________________________________________________________________

   AutoResource2
   _________________________________________________________________________ */

template<class HandleType1,
	 class HandleType2,
	 class Destroyer>
class AutoResource2
{
public:
  AutoResource2
  (/*[in]*/ HandleType1 handle1 = InvalidHandleValue<HandleType1>(),
   /*[in]*/ HandleType2 handle2 = InvalidHandleValue<HandleType2>())
    : handle1 (handle1),
      handle2 (handle2)
  {
  }

public:
  ~AutoResource2 ()
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
  HandleType2
  Get ()
  {
    return (handle2);
  }

public:
  void
  Attach (/*[in]*/ HandleType1 handle1,
	  /*[in]*/ HandleType2 handle2)
  {
    this->handle1 = handle1;
    this->handle2 = handle2;
  }

public:
  HandleType2
  Detach ()
  {
    HandleType2 handle2 = Get();
    Attach (InvalidHandleValue<HandleType1>(),
	    InvalidHandleValue<HandleType2>());
    return (handle2);
  }

public:
  void
  Reset ()
  {
    if (Get() != InvalidHandleValue<HandleType2>())
      {
	HandleType1 handle1 = this->handle1;
	HandleType2 handle2 = this->handle2;
	this->handle1 = InvalidHandleValue<HandleType1>();
	this->handle2 = InvalidHandleValue<HandleType2>();
	Destroyer() (handle1, handle2);
      }
  }

public:
  HandleType2 *
  operator & ()
  {
    return (&handle2);
  }

private:
  HandleType1 handle1;

private:
  HandleType2 handle2;
};

/* _________________________________________________________________________

   AutoFILE
   _________________________________________________________________________ */

class fclose_
{
public:
  void
  operator() (/*[in]*/ FILE * pFile)
  {
    if (fclose(pFile) != 0)
      {
	Session::FatalCrtError ("fclose",
				0,
				__FILE__,
				__LINE__);
      }
  }
};

typedef AutoResource<FILE *, fclose_> AutoFILE;

/* _________________________________________________________________________

   AutoErrorMode
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class SetErrorMode_
{
public:
  void
  operator() (/*[in]*/ unsigned int mode)
  {
    SetErrorMode (mode);
  }
};

typedef AutoResource<unsigned int, SetErrorMode_> AutoErrorMode;
#endif

/* _________________________________________________________________________

   AutoCrtDbgFlag
   _________________________________________________________________________ */

#if defined(_MSC_VER)
class CrtSetDbgFlag_
{
public:
  void
  operator() (/*[in]*/ int flag)
  {
    _CrtSetDbgFlag (flag);
  }
};

typedef AutoResource<int, CrtSetDbgFlag_> AutoCrtDbgFlag;
#endif

/* _________________________________________________________________________

   AutoHANDLE
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
template<>
inline
HANDLE
InvalidHandleValue<HANDLE> ()
{
  return (INVALID_HANDLE_VALUE);
}

class CloseHandle_
{
public:
  void
  operator() (/*[in]*/ HANDLE h)
  {
    if (! CloseHandle(h))
      {
	Session::FatalWindowsError ("CloseHandle",
				    0,
				    __FILE__,
				    __LINE__);
      }
  }
};

typedef AutoResource<HANDLE, CloseHandle_> AutoHANDLE;
#endif

/* _________________________________________________________________________

   AutoHKEY
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class RegCloseKey_
{
public:
  void
  operator() (/*[in]*/ HKEY hkey)
  {
    long result = RegCloseKey(hkey);
    if (result != ERROR_SUCCESS)
      {
	Session::FatalWindowsError ("RegCloseKey",
				    0,
				    __FILE__,
				    __LINE__);
      }
  }
};

typedef AutoResource<HKEY, RegCloseKey_> AutoHKEY;
#endif

/* _________________________________________________________________________

   AutoSid
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class FreeSid_
{
public:
  void
  operator() (/*[in]*/ PSID psid)
  {
    if (FreeSid(psid) != 0)
      {
	Session::FatalWindowsError ("FreeSid",
				    0,
				    __FILE__,
				    __LINE__);
      }
  }
};

typedef AutoResource<PSID, FreeSid_> AutoSid;
#endif

/* _________________________________________________________________________

   AutoGlobal
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class GlobalUnlock_
{
public:
  void
  operator() (/*[in]*/ HGLOBAL hMem)
  {
    if (GlobalUnlock(hMem) == 0 && GetLastError() != NO_ERROR)
      {
	Session::FatalWindowsError ("GlobalUnlock",
				    0,
				    __FILE__,
				    __LINE__);
      }
  }
};

typedef AutoResource<void *, GlobalUnlock_> AutoGlobal;
#endif

/* _________________________________________________________________________

   AutoGlobalMemory
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class GlobalFree_
{
public:
  void
  operator() (/*[in]*/ HGLOBAL hMem)
  {
    if (GlobalFree(hMem) != 0)
      {
	Session::FatalWindowsError ("GlobalFree",
				    0,
				    __FILE__,
				    __LINE__);
      }
  }
};

typedef AutoResource<void *, GlobalFree_> AutoGlobalMemory;
#endif

/* _________________________________________________________________________

   AutoLocalMemory
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class LocalFree_
{
public:
  void
  operator() (/*[in]*/ void * ptr)
  {
    if (LocalFree(ptr) != 0)
      {
	Session::FatalWindowsError ("LocalFree",
				    0,
				    __FILE__,
				    __LINE__);
      }
  }
};

typedef AutoResource<void *, LocalFree_> AutoLocalMemory;
typedef AutoResource<void *, LocalFree_> AutoLocalMem;
#endif

/* _________________________________________________________________________

   AutoCoTaskMem
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && defined(WINOLEAPI)
class CoTaskMemFree_
{
public:
  void
  operator() (/*[in]*/ void * p)
  {
    CoTaskMemFree (p);
  }
};

typedef AutoResource<void *, CoTaskMemFree_> AutoCoTaskMem;
#endif

/* _________________________________________________________________________

   AutoSysString
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && defined(WINOLEAUTAPI)
class SysFreeString_
{
public:
  void
  operator() (/*[in]*/ BSTR bstr)
  {
    SysFreeString (bstr);
  }
};

typedef AutoResource<BSTR, SysFreeString_> AutoSysString;
#endif

/* _________________________________________________________________________

   AutoClosePrinter
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && defined(PRINTER_ENUM_DEFAULT)
class ClosePrinter_
{
public:
  void
  operator() (/*[in]*/ HANDLE hPrinter)
  {
    if (! ClosePrinter(hPrinter))
      {
	Session::FatalWindowsError ("Closeprinter",
				    0,
				    __FILE__,
				    __LINE__);
      }
  }
};

typedef AutoResource<HANDLE, ClosePrinter_> AutoClosePrinter;
#endif

/* _________________________________________________________________________

   AutoUnmapViewOfFile
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class UnmapViewOfFile_
{
public:
  void
  operator() (/*[in]*/ void * p)
  {
    if (! UnmapViewOfFile(p))
      {
	Session::FatalWindowsError ("UnmapViewOfFile",
				    0,
				    __FILE__,
				    __LINE__);
      }
  }
};

typedef AutoResource<void *, UnmapViewOfFile_> AutoUnmapViewOfFile;
#endif

/* _________________________________________________________________________

   AutoDeleteMetaFile
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class DeleteMetaFile_
{
public:
  void
  operator() (/*[in]*/ HMETAFILE hMetaFile)
  {
    if (! DeleteMetaFile(hMetaFile) && GetVersion() < 0x80000000)
      {
	Session::FatalWindowsError ("DeleteMetaFile",
				    0,
				    __FILE__,
				    __LINE__);
      }
  }
};

typedef AutoResource<HMETAFILE, DeleteMetaFile_> AutoDeleteMetaFile;
#endif

/* _________________________________________________________________________

   AutoDdeUninitialize
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && defined(DDE_FACK)
class DdeUninitialize_
{
public:
  void
  operator() (/*[in]*/ unsigned long inst)
  {
    if (! DdeUninitialize(inst))
      {
      }
  }
};

typedef AutoResource<unsigned long, DdeUninitialize_> AutoDdeUninitialize;
#endif

/* _________________________________________________________________________

   AutoDdeFreeDataHandle
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && defined(DDE_FACK)
class DdeFreeDataHandle_
{
public:
  void
  operator() (/*[in]*/ HDDEDATA hData)
  {
    if (! DdeFreeDataHandle (hData))
      {
      }
  }
};

typedef AutoResource<HDDEDATA, DdeFreeDataHandle_> AutoDdeFreeDataHandle;
#endif

/* _________________________________________________________________________

   AutoDdeDisconnect
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && defined(DDE_FACK)
class DdeDisconnect_
{
public:
  void
  operator() (/*[in]*/ HCONV hConv)
  {
    if (! DdeDisconnect(hConv))
      {
      }
  }
};

typedef AutoResource<HCONV, DdeDisconnect_> AutoDdeDisconnect;
#endif

/* _________________________________________________________________________

   AutoDdeFreeStringHandle
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && defined(DDE_FACK)
class DdeFreeStringHandle_
{
public:
  void
  operator() (/*[in]*/ unsigned long	inst,
	      /*[in]*/ HSZ		hsz)
  {
    if (! DdeFreeStringHandle(inst, hsz))
      {
      }
  }
};

typedef
AutoResource2<unsigned long, HSZ, DdeFreeStringHandle_>
AutoDdeFreeStringHandle;
#endif

/* _________________________________________________________________________

   AutoMemoryPointer
   _________________________________________________________________________ */

class free_
{
public:
  void
  operator() (/*[in]*/ void * p)
  {
    free (p);
  }
};

typedef AutoResource<void *, free_> AutoMemoryPointer;

/* _________________________________________________________________________ */

MIKTEX_CORE_END_NAMESPACE;

#endif	// C++

#endif
