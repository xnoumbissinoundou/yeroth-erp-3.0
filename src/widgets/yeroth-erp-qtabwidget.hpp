/*
 * yeroth-erp-qtabwidget.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_ERP_QTABWIDGET_HPP_
#define YEROTH_ERP_QTABWIDGET_HPP_

#include <QtWidgets/QTabWidget>

#include "src/utils/yeroth-erp-utils.hpp"

class YerothUtils;

class YerothQTabWidget : public QTabWidget
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothQTabWidget(QWidget *parent)
	:QTabWidget(parent)
	{
	}

	inline ~YerothQTabWidget(){}

	inline void resetYerothQTabWidget()
	{
		setCurrentIndex(0);
	}
};


#endif /* YEROTH_ERP_QTABWIDGET_HPP_ */
