/* miktex/App/Application.h:					-*- C++ -*-

   Copyright (C) 2005-2009 Christian Schenk
 
   This file is part of the MiKTeX App Library.

   The MiKTeX App Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX App Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX App Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(F4C0E5199356C44CBA46523020038822)
#define F4C0E5199356C44CBA46523020038822

#include <memory>
#include <string>

#include <miktex/Core/Core>
#include <miktex/PackageManager/PackageManager>

#if defined(_MSC_VER)
#  include <hash_set>
#else
#  include <set>
#endif

#include <vector>

// DLL import/export switch
#if ! defined(BDF6E2537F116547846406B5B2B65949)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define MIKTEXAPPEXPORT __declspec(dllimport)
#  else
#    define MIKTEXAPPEXPORT
#  endif
#endif

// API decoration for exported member functions
#define MIKTEXAPPTHISAPI(type) MIKTEXAPPEXPORT type MIKTEXTHISCALL
#define MIKTEXAPPCEEAPI(type) MIKTEXAPPEXPORT type MIKTEXCEECALL

#define MIKTEX_APP_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace App {

#define MIKTEX_APP_END_NAMESPACE		\
    }						\
  }

MIKTEX_APP_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   Application
   _________________________________________________________________________ */

class
Application
  : public MiKTeX::Core::IFindFileCallback,
    public MiKTeX::Packages::PackageInstallerCallback
{

  /* _______________________________________________________________________
     
     Application
     _______________________________________________________________________ */

public:

  MIKTEXAPPEXPORT
  MIKTEXTHISCALL
  Application ();

  /* _______________________________________________________________________
     
     ~Application
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPEXPORT
  MIKTEXTHISCALL
  ~Application ();

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPTHISAPI(void)
  Init (/*[in]*/ const MiKTeX::Core::Session::InitInfo & initInfo);

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPTHISAPI(void)
  Init (/*[in,out]*/ std::vector</*[const]*/ char *> & args);

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPTHISAPI(void)
  Init (/*[in]*/ const char *	lpszProgramInvocationName);

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPTHISAPI(void)
  Init (/*[in]*/ const char *	lpszProgramInvocationName,
	/*[in]*/ const char *	lpszTheNameOfTheGame);

  /* _______________________________________________________________________
     
     Finalize
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPTHISAPI(void)
  Finalize ();

  /* _______________________________________________________________________
     
     InstallPackage
     _______________________________________________________________________ */

public:
  
  virtual
  MIKTEXAPPTHISAPI(bool)
  InstallPackage (/*[in]*/ const char *			lpszPackageName,
		  /*[in]*/ const char *			lpszTrigger,
		  /*[out]*/ MiKTeX::Core::PathName &	installRoot);

  /* _______________________________________________________________________
     
     ReportLine
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPTHISAPI(void)
  ReportLine (/*[in]*/ const char * lpszLine);

  /* _______________________________________________________________________
     
     OnRetryableError
     _______________________________________________________________________ */

public:
  virtual
  MIKTEXAPPTHISAPI(bool)
  OnRetryableError (/*[in]*/ const char * lpszMessage);

  /* _______________________________________________________________________
     
     OnProgress
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPTHISAPI(bool)
  OnProgress (/*[in]*/ MiKTeX::Packages::Notification	nf);

  /* _______________________________________________________________________
     
     EnableInstaller
     _______________________________________________________________________ */
  
public:

  MIKTEXAPPTHISAPI(void)
  EnableInstaller (/*[in]*/ MiKTeX::Core::TriState tri);

  /* _______________________________________________________________________
  
    GetQuietFlag
     _______________________________________________________________________ */
  
public:

  bool
  GetQuietFlag ()
    const
  {
    return (beQuiet);
  }

  /* _______________________________________________________________________
  
     SetQuietFlag
     _______________________________________________________________________ */
  
public:

  void
  SetQuietFlag (/*[in]*/ bool b)
  {
    beQuiet = b;
  }

  /* _______________________________________________________________________
  
     GetSession
     _______________________________________________________________________ */
  
public:

  MiKTeX::Core::Session *
  GetSession ()
    const
  {
    return (pSession.GetSession());
  }

  /* _______________________________________________________________________
  
     Cancelled
     _______________________________________________________________________ */

protected:
  static
  MIKTEXAPPCEEAPI(bool)
  Cancelled ();
  
  /* _______________________________________________________________________
  
     CheckCancel
     _______________________________________________________________________ */

public:
  static
  MIKTEXAPPCEEAPI(void)
  CheckCancel ();
  
  /* _______________________________________________________________________
     
     Variables
     _______________________________________________________________________ */

private:
  bool beQuiet;

private:
  MiKTeX::Core::TriState enableInstaller;

private:
  MiKTeX::Packages::PackageManagerPtr pPackageManager;

private:
  std::auto_ptr<MiKTeX::Packages::PackageInstaller> pInstaller;

private:
#if defined(_MSC_VER)
  stdext::hash_set<std::string> ignoredPackages;
#else
  std::set<std::string> ignoredPackages;
#endif

protected:
  MiKTeX::Core::SessionWrapper pSession;

private:
  bool initialized;
};

MIKTEX_APP_END_NAMESPACE;

#undef MIKTEX_APP_BEGIN_NAMESPACE
#undef MIKTEX_APP_END_NAMESPACE

#endif
