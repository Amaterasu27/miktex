<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
  <xsl:import href="http://db2latex.sourceforge.net/xsl/docbook.xsl"/>
  <xsl:variable name="graphic.default.extension">png</xsl:variable>
  <xsl:param name="latex.use.parskip">1</xsl:param>
  <xsl:param name="latex.admonition.path">/sgml/docbook/db2latex-xsl-0.8pre1/xsl/figures</xsl:param>

  <xsl:template match="code">
    <xsl:call-template name="inline.monoseq"/>
  </xsl:template>

  <!-- typesetting various logos-->
  <xsl:template match="application">
    <xsl:param name="app"><xsl:value-of select="."/></xsl:param>
    <xsl:choose>
      <xsl:when test="$app='BibTeX'"><xsl:text>Bib\TeX{}</xsl:text></xsl:when>
      <xsl:when test="$app='ConTeXt'"><xsl:text>Con\TeX{}t</xsl:text></xsl:when>
      <xsl:when test="$app='LaTeX'"><xsl:text>\LaTeX{}</xsl:text></xsl:when>
      <xsl:when test="$app='METAFONT'"><xsl:text>\MF{}</xsl:text></xsl:when>
      <xsl:when test="$app='METAFONTware'"><xsl:text>\MF{}ware</xsl:text></xsl:when>
      <xsl:when test="$app='MetaPost'"><xsl:text>\MP{}</xsl:text></xsl:when>
      <xsl:when test="$app='MiKTeX'"><xsl:text>MiK\TeX{}</xsl:text></xsl:when>
      <xsl:when test="$app='NTS'"><xsl:text>$\mathcal{N}$\lower.5ex\hbox{$\mathcal{T}$}$\mathcal{S}$</xsl:text></xsl:when>
      <xsl:when test="$app='Omega'"><xsl:text>{$\Omega$}</xsl:text></xsl:when>
      <xsl:when test="$app='TeX'"><xsl:text>\TeX{}</xsl:text></xsl:when>
      <xsl:when test="$app='TeXnicCenter'"><xsl:text>\TeX{}nicCenter</xsl:text></xsl:when>
      <xsl:when test="$app='TeXify'"><xsl:text>\TeX{}ify</xsl:text></xsl:when>
      <xsl:when test="$app='TeXware'"><xsl:text>\TeX{}ware</xsl:text></xsl:when>
      <xsl:when test="$app='e-Omega'"><xsl:text>{$\varepsilon$}-{$\Omega$}</xsl:text></xsl:when>
      <xsl:when test="$app='e-TeX'"><xsl:text>{$\varepsilon$}-\TeX{}</xsl:text></xsl:when>
      <xsl:when test="$app='pdf-e-TeX'"><xsl:text>pdf-{$\varepsilon$}-\TeX{}</xsl:text></xsl:when>
      <xsl:when test="$app='pdfTeX'"><xsl:text>pdf\TeX{}</xsl:text></xsl:when>
      <xsl:when test="$app='pdfLaTeX'"><xsl:text>pdf\LaTeX{}</xsl:text></xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="inline.charseq"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!-- typesetting various character entities; see the DB2LaTeX FAQ-->
  <xsl:template match="symbol">
    <xsl:param name="entity"><xsl:value-of select="."/></xsl:param>
    <xsl:choose>
      <xsl:when test="$entity='&#160;'"><xsl:text>~</xsl:text></xsl:when>
      <xsl:when test="$entity='&#8211;'"><xsl:text>--</xsl:text></xsl:when>
      <xsl:when test="$entity='&#8230;'"><xsl:text>\ldots{}</xsl:text></xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="inline.charseq"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- use \breaklabel to start descriptions on a new line, if the
       label is too long; requires expdlist package-->
  <xsl:template match="variablelist">
    <xsl:apply-templates select="node()[not(self::varlistentry)]"/>
    <xsl:text>&#10;\begin{description}[\breaklabel]&#10;</xsl:text> 
    <xsl:apply-templates select="varlistentry"/>
    <xsl:text>&#10;\end{description}&#10;</xsl:text> 
    <xsl:if test="$latex.use.noindent=1">
      <xsl:text>\noindent </xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:variable name="latex.fontenc">T1</xsl:variable>
  <xsl:variable name="latex.document.font">helvet</xsl:variable>

  <xsl:template name="admon.graphic">
    <xsl:param name="node" select="."/>
    <xsl:choose>
      <xsl:when test="name($node)='note'">note.pdf</xsl:when>
      <xsl:when test="name($node)='warning'">warning.pdf</xsl:when>
      <xsl:when test="name($node)='caution'">caution.pdf</xsl:when>
      <xsl:when test="name($node)='tip'">tip.pdf</xsl:when>
      <xsl:when test="name($node)='important'">important.pdf</xsl:when>
      <xsl:otherwise>note.pdf</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- fixes a bug in the db2latex HEAD revision -->
  <xsl:template match="markup">
    <xsl:call-template name="inline.charseq"/>
  </xsl:template>

  <xsl:template name="latex.float.preamble">
    <xsl:text> \floatstyle{plain}&#10;</xsl:text>
    <xsl:text> \restylefloat{figure}&#10;</xsl:text>
    <xsl:text> \floatstyle{plain}&#10;</xsl:text>
    <xsl:text> \restylefloat{table}&#10;</xsl:text>
    <xsl:text> \floatstyle{ruled}&#10;</xsl:text>
    <xsl:text> \newfloat{program}{ht}{lop}[section]&#10;</xsl:text>
    <xsl:text> \floatstyle{ruled}&#10;</xsl:text>
    <xsl:text> \newfloat{example}{ht}{loe}[section]&#10;</xsl:text>
    <xsl:text> \floatname{example}{</xsl:text>
    <xsl:call-template name="gentext.element.name">
      <xsl:with-param name="element.name">example</xsl:with-param>
    </xsl:call-template>
    <xsl:text>}&#10;</xsl:text>
    <xsl:text> \floatstyle{ruled}&#10;</xsl:text>
    <xsl:text> \newfloat{dbequation}{ht}{loe}[section]&#10;</xsl:text>
    <xsl:text> \floatname{dbequation}{</xsl:text>
    <xsl:call-template name="gentext.element.name">
      <xsl:with-param name="element.name">equation</xsl:with-param>
    </xsl:call-template>
    <xsl:text>}&#10;</xsl:text>
    <xsl:text> \floatstyle{boxed}&#10;</xsl:text>
    <xsl:text> \newfloat{algorithm}{ht}{loa}[section]&#10;</xsl:text>
    <xsl:text> \floatname{algorithm}{Algorithm}&#10;</xsl:text>
  </xsl:template>

</xsl:stylesheet>
