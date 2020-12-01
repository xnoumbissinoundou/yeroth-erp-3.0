/*
 * yeroth-erp-pointdevente-liststocks-window..hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
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
							   const QString 	 stockDesignation);

public slots:

	bool export_csv_file();

	virtual bool imprimer_pdf_document();

protected:

	inline void virtual resizeEvent(QResizeEvent *event)
	{
		tableView_historique_du_stock->resize(event->size());
	}

private:

    QString						_currentStockReference;
    QString						_currentStockDesignation;

    YerothLogger				*_logger;
};

#endif /* SRC_YEROTH_STOCKS_HISTORIQUE_DU_STOCK_WINDOW_HPP_ */
