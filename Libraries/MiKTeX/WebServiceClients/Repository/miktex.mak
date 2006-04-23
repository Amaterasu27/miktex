## miktex.mak:
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

!include <miktex.inc>

wsdl_url = http://api.miktex.org/Repository.asmx?WSDL
#wsdl_url = http://localhost:1046/api.miktex.org/Repository.asmx?WSDL

all: common-all repositoryRepositorySoapProxy.cpp

Repository.h:
	$(wsdl2h) -o $@ $(wsdl_url)

repositoryRepositorySoapProxy.cpp: Repository.h
	$(soapcpp2) \
		-C -p repository -i -I $(gsoapdir)\import -w -x Repository.h

install: common-install

check: common-check

mostlyclean: common-mostlyclean

clean: common-clean

distclean: common-distclean

maintainer-clean: common-maintainer-clean
	-del /f Repository.h
	-del /f RepositorySoap.nsmap
	-del /f repositoryC.cpp
	-del /f repositoryH.h
	-del /f repositoryRepositorySoapProxy.cpp
	-del /f repositoryRepositorySoapProxy.h
	-del /f repositoryStub.h

depend:

!include <common-dependencies.inc>
