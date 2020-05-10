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

	inline YerothQStandardItem(Qt::Alignment alignPosition = Qt::AlignHCenter | Qt::AlignCenter)
	:QStandardItem()
	{
		setTextAlignment(alignPosition);
	}

	inline YerothQStandardItem(const QString &text,
							   Qt::Alignment alignPosition = Qt::AlignHCenter | Qt::AlignCenter)
	:QStandardItem(text)
	{
		setTextAlignment(alignPosition);
	}

	inline YerothQStandardItem (const QIcon &icon,
							    const QString &text,
								Qt::Alignment alignPosition = Qt::AlignHCenter | Qt::AlignCenter)
	:QStandardItem(icon, text)
	{
		setTextAlignment(alignPosition);
	}

	inline YerothQStandardItem(int rows,
							   int columns = 1,
							   Qt::Alignment alignPosition = Qt::AlignHCenter | Qt::AlignCenter)
	:QStandardItem(rows, columns)
	{
		setTextAlignment(alignPosition);
	}

	inline ~YerothQStandardItem(){}
};


#endif /* YEROTH_QSTANDARD_ITEM_HPP_ */
