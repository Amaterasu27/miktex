/* PackageTableModel.cpp:

   Copyright (C) 2008-2015 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include <memory>

#include <QDateTime>

#include "PackageTableModel.h"

using namespace MiKTeX::Packages;
using namespace std;

/* _________________________________________________________________________

   PackageTableModel::PackageTableModel
   _________________________________________________________________________ */

PackageTableModel::PackageTableModel
(/*[in]*/ MiKTeX::Packages::PackageManager * pManager,
 /*[in]*/ QObject * pParent)
  : QAbstractTableModel (pParent),
    pManager (pManager)
{
  Reload ();
}

/* _________________________________________________________________________

   PackageTableModel::rowCount
   _________________________________________________________________________ */

int
PackageTableModel::rowCount (/*[in]*/ const QModelIndex & parent)
  const
{
  return (parent.isValid() ? 0 : packages.size());
}

/* _________________________________________________________________________

   PackageTableModel::columnCount
   _________________________________________________________________________ */

int
PackageTableModel::columnCount (/*[in]*/ const QModelIndex & parent)
  const
{
  return (parent.isValid() ? 0 : 6);
}

/* _________________________________________________________________________

   PackageTableModel::data
   _________________________________________________________________________ */

QVariant
PackageTableModel::data (/*[in]*/ const QModelIndex &	index,
			 /*[in]*/ int			role)
  const
{
  if (! (index.isValid()
	 && index.row() >= 0
	 && index.row() < packages.size()))
    {
      return (QVariant());
    }
    
  if (role == Qt::DisplayRole)
    {
      PackageInfo packageInfo;
      if (TryGetPackageInfo(index, packageInfo))
	{
	  switch (index.column())
	    {
	    case 0:
	      return (packageInfo.deploymentName.c_str());
	    case 1:
	      return (pManager->GetContainerPath(packageInfo.deploymentName,
						 true)
		      .c_str());
	    case 2:
	      return (packageInfo.GetSize());
	    case 3:
	      return (QDateTime::fromTime_t(packageInfo.timePackaged).date());
	    case 4:
	      if (packageInfo.timeInstalled == static_cast<time_t>(0))
	      {
		return ("");
	      }
	      else
	      {
		return (QDateTime::fromTime_t(packageInfo.timeInstalled).date());
	      }
	    case 5:
	      return (packageInfo.title.c_str());
	    }
	}
    }

  return (QVariant());
}

/* _________________________________________________________________________

   PackageTableModel::headerData
   _________________________________________________________________________ */

QVariant
PackageTableModel::headerData (/*[in]*/ int		section,
			       /*[in]*/ Qt::Orientation	orientation,
			       /*[in]*/ int		role)
  const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
      switch (section)
	{
	case 0:
	  return (tr("Name"));
	case 1:
	  return (tr("Category"));
	case 2:
	  return (tr("Size"));
	case 3:
	  return (tr("Packaged on"));
	case 4:
	  return (tr("Installed on"));
	case 5:
	  return (tr("Title"));
	}
    }
  return (QAbstractTableModel::headerData(section, orientation, role));
}

/* _________________________________________________________________________

   PackageTableModel::Reload
   _________________________________________________________________________ */

void
PackageTableModel::Reload ()
{
  beginResetModel();
  packages.clear ();
  pManager->UnloadDatabase ();
  auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
  PackageInfo packageInfo;
  int row = 0;
  while (pIter->GetNext(packageInfo))
    {
      if (! packageInfo.IsPureContainer())
	{
	  packages[row] = packageInfo;
	  ++ row;
	}
    }
  pIter->Dispose ();
  endResetModel();
}
