#!/bin/bash
# @author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)

sudo /etc/init.d/mysql stop

sudo /usr/bin/mysqld_safe &

sleep 5s

sudo expect $PWD/yeroth-erp-3-0-configure-mysql-server-set-root-pwd.exp
