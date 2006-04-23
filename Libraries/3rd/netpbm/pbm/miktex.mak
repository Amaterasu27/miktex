## miktex.mak: netpbm/pbm
## 
## Copyright (C) 1998-2006 Christian Schenk
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

sources = \
	atktopbm.c \
	brushtopbm.c \
	cmuwmtopbm.c \
	g3topbm.c \
	gemtopbm.c \
	icontopbm.c \
	libpbm1.c \
	libpbm2.c \
	libpbm3.c \
	libpbm4.c \
	libpbm5.c \
	macptopbm.c \
	mgrtopbm.c \
	pbmclean.c \
	pbmlife.c \
	pbmmake.c \
	pbmmask.c \
	pbmpscale.c \
	pbmreduce.c \
	pbmtext.c \
	pbmto10x.c \
	pbmto4425.c \
	pbmtoascii.c \
	pbmtoatk.c \
	pbmtobbnbg.c \
	pbmtocmuwm.c \
	pbmtoepsi.c \
	pbmtoepson.c \
	pbmtog3.c \
	pbmtogem.c \
	pbmtogo.c \
	pbmtoicon.c \
	pbmtolj.c \
	pbmtoln03.c \
	pbmtolps.c \
	pbmtomacp.c \
	pbmtomgr.c \
	pbmtopi3.c \
	pbmtopk.c \
	pbmtoplot.c \
	pbmtoptx.c \
	pbmtox10bm.c \
	pbmtoxbm.c \
	pbmtoybm.c \
	pbmtozinc.c \
	pbmupc.c \
	pi3topbm.c \
	pktopbm.c \
	xbmtopbm.c \
	ybmtopbm.c \

exes = \
	$(outdir)\atktopbm.exe \
	$(outdir)\brushtopbm.exe \
	$(outdir)\cmuwmtopbm.exe \
	$(outdir)\g3topbm.exe \
	$(outdir)\gemtopbm.exe \
	$(outdir)\icontopbm.exe \
	$(outdir)\macptopbm.exe \
	$(outdir)\mgrtopbm.exe \
	$(outdir)\pbmclean.exe \
	$(outdir)\pbmlife.exe \
	$(outdir)\pbmmake.exe \
	$(outdir)\pbmmask.exe \
	$(outdir)\pbmpscale.exe \
	$(outdir)\pbmreduce.exe \
	$(outdir)\pbmtext.exe \
	$(outdir)\pbmto10x.exe \
	$(outdir)\pbmto4425.exe \
	$(outdir)\pbmtoascii.exe \
	$(outdir)\pbmtoatk.exe \
	$(outdir)\pbmtobbnbg.exe \
	$(outdir)\pbmtocmuwm.exe \
	$(outdir)\pbmtoepsi.exe \
	$(outdir)\pbmtoepson.exe \
	$(outdir)\pbmtog3.exe \
	$(outdir)\pbmtogem.exe \
	$(outdir)\pbmtogo.exe \
	$(outdir)\pbmtoicon.exe \
	$(outdir)\pbmtolj.exe \
	$(outdir)\pbmtoln03.exe \
	$(outdir)\pbmtolps.exe \
	$(outdir)\pbmtomacp.exe \
	$(outdir)\pbmtomgr.exe \
	$(outdir)\pbmtopi3.exe \
	$(outdir)\pbmtopk.exe \
	$(outdir)\pbmtoplot.exe \
	$(outdir)\pbmtoptx.exe \
	$(outdir)\pbmtox10bm.exe \
	$(outdir)\pbmtoxbm.exe \
	$(outdir)\pbmtoybm.exe \
	$(outdir)\pbmtozinc.exe \
	$(outdir)\pbmupc.exe \
	$(outdir)\pi3topbm.exe \
	$(outdir)\pktopbm.exe \
	$(outdir)\xbmtopbm.exe \
	$(outdir)\ybmtopbm.exe \

binaries = $(outdir)\$(pbm_dll_name).dll $(exes)

libraries = $(outdir)\libpbm.lib

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		-DLIBTIFF \
		-DA_STRERROR \
		-I.. \
		$(ccopt_output_directory)$(outdir)\ \
		$<

{$(outdir)\}.obj{$(outdir)\}.exe:
	$(link) $(lstandard) \
		-out:$*.exe \
		$(outdir)\libpbm.lib \
		$<
	mt -nologo -manifest $*.exe.manifest -outputresource:$*.exe;1

all: common-all $(binaries) $(libraries)

install: common-install install-binaries

check: common-check

$(exes): $(outdir)\libpbm.lib

objects = \
	$(outdir)\libpbm.exp \
	$(outdir)\libpbm1.obj \
	$(outdir)\libpbm2.obj \
	$(outdir)\libpbm3.obj \
	$(outdir)\libpbm4.obj \
	$(outdir)\libpbm5.obj \

$(outdir)\$(pbm_dll_name).dll: \
			$(dllbases) \
			$(objects) \
			$(outdir) \
			$(outdir)\libpbm.exp \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(pbm_dll_name) \
		-out:$@ \
		$(objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\libpbm.lib $(outdir)\libpbm.exp: \
			$(outdir) \
			libpbm.def \

	$(libtool) $(libflags) \
		-def:libpbm.def \
		-name:$(pbm_dll_name) \
		-out:$(outdir)\libpbm.lib \

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) -I.. $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\atktopbm.obj: pbm.h ../pbmplus.h
$(outdir)\brushtopbm.obj: pbm.h ../pbmplus.h
$(outdir)\cmuwmtopbm.obj: pbm.h ../pbmplus.h cmuwm.h
$(outdir)\g3topbm.obj: pbm.h ../pbmplus.h g3.h
$(outdir)\gemtopbm.obj: pbm.h ../pbmplus.h
$(outdir)\icontopbm.obj: pbm.h ../pbmplus.h
$(outdir)\libpbm1.obj: pbm.h ../pbmplus.h ../version.h ../compile.h libpbm.h
$(outdir)\libpbm2.obj: pbm.h ../pbmplus.h libpbm.h
$(outdir)\libpbm3.obj: pbm.h ../pbmplus.h libpbm.h
$(outdir)\libpbm4.obj: pbm.h ../pbmplus.h libpbm.h
$(outdir)\libpbm5.obj: pbm.h ../pbmplus.h pbmfont.h
$(outdir)\macptopbm.obj: pbm.h ../pbmplus.h macp.h
$(outdir)\mgrtopbm.obj: pbm.h ../pbmplus.h mgr.h
$(outdir)\pbmclean.obj: pbm.h ../pbmplus.h
$(outdir)\pbmlife.obj: pbm.h ../pbmplus.h
$(outdir)\pbmmake.obj: pbm.h ../pbmplus.h
$(outdir)\pbmmask.obj: pbm.h ../pbmplus.h
$(outdir)\pbmpscale.obj: pbm.h ../pbmplus.h
$(outdir)\pbmreduce.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtext.obj: pbm.h ../pbmplus.h pbmfont.h
$(outdir)\pbmto10x.obj: pbm.h ../pbmplus.h
$(outdir)\pbmto4425.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtoascii.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtoatk.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtobbnbg.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtocmuwm.obj: pbm.h ../pbmplus.h cmuwm.h
$(outdir)\pbmtoepsi.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtoepson.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtog3.obj: pbm.h ../pbmplus.h g3.h
$(outdir)\pbmtogem.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtogo.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtoicon.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtolj.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtoln03.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtolps.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtomacp.obj: pbm.h ../pbmplus.h macp.h
$(outdir)\pbmtomgr.obj: pbm.h ../pbmplus.h mgr.h
$(outdir)\pbmtopi3.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtopk.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtoplot.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtoptx.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtox10bm.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtoxbm.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtoybm.obj: pbm.h ../pbmplus.h
$(outdir)\pbmtozinc.obj: pbm.h ../pbmplus.h
$(outdir)\pbmupc.obj: pbm.h ../pbmplus.h
$(outdir)\pi3topbm.obj: pbm.h ../pbmplus.h
$(outdir)\pktopbm.obj: pbm.h ../pbmplus.h
$(outdir)\xbmtopbm.obj: pbm.h ../pbmplus.h
$(outdir)\ybmtopbm.obj: pbm.h ../pbmplus.h
