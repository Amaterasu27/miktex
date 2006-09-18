<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: l10n.xsl,v 1.2 2003/05/20 03:56:16 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $												
|														
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

	<xsl:import href="./test.xsl"/>

	<!--
		NOTE:
		You should usually use l10n.gentext.default.language!
		l10n.gentext.language is used in this file because the
		document already specifies its own language and we want
		to override it.
	-->
	<xsl:param name="l10n.gentext.language" select="'fr'"/>

	<xsl:template match="processing-instruction('narrate')">
		<xsl:text><![CDATA[Body text (other than this) should be in the \texttt{\$l10n.gentext.language} (French).]]></xsl:text>
	</xsl:template>

</xsl:stylesheet>
