#!/bin/bash

ALL_PACKAGES="../yeroth-erp-3-0-ALL-DATABASE-TABLE-FOR-SCRIPT-REFACTORING.txt"

truncate --size=0 truncate_test_yeroth_erp_3_test.sql

while read LINE; do
		declare -l YEROTH_TEMP_LINE="${LINE}"
		typeset -l > /dev/null
		echo "truncate table ${YEROTH_TEMP_LINE};" >> truncate_test_yeroth_erp_3_test.sql
done < "$ALL_PACKAGES"
