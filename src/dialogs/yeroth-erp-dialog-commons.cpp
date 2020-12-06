/**
 * yeroth-erp-dialog-commons.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-dialog-commons.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/yeroth-erp-windows.hpp"


YerothERPWindows *YerothPOSDialogCommons::_allWindows(0);


void YerothPOSDialogCommons::showAsModalDialogWithParent(QWidget &parent)
{
	setParent(&parent, Qt::Popup | Qt::Dialog);
    show();
    activateWindow();
}


void YerothPOSDialogCommons::closeEvent(QCloseEvent *closeEvent)
{
	rendreInvisible();
}
