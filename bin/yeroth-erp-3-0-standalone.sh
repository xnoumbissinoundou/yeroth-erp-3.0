#!/bin/bash
# @author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (A.B.D.)
# 

[ -f /etc/environment ] && . /etc/environment

if [ $YEROTH_ERP_3_0_HOME_FOLDER ]; then
    "$YEROTH_ERP_3_0_HOME_FOLDER"/bin/yeroth-erp-3-0-standalone --gui --style cde
else
    echo "Please set the environment variable: 
    		YEROTH_ERP_3_0_HOME_FOLDER (file '/etc/environment' or
		in your '.bashrc' file, and, in, file: '.xsessionrc'),
		before you could use yeroth-erp-3.0 !"
fi
