default: yeroth-erp-3-0-document-comparaisons.pdf

yeroth-erp-3-0-document-comparaisons.pdf: clean yeroth-erp-3-0-document-comparaisons-1.tex yeroth-erp-3-0-document-comparaisons-3.tex
	pdflatex yeroth-erp-3-0-document-comparaisons-1
	pdflatex yeroth-erp-3-0-document-comparaisons-1
	pdflatex yeroth-erp-3-0-document-comparaisons-3
	pdflatex yeroth-erp-3-0-document-comparaisons-3
	pdfjoin yeroth-erp-3-0-document-comparaisons-1.pdf \
		yeroth-erp-3-0-document-comparaisons-3.pdf \
		--outfile yeroth-erp-3-0-document-comparaisons.pdf&
	chmod go+r yeroth-erp-3-0-document-comparaisons.pdf
	evince yeroth-erp-3-0-document-comparaisons.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
