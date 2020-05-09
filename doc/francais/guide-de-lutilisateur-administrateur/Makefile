default: yeroth-erp-3.0-manuel-de-lutilisateur-administrateur.pdf

yeroth-erp-3.0-manuel-de-lutilisateur-administrateur.pdf: clean yeroth-erp-3.0-manuel-de-lutilisateur-administrateur.tex
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-administrateur
	makeindex yeroth-erp-3.0-manuel-de-lutilisateur-administrateur.idx
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-administrateur
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-administrateur && make clean
	evince yeroth-erp-3.0-manuel-de-lutilisateur-administrateur.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
