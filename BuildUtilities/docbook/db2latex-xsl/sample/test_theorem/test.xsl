<?xml version='1.0'?>
<!--############################################################################# 
 |	$Id: test.xsl,v 1.4 2003/04/21 09:12:26 j-devenish Exp $
 |- #############################################################################
 |	$Author: j-devenish $												
 |														
 + ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:import href="../../docbook.xsl"/>
<xsl:output method="text" encoding="ISO-8859-1" indent="yes"/>

<xsl:variable name="latex.bibfiles">./citations</xsl:variable>
<xsl:variable name="latex.documentclass.book">a4paper,12pt</xsl:variable>
<xsl:variable name="latex.book.preamble.post">
<xsl:text><![CDATA[
\bibliographystyle{ieeetr}
\theoremstyle{plain}
\newtheorem{thm}{Thérème}[section]
\theoremstyle{definition}
\newtheorem{defn}{Définition}[section]
]]></xsl:text>
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




</xsl:stylesheet>
