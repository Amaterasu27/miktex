## miktex.mak:
##
## Copyright (C) 1996-2006 Christian Schenk
##
## This file is part of the MiKTeX Core Library.
##
## The MiKTeX Core Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX Core Library is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX Core Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

!include <miktex.inc>

sandbox = $(miktexsrc)\Libraries\MiKTeX\Core\test\sandbox-win
installroot = $(sandbox)\texmf
dataroot = $(sandbox)\localtexmf

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		-DDATAROOT=\"$(dataroot:\=/)\" \
		-DINSTALLROOT=\"$(installroot:\=/)\" \
		$<

{$(outdir)\}.obj{$(outdir)\}.exe:
	$(link) $(lstandard) \
		-out:$@ \
		$< \
		$(miktex_lib) \
		$(popt_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# check
# _____________________________________________________________________________

check: \
			common-check \
			$(outdir) \
			$(outdir)\1.exe \
			$(outdir)\2.exe \
			$(sandbox) \

	$(outdir)\1.exe $(test_options)
	$(outdir)\2.exe $(test_options)

# _____________________________________________________________________________
#
# sandbox
# _____________________________________________________________________________

$(sandbox):
	cmd /c mkdir $(dataroot)
	cmd /c mkdir $(installroot)\tex\latex\base
	cmd /c mkdir $(installroot)\tex\test\base
	echo.>$(installroot)\tex\latex\base\test.cls
	echo.>$(installroot)\tex\test\base\test.tex

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	if exist $(sandbox) rmdir /s /q $(sandbox)

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: 1.cpp 2.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/test.h
$(outdir)\1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\1.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\1.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\1.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\2.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/test.h
$(outdir)\2.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\2.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\2.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\2.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\2.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\2.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
