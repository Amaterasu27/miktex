## miktex.mak: libmpm-static
## 
## Copyright (C) 2001-2006 Christian Schenk
## 
## This file is part of MiKTeX Package Manager.
## 
## MiKTeX Package Manager is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## MiKTeX Package Manager is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with MiKTeX Package Manager; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

miktex_cc_use_crt_dll = 0
miktex_cc_warnings_as_errors = 1
no_miktex_dlls = 1

!include <miktex.inc>

srcdir = $(mpmlibdir)

objects = \
	$(outdir)\StdAfx.obj \
	\
	$(outdir)\CabExtractor.obj \
	$(outdir)\CurlWebFile.obj \
	$(outdir)\CurlWebSession.obj \
	$(outdir)\Extractor.obj \
	$(outdir)\PackageInstaller.obj \
	$(outdir)\PackageIterator.obj \
	$(outdir)\TarBzip2Extractor.obj \
	$(outdir)\TarLzmaExtractor.obj \
	$(outdir)\TpmParser.obj \
	$(outdir)\WebSession.obj \
	$(outdir)\mpm.obj \
	$(outdir)\repositoryC.obj \
	$(outdir)\repositoryRepositorySoapProxy.obj \
	$(outdir)\stdsoap2.obj \
	$(outdir)\winCabExtractor.obj \
	$(outdir)\winWebFile.obj \
	$(outdir)\winWebSession.obj \

sources = \
	$(gsoapdir)\stdsoap2.cpp \
	$(wsclientdir)\Repository\repositoryC.cpp \
	$(wsclientdir)\Repository\repositoryRepositorySoapProxy.cpp \
	$(mpmlibdir)\CabExtractor.cpp \
	$(mpmlibdir)\CurlWebFile.cpp \
	$(mpmlibdir)\CurlWebSession.cpp \
	$(mpmlibdir)\Extractor.cpp \
	$(mpmlibdir)\PackageInstaller.cpp \
	$(mpmlibdir)\PackageIterator.cpp \
	$(mpmlibdir)\StdAfx.cpp \
	$(mpmlibdir)\TarBzip2Extractor.cpp \
	$(mpmlibdir)\TarLzmaExtractor.cpp \
	$(mpmlibdir)\TpmParser.cpp \
	$(mpmlibdir)\WebSession.cpp \
	$(mpmlibdir)\mpm.cpp \
	$(mpmlibdir)\win\winCabExtractor.cpp \
	$(mpmlibdir)\win\winWebFile.cpp \
	$(mpmlibdir)\win\winWebSession.cpp \

extra_includes = \
	$(ccinc_gsoap) \
	$(ccinc_libcurl) \
	$(ccinc_libexpat) \
	$(ccinc_libmspack) \
	$(ccinc_7zip) \
	$(ccinc_liblzma) \

extra_defines = \
	-DHAVE_LIBCURL=1 \
	-DUSE_WEB_SERVICE=1 \

compile = \
	$(cc) \
		$(cppstandard) \
		$(extra_defines) \
		$(extra_includes) \
		$(ccopt_enable_rtti_support) \
		$(ccopt_name_precompiled_header_file)$(outdir)\mpm.pch \
		$(ccopt_output_directory)$(outdir)\ \
		$(ccopt_use_precompiled_header_file)StdAfx.h \
		-DCURL_STATICLIB \
		-DXML_STATIC \

soapcompile = \
	$(cc) $(cppstandard) \
		$(extra_defines) \
		$(extra_includes) \
		$(ccopt_enable_rtti_support) \
		$(ccopt_disable_warnings) \
		$(ccopt_output_directory)$(outdir)\ \
		-DCURL_STATICLIB \
		-DXML_STATIC \

{$(mpmlibdir)\}.cpp{$(outdir)\}.obj:
	 $(compile) $<

{$(mpmlibdir)\win\}.cpp{$(outdir)\}.obj:
	$(compile) $<

{$(wsclientdir)\Repository\}.cpp{$(outdir)\}.obj:
	 $(soapcompile) $<

{$(gsoapdir)\}.cpp{$(outdir)\}.obj:
	 $(soapcompile) $<

all: common-all $(mpm_static_lib)

install: common-install all

check: common-check

# _____________________________________________________________________________
#
# mpm.lib
# _____________________________________________________________________________

$(mpm_static_lib): \
			$(outdir) \
			$(objects) \

	$(libtool) $(libflags) $(libopt_output_file)$@ $(objects)

# _____________________________________________________________________________
#
# precompiled header
# _____________________________________________________________________________

$(outdir)\StdAfx.obj: \
			$(outdir) \
			$(mpmlibdir)\StdAfx.cpp \

	$(compile) \
		$(ccopt_create_precompiled_header_file)StdAfx.h \
		$(mpmlibdir)\StdAfx.cpp

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean

distclean: common-distclean

maintainer-clean: common-maintainer-clean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: $(sources)
	$(MAKEDEPEND) $(extra_defines) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\stdsoap2.obj: $(miktexsrcdir)/Libraries/3rd/gsoap/stdsoap2.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/CabExtractor.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/CurlWebFile.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/CurlWebSession.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/CurlWebSession.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/CurlWebFile.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/CabExtractor.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/TarBzip2Extractor.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/TarLzmaExtractor.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/win/winCabExtractor.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/TpmParser.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/TarBzip2Extractor.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\TarLzmaExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/TarLzmaExtractor.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/TpmParser.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/win/winWebSession.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/CurlWebSession.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/TpmParser.h
$(outdir)\mpm.obj: ../WebServiceClients/Repository/repositoryRepositorySoapProxy.h
$(outdir)\mpm.obj: ../WebServiceClients/Repository/repositoryH.h
$(outdir)\mpm.obj: ../WebServiceClients/Repository/repositoryStub.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/gsoap/stdsoap2.h
$(outdir)\mpm.obj: ../WebServiceClients/Repository/RepositorySoap.nsmap
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/win/winCabExtractor.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/win/winWebFile.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/win/winWebSession.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/StdAfx.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curlver.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/easy.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/multi.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libcurl/include/curl/curl.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/internal.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/mpm-version.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/Extractor.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebSession.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/WebFile.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/win/winWebFile.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/win/winWebSession.h
