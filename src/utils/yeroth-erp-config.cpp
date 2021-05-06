/*
 * yeroth-erp-config.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QDebug>


QString YerothERPConfig::_db_type("MYSQL");

QString YerothERPConfig::_db_name("yeroth_erp_3");

QString YerothERPConfig::_db_ip_address("localhost");

QString YerothERPConfig::_db_user_name("yeroth_erp_3");

QString YerothERPConfig::_db_user_pwd("12345");

QString YerothERPConfig::_db_connection_options("");

bool YerothERPConfig::ouvrirRegistreDeCaisse(false);

QString YerothERPConfig::currency("$");

QString YerothERPConfig::IP_ADDRESS_NETWORK_printer("127.0.0.1");

QString YerothERPConfig::printer("pdf");

QString YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY("/opt/yeroth-erp-3.0-standalone/YEROTH_sql_backup");

QString YerothERPConfig::pathToPdfCOMPANY_LOGO("");

QString YerothERPConfig::pathToPdfReader("/usr/bin/evince");

QString YerothERPConfig::pathToThermalPrinterDeviceFile("/dev/usb/lp0");

QString YerothERPConfig::pathToLatexSystemRootFolder("/usr/bin");

QString YerothERPConfig::receiptFormat("grand (A4)");

QString YerothERPConfig::receiptType("");

QString YerothERPConfig::salesStrategy("");

QString YerothERPConfig::YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER("/etc");

QString YerothERPConfig::YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER("");

QString YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER("");

QString YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER("");

QString YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE("");

QString YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER(FILE_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER);

QString YerothERPConfig::YEROTH_ERP_3_0_SYSTEM_DAEMON_ID_STR("yeroth-erp-3-0-");

QString YerothERPConfig::YEROTH_ERP_3_0_SYSTEM_DAEMON_ID("yeroth-erp-3-0-system-daemon");

uint YerothERPConfig::backup_restore_yeroth_erp_3_time_interval(0);

uint YerothERPConfig::alert_period_time_interval(0);

uint YerothERPConfig::alert_quantity_time_interval(0);

uint YerothERPConfig::pagination_number_of_table_rows(12);

uint YerothERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING(9);

uint YerothERPConfig::max_string_display_length(22);

double YerothERPConfig::tva_value(19.25/100.0);

QString YerothERPConfig::annee_depart_rapports_chiffre_affaire_value(GET_CURRENT_DATE_YEAR);

QString YerothERPConfig::annee_depart_pour_la_pagination(GET_CURRENT_DATE_YEAR);

QString YerothERPConfig::pathToMARIA_DB_BASE_DIR("/usr/bin");

QString YerothERPConfig::temporaryFilesDir("/home/yeroth");

#ifndef YEROTH_DEVEL_TARGET
QString YerothERPConfig::THIS_SITE_LOCALISATION_NAME("THIS_SITE_LOCALISATION_HARD_CODED_BY_DEVELOPER_FOR_MAXIMUM_BENEFIT_FOR_CLIENT");
#endif //YEROTH_DEVEL_TARGET

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

const QString YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO					(QObject::trUtf8(OPTION_SALES_STRATEGY_FEFO));

const int YerothERPConfig::OPEN_CASH_DRAWER(1);

const int YerothERPConfig::CLOSE_CASH_DRAWER(0);


const unsigned int YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL 	(0);

const unsigned int YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL	(1);

const unsigned int YerothERPConfig::CONFIG_TVA_VALUE (2);

const unsigned int YerothERPConfig::CONFIG_SALES_STRATEGY (3);

const unsigned int YerothERPConfig::CONFIG_CURRENCY (4);

const unsigned int YerothERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR (5);

const unsigned int YerothERPConfig::CONFIG_BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL (6);

const unsigned int YerothERPConfig::CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3 (7);


void YerothERPConfig::initYerothConfig(QString initCfg)
{
	QFile file(initCfg);

	if (!file.open(QFile::ReadOnly))
	{
		return ;
	}

	QTextStream init_cfg(&file);
	QString line;
	YerothQStringList list;

	do
	{
		line = init_cfg.readLine();
		list = line.split(YEROTH_ERP_3_0_CONFIGURATION_FILE_SEPARATION_OPERATOR);
		//logger << "++ line: " << line << "\n";

		if ("db_type" == list.at(0))
		{
			YerothERPConfig::_db_type = list.at(1).trimmed();
			// logger << "++ db_type = " << db_type << "\n";
		}
		if ("db_name" == list.at(0))
		{
#ifndef YEROTH_ERP_3_0_TESTING_UNIT_TEST
		    YerothERPConfig::_db_name = list.at(1).trimmed();
#else
		    YerothERPConfig::_db_name = QString("%1_test")
		    								.arg(list.at(1).trimmed());
#endif

			//logger << "++ db_name = " << db_name << "\n";
		}
		else if ("db_ip_address" == list.at(0))
		{
		    YerothERPConfig::_db_ip_address = list.at(1).trimmed();
			//logger << "++ db_ip_address = " << db_ip_address << "\n";
		}
		else if ("db_user_name" == list.at(0))
		{
		    YerothERPConfig::_db_user_name = list.at(1).trimmed();
			//logger << "++ db_user_name = " << db_user_name << "\n";
		}
		else if ("db_user_pwd" == list.at(0))
		{
		    YerothERPConfig::_db_user_pwd = list.at(1).trimmed();
			//logger << "++ db_user_pwd = " << db_user_pwd << "\n";
		}
		else if ("db_connection_options" == list.at(0))
		{
			YerothERPConfig::_db_connection_options = list.at(1).trimmed();
			//logger << "++ db_connection_options = " << db_connection_options << "\n";
		}
	}
	while(!line.isNull());
}


void YerothERPConfig::saveYerothConfig()
{
	QFile file(YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE);

	if (!file.open(QFile::WriteOnly))
	{
		return ;
	}

	QTextStream textStream(&file);

	textStream
		<< QString("local_parameter_full_path_pdf_reader=%1\n")
				.arg(YerothERPConfig::pathToPdfReader)

		<< QString("local_parameter_full_path_pdf_latex_executable_root_dir=%1\n")
				.arg(YerothERPConfig::pathToLatexSystemRootFolder)

		<< QString("local_parameter_full_path_mariadb_client_executables_root_dir=%1\n")
				.arg(YerothERPConfig::pathToMARIA_DB_BASE_DIR)

		<< QString("local_parameter_full_path_file_temporary_folder=%1\n")
				.arg(YerothERPConfig::temporaryFilesDir)

		<< QString("local_parameter_file_system_device_thermal_printer=%1\n")
				.arg(YerothERPConfig::pathToThermalPrinterDeviceFile)

		<< QString("local_parameter_type_receipt=%1\n")
				.arg(YerothERPConfig::receiptType)

		<< QString("local_parameter_IP_address_network_thermal_printer=%1\n")
				.arg(YerothERPConfig::IP_ADDRESS_NETWORK_printer)

		<< QString("local_parameter_thermal_printer=%1\n")
				.arg(YerothERPConfig::printer)

		<< QString("local_parameter_state_open_cash_register=%1\n")
				.arg(YerothERPConfig::ouvrirRegistreDeCaisse ? 1 : 0)

		<< QString("local_parameter_format_receipt=%1\n")
				.arg(YerothERPConfig::receiptFormat)

		<< QString("local_parameter_starting_year_business_dashboard=%1\n")
				.arg(YerothERPConfig::annee_depart_rapports_chiffre_affaire_value)

		<< QString("local_parameter_length_maximal_display_string=%1\n")
				.arg(QString::number(YerothERPConfig::max_string_display_length))

		<< QString("local_parameter_length_line_feed_thermal_print_character_count=%1\n")
				.arg(QString::number(YerothERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING))

		<< QString("local_parameter_length_standard_pagination_number=%1\n")
				.arg(QString::number(YerothERPConfig::pagination_number_of_table_rows));

	file.close();
}


