<?xml version='1.0'?>
<!--############################################################################# 
|	$Id: babel.xsl,v 1.1 2003/05/20 03:52:22 j-devenish Exp $
|- #############################################################################
|	$Author: j-devenish $												
|														
+ ############################################################################## -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

	<xsl:import href="./test.xsl"/>

	<xsl:variable name="latex.babel.language" select="'french'"/>

	<xsl:template match="processing-instruction('narrate')">
	<xsl:message>X</xsl:message>
		<xsl:text><![CDATA[\textsf{Varioref} and \textsf{Babel} portions should be in the \texttt{\$latex.\penalty0babel.language} (French)
		while ``gentext'' portions will be in the document's specified language (English).]]></xsl:text>
	</xsl:template>

</xsl:stylesheet>
