/*
 * yeroth-erp-sortir-liststocks-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_SORTIR_LISTSTOCKS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_SORTIR_LISTSTOCKS_WINDOW_HPP_

#include "../../ui_yeroth-erp-sortir-liststocks-window.h"


#include "yeroth-erp-window-commons.hpp"


class YerothERPWindows;
class YerothSqlTableModel;
class YerothPOSUser;

class YerothSortirListStocksWindow : public YerothWindowsCommons,
									 private Ui_YerothSortirListStocksWindow
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothSortirListStocksWindow();

	inline ~YerothSortirListStocksWindow()
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
		tableView_list_stocks->resize(event->size());
	}

private:

    YerothLogger				*_logger;
};

#endif /* SRC_WINDOWS_YEROTH_SORTIR_LISTSTOCKS_WINDOW_HPP_ */
