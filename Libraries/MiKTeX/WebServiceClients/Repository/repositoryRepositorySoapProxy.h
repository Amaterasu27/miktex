/* repositoryRepositorySoapProxy.h
   Generated by gSOAP 2.7.9d from C:/work3/miktex-2.6/Libraries/MiKTeX/WebServiceClients/Repository/Repository.h
   Copyright(C) 2000-2006, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef repositoryRepositorySoapProxy_H
#define repositoryRepositorySoapProxy_H
#include "repositoryH.h"

class SOAP_CMAC RepositorySoapProxy : public soap
{ public:
		RepositorySoapProxy();
		RepositorySoapProxy(soap_mode iomode);
		RepositorySoapProxy(soap_mode imode, soap_mode omode);
	virtual ~RepositorySoapProxy();
	virtual	void RepositorySoapProxy_init(soap_mode imode, soap_mode omode);
	virtual int TryGetRepositoryInfo(_mtrep__TryGetRepositoryInfo *mtrep__TryGetRepositoryInfo, _mtrep__TryGetRepositoryInfoResponse *mtrep__TryGetRepositoryInfoResponse);
	virtual int PickRepository(_mtrep__PickRepository *mtrep__PickRepository, _mtrep__PickRepositoryResponse *mtrep__PickRepositoryResponse);
	virtual int GetAllRepositories(_mtrep__GetAllRepositories *mtrep__GetAllRepositories, _mtrep__GetAllRepositoriesResponse *mtrep__GetAllRepositoriesResponse);
	virtual int GetRepositories(_mtrep__GetRepositories *mtrep__GetRepositories, _mtrep__GetRepositoriesResponse *mtrep__GetRepositoriesResponse);
	virtual int GetListCreationTime(_mtrep2__GetListCreationTime *mtrep2__GetListCreationTime, _mtrep2__GetListCreationTimeResponse *mtrep2__GetListCreationTimeResponse);
	virtual int GetRepositories2(_mtrep3__GetRepositories2 *mtrep3__GetRepositories2, _mtrep3__GetRepositories2Response *mtrep3__GetRepositories2Response);
	virtual int PickRepository2(_mtrep3__PickRepository2 *mtrep3__PickRepository2, _mtrep3__PickRepository2Response *mtrep3__PickRepository2Response);
	virtual int TryGetRepositoryInfo2(_mtrep3__TryGetRepositoryInfo2 *mtrep3__TryGetRepositoryInfo2, _mtrep3__TryGetRepositoryInfo2Response *mtrep3__TryGetRepositoryInfo2Response);
	virtual int TryGetRepositoryInfo_(_mtrep__TryGetRepositoryInfo *mtrep__TryGetRepositoryInfo, _mtrep__TryGetRepositoryInfoResponse *mtrep__TryGetRepositoryInfoResponse);
	virtual int PickRepository_(_mtrep__PickRepository *mtrep__PickRepository, _mtrep__PickRepositoryResponse *mtrep__PickRepositoryResponse);
	virtual int GetAllRepositories_(_mtrep__GetAllRepositories *mtrep__GetAllRepositories, _mtrep__GetAllRepositoriesResponse *mtrep__GetAllRepositoriesResponse);
	virtual int GetRepositories_(_mtrep__GetRepositories *mtrep__GetRepositories, _mtrep__GetRepositoriesResponse *mtrep__GetRepositoriesResponse);
	virtual int GetListCreationTime_(_mtrep2__GetListCreationTime *mtrep2__GetListCreationTime, _mtrep2__GetListCreationTimeResponse *mtrep2__GetListCreationTimeResponse);
	virtual int GetRepositories2_(_mtrep3__GetRepositories2 *mtrep3__GetRepositories2, _mtrep3__GetRepositories2Response *mtrep3__GetRepositories2Response);
	virtual int PickRepository2_(_mtrep3__PickRepository2 *mtrep3__PickRepository2, _mtrep3__PickRepository2Response *mtrep3__PickRepository2Response);
	virtual int TryGetRepositoryInfo2_(_mtrep3__TryGetRepositoryInfo2 *mtrep3__TryGetRepositoryInfo2, _mtrep3__TryGetRepositoryInfo2Response *mtrep3__TryGetRepositoryInfo2Response);
		const char *soap_endpoint;
};
#endif
