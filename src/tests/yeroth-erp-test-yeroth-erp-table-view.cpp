/*
 * yeroth-erp-test-yeroth-erp-windows.cpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#include "yeroth-erp-test-yeroth-erp-table-view.hpp"


#include "src/tests/utils/yeroth-erp-test-utils.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtTest/QtTest>


void Test_YerothERPTableView::initTestCase()
{
	QVERIFY(0 != _allWindows);

	Test_YerothERPTestUtils::TEST_UTILS_truncate_database_all_tables();

	test_TABLE_VIEW_lister_import_test_data();
}


void Test_YerothERPTableView::cleanupTestCase()
{
    QMapIterator<int, YerothERPDatabaseTableColumnInfo *>
    	_it_TEST_csvContentIdxToDatabaseTableColumnInfo(_TEST_csvContentIdxToDatabaseTableColumnInfo);

    YerothERPDatabaseTableColumnInfo *anERPDatabaseTableColumnInfo = 0;

    while(_it_TEST_csvContentIdxToDatabaseTableColumnInfo.hasNext())
    {
    	_it_TEST_csvContentIdxToDatabaseTableColumnInfo.next();

    	anERPDatabaseTableColumnInfo = _it_TEST_csvContentIdxToDatabaseTableColumnInfo.value();

    	if (0 != anERPDatabaseTableColumnInfo)
    	{
    		delete anERPDatabaseTableColumnInfo;
    	}
    }
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


void Test_YerothERPTableView::test_TABLE_VIEW_lister_import_test_data()
{
    QString csvFilePath(QString("%1/yeroth-erp-3-0-test_data/%2")
    						.arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER,
    							 "yeroth_test_data_stock_1.csv"));

    if (!csvFilePath.isEmpty())
    {
    	bool importSuccess = YerothUtils::import_csv_file_content(csvFilePath, _curCsvFileToImportContentWordList);

    	QVERIFY(importSuccess == true);
    }

	YerothERPStockImport erpStockImport(_curCsvFileToImportContentWordList,
										_TEST_csvContentIdxToDatabaseTableColumnInfo);




	QString curSqlTableImportHeaderStr;

    YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

	QStringList csvHeader = _curCsvFileToImportContentWordList.at(0)
					.split(YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

    int curCsvFileColumnSize = csvHeader.size();

	YerothUtils::fillDBTableColumnNameToDBTableColumnType_TEST(_allWindows->STOCKS,
															   _dbTableColumnToType,
															   _dbTableColumnToIsNotNULL);

    YerothERPDatabaseTableColumnInfo *anERPDatabaseTableColumnInfo = 0;

    QString aDBColumnName;

    for (int kTEST = 0; kTEST < curCsvFileColumnSize; ++kTEST)
    {
    	aDBColumnName = csvHeader.at(kTEST).trimmed();

    	anERPDatabaseTableColumnInfo = new YerothERPDatabaseTableColumnInfo(aDBColumnName,
    												_dbTableColumnToType.value(aDBColumnName));

    	_TEST_csvContentIdxToDatabaseTableColumnInfo.insert(kTEST, anERPDatabaseTableColumnInfo);
    }

    //	qDebug() << "++ _TEST_csvContentIdxToDatabaseTableColumnInfo: " << _TEST_csvContentIdxToDatabaseTableColumnInfo;

	int successImportCount = erpStockImport.import();

    QVERIFY(successImportCount > 0);
}

