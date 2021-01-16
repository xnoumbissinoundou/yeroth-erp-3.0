/*
 * yeroth-erp-creer-compte-client-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-creer-compte-client-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


YerothCreerCompteClientWindow::YerothCreerCompteClientWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerCompteClientWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("créer un compte client"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                                    	 COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);

    setupLineEdits();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsAUXFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_achats_aux_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAchatsAUXFournisseurs, SIGNAL(triggered()), this, SLOT(achats_aux_fournisseurs()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerImage, SIGNAL(triggered()), this, SLOT(supprimer_image_compte_client()));
    connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(creerEnregistrerCompteClient()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();
}

void YerothCreerCompteClientWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}

void YerothCreerCompteClientWindow::setupLineEdits()
{
	lineEdit_compte_client_dette_maximale->setYerothEnabled(false);
    lineEdit_compte_client_dette_maximale->setValidator(&YerothUtils::DoubleValidator);
}


void YerothCreerCompteClientWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothCreerCompteClientWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsAUXFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_achats_aux_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerothCreerCompteClientWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsAUXFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats_aux_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerothCreerCompteClientWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsAUXFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats_aux_fournisseurs->enable(this, SLOT(achats_aux_fournisseurs()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(clients()));
    pushButton_supprimer_limage_du_stock->enable(this, SLOT(supprimer_image_compte_client()));
    pushButton_selectionner_image->enable(this, SLOT(selectionner_image_compte_client()));
    pushButton_enregistrer->enable(this, SLOT(creerEnregistrerCompteClient()));
}


void YerothCreerCompteClientWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsAUXFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats_aux_fournisseurs->enable(this, SLOT(achats_aux_fournisseurs()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(clients()));
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->enable(this, SLOT(creerEnregistrerCompteClient()));
}


void YerothCreerCompteClientWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsAUXFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats_aux_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerothCreerCompteClientWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsAUXFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats_aux_fournisseurs->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}


bool YerothCreerCompteClientWindow::customerAccountAlreadyExist()
{
	bool result = false;

	YerothSqlTableModel &clientsTableModel = _allWindows->getSqlTableModel_clients();

	// ** check if customer account with same name exist
	bool resultNomEntreprise =
				YerothUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(*this, clientsTableModel,
																		*lineEdit_compte_client_nom_de_lentreprise);

	result = result || resultNomEntreprise;

	// ** check if customer account with same trade registry number exist
	bool resultReferenceRegistreDuCommerce =
			YerothUtils::checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE(*this, clientsTableModel,
																	*lineEdit_compte_client_reference_du_registre_du_commerce);

	result = result || resultReferenceRegistreDuCommerce;


	// ** check if customer account with same reference exist
	bool resultReferenceClient =
			YerothUtils::checkIfCustomerAccountAlreadyExist_REFERENCECLIENT(*this, clientsTableModel,
																	*lineEdit_compte_client_reference_client);

	result = result || resultReferenceClient;

	return result;
}


bool YerothCreerCompteClientWindow::check_fields()
{
    bool result;

    bool nom_entreprise = lineEdit_compte_client_nom_de_lentreprise->checkField();

    result = nom_entreprise;

    return result;
}


void YerothCreerCompteClientWindow::clear_all_fields()
{
	lineEdit_compte_client_reference_client->clear();
	lineEdit_compte_client_nom_de_lentreprise->clearField();
	lineEdit_compte_client_nom_du_representant->clearField();
    lineEdit_compte_client_quartier->clear();
    lineEdit_compte_client_ville->clear();
    lineEdit_compte_client_province_etat->clear();
    lineEdit_compte_client_pays->clear();
    lineEdit_compte_client_boite_postale->clear();
    lineEdit_compte_client_siege_social->clear();
    lineEdit_compte_client_email->clear();
    lineEdit_compte_client_numero_telephone_1->clear();
    lineEdit_compte_client_numero_telephone_2->clear();
    lineEdit_compte_client_reference_du_registre_du_commerce->clear();
    lineEdit_compte_client_numero_de_contribuable->clear();
    lineEdit_compte_client_dette_maximale->clear();
    lineEdit_compte_client_refereur->clear();
    textEdit_creer_compte_client_description_client->clear();
    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);
}

void YerothCreerCompteClientWindow::rendreInvisible()
{
    clear_all_fields();
    setVisible(false);
}

void YerothCreerCompteClientWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    YerothPOSUser *aUser = YerothUtils::getAllWindows()->getUser();

    if (0 != YerothUtils::getAllWindows())
    {
    	if (0 != aUser && aUser->isManager())
    	{
    		label_compte_client_dette_maximale->setVisible(true);
    		lineEdit_compte_client_dette_maximale->setVisible(true);
    	}
    	else
    	{
    		label_compte_client_dette_maximale->setVisible(false);
    		lineEdit_compte_client_dette_maximale->setVisible(false);
    	}
    }

    check_fields();

    lineEdit_compte_client_nom_de_lentreprise->setFocus();

    setVisible(true);
}


bool YerothCreerCompteClientWindow::creerEnregistrerCompteClient()
{
    if (check_fields())
    {
        if (customerAccountAlreadyExist())
        {
        	return false;
        }

        QString retMsg(QObject::tr("Le client '"));

        YerothSqlTableModel &clientsTableModel = _allWindows->getSqlTableModel_clients();

        QSqlRecord record = clientsTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, YerothERPWindows::getNextIdSqlTableModel_clients());
        record.setValue(YerothDatabaseTableColumn::REFERENCE_CLIENT, lineEdit_compte_client_reference_client->text());
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_compte_client_nom_de_lentreprise->text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT, lineEdit_compte_client_nom_du_representant->text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_compte_client_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_compte_client_ville->text());
        record.setValue(YerothDatabaseTableColumn::PROVINCE_ETAT, lineEdit_compte_client_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_compte_client_pays->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_compte_client_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL, lineEdit_compte_client_siege_social->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_compte_client_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_compte_client_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_compte_client_numero_telephone_2->text());
        record.setValue(YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE, lineEdit_compte_client_reference_du_registre_du_commerce->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_compte_client_numero_de_contribuable->text());
        record.setValue(YerothDatabaseTableColumn::REFEREUR_CLIENT, lineEdit_compte_client_refereur->text());
        record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, 0.0);
		record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, 0.0);

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CLIENT, textEdit_creer_compte_client_description_client->toPlainText());

        if (0 != label_image_produit 	&&
        	label_image_produit->pixmap())
        {
        	QByteArray bytes;
        	YerothUtils::savePixmapToByteArray(bytes, *label_image_produit->pixmap(), "JPG");
        	record.setValue(YerothDatabaseTableColumn::IMAGE_COMPTE_CLIENT, bytes);
        }

        retMsg.append(lineEdit_compte_client_nom_de_lentreprise->text());

        bool success = clientsTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("créer 1 compte client"),
                                 retMsg);
            return false;
        }

        clientsTableModel.select();

        retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("créer 1 compte client"),
                                 retMsg);

        clear_all_fields();

        clients();
    }

    return true;
}


