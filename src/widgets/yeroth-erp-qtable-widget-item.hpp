/*
 * yeroth-erp-qtable-widget-item.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_3_0_QTABLE_WIDGET_ITEM_HPP_
#define YEROTH_ERP_3_0_QTABLE_WIDGET_ITEM_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QTableWidgetItem>

class YerothQTableWidgetItem : public QTableWidgetItem
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothQTableWidgetItem()
	:QTableWidgetItem()
	{
		setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
	}

	inline YerothQTableWidgetItem(const QString &text)
	:QTableWidgetItem(text)
	{
		setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
	}

	inline YerothQTableWidgetItem (const QIcon &icon,
							   const QString &text)
	:QTableWidgetItem(icon, text)
	{
		setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
	}

	inline ~YerothQTableWidgetItem(){}
};


#endif /* YEROTH_ERP_3_0_QTABLE_WIDGET_ITEM_HPP_ */
