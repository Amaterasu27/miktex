<?xml version='1.0'?>
<!--############################################################################# 
 |	$Id: slides.xsl,v 1.1 2003/04/06 18:31:50 rcasellas Exp $
 |- #############################################################################
 |	$Author: rcasellas $												
 |														
 + ############################################################################## -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:import href="../slides.xsl"/>
<xsl:output method="text" encoding="ISO-8859-1" indent="yes"/>
<xsl:variable name="latex.use.babel">1</xsl:variable>
<xsl:variable name="latex.use.isolatin1">1</xsl:variable>
<xsl:variable name="latex.use.hyperref">1</xsl:variable>
<xsl:variable name="latex.use.fancyvrb">1</xsl:variable>
<xsl:variable name="latex.use.fancybox">1</xsl:variable>
<xsl:variable name="latex.use.fancyhdr">1</xsl:variable>
<xsl:variable name="latex.use.subfigure">1</xsl:variable>
<xsl:variable name="latex.use.rotating">1</xsl:variable>
<xsl:variable name="latex.use.makeidx">1</xsl:variable>
<xsl:variable name="latex.pdf.support">1</xsl:variable>
<xsl:variable name="latex.math.support">1</xsl:variable>

<xsl:variable name="latex.biblio.output">all</xsl:variable>
<xsl:variable name="latex.document.font">default</xsl:variable>
</xsl:stylesheet>
