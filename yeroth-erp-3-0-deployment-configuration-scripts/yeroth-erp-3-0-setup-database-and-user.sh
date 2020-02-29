#!/bin/bash
#@author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)

MAINDB="yeroth_erp_3"
USER_NAME="yeroth_erp_3"
PASSWDDB="1234567"
echo "Enter MariaDB administrative password YEROTH-ERP-3.0 database and user !"
mysql -uroot -p -e "CREATE DATABASE ${MAINDB} CHARACTER SET = 'utf8'; CREATE USER ${USER_NAME}@'%' IDENTIFIED BY '${PASSWDDB}'; GRANT ALL PRIVILEGES ON *.* TO '${USER_NAME}'@'%'; FLUSH PRIVILEGES;"
mysql_ret_code="$?"
exit 0
