default: yeroth-erp-3-0-application-architecture.pdf

yeroth-erp-3-0-application-architecture.pdf: clean yeroth-erp-3-0-application-architecture.tex 
	pdflatex yeroth-erp-3-0-application-architecture
	bibtex yeroth-erp-3-0-application-architecture
	pdflatex yeroth-erp-3-0-application-architecture
	pdflatex yeroth-erp-3-0-application-architecture
	evince yeroth-erp-3-0-application-architecture.pdf&

clean:
	rm -f *~ *.glo *.aux *.blg *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
