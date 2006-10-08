# Our intention is that you obtain Bv9ARM-book.xml
# from the BIND 9 distribution. It's in
# doc/arm/Bv9ARM-book.xml

# Requires wget, tar

TEST_NAME=Bv9ARM-book
EXTRA_DISTCLEAN=*.pdf *.tex bind-$(BIND_VERSION)* *.xml *.orig

BIND_VERSION=9.2.2
ARCHIVE=bind-$(BIND_VERSION).tar.gz
ORIGINAL=bind-$(BIND_VERSION)/doc/arm/$(TEST_NAME).xml

include ../Makefile

$(ARCHIVE):
	wget ftp://ftp.isc.org/isc/bind9/$(BIND_VERSION)/$(ARCHIVE)

$(ORIGINAL): $(ARCHIVE)
	tar xmzf $< $@

$(TEST_NAME).xml: $(ORIGINAL)
	cp $(ORIGINAL) .
	patch --quiet -f < $(TEST_NAME).patch

patches:
	test -f $(TEST_NAME).xml
	test -f $(ORIGINAL)
	diff -u $(ORIGINAL) $(TEST_NAME).xml > $(TEST_NAME).patch || true

