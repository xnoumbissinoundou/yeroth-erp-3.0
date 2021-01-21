/*
 * yeroth-erp-admin-modifier-window.cpp
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-admin-modifier-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include <QtCore/QDebug>

#include <QtWidgets/QDesktopWidget>

#include <QtSql/QSqlError>

YerothAdminModifierWindow::YerothAdminModifierWindow()
:YerothPOSAdminWindowsCommons(QObject::tr("administration ~ modifier")),
_logger(new YerothLogger("YerothAdminModifierWindow")),
_curSearchSqlTableModel(0),
_savePassword(false)
{
    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    comboBox_modifier_utilisateur_titre->setupPopulateNORawString(_allWindows->TITRES,
    															   YerothDatabaseTableColumn::APPELATION_TITRE,
																   &YerothUtils::_titreToUserViewString);

    comboBox_modifier_utilisateur_role->setupPopulateNORawString(_allWindows->ROLES,
    															  YerothDatabaseTableColumn::NOM_ROLE,
																  &YerothUtils::_roleToUserViewString);

    setupLineEdits();

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);

    pushButton_menu->enable(this, SLOT(menu()));

    pushButton_modifier_utilisateur_annuler->enable(this, SLOT(annuler()));
    pushButton_modifier_localisation_annuler->enable(this, SLOT(annuler()));
    pushButton_modifier_categorie_annuler->enable(this, SLOT(annuler()));
    pushButton_modifier_compte_bancaire_annuler->enable(this, SLOT(annuler()));
    pushButton_modifier_alerte_annuler->enable(this, SLOT(annuler()));

    pushButton_modifier_utilisateur_valider->enable(this, SLOT(modifier_utilisateur()));
    pushButton_modifier_localisation_valider->enable(this, SLOT(modifier_localisation()));
    pushButton_modifier_categorie_valider->enable(this, SLOT(modifier_categorie()));
    pushButton_modifier_compte_bancaire_valider->enable(this, SLOT(modifier_compte_bancaire()));
    pushButton_modifier_alerte_valider->enable(this, SLOT(modifier_alerte()));

    pushButton_creer->enable(this, SLOT(creer()));
    pushButton_lister->enable(this, SLOT(lister()));
    pushButton_supprimer->enable(this, SLOT(supprimer()));

    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionCreer, SIGNAL(triggered()), this, SLOT(creer()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));
    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this, SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(radioButton_modifier_alerte_quantite, SIGNAL(clicked(bool)), this, SLOT(radioButtons_quantite()));
    connect(radioButton_modifier_alerte_periode_temps, SIGNAL(clicked(bool)), this,
            SLOT(radioButtons_periode_temps()));
}

void YerothAdminModifierWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
}

void YerothAdminModifierWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
}

void YerothAdminModifierWindow::definirManager()
{
    _logger->log("definirManager");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, true);
}

void YerothAdminModifierWindow::setupLineEdits()
{
    lineEdit_modifier_utilisateur_id->setYerothEnabled(false);
    lineEdit_modifier_alerte_designation_alerte->setYerothEnabled(false);
    lineEdit_modifier_alerte_quantite->setYerothEnabled(false);
    lineEdit_modifier_localisation_nom->setYerothEnabled(false);
    lineEdit_modifier_utilisateur_mot_passe_1->setEchoMode(QLineEdit::Password);
    lineEdit_modifier_utilisateur_mot_passe_2->setEchoMode(QLineEdit::Password);
}

void YerothAdminModifierWindow::rendreInvisible()
{
    clear_utilisateur_all_fields();
    clear_departements_de_produits_all_fields();
    clear_categorie_all_fields();
    clear_localisation_all_fields();
    clear_alerte_all_fields();
    clear_remise_all_fields();

    _savePassword = false;

    YerothPOSAdminWindowsCommons::rendreInvisible();
}

void YerothAdminModifierWindow::enableOtherTabs(enum AdminSujetAction curAction, bool enabled)
{
    tabWidget_modifier->setTabEnabled(curAction, !enabled);
    for (int k = 0; k <= SUJET_ACTION_LOCALISATION; ++k)
    {
        if (k != curAction)
        {
            tabWidget_modifier->setTabEnabled(k, enabled);
        }
    }
}

void YerothAdminModifierWindow::rendreVisible(enum AdminSujetAction selectedSujetAction)
{
    clear_utilisateur_all_fields();
    clear_departements_de_produits_all_fields();
    clear_categorie_all_fields();
    clear_compte_bancaire_all_fields();
    clear_localisation_all_fields();
    clear_alerte_all_fields();
    clear_remise_all_fields();

    switch (selectedSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_COMPTE_UTILISATEUR);
        enableOtherTabs(SUJET_ACTION_COMPTE_UTILISATEUR, false);
        setupEditCompteUtilisateur();
        break;

    case SUJET_ACTION_LOCALISATION:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_LOCALISATION);
        enableOtherTabs(SUJET_ACTION_LOCALISATION, false);
        setupEditLocalisation();
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        enableOtherTabs(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS, false);
        setupEditDepartementsDeProduits();
        break;

    case SUJET_ACTION_CATEGORIE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_CATEGORIE);
        enableOtherTabs(SUJET_ACTION_CATEGORIE, false);
        setupEditCategorie();
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_COMPTE_BANCAIRE);
        enableOtherTabs(SUJET_ACTION_COMPTE_BANCAIRE, false);
        setupEditCompteBancaire();
        break;

    case SUJET_ACTION_ALERTE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_ALERTE);
        enableOtherTabs(SUJET_ACTION_ALERTE, false);
        setupEditAlerte();
        break;

    case SUJET_ACTION_REMISE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_REMISE);
        enableOtherTabs(SUJET_ACTION_REMISE, false);
        setupEditRemise();
        break;

    case SUJET_ACTION_BON_DE_COMMANDE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_BON_DE_COMMANDE);
        enableOtherTabs(SUJET_ACTION_BON_DE_COMMANDE, false);
        break;

    default:
        break;
    }
    setVisible(true);
}


void YerothAdminModifierWindow::retourListerCompteUtilisateur()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
    rendreInvisible();
}


void YerothAdminModifierWindow::retourListerLocalisation()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
    rendreInvisible();
}


void YerothAdminModifierWindow::retourListerDepartementsDeProduits()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
    rendreInvisible();
}



void YerothAdminModifierWindow::retourListerCategorie()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
    rendreInvisible();
}


void YerothAdminModifierWindow::retourListerCompteBancaire()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
    rendreInvisible();
}


void YerothAdminModifierWindow::retourListerAlerte()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ALERTE);
    rendreInvisible();
}


void YerothAdminModifierWindow::retourListerRemise()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_REMISE);
    rendreInvisible();
}


void YerothAdminModifierWindow::annuler()
{
    switch (tabWidget_modifier->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        clear_utilisateur_all_fields();
        retourListerCompteUtilisateur();
        break;

    case SUJET_ACTION_LOCALISATION:
        clear_localisation_all_fields();
        retourListerLocalisation();
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        clear_departements_de_produits_all_fields();
        retourListerDepartementsDeProduits();
        break;

    case SUJET_ACTION_CATEGORIE:
        clear_categorie_all_fields();
        retourListerCategorie();
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        clear_compte_bancaire_all_fields();
        retourListerCompteBancaire();
        break;

    case SUJET_ACTION_ALERTE:
        clear_alerte_all_fields();
        retourListerAlerte();
        break;

    case SUJET_ACTION_REMISE:
        clear_remise_all_fields();
        retourListerRemise();
        break;

    case SUJET_ACTION_BON_DE_COMMANDE:
        break;

    default:
        break;
    }
    rendreInvisible();
}

void YerothAdminModifierWindow::creer()
{
    _allWindows->_adminCreateWindow->rendreVisible(tabWidget_modifier->currentIndex());
    rendreInvisible();
}

void YerothAdminModifierWindow::lister()
{
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_modifier->currentIndex());
    rendreInvisible();
}

void YerothAdminModifierWindow::supprimer()
{
    _allWindows->_adminListerWindow->supprimer();
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_modifier->currentIndex());
    rendreInvisible();
}

void YerothAdminModifierWindow::radioButtons_quantite()
{
    if (radioButton_modifier_alerte_periode_temps->isChecked())
    {
        radioButton_modifier_alerte_periode_temps->setChecked(false);
    }
}

void YerothAdminModifierWindow::radioButtons_periode_temps()
{
    if (radioButton_modifier_alerte_quantite->isChecked())
    {
        radioButton_modifier_alerte_quantite->setChecked(false);
    }
}

#include"modifier-utilisateur.cpp"

#include"modifier-localisation.cpp"

#include"modifier-departements_de_produits.cpp"

#include"modifier-categorie.cpp"

#include"modifier-compte-bancaire.cpp"

#include"modifier-alerte.cpp"

#include"modifier-remise.cpp"
