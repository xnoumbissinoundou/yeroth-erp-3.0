/*
 * yeroth-erp-pointdevente-liststocks-window..hpp
 *
 *  Created on: Dec 5, 2016
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_VENTE_LISTSTOCKS_WINDOW_HPP_
#define SRC_YEROTH_VENTE_LISTSTOCKS_WINDOW_HPP_

#include "../../ui_yeroth-erp-pointdevente-liststocks-window.h"

#include "src/yeroth-erp-windows.hpp"
#include "yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class YerothSqlTableModel;
class YerothPOSUser;

class YerothPointDeVenteListStocksWindow : public YerothWindowsCommons, private Ui_YerothVenteListStocksWindow
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

private:

    static const QString 	_WINDOW_TITLE;

    YerothLogger				*_logger;
};

#endif /* SRC_YEROTH_VENTE_LISTSTOCKS_WINDOW_HPP_ */
