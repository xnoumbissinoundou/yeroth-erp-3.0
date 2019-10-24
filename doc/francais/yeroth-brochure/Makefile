default: yeroth-erp-3.0-brochure.pdf

yeroth-erp-3.0-brochure.pdf: clean yeroth-erp-3.0-brochure.tex
	pdflatex yeroth-erp-3.0-brochure
	pdflatex yeroth-erp-3.0-brochure
	evince yeroth-erp-3.0-brochure.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof *.log \
	   *.lot *.out *.thm *.toc
