#!/bin/bash
# @author: PR. Xavier NOUMBISSI NOUNDOU

symlinksfiles=(images yeroth-administration yeroth-user-tex-file)

for f in $(find . -name 'guide*'); do
		curDir="$f"
		for j in "${symlinksfiles[@]}"; do
				curFile=${curDir}/"${j}"
				if [ -L ${curFile} ]; then
						rm -f ${curFile}
						ln -s ${PWD}/${j} ${curFile}
				fi
		done
done

rm -f yeroth-erp-3.0-guide-de-lutilisateur-manager.pdf


ln -s "$PWD"/guide-de-lutilisateur-manager/yeroth-erp-3-0-guide-de-lutilisateur-manager.pdf yeroth-erp-3.0-guide-de-lutilisateur-manager.pdf

