/*
 * yeroth-erp-creer-nouveau-fournisseur-window.cpp
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-creer-nouveau-fournisseur-window.hpp"

/**
 * #include "../utils/yeroth-erp-database-table-column.hpp"
 * yeroth-erp-windows.hpp cannot be included in
 * the header file because it will caused circular
 * dependency that will lead to an unsuccessful
 * compilation.
 */

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/users/yeroth-erp-users.hpp"


YerothCreerNouveauFournisseurWindow::YerothCreerNouveauFournisseurWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerNouveauFournisseurWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("créer un nouveau fournisseur"));

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
    pushButton_creer_fournisseur_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_fournisseur_valider->enable(this, SLOT(valider()));

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

void YerothCreerNouveauFournisseurWindow::annuler()
{
    //*_logger << "[YerothCreerNouveauFournisseurWindow][annuler]" << "\n";
    _logger->log("annuler");
    _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
    rendreInvisible();
}

void YerothCreerNouveauFournisseurWindow::valider()
{
    _logger->log("valider");
    //qDebug() << "[YerothCreerNouveauFournisseurWindow][valider]" << "\n";
    if (creer_fournisseur())
    {
        _allWindows->_entrerWindow->setCurrentFournisseurName(lineEdit_creer_fournisseur_nom->text());
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);

        rendreInvisible();
    }
}

void YerothCreerNouveauFournisseurWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}

void YerothCreerNouveauFournisseurWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}

void YerothCreerNouveauFournisseurWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}


void YerothCreerNouveauFournisseurWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}


void YerothCreerNouveauFournisseurWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothCreerNouveauFournisseurWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothCreerNouveauFournisseurWindow::menu()
{
    _logger->log("menu");
    _allWindows->_mainWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_entrerWindow->rendreInvisible();

    rendreInvisible();
}

void YerothCreerNouveauFournisseurWindow::alertes()
{
    _logger->log("alertes");
    _allWindows->_listerAlertesWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_entrerWindow->rendreInvisible();

    YerothWindowsCommons::rendreInvisible();
}

void YerothCreerNouveauFournisseurWindow::ventes()
{
    _logger->log("caisse");
    _allWindows->_ventesWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_entrerWindow->rendreInvisible();

    YerothWindowsCommons::rendreInvisible();
}

void YerothCreerNouveauFournisseurWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible(YerothSqlTableModel *)");
    _curStocksTableModel = stocksTableModel;
    lineEdit_creer_fournisseur_nom->setFocus();
    YerothWindowsCommons::rendreVisible(_curStocksTableModel);
}

void YerothCreerNouveauFournisseurWindow::rendreInvisible()
{
    clear_fournisseur_all_fields();
    YerothWindowsCommons::rendreInvisible();
}

void YerothCreerNouveauFournisseurWindow::administration()
{
    _allWindows->_adminWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_entrerWindow->rendreInvisible();
    YerothWindowsCommons::rendreInvisible();
}

bool YerothCreerNouveauFournisseurWindow::creer_fournisseur()
{
    if (creer_fournisseur_check_fields())
    {
        QString retMsg(QString(QObject::trUtf8("Le fournisseur '%1"))
        					.arg(lineEdit_creer_fournisseur_nom->text()));

        YerothSqlTableModel & fournisseurTableModel = _allWindows->getSqlTableModel_fournisseurs();

        QSqlRecord record = fournisseurTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, YerothERPWindows::getNextIdSqlTableModel_fournisseurs());
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_creer_fournisseur_nom->text());
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR, textEdit_creer_fournisseur_description->toPlainText());

        bool success = fournisseurTableModel.insertNewRecord(record);

        if (success && fournisseurTableModel.select())
        {
            retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

            YerothQMessageBox::information(this, QObject::trUtf8("échec"), retMsg);

            return true;
        }

        retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

        YerothQMessageBox::warning(this, QObject::trUtf8("succès"), retMsg);

        return false;
    }

    return false;
}


bool YerothCreerNouveauFournisseurWindow::creer_fournisseur_check_fields()
{
    bool fournisseur = lineEdit_creer_fournisseur_nom->checkField();

    if (fournisseur)
    {
        YerothSqlTableModel & fournisseurTableModel = _allWindows->getSqlTableModel_fournisseurs();

        if (fournisseurTableModel.
                Is_SearchQSqlTable(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                   lineEdit_creer_fournisseur_nom->text()) > 0)
        {
            QString retMsg(QString(QObject::trUtf8("Le fournisseur '%1' est déjà existante dans la base de données !"))
            					.arg(lineEdit_creer_fournisseur_nom->text()));

            YerothQMessageBox::warning(this, QObject::trUtf8("fournisseur déjà existant"), retMsg);

            fournisseurTableModel.resetFilter();

            return false;
        }
        else
        {
            fournisseurTableModel.resetFilter();

            return true;
        }
    }

    return false;
}

void YerothCreerNouveauFournisseurWindow::clear_fournisseur_all_fields()
{
    lineEdit_creer_fournisseur_nom->clearField();
    textEdit_creer_fournisseur_description->clearField();
}

void YerothCreerNouveauFournisseurWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}
