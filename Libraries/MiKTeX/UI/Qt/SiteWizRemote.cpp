/* SiteWizRemote.cpp:

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
#include "SiteWizRemote.h"

#include <miktex/UI/Qt/ErrorDialog>

/* _________________________________________________________________________

   SiteWizRemote::SiteWizRemote
   _________________________________________________________________________ */

SiteWizRemote::SiteWizRemote (/*[in]*/ PackageManager *	pManager)
  : QWizardPage (0),
    pManager (pManager)
{
  setupUi (this);
  try
    {
      tableRepositories->setColumnCount (5);
      tableRepositories->setHorizontalHeaderItem
	(0, new QTableWidgetItem(T_("Country")));
      tableRepositories->setHorizontalHeaderItem
	(1, new QTableWidgetItem(T_("Protocol")));
      tableRepositories->setHorizontalHeaderItem
	(2, new QTableWidgetItem(T_("Host")));
      tableRepositories->setHorizontalHeaderItem
	(3, new QTableWidgetItem(T_("Version")));
      tableRepositories->setHorizontalHeaderItem
	(4, new QTableWidgetItem(T_("Description")));
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
    }
}

/* _________________________________________________________________________

   SiteWizRemote::initializePage
   _________________________________________________________________________ */

void
SiteWizRemote::initializePage ()
{
  if (repositories.size() == 0)
    {
      tableRepositories->setRowCount (1);
      QTableWidgetItem * pItem = new QTableWidgetItem(T_("Connecting..."));
      pItem->setFlags (Qt::ItemIsEnabled);
      tableRepositories->setItem (0, 0, pItem);
      pItem = new QTableWidgetItem();
      pItem->setFlags (0);
      tableRepositories->setItem (0, 1, pItem);
      pItem = new QTableWidgetItem();
      pItem->setFlags (0);
      tableRepositories->setItem (0, 2, pItem);
      pItem = new QTableWidgetItem();
      pItem->setFlags (0);
      tableRepositories->setItem (0, 3, pItem);
      pItem = new QTableWidgetItem();
      pItem->setFlags (0);
      tableRepositories->setItem (0, 4, pItem);
    }
}
