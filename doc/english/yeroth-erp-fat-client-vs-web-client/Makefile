default: yeroth-erp-fat-client-vs-web-client.pdf

yeroth-erp-fat-client-vs-web-client.pdf: clean yeroth-erp-fat-client-vs-web-client.tex 
	pdflatex yeroth-erp-fat-client-vs-web-client
	pdflatex yeroth-erp-fat-client-vs-web-client
	evince yeroth-erp-fat-client-vs-web-client.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
