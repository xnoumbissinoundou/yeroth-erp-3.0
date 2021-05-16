/*
 * yeroth-erp-ventes-table-view.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_VENTES_TABLE_VIEW_HPP_
#define SRC_YEROTH_VENTES_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;


class YerothERPVentesTableView :
		public YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPVentesTableView()
	:YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
	{
		_stdItemModel->_curTableView = this;
	}

	inline YerothERPVentesTableView(QWidget * parent)
	:YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPVentesTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel);

protected:

	virtual void selectionChanged(const QItemSelection & selected,
								  const QItemSelection & deselected);
};


#endif /* SRC_YEROTH_VENTES_TABLE_VIEW_HPP_ */
