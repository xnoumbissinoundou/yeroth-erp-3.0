/*
 * yeroth-erp-datetime-edit.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-datetime-edit.hpp"

#include "src/yeroth-erp-windows.hpp"
#include "src/utils/yeroth-erp-logger.hpp"

#include <QtCore/QPoint>

YerothDateTimeEdit::YerothDateTimeEdit(QWidget *parent)
:QDateTimeEdit(parent),
 _logger(new YerothLogger("YerothDateTimeEdit")),
 _calendarWidget(new QCalendarWidget)
{
    _logger->log("YerothDateTimeEdit");

    _calendarWidget->setMinimumDate(QDate(1900, 1, 1));
    _calendarWidget->setMaximumDate(QDate(70000, 1, 1));
    _calendarWidget->setGridVisible(true);

    setCalendarPopup(true);

    setCalendarWidget(_calendarWidget);

    setYerothERPDateTimeEditDisplayFormat();
}


void YerothDateTimeEdit::setYerothEnabled(bool enabled)
{
	QDateTimeEdit::setReadOnly(!enabled);

	setYerothERPDateTimeEditDisplayFormat();
}


void YerothDateTimeEdit::setYerothERPDateTimeEditDisplayFormat()
{
	if (isReadOnly())
	{
		setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
	}
	else
	{
		setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
	}
}


void YerothDateTimeEdit::setStartDate(QDate startDate)
{
    _startDate = startDate;
    setDate(_startDate);
}

