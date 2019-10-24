#!/bin/bash
# @file: yeroth-compile_yeroth-erp-3-0-all.sh
# @author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
# @email: xnoundou7@gmail.com

#yeroth-compile_yeroth-erp-3-0-standalone.sh "$@"; \
  yeroth-compile_yeroth-erp-3-0-server.sh "$@"; \
  yeroth-compile_yeroth-erp-3-0-client.sh "$@"; \
  yeroth-compile_yeroth-erp-3-0-academic.sh "$@"
