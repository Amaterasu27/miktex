<?xml version="1.0" encoding="ISO-8859-1"?>
<!--############################################################################# 
|	$Id: test.xsl,v 1.1.1.1 2003/03/14 10:43:00 rcasellas Exp $
|- #############################################################################
|	$Author: rcasellas $												
|														
+ ############################################################################## -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:import href="../../docbook.xsl"/>
  <xsl:output method="text" encoding="ISO-8859-1" indent="yes"/>
  <xsl:variable name="latex.math.support">1</xsl:variable>
  <xsl:variable name="latex.use.babel">0</xsl:variable>
  <xsl:variable name="latex.use.isolatin1">1</xsl:variable>
  <xsl:variable name="latex.use.hyperref">0</xsl:variable>
  <xsl:variable name="latex.use.fancyvrb">0</xsl:variable>
  <xsl:variable name="latex.use.fancyhdr">0</xsl:variable>
  <xsl:variable name="latex.use.subfigure">0</xsl:variable>
  <xsl:variable name="latex.use.times">0</xsl:variable>
  <xsl:variable name="latex.bibwidelabel">000</xsl:variable>
  <xsl:template name="biblioentry.output">
    <xsl:variable name="biblioentry.tag">
      <xsl:choose>
        <xsl:when test="@xreflabel">
          <xsl:value-of select="normalize-space(@xreflabel)"/>
        </xsl:when>
        <xsl:when test="abbrev">
          <xsl:apply-templates select="abbrev" mode="bibliography.mode"/>
        </xsl:when>
        <xsl:when test="@id">
          <xsl:value-of select="normalize-space(@id)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>UNKNOWN</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:text>% -------------- biblioentry 
</xsl:text>
<!--<xsl:text>\bibitem[</xsl:text><xsl:value-of select="$biblioentry.tag"/><xsl:text>]</xsl:text> -->
    <xsl:text>\bibitem</xsl:text>
    <xsl:text>{</xsl:text>
    <xsl:value-of select="$biblioentry.tag"/>
    <xsl:text>}</xsl:text>
    <xsl:apply-templates select="author|authorgroup" mode="bibliography.mode"/>
    <xsl:value-of select="$biblioentry.item.separator"/>
    <xsl:text>\emph{</xsl:text>
    <xsl:apply-templates select="title" mode="bibliography.mode"/>
    <xsl:text>}</xsl:text>
    <xsl:for-each select="child::copyright|child::publisher|child::pubdate|child::pagenums|child::isbn">
      <xsl:value-of select="$biblioentry.item.separator"/>
      <xsl:apply-templates select="." mode="bibliography.mode"/>
    </xsl:for-each>
    <xsl:text>. </xsl:text>
    <xsl:call-template name="label.id"/>
    <xsl:text>&#10;</xsl:text>
  </xsl:template>
</xsl:stylesheet>
