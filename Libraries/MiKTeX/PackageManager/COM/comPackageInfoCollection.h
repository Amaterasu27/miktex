/* comPackageInfoCollection.h:					-*- C++ -*-

   Copyright (C) 2001-2008 Christian Schenk

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

#pragma once

#include "Resource.h"

inline
void
CopyPackageInfo (/*[out]*/ MiKTeXPackageManagerLib::PackageInfo &	left,
		 /*[in]*/ const MiKTeX::Packages::PackageInfo &		right)
{
  left.deploymentName = _bstr_t(right.deploymentName.c_str()).Detach();
  left.displayName = _bstr_t(right.displayName.c_str()).Detach();
  left.title = _bstr_t(right.title.c_str()).Detach();
  left.version = _bstr_t(right.version.c_str()).Detach();
  left.description = _bstr_t(right.description.c_str()).Detach();
  left.creator = _bstr_t(right.creator.c_str()).Detach();
  left.sizeRunFiles = right.sizeRunFiles;
  left.sizeDocFiles = right.sizeDocFiles;
  left.sizeSourceFiles = right.sizeSourceFiles;
  left.numRunFiles = right.runFiles.size();
  left.numDocFiles = right.docFiles.size();
  left.numSourceFiles = right.sourceFiles.size();
  if (right.timePackaged == static_cast<time_t>(-1)
      || right.timePackaged == static_cast<time_t>(0))
    {
      left.timePackaged = COleDateTime();
    }
  else
    {
      left.timePackaged = COleDateTime(right.timePackaged);
    }
  if (right.timeInstalled == static_cast<time_t>(-1)
      || right.timeInstalled == static_cast<time_t>(0))
    {
      left.timeInstalled = COleDateTime();
      left.isInstalled = VARIANT_FALSE;
    }
  else
    {
      left.timeInstalled = COleDateTime(right.timeInstalled);
      left.isInstalled = VARIANT_TRUE;
    }
  left.archiveFileSize = right.archiveFileSize;
  memcpy (&left.digest, &right.digest, sizeof(left.digest));
}

template
<class DestinationType, class SourceType = DestinationType>
class InitCopyDestroy
{
public:
  static
  void
  init (DestinationType * ptr)
  {
    _Copy<DestinationType>::init (ptr);
  }
public:
  static
  HRESULT copy (DestinationType *	pDest,
		const SourceType *	pSrc)
  {
    return (_Copy<DestinationType>::copy(pDest, pSrc));
  }
public:
  static
  void
  destroy (DestinationType * ptr)
  {
    _Copy<DestinationType>::destroy (ptr);
  }
};

template<>
class InitCopyDestroy<MiKTeXPackageManagerLib::PackageInfo,
		      MiKTeX::Packages::PackageInfo>
{
public:
  static
  void
  init (MiKTeXPackageManagerLib::PackageInfo * pPackageInfo)
  {
    pPackageInfo->deploymentName = 0;
    pPackageInfo->displayName = 0;
    pPackageInfo->title = 0;
    pPackageInfo->version = 0;
    pPackageInfo->description = 0;
    pPackageInfo->creator = 0;
  }
public:
  static
  HRESULT copy (MiKTeXPackageManagerLib::PackageInfo *	pDest,
		const MiKTeX::Packages::PackageInfo *	pSrc)
  {
    CopyPackageInfo (*pDest, *pSrc);
    return (S_OK);
  }
public:
  static
  void
  destroy (MiKTeXPackageManagerLib::PackageInfo * pPackageInfo)
  {
    SysFreeString (pPackageInfo->deploymentName);
    SysFreeString (pPackageInfo->displayName);
    SysFreeString (pPackageInfo->title);
    SysFreeString (pPackageInfo->version);
    SysFreeString (pPackageInfo->description);
    SysFreeString (pPackageInfo->creator);
  }
};

template<>
class InitCopyDestroy<VARIANT, MiKTeX::Packages::PackageInfo>
{
public:
  static
  void
  init (VARIANT * pVariant)
  {
    VariantInit (pVariant);
  }
public:
  static
  HRESULT copy (VARIANT *				pDest,
		const MiKTeX::Packages::PackageInfo *	pSrc)
  {
    try
      {
	MiKTeXPackageManagerLib::PackageInfo packageInfo;
	CopyPackageInfo (packageInfo, *pSrc);
	IRecordInfoPtr pRecordInfo;
	HRESULT hr =
	  GetRecordInfoFromGuids
	  (__uuidof(MiKTeXPackageManagerLib
		    ::MAKE_CURVER_ID(__MiKTeXPackageManager)),
	   1,
	   0,
	   0,
	   __uuidof(MiKTeXPackageManagerLib::PackageInfo),
	   &pRecordInfo);
	if (FAILED(hr))
	  {
	    UNEXPECTED_CONDITION ("InitCopyDestroy::copy");
	  }
	VARIANT variant;
	variant.vt = VT_RECORD;
	variant.pvRecord = &packageInfo;
	variant.pRecInfo = pRecordInfo;
	VariantCopy (pDest, &variant);
	ULONG n = pRecordInfo->Release();
	MIKTEX_ASSERT (n >= 1);
	return (S_OK);
      }
    catch (const std::exception &)
      {
	return (E_FAIL);
      }
  }
public:
  static
  void
  destroy (VARIANT * pVariant)
  {
    try
      {
	VariantClear (pVariant);
      }
    catch (const std::exception &)
      {
      }
  }
};

typedef CComEnumOnSTL<IEnumVARIANT,
		      &__uuidof(IEnumVARIANT),
		      VARIANT,
		      InitCopyDestroy<VARIANT, MiKTeX::Packages::PackageInfo>,
		      std::vector<MiKTeX::Packages::PackageInfo> >
MyEnumerator;
		      

typedef ICollectionOnSTLImpl<MiKTeXPackageManagerLib::IPackageInfoCollection,
			     std::vector<MiKTeX::Packages::PackageInfo>,
			     VARIANT,
			     InitCopyDestroy<VARIANT,
					     MiKTeX::Packages::PackageInfo>,
			     MyEnumerator>
MyCollection;
			     

class ATL_NO_VTABLE comPackageInfoCollection
  : public CComObjectRootEx<CComMultiThreadModel>,
    public ISupportErrorInfo,
    public IDispatchImpl<MyCollection,
			 &__uuidof(MiKTeXPackageManagerLib::IPackageInfoCollection),
			 &__uuidof(MiKTeXPackageManagerLib
				   ::MAKE_CURVER_ID(__MiKTeXPackageManager)),
			 /*wMajor =*/ 1,
			 /*wMinor =*/ 0>
{
public:
  comPackageInfoCollection ();

public:
  virtual
  ~comPackageInfoCollection ();

public:
  BEGIN_COM_MAP(comPackageInfoCollection)
    COM_INTERFACE_ENTRY(MiKTeXPackageManagerLib::IPackageInfoCollection)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP();

public:
  STDMETHOD(InterfaceSupportsErrorInfo) (/*[in]*/ REFIID riid);
  
public:
  DECLARE_PROTECT_FINAL_CONSTRUCT();
  
public:
  HRESULT
  FinalConstruct ()
  {
    return (S_OK);
  }
  
public:
  void
  FinalRelease ();

public:
  void
  Initialize ();

private:
  MiKTeX::Packages::PackageManagerPtr pManager;
};





class ATL_NO_VTABLE comEnumPackageInfo
  : public CComObjectRootEx<CComMultiThreadModel>,
    public MiKTeXPackageManagerLib::IEnumPackageInfo,
    public ISupportErrorInfo
{
public:
  comEnumPackageInfo ();

public:
  virtual
  ~comEnumPackageInfo ();

public:
  BEGIN_COM_MAP(comEnumPackageInfo)
    COM_INTERFACE_ENTRY(MiKTeXPackageManagerLib::IEnumPackageInfo)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP();

public:
  STDMETHOD(InterfaceSupportsErrorInfo) (/*[in]*/ REFIID riid);
  
public:
  DECLARE_PROTECT_FINAL_CONSTRUCT();
  
public:
  HRESULT
  FinalConstruct ()
  {
    return (S_OK);
  }
  
public:
  void
  FinalRelease ();

public:
  void
  Initialize ();

public:
  STDMETHOD(Next)
  (/*[in]*/ ULONG					count, 
   /*[out]*/ MiKTeXPackageManagerLib::PackageInfo *	pPackageInfo,
   /*[out]*/ ULONG *					pFetched);

public:
  STDMETHOD(Skip) (/*[in] */ ULONG count);

public:
  STDMETHOD(Reset) ();

public:
  STDMETHOD(Clone)
  (/*[out]*/ MiKTeXPackageManagerLib::IEnumPackageInfo ** pOther);

private:
  MiKTeX::Packages::PackageManagerPtr pManager;

private:
  std::auto_ptr<MiKTeX::Packages::PackageIterator> pIter;
};





class ATL_NO_VTABLE comPackageIterator
  : public CComObjectRootEx<CComMultiThreadModel>,
    public MiKTeXPackageManagerLib::IPackageIterator,
    public ISupportErrorInfo
{
public:
  comPackageIterator ();

public:
  virtual
  ~comPackageIterator ();

public:
  BEGIN_COM_MAP(comPackageIterator)
    COM_INTERFACE_ENTRY(MiKTeXPackageManagerLib::IPackageIterator)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP();

public:
  STDMETHOD(InterfaceSupportsErrorInfo) (/*[in]*/ REFIID riid);
  
public:
  DECLARE_PROTECT_FINAL_CONSTRUCT();
  
public:
  HRESULT
  FinalConstruct ()
  {
    return (S_OK);
  }
  
public:
  void
  FinalRelease ();

public:
  void
  Initialize ();

public:
  STDMETHOD(GetNextPackageInfo)
  (/*[out]*/ MiKTeXPackageManagerLib::PackageInfo *	pPackageInfo,
   /*[out,retval]*/ VARIANT_BOOL *			pDone);
     
private:
  MiKTeX::Packages::PackageManagerPtr pManager;

private:
  std::auto_ptr<MiKTeX::Packages::PackageIterator> pIter;
};

