## miktex.mak: freetype/lib/arch/win32
## 
## Copyright (C) 1998-2004 Christian Schenk
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
	$(outdir)\freetype.obj \
	$(outdir)\ftxcmap.obj \
	$(outdir)\ftxerr18.obj \
	$(outdir)\ftxgasp.obj \
	$(outdir)\ftxgdef.obj \
	$(outdir)\ftxgpos.obj \
	$(outdir)\ftxgsub.obj \
	$(outdir)\ftxkern.obj \
	$(outdir)\ftxopen.obj \
	$(outdir)\ftxpost.obj \

sources = \
	..\..\extend\ftxcmap.c \
	..\..\extend\ftxerr18.c \
	..\..\extend\ftxgasp.c \
	..\..\extend\ftxgdef.c \
	..\..\extend\ftxgpos.c \
	..\..\extend\ftxgsub.c \
	..\..\extend\ftxkern.c \
	..\..\extend\ftxopen.c \
	..\..\extend\ftxpost.c \
	freetype.c \

includes = \
	-I..\.. \
	-I..\..\extend \

defines = \
	-DEXPORT_DEF=__declspec(dllimport) \
	-DEXPORT_FUNC=__declspec(dllexport) \

compile = \
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(includes) \

freetype_dll = $(freetype_dll_name).dll

binaries = $(outdir)\$(freetype_dll)

libraries = $(outdir)\freetype.lib

.c{$(outdir)\}.obj:
	$(compile) $<

{..\..\extend\}.c.obj:
	$(compile) $<

{..\..\}.c.obj:
	$(compile) $<

all: common-all $(binaries) $(libraries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# freetype.dll
# _____________________________________________________________________________

$(outdir)\$(freetype_dll) $(outdir)\freetype.lib: \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\freetype.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(freetype_dll_name) \
		-export:TT_Get_Kerning_Directory \
		-export:TT_Get_PS_Name \
		-export:TT_Init_Kerning_Extension \
		-export:TT_Init_Post_Extension \
		-export:TT_Load_Kerning_Table \
		-export:TT_Load_PS_Names \
		-implib:$(outdir)\freetype.lib \
		-out:$(outdir)\$(freetype_dll) \
		$(objects) \
		$(outdir)\freetype.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\freetype.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			freetype-version.h \
			freetype.rc \

	$(rc) $(rcstandard) $(includes) $(rcopt_output_file)$@ freetype.rc

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

depend: $(sources)
	$(MAKEDEPEND) $(includes) $(sources)
	$(fixdep)


# DO NOT DELETE

$(outdir)\ftxcmap.obj: ..\..\extend/ftxcmap.h ..\../freetype.h
$(outdir)\ftxcmap.obj: ..\../fterrid.h ..\../ftnameid.h
$(outdir)\ftxcmap.obj: ..\../tttypes.h ..\../ttconfig.h ft_conf.h
$(outdir)\ftxcmap.obj: ..\../ttobjs.h ..\../ttengine.h
$(outdir)\ftxcmap.obj: ..\../ttmutex.h ..\../ttcache.h
$(outdir)\ftxcmap.obj: ..\../tttables.h ..\../ttcmap.h
$(outdir)\ftxerr18.obj: ..\..\extend/ftxerr18.h ..\../freetype.h
$(outdir)\ftxerr18.obj: ..\../fterrid.h ..\../ftnameid.h
$(outdir)\ftxerr18.obj: ..\..\extend/ftxkern.h
$(outdir)\ftxerr18.obj: ..\..\extend/ftxpost.h
$(outdir)\ftxerr18.obj: ..\..\extend/ftxopen.h
$(outdir)\ftxerr18.obj: ..\..\extend/ftxgdef.h
$(outdir)\ftxerr18.obj: ..\..\extend/ftxgsub.h
$(outdir)\ftxerr18.obj: ..\..\extend/ftxgpos.h ..\../ttconfig.h
$(outdir)\ftxerr18.obj: ft_conf.h
$(outdir)\ftxgasp.obj: ..\..\extend/ftxgasp.h ..\../freetype.h
$(outdir)\ftxgasp.obj: ..\../fterrid.h ..\../ftnameid.h
$(outdir)\ftxgasp.obj: ..\../tttypes.h ..\../ttconfig.h ft_conf.h
$(outdir)\ftxgasp.obj: ..\../ttobjs.h ..\../ttengine.h
$(outdir)\ftxgasp.obj: ..\../ttmutex.h ..\../ttcache.h
$(outdir)\ftxgasp.obj: ..\../tttables.h ..\../ttcmap.h
$(outdir)\ftxgdef.obj: ..\../tttypes.h ..\../ttconfig.h ft_conf.h
$(outdir)\ftxgdef.obj: ..\../freetype.h ..\../fterrid.h
$(outdir)\ftxgdef.obj: ..\../ftnameid.h ..\../tttags.h
$(outdir)\ftxgdef.obj: ..\../ttload.h ..\../ttobjs.h
$(outdir)\ftxgdef.obj: ..\../ttengine.h ..\../ttmutex.h
$(outdir)\ftxgdef.obj: ..\../ttcache.h ..\../tttables.h
$(outdir)\ftxgdef.obj: ..\../ttcmap.h ..\../ttextend.h
$(outdir)\ftxgdef.obj: ..\../ttmemory.h ..\../ttfile.h
$(outdir)\ftxgdef.obj: ..\../ttdebug.h ..\..\extend/ftxopen.h
$(outdir)\ftxgdef.obj: ..\..\extend/ftxgdef.h ..\..\extend/ftxgsub.h
$(outdir)\ftxgdef.obj: ..\..\extend/ftxgpos.h
$(outdir)\ftxgdef.obj: ..\..\extend/ftxopenf.h
$(outdir)\ftxgpos.obj: ..\../tttypes.h ..\../ttconfig.h ft_conf.h
$(outdir)\ftxgpos.obj: ..\../freetype.h ..\../fterrid.h
$(outdir)\ftxgpos.obj: ..\../ftnameid.h ..\../tttags.h
$(outdir)\ftxgpos.obj: ..\../ttload.h ..\../ttobjs.h
$(outdir)\ftxgpos.obj: ..\../ttengine.h ..\../ttmutex.h
$(outdir)\ftxgpos.obj: ..\../ttcache.h ..\../tttables.h
$(outdir)\ftxgpos.obj: ..\../ttcmap.h ..\../ttextend.h
$(outdir)\ftxgpos.obj: ..\../ttmemory.h ..\../ttfile.h
$(outdir)\ftxgpos.obj: ..\../ttdebug.h ..\..\extend/ftxopen.h
$(outdir)\ftxgpos.obj: ..\..\extend/ftxgdef.h ..\..\extend/ftxgsub.h
$(outdir)\ftxgpos.obj: ..\..\extend/ftxgpos.h
$(outdir)\ftxgpos.obj: ..\..\extend/ftxopenf.h
$(outdir)\ftxgsub.obj: ..\../tttypes.h ..\../ttconfig.h ft_conf.h
$(outdir)\ftxgsub.obj: ..\../freetype.h ..\../fterrid.h
$(outdir)\ftxgsub.obj: ..\../ftnameid.h ..\../tttags.h
$(outdir)\ftxgsub.obj: ..\../ttload.h ..\../ttobjs.h
$(outdir)\ftxgsub.obj: ..\../ttengine.h ..\../ttmutex.h
$(outdir)\ftxgsub.obj: ..\../ttcache.h ..\../tttables.h
$(outdir)\ftxgsub.obj: ..\../ttcmap.h ..\../ttextend.h
$(outdir)\ftxgsub.obj: ..\../ttmemory.h ..\../ttfile.h
$(outdir)\ftxgsub.obj: ..\../ttdebug.h ..\..\extend/ftxopen.h
$(outdir)\ftxgsub.obj: ..\..\extend/ftxgdef.h ..\..\extend/ftxgsub.h
$(outdir)\ftxgsub.obj: ..\..\extend/ftxgpos.h
$(outdir)\ftxgsub.obj: ..\..\extend/ftxopenf.h
$(outdir)\ftxkern.obj: ..\..\extend/ftxkern.h ..\../freetype.h
$(outdir)\ftxkern.obj: ..\../fterrid.h ..\../ftnameid.h
$(outdir)\ftxkern.obj: ..\../ttextend.h ..\../ttconfig.h ft_conf.h
$(outdir)\ftxkern.obj: ..\../tttypes.h ..\../ttobjs.h
$(outdir)\ftxkern.obj: ..\../ttengine.h ..\../ttmutex.h
$(outdir)\ftxkern.obj: ..\../ttcache.h ..\../tttables.h
$(outdir)\ftxkern.obj: ..\../ttcmap.h ..\../ttdebug.h ..\../ttload.h
$(outdir)\ftxkern.obj: ..\../ttmemory.h ..\../ttfile.h
$(outdir)\ftxkern.obj: ..\../tttags.h
$(outdir)\ftxopen.obj: ..\../tttypes.h ..\../ttconfig.h ft_conf.h
$(outdir)\ftxopen.obj: ..\../freetype.h ..\../fterrid.h
$(outdir)\ftxopen.obj: ..\../ftnameid.h ..\../ttload.h
$(outdir)\ftxopen.obj: ..\../ttobjs.h ..\../ttengine.h
$(outdir)\ftxopen.obj: ..\../ttmutex.h ..\../ttcache.h
$(outdir)\ftxopen.obj: ..\../tttables.h ..\../ttcmap.h
$(outdir)\ftxopen.obj: ..\../ttextend.h ..\../ttmemory.h
$(outdir)\ftxopen.obj: ..\../ttfile.h ..\../ttdebug.h
$(outdir)\ftxopen.obj: ..\..\extend/ftxopen.h ..\..\extend/ftxgdef.h
$(outdir)\ftxopen.obj: ..\..\extend/ftxgsub.h ..\..\extend/ftxgpos.h
$(outdir)\ftxopen.obj: ..\..\extend/ftxopenf.h
$(outdir)\ftxpost.obj: ..\..\extend/ftxpost.h ..\../freetype.h
$(outdir)\ftxpost.obj: ..\../fterrid.h ..\../ftnameid.h
$(outdir)\ftxpost.obj: ..\../tttypes.h ..\../ttconfig.h ft_conf.h
$(outdir)\ftxpost.obj: ..\../ttobjs.h ..\../ttengine.h
$(outdir)\ftxpost.obj: ..\../ttmutex.h ..\../ttcache.h
$(outdir)\ftxpost.obj: ..\../tttables.h ..\../ttcmap.h
$(outdir)\ftxpost.obj: ..\../ttload.h ..\../ttfile.h ..\../ttdebug.h
$(outdir)\ftxpost.obj: ..\../tttags.h ..\../ttmemory.h
$(outdir)\ftxpost.obj: ..\../ttextend.h
$(outdir)\freetype.obj: ..\../ttapi.c ..\../ttconfig.h ft_conf.h ..\../freetype.h
$(outdir)\freetype.obj: ..\../fterrid.h ..\../ftnameid.h ..\../ttengine.h
$(outdir)\freetype.obj: ..\../tttypes.h ..\../ttmutex.h ..\../ttcalc.h
$(outdir)\freetype.obj: ..\../ttmemory.h ..\../ttcache.h ..\../ttfile.h
$(outdir)\freetype.obj: ..\../ttdebug.h ..\../ttobjs.h ..\../tttables.h
$(outdir)\freetype.obj: ..\../ttcmap.h ..\../ttload.h ..\../ttgload.h
$(outdir)\freetype.obj: ..\../ttraster.h ..\../ttextend.h ..\../ttcache.c
$(outdir)\freetype.obj: ..\../ttcalc.c ..\../ttcmap.c ..\../ttdebug.c
$(outdir)\freetype.obj: ..\../ttgload.c ..\../tttags.h ..\../ttinterp.c
$(outdir)\freetype.obj: ..\../ttinterp.h ..\../ttload.c ..\../ttobjs.c
$(outdir)\freetype.obj: ..\../ttraster.c ..\../ttfile.c ..\../ttmemory.c
$(outdir)\freetype.obj: ..\../ttmutex.c ..\../ttextend.c
