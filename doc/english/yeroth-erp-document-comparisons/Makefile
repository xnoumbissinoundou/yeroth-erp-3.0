default: yeroth-erp-document-comparisons.pdf

yeroth-erp-document-comparisons.pdf: clean yeroth-erp-document-comparisons.tex 
	pdflatex yeroth-erp-document-comparisons
	pdflatex yeroth-erp-document-comparisons
	evince yeroth-erp-document-comparisons.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
