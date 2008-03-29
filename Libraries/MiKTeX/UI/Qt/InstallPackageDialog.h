/* InstallPackageDialog.h:					-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(FA975C232D1F491C84BA4294548DFE25)
#define FA975C232D1F491C84BA4294548DFE25

#include <miktex/PackageManager/PackageManager>

#include "ui_InstallPackageDialog.h"

class InstallPackageDialog
  : public QDialog,
    private Ui::InstallPackageDialog
{
private:
  Q_OBJECT;

public:
  InstallPackageDialog
  (/*[in]*/ QWidget *				pParent,
   /*[in]*/ MiKTeX::Packages::PackageManager *	pManager,
   /*[in]*/ const char *			lpszPackageName,
   /*[in]*/ const char *			lpszTrigger);

public:
  void
  SetAlwaysAsk (/*[in]*/ bool alwaysAsk)
  {
    chkAlwaysAsk->setCheckState (alwaysAsk ? Qt::Checked : Qt::Unchecked);
  }

public:
  bool
  GetAlwaysAsk ()
    const
  {
    Qt::CheckState checkState = chkAlwaysAsk->checkState();
    return (checkState == Qt::Checked ? true : false);
  }

public:
  MiKTeX::Packages::PackageManagerPtr pManager;

private slots:
  void on_btnChange_clicked();
};

#endif
