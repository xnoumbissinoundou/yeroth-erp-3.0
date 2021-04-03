/*
 * yeroth-erp-qstandard-item.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_QSTANDARD_ITEM_HPP_
#define YEROTH_QSTANDARD_ITEM_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"


#include <QtGui/QStandardItem>


class YerothQStandardItem : public QStandardItem
{

public:

	YEROTH_CLASS_OPERATORS

	YerothQStandardItem(Qt::Alignment alignPosition = Qt::AlignHCenter | Qt::AlignCenter);

	YerothQStandardItem(int rows,
						int columns = 1,
						Qt::Alignment alignPosition = Qt::AlignHCenter | Qt::AlignCenter);

	YerothQStandardItem(const QString &text,
						bool display__rawUNMODIFIED_FOR_USER_VIEWING_text = true,
						Qt::Alignment alignPosition = Qt::AlignHCenter | Qt::AlignCenter);

	YerothQStandardItem (const QIcon &icon,
						 const QString &text,
						 bool display__rawUNMODIFIED_FOR_USER_VIEWING_text = true,
						 Qt::Alignment alignPosition = Qt::AlignHCenter | Qt::AlignCenter);

	inline ~YerothQStandardItem(){}

	inline void set_rawUNMODIFIED_FOR_USER_VIEWING_text(const QString &aRawUNMODIFIED_FOR_USER_VIEWING_text)
	{
		_rawUNMODIFIED_FOR_USER_VIEWING_text = aRawUNMODIFIED_FOR_USER_VIEWING_text;
	}

	inline QString get_rawUNMODIFIED_FOR_USER_VIEWING_text()
	{
		return _rawUNMODIFIED_FOR_USER_VIEWING_text;
	}

	inline void set__display__rawUNMODIFIED_FOR_USER_VIEWING_text(bool display__rawUNMODIFIED_FOR_USER_VIEWING_text)
	{
		_display__rawUNMODIFIED_FOR_USER_VIEWING_text = display__rawUNMODIFIED_FOR_USER_VIEWING_text;
	}

	inline bool get__display__rawUNMODIFIED_FOR_USER_VIEWING_text()
	{
		return _display__rawUNMODIFIED_FOR_USER_VIEWING_text;
	}

protected:

	bool	_display__rawUNMODIFIED_FOR_USER_VIEWING_text;

	QString _rawUNMODIFIED_FOR_USER_VIEWING_text;
};


#endif /* YEROTH_QSTANDARD_ITEM_HPP_ */
