#!/bin/bash
# @author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (A.B.D.)

echo "yeroth-erp-3.0 | compilation des manuels des utilisateurs de yeroth-erp-3.0"

for f in $(find . -name 'manuel*'); do
	if [ -d "$f" ]; then
		makefile="$(readlink -f $f/Makefile)"
		echo "yeroth-erp-3.0 | compilation actuelle | ${makefile}"
		if [ -e ${makefile} ]; then
				cd "$f" && make -j16 > /dev/null 2> /dev/null && cd ..
		fi
  fi
done
