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

add_definitions(-DBUILDING_LIBCURL -DCURL_STATICLIB)

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir})

use_static_crt()

set(${curl_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/arpa_telnet.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/base64.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/base64.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/ca-bundle.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/connect.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/connect.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/content_encoding.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/content_encoding.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/cookie.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/cookie.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/curl-version.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/dict.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/dict.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/easy.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/easyif.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/escape.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/escape.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/file.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/file.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/formdata.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/formdata.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/ftp.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/ftp.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/getenv.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/getinfo.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/getinfo.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/gtls.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hash.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hash.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hostares.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hostasyn.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hostip.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hostip.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hostip4.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hostip6.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hostsyn.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/hostthre.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http_chunks.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http_chunks.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http_digest.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http_digest.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http_negotiate.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http_negotiate.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http_ntlm.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/http_ntlm.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/if2ip.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/if2ip.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/inet_ntoa_r.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/inet_ntop.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/inet_ntop.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/inet_pton.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/inet_pton.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/krb4.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/ldap.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/ldap.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/llist.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/llist.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/md5.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/md5.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/memdebug.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/memdebug.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/memory.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/mprintf.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/multi.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/multiif.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/netrc.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/netrc.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/parsedate.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/parsedate.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/progress.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/progress.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/select.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/select.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/sendf.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/sendf.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/setup.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/share.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/share.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/sockaddr.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/speedcheck.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/speedcheck.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/splay.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/splay.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/sslgen.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/sslgen.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/ssluse.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/ssluse.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/strequal.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/strequal.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/strerror.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/strerror.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/strtok.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/strtok.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/strtoofft.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/strtoofft.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/telnet.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/telnet.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/tftp.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/tftp.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/timeval.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/timeval.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/transfer.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/transfer.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/url.c
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/url.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/urldata.h
  ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/version.c
)

if(WIN32)
  set(${curl_lib_name}_sources ${${curl_lib_name}_sources}
    ${CMAKE_SOURCE_DIR}/${rel_libcurl_dir}/config-win32.h
  )
endif(WIN32)

add_library(${curl_lib_name} STATIC ${${curl_lib_name}_sources})
