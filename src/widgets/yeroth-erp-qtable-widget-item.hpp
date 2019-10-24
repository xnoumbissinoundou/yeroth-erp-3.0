/*
 * yeroth-erp-qtable-widget-item.hpp
 *
 *  Created on: Jan 24, 2018
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
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
