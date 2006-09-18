<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: defguide.xsl,v 1.2 2003/05/21 06:00:18 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $
|
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:import href="../../docbook.xsl"/>

<xsl:output encoding="UTF-8"/>
<xsl:variable name="latex.inputenc">utf8</xsl:variable>
<xsl:variable name="latex.use.ucs">1</xsl:variable>
<xsl:variable name="latex.fontenc">T1</xsl:variable>
<!--<xsl:variable name="latex.ucs.options">postscript</xsl:variable>-->

<xsl:variable name="latex.use.ltxtable">1</xsl:variable>
<xsl:variable name="latex.admonition.path">../../figures</xsl:variable>
<xsl:variable name="latex.graphics.formats">PDF,PNG</xsl:variable>
<xsl:variable name="latex.mapping.xml" select="document('defguide.map')"/>

<xsl:variable name="ulink.protocols.relaxed">1</xsl:variable>
<xsl:variable name="ulink.show">1</xsl:variable>

<xsl:template match="screen">
   <xsl:text>&#10;\par\noindent\begin{cminipage}</xsl:text>
   <xsl:apply-imports />   
   <xsl:text>\end{cminipage}&#10;</xsl:text>
</xsl:template>

<xsl:variable name="latex.document.font">mathptm</xsl:variable>

<xsl:variable name="latex.book.preamble.pre">
	<xsl:text>\usepackage[scaled=0.9]{helvet}&#10;</xsl:text>
	<xsl:text>\usepackage{courier}&#10;</xsl:text>
</xsl:variable>

<xsl:variable name="latex.book.preamble.post">
	\graphicspath{{./defguide/en/figures/300dpi}{./defguide/en/}}
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
	% Override the default from fancyhdr (which would be to have all-caps headings).
	\makeatletter
	\renewcommand{\chaptermark}[1]{\markboth{%
	 {\ifnum \c@secnumdepth>\m@ne \@chapapp\ \thechapter. \ \fi #1}}{}}
	\renewcommand{\sectionmark}[1]{\markright{%
	 {\ifnum \c@secnumdepth >\z@ \thesection. \ \fi #1}}}
	% Suppress some hyperref warnings
	% From: Heiko Oberdiek oberdiek@uni-freiburg.de
	% Date: Tue, 23 Apr 2002 14:15:49 +0200
	% Newsgroups: comp.text.tex
	% Message-ID: aa3j6e$llb$1@n.ruf.uni-freiburg.de
	\providecommand{\toclevel@example}{0}
	% Grey backgrounds for some verbatim text.
    \newenvironment{cminipage}{\begin{lrbox}{\@tempboxa}\begin{minipage}{\linewidth}}{\end{minipage}\end{lrbox}\colorbox[gray]{0.8}{\usebox{\@tempboxa}}}
	\makeatother
</xsl:variable>

<!-- Adds 10Mb to the PDF and requires increased TeX capacity.
<xsl:template match="sgmltag[(@class='' or @class='element') and (@role='' or @role='docbook')]">
	<xsl:call-template name="generate.hyperlink">
		<xsl:with-param name="target" select="concat(.,'.element')"/>
		<xsl:with-param name="text"><xsl:apply-imports/></xsl:with-param>
	</xsl:call-template>
</xsl:template>
-->

<xsl:template match="simplelist[@role='enum' or @role='notationenum']">
	<xsl:call-template name="generate.simplelist.inline"/>
</xsl:template>

<xsl:template match="copyright">
	<xsl:text>\begin{center}&#10;</xsl:text>
	<xsl:apply-imports/>
	<xsl:text>\end{center}&#10;</xsl:text>
</xsl:template>

<xsl:template match="phrase[@condition='online']|para[@condition='online']">
	<!-- imitate whitespace -->
	<xsl:text>{\unskip}</xsl:text>
</xsl:template>

<xsl:template match="colophon[@condition='online']"/>

<xsl:template match="informaltable" name="informaltable">
	<xsl:choose>
		<xsl:when test="@role='elemsynop'">
			<xsl:text>\begin{quote}&#10;</xsl:text>
			<xsl:apply-templates select=".//row" mode="elemsynop"/>
			<xsl:text>\end{quote}&#10;</xsl:text>
		</xsl:when>
		<xsl:otherwise><xsl:apply-imports/></xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="row" mode="elemsynop">
	<xsl:text>&#10;</xsl:text>
	<xsl:apply-templates select="entry" mode="elemsynop"/>
	<xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="entry" mode="elemsynop">
	<xsl:text> </xsl:text>
	<xsl:apply-templates/>
	<xsl:text> </xsl:text>
</xsl:template>

<xsl:template match="processing-instruction('lb')">
  <xsl:text>\\&#10;</xsl:text>
</xsl:template>

<xsl:template match="imagedata">
	<xsl:choose>
		<xsl:when test="starts-with(@fileref,'http://www.oasis-open.org/docbook/xmlcharent/')">
			<xsl:call-template name="imagedata">
				<xsl:with-param name="filename" select="concat('./defguide/',substring-after(@fileref,'http://www.oasis-open.org/docbook/xmlcharent/'))"/>
			</xsl:call-template>
		</xsl:when>
		<xsl:otherwise>
			<xsl:apply-imports/>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>

