/*
 * yeroth-test-pointdevente.cpp
 *
 *      Author: Prof. Dr.-Ing. Xavier NOUMBISSI-NOUNDOU
 */

#include "test-pointdevente.hpp"



void TestCalculator::testConstructor()
{
	// default values
	Calculator c1;
	QVERIFY(c1.getA() == 0);
	QVERIFY(c1.getB() == 0);

	// full constructor
	const int A = 10;
	const int B = 2;

	Calculator c2(A, B);

	QVERIFY2(c2.getA() == A, "first operand doesn't match");
	QVERIFY2(c2.getB() == B, "second operand doesn't match");
}

void Test_PointDeVente::TEST_Single_Item_Codebar_Buy()
{

}

