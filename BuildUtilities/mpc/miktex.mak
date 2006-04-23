## miktex.mak:
## 
## Copyright (C) 2001-2006 Christian Schenk
## 
## This file is part of MPC.
## 
## MPC is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## MPC is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
## or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
## License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with MPC; if not, write to the Free Software Foundation, 59
## Temple Place - Suite 330, Boston, MA 02111-1307, USA.

no_miktex_dlls = 1
miktex_cc_use_crt_dll = 0

miktex_cc_warnings_as_errors = 1
miktex_cc_64bit_portability = 1

!include <miktex.inc>

binaries = $(outdir)\mpc.exe

cc_options = \
	$(ccopt_create_pdb) \
	$(cflags) \
	$(cdebug) \
	$(cvarsdll) \
	$(cflagseh) \
	$(incdirs) \
	$(miktexvars) \
	$(cflagseh) \
	$(ccopt_output_directory)$(outdir)\ \
	$(ccopt_name_precompiled_header_file)$(outdir)\mpc.pch

.cpp{$(outdir)\}.obj:
	$(cc) \
		$(cc_options) \
		$(ccopt_use_precompiled_header_file)StdAfx.h \
			$<

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# mpc
# _____________________________________________________________________________

mpc_objects = \
	$(outdir)\StdAfx.obj \
	\
	$(bz2_static_lib) \
	$(curl_static_lib) \
	$(expat_static_lib) \
	$(md5_static_lib) \
	$(miktex_static_lib) \
	$(mpm_static_lib) \
	$(mspack_static_lib) \
	$(outdir)\mpc.obj \
#	$(outdir)\mpc.res \
	$(gnu_static_lib) \

$(outdir)\mpc.exe: $(outdir) $(mpc_objects)
	$(link) $(conlflags) $(ldebug) \
		-delayload:advapi32.dll \
		-delayload:winspool.drv \
		-delayload:wininet.dll \
		-out:$@ \
		$(mpc_objects) \
			$(conlibsdll) \
			delayimp.lib \
			Winmm.lib \
			Ws2_32.lib \
			fdi.lib \
			user32.lib \
			wininet.lib \

$(outdir)\mpc.res: \
			$(outdir) \
			mpc.rc \
			$(libdir)\include\miktex\miktex.version \
			$(libdir)\include\miktex\version.h
	$(rc) $(rcstandard) $(rcopt_output_file)$@ mpc.rc

$(outdir)\StdAfx.obj: $(outdir) StdAfx.cpp
	$(cc) \
		$(cc_options) \
		$(ccopt_create_precompiled_header_file)StdAfx.h \
		StdAfx.cpp

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

depend: mpc.cpp StdAfx.cpp
	$(MAKEDEPEND) -I$(mpmlibdir) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\mpc.obj: StdAfx.h $(miktexsrcdir)/Libraries\3rd\libgnu/getopt.h
$(outdir)\mpc.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\mpc.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\mpc.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\mpc.obj: $(miktexsrcdir)/Libraries\MiKTeX\PackageManager/include/miktex/mpm.h
$(outdir)\mpc.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\mpc.obj: internal.h
$(outdir)\StdAfx.obj: StdAfx.h $(miktexsrcdir)/Libraries\3rd\libgnu/getopt.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\MiKTeX\PackageManager/include/miktex/mpm.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
