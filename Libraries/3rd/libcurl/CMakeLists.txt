## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2012 Christian Schenk
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
  asyn-ares.c
  asyn-thread.c
  base64.c
  connect.c
  content_encoding.c
  cookie.c
  curl_addrinfo.c
  curl_fnmatch.c
  curl_gethostname.c
  curl_memrchr.c
  curl_ntlm.c
  curl_ntlm_core.c
  curl_ntlm_msgs.c
  curl_ntlm_wb.c
  curl_rand.c
  curl_rtmp.c
  curl_sspi.c
  curl_threads.c
  dict.c
  easy.c
  escape.c
  file.c
  fileinfo.c
  formdata.c
  ftp.c
  ftplistparser.c
  getenv.c
  getinfo.c
  gopher.c
  gtls.c
  hash.c
  hmac.c
  hostasyn.c
  hostip.c
  hostip4.c
  hostip6.c
  hostsyn.c
  http.c
  http_chunks.c
  http_digest.c
  http_negotiate.c
  http_negotiate_sspi.c
  http_proxy.c
  if2ip.c
  imap.c
  inet_ntop.c
  inet_pton.c
  ldap.c
  llist.c
  md4.c
  md5.c
  memdebug.c
  mprintf.c
  multi.c
  netrc.c
  nonblock.c
  openldap.c
  parsedate.c
  pingpong.c
  polarssl.c
  pop3.c
  progress.c
  rawstr.c
  rtsp.c
  select.c
  sendf.c
  share.c
  slist.c
  smtp.c
  socks.c
  socks_gssapi.c
  socks_sspi.c
  speedcheck.c
  splay.c
  ssh.c
  sslgen.c
  ssluse.c
  strequal.c
  strerror.c
  strtok.c
  strtoofft.c
  telnet.c
  tftp.c
  timeval.c
  transfer.c
  url.c
  version.c
  warnless.c
  wildcard.c

  arpa_telnet.h
  asyn.h
  axtls.h
  config-win32.h
  connect.h
  content_encoding.h
  cookie.h
  curl-version.h
  curl_addrinfo.h
  curl_base64.h
  curl_fnmatch.h
  curl_gethostname.h
  curl_gssapi.h
  curl_hmac.h
  curl_ldap.h
  curl_md4.h
  curl_md5.h
  curl_memory.h
  curl_memrchr.h
  curl_ntlm.h
  curl_ntlm_core.h
  curl_ntlm_msgs.h
  curl_ntlm_wb.h
  curl_rand.h
  curl_rtmp.h
  curl_sspi.h
  curl_threads.h
  curlx.h
  cyassl.h
  dict.h
  easyif.h
  escape.h
  file.h
  fileinfo.h
  formdata.h
  ftp.h
  ftplistparser.h
  getinfo.h
  gopher.h
  gtls.h
  hash.h
  hostip.h
  http.h
  http_chunks.h
  http_digest.h
  http_negotiate.h
  http_proxy.h
  if2ip.h
  imap.h
  inet_ntop.h
  inet_pton.h
  krb4.h
  llist.h
  memdebug.h
  multiif.h
  netrc.h
  non-ascii.h
  nonblock.h
  nssg.h
  parsedate.h
  pingpong.h
  polarssl.h
  pop3.h
  progress.h
  qssl.h
  rawstr.h
  rtsp.h
  select.h
  sendf.h
  setup.h
  setup_once.h
  share.h
  slist.h
  smtp.h
  sockaddr.h
  socks.h
  speedcheck.h
  splay.h
  ssh.h
  sslgen.h
  ssluse.h
  strdup.h
  strequal.h
  strerror.h
  strtok.h
  strtoofft.h
  telnet.h
  tftp.h
  timeval.h
  transfer.h
  url.h
  urldata.h
  warnless.h
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
