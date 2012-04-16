file = paper
file2 = myart

all:
	pdflatex $(file).tex
	bibtex $(file)
	pdflatex $(file).tex
	pdflatex $(file).tex
1: 
	pdflatex $(file).tex
	bibtex $(file)
	pdflatex $(file).tex
	pdflatex $(file).tex
	open $(file).pdf

2:
	pdflatex $(file2).tex
	bibtex $(file2)
	pdflatex $(file2).tex
	pdflatex $(file2).tex
	open $(file).pdf

clean:
	rm -rf *.aux *.log *.toc *.bbl *.blg

sweep:
	rm -rf *~
