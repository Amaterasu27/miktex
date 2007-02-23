/* app.h:							-*- C++ -*-

   Copyright (C) 2005-2007 Christian Schenk
 
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

#include <miktex/core.h>
#include <miktex/mpm.h>

#if defined(_MSC_VER)
#  include <hash_set>
#else
#  include <set>
#endif

#if ! defined(BDF6E2537F116547846406B5B2B65949)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define MIKTEXAPPEXPORT __declspec(dllimport)
#  else
#    define MIKTEXAPPEXPORT
#  endif
#endif

#if defined(_MSC_VER)
#  define MIKTEXAPPCALL __stdcall
#else
#  define MIKTEXAPPCALL
#endif

#define MIKTEXAPPAPI(type) MIKTEXAPPEXPORT type MIKTEXAPPCALL
#define MIKTEXAPPDATA(type) MIKTEXAPPEXPORT type

#define MIKTEXAPP_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace App {

#define MIKTEXAPP_END_NAMESPACE			\
    }						\
  }

MIKTEXAPP_BEGIN_NAMESPACE;

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
  MIKTEXAPPCALL
  Application ();

  /* _______________________________________________________________________
     
     ~Application
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPEXPORT
  MIKTEXAPPCALL
  ~Application ();

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPAPI(void)
  Init (/*[in]*/ const MiKTeX::Core::Session::InitInfo & initInfo);

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPAPI(void)
  Init (/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName);

  /* _______________________________________________________________________
     
     Finalize
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPAPI(void)
  Finalize ();

  /* _______________________________________________________________________
     
     InstallPackage
     _______________________________________________________________________ */

public:
  
  virtual
  MIKTEXAPPEXPORT
  bool
  MIKTEXCALL
  InstallPackage (/*[in]*/ const MIKTEXCHAR *	lpszPackageName,
		  /*[in]*/ const MIKTEXCHAR *	lpszTrigger);

  /* _______________________________________________________________________
     
     ReportLine
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPEXPORT
  void
  MPMCALL
  ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine);

  /* _______________________________________________________________________
     
     OnRetryableError
     _______________________________________________________________________ */

public:
  virtual
  MIKTEXAPPEXPORT
  bool
  MPMCALL
  OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage);

  /* _______________________________________________________________________
     
     OnProgress
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXAPPEXPORT
  bool
  MPMCALL
  OnProgress (/*[in]*/ MiKTeX::Packages::Notification	nf);

  /* _______________________________________________________________________
     
     EnableInstaller
     _______________________________________________________________________ */
  
public:

  MIKTEXAPPAPI(void)
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
  MIKTEXAPPEXPORT
  bool
  MIKTEXCALL
  Cancelled ();
  
  /* _______________________________________________________________________
  
     CheckCancel
     _______________________________________________________________________ */

public:
  static
  MIKTEXAPPEXPORT
  void
  MIKTEXCALL
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
  stdext::hash_set<MiKTeX::Core::tstring> ignoredPackages;
#else
  std::set<MiKTeX::Core::tstring> ignoredPackages;
#endif

protected:
  MiKTeX::Core::SessionWrapper pSession;

private:
  bool initialized;
};

MIKTEXAPP_END_NAMESPACE;

#undef MIKTEXAPP_BEGIN_NAMESPACE
#undef MIKTEXAPP_END_NAMESPACE

#endif // miktexapp.h
