/*
 * yeroth-erp-changer-utilisateur-dialog.hpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef YEROTH_ERP_CHANGER_UTILISATEUR_DIALOG_
#define YEROTH_ERP_CHANGER_UTILISATEUR_DIALOG_

#include "../../ui_yeroth-erp-changer-utilisateur-dialog.h"

#include "yeroth-erp-dialog-commons.hpp"

class YerothERPWindows;
class YerothPOSUser;

class YerothPOSChangerUtilisateurDialog : public YerothPOSDialogCommons, private Ui_YerothPOSChangerUtilisateurDialog
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothPOSChangerUtilisateurDialog(YerothERPWindows *allWindows, QMainWindow *parent = 0);

    ~YerothPOSChangerUtilisateurDialog();

    virtual QPoint * getCurrentPosition()
    {
    	return _currentPosition;
    }

//signals:

	//void disconnectUser();

public slots:

	inline void annuler()
	{
		this->close();
	}

	void valider();

	virtual void show();

protected:

	virtual void closeEvent(QCloseEvent * closeEvent);

private:

	YerothPOSUser *createUser(QSqlRecord &userRecord,
							    QString &role);

	void checkCourriersAlertes();

	static const QString 	DIALOG_WINDOW_TITLE;

	QPoint					*_currentPosition;

	YerothPOSUser 			*_user;

};


#endif /* YEROTH_ERP_CHANGER_UTILISATEUR_DIALOG_ */
