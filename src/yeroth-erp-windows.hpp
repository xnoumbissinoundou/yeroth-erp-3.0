/*
 * yeroth-erp-windows.hpp
 *
 *  Created on: Oct 7, 2015
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_WINDOWS_HPP_
#define SRC_YEROTH_WINDOWS_HPP_


#include "src/dialogs/yeroth-erp-pointdevente-methode-paiement-carte-credit-dialog.hpp"

#include "src/dialogs/yeroth-erp-pointdevente-methode-paiement-comptant-entree-dialog.hpp"
#include "src/dialogs/yeroth-erp-pointdevente-methode-paiement-dialog.hpp"

#include "src/windows/yeroth-erp-main-window.hpp"
#include "src/windows/yeroth-erp-entrer-window.hpp"

#ifndef YEROTH_ERP_3_0_TEST
	#include "src/windows/yeroth-erp-pointdevente-window.hpp"
#else
	#include "src/windows/tests/yeroth-erp-test-vente-window.hpp"
#endif

#include "windows/yeroth-erp-paiements-window.hpp"
#include "src/windows/yeroth-erp-historique-du-stock-window.hpp"
#include "src/windows/yeroth-erp-sortir-window.hpp"
#include "src/windows/yeroth-erp-tableaux-de-bord-window.hpp"
#include "src/windows/yeroth-erp-transactions-window.hpp"
#include "src/windows/yeroth-erp-clients-detail-window.hpp"
#include "src/windows/yeroth-erp-achats-detail-window.hpp"
#include "src/windows/yeroth-erp-stock-detail-window.hpp"
#include "src/dialogs/yeroth-erp-changer-utilisateur-dialog.hpp"
#include "src/windows/yeroth-erp-creer-compte-client-window.hpp"
#include "src/windows/yeroth-erp-creer-nouveau-client-window.hpp"
#include "src/windows/yeroth-erp-creer-nouveau-fournisseur-window.hpp"
#include "src/windows/yeroth-erp-creer-nouvelle-categorie-window.hpp"
#include "src/windows/yeroth-erp-achats-window.hpp"
#include "src/windows/yeroth-erp-stocks-window.hpp"
#include "src/windows/yeroth-erp-marchandises-window.hpp"
#include "src/windows/yeroth-erp-alertes-window.hpp"
#include "src/windows/yeroth-erp-ventes-window.hpp"
#include "src/windows/yeroth-erp-modifier-window.hpp"
#include "src/windows/yeroth-erp-pointdevente-liststocks-window.hpp"
#include "src/windows/yeroth-erp-clients-window.hpp"
#include "src/windows/yeroth-erp-sortir-liststocks-window.hpp"

#include "src/admin/yeroth-erp-admin-window.hpp"
#include "src/admin/creer/yeroth-erp-admin-create-window.hpp"
#include "src/admin/lister/yeroth-erp-admin-lister-window.hpp"
#include "src/admin/detail/yeroth-erp-admin-detail-window.hpp"
#include "src/admin/modifier/yeroth-erp-admin-modifier-window.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"
#include "src/utils/yeroth-erp-info-entreprise.hpp"
#include "src/utils/yeroth-erp-database.hpp"
#include "src/utils/yeroth-erp-utils.hpp"

#include <QtSql/QSqlDatabase>
#include "utils/yeroth-erp-database-table-column.hpp"

class YerothSqlTableModel;
class YerothMainWindow;
class YerothPaiementsWindow;
class YerothHistoriqueDuStockWindow;
class YerothPOSChangerUtilisateurDialog;
class YerothCreerCompteClientWindow;
class YerothCreerNouveauClientWindow;
class YerothCreerNouveauFournisseurWindow;
class YerothCreerNouvelleCategorieWindow;
class YerothAchatsWindow;
class YerothStocksWindow;
class YerothMarchandisesWindow;
class YerothAlertesWindow;
class YerothVentesWindow;
class YerothModifierWindow;
class YerothSortirWindow;
class YerothTableauxDeBordWindow;
class YerothTransactionsWindow;
class YerothPointDeVenteWindow;
class YerothPOSVenteMethodePaiementCarteCreditDialog;
class YerothPOSVenteMethodePaiementComptantEntreeDialog;
class YerothPOSVenteMethodePaiementDialog;
class YerothClientsDetailWindow;
class YerothAchatsDetailWindow;
class YerothStockDetailWindow;
class YerothEntrerWindow;
class YerothAdminWindow;
class YerothAdminCreateWindow;
class YerothAdminListerWindow;
class YerothAdminDetailWindow;
class YerothAdminModifierWindow;
class YerothPOSUser;
class YerothInfoEntreprise;
class YerothLogger;
class YerothDatabase;
class YerothPointDeVenteListStocksWindow;
class YerothERPClientsWindow;
class YerothSortirListStocksWindow;
class YerothUtils;

#ifdef YEROTH_SERVER
	class YerothDBusServer;
#endif

#ifdef YEROTH_CLIENT
	class YerothDBusClient;
#endif

class QString;
class QDesktopWidget;

class YerothERPWindows
{

public:

	YEROTH_CLASS_OPERATORS

	YerothERPWindows(QDesktopWidget *desktopWidget);

	~YerothERPWindows();

	void createAllYerothPosUserWindows();

	void reinitialiseSqlTableModels();

	void setupConfiguration();

	YerothSqlTableModel &getSqlTableModel_entreprise_info();

	YerothSqlTableModel &getSqlTableModel_paiements();

	YerothSqlTableModel &getSqlTableModel_users();

	YerothSqlTableModel &getSqlTableModel_titres();

	YerothSqlTableModel &getSqlTableModel_localisations();

	YerothSqlTableModel &getSqlTableModel_categories();

	YerothSqlTableModel &getSqlTableModel_alertes();

	YerothSqlTableModel &getSqlTableModel_remises();

	YerothSqlTableModel &getSqlTableModel_conditions_alertes();

	YerothSqlTableModel &getSqlTableModel_courriers_alertes();

	YerothSqlTableModel &getSqlTableModel_clients();

	YerothSqlTableModel &getSqlTableModel_fournisseurs();

	YerothSqlTableModel &getSqlTableModel_achats();

	YerothSqlTableModel &getSqlTableModel_stocks();

	YerothSqlTableModel &getSqlTableModel_marchandises();

	YerothSqlTableModel &getSqlTableModel_stocks_vendu();

	YerothSqlTableModel &getSqlTableModel_stocks_sorties();

	YerothSqlTableModel &getSqlTableModel_configurations();

	YerothSqlTableModel &getSqlTableModel_init_configurations();

#ifdef YEROTH_SERVER
	inline YerothDBusServer	*dbusServer()
	{
		return _dbusServer;
	}
#endif

#ifdef YEROTH_CLIENT
	inline YerothDBusClient *dbusClient()
	{
		return _dbusClient;
	}
#endif

	inline int getLastSelectedListerRow()
	{
		return this->_stocksWindow->getLastListerSelectedRow();
	}

	inline void setDesktopWidget(QDesktopWidget *desktopWidget)
	{
		_desktopWidget = desktopWidget;
	}

	inline QDesktopWidget &desktopWidget()
	{
		return *_desktopWidget;
	}

	inline int getNextIdSqlTableModel_users()
	{
		return YerothUtils::getNextIdFromTable(this->USERS);
	}

	inline int getNextIdSqlTableModel_titres()
	{
		return YerothUtils::getNextIdFromTable(this->TITRES);
	}

	inline int getNextIdSqlTableModel_localisations()
	{
		return YerothUtils::getNextIdFromTable(this->LOCALISATIONS);
	}

	inline int getNextIdSqlTableModel_categories()
	{
		return YerothUtils::getNextIdFromTable(this->CATEGORIES);
	}

	inline int getNextIdSqlTableModel_alertes()
	{
		return YerothUtils::getNextIdFromTable(this->ALERTES);
	}

	inline int getNextIdSqlTableModel_conditions_alertes()
	{
		return YerothUtils::getNextIdFromTable(this->CONDITIONS_ALERTES);
	}

	inline int getNextIdSqlTableModel_courriers_alertes()
	{
		return YerothUtils::getNextIdFromTable(this->COURRIERS_ALERTES);
	}

	inline int getNextIdSqlTableModel_clients()
	{
		return YerothUtils::getNextIdFromTable(this->CLIENTS);
	}

	inline int getNextIdSqlTableModel_fournisseurs()
	{
		return YerothUtils::getNextIdFromTable(this->FOURNISSEURS);
	}

	inline int getNextIdSqlTableModel_achats()
	{
		return YerothUtils::getNextIdFromTable(this->ACHATS);
	}

	inline int getNextIdSqlTableModel_stocks()
	{
		return YerothUtils::getNextIdFromTable(this->STOCKS);
	}

	inline int getNextIdSqlTableModel_stocks_vendu()
	{
		return YerothUtils::getNextIdFromTable(this->STOCKS_VENDU);
	}

	inline int getNextIdSqlTableModel_stocks_sorties()
	{
		return YerothUtils::getNextIdFromTable(this->STOCKS_SORTIES);
	}

	inline void setCurrentPosition(QPoint *currentPosition)
	{
		_currentPosition = currentPosition;
	}

	void setCurrentPosition(int xPos, int yPos);

	inline QPoint *getCurrentPosition()
	{
		return _currentPosition;
	}

	inline YerothPOSUser *getUser()
	{
		return _user;
	}

	inline YerothDatabase &getDatabase()
	{
		return *_database;
	}

	inline void setDatabase(YerothDatabase *database)
	{
		_database = database;
	}

	inline YerothInfoEntreprise &getInfoEntreprise()
	{
		return *this->_infoEntreprise;
	}

	bool move(QMainWindow &aWindow);

	bool updateUserData(int userRecordRow);

	void setUser(YerothPOSUser *user);

	void definirAdministrateur();

	void definirMagasinier();

	void definirCaissier();

	void definirManager();

	void definirVendeur();

	void definirGestionaireDesStocks();

	void definirPasDeRole();

	void hideAllWindows();

	static const QString ENTREPRISE_INFO;
	static const QString PAIEMENTS;
	static const QString USERS;
	static const QString TITRES;
	static const QString LOCALISATIONS;
	static const QString CATEGORIES;
	static const QString CLIENTS;
	static const QString FOURNISSEURS;
	static const QString ALERTES;
	static const QString REMISES;
	static const QString ACHATS;
	static const QString STOCKS;
	static const QString STOCKS_VENDU;
	static const QString STOCKS_SORTIES;
	static const QString MARCHANDISES;
	static const QString CONFIGURATIONS;
	static const QString INIT_CONFIGURATIONS;
	static const QString ROLES;
	static const QString CREDIT_CARD_ISSUING_COMPANY;
	static const QString CONDITIONS_ALERTES;
	static const QString COURRIERS_ALERTES;
	static const QString DBMS;

	YerothMainWindow 									*_mainWindow;

	YerothPOSChangerUtilisateurDialog 					*_changerUtilisateurDialog;
	YerothHistoriqueDuStockWindow						*_historiqueDuStockWindow;
	YerothCreerCompteClientWindow 						*_creerCompteClientWindow;
	YerothCreerNouveauClientWindow 						*_creerNouveauClientWindow;
	YerothCreerNouveauFournisseurWindow 				*_creerNouveauFournisseurWindow;
	YerothCreerNouvelleCategorieWindow 					*_creerNouvelleCategorieWindow;
	YerothAchatsWindow 									*_achatsWindow;
	YerothStocksWindow 									*_stocksWindow;
	YerothMarchandisesWindow 							*_marchandisesWindow;
	YerothAlertesWindow									*_listerAlertesWindow;
	YerothPaiementsWindow								*_historiquePaiementsWindow;
	YerothVentesWindow									*_ventesWindow;
	YerothModifierWindow 								*_modifierWindow;
	YerothSortirWindow 									*_sortirWindow;
	YerothTableauxDeBordWindow 							*_statistiquesWindow;
	YerothTransactionsWindow 							*_transactionsWindow;
	YerothPointDeVenteWindow 							*_pdVenteWindow;
	YerothClientsDetailWindow 							*_clientsDetailWindow;
	YerothAchatsDetailWindow 							*_achatsDetailWindow;
	YerothStockDetailWindow 							*_detailWindow;
	YerothEntrerWindow 									*_entrerWindow;
	YerothPointDeVenteListStocksWindow					*_pdVenteListStocksWindow;
	YerothPOSVenteMethodePaiementCarteCreditDialog		*_pdVenteMethodePaiementCarteCreditDialog;
	YerothPOSVenteMethodePaiementComptantEntreeDialog	*_pdVenteMethodePaiementComptantEntreeDialog;
	YerothPOSVenteMethodePaiementDialog					*_pdVenteMethodePaiementDialog;
	YerothERPClientsWindow								*_clientWindow;
	YerothSortirListStocksWindow						*_sortirListStocksWindow;
	YerothAdminWindow 									*_adminWindow;
	YerothAdminCreateWindow 							*_adminCreateWindow;
	YerothAdminListerWindow 							*_adminListerWindow;
	YerothAdminDetailWindow 							*_adminDetailWindow;
	YerothAdminModifierWindow							*_adminModifierWindow;

private:

	YerothDatabase 			*_database;

	YerothSqlTableModel 	*_tableModel_entreprise_info;
	YerothSqlTableModel 	*_tableModel_historique_paiements;
	YerothSqlTableModel 	*_tableModel_users;
	YerothSqlTableModel 	*_tableModel_titres;
	YerothSqlTableModel 	*_tableModel_localisations;
	YerothSqlTableModel 	*_tableModel_categories;
	YerothSqlTableModel 	*_tableModel_clients;
	YerothSqlTableModel 	*_tableModel_fournisseurs;
	YerothSqlTableModel 	*_tableModel_alertes;
	YerothSqlTableModel 	*_tableModel_remises;
	YerothSqlTableModel 	*_tableModel_conditions_alertes;
	YerothSqlTableModel 	*_tableModel_courriers_alertes;
	YerothSqlTableModel 	*_tableModel_achats;
	YerothSqlTableModel 	*_tableModel_stocks;
	YerothSqlTableModel 	*_tableModel_marchandises;
	YerothSqlTableModel 	*_tableModel_stocks_vendu;
	YerothSqlTableModel 	*_tableModel_stocks_sorties;
	YerothSqlTableModel 	*_tableModel_configurations;
	YerothSqlTableModel 	*_tableModel_init_configurations;

#ifdef YEROTH_SERVER
	YerothDBusServer		*_dbusServer;
#endif

#ifdef YEROTH_CLIENT
	YerothDBusClient		*_dbusClient;
#endif

	YerothLogger			*_logger;

	YerothInfoEntreprise 	*_infoEntreprise;
	YerothPOSUser 			*_user;

	QPoint 					*_currentPosition;
	QDesktopWidget 			*_desktopWidget;
};

#endif /* SRC_YEROTH_WINDOWS_HPP_ */
