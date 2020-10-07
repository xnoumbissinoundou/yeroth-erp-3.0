/*
 * yeroth-erp-historique-du-stock-window.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-historique-du-stock-window.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/qmath.h>


YerothHistoriqueDuStockWindow::YerothHistoriqueDuStockWindow()
:YerothWindowsCommons("yeroth-erp-historique-dun-stock"),
 _logger(new YerothLogger("YerothHistoriqueDuStockWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("historique du stock"));

    setupUi(this);

    mySetupUi(this);

    enableResizing();

    _yerothTableView_FROM_WINDOWS_COMMONS = tableView_historique_du_stock;


    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0,
                                    	 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));

    setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(0);
	setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(1);

    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(2);
    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(3);
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

	tableView_historique_du_stock->resizeColumnsToContents();

	if (tableView_historique_du_stock->rowCount() > 0)
	{
		tableView_historique_du_stock->selectRow(0);
	}

	show();
}


bool YerothHistoriqueDuStockWindow::export_csv_file()
{
	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

	QString aCurrentStockDesignation =
				YerothUtils::YEROTH_REPLACE_ALL_EMPTYSPACES_WITH_UNDERSCORE_CHARACTER(_currentStockDesignation);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_historique_du_stock,
										   tableColumnsToIgnore,
										   QString("yeroth-erp-historique-stock-%1-format-csv")
										   	   .arg(aCurrentStockDesignation),
										   "historique d'un stock");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_historique_du_stock,
										   tableColumnsToIgnore,
										   QString("yeroth-erp-%1-stock-history-csv-format")
										   	   .arg(aCurrentStockDesignation),
										   "stock history");
#endif

	return success;
}


bool YerothHistoriqueDuStockWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_historique_dun_stock_tex;

    QMap<QString, QString> documentSpecificElements;

    documentSpecificElements.insert("YEROTHSTOCKHISTORYREFERENCE",
    		YerothUtils::LATEX_IN_OUT_handleForeignAccents(_currentStockReference));

    documentSpecificElements.insert("YEROTHSTOCKHISTORYDESIGNATION",
    		YerothUtils::LATEX_IN_OUT_handleForeignAccents(_currentStockDesignation));

    documentSpecificElements.insert("YEROTHSTOCKHISTORYSTOCKID", _currentStockID);

	return YerothWindowsCommons::imprimer_pdf_document(&documentSpecificElements);
}

