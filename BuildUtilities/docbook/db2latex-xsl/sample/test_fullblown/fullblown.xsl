<?xml version="1.0" encoding="ISO-8859-1"?>
<!--############################################################################# 
 |	$Id: fullblown.xsl,v 1.2 2003/04/25 08:38:24 j-devenish Exp $
 |- #############################################################################
 |	$Author: j-devenish $												
 |														
 + ############################################################################## -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:import href="../../docbook.xsl"/>
<xsl:output method="text" encoding="ISO-8859-1" indent="yes"/>
<xsl:variable name="latex.bibwidelabel">000</xsl:variable>
<xsl:variable name="latex.documentclass">rcthesis</xsl:variable>
<xsl:variable name="latex.documentclass.common">french,spanish,english,final,titlepage,smallheadings</xsl:variable>
<xsl:variable name="latex.hyperref.param.pdftex">hyperfigures,hyperindex,citecolor=blue,urlcolor=blue,pdftitle={Partage de Charge et Ing. de Trafic},pdfauthor={Ramon Casellas},pdfstartview=FitV</xsl:variable>
<xsl:variable name="latex.document.font">default</xsl:variable>
<xsl:variable name="latex.titlepage.file">tex/title</xsl:variable>
<xsl:variable name="latex.admonition.path">../../figures</xsl:variable>

<xsl:variable name="latex.book.preamble.post">
\usepackage{xspace}
\theoremstyle{plain}
\newtheorem{thm}[section]{Théorème}
\newtheorem{cor}[section]{Corollaire}
\newtheorem{lem}[section]{Lemme}
\newtheorem{prop}[section]{Proposition}
\newtheorem{ax}[section]{Axiome}

\theoremstyle{remark}
\newtheorem{rem}[section]{Remarque}
\newtheorem{exm}[section]{Exemple}
\newtheorem{notation}[section]{Notation}

\theoremstyle{definition}
\newtheorem{defn}[section]{Définition}

\newcommand{\ntt}{\normalfont\ttfamily}
\newcommand{\thmref}[1]{Théorème~\ref{#1}}
\newcommand{\secref}[1]{\S\ref{#1}}
\newcommand{\lemref}[1]{Lemme~\ref{#1}}

\DeclareMathOperator{\essinf}{ess\,inf}
\DeclareMathOperator{\avg}{avg}

\renewcommand{\chaptermark}[1]{\markboth{\thechapter.\ #1}{}}
\renewcommand{\sectionmark}[1]{\markright{\thesection.\ #1}}

\newcommand{\indic}{\mathtt{1}\!\!\mathtt{l}}
\newcommand{\proba}{\mathbb{P}}
\newcommand{\esper}{\mathbb{E}}
\newcommand{\Nats}{I\!\!N}
\newcommand{\nats}{I\!\!N}
\newcommand{\Reals}{I\!\!R}
\newcommand{\reals}{I\!\!R}
\newcommand{\espalm}{\mathbb{E}_N^{o}}
 % --------------------------------------------
 \newcommand{\xmin}{x_{\min}}
 \newcommand{\xmax}{x_{\max}}
 \newcommand{\cmin}{c_{\min}}
 \newcommand{\cmax}{c_{\max}}
 \newcommand{\exptx}{e^{\theta x[0,t)}}
 \newcommand{\expsx}{e^{s x[0,t)}}
 \newcommand{\loglsx}{\log \esper[e^{s x[0,t)}]}
 \newcommand{\logltx}{\log \esper[e^{s x[0,t)}]}
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The next line sets 1.5 spacing
\renewcommand{\baselinestretch}{1.5}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% No indent and small paragraph sep.
\setlength{\parindent}{0mm} 
\addtolength{\parskip}{1mm}

\setcounter{totalnumber}{50}
\setcounter{topnumber}{50}
\setcounter{bottomnumber}{50}
</xsl:variable>


<xsl:template match="mathelement/mathremark">
<xsl:text>\begin{rem}[</xsl:text>
<xsl:value-of select="title"/>
<xsl:text>]&#10;</xsl:text>
<xsl:variable name="id"> <xsl:call-template name="label.id"/> </xsl:variable>
<xsl:apply-templates/>
<xsl:text>\end{rem}&#10;</xsl:text>
</xsl:template>

<xsl:template match="mathelement/mathexample">
<xsl:text>\begin{exm}[</xsl:text>
<xsl:value-of select="title"/>
<xsl:text>]&#10;</xsl:text>
<xsl:variable name="id"> <xsl:call-template name="label.id"/> </xsl:variable>
<xsl:apply-templates/>
<xsl:text>\end{exm}&#10;</xsl:text>
</xsl:template>


<xsl:template match="mathelement/mathproposition">
<xsl:text>\begin{prop}[</xsl:text>
<xsl:value-of select="title"/>
<xsl:text>]&#10;</xsl:text>
<xsl:variable name="id"> <xsl:call-template name="label.id"/> </xsl:variable>
<xsl:apply-templates/>
<xsl:text>\end{prop}&#10;</xsl:text>
</xsl:template>


<!--
##########################################
#
#  \begin{thm}[title]
#
#  \end{thm}
#
##########################################
-->
<xsl:template match="mathelement/maththeorem">
<xsl:text>\begin{thm}[</xsl:text>
<xsl:value-of select="title"/>
<xsl:text>]&#10;</xsl:text>
<xsl:variable name="id"> <xsl:call-template name="label.id"/> </xsl:variable>
<xsl:apply-templates/>
<xsl:text>\end{thm}&#10;</xsl:text>
</xsl:template>



<!--
##########################################
#
#  \begin{defn}[definition title]
#
#  \end{defn}
#
##########################################
-->
<xsl:template match="mathelement/mathdefinition">
<xsl:text>\begin{defn}[</xsl:text>
<xsl:value-of select="title"/>
<xsl:text>]&#10;</xsl:text>
<xsl:variable name="id"> <xsl:call-template name="label.id"/> </xsl:variable>
<xsl:apply-templates/>
<xsl:text>\end{defn}&#10;</xsl:text>
</xsl:template>

<!--
##########################################
#
#  \begin{lem}[lemma title]
#
#  \end{lem}
#
##########################################
-->
<xsl:template match="mathelement/mathlemma">
    <xsl:text>\begin{lem}[</xsl:text>
    <xsl:value-of select="title"/>
<!--
<xsl:call-template name="normalize-scape">
	<xsl:with-param name="string" select="title"/> 
    </xsl:call-template>
    -->
<xsl:text>]&#10;</xsl:text>
<xsl:variable name="id"> <xsl:call-template name="label.id"/> </xsl:variable>
<xsl:apply-templates/>
<xsl:text>\end{lem}&#10;</xsl:text>
</xsl:template>


<!--
##########################################
#
#  \begin{proof}
#
#  \end{proof}
#
##########################################
-->
<xsl:template match="mathproof">
<xsl:text>\begin{proof}</xsl:text>
<xsl:variable name="id"> <xsl:call-template name="label.id"/> </xsl:variable>
<xsl:apply-templates/>
<xsl:text>\end{proof}&#10;</xsl:text>
</xsl:template>



<xsl:template match="formalpara/title"/>
<xsl:template match="formalpara">
<xsl:call-template name="map.begin"/>
<xsl:apply-templates/>
</xsl:template>


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
<xsl:text>&#10;</xsl:text>
<xsl:text>% -------------- biblioentry &#10;</xsl:text>
<!--<xsl:text>\bibitem[</xsl:text><xsl:value-of select="$biblioentry.tag"/><xsl:text>]</xsl:text> -->
<xsl:text>\bibitem</xsl:text>
<xsl:text>{</xsl:text><xsl:value-of select="$biblioentry.tag"/><xsl:text>}&#10;</xsl:text> 
<xsl:apply-templates select="author|authorgroup" mode="bibliography.mode"/>
<xsl:value-of select="$biblioentry.item.separator"/>
<xsl:text>\emph{</xsl:text> <xsl:apply-templates select="title" mode="bibliography.mode"/><xsl:text>}</xsl:text>
<xsl:for-each select="child::copyright|child::publisher|child::pubdate|child::pagenums|child::isbn">
<xsl:value-of select="$biblioentry.item.separator"/>
<xsl:apply-templates select="." mode="bibliography.mode"/> 
</xsl:for-each>
<xsl:text>. </xsl:text>
<xsl:call-template name="label.id"/> 
<xsl:text>&#10;&#10;</xsl:text>
</xsl:template>
</xsl:stylesheet>

