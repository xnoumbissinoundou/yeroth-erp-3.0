#!/bin/bash

cp yeroth-erp-3-0-development-scripts/yeroth-WINDOWS-compile_yeroth-erp-3-0.sh yeroth-erp-3-0-development-scripts/yeroth-compile_yeroth-erp-3-0.sh
git rm -r yeroth-erp-3-0-development-scripts/yeroth-create-dpkg*
git rm -r yeroth-erp-3-0-deb-file-repository
git rm -r yeroth-erp-3.0-academic
git rm -r yeroth-erp-3.0-standalone
git rm -r yeroth-erp-3-0-gnome-files-desktop
git rm -r test
git rm bin/*.sh
