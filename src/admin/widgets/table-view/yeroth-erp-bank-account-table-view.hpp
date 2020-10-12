/*
 * yeroth-erp-bank-account-table-view.hpp
 *
 * Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ADMIN_BANK_ACCOUNT_TABLE_VIEW_HPP_
#define SRC_YEROTH_ADMIN_BANK_ACCOUNT_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPBankAccountTableView : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPBankAccountTableView()
	:YerothTableView()
	{
		_stdItemModel->_curTableView = this;
	}

	YerothERPBankAccountTableView(QWidget * parent)
	:YerothTableView(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPBankAccountTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &clientSqlTableModel,
												QString aStockListingStrategy = "");
};


#endif /* SRC_YEROTH_ADMIN_BANK_ACCOUNT_TABLE_VIEW_HPP_ */
