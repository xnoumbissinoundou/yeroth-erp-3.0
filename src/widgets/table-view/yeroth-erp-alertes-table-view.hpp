/*
 * yeroth-erp-alertes-table-view.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ALERTES_TABLE_VIEW_HPP_
#define SRC_YEROTH_ALERTES_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPAlertesTableView : public YerothTableViewWITHpagination
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPAlertesTableView()
	:YerothTableViewWITHpagination()
	{
		_stdItemModel->_curTableView = this;
	}

	inline YerothERPAlertesTableView(QWidget * parent)
	:YerothTableViewWITHpagination(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPAlertesTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &alertesSqlTableModel);
};


#endif /* SRC_YEROTH_ALERTES_TABLE_VIEW_HPP_ */
