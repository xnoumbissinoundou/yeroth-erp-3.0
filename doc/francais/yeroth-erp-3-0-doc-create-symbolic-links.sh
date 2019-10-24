#!/bin/bash
# @author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (A.B.D.)

symlinksfiles=(images yeroth-administration yeroth-user-tex-file)

for f in $(find . -name 'manuel*'); do
		curDir="$f"
		for j in "${symlinksfiles[@]}"; do
				curFile=${curDir}/"${j}"
				if [ -L ${curFile} ]; then
						rm -f ${curFile}
						ln -s ${PWD}/${j} ${curFile}
				fi
		done
done

rm -f yeroth-erp-3.0-manuel-de-lutilisateur-manager.pdf

rm -f yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks.pdf

rm -f yeroth-erp-3.0-manuel-de-lutilisateur-magasinier.pdf

rm -f yeroth-erp-3.0-manuel-de-lutilisateur-caissier.pdf

rm -f yeroth-erp-3.0-manuel-de-lutilisateur-administrateur.pdf

ln -s "$PWD"/manuel-de-lutilisateur-manager/yeroth-erp-3.0-manuel-de-lutilisateur-manager.pdf yeroth-erp-3.0-manuel-de-lutilisateur-manager.pdf

ln -s "$PWD"/manuel-de-lutilisateur-gestionairedestocks/yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks.pdf yeroth-erp-3.0-manuel-de-lutilisateur-gestionairedestocks.pdf

ln -s "$PWD"/manuel-de-lutilisateur-magasinier/yeroth-erp-3.0-manuel-de-lutilisateur-magasinier.pdf yeroth-erp-3.0-manuel-de-lutilisateur-magasinier.pdf

ln -s "$PWD"/manuel-de-lutilisateur-caissier/yeroth-erp-3.0-manuel-de-lutilisateur-caissier.pdf yeroth-erp-3.0-manuel-de-lutilisateur-caissier.pdf

ln -s "$PWD"/manuel-de-lutilisateur-administrateur/yeroth-erp-3.0-manuel-de-lutilisateur-administrateur.pdf yeroth-erp-3.0-manuel-de-lutilisateur-administrateur.pdf
