/* PropSheet.h:							-*- C++ -*-

   Copyright (C) 2000-2006 Christian Schenk

   This file is part of MiKTeX Options.

   MiKTeX Options is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX Options is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Options; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#include "PropPageFormats.h"
#include "PropPageGeneral.h"
#include "PropPageLanguages.h"
#include "PropPagePackages.h"
#include "PropPageRoots.h"

class PropSheet
  : public CPropertySheet,
    public IRunProcessCallback
{
private:
  DECLARE_DYNAMIC(PropSheet);

public:
  PropSheet (/*[in]*/ PackageManager * pManager = 0);

public:
  virtual
  ~PropSheet ();
  
public:
  bool
  RunIniTeXMF (/*[in]*/ const MIKTEXCHAR *		lpszTitle,
	       /*[in]*/ const CommandLineBuilder &	cmdLine,
	       /*[in]*/ ProgressDialog *		pProgressDialog);

public:
  void
  BuildFormats ();

public:
  void
  ScheduleBuildFormats ()
  {
    mustBuildFormats = true;
  }

public:
  bool
  MustBuildFormats ()
    const
  {
    return (mustBuildFormats);
  }

protected:
  DECLARE_MESSAGE_MAP();

private:
  PropPageGeneral generalPage;

private:
  PropPageTeXMFRoots texmfRootsPage;

private:
  PropPageFormats formatsPage;

private:
  PropPageLanguages languagesPage;

private:
  PropPagePackages packagesPage;
  
private:
  virtual
  BOOL
  OnInitDialog ();

private:
  virtual
  bool
  MIKTEXCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n);

private:
  tstring processOutput;

private:
  ProgressDialog * pProgressDialog;

private:
  bool mustBuildFormats;

private:
  PackageManagerPtr pManager;
};
