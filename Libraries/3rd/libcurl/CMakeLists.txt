## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2011 Christian Schenk
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
  add_definitions(-DENABLE_IPV6)
  remove_definitions(-DHAVE_CONFIG_H)
endif(NATIVE_WINDOWS)

set(${curl_dll_name}_sources
  arpa_telnet.h
  base64.c
  config-win32.h
  connect.c
  connect.h
  content_encoding.c
  content_encoding.h
  cookie.c
  cookie.h
  curl-version.h
  curl_addrinfo.c
  curl_addrinfo.h
  curl_base64.h
  curl_fnmatch.c
  curl_fnmatch.h
  curl_hmac.h
  curl_ldap.h
  curl_md5.h
  curl_memory.h
  curl_memrchr.c
  curl_memrchr.h
  curl_rand.c
  curl_rand.h
  curl_rtmp.c
  curl_rtmp.h
  curl_sspi.c
  curl_sspi.h
  curl_threads.c
  curl_threads.h
  curlx.h
  dict.c
  dict.h
  easy.c
  easyif.h
  escape.c
  escape.h
  file.c
  file.h
  fileinfo.c
  fileinfo.h
  formdata.c
  formdata.h
  ftp.c
  ftp.h
  ftplistparser.c
  ftplistparser.h
  getenv.c
  getinfo.c
  getinfo.h
  gtls.c
  gtls.h
  hash.c
  hash.h
  hmac.c
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
  imap.c
  imap.h
  inet_ntop.c
  inet_ntop.h
  inet_pton.c
  inet_pton.h
  krb4.h
  ldap.c
  llist.c
  llist.h
  md5.c
  memdebug.c
  memdebug.h
  mprintf.c
  multi.c
  multiif.h
  netrc.c
  netrc.h
  nonblock.c
  nonblock.h
  nssg.h
  openldap.c
  parsedate.c
  parsedate.h
  pingpong.c
  pingpong.h
  polarssl.c
  polarssl.h
  pop3.c
  pop3.h
  progress.c
  progress.h
  qssl.h
  rawstr.c
  rawstr.h
  rtsp.c
  rtsp.h
  select.c
  select.h
  sendf.c
  sendf.h
  setup.h
  setup_once.h
  share.c
  share.h
  slist.c
  slist.h
  smtp.c
  smtp.h
  sockaddr.h
  socks.c
  socks.h
  socks_gssapi.c
  socks_sspi.c
  speedcheck.c
  speedcheck.h
  splay.c
  splay.h
  ssh.c
  ssh.h
  sslgen.c
  sslgen.h
  ssluse.c
  ssluse.h
  strdup.h
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
  warnless.c
  warnless.h
  wildcard.c
  wildcard.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/curl.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/curl.rc
  )
  set(${curl_dll_name}_sources ${${curl_dll_name}_sources}
    config-win32.h
    ${CMAKE_CURRENT_BINARY_DIR}/curl.rc
  )
endif(NATIVE_WINDOWS)

add_library(${curl_dll_name} SHARED ${${curl_dll_name}_sources})

set_target_properties(${curl_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

if(NATIVE_WINDOWS)
  target_link_libraries(${curl_dll_name} winmm wsock32 ws2_32 Wldap32)
endif(NATIVE_WINDOWS)

rebase(${curl_dll_name})

install(TARGETS ${curl_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
