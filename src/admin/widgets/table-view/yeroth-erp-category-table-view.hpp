/*
 * yeroth-erp-category-table-view.hpp
 *
 * Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_ADMIN_CATEGORY_TABLE_VIEW_HPP_
#define SRC_YEROTH_ADMIN_CATEGORY_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPCategoryTableView : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPCategoryTableView();

	YerothERPCategoryTableView(QWidget * parent);

	virtual ~YerothERPCategoryTableView();

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &clientSqlTableModel);
};


#endif /* SRC_YEROTH_ADMIN_CATEGORY_TABLE_VIEW_HPP_ */
