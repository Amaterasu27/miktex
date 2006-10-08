# Our intention is that you obtain the user-guide
# directory from the eCos distribution. It's at
# ecos/doc/sgml/user-guide/ (see the 'fetch' target).

# Use 'fetch' to bootstrap our editing.
# Then use 'setup' to produce XML from eCos documentation.
# Then use standard targets to build the PDF.
# Use 'patches' after you have performed editing.

# Requires the osx utility from OpenJade

TEST_NAME=user-guide
EXTRA_DISTCLEAN=*.xml $(TEST_NAME).tex $(TEST_NAME).pdf *.orig

pdfs = $(addsuffix .pdf,$(basename $(wildcard $(TEST_NAME).xml)))

include ../Makefile

fetch:
	cvs -d :pserver:anoncvs:@sources.redhat.com:/cvs/ecos login
	cvs -d :pserver:anoncvs@sources.redhat.com:/cvs/ecos co -P -d ecos ecos/doc/sgml
	cd ecos && patch -N -p0 < ../ecos.patch

setup: user-guide.xml

patches:
	test -f $(TEST_NAME).xml
	test -f $(TEST_NAME).xml.orig
	diff -u $(TEST_NAME).xml.orig $(TEST_NAME).xml > $(TEST_NAME).patch || true
	cd ecos && ( cvs diff -u > ../ecos.patch || true )

user-guide.xml:ecos/user-guide/user-guide.sgml ecos/user-guide/*.sgml
	-osx -xempty -xlower -xndata -xno-nl-in-tag $< > $@
	test -f $(TEST_NAME).xml
	patch --quiet -f < $(TEST_NAME).patch

