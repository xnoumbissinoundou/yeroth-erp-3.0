default: yeroth-devis-materiel-et-estimatif-client.pdf

yeroth-devis-materiel-et-estimatif-client.pdf: clean yeroth-devis-materiel-et-estimatif-client.tex 
	pdflatex yeroth-devis-materiel-et-estimatif-client
	pdflatex yeroth-devis-materiel-et-estimatif-client
	evince yeroth-devis-materiel-et-estimatif-client.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
