#!/bin/bash
# @author: PR. XAVIER NOUMBISSI NOUNDOU

echo "yeroth-erp-3.0 | compilation des manuels des utilisateurs de yeroth-erp-3.0"

for f in $(find . -name 'guide*'); do
	if [ -d "$f" ]; then
		makefile="$(readlink -f $f/Makefile)"
		echo "yeroth-erp-3.0 | compilation actuelle | ${makefile}"
		if [ -e ${makefile} ]; then
				cd "$f" && make -j16 > /dev/null 2> /dev/null && cd ..
		fi
  fi
done
