<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: test.xsl,v 1.2 2003/06/29 08:33:07 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $												
|														
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

	<xsl:import href="../../docbook.xsl"/>

	<xsl:variable name="latex.use.varioref" select="'1'"/>
	<xsl:variable name="insert.xref.page.number" select="'1'"/>
	<xsl:variable name="latex.use.parskip" select="'1'"/>
	<xsl:variable name="latex.document.font" select="'courier,helvet,times'"/>

	<xsl:template match="productname">
		<xsl:text>{\sffamily \bfseries </xsl:text>
		<xsl:call-template name="inline.charseq" />
		<xsl:text>}</xsl:text>
	</xsl:template>

	<xsl:template match="processing-instruction('\')">
		<xsl:text>\</xsl:text>
	</xsl:template>

	<xsl:template match="processing-instruction('newpage')">
		<xsl:text>&#10;\clearpage&#10;</xsl:text>
	</xsl:template>

	<xsl:template match="processing-instruction('narrate')">
		<xsl:text><![CDATA[All text should be in the source document's specified language (English).]]></xsl:text>
	</xsl:template>

</xsl:stylesheet>
