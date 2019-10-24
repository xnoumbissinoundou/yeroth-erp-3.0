#!/bin/bash
# @file: yeroth-compile_yeroth-erp-3-0-academic.sh
# @author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
# @email: xnoundou7@gmail.com

OUT_CMD_TIME=$(yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_ACADEMIC_EVALUATION_VERSION "${@}" 2> academic.error.log) && \
  mv bin/yeroth-erp-3-0 bin/yeroth-erp-3.0-academic

val=$(cat academic.error.log | grep error | wc -l)

if [ ${val} -eq 0 ]; then
		echo [compilation avec succes]
else
		echo [compilation avec des erreurs]
fi

echo ${OUT_CMD_TIME}

