/*
 * yeroth-erp-creer-nouveau-client-window.cpp
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-creer-nouveau-client-window.hpp"

/**
 * yeroth-erp-windows.hpp cannot be included in
 * the header file because it will caused circular
 * dependency that will lead to an unsuccessful
 * compilation.
 */

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "yeroth-erp-search-form.hpp"


const QString YerothCreerNouveauClientWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2"))
												.arg(YEROTH_ERP_WINDOW_TITLE,
													 QObject::trUtf8("créer un nouveau client")));


YerothCreerNouveauClientWindow::YerothCreerNouveauClientWindow():YerothWindowsCommons(YerothCreerNouveauClientWindow::_WINDOW_TITLE),
    _logger(new
            YerothLogger("YerothCreerNouveauClientWindow"))
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_GRAY_78_78_78, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _logger->log("YerothCreerNouveauClientWindow");

    lineEdit_client_nom_entreprise->setFocus();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_client_annuler->enable(this, SLOT(annuler()));
    pushButton_client_valider->enable(this, SLOT(valider()));


    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

}

void YerothCreerNouveauClientWindow::annuler()
{
    //*_logger << "[YerothCreerNouveauClientWindow][annuler]" << "\n";
    _logger->log("annuler");
    YerothWindowsCommons::vendre();
}

void YerothCreerNouveauClientWindow::valider()
{
    _logger->log("valider");
    //qDebug() << "[YerothCreerNouveauClientWindow][valider]" << "\n";
    if (creer_client())
    {
        _allWindows->_pdVenteWindow->setCurrentClientName(lineEdit_client_nom_entreprise->text());
        YerothWindowsCommons::vendre();
    }
}

void YerothCreerNouveauClientWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}

void YerothCreerNouveauClientWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothCreerNouveauClientWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}


void YerothCreerNouveauClientWindow::definirVendeur()
{
    _logger->log("definirVendeurr");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}


void YerothCreerNouveauClientWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothCreerNouveauClientWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothCreerNouveauClientWindow::menu()
{
    _logger->log("menu");
    _allWindows->_mainWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_pdVenteWindow->rendreInvisible();
    this->rendreInvisible();
}

void YerothCreerNouveauClientWindow::alertes()
{
    _logger->log("alertes");
    _allWindows->_listerAlertesWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_pdVenteWindow->rendreInvisible();
    this->rendreInvisible();
}

void YerothCreerNouveauClientWindow::ventes()
{
    _logger->log("caisse");
    _allWindows->_ventesWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_pdVenteWindow->rendreInvisible();
    this->rendreInvisible();
}

void YerothCreerNouveauClientWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible(YerothSqlTableModel *)");
    _curStocksTableModel = stocksTableModel;
    lineEdit_client_nom_entreprise->setFocus();
    this->setVisible(true);
}

void YerothCreerNouveauClientWindow::rendreInvisible()
{
    clear_client_all_fields();
    YerothWindowsCommons::rendreInvisible();
}

void YerothCreerNouveauClientWindow::administration()
{
    _allWindows->_adminWindow->rendreVisible(_curStocksTableModel);
    _allWindows->_pdVenteWindow->rendreInvisible();
    this->rendreInvisible();
}

bool YerothCreerNouveauClientWindow::creer_client()
{
    if (creer_client_check_fields())
    {
        QString retMsg(QString(QObject::trUtf8("Le client '%1"))
        				.arg(lineEdit_client_nom_entreprise->text()));

        YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

        QSqlRecord record = clientsTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_clients());
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_client_nom_entreprise->text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT, lineEdit_client_nom_representant->text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_client_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_client_ville->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_client_pays->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_client_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL, lineEdit_client_siege_social->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_client_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_client_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_client_numero_telephone_2->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_client_numero_contribuable->text());
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CLIENT, textEdit_client_description->toPlainText());

        bool success = clientsTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

            YerothQMessageBox::warning(this, QObject::trUtf8("succès"), retMsg);

            return false;
        }

        retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

        YerothQMessageBox::information(this, QObject::trUtf8("échec"), retMsg);

        return true;
    }
    return false;
}

void YerothCreerNouveauClientWindow::clear_client_all_fields()
{
    lineEdit_client_nom_entreprise->clearField();
    lineEdit_client_nom_representant->clearField();
    lineEdit_client_quartier->clear();
    lineEdit_client_ville->clear();
    lineEdit_client_pays->clear();
    lineEdit_client_boite_postale->clear();
    lineEdit_client_siege_social->clear();
    lineEdit_client_email->clear();
    lineEdit_client_numero_telephone_1->clear();
    lineEdit_client_numero_telephone_2->clear();
    lineEdit_client_numero_contribuable->clear();
    textEdit_client_description->clear();
}

void YerothCreerNouveauClientWindow::setupShortcuts()
{
    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

bool YerothCreerNouveauClientWindow::creer_client_check_fields()
{
    bool nom_entreprise = lineEdit_client_nom_entreprise->checkField();

    if (nom_entreprise)
    {
        YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

        QString nom_entreprise_filter("nom_entreprise = '");

        nom_entreprise_filter.append(lineEdit_client_nom_entreprise->text()).append("'");

        clientsTableModel.yerothSetFilter(nom_entreprise_filter);

        int clientsTableModelRowCount = clientsTableModel.rowCount();

        if (clientsTableModelRowCount > 0)
        {
            QString retMsg(QString(QObject::trUtf8("L'entreprise nommée '%1' est déjà existante dans la base de données !"))
            					.arg(lineEdit_client_nom_entreprise->text()));

            YerothQMessageBox::warning(this, QObject::trUtf8("déjà existant"), retMsg);

            clientsTableModel.resetFilter();

            return false;
        }
        clientsTableModel.resetFilter();
    }

    bool nom_representant = lineEdit_client_nom_representant->checkField();

    return nom_entreprise && nom_representant;
}
