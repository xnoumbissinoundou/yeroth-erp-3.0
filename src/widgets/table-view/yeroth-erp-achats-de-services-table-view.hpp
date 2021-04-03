/*
 * yeroth-erp-achats-de-services-table-view.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ACHATS_DE_SERVICES_TABLE_VIEW_HPP_
#define SRC_YEROTH_ACHATS_DE_SERVICES_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPAchatsAUXFournisseursTableView : public YerothTableViewWITHpagination
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPAchatsAUXFournisseursTableView()
	:YerothTableViewWITHpagination()
	{
		_stdItemModel->_curTableView = this;
	}

	inline YerothERPAchatsAUXFournisseursTableView(QWidget * parent)
	:YerothTableViewWITHpagination(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPAchatsAUXFournisseursTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &achatAUXFournisseursSqlTableModel);

protected:

	virtual void selectionChanged(const QItemSelection & selected,
								  const QItemSelection & deselected);
};


#endif /* SRC_YEROTH_ACHATS_DE_SERVICES_TABLE_VIEW_HPP_ */
