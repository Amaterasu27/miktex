## miktex.mak: libcurl
##
## Copyright (C) 2005-2006 Christian Schenk
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

objects = \
	$(outdir)\base64.obj \
	$(outdir)\connect.obj \
	$(outdir)\content_encoding.obj \
	$(outdir)\cookie.obj \
	$(outdir)\dict.obj \
	$(outdir)\easy.obj \
	$(outdir)\escape.obj \
	$(outdir)\file.obj \
	$(outdir)\formdata.obj \
	$(outdir)\ftp.obj \
	$(outdir)\getenv.obj \
	$(outdir)\getinfo.obj \
	$(outdir)\hash.obj \
	$(outdir)\hostares.obj \
	$(outdir)\hostasyn.obj \
	$(outdir)\hostip.obj \
	$(outdir)\hostip4.obj \
	$(outdir)\hostip6.obj \
	$(outdir)\hostsyn.obj \
	$(outdir)\hostthre.obj \
	$(outdir)\http.obj \
	$(outdir)\http_chunks.obj \
	$(outdir)\http_digest.obj \
	$(outdir)\if2ip.obj \
	$(outdir)\inet_ntop.obj \
	$(outdir)\inet_pton.obj \
	$(outdir)\ldap.obj \
	$(outdir)\llist.obj \
	$(outdir)\md5.obj \
	$(outdir)\memdebug.obj \
	$(outdir)\mprintf.obj \
	$(outdir)\multi.obj \
	$(outdir)\netrc.obj \
	$(outdir)\parsedate.obj \
	$(outdir)\progress.obj \
	$(outdir)\select.obj \
	$(outdir)\sendf.obj \
	$(outdir)\share.obj \
	$(outdir)\speedcheck.obj \
	$(outdir)\sslgen.obj \
	$(outdir)\ssluse.obj \
	$(outdir)\strequal.obj \
	$(outdir)\strerror.obj \
	$(outdir)\strtok.obj \
	$(outdir)\strtoofft.obj \
	$(outdir)\telnet.obj \
	$(outdir)\tftp.obj \
	$(outdir)\timeval.obj \
	$(outdir)\transfer.obj \
	$(outdir)\url.obj \
	$(outdir)\version.obj \
        $(outdir)\http_negotiate.obj \
        $(outdir)\http_ntlm.obj \

sources = \
	base64.c \
	connect.c \
	content_encoding.c \
	cookie.c \
	dict.c \
	easy.c \
	escape.c \
	file.c \
	formdata.c \
	ftp.c \
	getenv.c \
	getinfo.c \
	hash.c \
	hostares.c \
	hostasyn.c \
	hostip.c \
	hostip4.c \
	hostip6.c \
	hostsyn.c \
	hostthre.c \
	http.c \
	http_chunks.c \
	http_digest.c \
	if2ip.c \
	inet_ntop.c \
	inet_pton.c \
	ldap.c \
	llist.c \
	md5.c \
	memdebug.c \
	mprintf.c \
	multi.c \
	netrc.c \
	parsedate.c \
	progress.c \
	select.c \
	sendf.c \
	share.c \
	speedcheck.c \
	sslgen.c \
	ssluse.c \
	strequal.c \
	strerror.c \
	strtok.c \
	strtoofft.c \
	telnet.c \
	tftp.c \
	timeval.c \
	transfer.c \
	url.c \
	version.c \
        http_negotiate.c \
        http_ntlm.c \

binaries = $(outdir)\$(curl_dll_name).dll

extra_defines = \
	/DBUILDING_LIBCURL \

extra_includes = \
	$(ccinc_libcurl) \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(extra_defines) \
		$(extra_includes) \
		$(ccopt_output_directory)$(outdir)\ $<

all: common-all

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# curl.dll
# _____________________________________________________________________________

$(outdir)\$(curl_dll_name).dll $(curl_lib): \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\curl.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(curl_dll_name) \
		-implib:$(curl_lib) \
		-out:$(outdir)\$(curl_dll_name).dll \
		$(objects) \
		$(libs) \
		$(outdir)\curl.res \
		winmm.lib \
		wsock32.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\curl.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		curl-version.h \
		curl.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ curl.rc

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) $(extra_defines) $(extra_includes) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\base64.obj: setup.h config-win32.h
$(outdir)\base64.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\base64.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\base64.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\base64.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\base64.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\base64.obj: base64.h memory.h
$(outdir)\base64.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\base64.obj: memdebug.h
$(outdir)\connect.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\connect.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\connect.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\connect.obj: sendf.h if2ip.h strerror.h connect.h memory.h select.h
$(outdir)\connect.obj: url.h memdebug.h
$(outdir)\content_encoding.obj: setup.h config-win32.h
$(outdir)\cookie.obj: setup.h config-win32.h
$(outdir)\cookie.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\cookie.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\cookie.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\cookie.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\cookie.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\cookie.obj: urldata.h cookie.h
$(outdir)\cookie.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\cookie.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\cookie.obj: strequal.h strtok.h sendf.h memory.h share.h strtoofft.h
$(outdir)\dict.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\dict.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\dict.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\dict.obj: transfer.h sendf.h progress.h strequal.h dict.h
$(outdir)\dict.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\dict.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\dict.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\dict.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\dict.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\easy.obj: setup.h config-win32.h strequal.h
$(outdir)\easy.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\easy.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h
$(outdir)\easy.obj: hostip.h hash.h llist.h transfer.h sslgen.h url.h getinfo.h
$(outdir)\easy.obj: share.h memory.h progress.h easyif.h
$(outdir)\easy.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\easy.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\easy.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\easy.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\easy.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\easy.obj: memdebug.h
$(outdir)\escape.obj: setup.h config-win32.h
$(outdir)\escape.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\escape.obj: memory.h
$(outdir)\escape.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\escape.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\escape.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\escape.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\escape.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\escape.obj: memdebug.h
$(outdir)\file.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\file.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\file.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\file.obj: progress.h sendf.h escape.h file.h speedcheck.h getinfo.h
$(outdir)\file.obj: transfer.h url.h memory.h parsedate.h
$(outdir)\file.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\file.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\file.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\file.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\file.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\file.obj: memdebug.h
$(outdir)\formdata.obj: setup.h config-win32.h
$(outdir)\formdata.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\formdata.obj: formdata.h strequal.h memory.h
$(outdir)\formdata.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\formdata.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\formdata.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\formdata.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\formdata.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\formdata.obj: memdebug.h
$(outdir)\ftp.obj: setup.h config-win32.h
$(outdir)\ftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\ftp.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h hostip.h
$(outdir)\ftp.obj: hash.h llist.h sendf.h if2ip.h progress.h transfer.h escape.h
$(outdir)\ftp.obj: http.h ftp.h strtoofft.h strequal.h sslgen.h connect.h
$(outdir)\ftp.obj: strerror.h memory.h inet_ntop.h select.h parsedate.h
$(outdir)\ftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\ftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\ftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\ftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\ftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\getenv.obj: setup.h config-win32.h
$(outdir)\getenv.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\getenv.obj: memory.h memdebug.h
$(outdir)\getinfo.obj: setup.h config-win32.h
$(outdir)\getinfo.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\getinfo.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h
$(outdir)\getinfo.obj: hostip.h hash.h llist.h getinfo.h memory.h sslgen.h
$(outdir)\getinfo.obj: memdebug.h
$(outdir)\hash.obj: setup.h config-win32.h hash.h llist.h memory.h
$(outdir)\hash.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hash.obj: memdebug.h
$(outdir)\hostares.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\hostares.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostares.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h
$(outdir)\hostares.obj: llist.h sendf.h share.h strerror.h url.h
$(outdir)\hostares.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\hostares.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostares.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\hostares.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\hostares.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\hostares.obj: memory.h memdebug.h
$(outdir)\hostasyn.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\hostasyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostasyn.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h
$(outdir)\hostasyn.obj: llist.h sendf.h share.h strerror.h url.h
$(outdir)\hostasyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\hostasyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostasyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\hostasyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\hostasyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\hostasyn.obj: memory.h memdebug.h
$(outdir)\hostip.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\hostip.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostip.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\hostip.obj: sendf.h share.h strerror.h url.h inet_ntop.h
$(outdir)\hostip.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\hostip.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostip.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\hostip.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\hostip.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\hostip.obj: memory.h memdebug.h
$(outdir)\hostip4.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\hostip4.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostip4.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\hostip4.obj: sendf.h share.h strerror.h url.h inet_pton.h
$(outdir)\hostip4.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\hostip4.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostip4.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\hostip4.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\hostip4.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\hostip4.obj: memory.h memdebug.h
$(outdir)\hostip6.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\hostip6.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostip6.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\hostip6.obj: sendf.h share.h strerror.h url.h inet_pton.h
$(outdir)\hostip6.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\hostip6.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostip6.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\hostip6.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\hostip6.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\hostip6.obj: memory.h memdebug.h
$(outdir)\hostsyn.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\hostsyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostsyn.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\hostsyn.obj: sendf.h share.h strerror.h url.h
$(outdir)\hostsyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\hostsyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostsyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\hostsyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\hostsyn.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\hostsyn.obj: memory.h memdebug.h
$(outdir)\hostthre.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\hostthre.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostthre.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h
$(outdir)\hostthre.obj: llist.h sendf.h share.h strerror.h url.h
$(outdir)\hostthre.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\hostthre.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\hostthre.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\hostthre.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\hostthre.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\hostthre.obj: inet_ntop.h memory.h memdebug.h
$(outdir)\http.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\http.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\http.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\http.obj: transfer.h sendf.h progress.h base64.h strequal.h sslgen.h
$(outdir)\http.obj: http_digest.h http_ntlm.h http_negotiate.h url.h share.h
$(outdir)\http.obj: http.h memory.h select.h parsedate.h strtoofft.h
$(outdir)\http.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\http.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\http.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\http.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\http.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\http.obj: memdebug.h
$(outdir)\http_chunks.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\http_chunks.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\http_chunks.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h
$(outdir)\http_chunks.obj: llist.h sendf.h content_encoding.h http.h memory.h
$(outdir)\http_chunks.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\http_chunks.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\http_chunks.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\http_chunks.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\http_chunks.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\http_chunks.obj: memdebug.h
$(outdir)\http_digest.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\http_digest.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\http_digest.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h
$(outdir)\http_digest.obj: llist.h sendf.h strequal.h base64.h md5.h
$(outdir)\http_digest.obj: http_digest.h strtok.h url.h memory.h
$(outdir)\http_digest.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\http_digest.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\http_digest.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\http_digest.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\http_digest.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\http_digest.obj: memdebug.h
$(outdir)\if2ip.obj: setup.h config-win32.h if2ip.h
$(outdir)\inet_ntop.obj: setup.h config-win32.h
$(outdir)\inet_ntop.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\inet_ntop.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\inet_ntop.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\inet_ntop.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\inet_ntop.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\inet_ntop.obj: inet_ntop.h
$(outdir)\inet_pton.obj: setup.h config-win32.h inet_pton.h
$(outdir)\ldap.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\ldap.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\ldap.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\ldap.obj: sendf.h escape.h transfer.h strequal.h strtok.h ldap.h
$(outdir)\ldap.obj: memory.h
$(outdir)\ldap.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\ldap.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\ldap.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\ldap.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\ldap.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\ldap.obj: memdebug.h
$(outdir)\llist.obj: setup.h config-win32.h llist.h memory.h
$(outdir)\llist.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\llist.obj: memdebug.h
$(outdir)\md5.obj: setup.h config-win32.h md5.h
$(outdir)\mprintf.obj: setup.h config-win32.h
$(outdir)\mprintf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\mprintf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\mprintf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\mprintf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\mprintf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\mprintf.obj: memory.h
$(outdir)\mprintf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\mprintf.obj: memdebug.h
$(outdir)\multi.obj: setup.h config-win32.h
$(outdir)\multi.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\multi.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h
$(outdir)\multi.obj: hostip.h hash.h llist.h transfer.h url.h connect.h
$(outdir)\multi.obj: progress.h memory.h easyif.h multiif.h sendf.h memdebug.h
$(outdir)\netrc.obj: setup.h config-win32.h
$(outdir)\netrc.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\netrc.obj: netrc.h strequal.h strtok.h memory.h
$(outdir)\netrc.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\netrc.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\netrc.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\netrc.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\netrc.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\netrc.obj: memdebug.h
$(outdir)\parsedate.obj: setup.h config-win32.h
$(outdir)\parsedate.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\progress.obj: setup.h config-win32.h
$(outdir)\progress.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\progress.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h
$(outdir)\progress.obj: hostip.h hash.h llist.h sendf.h progress.h
$(outdir)\progress.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\progress.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\progress.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\progress.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\progress.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\select.obj: setup.h config-win32.h
$(outdir)\select.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\select.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h
$(outdir)\select.obj: hostip.h hash.h llist.h connect.h select.h
$(outdir)\sendf.obj: setup.h config-win32.h
$(outdir)\sendf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\sendf.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h
$(outdir)\sendf.obj: hostip.h hash.h llist.h sendf.h connect.h sslgen.h
$(outdir)\sendf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\sendf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\sendf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\sendf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\sendf.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\sendf.obj: memory.h strerror.h memdebug.h
$(outdir)\share.obj: setup.h config-win32.h
$(outdir)\share.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\share.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h
$(outdir)\share.obj: hostip.h hash.h llist.h share.h memory.h memdebug.h
$(outdir)\speedcheck.obj: setup.h config-win32.h
$(outdir)\speedcheck.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\speedcheck.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h
$(outdir)\speedcheck.obj: hostip.h hash.h llist.h sendf.h speedcheck.h
$(outdir)\sslgen.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\sslgen.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\sslgen.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\sslgen.obj: sslgen.h ssluse.h gtls.h sendf.h strequal.h url.h memory.h
$(outdir)\sslgen.obj: memdebug.h
$(outdir)\ssluse.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\ssluse.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\ssluse.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\ssluse.obj: sendf.h url.h inet_pton.h ssluse.h connect.h strequal.h
$(outdir)\ssluse.obj: select.h sslgen.h
$(outdir)\ssluse.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\ssluse.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\ssluse.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\ssluse.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\ssluse.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\strequal.obj: setup.h config-win32.h strequal.h
$(outdir)\strequal.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\strerror.obj: setup.h config-win32.h
$(outdir)\strerror.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\strerror.obj: strerror.h urldata.h cookie.h formdata.h timeval.h
$(outdir)\strerror.obj: http_chunks.h hostip.h hash.h llist.h
$(outdir)\strerror.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\strerror.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\strerror.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\strerror.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\strerror.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\strtok.obj: setup.h config-win32.h strtok.h
$(outdir)\strtoofft.obj: setup.h config-win32.h strtoofft.h
$(outdir)\strtoofft.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\telnet.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\telnet.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\telnet.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\telnet.obj: transfer.h sendf.h telnet.h
$(outdir)\telnet.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\telnet.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\telnet.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\telnet.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\telnet.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\telnet.obj: arpa_telnet.h memory.h select.h memdebug.h
$(outdir)\tftp.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\tftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\tftp.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\tftp.obj: transfer.h sendf.h tftp.h progress.h connect.h strerror.h
$(outdir)\tftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\tftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\tftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\tftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\tftp.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\tftp.obj: memory.h select.h memdebug.h
$(outdir)\timeval.obj: timeval.h setup.h config-win32.h
$(outdir)\transfer.obj: setup.h config-win32.h strtoofft.h
$(outdir)\transfer.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\transfer.obj: strequal.h urldata.h cookie.h formdata.h timeval.h
$(outdir)\transfer.obj: http_chunks.h hostip.h hash.h llist.h netrc.h
$(outdir)\transfer.obj: content_encoding.h transfer.h sendf.h speedcheck.h
$(outdir)\transfer.obj: progress.h http.h url.h getinfo.h sslgen.h http_digest.h
$(outdir)\transfer.obj: http_ntlm.h http_negotiate.h share.h memory.h select.h
$(outdir)\transfer.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\transfer.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\transfer.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\transfer.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\transfer.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\transfer.obj: memdebug.h
$(outdir)\url.obj: setup.h config-win32.h urldata.h cookie.h
$(outdir)\url.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\url.obj: formdata.h timeval.h http_chunks.h hostip.h hash.h llist.h
$(outdir)\url.obj: netrc.h base64.h sslgen.h transfer.h sendf.h progress.h
$(outdir)\url.obj: strequal.h strerror.h escape.h strtok.h share.h
$(outdir)\url.obj: content_encoding.h http_digest.h http_negotiate.h select.h
$(outdir)\url.obj: multiif.h ftp.h dict.h telnet.h tftp.h http.h file.h ldap.h
$(outdir)\url.obj: url.h connect.h inet_ntop.h http_ntlm.h ./ca-bundle.h
$(outdir)\url.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\url.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\url.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\url.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\url.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\url.obj: memory.h memdebug.h
$(outdir)\version.obj: setup.h config-win32.h
$(outdir)\version.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\version.obj: urldata.h cookie.h formdata.h timeval.h http_chunks.h
$(outdir)\version.obj: hostip.h hash.h llist.h sslgen.h
$(outdir)\version.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/mprintf.h
$(outdir)\version.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\version.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\version.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\version.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\http_negotiate.obj: setup.h config-win32.h
$(outdir)\http_ntlm.obj: setup.h config-win32.h
