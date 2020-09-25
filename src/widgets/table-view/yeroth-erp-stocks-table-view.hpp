/*
 * yeroth-erp-stocks-table-view.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_STOCKS_TABLE_VIEW_HPP_
#define SRC_YEROTH_STOCKS_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPStocksTableView : public YerothTableViewWITHpagination
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPStocksTableView()
	:YerothTableViewWITHpagination()
	{
		_stdItemModel->_curTableView = this;
	}


	inline YerothERPStocksTableView(QWidget * parent)
	:YerothTableViewWITHpagination(parent)
	{
		_stdItemModel->_curTableView = this;
	}

	inline virtual ~YerothERPStocksTableView()
	{
	}

	virtual void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel,
												QString aStockListingStrategy = YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

private:

	static QMultiMap<QString, QString> _DUMMY_STOCKNAME_TO_STOCKID_IN_OUT;
};


#endif /* SRC_YEROTH_STOCKS_TABLE_VIEW_HPP_ */
