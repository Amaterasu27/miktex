## miktex.mak: libjpeg
## 
## Copyright (C) 2001-2005 Christian Schenk
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
	$(outdir)\jcapimin.obj \
	$(outdir)\jcapistd.obj \
	$(outdir)\jccoefct.obj \
	$(outdir)\jccolor.obj \
	$(outdir)\jcdctmgr.obj \
	$(outdir)\jchuff.obj \
	$(outdir)\jcinit.obj \
	$(outdir)\jcmainct.obj \
	$(outdir)\jcmarker.obj \
	$(outdir)\jcmaster.obj \
	$(outdir)\jcomapi.obj \
	$(outdir)\jcparam.obj \
	$(outdir)\jcphuff.obj \
	$(outdir)\jcprepct.obj \
	$(outdir)\jcsample.obj \
	$(outdir)\jctrans.obj \
	$(outdir)\jdapimin.obj \
	$(outdir)\jdapistd.obj \
	$(outdir)\jdatadst.obj \
	$(outdir)\jdatasrc.obj \
	$(outdir)\jdcoefct.obj \
	$(outdir)\jdcolor.obj \
	$(outdir)\jddctmgr.obj \
	$(outdir)\jdhuff.obj \
	$(outdir)\jdinput.obj \
	$(outdir)\jdmainct.obj \
	$(outdir)\jdmarker.obj \
	$(outdir)\jdmaster.obj \
	$(outdir)\jdmerge.obj \
	$(outdir)\jdphuff.obj \
	$(outdir)\jdpostct.obj \
	$(outdir)\jdsample.obj \
	$(outdir)\jdtrans.obj \
	$(outdir)\jerror.obj \
	$(outdir)\jfdctflt.obj \
	$(outdir)\jfdctfst.obj \
	$(outdir)\jfdctint.obj \
	$(outdir)\jidctflt.obj \
	$(outdir)\jidctfst.obj \
	$(outdir)\jidctint.obj \
	$(outdir)\jidctred.obj \
	$(outdir)\jmemmgr.obj \
	$(outdir)\jmemnobs.obj \
	$(outdir)\jquant1.obj \
	$(outdir)\jquant2.obj \
	$(outdir)\jutils.obj \

sources = \
	jcapimin.c \
	jcapistd.c \
	jccoefct.c \
	jccolor.c \
	jcdctmgr.c \
	jchuff.c \
	jcinit.c \
	jcmainct.c \
	jcmarker.c \
	jcmaster.c \
	jcomapi.c \
	jcparam.c \
	jcphuff.c \
	jcprepct.c \
	jcsample.c \
	jctrans.c \
	jdapimin.c \
	jdapistd.c \
	jdatadst.c \
	jdatasrc.c \
	jdcoefct.c \
	jdcolor.c \
	jddctmgr.c \
	jdhuff.c \
	jdinput.c \
	jdmainct.c \
	jdmarker.c \
	jdmaster.c \
	jdmerge.c \
	jdphuff.c \
	jdpostct.c \
	jdsample.c \
	jdtrans.c \
	jerror.c \
	jfdctflt.c \
	jfdctfst.c \
	jfdctint.c \
	jidctflt.c \
	jidctfst.c \
	jidctint.c \
	jidctred.c \
	jmemmgr.c \
	jmemnobs.c \
	jquant1.c \
	jquant2.c \
	jutils.c \

defines = \
	-DIN_JPEG_DLL \

cc_options =  \
	$(cflags) $(cdebug) $(cvarsdll) $(miktexvars) -I. $(ccinc_libgnu)

libraries = $(outdir)\libjpeg.lib

jpeg_dll = $(jpeg_dll_name).dll

binaries = $(outdir)\$(jpeg_dll)

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(ccinc_libgnu) \
		$<

all: common-all $(libraries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# libjpeg.dll $(outdir)\libjpeg.exp \
# _____________________________________________________________________________

$(outdir)\$(jpeg_dll): \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\libjpeg.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(jpeg_dll_name) \
		-implib:$(outdir)\libjpeg.lib \
		-out:$@ \
		$(objects) \
		$(outdir)\libjpeg.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\libjpeg.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			libjpeg.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ libjpeg.rc

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

depend:
	$(MAKEDEPEND) -DIN_JPEG_DLL $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\jcapimin.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcapimin.obj: jerror.h
$(outdir)\jcapistd.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcapistd.obj: jerror.h
$(outdir)\jccoefct.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jccoefct.obj: jerror.h
$(outdir)\jccolor.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jccolor.obj: jerror.h
$(outdir)\jcdctmgr.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcdctmgr.obj: jerror.h jdct.h
$(outdir)\jchuff.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jchuff.obj: jerror.h jchuff.h
$(outdir)\jcinit.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcinit.obj: jerror.h
$(outdir)\jcmainct.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcmainct.obj: jerror.h
$(outdir)\jcmarker.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcmarker.obj: jerror.h
$(outdir)\jcmaster.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcmaster.obj: jerror.h
$(outdir)\jcomapi.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcomapi.obj: jerror.h
$(outdir)\jcparam.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcparam.obj: jerror.h
$(outdir)\jcphuff.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcphuff.obj: jerror.h jchuff.h
$(outdir)\jcprepct.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcprepct.obj: jerror.h
$(outdir)\jcsample.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jcsample.obj: jerror.h
$(outdir)\jctrans.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jctrans.obj: jerror.h
$(outdir)\jdapimin.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdapimin.obj: jerror.h
$(outdir)\jdapistd.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdapistd.obj: jerror.h
$(outdir)\jdatadst.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdatadst.obj: jerror.h
$(outdir)\jdatasrc.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdatasrc.obj: jerror.h
$(outdir)\jdcoefct.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdcoefct.obj: jerror.h
$(outdir)\jdcolor.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdcolor.obj: jerror.h
$(outdir)\jddctmgr.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jddctmgr.obj: jerror.h jdct.h
$(outdir)\jdhuff.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdhuff.obj: jerror.h jdhuff.h
$(outdir)\jdinput.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdinput.obj: jerror.h
$(outdir)\jdmainct.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdmainct.obj: jerror.h
$(outdir)\jdmarker.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdmarker.obj: jerror.h
$(outdir)\jdmaster.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdmaster.obj: jerror.h
$(outdir)\jdmerge.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdmerge.obj: jerror.h
$(outdir)\jdphuff.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdphuff.obj: jerror.h jdhuff.h
$(outdir)\jdpostct.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdpostct.obj: jerror.h
$(outdir)\jdsample.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdsample.obj: jerror.h
$(outdir)\jdtrans.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jdtrans.obj: jerror.h
$(outdir)\jerror.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jerror.obj: jerror.h jversion.h
$(outdir)\jfdctflt.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jfdctflt.obj: jerror.h jdct.h
$(outdir)\jfdctfst.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jfdctfst.obj: jerror.h jdct.h
$(outdir)\jfdctint.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jfdctint.obj: jerror.h jdct.h
$(outdir)\jidctflt.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jidctflt.obj: jerror.h jdct.h
$(outdir)\jidctfst.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jidctfst.obj: jerror.h jdct.h
$(outdir)\jidctint.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jidctint.obj: jerror.h jdct.h
$(outdir)\jidctred.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jidctred.obj: jerror.h jdct.h
$(outdir)\jmemmgr.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jmemmgr.obj: jerror.h jmemsys.h
$(outdir)\jmemnobs.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jmemnobs.obj: jerror.h jmemsys.h
$(outdir)\jquant1.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jquant1.obj: jerror.h
$(outdir)\jquant2.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jquant2.obj: jerror.h
$(outdir)\jutils.obj: jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h
$(outdir)\jutils.obj: jerror.h
