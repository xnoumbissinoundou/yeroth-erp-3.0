default: YEROTH-CV-ENGLISH.pdf

YEROTH-CV-ENGLISH.pdf: clean YEROTH-CV-ENGLISH.tex
	pdflatex YEROTH-CV-ENGLISH
	pdflatex YEROTH-CV-ENGLISH
	evince YEROTH-CV-ENGLISH.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
