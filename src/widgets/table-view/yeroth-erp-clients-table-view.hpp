/*
 * yeroth-erp-clients-table-view.hpp
 *
 * Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_CLIENTS_TABLE_VIEW_HPP_
#define SRC_YEROTH_CLIENTS_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPClientsTableView : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPClientsTableView();

	YerothERPClientsTableView(QWidget * parent);

	virtual ~YerothERPClientsTableView();

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &clientSqlTableModel);


	static const int REFERENCE_CLIENT_COLUMN;
};


#endif /* SRC_YEROTH_CLIENTS_TABLE_VIEW_HPP_ */
