/*
 * yeroth-erp-tableau-des-transactions-du-client-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-tableau-des-transactions-du-client-window.hpp"


#include "src/process/yeroth-erp-process.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/qmath.h>


YerothTableauDesTransactionsDuClientWindow::YerothTableauDesTransactionsDuClientWindow()
:YerothWindowsCommons("yeroth-erp-tableau-des-transactions-dun-client")
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("transactions financières d'un client"));

    setupUi(this);

    mySetupUi(this);

    enableResizing();

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_tableau_des_transactions_du_client, false);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}")
			.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);


    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));


    setupShortcuts();


    setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(0);
	setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(1);
    setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(2);
    setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(5);
	setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(6);
}


void YerothTableauDesTransactionsDuClientWindow::listerTransactionsDunClient(QDate dateDebutTransactions,
																			 QDate dateFinTransactions,
																			 QString clientCompanyName,
																			 QSqlQuery &sqlClientTransactionsUnionQuery)
{
	_curDateDebutTransactions = dateDebutTransactions;

	_curDateFinTransactions = dateFinTransactions;

	_clientCompanyName = clientCompanyName;

	QString curWindowTitle(QString("%1 - %2 - '%3'")
								.arg(YEROTH_ERP_WINDOW_TITLE,
									 QObject::tr("transactions d'un client"),
									 clientCompanyName));

	setWindowTitle(curWindowTitle);

	tableView_tableau_des_transactions_du_client
			->lister_les_transactions_dun_client(sqlClientTransactionsUnionQuery);

	tableView_tableau_des_transactions_du_client->resizeColumnsToContents();

	if (tableView_tableau_des_transactions_du_client->rowCount() > 0)
	{
		tableView_tableau_des_transactions_du_client->selectRow(0);
	}

	show();
}


bool YerothTableauDesTransactionsDuClientWindow::export_csv_file()
{
	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

	QString aNewClientCompanyName =
			YerothUtils::YEROTH_REPLACE_ALL_EMPTYSPACES_WITH_UNDERSCORE_CHARACTER(_clientCompanyName);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_tableau_des_transactions_du_client,
										   tableColumnsToIgnore,
										   QString("yeroth-erp-transaction-client-%1-format-csv")
										   	   .arg(aNewClientCompanyName),
										   "transactions d'un client");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_tableau_des_transactions_du_client,
										   tableColumnsToIgnore,
										   QString("yeroth-erp-%1-client-transaction-csv-format")
										   	   .arg(aNewClientCompanyName),
										   "client transaction");
#endif

	return success;
}


bool YerothTableauDesTransactionsDuClientWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_transactions_dun_client_tex;

	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();

	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
		insert("YEROTHCLIENT", YerothUtils::LATEX_IN_OUT_handleForeignAccents(_clientCompanyName));

	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
		insert("YEROTHVENTESDEBUT", SET_DATE_TO_STRING(_curDateDebutTransactions));

	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
		insert("YEROTHVENTESFIN", SET_DATE_TO_STRING(_curDateFinTransactions));

	return YerothWindowsCommons::imprimer_pdf_document();
}


void YerothTableauDesTransactionsDuClientWindow::setupShortcuts()
{
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
}

