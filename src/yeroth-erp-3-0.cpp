/*
 * yeroth-erp-3-0.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */
#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/utils/virtual-keyboard/yeroth-erp-key-emitter.hpp"

#include "src/yeroth-erp-windows.hpp"

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

    QFile fileLandscapeTransactions(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_TRANSACTIONS_FR);
    yeroth_read_file(fileLandscapeTransactions, YerothUtils::FR_template_journal_des_transactions_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileLandscapeTransactionsEN(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_TRANSACTIONS_EN);
    yeroth_read_file(fileLandscapeTransactionsEN, YerothUtils::EN_template_journal_des_transactions_tex);

#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE

    QFile fileCustomerTransactionListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_CLIENT_FR);
    yeroth_read_file(fileCustomerTransactionListingFr, YerothUtils::template_transactions_dun_client_tex);

#else //YEROTH_ENGLISH_LANGUAGE

    QFile fileCustomerTransactionListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_CLIENT_EN);
    yeroth_read_file(fileCustomerTransactionListingEn, YerothUtils::template_transactions_dun_client_tex);

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


void read_yeroth_configuration(YerothLogger &logger, YerothERPWindows &allWindows)
{
    YerothSqlTableModel &configurationsTableModel =
        allWindows.getSqlTableModel_configurations();

    QSqlRecord configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_PDF_READER);
    QString pdfReaderValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_MAX_STRING_DISPLAY_LENGTH);
    QString maxStringLengthValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_THERMAL_PRINTER_DEVICE_FILE_FULL_PATH);
    QString thermalPrinterDeviceFileFullPathValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_LATEX_SYSTEM_ROOT_FOLDER);
    QString latexSystemRootFolderValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);
    QString alertPeriodTimeIntervalValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);
    QString alertQuantityTimeIntervalValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_TEMPORARY_FILES_DIR);
    QString temporaryFilesDirValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_TVA_VALUE);
    QString tvaValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_ANNEE_DEPART_RAPPORTS_CHIFFRE_AFFAIRE);
    QString anneeDepartRapportsChiffreDaffaireValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_SALES_STRATEGY);
    QString salesStrategyValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_PRINTER);
    QString printerValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_OPEN_CASH_DRAWER);
    int openCashDrawerValue = GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration").toInt();

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_CURRENCY);
    QString currencyValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_TYPE_OF_FACTURATION);
    QString receiptFormatValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

    /*
     * Now setting the configuration values
     */
    if (YerothERPConfig::OPEN_CASH_DRAWER == openCashDrawerValue)
    {
    	YerothERPConfig::ouvrirRegistreDeCaisse = true;
    }
    else
    {
    	YerothERPConfig::ouvrirRegistreDeCaisse = false;
    }

    QString userReceiptFormatValue
    (YerothUtils::getCurrentAdminWindowReceiptsFormatAccordingToLanguage(receiptFormatValue));

    receiptFormatValue = userReceiptFormatValue;

    YerothERPConfig::receiptFormat = receiptFormatValue;

    YerothERPConfig::currency = currencyValue;
    YerothERPConfig::printer = printerValue;
    YerothERPConfig::pathToPdfReader = pdfReaderValue;
    YerothERPConfig::pathToThermalPrinterDeviceFile = thermalPrinterDeviceFileFullPathValue;
    YerothERPConfig::pathToLatexSystemRootFolder = latexSystemRootFolderValue;
    YerothERPConfig::alert_period_time_interval = alertPeriodTimeIntervalValue.toUInt();
    YerothERPConfig::alert_quantity_time_interval = alertQuantityTimeIntervalValue.toUInt();
    YerothERPConfig::max_string_display_length = maxStringLengthValue.toUInt();
    YerothERPConfig::tva_value = (tvaValue.toDouble() / 100.0);
    YerothERPConfig::annee_depart_rapports_chiffre_affaire_value = anneeDepartRapportsChiffreDaffaireValue;
    YerothERPConfig::salesStrategy = salesStrategyValue;

    //qDebug() << "++ 1. YerothConfig::salesStrategy: " << YerothConfig::salesStrategy;

    YerothERPConfig::temporaryFilesDir = temporaryFilesDirValue;

    logger.log("[main] read_yeroth_configuration",
               QString("Folder for temporary files: %1").arg(YerothERPConfig::temporaryFilesDir));

    QFileInfo tempDirInfo(YerothERPConfig::temporaryFilesDir);

    if (!tempDirInfo.exists())
    {
        logger.log("[main] read_yeroth_configuration",
                   QString("Folder '%1' for temporary files does not exist!").arg(YerothERPConfig::temporaryFilesDir));

        QDir tempFileDir;
        if (tempFileDir.mkpath(YerothERPConfig::temporaryFilesDir))
        {
            logger.log("[main] read_yeroth_configuration",
                       QString("Created folder '%1' for temporary files").arg(YerothERPConfig::temporaryFilesDir));
        }
        else
        {
            logger.log("[main] read_yeroth_configuration",
                       QString("Could not Create folder '%1' for temporary files!").arg(YerothERPConfig::temporaryFilesDir));
        }
    }
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

    YerothERPConfig::YEROTH_ERP_ALERT_3_0_HOME_FOLDER = QString(std::getenv("YEROTH_ERP_ALERT_3_0_HOME_FOLDER")).trimmed();

    YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER = QString(std::getenv("YEROTH_ERP_3_0_HOME_FOLDER")).trimmed();

    YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER.prepend('/').prepend(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER);

    YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_GESTIONAIREDESTOCKS.prepend('/').prepend(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER);

    YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MAGASINIER.prepend('/').prepend(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER);

    YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_CAISSIER.prepend('/').prepend(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER);

    YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL__DE_LUTILISATEUR_ADMINISTRATEUR.prepend('/').prepend(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER);

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
               QString("yeroth home folder (located in the environment variable YEROTHPOS_HOME_FOLDER): %1")
               .arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER));

    logger.log("main", QString("log file name: %1").arg(logFileName));

    YerothMain stockFacileMain(&logger);

    QString initCfg(QString("%1/%2")
    					.arg(YerothERPConfig::YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER,
    						 FILE_YEROTH_ERP_3_0_PUBLIC_USER_PROPERTIES));

    logger.log("main", QString("%1 path: %2").arg(FILE_YEROTH_ERP_3_0_PUBLIC_USER_PROPERTIES, initCfg));

    YerothERPConfig::initYerothConfig(initCfg);

    YerothDatabase database;

    YerothERPWindows allWindows(qApp->desktop());

    QApplication::setStyle(QStyleFactory::create(YerothUtils::YEROTH_QT_STYLE_CDE));

    YerothUtils::setAllWindows(&allWindows);

    allWindows.createAllYerothPosUserWindows();

    YerothERPConfig::_connectedSite = allWindows.getInfoEntreprise().getLocalisation();

    QApplication::setFont(QFont(STRING_APPLICATION_WIDE_FONT_TYPE_YEROTH_ERP_3_0,
                                INT_APPLICATION_WIDE_FONT_SIZE_YEROTH_ERP_3_0));

    QApplication::setWindowIcon(QIcon(":yeroth-erp-3-0-images/yeroth-erp-3.0-icon.png"));

    allWindows.setDatabase(&database);

    logger.log("main", QString("db_name: %1").arg(database.db_name()));
    logger.log("main", QString("db_ip_address: %1").arg(database.db_ip_address()));
    logger.log("main", QString("db_connection_options: %1").arg(database.db_connection_options()));

    if (!database.open())
    {
        QString errMsg(QString(QObject::trUtf8("Impossible de se connecter au serveur '%1' !\n\n"
        									   "Contacter l'administrateur de '%2'\n\n"
        		                			   "Cliquer sur 'Cancel' pour terminer '%3'"))
        					.arg(database.db_type(),
        						 YerothUtils::APPLICATION_NAME,
								 YerothUtils::APPLICATION_NAME));

        QMessageBox::critical(0, YerothUtils::APPLICATION_NAME, errMsg, QMessageBox::Cancel);

//        qDebug() << QString("++ test: %1").arg(database.toString());

        logger.log("main",
                   QString(QObject::tr("Impossible de se connecter au serveur '%1': %2 !"))
				   	   .arg(database.db_type(),
				   			database.lastError().text()));

        exit(0);
    }

    logger.log("main", "Database connection could be opened successfully");

    QStringList sql_table_list;
    sql_table_list
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_COMPTES_BANCAIRES
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_ENTREPRISE_INFO
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_TYPE_DE_PAIEMENT
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_PAIEMENTS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_CREDIT_CARD_ISSUING_COMPANY
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_INIT_CONFIGURATION
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CONFIGURATIONS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_ROLES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_USERS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_TITRES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_LOCALISATIONS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_ACHATS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_STOCKS
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_SERVICES_COMPLETES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_MARCHANDISES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_STOCKS_SORTIES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_STOCKS_DE_SECURITE
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_ALERTES
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_REMISES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CONDITIONS_ALERTES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_COURRIERS_ALERTES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CLIENTS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_FOURNISSEURS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_DBMS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CATEGORIES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_STRATEGIE_VENTE_SORTIE;

    for(int k = 0; k < sql_table_list.size(); ++k)
    {
        YerothUtils::executer_fichier_sql(sql_table_list.at(k), &logger);
    }

    read_yeroth_configuration(logger, allWindows);

    readTexTemplateFiles(logger);

    allWindows._mainWindow->show();

    app.exec();

    database.close();

    return 0;
}
