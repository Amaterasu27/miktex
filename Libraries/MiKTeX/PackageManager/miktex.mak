## miktex.mak: libmpm
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

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

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
	CabExtractor.cpp \
	CurlWebFile.cpp \
	CurlWebSession.cpp \
	Extractor.cpp \
	PackageInstaller.cpp \
	PackageIterator.cpp \
	StdAfx.cpp \
	TarBzip2Extractor.cpp \
	TpmParser.cpp \
	WebSession.cpp \
	mpm.cpp \
	win\winCabExtractor.cpp \
	win\winWebFile.cpp \
	win\winWebSession.cpp \

mpm_dll = $(mpm_dll_name).dll

binaries = $(outdir)\$(mpm_dll)

extra_includes = \
	$(ccinc_gsoap) \
	$(ccinc_libcurl) \
	$(ccinc_libexpat) \
	$(ccinc_libmspack) \

extra_defines = \
	-DHAVE_LIBCURL=1 \

compile = \
	$(cc) $(cppstandard) \
		$(extra_defines) \
		$(extra_includes) \
		$(ccopt_enable_rtti_support) \
		$(ccopt_name_precompiled_header_file)$(outdir)\mpm.pch \
		$(ccopt_output_directory)$(outdir)\ \
		$(ccopt_use_precompiled_header_file)StdAfx.h \

soapcompile = \
	$(cc) $(cppstandard) \
		$(extra_defines) \
		$(extra_includes) \
		$(ccopt_enable_rtti_support) \
		$(ccopt_disable_warnings) \
		$(ccopt_output_directory)$(outdir)\ \

.cpp{$(outdir)\}.obj:
	 $(compile) $<

{win\}.cpp{$(outdir)\}.obj:
	$(compile) $<

{$(wsclientdir)\Repository\}.cpp{$(outdir)\}.obj:
	 $(soapcompile) $<

{$(gsoapdir)\}.cpp{$(outdir)\}.obj:
	 $(soapcompile) $<

all: common-all $(mpm_lib)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# mpm.dll
# _____________________________________________________________________________

linkfiles = \
	$(bz2_lib) \
	$(curl_lib) \
	$(expat_lib) \
	$(miktex_lib) \
	$(mspack_lib) \
	$(objects) \
	$(outdir)\mpm.res \

$(outdir)\$(mpm_dll) $(mpm_lib): \
			$(outdir) \
			$(dllbases) \
			$(linkfiles) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(mpm_dll_name) \
		-delayload:$(expat_dll_name).dll \
		-delayload:$(miktex_dll_name).dll \
		-delayload:wininet.dll \
		-delayload:user32.dll \
		-implib:$(mpm_lib) \
		-out:$(outdir)\$(mpm_dll) \
		$(linkfiles) \
		delayimp.lib \
		fdi.lib \
		wininet.lib \
		Ws2_32.lib \
		user32.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\mpm.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mpm-version.h \
			mpm.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mpm.rc

# _____________________________________________________________________________
#
# precompiled header
# _____________________________________________________________________________

$(outdir)\StdAfx.obj: \
			$(outdir) \
			StdAfx.cpp \

	$(compile) $(ccopt_create_precompiled_header_file)StdAfx.h StdAfx.cpp

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
$(outdir)\CabExtractor.obj: StdAfx.h
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
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\CabExtractor.obj: internal.h
$(outdir)\CabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\CabExtractor.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\CabExtractor.obj: CabExtractor.h
$(outdir)\CurlWebFile.obj: StdAfx.h
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
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\CurlWebFile.obj: internal.h
$(outdir)\CurlWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\CurlWebFile.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\CurlWebFile.obj: CurlWebFile.h CurlWebSession.h
$(outdir)\CurlWebSession.obj: StdAfx.h
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
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\CurlWebSession.obj: internal.h
$(outdir)\CurlWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\CurlWebSession.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\CurlWebSession.obj: CurlWebSession.h CurlWebFile.h
$(outdir)\Extractor.obj: StdAfx.h
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
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\Extractor.obj: internal.h
$(outdir)\Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\Extractor.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\Extractor.obj: CabExtractor.h TarBzip2Extractor.h win/winCabExtractor.h
$(outdir)\PackageInstaller.obj: StdAfx.h
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
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\PackageInstaller.obj: internal.h
$(outdir)\PackageInstaller.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\PackageInstaller.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\PackageInstaller.obj: TpmParser.h
$(outdir)\PackageIterator.obj: StdAfx.h
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
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\PackageIterator.obj: internal.h
$(outdir)\PackageIterator.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\PackageIterator.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\StdAfx.obj: StdAfx.h
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
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\TarBzip2Extractor.obj: StdAfx.h
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
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\TarBzip2Extractor.obj: internal.h
$(outdir)\TarBzip2Extractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\TarBzip2Extractor.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\TarBzip2Extractor.obj: TarBzip2Extractor.h
$(outdir)\TpmParser.obj: StdAfx.h
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
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\TpmParser.obj: internal.h
$(outdir)\TpmParser.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\TpmParser.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\TpmParser.obj: TpmParser.h
$(outdir)\WebSession.obj: StdAfx.h
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
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\WebSession.obj: internal.h
$(outdir)\WebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\WebSession.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\WebSession.obj: win/winWebSession.h CurlWebSession.h
$(outdir)\mpm.obj: StdAfx.h
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
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\mpm.obj: internal.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\mpm.obj: mpm-version.h Extractor.h WebSession.h WebFile.h TpmParser.h
$(outdir)\mpm.obj: ../WebServiceClients/Repository/repositoryRepositorySoapProxy.h
$(outdir)\mpm.obj: ../WebServiceClients/Repository/repositoryH.h
$(outdir)\mpm.obj: ../WebServiceClients/Repository/repositoryStub.h
$(outdir)\mpm.obj: $(miktexsrcdir)/Libraries/3rd/gsoap/stdsoap2.h
$(outdir)\mpm.obj: ../WebServiceClients/Repository/RepositorySoap.nsmap
$(outdir)\winCabExtractor.obj: StdAfx.h
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
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winCabExtractor.obj: internal.h
$(outdir)\winCabExtractor.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\winCabExtractor.obj: mpm-version.h Extractor.h WebSession.h
$(outdir)\winCabExtractor.obj: WebFile.h win/winCabExtractor.h
$(outdir)\winWebFile.obj: StdAfx.h
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
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winWebFile.obj: internal.h
$(outdir)\winWebFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\winWebFile.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\winWebFile.obj: win/winWebFile.h win/winWebSession.h
$(outdir)\winWebSession.obj: StdAfx.h
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
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winWebSession.obj: internal.h
$(outdir)\winWebSession.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\winWebSession.obj: mpm-version.h Extractor.h WebSession.h WebFile.h
$(outdir)\winWebSession.obj: win/winWebFile.h win/winWebSession.h
