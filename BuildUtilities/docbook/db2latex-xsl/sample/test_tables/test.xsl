<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<!--############################################################################# 
 |	$Id: test.xsl,v 1.3 2003/05/20 10:07:45 j-devenish Exp $
 |- #############################################################################
 |	$Author: j-devenish $												
 |														
 + ############################################################################## -->

<xsl:import href="../../docbook.xsl"/>
<xsl:output method="text" encoding="ISO-8859-1" indent="yes"/>
<xsl:variable name="latex.use.babel">0</xsl:variable>
<xsl:variable name="latex.use.isolatin1">0</xsl:variable>
<xsl:variable name="latex.use.hyperref">0</xsl:variable>
<xsl:variable name="latex.use.fancyvrb">0</xsl:variable>
<xsl:variable name="latex.use.fancybox">1</xsl:variable>
<xsl:variable name="latex.use.fancyhdr">0</xsl:variable>
<xsl:variable name="latex.use.subfigure">1</xsl:variable>
<xsl:variable name="latex.use.rotating">1</xsl:variable>
<xsl:variable name="latex.use.makeidx">1</xsl:variable>
<xsl:variable name="latex.pdf.support">1</xsl:variable>
<xsl:variable name="latex.math.support">0</xsl:variable>
<xsl:variable name="latex.use.ltxtable">0</xsl:variable>
<xsl:variable name="latex.document.font">palatino</xsl:variable>
<xsl:variable name="latex.use.dcolumn">0</xsl:variable>
<xsl:variable name="latex.decimal.point">-</xsl:variable>

<xsl:variable name="latex.biblio.output">cited</xsl:variable>

</xsl:stylesheet>
