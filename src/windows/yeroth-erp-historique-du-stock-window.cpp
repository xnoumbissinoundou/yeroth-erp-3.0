/*
 * yeroth-erp-historique-du-stock-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-historique-du-stock-window.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/qmath.h>


const QString YerothHistoriqueDuStockWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2"))
																.arg(YEROTH_ERP_WINDOW_TITLE,
																	 QObject::trUtf8("historique du stock")));


YerothHistoriqueDuStockWindow::YerothHistoriqueDuStockWindow()
:YerothWindowsCommons(YerothHistoriqueDuStockWindow::_WINDOW_TITLE,
					  "yeroth-erp-historique-dun-stock"),
 _logger(new YerothLogger("YerothHistoriqueDuStockWindow"))
{
    setupUi(this);

    mySetupUi(this);

    _yerothTableView_FROM_WINDOWS_COMMONS = tableView_historique_du_stock;

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0,
                                    	 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
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

