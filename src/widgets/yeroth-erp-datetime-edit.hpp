/*
 * yeroth-erp-datetime-edit.hpp
 *
 *  Created on: Mar 16, 2016
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_WIDGETS_YEROTH_DATETIME_EDIT_HPP_
#define SRC_WIDGETS_YEROTH_DATETIME_EDIT_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QCalendarWidget>

class QPoint;
class QDateTimeEdit;
class QCalendarWidget;
class QMainWindow;

class YerothERPWindows;
class YerothLogger;

class YerothDateTimeEdit : public QDateTimeEdit
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothDateTimeEdit(QWidget *parent = 0);

	inline ~YerothDateTimeEdit()
	{
	    delete _calendarWidget;
	}

	void setYerothEnabled(bool enabled);

	void setYerothERPDateTimeEditDisplayFormat();

	void setStartDate(QDate startDate);

	inline void setMyDate(QDate aDate)
	{
	    setDate(aDate);
	}

	inline QCalendarWidget *getCalendarWidget()
	{
		return _calendarWidget;
	}

public slots:

	inline void reset()
	{
	    setDate(_startDate);
	}

private:

	YerothLogger	*_logger;

	QDate				_startDate;
	QCalendarWidget 	*_calendarWidget;
};


#endif /* SRC_WIDGETS_YEROTH_DATETIME_EDIT_HPP_ */
