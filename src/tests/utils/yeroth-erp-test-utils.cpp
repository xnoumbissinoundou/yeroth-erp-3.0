/*
 * yeroth-erp-test-utils.cpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#include "yeroth-erp-test-utils.hpp"


#include "src/utils/yeroth-erp-database.hpp"


#include <QtTest/QtTest>


void Test_YerothERPTestUtils::TEST_UTILS_truncate_database_all_tables()
{
	YerothDatabase * yeroth_erp_3_0_db = YerothUtils::getDatabase();

	QVERIFY(0 != yeroth_erp_3_0_db);

	YerothUtils::executer_fichier_sql("yeroth-erp-3-0-test_data/truncate_test_yeroth_erp_3_test.sql");
}
