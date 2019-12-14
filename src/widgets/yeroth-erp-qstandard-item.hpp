/*
 * yeroth-erp-qstandard-item.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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
