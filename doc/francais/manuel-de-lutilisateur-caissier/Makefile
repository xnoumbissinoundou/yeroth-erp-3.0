default: yeroth-erp-3.0-manuel-de-lutilisateur-caissier.pdf

yeroth-erp-3.0-manuel-de-lutilisateur-caissier.pdf: clean yeroth-erp-3.0-manuel-de-lutilisateur-caissier.tex
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-caissier
	makeindex yeroth-erp-3.0-manuel-de-lutilisateur-caissier.idx
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-caissier
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-caissier && make clean
	evince yeroth-erp-3.0-manuel-de-lutilisateur-caissier.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
