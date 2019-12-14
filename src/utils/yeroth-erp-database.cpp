/*
 * yeroth-erp-database.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-database.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

const QString YerothDatabase::MYSQL("MySQL");

const QString YerothDatabase::QSQLITE("QSQLITE");

QString YerothDatabase::_db_type("");

QString YerothDatabase::_db_name("");

QString YerothDatabase::_db_ip_address("");

QString YerothDatabase::_db_user_name("");

QString YerothDatabase::_db_user_pwd("");

QString YerothDatabase::_db_connection_options("");

YerothDatabase::YerothDatabase()
{
    this->set_db_name(YerothERPConfig::_db_name);
    this->set_db_ip_address(YerothERPConfig::_db_ip_address);
    this->set_db_user_name(YerothERPConfig::_db_user_name);
    this->set_db_user_pwd(YerothERPConfig::_db_user_pwd);
    this->set_db_type(YerothERPConfig::_db_type);
    this->set_db_connection_options(YerothERPConfig::_db_connection_options);

    if (YerothUtils::isEqualCaseInsensitive(YerothDatabase::MYSQL, YerothERPConfig::_db_type))
    {
        //logger << "++ main(): QMYSQL" << "\n";
        _database = QSqlDatabase::addDatabase("QMYSQL");
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothDatabase::QSQLITE, YerothERPConfig::_db_type))
    {
        //logger << "++ main(): QSQLITE" << "\n";
        _database = QSqlDatabase::addDatabase("QSQLITE");
    }

    _database.setDatabaseName(_db_name);
    _database.setHostName(_db_ip_address);
    _database.setUserName(_db_user_name);
    _database.setPassword(_db_user_pwd);
    _database.setConnectOptions(_db_connection_options);
}

void YerothDatabase::set_db_name(QString db_name)
{
    _db_name = db_name;
    _database.setDatabaseName(_db_name);
}

void YerothDatabase::set_db_ip_address(QString db_ip_address)
{
    _db_ip_address = db_ip_address;
    _database.setHostName(_db_ip_address);
}

void YerothDatabase::set_db_user_name(QString db_user_name)
{
    _db_user_name = db_user_name;
    _database.setUserName(_db_user_name);
}

void YerothDatabase::set_db_user_pwd(QString db_user_pwd)
{
    _db_user_pwd = db_user_pwd;
    _database.setPassword(_db_user_pwd);
}

void YerothDatabase::set_db_connection_options(QString db_connection_options)
{
    _db_connection_options = db_connection_options;
    _database.setConnectOptions(_db_connection_options);

}

QString YerothDatabase::toString() const
{
    QString ret;

    if (_database.isValid())
    {
        ret.append(QString("db_type: %1\n"
                           "db_name: %2\n"
                           "db_ip_address: %3\n"
                           "db_connection_options: %4")
                   .arg(_db_type,
                        _database.databaseName(),
                        _database.hostName(),
                        _database.connectOptions()));
    }
    else
    {
        ret.append(QString("db_type: %1\n"
                           "db_name: %2\n"
                           "db_ip_address: %3\n"
                           "db_connection_options: %4")
                   .arg(_db_type,
                        YerothDatabase::_db_name,
                        YerothDatabase::_db_ip_address,
                        YerothDatabase::_db_connection_options));
    }

    return ret;
}
