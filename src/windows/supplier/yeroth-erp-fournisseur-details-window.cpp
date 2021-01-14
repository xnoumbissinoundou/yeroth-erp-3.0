/*
 * yeroth-erp-fournisseur-details-window.cpp
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-fournisseur-details-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


YerothFournisseurDetailsWindow::YerothFournisseurDetailsWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothFournisseurDetailsWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("détails d'un fournisseur"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                   .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                                		COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();

    textEdit_fournisseur_details_description_du_fournisseur->setYerothEnabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_payer_au_fournisseur->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
    connect(actionFournisseurs, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionModifierFournisseur, SIGNAL(triggered()), this, SLOT(modifierFournisseur()));
    connect(actionSupprimerFournisseur, SIGNAL(triggered()), this, SLOT(supprimerFournisseur()));
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


void YerothFournisseurDetailsWindow::private_payer_au_fournisseur()
{
	rendreInvisible();

	_allWindows->_payerAuFournisseurWindow->rendreVisible(_curFournisseurTableModel,
														  _curStocksTableModel);
}


void YerothFournisseurDetailsWindow::modifierFournisseur()
{
    rendreInvisible();

    _allWindows->_modifierFournisseurWindow->rendreVisible(_curFournisseurTableModel,
														   _curStocksTableModel);
}


void YerothFournisseurDetailsWindow::supprimerFournisseur()
{
	QSqlRecord record;

	_allWindows->_fournisseursWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString msgSupprimer(QObject::trUtf8("Poursuivre avec la suppression du compte fournisseur '%1' ?")
    						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                       QObject::tr("suppression"), msgSupprimer,
                                       QMessageBox::Cancel, QMessageBox::Ok))
    {
    	bool resRemoved = _allWindows->_fournisseursWindow->
    			SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW();

        //qDebug() << "YerothModifierWindow::supprimer_ce_stock() " << resRemoved;
        fournisseurs();
        if (resRemoved)
        {
            msgSupprimer = QObject::trUtf8("Le compte fournisseur '%1' a été supprimé !")
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("suppression - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer = QObject::trUtf8("Le compte fournisseur '%1' ne pouvait pas être supprimé !")
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("suppression - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerothFournisseurDetailsWindow::setupLineEdits()
{
	lineEdit_fournisseur_details_reference_registre_du_commerce->setYerothEnabled(false);
	lineEdit_fournisseur_details_reference_fournisseur->setYerothEnabled(false);
	lineEdit_fournisseur_details_nom_entreprise->setYerothEnabled(false);
	lineEdit_fournisseur_details_nom_representant->setYerothEnabled(false);
	lineEdit_fournisseur_details_quartier->setYerothEnabled(false);
	lineEdit_fournisseur_details_ville->setYerothEnabled(false);
	lineEdit_fournisseur_details_province_etat->setYerothEnabled(false);
	lineEdit_fournisseur_details_pays->setYerothEnabled(false);
	lineEdit_fournisseur_details_boite_postale->setYerothEnabled(false);
	lineEdit_fournisseur_details_siege_social->setYerothEnabled(false);
	lineEdit_fournisseur_details_email->setYerothEnabled(false);
	lineEdit_fournisseur_details_numero_telephone_1->setYerothEnabled(false);
	lineEdit_fournisseur_details_numero_telephone_2->setYerothEnabled(false);
	lineEdit_fournisseur_details_numero_contribuable->setYerothEnabled(false);
	lineEdit_fournisseur_details_dette_maximale->setYerothEnabled(false);
	lineEdit_fournisseur_details_fournisseur->setYerothEnabled(false);
}


void YerothFournisseurDetailsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_payer_au_fournisseur->disable(this);
}

void YerothFournisseurDetailsWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_payer_au_fournisseur->disable(this);
}

void YerothFournisseurDetailsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_modifier->enable(this, SLOT(modifierFournisseur()));
    pushButton_supprimer->enable(this, SLOT(supprimerFournisseur()));

    pushButton_payer_au_fournisseur->enable(this, SLOT(private_payer_au_fournisseur()));

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif
}


void YerothFournisseurDetailsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_modifier->enable(this, SLOT(modifierFournisseur()));
    pushButton_supprimer->enable(this, SLOT(supprimerFournisseur()));
    pushButton_payer_au_fournisseur->enable(this, SLOT(private_payer_au_fournisseur()));
}


void YerothFournisseurDetailsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_payer_au_fournisseur->disable(this);
}

void YerothFournisseurDetailsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_payer_au_fournisseur->disable(this);
}


bool YerothFournisseurDetailsWindow::imprimer_pdf_document()
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

    QString latexFileNamePrefix("yeroth-erp-liste-fournisseur");

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-supplier-file");
#endif

    QString texDocument;

    YerothUtils::getLatexSupplierData(texDocument);

    QString data;

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Référence fournisseur: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_reference_fournisseur->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Nom de l'entreprise: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_nom_entreprise->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Nom du Représentant: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_nom_representant->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Quartier: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_quartier->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Ville: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_ville->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Province / État: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_province_etat->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Pays: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_pays->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Boîte postale: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_boite_postale->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Siège social: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_siege_social->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Émail: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_email->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Numéro de téléphone 1: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_numero_telephone_1->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Numéro de téléphone 2: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_numero_telephone_2->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Numéro de contribuable: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_numero_contribuable->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("RCCM N\\textsuperscript{o}: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_reference_registre_du_commerce->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Dette maximale: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_dette_maximale->textForLatex()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Fournisseur: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_fournisseur_details_fournisseur->textForLatex()));

    data.append("\n\n\\vspace{0.3cm}\n\n");

    data.append(QString("%1").arg(YerothUtils::get_latex_bold_text(QObject::tr("DESCRIPTION FOURNISSEUR:"))));
    data.append("\n\n\\vspace{0.3cm}\n\n");

    texDocument.replace("YEROTHDETAILSFOURNISSEUR", data);

    data.clear();
    data.append(QString("%1\\\\").arg(textEdit_fournisseur_details_description_du_fournisseur->toPlainTextForLatex()));

    texDocument.replace("YEROTHDESCRIPTIONFOURNISSEUR", data);

    if (0 != label_image_produit_pixmap)
    {
    	texDocument.replace("YEROTHCHEMINCOMPLETIMAGEFOURNISSEUR", yerothCustomerAccountImageTmpFile);
    }
    else
    {
    	texDocument.replace("YEROTHCHEMINCOMPLETIMAGEFOURNISSEUR", "");
    }


    YerothInfoEntreprise & infoEntreprise = YerothUtils::getAllWindows()->getInfoEntreprise();

    QString fileDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVilleTex()));

    YerothUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHFOURNISSEUR", QString("%1").arg(lineEdit_fournisseur_details_nom_entreprise->textForLatex()));
    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
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

    QString pdfCustomerDataFileName(YerothERPProcess::compileLatex(yerothPrefixFileName));

    if (pdfCustomerDataFileName.isEmpty())
    {
    	return false;
    }

    YerothERPProcess::startPdfViewerProcess(pdfCustomerDataFileName);

    return true;
}


void YerothFournisseurDetailsWindow::rendreInvisible()
{
	lineEdit_fournisseur_details_reference_registre_du_commerce->clear();
	lineEdit_fournisseur_details_reference_fournisseur->clear();
	lineEdit_fournisseur_details_nom_entreprise->clear();
	lineEdit_fournisseur_details_nom_representant->clear();
	lineEdit_fournisseur_details_quartier->clear();
	lineEdit_fournisseur_details_ville->clear();
	lineEdit_fournisseur_details_province_etat->clear();
	lineEdit_fournisseur_details_pays->clear();
	lineEdit_fournisseur_details_boite_postale->clear();
	lineEdit_fournisseur_details_siege_social->clear();
	lineEdit_fournisseur_details_email->clear();
	lineEdit_fournisseur_details_numero_telephone_1->clear();
	lineEdit_fournisseur_details_numero_telephone_2->clear();
	lineEdit_fournisseur_details_numero_contribuable->clear();
	lineEdit_fournisseur_details_dette_maximale->clear();
	lineEdit_fournisseur_details_fournisseur->clear();

	textEdit_fournisseur_details_description_du_fournisseur->clear();

    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);

    YerothWindowsCommons::rendreInvisible();
}


void YerothFournisseurDetailsWindow::rendreVisible(YerothSqlTableModel * fournisseurTableModel,
												   YerothSqlTableModel * stocksTableModel)
{
	_curStocksTableModel = stocksTableModel;

	_curFournisseurTableModel = fournisseurTableModel;

    //qDebug() << "++ last selected row: " << _allWindows->getLastSelectedListerRow();

	setVisible(true);

    showFournisseurDetail();
}


void YerothFournisseurDetailsWindow::showFournisseurDetail()
{
	QSqlRecord record;

	_allWindows->_fournisseursWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

	lineEdit_fournisseur_details_reference_fournisseur->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR));

	lineEdit_fournisseur_details_reference_registre_du_commerce->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

	lineEdit_fournisseur_details_nom_entreprise->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

	lineEdit_fournisseur_details_nom_representant->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_REPRESENTANT));

	lineEdit_fournisseur_details_quartier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUARTIER));

	lineEdit_fournisseur_details_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));

	lineEdit_fournisseur_details_province_etat->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PROVINCE_ETAT));

	lineEdit_fournisseur_details_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));

	lineEdit_fournisseur_details_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));

	lineEdit_fournisseur_details_siege_social->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::SIEGE_SOCIAL));

	lineEdit_fournisseur_details_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));

	lineEdit_fournisseur_details_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

	lineEdit_fournisseur_details_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));

	lineEdit_fournisseur_details_numero_contribuable->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));

	double dette_maximale = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

	lineEdit_fournisseur_details_dette_maximale->setText(GET_CURRENCY_STRING_NUM(dette_maximale));

	double fournisseur = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

	lineEdit_fournisseur_details_fournisseur->setText(GET_CURRENCY_STRING_NUM(fournisseur));

	textEdit_fournisseur_details_description_du_fournisseur->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR));

    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_FOURNISSEUR));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }
}


void YerothFournisseurDetailsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
}

