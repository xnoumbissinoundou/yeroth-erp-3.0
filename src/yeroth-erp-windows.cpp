/*
 * yeroth-erp-windows.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
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

#include <QtSql/QSqlQuery>

#include <QtWidgets/QDesktopWidget>


bool YerothERPWindows::CURRENTLY_CHANGING_USER(false);


QMap<QString, YerothSqlTableModel *> YerothERPWindows::_sqltablenameTOsqltablemodel;


const QString YerothERPWindows::COMPTES_BANCAIRES			("comptes_bancaires");

const QString YerothERPWindows::ENTREPRISE_INFO				("entreprise_info");

const QString YerothERPWindows::PAIEMENTS					("paiements");

const QString YerothERPWindows::USERS						("users");

const QString YerothERPWindows::TITRES						("titres");

const QString YerothERPWindows::LOCALISATIONS				("localisations");

const QString YerothERPWindows::CATEGORIES					("categories");

const QString YerothERPWindows::PROGRAMMES_DE_FIDELITE_CLIENTS	("programmes_de_fidelite_clients");

const QString YerothERPWindows::CLIENTS						("clients");

const QString YerothERPWindows::GROUPES_DE_CLIENTS			("groupes_de_clients");

const QString YerothERPWindows::FOURNISSEURS				("fournisseurs");

const QString YerothERPWindows::ALERTES						("alertes");

const QString YerothERPWindows::REMISES						("remises");

const QString YerothERPWindows::CREDIT_CARD_ISSUING_COMPANY	("credit_card_issuing_company");

const QString YerothERPWindows::CONDITIONS_ALERTES			("conditions_alertes");

const QString YerothERPWindows::COURRIERS_ALERTES			("courriers_alertes");

const QString YerothERPWindows::ACHATS_AUX_FOURNISSEURS		("achats_aux_fournisseurs");

const QString YerothERPWindows::ACHATS						("achats");

const QString YerothERPWindows::STOCKS						("stocks");

const QString YerothERPWindows::SERVICES_COMPLETES			("services_completes");

const QString YerothERPWindows::STOCKS_VENDU				("stocks_vendu");

const QString YerothERPWindows::STOCKS_SORTIES				("stocks_sorties");

const QString YerothERPWindows::MARCHANDISES				("marchandises");

const QString YerothERPWindows::CONFIGURATIONS				("configurations");

const QString YerothERPWindows::INIT_CONFIGURATIONS			("init_configurations");

const QString YerothERPWindows::TYPE_DE_VENTE				("type_de_vente");

const QString YerothERPWindows::TYPE_DE_PAIEMENT			("type_de_paiement");

const QString YerothERPWindows::ROLES						("roles");

const QString YerothERPWindows::DBMS						("dbms");

YerothERPWindows::YerothERPWindows(QDesktopWidget *desktopWidget)
    :_mainWindow(0),
	 _transactionsDunFournisseurWindow(0),
	 _transactionsDunClientWindow(0),
	 _historiqueDuStockWindow(0),
     _changerUtilisateurDialog(0),
	 _creerFournisseurWindow(0),
	 _detailsDunProgrammeDeFideliteClientsWindow(0),
	 _creerUnProgrammeDeFideliteClientsWindow(0),
	 _creerGroupeDeClientsWindow(0),
	 _creerCompteClientWindow(0),
	 _modifierFournisseurWindow(0),
	 _modifierCompteClientWindow(0),
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
     _mouvementsDeStocksWindow(0),
     _pdVenteWindow(0),
	 _fournisseurDetailsWindow(0),
	 _programmesDeFideliteClientsWindow(0),
	 _detailsGroupeDeClientsWindow(0),
	 _groupesDeClientsWindow(0),
	 _groupesDunClientWindow(0),
	 _clientsDetailWindow(0),
	 _achats_aux_fournisseursWindow(0),
	 _achatsDetailWindow(0),
	 _detailWindow(0),
     _entrerWindow(0),
     _pdVenteListStocksWindow(0),
	 _pdVenteMethodePaiementCarteCreditDialog(0),
	 _pdVenteMethodePaiementComptantEntreeDialog(0),
	 _pdVenteMethodePaiementDialog(0),
	 _fournisseursWindow(0),
	 _clientsWindow(0),
	 _payerAuFournisseurWindow(0),
	 _payerAuCompteClientWindow(0),
     _sortirListStocksWindow(0),
     _adminWindow(0),
     _adminCreateWindow(0),
     _adminListerWindow(0),
     _adminDetailWindow(0),
     _adminModifierWindow(0),
     _database(0),
	 _tableModel_comptes_bancaires(0),
	 _tableModel_entreprise_info(0),
	 _tableModel_historique_paiements(0),
     _tableModel_users(0),
     _tableModel_titres(0),
     _tableModel_localisations(0),
     _tableModel_categories(0),
	 _tableModel_programmes_de_fidelite_clients(0),
	 _tableModel_clients(0),
	 _tableModel_groupes_de_clients(0),
     _tableModel_fournisseurs(0),
     _tableModel_alertes(0),
	 _tableModel_remises(0),
     _tableModel_conditions_alertes(0),
     _tableModel_courriers_alertes(0),
	 _tableModel_achats_aux_fournisseurs(0),
	 _tableModel_achats(0),
     _tableModel_stocks(0),
	 _tableModel_services_completes(0),
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

    YerothERPWindows::setupConfiguration();

    static YerothDatabaseTableColumn aYerothDatabaseTableColumnStaticInstance;

    setupSqlTableModelFromName(YerothERPWindows::COMPTES_BANCAIRES, &_tableModel_comptes_bancaires);
    setupSqlTableModelFromName(YerothERPWindows::ENTREPRISE_INFO, &_tableModel_entreprise_info);
    setupSqlTableModelFromName(YerothERPWindows::PAIEMENTS, &_tableModel_historique_paiements);
    setupSqlTableModelFromName(YerothERPWindows::USERS, &_tableModel_users);
    setupSqlTableModelFromName(YerothERPWindows::TITRES, &_tableModel_titres);
    setupSqlTableModelFromName(YerothERPWindows::LOCALISATIONS, &_tableModel_localisations);
    setupSqlTableModelFromName(YerothERPWindows::CATEGORIES, &_tableModel_categories);
    setupSqlTableModelFromName(YerothERPWindows::PROGRAMMES_DE_FIDELITE_CLIENTS, &_tableModel_programmes_de_fidelite_clients);
    setupSqlTableModelFromName(YerothERPWindows::CLIENTS, &_tableModel_clients);
    setupSqlTableModelFromName(YerothERPWindows::GROUPES_DE_CLIENTS, &_tableModel_groupes_de_clients);
    setupSqlTableModelFromName(YerothERPWindows::FOURNISSEURS, &_tableModel_fournisseurs);
    setupSqlTableModelFromName(YerothERPWindows::ALERTES, &_tableModel_alertes);
    setupSqlTableModelFromName(YerothERPWindows::REMISES, &_tableModel_remises);
    setupSqlTableModelFromName(YerothERPWindows::CONDITIONS_ALERTES, &_tableModel_conditions_alertes);
    setupSqlTableModelFromName(YerothERPWindows::COURRIERS_ALERTES, &_tableModel_courriers_alertes);
    setupSqlTableModelFromName(YerothERPWindows::ACHATS_AUX_FOURNISSEURS, &_tableModel_achats_aux_fournisseurs);
    setupSqlTableModelFromName(YerothERPWindows::ACHATS, &_tableModel_achats);
    setupSqlTableModelFromName(YerothERPWindows::STOCKS, &_tableModel_stocks);
    setupSqlTableModelFromName(YerothERPWindows::SERVICES_COMPLETES, &_tableModel_services_completes);
    setupSqlTableModelFromName(YerothERPWindows::STOCKS_VENDU, &_tableModel_stocks_vendu);
    setupSqlTableModelFromName(YerothERPWindows::STOCKS_SORTIES, &_tableModel_stocks_sorties);
    setupSqlTableModelFromName(YerothERPWindows::MARCHANDISES, &_tableModel_marchandises);
    setupSqlTableModelFromName(YerothERPWindows::CONFIGURATIONS, &_tableModel_configurations);
    setupSqlTableModelFromName(YerothERPWindows::INIT_CONFIGURATIONS, &_tableModel_init_configurations);

    /*
     * This is to initialize constants useful for 'INTERNATIONALIZATION'.
     */
    YerothUtils yerothUtilsDummy;

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
    delete _mouvementsDeStocksWindow;
    delete _pdVenteMethodePaiementCarteCreditDialog;
    delete _pdVenteMethodePaiementComptantEntreeDialog;
    delete _pdVenteMethodePaiementDialog;
    delete _pdVenteWindow;
    delete _detailWindow;
    delete _achatsDetailWindow;
    delete _achats_aux_fournisseursWindow;
    delete _clientsDetailWindow;
    delete _groupesDunClientWindow;
    delete _groupesDeClientsWindow;
    delete _detailsGroupeDeClientsWindow;
    delete _programmesDeFideliteClientsWindow;
    delete _fournisseurDetailsWindow;
    delete _modifierWindow;
    delete _entrerWindow;
    delete _creerCompteClientWindow;
    delete _creerGroupeDeClientsWindow;
    delete _creerUnProgrammeDeFideliteClientsWindow;
    delete _detailsDunProgrammeDeFideliteClientsWindow;
    delete _creerFournisseurWindow;
    delete _modifierCompteClientWindow;
    delete _modifierFournisseurWindow;
    delete _stocksWindow;
    delete _achatsWindow;
    delete _marchandisesWindow;
    delete _configurationComptabiliteWindow;
    delete _listerAlertesWindow;
    delete _historiquePaiementsWindow;
    delete _ventesWindow;
    delete _pdVenteListStocksWindow;
    delete _historiqueDuStockWindow;
    delete _transactionsDunClientWindow;
    delete _transactionsDunFournisseurWindow;
    delete _clientsWindow;
    delete _fournisseursWindow;
    delete _payerAuCompteClientWindow;
    delete _payerAuFournisseurWindow;
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

    _transactionsDunFournisseurWindow	= new YerothTableauDesTransactionsDuFournisseurWindow;
    _transactionsDunClientWindow	= new YerothTableauDesTransactionsDuClientWindow;
    _historiqueDuStockWindow		= new YerothHistoriqueDuStockWindow;
    _sortirWindow 					= new YerothSortirWindow;
    _statistiquesWindow				= new YerothTableauxDeBordWindow;
    _mouvementsDeStocksWindow				= new YerothMouvementsDeStocksWindow;

    _pdVenteMethodePaiementCarteCreditDialog
									= new YerothPOSVenteMethodePaiementCarteCreditDialog(_pdVenteWindow);

    _pdVenteMethodePaiementComptantEntreeDialog
									= new YerothPOSVenteMethodePaiementComptantEntreeDialog(this, _pdVenteWindow);

    _pdVenteMethodePaiementDialog		= new YerothPOSVenteMethodePaiementDialog(this, _pdVenteWindow);

    _pdVenteWindow 					= new YerothPointDeVenteWindow;
    _fournisseurDetailsWindow 		= new YerothFournisseurDetailsWindow;
    _programmesDeFideliteClientsWindow = new YerothERPProgrammesDeFideliteClientsWindow;
    _detailsGroupeDeClientsWindow	= new YerothDetailsDunGroupeDeClientsWindow;
    _groupesDeClientsWindow			= new YerothGroupesDeClientsWindow;
    _groupesDunClientWindow			= new YerothGroupesDunClientWindow;
    _clientsDetailWindow 			= new YerothClientsDetailWindow;
    _achats_aux_fournisseursWindow 	= new YerothAchatsAUXFournisseursWindow;
    _achatsDetailWindow 			= new YerothAchatsDetailWindow;
    _detailWindow 					= new YerothStockDetailWindow;
    _modifierWindow 				= new YerothModifierWindow;
    _entrerWindow 					= new YerothEntrerWindow;
    _creerFournisseurWindow			= new YerothCreerFournisseurWindow;
    _detailsDunProgrammeDeFideliteClientsWindow		= new YerothDetailsDunProgrammeDeFideliteClientsWindow;
    _creerUnProgrammeDeFideliteClientsWindow		= new YerothCreerUnProgrammeDeFideliteClientsWindow;
    _creerGroupeDeClientsWindow		= new YerothCreerGroupeDeClientsWindow;
    _creerCompteClientWindow		= new YerothCreerCompteClientWindow;
    _modifierFournisseurWindow		= new YerothModifierFournisseurWindow;
    _modifierCompteClientWindow		= new YerothModifierCompteClientWindow;
    _achatsWindow 					= new YerothAchatsWindow;
    _stocksWindow 					= new YerothStocksWindow;
    _marchandisesWindow 			= new YerothMarchandisesWindow;
    _configurationComptabiliteWindow = new YerothConfigurationComptabiliteWindow;
    _listerAlertesWindow			= new YerothAlertesWindow;
    _historiquePaiementsWindow		= new YerothPaiementsWindow;
    _ventesWindow					= new YerothVentesWindow;
    _pdVenteListStocksWindow		= new YerothPointDeVenteListStocksWindow;
    _fournisseursWindow				= new YerothERPFournisseursWindow;
    _clientsWindow					= new YerothERPClientsWindow;
    _payerAuFournisseurWindow		= new YerothPayerFournisseurWindow;
    _payerAuCompteClientWindow		= new YerothPayerCompteClientWindow;
    _sortirListStocksWindow			= new YerothSortirListStocksWindow;

    YerothERPWindows::move(*_mainWindow);
}

void YerothERPWindows::reinitialiseSqlTableModels()
{
	delete _tableModel_comptes_bancaires;
	delete _tableModel_entreprise_info;
	delete _tableModel_historique_paiements;
    delete _tableModel_users;
    delete _tableModel_titres;
    delete _tableModel_localisations;
    delete _tableModel_categories;
    delete _tableModel_programmes_de_fidelite_clients;
    delete _tableModel_clients;
    delete _tableModel_groupes_de_clients;
    delete _tableModel_fournisseurs;
    delete _tableModel_alertes;
    delete _tableModel_remises;
    delete _tableModel_conditions_alertes;
    delete _tableModel_courriers_alertes;
    delete _tableModel_stocks;
    delete _tableModel_achats;
    delete _tableModel_marchandises;
    delete _tableModel_services_completes;
    delete _tableModel_stocks_vendu;
    delete _tableModel_stocks_sorties;
    delete _tableModel_configurations;
    delete _tableModel_init_configurations;

    _tableModel_comptes_bancaires			= new YerothSqlTableModel(YerothERPWindows::COMPTES_BANCAIRES);
    _tableModel_entreprise_info				= new YerothSqlTableModel(YerothERPWindows::ENTREPRISE_INFO);
    _tableModel_historique_paiements		= new YerothSqlTableModel(YerothERPWindows::PAIEMENTS);
    _tableModel_users 						= new YerothSqlTableModel(YerothERPWindows::USERS);
    _tableModel_titres 						= new YerothSqlTableModel(YerothERPWindows::TITRES);
    _tableModel_localisations 				= new YerothSqlTableModel(YerothERPWindows::LOCALISATIONS);
    _tableModel_categories 					= new YerothSqlTableModel(YerothERPWindows::CATEGORIES);
    _tableModel_programmes_de_fidelite_clients = new YerothSqlTableModel(YerothERPWindows::PROGRAMMES_DE_FIDELITE_CLIENTS);
    _tableModel_clients						= new YerothSqlTableModel(YerothERPWindows::CLIENTS);
    _tableModel_groupes_de_clients 			= new YerothSqlTableModel(YerothERPWindows::GROUPES_DE_CLIENTS);
    _tableModel_fournisseurs 				= new YerothSqlTableModel(YerothERPWindows::FOURNISSEURS);
    _tableModel_alertes 					= new YerothSqlTableModel(YerothERPWindows::ALERTES);
    _tableModel_remises 					= new YerothSqlTableModel(YerothERPWindows::REMISES);
    _tableModel_conditions_alertes 			= new YerothSqlTableModel(YerothERPWindows::CONDITIONS_ALERTES);
    _tableModel_courriers_alertes 			= new YerothSqlTableModel(YerothERPWindows::COURRIERS_ALERTES);
    _tableModel_achats 						= new YerothSqlTableModel(YerothERPWindows::ACHATS);
    _tableModel_stocks 						= new YerothSqlTableModel(YerothERPWindows::STOCKS);
    _tableModel_services_completes			= new YerothSqlTableModel(YerothERPWindows::SERVICES_COMPLETES);
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

    case YerothUtils::ROLE_ADMINISTRATEUR:
         definirAdministrateur();
        break;

    case YerothUtils::ROLE_MANAGER:
         definirManager();
        break;

    case YerothUtils::ROLE_VENDEUR:
         definirVendeur();
        break;

    case YerothUtils::ROLE_GESTIONNAIREDESTOCK:
         definirGestionaireDesStocks();
        break;

    case YerothUtils::ROLE_MAGASINIER:
         definirMagasinier();
        break;

    case YerothUtils::ROLE_CAISSIER:
         definirCaissier();
        break;

    default:
        break;
    }
}


void YerothERPWindows::setupSqlTableModelFromName(const QString &aSqlTableName,
 	 											  YerothSqlTableModel **aYerothSqlTableModel)
{
	*aYerothSqlTableModel = new YerothSqlTableModel(aSqlTableName);

	_sqltablenameTOsqltablemodel.insert(aSqlTableName, *aYerothSqlTableModel);
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
    _mouvementsDeStocksWindow->definirMagasinier();
    _pdVenteWindow->definirMagasinier();
    _fournisseurDetailsWindow->definirMagasinier();
    _programmesDeFideliteClientsWindow->definirMagasinier();
    _detailsGroupeDeClientsWindow->definirMagasinier();
    _groupesDeClientsWindow->definirMagasinier();
    _groupesDunClientWindow->definirMagasinier();
    _clientsDetailWindow->definirMagasinier();
    _achats_aux_fournisseursWindow->definirMagasinier();
    _achatsDetailWindow->definirMagasinier();
    _detailWindow->definirMagasinier();
    _modifierWindow->definirMagasinier();
    _entrerWindow->definirMagasinier();
    _creerFournisseurWindow->definirMagasinier();
    _detailsDunProgrammeDeFideliteClientsWindow->definirMagasinier();
    _creerUnProgrammeDeFideliteClientsWindow->definirMagasinier();
    _creerGroupeDeClientsWindow->definirMagasinier();
    _creerCompteClientWindow->definirMagasinier();
    _modifierFournisseurWindow->definirMagasinier();
    _modifierCompteClientWindow->definirMagasinier();
    _achatsWindow->definirMagasinier();
    _stocksWindow->definirMagasinier();
    _marchandisesWindow->definirMagasinier();
    _configurationComptabiliteWindow->definirMagasinier();
    _listerAlertesWindow->definirMagasinier();
    _historiquePaiementsWindow->definirMagasinier();
    _ventesWindow->definirMagasinier();
    _fournisseursWindow->definirMagasinier();
    _clientsWindow->definirMagasinier();
    _payerAuFournisseurWindow->definirMagasinier();
    _payerAuCompteClientWindow->definirMagasinier();
}

void YerothERPWindows::definirCaissier()
{
    _mainWindow->definirCaissier();
    _sortirWindow->definirCaissier();
    _statistiquesWindow->definirCaissier();
    _mouvementsDeStocksWindow->definirCaissier();
    _pdVenteWindow->definirCaissier();
    _fournisseurDetailsWindow->definirCaissier();
    _programmesDeFideliteClientsWindow->definirCaissier();
    _detailsGroupeDeClientsWindow->definirCaissier();
    _groupesDeClientsWindow->definirCaissier();
    _groupesDunClientWindow->definirCaissier();
    _clientsDetailWindow->definirCaissier();
    _achats_aux_fournisseursWindow->definirCaissier();
    _achatsDetailWindow->definirCaissier();
    _detailWindow->definirCaissier();
    _modifierWindow->definirCaissier();
    _entrerWindow->definirCaissier();
    _creerFournisseurWindow->definirCaissier();
    _detailsDunProgrammeDeFideliteClientsWindow->definirCaissier();
    _creerUnProgrammeDeFideliteClientsWindow->definirCaissier();
    _creerGroupeDeClientsWindow->definirCaissier();
    _creerCompteClientWindow->definirCaissier();
    _modifierFournisseurWindow->definirCaissier();
    _modifierCompteClientWindow->definirCaissier();
    _achatsWindow->definirCaissier();
    _stocksWindow->definirCaissier();
    _marchandisesWindow->definirCaissier();
    _configurationComptabiliteWindow->definirCaissier();
    _listerAlertesWindow->definirCaissier();
    _historiquePaiementsWindow->definirCaissier();
    _ventesWindow->definirCaissier();
    _fournisseursWindow->definirCaissier();
    _clientsWindow->definirCaissier();
    _payerAuFournisseurWindow->definirCaissier();
    _payerAuCompteClientWindow->definirCaissier();
}

void YerothERPWindows::definirManager()
{
    _mainWindow->definirManager();
    _sortirWindow->definirManager();
    _statistiquesWindow->definirManager();
    _mouvementsDeStocksWindow->definirManager();
    _pdVenteWindow->definirManager();
    _fournisseurDetailsWindow->definirManager();
    _programmesDeFideliteClientsWindow->definirManager();
    _detailsGroupeDeClientsWindow->definirManager();
    _groupesDeClientsWindow->definirManager();
    _groupesDunClientWindow->definirManager();
    _clientsDetailWindow->definirManager();
    _achats_aux_fournisseursWindow->definirManager();
    _achatsDetailWindow->definirManager();
    _detailWindow->definirManager();
    _modifierWindow->definirManager();
    _entrerWindow->definirManager();
    _creerFournisseurWindow->definirManager();
    _detailsDunProgrammeDeFideliteClientsWindow->definirManager();
    _creerUnProgrammeDeFideliteClientsWindow->definirManager();
    _creerGroupeDeClientsWindow->definirManager();
    _creerCompteClientWindow->definirManager();
    _modifierFournisseurWindow->definirManager();
    _modifierCompteClientWindow->definirManager();
    _achatsWindow->definirManager();
    _stocksWindow->definirManager();
    _marchandisesWindow->definirManager();
    _configurationComptabiliteWindow->definirManager();
    _listerAlertesWindow->definirManager();
    _historiquePaiementsWindow->definirManager();
    _ventesWindow->definirManager();
    _fournisseursWindow->definirManager();
    _clientsWindow->definirManager();
    _payerAuFournisseurWindow->definirManager();
    _payerAuCompteClientWindow->definirManager();

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
    _mouvementsDeStocksWindow->definirVendeur();
    _pdVenteWindow->definirVendeur();
    _fournisseurDetailsWindow->definirVendeur();
    _programmesDeFideliteClientsWindow->definirVendeur();
    _detailsGroupeDeClientsWindow->definirVendeur();
    _groupesDeClientsWindow->definirVendeur();
    _groupesDunClientWindow->definirVendeur();
    _clientsDetailWindow->definirVendeur();
    _achats_aux_fournisseursWindow->definirVendeur();
    _achatsDetailWindow->definirVendeur();
    _detailWindow->definirVendeur();
    _modifierWindow->definirVendeur();
    _entrerWindow->definirVendeur();
    _creerFournisseurWindow->definirVendeur();
    _detailsDunProgrammeDeFideliteClientsWindow->definirVendeur();
    _creerUnProgrammeDeFideliteClientsWindow->definirVendeur();
    _creerGroupeDeClientsWindow->definirVendeur();
    _creerCompteClientWindow->definirVendeur();
    _modifierFournisseurWindow->definirVendeur();
    _modifierCompteClientWindow->definirVendeur();
    _achatsWindow->definirVendeur();
    _stocksWindow->definirVendeur();
    _marchandisesWindow->definirVendeur();
    _configurationComptabiliteWindow->definirVendeur();
    _listerAlertesWindow->definirVendeur();
    _historiquePaiementsWindow->definirVendeur();
    _ventesWindow->definirVendeur();
    _fournisseursWindow->definirVendeur();
    _clientsWindow->definirVendeur();
    _payerAuFournisseurWindow->definirVendeur();
    _payerAuCompteClientWindow->definirVendeur();

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
    _mouvementsDeStocksWindow->definirGestionaireDesStocks();
    _pdVenteWindow->definirGestionaireDesStocks();
    _fournisseurDetailsWindow->definirGestionaireDesStocks();
    _programmesDeFideliteClientsWindow->definirGestionaireDesStocks();
    _detailsGroupeDeClientsWindow->definirGestionaireDesStocks();
    _groupesDeClientsWindow->definirGestionaireDesStocks();
    _groupesDunClientWindow->definirGestionaireDesStocks();
    _clientsDetailWindow->definirGestionaireDesStocks();
    _achats_aux_fournisseursWindow->definirGestionaireDesStocks();
    _achatsDetailWindow->definirGestionaireDesStocks();
    _detailWindow->definirGestionaireDesStocks();
    _modifierWindow->definirGestionaireDesStocks();
    _entrerWindow->definirGestionaireDesStocks();
    _creerFournisseurWindow->definirGestionaireDesStocks();
    _detailsDunProgrammeDeFideliteClientsWindow->definirGestionaireDesStocks();
    _creerUnProgrammeDeFideliteClientsWindow->definirGestionaireDesStocks();
    _creerGroupeDeClientsWindow->definirGestionaireDesStocks();
    _creerCompteClientWindow->definirGestionaireDesStocks();
    _modifierFournisseurWindow->definirGestionaireDesStocks();
    _modifierCompteClientWindow->definirGestionaireDesStocks();
    _achatsWindow->definirGestionaireDesStocks();
    _stocksWindow->definirGestionaireDesStocks();
    _marchandisesWindow->definirGestionaireDesStocks();
    _configurationComptabiliteWindow->definirGestionaireDesStocks();
    _listerAlertesWindow->definirGestionaireDesStocks();
    _historiquePaiementsWindow->definirGestionaireDesStocks();
    _ventesWindow->definirGestionaireDesStocks();
    _fournisseursWindow->definirGestionaireDesStocks();
    _clientsWindow->definirGestionaireDesStocks();
    _payerAuFournisseurWindow->definirGestionaireDesStocks();
    _payerAuCompteClientWindow->definirGestionaireDesStocks();

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
    _mouvementsDeStocksWindow->definirPasDeRole();
    _pdVenteWindow->definirPasDeRole();
    _fournisseurDetailsWindow->definirPasDeRole();
    _programmesDeFideliteClientsWindow->definirPasDeRole();
    _detailsGroupeDeClientsWindow->definirPasDeRole();
    _groupesDeClientsWindow->definirPasDeRole();
    _groupesDunClientWindow->definirPasDeRole();
    _clientsDetailWindow->definirPasDeRole();
    _achats_aux_fournisseursWindow->definirPasDeRole();
    _achatsDetailWindow->definirPasDeRole();
    _detailWindow->definirPasDeRole();
    _modifierWindow->definirPasDeRole();
    _entrerWindow->definirPasDeRole();
    _creerFournisseurWindow->definirPasDeRole();
    _detailsDunProgrammeDeFideliteClientsWindow->definirPasDeRole();
    _creerUnProgrammeDeFideliteClientsWindow->definirPasDeRole();
    _creerGroupeDeClientsWindow->definirPasDeRole();
    _creerCompteClientWindow->definirPasDeRole();
    _modifierFournisseurWindow->definirPasDeRole();
    _modifierCompteClientWindow->definirPasDeRole();
    _achatsWindow->definirPasDeRole();
    _stocksWindow->definirPasDeRole();
    _marchandisesWindow->definirPasDeRole();
    _configurationComptabiliteWindow->definirPasDeRole();
    _listerAlertesWindow->definirPasDeRole();
    _historiquePaiementsWindow->definirPasDeRole();
    _ventesWindow->definirPasDeRole();
    _fournisseursWindow->definirPasDeRole();
    _clientsWindow->definirPasDeRole();
    _payerAuFournisseurWindow->definirPasDeRole();
    _payerAuCompteClientWindow->definirPasDeRole();

    _adminWindow->definirPasDeRole();
    _adminCreateWindow->definirPasDeRole();
    _adminListerWindow->definirPasDeRole();
    _adminDetailWindow->definirPasDeRole();
    _adminModifierWindow->definirPasDeRole();
}

void YerothERPWindows::hideAllWindows()
{
    _sortirWindow->rendreInvisible();
    _transactionsDunFournisseurWindow->rendreInvisible();
    _transactionsDunClientWindow->rendreInvisible();
    _historiqueDuStockWindow->rendreInvisible();
    _statistiquesWindow->rendreInvisible();
    _mouvementsDeStocksWindow->rendreInvisible();
    _pdVenteWindow->rendreInvisible();
    _pdVenteListStocksWindow->rendreInvisible();
    _pdVenteMethodePaiementCarteCreditDialog->rendreInvisible();
    _pdVenteMethodePaiementComptantEntreeDialog->rendreInvisible();
    _pdVenteMethodePaiementDialog->rendreInvisible();
    _sortirListStocksWindow->rendreInvisible();
    _fournisseurDetailsWindow->rendreInvisible();
    _programmesDeFideliteClientsWindow->rendreInvisible();
    _detailsGroupeDeClientsWindow->rendreInvisible();
    _groupesDeClientsWindow->rendreInvisible();
    _groupesDunClientWindow->rendreInvisible();
    _clientsDetailWindow->rendreInvisible();
    _achats_aux_fournisseursWindow->rendreInvisible();
    _achatsDetailWindow->rendreInvisible();
    _detailWindow->rendreInvisible();
    _creerFournisseurWindow->rendreInvisible();
    _detailsDunProgrammeDeFideliteClientsWindow->rendreInvisible();
    _creerUnProgrammeDeFideliteClientsWindow->rendreInvisible();
    _creerGroupeDeClientsWindow->rendreInvisible();
    _creerCompteClientWindow->rendreInvisible();
    _modifierFournisseurWindow->rendreInvisible();
    _modifierCompteClientWindow->rendreInvisible();
    _achatsWindow->rendreInvisible();
    _stocksWindow->rendreInvisible();
    _marchandisesWindow->rendreInvisible();
    _configurationComptabiliteWindow->rendreInvisible();
    _listerAlertesWindow->rendreInvisible();
    _historiquePaiementsWindow->rendreInvisible();
    _ventesWindow->rendreInvisible();
    _fournisseursWindow->rendreInvisible();
    _clientsWindow->rendreInvisible();
    _payerAuFournisseurWindow->rendreInvisible();
    _payerAuCompteClientWindow->rendreInvisible();
    _modifierWindow->rendreInvisible();
    _entrerWindow->rendreInvisible();
    _adminWindow->rendreInvisible();
    _adminCreateWindow->rendreInvisible();
    _adminListerWindow->rendreInvisible();
    _adminDetailWindow->rendreInvisible();
    _adminModifierWindow->rendreInvisible();

    _mainWindow->rendreInvisible();
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_comptes_bancaires()
{
    _tableModel_comptes_bancaires->resetFilter();
    return *_tableModel_comptes_bancaires;
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

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_programmes_de_fidelite_clients()
{
    _tableModel_programmes_de_fidelite_clients->resetFilter();
    return *_tableModel_programmes_de_fidelite_clients;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_clients()
{
    _tableModel_clients->resetFilter();
    return *_tableModel_clients;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_groupes_de_clients()
{
    _tableModel_groupes_de_clients->resetFilter();
    return *_tableModel_groupes_de_clients;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_fournisseurs()
{
    _tableModel_fournisseurs->resetFilter();
    return *_tableModel_fournisseurs;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_achats_aux_fournisseurs()
{
	_tableModel_achats_aux_fournisseurs->resetFilter();
    return *_tableModel_achats_aux_fournisseurs;
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


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_services_completes()
{
	_tableModel_services_completes->resetFilter();
    return *_tableModel_services_completes;
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


/**
 * !!! DO NOT MODIFY AT ALL !!!
 *
 *  !!! THIS IS A VERY IMPORTANT PIECE OF CODE !!!
 */
int YerothERPWindows::NEXT_TABLE_ID_MYSQL_FOR_STOCKS()
{
	int max_id = 0;

	QString MAX_ID_QUERY_FOR_STOCKS(QString("SELECT IFNULL(MAX(id), 0)"
						  " FROM"
						  " ("
						  " SELECT id FROM %1"
						  " UNION ALL"
						  " SELECT %2 as id FROM %3"
						  " UNION ALL"
						  " SELECT %4 as id FROM %5"
						  " ) a")
					.arg(STOCKS,
						 YerothDatabaseTableColumn::STOCKS_ID,
						 STOCKS_VENDU,
						 YerothDatabaseTableColumn::STOCKS_ID,
						 SERVICES_COMPLETES));

//	QDEBUG_STRINGS_OUTPUT_2("MAX_ID_QUERY_FOR_STOCKS", MAX_ID_QUERY_FOR_STOCKS);

	QSqlQuery aQuery;

    int querySize = YerothUtils::execQuery(aQuery, MAX_ID_QUERY_FOR_STOCKS);

    if (querySize > 0 && aQuery.next())
    {
    	max_id = aQuery.value(0).toInt();

    	max_id += 1;
    }

    return max_id;
}


int YerothERPWindows::getNextIdFromTable(const QString &tableName)
{
    //qDebug() << "[YerothUtils::getLastIdFromTable()] : next id from table '" << tableName << "'";
    QString strQuery(QString("SELECT %1 FROM %2 ORDER BY %3 DESC LIMIT 0, 1")
    					.arg(YerothDatabaseTableColumn::ID,
    						 tableName,
							 YerothDatabaseTableColumn::ID));

    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();

    if (query.last())
    {
        int lastId = query.value(0).toInt();
        ++lastId;
//        qDebug() << "\t next id 1: " << lastId;
        return lastId;
    }

    return 0;
}
