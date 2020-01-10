/**
 * yeroth-erp-admin-window.cpp
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-admin-window.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/process/yeroth-erp-process-info.hpp"

#include "src/utils/yeroth-erp-database.hpp"

#include "src/dbus/yeroth-erp-dbus-server.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include <QtCore/QDebug>

#include <QtCore/QTemporaryFile>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QStyleFactory>


const QString YerothAdminWindow::EXPORTER(QObject::trUtf8("exporter"));
const QString YerothAdminWindow::IMPORTER(QObject::trUtf8("importer"));
const QString YerothAdminWindow::EFFACER(QObject::trUtf8("effacer"));

const QString YerothAdminWindow::DB(QObject::trUtf8("base de données"));
const QString YerothAdminWindow::TABLEAU(QObject::trUtf8("tableau"));
const QString YerothAdminWindow::DONNEES(QObject::trUtf8("données"));

#ifdef YEROTH_FRANCAIS_LANGUAGE
const QString YerothAdminWindow::CREER(QObject::trUtf8(ACTION_ADMIN_OPERATIONS_CREATE_FR));
const QString YerothAdminWindow::LISTER(QObject::trUtf8(ACTION_ADMIN_OPERATIONS_LIST_FR));
const QString YerothAdminWindow::MODIFIER(QObject::trUtf8(ACTION_ADMIN_OPERATIONS_MODIFY_FR));
const QString YerothAdminWindow::SUPPRIMER(QObject::trUtf8(ACTION_ADMIN_OPERATIONS_DELETE_FR));

const QString YerothAdminWindow::COMPTE_UTILISATEUR(QObject::trUtf8(SUBJECT_ADMIN_OPERATIONS_USER_ACCOUNT_FR));
const QString YerothAdminWindow::LOCALISATION(QObject::trUtf8(SUBJECT_ADMIN_OPERATIONS_SITE_FR));
const QString YerothAdminWindow::CATEGORIE(QObject::trUtf8(SUBJECT_ADMIN_OPERATIONS_CATEGORY_FR));
const QString YerothAdminWindow::FOURNISSEUR(QObject::trUtf8(SUBJECT_ADMIN_OPERATIONS_SUPPLIER_FR));
const QString YerothAdminWindow::ALERTE(QObject::trUtf8(SUBJECT_ADMIN_OPERATIONS_ALERT_FR));
const QString YerothAdminWindow::BON_DE_COMMANDE(QObject::trUtf8(SUBJECT_ADMIN_OPERATIONS_COMMAND_SHEET_FR));
const QString YerothAdminWindow::REMISE(QObject::trUtf8(SUBJECT_ADMIN_OPERATIONS_DISCOUNT_FR));

#elif YEROTH_ENGLISH_LANGUAGE
const QString YerothAdminWindow::CREER(QObject::tr(ACTION_ADMIN_OPERATIONS_CREATE_EN));
const QString YerothAdminWindow::LISTER(QObject::tr(ACTION_ADMIN_OPERATIONS_LIST_EN));
const QString YerothAdminWindow::MODIFIER(QObject::tr(ACTION_ADMIN_OPERATIONS_MODIFY_EN));
const QString YerothAdminWindow::SUPPRIMER(QObject::tr(ACTION_ADMIN_OPERATIONS_DELETE_EN));

const QString YerothAdminWindow::COMPTE_UTILISATEUR(QObject::tr(SUBJECT_ADMIN_OPERATIONS_USER_ACCOUNT_EN));
const QString YerothAdminWindow::LOCALISATION(QObject::tr(SUBJECT_ADMIN_OPERATIONS_SITE_EN));
const QString YerothAdminWindow::CATEGORIE(QObject::tr(SUBJECT_ADMIN_OPERATIONS_CATEGORY_EN));
const QString YerothAdminWindow::FOURNISSEUR(QObject::tr(SUBJECT_ADMIN_OPERATIONS_SUPPLIER_EN));
const QString YerothAdminWindow::ALERTE(QObject::tr(SUBJECT_ADMIN_OPERATIONS_ALERT_EN));
const QString YerothAdminWindow::BON_DE_COMMANDE(QObject::tr(SUBJECT_ADMIN_OPERATIONS_COMMAND_SHEET_EN));
const QString YerothAdminWindow::REMISE(QObject::tr(SUBJECT_ADMIN_OPERATIONS_DISCOUNT_EN));
#endif

YerothAdminWindow::YerothAdminWindow()
:YerothPOSAdminWindowsCommons(QObject::trUtf8("administration ~ acceuil")),
 _logger(new YerothLogger("YerothAdminWindow")),
 _curAdminAction(ACTION_CREER),
 _curAdminSujetAction(SUJET_ACTION_COMPTE_UTILISATEUR),
 _curAdminMaintainAction(ACTION_EXPORTER),
 _curAdminSujetsMaintainAction(SUJET_ACTION_DB),
 _actionsToConst(0),
 _sujetActionsToConst(0)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    lineEdit_this_localisation->setEnabled(false);

    lineEdit_this_localisation->setText(_allWindows->getInfoEntreprise().getLocalisation());

    _yerothAdminWindowTitleStart.append(this->windowTitle());

    //en cours de programmation
    tabWidget_administration->setTabEnabled(MAINTENANCE, false);

    this->setupValidators();

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    _actionsToConst = new QMap < QString, AdminAction >;
    _sujetActionsToConst = new QMap < QString, AdminSujetAction >;

    _actionsToConst->insert(CREER, ACTION_CREER);
    _actionsToConst->insert(LISTER, ACTION_LISTER);
    _actionsToConst->insert(MODIFIER, ACTION_MODIFIER);
    _actionsToConst->insert(SUPPRIMER, ACTION_SUPPRIMER);
    _actionsToConst->insert(EXPORTER, ACTION_EXPORTER);
    _actionsToConst->insert(IMPORTER, ACTION_IMPORTER);
    _actionsToConst->insert(EFFACER, ACTION_EFFACER);

    _sujetActionsToConst->insert(ALERTE, SUJET_ACTION_ALERTE);
    _sujetActionsToConst->insert(BON_DE_COMMANDE, SUJET_ACTION_BON_DE_COMMANDE);
    _sujetActionsToConst->insert(CATEGORIE, SUJET_ACTION_CATEGORIE);
    _sujetActionsToConst->insert(COMPTE_UTILISATEUR, SUJET_ACTION_COMPTE_UTILISATEUR);
    _sujetActionsToConst->insert(FOURNISSEUR, SUJET_ACTION_FOURNISSEUR);
    _sujetActionsToConst->insert(LOCALISATION, SUJET_ACTION_LOCALISATION);
    _sujetActionsToConst->insert(REMISE, SUJET_ACTION_REMISE);

    _sujetActionsToConst->insert(DB, SUJET_ACTION_DB);
    _sujetActionsToConst->insert(TABLEAU, SUJET_ACTION_TABLEAU);
    _sujetActionsToConst->insert(DONNEES, SUJET_ACTION_DONNEES);

    comboBox_sujets_actions->addItem(ALERTE);
    comboBox_sujets_actions->addItem(BON_DE_COMMANDE);
    comboBox_sujets_actions->addItem(CATEGORIE);
    comboBox_sujets_actions->addItem(FOURNISSEUR);
    comboBox_sujets_actions->addItem(COMPTE_UTILISATEUR);
    comboBox_sujets_actions->addItem(LOCALISATION);
    comboBox_sujets_actions->addItem(REMISE);

    comboBox_sujets_maintain->addItem(DB);
    comboBox_sujets_maintain->addItem(TABLEAU);
    comboBox_sujets_maintain->addItem(DONNEES);

    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);
    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO);
    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);
    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    comboBox_actions->addItem(CREER);
    comboBox_actions->addItem(LISTER);
    comboBox_actions->addItem(MODIFIER);
    comboBox_actions->addItem(SUPPRIMER);

    comboBox_actions_maintain->addItem(EXPORTER);
    comboBox_actions_maintain->addItem(IMPORTER);
    comboBox_actions_maintain->addItem(EFFACER);

    pushButton_menu_go->disable(this);
    pushButton_creer->disable(this);
    pushButton_lister->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_operation_go->disable(this);

    pushButton_entreprise_reinitialiser->enable(this, SLOT(read_entreprise_info_database_table()));

    pushButton_entreprise_enregistrer->enable(this, SLOT(enregistrer_entreprise_info_database_table()));

    pushButton_rafraichir_statut_alert_daemon->enable(this, SLOT(set_colour_status_of_alert_daemon_process_buttons()));

    pushButton_demarrer_alert_daemon->enable(this, SLOT(start_alert_daemon_process()));

    pushButton_stopper_alert_daemon->enable(this, SLOT(stop_alert_daemon_process()));

    label_ON->setVisible(false);
    label_OFF->setVisible(true);

    label_ONLINE->setVisible(false);
    label_OFFLINE->setVisible(true);

    pushButton_choose_pdfReader->enable(this, SLOT(choose_path_pdfReader()));

    pushButton_choose_fichier_systeme_imprimante_thermique->enable(this, SLOT(choose_path_thermalPrinterDeviceFile()));

    pushButton_choose_repertoire_systeme_latex->enable(this, SLOT(choose_path_latex_system_root_folder()));

    pushButton_choose_repertoire_fichiers_temporaires->enable(this, SLOT(choose_repertoire_fichiers_temporaires()));

    pushButton_alertes_reinitialiser->enable(this, SLOT(reinitialiser_alert_system_configuration()));
    pushButton_alertes_enregistrer->enable(this, SLOT(enregistrer_alert_system_configuration()));
    pushButton_parametres_reinitialiser->enable(this, SLOT(reinitialiser_app_parameters_configuration()));
    pushButton_parametres_enregistrer->enable(this, SLOT(enregistrer_app_parameters_configuration()));
    pushButton_connecter_localisation->enable(this, SLOT(connecter_localisation_db()));
    pushButton_deconnecter_localisation->enable(this, SLOT(deconnecter_localisation_db()));

    connect(comboBox_impression_sur, SIGNAL(currentIndexChanged(const QString &)),
    		this, SLOT(handleThermalPrinterConfiguration(const QString &)));

    connect(checkBox_activer_registre_de_caisse, SIGNAL(stateChanged(int)),
    		this, SLOT(handleCheckboxActiverRegistreCaisse(int)));

    connect(comboBox_impression_sur, SIGNAL(currentTextChanged(const QString &)),
    		this, SLOT(choix_registre_de_caisse(const QString &)));

    connect(actionStocks, SIGNAL(triggered()), this, SLOT(action_lister()));
    connect(actionCreer, SIGNAL(triggered()), this, SLOT(action_creer()));
    connect(actionGo, SIGNAL(triggered()), this, SLOT(gerer_choix_action()));
    connect(actionModifier, SIGNAL(triggered()), this, SLOT(action_lister()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(action_lister()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionGuideUtilisateur, SIGNAL(triggered()), this, SLOT(getManuelUtilisateurPDF()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this, SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

    connect(comboBox_sujets_actions, SIGNAL(activated(int)), this, SLOT(gerer_choix_action()));

    connect(lineEdit_localisation, SIGNAL(editingFinished()), this, SLOT(set_localisation_adresse_ip_text()));

    connect(tabWidget_administration, SIGNAL(currentChanged(int)), this, SLOT(handleTabChanged(int)));
}

YerothAdminWindow::~YerothAdminWindow()
{
    delete _logger;
    delete _actionsToConst;
    delete _sujetActionsToConst;
}

void YerothAdminWindow::setupValidators()
{
    lineEdit_alert_period_time_interval->setValidator(&YerothUtils::IntValidator);
    lineEdit_alert_quantity_time_interval->setValidator(&YerothUtils::IntValidator);
    lineEdit_annee_depart_rapports_chiffre_affaire->setValidator(&YerothUtils::IntValidator);
}

void YerothAdminWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
    pushButton_menu_go->disable(this);
    pushButton_creer->disable(this);
    pushButton_lister->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_operation_go->disable(this);
}

void YerothAdminWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
    pushButton_menu_go->enable(this, SLOT(gerer_choix_action()));
    pushButton_creer->enable(this, SLOT(action_creer()));
    pushButton_lister->enable(this, SLOT(action_lister()));
    pushButton_modifier->enable(this, SLOT(action_modifier()));
    pushButton_supprimer->enable(this, SLOT(action_lister()));
    pushButton_operation_go->enable(this, SLOT(gerer_choix_action()));
    _allWindows->_adminWindow->rendreVisible(_curStocksTableModel);
}

void YerothAdminWindow::definirManager()
{
    _logger->log("definirManager");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, true);
    pushButton_menu_go->enable(this, SLOT(gerer_choix_action()));
    pushButton_creer->enable(this, SLOT(action_creer()));
    pushButton_lister->enable(this, SLOT(action_lister()));
    pushButton_modifier->enable(this, SLOT(action_modifier()));
    pushButton_supprimer->enable(this, SLOT(action_lister()));
    pushButton_operation_go->enable(this, SLOT(gerer_choix_action()));
}

void YerothAdminWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    if (0 != stocksTableModel)
    {
        _curStocksTableModel = stocksTableModel;
    }

	YerothPOSUser *aUser = _allWindows->getUser();

	if (0 != aUser)
	{
		if (aUser->isManager())
		{
			tabWidget_administration->setTabEnabled(5, true);
		}
		else
		{
			tabWidget_administration->setTabEnabled(5, false);
		}
	}

    set_colour_status_of_alert_daemon_process_buttons();

    this->read_configuration();

    lineEdit_localisation->setupMyStaticQCompleter(_allWindows->LOCALISATIONS, "nom_localisation", false);

    if (YerothERPConfig::_distantSiteConnected)
    {
        lineEdit_localisation->setText(YerothERPConfig::_connectedSite);
        lineEdit_localisation_adresse_ip->setText(YerothERPConfig::_connectedSiteIPAddress);

        label_ONLINE->setVisible(true);
        label_OFFLINE->setVisible(false);
    }
    else
    {
        lineEdit_localisation->clear();
        lineEdit_localisation_adresse_ip->clear();

        label_ONLINE->setVisible(false);
        label_OFFLINE->setVisible(true);
    }

    _allWindows->_adminListerWindow->lister_categorie(&_allWindows->getSqlTableModel_categories());
    _allWindows->_adminListerWindow->lister_utilisateur(&_allWindows->getSqlTableModel_users());
    _allWindows->_adminListerWindow->lister_fournisseur(&_allWindows->getSqlTableModel_fournisseurs());

    handleTabChanged(tabWidget_administration->currentIndex());

    read_entreprise_info_database_table(false);

    this->setVisible(true);
}

void YerothAdminWindow::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    this->rendreInvisible();
}


void YerothAdminWindow::handleCheckboxActiverRegistreCaisse(int state)
{
	if (Qt::Checked	== state)
	{
		YerothERPConfig::ouvrirRegistreDeCaisse = true;
	}
	else
	{
		YerothERPConfig::ouvrirRegistreDeCaisse = false;
	}
}


void YerothAdminWindow::choix_registre_de_caisse(const QString &labelImpressionSur)
{
	if (YerothUtils::isEqualCaseInsensitive(labelImpressionSur, YerothUtils::IMPRIMANTE_EPSON_TM_T20ii))
	{
		label_activer_registre_de_caisse->setEnabled(true);
		checkBox_activer_registre_de_caisse->setEnabled(true);

		comboBox_format_de_facture->clear();
        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_PETIT);
        comboBox_format_de_facture->setYerothEnabled(false);
	}
	else if (YerothUtils::isEqualCaseInsensitive(labelImpressionSur, YerothUtils::IMPRIMANTE_PDF))
	{
		label_activer_registre_de_caisse->setEnabled(false);
		checkBox_activer_registre_de_caisse->setChecked(false);
		checkBox_activer_registre_de_caisse->setEnabled(false);

		comboBox_format_de_facture->clear();
        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_PETIT);
        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER);
        comboBox_format_de_facture->setYerothEnabled(true);
	}
}


void YerothAdminWindow::creer(enum AdminSujetAction selectedSujetAction)
{
    _allWindows->_adminCreateWindow->rendreVisible(selectedSujetAction);
    this->rendreInvisible();
}

void YerothAdminWindow::lister(enum AdminSujetAction selectedSujetAction)
{
    _allWindows->_adminListerWindow->rendreVisible(selectedSujetAction);
    this->rendreInvisible();
}

void YerothAdminWindow::modifier(enum AdminSujetAction selectedSujetAction)
{
    _allWindows->_adminListerWindow->rendreVisible(selectedSujetAction);
    this->rendreInvisible();
}

void YerothAdminWindow::enableAllOperationsTabPushButtons()
{
    pushButton_menu_go->enable(this, SLOT(gerer_choix_action()));
    pushButton_creer->enable(this, SLOT(action_creer()));
    pushButton_lister->enable(this, SLOT(action_lister()));
    pushButton_modifier->enable(this, SLOT(action_modifier()));
    pushButton_supprimer->enable(this, SLOT(action_lister()));
}

void YerothAdminWindow::disableAllOperationsTabPushButtons()
{
	pushButton_menu_go->disable(this);
    pushButton_creer->disable(this);
    pushButton_lister->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
}

void YerothAdminWindow::setCurrentAdminWindowTitle(enum YerothAdminWindowTabPositionsTypes tabPosition)
{
    QStringList adminWindowTitleParts = _yerothAdminWindowTitleStart.split('~');
    //adminWindowTitleParts.at(0) contient le string 'Yeroth-erp-3.0'.

    if (2 <= adminWindowTitleParts.size())
    {
        QString newAdminWindowTitle(adminWindowTitleParts.at(0).trimmed());
        newAdminWindowTitle.append(" ~ ").append(tabWidget_administration->tabText(tabPosition)).append(" ~ ").
        append(adminWindowTitleParts.at(1).trimmed());
        setWindowTitle(newAdminWindowTitle);
    }
}

void YerothAdminWindow::handleTabChanged(int currentTab)
{
    switch (currentTab)
    {
    case OPERATIONS:
        enableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(OPERATIONS);
        break;
    case CONNECTER_LOCALISATION:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(CONNECTER_LOCALISATION);
        break;
    case MAINTENANCE:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(MAINTENANCE);
        break;
    case PARAMETRES_APPLICATION:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(PARAMETRES_APPLICATION);
        break;
    case SYSTEME_DALERTES:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(SYSTEME_DALERTES);
        break;
    case DONNEES_ENTREPRISE:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(DONNEES_ENTREPRISE);
        break;
    default:
        disableAllOperationsTabPushButtons();
        setWindowTitle(_yerothAdminWindowTitleStart);
        break;
    }
}

void YerothAdminWindow::action_creer()
{
    _curAdminSujetAction = _sujetActionsToConst->value(comboBox_sujets_actions->currentText());
    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        creer(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;
    case SUJET_ACTION_LOCALISATION:
        creer(SUJET_ACTION_LOCALISATION);
        break;
    case SUJET_ACTION_CATEGORIE:
        creer(SUJET_ACTION_CATEGORIE);
        break;
    case SUJET_ACTION_FOURNISSEUR:
        creer(SUJET_ACTION_FOURNISSEUR);
        break;
    case SUJET_ACTION_ALERTE:
        creer(SUJET_ACTION_ALERTE);
        break;
    case SUJET_ACTION_REMISE:
        creer(SUJET_ACTION_REMISE);
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        creer(SUJET_ACTION_BON_DE_COMMANDE);
        break;
    default:
        break;
    }
}

void YerothAdminWindow::action_lister()
{
    _curAdminSujetAction = _sujetActionsToConst->value(comboBox_sujets_actions->currentText());
    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        lister(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;
    case SUJET_ACTION_LOCALISATION:
        lister(SUJET_ACTION_LOCALISATION);
        break;
    case SUJET_ACTION_CATEGORIE:
        lister(SUJET_ACTION_CATEGORIE);
        break;
    case SUJET_ACTION_FOURNISSEUR:
        lister(SUJET_ACTION_FOURNISSEUR);
        break;
    case SUJET_ACTION_ALERTE:
        lister(SUJET_ACTION_ALERTE);
        break;
    case SUJET_ACTION_REMISE:
        lister(SUJET_ACTION_REMISE);
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        lister(SUJET_ACTION_BON_DE_COMMANDE);
        break;
    default:
        break;
    }
}

void YerothAdminWindow::action_modifier()
{
    _curAdminSujetAction = _sujetActionsToConst->value(comboBox_sujets_actions->currentText());
    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        modifier(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;
    case SUJET_ACTION_LOCALISATION:
        modifier(SUJET_ACTION_LOCALISATION);
        break;
    case SUJET_ACTION_CATEGORIE:
        modifier(SUJET_ACTION_CATEGORIE);
        break;
    case SUJET_ACTION_FOURNISSEUR:
        modifier(SUJET_ACTION_FOURNISSEUR);
        break;
    case SUJET_ACTION_REMISE:
        modifier(SUJET_ACTION_REMISE);
        break;
    case SUJET_ACTION_ALERTE:
        modifier(SUJET_ACTION_ALERTE);
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        modifier(SUJET_ACTION_BON_DE_COMMANDE);
        break;
    default:
        break;
    }
}

void YerothAdminWindow::action_supprimer()
{
    _curAdminSujetAction = _sujetActionsToConst->value(comboBox_sujets_actions->currentText());
    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        modifier(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;
    case SUJET_ACTION_LOCALISATION:
        modifier(SUJET_ACTION_LOCALISATION);
        break;
    case SUJET_ACTION_CATEGORIE:
        modifier(SUJET_ACTION_CATEGORIE);
        break;
    case SUJET_ACTION_FOURNISSEUR:
        modifier(SUJET_ACTION_FOURNISSEUR);
        break;
    case SUJET_ACTION_ALERTE:
        modifier(SUJET_ACTION_ALERTE);
        break;
    case SUJET_ACTION_REMISE:
        modifier(SUJET_ACTION_REMISE);
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        modifier(SUJET_ACTION_BON_DE_COMMANDE);
        break;
    default:
        break;
    }
}

void YerothAdminWindow::action_exporter()
{
    _curAdminSujetsMaintainAction = _sujetActionsToConst->value(comboBox_sujets_maintain->currentText());
    switch (_curAdminSujetsMaintainAction)
    {
    case SUJET_ACTION_DB:
        maintenance_exporter(SUJET_ACTION_DB);
        break;
    case SUJET_ACTION_TABLEAU:
        maintenance_exporter(SUJET_ACTION_TABLEAU);
        break;
    default:
        break;
    }
}

void YerothAdminWindow::action_importer()
{
    _curAdminSujetsMaintainAction = _sujetActionsToConst->value(comboBox_sujets_maintain->currentText());
    switch (_curAdminSujetsMaintainAction)
    {
    case SUJET_ACTION_DB:
        maintenance_importer(SUJET_ACTION_DB);
        break;
    case SUJET_ACTION_TABLEAU:
        maintenance_importer(SUJET_ACTION_TABLEAU);
        break;
    default:
        break;
    }
}

void YerothAdminWindow::action_effacer()
{
    _curAdminSujetsMaintainAction = _sujetActionsToConst->value(comboBox_sujets_maintain->currentText());
    switch (_curAdminSujetsMaintainAction)
    {
    case SUJET_ACTION_DB:
        maintenance_importer(SUJET_ACTION_DB);
        break;
    case SUJET_ACTION_TABLEAU:
        maintenance_importer(SUJET_ACTION_TABLEAU);
        break;
    case SUJET_ACTION_DONNEES:
        maintenance_effacer(SUJET_ACTION_DONNEES);
        break;
    default:
        break;
    }
}

void YerothAdminWindow::gerer_choix_action()
{
    //qDebug() << "YerothAdminWindow::gerer_choix_action()";
    _curAdminAction = _actionsToConst->value(comboBox_actions->currentText());
    switch (_curAdminAction)
    {
    case ACTION_CREER:
        action_creer();
        break;
    case ACTION_LISTER:
        action_lister();
        break;
    case ACTION_MODIFIER:
        action_modifier();
        break;
    case ACTION_SUPPRIMER:
        action_supprimer();
        break;
    case ACTION_EXPORTER:
        action_exporter();
        break;
    case ACTION_IMPORTER:
        action_importer();
        break;
    default:
        break;
    }
}

void YerothAdminWindow::choose_path_pdfReader()
{
    QString pdfReaderFilePath =
        QFileDialog::getOpenFileName(this,
                                     QObject::trUtf8("Choisir le chemin qui mène au lecteur de fichiers PDF"),
                                     QString::null, QString::null);
    if (!pdfReaderFilePath.isEmpty())
    {
        lineEdit_pdfReader->setText(pdfReaderFilePath);
    }
}


void YerothAdminWindow::choose_path_thermalPrinterDeviceFile()
{
    QString thermalPrinterDeviceFileFullPath =
        QFileDialog::getOpenFileName(this, QObject::trUtf8("Chemin jusqu'au fichier système représentant l'imprimante thermique"),
                                     QString::null, QString::null);
    if (!thermalPrinterDeviceFileFullPath.isEmpty())
    {
    	lineEdit_fichier_systeme_imprimante_thermique->setText(thermalPrinterDeviceFileFullPath);
    }
}


void YerothAdminWindow::choose_path_latex_system_root_folder()
{
    QString latexSystemRootFolderPath =
    		QFileDialog::getExistingDirectory(this,
    										  QObject::trUtf8("Choisir le chemin jusqu'au répertoire du système de documentation Latex"),
											  QString::null,
											  QFileDialog::ShowDirsOnly);
    if (!latexSystemRootFolderPath.isEmpty())
    {
    	lineEdit_repertoire_systeme_latex->setText(latexSystemRootFolderPath);
    }
}

void YerothAdminWindow::choose_repertoire_fichiers_temporaires()
{
    QString temporaryFileDirPath =
        QFileDialog::getExistingDirectory(this,
                                          QObject::
                                          trUtf8
                                          ("Choisir le chemin qui mène au répertoire des fichiers temporaires"),
                                          QString::null, QFileDialog::ShowDirsOnly);
    if (!temporaryFileDirPath.isEmpty())
    {
        lineEdit_repertoire_fichiers_temporaires->setText(temporaryFileDirPath);
    }
}

void YerothAdminWindow::read_configuration()
{
   	checkBox_activer_registre_de_caisse->setChecked(YerothERPConfig::ouvrirRegistreDeCaisse);

	lineEdit_devise->setText(YerothERPConfig::currency);

	lineEdit_pdfReader->setText(YerothERPConfig::pathToPdfReader);

	lineEdit_fichier_systeme_imprimante_thermique->setText(YerothERPConfig::pathToThermalPrinterDeviceFile);

	lineEdit_repertoire_systeme_latex->setText(YerothERPConfig::pathToLatexSystemRootFolder);

    lineEdit_alert_period_time_interval->setText(QString::number(YerothERPConfig::alert_period_time_interval));

    lineEdit_alert_quantity_time_interval->setText(QString::number(YerothERPConfig::alert_quantity_time_interval));

    lineEdit_repertoire_fichiers_temporaires->setText(YerothERPConfig::temporaryFilesDir);

    lineEdit_tva_value->setText(YerothUtils::getTvaString());

    lineEdit_annee_depart_rapports_chiffre_affaire->setText(YerothERPConfig::annee_depart_rapports_chiffre_affaire_value);

    comboBox_strategie_vente_sortie->clear();
    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);
    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO);
    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);
    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL))
    {
        comboBox_strategie_vente_sortie->setCurrentIndex(0);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO))
    {
        comboBox_strategie_vente_sortie->setCurrentIndex(1);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO))
    {
        comboBox_strategie_vente_sortie->setCurrentIndex(2);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO))
    {
        comboBox_strategie_vente_sortie->setCurrentIndex(3);
    }

    comboBox_impression_sur->clear();
    comboBox_impression_sur->addItem(YerothERPConfig::printer);

    if (YerothUtils::
            isEqualCaseInsensitive(YerothERPConfig::printer, YerothUtils::IMPRIMANTE_PDF))
    {
        comboBox_impression_sur->addItem(YerothUtils::IMPRIMANTE_EPSON_TM_T20ii);
    }
    else
    {
        comboBox_impression_sur->addItem(YerothUtils::IMPRIMANTE_PDF);
    }
}


void YerothAdminWindow::handleThermalPrinterConfiguration(const QString &addedPrinterValue)
{
	if (YerothUtils::isEqualCaseInsensitive(YerothUtils::IMPRIMANTE_EPSON_TM_T20ii, addedPrinterValue))
	{
		comboBox_format_de_facture->clear();
        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_PETIT);
        comboBox_format_de_facture->setYerothEnabled(false);

        YerothERPConfig::receiptFormat = YerothERPConfig::RECEIPT_FORMAT_PETIT;
	}
	else
	{
		comboBox_format_de_facture->clear();

		comboBox_format_de_facture->addItem(YerothERPConfig::receiptFormat);

	    if (YerothUtils::
	            isEqualCaseInsensitive(YerothERPConfig::RECEIPT_FORMAT_PETIT, YerothERPConfig::receiptFormat))
	    {
	        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER);
	    }
	    else if (YerothUtils::
	             isEqualCaseInsensitive(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER,
	                                    YerothERPConfig::receiptFormat))
	    {
	        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_PETIT);
	    }

        comboBox_format_de_facture->setYerothEnabled(true);
	}
}


void YerothAdminWindow::read_entreprise_info_database_table(bool initalizationValues /* = true */)
{
	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 != allWindows)
	{
		YerothSqlTableModel &entreprise_info_TableModel =
				allWindows->getSqlTableModel_entreprise_info();

		int row = 0;

		if (true == initalizationValues)
		{
		    QString msgReinit(QObject::trUtf8("Réinitialiser les données de l'entreprise "
		                                      "avec les paramètres par défaut ?"));

		    if (QMessageBox::Ok == YerothQMessageBox::question(this,
		    		QObject::trUtf8("réinitialiser"),
		            msgReinit, QMessageBox::Cancel, QMessageBox::Ok))
		    {
				row = 0;
		    }
		    else
		    {
		    	return ;
		    }
		}
		else
		{
			row = 1;
		}

		QSqlRecord entrepriseInfoRecord = entreprise_info_TableModel.record(row);

		lineEdit_entreprise_denomination_de_lentreprise->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 1) ));

		lineEdit_entreprise_siege_social->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 2) ));

		lineEdit_entreprise_localisation->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 3) ));

		lineEdit_entreprise_boite_postale->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 4) ));

		lineEdit_entreprise_adresse->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 5) ));

		lineEdit_entreprise_ville->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 6) ));

		lineEdit_entreprise_pays->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 7) ));

		lineEdit_entreprise_email->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 8) ));

		lineEdit_entreprise_telephone->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 9) ));

		lineEdit_entreprise_numero_de_contribuable->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 10) ));

		lineEdit_entreprise_secteurs_dactivites->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 11) ));

		lineEdit_entreprise_agence_de_compte_bancaire->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 12) ));

		lineEdit_entreprise_secteurs_numero_de_compte_bancaire->setText(
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 13) ));
	}
}


void YerothAdminWindow::read_alert_system_init_configuration()
{
    YerothSqlTableModel & initConfigurationsTableModel = _allWindows->getSqlTableModel_init_configurations();

    QSqlRecord initConfigurationRecord =
        initConfigurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);

    QString alertPeriodTimeIntervalValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));
    initConfigurationRecord =
        initConfigurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);

    QString alertQuantityTimeIntervalValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));
    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_SALES_STRATEGY);

    QString salesStrategyValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    lineEdit_alert_period_time_interval->setText(alertPeriodTimeIntervalValue);
    lineEdit_alert_quantity_time_interval->setText(alertQuantityTimeIntervalValue);
}

void YerothAdminWindow::read_app_parameters_init_configuration()
{
    YerothSqlTableModel & initConfigurationsTableModel = _allWindows->getSqlTableModel_init_configurations();

    QSqlRecord initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_PDF_READER);
    QString pdfReaderValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_THERMAL_PRINTER_DEVICE_FILE_FULL_PATH);
    QString thermalPrinterDeviceFileFullPathValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_LATEX_SYSTEM_ROOT_FOLDER);
    QString latexSystemRootFolderValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_TEMPORARY_FILES_DIR);
    QString temporaryFilesDirValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_TVA_VALUE);
    QString tvaValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_SALES_STRATEGY);
    QString salesStrategyValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_CURRENCY);
    QString currencyValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_PRINTER);
    QString printerValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_OPEN_CASH_DRAWER);
    int openCashDrawerValue = GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration").toInt();

    initConfigurationRecord = initConfigurationsTableModel.record(YerothERPConfig::CONFIG_TYPE_OF_FACTURATION);
    QString typeOfFacturationValue(GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));

    QString userLanguageTypeOfFacturationValue(
    		YerothUtils::getCurrentAdminWindowReceiptsFormatAccordingToLanguage(typeOfFacturationValue));

    /*
     * OPEN_CASH_DRAWER INITIALIZATION CONFIGURATION
     */
    if (YerothERPConfig::CLOSE_CASH_DRAWER == openCashDrawerValue)
    {
    	checkBox_activer_registre_de_caisse->setChecked(false);
    }
    else
    {
    	checkBox_activer_registre_de_caisse->setChecked(true);
    }

    /*
     * PRINTER INITIALIZATION CONFIGURATION
     */
    comboBox_impression_sur->clear();

    comboBox_impression_sur->addItem(printerValue);

    if (YerothUtils::isEqualCaseInsensitive(YerothUtils::IMPRIMANTE_PDF, printerValue))
    {
        comboBox_impression_sur->addItem(YerothUtils::IMPRIMANTE_EPSON_TM_T20ii);
    }
    else
    {
        comboBox_impression_sur->addItem(YerothUtils::IMPRIMANTE_PDF);
    }

    comboBox_format_de_facture->clear();

    comboBox_format_de_facture->addItem(userLanguageTypeOfFacturationValue);

    if (YerothUtils::
            isEqualCaseInsensitive(YerothERPConfig::RECEIPT_FORMAT_PETIT, userLanguageTypeOfFacturationValue))
    {
        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER);
        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_PETIT);
    }
    else
    {
        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_PETIT);
        comboBox_format_de_facture->addItem(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER);
    }

    lineEdit_devise->setText(currencyValue);

    lineEdit_pdfReader->setText(pdfReaderValue);

    lineEdit_fichier_systeme_imprimante_thermique->setText(thermalPrinterDeviceFileFullPathValue);

    lineEdit_repertoire_systeme_latex->setText(latexSystemRootFolderValue);

    lineEdit_repertoire_fichiers_temporaires->setText(temporaryFilesDirValue);

    lineEdit_tva_value->setText(tvaValue);

    {
        comboBox_strategie_vente_sortie->clear();
        //qDebug() << "++ test";
        comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);
        comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO);
        comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);
        comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

        int salesStrategyId = comboBox_strategie_vente_sortie->findText(salesStrategyValue);

        comboBox_strategie_vente_sortie->setCurrentIndex(salesStrategyId);
    }
}

void YerothAdminWindow::reinitialiser_alert_system_configuration()
{
    _logger->log("reinitialiser_configuration");
    QString msgReinit(QObject::trUtf8("Réinitialiser la configuration du système d'alertes "
                                      "avec les paramètres par défaut ?"));
    if (QMessageBox::Ok == YerothQMessageBox::question(this, QObject::trUtf8("réinitialiser"), msgReinit,
            QMessageBox::Cancel, QMessageBox::Ok))
    {
        read_alert_system_init_configuration();
    }
    else
    {
    }
}

void YerothAdminWindow::reinitialiser_app_parameters_configuration()
{
    _logger->log("reinitialiser_configuration");
    QString msgReinit(QObject::trUtf8("Réinitialiser la configuration (générale) "
                                      "avec les paramètres par défaut ?"));
    if (QMessageBox::Ok == YerothQMessageBox::question(this, QObject::trUtf8("réinitialiser"),
            msgReinit, QMessageBox::Cancel, QMessageBox::Ok))
    {
        read_app_parameters_init_configuration();
    }
    else
    {
    }
}


void YerothAdminWindow::enregistrer_entreprise_info_database_table()
{
	_logger->log("enregistrer_entreprise_info_database_table");

	QString msgEnregistrer(QObject::
			trUtf8("Enregistrer les données de l'entreprise (modifiées) ?"));
	if (QMessageBox::Ok ==
			YerothQMessageBox::question(this,
					QObject::
					trUtf8("enregistrer"),
					msgEnregistrer, QMessageBox::Cancel, QMessageBox::Ok))
	{
		YerothSqlTableModel &entreprise_info_TableModel =
				_allWindows->getSqlTableModel_entreprise_info();

		QSqlRecord aNewEntrepriseInfoRecord = entreprise_info_TableModel.record(1);

		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_entreprise_denomination_de_lentreprise->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL, lineEdit_entreprise_siege_social->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::LOCALISATION, lineEdit_entreprise_localisation->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_entreprise_boite_postale->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::ADRESSE, lineEdit_entreprise_adresse->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_entreprise_ville->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_entreprise_pays->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_entreprise_email->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_entreprise_telephone->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_entreprise_numero_de_contribuable->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::SECTEURS_DACTIVITES, lineEdit_entreprise_secteurs_dactivites->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::AGENCE_DE_COMPTE_BANCAIRE, lineEdit_entreprise_agence_de_compte_bancaire->text());
		aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::NUMERO_DE_COMPTE_BANCAIRE, lineEdit_entreprise_secteurs_numero_de_compte_bancaire->text());

		bool recordUpdated = entreprise_info_TableModel.updateRecord(1, aNewEntrepriseInfoRecord);

		QString retMesg;

		if (true == recordUpdated)
		{
			retMesg = "Les données commerciales de l'entreprise "
					"ont été enregistrées avec succès !";

			if (QMessageBox::Ok == YerothQMessageBox::information(this, QObject::trUtf8("succès"), retMesg))
			{
			}
			else
			{
			}
		}
		else
		{
			retMesg = "Échec de l'enregistrement des données commerciales de l'entreprise !";

			if (QMessageBox::Ok == YerothQMessageBox::warning(this, QObject::trUtf8("échec"), retMesg))
			{
			}
			else
			{
			}
		}
	}
	else
	{
	}
}


void YerothAdminWindow::enregistrer_alert_system_configuration()
{
    _logger->log("enregistrer_alert_system_configuration");

    QString msgEnregistrer(QObject::
                           trUtf8("Enregistrer la configuration (système d'alertes) modifiée ?"));
    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                  QObject::
                                  trUtf8("enregistrer"),
                                  msgEnregistrer, QMessageBox::Cancel, QMessageBox::Ok))
    {
        YerothSqlTableModel & configurationsTableModel = _allWindows->getSqlTableModel_configurations();
        QSqlRecord configurationsRecord;
        if (lineEdit_alert_period_time_interval->checkField())
        {
            configurationsRecord =
                configurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);
            configurationsRecord.setValue("valeur_configuration", lineEdit_alert_period_time_interval->text());
            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL,
                        configurationsRecord);
            if (success)
            {
                YerothERPConfig::alert_period_time_interval = lineEdit_alert_period_time_interval->text().toUInt();
            }
        }
        if (lineEdit_alert_quantity_time_interval->checkField())
        {
            configurationsRecord =
                configurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);
            configurationsRecord.setValue("valeur_configuration", lineEdit_alert_quantity_time_interval->text());
            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL,
                        configurationsRecord);
            if (success)
            {
                YerothERPConfig::alert_quantity_time_interval =
                    lineEdit_alert_quantity_time_interval->text().toUInt();
            }
        }
    }
    else
    {
    }
}

void YerothAdminWindow::enregistrer_app_parameters_configuration()
{
    _logger->log("enregistrer_app_parameters_configuration");

    QString msgEnregistrer(QObject::trUtf8("Enregistrer la configuration (générale) modifiée ?"));

    if (QMessageBox::Ok == YerothQMessageBox::question(this, QObject::trUtf8("enregistrer"),
            msgEnregistrer, QMessageBox::Cancel, QMessageBox::Ok))
    {
        YerothSqlTableModel & configurationsTableModel = _allWindows->getSqlTableModel_configurations();

        QSqlRecord configurationsRecord;

        if (checkBox_activer_registre_de_caisse->isChecked())
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_OPEN_CASH_DRAWER);
            configurationsRecord.setValue("valeur_configuration", YerothERPConfig::OPEN_CASH_DRAWER);

            bool success = configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_OPEN_CASH_DRAWER, configurationsRecord);

            if (success)
            {
            	YerothERPConfig::ouvrirRegistreDeCaisse = true;
            }
        }
        else
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_OPEN_CASH_DRAWER);
            configurationsRecord.setValue("valeur_configuration", YerothERPConfig::CLOSE_CASH_DRAWER);

            bool success = configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_OPEN_CASH_DRAWER, configurationsRecord);

            if (success)
            {
            	YerothERPConfig::ouvrirRegistreDeCaisse = false;
            }
        }

        if (lineEdit_devise->checkField())
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_CURRENCY);
            configurationsRecord.setValue("valeur_configuration", lineEdit_devise->text());

            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_CURRENCY, configurationsRecord);

            if (success)
            {
                YerothERPConfig::currency = lineEdit_devise->text();
            }
        }

        if (lineEdit_fichier_systeme_imprimante_thermique->checkField())
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_THERMAL_PRINTER_DEVICE_FILE_FULL_PATH);
            configurationsRecord.setValue("valeur_configuration", lineEdit_fichier_systeme_imprimante_thermique->text());

            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_THERMAL_PRINTER_DEVICE_FILE_FULL_PATH, configurationsRecord);

            if (success)
            {
                YerothERPConfig::pathToThermalPrinterDeviceFile = lineEdit_fichier_systeme_imprimante_thermique->text();
            }
        }

        if (lineEdit_repertoire_systeme_latex->checkField())
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_LATEX_SYSTEM_ROOT_FOLDER);
            configurationsRecord.setValue("valeur_configuration", lineEdit_repertoire_systeme_latex->text());

            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_LATEX_SYSTEM_ROOT_FOLDER, configurationsRecord);

            if (success)
            {
                YerothERPConfig::pathToLatexSystemRootFolder = lineEdit_repertoire_systeme_latex->text();
            }
        }

        if (lineEdit_pdfReader->checkField())
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_PDF_READER);
            configurationsRecord.setValue("valeur_configuration", lineEdit_pdfReader->text());

            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_PDF_READER, configurationsRecord);

            if (success)
            {
                YerothERPConfig::pathToPdfReader = lineEdit_pdfReader->text();
            }
        }

        if (comboBox_format_de_facture->checkField())
        {
            YerothUtils::saveCurrentAdminWindowFacturesTypeAccordingToLanguage(comboBox_format_de_facture->
                    currentText());
        }

        if (lineEdit_repertoire_fichiers_temporaires->checkField())
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_TEMPORARY_FILES_DIR);

            QString repertoireFichiersTemporaires(lineEdit_repertoire_fichiers_temporaires->text());

            configurationsRecord.setValue("valeur_configuration", repertoireFichiersTemporaires);

            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_TEMPORARY_FILES_DIR,
                        configurationsRecord);

            if (success)
            {
                YerothERPConfig::temporaryFilesDir = repertoireFichiersTemporaires;
            }
        }

        if (lineEdit_tva_value->checkField())
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_TVA_VALUE);
            configurationsRecord.setValue("valeur_configuration", lineEdit_tva_value->text());
            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_TVA_VALUE, configurationsRecord);
            if (success)
            {
                YerothERPConfig::tva_value = (lineEdit_tva_value->text().toDouble() / 100.0);
            }
        }

        if (lineEdit_annee_depart_rapports_chiffre_affaire->checkField())
        {
            configurationsRecord =
                configurationsTableModel.record(YerothERPConfig::CONFIG_ANNEE_DEPART_RAPPORTS_CHIFFRE_AFFAIRE);
            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_annee_depart_rapports_chiffre_affaire->text());
            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_ANNEE_DEPART_RAPPORTS_CHIFFRE_AFFAIRE,
                        configurationsRecord);
            if (success)
            {
                YerothERPConfig::annee_depart_rapports_chiffre_affaire_value =
                    lineEdit_annee_depart_rapports_chiffre_affaire->text();
            }
        }

        if (comboBox_strategie_vente_sortie->checkField())
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_SALES_STRATEGY);
            configurationsRecord.setValue("valeur_configuration", comboBox_strategie_vente_sortie->currentText());

            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_SALES_STRATEGY, configurationsRecord);

            if (success)
            {
                YerothERPConfig::salesStrategy = comboBox_strategie_vente_sortie->currentText();
            }
        }

        if (comboBox_impression_sur->checkField())
        {
            configurationsRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_PRINTER);
            configurationsRecord.setValue("valeur_configuration", comboBox_impression_sur->currentText());

            bool success =
                configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_PRINTER, configurationsRecord);

            if (success)
            {
                YerothERPConfig::printer = comboBox_impression_sur->currentText();
            }
        }


#ifdef YEROTH_SERVER
        YerothDBusServer *dbusServer = _allWindows->dbusServer();
        if (dbusServer)
        {
            dbusServer->emit_signal_refresh_app_parameters();
            //qDebug() << "YerothAdminWindow application "
            //  "parameters have been changed on the server";
        }
#endif

    }
    else
    {
    }
}

void YerothAdminWindow::set_colour_status_of_alert_daemon_process_buttons()
{
    _logger->log("yeroth-erp-3-0 | YerothAdminWindow::check_alert_daemon_process");

    bool started = YerothProcessInfo::checkYerothERPAlertDaemonRunning(_logger);

    if (started)
    {
        //qDebug() << "++ OK 3: " << checkAlertDeamonProcessOutput ;
        label_ON->setVisible(true);
        label_OFF->setVisible(false);
    }
    else
    {
        //qDebug() << "++ OK pas demarre: ";
        label_ON->setVisible(false);
        label_OFF->setVisible(true);
    }
}

void YerothAdminWindow::start_alert_daemon_process()
{
    _logger->log("start_alert_daemon_process");

    YerothERPProcess::startYerothERPAlertProcess();

    set_colour_status_of_alert_daemon_process_buttons();
}

void YerothAdminWindow::stop_alert_daemon_process()
{
    _logger->log("stop_alert_daemon_process");

    YerothERPProcess::stopYerothERPAlertProcess();

    set_colour_status_of_alert_daemon_process_buttons();
}

void YerothAdminWindow::set_localisation_adresse_ip_text()
{
    YerothSqlTableModel & localisationSqlTableModel = _allWindows->getSqlTableModel_localisations();
    localisationSqlTableModel.yerothSetFilter(GENERATE_SQL_IS_STMT("nom_localisation", lineEdit_localisation->text()));
    int localisationSqlTableModelRowCount = localisationSqlTableModel.easySelect();
    if (localisationSqlTableModelRowCount > 0)
    {
        QSqlRecord localisationRecord = localisationSqlTableModel.record(0);
        _localisation_adresse_ip = GET_SQL_RECORD_DATA(localisationRecord, "adresse_ip");
        lineEdit_localisation_adresse_ip->setText(_localisation_adresse_ip);
    }
    //qDebug() << "++localisation_adresse_ip: " << _localisation_adresse_ip;
    localisationSqlTableModel.resetFilter();
}

void YerothAdminWindow::connecter_localisation_db()
{
    if (YerothUtils::slot_connecter_localisation(*this, _allWindows, lineEdit_localisation->text()))
    {
        label_ONLINE->setVisible(true);
        label_OFFLINE->setVisible(false);
    }
    else
    {
        lineEdit_localisation->clear();
        lineEdit_localisation_adresse_ip->clear();
        label_ONLINE->setVisible(false);
        label_OFFLINE->setVisible(true);
    }
    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();
}

void YerothAdminWindow::deconnecter_localisation_db()
{
    YerothUtils::slot_deconnecter_localisation(_allWindows);
    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();
    lineEdit_localisation->clear();
    lineEdit_localisation_adresse_ip->clear();
    label_ONLINE->setVisible(false);
    label_OFFLINE->setVisible(true);
}
