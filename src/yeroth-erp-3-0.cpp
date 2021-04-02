/*
 * yeroth-erp-3-0.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/utils/yeroth-erp-command-line-parser.hpp"

#include "src/utils/virtual-keyboard/yeroth-erp-key-emitter.hpp"

#include "src/yeroth-erp-windows.hpp"


#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

	#include "src/tests/yeroth-erp-test-yeroth-erp-windows.hpp"

	#include "src/tests/yeroth-erp-test-yeroth-erp-table-view.hpp"

#endif


#include "src/utils/yeroth-erp-command-line-parser.hpp"
#include "src/utils/yeroth-erp-config.hpp"
#include "src/utils/yeroth-erp-utils.hpp"
#include "src/utils/yeroth-erp-database.hpp"
#include "src/utils/yeroth-erp-logger.hpp"
#include "src/process/yeroth-erp-process-info.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QtCore/QList>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QCryptographicHash>
#include <QtCore/QLocale>

#include <QtCore/QTranslator>

#include <QtWidgets/QCommonStyle>
#include <QtWidgets/QStylePlugin>

#include <QtNetwork/QNetworkInterface>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>

#include <cstdlib>

#ifdef YEROTH_CHECK_MEMORY
#include <mcheck.h>
#endif

class YerothMain
{
public:

    inline YerothMain(YerothLogger *logger)
    {
        _logger = logger;
    }

    inline ~YerothMain()
    {
    }

private:

    YerothLogger *_logger;
};


void yeroth_read_file(QFile &file, QString &fileContentVar)
{
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line;

        do
        {
            line = stream.readLine().trimmed();
            fileContentVar.append(line).append("\n");
        }
        while (!line.isNull());
        file.close();
    }
}


void readTexTemplateFiles(YerothLogger &logger)
{
    logger.log("main", "readTexTemplateFiles");


#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileFinancialAccountingReportDataTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_FINANCIAL_ACCOUNTING_REPORT_DATA_FR);
    yeroth_read_file(fileFinancialAccountingReportDataTemplate, YerothUtils::template_bilan_comptable_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileFinancialAccountingReportDataENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_FINANCIAL_ACCOUNTING_REPORT_DATA_EN);
    yeroth_read_file(fileFinancialAccountingReportDataENTemplate, YerothUtils::template_bilan_comptable_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileClientLOYALTY_GROUP_LISTING_FR(FILE_YEROTH_CLIENT_LOYALTY_GROUPS_LISTING_TEMPLATE_FR);
    yeroth_read_file(fileClientLOYALTY_GROUP_LISTING_FR, YerothUtils::template_CLIENT_LOYALTY_GROUP_LISTING_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileClientLOYALTY_GROUP_LISTING_EN(FILE_YEROTH_CLIENT_LOYALTY_GROUPS_LISTING_TEMPLATE_EN);
    yeroth_read_file(fileClientLOYALTY_GROUP_LISTING_EN, YerothUtils::template_CLIENT_LOYALTY_GROUP_LISTING_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileClientLOYALTYprogramLISTING_FR(FILE_YEROTH_CLIENT_LOYALTY_PROGRAM_LISTING_TEMPLATE_FR);
    yeroth_read_file(fileClientLOYALTYprogramLISTING_FR, YerothUtils::template_CLIENT_LOYALTY_PROGRAM_LISTING_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileClientLOYALTYprogramLISTING_EN(FILE_YEROTH_CLIENT_LOYALTY_PROGRAM_LISTING_TEMPLATE_EN);
    yeroth_read_file(fileClientLOYALTYprogramLISTING_EN, YerothUtils::template_CLIENT_LOYALTY_PROGRAM_LISTING_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileClientLOYALTYprogramTemplateFR(FILE_YEROTH_LOYALTY_MEMBERSHIP_CARD_TEMPLATE_FR);
    yeroth_read_file(fileClientLOYALTYprogramTemplateFR, YerothUtils::template_carte_de_fidelite_client_TEMPLATE_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileClientLOYALTYprogramTemplateEN(FILE_YEROTH_LOYALTY_MEMBERSHIP_CARD_TEMPLATE_EN);
    yeroth_read_file(fileClientLOYALTYprogramTemplateEN, YerothUtils::template_carte_de_fidelite_client_TEMPLATE_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileSupplierDataTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_SUPPLIER_DATA_FR);
    yeroth_read_file(fileSupplierDataTemplate, YerothUtils::template_fiche_dun_fournisseur_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileSupplierDataENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_SUPPLIER_DATA_EN );
    yeroth_read_file(fileSupplierDataENTemplate, YerothUtils::template_fiche_dun_fournisseur_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileCustomerDataTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_CUSTOMER_DATA_FR);
    yeroth_read_file(fileCustomerDataTemplate, YerothUtils::template_fiche_client_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileCustomerDataENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_CUSTOMER_DATA_EN );
    yeroth_read_file(fileCustomerDataENTemplate, YerothUtils::template_fiche_client_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile barDiagTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_BAR_DIAG_FR);
    yeroth_read_file(barDiagTemplate, YerothUtils::FR_bar_diag_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile barDiagENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_BAR_DIAG_EN );
    yeroth_read_file(barDiagENTemplate, YerothUtils::EN_bar_diag_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile barChartTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_BAR_CHART_FR);
    yeroth_read_file(barChartTemplate, YerothUtils::FR_bar_chart_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile barChartENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_BAR_CHART_EN);
    yeroth_read_file(barChartENTemplate, YerothUtils::EN_bar_chart_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile ZERO_ventes_Template(FILE_YEROTH_ERP_3_0_TEMPLATE_ZERO_VENTES_FR);
    yeroth_read_file(ZERO_ventes_Template, YerothUtils::FR_ZERO_ventes_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile ZERO_ventes_ENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_ZERO_VENTES_EN);
    yeroth_read_file(ZERO_ventes_ENTemplate, YerothUtils::EN_ZERO_ventes_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile pieChartTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_PIE_CHART_FR);
    yeroth_read_file(pieChartTemplate, YerothUtils::FR_pie_chart_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile pieChartENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_PIE_CHART_EN);
    yeroth_read_file(pieChartENTemplate, YerothUtils::EN_pie_chart_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileFactureSmall_fr(FILE_YEROTH_ERP_3_0_TEMPLATE_RECEIPT_SMALL_FR);
    yeroth_read_file(fileFactureSmall_fr, YerothUtils::FR_template_facture_petit_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileFactureSmall_en(FILE_YEROTH_ERP_3_0_TEMPLATE_RECEIPT_SMALL_EN);
    yeroth_read_file(fileFactureSmall_en, YerothUtils::EN_template_facture_petit_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileFacture(FILE_YEROTH_ERP_3_0_TEMPLATE_RECEIPT_LARGE_FR);
    yeroth_read_file(fileFacture, YerothUtils::FR_template_facture_grand_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileFactureEn(FILE_YEROTH_ERP_3_0_TEMPLATE_RECEIPT_LARGE_EN);
    yeroth_read_file(fileFactureEn, YerothUtils::EN_template_facture_grand_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileStockHistory(FILE_YEROTH_ERP_3_0_TEMPLATE_HISTORIQUE_DUN_STOCK_FR);
    yeroth_read_file(fileStockHistory, YerothUtils::template_historique_dun_stock_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileStockHistoryEN(FILE_YEROTH_ERP_3_0_TEMPLATE_HISTORIQUE_DUN_STOCK_EN);
    yeroth_read_file(fileStockHistoryEN, YerothUtils::template_historique_dun_stock_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile filePayments(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_PAYMENTS_FR);
    yeroth_read_file(filePayments, YerothUtils::template_journal_des_paiements_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile filePaymentsEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_PAYMENTS_EN);
    yeroth_read_file(filePaymentsEn, YerothUtils::template_journal_des_paiements_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileLandscape(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_SALES_FR);
    yeroth_read_file(fileLandscape, YerothUtils::template_journal_des_ventes_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileLandscapeEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_SALES_EN);
    yeroth_read_file(fileLandscapeEn, YerothUtils::template_journal_des_ventes_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileLandscapeTransactions(FILE_YEROTH_ERP_3_0_TEMPLATE_MOUVEMENTSDESTOCKS_FR);
    yeroth_read_file(fileLandscapeTransactions, YerothUtils::FR_template_liste_des_mouvements_de_stocks_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileLandscapeTransactionsEN(FILE_YEROTH_ERP_3_0_TEMPLATE_MOUVEMENTSDESTOCKS_EN);
    yeroth_read_file(fileLandscapeTransactionsEN, YerothUtils::EN_template_liste_des_mouvements_de_stocks_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileSupplierTransactionListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_FOURNISSEUR_FR);
    yeroth_read_file(fileSupplierTransactionListingFr, YerothUtils::template_transactions_dun_fournisseur_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileSupplierTransactionListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_FOURNISSEUR_EN);
    yeroth_read_file(fileSupplierTransactionListingEn, YerothUtils::template_transactions_dun_fournisseur_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileCustomerTransactionListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_CLIENT_FR);
    yeroth_read_file(fileCustomerTransactionListingFr, YerothUtils::template_transactions_dun_client_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileCustomerTransactionListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_CLIENT_EN);
    yeroth_read_file(fileCustomerTransactionListingEn, YerothUtils::template_transactions_dun_client_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileSupplierListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_FOURNISSEURS_FR);
    yeroth_read_file(fileSupplierListingFr, YerothUtils::template_fournisseurs_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileSupplierListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_FOURNISSEURS_EN);
    yeroth_read_file(fileSupplierListingEn, YerothUtils::template_fournisseurs_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileCustomerAccountListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_COMPTES_CLIENTS_FR);
    yeroth_read_file(fileCustomerAccountListingFr, YerothUtils::template_comptes_clients_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileCustomerAccountListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_COMPTES_CLIENTS_EN);
    yeroth_read_file(fileCustomerAccountListingEn, YerothUtils::template_comptes_clients_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileStockListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_MARCHANDISES_FR);
    yeroth_read_file(fileStockListingFr, YerothUtils::template_marchandises_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileStockListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_MARCHANDISES_EN);
    yeroth_read_file(fileStockListingEn, YerothUtils::template_marchandises_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileListeAlertesFr(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ALERTES_FR);
    yeroth_read_file(fileListeAlertesFr, YerothUtils::template_lister_alertes_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileListeAlertesEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ALERTES_EN);
    yeroth_read_file(fileListeAlertesEn, YerothUtils::template_lister_alertes_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileListe_CHARGES_FINANCIERES_FR(FILE_YEROTH_ERP_3_0_TEMPLATE_CHARGES_FINANCIERES_FR);
    yeroth_read_file(fileListe_CHARGES_FINANCIERES_FR, YerothUtils::template_CHARGES_FINANCIERES_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileListe_FINANCIAL_EXPENSES_EN(FILE_YEROTH_ERP_3_0_TEMPLATE_CHARGES_FINANCIERES_EN);
    yeroth_read_file(fileListe_FINANCIAL_EXPENSES_EN, YerothUtils::template_CHARGES_FINANCIERES_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileListeAchatsFr(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ACHATS_FR);
    yeroth_read_file(fileListeAchatsFr, YerothUtils::template_lister_achats_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileListeAchatsEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ACHATS_EN);
    yeroth_read_file(fileListeAchatsEn, YerothUtils::template_lister_achats_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileStockDeSecuriteFr(FILE_YEROTH_ERP_3_0_TEMPLATE_FICHE_STOCKS_FR);
    yeroth_read_file(fileStockDeSecuriteFr, YerothUtils::template_lister_stock_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileStockDeSecuriteEn(FILE_YEROTH_ERP_3_0_TEMPLATE_FICHE_STOCKS_EN);
    yeroth_read_file(fileStockDeSecuriteEn, YerothUtils::template_lister_stock_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileTransfertsTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_SMALL_FR);
    yeroth_read_file(fileTransfertsTemplate, YerothUtils::FR_template_sortie_des_stocks_petit_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileTransfertsTemplateEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_SMALL_EN);
    yeroth_read_file(fileTransfertsTemplateEn, YerothUtils::EN_template_sortie_des_stocks_petit_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileTransfertsBigTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_LARGE_FR);
    yeroth_read_file(fileTransfertsBigTemplate, YerothUtils::FR_template_sortie_des_stocks_grand_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileTransfertsBigTemplateEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_LARGE_EN);
    yeroth_read_file(fileTransfertsBigTemplateEn, YerothUtils::EN_template_sortie_des_stocks_grand_tex);

#endif

    QFile barDiagTemplate2(FILE_YEROTH_ERP_3_0_TEMPLATE_1A_TEX);
    yeroth_read_file(barDiagTemplate2, YerothUtils::_1a_tex);

}


void read_system_local_yeroth_configuration()
{
	QFile file(YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTIES);

	if (!file.open(QFile::ReadOnly))
	{
		return ;
	}

	QTextStream init_cfg(&file);
	QString aValue;
	QString line;
	QStringList list;

	do
	{
		line = init_cfg.readLine();

		list = line.split(YEROTH_ERP_3_0_CONFIGURATION_FILE_SEPARATION_OPERATOR);

//		QDEBUG_STRING_OUTPUT_2("line", line);

		if (YerothUtils::isEqualCaseInsensitive("local_parameter_full_path_pdf_reader", list.at(0)))
		{
			YerothERPConfig::pathToPdfReader = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_full_path_pdf_latex_executable_root_dir", list.at(0)))
		{
			YerothERPConfig::pathToLatexSystemRootFolder = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_full_path_mariadb_client_executables_root_dir", list.at(0)))
		{
			YerothERPConfig::pathToMARIA_DB_BASE_DIR = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_full_path_file_temporary_folder", list.at(0)))
		{
			YerothERPConfig::temporaryFilesDir = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_file_system_device_thermal_printer", list.at(0)))
		{
			YerothERPConfig::pathToThermalPrinterDeviceFile = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_type_receipt", list.at(0)))
		{
			YerothERPConfig::receiptType = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_thermal_printer", list.at(0)))
		{
			YerothERPConfig::printer = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_state_open_cash_register", list.at(0)))
		{
			aValue = list.at(1).trimmed();

			if (aValue.toInt() == YerothERPConfig::CLOSE_CASH_DRAWER)
			{
				YerothERPConfig::ouvrirRegistreDeCaisse = false;
			}
			else
			{
				YerothERPConfig::ouvrirRegistreDeCaisse = true;
			}
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_format_receipt", list.at(0)))
		{
			YerothERPConfig::receiptFormat = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_starting_year_business_dashboard", list.at(0)))
		{
			YerothERPConfig::annee_depart_rapports_chiffre_affaire_value = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_length_line_feed_thermal_print_character_count", list.at(0)))
		{
			YerothERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING = list.at(1).trimmed().toInt();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_length_maximal_display_string", list.at(0)))
		{
			YerothERPConfig::max_string_display_length = list.at(1).trimmed().toInt();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_length_standard_pagination_number", list.at(0)))
		{
			YerothERPConfig::pagination_number_of_table_rows = list.at(1).trimmed().toInt();
		}
	}
	while(!line.isNull());
}


void YEROTH_CREATE_FOLDER(YerothLogger &logger,
						  const QString &aFullPathDir)
{
    QFileInfo aFullPathDirInfo(aFullPathDir);

    if (!aFullPathDirInfo.exists())
    {
        logger.log("[main] read_yeroth_configuration",
                   QString("Folder '%1' for temporary files does not exist!")
				   	   .arg(aFullPathDir));

        QDir tempFileDir;

        if (tempFileDir.mkpath(aFullPathDir))
        {
            logger.log("[main] read_yeroth_configuration",
                       QString("Created folder '%1' for temporary files")
					   	   .arg(aFullPathDir));
        }
        else
        {
            logger.log("[main] read_yeroth_configuration",
                       QString("Could not Create folder '%1' for temporary files!")
					   	   .arg(aFullPathDir));
        }
    }
}


void read_yeroth_configuration(YerothLogger &logger, YerothERPWindows &allWindows)
{
    YerothSqlTableModel &configurationsTableModel =
        allWindows.getSqlTableModel_configurations();

    //***
    QSqlRecord configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);
    QString alertPeriodTimeIntervalValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));


    //***
    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);
    QString alertQuantityTimeIntervalValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));


    //***
    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_TVA_VALUE);
    QString tvaValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));


    //***
    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_SALES_STRATEGY);
    QString salesStrategyValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));


    //***
    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_CURRENCY);
    QString currencyValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));


    //***
    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR);
    QString pageBeginYearValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));


    //***
    configurationRecord = configurationsTableModel.
    		record(YerothERPConfig::CONFIG_BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL);
    QString BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL_VALUE
		(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));


    //***
    configurationRecord = configurationsTableModel.
    		record(YerothERPConfig::CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3);

    QString DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3_VALUE
		(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));



    /*
     * Now setting the configuration values
     */
    YerothERPConfig::currency = currencyValue;
    YerothERPConfig::alert_period_time_interval = alertPeriodTimeIntervalValue.toUInt();
    YerothERPConfig::alert_quantity_time_interval = alertQuantityTimeIntervalValue.toUInt();
    YerothERPConfig::tva_value = (tvaValue.toDouble() / 100.0);
    YerothERPConfig::salesStrategy = salesStrategyValue;

    YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY =
    		DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3_VALUE;


    if (BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL_VALUE.isEmpty())
    {
    	YerothERPConfig::backup_restore_yeroth_erp_3_time_interval = 169200;
    }
    else
    {
    	YerothERPConfig::backup_restore_yeroth_erp_3_time_interval =
    			BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL_VALUE.toUInt();
    }

    if (!pageBeginYearValue.isEmpty())
    {
    	YerothERPConfig::annee_depart_pour_la_pagination = pageBeginYearValue;
    }
    else
    {
    	YerothERPConfig::annee_depart_pour_la_pagination = GET_CURRENT_DATE_YEAR;
    }


    logger.log("[main] read_yeroth_configuration",
               QString("Folder for backup: %1")
			   	   .arg(YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY));

    YEROTH_CREATE_FOLDER(logger, YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY);


    logger.log("[main] read_yeroth_configuration",
               QString("Folder for temporary files: %1")
			   	   .arg(YerothERPConfig::temporaryFilesDir));

    YEROTH_CREATE_FOLDER(logger, YerothERPConfig::temporaryFilesDir);
}


int main(int argc, char *argv[])
{

#ifdef YEROTH_CHECK_MEMORY
    mtrace();
#endif


#ifdef YEROTH_ERP_3_0_TOUCH_SCREEN

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#endif

    YerothERPConfig::YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER = QString(std::getenv("YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER")).trimmed();

    YerothERPConfig::YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER = QString(std::getenv("YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER")).trimmed();

    YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER = QString(std::getenv("YEROTH_ERP_3_0_HOME_FOLDER")).trimmed();

    YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTIES
		=	QString("%1/%2")
				.arg(YerothERPConfig::YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER,
					 FILE_YEROTH_ERP_3_0_PUBLIC_SYSTEM_LOCAL_CONFIGURATION_PROPERTIES);

    YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER.prepend('/').prepend(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER);

    QApplication app(argc, argv);

#ifdef YEROTH_FRANCAIS_LANGUAGE



#else //YEROTH_ENGLISH_LANGUAGE

    QString path(QString("%1/bin/%2")
    				.arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER,
    					 TRANSLATION_FILE_ENGLISH));

    QTranslator translator;
    translator.load(path);
    app.installTranslator(&translator);

#endif

    QString logFileName(QString("%1/%2")
    						.arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER,
    							 FILE_YEROTH_ERP_3_0_PUBLIC_LOG_INFO));

    YerothUtils::setMainWorkingDirectory(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER);

    YerothUtils::setLogFileName(logFileName);

    YerothLogger logger("yeroth-erp-3.0");

    logger.log("main",
               QString("yeroth home folder (located in the environment variable YEROTH_ERP_3_0_HOME_FOLDER): %1")
               .arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER));

    logger.log("main", QString("log file name: %1").arg(logFileName));

    YerothMain stockFacileMain(&logger);

    QString initCfg(QString("%1/%2")
    					.arg(YerothERPConfig::YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER,
    						 FILE_YEROTH_ERP_3_0_PUBLIC_USER_PROPERTIES));

    logger.log("main", QString("%1 path: %2").arg(FILE_YEROTH_ERP_3_0_PUBLIC_USER_PROPERTIES, initCfg));

    YerothERPConfig::initYerothConfig(initCfg);

    read_system_local_yeroth_configuration();

    YerothDatabase database;

    YerothERPWindows allWindows(qApp->desktop());

    QApplication::setStyle(QStyleFactory::create(YerothUtils::YEROTH_QT_STYLE_CDE));

    YerothUtils::setAllWindows(&allWindows);


#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

    Test_YerothERPTestUtils::setAllWindows(&allWindows);

#endif

    bool yerothCmdLineParser_isUsed = false;

    YerothCommandLineParser yerothCmdLineParser;

    enum YerothCommandLineArgumentType yerothCmdLineParseResult =
    		yerothCmdLineParser.parseProgramArguments(argc, argv);

    switch(yerothCmdLineParseResult)
    {
    case VERSION:
    	yerothCmdLineParser_isUsed = true;
    	YerothUtils::YEROTH_PRINT_TO_STDOUT(YerothUtils::YEROTH_ERP_3_0_LAST_BUILD_ID);
    	break;

    default:
    	break;
    }

    if (yerothCmdLineParser_isUsed)
    {
    	if (VERSION == yerothCmdLineParseResult)
    	{
    		return 0;
    	}
    }

	qDebug() << QString("db_name: %1, "
						"db_ip_address: %2, "
						"db_connection_options: %3")
					.arg(database.db_name(),
						 database.db_ip_address(),
						 database.db_connection_options());

    allWindows.setDatabase(&database);

    bool isDatabaseOpened = database.open();

    logger.log("main", "Database connection could be opened successfully");

    QStringList sql_table_list;

#ifdef YEROTH_FEATURE_COMPTABILITE
    sql_table_list
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_OPERATIONS_COMPTABLES
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_COMPTES_DOPERATIONS_COMPTABLES;
#endif

    sql_table_list
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_COMPTES_BANCAIRES
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_ENTREPRISE_INFO
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_TYPE_DE_VENTE
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_TYPE_DE_PAIEMENT
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_PAIEMENTS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_CREDIT_CARD_ISSUING_COMPANY
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_INIT_CONFIGURATION
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CONFIGURATIONS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_ROLES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_USERS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_TITRES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_LOCALISATIONS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_ACHATS_DE_SERVICES
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_ACHATS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_STOCKS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_SERVICES_COMPLETES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_MARCHANDISES
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_STOCKS_VENDU
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_STOCKS_SORTIES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_ALERTES
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_REMISES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CONDITIONS_ALERTES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_COURRIERS_ALERTES
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_PROGRAMMES_DE_FIDELITES_CLIENTS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_GROUPES_DE_CLIENTS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_CLIENTS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_FOURNISSEURS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_DBMS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_DEPARTEMENTS_PRODUITS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_CATEGORIES;

    for(int k = 0; k < sql_table_list.size(); ++k)
    {
        YerothUtils::executer_fichier_sql(sql_table_list.at(k), &logger);
    }

    read_yeroth_configuration(logger, allWindows);

    readTexTemplateFiles(logger);

    allWindows.createAllYerothPosUserWindows();


    QApplication::setFont(QFont(STRING_APPLICATION_WIDE_FONT_TYPE_YEROTH_ERP_3_0,
                                INT_APPLICATION_WIDE_FONT_SIZE_YEROTH_ERP_3_0));

    QApplication::setWindowIcon(QIcon(":yeroth-erp-3-0-images/yeroth-erp-3-0-icon.png"));


//#define YEROTH_DEVEL_TARGET

#ifdef YEROTH_DEVEL_TARGET

    YerothERPConfig::THIS_SITE_LOCALISATION_NAME = "YEROTH_ERP_3_0_TESTING_RD_BUILDING";

#endif

//	QDEBUG_STRING_OUTPUT_2("YerothERPConfig::THIS_SITE_LOCALISATION_NAME",
//						   YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

#ifndef YEROTH_ERP_3_0_TESTING_UNIT_TEST

    allWindows._mainWindow->rendreVisibleLocalTOCLASS(isDatabaseOpened);

    app.exec();

    database.close();

    return 0;

#else

    Test_YerothERPTableView aYEROTHERPTableView_TestInstance;

    return QTest::qExec(&aYEROTHERPTableView_TestInstance, argc, argv);

#endif
}
