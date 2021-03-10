/*
 * yeroth-erp-tab-widget.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_TAB_WIDGET_HPP_
#define YEROTH_ERP_TAB_WIDGET_HPP_

#include <QtWidgets/QTabWidget>

#include "src/utils/yeroth-erp-utils.hpp"

class YerothUtils;

class YerothERPTabWidget : public QTabWidget
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPTabWidget(QWidget *parent)
	:QTabWidget(parent)
	{
	}

	inline ~YerothERPTabWidget(){}

	inline void resetYerothERPTabWidget()
	{
		setCurrentIndex(0);
	}
};


#endif /* YEROTH_ERP_TAB_WIDGET_HPP_ */
