/*
 * yeroth-erp-test-utils.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#ifndef YEROTH_TEST_UTILS_HPP_
#define YEROTH_TEST_UTILS_HPP_


#include "src/imports/yeroth-erp-stock-import.hpp"


class YerothERPWindows;


class Test_YerothERPTestUtils : public QObject
{
    Q_OBJECT

public:

	static void TEST_UTILS_truncate_database_all_tables();

private:

	YerothERPWindows 		*_allWindows;
};


#endif /* YEROTH_TEST_UTILS_HPP_ */
