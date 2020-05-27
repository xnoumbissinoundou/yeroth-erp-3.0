/*
 * yeroth-erp-test-yeroth-erp-windows.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#ifndef YEROTH_TEST_YEROTH_ERP_TABLE_VIEW_WINDOWS_HPP_
#define YEROTH_TEST_YEROTH_ERP_TABLE_VIEW_WINDOWS_HPP_


#include <QtTest/QtTest>


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

private slots:

	inline void initTestCase()
    {
		QVERIFY(0 != _allWindows);
    }

	void test_TABLE_VIEW_lister_fefo();

	void test_TABLE_VIEW_lister_fifo();

	void test_TABLE_VIEW_lister_lifo();

private:

	YerothERPWindows *_allWindows;
};


#endif /* YEROTH_TEST_YEROTH_ERP_TABLE_VIEW_WINDOWS_HPP_ */
