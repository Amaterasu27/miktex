/* SiteWizDrive.cpp:

   Copyright (C) 2008 Christian Schenk

   This file is part of the MiKTeX UI Library.

   The MiKTeX UI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX UI Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#include "SiteWizSheet.h"
#include "SiteWizDrive.h"

#include <miktex/UI/Qt/ErrorDialog>

/* _________________________________________________________________________

   SiteWizDrive::SiteWizDrive
   _________________________________________________________________________ */

SiteWizDrive::SiteWizDrive (/*[in]*/ PackageManager *	pManager)
  : QWizardPage (0),
    pManager (pManager),
    firstVisit (true)
{
  setupUi (this);
}

/* _________________________________________________________________________

   SiteWizDrive::initializePage
   _________________________________________________________________________ */

void
SiteWizDrive::initializePage ()
{
  try
    {
      if (! firstVisit)
	{
	  return;
	}
      firstVisit = false;
#if defined(MIKTEX_WINDOWS)
      FindMiKTeXCDs (locations);
#endif
      cbDrives->clear ();
      for (vector<Location>::const_iterator it = locations.begin();
	   it != locations.end();
	   ++ it)
	{
	  cbDrives->addItem (QString::fromLocal8Bit(it->description.c_str()));
	}
      if (locations.empty())
	{
	  cbDrives->addItem (T_("No MiKTeX CD/DVD found"));
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   SiteWizDrive::isComplete
   _________________________________________________________________________ */

bool
SiteWizDrive::isComplete ()
  const
{
  return (! locations.empty());
}

/* _________________________________________________________________________

   SiteWizDrive::validatePage
   _________________________________________________________________________ */

bool
SiteWizDrive::validatePage ()
{
  try
    {
      if (! QWizardPage::validatePage())
	{
	  return (false);
	}
      pManager->SetDefaultPackageRepository
	(RepositoryType::MiKTeXDirect,
	 locations[cbDrives->currentIndex()].directory);
      return (true);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (false);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (false);
    }
}

/* _________________________________________________________________________

   SiteWizDrive::FindMiKTeXCDs
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
void
SiteWizDrive::FindMiKTeXCDs
(/*[in,out]*/ vector<SiteWizDrive::Location> & locations)
{
  AutoErrorMode autoMode (SetErrorMode(SEM_FAILCRITICALERRORS));

  DWORD logicalDrives = GetLogicalDrives();
  for (int drv = 0;
       logicalDrives != 0;
       logicalDrives >>= 1, ++ drv)
    {
      if ((logicalDrives & 1) == 0)
	{
	  continue;
	}

      char szDrive[4];
      szDrive[0] = char('A') + static_cast<char>(drv);
      szDrive[1] = ':';
      szDrive[2] = '\\';
      szDrive[3] = 0;

      DWORD fileSystemFlags;
      DWORD maximumComponentLength;
      char fileSystemName[BufferSizes::MaxPath];
      char volumeName[BufferSizes::MaxPath];

      if (! GetVolumeInformation(szDrive,
				 volumeName,
				 BufferSizes::MaxPath,
				 0,
				 &maximumComponentLength, 
				 &fileSystemFlags,
				 fileSystemName,
				 BufferSizes::MaxPath))
	{
	  continue;
	}

      if (! Utils::IsMiKTeXDirectRoot(szDrive))
	{
	  continue;
	}

      Location location;
      location.directory = szDrive;

      szDrive[2] = 0;
      location.description = szDrive;
      location.description += " (";
      location.description += volumeName;
      location.description += ')';

      locations.push_back (location);
    }
}
#endif
