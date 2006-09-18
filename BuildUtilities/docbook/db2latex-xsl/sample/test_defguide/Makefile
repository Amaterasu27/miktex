# Our intention is that you checkout the defguide module
# from the DocBook Open Repository (see the 'fetch' target).

# Use 'fetch' to bootstrap our editing.
# Then use 'setup' to produce XML from eCos documentation.
# Then use standard targets to build the PDF.
# Use 'patches' after you have performed editing.

TEST_NAME=defguide

pdfs = $(addsuffix .pdf,$(basename $(notdir $(wildcard defguide/en/defguide.xml))))

include ../Makefile

fetch:
	cvs -d :pserver:anonymous:@cvs.sourceforge.net:/cvsroot/docbook login
	cvs -d :pserver:anonymous@cvs.sourceforge.net:/cvsroot/docbook co defguide
	cd ecos && patch -p0 < ../defguide.patch

setup:
	ln -s ./defguide/en/figures/300dpi figures
	test -d defguide
	cd defguide && patch --quiet -f < defguide.patch

patches:
	cd defguide && ( cvs diff -u > ../defguide.patch || true )

