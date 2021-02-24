default: yeroth-erp-3-0-document-comparaisons.pdf

yeroth-erp-3-0-document-comparaisons.pdf: clean yeroth-erp-3-0-document-comparaisons.tex 
	pdflatex yeroth-erp-3-0-document-comparaisons
	pdflatex yeroth-erp-3-0-document-comparaisons
	evince yeroth-erp-3-0-document-comparaisons.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
