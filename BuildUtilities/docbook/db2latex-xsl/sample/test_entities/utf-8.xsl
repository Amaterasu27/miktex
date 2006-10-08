<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: utf-8.xsl,v 1.2 2003/05/21 06:02:48 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $												
|														
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:import href="./test.xsl"/>
<xsl:output encoding="UTF-8"/>
<xsl:variable name="latex.inputenc">utf8</xsl:variable>
<xsl:variable name="latex.use.ucs">1</xsl:variable>
<xsl:variable name="latex.ucs.options">postscript</xsl:variable>

</xsl:stylesheet>
