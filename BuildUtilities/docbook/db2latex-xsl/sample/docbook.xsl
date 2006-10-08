<?xml version='1.0'?>
<!--############################################################################# 
 |	$Id: docbook.xsl,v 1.3 2004/01/26 11:12:50 j-devenish Exp $
 |- #############################################################################
 |	$Author: j-devenish $
 |	PURPOSE: This customisation layer defines some useful habits for all sample
 |	files.
 + ############################################################################## -->
<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:doc="http://nwalsh.com/xsl/documentation/1.0"
	exclude-result-prefixes="doc" version='1.0'>

	<xsl:import href="../docbook.xsl"/>

	<!--
	<xsl:output method="text" encoding="ISO-8859-1" indent="yes"/>
	<xsl:variable name="latex.inputenc">latin1</xsl:variable>
	-->

	<xsl:param name="admon.graphics.path">../../figures</xsl:param>
	<xsl:param name="latex.graphics.formats">PDF,PNG</xsl:param>

	<xsl:param name="latex.document.font">mathptm,courier</xsl:param>
	<xsl:param name="my.hyperref.param.pdftex.extra" select="''"/>
	<xsl:param name="latex.hyperref.param.pdftex">
		<xsl:text>pdfpagelabels,pdfstartview=FitH,pdfpagemode=None,pdfsubject={http://db2latex.sourceforge.net}</xsl:text>
		<xsl:value-of select="$my.hyperref.param.pdftex.extra"/>
	</xsl:param>

	<xsl:param name="my.common.preamble"><![CDATA[
\makeatletter
\def\Hy@Warning#1{}
\def\Hy@WarningNoLine#1{}
\makeatother
]]></xsl:param>
	<xsl:param name="latex.article.preamble.post" select="$my.common.preamble"/>
	<xsl:param name="latex.book.preamble.post" select="$my.common.preamble"/>

	<doc:template xmlns="">
		<refpurpose> Generate a stable cross-reference id </refpurpose>
		<doc:description>
			<para>
				Due to the instability of the XSLT <function>generate-id()</function>
				function, this template instead bases IDs upon the position of the
				source node within the source document. This makes <quote>diffing</quote>
				easier.
			</para>
		</doc:description>
	</doc:template>
	<xsl:template name="generate.label.id">
		<xsl:param name="object" select="."/>
		<xsl:variable name="id">
			<xsl:choose>
				<xsl:when test="$object/@id">
					<xsl:value-of select="$object/@id"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="local-name($object)"/>
					<xsl:for-each select="$object">
						<xsl:number level="any"/>
					</xsl:for-each>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:value-of select="normalize-space($id)"/>
	</xsl:template>

<!--
   <xsl:template match="bibliography">
      <xsl:text>\begin{singlespace}</xsl:text>
      <xsl:apply-imports/>
      <xsl:text>\end{singlespace}</xsl:text>
   </xsl:template>
-->

   <xsl:template match="processing-instruction('newpage')">
      <xsl:text>&#10;\clearpage&#10;</xsl:text>
   </xsl:template>

</xsl:stylesheet>

