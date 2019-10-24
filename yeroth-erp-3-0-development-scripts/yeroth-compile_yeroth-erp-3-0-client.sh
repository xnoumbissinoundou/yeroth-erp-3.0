#!/bin/bash
# @file: yeroth-compile_yeroth-erp-3-0-client.sh
# @author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
# @email: xnoundou7@gmail.com

yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_CLIENT "${@}" 2> client.error.log && \
	mv bin/yeroth-erp-3-0 bin/yeroth-erp-3.0-client
