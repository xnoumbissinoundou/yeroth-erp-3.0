/*
 * yeroth-erp-pointdevente-liststocks-window..hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_STOCKS_HISTORIQUE_DU_STOCK_WINDOW_HPP_
#define SRC_YEROTH_STOCKS_HISTORIQUE_DU_STOCK_WINDOW_HPP_

#include "../../ui_yeroth-erp-historique-du-stock-window.h"

#include "src/yeroth-erp-windows.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"



class YerothHistoriqueDuStockWindow : public YerothWindowsCommons,
									  private Ui_YerothHistoriqueDuStockWindow
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothHistoriqueDuStockWindow();

	inline ~YerothHistoriqueDuStockWindow()
	{
		delete _logger;
	}

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return toolBar_historiqueDunStock;
	}

	void listHistoriqueDuStock(const QStringList &aMouvementStockList,
							   const QString	 stockReference,
							   const QString 	 stockID,
							   const QString 	 stockDesignation);

public slots:

	virtual bool imprimer_pdf_document();

private:


    QString						_currentStockID;
    QString						_currentStockReference;
    QString						_currentStockDesignation;

    YerothLogger					*_logger;
};

#endif /* SRC_YEROTH_STOCKS_HISTORIQUE_DU_STOCK_WINDOW_HPP_ */
