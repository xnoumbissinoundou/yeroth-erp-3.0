/*
 * yeroth-erp-database.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-database.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


const QString YerothDatabase::IMPRIMANTERESEAU_RECUS_PETITS	("imprimantereseau_recus_petits");

const QString YerothDatabase::COMPTES_BANCAIRES			("comptes_bancaires");

const QString YerothDatabase::OPERATIONS_COMPTABLES		("operations_comptables");

const QString YerothDatabase::COMPTES_DOPERATIONS_COMPTABLES	("comptes_doperations_comptables");

const QString YerothDatabase::ENTREPRISE_INFO			("entreprise_info");

const QString YerothDatabase::PAIEMENTS					("paiements");

const QString YerothDatabase::USERS						("users");

const QString YerothDatabase::TITRES					("titres");

const QString YerothDatabase::LOCALISATIONS				("localisations");

const QString YerothDatabase::DEPARTEMENTS_PRODUITS		("departements_produits");

const QString YerothDatabase::CATEGORIES					("categories");

const QString YerothDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS	("programmes_de_fidelite_clients");

const QString YerothDatabase::CLIENTS						("clients");

const QString YerothDatabase::GROUPES_DE_CLIENTS			("groupes_de_clients");

const QString YerothDatabase::FOURNISSEURS				("fournisseurs");

const QString YerothDatabase::ALERTES						("alertes");

const QString YerothDatabase::REMISES						("remises");

const QString YerothDatabase::CREDIT_CARD_ISSUING_COMPANY	("credit_card_issuing_company");

const QString YerothDatabase::CONDITIONS_ALERTES			("conditions_alertes");

const QString YerothDatabase::COURRIERS_ALERTES			("courriers_alertes");

const QString YerothDatabase::ACHATS_DE_SERVICES		("achats_de_services");

const QString YerothDatabase::ACHATS						("achats");

const QString YerothDatabase::STOCKS						("stocks");

const QString YerothDatabase::SERVICES_COMPLETES			("services_completes");

const QString YerothDatabase::STOCKS_VENDU				("stocks_vendu");

const QString YerothDatabase::STOCKS_SORTIES				("stocks_sorties");

const QString YerothDatabase::MARCHANDISES				("marchandises");

const QString YerothDatabase::CONFIGURATIONS				("configurations");

const QString YerothDatabase::INIT_CONFIGURATIONS			("init_configurations");

const QString YerothDatabase::TYPE_DE_VENTE				("type_de_vente");

const QString YerothDatabase::TYPE_DE_PAIEMENT			("type_de_paiement");

const QString YerothDatabase::ROLES						("roles");

const QString YerothDatabase::DBMS						("dbms");


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
     set_db_name(YerothERPConfig::_db_name);
     set_db_ip_address(YerothERPConfig::_db_ip_address);
     set_db_user_name(YerothERPConfig::_db_user_name);
     set_db_user_pwd(YerothERPConfig::_db_user_pwd);
     set_db_type(YerothERPConfig::_db_type);
     set_db_connection_options(YerothERPConfig::_db_connection_options);

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

void YerothDatabase::set_db_name(const QString &db_name)
{
    _db_name = db_name;
    _database.setDatabaseName(_db_name);
}

void YerothDatabase::set_db_ip_address(const QString &db_ip_address)
{
    _db_ip_address = db_ip_address;
    _database.setHostName(_db_ip_address);
}

void YerothDatabase::set_db_user_name(const QString &db_user_name)
{
    _db_user_name = db_user_name;
    _database.setUserName(_db_user_name);
}

void YerothDatabase::set_db_user_pwd(const QString &db_user_pwd)
{
    _db_user_pwd = db_user_pwd;
    _database.setPassword(_db_user_pwd);
}

void YerothDatabase::set_db_connection_options(const QString &db_connection_options)
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
