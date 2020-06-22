/*
 * yeroth-erp-generic-dialog.cpp
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-generic-select-db-field-dialog.hpp"

#include "src/yeroth-erp-windows.hpp"

#include <QtWidgets/QDesktopWidget>


YerothERPGenericSelectDBFieldDialog::YerothERPGenericSelectDBFieldDialog(YerothERPWindows * allWindows,
																		 YerothWindowsCommons * associatedWindow)
:YerothPOSDialogCommons(""),
 _associatedWindow(associatedWindow),
 _currentPosition(new QPoint)
{
	QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

	YerothUtils::getCenterPosition(desktopWidget.width(),
								  desktopWidget.height(),
								  *this,
								  *_currentPosition);

    setFixedSize(width(), height());

    setModal(true);
}

YerothERPGenericSelectDBFieldDialog::~YerothERPGenericSelectDBFieldDialog()
{
	delete _currentPosition;
}


void YerothERPGenericSelectDBFieldDialog::show()
{
	move(*_currentPosition);

    YerothPOSDialogCommons::show();
}


void YerothERPGenericSelectDBFieldDialog::closeEvent(QCloseEvent * closeEvent)
{
	/*
	 * Works fine only if this dialog is set as modal.
	 */
	if (isVisible() && 0 != _associatedWindow)
	{
		_associatedWindow->rendreVisible(_associatedWindow->getCurStocksTableModel());
	}

    YerothPOSDialogCommons::closeEvent(closeEvent);
}
