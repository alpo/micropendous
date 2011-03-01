<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:import href="/usr/share/xml/docbook/stylesheet/nwalsh/fo/docbook.xsl"/>

<xsl:param name="chapter.autolabel"     select="1"/>
<xsl:param name="section.autolabel"     select="0"/>
<xsl:param name="appendix.autolabel"    select="1"/>
<xsl:param name="use.id.as.filename"    select="1"/>
<xsl:param name="generate.toc"          select="'book toc'"/>
<xsl:param name="paper.type"            select="'A4'"/>
<!--
<xsl:param name="xref.with.number.and.title"    select="0"/>
-->

<!-- FO specific options -->
<xsl:param name="shade.verbatim"        select="1"/>
<xsl:param name="fop1.extensions"       select="1"/>
<xsl:param name="draft.watermark.image" select="/usr/share/doc/docbook-xsl/doc/images/draft.png"/>
<!--
<xsl:param name="double.sided"          select="1"/>
-->
<xsl:attribute-set name="monospace.verbatim.properties">
    <xsl:attribute name="font-size">9pt</xsl:attribute>
</xsl:attribute-set>

<!--
<xsl:param name="" select=""/>
-->
</xsl:stylesheet>
