/* repositoryStub.h
   Generated by gSOAP 2.7.9d from C:/work3/miktex-2.7/Libraries/MiKTeX/WebServiceClients/Repository/Repository.h
   Copyright(C) 2000-2006, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef repositoryStub_H
#define repositoryStub_H
#include <vector>
#include "stdsoap2.h"

/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


#ifndef SOAP_TYPE_mtrep__Integrities
#define SOAP_TYPE_mtrep__Integrities (32)
/* mtrep:Integrities */
enum mtrep__Integrities {mtrep__Integrities__Intact = 0, mtrep__Integrities__Corrupted = 1, mtrep__Integrities__Unknown = 2};
#endif

#ifndef SOAP_TYPE_mtrep__Levels
#define SOAP_TYPE_mtrep__Levels (33)
/* mtrep:Levels */
enum mtrep__Levels {mtrep__Levels__Essential = 0, mtrep__Levels__Basic = 1, mtrep__Levels__Advanced = 2, mtrep__Levels__Complete = 3};
#endif

#ifndef SOAP_TYPE_mtrep__RepositoryStatus
#define SOAP_TYPE_mtrep__RepositoryStatus (34)
/* mtrep:RepositoryStatus */
enum mtrep__RepositoryStatus {mtrep__RepositoryStatus__Online = 0, mtrep__RepositoryStatus__Offline = 1, mtrep__RepositoryStatus__Unknown = 2};
#endif

#ifndef SOAP_TYPE_mtrep3__Integrities
#define SOAP_TYPE_mtrep3__Integrities (35)
/* mtrep3:Integrities */
enum mtrep3__Integrities {mtrep3__Integrities__Intact = 0, mtrep3__Integrities__Corrupted = 1, mtrep3__Integrities__Unknown = 2};
#endif

#ifndef SOAP_TYPE_mtrep3__Levels
#define SOAP_TYPE_mtrep3__Levels (36)
/* mtrep3:Levels */
enum mtrep3__Levels {mtrep3__Levels__Essential = 0, mtrep3__Levels__Basic = 1, mtrep3__Levels__Advanced = 2, mtrep3__Levels__Complete = 3};
#endif

#ifndef SOAP_TYPE_mtrep3__RepositoryStatus
#define SOAP_TYPE_mtrep3__RepositoryStatus (37)
/* mtrep3:RepositoryStatus */
enum mtrep3__RepositoryStatus {mtrep3__RepositoryStatus__Online = 0, mtrep3__RepositoryStatus__Offline = 1, mtrep3__RepositoryStatus__Unknown = 2};
#endif

#ifndef SOAP_TYPE_mtrep4__Integrities
#define SOAP_TYPE_mtrep4__Integrities (38)
/* mtrep4:Integrities */
enum mtrep4__Integrities {mtrep4__Integrities__Intact = 0, mtrep4__Integrities__Corrupted = 1, mtrep4__Integrities__Unknown = 2};
#endif

#ifndef SOAP_TYPE_mtrep4__Levels
#define SOAP_TYPE_mtrep4__Levels (39)
/* mtrep4:Levels */
enum mtrep4__Levels {mtrep4__Levels__Essential = 0, mtrep4__Levels__Basic = 1, mtrep4__Levels__Advanced = 2, mtrep4__Levels__Complete = 3};
#endif

#ifndef SOAP_TYPE_mtrep4__RepositoryStatus
#define SOAP_TYPE_mtrep4__RepositoryStatus (40)
/* mtrep4:RepositoryStatus */
enum mtrep4__RepositoryStatus {mtrep4__RepositoryStatus__Online = 0, mtrep4__RepositoryStatus__Offline = 1, mtrep4__RepositoryStatus__Unknown = 2};
#endif

/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not redeclare */

#endif



#ifndef SOAP_TYPE_mtrep__RepositoryInfo
#define SOAP_TYPE_mtrep__RepositoryInfo (7)
/* mtrep:RepositoryInfo */
class SOAP_CMAC mtrep__RepositoryInfo
{
public:
	std::string *Country;	/* optional element of type xsd:string */
	int Date;	/* required element of type xsd:int */
	int Delay;	/* required element of type xsd:int */
	std::string *Description;	/* optional element of type xsd:string */
	enum mtrep__Integrities Integrity;	/* required element of type mtrep:Integrities */
	enum mtrep__Levels Level;	/* required element of type mtrep:Levels */
	enum mtrep__RepositoryStatus Status;	/* required element of type mtrep:RepositoryStatus */
	std::string *Url;	/* optional element of type xsd:string */
	int Version;	/* required element of type xsd:int */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 7; } /* = unique id SOAP_TYPE_mtrep__RepositoryInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         mtrep__RepositoryInfo() : Country(NULL), Description(NULL), Url(NULL), soap(NULL) { }
	virtual ~mtrep__RepositoryInfo() { }
};
#endif

#ifndef SOAP_TYPE_mtrep__ArrayOfRepositoryInfo
#define SOAP_TYPE_mtrep__ArrayOfRepositoryInfo (8)
/* mtrep:ArrayOfRepositoryInfo */
class SOAP_CMAC mtrep__ArrayOfRepositoryInfo
{
public:
	std::vector<mtrep__RepositoryInfo * >RepositoryInfo;	/* optional element of type mtrep:RepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 8; } /* = unique id SOAP_TYPE_mtrep__ArrayOfRepositoryInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         mtrep__ArrayOfRepositoryInfo() : soap(NULL) { }
	virtual ~mtrep__ArrayOfRepositoryInfo() { }
};
#endif

#ifndef SOAP_TYPE__mtrep__TryGetRepositoryInfo
#define SOAP_TYPE__mtrep__TryGetRepositoryInfo (9)
/* mtrep:TryGetRepositoryInfo */
class SOAP_CMAC _mtrep__TryGetRepositoryInfo
{
public:
	std::string *url;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 9; } /* = unique id SOAP_TYPE__mtrep__TryGetRepositoryInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep__TryGetRepositoryInfo() : url(NULL), soap(NULL) { }
	virtual ~_mtrep__TryGetRepositoryInfo() { }
};
#endif

#ifndef SOAP_TYPE__mtrep__TryGetRepositoryInfoResponse
#define SOAP_TYPE__mtrep__TryGetRepositoryInfoResponse (10)
/* mtrep:TryGetRepositoryInfoResponse */
class SOAP_CMAC _mtrep__TryGetRepositoryInfoResponse
{
public:
	bool TryGetRepositoryInfoResult;	/* RPC return element */	/* required element of type xsd:boolean */
	mtrep__RepositoryInfo *repositoryInfo;	/* optional element of type mtrep:RepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 10; } /* = unique id SOAP_TYPE__mtrep__TryGetRepositoryInfoResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep__TryGetRepositoryInfoResponse() : repositoryInfo(NULL), soap(NULL) { }
	virtual ~_mtrep__TryGetRepositoryInfoResponse() { }
};
#endif

#ifndef SOAP_TYPE__mtrep__PickRepository
#define SOAP_TYPE__mtrep__PickRepository (11)
/* mtrep:PickRepository */
class SOAP_CMAC _mtrep__PickRepository
{
public:
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 11; } /* = unique id SOAP_TYPE__mtrep__PickRepository */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep__PickRepository() : soap(NULL) { }
	virtual ~_mtrep__PickRepository() { }
};
#endif

#ifndef SOAP_TYPE__mtrep__PickRepositoryResponse
#define SOAP_TYPE__mtrep__PickRepositoryResponse (12)
/* mtrep:PickRepositoryResponse */
class SOAP_CMAC _mtrep__PickRepositoryResponse
{
public:
	mtrep__RepositoryInfo *PickRepositoryResult;	/* RPC return element */	/* optional element of type mtrep:RepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 12; } /* = unique id SOAP_TYPE__mtrep__PickRepositoryResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep__PickRepositoryResponse() : PickRepositoryResult(NULL), soap(NULL) { }
	virtual ~_mtrep__PickRepositoryResponse() { }
};
#endif

#ifndef SOAP_TYPE__mtrep__GetAllRepositories
#define SOAP_TYPE__mtrep__GetAllRepositories (13)
/* mtrep:GetAllRepositories */
class SOAP_CMAC _mtrep__GetAllRepositories
{
public:
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 13; } /* = unique id SOAP_TYPE__mtrep__GetAllRepositories */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep__GetAllRepositories() : soap(NULL) { }
	virtual ~_mtrep__GetAllRepositories() { }
};
#endif

#ifndef SOAP_TYPE__mtrep__GetAllRepositoriesResponse
#define SOAP_TYPE__mtrep__GetAllRepositoriesResponse (14)
/* mtrep:GetAllRepositoriesResponse */
class SOAP_CMAC _mtrep__GetAllRepositoriesResponse
{
public:
	mtrep__ArrayOfRepositoryInfo *GetAllRepositoriesResult;	/* RPC return element */	/* optional element of type mtrep:ArrayOfRepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 14; } /* = unique id SOAP_TYPE__mtrep__GetAllRepositoriesResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep__GetAllRepositoriesResponse() : GetAllRepositoriesResult(NULL), soap(NULL) { }
	virtual ~_mtrep__GetAllRepositoriesResponse() { }
};
#endif

#ifndef SOAP_TYPE__mtrep__GetRepositories
#define SOAP_TYPE__mtrep__GetRepositories (15)
/* mtrep:GetRepositories */
class SOAP_CMAC _mtrep__GetRepositories
{
public:
	bool onlyOnline;	/* required element of type xsd:boolean */
	bool noCorrupted;	/* required element of type xsd:boolean */
	int maxDelay;	/* required element of type xsd:int */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 15; } /* = unique id SOAP_TYPE__mtrep__GetRepositories */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep__GetRepositories() : soap(NULL) { }
	virtual ~_mtrep__GetRepositories() { }
};
#endif

#ifndef SOAP_TYPE__mtrep__GetRepositoriesResponse
#define SOAP_TYPE__mtrep__GetRepositoriesResponse (16)
/* mtrep:GetRepositoriesResponse */
class SOAP_CMAC _mtrep__GetRepositoriesResponse
{
public:
	mtrep__ArrayOfRepositoryInfo *GetRepositoriesResult;	/* RPC return element */	/* optional element of type mtrep:ArrayOfRepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 16; } /* = unique id SOAP_TYPE__mtrep__GetRepositoriesResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep__GetRepositoriesResponse() : GetRepositoriesResult(NULL), soap(NULL) { }
	virtual ~_mtrep__GetRepositoriesResponse() { }
};
#endif

#ifndef SOAP_TYPE__mtrep2__GetListCreationTime
#define SOAP_TYPE__mtrep2__GetListCreationTime (17)
/* mtrep2:GetListCreationTime */
class SOAP_CMAC _mtrep2__GetListCreationTime
{
public:
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 17; } /* = unique id SOAP_TYPE__mtrep2__GetListCreationTime */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep2__GetListCreationTime() : soap(NULL) { }
	virtual ~_mtrep2__GetListCreationTime() { }
};
#endif

#ifndef SOAP_TYPE__mtrep2__GetListCreationTimeResponse
#define SOAP_TYPE__mtrep2__GetListCreationTimeResponse (18)
/* mtrep2:GetListCreationTimeResponse */
class SOAP_CMAC _mtrep2__GetListCreationTimeResponse
{
public:
	int GetListCreationTimeResult;	/* RPC return element */	/* required element of type xsd:int */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 18; } /* = unique id SOAP_TYPE__mtrep2__GetListCreationTimeResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep2__GetListCreationTimeResponse() : soap(NULL) { }
	virtual ~_mtrep2__GetListCreationTimeResponse() { }
};
#endif

#ifndef SOAP_TYPE_mtrep3__ClientInfo
#define SOAP_TYPE_mtrep3__ClientInfo (19)
/* mtrep3:ClientInfo */
class SOAP_CMAC mtrep3__ClientInfo
{
public:
	std::string *Name;	/* optional element of type xsd:string */
	std::string *Version;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 19; } /* = unique id SOAP_TYPE_mtrep3__ClientInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         mtrep3__ClientInfo() : Name(NULL), Version(NULL), soap(NULL) { }
	virtual ~mtrep3__ClientInfo() { }
};
#endif

#ifndef SOAP_TYPE_mtrep3__ArrayOfRepositoryInfo
#define SOAP_TYPE_mtrep3__ArrayOfRepositoryInfo (20)
/* mtrep3:ArrayOfRepositoryInfo */
class SOAP_CMAC mtrep3__ArrayOfRepositoryInfo
{
public:
	std::vector<class mtrep3__RepositoryInfo * >RepositoryInfo;	/* optional element of type mtrep3:RepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 20; } /* = unique id SOAP_TYPE_mtrep3__ArrayOfRepositoryInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         mtrep3__ArrayOfRepositoryInfo() : soap(NULL) { }
	virtual ~mtrep3__ArrayOfRepositoryInfo() { }
};
#endif

#ifndef SOAP_TYPE_mtrep3__RepositoryInfo
#define SOAP_TYPE_mtrep3__RepositoryInfo (21)
/* mtrep3:RepositoryInfo */
class SOAP_CMAC mtrep3__RepositoryInfo
{
public:
	std::string *Country;	/* optional element of type xsd:string */
	int Date;	/* required element of type xsd:int */
	int Delay;	/* required element of type xsd:int */
	std::string *Description;	/* optional element of type xsd:string */
	enum mtrep3__Integrities Integrity;	/* required element of type mtrep3:Integrities */
	enum mtrep3__Levels Level;	/* required element of type mtrep3:Levels */
	enum mtrep3__RepositoryStatus Status;	/* required element of type mtrep3:RepositoryStatus */
	std::string *Url;	/* optional element of type xsd:string */
	int Version;	/* required element of type xsd:int */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 21; } /* = unique id SOAP_TYPE_mtrep3__RepositoryInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         mtrep3__RepositoryInfo() : Country(NULL), Description(NULL), Url(NULL), soap(NULL) { }
	virtual ~mtrep3__RepositoryInfo() { }
};
#endif

#ifndef SOAP_TYPE__mtrep3__GetRepositories2
#define SOAP_TYPE__mtrep3__GetRepositories2 (22)
/* mtrep3:GetRepositories2 */
class SOAP_CMAC _mtrep3__GetRepositories2
{
public:
	mtrep3__ClientInfo *clientInfo;	/* optional element of type mtrep3:ClientInfo */
	bool onlyOnline;	/* required element of type xsd:boolean */
	bool noCorrupted;	/* required element of type xsd:boolean */
	int maxDelay;	/* required element of type xsd:int */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 22; } /* = unique id SOAP_TYPE__mtrep3__GetRepositories2 */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep3__GetRepositories2() : clientInfo(NULL), soap(NULL) { }
	virtual ~_mtrep3__GetRepositories2() { }
};
#endif

#ifndef SOAP_TYPE__mtrep3__GetRepositories2Response
#define SOAP_TYPE__mtrep3__GetRepositories2Response (23)
/* mtrep3:GetRepositories2Response */
class SOAP_CMAC _mtrep3__GetRepositories2Response
{
public:
	mtrep3__ArrayOfRepositoryInfo *GetRepositories2Result;	/* RPC return element */	/* optional element of type mtrep3:ArrayOfRepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 23; } /* = unique id SOAP_TYPE__mtrep3__GetRepositories2Response */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep3__GetRepositories2Response() : GetRepositories2Result(NULL), soap(NULL) { }
	virtual ~_mtrep3__GetRepositories2Response() { }
};
#endif

#ifndef SOAP_TYPE__mtrep3__PickRepository2
#define SOAP_TYPE__mtrep3__PickRepository2 (24)
/* mtrep3:PickRepository2 */
class SOAP_CMAC _mtrep3__PickRepository2
{
public:
	mtrep3__ClientInfo *clientInfo;	/* optional element of type mtrep3:ClientInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 24; } /* = unique id SOAP_TYPE__mtrep3__PickRepository2 */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep3__PickRepository2() : clientInfo(NULL), soap(NULL) { }
	virtual ~_mtrep3__PickRepository2() { }
};
#endif

#ifndef SOAP_TYPE__mtrep3__PickRepository2Response
#define SOAP_TYPE__mtrep3__PickRepository2Response (25)
/* mtrep3:PickRepository2Response */
class SOAP_CMAC _mtrep3__PickRepository2Response
{
public:
	mtrep3__RepositoryInfo *PickRepository2Result;	/* RPC return element */	/* optional element of type mtrep3:RepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 25; } /* = unique id SOAP_TYPE__mtrep3__PickRepository2Response */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep3__PickRepository2Response() : PickRepository2Result(NULL), soap(NULL) { }
	virtual ~_mtrep3__PickRepository2Response() { }
};
#endif

#ifndef SOAP_TYPE__mtrep3__TryGetRepositoryInfo2
#define SOAP_TYPE__mtrep3__TryGetRepositoryInfo2 (26)
/* mtrep3:TryGetRepositoryInfo2 */
class SOAP_CMAC _mtrep3__TryGetRepositoryInfo2
{
public:
	mtrep3__ClientInfo *clientInfo;	/* optional element of type mtrep3:ClientInfo */
	std::string *url;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 26; } /* = unique id SOAP_TYPE__mtrep3__TryGetRepositoryInfo2 */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep3__TryGetRepositoryInfo2() : clientInfo(NULL), url(NULL), soap(NULL) { }
	virtual ~_mtrep3__TryGetRepositoryInfo2() { }
};
#endif

#ifndef SOAP_TYPE__mtrep3__TryGetRepositoryInfo2Response
#define SOAP_TYPE__mtrep3__TryGetRepositoryInfo2Response (27)
/* mtrep3:TryGetRepositoryInfo2Response */
class SOAP_CMAC _mtrep3__TryGetRepositoryInfo2Response
{
public:
	bool TryGetRepositoryInfo2Result;	/* RPC return element */	/* required element of type xsd:boolean */
	mtrep3__RepositoryInfo *repositoryInfo;	/* optional element of type mtrep3:RepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 27; } /* = unique id SOAP_TYPE__mtrep3__TryGetRepositoryInfo2Response */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep3__TryGetRepositoryInfo2Response() : repositoryInfo(NULL), soap(NULL) { }
	virtual ~_mtrep3__TryGetRepositoryInfo2Response() { }
};
#endif

#ifndef SOAP_TYPE_mtrep4__ClientInfo
#define SOAP_TYPE_mtrep4__ClientInfo (28)
/* mtrep4:ClientInfo */
class SOAP_CMAC mtrep4__ClientInfo
{
public:
	std::string *Name;	/* optional element of type xsd:string */
	std::string *Version;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 28; } /* = unique id SOAP_TYPE_mtrep4__ClientInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         mtrep4__ClientInfo() : Name(NULL), Version(NULL), soap(NULL) { }
	virtual ~mtrep4__ClientInfo() { }
};
#endif

#ifndef SOAP_TYPE_mtrep4__RepositoryInfo
#define SOAP_TYPE_mtrep4__RepositoryInfo (29)
/* mtrep4:RepositoryInfo */
class SOAP_CMAC mtrep4__RepositoryInfo
{
public:
	std::string *Country;	/* optional element of type xsd:string */
	int Date;	/* required element of type xsd:int */
	int Delay;	/* required element of type xsd:int */
	std::string *Description;	/* optional element of type xsd:string */
	enum mtrep4__Integrities Integrity;	/* required element of type mtrep4:Integrities */
	enum mtrep4__Levels Level;	/* required element of type mtrep4:Levels */
	enum mtrep4__RepositoryStatus Status;	/* required element of type mtrep4:RepositoryStatus */
	std::string *Url;	/* optional element of type xsd:string */
	int Version;	/* required element of type xsd:int */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 29; } /* = unique id SOAP_TYPE_mtrep4__RepositoryInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         mtrep4__RepositoryInfo() : Country(NULL), Description(NULL), Url(NULL), soap(NULL) { }
	virtual ~mtrep4__RepositoryInfo() { }
};
#endif

#ifndef SOAP_TYPE__mtrep4__VerifyRepository
#define SOAP_TYPE__mtrep4__VerifyRepository (30)
/* mtrep4:VerifyRepository */
class SOAP_CMAC _mtrep4__VerifyRepository
{
public:
	mtrep4__ClientInfo *clientInfo;	/* optional element of type mtrep4:ClientInfo */
	std::string *url;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 30; } /* = unique id SOAP_TYPE__mtrep4__VerifyRepository */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep4__VerifyRepository() : clientInfo(NULL), url(NULL), soap(NULL) { }
	virtual ~_mtrep4__VerifyRepository() { }
};
#endif

#ifndef SOAP_TYPE__mtrep4__VerifyRepositoryResponse
#define SOAP_TYPE__mtrep4__VerifyRepositoryResponse (31)
/* mtrep4:VerifyRepositoryResponse */
class SOAP_CMAC _mtrep4__VerifyRepositoryResponse
{
public:
	bool VerifyRepositoryResult;	/* RPC return element */	/* required element of type xsd:boolean */
	mtrep4__RepositoryInfo *repositoryInfo;	/* optional element of type mtrep4:RepositoryInfo */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 31; } /* = unique id SOAP_TYPE__mtrep4__VerifyRepositoryResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _mtrep4__VerifyRepositoryResponse() : repositoryInfo(NULL), soap(NULL) { }
	virtual ~_mtrep4__VerifyRepositoryResponse() { }
};
#endif

#ifndef SOAP_TYPE___mtrep4__TryGetRepositoryInfo
#define SOAP_TYPE___mtrep4__TryGetRepositoryInfo (58)
/* Operation wrapper: */
struct __mtrep4__TryGetRepositoryInfo
{
public:
	_mtrep__TryGetRepositoryInfo *mtrep__TryGetRepositoryInfo;	/* optional element of type mtrep:TryGetRepositoryInfo */
};
#endif

#ifndef SOAP_TYPE___mtrep4__PickRepository
#define SOAP_TYPE___mtrep4__PickRepository (62)
/* Operation wrapper: */
struct __mtrep4__PickRepository
{
public:
	_mtrep__PickRepository *mtrep__PickRepository;	/* optional element of type mtrep:PickRepository */
};
#endif

#ifndef SOAP_TYPE___mtrep4__GetAllRepositories
#define SOAP_TYPE___mtrep4__GetAllRepositories (66)
/* Operation wrapper: */
struct __mtrep4__GetAllRepositories
{
public:
	_mtrep__GetAllRepositories *mtrep__GetAllRepositories;	/* optional element of type mtrep:GetAllRepositories */
};
#endif

#ifndef SOAP_TYPE___mtrep4__GetRepositories
#define SOAP_TYPE___mtrep4__GetRepositories (70)
/* Operation wrapper: */
struct __mtrep4__GetRepositories
{
public:
	_mtrep__GetRepositories *mtrep__GetRepositories;	/* optional element of type mtrep:GetRepositories */
};
#endif

#ifndef SOAP_TYPE___mtrep4__GetListCreationTime
#define SOAP_TYPE___mtrep4__GetListCreationTime (74)
/* Operation wrapper: */
struct __mtrep4__GetListCreationTime
{
public:
	_mtrep2__GetListCreationTime *mtrep2__GetListCreationTime;	/* optional element of type mtrep2:GetListCreationTime */
};
#endif

#ifndef SOAP_TYPE___mtrep4__GetRepositories2
#define SOAP_TYPE___mtrep4__GetRepositories2 (78)
/* Operation wrapper: */
struct __mtrep4__GetRepositories2
{
public:
	_mtrep3__GetRepositories2 *mtrep3__GetRepositories2;	/* optional element of type mtrep3:GetRepositories2 */
};
#endif

#ifndef SOAP_TYPE___mtrep4__PickRepository2
#define SOAP_TYPE___mtrep4__PickRepository2 (82)
/* Operation wrapper: */
struct __mtrep4__PickRepository2
{
public:
	_mtrep3__PickRepository2 *mtrep3__PickRepository2;	/* optional element of type mtrep3:PickRepository2 */
};
#endif

#ifndef SOAP_TYPE___mtrep4__TryGetRepositoryInfo2
#define SOAP_TYPE___mtrep4__TryGetRepositoryInfo2 (86)
/* Operation wrapper: */
struct __mtrep4__TryGetRepositoryInfo2
{
public:
	_mtrep3__TryGetRepositoryInfo2 *mtrep3__TryGetRepositoryInfo2;	/* optional element of type mtrep3:TryGetRepositoryInfo2 */
};
#endif

#ifndef SOAP_TYPE___mtrep4__VerifyRepository
#define SOAP_TYPE___mtrep4__VerifyRepository (90)
/* Operation wrapper: */
struct __mtrep4__VerifyRepository
{
public:
	_mtrep4__VerifyRepository *mtrep4__VerifyRepository;	/* optional element of type mtrep4:VerifyRepository */
};
#endif

#ifndef SOAP_TYPE___mtrep4__TryGetRepositoryInfo_
#define SOAP_TYPE___mtrep4__TryGetRepositoryInfo_ (92)
/* Operation wrapper: */
struct __mtrep4__TryGetRepositoryInfo_
{
public:
	_mtrep__TryGetRepositoryInfo *mtrep__TryGetRepositoryInfo;	/* optional element of type mtrep:TryGetRepositoryInfo */
};
#endif

#ifndef SOAP_TYPE___mtrep4__PickRepository_
#define SOAP_TYPE___mtrep4__PickRepository_ (94)
/* Operation wrapper: */
struct __mtrep4__PickRepository_
{
public:
	_mtrep__PickRepository *mtrep__PickRepository;	/* optional element of type mtrep:PickRepository */
};
#endif

#ifndef SOAP_TYPE___mtrep4__GetAllRepositories_
#define SOAP_TYPE___mtrep4__GetAllRepositories_ (96)
/* Operation wrapper: */
struct __mtrep4__GetAllRepositories_
{
public:
	_mtrep__GetAllRepositories *mtrep__GetAllRepositories;	/* optional element of type mtrep:GetAllRepositories */
};
#endif

#ifndef SOAP_TYPE___mtrep4__GetRepositories_
#define SOAP_TYPE___mtrep4__GetRepositories_ (98)
/* Operation wrapper: */
struct __mtrep4__GetRepositories_
{
public:
	_mtrep__GetRepositories *mtrep__GetRepositories;	/* optional element of type mtrep:GetRepositories */
};
#endif

#ifndef SOAP_TYPE___mtrep4__GetListCreationTime_
#define SOAP_TYPE___mtrep4__GetListCreationTime_ (100)
/* Operation wrapper: */
struct __mtrep4__GetListCreationTime_
{
public:
	_mtrep2__GetListCreationTime *mtrep2__GetListCreationTime;	/* optional element of type mtrep2:GetListCreationTime */
};
#endif

#ifndef SOAP_TYPE___mtrep4__GetRepositories2_
#define SOAP_TYPE___mtrep4__GetRepositories2_ (102)
/* Operation wrapper: */
struct __mtrep4__GetRepositories2_
{
public:
	_mtrep3__GetRepositories2 *mtrep3__GetRepositories2;	/* optional element of type mtrep3:GetRepositories2 */
};
#endif

#ifndef SOAP_TYPE___mtrep4__PickRepository2_
#define SOAP_TYPE___mtrep4__PickRepository2_ (104)
/* Operation wrapper: */
struct __mtrep4__PickRepository2_
{
public:
	_mtrep3__PickRepository2 *mtrep3__PickRepository2;	/* optional element of type mtrep3:PickRepository2 */
};
#endif

#ifndef SOAP_TYPE___mtrep4__TryGetRepositoryInfo2_
#define SOAP_TYPE___mtrep4__TryGetRepositoryInfo2_ (106)
/* Operation wrapper: */
struct __mtrep4__TryGetRepositoryInfo2_
{
public:
	_mtrep3__TryGetRepositoryInfo2 *mtrep3__TryGetRepositoryInfo2;	/* optional element of type mtrep3:TryGetRepositoryInfo2 */
};
#endif

#ifndef SOAP_TYPE___mtrep4__VerifyRepository_
#define SOAP_TYPE___mtrep4__VerifyRepository_ (108)
/* Operation wrapper: */
struct __mtrep4__VerifyRepository_
{
public:
	_mtrep4__VerifyRepository *mtrep4__VerifyRepository;	/* optional element of type mtrep4:VerifyRepository */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (111)
/* SOAP Header: */
struct SOAP_ENV__Header
{
public:
	void *dummy;	/* transient */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (112)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (114)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
	char *__any;
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (115)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (116)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
public:
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
};
#endif

/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (4)
typedef char *_XML;
#endif

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif


/******************************************************************************\
 *                                                                            *
 * Typedef Synonyms                                                           *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


#endif

/* End of repositoryStub.h */
