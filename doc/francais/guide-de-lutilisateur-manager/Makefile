default: yeroth-erp-3.0-manuel-de-lutilisateur-manager.pdf

yeroth-erp-3.0-manuel-de-lutilisateur-manager.pdf: clean yeroth-erp-3.0-manuel-de-lutilisateur-manager.tex
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-manager
	makeindex yeroth-erp-3.0-manuel-de-lutilisateur-manager.idx
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-manager
	pdflatex yeroth-erp-3.0-manuel-de-lutilisateur-manager && make clean
	evince yeroth-erp-3.0-manuel-de-lutilisateur-manager.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
