/*
 * yeroth-erp-test-yeroth-erp-windows.hpp
 *
 *      @author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_TEST_YEROTH_ERP_WINDOWS_HPP_
#define YEROTH_TEST_YEROTH_ERP_WINDOWS_HPP_


#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST


#include <QtTest/QtTest>


class YerothERPWindows;


class Test_YerothERPWindows : public QObject
{
    Q_OBJECT

public:

	inline Test_YerothERPWindows(YerothERPWindows *allWindows)
	:_allWindows(0)
	{
		_allWindows = allWindows;
	}

private slots:

	inline void initTestCase()
    {
		QVERIFY(0 != _allWindows);
    }

private:

	YerothERPWindows *_allWindows;
};


#endif /* YEROTH_ERP_3_0_TESTING_UNIT_TEST */


#endif /* YEROTH_TEST_YEROTH_ERP_WINDOWS_HPP_ */
