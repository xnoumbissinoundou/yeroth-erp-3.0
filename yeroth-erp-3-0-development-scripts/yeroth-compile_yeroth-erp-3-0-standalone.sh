#!/bin/bash
# @file: yeroth-compile_yeroth-erp-3-0-standalone.sh
# @author: DR. XAVIER NOUMBISSI NOUNDOU
# @email: xnoundou7@gmail.com

if [[ "${@}" == *"s"* || "${@}" == *"h"* ]]; then
		yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_STANDALONE "${@}"
		exit
fi

OUT_CMD_TIME=$(time yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_STANDALONE "${@}" > standalone.out.log 2> standalone.error.log) && \
  mv bin/yeroth-erp-3-0 bin/yeroth-erp-3.0-standalone

val=$(cat standalone.error.log | grep 'error\|ERROR\|Error' | wc -l)

if [ ${val} -eq 0 ]; then
		echo "[compilation avec succes]"
else
		echo "[compilation avec des erreurs (${val})]"
fi

echo ${OUT_CMD_TIME}
