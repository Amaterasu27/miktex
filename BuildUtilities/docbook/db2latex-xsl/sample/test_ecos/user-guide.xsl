<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: user-guide.xsl,v 1.5 2004/01/11 11:39:57 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $
|
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

	<xsl:import href="../docbook.xsl"/>

	<!-- try to cope with UTF 8 -->
	<xsl:output encoding="UTF-8"/>
	<xsl:variable name="latex.inputenc">utf8</xsl:variable>
	<xsl:variable name="latex.use.ucs">1</xsl:variable>

	<xsl:param name="l10n.gentext.default.language" select="'en-US'"/>

	<xsl:param name="latex.document.font">palatino</xsl:param>

	<xsl:param name="my.hyperref.param.pdftex.extra">,pdftitle={test-ecos},linkcolor=blue,pagecolor=blue</xsl:param>

	<!-- hide the inner-side portion of each header  -->
	<xsl:param name="latex.fancyhdr.truncation.partition">0</xsl:param>

	<!-- this is only needed because of our ../docbook.xsl -->
	<xsl:variable name="latex.use.parskip">1</xsl:variable>
	<xsl:variable name="latex.figure.title.style">\textit</xsl:variable>
	<!--
	<xsl:variable name="latex.use.ltxtable">1</xsl:variable>
	-->

	<xsl:template name="latex.fancyvrb.options">
		<xsl:text>,fontsize=\small</xsl:text>
	</xsl:template>

	<!-- original document is SGML and does not include <toc/> -->
	<xsl:template match="bookinfo">
		<xsl:apply-imports/>
		<xsl:text>&#10;</xsl:text>
		<xsl:text>\tableofcontents&#10;</xsl:text>
		<xsl:text>\listoffigures&#10;</xsl:text>
		<xsl:text>\listoftables&#10;</xsl:text>
	</xsl:template>

	<xsl:template match="note">
		<xsl:text>&#10;</xsl:text>
		<xsl:text>\paragraph*{</xsl:text>
		<xsl:choose>
			<xsl:when test="title">
				<xsl:apply-templates select="title"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="gentext.element.name"/>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>:} </xsl:text>
		<xsl:call-template name="content-templates"/>
		<xsl:text>&#10;</xsl:text>
	</xsl:template>

	<!-- grey background shading -->
	<xsl:template match="programlisting">
	   <xsl:text>&#10;\par\noindent\begin{cminipage}</xsl:text>
	   <xsl:apply-imports />   
	   <xsl:text>\end{cminipage}\par&#10;</xsl:text>
	</xsl:template>

	<xsl:variable name="latex.book.preamble.post">
		\setlength{\arrayrulewidth}{1pt}
		\makeatletter
		\providecommand{\toclevel@example}{0}
		% Grey backgrounds for some verbatim text.
		\IfFileExists{framed.sty}{
			\newenvironment{cminipage}{\begin{shaded}}{\end{shaded}}
		  \usepackage{color,framed}
		  \definecolor{shadecolor}{gray}{0.8}
		}{
			\newenvironment{cminipage}{\begin{lrbox}{\@tempboxa}\begin{minipage}{\linewidth}}{\end{minipage}\end{lrbox}\noindent\colorbox[gray]{0.8}{\usebox{\@tempboxa}}}
		}
		\makeatother
		\floatstyle{plain}
		\restylefloat{figure}
		\restylefloat{table}
	</xsl:variable>

</xsl:stylesheet>

