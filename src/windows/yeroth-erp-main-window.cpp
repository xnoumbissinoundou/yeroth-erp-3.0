/*
 * yeroth-erp-main-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-main-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-user-administrateur.hpp"

#include "src/users/yeroth-erp-user-caissier.hpp"

#include "src/users/yeroth-erp-user-magasinier.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/utils/yeroth-erp-info-entreprise.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/dialogs/yeroth-erp-changer-utilisateur-dialog.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>


YerothMainWindow::YerothMainWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothMainWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("fenêtre de l'utilisateur"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {messagebox-text-interaction-flags: 5; background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);} ")
                                    .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                                    	 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();

    label_display_yeroth->setStyleSheet(YerothUtils::STRING_STYLESHEETS_YEROTH_ERP_3_0);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);

    pushButton_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_marchandises->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_vendre->disable(this);

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionGuide_PRATIQUE_DE_LUTILISATEUR, SIGNAL(triggered()), this, SLOT(getManuelUtilisateurPDF()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionMarchandises, SIGNAL(triggered()), this, SLOT(afficherMarchandises()));
    connect(actionCHARGES_FINANCIERES, SIGNAL(triggered()), this, SLOT(charges_financieres()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionTableauxDeBords, SIGNAL(triggered()), this, SLOT(tableaux_de_bords()));
    connect(actionVente, SIGNAL(triggered()), this, SLOT(ventes()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
	connect(actionPaiements, SIGNAL(triggered()), this, SLOT(paiements()));
	connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
	connect(actionMouvementsDeStocks, SIGNAL(triggered()), this, SLOT(mouvements_de_stocks()));
	connect(actionFournisseurs, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(courrier()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

#ifdef YEROTH_FEATURES_COMPTABILITE_yes
    connect(action_acceder_aux_comptes_de_lentreprise, SIGNAL(triggered()), this, SLOT(comptabilite()));
#endif

    setupShortcuts();
}

void YerothMainWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothMainWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->enable(this, SLOT(ventes()));
    pushButton_vendre->enable(this, SLOT(vendre()));

    label_display_yeroth->setVisible(false);
}

void YerothMainWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);

#ifdef YEROTH_FEATURES_COMPTABILITE_yes
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, true);
#endif

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_charges_financieres->enable(this, SLOT(charges_financieres()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises->enable(this, SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_rapports->enable(this, SLOT(tableaux_de_bords()));
    pushButton_mouvementsDeStocks->enable(this, SLOT(mouvements_de_stocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
    pushButton_vendre->enable(this, SLOT(vendre()));

    label_display_yeroth->setVisible(false);
}


void YerothMainWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#endif

    pushButton_fournisseurs->disable(this);
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises->enable(this, SLOT(afficherMarchandises()));
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->enable(this, SLOT(ventes()));
    pushButton_vendre->enable(this, SLOT(vendre()));

    label_display_yeroth->setVisible(false);
}


void YerothMainWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#endif

    pushButton_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises->enable(this, SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_mouvementsDeStocks->enable(this, SLOT(mouvements_de_stocks()));

    label_display_yeroth->setVisible(false);
}

void YerothMainWindow::definirAdministrateur()
{

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    _logger->log("definirAdministrateur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    _allWindows->_adminWindow->definirAdministrateur();

#endif

    label_display_yeroth->setVisible(false);
}

void YerothMainWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_marchandises->disable(this);
    pushButton_rapports->disable(this);
    pushButton_ventes->disable(this);
    pushButton_vendre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_mouvementsDeStocks->enable(this, SLOT(mouvements_de_stocks()));
    label_display_yeroth->setVisible(false);
}

void YerothMainWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_marchandises->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_vendre->disable(this);

    label_display_yeroth->setVisible(true);
}


void YerothMainWindow::setup_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY()
{
	YerothUtils::YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY =
			QString("database ip address: \"%1\" - "
					"database table: \"%2\" - "
					"database options: \"%3\"")
				.arg(_allWindows->getDatabase().db_ip_address(),
					 _allWindows->getDatabase().db_name(),
					 _allWindows->getDatabase().db_connection_options());
}


void YerothMainWindow::rendreVisibleLocalTOCLASS(bool isDatabaseOpened /* = false */)
{
	setup_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

	if (isDatabaseOpened)
	{
		label_display_yeroth_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
								.arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255));

		_allWindows->_adminWindow->label_ADMIN_WINDOW_display_yeroth_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
								.arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255));

		_allWindows->_pdVenteWindow->label_PDV_display_yeroth_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
								.arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255));
	}
	else
	{
		label_display_yeroth_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
								.arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48));

		_allWindows->_adminWindow->label_ADMIN_WINDOW_display_yeroth_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
								.arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48));

		_allWindows->_pdVenteWindow->label_PDV_display_yeroth_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
								.arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48));
	}


	label_display_yeroth_erp_3_0_server_parameters
		->setText(YerothUtils::YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY);

	_allWindows->_adminWindow->label_ADMIN_WINDOW_display_yeroth_erp_3_0_server_parameters
		->setText(YerothUtils::YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY);

	_allWindows->_pdVenteWindow->label_PDV_display_yeroth_erp_3_0_server_parameters
		->setText(YerothUtils::YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY);

	show();
}


void YerothMainWindow::help()
{
//#define YEROTH_DEVEL_TARGET

#ifndef YEROTH_DEVEL_TARGET
	YerothPOSUser * currentUser = _allWindows->getUser();

	if (0 != currentUser &&
		! currentUser->isPasDeRole())
	{
	    YerothQMessageBox::information(this, QObject::tr("aide"),
	    				QObject::trUtf8("1) Veuillez vous enregistrer en cliquant sur le premier icône !\n\n"
	                                    "2) Déconnectez vous en cliquant sur le deuxième icône !\n\n"
	                                    "3) Lorsque vous êtes enregistré, choisissez une opération "
	                                    "à réaliser en cliquant " "sur le bouton correspondant !"));
	}
	else
	{
	    YerothQMessageBox::information(this, QObject::tr("aide"),
	    				QObject::trUtf8("1) Veuillez vous enregistrer en cliquant sur le premier icône !\n\n"
	                                    "2) Recevez un message d'aide en cliquant sur le deuxieme icone !"));
	}
#else

    QDEBUG_STRING_OUTPUT_2("YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER",
    		YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER);

    YerothUtils::YEROTH_CREATE_FOLDER(YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER);

	YerothERPConfig::THIS_SITE_LOCALISATION_NAME = "YEROTH_RD_TEST_NOW";

	_allWindows->_adminWindow->start_TESTING_MAINTENANCE();

    //YerothUserLAdministrateur *u = new YerothUserLAdministrateur(_allWindows);
    //YerothUserLeCaissier *u = new YerothUserLeCaissier(_allWindows);
    //YerothUserLeMagasinier *u = new YerothUserLeMagasinier(_allWindows);

	YerothERPConfig::tva_value = 7.25/100.0;

	YerothERPConfig::currency = "FCFA";

	YerothERPConfig::printer = YerothUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU;

    YerothPOSUserManager *u = new YerothPOSUserManager(_allWindows);
    u->set_titre("M.");
    u->set_prenom("Xavier");
    u->set_nom("NOUMBISSI NOUNDOU");
    u->set_nom_utilisateur("xavierp");
    u->set_localisation("Yaoundé");
    u->set_nom_complet("DR. XAVIER NOUMBISSI-NOUNDOU");

    _allWindows->setUser(u);

#endif
}


void YerothMainWindow::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();

	YerothPOSUser *currentUser = _allWindows->getUser();

	if (0 != currentUser)
	{
		currentUser->setRole(YerothUtils::ROLE_INDEFINI);
	}
}


void YerothMainWindow::cleanupAllOtherWindows()
{
    if (0 != _allWindows->getUser())
    {
        definirPasDeRole();
        _allWindows->hideAllWindows();
        show();
    }
}
