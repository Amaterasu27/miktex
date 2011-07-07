/* Update.h:							-*- C++ -*-

   Copyright (C) 2002-2011 Christian Schenk

   This file is part of the MiKTeX Update Wizard.

   The MiKTeX Update Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Update Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Update Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#pragma once

#include "resource.h"

class UpdateWizardCommandLineInfo;

#define REMOVE_FORMAT_FILES 1

/* _________________________________________________________________________

   Error Macros
   _________________________________________________________________________ */

#define INVALID_ARGUMENT(function, param)				\
  FATAL_MIKTEX_ERROR (function, T_("Invalid argument."), param)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     __FILE__,				\
			     __LINE__)

#define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,		\
			      lpszInfo,			\
			      __FILE__,		\
			      __LINE__)

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#if ! defined(UNICODE)
#  error UNICODE required
#endif

#define TU_(x) MiKTeX::Core::CharBuffer<char>(x).GetBuffer()
#define UT_(x) MiKTeX::Core::CharBuffer<wchar_t>(x).GetBuffer()

#define WU_(x) MiKTeX::Core::CharBuffer<char>(x).GetBuffer()
#define UW_(x) MiKTeX::Core::CharBuffer<wchar_t>(x).GetBuffer()

/* _________________________________________________________________________

   AutoToggle
   _________________________________________________________________________ */

template<typename T>
class AutoToggle
{
public:
  AutoToggle (/*[in,out]*/ T &	val,
	      /*[in]*/ T	newVal)
    : val (val),
      oldVal (val)
  {
    val = newVal;
  }
public:
  ~AutoToggle ()
  {
    val = oldVal;
  }
private:
  T & val;
  T oldVal;
};

#define AUTO_TOGGLE(var) AutoToggle<bool> autoToggle (var, ! var)

/* _________________________________________________________________________

   UpdateWizardApplication
   _________________________________________________________________________ */

class UpdateWizardApplication : public CWinApp
{
public:
  UpdateWizardApplication ();
  
public:
  virtual
  BOOL
  InitInstance ();

private:
  bool
  Upgrade (/*[out]*/ bool & upgrading);

public:
  DECLARE_MESSAGE_MAP();
};

/* _________________________________________________________________________

   Global Variables
   _________________________________________________________________________ */

extern PathName g_logFileName;
extern PackageManagerPtr g_pManager;
extern bool g_upgrading;

/* _________________________________________________________________________

   Protos
   _________________________________________________________________________ */

void
ULogOpen ();

void
ULogAddFile (/*[in]*/ const PathName & path);

void
ULogClose (/*[in]*/ bool doRegister = false);

void
ReportError (/*[in]*/ const MiKTeXException & e);

void
ReportError (/*[in]*/ const exception & e);

void
SplitUrl (/*[in]*/ const string &	url,
	  /*[out]*/ string &		protocol,
	  /*[out]*/ string &		host);

/* _________________________________________________________________________

   GetMainConfigDir
   _________________________________________________________________________ */

inline
PathName
GetMainConfigDir ()
{
  return (PathName((SessionWrapper(true)
		    ->GetSpecialPath(SpecialPath::InstallRoot)),
		   MIKTEX_PATH_MIKTEX_CONFIG_DIR));
}

/* _________________________________________________________________________

   GetLocalConfigDir
   _________________________________________________________________________ */

inline
PathName
GetLocalConfigDir ()
{
  return (PathName((SessionWrapper(true)
		    ->GetSpecialPath(SpecialPath::DataRoot)),
		   MIKTEX_PATH_MIKTEX_CONFIG_DIR));
}

/* _________________________________________________________________________

   IsMiKTeXPackage
   _________________________________________________________________________ */

inline
bool
IsMiKTeXPackage (/*[in]*/ const string & deploymentName)
{
  return (deploymentName.compare(0, 7, T_("miktex-")) == 0);
}
