<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
  <xsl:import href="db2latex-common.xsl"/>
  <xsl:param name="latex.documentclass">article</xsl:param>
  <xsl:param name="latex.documentclass.article">a4paper,onecolumn</xsl:param>
  <xsl:param name="ulink.footnotes">1</xsl:param>
  <xsl:variable name="latex.hyperref.param.pdftex">
    pdfauthor={Christian Schenk}, 
    pdftitle={MiKTeX Installation Instructions}
  </xsl:variable>
  <xsl:variable name="latex.article.preamble.pre">
    <xsl:text>\renewcommand*\rmfamily{\sffamily}%&#10;</xsl:text>
    <xsl:text>\renewcommand{\familydefault}{\sfdefault}%&#10;</xsl:text>
  </xsl:variable>
  <xsl:variable name="latex.article.preamble.post">
    <xsl:text>\usepackage{mflogo}%&#10;</xsl:text>
    <xsl:text>\usepackage{lmodern}%&#10;</xsl:text>
    <!-- a better description environment; our labels tend to be very
    long-->
    <xsl:text>\usepackage{expdlist}%&#10;</xsl:text>
    <!--<xsl:text>\renewcommand{\familydefault}{\sfdefault}%&#10;</xsl:text>-->
    <!-- \raggedbottom because we have lots of screenshots-->
    <xsl:text>\raggedbottom%&#10;</xsl:text>
  </xsl:variable>
</xsl:stylesheet>
