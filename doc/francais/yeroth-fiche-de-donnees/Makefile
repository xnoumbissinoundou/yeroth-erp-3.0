default: yeroth-erp-3-0-fiche-de-donnees.pdf

yeroth-erp-3-0-fiche-de-donnees.pdf: clean yeroth-erp-3-0-fiche-de-donnees.tex 
	pdflatex yeroth-erp-3-0-fiche-de-donnees
	pdflatex yeroth-erp-3-0-fiche-de-donnees
	evince yeroth-erp-3-0-fiche-de-donnees.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
