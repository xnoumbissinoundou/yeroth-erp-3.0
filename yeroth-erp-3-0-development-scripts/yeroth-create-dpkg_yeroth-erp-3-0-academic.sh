#!/bin/bash

rm yeroth-erp-3.0-academic/DEBIAN/postinst
yeroth-create-dpkg_yeroth-erp-3-0.sh -e -v "academic"
