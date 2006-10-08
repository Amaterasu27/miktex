<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: test.xsl,v 1.4 2003/06/29 08:32:33 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $												
|														
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

	<xsl:import href="../../docbook.xsl"/>

	<xsl:variable name="latex.use.parskip" select="'1'"/>
	<xsl:variable name="latex.use.hyperref" select="'0'"/>
	<xsl:variable name="latex.document.font" select="'courier,helvet,times'"/>

	<xsl:variable name="latex.use.makeidx" select="'1'"/>
	<xsl:variable name="latex.article.preamble.post">
		<xsl:text>\makeindex&#10;</xsl:text>
	</xsl:variable>

	<xsl:template match="processing-instruction('newpage')">
		<xsl:text>&#10;\clearpage&#10;</xsl:text>
	</xsl:template>

</xsl:stylesheet>
