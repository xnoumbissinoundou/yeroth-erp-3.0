/*
 * yeroth-erp-test-yeroth-erp-windows.cpp
 *
 *      Author: XAVIER NOUMBISSI NOUNDOU, DR., PH.D. (ABD)
 */

#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST


#include "yeroth-erp-test-yeroth-erp-table-view.hpp"


#include "src/tests/utils/yeroth-erp-test-import-csv-file-data.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtTest/QtTest>


Test_YerothERPTableView::Test_YerothERPTableView()
:_yerothERPTestImportCSVFileData(0),
 _allWindows(0)
{
	_yerothERPTestImportCSVFileData = new Test_YerothERPTestImportCSVFileData;

	_allWindows = Test_YerothERPTestUtils::getAllWindows();
}


void Test_YerothERPTableView::initTestCase()
{
	Test_YerothERPTestUtils::TEST_UTILS_truncate_database_all_tables();

	_yerothERPTestImportCSVFileData->
		test_TABLE_VIEW_lister_import_test_data("yeroth_test_data_stock_1.csv");
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_fefo()
{
	YerothTableView a_test_TableView_object;

	QMap<QString, QString> stockNameToStockID_in_out;

	YerothSqlTableModel &aQSqlStockTableModel = _allWindows->getSqlTableModel_stocks();

	a_test_TableView_object.lister_FEFO(aQSqlStockTableModel, stockNameToStockID_in_out);

	qDebug() << "++ stockNameToStockID_in_out: " << stockNameToStockID_in_out;

	int TEST_VAR_QMAP_STOCK_RESULT_COUNT = stockNameToStockID_in_out.size();

	QVERIFY(TEST_VAR_QMAP_STOCK_RESULT_COUNT == 1);

	int stockID = stockNameToStockID_in_out.value("test_yeroth_1").toInt();

	QVERIFY(stockID == 2);
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_fifo()
{
	YerothTableView a_test_TableView_object;

	QMap<QString, QString> stockNameToStockID_in_out;

	YerothSqlTableModel &aQSqlStockTableModel = _allWindows->getSqlTableModel_stocks();

	a_test_TableView_object.lister_FIFO(aQSqlStockTableModel, stockNameToStockID_in_out);

	qDebug() << "++ stockNameToStockID_in_out: " << stockNameToStockID_in_out;

	int TEST_VAR_QMAP_STOCK_RESULT_COUNT = stockNameToStockID_in_out.size();

	QVERIFY(TEST_VAR_QMAP_STOCK_RESULT_COUNT == 1);

	int stockID = stockNameToStockID_in_out.value("test_yeroth_1").toInt();

	QVERIFY(stockID == 1);
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_lifo()
{
	YerothTableView a_test_TableView_object;

	QMap<QString, QString> stockNameToStockID_in_out;

	YerothSqlTableModel &aQSqlStockTableModel = _allWindows->getSqlTableModel_stocks();

	a_test_TableView_object.lister_LIFO(aQSqlStockTableModel, stockNameToStockID_in_out);

	qDebug() << "++ stockNameToStockID_in_out: " << stockNameToStockID_in_out;

	int TEST_VAR_QMAP_STOCK_RESULT_COUNT = stockNameToStockID_in_out.size();

	QVERIFY(TEST_VAR_QMAP_STOCK_RESULT_COUNT == 1);

	int stockID = stockNameToStockID_in_out.value("test_yeroth_1").toInt();

	QVERIFY(stockID == 3);
}


#endif /* YEROTH_ERP_3_0_TESTING_UNIT_TEST */

