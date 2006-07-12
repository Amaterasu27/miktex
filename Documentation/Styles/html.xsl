<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

  <xsl:import href="http://docbook.sourceforge.net/release/xsl/current/html/docbook.xsl"/>
  
  <xsl:param name="admon.graphics" select="1"/>
  <xsl:param name="graphic.default.extension">png</xsl:param>
  <xsl:param name="html.stylesheet">miktexman.css</xsl:param>
  <xsl:param name="navig.graphics" select="1"/>
  <xsl:param name="navig.graphics.extension" select="'.png'"/>

  <xsl:param name="local.l10n.xml" select="document('')"/>
  <l:i18n xmlns:l="http://docbook.sourceforge.net/xmlns/l10n/1.0">
    <l:l10n language="en">
      <l:context name="title">
        <l:template name="table" text="%t"/>
      </l:context>
    </l:l10n>
  </l:i18n>

<!--
  <xsl:template name="user.footer.content">
    <hr/>
    <table width="100%">
      <tr>
        <td align="right">the MiKTeX project is hosted by
          <a href="http://sourceforge.net">
            <img src="http://sourceforge.net/sflogo.php?group_id=10783&amp;type=1"
                 width="88"
                 height="31"
                 border="0"
                 align="absbottom"
		 alt="SourceForge Logo"/>
          </a>
        </td>
      </tr>
    </table>
  </xsl:template>
-->

</xsl:stylesheet>
