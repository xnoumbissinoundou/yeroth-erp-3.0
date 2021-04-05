default: YEROTH-CV-FRENCH.pdf

YEROTH-CV-FRENCH.pdf: clean YEROTH-CV-FRENCH.tex
	pdflatex YEROTH-CV-FRENCH
	pdflatex YEROTH-CV-FRENCH
	evince YEROTH-CV-FRENCH.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
