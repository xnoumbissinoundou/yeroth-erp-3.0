default: yeroth-software-system-architecture.pdf

yeroth-software-system-architecture.pdf: clean yeroth-software-system-architecture.tex 
	pdflatex yeroth-software-system-architecture
	bibtex yeroth-software-system-architecture
	pdflatex yeroth-software-system-architecture
	pdflatex yeroth-software-system-architecture
	evince yeroth-software-system-architecture.pdf&

clean:
	rm -f *~ *.glo *.aux *.blg *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
