default: yeroth-erp-3.0-brochure-english.pdf

yeroth-erp-3.0-brochure-english.pdf: clean yeroth-erp-3.0-brochure-english.tex 
	pdflatex yeroth-erp-3.0-brochure-english
	pdflatex yeroth-erp-3.0-brochure-english
	evince yeroth-erp-3.0-brochure-english.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
