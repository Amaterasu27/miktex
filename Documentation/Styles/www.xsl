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

  <xsl:template name="user.header.content">

<script type="text/javascript"><xsl:text>&lt;!--
google_ad_client = "pub-4672682363977476";
/* 728x90, Documentation (Top) */
google_ad_slot = "4224958960";
google_ad_width = 728;
google_ad_height = 90;
//--&gt;</xsl:text>
</script>
<script type="text/javascript"
src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script>

  </xsl:template>

  <xsl:template name="user.footer.content">

<script type="text/javascript"><xsl:text>&lt;!--
google_ad_client = "pub-4672682363977476";
/* 728x90, Documentation (Bottom) */
google_ad_slot = "0448930089";
google_ad_width = 728;
google_ad_height = 90;
//--&gt;</xsl:text>
</script>
<script type="text/javascript"
src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script>

  </xsl:template>
</xsl:stylesheet>
