default: yeroth-devis-materiel-et-estimatif-client.pdf

yeroth-devis-materiel-et-estimatif-client.pdf: clean \
		yeroth-devis-materiel-et-estimatif-client.tex \
		yeroth-plan-daffaire-mise-en-place-client.tex

	pdflatex yeroth-plan-daffaire-mise-en-place-client
	pdflatex yeroth-plan-daffaire-mise-en-place-client
	pdflatex yeroth-devis-materiel-et-estimatif-client
	pdflatex yeroth-devis-materiel-et-estimatif-client
	evince yeroth-plan-daffaire-mise-en-place-client.pdf&

clean:
	rm -f *~ *.glo *.aux *.bbl *.brf *.idx *.bak *.ilg *.ind *.lof \
	   *.lot *.out *.thm *.toc
