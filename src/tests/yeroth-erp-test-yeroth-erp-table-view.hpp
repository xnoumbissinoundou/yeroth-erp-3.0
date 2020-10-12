/*
 * yeroth-erp-test-yeroth-erp-windows.hpp
 *
 *      Author: XAVIER NOUMBISSI NOUNDOU, DIPL.-INF., PH.D. (ABD)
 */

#ifndef YEROTH_TEST_YEROTH_ERP_TABLE_VIEW_WINDOWS_HPP_
#define YEROTH_TEST_YEROTH_ERP_TABLE_VIEW_WINDOWS_HPP_


#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST


#include "src/include/yeroth-erp-3-0-definition-oo-class-operators.hpp"

#include "src/tests/utils/yeroth-erp-test-utils.hpp"


#include <QtCore/QObject>


class Test_YerothERPTestImportCSVFileData;

class YerothERPDatabaseTableColumnInfo;

class YerothERPWindows;


class Test_YerothERPTableView : public QObject
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	Test_YerothERPTableView();

	inline ~Test_YerothERPTableView()
	{
		delete _yerothERPTestImportCSVFileData;
	}

private slots:

	void initTestCase();

	void test_TABLE_VIEW_lister_fefo();

	void test_TABLE_VIEW_lister_fifo();

	void test_TABLE_VIEW_lister_lifo();

private:

	Test_YerothERPTestImportCSVFileData *_yerothERPTestImportCSVFileData;

	YerothERPWindows 		*_allWindows;
};


#endif /* YEROTH_ERP_3_0_TESTING_UNIT_TEST */


#endif /* YEROTH_TEST_YEROTH_ERP_TABLE_VIEW_WINDOWS_HPP_ */
