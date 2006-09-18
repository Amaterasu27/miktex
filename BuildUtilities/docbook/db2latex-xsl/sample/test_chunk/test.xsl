<?xml version='1.0'?>
<!--############################################################################# 
 |	$Id: test.xsl,v 1.2 2003/03/15 19:07:23 rcasellas Exp $
 |- #############################################################################
 |	$Author: rcasellas $												
 |														
 |	This test/sample shows how to add chunking support to DB2LaTeX
 |	reusing Norman Walsh Stylesheets. The only file required from 
 |	Norm's XSL distribution is chunker.xsl
 + ############################################################################## -->


<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/html/chunker.xsl"/>
<xsl:include href="../../docbook.xsl"/>
<xsl:output method="text" encoding="ISO-8859-1" indent="yes" omit-xml-declaration="yes"/>

<xsl:variable name="latex.math.support">0</xsl:variable>
<xsl:variable name="latex.use.babel">0</xsl:variable>
<xsl:variable name="latex.use.isolatin1">0</xsl:variable>
<xsl:variable name="latex.use.hyperref">0</xsl:variable>
<xsl:variable name="latex.use.fancyvrb">0</xsl:variable>
<xsl:variable name="latex.use.fancyhdr">0</xsl:variable>
<xsl:variable name="latex.use.subfigure">0</xsl:variable>
<xsl:variable name="latex.use.times">0</xsl:variable>


<xsl:template match="chapter">
<xsl:text>% -------------------------------------------------------------&#10;</xsl:text>
<xsl:text>% Chapter </xsl:text><xsl:value-of select="title"/><xsl:text>  &#10;</xsl:text>
<xsl:text>% -------------------------------------------------------------&#10;</xsl:text>
<xsl:text>\InputIfFileExists{</xsl:text><xsl:value-of select="@id"/><xsl:text>}</xsl:text>
<xsl:text>{\typeout{Using file </xsl:text><xsl:value-of select="@id"/><xsl:text>.tex}}</xsl:text>
<xsl:text>{\typeout{WARNING file </xsl:text><xsl:value-of select="@id"/>
<xsl:text>.tex NOT FOUND!}}&#10;</xsl:text>
<xsl:variable name="filename">
	<xsl:text>./</xsl:text>
	<xsl:value-of select="./@id"/>
	<xsl:text>.tex</xsl:text>
</xsl:variable>
<xsl:call-template name="write.chunk">
	<xsl:with-param name="filename" select="$filename"/>
	<xsl:with-param name="method" select="'text'"/>
	<xsl:with-param name="encoding" select="'iso-8859-1'"/>
	<xsl:with-param name="indent" select="'yes'"/>
	<xsl:with-param name="omit-xml-declaration" select="'yes'"/>
	<xsl:with-param name="standalone" select="'yes'"/>
	<xsl:with-param name="doctype-public" select="''"/>
	<xsl:with-param name="doctype-system" select="''"/>

    <xsl:with-param name="content">
		<xsl:call-template name="map.begin"/>
		<xsl:apply-templates/>
		<xsl:call-template name="map.end"/>
    </xsl:with-param>
</xsl:call-template>
</xsl:template>
</xsl:stylesheet>
