/*
 * yeroth-erp-admin-windows-commons.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
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
     rendreInvisible();
}

void YerothPOSAdminWindowsCommons::retour_menu_principal()
{
    _allWindows->_mainWindow->rendreVisible(_allWindows->_adminWindow->getStocksTableModel());
     rendreInvisible();
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
        YerothQStringList progArguments;
        QProcess aProcess;

        if (user->isAdmin() ||
        	user->isManager())
        {
        	progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER;
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
    YerothQMessageBox::information(this, QObject::tr("qui suis je ?"), _allWindows->getUser()->toString());
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
     setVisible(false);
}

void YerothPOSAdminWindowsCommons::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;
     setVisible(true);
}
