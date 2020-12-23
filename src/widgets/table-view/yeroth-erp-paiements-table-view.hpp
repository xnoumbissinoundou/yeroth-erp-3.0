/*
 * yeroth-erp-paiements-table-view.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_PAIEMENTS_TABLE_VIEW_HPP_
#define SRC_YEROTH_PAIEMENTS_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPPaiementsTableView : public YerothTableViewWITHpagination
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPPaiementsTableView()
	:YerothTableViewWITHpagination()
	{
		_stdItemModel->_curTableView = this;
	}

	inline YerothERPPaiementsTableView(QWidget * parent)
	:YerothTableViewWITHpagination(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPPaiementsTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel);

protected:

	virtual void selectionChanged(const QItemSelection & selected,
								  const QItemSelection & deselected);
};


#endif /* SRC_YEROTH_PAIEMENTS_TABLE_VIEW_HPP_ */
