<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
  <xsl:import href="db2latex-common.xsl"/>
  <xsl:param name="refentry.generate.name" select="1"/>
  <xsl:param name="latex.documentclass">book</xsl:param>
  <xsl:param name="latex.documentclass.book">a4paper,10pt,oneside,openright</xsl:param>
  <xsl:param name="ulink.footnotes">1</xsl:param>
  <xsl:variable name="latex.hyperref.param.pdftex">
    pdfauthor={Christian Schenk}, 
    pdftitle={MiKTeX Manual}
  </xsl:variable>
  <xsl:variable name="latex.book.preamble.pre">
    <xsl:text>\renewcommand*\rmfamily{\sffamily}%&#10;</xsl:text>
    <xsl:text>\renewcommand{\familydefault}{\sfdefault}%&#10;</xsl:text>
  </xsl:variable>
  <xsl:variable name="latex.book.preamble.post">
    <xsl:text>\usepackage{mflogo}%&#10;</xsl:text>
    <xsl:text>\usepackage{lmodern}%&#10;</xsl:text>
    <!-- a better description environment; our labels tend to be very long-->
    <xsl:text>\usepackage{expdlist}%&#10;</xsl:text>
    <!--<xsl:text>\renewcommand{\familydefault}{\sfdefault}%&#10;</xsl:text>-->
    <!-- \raggedbottom because we have lots of screenshots-->
    <xsl:text>\raggedbottom%&#10;</xsl:text>
  </xsl:variable>
<!-- not necessary with HEAD revision of db2latex
  <xsl:template match="index|setindex">
    <xsl:text>\clearpage&#10;</xsl:text>
    <xsl:text>\addcontentsline{toc}{chapter}{Index}&#10;</xsl:text>
    <xsl:text>\docbooktolatexprintindex{</xsl:text>
    <xsl:call-template name="generate.label.id"/>
    <xsl:text>}&#10;</xsl:text>
  </xsl:template>
-->
</xsl:stylesheet>
