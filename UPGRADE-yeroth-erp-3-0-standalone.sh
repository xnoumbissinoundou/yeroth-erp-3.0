#!/bin/bash
#@author: DR. XAVIER NOUMBISSI NOUNDOU

CURRENT_BINARY_PROGRAM="$(basename $0)"

passwd

if [ $? -eq 0 ]; then
		echo "[${CURRENT_BINARY_PROGRAM}] COPYING YEROTH-ERP-3.0-STANDALONE UPGRADE FILES ..."
		echo "[${CURRENT_BINARY_PROGRAM}] FROM $(pwd) ==> ${YEROTH_ERP_3_0_HOME_FOLDER}/bin"
		sudo cp bin/yeroth-erp-3* "${YEROTH_ERP_3_0_HOME_FOLDER}/bin"
else
		echo "[${CURRENT_BINARY_PROGRAM}] WRONG PASSWORD, COULDN'T PERFORM YEROTH-ERP-3.0-STANDALONE UPGRADE"
fi

sleep 1

#echo "[${CURRENT_BINARY_PROGRAM}] sudo passwd "$(whoami)" -d"

sudo passwd "$(whoami)" -d

sleep 1
