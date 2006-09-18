<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: article.xsl,v 1.8 2004/01/31 12:00:25 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $												
|														
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:import href="../../docbook.xsl"/>

<xsl:output method="text" encoding="ISO-8859-1" indent="yes"/>
<xsl:variable name="latex.pagestyle">plain</xsl:variable>
<xsl:variable name="latex.document.font">times</xsl:variable>
<xsl:variable name="latex.admonition.path">../../figures</xsl:variable>
<xsl:variable name="ulink.show">0</xsl:variable>

</xsl:stylesheet>
