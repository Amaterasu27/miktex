## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2006-2015 Christian Schenk
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

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/${rel_curl_dir})

add_definitions(-DBUILDING_LIBCURL)

if(NATIVE_WINDOWS)
  add_definitions(-D_WIN32_WINNT=0x0501)
  remove_definitions(-DHAVE_CONFIG_H)
endif()

use_static_crt()
use_static_libraries()

set(${curl_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/asyn-ares.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/asyn-thread.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/base64.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/connect.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/content_encoding.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/cookie.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_addrinfo.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_fnmatch.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_gethostname.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_memrchr.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_ntlm.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_ntlm_core.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_ntlm_msgs.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_ntlm_wb.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_rand.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_rtmp.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_sspi.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_threads.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/dict.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/easy.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/escape.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/file.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/fileinfo.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/formdata.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/ftp.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/ftplistparser.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/getenv.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/getinfo.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/gopher.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/gtls.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/hash.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/hmac.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/hostasyn.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/hostip.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/hostip4.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/hostip6.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/hostsyn.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http_chunks.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http_digest.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http_negotiate.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http_negotiate_sspi.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http_proxy.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/if2ip.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/imap.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/inet_ntop.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/inet_pton.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/ldap.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/llist.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/md4.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/md5.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/memdebug.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/mprintf.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/multi.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/netrc.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/nonblock.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/openldap.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/parsedate.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/pingpong.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/polarssl.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/pop3.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/progress.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/rawstr.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/rtsp.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/select.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/sendf.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/share.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/slist.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/smtp.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/socks.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/socks_gssapi.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/socks_sspi.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/speedcheck.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/splay.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/ssh.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/sslgen.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/ssluse.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/strequal.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/strerror.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/strtok.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/strtoofft.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/telnet.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/tftp.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/timeval.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/transfer.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/url.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/version.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/warnless.c
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/wildcard.c

  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/arpa_telnet.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/asyn.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/axtls.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/config-win32.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/connect.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/content_encoding.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/cookie.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl-version.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_addrinfo.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_base64.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_fnmatch.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_gethostname.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_gssapi.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_hmac.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_ldap.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_md4.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_md5.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_memory.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_memrchr.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_ntlm.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_ntlm_core.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_ntlm_msgs.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_ntlm_wb.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_rand.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_rtmp.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_sspi.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curl_threads.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/curlx.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/cyassl.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/dict.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/easyif.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/escape.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/file.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/fileinfo.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/formdata.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/ftp.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/ftplistparser.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/getinfo.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/gopher.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/gtls.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/hash.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/hostip.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http_chunks.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http_digest.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http_negotiate.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/http_proxy.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/if2ip.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/imap.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/inet_ntop.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/inet_pton.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/krb4.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/llist.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/memdebug.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/multiif.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/netrc.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/non-ascii.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/nonblock.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/nssg.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/parsedate.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/pingpong.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/polarssl.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/pop3.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/progress.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/qssl.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/rawstr.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/rtsp.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/select.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/sendf.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/setup.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/setup_once.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/share.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/slist.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/smtp.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/sockaddr.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/socks.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/speedcheck.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/splay.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/ssh.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/sslgen.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/ssluse.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/strdup.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/strequal.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/strerror.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/strtok.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/strtoofft.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/telnet.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/tftp.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/timeval.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/transfer.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/url.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/urldata.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/warnless.h
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/wildcard.h
)

if(NATIVE_WINDOWS)
  set(${curl_lib_name}_sources ${${curl_lib_name}_sources}
    ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/config-win32.h
  )
endif()

add_library(${curl_lib_name} STATIC ${${curl_lib_name}_sources})

target_compile_definitions(${curl_lib_name}
  PUBLIC
    -DCURL_STATICLIB
)

target_include_directories(${curl_lib_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/include
)
