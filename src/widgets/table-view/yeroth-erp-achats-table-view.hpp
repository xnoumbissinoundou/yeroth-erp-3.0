/*
 * yeroth-erp-achats-table-view.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ACHATS_TABLE_VIEW_HPP_
#define SRC_YEROTH_ACHATS_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-paging.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPAchatsTableView : public YerothTableViewWITHpaging
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPAchatsTableView()
	:YerothTableViewWITHpaging()
	{
		_stdItemModel->_curTableView = this;
	}

	inline YerothERPAchatsTableView(QWidget * parent)
	:YerothTableViewWITHpaging(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPAchatsTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &achatSqlTableModel,
												bool execSelectStatement = true);
};


#endif /* SRC_YEROTH_ACHATS_TABLE_VIEW_HPP_ */
