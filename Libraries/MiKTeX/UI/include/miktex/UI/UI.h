/* miktex/UI/UI.h:						-*- C++ -*-

   Copyright (C) 2000-2009 Christian Schenk

   This file is part of MiKTeX UI Library.

   MiKTeX UI Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX UI Library; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(DD977A74EEC1466EAF194BE92B20C9C4)
#define DD977A74EEC1466EAF194BE92B20C9C4

#if defined(MIKTEX_ATLMFC)
#  include <miktex/UI/MFC/Prototypes>
#endif

#if defined(MIKTEX_QT)
#  include <miktex/UI/Qt/Prototypes>
#endif

#include <miktex/Core/Registry>

namespace MiKTeX
{
  namespace Packages
  {
    class PackageManager;
  }
};

#define MIKUI_BEGIN_NAMESPACE			\
  namespace MiKTeX {				\
    namespace UI {				

#define MIKUI_END_NAMESPACE			\
      }						\
    }

MIKUI_BEGIN_NAMESPACE;

class FrameworkEnum
{
public:
  enum EnumType {
    None,
#if defined(MIKTEX_ATLMFC)
    MFC,
#endif
#if defined(MIKTEX_QT)
    Qt,
#endif
  };
};

typedef MiKTeX::Core::EnumWrapper<FrameworkEnum> Framework;

inline
int
defaultFramework ()
{
  static int cached = -1;
  if (cached < 0)
    {
      int defaultGUIFramework = Framework::None;
#if defined(MIKTEX_QT)
      defaultGUIFramework = Framework::Qt;
#elif defined(MIKTEX_ATLMFC)
      defaultGUIFramework = Framework::MFC;
#endif
      cached =
	MiKTeX::Core::SessionWrapper(true)
	->GetConfigValue(0,
			 MIKTEX_REGVAL_GUI_FRAMEWORK,
			 defaultGUIFramework);
    }
  return (cached);
}

const unsigned int YES = 1;
const unsigned int NO = 2;
const unsigned int DONTASKAGAIN = 4;
const unsigned int ADMIN = 8;

inline
void
InitializeFramework ()
{
#if defined(MIKTEX_QT)
  if (defaultFramework() == Framework::Qt)
    {
      Qt::InitializeFramework ();
    }
#endif
#if defined(MIKTEX_ATLMFC)
  if (defaultFramework() == Framework::MFC)
    {
      MFC::InitializeFramework ();
    }
#endif
}

inline
void
FinalizeFramework ()
{
#if defined(MIKTEX_QT)
  if (defaultFramework() == Framework::Qt)
    {
      Qt::FinalizeFramework ();
    }
#endif
#if defined(MIKTEX_ATLMFC)
  if (defaultFramework() == Framework::MFC)
    {
      //
    }
#endif
}

inline
unsigned int
InstallPackageMessageBox
(/*[in]*/ MiKTeX::Packages::PackageManager *	pManager,
 /*[in]*/ const char *				lpszPackageName,
 /*[in]*/ const char *				lpszTrigger)
{
  unsigned ret = NO | DONTASKAGAIN;
#if defined(MIKTEX_QT)
  if (defaultFramework() == Framework::Qt)
    {
      ret =
	Qt::InstallPackageMessageBox(pManager,
				     lpszPackageName,
				     lpszTrigger);
}
#endif
#if defined(MIKTEX_ATLMFC)
  if (defaultFramework() == Framework::MFC)
    {
      ret =
	MFC::InstallPackageMessageBox(pManager,
				      lpszPackageName,
				      lpszTrigger);
    }
#endif
  return (ret);
}

inline
bool
ProxyAuthenticationDialog ()
{
  bool ret = false;
#if defined(MIKTEX_QT)
  if (defaultFramework() == Framework::Qt)
    {
      ret = Qt::ProxyAuthenticationDialog();
    }
#endif
#if defined(MIKTEX_ATLMFC)
  if (defaultFramework() == Framework::MFC)
    {
      ret = MFC::ProxyAuthenticationDialog();
    }
#endif
  return (ret);
}

MIKUI_END_NAMESPACE;

#endif
