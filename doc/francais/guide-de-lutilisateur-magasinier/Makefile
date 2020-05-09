default: yeroth-erp-3.0-manuel-de-lutilisateur-magasinier.pdf

yeroth-erp-3.0-manuel-de-lutilisateur-magasinier.pdf: clean yeroth-erp-3.0-manuel-de-lutilisateur-magasinier.tex
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-magasinier
	makeindex yeroth-erp-3.0-manuel-de-lutilisateur-magasinier.idx
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-magasinier
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-magasinier && make clean
	evince yeroth-erp-3.0-manuel-de-lutilisateur-magasinier.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
