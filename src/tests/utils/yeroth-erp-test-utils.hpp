/*
 * yeroth-erp-test-utils.hpp
 *
 *      Author: XAVIER NOUMBISSI NOUNDOU, DIPL.-INF., PH.D. (ABD)
 */

#ifndef YEROTH_TEST_UTILS_HPP_
#define YEROTH_TEST_UTILS_HPP_


#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST


#include "src/imports/yeroth-erp-stock-import.hpp"


class YerothERPWindows;


class Test_YerothERPTestUtils : public QObject
{
    Q_OBJECT

public:

	static void TEST_UTILS_truncate_database_all_tables();

	inline static YerothERPWindows *getAllWindows()
	{
		return _allWindows;
	}

	static void setAllWindows(YerothERPWindows *allWindows);

private:

	static YerothERPWindows		*_allWindows;
};


#endif /* YEROTH_ERP_3_0_TESTING_UNIT_TEST */


#endif /* YEROTH_TEST_UTILS_HPP_ */
