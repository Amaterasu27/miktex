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

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DBUILDING_LIBCURL)

if(NATIVE_WINDOWS)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif(NATIVE_WINDOWS)

set(${curl_dll_name}_sources
  arpa_telnet.h
  base64.c
  base64.h
  ca-bundle.h
  connect.c
  connect.h
  content_encoding.c
  content_encoding.h
  cookie.c
  cookie.h
  curl-version.h
  dict.c
  dict.h
  easy.c
  easyif.h
  escape.c
  escape.h
  file.c
  file.h
  formdata.c
  formdata.h
  ftp.c
  ftp.h
  getenv.c
  getinfo.c
  getinfo.h
  gtls.h
  hash.c
  hash.h
  hostares.c
  hostasyn.c
  hostip.c
  hostip.h
  hostip4.c
  hostip6.c
  hostsyn.c
  hostthre.c
  http.c
  http.h
  http_chunks.c
  http_chunks.h
  http_digest.c
  http_digest.h
  http_negotiate.c
  http_negotiate.h
  http_ntlm.c
  http_ntlm.h
  if2ip.c
  if2ip.h
  inet_ntoa_r.h
  inet_ntop.c
  inet_ntop.h
  inet_pton.c
  inet_pton.h
  krb4.h
  ldap.c
  ldap.h
  llist.c
  llist.h
  md5.c
  md5.h
  memdebug.c
  memdebug.h
  memory.h
  mprintf.c
  multi.c
  multiif.h
  netrc.c
  netrc.h
  parsedate.c
  parsedate.h
  progress.c
  progress.h
  select.c
  select.h
  sendf.c
  sendf.h
  setup.h
  share.c
  share.h
  sockaddr.h
  speedcheck.c
  speedcheck.h
  splay.c
  splay.h
  sslgen.c
  sslgen.h
  ssluse.c
  ssluse.h
  strequal.c
  strequal.h
  strerror.c
  strerror.h
  strtok.c
  strtok.h
  strtoofft.c
  strtoofft.h
  telnet.c
  telnet.h
  tftp.c
  tftp.h
  timeval.c
  timeval.h
  transfer.c
  transfer.h
  url.c
  url.h
  urldata.h
  version.c
)

if(NATIVE_WINDOWS)
  set(${curl_dll_name}_sources ${${curl_dll_name}_sources}
    config-win32.h
    curl.rc
  )
endif(NATIVE_WINDOWS)

add_library(${curl_dll_name} SHARED ${${curl_dll_name}_sources})

if(NATIVE_WINDOWS)
  target_link_libraries(${curl_dll_name} winmm wsock32)
endif(NATIVE_WINDOWS)

install(TARGETS ${curl_dll_name} DESTINATION "${bindir}")
