#!/bin/bash
# @authors: Jean-Pierre KAMGAIN, BSc
# 	    Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)

SCRIPT_NAME="$(basename "$0")"

YEROTH_ERP_3_0_HOME_FOLDER="yeroth-erp-3.0"

git clone https://github.com/xnoumbissinoundou/"${YEROTH_ERP_3_0_HOME_FOLDER}"

cd "${YEROTH_ERP_3_0_HOME_FOLDER}"

git checkout -b windows8

if [ $? -ne 0 ]; then
	git checkout windows8
else
	echo "${SCRIPT_NAME} | setup windows 8 branch"
	yeroth-WINDOWS-setup-yeroth_windows_branch.sh
	sleep 5
	git commit .
fi

echo "${SCRIPT_NAME} | compiling standalone for windows 8"

yeroth-compile_yeroth-erp-3-0-standalone.sh -j8
