/*
 * yeroth-erp-pointdevente-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_TEST_VENTE_WINDOW_HPP_
#define YEROTH_TEST_VENTE_WINDOW_HPP_


#include "src/windows/yeroth-erp-pointdevente-window.hpp"

#include <QtTest/QtTest>


class Test_YerothPointDeVenteWindow : public YerothPointDeVenteWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

private slots:

	void test_vente_par_codebar();

};


#endif /* YEROTH_TEST_VENTE_WINDOW_HPP_ */
