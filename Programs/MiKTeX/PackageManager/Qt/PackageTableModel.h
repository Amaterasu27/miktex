/* PackageTableModel.h:						-*- C++ -*-

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

#if ! defined(E1A8156B46A44B4F948824CCE86028AF)
#define E1A8156B46A44B4F948824CCE86028AF

#include <miktex/PackageManager/PackageManager>
#include <QAbstractTableModel>

class PackageTableModel : public QAbstractTableModel
{
private:
  Q_OBJECT;

public:
  PackageTableModel (/*[in]*/ MiKTeX::Packages::PackageManager * pManager,
		     /*[in]*/ QObject * pParent = 0);

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

public:
  void
  Reload ();

public:
  bool
  TryGetPackageInfo (/*[in]*/ const QModelIndex &		index,
                     /*[out]*/ MiKTeX::Packages::PackageInfo &	packageInfo)
    const
  {
    std::map<int, MiKTeX::Packages::PackageInfo>::const_iterator it =
      packages.find(index.row());
    if (it == packages.end())
    {
      return (false);
    }
    else
    {
      packageInfo = it->second;
      return (true);
    }
  }

private:
  MiKTeX::Packages::PackageManagerPtr pManager;

private:
  std::map<int, MiKTeX::Packages::PackageInfo> packages;
};

#endif
