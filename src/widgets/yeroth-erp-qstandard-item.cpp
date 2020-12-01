/*
 * yeroth-erp-qstandard-item.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-qstandard-item.hpp"


#include "src/utils/yeroth-erp-utils.hpp"


YerothQStandardItem::YerothQStandardItem(Qt::Alignment alignPosition /* = Qt::AlignHCenter |
																	      Qt::AlignCenter    */)
:QStandardItem(),
 _display__rawUNMODIFIED_FOR_USER_VIEWING_text(true)
{
	setTextAlignment(alignPosition);
}


YerothQStandardItem::YerothQStandardItem(int rows,
										 int columns /* = 1 */,
										 Qt::Alignment alignPosition /* = Qt::AlignHCenter |
										 	 	 	 	 	 	 	      Qt::AlignCenter    */)
:QStandardItem(rows, columns),
 _display__rawUNMODIFIED_FOR_USER_VIEWING_text(true)
{
	setTextAlignment(alignPosition);
}


YerothQStandardItem::YerothQStandardItem(const QString &text,
										 bool display__rawUNMODIFIED_FOR_USER_VIEWING_text /* = true */,
										 Qt::Alignment alignPosition /* = Qt::AlignHCenter |
										 	 	 	 	 	 	 	      Qt::AlignCenter   */)
:QStandardItem(display__rawUNMODIFIED_FOR_USER_VIEWING_text ? text : YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(text)),
 _display__rawUNMODIFIED_FOR_USER_VIEWING_text(display__rawUNMODIFIED_FOR_USER_VIEWING_text),
 _rawUNMODIFIED_FOR_USER_VIEWING_text(text)
{
	setTextAlignment(alignPosition);
}


YerothQStandardItem::YerothQStandardItem (const QIcon &icon,
										  const QString &text,
										  bool display__rawUNMODIFIED_FOR_USER_VIEWING_text /* = true */,
										  Qt::Alignment alignPosition /* = Qt::AlignHCenter |
										  	  	  	  	  	  	  	       Qt::AlignCenter    */)
:QStandardItem(icon, display__rawUNMODIFIED_FOR_USER_VIEWING_text ? text : YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(text)),
 _display__rawUNMODIFIED_FOR_USER_VIEWING_text(display__rawUNMODIFIED_FOR_USER_VIEWING_text),
 _rawUNMODIFIED_FOR_USER_VIEWING_text(text)
{
	setTextAlignment(alignPosition);
}
