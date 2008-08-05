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

   CountryComparer
   _________________________________________________________________________ */

class CountryComparer
{
public:
  inline
  bool
  operator() (/*[in]*/ const RepositoryInfo &	lhs,
	      /*[in]*/ const RepositoryInfo &	rhs)
  {
    return (StringCompare(lhs.country.c_str(),
			  rhs.country.c_str(),
			  true)
	    < 0);
  }
};

/* _________________________________________________________________________

   SiteWizRemote::DownloadThread::run
   _________________________________________________________________________ */

void
SiteWizRemote::DownloadThread::run ()
{
  SiteWizRemote * This = reinterpret_cast<SiteWizRemote *>(parent());
  try
    {
      This->pManager->DownloadRepositoryList ();
      This->repositories = This->pManager->GetRepositories();
      sort (This->repositories.begin(),
	    This->repositories.end(),
	    CountryComparer());
    }
  catch (const MiKTeXException & e)
    {
      threadMiKTeXException = e;
      error = true;
    }
  catch (const exception & e)
    {
    }
}

/* _________________________________________________________________________

   SiteWizRemote::SiteWizRemote
   _________________________________________________________________________ */

SiteWizRemote::SiteWizRemote (/*[in]*/ PackageManager *	pManager)
  : QWizardPage (0),
    pDownloadThread (0),
    pManager (pManager),
    firstVisit (true)
{
  setupUi (this);
  setTitle (T_("Remote Package Repository"));
  setSubTitle (T_("\
Packages will be installed from the Internet. Choose \
a remote package repository."));
  connect (tableRepositories,
    SIGNAL(clicked(const QModelIndex &)),
    this,
    SIGNAL(completeChanged()));
}

/* _________________________________________________________________________

   SiteWizRemote::initializePage
   _________________________________________________________________________ */

void
SiteWizRemote::initializePage ()
{
  if (firstVisit)
    {
      firstVisit = false;
      tableRepositories->horizontalHeader()->setVisible (false);
      tableRepositories->horizontalHeader()->setStretchLastSection (true);
      tableRepositories->verticalHeader()->setVisible (false);
      tableRepositories->setRowCount (1);
      tableRepositories->setColumnCount (1);
      QTableWidgetItem * pItem = new QTableWidgetItem(T_("Connecting..."));
      pItem->setFlags (Qt::ItemIsEnabled);
      tableRepositories->setItem (0, 0, pItem);
      pDownloadThread = new DownloadThread(this);
      connect (pDownloadThread, SIGNAL(finished()),
	       this, SLOT(FillList()));
      pDownloadThread->start ();
    }
}

/* _________________________________________________________________________

   SiteWizRemote::isComplete
   _________________________________________________________________________ */

bool
SiteWizRemote::isComplete ()
  const
{
  return (! tableRepositories->selectedItems().isEmpty());
}

/* _________________________________________________________________________

   SiteWizRemote::validatePage
   _________________________________________________________________________ */

bool
SiteWizRemote::validatePage ()
{
  try
    {
      QList<QTableWidgetItem*> selectedItems =
	tableRepositories->selectedItems();
      if (! QWizardPage::validatePage() || selectedItems.isEmpty())
	{
	  return (false);
	}
      int idx = selectedItems.first()->row();
      pManager->SetDefaultPackageRepository (RepositoryType::Remote,
					     repositories[idx].url);
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

   SiteWizRemote::SetItemText
   _________________________________________________________________________ */

void
SiteWizRemote::SetItemText (/*[in]*/ int		row,
			    /*[in]*/ int		column,
			    /*[in]*/ const QString &	text)
{
  QTableWidgetItem * pItem = new QTableWidgetItem(text);
  pItem->setFlags (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
  tableRepositories->setItem (row, column, pItem);
}

/* _________________________________________________________________________

   SiteWizRemote::FillList
   _________________________________________________________________________ */

void
SiteWizRemote::FillList ()
{
  try
    {
      tableRepositories->clearContents ();

      if (pDownloadThread != 0
	  && pDownloadThread->isFinished()
	  && pDownloadThread->error)
	{
	  throw pDownloadThread->threadMiKTeXException;
	}

      tableRepositories->setRowCount (repositories.size());

      tableRepositories->horizontalHeader()->setVisible (true);

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

      string selected;

      if (! pManager->TryGetRemotePackageRepository(selected))
	{
	  selected = "";
	}

      QTableWidgetItem * pSelectedItem = 0;
      
      int row = 0;
      for (vector<RepositoryInfo>::const_iterator it = repositories.begin();
	   it != repositories.end();
	   ++ it, ++ row)
	{
#if MIKTEX_RELEASE_STATE < 4
	  if (it->packageLevel == PackageLevel::None)
	    {
	      continue;
	    }
#endif

	  QUrl url (QString::fromLocal8Bit(it->url.c_str()));

	  SetItemText (row, 0, QString::fromLocal8Bit(it->country.c_str()));
	  SetItemText (row, 1, url.scheme().toUpper());
	  SetItemText (row, 2, url.host());
      	  SetItemText (row,
		       3,
		       QDateTime::fromTime_t
		       (it->timeDate).toString("yyyy-MM-dd"));
	  SetItemText (row,
		       4,
		       QString::fromLocal8Bit(it->description.c_str()));
	  if (it->url == selected)
	    {
	      tableRepositories->selectRow (row);
	      pSelectedItem = tableRepositories->item(row, 0);
	      emit completeChanged();
	    }
	}

      tableRepositories->horizontalHeader()
	->resizeSections(QHeaderView::ResizeToContents);
      tableRepositories->horizontalHeader()->setStretchLastSection (true);

      tableRepositories->verticalHeader()
	->resizeSections(QHeaderView::ResizeToContents);

      if (pSelectedItem != 0)
	{
	  tableRepositories->scrollToItem (pSelectedItem);
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
