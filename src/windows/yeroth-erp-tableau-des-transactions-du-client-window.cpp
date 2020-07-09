/*
 * yeroth-erp-tableau-des-transactions-du-client-window.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
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

    _yerothTableView_FROM_WINDOWS_COMMONS = tableView_tableau_des_transactions_du_client;

    _DBFieldNamesToPrintLeftAligned.insert(0);
    _DBFieldNamesToPrintLeftAligned.insert(1);
    _DBFieldNamesToPrintLeftAligned.insert(2);
    _DBFieldNamesToPrintLeftAligned.insert(5);
    _DBFieldNamesToPrintLeftAligned.insert(6);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}")
			.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
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


bool YerothTableauDesTransactionsDuClientWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_transactions_dun_client_tex;

    QMap<QString, QString> documentSpecificElements;

    documentSpecificElements.insert("YEROTHCLIENT", YerothUtils::LATEX_IN_OUT_handleForeignAccents(_clientCompanyName));

    documentSpecificElements.insert("YEROTHVENTESDEBUT", SET_DATE_TO_STRING(_curDateDebutTransactions));

    documentSpecificElements.insert("YEROTHVENTESFIN", SET_DATE_TO_STRING(_curDateFinTransactions));

	return YerothWindowsCommons::imprimer_pdf_document(&documentSpecificElements);
}


