/*
 * yeroth-erp-marchandises-table-view.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_MARCHANDISES_TABLE_VIEW_HPP_
#define SRC_YEROTH_MARCHANDISES_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>


class YerothERPWindows;

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;


class YerothERPMarchandisesTableView : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPMarchandisesTableView();

	YerothERPMarchandisesTableView(QWidget * parent);

	inline virtual ~YerothERPMarchandisesTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel_in_out,
												YerothWindowsCommons *aCallingWindows);

private:

    YerothERPWindows *_allWindows;
};


#endif /* SRC_YEROTH_MARCHANDISES_TABLE_VIEW_HPP_ */
