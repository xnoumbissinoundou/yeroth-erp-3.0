default: yeroth-erp-document-comparison.pdf

yeroth-erp-document-comparison.pdf: clean yeroth-erp-document-comparison.tex 
	pdflatex yeroth-erp-document-comparison
	pdflatex yeroth-erp-document-comparison
	evince yeroth-erp-document-comparison.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
