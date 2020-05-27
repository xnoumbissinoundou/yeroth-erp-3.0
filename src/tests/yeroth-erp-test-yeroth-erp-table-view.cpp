/*
 * yeroth-erp-test-yeroth-erp-windows.cpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#include "yeroth-erp-test-yeroth-erp-table-view.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


void Test_YerothERPTableView::test_TABLE_VIEW_lister_fefo()
{

}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_fifo()
{
	YerothTableView a_test_TableView_object;

	QMap<QString, QString> stockNameToStockID_in_out;

	a_test_TableView_object.lister_FIFO(_allWindows->getSqlTableModel_stocks(), stockNameToStockID_in_out);

	qDebug() << "++ stockNameToStockID_in_out: " << stockNameToStockID_in_out;

	QVERIFY(stockNameToStockID_in_out.size() > 0);
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_lifo()
{

}
