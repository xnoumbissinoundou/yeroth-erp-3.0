/*
 * yeroth-erp-ventes-table-view.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_VENTES_TABLE_VIEW_HPP_
#define SRC_YEROTH_VENTES_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPVentesTableView : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPVentesTableView();

	YerothERPVentesTableView(QWidget * parent);

	virtual ~YerothERPVentesTableView();

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel_in_out);


	static const int REFERENCE_COLUMN;

	static const int TYPE_DE_VENTE_COLUMN;
};


#endif /* SRC_YEROTH_VENTES_TABLE_VIEW_HPP_ */
