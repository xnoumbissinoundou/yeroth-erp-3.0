#!/bin/bash
# @author: DR. XAVIER NOUMBISSI NOUNDOU

sudo /etc/init.d/mysql stop

sudo /usr/bin/mysqld_safe &

sleep 5s

sudo expect $PWD/yeroth-erp-3-0-configure-mysql-server-set-root-pwd.exp
