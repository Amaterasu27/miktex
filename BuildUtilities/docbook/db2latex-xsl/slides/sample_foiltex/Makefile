all: slides
	pdflatex -interaction=nonstopmode slides.tex
	pdflatex -interaction=nonstopmode slides.tex
	pdflatex -interaction=nonstopmode slides.tex
	acroread slides.pdf
	make clean
slides: 
	xsltproc slides.xsl slides.xml > slides.tex

clean:
	rm -rf  *.log *.out *.brf *.aux *.glo *.idx *.toc *.dvi
