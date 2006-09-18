<?xml version='1.0'?>
<!--############################################################################# 
 |	$Id: test.xsl,v 1.2 2003/03/14 15:06:37 rcasellas Exp $
 |- #############################################################################
 |	$Author: rcasellas $												
 |														
 + ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:import href="../../docbook.xsl"/>
<xsl:output method="text" encoding="ISO-8859-1" indent="yes"/>
<xsl:variable name="latex.use.babel">0</xsl:variable>
<xsl:variable name="latex.use.isolatin1">1</xsl:variable>
<xsl:variable name="latex.use.hyperref">1</xsl:variable>
<xsl:variable name="latex.use.fancyvrb">0</xsl:variable>
<xsl:variable name="latex.use.fancybox">1</xsl:variable>
<xsl:variable name="latex.use.fancyhdr">1</xsl:variable>
<xsl:variable name="latex.use.subfigure">0</xsl:variable>
<xsl:variable name="latex.use.rotating">1</xsl:variable>
<xsl:variable name="latex.use.makeidx">1</xsl:variable>
<xsl:variable name="latex.pdf.support">1</xsl:variable>
<xsl:variable name="latex.math.support">0</xsl:variable>
<xsl:variable name="latex.biblio.output">cited</xsl:variable>

</xsl:stylesheet>
