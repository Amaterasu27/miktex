## miktex.mak: libtiff
##
## Copyright (C) 2000-2005 Christian Schenk
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

miktex_cc_no_runtime_checks = 1
miktex_cc_buffer_security_check = 1

!include <miktex.inc>

objects = \
	$(outdir)\tif_aux.obj \
	$(outdir)\tif_close.obj \
	$(outdir)\tif_codec.obj \
	$(outdir)\tif_color.obj \
	$(outdir)\tif_compress.obj \
	$(outdir)\tif_dir.obj \
	$(outdir)\tif_dirinfo.obj \
	$(outdir)\tif_dirread.obj \
	$(outdir)\tif_dirwrite.obj \
	$(outdir)\tif_dumpmode.obj \
	$(outdir)\tif_error.obj \
	$(outdir)\tif_extension.obj \
	$(outdir)\tif_fax3.obj \
	$(outdir)\tif_fax3sm.obj \
	$(outdir)\tif_flush.obj \
	$(outdir)\tif_getimage.obj \
	$(outdir)\tif_jpeg.obj \
	$(outdir)\tif_luv.obj \
	$(outdir)\tif_lzw.obj \
	$(outdir)\tif_next.obj \
	$(outdir)\tif_ojpeg.obj \
	$(outdir)\tif_open.obj \
	$(outdir)\tif_packbits.obj \
	$(outdir)\tif_pixarlog.obj \
	$(outdir)\tif_predict.obj \
	$(outdir)\tif_print.obj \
	$(outdir)\tif_read.obj \
	$(outdir)\tif_strip.obj \
	$(outdir)\tif_swab.obj \
	$(outdir)\tif_thunder.obj \
	$(outdir)\tif_tile.obj \
	$(outdir)\tif_version.obj \
	$(outdir)\tif_warning.obj \
	$(outdir)\tif_win32.obj \
	$(outdir)\tif_write.obj \
	$(outdir)\tif_zip.obj \

sources = \
	tif_aux.c \
	tif_close.c \
	tif_codec.c \
	tif_color.c \
	tif_compress.c \
	tif_dir.c \
	tif_dirinfo.c \
	tif_dirread.c \
	tif_dirwrite.c \
	tif_dumpmode.c \
	tif_error.c \
	tif_extension.c \
	tif_fax3.c \
	tif_fax3sm.c \
	tif_flush.c \
	tif_getimage.c \
	tif_jpeg.c \
	tif_luv.c \
	tif_lzw.c \
	tif_next.c \
	tif_ojpeg.c \
	tif_open.c \
	tif_packbits.c \
	tif_pixarlog.c \
	tif_predict.c \
	tif_print.c \
	tif_read.c \
	tif_strip.c \
	tif_swab.c \
	tif_thunder.c \
	tif_tile.c \
	tif_version.c \
	tif_warning.c \
	tif_win32.c \
	tif_write.c \
	tif_zip.c \

libraries = $(tiff_lib)

tiff_dll = $(tiff_dll_name).dll

binaries = $(outdir)\$(tiff_dll)

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(libraries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# libtiff.dll
# _____________________________________________________________________________

$(outdir)\$(tiff_dll): \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\libtiff.exp \
			$(outdir)\libtiff.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(tiff_dll_name) \
		-out:$@ \
		$(objects) \
		$(outdir)\libtiff.exp \
		$(outdir)\libtiff.res \
		user32.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(tiff_lib) $(outdir)\libtiff.exp: \
			$(outdir) \
			libtiff.def \

	$(libtool) $(libflags) \
		-def:libtiff.def \
		-name:$(tiff_dll_name) \
		-out:$(tiff_lib) \

$(outdir)\libtiff.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			libtiff-version.h \
			libtiff.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ libtiff.rc

# _____________________________________________________________________________
#
# tiffvers.h
# _____________________________________________________________________________

tiffvers.h: \
			$(outdir)\mkversion.exe \
			RELEASE-DATE \
			VERSION \

	$(outdir)\mkversion \
		-r RELEASE-DATE \
		-v VERSION \
		$@

$(outdir)\mkversion.exe: \
			$(outdir) \
			$(outdir)\mkversion.obj \

	$(link) $(lstandard) \
		-out:$@ \
		$(outdir)\mkversion.obj
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# tif_fax3sm.c
# _____________________________________________________________________________

tif_fax3sm.c: \
			$(outdir)\mkg3states.exe \
			mkg3states.c \

	$(outdir)\mkg3states -c const $@

$(outdir)\mkg3states.exe: \
			$(outdir) \
			$(gnu_lib) \
			$(outdir)\mkg3states.obj \

	$(link) $(lstandard) \
		-out:$@ \
		$(outdir)\mkg3states.obj \
		$(gnu_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	-del /f tiffvers.h
	-del /f tif_fax3sm.c

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

$(objects): tiffvers.h

depend:
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\tif_aux.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h tiffvers.h
$(outdir)\tif_aux.obj: tif_dir.h tif_predict.h
$(outdir)\tif_close.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_close.obj: tiffvers.h tif_dir.h
$(outdir)\tif_codec.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_codec.obj: tiffvers.h tif_dir.h
$(outdir)\tif_color.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_color.obj: tiffvers.h tif_dir.h
$(outdir)\tif_compress.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_compress.obj: tiffvers.h tif_dir.h
$(outdir)\tif_dir.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h tiffvers.h
$(outdir)\tif_dir.obj: tif_dir.h
$(outdir)\tif_dirinfo.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_dirinfo.obj: tiffvers.h tif_dir.h
$(outdir)\tif_dirread.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_dirread.obj: tiffvers.h tif_dir.h
$(outdir)\tif_dirwrite.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_dirwrite.obj: tiffvers.h tif_dir.h
$(outdir)\tif_dumpmode.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_dumpmode.obj: tiffvers.h tif_dir.h
$(outdir)\tif_error.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_error.obj: tiffvers.h tif_dir.h
$(outdir)\tif_extension.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_extension.obj: tiffvers.h tif_dir.h
$(outdir)\tif_fax3.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_fax3.obj: tiffvers.h tif_dir.h tif_fax3.h t4.h
$(outdir)\tif_fax3sm.obj: tiff.h tif_fax3.h
$(outdir)\tif_flush.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_flush.obj: tiffvers.h tif_dir.h
$(outdir)\tif_getimage.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_getimage.obj: tiffvers.h tif_dir.h
$(outdir)\tif_jpeg.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_jpeg.obj: tiffvers.h tif_dir.h
$(outdir)\tif_luv.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h tiffvers.h
$(outdir)\tif_luv.obj: tif_dir.h uvcode.h
$(outdir)\tif_lzw.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h tiffvers.h
$(outdir)\tif_lzw.obj: tif_dir.h tif_predict.h
$(outdir)\tif_next.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_next.obj: tiffvers.h tif_dir.h
$(outdir)\tif_ojpeg.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_ojpeg.obj: tiffvers.h tif_dir.h
$(outdir)\tif_open.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_open.obj: tiffvers.h tif_dir.h
$(outdir)\tif_packbits.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_packbits.obj: tiffvers.h tif_dir.h
$(outdir)\tif_pixarlog.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_pixarlog.obj: tiffvers.h tif_dir.h
$(outdir)\tif_predict.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_predict.obj: tiffvers.h tif_dir.h tif_predict.h
$(outdir)\tif_print.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_print.obj: tiffvers.h tif_dir.h
$(outdir)\tif_read.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_read.obj: tiffvers.h tif_dir.h
$(outdir)\tif_strip.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_strip.obj: tiffvers.h tif_dir.h
$(outdir)\tif_swab.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_swab.obj: tiffvers.h tif_dir.h
$(outdir)\tif_thunder.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_thunder.obj: tiffvers.h tif_dir.h
$(outdir)\tif_tile.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_tile.obj: tiffvers.h tif_dir.h
$(outdir)\tif_version.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_version.obj: tiffvers.h tif_dir.h
$(outdir)\tif_warning.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_warning.obj: tiffvers.h tif_dir.h
$(outdir)\tif_win32.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_win32.obj: tiffvers.h tif_dir.h
$(outdir)\tif_write.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h
$(outdir)\tif_write.obj: tiffvers.h tif_dir.h
$(outdir)\tif_zip.obj: tiffiop.h tiffconf.h tiffcomp.h tiffio.h tiff.h tiffvers.h
$(outdir)\tif_zip.obj: tif_dir.h
