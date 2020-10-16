/*
 * yeroth-erp-paiements-table-view.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_PAIEMENTS_TABLE_VIEW_HPP_
#define SRC_YEROTH_PAIEMENTS_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPPaiementsTableView : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPPaiementsTableView()
	:YerothTableView()
	{
		_stdItemModel->_curTableView = this;
	}

	inline YerothERPPaiementsTableView(QWidget * parent)
	:YerothTableView(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPPaiementsTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel);
};


#endif /* SRC_YEROTH_PAIEMENTS_TABLE_VIEW_HPP_ */
