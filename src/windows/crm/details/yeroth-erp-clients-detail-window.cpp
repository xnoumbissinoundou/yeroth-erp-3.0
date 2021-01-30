/*
 * yeroth-erp-clients-detail-window.cpp
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-clients-detail-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/widgets/yeroth-erp-progress-bar.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


const int YerothClientsDetailWindow::TRUNCATE_POS_FOR_NOM_ENTREPRISE = 23;


YerothClientsDetailWindow::YerothClientsDetailWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothClientsDetailWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("détails d'un client"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                            		.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                            		     COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);

    setupLineEdits();

    textEdit_client_details_description_du_client->setYerothEnabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGenererCARTE_DE_FIDELITE_ClIENTS, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_clients->disable(this);
    pushButton_groupes_du_client->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_payer_au_compteclient->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionGenererCARTE_DE_FIDELITE_ClIENTS, SIGNAL(triggered()), this, SLOT(YEROTH_PROGRESS_BAR_generer_la_carte_de_fidelite_du_client()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionModifierCompteClient, SIGNAL(triggered()), this, SLOT(modifierCompteClient()));
    connect(actionSupprimerCompteClient, SIGNAL(triggered()), this, SLOT(supprimerCompteClient()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#endif

    setupShortcuts();
}


bool YerothClientsDetailWindow::generer_la_carte_de_fidelite_du_client()
{
    _logger->log("generer_la_carte_de_fidelite_du_client");


    YerothInfoEntreprise::load_COMPANY_INFO_LOGO_FROM_DB_TABLE_ENTREPRISE_INFO();


    QString yerothCustomerAccountImageTmpFile(QString("%1JPG")
    		.arg(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth")));

    const QPixmap *label_image_produit_pixmap = label_image_produit->pixmap();

    if (0 != label_image_produit_pixmap)
    {
        YerothUtils::savePixmapToFile(yerothCustomerAccountImageTmpFile,
        							  *label_image_produit_pixmap,
                                      "JPG");
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(7);


    QString latexFileNamePrefix("yeroth-erp-carte-de-fidelite-client-LOYALTY");

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-client-LOYALTY-program-card");
#endif

    QString texDocument;

    YerothUtils::getLatexCLIENT_LOYALTY_CARD_template(texDocument);


    texDocument.replace("YEROTHCARTEDEFIDELITELOGO", YerothERPConfig::pathToPdfCOMPANY_LOGO);


    if (0 != label_image_produit_pixmap)
    {
    	texDocument.replace("YEROTHCHEMINCOMPLETIMAGECOMPTECLIENT", yerothCustomerAccountImageTmpFile);
    }
    else
    {
    	texDocument.replace("YEROTHCHEMINCOMPLETIMAGECOMPTECLIENT", "");
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(12);


    texDocument.replace("YEROTHNOMDUPROGRAMMEDEFIDELITEUN",
    		YerothUtils::LATEX_IN_OUT_handleForeignAccents(_curClientGroups.at(0).toLower().trimmed()));


    YerothInfoEntreprise & infoEntreprise = YerothUtils::getAllWindows()->getInfoEntreprise();

    QString fileDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.get_CARTE_DE_FIDELITE_nom_entreprise());

    texDocument.replace("YEROTHNOMDUCLIENT",
    		QString("%1").arg(lineEdit_clients_details_nom_entreprise->text_LATEX(TRUNCATE_POS_FOR_NOM_ENTREPRISE)));

    texDocument.replace("YEROTHNOMDUPROGRAMMEDEFIDELITE", "");

    texDocument.replace("YEROTHCLIENTNUMEROCOMPTE",
    		YerothUtils::GET_STRING_WITH_NUMBERS_WITHIN_KEPT_FOR_LATEX(
    				lineEdit_clients_details_reference_client->text()));

    texDocument.replace("YEROTHCLIENTRUEOUQUARTIER",
    		YerothUtils::GET_STRING_WITH_NUMBERS_WITHIN_KEPT_FOR_LATEX(
    				lineEdit_clients_details_quartier->text()));

    texDocument.replace("YEROTHCLIENTVILLE", lineEdit_clients_details_ville->text_LATEX());

    texDocument.replace("YEROTHCLIENTETATOUPROVINCE", lineEdit_clients_details_province_etat->text_LATEX());

    texDocument.replace("YEROTHCLIENTPAYS", lineEdit_clients_details_pays->text_LATEX());

    texDocument.replace("YEROTHSERVICEGESTIONRELATIONCLIENTELLE",
    		infoEntreprise.get_CARTE_DE_FIDELITE_service_de_gestion_clientele_FOR_LATEX());

    texDocument.replace("YEROTHSERVICEDUPROGRAMMEDEFIDELITECLIENTS",
    		infoEntreprise.get_CARTE_DE_FIDELITE_service_du_programme_de_fidelite_clients_FOR_LATEX());

    texDocument.replace("YEROTHSIEGEDUSERVICEGESTIONRELATIONCLIENTELLE",
    		infoEntreprise.get_CARTE_DE_FIDELITE_siege_FOR_LATEX());

    texDocument.replace("YEROTHEMAILDUSERVICEGESTIONRELATIONCLIENTELLE",
    		infoEntreprise.get_CARTE_DE_FIDELITE_email_FOR_LATEX());

    texDocument.replace("YEROTHTELEPHONEDUSERVICEGESTIONRELATIONCLIENTELLE",
    		infoEntreprise.get_CARTE_DE_FIDELITE_telephone_FOR_LATEX());

    QString yerothPrefixFileName;

    yerothPrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir(latexFileNamePrefix));

	//qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex")
    					.arg(yerothPrefixFileName));

    emit SIGNAL_INCREMENT_PROGRESS_BAR(48);

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile, texDocument);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(96);

    QString pdfCustomerDataFileName(YerothERPProcess::compileWITH_LUATEX_Latex(yerothPrefixFileName));

    if (pdfCustomerDataFileName.isEmpty())
    {
    	return false;
    }

//    YEROTH_DELETE_FREE_POINTER_NOW(a_label_company_logo_pixmap)


    YerothERPProcess::startPdfViewerProcess(pdfCustomerDataFileName);


    emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

    return true;
}


void YerothClientsDetailWindow::afficher_groupes_dun_client()
{
	rendreInvisible();

	_allWindows->_groupesDunClientWindow->rendreVisible(_curClientTableModel,
														_curStocksTableModel);
}


void YerothClientsDetailWindow::private_payer_au_compteclient()
{
	rendreInvisible();

	_allWindows->_payerAuCompteClientWindow->rendreVisible(_curClientTableModel,
														   _curStocksTableModel);
}


void YerothClientsDetailWindow::modifierCompteClient()
{
    rendreInvisible();

    _allWindows->_modifierCompteClientWindow->rendreVisible(_curClientTableModel,
															_curStocksTableModel);
}


void YerothClientsDetailWindow::supprimerCompteClient()
{
	QSqlRecord record;

	_allWindows->_clientsWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString msgSupprimer(QObject::trUtf8("Poursuivre avec la suppression du compte client '%1' ?")
    						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                       QObject::tr("suppression d'un compte client"), msgSupprimer,
                                       QMessageBox::Cancel, QMessageBox::Ok))
    {
    	bool resRemoved = _allWindows->_clientsWindow->
    			SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW();

        //qDebug() << "YerothModifierWindow::supprimer_ce_stock() " << resRemoved;
        clients();
        if (resRemoved)
        {
            msgSupprimer = QObject::trUtf8("Le compte client '%1' a été supprimé !")
            					.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("supprimer - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer = QObject::trUtf8("Le compte client '%1' ne pouvait pas être supprimé !")
            					.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("supprimer - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerothClientsDetailWindow::setupLineEdits()
{
	lineEdit_clients_details_reference_registre_du_commerce->setYerothEnabled(false);
	lineEdit_clients_details_reference_client->setYerothEnabled(false);
	lineEdit_clients_details_nom_entreprise->setYerothEnabled(false);
	lineEdit_clients_details_nom_representant->setYerothEnabled(false);
	lineEdit_clients_details_quartier->setYerothEnabled(false);
	lineEdit_clients_details_ville->setYerothEnabled(false);
	lineEdit_clients_details_province_etat->setYerothEnabled(false);
	lineEdit_clients_details_pays->setYerothEnabled(false);
	lineEdit_clients_details_boite_postale->setYerothEnabled(false);
	lineEdit_clients_details_siege_social->setYerothEnabled(false);
	lineEdit_clients_details_email->setYerothEnabled(false);
	lineEdit_clients_details_numero_telephone_1->setYerothEnabled(false);
	lineEdit_clients_details_numero_telephone_2->setYerothEnabled(false);
	lineEdit_clients_details_refereur->setYerothEnabled(false);
	lineEdit_clients_details_numero_contribuable->setYerothEnabled(false);
	lineEdit_clients_details_dette_maximale->setYerothEnabled(false);
	lineEdit_clients_details_compte_client->setYerothEnabled(false);
}


void YerothClientsDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGenererCARTE_DE_FIDELITE_ClIENTS, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_groupes_du_client->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_payer_au_compteclient->disable(this);
}

void YerothClientsDetailWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole");

    definirPasDeRole();
}

void YerothClientsDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGenererCARTE_DE_FIDELITE_ClIENTS, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_groupes_du_client->enable(this, SLOT(afficher_groupes_dun_client()));
    pushButton_modifier->enable(this, SLOT(modifierCompteClient()));
    pushButton_supprimer->enable(this, SLOT(supprimerCompteClient()));

    pushButton_payer_au_compteclient->enable(this, SLOT(private_payer_au_compteclient()));

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif
}


void YerothClientsDetailWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGenererCARTE_DE_FIDELITE_ClIENTS, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_groupes_du_client->enable(this, SLOT(afficher_groupes_dun_client()));
    pushButton_modifier->enable(this, SLOT(modifierCompteClient()));
    pushButton_supprimer->disable(this);

    pushButton_payer_au_compteclient->enable(this, SLOT(private_payer_au_compteclient()));
}


void YerothClientsDetailWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole");

    definirPasDeRole();
}

void YerothClientsDetailWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole");

    definirPasDeRole();
}


bool YerothClientsDetailWindow::imprimer_pdf_document()
{
    _logger->log("imprimer_pdf_document");

    QString yerothCustomerAccountImage("yeroth");

    QString yerothCustomerAccountImageTmpFile(QString("%1JPG")
    		.arg(YerothUtils::getUniquePrefixFileInTemporaryFilesDir(yerothCustomerAccountImage)));

    const QPixmap *label_image_produit_pixmap = label_image_produit->pixmap();

    if (0 != label_image_produit_pixmap)
    {
        YerothUtils::savePixmapToFile(yerothCustomerAccountImageTmpFile,
        							  *label_image_produit_pixmap,
                                      "JPG");
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(12);

    QString latexFileNamePrefix("yeroth-erp-fiche-client");

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-customer-account-file");
#endif

    QString texDocument;

    YerothUtils::getLatexCustomerData(texDocument);

    QString data;

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Référence client: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_reference_client->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Nom de l'entreprise: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_nom_entreprise->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Référeur: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_refereur->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Nom du Représentant: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_nom_representant->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Quartier: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_quartier->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Ville: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_ville->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Province / État: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_province_etat->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Pays: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_pays->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Boîte postale: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_boite_postale->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Siège social: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_siege_social->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Émail: ")));
    data.append(QString("%1\\\\\n").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(lineEdit_clients_details_email->text().toLower())));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Numéro de téléphone 1: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_numero_telephone_1->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Numéro de téléphone 2: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_numero_telephone_2->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Numéro de contribuable: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_numero_contribuable->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("RCCM N\\textsuperscript{o}: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_reference_registre_du_commerce->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Dette maximale: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_dette_maximale->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Compte client: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_compte_client->text_LATEX()));

    data.append("\n\n\\vspace{0.3cm}\n\n");

    data.append(QString("%1").arg(YerothUtils::get_latex_bold_text(QObject::tr("DESCRIPTION CLIENT:"))));
    data.append("\n\n\\vspace{0.3cm}\n\n");

    texDocument.replace("YEROTHDETAILSCOMPTECLIENT", data);

    data.clear();
    data.append(QString("%1\\\\").arg(textEdit_client_details_description_du_client->toPlainTextForLatex()));

    texDocument.replace("YEROTHDESCRIPTIONCOMPTECLIENT", data);

    if (0 != label_image_produit_pixmap)
    {
    	texDocument.replace("YEROTHCHEMINCOMPLETIMAGECOMPTECLIENT", yerothCustomerAccountImageTmpFile);
    }
    else
    {
    	texDocument.replace("YEROTHCHEMINCOMPLETIMAGECOMPTECLIENT", "");
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(24);

    YerothInfoEntreprise & infoEntreprise = YerothUtils::getAllWindows()->getInfoEntreprise();

    QString fileDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHCLIENT", QString("%1").arg(lineEdit_clients_details_nom_entreprise->text_LATEX()));
    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", fileDate);
    texDocument.replace("YEROTHNOMUTILISATEUR", _allWindows->getUser()->nom_completTex());
    texDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());

    QString yerothPrefixFileName;

    yerothPrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir(latexFileNamePrefix));

	//qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex")
    					.arg(yerothPrefixFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile, texDocument);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(92);

    QString pdfCustomerDataFileName(YerothERPProcess::compileLatex(yerothPrefixFileName));

    if (pdfCustomerDataFileName.isEmpty())
    {
    	return false;
    }

    YerothERPProcess::startPdfViewerProcess(pdfCustomerDataFileName);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

    return true;
}


void YerothClientsDetailWindow::rendreInvisible()
{
	lineEdit_clients_details_reference_registre_du_commerce->clear();
	lineEdit_clients_details_reference_client->clear();
	lineEdit_clients_details_nom_entreprise->clear();
	lineEdit_clients_details_nom_representant->clear();
	lineEdit_clients_details_quartier->clear();
	lineEdit_clients_details_ville->clear();
	lineEdit_clients_details_province_etat->clear();
	lineEdit_clients_details_pays->clear();
	lineEdit_clients_details_boite_postale->clear();
	lineEdit_clients_details_siege_social->clear();
	lineEdit_clients_details_email->clear();
	lineEdit_clients_details_numero_telephone_1->clear();
	lineEdit_clients_details_numero_telephone_2->clear();
	lineEdit_clients_details_refereur->clear();
	lineEdit_clients_details_numero_contribuable->clear();
	lineEdit_clients_details_dette_maximale->clear();
	lineEdit_clients_details_compte_client->clear();

	textEdit_client_details_description_du_client->clear();

	YerothUtils::supprimer_image(*label_image_produit);

    YerothWindowsCommons::rendreInvisible();
}


void YerothClientsDetailWindow::rendreVisible(YerothSqlTableModel * clientTableModel,
											  YerothSqlTableModel * stocksTableModel)
{
	_curStocksTableModel = stocksTableModel;

	_curClientTableModel = clientTableModel;

    //qDebug() << "++ last selected row: " << YerothERPWindows::get_last_lister_selected_row_ID();

	setVisible(true);

    showClientDetail();
}


void YerothClientsDetailWindow::showClientDetail()
{
	QSqlRecord record;

	_allWindows->_clientsWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);


    QString groupes_du_client(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::GROUPES_DU_CLIENT));


    _curClientGroups.clear();

    _curClientGroups.append(groupes_du_client.split(YerothUtils::STAR_CHAR));


	lineEdit_clients_details_reference_client->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_CLIENT));

	lineEdit_clients_details_reference_registre_du_commerce->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

	lineEdit_clients_details_nom_entreprise->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

	lineEdit_clients_details_nom_representant->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_REPRESENTANT));

	lineEdit_clients_details_quartier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUARTIER));

	lineEdit_clients_details_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));

	lineEdit_clients_details_province_etat->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PROVINCE_ETAT));

	lineEdit_clients_details_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));

	lineEdit_clients_details_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));

	lineEdit_clients_details_siege_social->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::SIEGE_SOCIAL));

	lineEdit_clients_details_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));

	lineEdit_clients_details_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

	lineEdit_clients_details_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));

	lineEdit_clients_details_refereur->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFEREUR_CLIENT));

	lineEdit_clients_details_numero_contribuable->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));

	double dette_maximale = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

	lineEdit_clients_details_dette_maximale->setText(GET_CURRENCY_STRING_NUM(dette_maximale));

	double compte_client = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

	lineEdit_clients_details_compte_client->setText(GET_CURRENCY_STRING_NUM(compte_client));

	textEdit_client_details_description_du_client->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_CLIENT));

    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_COMPTE_CLIENT));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
    	YerothUtils::supprimer_image(*label_image_produit);
    }
}


void YerothClientsDetailWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
}

