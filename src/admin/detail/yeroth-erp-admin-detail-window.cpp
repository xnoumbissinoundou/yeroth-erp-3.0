/*
 * yeroth-erp-admin-detail-window.cpp
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
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

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();

    dateEdit_detail_utilisateur_date_naissance->setEnabled(false);
    dateEdit_detail_localisation_date_ouverture->setEnabled(false);
    textEdit_detail_departements_de_produits_description->setYerothEnabled(false);
    textEdit_detail_categorie_description->setYerothEnabled(false);
    textEdit_detail_compte_bancaire_description_du_compte->setYerothEnabled(false);
    textEdit_detail_localisation_description_lieu->setYerothEnabled(false);
    textEdit_detail_alerte_message->setYerothEnabled(false);

    radioButton_detail_alerte_date_periode_temps->setEnabled(false);
    radioButton_detail_alerte_quantite->setEnabled(false);

    dateEdit_detail_alerte_date_debut->setYerothEnabled(false);
    dateEdit_detail_alerte_date_fin->setYerothEnabled(false);

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
    pushButton_detail_departements_de_produits_retour->enable(this, SLOT(retourListerDepartementsDeProduits()));
    pushButton_detail_categorie_retour->enable(this, SLOT(retourListerCategorie()));
    pushButton_detail_compte_bancaire_retour->enable(this, SLOT(retourListerCompteBancaire()));
    pushButton_detail_remise_retour->enable(this, SLOT(retourListerRemise()));
    pushButton_detail_alerte_retour->enable(this, SLOT(retourListerAlerte()));

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
    lineEdit_detail_utilisateur_prenom->setYerothEnabled(false);
    lineEdit_detail_utilisateur_nom->setYerothEnabled(false);
    lineEdit_detail_utilisateur_lieu_naissance->setYerothEnabled(false);
    lineEdit_detail_utilisateur_email->setYerothEnabled(false);
    lineEdit_detail_utilisateur_ville->setYerothEnabled(false);
    lineEdit_detail_utilisateur_province_etat->setYerothEnabled(false);
    lineEdit_detail_utilisateur_pays->setYerothEnabled(false);
    lineEdit_detail_utilisateur_boite_postale->setYerothEnabled(false);
    lineEdit_detail_utilisateur_numero_telephone_1->setYerothEnabled(false);
    lineEdit_detail_utilisateur_numero_telephone_2->setYerothEnabled(false);
    lineEdit_detail_utilisateur_id->setYerothEnabled(false);
    lineEdit_detail_utilisateur_mot_passe->setYerothEnabled(false);

    lineEdit_detail_departements_de_produits_nom->setYerothEnabled(false);

    lineEdit_detail_categorie_nom->setYerothEnabled(false);

    lineEdit_detail_compte_bancaire_reference_du_compte_bancaire->setYerothEnabled(false);
	lineEdit_detail_compte_bancaire_intitule_du_compte_bancaire->setYerothEnabled(false);
	lineEdit_detail_compte_bancaire_institut_bancaire->setYerothEnabled(false);

    lineEdit_detail_localisation_adresse_ip->setYerothEnabled(false);
    lineEdit_detail_localisation_nom->setYerothEnabled(false);
    lineEdit_detail_localisation_numero_unique->setYerothEnabled(false);
    lineEdit_detail_localisation_quartier->setYerothEnabled(false);
    lineEdit_detail_localisation_ville->setYerothEnabled(false);
    lineEdit_detail_localisation_province_etat->setYerothEnabled(false);
    lineEdit_detail_localisation_pays->setYerothEnabled(false);
    lineEdit_detail_localisation_boite_postale->setYerothEnabled(false);
    lineEdit_detail_localisation_email->setYerothEnabled(false);
    lineEdit_detail_localisation_numero_telephone_1->setYerothEnabled(false);
    lineEdit_detail_localisation_numero_telephone_2->setYerothEnabled(false);

    lineEdit_detail_alerte_designation_article->setYerothEnabled(false);
    lineEdit_detail_alerte_quantite->setYerothEnabled(false);
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
   rendreInvisible();
}


void YerothAdminDetailWindow::lister()
{
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_detail->currentIndex());
   rendreInvisible();
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
    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;
    case SUJET_ACTION_CATEGORIE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
        break;
    case SUJET_ACTION_COMPTE_BANCAIRE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
        break;
    case SUJET_ACTION_REMISE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_REMISE);
        break;
    case SUJET_ACTION_ALERTE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_ALERTE);
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        break;
    default:
        break;
    }

   rendreInvisible();
}


void YerothAdminDetailWindow::supprimer()
{
    _logger->log("supprimer");
    _allWindows->_adminListerWindow->supprimer();
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_detail->currentIndex());
   rendreInvisible();
}


void YerothAdminDetailWindow::rendreInvisible()
{
	_logger->log("rendreInvisible");
	lineEdit_detail_alerte_id_destinataire->clear();
	lineEdit_detail_alerte_nom_destinataire->clear();
	lineEdit_detail_alerte_designation_article->clear();
	clear_set_edit_comboBoxes();
	setVisible(false);
}


void YerothAdminDetailWindow::enableOtherTabs(enum AdminSujetAction curAction, bool enabled)
{
    tabWidget_detail->setTabEnabled(curAction, !enabled);
    for (int k = 0; k <= 7; ++k)
    {
        if (k != curAction)
        {
            tabWidget_detail->setTabEnabled(k, enabled);
        }
    }
}


void YerothAdminDetailWindow::rendreVisibleCompteUtilisateur(int sqlTableRow)
{
   tabWidget_detail->setCurrentIndex(SUJET_ACTION_COMPTE_UTILISATEUR);
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
    lineEdit_detail_utilisateur_prenom->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRENOM));
    lineEdit_detail_utilisateur_nom->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM));

    comboBox_detail_utilisateur_titre
		->addItem(YerothUtils::_titreToUserViewString.value(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::TITRE).toInt()));

    lineEdit_detail_utilisateur_lieu_naissance->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LIEU_NAISSANCE));

    dateEdit_detail_utilisateur_date_naissance->
    	setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_NAISSANCE)));

    lineEdit_detail_utilisateur_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
    lineEdit_detail_utilisateur_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));
    lineEdit_detail_utilisateur_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
    lineEdit_detail_utilisateur_province_etat->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PROVINCE_ETAT));
    lineEdit_detail_utilisateur_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_detail_utilisateur_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_detail_utilisateur_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));

    comboBox_detail_utilisateur_role->
		addItem(YerothUtils::_roleToUserViewString.value(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ROLE).toInt()));

    lineEdit_detail_utilisateur_id->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_UTILISATEUR));

    lineEdit_detail_utilisateur_mot_passe->setText("****");

    lineEdit_detail_utilisateur_localisation->setYerothEnabled(false);
    lineEdit_detail_utilisateur_localisation->setText(_allWindows->getInfoEntreprise().getLocalisation());
   enableOtherTabs(SUJET_ACTION_COMPTE_UTILISATEUR, false);
   setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleLocalisation(int sqlTableRow)
{
	tabWidget_detail->setCurrentIndex(SUJET_ACTION_LOCALISATION);

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

	enableOtherTabs(SUJET_ACTION_LOCALISATION, false);

	setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleDepartementsDeProduits(int sqlTableRow)
{
	tabWidget_detail->setCurrentIndex(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);

	YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

	YerothSqlTableModel *departementsDeProduitsTableModel = lw->getCurSearchSqlTableModel();

	if (0 == departementsDeProduitsTableModel)
	{
		departementsDeProduitsTableModel = &_allWindows->getSqlTableModel_departements_produits();
	}
	else if (0 != departementsDeProduitsTableModel
			 && !YerothUtils::isEqualCaseInsensitive(departementsDeProduitsTableModel->sqlTableName(),
												    _allWindows->DEPARTEMENTS_PRODUITS))
	{
		departementsDeProduitsTableModel = &_allWindows->getSqlTableModel_departements_produits();
	}

	QSqlRecord record = departementsDeProduitsTableModel->record(sqlTableRow);

	lineEdit_detail_departements_de_produits_nom->
		setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

	textEdit_detail_departements_de_produits_description->
		setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT));

	enableOtherTabs(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS, false);

	setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleCategorie(int sqlTableRow)
{
	tabWidget_detail->setCurrentIndex(SUJET_ACTION_CATEGORIE);

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

	enableOtherTabs(SUJET_ACTION_CATEGORIE, false);

	setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleCompteBancaire(int sqlTableRow)
{
    tabWidget_detail->setCurrentIndex(SUJET_ACTION_COMPTE_BANCAIRE);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *comptesBancairesTableModel = lw->getCurSearchSqlTableModel();

    if (!comptesBancairesTableModel)
    {
        comptesBancairesTableModel = &_allWindows->getSqlTableModel_comptes_bancaires();
    }
    else if (comptesBancairesTableModel
             && !YerothUtils::isEqualCaseInsensitive(comptesBancairesTableModel->sqlTableName(),
                     	 	 	 	 	 	 	 	 _allWindows->COMPTES_BANCAIRES))
    {
        comptesBancairesTableModel = &_allWindows->getSqlTableModel_comptes_bancaires();
    }

    QSqlRecord record = comptesBancairesTableModel->record(sqlTableRow);

    lineEdit_detail_compte_bancaire_reference_du_compte_bancaire
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE));

    lineEdit_detail_compte_bancaire_intitule_du_compte_bancaire
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

    lineEdit_detail_compte_bancaire_institut_bancaire
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::INSTITUT_BANCAIRE));

    textEdit_detail_compte_bancaire_description_du_compte
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE));

    enableOtherTabs(SUJET_ACTION_COMPTE_BANCAIRE, false);

    setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleRemise(int sqlTableRow)
{
   tabWidget_detail->setCurrentIndex(SUJET_ACTION_REMISE);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *remiseTableModel = lw->getCurSearchSqlTableModel();

    if (!remiseTableModel)
    {
        remiseTableModel = &_allWindows->getSqlTableModel_remises();
    }
    else if (remiseTableModel &&
    		 !YerothUtils::isEqualCaseInsensitive(remiseTableModel->sqlTableName(), _allWindows->REMISES))
    {
        remiseTableModel = &_allWindows->getSqlTableModel_remises();
    }

    QSqlRecord record = remiseTableModel->record(sqlTableRow);

    lineEdit_detail_remise_nom->setYerothEnabled(false);
    lineEdit_detail_remise_designation_article->setYerothEnabled(false);
    lineEdit_detail_remise_montant->setYerothEnabled(false);

    spinBox_detail_remise_pourcentage->setEnabled(false);

    dateEdit_detail_remise_date_debut->setYerothEnabled(false);
    dateEdit_detail_remise_date_fin->setYerothEnabled(false);

    textEdit_detail_remise_message->setYerothEnabled(false);

    lineEdit_detail_remise_nom->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_REMISE));
    lineEdit_detail_remise_designation_article->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

    spinBox_detail_remise_pourcentage->setValue(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REMISE_POURCENTAGE).toDouble());

    QString date_debut(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_DEBUT));
    dateEdit_detail_remise_date_debut->setDate(GET_DATE_FROM_STRING(date_debut));

    QString date_fin(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_FIN));
    dateEdit_detail_remise_date_fin->setDate(GET_DATE_FROM_STRING(date_fin));

    textEdit_detail_remise_message->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REMISE_NOTES));

   enableOtherTabs(SUJET_ACTION_REMISE, false);
   setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleAlerte(int sqlTableRow)
{
   tabWidget_detail->setCurrentIndex(SUJET_ACTION_ALERTE);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *alertesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == alertesTableModel)
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }
    else if ((0 != alertesTableModel) &&
    		 !YerothUtils::isEqualCaseInsensitive(alertesTableModel->sqlTableName(), _allWindows->ALERTES))
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }

    QSqlRecord record = alertesTableModel->record(sqlTableRow);

    lineEdit_detail_alerte_designation_alerte->setYerothEnabled(false);
    lineEdit_detail_alerte_id_destinataire->setYerothEnabled(false);
    lineEdit_detail_alerte_nom_destinataire->setYerothEnabled(false);
    lineEdit_detail_alerte_designation_article->setYerothEnabled(false);

    lineEdit_detail_alerte_designation_alerte->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_ALERTE));

    lineEdit_detail_alerte_id_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESTINATAIRE));

    lineEdit_detail_alerte_nom_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE));

    lineEdit_detail_alerte_designation_article->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

    comboBox_detail_alerte_condition->addItem(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CONDITION_ALERTE));

    double quantite = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE).toDouble();

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

   enableOtherTabs(SUJET_ACTION_ALERTE, false);

   setVisible(true);
}

void YerothAdminDetailWindow::retourListerCompteUtilisateur()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
   rendreInvisible();
}

void YerothAdminDetailWindow::retourListerLocalisation()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
   rendreInvisible();
}


void YerothAdminDetailWindow::retourListerDepartementsDeProduits()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
   rendreInvisible();
}


void YerothAdminDetailWindow::retourListerCategorie()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
   rendreInvisible();
}


void YerothAdminDetailWindow::retourListerCompteBancaire()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
   rendreInvisible();
}


void YerothAdminDetailWindow::retourListerRemise()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_REMISE);
   rendreInvisible();
}

void YerothAdminDetailWindow::retourListerAlerte()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ALERTE);
   rendreInvisible();
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
   clear_set_edit_utilisateur_comboBoxes();
   clear_set_edit_alerte_comboBoxes();
}
