/* RepositoryTableModel.cpp:

   Copyright (C) 2008 Christian Schenk

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

#include <QDateTime>
#include <QUrl>

#include "RepositoryTableModel.h"

using namespace MiKTeX::Packages;
using namespace std;

/* _________________________________________________________________________

   RepositoryTableModel::RepositoryTableModel
   _________________________________________________________________________ */

RepositoryTableModel::RepositoryTableModel (/*[in]*/ QObject * pParent)
  : QAbstractTableModel (pParent)
{
}

/* _________________________________________________________________________

   RepositoryTableModel::rowCount
   _________________________________________________________________________ */

int
RepositoryTableModel::rowCount (/*[in]*/ const QModelIndex & parent)
  const
{
  return (parent.isValid() ? 0 : repositories.size());
}

/* _________________________________________________________________________

   RepositoryTableModel::columnCount
   _________________________________________________________________________ */

int
RepositoryTableModel::columnCount (/*[in]*/ const QModelIndex & parent)
  const
{
  return (parent.isValid() ? 0 : 5);
}

/* _________________________________________________________________________

   RepositoryTableModel::data
   _________________________________________________________________________ */

QVariant
RepositoryTableModel::data (/*[in]*/ const QModelIndex &	index,
			    /*[in]*/ int			role)
  const
{
  if (! (index.isValid()
	 && index.row() >= 0
	 && index.row() < repositories.size()))
    {
      return (QVariant());
    }
    
  if (role == Qt::DisplayRole)
    {
      const RepositoryInfo & repositoryInfo = repositories[index.row()];
      switch (index.column())
	{
	case 0:
	  return (QString::fromLocal8Bit(repositoryInfo.country.c_str()));
	case 1:
	  {
	    QUrl url (QString::fromLocal8Bit(repositoryInfo.url.c_str()));
	    return (url.scheme().toUpper());
	  }
	case 2:
	  {
	    QUrl url (QString::fromLocal8Bit(repositoryInfo.url.c_str()));
	    return (url.host());
	  }
	case 3:
	  return (QDateTime::fromTime_t(repositoryInfo.timeDate).date());
	case 4:
	  return (QString::fromLocal8Bit(repositoryInfo.description.c_str()));
	}
    }

  return (QVariant());
}

/* _________________________________________________________________________

   RepositoryTableModel::headerData
   _________________________________________________________________________ */

QVariant
RepositoryTableModel::headerData (/*[in]*/ int		section,
			       /*[in]*/ Qt::Orientation	orientation,
			       /*[in]*/ int		role)
  const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
      switch (section)
	{
	case 0:
	  return (tr("Country"));
	case 1:
	  return (tr("Protocol"));
	case 2:
	  return (tr("Host"));
	case 3:
	  return (tr("Version"));
	case 4:
	  return (tr("Description"));
	}
    }
  return (QAbstractTableModel::headerData(section, orientation, role));
}
