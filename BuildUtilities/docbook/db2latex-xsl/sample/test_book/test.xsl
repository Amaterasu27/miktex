<?xml version='1.0'?>
<!--############################################################################# 
 |	$Id: test.xsl,v 1.4 2004/01/05 06:09:57 j-devenish Exp $
 |- #############################################################################
 |	$Author: j-devenish $												
 |														
 + ############################################################################## -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:import href="../docbook.xsl"/>

<xsl:output method="text" encoding="ISO-8859-1" indent="yes"/>
<xsl:variable name="latex.inputenc">latin1</xsl:variable>

<xsl:variable name="latex.biblio.output">all</xsl:variable>
<xsl:variable name="latex.document.font">default</xsl:variable>

<!--
<xsl:variable name="insert.xref.page.number">1</xsl:variable>
<xsl:variable name="latex.use.varioref">0</xsl:variable>

<xsl:variable name="latex.use.babel">1</xsl:variable>
<xsl:variable name="latex.use.hyperref">1</xsl:variable>
<xsl:variable name="latex.use.fancyvrb">1</xsl:variable>
<xsl:variable name="latex.use.fancybox">1</xsl:variable>
<xsl:variable name="latex.use.fancyhdr">1</xsl:variable>
<xsl:variable name="latex.use.subfigure">1</xsl:variable>
<xsl:variable name="latex.use.rotating">1</xsl:variable>
<xsl:variable name="latex.use.makeidx">1</xsl:variable>
<xsl:variable name="latex.pdf.support">1</xsl:variable>
<xsl:variable name="latex.math.support">1</xsl:variable>

<xsl:variable name="latex.admonition.path">../../figures</xsl:variable>
-->

<!--
<xsl:variable name="latex.book.preamblestart">
<xsl:text>% This is an example 					&#10;</xsl:text>
<xsl:text>% Adding a customization Layer			&#10;</xsl:text>
<xsl:text>% Autogenerated LaTeX file for books 			&#10;</xsl:text>
<xsl:text>\documentclass[english,a4paper,10pt,final]{report}&#10;</xsl:text>
<xsl:text>\documentclass[spanish,french,english,a4paper,10pt,final]{report}&#10;</xsl:text>
<xsl:text>\usepackage{longtable}&#10;</xsl:text>
</xsl:variable>
-->


</xsl:stylesheet>
