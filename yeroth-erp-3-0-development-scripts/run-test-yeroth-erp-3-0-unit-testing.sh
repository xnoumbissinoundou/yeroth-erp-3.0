#/bin/bash
#@author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)

${YEROTH_ERP_3_0_HOME_FOLDER}/bin/yeroth-erp-3.0-standalone > test-results-yeroth-erp-3-0.txt 2>&1

run-test-yeroth-erp-3-0-coverage-gen-html.sh
