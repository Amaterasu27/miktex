/* RepositoryTableModel.h:					-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(EED8F292DA8F4313B3BB1E684E900634)
#define EED8F292DA8F4313B3BB1E684E900634

#include <QAbstractTableModel>
#include <miktex/PackageManager/PackageManager>
#include <vector>

class RepositoryTableModel : public QAbstractTableModel
{
private:
  Q_OBJECT;

public:
  RepositoryTableModel (/*[in]*/ QObject * pParent = 0);

public:
  void
  SetRepositories (/*[in]*/ std::vector<MiKTeX::Packages::RepositoryInfo> &
		   repositories)
  {
    this->repositories = repositories;
  }

public:
  virtual
  int
  rowCount (/*[in]*/ const QModelIndex & parent)
    const;

public:
  virtual
  int
  columnCount (/*[in]*/ const QModelIndex & parent)
    const;

public:
  virtual
  QVariant
  data (/*[in]*/ const QModelIndex &	index,
	/*[in]*/ int			role)
    const;

public:
  virtual
  QVariant
  headerData (/*[in]*/ int		section,
	      /*[in]*/ Qt::Orientation	orientation,
	      /*[in]*/ int		role)
    const;

private:
  std::vector<MiKTeX::Packages::RepositoryInfo> repositories;
};

#endif
