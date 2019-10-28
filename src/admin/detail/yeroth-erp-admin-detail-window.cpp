/*
 * yeroth-erp-admin-detail-window.cpp
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#include "yeroth-erp-admin-detail-window.hpp"
#include "src/yeroth-erp-windows.hpp"
#include <QtCore/QDebug>
#include <QtWidgets/QDesktopWidget>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

YerothAdminDetailWindow::YerothAdminDetailWindow():YerothPOSAdminWindowsCommons(QObject::tr("administration ~ détail")),
    _logger(new YerothLogger("YerothAdminDetailWindow"))
{
    setupUi(this);
    this->mySetupUi(this);

    _logger->log("YerothAdminDetailWindow");

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    this->setupLineEdits();

    dateEdit_detail_utilisateur_date_naissance->setEnabled(false);
    dateEdit_detail_localisation_date_ouverture->setEnabled(false);
    textEdit_detail_client_description->setEnabled(false);
    textEdit_detail_fournisseur_description->setEnabled(false);
    textEdit_detail_categorie_description->setEnabled(false);
    textEdit_detail_localisation_description_lieu->setEnabled(false);
    textEdit_detail_alerte_message->setEnabled(false);
    radioButton_detail_alerte_date_periode_temps->setEnabled(false);
    radioButton_detail_alerte_quantite->setEnabled(false);
    dateEdit_detail_alerte_date_debut->setEnabled(false);
    dateEdit_detail_alerte_date_fin->setEnabled(false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
    pushButton_creer->enable(this, SLOT(creer()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(lister()));
    pushButton_modifier->enable(this, SLOT(modifier()));
    pushButton_supprimer->enable(this, SLOT(supprimer()));
    connect(actionCreer, SIGNAL(triggered()), this, SLOT(creer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionModifier, SIGNAL(triggered()), this, SLOT(modifier()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));
    pushButton_detail_utilisateur_retour->enable(this, SLOT(retourListerCompteUtilisateur()));
    pushButton_detail_localisation_retour->enable(this, SLOT(retourListerLocalisation()));
    pushButton_detail_categorie_retour->enable(this, SLOT(retourListerCategorie()));
    pushButton_detail_alerte_retour->enable(this, SLOT(retourListerAlerte()));
    pushButton_detail_client_retour->enable(this, SLOT(retourListerClient()));
    pushButton_detail_fournisseur_retour->enable(this, SLOT(retourListerFournisseur()));
    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this, SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
}

void YerothAdminDetailWindow::setupLineEdits()
{
    lineEdit_detail_utilisateur_prenom->setEnabled(false);
    lineEdit_detail_utilisateur_nom->setEnabled(false);
    lineEdit_detail_utilisateur_lieu_naissance->setEnabled(false);
    lineEdit_detail_utilisateur_email->setEnabled(false);
    lineEdit_detail_utilisateur_ville->setEnabled(false);
    lineEdit_detail_utilisateur_province_etat->setEnabled(false);
    lineEdit_detail_utilisateur_pays->setEnabled(false);
    lineEdit_detail_utilisateur_boite_postale->setEnabled(false);
    lineEdit_detail_utilisateur_numero_telephone_1->setEnabled(false);
    lineEdit_detail_utilisateur_numero_telephone_2->setEnabled(false);
    lineEdit_detail_utilisateur_id->setEnabled(false);
    lineEdit_detail_utilisateur_mot_passe->setEnabled(false);
    lineEdit_detail_client_nom_entreprise->setEnabled(false);
    lineEdit_detail_client_nom_representant->setEnabled(false);
    lineEdit_detail_client_quartier->setEnabled(false);
    lineEdit_detail_client_ville->setEnabled(false);
    lineEdit_detail_client_province_etat->setEnabled(false);
    lineEdit_detail_client_pays->setEnabled(false);
    lineEdit_detail_client_boite_postale->setEnabled(false);
    lineEdit_detail_client_siege_social->setEnabled(false);
    lineEdit_detail_client_email->setEnabled(false);
    lineEdit_detail_client_numero_telephone_1->setEnabled(false);
    lineEdit_detail_client_numero_telephone_2->setEnabled(false);
    lineEdit_detail_client_numero_contribuable->setEnabled(false);
    lineEdit_detail_fournisseur_nom_entreprise->setEnabled(false);
    lineEdit_detail_fournisseur_nom_representant->setEnabled(false);
    lineEdit_detail_fournisseur_quartier->setEnabled(false);
    lineEdit_detail_fournisseur_ville->setEnabled(false);
    lineEdit_detail_fournisseur_province_etat->setEnabled(false);
    lineEdit_detail_fournisseur_pays->setEnabled(false);
    lineEdit_detail_fournisseur_boite_postale->setEnabled(false);
    lineEdit_detail_fournisseur_siege_social->setEnabled(false);
    lineEdit_detail_fournisseur_email->setEnabled(false);
    lineEdit_detail_fournisseur_numero_telephone_1->setEnabled(false);
    lineEdit_detail_fournisseur_numero_telephone_2->setEnabled(false);
    lineEdit_detail_fournisseur_numero_contribuable->setEnabled(false);
    lineEdit_detail_categorie_nom->setEnabled(false);
    lineEdit_detail_localisation_adresse_ip->setEnabled(false);
    lineEdit_detail_localisation_nom->setEnabled(false);
    lineEdit_detail_localisation_numero_unique->setEnabled(false);
    lineEdit_detail_localisation_quartier->setEnabled(false);
    lineEdit_detail_localisation_ville->setEnabled(false);
    lineEdit_detail_localisation_province_etat->setEnabled(false);
    lineEdit_detail_localisation_pays->setEnabled(false);
    lineEdit_detail_localisation_boite_postale->setEnabled(false);
    lineEdit_detail_localisation_email->setEnabled(false);
    lineEdit_detail_localisation_numero_telephone_1->setEnabled(false);
    lineEdit_detail_localisation_numero_telephone_2->setEnabled(false);
    lineEdit_detail_alerte_designation_article->setEnabled(false);
    lineEdit_detail_alerte_quantite->setEnabled(false);
    lineEdit_detail_client_dette_maximale_compte_client->setEnabled(false);
}

void YerothAdminDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
}

void YerothAdminDetailWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
}

void YerothAdminDetailWindow::definirManager()
{
    _logger->log("definirManager");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, true);
}

void YerothAdminDetailWindow::creer()
{
    _allWindows->_adminCreateWindow->rendreVisible(tabWidget_detail->currentIndex());
    this->rendreInvisible();
}

void YerothAdminDetailWindow::lister()
{
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_detail->currentIndex());
    this->rendreInvisible();
}

void YerothAdminDetailWindow::modifier()
{
    _logger->log("modifier");
    switch (tabWidget_detail->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;
    case SUJET_ACTION_LOCALISATION:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
        break;
    case SUJET_ACTION_CATEGORIE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
        break;
    case SUJET_ACTION_CLIENT:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CLIENT);
        break;
    case SUJET_ACTION_FOURNISSEUR:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_FOURNISSEUR);
        break;
    case SUJET_ACTION_ALERTE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_ALERTE);
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        break;
    default:
        break;
    }
    this->rendreInvisible();
}

void YerothAdminDetailWindow::supprimer()
{
    _logger->log("supprimer");
    _allWindows->_adminListerWindow->supprimer();
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_detail->currentIndex());
    this->rendreInvisible();
}

void YerothAdminDetailWindow::rendreInvisible()
{
    _logger->log("rendreInvisible");
    lineEdit_detail_alerte_id_destinataire->clear();
    lineEdit_detail_alerte_nom_destinataire->clear();
    lineEdit_detail_alerte_designation_article->clear();
    this->clear_set_edit_comboBoxes();
    this->setVisible(false);
}

void YerothAdminDetailWindow::enableOtherTabs(enum AdminSujetAction curAction, bool enabled)
{
    tabWidget_detail->setTabEnabled(curAction, !enabled);
    for (int k = 0; k <= 6; ++k)
    {
        if (k != curAction)
        {
            tabWidget_detail->setTabEnabled(k, enabled);
        }
    }
}

void YerothAdminDetailWindow::rendreVisibleCompteUtilisateur(int sqlTableRow)
{
    this->tabWidget_detail->setCurrentIndex(SUJET_ACTION_COMPTE_UTILISATEUR);
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *usersSqlTableModel = lw->getCurSearchSqlTableModel();
    if (!usersSqlTableModel)
    {
        usersSqlTableModel = &_allWindows->getSqlTableModel_users();
    }
    else if (usersSqlTableModel
             && !YerothUtils::isEqualCaseInsensitive(usersSqlTableModel->sqlTableName(), _allWindows->USERS))
    {
        usersSqlTableModel = &_allWindows->getSqlTableModel_users();
    }
    //YerothSqlTableModel &usersSqlTableRecord = _allWindows->getSqlTableModel_users();
    QSqlRecord record = usersSqlTableModel->record(sqlTableRow);
    lineEdit_detail_utilisateur_prenom->setText(GET_SQL_RECORD_DATA(record, "prenom"));
    lineEdit_detail_utilisateur_nom->setText(GET_SQL_RECORD_DATA(record, "nom"));
    comboBox_detail_utilisateur_titre->addItem(GET_SQL_RECORD_DATA(record, "titre"));
    lineEdit_detail_utilisateur_lieu_naissance->setText(GET_SQL_RECORD_DATA(record, "lieu_naissance"));
    dateEdit_detail_utilisateur_date_naissance->
    setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, "date_naissance")));
    lineEdit_detail_utilisateur_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
    lineEdit_detail_utilisateur_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));
    lineEdit_detail_utilisateur_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
    lineEdit_detail_utilisateur_province_etat->setText(GET_SQL_RECORD_DATA(record, "province_etat"));
    lineEdit_detail_utilisateur_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_detail_utilisateur_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_detail_utilisateur_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
    comboBox_detail_utilisateur_role->addItem(GET_SQL_RECORD_DATA(record, "role"));
    lineEdit_detail_utilisateur_id->setText(GET_SQL_RECORD_DATA(record, "nom_utilisateur"));
    lineEdit_detail_utilisateur_mot_passe->setText("****");
    lineEdit_detail_utilisateur_localisation->setEnabled(false);
    lineEdit_detail_utilisateur_localisation->setText(_allWindows->getInfoEntreprise().getLocalisation());
    this->enableOtherTabs(SUJET_ACTION_COMPTE_UTILISATEUR, false);
    this->setVisible(true);
}

void YerothAdminDetailWindow::rendreVisibleLocalisation(int sqlTableRow)
{
    this->tabWidget_detail->setCurrentIndex(SUJET_ACTION_LOCALISATION);
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *localisationSqlTableModel = lw->getCurSearchSqlTableModel();
    if (!localisationSqlTableModel)
    {
        localisationSqlTableModel = &_allWindows->getSqlTableModel_localisations();
    }
    else if (localisationSqlTableModel
             && !YerothUtils::isEqualCaseInsensitive(localisationSqlTableModel->sqlTableName(),
                     _allWindows->LOCALISATIONS))
    {
        localisationSqlTableModel = &_allWindows->getSqlTableModel_localisations();
    }
    //YerothSqlTableModel &localisationSqlTableModel = _allWindows->getSqlTableModel_localisations();
    QSqlRecord record = localisationSqlTableModel->record(sqlTableRow);
    lineEdit_detail_localisation_adresse_ip->setText(GET_SQL_RECORD_DATA(record, "adresse_ip"));
    lineEdit_detail_localisation_nom->setText(GET_SQL_RECORD_DATA(record, "nom_localisation"));
    lineEdit_detail_localisation_numero_unique->setText(GET_SQL_RECORD_DATA(record, "numero_unique"));
    lineEdit_detail_localisation_quartier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUARTIER));
    lineEdit_detail_localisation_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
    lineEdit_detail_localisation_province_etat->setText(GET_SQL_RECORD_DATA(record, "province_etat"));
    lineEdit_detail_localisation_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));
    lineEdit_detail_localisation_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
    dateEdit_detail_localisation_date_ouverture->
    setDate(QDate::fromString(GET_SQL_RECORD_DATA(record, "date_ouverture"), YerothUtils::DATE_FORMAT));
    lineEdit_detail_localisation_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
    lineEdit_detail_localisation_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_detail_localisation_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
    lineEdit_detail_localisation_base_donnees->setText(GET_SQL_RECORD_DATA(record, "dbms"));
    textEdit_detail_localisation_description_lieu->setText(GET_SQL_RECORD_DATA(record, "description_lieu"));
    this->enableOtherTabs(SUJET_ACTION_LOCALISATION, false);
    this->setVisible(true);
}

void YerothAdminDetailWindow::rendreVisibleCategorie(int sqlTableRow)
{
    this->tabWidget_detail->setCurrentIndex(SUJET_ACTION_CATEGORIE);
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *categoriesTableModel = lw->getCurSearchSqlTableModel();
    if (!categoriesTableModel)
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }
    else if (categoriesTableModel
             && !YerothUtils::isEqualCaseInsensitive(categoriesTableModel->sqlTableName(),
                     _allWindows->CATEGORIES))
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }
    QSqlRecord record = categoriesTableModel->record(sqlTableRow);
    lineEdit_detail_categorie_nom->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_CATEGORIE));
    textEdit_detail_categorie_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE));
    this->enableOtherTabs(SUJET_ACTION_CATEGORIE, false);
    this->setVisible(true);
}

void YerothAdminDetailWindow::rendreVisibleClient(int sqlTableRow)
{
    tabWidget_detail->setCurrentIndex(SUJET_ACTION_CLIENT);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *clientsTableModel = lw->getCurSearchSqlTableModel();

    if (!clientsTableModel)
    {
        clientsTableModel = &_allWindows->getSqlTableModel_clients();
    }
    else if (clientsTableModel
             && !YerothUtils::isEqualCaseInsensitive(clientsTableModel->sqlTableName(), _allWindows->CLIENTS))
    {
        clientsTableModel = &_allWindows->getSqlTableModel_clients();
    }

    QSqlRecord record = clientsTableModel->record(sqlTableRow);

    lineEdit_detail_client_nom_entreprise->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));
    lineEdit_detail_client_nom_representant->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_REPRESENTANT));
    lineEdit_detail_client_quartier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUARTIER));
    lineEdit_detail_client_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
    lineEdit_detail_client_province_etat->setText(GET_SQL_RECORD_DATA(record, "province_etat"));
    lineEdit_detail_client_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));
    lineEdit_detail_client_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_detail_client_siege_social->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::SIEGE_SOCIAL));
    lineEdit_detail_client_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
    lineEdit_detail_client_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_detail_client_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
    lineEdit_detail_client_numero_contribuable->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));

    YerothPOSUser *aUser = YerothUtils::getAllWindows()->getUser();

    if (0 != YerothUtils::getAllWindows())
    {
    	if (0 != aUser && aUser->isManager())
    	{
        	label_admin_detail_client_dette_maximale_compte_client->setVisible(true);
    		lineEdit_detail_client_dette_maximale_compte_client->setVisible(true);

    	    double dette_maximale_compte_client = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

    	    lineEdit_detail_client_dette_maximale_compte_client->setText(GET_DOUBLE_STRING(dette_maximale_compte_client));
    	}
    	else
    	{
    		label_admin_detail_client_dette_maximale_compte_client->setVisible(false);
    		lineEdit_detail_client_dette_maximale_compte_client->setVisible(false);
    	}
    }
    else
    {
    	label_admin_detail_client_dette_maximale_compte_client->setVisible(false);
		lineEdit_detail_client_dette_maximale_compte_client->setVisible(false);
    }

    textEdit_detail_client_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_CLIENT));

    enableOtherTabs(SUJET_ACTION_CLIENT, false);

    setVisible(true);
}

void YerothAdminDetailWindow::rendreVisibleFournisseur(int sqlTableRow)
{
    this->tabWidget_detail->setCurrentIndex(SUJET_ACTION_FOURNISSEUR);
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *fournisseursTableModel = lw->getCurSearchSqlTableModel();
    if (!fournisseursTableModel)
    {
        fournisseursTableModel = &_allWindows->getSqlTableModel_fournisseurs();
    }
    else if (fournisseursTableModel
             && !YerothUtils::isEqualCaseInsensitive(fournisseursTableModel->sqlTableName(),
                     _allWindows->FOURNISSEURS))
    {
        fournisseursTableModel = &_allWindows->getSqlTableModel_fournisseurs();
    }
    QSqlRecord record = fournisseursTableModel->record(sqlTableRow);
    lineEdit_detail_fournisseur_nom_entreprise->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));
    lineEdit_detail_fournisseur_nom_representant->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_REPRESENTANT));
    lineEdit_detail_fournisseur_quartier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUARTIER));
    lineEdit_detail_fournisseur_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
    lineEdit_detail_fournisseur_province_etat->setText(GET_SQL_RECORD_DATA(record, "province_etat"));
    lineEdit_detail_fournisseur_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));
    lineEdit_detail_fournisseur_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_detail_fournisseur_siege_social->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::SIEGE_SOCIAL));
    lineEdit_detail_fournisseur_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
    lineEdit_detail_fournisseur_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_detail_fournisseur_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
    lineEdit_detail_fournisseur_numero_contribuable->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));
    textEdit_detail_fournisseur_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR));
    this->enableOtherTabs(SUJET_ACTION_FOURNISSEUR, false);
    this->setVisible(true);
}

void YerothAdminDetailWindow::rendreVisibleAlerte(int sqlTableRow)
{
    this->tabWidget_detail->setCurrentIndex(SUJET_ACTION_ALERTE);
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *alertesTableModel = lw->getCurSearchSqlTableModel();
    if (!alertesTableModel)
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }
    else if (alertesTableModel
             && !YerothUtils::isEqualCaseInsensitive(alertesTableModel->sqlTableName(), _allWindows->ALERTES))
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }
    QSqlRecord record = alertesTableModel->record(sqlTableRow);

    lineEdit_detail_alerte_designation_alerte->setEnabled(false);
    lineEdit_detail_alerte_designation_alerte->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_ALERTE));

    lineEdit_detail_alerte_id_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESTINATAIRE));
    lineEdit_detail_alerte_id_destinataire->setEnabled(false);

    lineEdit_detail_alerte_nom_destinataire->setEnabled(false);
    lineEdit_detail_alerte_nom_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE));

    lineEdit_detail_alerte_designation_article->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    lineEdit_detail_alerte_designation_article->setEnabled(false);

    comboBox_detail_alerte_condition->addItem(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CONDITION_ALERTE));

    int quantite = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE).toInt();
    if (quantite > -1)
    {
        radioButton_detail_alerte_quantite->setChecked(true);
        lineEdit_detail_alerte_quantite->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE));
    }
    else
    {
        radioButton_detail_alerte_date_periode_temps->setChecked(true);
        QString date_debut(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_DEBUT));
        dateEdit_detail_alerte_date_debut->setDate(GET_DATE_FROM_STRING(date_debut));
        QString date_fin(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_FIN));
        dateEdit_detail_alerte_date_fin->setDate(GET_DATE_FROM_STRING(date_fin));
    }
    textEdit_detail_alerte_message->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MESSAGE_ALERTE));
    this->enableOtherTabs(SUJET_ACTION_ALERTE, false);
    this->setVisible(true);
}

void YerothAdminDetailWindow::retourListerCompteUtilisateur()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
    this->rendreInvisible();
}

void YerothAdminDetailWindow::retourListerLocalisation()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
    this->rendreInvisible();
}

void YerothAdminDetailWindow::retourListerCategorie()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
    this->rendreInvisible();
}

void YerothAdminDetailWindow::retourListerClient()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CLIENT);
    this->rendreInvisible();
}

void YerothAdminDetailWindow::retourListerFournisseur()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_FOURNISSEUR);
    this->rendreInvisible();
}

void YerothAdminDetailWindow::retourListerAlerte()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ALERTE);
    this->rendreInvisible();
}

void YerothAdminDetailWindow::clear_set_edit_utilisateur_comboBoxes()
{
    comboBox_detail_utilisateur_role->clear();
    comboBox_detail_utilisateur_titre->clear();
}

void YerothAdminDetailWindow::clear_set_edit_alerte_comboBoxes()
{
    comboBox_detail_alerte_condition->clear();
}

void YerothAdminDetailWindow::clear_set_edit_comboBoxes()
{
    this->clear_set_edit_utilisateur_comboBoxes();
    this->clear_set_edit_alerte_comboBoxes();
}
