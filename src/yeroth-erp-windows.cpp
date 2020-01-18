/*
 * yeroth-erp-windows.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/users/yeroth-erp-user-caissier.hpp"

#include "src/users/yeroth-erp-user-magasinier.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/users/yeroth-erp-user-administrateur.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/admin/yeroth-erp-admin-window.hpp"


#ifdef YEROTH_SERVER
	#include "src/dbus/yeroth-erp-dbus-server.hpp"
#endif

#ifdef YEROTH_CLIENT
	#include "src/dbus/yeroth-erp-dbus-client.hpp"
#endif


#include <QtCore/QList>

#include <QtCore/QMap>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlDriver>

#include <QtWidgets/QDesktopWidget>


const QString YerothERPWindows::ENTREPRISE_INFO					("entreprise_info");

const QString YerothERPWindows::PAIEMENTS						("paiements");

const QString YerothERPWindows::YerothERPWindows::USERS							("users");

const QString YerothERPWindows::YerothERPWindows::TITRES							("titres");

const QString YerothERPWindows::YerothERPWindows::LOCALISATIONS					("localisations");

const QString YerothERPWindows::YerothERPWindows::CATEGORIES						("categories");

const QString YerothERPWindows::YerothERPWindows::CLIENTS							("clients");

const QString YerothERPWindows::YerothERPWindows::FOURNISSEURS					("fournisseurs");

const QString YerothERPWindows::YerothERPWindows::ALERTES							("alertes");

const QString YerothERPWindows::YerothERPWindows::REMISES							("remises");

const QString YerothERPWindows::CREDIT_CARD_ISSUING_COMPANY		("credit_card_issuing_company");

const QString YerothERPWindows::YerothERPWindows::CONDITIONS_ALERTES				("conditions_alertes");

const QString YerothERPWindows::YerothERPWindows::COURRIERS_ALERTES				("courriers_alertes");

const QString YerothERPWindows::YerothERPWindows::ACHATS							("achats");

const QString YerothERPWindows::YerothERPWindows::STOCKS							("stocks");

const QString YerothERPWindows::YerothERPWindows::STOCKS_VENDU					("stocks_vendu");

const QString YerothERPWindows::YerothERPWindows::STOCKS_SORTIES					("stocks_sorties");

const QString YerothERPWindows::YerothERPWindows::MARCHANDISES				("marchandises");

const QString YerothERPWindows::YerothERPWindows::CONFIGURATIONS					("configurations");

const QString YerothERPWindows::YerothERPWindows::INIT_CONFIGURATIONS				("init_configurations");

const QString YerothERPWindows::ROLES							("roles");

const QString YerothERPWindows::DBMS							("dbms");

YerothERPWindows::YerothERPWindows(QDesktopWidget *desktopWidget)
    :_mainWindow(0),
	 _historiqueDuStockWindow(0),
     _changerUtilisateurDialog(0),
	 _creerCompteClientWindow(0),
	 _modifierCompteClientWindow(0),
     _creerNouveauClientWindow(0),
     _creerNouveauFournisseurWindow(0),
     _creerNouvelleCategorieWindow(0),
	 _achatsWindow(0),
     _stocksWindow(0),
     _marchandisesWindow(0),
	 _configurationComptabiliteWindow(0),
     _listerAlertesWindow(0),
	 _historiquePaiementsWindow(0),
     _ventesWindow(0),
     _modifierWindow(0),
     _sortirWindow(0),
     _statistiquesWindow(0),
     _transactionsWindow(0),
     _pdVenteWindow(0),
	 _clientsDetailWindow(0),
	 _achatsDetailWindow(0),
	 _detailWindow(0),
     _entrerWindow(0),
     _pdVenteListStocksWindow(0),
	 _pdVenteMethodePaiementCarteCreditDialog(0),
	 _pdVenteMethodePaiementComptantEntreeDialog(0),
	 _pdVenteMethodePaiementDialog(0),
	 _clientWindow(0),
     _sortirListStocksWindow(0),
     _adminWindow(0),
     _adminCreateWindow(0),
     _adminListerWindow(0),
     _adminDetailWindow(0),
     _adminModifierWindow(0),
     _database(0),
	 _tableModel_entreprise_info(0),
	 _tableModel_historique_paiements(0),
     _tableModel_users(0),
     _tableModel_titres(0),
     _tableModel_localisations(0),
     _tableModel_categories(0),
     _tableModel_fournisseurs(0),
     _tableModel_alertes(0),
	 _tableModel_remises(0),
     _tableModel_conditions_alertes(0),
     _tableModel_courriers_alertes(0),
	 _tableModel_achats(0),
     _tableModel_stocks(0),
     _tableModel_stocks_vendu(0),
     _tableModel_stocks_sorties(0),
     _tableModel_marchandises(0),
     _tableModel_configurations(0),
     _tableModel_init_configurations(0),

# ifdef YEROTH_SERVER
     _dbusServer(0),
# endif

# ifdef YEROTH_CLIENT
     _dbusClient(0),
# endif

     _logger(0),
     _infoEntreprise(0),
     _user(0),
     _currentPosition(0),
     _desktopWidget(desktopWidget)
{
    _logger = new YerothLogger("YerothWindows");

    this->setupConfiguration();

    static YerothDatabaseTableColumn aYerothDatabaseTableColumnStaticInstance;

    _tableModel_entreprise_info				= new YerothSqlTableModel(YerothERPWindows::ENTREPRISE_INFO);
    _tableModel_historique_paiements		= new YerothSqlTableModel(YerothERPWindows::PAIEMENTS);
    _tableModel_users 						= new YerothSqlTableModel(YerothERPWindows::USERS);
    _tableModel_titres 						= new YerothSqlTableModel(YerothERPWindows::TITRES);
    _tableModel_localisations 				= new YerothSqlTableModel(YerothERPWindows::LOCALISATIONS);
    _tableModel_categories 					= new YerothSqlTableModel(YerothERPWindows::CATEGORIES);
    _tableModel_clients						= new YerothSqlTableModel(YerothERPWindows::CLIENTS);
    _tableModel_fournisseurs 				= new YerothSqlTableModel(YerothERPWindows::FOURNISSEURS);
    _tableModel_alertes 					= new YerothSqlTableModel(YerothERPWindows::ALERTES);
    _tableModel_remises 					= new YerothSqlTableModel(YerothERPWindows::REMISES);
    _tableModel_conditions_alertes 			= new YerothSqlTableModel(YerothERPWindows::CONDITIONS_ALERTES);
    _tableModel_courriers_alertes 			= new YerothSqlTableModel(YerothERPWindows::COURRIERS_ALERTES);
    _tableModel_achats 						= new YerothSqlTableModel(YerothERPWindows::ACHATS);
    _tableModel_stocks 						= new YerothSqlTableModel(YerothERPWindows::STOCKS);
    _tableModel_stocks_vendu 				= new YerothSqlTableModel(YerothERPWindows::STOCKS_VENDU);
    _tableModel_stocks_sorties				= new YerothSqlTableModel(YerothERPWindows::STOCKS_SORTIES);
    _tableModel_marchandises 				= new YerothSqlTableModel(YerothERPWindows::MARCHANDISES);
    _tableModel_configurations 				= new YerothSqlTableModel(YerothERPWindows::CONFIGURATIONS);
    _tableModel_init_configurations 		= new YerothSqlTableModel(YerothERPWindows::INIT_CONFIGURATIONS);

#ifdef YEROTH_SERVER
    _dbusServer = new YerothDBusServer(this);
#endif

#ifdef YEROTH_CLIENT
    _dbusClient = new YerothDBusClient(this);
#endif
}

YerothERPWindows::~YerothERPWindows()
{
    delete _changerUtilisateurDialog;

    delete _mainWindow;
    delete _sortirWindow;
    delete _statistiquesWindow;
    delete _transactionsWindow;
    delete _pdVenteMethodePaiementCarteCreditDialog;
    delete _pdVenteMethodePaiementComptantEntreeDialog;
    delete _pdVenteMethodePaiementDialog;
    delete _pdVenteWindow;
    delete _detailWindow;
    delete _achatsDetailWindow;
    delete _clientsDetailWindow;
    delete _modifierWindow;
    delete _entrerWindow;
    delete _creerCompteClientWindow;
    delete _modifierCompteClientWindow;
    delete _creerNouveauClientWindow;
    delete _creerNouveauFournisseurWindow;
    delete _creerNouvelleCategorieWindow;
    delete _stocksWindow;
    delete _achatsWindow;
    delete _marchandisesWindow;
    delete _configurationComptabiliteWindow;
    delete _listerAlertesWindow;
    delete _historiquePaiementsWindow;
    delete _ventesWindow;
    delete _pdVenteListStocksWindow;
    delete _historiqueDuStockWindow;
    delete _clientWindow;
    delete _sortirListStocksWindow;

    delete _adminWindow;
    delete _adminCreateWindow;
    delete _adminListerWindow;
    delete _adminDetailWindow;
    delete _adminModifierWindow;

#ifdef YEROTH_SERVER
    delete _dbusServer;
#endif

#ifdef YEROTH_CLIENT
    delete _dbusClient;
#endif

    delete _logger;
    delete _infoEntreprise;
}

void YerothERPWindows::createAllYerothPosUserWindows()
{
	_infoEntreprise = new YerothInfoEntreprise;

    _changerUtilisateurDialog 		= new YerothPOSChangerUtilisateurDialog(this);

    _mainWindow 					= new YerothMainWindow;

    _currentPosition = new QPoint;

    YerothUtils::getCenterPosition(_desktopWidget->width(),
                                  _desktopWidget->height(),
                                  *_mainWindow,
                                  *_currentPosition);

    YerothWindowsCommons::setCenterPosition(_currentPosition);

    YerothPOSAdminWindowsCommons::setCenterPosition(_currentPosition);

    _adminWindow 					= new YerothAdminWindow;
    _adminCreateWindow 				= new YerothAdminCreateWindow;
    _adminListerWindow 				= new YerothAdminListerWindow;
    _adminDetailWindow 				= new YerothAdminDetailWindow;
    _adminModifierWindow			= new YerothAdminModifierWindow;

    _historiqueDuStockWindow		= new YerothHistoriqueDuStockWindow;
    _sortirWindow 					= new YerothSortirWindow;
    _statistiquesWindow				= new YerothTableauxDeBordWindow;
    _transactionsWindow				= new YerothTransactionsWindow;

    _pdVenteMethodePaiementCarteCreditDialog
									= new YerothPOSVenteMethodePaiementCarteCreditDialog(_pdVenteWindow);

    _pdVenteMethodePaiementComptantEntreeDialog
									= new YerothPOSVenteMethodePaiementComptantEntreeDialog(this, _pdVenteWindow);

    _pdVenteMethodePaiementDialog		= new YerothPOSVenteMethodePaiementDialog(this, _pdVenteWindow);

#ifndef YEROTH_ERP_3_0_TEST
    _pdVenteWindow 					= new YerothPointDeVenteWindow;
#else
    _pdVenteWindow 					= new Test_YerothPointDeVenteWindow;
#endif

    _clientsDetailWindow 			= new YerothClientsDetailWindow;
    _achatsDetailWindow 			= new YerothAchatsDetailWindow;
    _detailWindow 					= new YerothStockDetailWindow;
    _modifierWindow 				= new YerothModifierWindow;
    _entrerWindow 					= new YerothEntrerWindow;
    _creerCompteClientWindow		= new YerothCreerCompteClientWindow;
    _modifierCompteClientWindow		= new YerothModifierCompteClientWindow;
    _creerNouveauClientWindow 		= new YerothCreerNouveauClientWindow;
    _creerNouveauFournisseurWindow  = new YerothCreerNouveauFournisseurWindow;
    _creerNouvelleCategorieWindow 	= new YerothCreerNouvelleCategorieWindow;
    _achatsWindow 					= new YerothAchatsWindow;
    _stocksWindow 					= new YerothStocksWindow;
    _marchandisesWindow 			= new YerothMarchandisesWindow;
    _configurationComptabiliteWindow
									= new YerothConfigurationComptabiliteWindow;
    _listerAlertesWindow			= new YerothAlertesWindow;
    _historiquePaiementsWindow		= new YerothPaiementsWindow;
    _ventesWindow					= new YerothVentesWindow;
    _pdVenteListStocksWindow		= new YerothPointDeVenteListStocksWindow;
    _clientWindow			= new YerothERPClientsWindow;
    _sortirListStocksWindow			= new YerothSortirListStocksWindow;

    this->move(*_mainWindow);
}

void YerothERPWindows::reinitialiseSqlTableModels()
{
	delete _tableModel_entreprise_info;
	delete _tableModel_historique_paiements;
    delete _tableModel_users;
    delete _tableModel_titres;
    delete _tableModel_localisations;
    delete _tableModel_categories;
    delete _tableModel_fournisseurs;
    delete _tableModel_alertes;
    delete _tableModel_remises;
    delete _tableModel_conditions_alertes;
    delete _tableModel_courriers_alertes;
    delete _tableModel_stocks;
    delete _tableModel_achats;
    delete _tableModel_marchandises;
    delete _tableModel_stocks_vendu;
    delete _tableModel_stocks_sorties;
    delete _tableModel_configurations;
    delete _tableModel_init_configurations;

    _tableModel_entreprise_info				= new YerothSqlTableModel(YerothERPWindows::ENTREPRISE_INFO);
    _tableModel_historique_paiements		= new YerothSqlTableModel(YerothERPWindows::PAIEMENTS);
    _tableModel_users 						= new YerothSqlTableModel(YerothERPWindows::USERS);
    _tableModel_titres 						= new YerothSqlTableModel(YerothERPWindows::TITRES);
    _tableModel_localisations 				= new YerothSqlTableModel(YerothERPWindows::LOCALISATIONS);
    _tableModel_categories 					= new YerothSqlTableModel(YerothERPWindows::CATEGORIES);
    _tableModel_clients						= new YerothSqlTableModel(YerothERPWindows::CLIENTS);
    _tableModel_fournisseurs 				= new YerothSqlTableModel(YerothERPWindows::FOURNISSEURS);
    _tableModel_alertes 					= new YerothSqlTableModel(YerothERPWindows::ALERTES);
    _tableModel_remises 					= new YerothSqlTableModel(YerothERPWindows::REMISES);
    _tableModel_conditions_alertes 			= new YerothSqlTableModel(YerothERPWindows::CONDITIONS_ALERTES);
    _tableModel_courriers_alertes 			= new YerothSqlTableModel(YerothERPWindows::COURRIERS_ALERTES);
    _tableModel_achats 						= new YerothSqlTableModel(YerothERPWindows::ACHATS);
    _tableModel_stocks 						= new YerothSqlTableModel(YerothERPWindows::STOCKS);
    _tableModel_marchandises				= new YerothSqlTableModel(YerothERPWindows::MARCHANDISES);
    _tableModel_stocks_vendu 				= new YerothSqlTableModel(YerothERPWindows::STOCKS_VENDU);
    _tableModel_stocks_sorties 				= new YerothSqlTableModel(YerothERPWindows::STOCKS_SORTIES);
    _tableModel_configurations 				= new YerothSqlTableModel(YerothERPWindows::CONFIGURATIONS);
    _tableModel_init_configurations 		= new YerothSqlTableModel(YerothERPWindows::INIT_CONFIGURATIONS);
}

void YerothERPWindows::setUser(YerothPOSUser *user)
{
    _user = user;

    switch(_user->role())
    {

    case RoleAdministrateur:
        this->definirAdministrateur();
        break;

    case RoleManager:
        this->definirManager();
        break;

    case RoleVendeur:
        this->definirVendeur();
        break;

    case RoleGestionaireDesStocks:
        this->definirGestionaireDesStocks();
        break;

    case RoleMagasinier:
        this->definirMagasinier();
        break;

    case RoleCaissier:
        this->definirCaissier();
        break;

    default:
        break;
    }
}

void YerothERPWindows::setupConfiguration()
{
    _logger->log("setupConfiguration");
}

void YerothERPWindows::definirAdministrateur()
{
    _adminWindow->definirAdministrateur();
    _adminCreateWindow->definirAdministrateur();
    _adminListerWindow->definirAdministrateur();
    _adminDetailWindow->definirAdministrateur();
    _adminModifierWindow->definirAdministrateur();
}

void YerothERPWindows::definirMagasinier()
{
    _mainWindow->definirMagasinier();
    _sortirWindow->definirMagasinier();
    _statistiquesWindow->definirMagasinier();
    _transactionsWindow->definirMagasinier();
    _pdVenteWindow->definirMagasinier();
    _clientsDetailWindow->definirMagasinier();
    _achatsDetailWindow->definirMagasinier();
    _detailWindow->definirMagasinier();
    _modifierWindow->definirMagasinier();
    _entrerWindow->definirMagasinier();
    _creerCompteClientWindow->definirMagasinier();
    _modifierCompteClientWindow->definirMagasinier();
    _creerNouveauClientWindow->definirMagasinier();
    _creerNouveauFournisseurWindow->definirMagasinier();
    _creerNouvelleCategorieWindow->definirMagasinier();
    _achatsWindow->definirMagasinier();
    _stocksWindow->definirMagasinier();
    _marchandisesWindow->definirMagasinier();
    _configurationComptabiliteWindow->definirMagasinier();
    _listerAlertesWindow->definirMagasinier();
    _historiquePaiementsWindow->definirMagasinier();
    _ventesWindow->definirMagasinier();
    _clientWindow->definirMagasinier();
}

void YerothERPWindows::definirCaissier()
{
    _mainWindow->definirCaissier();
    _sortirWindow->definirCaissier();
    _statistiquesWindow->definirCaissier();
    _transactionsWindow->definirCaissier();
    _pdVenteWindow->definirCaissier();
    _clientsDetailWindow->definirCaissier();
    _achatsDetailWindow->definirCaissier();
    _detailWindow->definirCaissier();
    _modifierWindow->definirCaissier();
    _entrerWindow->definirCaissier();
    _creerCompteClientWindow->definirCaissier();
    _modifierCompteClientWindow->definirCaissier();
    _creerNouveauClientWindow->definirCaissier();
    _creerNouveauFournisseurWindow->definirCaissier();
    _creerNouvelleCategorieWindow->definirCaissier();
    _achatsWindow->definirCaissier();
    _stocksWindow->definirCaissier();
    _marchandisesWindow->definirCaissier();
    _configurationComptabiliteWindow->definirCaissier();
    _listerAlertesWindow->definirCaissier();
    _historiquePaiementsWindow->definirCaissier();
    _ventesWindow->definirCaissier();
    _clientWindow->definirCaissier();
}

void YerothERPWindows::definirManager()
{
    _mainWindow->definirManager();
    _sortirWindow->definirManager();
    _statistiquesWindow->definirManager();
    _transactionsWindow->definirManager();
    _pdVenteWindow->definirManager();
    _clientsDetailWindow->definirManager();
    _achatsDetailWindow->definirManager();
    _detailWindow->definirManager();
    _modifierWindow->definirManager();
    _entrerWindow->definirManager();
    _creerCompteClientWindow->definirManager();
    _modifierCompteClientWindow->definirManager();
    _creerNouveauClientWindow->definirManager();
    _creerNouveauFournisseurWindow->definirManager();
    _creerNouvelleCategorieWindow->definirManager();
    _achatsWindow->definirManager();
    _stocksWindow->definirManager();
    _marchandisesWindow->definirManager();
    _configurationComptabiliteWindow->definirManager();
    _listerAlertesWindow->definirManager();
    _historiquePaiementsWindow->definirManager();
    _ventesWindow->definirManager();
    _clientWindow->definirManager();

    _adminWindow->definirManager();
    _adminCreateWindow->definirManager();
    _adminListerWindow->definirManager();
    _adminDetailWindow->definirManager();
    _adminModifierWindow->definirManager();
}


void YerothERPWindows::definirVendeur()
{
    _mainWindow->definirVendeur();
    _sortirWindow->definirVendeur();
    _statistiquesWindow->definirVendeur();
    _transactionsWindow->definirVendeur();
    _pdVenteWindow->definirVendeur();
    _clientsDetailWindow->definirVendeur();
    _achatsDetailWindow->definirVendeur();
    _detailWindow->definirVendeur();
    _modifierWindow->definirVendeur();
    _entrerWindow->definirVendeur();
    _creerCompteClientWindow->definirVendeur();
    _modifierCompteClientWindow->definirVendeur();
    _creerNouveauClientWindow->definirVendeur();
    _creerNouveauFournisseurWindow->definirVendeur();
    _creerNouvelleCategorieWindow->definirVendeur();
    _achatsWindow->definirVendeur();
    _stocksWindow->definirVendeur();
    _marchandisesWindow->definirVendeur();
    _configurationComptabiliteWindow->definirVendeur();
    _listerAlertesWindow->definirVendeur();
    _historiquePaiementsWindow->definirVendeur();
    _ventesWindow->definirVendeur();
    _clientWindow->definirVendeur();

    _adminWindow->definirVendeur();
    _adminCreateWindow->definirVendeur();
    _adminListerWindow->definirVendeur();
    _adminDetailWindow->definirVendeur();
    _adminModifierWindow->definirVendeur();
}


void YerothERPWindows::definirGestionaireDesStocks()
{
    _mainWindow->definirGestionaireDesStocks();
    _sortirWindow->definirGestionaireDesStocks();
    _statistiquesWindow->definirGestionaireDesStocks();
    _transactionsWindow->definirGestionaireDesStocks();
    _pdVenteWindow->definirGestionaireDesStocks();
    _clientsDetailWindow->definirGestionaireDesStocks();
    _achatsDetailWindow->definirGestionaireDesStocks();
    _detailWindow->definirGestionaireDesStocks();
    _modifierWindow->definirGestionaireDesStocks();
    _entrerWindow->definirGestionaireDesStocks();
    _creerCompteClientWindow->definirGestionaireDesStocks();
    _modifierCompteClientWindow->definirGestionaireDesStocks();
    _creerNouveauClientWindow->definirGestionaireDesStocks();
    _creerNouveauFournisseurWindow->definirGestionaireDesStocks();
    _creerNouvelleCategorieWindow->definirGestionaireDesStocks();
    _achatsWindow->definirGestionaireDesStocks();
    _stocksWindow->definirGestionaireDesStocks();
    _marchandisesWindow->definirGestionaireDesStocks();
    _configurationComptabiliteWindow->definirGestionaireDesStocks();
    _listerAlertesWindow->definirGestionaireDesStocks();
    _historiquePaiementsWindow->definirGestionaireDesStocks();
    _ventesWindow->definirGestionaireDesStocks();
    _clientWindow->definirGestionaireDesStocks();

    _adminWindow->definirGestionaireDesStocks();
    _adminCreateWindow->definirGestionaireDesStocks();
    _adminListerWindow->definirGestionaireDesStocks();
    _adminDetailWindow->definirGestionaireDesStocks();
    _adminModifierWindow->definirGestionaireDesStocks();
}


/**
 * We only need to call the method 'definirPasDe Role'
 * on the _mainWindow instance because all other widget
 * instances are not yet instantiated.
 */
void YerothERPWindows::definirPasDeRole()
{
    _mainWindow->definirPasDeRole();
    _sortirWindow->definirPasDeRole();
    _statistiquesWindow->definirPasDeRole();
    _transactionsWindow->definirPasDeRole();
    _pdVenteWindow->definirPasDeRole();
    _clientsDetailWindow->definirPasDeRole();
    _achatsDetailWindow->definirPasDeRole();
    _detailWindow->definirPasDeRole();
    _modifierWindow->definirPasDeRole();
    _entrerWindow->definirPasDeRole();
    _creerCompteClientWindow->definirPasDeRole();
    _modifierCompteClientWindow->definirPasDeRole();
    _creerNouveauClientWindow->definirPasDeRole();
    _creerNouveauFournisseurWindow->definirPasDeRole();
    _creerNouvelleCategorieWindow->definirPasDeRole();
    _achatsWindow->definirPasDeRole();
    _stocksWindow->definirPasDeRole();
    _marchandisesWindow->definirPasDeRole();
    _configurationComptabiliteWindow->definirPasDeRole();
    _listerAlertesWindow->definirPasDeRole();
    _historiquePaiementsWindow->definirPasDeRole();
    _ventesWindow->definirPasDeRole();
    _clientWindow->definirPasDeRole();

    _adminWindow->definirPasDeRole();
    _adminCreateWindow->definirPasDeRole();
    _adminListerWindow->definirPasDeRole();
    _adminDetailWindow->definirPasDeRole();
    _adminModifierWindow->definirPasDeRole();
}

void YerothERPWindows::hideAllWindows()
{
    _sortirWindow->rendreInvisible();
    _historiqueDuStockWindow->rendreInvisible();
    _statistiquesWindow->rendreInvisible();
    _transactionsWindow->rendreInvisible();
    _pdVenteWindow->rendreInvisible();
    _pdVenteListStocksWindow->rendreInvisible();
    _pdVenteMethodePaiementCarteCreditDialog->rendreInvisible();
    _pdVenteMethodePaiementComptantEntreeDialog->rendreInvisible();
    _pdVenteMethodePaiementDialog->rendreInvisible();
    _sortirListStocksWindow->rendreInvisible();
    _clientsDetailWindow->rendreInvisible();
    _achatsDetailWindow->rendreInvisible();
    _detailWindow->rendreInvisible();
    _creerCompteClientWindow->rendreInvisible();
    _modifierCompteClientWindow->rendreInvisible();
    _creerNouveauClientWindow->rendreInvisible();
    _creerNouveauFournisseurWindow->rendreInvisible();
    _creerNouvelleCategorieWindow->rendreInvisible();
    _achatsWindow->rendreInvisible();
    _stocksWindow->rendreInvisible();
    _marchandisesWindow->rendreInvisible();
    _configurationComptabiliteWindow->rendreInvisible();
    _listerAlertesWindow->rendreInvisible();
    _historiquePaiementsWindow->rendreInvisible();
    _ventesWindow->rendreInvisible();
    _clientWindow->rendreInvisible();
    _modifierWindow->rendreInvisible();
    _entrerWindow->rendreInvisible();
    _adminWindow->rendreInvisible();
    _adminCreateWindow->rendreInvisible();
    _adminListerWindow->rendreInvisible();
    _adminDetailWindow->rendreInvisible();
    _adminModifierWindow->rendreInvisible();

    _mainWindow->rendreInvisible();
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_entreprise_info()
{
    _tableModel_entreprise_info->resetFilter();
    return *_tableModel_entreprise_info;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_paiements()
{
    _tableModel_historique_paiements->resetFilter();
    return *_tableModel_historique_paiements;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_users()
{
    _tableModel_users->resetFilter();
    return *_tableModel_users;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_titres()
{
    _tableModel_titres->resetFilter();
    return *_tableModel_titres;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_localisations()
{
    _tableModel_localisations->resetFilter();
    return *_tableModel_localisations;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_categories()
{
    _tableModel_categories->resetFilter();
    return *_tableModel_categories;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_alertes()
{
    _tableModel_alertes->resetFilter();
    return *_tableModel_alertes;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_remises()
{
    _tableModel_remises->resetFilter();
    return *_tableModel_remises;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_conditions_alertes()
{
    _tableModel_conditions_alertes->resetFilter();
    return *_tableModel_conditions_alertes;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_courriers_alertes()
{
    _tableModel_courriers_alertes->resetFilter();
    return *_tableModel_courriers_alertes;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_clients()
{
    _tableModel_clients->resetFilter();
    return *_tableModel_clients;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_fournisseurs()
{
    _tableModel_fournisseurs->resetFilter();
    return *_tableModel_fournisseurs;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_achats()
{
    _tableModel_achats->resetFilter();
    return *_tableModel_achats;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_stocks()
{
    _tableModel_stocks->resetFilter();
    return *_tableModel_stocks;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_marchandises()
{
    _tableModel_marchandises->resetFilter();
    return *_tableModel_marchandises;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_stocks_vendu()
{
    _tableModel_stocks_vendu->resetFilter();
    return *_tableModel_stocks_vendu;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_stocks_sorties()
{
    _tableModel_stocks_sorties->resetFilter();
    return *_tableModel_stocks_sorties;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_configurations()
{
    _tableModel_configurations->resetFilter();
    return *_tableModel_configurations;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_init_configurations()
{
    _tableModel_init_configurations->resetFilter();
    return *_tableModel_init_configurations;
}

void YerothERPWindows::setCurrentPosition(int xPos, int yPos)
{
    if (_currentPosition)
    {
        _currentPosition->setX(xPos);
        _currentPosition->setY(yPos);
    }
    else
    {
        _logger->log("YerothWindows::setCurrentPosition(int, int)",
                     "_currentPosition is NULL!");
    }
}

bool YerothERPWindows::move(QMainWindow &aWindow)
{
    if  (0 != _currentPosition)
    {
        //qDebug() << "pos: " << *_currentPosition;
        aWindow.move(*_currentPosition);
        return true;
    }

    //qDebug() << "no move: ";
    return false;
}
