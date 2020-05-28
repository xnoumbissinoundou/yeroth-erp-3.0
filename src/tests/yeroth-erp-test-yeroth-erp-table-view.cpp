/*
 * yeroth-erp-test-yeroth-erp-windows.cpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#include "yeroth-erp-test-yeroth-erp-table-view.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"

#include "src/imports/yeroth-erp-stock-import.hpp"


void Test_YerothERPTableView::initTestCase()
{
	QVERIFY(0 != _allWindows);

	test_TABLE_VIEW_lister_import_test_data();
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_fefo()
{
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_fifo()
{
	YerothTableView a_test_TableView_object;

	QMap<QString, QString> stockNameToStockID_in_out;

	YerothSqlTableModel &aQSqlStockTableModel = _allWindows->getSqlTableModel_stocks();

	a_test_TableView_object.lister_FIFO(aQSqlStockTableModel, stockNameToStockID_in_out);

	qDebug() << "++ stockNameToStockID_in_out: " << stockNameToStockID_in_out;

	int TEST_VAR_QMAP_STOCK_RESULT_COUNT = stockNameToStockID_in_out.size();

	QVERIFY(TEST_VAR_QMAP_STOCK_RESULT_COUNT > 0);

	QMapIterator<QString, QString> itStockNameToStockID(stockNameToStockID_in_out);

	QVariant anInsertDate;

	QList<QDate> allStocksInsertDate;

	int TEST_VAR_ROW_COUNT;

	QSqlRecord aQSqlRecord;

	QString stockID;

	while(itStockNameToStockID.hasNext())
	{
		itStockNameToStockID.next();

		stockID = itStockNameToStockID.value();

		aQSqlStockTableModel.yerothSetFilter(QString("%1 = '%2'")
												.arg(YerothDatabaseTableColumn::ID,
													 stockID));
		TEST_VAR_ROW_COUNT = aQSqlStockTableModel.easySelect();

		QVERIFY(TEST_VAR_ROW_COUNT == 1);

		aQSqlRecord = aQSqlStockTableModel.record(0);

		anInsertDate = aQSqlRecord.value(YerothDatabaseTableColumn::DATE_ENTREE);

		allStocksInsertDate.append(anInsertDate.toDate());

		aQSqlStockTableModel.resetFilter();
	}

	QDate curStockInsertDate = allStocksInsertDate.at(0);

	QDate previousStockInsertDate;

	for(unsigned int k = 1; k < allStocksInsertDate.size(); ++k)
	{
		previousStockInsertDate = curStockInsertDate;

		curStockInsertDate = allStocksInsertDate.at(k);

		QVERIFY(previousStockInsertDate <= curStockInsertDate);
	}
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_lifo()
{

}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_collect_data_result(
		const QList<QDate> &allStocksInsertDate_in_out)
{
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_import_test_data()
{
//	QStringList curCsvFileToImportContentWordList;
//
//    QString csvFilePath = "";
//
//    if (!csvFilePath.isEmpty())
//    {
//    	bool importSuccess = YerothUtils::import_csv_file_content(csvFilePath, curCsvFileToImportContentWordList);
//
//    	QVERIFY(importSuccess == true);
//    }
//
//	QStringList csvHeaderContent = curCsvFileToImportContentWordList.at(0)
//			.split(YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);
//
//	int curCsvFileLineSize = csvHeaderContent.size();
//
//	QString dbFieldName;
//
//	YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;
//
//	YerothComboBox *aMappedComboBox = 0;
//
//	for (int i = 0; i < curCsvFileLineSize; ++i)
//	{
//		aMappedComboBox = _indexToSQLTableImportHeader.value(i);
//
//		if (0 != aMappedComboBox)
//		{
//			dbFieldName = aMappedComboBox->currentText();
//
//			curDatabaseTableColumnInfo =
//					new YerothERPDatabaseTableColumnInfo(dbFieldName,
//														 _dbTableColumnToType.value(dbFieldName));
//
//			_indexToDatabaseTableColumnInfo.insert(i, curDatabaseTableColumnInfo);
//
//			_allDatabaseTableColumnInfo.append(curDatabaseTableColumnInfo);
//		}
//	}
//
//	QMap<int, YerothERPDatabaseTableColumnInfo *> indexToDatabaseTableColumnInfo;
//
//	YerothERPStockImport erpStockImport(curCsvFileToImportContentWordList,
//										indexToDatabaseTableColumnInfo);
//
//	int successImportCount = erpStockImport.import();
}

