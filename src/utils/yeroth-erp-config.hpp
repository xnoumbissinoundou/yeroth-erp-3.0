/*
 * yeroth-erp-config.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_CONFIG_HPP_
#define SRC_YEROTH_CONFIG_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

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

	static QString pathToPdfReader;

	static QString pathToThermalPrinterDeviceFile;

	static QString pathToLatexSystemRootFolder;

	static QString receiptFormat;

	static QString receiptType;

	static QString salesStrategy;

	static QString YEROTH_ALERT_DAEMON_ID_STR;

	static QString YEROTH_ALERT_DAEMON_ID;

	static QString YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER;

	static QString YEROTH_ERP_ALERT_3_0_HOME_FOLDER;

	static QString YEROTH_ERP_3_0_HOME_FOLDER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTIES;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_GESTIONAIREDESTOCKS;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MAGASINIER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_CAISSIER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL__DE_LUTILISATEUR_ADMINISTRATEUR;

	static uint alert_period_time_interval;

	static uint alert_quantity_time_interval;

	static uint standard_pagination_number;

	static uint max_string_display_length;

	static double tva_value;

	static QString annee_depart_rapports_chiffre_affaire_value;

	static QString temporaryFilesDir;

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

	static void initYerothConfig(QString initCfg);

	static void saveYerothConfig();
};

#endif /* SRC_YEROTH_CONFIG_HPP_ */
