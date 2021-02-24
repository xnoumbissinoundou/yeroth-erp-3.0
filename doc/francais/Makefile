default: yeroth-erp-3-0-manuel-de-lutilisateur.pdf

yeroth-erp-3-0-manuel-de-lutilisateur.pdf: clean yeroth-erp-3-0-manuel-de-lutilisateur.tex\
  yeroth-erp-3-0-manuel-de-lutilisateur.bib
	pdflatex yeroth-erp-3-0-manuel-de-lutilisateur
	makeindex yeroth-erp-3-0-manuel-de-lutilisateur.idx
	pdflatex yeroth-erp-3-0-manuel-de-lutilisateur
	pdflatex yeroth-erp-3-0-manuel-de-lutilisateur
	make clean
	evince yeroth-erp-3-0-manuel-de-lutilisateur.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
