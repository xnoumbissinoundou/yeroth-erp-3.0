#!/bin/bash
#@author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)

YEROTH_ERP_3_0_IMAGES=${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-images

echo "$(basename $0): processing '.png' files in folder ${YEROTH_ERP_3_0_IMAGES}"

find ${YEROTH_ERP_3_0_IMAGES} -type f -name "*.png" -exec convert {} -strip {} \;
