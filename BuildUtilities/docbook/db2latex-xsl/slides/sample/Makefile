all: slides
	latex -interaction=nonstopmode slides.tex
	latex -interaction=nonstopmode slides.tex
	latex -interaction=nonstopmode slides.tex
	dvipdf slides.dvi slides.pdf
	xpdf slides.pdf
	make clean
slides: 
	xsltproc slides.xsl slides.xml > slides.tex

clean:
	rm -rf  *.log *.out *.brf *.aux *.glo *.idx *.toc *.dvi
	rm -rf thumbs
	rm -rf HandOut2.tex 
