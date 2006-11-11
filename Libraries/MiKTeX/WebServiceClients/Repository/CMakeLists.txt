## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006 Christian Schenk
## 
## This file is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation; either version 2, or (at your
## option) any later version.
## 
## This file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this file; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

if(LOCALWEBSERVICE)
  set(wsdl_url http://localhost:1046/api.miktex.org/Repository.asmx?WSDL)
else(LOCALWEBSERVICE)
  set(wsdl_url http://api.miktex.org/Repository.asmx?WSDL)
endif(LOCALWEBSERVICE)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_SOURCE_DIR}/Repository.h
  COMMAND
	wsdl2h -o ${CMAKE_CURRENT_SOURCE_DIR}/Repository.h ${wsdl_url}
  DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/typemap.dat
  WORKING_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_SOURCE_DIR}/RepositorySoap.nsmap
	${CMAKE_CURRENT_SOURCE_DIR}/repositoryC.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/repositoryH.h
	${CMAKE_CURRENT_SOURCE_DIR}/repositoryRepositorySoapProxy.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/repositoryRepositorySoapProxy.h
	${CMAKE_CURRENT_SOURCE_DIR}/repositoryStub.h
  COMMAND
	soapcpp2 -C -p repository -i
		-I ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}/import
		-w -x ${CMAKE_CURRENT_SOURCE_DIR}/Repository.h
  DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/Repository.h
  WORKING_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}
  VERBATIM
)

add_custom_target(proxy
  DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/repositoryRepositorySoapProxy.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Repository.h
)
