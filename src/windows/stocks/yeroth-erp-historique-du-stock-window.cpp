/*
 * yeroth-erp-historique-du-stock-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
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

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_historique_du_stock, false);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0,
                                    	 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this, SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(print_PDF_PREVIOUSLY_SETUP()));


    setupShortcuts();


    setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(0);
	setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(1);

    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(2);
    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(3);
}


void YerothHistoriqueDuStockWindow::listHistoriqueDuStock(const QStringList &aMouvementStockList,
														  const QString	    stockReference,
														  const QString 	stockDesignation)
{
	QString curTitle(windowTitle());

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
						.arg(YerothWindowsCommons::get_last_lister_selected_row_ID(),
							 stockDesignation));

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

	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();

	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
		insert("YEROTHSTOCKHISTORYREFERENCE",
    		   YerothUtils::LATEX_IN_OUT_handleForeignAccents(_currentStockReference));

	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
		insert("YEROTHSTOCKHISTORYDESIGNATION",
			   YerothUtils::LATEX_IN_OUT_handleForeignAccents(_currentStockDesignation));

	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
		insert("YEROTHSTOCKHISTORYSTOCKID",
			   YerothWindowsCommons::getLastListerSelectedRow__ID());

	return YerothWindowsCommons::imprimer_pdf_document();
}


void YerothHistoriqueDuStockWindow::setupShortcuts()
{
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
}

