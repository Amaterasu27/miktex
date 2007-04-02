// Reminder: Modify typemap.dat to customize the header file generated by wsdl2h
/* C:/work3/miktex-2.6/Libraries/MiKTeX/WebServiceClients/Repository/Repository.h
   Generated by wsdl2h 1.2.9d from http://api.miktex.org/Repository.asmx?WSDL and typemap.dat
   2007-04-01 17:57:26 GMT
   Copyright (C) 2001-2006 Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL or Genivia's license for commercial use.
*/

/* NOTE:

 - Compile this file with soapcpp2 to complete the code generation process.
 - Use soapcpp2 option -I to specify paths for #import
   To build with STL, 'stlvector.h' is imported from 'import' dir in package.
 - Use wsdl2h options -c and -s to generate pure C code or C++ code without STL.
 - Use 'typemap.dat' to control schema namespace bindings and type mappings.
   It is strongly recommended to customize the names of the namespace prefixes
   generated by wsdl2h. To do so, modify the prefix bindings in the Namespaces
   section below and add the modified lines to 'typemap.dat' to rerun wsdl2h.
 - Use Doxygen (www.doxygen.org) to browse this file.
 - Use wsdl2h option -l to view the software license terms.

   DO NOT include this file directly into your project.
   Include only the soapcpp2-generated headers and source code files.
*/

//gsoapopt w

/******************************************************************************\
 *                                                                            *
 * http://miktex.org/2007/03/repository                                       *
 *                                                                            *
\******************************************************************************/

/* WSDL Documentation:

Retrieve information about the MiKTeX package repositories.
*/


/******************************************************************************\
 *                                                                            *
 * Import                                                                     *
 *                                                                            *
\******************************************************************************/


// STL vector containers (use option -s to disable)
#import "stlvector.h"

/******************************************************************************\
 *                                                                            *
 * Schema Namespaces                                                          *
 *                                                                            *
\******************************************************************************/

// This service uses SOAP 1.2 namespaces:
//gsoap SOAP-ENV schema namespace:	http://www.w3.org/2003/05/soap-envelope
//gsoap SOAP-ENC schema namespace:	http://www.w3.org/2003/05/soap-encoding

/* NOTE:

It is strongly recommended to customize the names of the namespace prefixes
generated by wsdl2h. To do so, modify the prefix bindings below and add the
modified lines to typemap.dat to rerun wsdl2h:

ns1 = "http://miktex.org/2007/03/repository"
mtrep = "http://miktex.org/2006/06/repository"

*/

//gsoap mtrep schema namespace:	http://miktex.org/2006/06/repository
//gsoap ns1   schema namespace:	http://miktex.org/2007/03/repository
//gsoap mtrep schema elementForm:	qualified
//gsoap mtrep schema attributeForm:	unqualified
//gsoap ns1   schema elementForm:	qualified
//gsoap ns1   schema attributeForm:	unqualified

/******************************************************************************\
 *                                                                            *
 * Schema Types                                                               *
 *                                                                            *
\******************************************************************************/



//  Forward declaration of class mtrep__RepositoryInfo.
class mtrep__RepositoryInfo;

//  Forward declaration of class mtrep__ArrayOfRepositoryInfo.
class mtrep__ArrayOfRepositoryInfo;

//  Forward declaration of class _mtrep__TryGetRepositoryInfo.
class _mtrep__TryGetRepositoryInfo;

//  Forward declaration of class _mtrep__TryGetRepositoryInfoResponse.
class _mtrep__TryGetRepositoryInfoResponse;

//  Forward declaration of class _mtrep__PickRepository.
class _mtrep__PickRepository;

//  Forward declaration of class _mtrep__PickRepositoryResponse.
class _mtrep__PickRepositoryResponse;

//  Forward declaration of class _mtrep__GetAllRepositories.
class _mtrep__GetAllRepositories;

//  Forward declaration of class _mtrep__GetAllRepositoriesResponse.
class _mtrep__GetAllRepositoriesResponse;

//  Forward declaration of class _mtrep__GetRepositories.
class _mtrep__GetRepositories;

//  Forward declaration of class _mtrep__GetRepositoriesResponse.
class _mtrep__GetRepositoriesResponse;

//  Forward declaration of class _ns1__GetListCreationTime.
class _ns1__GetListCreationTime;

//  Forward declaration of class _ns1__GetListCreationTimeResponse.
class _ns1__GetListCreationTimeResponse;

/// "http://miktex.org/2006/06/repository":Integrities is a simpleType restriction of xs:string.
/// Note: enum values are prefixed with 'mtrep__Integrities' to avoid name clashes, please use wsdl2h option -e to omit this prefix
enum mtrep__Integrities
{
	mtrep__Integrities__Intact,	///< xs:string value="Intact"
	mtrep__Integrities__Corrupted,	///< xs:string value="Corrupted"
	mtrep__Integrities__Unknown,	///< xs:string value="Unknown"
};

/// "http://miktex.org/2006/06/repository":Levels is a simpleType restriction of xs:string.
/// Note: enum values are prefixed with 'mtrep__Levels' to avoid name clashes, please use wsdl2h option -e to omit this prefix
enum mtrep__Levels
{
	mtrep__Levels__Essential,	///< xs:string value="Essential"
	mtrep__Levels__Basic,	///< xs:string value="Basic"
	mtrep__Levels__Advanced,	///< xs:string value="Advanced"
	mtrep__Levels__Complete,	///< xs:string value="Complete"
};

/// "http://miktex.org/2006/06/repository":RepositoryStatus is a simpleType restriction of xs:string.
/// Note: enum values are prefixed with 'mtrep__RepositoryStatus' to avoid name clashes, please use wsdl2h option -e to omit this prefix
enum mtrep__RepositoryStatus
{
	mtrep__RepositoryStatus__Online,	///< xs:string value="Online"
	mtrep__RepositoryStatus__Offline,	///< xs:string value="Offline"
	mtrep__RepositoryStatus__Unknown,	///< xs:string value="Unknown"
};

/// "http://miktex.org/2006/06/repository":RepositoryInfo is a complexType.
class mtrep__RepositoryInfo
{ public:
/// Element Country of type xs:string.
    std::string*                         Country                        0;	///< Optional element.
/// Element Date of type xs:int.
    int                                  Date                           1;	///< Required element.
/// Element Delay of type xs:int.
    int                                  Delay                          1;	///< Required element.
/// Element Description of type xs:string.
    std::string*                         Description                    0;	///< Optional element.
/// Element Integrity of type "http://miktex.org/2006/06/repository":Integrities.
    enum mtrep__Integrities              Integrity                      1;	///< Required element.
/// Element Level of type "http://miktex.org/2006/06/repository":Levels.
    enum mtrep__Levels                   Level                          1;	///< Required element.
/// Element Status of type "http://miktex.org/2006/06/repository":RepositoryStatus.
    enum mtrep__RepositoryStatus         Status                         1;	///< Required element.
/// Element Url of type xs:string.
    std::string*                         Url                            0;	///< Optional element.
/// Element Version of type xs:int.
    int                                  Version                        1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "http://miktex.org/2006/06/repository":ArrayOfRepositoryInfo is a complexType.
class mtrep__ArrayOfRepositoryInfo
{ public:
/// Vector of mtrep__RepositoryInfo* with length 0..unbounded
    std::vector<mtrep__RepositoryInfo* > RepositoryInfo                 0;	///< Nullable pointer.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2006/06/repository":TryGetRepositoryInfo of complexType.

/// "http://miktex.org/2006/06/repository":TryGetRepositoryInfo is a complexType.
class _mtrep__TryGetRepositoryInfo
{ public:
/// Element url of type xs:string.
    std::string*                         url                            0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2006/06/repository":TryGetRepositoryInfoResponse of complexType.

/// "http://miktex.org/2006/06/repository":TryGetRepositoryInfoResponse is a complexType.
class _mtrep__TryGetRepositoryInfoResponse
{ public:
/// Element TryGetRepositoryInfoResult of type xs:boolean.
    bool                                 TryGetRepositoryInfoResult     1;	///< Required element.
/// Element repositoryInfo of type "http://miktex.org/2006/06/repository":RepositoryInfo.
    mtrep__RepositoryInfo*               repositoryInfo                 0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2006/06/repository":PickRepository of complexType.

/// "http://miktex.org/2006/06/repository":PickRepository is a complexType.
class _mtrep__PickRepository
{ public:
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2006/06/repository":PickRepositoryResponse of complexType.

/// "http://miktex.org/2006/06/repository":PickRepositoryResponse is a complexType.
class _mtrep__PickRepositoryResponse
{ public:
/// Element PickRepositoryResult of type "http://miktex.org/2006/06/repository":RepositoryInfo.
    mtrep__RepositoryInfo*               PickRepositoryResult           0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2006/06/repository":GetAllRepositories of complexType.

/// "http://miktex.org/2006/06/repository":GetAllRepositories is a complexType.
class _mtrep__GetAllRepositories
{ public:
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2006/06/repository":GetAllRepositoriesResponse of complexType.

/// "http://miktex.org/2006/06/repository":GetAllRepositoriesResponse is a complexType.
class _mtrep__GetAllRepositoriesResponse
{ public:
/// Element GetAllRepositoriesResult of type "http://miktex.org/2006/06/repository":ArrayOfRepositoryInfo.
    mtrep__ArrayOfRepositoryInfo*        GetAllRepositoriesResult       0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2006/06/repository":GetRepositories of complexType.

/// "http://miktex.org/2006/06/repository":GetRepositories is a complexType.
class _mtrep__GetRepositories
{ public:
/// Element onlyOnline of type xs:boolean.
    bool                                 onlyOnline                     1;	///< Required element.
/// Element noCorrupted of type xs:boolean.
    bool                                 noCorrupted                    1;	///< Required element.
/// Element maxDelay of type xs:int.
    int                                  maxDelay                       1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2006/06/repository":GetRepositoriesResponse of complexType.

/// "http://miktex.org/2006/06/repository":GetRepositoriesResponse is a complexType.
class _mtrep__GetRepositoriesResponse
{ public:
/// Element GetRepositoriesResult of type "http://miktex.org/2006/06/repository":ArrayOfRepositoryInfo.
    mtrep__ArrayOfRepositoryInfo*        GetRepositoriesResult          0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2007/03/repository":GetListCreationTime of complexType.

/// "http://miktex.org/2007/03/repository":GetListCreationTime is a complexType.
class _ns1__GetListCreationTime
{ public:
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Element "http://miktex.org/2007/03/repository":GetListCreationTimeResponse of complexType.

/// "http://miktex.org/2007/03/repository":GetListCreationTimeResponse is a complexType.
class _ns1__GetListCreationTimeResponse
{ public:
/// Element GetListCreationTimeResult of type xs:int.
    int                                  GetListCreationTimeResult      1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/******************************************************************************\
 *                                                                            *
 * Services                                                                   *
 *                                                                            *
\******************************************************************************/


//gsoap ns1  service name:	RepositorySoap 
//gsoap ns1  service type:	RepositorySoap 
//gsoap ns1  service port:	http://api.miktex.org/Repository.asmx 
//gsoap ns1  service namespace:	http://miktex.org/2007/03/repository 
//gsoap ns1  service transport:	http://schemas.xmlsoap.org/soap/http 

/** @mainpage Service Definitions

@section Service_documentation Documentation
Retrieve information about the MiKTeX package repositories.

@section Service_bindings Bindings
  - @ref RepositorySoap

*/

/**

@page RepositorySoap Binding "RepositorySoap"

@section Repository_service Service Documentation "Repository"
Retrieve information about the MiKTeX package repositories.

@section RepositorySoap_operations Operations of Binding  "RepositorySoap"
  - @ref __ns1__TryGetRepositoryInfo
  - @ref __ns1__PickRepository
  - @ref __ns1__GetAllRepositories
  - @ref __ns1__GetRepositories
  - @ref __ns1__GetListCreationTime
  - @ref __ns1__TryGetRepositoryInfo_
  - @ref __ns1__PickRepository_
  - @ref __ns1__GetAllRepositories_
  - @ref __ns1__GetRepositories_
  - @ref __ns1__GetListCreationTime_

@section RepositorySoap_ports Endpoints of Binding  "RepositorySoap"
  - http://api.miktex.org/Repository.asmx

*/

/******************************************************************************\
 *                                                                            *
 * RepositorySoap                                                             *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * __ns1__TryGetRepositoryInfo                                                *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__TryGetRepositoryInfo" of service binding "RepositorySoap"

/**

Operation details:

Try to get information about a package repository.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2006/06/repository/TryGetRepositoryInfo"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__TryGetRepositoryInfo(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _mtrep__TryGetRepositoryInfo*       mtrep__TryGetRepositoryInfo,
    // response parameters:
    _mtrep__TryGetRepositoryInfoResponse* mtrep__TryGetRepositoryInfoResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__TryGetRepositoryInfo(
    struct soap *soap,
    // request parameters:
    _mtrep__TryGetRepositoryInfo*       mtrep__TryGetRepositoryInfo,
    // response parameters:
    _mtrep__TryGetRepositoryInfoResponse* mtrep__TryGetRepositoryInfoResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	TryGetRepositoryInfo document
//gsoap ns1  service method-encoding:	TryGetRepositoryInfo literal
//gsoap ns1  service method-action:	TryGetRepositoryInfo http://miktex.org/2006/06/repository/TryGetRepositoryInfo
int __ns1__TryGetRepositoryInfo(
    _mtrep__TryGetRepositoryInfo*       mtrep__TryGetRepositoryInfo,	///< Request parameter
    _mtrep__TryGetRepositoryInfoResponse* mtrep__TryGetRepositoryInfoResponse	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __ns1__PickRepository                                                      *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__PickRepository" of service binding "RepositorySoap"

/**

Operation details:

Pick out a random package repository.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2006/06/repository/PickRepository"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__PickRepository(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _mtrep__PickRepository*             mtrep__PickRepository,
    // response parameters:
    _mtrep__PickRepositoryResponse*     mtrep__PickRepositoryResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__PickRepository(
    struct soap *soap,
    // request parameters:
    _mtrep__PickRepository*             mtrep__PickRepository,
    // response parameters:
    _mtrep__PickRepositoryResponse*     mtrep__PickRepositoryResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	PickRepository document
//gsoap ns1  service method-encoding:	PickRepository literal
//gsoap ns1  service method-action:	PickRepository http://miktex.org/2006/06/repository/PickRepository
int __ns1__PickRepository(
    _mtrep__PickRepository*             mtrep__PickRepository,	///< Request parameter
    _mtrep__PickRepositoryResponse*     mtrep__PickRepositoryResponse	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __ns1__GetAllRepositories                                                  *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__GetAllRepositories" of service binding "RepositorySoap"

/**

Operation details:

Get all known package repositories.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2006/06/repository/GetAllRepositories"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__GetAllRepositories(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _mtrep__GetAllRepositories*         mtrep__GetAllRepositories,
    // response parameters:
    _mtrep__GetAllRepositoriesResponse* mtrep__GetAllRepositoriesResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__GetAllRepositories(
    struct soap *soap,
    // request parameters:
    _mtrep__GetAllRepositories*         mtrep__GetAllRepositories,
    // response parameters:
    _mtrep__GetAllRepositoriesResponse* mtrep__GetAllRepositoriesResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	GetAllRepositories document
//gsoap ns1  service method-encoding:	GetAllRepositories literal
//gsoap ns1  service method-action:	GetAllRepositories http://miktex.org/2006/06/repository/GetAllRepositories
int __ns1__GetAllRepositories(
    _mtrep__GetAllRepositories*         mtrep__GetAllRepositories,	///< Request parameter
    _mtrep__GetAllRepositoriesResponse* mtrep__GetAllRepositoriesResponse	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __ns1__GetRepositories                                                     *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__GetRepositories" of service binding "RepositorySoap"

/**

Operation details:

Get a filtered list of known package repositories.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2006/06/repository/GetRepositories"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__GetRepositories(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _mtrep__GetRepositories*            mtrep__GetRepositories,
    // response parameters:
    _mtrep__GetRepositoriesResponse*    mtrep__GetRepositoriesResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__GetRepositories(
    struct soap *soap,
    // request parameters:
    _mtrep__GetRepositories*            mtrep__GetRepositories,
    // response parameters:
    _mtrep__GetRepositoriesResponse*    mtrep__GetRepositoriesResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	GetRepositories document
//gsoap ns1  service method-encoding:	GetRepositories literal
//gsoap ns1  service method-action:	GetRepositories http://miktex.org/2006/06/repository/GetRepositories
int __ns1__GetRepositories(
    _mtrep__GetRepositories*            mtrep__GetRepositories,	///< Request parameter
    _mtrep__GetRepositoriesResponse*    mtrep__GetRepositoriesResponse	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __ns1__GetListCreationTime                                                 *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__GetListCreationTime" of service binding "RepositorySoap"

/**

Operation details:

Get the creation date and time of the repository list.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2007/03/repository/GetListCreationTime"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__GetListCreationTime(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _ns1__GetListCreationTime*          ns1__GetListCreationTime,
    // response parameters:
    _ns1__GetListCreationTimeResponse*  ns1__GetListCreationTimeResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__GetListCreationTime(
    struct soap *soap,
    // request parameters:
    _ns1__GetListCreationTime*          ns1__GetListCreationTime,
    // response parameters:
    _ns1__GetListCreationTimeResponse*  ns1__GetListCreationTimeResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	GetListCreationTime document
//gsoap ns1  service method-encoding:	GetListCreationTime literal
//gsoap ns1  service method-action:	GetListCreationTime http://miktex.org/2007/03/repository/GetListCreationTime
int __ns1__GetListCreationTime(
    _ns1__GetListCreationTime*          ns1__GetListCreationTime,	///< Request parameter
    _ns1__GetListCreationTimeResponse*  ns1__GetListCreationTimeResponse	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __ns1__TryGetRepositoryInfo_                                               *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__TryGetRepositoryInfo_" of service binding "RepositorySoap"

/**

Operation details:

Try to get information about a package repository.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2006/06/repository/TryGetRepositoryInfo"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__TryGetRepositoryInfo_(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _mtrep__TryGetRepositoryInfo*       mtrep__TryGetRepositoryInfo,
    // response parameters:
    _mtrep__TryGetRepositoryInfoResponse* mtrep__TryGetRepositoryInfoResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__TryGetRepositoryInfo_(
    struct soap *soap,
    // request parameters:
    _mtrep__TryGetRepositoryInfo*       mtrep__TryGetRepositoryInfo,
    // response parameters:
    _mtrep__TryGetRepositoryInfoResponse* mtrep__TryGetRepositoryInfoResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	TryGetRepositoryInfo_ document
//gsoap ns1  service method-encoding:	TryGetRepositoryInfo_ literal
//gsoap ns1  service method-action:	TryGetRepositoryInfo_ http://miktex.org/2006/06/repository/TryGetRepositoryInfo
int __ns1__TryGetRepositoryInfo_(
    _mtrep__TryGetRepositoryInfo*       mtrep__TryGetRepositoryInfo,	///< Request parameter
    _mtrep__TryGetRepositoryInfoResponse* mtrep__TryGetRepositoryInfoResponse	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __ns1__PickRepository_                                                     *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__PickRepository_" of service binding "RepositorySoap"

/**

Operation details:

Pick out a random package repository.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2006/06/repository/PickRepository"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__PickRepository_(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _mtrep__PickRepository*             mtrep__PickRepository,
    // response parameters:
    _mtrep__PickRepositoryResponse*     mtrep__PickRepositoryResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__PickRepository_(
    struct soap *soap,
    // request parameters:
    _mtrep__PickRepository*             mtrep__PickRepository,
    // response parameters:
    _mtrep__PickRepositoryResponse*     mtrep__PickRepositoryResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	PickRepository_ document
//gsoap ns1  service method-encoding:	PickRepository_ literal
//gsoap ns1  service method-action:	PickRepository_ http://miktex.org/2006/06/repository/PickRepository
int __ns1__PickRepository_(
    _mtrep__PickRepository*             mtrep__PickRepository,	///< Request parameter
    _mtrep__PickRepositoryResponse*     mtrep__PickRepositoryResponse	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __ns1__GetAllRepositories_                                                 *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__GetAllRepositories_" of service binding "RepositorySoap"

/**

Operation details:

Get all known package repositories.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2006/06/repository/GetAllRepositories"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__GetAllRepositories_(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _mtrep__GetAllRepositories*         mtrep__GetAllRepositories,
    // response parameters:
    _mtrep__GetAllRepositoriesResponse* mtrep__GetAllRepositoriesResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__GetAllRepositories_(
    struct soap *soap,
    // request parameters:
    _mtrep__GetAllRepositories*         mtrep__GetAllRepositories,
    // response parameters:
    _mtrep__GetAllRepositoriesResponse* mtrep__GetAllRepositoriesResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	GetAllRepositories_ document
//gsoap ns1  service method-encoding:	GetAllRepositories_ literal
//gsoap ns1  service method-action:	GetAllRepositories_ http://miktex.org/2006/06/repository/GetAllRepositories
int __ns1__GetAllRepositories_(
    _mtrep__GetAllRepositories*         mtrep__GetAllRepositories,	///< Request parameter
    _mtrep__GetAllRepositoriesResponse* mtrep__GetAllRepositoriesResponse	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __ns1__GetRepositories_                                                    *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__GetRepositories_" of service binding "RepositorySoap"

/**

Operation details:

Get a filtered list of known package repositories.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2006/06/repository/GetRepositories"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__GetRepositories_(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _mtrep__GetRepositories*            mtrep__GetRepositories,
    // response parameters:
    _mtrep__GetRepositoriesResponse*    mtrep__GetRepositoriesResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__GetRepositories_(
    struct soap *soap,
    // request parameters:
    _mtrep__GetRepositories*            mtrep__GetRepositories,
    // response parameters:
    _mtrep__GetRepositoriesResponse*    mtrep__GetRepositoriesResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	GetRepositories_ document
//gsoap ns1  service method-encoding:	GetRepositories_ literal
//gsoap ns1  service method-action:	GetRepositories_ http://miktex.org/2006/06/repository/GetRepositories
int __ns1__GetRepositories_(
    _mtrep__GetRepositories*            mtrep__GetRepositories,	///< Request parameter
    _mtrep__GetRepositoriesResponse*    mtrep__GetRepositoriesResponse	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __ns1__GetListCreationTime_                                                *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__GetListCreationTime_" of service binding "RepositorySoap"

/**

Operation details:

Get the creation date and time of the repository list.
  - SOAP document/literal style
  - SOAP action="http://miktex.org/2007/03/repository/GetListCreationTime"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__GetListCreationTime_(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    _ns1__GetListCreationTime*          ns1__GetListCreationTime,
    // response parameters:
    _ns1__GetListCreationTimeResponse*  ns1__GetListCreationTimeResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__GetListCreationTime_(
    struct soap *soap,
    // request parameters:
    _ns1__GetListCreationTime*          ns1__GetListCreationTime,
    // response parameters:
    _ns1__GetListCreationTimeResponse*  ns1__GetListCreationTimeResponse
  );
@endcode

C++ proxy class (defined in soapRepositorySoapProxy.h):
  class RepositorySoap;

*/

//gsoap ns1  service method-style:	GetListCreationTime_ document
//gsoap ns1  service method-encoding:	GetListCreationTime_ literal
//gsoap ns1  service method-action:	GetListCreationTime_ http://miktex.org/2007/03/repository/GetListCreationTime
int __ns1__GetListCreationTime_(
    _ns1__GetListCreationTime*          ns1__GetListCreationTime,	///< Request parameter
    _ns1__GetListCreationTimeResponse*  ns1__GetListCreationTimeResponse	///< Response parameter
);

/* End of C:/work3/miktex-2.6/Libraries/MiKTeX/WebServiceClients/Repository/Repository.h */
