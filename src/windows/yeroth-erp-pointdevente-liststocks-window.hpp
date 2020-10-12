/*
 * yeroth-erp-pointdevente-liststocks-window.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_VENTE_LISTSTOCKS_WINDOW_HPP_
#define SRC_YEROTH_VENTE_LISTSTOCKS_WINDOW_HPP_

#include "../../ui_yeroth-erp-pointdevente-liststocks-window.h"

#include "src/yeroth-erp-windows.hpp"

#include "yeroth-erp-window-commons.hpp"


class YerothERPWindows;
class YerothSqlTableModel;
class YerothPOSUser;


class YerothPointDeVenteListStocksWindow : public YerothWindowsCommons,
										   private Ui_YerothVenteListStocksWindow
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothPointDeVenteListStocksWindow();

	inline ~YerothPointDeVenteListStocksWindow()
	{
		delete _logger;
	}

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return 0;
	}

	void listStocks(YerothSqlTableModel &aSqlTableModel);

protected:

	inline void virtual resizeEvent(QResizeEvent *event)
	{
		tableView_PDV_list_stocks->resize(event->size());
	}

private:

    YerothLogger				*_logger;
};

#endif /* SRC_YEROTH_VENTE_LISTSTOCKS_WINDOW_HPP_ */
