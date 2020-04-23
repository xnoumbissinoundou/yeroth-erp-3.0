/*
 * yeroth-erp-config.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

QString YerothERPConfig::_db_type("MYSQL");

QString YerothERPConfig::_db_name("yeroth_erp_3");

QString YerothERPConfig::_db_ip_address("localhost");

QString YerothERPConfig::_db_user_name("yeroth_erp_3");

QString YerothERPConfig::_db_user_pwd("12345");

QString YerothERPConfig::_db_connection_options("");

bool YerothERPConfig::ouvrirRegistreDeCaisse(false);

QString YerothERPConfig::currency("$");

QString YerothERPConfig::printer("pdf");

QString YerothERPConfig::pathToPdfReader("/usr/bin/evince");

QString YerothERPConfig::pathToThermalPrinterDeviceFile("/dev/usb/lp0");

QString YerothERPConfig::pathToLatexSystemRootFolder("/usr/bin");

QString YerothERPConfig::receiptFormat("grand (A4)");

QString YerothERPConfig::salesStrategy("");

QString YerothERPConfig::YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER("/etc");

QString YerothERPConfig::YEROTH_ERP_ALERT_3_0_HOME_FOLDER("");

QString YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER("");

QString YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER(FILE_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER);

QString YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_GESTIONAIREDESTOCKS(FILE_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_GESTIONAIREDESTOCKS);

QString YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MAGASINIER(FILE_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MAGASINIER);

QString YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_CAISSIER(FILE_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_CAISSIER);

QString YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL__DE_LUTILISATEUR_ADMINISTRATEUR(FILE_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_ADMINISTRATEUR);

QString YerothERPConfig::YEROTH_ALERT_DAEMON_ID_STR("yeroth-erp-aler");

QString YerothERPConfig::YEROTH_ALERT_DAEMON_ID("yeroth-erp-alert-3-0");

unsigned int YerothERPConfig::alert_period_time_interval(0);

unsigned int YerothERPConfig::alert_quantity_time_interval(0);

unsigned int YerothERPConfig::max_string_display_length(22);

double YerothERPConfig::tva_value(19.25/100.0);

QString YerothERPConfig::annee_depart_rapports_chiffre_affaire_value("2020");

QString YerothERPConfig::temporaryFilesDir("/home/yeroth");

bool YerothERPConfig::_distantSiteConnected(false);

QString YerothERPConfig::_connectedSite;

QString YerothERPConfig::_connectedSiteIPAddress;

# ifdef YEROTH_FRANCAIS_LANGUAGE
const QString YerothERPConfig::RECEIPT_FORMAT_PETIT					(QObject::trUtf8(RECEIPT_FORMAT_SMALL_FR));
const QString YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER			(QObject::trUtf8(RECEIPT_FORMAT_LARGE_A4PAPER_FR));
# endif

# ifdef YEROTH_ENGLISH_LANGUAGE
const QString YerothERPConfig::RECEIPT_FORMAT_PETIT					(QObject::trUtf8(RECEIPT_FORMAT_SMALL_EN));
const QString YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER			(QObject::trUtf8(RECEIPT_FORMAT_LARGE_A4PAPER_EN));
# endif

const QString YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL						(QObject::trUtf8(OPTION_SALES_STRATEGY_ALL));

const QString YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO						(QObject::trUtf8(OPTION_SALES_STRATEGY_FIFO));

const QString YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO						(QObject::trUtf8(OPTION_SALES_STRATEGY_LIFO));

const QString YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO					(QObject::trUtf8(OPTION_SALES_STRATEGY_DEF_DEO));

const int YerothERPConfig::OPEN_CASH_DRAWER(1);

const int YerothERPConfig::CLOSE_CASH_DRAWER(0);

const unsigned int YerothERPConfig::CONFIG_PDF_READER             		(0);

const unsigned int YerothERPConfig::CONFIG_TYPE_OF_FACTURATION          (1);

const unsigned int YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL 	(2);

const unsigned int YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL	(3);

const unsigned int YerothERPConfig::CONFIG_TEMPORARY_FILES_DIR (4);

const unsigned int YerothERPConfig::CONFIG_TVA_VALUE (5);

const unsigned int YerothERPConfig::CONFIG_ANNEE_DEPART_RAPPORTS_CHIFFRE_AFFAIRE (6);

const unsigned int YerothERPConfig::CONFIG_SALES_STRATEGY (7);

const unsigned int YerothERPConfig::CONFIG_PRINTER (8);

const unsigned int YerothERPConfig::CONFIG_CURRENCY (9);

const unsigned int YerothERPConfig::CONFIG_OPEN_CASH_DRAWER (10);

const unsigned int YerothERPConfig::CONFIG_LATEX_SYSTEM_ROOT_FOLDER (11);

const unsigned int YerothERPConfig::CONFIG_THERMAL_PRINTER_DEVICE_FILE_FULL_PATH (12);

const unsigned int YerothERPConfig::CONFIG_MAX_STRING_DISPLAY_LENGTH (13);


