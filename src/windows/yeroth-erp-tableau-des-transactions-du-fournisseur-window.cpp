/*
 * yeroth-erp-tableau-des-transactions-du-fournisseur-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-tableau-des-transactions-du-fournisseur-window.hpp"


#include "src/process/yeroth-erp-process.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/qmath.h>


YerothTableauDesTransactionsDuFournisseurWindow::YerothTableauDesTransactionsDuFournisseurWindow()
:YerothWindowsCommons("yeroth-erp-tableau-des-transactions-dun-fournisseur")
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("transactions financières d'un fournisseur"));

    setupUi(this);

    mySetupUi(this);

    enableResizing();

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_tableau_des_transactions_du_fournisseur);

    _DBFieldNamesToPrintLeftAligned.insert(0);
    _DBFieldNamesToPrintLeftAligned.insert(1);
    _DBFieldNamesToPrintLeftAligned.insert(2);
    _DBFieldNamesToPrintLeftAligned.insert(5);
    _DBFieldNamesToPrintLeftAligned.insert(6);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}")
			.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);


    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
}


void YerothTableauDesTransactionsDuFournisseurWindow::listerTransactionsDunFournisseur(QDate dateDebutTransactions,
																			 QDate dateFinTransactions,
																			 QString fournisseurCompanyName,
																			 QSqlQuery &sqlFournisseurTransactionsUnionQuery)
{
	_curDateDebutTransactions = dateDebutTransactions;

	_curDateFinTransactions = dateFinTransactions;

	_fournisseurCompanyName = fournisseurCompanyName;

	QString curWindowTitle(QString("%1 - %2 - '%3'")
								.arg(YEROTH_ERP_WINDOW_TITLE,
									 QObject::tr("transactions d'un fournisseur"),
									 fournisseurCompanyName));

	setWindowTitle(curWindowTitle);

	tableView_tableau_des_transactions_du_fournisseur
			->lister_les_transactions_dun_fournisseur(sqlFournisseurTransactionsUnionQuery);

	tableView_tableau_des_transactions_du_fournisseur->resizeColumnsToContents();

	if (tableView_tableau_des_transactions_du_fournisseur->rowCount() > 0)
	{
		tableView_tableau_des_transactions_du_fournisseur->selectRow(0);
	}

	show();
}


bool YerothTableauDesTransactionsDuFournisseurWindow::export_csv_file()
{
	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

	QString aNewFournisseurCompanyName =
				YerothUtils::YEROTH_REPLACE_ALL_EMPTYSPACES_WITH_UNDERSCORE_CHARACTER(_fournisseurCompanyName);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_tableau_des_transactions_du_fournisseur,
										   tableColumnsToIgnore,
										   QString("yeroth-erp-transaction-fournisseur-%1-format-csv")
										   	   .arg(aNewFournisseurCompanyName),
										   "transactions d'un client");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_tableau_des_transactions_du_fournisseur,
										   tableColumnsToIgnore,
										   QString("yeroth-erp-%1-supplier-transaction-csv-format")
										   	   .arg(aNewFournisseurCompanyName),
										   "client transaction");
#endif

	return success;
}


bool YerothTableauDesTransactionsDuFournisseurWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_transactions_dun_fournisseur_tex;

    QMap<QString, QString> documentSpecificElements;

    documentSpecificElements.insert("YEROTHFOURNISSEUR", YerothUtils::LATEX_IN_OUT_handleForeignAccents(_fournisseurCompanyName));

    documentSpecificElements.insert("YEROTHVENTESDEBUT", SET_DATE_TO_STRING(_curDateDebutTransactions));

    documentSpecificElements.insert("YEROTHVENTESFIN", SET_DATE_TO_STRING(_curDateFinTransactions));

	return YerothWindowsCommons::imprimer_pdf_document(&documentSpecificElements);
}


