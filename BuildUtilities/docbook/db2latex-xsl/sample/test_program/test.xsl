<?xml version='1.0'?>
<!--############################################################################# 
 |	$Id: test.xsl,v 1.4 2004/01/26 09:55:26 j-devenish Exp $
 |- #############################################################################
 |	$Author: j-devenish $												
 |														
 + ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

	<xsl:import href="../docbook.xsl"/>

	<xsl:variable name="latex.documentclass.article">a4paper,10pt</xsl:variable>
	<xsl:variable name="my.hyperref.param.pdftex.extra">,pdftitle={test-program}</xsl:variable>

	<xsl:variable name="latex.fancyvrb.tabsize">4</xsl:variable>

	<xsl:variable name="latex.use.hyperref">0</xsl:variable>
	<xsl:variable name="latex.use.fancyhdr">0</xsl:variable>

</xsl:stylesheet>
