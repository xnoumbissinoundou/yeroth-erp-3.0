/*
 * yeroth-erp-config.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_CONFIG_HPP_
#define SRC_YEROTH_CONFIG_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QDate>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

class YerothERPConfig
{
public:

	YEROTH_CLASS_OPERATORS

	static QString _db_type;

	static QString _db_name;

	static QString _db_ip_address;

	static QString _db_user_name;

	static QString _db_user_pwd;

	static QString _db_connection_options;

	static bool ouvrirRegistreDeCaisse;

	static QString currency;

	static QString printer;

	static QString fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY;

	static QString pathToPdfCOMPANY_LOGO;

	static QString pathToPdfReader;

	static QString pathToThermalPrinterDeviceFile;

	static QString pathToLatexSystemRootFolder;

	static QString receiptFormat;

	static QString receiptType;

	static QString salesStrategy;

	static QString YEROTH_ERP_3_0_SYSTEM_DAEMON_ID_STR;

	static QString YEROTH_ERP_3_0_SYSTEM_DAEMON_ID;

	static QString YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER;

	static QString YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER;

	static QString YEROTH_ERP_3_0_HOME_FOLDER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTIES;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_GESTIONAIREDESTOCKS;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MAGASINIER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_CAISSIER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL__DE_LUTILISATEUR_ADMINISTRATEUR;

	static uint backup_restore_yeroth_erp_3_time_interval;

	static uint alert_period_time_interval;

	static uint alert_quantity_time_interval;

	static uint pagination_number_of_table_rows;

	static uint LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING;

	static uint max_string_display_length;

	static double tva_value;

	static QString annee_depart_rapports_chiffre_affaire_value;

	static QString annee_depart_pour_la_pagination;

	static QString pathToMARIA_DB_BASE_DIR;

	static QString temporaryFilesDir;

	static QString THIS_SITE_LOCALISATION_NAME;

	static bool _distantSiteConnected;

	static QString _connectedSite;

	static QString _connectedSiteIPAddress;

	static const QString RECEIPT_FORMAT_PETIT;

	static const QString RECEIPT_FORMAT_GRAND_A4PAPER;

	static const QString STRATEGIE_VENTE_SORTIE_ALL;

	static const QString STRATEGIE_VENTE_SORTIE_FIFO;

	static const QString STRATEGIE_VENTE_SORTIE_LIFO;

	static const QString STRATEGIE_VENTE_SORTIE_FEFO;

	static const int OPEN_CASH_DRAWER;

	static const int CLOSE_CASH_DRAWER;

	static const unsigned int CONFIG_ALERT_PERIOD_TIME_INTERVAL;

	static const unsigned int CONFIG_ALERT_QUANTITY_TIME_INTERVAL;

	static const unsigned int CONFIG_TVA_VALUE;

	static const unsigned int CONFIG_SALES_STRATEGY;

	static const unsigned int CONFIG_CURRENCY;

	static const unsigned int CONFIG_PAGINATION_PAGE_BEGIN_YEAR;

	static const unsigned int CONFIG_BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL;

	static const unsigned int CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3;

	//static const SALES_STRATEGY getSalesStrategy(QString salesStrategyValue);

	inline static QString pathToPs2Pdf()
	{
		return QString("%1/ps2pdf")
				.arg(pathToLatexSystemRootFolder);
	}

	inline static QString pathToDvips()
	{
		return QString("%1/dvips")
				.arg(pathToLatexSystemRootFolder);
	}

	inline static QString pathToLatex()
	{
		return QString("%1/latex")
				.arg(pathToLatexSystemRootFolder);
	}

	inline static QString pathToPdfLatex()
	{
		return QString("%1/pdflatex")
				.arg(pathToLatexSystemRootFolder);
	}

	inline static QString pathToLualatex()
	{
		return QString("%1/lualatex")
				.arg(pathToLatexSystemRootFolder);
	}

	inline static QDate GET_YEROTH_BUSINESS_DASHBOARD_DEFAULT_START_DATE()
	{
		return QDate(YerothERPConfig::annee_depart_rapports_chiffre_affaire_value.toInt(), 1, 1);
	}

	inline static QDate GET_YEROTH_PAGING_DEFAULT_START_DATE()
	{
		return QDate(YerothERPConfig::annee_depart_pour_la_pagination.toInt(), 1, 1);
	}

	static void initYerothConfig(QString initCfg);

	static void saveYerothConfig();
};

#endif /* SRC_YEROTH_CONFIG_HPP_ */
