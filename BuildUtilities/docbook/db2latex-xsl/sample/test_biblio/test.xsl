<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: test.xsl,v 1.1 2003/05/22 06:26:47 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $												
|														
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

	<xsl:import href="../../docbook.xsl"/>

	<xsl:variable name="latex.use.parskip" select="'1'"/>
	<xsl:variable name="latex.document.font" select="'courier,helvet,times'"/>

	<xsl:variable name="latex.hyperref.param.pdftex">
		<xsl:text>pdfstartview=FitH,pdfpagemode=Usenone</xsl:text>
	</xsl:variable>

	<!-- cosmetics -->
	<xsl:template match="bibliography">
		<xsl:apply-imports/>
		<xsl:text>\par\hrulefill\clearpage&#10;</xsl:text>
	</xsl:template>

	<!-- suppress labels and anchors, as they will be duplicated throughout test.tex -->
	<xsl:variable name="latex.article.preamble.post">
\makeatletter
\def\@bibitem#1{\item\ignorespaces}
\def\@lbibitem[#1]#2{\item[\@biblabel{#1}\hfill]\ignorespaces}
\def\docbooktolatexbibaux#1#2{}
\def\hypertarget#1{}
\makeatother
	</xsl:variable>

</xsl:stylesheet>
