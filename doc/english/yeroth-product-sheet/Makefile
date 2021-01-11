default: yeroth-erp-3-0-product-sheet.pdf

yeroth-erp-3-0-product-sheet.pdf: clean yeroth-erp-3-0-product-sheet.tex 
	pdflatex yeroth-erp-3-0-product-sheet
	pdflatex yeroth-erp-3-0-product-sheet
	evince yeroth-erp-3-0-product-sheet.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
