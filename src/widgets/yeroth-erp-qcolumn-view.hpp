/*
 * yeroth-erp-qcolumn-view.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_QCOLUMN_VIEW_HPP_
#define YEROTH_QCOLUMN_VIEW_HPP_


#include "src/utils/yeroth-erp-utils.hpp"


#include <QtWidgets/QColumnView>


class YerothQColumnView : public QComboBox
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothQColumnView(QWidget *parent)
	:QColumnView(parent)
	{
	}

	inline ~YerothQColumnView(){}

private:

};


#endif /* YEROTH_QCOLUMN_VIEW_HPP_ */
