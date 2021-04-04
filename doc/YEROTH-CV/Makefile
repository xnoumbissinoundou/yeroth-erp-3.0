default: YEROTH-CV.pdf

YEROTH-CV.pdf: clean YEROTH-CV.tex
	pdflatex YEROTH-CV
	pdflatex YEROTH-CV
	evince YEROTH-CV.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
