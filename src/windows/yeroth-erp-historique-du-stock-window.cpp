/*
 * yeroth-erp-pointdevente-liststocks-window.cpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#include "yeroth-erp-historique-du-stock-window.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/qmath.h>


const QString YerothHistoriqueDuStockWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("historique du stock")));


YerothHistoriqueDuStockWindow::YerothHistoriqueDuStockWindow()
:YerothWindowsCommons(YerothHistoriqueDuStockWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothHistoriqueDuStockWindow"))
{
    setupUi(this);

    this->mySetupUi(this);

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_document()));
}


void YerothHistoriqueDuStockWindow::listHistoriqueDuStock(const QStringList &aMouvementStockList,
														  const QString	    stockReference,
														  const QString 	stockID,
														  const QString 	stockDesignation)
{
	QString curTitle(windowTitle());

	_currentStockID = stockID;

	_currentStockReference = stockReference;

	_currentStockDesignation = stockDesignation;

	//qDebug() << QString("curTitle: %1")
	//				.arg(curTitle);

	static QString preambleTitle(QString("%1 - %2 -")
									.arg(YEROTH_ERP_WINDOW_TITLE,
										 "historique du stock"));

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

	preambleTitle.append(QString(" ID (%1), stock \"%2\"")
						.arg(stockID, stockDesignation));

	setWindowTitle(preambleTitle);

	tableView_historique_du_stock->lister_lhistorique_du_stock(aMouvementStockList);

	tableView_historique_du_stock->hideColumn(3);

	show();
}


void YerothHistoriqueDuStockWindow::getStockHistoryListingTexTableString(QString &texTable_in_out,
       																	 QStandardItemModel &tableStandardItemModel,
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
    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        texTable_in_out.append("c|");
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


bool YerothHistoriqueDuStockWindow::imprimer_document()
{
	_logger->log("imprimer_document");

	QString latexFileNamePrefix("yeroth-erp-historique-dun-stock");

	QStandardItemModel *tableModel = 0;

	QList<int> columnsToIgnore;

	tableModel = tableView_historique_du_stock->getStandardItemModel();

	columnsToIgnore << 3;

	int tableModelRowCount = tableView_historique_du_stock->rowCount();

	if (tableModelRowCount <= 0)
	{
		YerothQMessageBox::information(this, QObject::trUtf8("impression"),
				FROM_UTF8_STRING(tr("Il n'y a pas de données à imprimer !")));
		return false;
	}

	QString texTable;

	int fromRowIndex = 0;

	int toRowIndex = tableModelRowCount;

	double MAX_TABLE_ROW_COUNT = 35.0;

	int pageNumber = qCeil(tableModelRowCount / MAX_TABLE_ROW_COUNT);

	//qDebug() << QString("number of pages to print: %1").arg(pageNumber);
	//_logger->log("imprimer_document",
	//                  QString("number of pages to print: %1").arg(pageNumber));
	getStockHistoryListingTexTableString(texTable, *tableModel, columnsToIgnore, 0,
			(20 >= tableModelRowCount) ? tableModelRowCount : 20,
					tableModelRowCount <= 20);

	//qDebug() << "++ texTable: " << texTable;
	if (tableModelRowCount >= 20)
	{
		fromRowIndex = 20;

		toRowIndex = (fromRowIndex >= tableModelRowCount) ? fromRowIndex : fromRowIndex + MAX_TABLE_ROW_COUNT;

		int k = 1;

		do
		{
			//qDebug() << QString("## fromRowIndex: %1, toRowIndex: %2")
			//  .arg(QString::number(fromRowIndex), QString::number(toRowIndex));
			getStockHistoryListingTexTableString(texTable, *tableModel, columnsToIgnore,
														  (fromRowIndex >= tableModelRowCount) ? tableModelRowCount :
														  fromRowIndex,
														  (toRowIndex >= tableModelRowCount) ? (tableModelRowCount + 1) : toRowIndex,
														  k == pageNumber);
			texTable.append(QString("\\newpage \n"));

			fromRowIndex = toRowIndex;

			toRowIndex =
					(fromRowIndex >= tableModelRowCount) ? (fromRowIndex + 1) : fromRowIndex + MAX_TABLE_ROW_COUNT;

			++k;
		}
		while (k <= pageNumber && fromRowIndex <= toRowIndex);
	}

	YerothInfoEntreprise & infoEntreprise = _allWindows->getInfoEntreprise();

	QString texDocument;

	QString factureDate(YerothUtils::handleForeignAccents(infoEntreprise.getVilleTex()));

	YerothUtils::getCurrentLocaleDate(factureDate);

#ifdef YEROTH_FRANCAIS_LANGUAGE

	YerothUtils::getLatexStockHistoryFR(texDocument, texTable);

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

	YerothUtils::getLatexStockHistoryEN(texDocument, texTable);

#endif

	texDocument.replace("YEROTHPAPERSPEC", "a4paper");

	texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
	texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
	texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
	texDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
	texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
	texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
	texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
	texDocument.replace("YEROTHDATE", factureDate);
	texDocument.replace("YEROTHNOMUTILISATEUR", _allWindows->getUser()->nom_completTex());
	texDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
	texDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
	texDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
	texDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());

	texDocument.replace("YEROTHSTOCKHISTORYREFERENCE", _currentStockReference);
	texDocument.replace("YEROTHSTOCKHISTORYDESIGNATION", _currentStockDesignation);
	texDocument.replace("YEROTHSTOCKHISTORYSTOCKID", _currentStockID);

	QString yerothstockHistoryFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE
		yerothstockHistoryFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-historique-dun-stock"));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
		yerothstockHistoryFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-stock-history"));
#endif

	QFile tmpLatexFile(QString("%1tex").arg(yerothstockHistoryFileName));

	YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
													texDocument);

	QString pdfStockHistoryFileName(YerothERPProcess::compileLatex(yerothstockHistoryFileName));

	YerothERPProcess::startPdfViewerProcess(pdfStockHistoryFileName);

	return true;
}


