/*
 * yeroth-erp-push-button.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_PUSH_BUTTON_HPP_
#define SRC_WIDGETS_YEROTH_PUSH_BUTTON_HPP_


#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtWidgets/QPushButton>


class YerothWindowsCommons;
class YerothTableViewWITHpagination;
class YerothSqlTableModel;


class YerothPushButton : public QPushButton
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPushButton(QWidget *w)
	:QPushButton(w),
	 _yerothCurWindowSqlTableModel(0),
	 _receiver(0),
	 _func(0)
	{
	}

	inline YerothPushButton(const QString &s, QWidget *w)
	:QPushButton(s, w),
	 _yerothCurWindowSqlTableModel(0),
	 _receiver(0),
	 _func(0)
	{
	}

	inline YerothPushButton(const QIcon &i, const QString &s, QWidget *w)
	:QPushButton(i, s, w),
	 _yerothCurWindowSqlTableModel(0),
	 _receiver(0),
	 _func(0)
	{
	}

	~YerothPushButton();

	void enable(const QObject *receiver, const char *f);

	void disable(const QObject *receiver, bool buttonVisible = false);

	void enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT(YerothAbstractClassYerothSearchWindow *receiver,
													  YerothAbstractClassYerothSearchWindow::fptr func,
													  YerothSqlTableModel **aYerothSqlTableModel_IN);

	inline void emptyEnable()
	{
		this->setEnabled(true);
	}

public slots:

	void call_view_page_function();

private:

	YerothSqlTableModel 							**_yerothCurWindowSqlTableModel;

	YerothAbstractClassYerothSearchWindow 			*_receiver;

	YerothAbstractClassYerothSearchWindow::fptr		_func;
};


#endif /* SRC_WIDGETS_YEROTH_PUSH_BUTTON_HPP_ */
