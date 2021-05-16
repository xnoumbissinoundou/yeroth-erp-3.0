/*
 * yeroth-erp-test-import-csv-file-data.cpp
 *
 *      @author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

#include "yeroth-erp-test-import-csv-file-data.hpp"


#include "src/yeroth-erp-windows.hpp"


#include "src/imports/yeroth-erp-stock-import.hpp"


#include <QtTest/QtTest>


void Test_YerothERPTestImportCSVFileData::test_TABLE_VIEW_lister_import_test_data(const QString &aCsvFile_IN)
{
	cleanUP();

    QString csvFilePath(QString("%1/yeroth-erp-3-0-test_data/%2")
    						.arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER,
    							 aCsvFile_IN));

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

	YerothUtils::fillDBTableColumnNameToDBTableColumnType_TEST(YerothDatabase::STOCKS,
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

    cleanUP();
}


void Test_YerothERPTestImportCSVFileData::cleanUP()
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


#endif /* YEROTH_ERP_3_0_TESTING_UNIT_TEST */

