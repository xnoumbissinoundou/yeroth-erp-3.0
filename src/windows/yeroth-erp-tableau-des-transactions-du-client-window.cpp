/*
 * yeroth-erp-tableau-des-transactions-du-client-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-tableau-des-transactions-du-client-window.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/qmath.h>


const QString YerothTableauDesTransactionsDuClientWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("transactions financières d'un client")));


YerothTableauDesTransactionsDuClientWindow::YerothTableauDesTransactionsDuClientWindow()
:YerothWindowsCommons(YerothTableauDesTransactionsDuClientWindow::_WINDOW_TITLE)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}")
			.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
}


void YerothTableauDesTransactionsDuClientWindow::listerTransactionsDunClient(QString clientCompanyName,
																			 QSqlQuery &sqlClientTransactionsUnionQuery)
{
	_clientCompanyName = clientCompanyName;

	QString curTitle(windowTitle());

	static QString preambleTitle(QString("%1 - %2")
									.arg(YEROTH_ERP_WINDOW_TITLE,
										 QObject::tr("transactions d'un client")));

	static bool first_time_inside = true;

	static int preambleTitleLength = preambleTitle.length();

	if (!first_time_inside)
	{
		int len = curTitle.length() - preambleTitleLength;

		preambleTitle.remove(preambleTitleLength, len);

		//qDebug() << QString("preambleTitle: %1")
		//				.arg(preambleTitle);
	}
	else
	{
		first_time_inside = false;
	}

//	preambleTitle.append(QString(" client (%1)")
//						.arg(stockID));

	setWindowTitle(preambleTitle);

	tableView_tableau_des_transactions_du_client
			->lister_les_transactions_dun_client(sqlClientTransactionsUnionQuery);

	tableView_tableau_des_transactions_du_client->resizeColumnsToContents();

	if (tableView_tableau_des_transactions_du_client->rowCount() > 0)
	{
		tableView_tableau_des_transactions_du_client->selectRow(0);
	}

	show();
}


void YerothTableauDesTransactionsDuClientWindow::getTransactionsDunClientTexTableString(QString &texTable_in_out,
       																	 QStandardItemModel &tableStandardItemModel,
																		 QList<int> &dbFieldNameOfTypeString,
																		 QList<int> &columnsToIgnore,
																		 int fromRowIndex,
																		 int toRowIndex,
																		 bool lastPage)
{
    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    .append("\\centering").append("\n")
    .append("\\begin{tabular}")
    .append("{|");

    int texTableColumnCount = tableStandardItemModel.columnCount();

    //Tex table header
    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        if (3 == k ||
		    4 == k )
        {
        	texTable_in_out.append("r|");
        }
        else
        {
        	texTable_in_out.append("c|");
        }
    }

    texTable_in_out.append("} \\hline").append("\n");

    QStandardItem *item;

    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);
        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{").append("}"));
            YerothUtils::handleTexTableItemText(tableStandardItemModel.columnCount(),
                                   texTable_in_out,
                                   k,
                                   itemText);
        }
    }
    /** Closing Tex table header */
    YerothUtils::cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append("\\hline\n");


    for (int j = 0; j < tableStandardItemModel.rowCount(); ++j)
    {
        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }

            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                QString itemText(item->text());
                YerothUtils::handleFactureTexTableItemText(tableStandardItemModel.columnCount(),
                                              texTable_in_out,
                                              k,
                                              itemText);
            }
            else
            {
                if (k < tableStandardItemModel.columnCount() - 1)
                {
                    texTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    texTable_in_out.append("\"\"").append("\\\\ \\hline\n");
                }
            }
        }

        texTable_in_out = texTable_in_out.trimmed();

        YerothUtils::cleanUpTexTableLastString(texTable_in_out);

        texTable_in_out.append("\\hline\n");
    }

    //Removes the empty character "" from Latex output
    texTable_in_out.replace("\"\"", "");

    texTable_in_out.append("\\end{tabular}").append("\n")
    .append("\\end{table*}").append("\n");

    //qDebug() << "++ texTable_in_out in getStocksListingTexTableString: " << texTable_in_out;
}


void YerothTableauDesTransactionsDuClientWindow::getTransactionsDunClientTexDocumentString(QString &texDocumentString_in_out,
        											   	   	    						   QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(YerothUtils::template_transactions_dun_client_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


bool YerothTableauDesTransactionsDuClientWindow::imprimer_pdf_document()
{
	QString latexFileNamePrefix("yeroth-erp-tableau-des-transactions-dun-client");

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    QString pdfStockFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-tableau-des-transactions-dun-client");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-customer-transaction-table");
#endif

    QMap<QString, QString> documentSpecificElements;

    documentSpecificElements.insert("YEROTHCLIENT", YerothUtils::LATEX_IN_OUT_handleForeignAccents(_clientCompanyName));
    documentSpecificElements.insert("YEROTHVENTESFIN", SET_DATE_TO_STRING(GET_CURRENT_DATE));
    documentSpecificElements.insert("YEROTHVENTESDEBUT", SET_DATE_TO_STRING(GET_CURRENT_DATE));


    pdfStockFileName = YerothUtils::prindDocumentFromTableView(this,
    														   *tableView_tableau_des_transactions_du_client,
															   tableColumnsToIgnore,
															   &YerothTableauDesTransactionsDuClientWindow::getTransactionsDunClientTexTableString,
															   &YerothTableauDesTransactionsDuClientWindow::getTransactionsDunClientTexDocumentString,
															   latexFileNamePrefix,
															   &documentSpecificElements);

    if (pdfStockFileName.isEmpty())
    {
    	return false;
    }

    YerothERPProcess::startPdfViewerProcess(pdfStockFileName);

    return true;
}


