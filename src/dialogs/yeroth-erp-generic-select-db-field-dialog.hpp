/*
 * yeroth-erp-generic-dialog.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_GENERIC_SELECT_DB_FIELDS_DIALOG_
#define YEROTH_ERP_GENERIC_SELECT_DB_FIELDS_DIALOG_

#include "../../ui_yeroth-erp-changer-utilisateur-dialog.h"

#include "yeroth-erp-dialog-commons.hpp"

class YerothERPWindows;
class YerothPOSUser;

class YerothERPGenericSelectDBFieldDialog : public YerothPOSDialogCommons
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPGenericSelectDBFieldDialog(YerothERPWindows *allWindows,
										YerothWindowsCommons *associatedWindow);

    ~YerothERPGenericSelectDBFieldDialog();

    virtual void show();

    virtual QPoint * getCurrentPosition()
    {
    	return _currentPosition;
    }

protected:

	virtual void closeEvent(QCloseEvent * closeEvent);

private:

	YerothWindowsCommons 	*_associatedWindow;

	QPoint					*_currentPosition;

};


#endif /* YEROTH_ERP_GENERIC_SELECT_DB_FIELDS_DIALOG_ */
