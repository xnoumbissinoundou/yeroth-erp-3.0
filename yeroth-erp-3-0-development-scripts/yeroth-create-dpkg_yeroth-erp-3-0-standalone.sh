#!/bin/bash

set -e

sudo rm -rf yeroth-erp-3.0-standalone

yeroth-create-dpkg_yeroth-erp-3-0.sh -v standalone
