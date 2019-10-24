/*

   * yeroth-erp-admin-windows-commons.cpp

   *

   *  Created on: Mar 6, 2018

   *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)

   *      Email:  xnoundou7@gmail.com

   */

#include "src/admin/yeroth-erp-admin-windows-commons.hpp"

#include "src/yeroth-erp-windows.hpp"

#include <QtCore/QProcess>

#include <QtCore/QObject>

#include <QtWidgets/QDesktopWidget>


YerothERPWindows *YerothPOSAdminWindowsCommons::_allWindows(0);

QPoint *YerothPOSAdminWindowsCommons::_centerPosition(new QPoint);


void YerothPOSAdminWindowsCommons::menu()
{
    _allWindows->_adminWindow->rendreVisible();
    this->rendreInvisible();
}

void YerothPOSAdminWindowsCommons::retour_menu_principal()
{
    _allWindows->_mainWindow->rendreVisible(_allWindows->_adminWindow->getStocksTableModel());
    this->rendreInvisible();
}

void YerothPOSAdminWindowsCommons::infosEntreprise()
{
    YerothUtils::infosEntreprise(*this, _allWindows->getInfoEntreprise().toString());
}

void YerothPOSAdminWindowsCommons::getManuelUtilisateurPDF()
{
    YerothPOSUser *user = _allWindows->getUser();
    if (user)
    {
        QStringList progArguments;
        QProcess aProcess;

        if (user->isAdmin())
        {
            progArguments << FILE_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_ADMINISTRATEUR;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
    }
}

void YerothPOSAdminWindowsCommons::mySetupUi(QMainWindow * aWindow)
{
    aWindow->setFixedSize(aWindow->width(), aWindow->height());
    aWindow->move(*_centerPosition);
}

void YerothPOSAdminWindowsCommons::qui_suis_je()
{
    QString dialogBoxTitle(YEROTH_ERP_WINDOW_TITLE);
    dialogBoxTitle.append(QString(" - %1 - %2").arg(_windowName, "qui suis je ?"));
    YerothQMessageBox::information(this, TR_UTF8_QSTRING(dialogBoxTitle), _allWindows->getUser()->toString());
}

void YerothPOSAdminWindowsCommons::changer_utilisateur()
{
    _allWindows->_changerUtilisateurDialog->setParent(this, Qt::Popup | Qt::Dialog);
    _allWindows->_changerUtilisateurDialog->show();
    _allWindows->_changerUtilisateurDialog->activateWindow();
}

void YerothPOSAdminWindowsCommons::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->rendreVisible(&_allWindows->getSqlTableModel_stocks());
    this->setVisible(false);
}

void YerothPOSAdminWindowsCommons::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;
    this->setVisible(true);
}
