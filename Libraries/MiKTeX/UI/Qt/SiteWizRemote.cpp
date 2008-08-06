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

#include "RepositoryTableModel.h"

#include "SiteWizSheet.h"
#include "SiteWizRemote.h"

#include <miktex/UI/Qt/ErrorDialog>

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
    }
  catch (const MiKTeXException & e)
    {
      threadMiKTeXException = e;
      error = true;
    }
  catch (const exception &)
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
    firstVisit (true),
    pProxyModel (0)
{
  setupUi (this);
  connect (treeView,
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
  return (treeView->selectionModel() != 0
    && treeView->selectionModel()->selectedRows().count() == 1);
}

/* _________________________________________________________________________

   SiteWizRemote::validatePage
   _________________________________________________________________________ */

bool
SiteWizRemote::validatePage ()
{
  try
    {
      QModelIndexList selectedRows =
	treeView->selectionModel()->selectedRows();
      if (! QWizardPage::validatePage() || selectedRows.count() != 1)
	{
	  return (false);
	}
      int idx = pProxyModel->mapToSource(selectedRows.first()).row();
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

   SiteWizRemote::FillList
   _________________________________________________________________________ */

void
SiteWizRemote::FillList ()
{
  try
    {
      if (pDownloadThread != 0
	  && pDownloadThread->isFinished()
	  && pDownloadThread->error)
	{
	  throw pDownloadThread->threadMiKTeXException;
	}

      RepositoryTableModel * pModel = new RepositoryTableModel(this);
      pModel->SetRepositories (repositories);

      pProxyModel = new QSortFilterProxyModel(this);
      pProxyModel->setSourceModel (pModel);

      treeView->setModel (pProxyModel);
      treeView->sortByColumn (0, Qt::AscendingOrder);

      treeView->header()->resizeSections(QHeaderView::ResizeToContents);

      string selected;

      if (! pManager->TryGetRemotePackageRepository(selected))
	{
#if 0
	  selected = pManager->PickRepositoryUrl();
#endif
	}

      QModelIndex selectedItem;
      
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
	  if (it->url == selected)
	    {
	      selectedItem = pProxyModel->mapFromSource(pModel->index(row, 0));
	      treeView->selectionModel()->select (selectedItem,
		QItemSelectionModel::Select
		| QItemSelectionModel::Rows);
	      emit completeChanged();
	    }
	}

      if (selectedItem.isValid())
	{
	  treeView->scrollTo (selectedItem);
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
