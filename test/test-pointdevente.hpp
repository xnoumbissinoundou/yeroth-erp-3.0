/*
 * yeroth-test-pointdevente.hpp
 *
 *      Author: Prof. Dr.-Ing. Xavier NOUMBISSI-NOUNDOU
 */

#include <QtTest/QTest>

class Test_PointDeVente : public QObject
{
	Q_OBJECT

	private slots:

	// called before the first test function
	void initTestCase();

	// called before every test function
	void init();

	// called after every test function
	void cleanup();

	// called after the last test function
	void cleanupTestCase();


	void TEST_Single_Item_Codebar_Buy();
};
