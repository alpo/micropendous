XSLTPROC=xsltproc
HTML_STYLE_SHEET=meta/stylesheet-html.xsl
FO_STYLE_SHEET=meta/stylesheet-fo.xsl

target=userial
outdir=out
cssfile=style.css
images=$(wildcard images/*.svg) $(wildcard images/*.png)
htmlimgs=$(addprefix $(outdir)/,$(notdir $(images:.svg=.png)))
pdfimgs=$(outdir)/schematic_ls.jpg $(outdir)/board.jpg

all: html
pdf: $(outdir)/$(target).pdf
html: $(outdir)/$(target).html $(outdir)/$(cssfile) $(htmlimgs)

$(outdir)/$(target).html: $(target).xml $(HTML_STYLE_SHEET) $(wildcard *.xml) $(outdir)
	$(XSLTPROC) -o $@ $(HTML_STYLE_SHEET) $(firstword $^)

$(target).fo: $(target).xml $(FO_STYLE_SHEET) $(wildcard *.xml)
	$(XSLTPROC) -o $@ $(FO_STYLE_SHEET) $(firstword $^)

$(outdir)/$(target).pdf: $(target).fo $(pdfimgs) $(outdir)
	fop -fo $(firstword $^) -pdf $@

$(outdir)/%.png: images/%.svg
	gm convert $^ $@

$(outdir)/%.jpg: images/%.png
	gm convert $^ $@

$(outdir)/%_ls.jpg: images/%.png
	gm convert $^ -rotate -90 $@

$(outdir)/%.png: images/%.png
	cp $^ $@

$(outdir)/%.css: meta/%.css
	cp $^ $@

$(outdir):
	mkdir $@

.PHONY: clean
clean:
	$(RM) -rf $(outdir) $(target).fo

.PHONY: check
check:
	xmllint --valid --noout $(target).xml

.PHONY: spellcheck
spellcheck:
	aspell -c -H $(wildcard chap_*.xml)

.INTERMEDIATE: $(pdfimgs) $(target).fo
