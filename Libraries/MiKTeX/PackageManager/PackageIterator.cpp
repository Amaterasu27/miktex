/* PackageIterator.cpp:

   Copyright (C) 2001-2006 Christian Schenk

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
   along with MiKTeX Package Manager; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"

#include "internal.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

/* _________________________________________________________________________

   PackageIteratorImpl::PackageIteratorImpl
   _________________________________________________________________________ */

PackageIteratorImpl::PackageIteratorImpl
(/*[in]*/ PackageManagerImpl * pManager)
  : filter (PackageFilter::None),
    pManager (pManager)
{
  pManager->GetAllPackageDefinitions (snapshot);
  iter = snapshot.begin();
}

/* _________________________________________________________________________

   PackageIteratorImpl::GetNext
   _________________________________________________________________________ */

bool
PackageIteratorImpl::GetNext (/*[out]*/ PackageInfo & packageInfo)
{
  bool found = false;
  for ( ; ! found && iter != snapshot.end(); ++ iter)
    {
      if ((filter & PackageFilter::Top) != 0
	  && iter->requiredBy.size() != 0)
	{
	  continue;
	}
      if ((filter & PackageFilter::RequiredBy) != 0
	  && (find(iter->requiredBy.begin(),
		   iter->requiredBy.end(),
		   requiredBy)
	      == iter->requiredBy.end()))
	{
	  continue;
	}
      if ((filter & PackageFilter::Obsolete) != 0
	  && ! pManager->IsPackageObsolete(iter->deploymentName.c_str()))
	{
	  continue;
	}
      packageInfo = *iter;
      found = true;
    }
  return (found);
}

/* _________________________________________________________________________

   PackageIteratorImpl::~PackageIteratorImpl
   _________________________________________________________________________ */

PackageIteratorImpl::~PackageIteratorImpl ()
{
  try
    {
      Dispose ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   PackageIteratorImpl::Dispose
   _________________________________________________________________________ */

void
MPMCALL
PackageIteratorImpl::Dispose ()
{
}

/* _________________________________________________________________________

   PackageIterator::~PackageIterator
   _________________________________________________________________________ */

MPMCALL
PackageIterator::~PackageIterator ()
{
}
