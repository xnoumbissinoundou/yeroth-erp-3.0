/*
 * yeroth-erp-groupes-de-clients-table-view.hpp
 *
 * Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_GROUPES_DE_CLIENTS_TABLE_VIEW_HPP_
#define SRC_YEROTH_GROUPES_DE_CLIENTS_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>


class YerothSqlTableModel;

class YerothPOSQStandardItemModel;


class YerothERPGroupesDeClientsTableView : public YerothTableViewWITHpagination
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPGroupesDeClientsTableView()
	:YerothTableViewWITHpagination()
	{
		_stdItemModel->_curTableView = this;
	}


	inline YerothERPGroupesDeClientsTableView(QWidget * parent)
	:YerothTableViewWITHpagination(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPGroupesDeClientsTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &clientGroupSqlTableModel);

protected:

	virtual void selectionChanged(const QItemSelection & selected,
								  const QItemSelection & deselected);
};


#endif /* SRC_YEROTH_GROUPES_DE_CLIENTS_TABLE_VIEW_HPP_ */
