# Tests from separate .xml files
pdfs := $(patsubst %.xml,%.pdf,$(wildcard *.xml))

include ../Makefile

%.tex: %.xml test.xsl $(FORCE_TARGET)
	$(call XSLT, $@, test.xsl, $< ) 2>&1 | tee $*.msg
