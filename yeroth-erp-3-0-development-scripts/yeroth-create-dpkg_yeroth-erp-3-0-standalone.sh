#!/bin/bash

set -e

sudo rm -rf "${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3.0-standalone"

"${YEROTH_ERP_3_0_HOME_FOLDER}"/yeroth-erp-3-0-development-scripts/yeroth-create-dpkg_yeroth-erp-3-0.sh -v standalone
