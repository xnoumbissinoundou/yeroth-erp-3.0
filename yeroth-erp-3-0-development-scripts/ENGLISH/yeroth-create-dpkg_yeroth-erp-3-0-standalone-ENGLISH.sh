#!/bin/bash

set -e

sudo rm -rf "${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-standalone-ENGLISH"

"${YEROTH_ERP_3_0_HOME_FOLDER}"/yeroth-erp-3-0-development-scripts/ENGLISH/yeroth-create-dpkg_yeroth-erp-3-0-ENGLISH.sh -v standalone
