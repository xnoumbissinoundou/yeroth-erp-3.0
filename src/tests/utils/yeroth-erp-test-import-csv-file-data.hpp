/*
 * yeroth-erp-test-import-csv-file-data.hpp
 *
 *      Author: XAVIER NOUMBISSI NOUNDOU, DIPL.-INF., PH.D. (ABD)
 */

#ifndef YEROTH_TEST_IMPORT_CSV_FILE_DATA_HPP_
#define YEROTH_TEST_IMPORT_CSV_FILE_DATA_HPP_


#include "src/tests/utils/yeroth-erp-test-utils.hpp"

#include "src/imports/yeroth-erp-stock-import.hpp"


class YerothERPWindows;


class Test_YerothERPTestImportCSVFileData : public QObject
{
    Q_OBJECT

public:

	inline Test_YerothERPTestImportCSVFileData()
	{
		_allWindows = Test_YerothERPTestUtils::getAllWindows();
	}

	void test_TABLE_VIEW_lister_import_test_data(const QString &aCsvFile_IN);

private:

	void cleanUP();

protected:

	QMap<int, YerothERPDatabaseTableColumnInfo *> _TEST_csvContentIdxToDatabaseTableColumnInfo;

	QStringList 			_curCsvFileToImportContentWordList;

    QMap<QString, bool>		_dbTableColumnToIsNotNULL;

    QMap<QString, QString>	_dbTableColumnToType;

	YerothERPWindows 		*_allWindows;
};


#endif /* YEROTH_TEST_IMPORT_CSV_FILE_DATA_HPP_ */
