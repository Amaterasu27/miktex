<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: test.xsl,v 1.7 2003/05/21 07:17:39 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $												
|														
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:import href="../../docbook.xsl"/>

<xsl:variable name="latex.override">
<xsl:text>\documentclass[pdftex,a4paper,10pt]{article}&#10;</xsl:text>
<xsl:text>\usepackage[pdftex,bookmarksnumbered,colorlinks,backref, bookmarks, breaklinks, linktocpage,pdfstartview=FitH,pdfpagemode=UseNone]{hyperref}&#10;</xsl:text>
<xsl:text>\usepackage[pdftex]{graphicx}&#10;</xsl:text>
<xsl:text>\usepackage[latin1]{inputenc}&#10;</xsl:text>
<xsl:text>\usepackage{times}&#10;</xsl:text>
<xsl:text>\usepackage{varioref}&#10;</xsl:text>
<xsl:text>\setlength{\textwidth}{11cm}&#10;</xsl:text>
<xsl:text>\pdfcompresslevel=9&#10;</xsl:text>
<xsl:text><![CDATA[% In any other LaTeX context, this would probably go into a style file.
\newcommand{\docbookhyphenateurl}[1]{{\hyphenchar\font=`\/\relax #1\hyphenchar\font=`\-}}
\newcommand{\docbookhyphenatedot}[1]{{\hyphenchar\font=`.\relax #1\hyphenchar\font=`\-}}
]]></xsl:text>
</xsl:variable>

<xsl:variable name="latex.use.varioref">0</xsl:variable>

<xsl:variable name="phrase.suppressed">pageref</xsl:variable>

<xsl:template match="phrase">
	<xsl:if test="@role!=$phrase.suppressed">
		<xsl:apply-imports/>
	</xsl:if>
</xsl:template>

</xsl:stylesheet>
