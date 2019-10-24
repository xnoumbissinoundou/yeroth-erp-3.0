/*
 * yeroth-erp-qstandard-item.hpp
 *
 *  Created on: Jan 6, 2018
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef YEROTH_QSTANDARD_ITEM_HPP_
#define YEROTH_QSTANDARD_ITEM_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtGui/QStandardItem>

class YerothQStandardItem : public QStandardItem
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothQStandardItem()
	:QStandardItem()
	{
		setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
	}

	inline YerothQStandardItem(const QString &text)
	:QStandardItem(text)
	{
		setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
	}

	inline YerothQStandardItem (const QIcon &icon,
							   const QString &text)
	:QStandardItem(icon, text)
	{
		setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
	}

	inline YerothQStandardItem(int rows, int columns = 1)
	:QStandardItem(rows, columns)
	{
		setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
	}

	inline ~YerothQStandardItem(){}
};


#endif /* YEROTH_QSTANDARD_ITEM_HPP_ */
