/*
 * yeroth-erp-customer-table-view.hpp
 *
 * Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_ADMIN_CUSTOMER_TABLE_VIEW_HPP_
#define SRC_YEROTH_ADMIN_CUSTOMER_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPCustomerTableView : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPCustomerTableView();

	YerothERPCustomerTableView(QWidget * parent);

	virtual ~YerothERPCustomerTableView();

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &clientSqlTableModel);
};


#endif /* SRC_YEROTH_ADMIN_CUSTOMER_TABLE_VIEW_HPP_ */
