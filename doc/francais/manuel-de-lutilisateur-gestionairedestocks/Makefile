default: yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks.pdf

yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks.pdf: clean yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks.tex
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks
	makeindex yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks.idx
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks && make clean
	evince yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
