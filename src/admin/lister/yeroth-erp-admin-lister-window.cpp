/**
 * yeroth-erp-admin-lister-window.cpp
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */
#include "yeroth-erp-admin-lister-window.hpp"
#include "src/yeroth-erp-windows.hpp"
#include "src/admin/yeroth-erp-admin-search-form.hpp"
#include <QtCore/QDebug>
#include <QtWidgets/QDesktopWidget>
#include <QtGui/QStandardItemModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

YerothAdminListerWindow::YerothAdminListerWindow()
:YerothPOSAdminWindowsCommons(QObject::tr("administration ~ lister")),
 _alertCurrentlyFiltered(false),
 _categoryCurrentlyFiltered(false),
 _customerCurrentlyFiltered(false),
 _userCurrentlyFiltered(false),
 _supplierCurrentlyFiltered(false),
 _siteCurrentlyFiltered(false),
 _discountCurrentlyFiltered(false),
 _pushButton_admin_rechercher_font(0),
 _logger(new YerothLogger("YerothAdminListerWindow")),
 _adminSearchForm(0),
 _curSearchSqlTableModel(0),
 _lastItemSelectedForModification(0)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _pushButton_admin_rechercher_font = new QFont(pushButton_admin_rechercher->font());

    _adminSearchForm = new YerothAdminSearchForm(_allWindows, this);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);

    tableView_lister_utilisateur->setTableName(&YerothERPWindows::USERS);
    tableView_lister_localisation->setTableName(&YerothERPWindows::LOCALISATIONS);
    tableView_lister_categorie->setTableName(&YerothERPWindows::CATEGORIES);
    tableView_lister_client->setTableName(&YerothERPWindows::CLIENTS);
    tableView_lister_fournisseur->setTableName(&YerothERPWindows::FOURNISSEURS);
    tableView_lister_alerte->setTableName(&YerothERPWindows::ALERTES);

    pushButton_menu->enable(this, SLOT(menu()));

    pushButton_creer->enable(this, SLOT(creer()));

    pushButton_modifier->enable(this, SLOT(modifier()));

    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));

    pushButton_supprimer->enable(this, SLOT(supprimer()));

    pushButton_admin_reinitialiser->enable(this, SLOT(reinitialiser()));

    pushButton_admin_rechercher->enable(this, SLOT(rechercher()));

    connect(actionAfficher, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));

    connect(actionCreer, SIGNAL(triggered()), this, SLOT(creer()));

    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));

    connect(actionModifier, SIGNAL(triggered()), this, SLOT(modifier()));

    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));

    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

    connect(tabWidget_lister, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));

    connect(tableView_lister_utilisateur, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_localisation, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_categorie, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_client, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_fournisseur, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_alerte, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_utilisateur, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_utilisateur()));

    connect(tableView_lister_localisation, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_localisation()));

    connect(tableView_lister_categorie, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_categorie()));

    connect(tableView_lister_client, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_client()));

    connect(tableView_lister_fournisseur, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_fournisseur()));

    connect(tableView_lister_alerte, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_alerte()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));

    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));

    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));

    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));

    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this, SLOT(retour_menu_principal()));
}

YerothAdminListerWindow::~YerothAdminListerWindow()
{
	delete _pushButton_admin_rechercher_font;
    delete _logger;
    delete _adminSearchForm;
}

void YerothAdminListerWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
}

void YerothAdminListerWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
}

void YerothAdminListerWindow::definirManager()
{
    _logger->log("definirManager");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, true);
}

void YerothAdminListerWindow::self_reset_view(int currentIndex)
{
    this->rendreInvisible();
    this->rendreVisible(currentIndex);
}

void YerothAdminListerWindow::rendreVisible(unsigned selectedSujetAction)
{
    switch (selectedSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
    	if (false == isUserCurrentlyFiltered())
    	{
    		lister_utilisateur();
    	}
        break;

    case SUJET_ACTION_LOCALISATION:
    	if (false == isSiteCurrentlyFiltered())
    	{
    		lister_localisation();
    	}
        break;

    case SUJET_ACTION_CATEGORIE:
    	if (false == isCategoryCurrentlyFiltered())
    	{
    		lister_categorie();
    	}
        break;

    case SUJET_ACTION_CLIENT:
    	if (false == isCustomerCurrentlyFiltered())
    	{
    		lister_client();
    	}
        break;

    case SUJET_ACTION_FOURNISSEUR:
    	if (false == isSupplierCurrentlyFiltered())
    	{
    		lister_fournisseur();
    	}
        break;

    case SUJET_ACTION_ALERTE:
    	if (false == isAlertCurrentlyFiltered())
    	{
    		lister_alerte();
    	}
        break;

    case SUJET_ACTION_REMISE:
    	if (false == isDiscountCurrentlyFiltered())
    	{
    		lister_remise();
    	}
        break;

    case SUJET_ACTION_BON_DE_COMMANDE:
        break;

    default:
        break;
    }

    tabWidget_lister->setCurrentIndex(selectedSujetAction);
    setVisible(true);
}

void YerothAdminListerWindow::reinitialiser()
{
    _logger->log("reinitialiser");

    _adminSearchForm->reinitialiser();
    _adminSearchForm->rendreInvisible();

    if (_curSearchSqlTableModel)
    {
        _curSearchSqlTableModel->resetFilter();
        _curSearchSqlTableModel->easySelect();
    }

    lister_utilisateur();

    lister_localisation();

    lister_categorie();

    lister_client();

    lister_fournisseur();

    lister_alerte();

    lister_remise();

    setCurSearchSqlTableModel(0);

    set_admin_rechercher_font();
}


void YerothAdminListerWindow::set_admin_rechercher_font()
{
    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
    	MACRO_SET_ADMIN_RECHERCHER_FONT(_userCurrentlyFiltered)
        break;

    case SUJET_ACTION_LOCALISATION:
    	MACRO_SET_ADMIN_RECHERCHER_FONT(_siteCurrentlyFiltered)
        break;

    case SUJET_ACTION_CATEGORIE:
    	MACRO_SET_ADMIN_RECHERCHER_FONT(_categoryCurrentlyFiltered)
        break;

    case SUJET_ACTION_CLIENT:
    	MACRO_SET_ADMIN_RECHERCHER_FONT(_customerCurrentlyFiltered)
        break;

    case SUJET_ACTION_FOURNISSEUR:
    	MACRO_SET_ADMIN_RECHERCHER_FONT(_supplierCurrentlyFiltered)
        break;

    case SUJET_ACTION_ALERTE:
    	MACRO_SET_ADMIN_RECHERCHER_FONT(_alertCurrentlyFiltered)
        break;

    case SUJET_ACTION_BON_DE_COMMANDE:
        break;

    default:
        break;
    }

    pushButton_admin_rechercher->setFont(*_pushButton_admin_rechercher_font);
}


void YerothAdminListerWindow::rechercher()
{
    _adminSearchForm->rendreVisible(tabWidget_lister->currentIndex());
}

void YerothAdminListerWindow::handleCurrentChanged(int index)
{
	set_admin_rechercher_font();
}

void YerothAdminListerWindow::lister_utilisateur(YerothSqlTableModel * aSqlTableModel)
{
	//In case, for e.g. there is filtering applied to aSqlTableModel
    if (0 != aSqlTableModel &&
    	true == YerothUtils::isEqualCaseInsensitive(_allWindows->USERS, aSqlTableModel->sqlTableName()))
    {
        tableView_lister_utilisateur->lister_les_elements_du_tableau(*aSqlTableModel);
        _curSearchSqlTableModel = aSqlTableModel;
    }
    else
    {
    	setUserCurrentlyFiltered(false);
        tableView_lister_utilisateur->lister_les_elements_du_tableau(_allWindows->getSqlTableModel_users());
    }

    tableView_lister_utilisateur->hideColumn(0);
    tableView_lister_utilisateur->hideColumn(1);
    tableView_lister_utilisateur->hideColumn(2);
    tableView_lister_utilisateur->hideColumn(5);
    tableView_lister_utilisateur->hideColumn(7);
    tableView_lister_utilisateur->hideColumn(10);
    tableView_lister_utilisateur->hideColumn(11);
    tableView_lister_utilisateur->hideColumn(12);
    tableView_lister_utilisateur->hideColumn(13);
    tableView_lister_utilisateur->hideColumn(16);
    tableView_lister_utilisateur->hideColumn(17);

    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_utilisateur->selectRow(this->_lastItemSelectedForModification);
}

void YerothAdminListerWindow::lister_localisation(YerothSqlTableModel * aSqlTableModel)
{
    if (aSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->LOCALISATIONS, aSqlTableModel->sqlTableName()))
    {
        tableView_lister_localisation->lister_les_elements_du_tableau(*aSqlTableModel);
        _curSearchSqlTableModel = aSqlTableModel;
    }
    else
    {
    	setSiteCurrentlyFiltered(false);
        tableView_lister_localisation->lister_les_elements_du_tableau(_allWindows->getSqlTableModel_localisations());
    }

    tableView_lister_localisation->hideColumn(0);
    tableView_lister_localisation->hideColumn(3);
    tableView_lister_localisation->hideColumn(5);
    tableView_lister_localisation->hideColumn(11);
    tableView_lister_localisation->hideColumn(12);
    tableView_lister_localisation->hideColumn(13);
    tableView_lister_localisation->hideColumn(14);

    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_localisation->selectRow(this->_lastItemSelectedForModification);
}

void YerothAdminListerWindow::lister_categorie(YerothSqlTableModel * aSqlTableModel)
{
    int toSelectRow = 0;

    if (0 != aSqlTableModel &&
        true == YerothUtils::isEqualCaseInsensitive(_allWindows->CATEGORIES, aSqlTableModel->sqlTableName()))
    {
    	tableView_lister_categorie->lister_les_elements_du_tableau(*aSqlTableModel);

    	_curSearchSqlTableModel = aSqlTableModel;
    	//qDebug() << QString("++ lister_categorie | setting new _curSearchSqlTableModel| sql table filter: %1")
    	//				.arg(_curSearchSqlTableModel->filter());
    }
    else
    {
    	setCategoryCurrentlyFiltered(false);
        tableView_lister_categorie->lister_les_elements_du_tableau(_allWindows->getSqlTableModel_categories());
    }

	//qDebug() << QString("++ lister_categorie | sql table filter: %1")
		//			.arg(_curSearchSqlTableModel->filter());

    tableView_lister_categorie->hideColumn(0);

    _lastItemSelectedForModification = toSelectRow;

    set_admin_rechercher_font();

    tableView_lister_categorie->selectRow(_lastItemSelectedForModification);
}

void YerothAdminListerWindow::lister_client(YerothSqlTableModel * aSqlTableModel)
{
    int toSelectRow = 0;
    if (aSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->CLIENTS, aSqlTableModel->sqlTableName()))
    {
        tableView_lister_client->lister_les_elements_du_tableau(*aSqlTableModel);
        _curSearchSqlTableModel = aSqlTableModel;
    }
    else
    {
    	setCustomerCurrentlyFiltered(false);
        tableView_lister_client->lister_les_elements_du_tableau(_allWindows->getSqlTableModel_clients());
    }
    tableView_lister_client->hideColumn(0);
    tableView_lister_client->hideColumn(4);
    tableView_lister_client->hideColumn(10);
    tableView_lister_client->hideColumn(11);
    tableView_lister_client->hideColumn(12);
    tableView_lister_client->hideColumn(13);
    tableView_lister_client->hideColumn(14);
    tableView_lister_client->hideColumn(15);
    tableView_lister_client->hideColumn(16);

    _lastItemSelectedForModification = toSelectRow;

    set_admin_rechercher_font();

    tableView_lister_client->selectRow(this->_lastItemSelectedForModification);
}

void YerothAdminListerWindow::lister_fournisseur(YerothSqlTableModel * aSqlTableModel)
{
    if (aSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->FOURNISSEURS, aSqlTableModel->sqlTableName()))
    {
        tableView_lister_fournisseur->lister_les_elements_du_tableau(*aSqlTableModel);
        _curSearchSqlTableModel = aSqlTableModel;
    }
    else
    {
    	setSupplierCurrentlyFiltered(false);
        tableView_lister_fournisseur->lister_les_elements_du_tableau(_allWindows->getSqlTableModel_fournisseurs());
    }

    tableView_lister_fournisseur->hideColumn(0);
    tableView_lister_fournisseur->hideColumn(3);
    tableView_lister_fournisseur->hideColumn(4);
    tableView_lister_fournisseur->hideColumn(8);
    tableView_lister_fournisseur->hideColumn(9);
    tableView_lister_fournisseur->hideColumn(10);
    tableView_lister_fournisseur->hideColumn(11);
    tableView_lister_fournisseur->hideColumn(12);
    tableView_lister_fournisseur->hideColumn(13);

    set_admin_rechercher_font();

    tableView_lister_fournisseur->selectRow(this->_lastItemSelectedForModification);
}

void YerothAdminListerWindow::lister_alerte(YerothSqlTableModel * aSqlTableModel)
{
    if (aSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->ALERTES, aSqlTableModel->sqlTableName()))
    {
        tableView_lister_alerte->lister_les_elements_du_tableau(*aSqlTableModel);
        _curSearchSqlTableModel = aSqlTableModel;
    }
    else
    {
    	setAlertCurrentlyFiltered(false);
        tableView_lister_alerte->lister_les_elements_du_tableau(_allWindows->getSqlTableModel_alertes());
    }
    tableView_lister_alerte->hideColumn(0);
    tableView_lister_alerte->hideColumn(1);
    tableView_lister_alerte->hideColumn(8);
    tableView_lister_alerte->hideColumn(10);

    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_alerte->selectRow(this->_lastItemSelectedForModification);
}


void YerothAdminListerWindow::lister_remise(YerothSqlTableModel * aSqlTableModel)
{
    /*if (aSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->REMISES, aSqlTableModel->sqlTableName()))
    {
        tableView_lister_remise->lister_les_elements_du_tableau(*aSqlTableModel);
        _curSearchSqlTableModel = aSqlTableModel;
    }
    else
    {
    	setAlertCurrentlyFiltered(false);
        tableView_lister_remise->lister_les_elements_du_tableau(_allWindows->getSqlTableModel_remises());
    }
    tableView_lister_remise->hideColumn(0);
    tableView_lister_remise->hideColumn(1);
    tableView_lister_remise->hideColumn(8);
    tableView_lister_remise->hideColumn(10);*/

    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_alerte->selectRow(this->_lastItemSelectedForModification);
}


void YerothAdminListerWindow::creer()
{
    _allWindows->_adminCreateWindow->rendreVisible(tabWidget_lister->currentIndex());
    this->rendreInvisible();
}

void YerothAdminListerWindow::handleItemModification(const QModelIndex & index)
{
    //_logger->debug("handleItemModification", QString("model index: %1").arg(index.row()));
    _lastItemSelectedForModification = index.row();
}


void YerothAdminListerWindow::setAlertCurrentlyFiltered(bool alertCurrentlyFiltered)
{
	_alertCurrentlyFiltered = alertCurrentlyFiltered;
}

void YerothAdminListerWindow::setCategoryCurrentlyFiltered(bool categoryCurrentlyFiltered)
{
	_categoryCurrentlyFiltered = categoryCurrentlyFiltered;
}

void YerothAdminListerWindow::setCustomerCurrentlyFiltered(bool customerCurrentlyFiltered)
{
	_customerCurrentlyFiltered = customerCurrentlyFiltered;
}

void YerothAdminListerWindow::setSiteCurrentlyFiltered(bool siteCurrentlyFiltered)
{
	_siteCurrentlyFiltered = siteCurrentlyFiltered;
}

void YerothAdminListerWindow::setSupplierCurrentlyFiltered(bool supplierCurrentlyFiltered)
{
	_supplierCurrentlyFiltered = supplierCurrentlyFiltered;
}

void YerothAdminListerWindow::setUserCurrentlyFiltered(bool userCurrentlyFiltered)
{
	_userCurrentlyFiltered = userCurrentlyFiltered;
}


void YerothAdminListerWindow::modifier()
{
    _logger->log("modifier", QString("current index: %1").arg(tabWidget_lister->currentIndex()));

    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        if (tableView_lister_utilisateur->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
            this->rendreInvisible();
        }
        break;
    case SUJET_ACTION_LOCALISATION:
        if (tableView_lister_localisation->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
            this->rendreInvisible();
        }
        break;
    case SUJET_ACTION_CATEGORIE:
        if (tableView_lister_categorie->rowCount() > 0)
        {
        	//qDebug() << "++ YerothAdminListerWindow::modifier | categorie";

            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
            this->rendreInvisible();
        }
        break;
    case SUJET_ACTION_CLIENT:
        if (tableView_lister_client->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CLIENT);
            this->rendreInvisible();
        }
        break;
    case SUJET_ACTION_FOURNISSEUR:
        if (tableView_lister_fournisseur->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_FOURNISSEUR);
            this->rendreInvisible();
        }
        break;
    case SUJET_ACTION_ALERTE:
        if (tableView_lister_alerte->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_ALERTE);
            this->rendreInvisible();
        }
        break;
    case SUJET_ACTION_REMISE:
        if (tableView_lister_remise->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_REMISE);
            this->rendreInvisible();
        }
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        //_allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_BON_DE_COMMANDE);
        //this->rendreInvisible();
        break;
    default:
        break;
    }
}

void YerothAdminListerWindow::afficher_au_detail()
{
    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        afficher_detail_utilisateur();
        break;
    case SUJET_ACTION_LOCALISATION:
        afficher_detail_localisation();
        break;
    case SUJET_ACTION_CATEGORIE:
        afficher_detail_categorie();
        break;
    case SUJET_ACTION_CLIENT:
        afficher_detail_client();
        break;
    case SUJET_ACTION_FOURNISSEUR:
        afficher_detail_fournisseur();
        break;
    case SUJET_ACTION_ALERTE:
        afficher_detail_alerte();
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        break;
    default:
        break;
    }
}

void YerothAdminListerWindow::afficher_detail_utilisateur()
{
    _allWindows->_adminDetailWindow->rendreVisibleCompteUtilisateur(this->lastSelectedItemForModification());
    this->rendreInvisible();
}

void YerothAdminListerWindow::afficher_detail_localisation()
{
    _allWindows->_adminDetailWindow->rendreVisibleLocalisation(this->lastSelectedItemForModification());
    this->rendreInvisible();
}

void YerothAdminListerWindow::afficher_detail_categorie()
{
    //_logger->debug("afficher_detail_categorie",
    //  QString("lastSelectedItemForModification: %1").arg(this->lastSelectedItemForModification()));
    _allWindows->_adminDetailWindow->rendreVisibleCategorie(this->lastSelectedItemForModification());
    this->rendreInvisible();
}

void YerothAdminListerWindow::afficher_detail_client()
{
    _allWindows->_adminDetailWindow->rendreVisibleClient(this->lastSelectedItemForModification());
    this->rendreInvisible();
}

void YerothAdminListerWindow::afficher_detail_fournisseur()
{
    _allWindows->_adminDetailWindow->rendreVisibleFournisseur(this->lastSelectedItemForModification());
    this->rendreInvisible();
}

void YerothAdminListerWindow::afficher_detail_alerte()
{
    _allWindows->_adminDetailWindow->rendreVisibleAlerte(this->lastSelectedItemForModification());
    this->rendreInvisible();
}

void YerothAdminListerWindow::supprimer()
{
    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        supprimer_utilisateur();
        break;
    case SUJET_ACTION_LOCALISATION:
        supprimer_localisation();
        break;
    case SUJET_ACTION_CATEGORIE:
        supprimer_categorie();
        break;
    case SUJET_ACTION_CLIENT:
        supprimer_client();
        break;
    case SUJET_ACTION_FOURNISSEUR:
        supprimer_fournisseur();
        break;
    case SUJET_ACTION_ALERTE:
        supprimer_alerte();
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        break;
    default:
        break;
    }
}

void YerothAdminListerWindow::supprimer_utilisateur()
{
    _logger->log("supprimer_utilisateur");

    YerothSqlTableModel *usersTableModel = 0;

    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->USERS, _curSearchSqlTableModel->sqlTableName()))
    {
        usersTableModel = _curSearchSqlTableModel;
    }
    else
    {
        usersTableModel = &_allWindows->getSqlTableModel_users();
    }

    QSqlRecord record = usersTableModel->record(this->lastSelectedItemForModification());

    if (record.isEmpty() || record.isNull("nom_utilisateur"))
    {
        return;
    }

    QString prenom(GET_SQL_RECORD_DATA(record, "prenom"));
    QString nom(GET_SQL_RECORD_DATA(record, "nom"));
    QString id(GET_SQL_RECORD_DATA(record, "nom_utilisateur"));

    prenom.append(QString(" %1 (%2)")
    				.arg(nom,
    					 id));

    QString msgConfirmation(QString(QObject::trUtf8("Supprimer l'utilisateur '%1' ?'"))
    							.arg(prenom));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("admin-lister-supprimer-utilisateur"),
                                  msgConfirmation, QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool success = usersTableModel->removeRow(this->lastSelectedItemForModification());
        if (success)
        {
            QString msg(QString(QObject::trUtf8("L'utilisateur '%1' a été supprimée de la base de données !"))
            				.arg(prenom));

            YerothQMessageBox::information(this, QObject::tr("admin-lister-supprimer-utilisateur"),
                                     msg, QMessageBox::Ok);

            this->self_reset_view(SUJET_ACTION_COMPTE_UTILISATEUR);
        }
        else
        {
            QString msg(QString(QObject::trUtf8("L'utilisateur '%1 %2' n'a pas été supprimée de la base de données !"))
            				.arg(prenom, nom));

            YerothQMessageBox::information(this, QObject::tr("admin-lister-supprimer-utilisateur"),
                                     msg, QMessageBox::Ok);
        }
    }
}

void YerothAdminListerWindow::supprimer_localisation()
{
    _logger->log("supprimer_localisation");
    YerothSqlTableModel *localisationsTableModel = 0;
    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->LOCALISATIONS,
                    _curSearchSqlTableModel->sqlTableName()))
    {
        localisationsTableModel = _curSearchSqlTableModel;
    }
    else
    {
        localisationsTableModel = &_allWindows->getSqlTableModel_localisations();
    }

    QSqlRecord record = localisationsTableModel->record(this->lastSelectedItemForModification());

    if (record.isEmpty() || record.isNull("nom_localisation"))
    {
        return;
    }

    QString nom_localisation(GET_SQL_RECORD_DATA(record, "nom_localisation"));

    QString msgConfirmation(QString(QObject::tr("Supprimer la localisation '%1' ?"))
    							.arg(nom_localisation));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("admin-lister-supprimer-localisation"),
                                  msgConfirmation, QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool success = localisationsTableModel->removeRow(this->lastSelectedItemForModification());

        QString msg(QString(QObject::tr("La localisation '%1"))
        				.arg(nom_localisation));

        if (success)
        {
            msg.append(QObject::trUtf8("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this, QObject::tr("admin-lister-supprimer-localisation"),
                                     msg, QMessageBox::Ok);

            this->self_reset_view(SUJET_ACTION_LOCALISATION);
        }
        else
        {
            msg.append(QObject::trUtf8(" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::information(this, QObject::tr("admin-lister-supprimer-localisation"),
                                     msg, QMessageBox::Ok);
        }
    }
}

void YerothAdminListerWindow::supprimer_categorie()
{
    _logger->log("supprimer_categorie");

    YerothSqlTableModel *categoriesTableModel = 0;

    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->CATEGORIES, _curSearchSqlTableModel->sqlTableName()))
    {
        categoriesTableModel = _curSearchSqlTableModel;
    }
    else
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }

    QSqlRecord record = categoriesTableModel->record(this->lastSelectedItemForModification());

    if (record.isEmpty() || record.isNull(YerothDatabaseTableColumn::NOM_CATEGORIE))
    {
        return;
    }

    QString nom_categorie(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_CATEGORIE));

    QString msgConfirmation(QString(QObject::trUtf8("Supprimer la catégorie '%1' ?"))
    							.arg(nom_categorie));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::trUtf8("admin-lister-supprimer-catégorie"),
                                  msgConfirmation, QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool success = categoriesTableModel->removeRow(this->lastSelectedItemForModification());

        QString msg(QString(QObject::trUtf8("La catégorie '%1"))
        				.arg(nom_categorie));

        if (success)
        {
            msg.append(QObject::trUtf8("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this, QObject::trUtf8("admin-lister-supprimer-catégorie"),
                                     msg, QMessageBox::Ok);

            this->self_reset_view(SUJET_ACTION_CATEGORIE);
        }
        else
        {
            msg.append(QObject::trUtf8(" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::information(this, tr("admin-lister-supprimer-catégorie"), msg,
                                     QMessageBox::Ok);
        }
    }
}

void YerothAdminListerWindow::supprimer_client()
{
    _logger->log("supprimer_client");
    YerothSqlTableModel *clientsTableModel = 0;

    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->CLIENTS, _curSearchSqlTableModel->sqlTableName()))
    {
        clientsTableModel = _curSearchSqlTableModel;
    }
    else
    {
        clientsTableModel = &_allWindows->getSqlTableModel_clients();
    }

    QSqlRecord record = clientsTableModel->record(this->lastSelectedItemForModification());

    if (record.isEmpty() || record.isNull(YerothDatabaseTableColumn::NOM_ENTREPRISE))
    {
        return;
    }

    QString nom_entreprise(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

    QString msgConfirmation(QString(QObject::trUtf8("Supprimer le client '%1' ?"))
    							.arg(nom_entreprise));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
            							QObject::tr("admin-lister-supprimer-client"),
            							msgConfirmation,
										QMessageBox::Cancel,
										QMessageBox::Ok))
    {
        bool success = clientsTableModel->removeRow(this->lastSelectedItemForModification());

        QString msg(QString(QObject::trUtf8("Le client '%1"))
        				.arg(nom_entreprise));

        if (success)
        {
            msg.append(QObject::trUtf8("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("admin-lister-supprimer-client"),
            							   msg,
										   QMessageBox::Ok);

            this->self_reset_view(SUJET_ACTION_CLIENT);
        }
        else
        {
            msg.append(QObject::trUtf8(" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("admin-lister-supprimer-client"),
										   msg,
										   QMessageBox::Ok);
        }
    }
}

void YerothAdminListerWindow::supprimer_fournisseur()
{
    _logger->log("supprimer_fournisseur");

    YerothSqlTableModel *fournisseursTableModel = 0;

    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->FOURNISSEURS, _curSearchSqlTableModel->sqlTableName()))
    {
        fournisseursTableModel = _curSearchSqlTableModel;
        //qDebug() << "++ _curSearchSqlTableModel not null ";
    }
    else
    {
        fournisseursTableModel = &_allWindows->getSqlTableModel_fournisseurs();
        //qDebug() << "++ _curSearchSqlTableModel is null ";
    }

    QSqlRecord record = fournisseursTableModel->record(this->lastSelectedItemForModification());

    if (record.isEmpty() || record.isNull(YerothDatabaseTableColumn::NOM_ENTREPRISE))
    {
        //_logger->log("record is empty or null");
        return;
    }

    QString nom_entreprise(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

    QString msgConfirmation(QString(QObject::tr("Supprimer le fournisseur '%1' ?"))
    							.arg(nom_entreprise));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
            							QObject::tr("admin-lister-supprimer-fournisseur"),
                                  	  	msgConfirmation,
										QMessageBox::Cancel,
										QMessageBox::Ok))
    {
        bool success = fournisseursTableModel->removeRow(this->lastSelectedItemForModification());

        QString msg(QString(QObject::tr("Le fournisseur '%1"))
        				.arg(nom_entreprise));

        if (success)
        {
            msg.append(QObject::trUtf8("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("admin-lister-supprimer-fournisseur"),
										   msg,
										   QMessageBox::Ok);

            this->self_reset_view(SUJET_ACTION_FOURNISSEUR);
        }
        else
        {
            msg.append(QObject::trUtf8(" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("admin-lister-supprimer-fournisseur"),
										   msg,
										   QMessageBox::Ok);
        }
    }
}

void YerothAdminListerWindow::supprimer_alerte()
{
    _logger->log("supprimer_alerte");
    YerothSqlTableModel *alertesTableModel = 0;

    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->ALERTES, _curSearchSqlTableModel->sqlTableName()))
    {
        alertesTableModel = _curSearchSqlTableModel;
    }
    else
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }

    QSqlRecord record = alertesTableModel->record(this->lastSelectedItemForModification());

    if (record.isEmpty() || record.isNull(YerothDatabaseTableColumn::DESIGNATION_ALERTE))
    {
        return;
    }

    QString designation_alerte(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_ALERTE));

    QString msgConfirmation(QString(QObject::tr("Supprimer l'alerte '%1' ?"))
    							.arg(designation_alerte));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
            							QObject::tr("admin-lister-supprimer-alerte"),
										msgConfirmation,
										QMessageBox::Cancel,
										QMessageBox::Ok))
    {
        bool success = alertesTableModel->removeRow(this->lastSelectedItemForModification());
        QString msg("L'alerte '");
        msg.append(designation_alerte);
        if (success)
        {
            msg.append(QObject::trUtf8("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("admin-lister-supprimer-alerte"),
										   msg,
										   QMessageBox::Ok);

            this->self_reset_view(SUJET_ACTION_ALERTE);
        }
        else
        {
            msg.append(QObject::trUtf8(" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("admin-lister-supprimer-alerte"),
										   msg,
										   QMessageBox::Ok);
        }
    }
}
