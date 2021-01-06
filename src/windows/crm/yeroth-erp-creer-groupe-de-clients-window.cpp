/*
 * yeroth-erp-creer-groupe-de-clients-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-creer-groupe-de-clients-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


YerothCreerGroupeDeClientsWindow::YerothCreerGroupeDeClientsWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerGroupeDeClientsWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("créer un groupe de clients"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                                    	 COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);

    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
		<< YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(_allWindows->GROUPES_DE_CLIENTS);

    setupLineEdits();

    setupLineEditsQCompleters();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_clients->disable(this);
    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_groupes_de_clients->disable(this);
    pushButton_supprimer_membre_du_groupe_de_clients->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionListerLesGroupesDeClients, SIGNAL(triggered()), this, SLOT(groupes_de_clients()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerUnMembreDuGroupeDeClients, SIGNAL(triggered()), this, SLOT(supprimerUnMembreDunGroupeDeClients()));
    connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(creerEnregistrerUnGroupeDeClients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();


    lineEdit_creer_groupe_clients_designation->setFocus();
}

void YerothCreerGroupeDeClientsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}

void YerothCreerGroupeDeClientsWindow::setupLineEdits()
{
	lineEdit_creer_groupe_clients_maximum_de_membres->setValidator(&YerothUtils::DoubleValidator);
}


void YerothCreerGroupeDeClientsWindow::setupLineEditsQCompleters()
{
//	lineEdit_creer_groupe_clients_programme_de_fidelite_de_clients->
//		setupMyStaticQCompleter(_allWindows->,
//								YerothDatabaseTableColumn::);

	lineEdit_creer_groupe_clients_recherche_client_initiaux->
		setupMyStaticQCompleter(_allWindows->CLIENTS,
								YerothDatabaseTableColumn::NOM_ENTREPRISE);
}


void YerothCreerGroupeDeClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothCreerGroupeDeClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_clients->disable(this);
    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_groupes_de_clients->disable(this);
    pushButton_supprimer_membre_du_groupe_de_clients->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerothCreerGroupeDeClientsWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

   definirPasDeRole();
}

void YerothCreerGroupeDeClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_annuler->enable(this, SLOT(annuler_la_creation_dun_groupe_de_clients()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
    pushButton_supprimer_membre_du_groupe_de_clients->enable(this, SLOT(supprimerUnMembreDunGroupeDeClients()));
    pushButton_enregistrer->enable(this, SLOT(creerEnregistrerUnGroupeDeClients()));
}


void YerothCreerGroupeDeClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_annuler->enable(this, SLOT(annuler_la_creation_dun_groupe_de_clients()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
    pushButton_supprimer_membre_du_groupe_de_clients->enable(this, SLOT(supprimerUnMembreDunGroupeDeClients()));
    pushButton_enregistrer->enable(this, SLOT(creerEnregistrerUnGroupeDeClients()));
}


void YerothCreerGroupeDeClientsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}


void YerothCreerGroupeDeClientsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}


bool YerothCreerGroupeDeClientsWindow::clientGroupAlreadyExists()
{
	bool result = false;

	YerothSqlTableModel &groupes_de_clients_TableModel = _allWindows->getSqlTableModel_groupes_de_clients();

	// ** check if customer account with same name exist
	result =
		YerothUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(*this,
																	  groupes_de_clients_TableModel,
																	  *lineEdit_creer_groupe_clients_recherche_client_initiaux);

	return result;
}


bool YerothCreerGroupeDeClientsWindow::check_fields()
{
    bool result;

    bool nom_du_groupe_de_clients_cree = lineEdit_creer_groupe_clients_designation->checkField();

    result = nom_du_groupe_de_clients_cree;

    return result;
}


void YerothCreerGroupeDeClientsWindow::clear_all_fields()
{
	lineEdit_creer_groupe_clients_recherche_client_initiaux->clear();

	lineEdit_creer_groupe_clients_reference_groupe->clear();
	lineEdit_creer_groupe_clients_designation->clearField();
	lineEdit_creer_groupe_clients_maximum_de_membres->clear();
	lineEdit_creer_groupe_clients_programme_de_fidelite_de_clients->clearField();

	textEdit_creer_groupe_clients_description_groupe->clear();

	tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->clearContents();
}


void YerothCreerGroupeDeClientsWindow::rendreInvisible()
{
    setVisible(false);
}


void YerothCreerGroupeDeClientsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    check_fields();

    setVisible(true);
}


void YerothCreerGroupeDeClientsWindow::supprimerUnMembreDunGroupeDeClients()
{

}


bool YerothCreerGroupeDeClientsWindow::creerEnregistrerUnGroupeDeClients()
{
	if (!check_fields())
	{
		if (QMessageBox::Ok ==
				YerothQMessageBox::warning(this, "stock (service)",
										   QObject::tr("Remplisser tous les champs obligatoires !")))
		{
		}
		else
		{
		}

		return false;
	}

	if (clientGroupAlreadyExists())
	{
		QDEBUG_STRINGS_OUTPUT_1("++ TEST TRUE EXISTS ALREADY");
		return false;
	}

	QDEBUG_STRINGS_OUTPUT_1("++ TEST FALSE DOESN'T EXISTS");


//        QString retMsg(QObject::tr("Le client '"));
//
//        YerothSqlTableModel &clientsTableModel = _allWindows->getSqlTableModel_clients();
//
//        QSqlRecord record = clientsTableModel.record();
//
//        record.setValue(YerothDatabaseTableColumn::ID, YerothERPWindows::getNextIdSqlTableModel_clients());
//        record.setValue(YerothDatabaseTableColumn::REFERENCE_CLIENT, lineEdit_compte_client_reference_client->text());
//        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_compte_client_nom_de_lentreprise->text());
//        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT, lineEdit_compte_client_nom_du_representant->text());
//        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_compte_client_quartier->text());
//        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_compte_client_ville->text());
//        record.setValue(YerothDatabaseTableColumn::PROVINCE_ETAT, lineEdit_compte_client_province_etat->text());
//        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_compte_client_pays->text());
//        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_compte_client_boite_postale->text());
//        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL, lineEdit_compte_client_siege_social->text());
//        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_compte_client_email->text());
//        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_compte_client_numero_telephone_1->text());
//        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_compte_client_numero_telephone_2->text());
//        record.setValue(YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE, lineEdit_compte_client_reference_du_registre_du_commerce->text());
//        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_compte_client_numero_de_contribuable->text());
//		record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, 0.0);
//		record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, 0.0);
//
//        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CLIENT, textEdit_creer_compte_client_description_client->toPlainText());
//
//        if (0 != label_image_produit 	&&
//        	label_image_produit->pixmap())
//        {
//        	QByteArray bytes;
//        	YerothUtils::savePixmapToByteArray(bytes, *label_image_produit->pixmap(), "JPG");
//        	record.setValue(YerothDatabaseTableColumn::IMAGE_COMPTE_CLIENT, bytes);
//        }
//
//        retMsg.append(lineEdit_compte_client_nom_de_lentreprise->text());
//
//        bool success = clientsTableModel.insertNewRecord(record);
//
//        if (!success)
//        {
//            retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));
//
//            YerothQMessageBox::warning(this,
//                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ client"),
//                                 retMsg);
//            return false;
//        }
//
//        clientsTableModel.select();
//
//        retMsg.append(QObject::trUtf8("' a été créer avec succès !"));
//
//        YerothQMessageBox::information(this,
//                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ client"),
//                                 retMsg);
//
//        clear_all_fields();
//
//        clients();
//    }

    return true;
}


