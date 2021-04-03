#!/bin/bash
# @file: yeroth-compile_yeroth-erp-3-0-server.sh
# @author: PR. XAVIER NOUMBISSI NOUNDOU
# @email: xnoundou7@gmail.com

yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_SERVER "${@}" 2> server.error.log && \
	mv bin/yeroth-erp-3-0 bin/yeroth-erp-3.0-server
