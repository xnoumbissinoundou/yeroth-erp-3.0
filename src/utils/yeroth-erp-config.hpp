/*
 * yeroth-erp-config.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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

	static QString salesStrategy;

	static QString YEROTH_ALERT_DAEMON_ID_STR;

	static QString YEROTH_ALERT_DAEMON_ID;

	static QString YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER;

	static QString YEROTH_ERP_ALERT_3_0_HOME_FOLDER;

	static QString YEROTH_ERP_3_0_HOME_FOLDER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_GESTIONAIREDESTOCKS;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MAGASINIER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_CAISSIER;

	static QString FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL__DE_LUTILISATEUR_ADMINISTRATEUR;

	static unsigned int alert_period_time_interval;

	static unsigned int alert_quantity_time_interval;

	static unsigned int max_string_display_length;

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

	static const QString STRATEGIE_VENTE_SORTIE_DEF_DEO;

	static const int OPEN_CASH_DRAWER;

	static const int CLOSE_CASH_DRAWER;

	static const unsigned int CONFIG_PDF_READER;

	static const unsigned int CONFIG_TYPE_OF_FACTURATION;

	static const unsigned int CONFIG_ALERT_PERIOD_TIME_INTERVAL;

	static const unsigned int CONFIG_ALERT_QUANTITY_TIME_INTERVAL;

	static const unsigned int CONFIG_TEMPORARY_FILES_DIR;

	static const unsigned int CONFIG_TVA_VALUE;

	static const unsigned int CONFIG_ANNEE_DEPART_RAPPORTS_CHIFFRE_AFFAIRE;

	static const unsigned int CONFIG_SALES_STRATEGY;

	static const unsigned int CONFIG_PRINTER;

	static const unsigned int CONFIG_CURRENCY;

	static const unsigned int CONFIG_OPEN_CASH_DRAWER;

	static const unsigned int CONFIG_LATEX_SYSTEM_ROOT_FOLDER;

	static const unsigned int CONFIG_THERMAL_PRINTER_DEVICE_FILE_FULL_PATH;

	static const unsigned int CONFIG_MAX_STRING_DISPLAY_LENGTH;

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

	static void initYerothConfig(QString initCfg)
	{
		QFile file(initCfg);

		if (!file.open(QFile::ReadOnly))
		{
			return ;
		}

		QTextStream init_cfg(&file);
		QString line;
		QStringList list;

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
			    YerothERPConfig::_db_name = list.at(1).trimmed();
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
};

#endif /* SRC_YEROTH_CONFIG_HPP_ */
