/*
 * yeroth-erp-alert-table-view.hpp
 *
 * Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ADMIN_ALERT_TABLE_VIEW_HPP_
#define SRC_YEROTH_ADMIN_ALERT_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPAlertTableView : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPAlertTableView()
	:YerothTableView()
	{
		_stdItemModel->_curTableView = this;
	}

	inline YerothERPAlertTableView(QWidget * parent)
	:YerothTableView(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPAlertTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &clientSqlTableModel,
												QString aStockListingStrategy = "");
};


#endif /* SRC_YEROTH_ADMIN_ALERT_TABLE_VIEW_HPP_ */
