#!/bin/zsh



function echoLaTeXPreamble ()
{
 echo "\\\\documentclass[english,a4paper]{article}"
 echo "\\\\usepackage{amsmath,amsthm, amsfonts, amssymb, amsxtra,amsopn}"
 echo "\\\\usepackage{graphicx}"
 echo "\\\\usepackage{a4wide}"
 echo "\\\\usepackage{hyperref}"
 echo "\\\\DeclareGraphicsExtensions{.eps}"
 echo "\\\\usepackage{subfigure}"
 echo "\\\\usepackage{float}"
 echo "\\\\floatstyle{ruled}"
 echo "\\\\oddsidemargin -1.5 cm"
 echo "\\\\evensidemargin -1.5 cm"
 echo "\\\\textwidth 20 cm"
 echo "\\\\topmargin -1 cm"
 echo "\\\\textheight 24 cm"
 echo "\\\\title{HandOut 2}"
 echo "\\\\author{Ramon Casellas}"
 echo "\\\\begin{document}"
 echo "\\\\clearpage"
}
function echoEndDocument ()
{
 echo "\\\\end{document}"
}

function echoFigures ()
{
for i in thumbs/thumb*.eps ; do
 echo "% figure ------------------------------------------------------"
 echo "\\\\begin{center}"
 echo "\\\\framebox{"
 echo "  \\\\includegraphics[width=.7\linewidth]{thumbs/`basename $i .pdf`}"
 echo "}"
 echo "\\\\end{center}"
done
}


function generateThumbs ()
{
	echo "###############################################"
	echo " Generating EPS thumbnails "
	echo "###############################################"
	pages=`pdfinfo slides.pdf| grep Pages | sed -e "s/Pages:        //"`
	mkdir -p thumbs
	# pdf2ps slides.pdf thumbs/slides.ps
	echo  "$pages" pags.
	i=0
	gs -dNOPAUSE -q -dBATCH -sDEVICE=epswrite -sPAPERSIZE=a5 -sOutputFile=thumbs/thumbs.eps slides.pdf
	while [ "$i" -lt "$pages" ]
    	do	
      		i=`expr $i + 1`
		if [ "$i" -lt 10 ]
		then
			psselect $i thumbs/thumbs.eps > thumbs/thumb00$i.eps
#			ps2pdf13 thumbs/thumb00$i.eps > thumbs/thumb00$i.pdf 
		elif [ "$i" -lt 100 ]
		then	
			psselect $i thumbs/thumbs.eps > thumbs/thumb0$i.eps
#			ps2pdf13 thumbs/thumb0$i.eps > thumbs/thumb0$i.pdf 
		else
			psselect $i thumbs/thumbs.eps > thumbs/thumb$i.eps
#			ps2pdf13 thumbs/thumb$i.eps > thumbs/thumb$i.pdf 
		fi
	done
	rm -rf thumbs/thumbs.eps
	return 1
}




##########################################
# Make sure the pdf file is up to date
make

##########################################
# Generate Thumbnails in thumbs dir 
generateThumbs


##########################################
# Generate HandOut2.tex 
#cat << EOF > HandOut2.tex 
#echoLaTeXPreamble 
#echoFigures
#echoEndDocument
#EOF
echoLaTeXPreamble >> HandOut2.tex
echoFigures >> HandOut2.tex
echoEndDocument >> HandOut2.tex


##########################################
# Compile 
latex -interaction=nonstopmode HandOut2.tex
latex -interaction=nonstopmode HandOut2.tex
latex -interaction=nonstopmode HandOut2.tex
dvipdf HandOut2.dvi HandOut2.pdf
xpdf HandOut2.pdf

##########################################
# Clean 
make clean

exit 0
