#!/bin/bash
# @author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)

SCRIPT_NAME="$(basename "$0")"

cd "${YEROTH_ERP_3_0_HOME_FOLDER}"

git checkout -b windows10

if [ $? -ne 0 ]; then
	git checkout windows10
else
	echo "${SCRIPT_NAME} | setup windows 10 branch"
	yeroth-WINDOWS-setup-yeroth_windows_branch.sh
	sleep 5
	git commit .
fi

echo "${SCRIPT_NAME} | compiling standalone for windows 10"

yeroth-compile_yeroth-erp-3-0-standalone.sh -j21 -r "${@}"
