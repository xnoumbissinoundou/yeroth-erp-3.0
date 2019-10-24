/*
 * yeroth-erp-marchandises-table-view.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_MARCHANDISES_TABLE_VIEW_HPP_
#define SRC_YEROTH_MARCHANDISES_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPMarchandisesTableView : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPMarchandisesTableView();

	YerothERPMarchandisesTableView(QWidget * parent);

	virtual ~YerothERPMarchandisesTableView();

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel_in_out);


	static const int REFERENCE_COLUMN;

	static const int VALEUR_DINVENTAIRE_COLUMN;

    static const int QUANTITE_TOTAL_COLUMN;
};


#endif /* SRC_YEROTH_MARCHANDISES_TABLE_VIEW_HPP_ */
