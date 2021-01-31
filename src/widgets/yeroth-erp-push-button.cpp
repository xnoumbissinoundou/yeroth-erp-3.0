/*
 * yeroth-erp-push-button.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-push-button.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


YerothPushButton::~YerothPushButton()
{
	_yerothCurWindowSqlTableModel = 0;
	_receiver = 0;
	_func = 0;
}


void YerothPushButton::setAuthorizedForCurrentUser(bool authorized)
{
	_is_yeroth_pushbutton_authorized_for_current_user = authorized;
	QWidget::setEnabled(authorized);
	QWidget::setVisible(authorized);
}


void YerothPushButton::enable(const QObject *receiver, const char *f)
{
    QObject::connect(this, SIGNAL(clicked()), receiver, f, Qt::UniqueConnection);
    setAuthorizedForCurrentUser(true);
}


void YerothPushButton::disable(const QObject *receiver)
{
    QObject::disconnect(this, SIGNAL(clicked()), receiver, 0);
    setAuthorizedForCurrentUser(false);
}


void YerothPushButton::enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT(YerothAbstractClassYerothSearchWindow *receiver,
																	YerothAbstractClassYerothSearchWindow::fptr func,
																	YerothSqlTableModel **aYerothSqlTableModel_IN)
{
	_yerothCurWindowSqlTableModel = aYerothSqlTableModel_IN;

	if (0 != receiver)
	{
		_receiver = receiver;
	}

	if (0 != func)
	{
		_func = func;
	}

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(call_view_page_function()), Qt::UniqueConnection);

    setEnabled(true);
}


void YerothPushButton::call_view_page_function()
{
	if (0 != _receiver 				&&
		0 != _func     				&&
		0 != *_yerothCurWindowSqlTableModel)
	{
		(_receiver->*_func)(*(*_yerothCurWindowSqlTableModel));
	}
}

