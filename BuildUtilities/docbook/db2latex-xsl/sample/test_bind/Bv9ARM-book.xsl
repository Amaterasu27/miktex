<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: Bv9ARM-book.xsl,v 1.2 2003/05/23 11:45:19 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $
|
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:import href="../../docbook.xsl"/>

<xsl:variable name="latex.entities">catcode</xsl:variable>
<xsl:variable name="latex.fontenc">T1</xsl:variable>

<xsl:param name="l10n.gentext.default.language" select="'en-US'"/>

<xsl:variable name="latex.document.font">mathptm</xsl:variable>
<xsl:variable name="latex.admonition.path">../../figures</xsl:variable>
<xsl:variable name="latex.graphics.formats">PDF,PNG</xsl:variable>
<xsl:variable name="latex.use.parskip">1</xsl:variable>
<xsl:variable name="latex.use.ltxtable">1</xsl:variable>

<xsl:template name="latex.fancyvrb.options">
	<xsl:text>,fontsize=\small</xsl:text>
</xsl:template>

<xsl:template match="copyright">
	<xsl:param name="emit" select="false()"/>
	<xsl:if test="$emit"><xsl:apply-imports/></xsl:if>
</xsl:template>

<xsl:variable name="latex.book.preamble.pre">
	<xsl:text>\usepackage[scaled=0.9]{helvet}&#10;</xsl:text>
	<xsl:text>\usepackage{courier}&#10;</xsl:text>
</xsl:variable>

<xsl:variable name="latex.book.preamble.post">
	\setlength{\arrayrulewidth}{1pt}
	\IfFileExists{truncate.sty}{
		% Safeguard against long headers.
		\usepackage{truncate}
		% Use an ellipsis when text would be larger than 49% of the text width.
		% Preserve left/right text alignment using \hfill (works for English).
		\fancyhead[ol]{\truncate{0.49\textwidth}{\sl\leftmark}}
		\fancyhead[or]{\truncate{0.49\textwidth}{\hfill\sl\rightmark}}
		% Use an ellipsis when text would be larger than 49% of the text width
		% Preserve left/right text alignment using \hfill (works for English).
		\fancyhead[el]{\truncate{0.49\textwidth}{\sl\leftmark}}
		\fancyhead[er]{\truncate{0.49\textwidth}{\hfill\sl\rightmark}}
	}{}
	\AtBeginDocument{
		\let\oldmaketitle\maketitle
		\def\maketitle{\author{<xsl:apply-templates select="/book/bookinfo/copyright"><xsl:with-param name="emit" select="true()"/></xsl:apply-templates>}\oldmaketitle}
	}
</xsl:variable>

</xsl:stylesheet>

