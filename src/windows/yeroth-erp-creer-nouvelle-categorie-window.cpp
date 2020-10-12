/**
 * yeroth-erp-creer-nouvelle-categorie-window.cpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-creer-nouvelle-categorie-window.hpp"

/**
* yeroth-erp-windows.hpp cannot be included in
* the header file because it will caused circular
* dependency that will lead to an unsuccessful
* compilation.
*/

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/users/yeroth-erp-users.hpp"


YerothCreerNouvelleCategorieWindow::YerothCreerNouvelleCategorieWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerNouvelleCategorieWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("créer une nouvelle catégorie"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_GRAY_78_78_78,
                            	   COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_creer_categorie_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_categorie_valider->enable(this, SLOT(valider()));

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

}

void YerothCreerNouvelleCategorieWindow::annuler()
{
    //*_logger << "[YerothCreerNouvelleCategorieWindow][annuler]" << "\n";
    _logger->log("annuler");
    _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
    rendreInvisible();
}

void YerothCreerNouvelleCategorieWindow::valider()
{
    _logger->log("valider");
    //qDebug() << "[YerothCreerNouvelleCategorieWindow][valider]" << "\n";
    if (creer_categorie())
    {
        _allWindows->_entrerWindow->setCurrentCategorieName(lineEdit_creer_categorie_nom->text());
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
        rendreInvisible();
    }
}

void YerothCreerNouvelleCategorieWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}

void YerothCreerNouvelleCategorieWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothCreerNouvelleCategorieWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);

#ifdef YEROTH_SERVER
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}


void YerothCreerNouvelleCategorieWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}



void YerothCreerNouvelleCategorieWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);

#ifdef YEROTH_SERVER
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothCreerNouvelleCategorieWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothCreerNouvelleCategorieWindow::menu()
{
    _logger->log("menu");
    _allWindows->_mainWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_entrerWindow->rendreInvisible();
    rendreInvisible();
}

void YerothCreerNouvelleCategorieWindow::alertes()
{
    _logger->log("alertes");
    _allWindows->_listerAlertesWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_entrerWindow->rendreInvisible();
    rendreInvisible();
}

void YerothCreerNouvelleCategorieWindow::ventes()
{
    _logger->log("caisse");
    _allWindows->_ventesWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_entrerWindow->rendreInvisible();
    rendreInvisible();
}

void YerothCreerNouvelleCategorieWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible(YerothSqlTableModel *)");
    _curStocksTableModel = stocksTableModel;
    lineEdit_creer_categorie_nom->setFocus();
    setVisible(true);
}

void YerothCreerNouvelleCategorieWindow::rendreInvisible()
{
    clear_categorie_all_fields();
    YerothWindowsCommons::rendreInvisible();
}

void YerothCreerNouvelleCategorieWindow::administration()
{
    _allWindows->_adminWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_entrerWindow->rendreInvisible();
    rendreInvisible();
}

bool YerothCreerNouvelleCategorieWindow::creer_categorie()
{
    if (creer_categorie_check_fields())
    {
        QString retMsg(QString(QObject::trUtf8("La catégorie '%1"))
        					.arg(lineEdit_creer_categorie_nom->text()));

        YerothSqlTableModel & categorieTableModel = _allWindows->getSqlTableModel_categories();

        QSqlRecord record = categorieTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, YerothERPWindows::getNextIdSqlTableModel_categories());
        record.setValue(YerothDatabaseTableColumn::NOM_CATEGORIE, lineEdit_creer_categorie_nom->text());
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE, textEdit_creer_categorie_description->toPlainText());

        bool success = categorieTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

            YerothQMessageBox::warning(this, QObject::trUtf8("échec"), retMsg);

            return false;
        }

        retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

        YerothQMessageBox::information(this, QObject::trUtf8("succès"), retMsg);

        return true;
    }
    return false;
}

bool YerothCreerNouvelleCategorieWindow::creer_categorie_check_fields()
{
    bool categorie = lineEdit_creer_categorie_nom->checkField();

    if (categorie)
    {
        YerothSqlTableModel & categorieTableModel = _allWindows->getSqlTableModel_categories();

        if (categorieTableModel.Is_SearchQSqlTable(QString(YerothDatabaseTableColumn::NOM_CATEGORIE),
        												lineEdit_creer_categorie_nom->text()) > 0)
        {
            QString retMsg(QString(QObject::trUtf8("La catégorie '%1' est déjà existante dans la base de données !"))
            				.arg(lineEdit_creer_categorie_nom->text()));

            YerothQMessageBox::warning(this, QObject::trUtf8("déjà existant"), retMsg);

            categorieTableModel.resetFilter();

            return false;
        }
        else
        {
            categorieTableModel.resetFilter();

            return true;
        }
    }
    return false;
}

void YerothCreerNouvelleCategorieWindow::clear_categorie_all_fields()
{
    lineEdit_creer_categorie_nom->clearField();
    textEdit_creer_categorie_description->clearField();
}

void YerothCreerNouvelleCategorieWindow::setupShortcuts()
{
	setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

