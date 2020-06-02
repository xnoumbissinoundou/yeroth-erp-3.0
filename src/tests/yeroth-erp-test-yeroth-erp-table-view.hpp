/*
 * yeroth-erp-test-yeroth-erp-windows.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#ifndef YEROTH_TEST_YEROTH_ERP_TABLE_VIEW_WINDOWS_HPP_
#define YEROTH_TEST_YEROTH_ERP_TABLE_VIEW_WINDOWS_HPP_


#include "src/imports/yeroth-erp-stock-import.hpp"


#include <QtTest/QtTest>


class YerothERPDatabaseTableColumnInfo;

class YerothERPWindows;


class Test_YerothERPTableView : public QObject
{
    Q_OBJECT

public:

	inline Test_YerothERPTableView(YerothERPWindows *allWindows)
	:_allWindows(0)
	{
		_allWindows = allWindows;
	}

	inline virtual ~Test_YerothERPTableView()
	{
	}

private slots:

	void initTestCase();

	void cleanupTestCase();

	void test_TABLE_VIEW_lister_fefo();

	void test_TABLE_VIEW_lister_fifo();

	void test_TABLE_VIEW_lister_lifo();

private:

	void test_TABLE_VIEW_lister_collect_data_result(const QList<QDate> &allStocksInsertDate_in_out);

	void test_TABLE_VIEW_lister_import_test_data();

	QMap<int, YerothERPDatabaseTableColumnInfo *> _TEST_csvContentIdxToDatabaseTableColumnInfo;

	QStringList 			_curCsvFileToImportContentWordList;

    QMap<QString, bool>		_dbTableColumnToIsNotNULL;

    QMap<QString, QString>	_dbTableColumnToType;

	YerothERPWindows 		*_allWindows;
};


#endif /* YEROTH_TEST_YEROTH_ERP_TABLE_VIEW_WINDOWS_HPP_ */
