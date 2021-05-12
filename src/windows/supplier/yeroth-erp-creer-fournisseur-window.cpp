/*
 * yeroth-erp-creer-fournisseur-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-creer-fournisseur-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>


YerothCreerFournisseurWindow::YerothCreerFournisseurWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerFournisseurWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("créer un fournisseur"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                   .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                                		COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_charges_financieres->disable(this);
    pushButton_fournisseurs->disable(this);
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
    connect(actionCHARGES_FINANCIERES, SIGNAL(triggered()), this, SLOT(charges_financieres()));
    connect(actionFournisseurs, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerImage, SIGNAL(triggered()), this, SLOT(supprimer_image_fournisseur()));
    connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(creerEnregistrerFournisseur()));
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

void YerothCreerFournisseurWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}

void YerothCreerFournisseurWindow::setupLineEdits()
{
	lineEdit_fournisseur_dette_maximale->setYerothEnabled(false);
    lineEdit_fournisseur_dette_maximale->setValidator(&YerothUtils::POSITIVE_DoubleValidator);
}


void YerothCreerFournisseurWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothCreerFournisseurWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_charges_financieres->disable(this);
    pushButton_fournisseurs->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerothCreerFournisseurWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
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

    pushButton_charges_financieres->disable(this);
    pushButton_fournisseurs->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerothCreerFournisseurWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
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

    pushButton_charges_financieres->enable(this, SLOT(charges_financieres()));
    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(fournisseurs()));
    pushButton_supprimer_limage_du_stock->enable(this, SLOT(supprimer_image_fournisseur()));
    pushButton_selectionner_image->enable(this, SLOT(selectionner_image_fournisseur()));
    pushButton_enregistrer->enable(this, SLOT(creerEnregistrerFournisseur()));
}


void YerothCreerFournisseurWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
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

    pushButton_charges_financieres->enable(this, SLOT(charges_financieres()));
    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(fournisseurs()));
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->enable(this, SLOT(creerEnregistrerFournisseur()));
}


void YerothCreerFournisseurWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
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

    pushButton_charges_financieres->disable(this);
    pushButton_fournisseurs->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerothCreerFournisseurWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
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

    pushButton_charges_financieres->disable(this);
    pushButton_fournisseurs->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}


bool YerothCreerFournisseurWindow::supplierAlreadyExist()
{
	bool result = false;

	YerothSqlTableModel &fournisseursTableModel = _allWindows->getSqlTableModel_fournisseurs();

	// ** check if customer account with same name exist
	bool resultNomEntreprise =
				YerothUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(*this, fournisseursTableModel,
																		*lineEdit_fournisseur_nom_de_lentreprise);

	result = result || resultNomEntreprise;

	// ** check if customer account with same trade registry number exist
	bool resultReferenceRegistreDuCommerce =
			YerothUtils::checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE(*this, fournisseursTableModel,
																	*lineEdit_fournisseur_reference_du_registre_du_commerce);

	result = result || resultReferenceRegistreDuCommerce;


	// ** check if SUPPLIER account with same reference exist
	bool resultReference_FOURNISSEUR =
			YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
													    fournisseursTableModel,
														*lineEdit_fournisseur_reference_du_registre_du_commerce,
														QObject::trUtf8("la référence du fournisseur"),
														YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR);

	result = result || resultReference_FOURNISSEUR;

	return result;
}


bool YerothCreerFournisseurWindow::check_fields()
{
    bool result;

    bool nom_entreprise = lineEdit_fournisseur_nom_de_lentreprise->checkField();

    result = nom_entreprise;

    return result;
}


void YerothCreerFournisseurWindow::clear_all_fields()
{
	lineEdit_fournisseur_reference_fournisseur->clear();
	lineEdit_fournisseur_nom_de_lentreprise->clearField();
	lineEdit_fournisseur_nom_du_representant->clearField();
    lineEdit_fournisseur_quartier->clear();
    lineEdit_fournisseur_ville->clear();
    lineEdit_fournisseur_province_etat->clear();
    lineEdit_fournisseur_pays->clear();
    lineEdit_fournisseur_boite_postale->clear();
    lineEdit_fournisseur_siege_social->clear();
    lineEdit_fournisseur_email->clear();
    lineEdit_fournisseur_numero_telephone_1->clear();
    lineEdit_fournisseur_numero_telephone_2->clear();
    lineEdit_fournisseur_reference_du_registre_du_commerce->clear();
    lineEdit_fournisseur_numero_de_contribuable->clear();
    lineEdit_fournisseur_dette_maximale->clear();
    textEdit_creer_fournisseur_description_fournisseur->clear();
    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);
}

void YerothCreerFournisseurWindow::rendreInvisible()
{
    clear_all_fields();
    setVisible(false);
}

void YerothCreerFournisseurWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    YerothPOSUser *aUser = YerothUtils::getAllWindows()->getUser();

    if (0 != YerothUtils::getAllWindows())
    {
    	if (0 != aUser && aUser->isManager())
    	{
    		label_fournisseur_dette_maximale->setVisible(true);
    		lineEdit_fournisseur_dette_maximale->setVisible(true);
    	}
    	else
    	{
    		label_fournisseur_dette_maximale->setVisible(false);
    		lineEdit_fournisseur_dette_maximale->setVisible(false);
    	}
    }

    check_fields();

    lineEdit_fournisseur_nom_de_lentreprise->setFocus();

    setVisible(true);
}


bool YerothCreerFournisseurWindow::creerEnregistrerFournisseur()
{
    if (check_fields())
    {
        if (supplierAlreadyExist())
        {
        	return false;
        }

        QString retMsg(QObject::tr("Le fournisseur '"));

        YerothSqlTableModel &fournisseursTableModel = _allWindows->getSqlTableModel_fournisseurs();

        QSqlRecord record = fournisseursTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, YerothERPWindows::getNextIdSqlTableModel_fournisseurs());
        record.setValue(YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR, lineEdit_fournisseur_reference_fournisseur->text());
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_fournisseur_nom_de_lentreprise->text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT, lineEdit_fournisseur_nom_du_representant->text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_fournisseur_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_fournisseur_ville->text());
        record.setValue(YerothDatabaseTableColumn::PROVINCE_ETAT, lineEdit_fournisseur_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_fournisseur_pays->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_fournisseur_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL, lineEdit_fournisseur_siege_social->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_fournisseur_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_fournisseur_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_fournisseur_numero_telephone_2->text());
        record.setValue(YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE, lineEdit_fournisseur_reference_du_registre_du_commerce->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_fournisseur_numero_de_contribuable->text());
		record.setValue(YerothDatabaseTableColumn::COMPTE_FOURNISSEUR, 0.0);

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR,
        		textEdit_creer_fournisseur_description_fournisseur->toPlainText());

        if (0 != label_image_produit 	&&
        	label_image_produit->pixmap())
        {
        	QByteArray bytes;
        	YerothUtils::savePixmapToByteArray(bytes, *label_image_produit->pixmap(), "JPG");
        	record.setValue(YerothDatabaseTableColumn::IMAGE_FOURNISSEUR, bytes);
        }

        retMsg.append(lineEdit_fournisseur_nom_de_lentreprise->text());

        bool success = fournisseursTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-pgi-3.0 ~ administration ~ créer ~ fournisseur"),
                                 retMsg);
            return false;
        }

        fournisseursTableModel.select();

        retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-pgi-3.0 ~ administration ~ créer ~ fournisseur"),
                                 retMsg);

        clear_all_fields();

        fournisseurs();
    }

    return true;
}


