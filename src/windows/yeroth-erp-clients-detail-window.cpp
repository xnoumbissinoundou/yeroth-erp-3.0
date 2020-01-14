/*
 * yeroth-erp-clients-detail-window.cpp
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "src/windows/yeroth-erp-clients-detail-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


const unsigned int YerothClientsDetailWindow::INDEX_ZERO(0);

const QString YerothClientsDetailWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("détails d'un client")));


YerothClientsDetailWindow::YerothClientsDetailWindow()
:YerothWindowsCommons(YerothClientsDetailWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothClientsDetailWindow"))
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                            	   COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);

    setupLineEdits();

    textEdit_client_details_description_du_client->setEnabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_clients->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_retour->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_document()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionModifierCompteClient, SIGNAL(triggered()), this, SLOT(modifierCompteClient()));
    connect(actionSupprimerCompteClient, SIGNAL(triggered()), this, SLOT(supprimerCompteClient()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#endif

    setupShortcuts();
}


void YerothClientsDetailWindow::modifierCompteClient()
{
    rendreInvisible();

    _allWindows->_modifierCompteClientWindow->rendreVisible(_clientLastSelectedRow,
    												 	 	_curClientTableModel,
															_curStocksTableModel);
}


void YerothClientsDetailWindow::supprimerCompteClient()
{
    QSqlRecord record = _curClientTableModel->record(_clientLastSelectedRow);

    QString msgSupprimer(QString(QObject::trUtf8("Poursuivre avec la suppression du compte client '%1' ?"))
    						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                       QObject::tr("suppression d'un compte client"), msgSupprimer,
                                       QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool resRemoved = _curClientTableModel->removeRow(_allWindows->getLastSelectedListerRow());
        //qDebug() << "YerothModifierWindow::supprimer_ce_stock() " << resRemoved;
        clients();
        if (resRemoved)
        {
            msgSupprimer.clear();
            msgSupprimer.append(QString(QObject::trUtf8("Le compte client '%1' a été supprimé !"))
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE)));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("suppression d'un compte client avec succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();
            msgSupprimer.append(QString(QObject::trUtf8("Le compte client '%1' ne pouvait pas être supprimé !"))
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE)));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("échec de la suppression d'un compte client"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerothClientsDetailWindow::setupLineEdits()
{
	lineEdit_clients_details_reference_registre_du_commerce->setEnabled(false);
	lineEdit_clients_details_reference_client->setEnabled(false);
	lineEdit_clients_details_nom_entreprise->setEnabled(false);
	lineEdit_clients_details_nom_representant->setEnabled(false);
	lineEdit_clients_details_quartier->setEnabled(false);
	lineEdit_clients_details_ville->setEnabled(false);
	lineEdit_clients_details_province_etat->setEnabled(false);
	lineEdit_clients_details_pays->setEnabled(false);
	lineEdit_clients_details_boite_postale->setEnabled(false);
	lineEdit_clients_details_siege_social->setEnabled(false);
	lineEdit_clients_details_email->setEnabled(false);
	lineEdit_clients_details_numero_telephone_1->setEnabled(false);
	lineEdit_clients_details_numero_telephone_2->setEnabled(false);
	lineEdit_clients_details_numero_contribuable->setEnabled(false);
	lineEdit_clients_details_dette_maximale->setEnabled(false);
	lineEdit_clients_details_compte_client->setEnabled(false);
}


void YerothClientsDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_retour->disable(this);
}

void YerothClientsDetailWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_retour->disable(this);
}

void YerothClientsDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_modifier->enable(this, SLOT(modifierCompteClient()));
    pushButton_supprimer->enable(this, SLOT(supprimerCompteClient()));

    pushButton_retour->enable(this, SLOT(clients()));

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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->enable(this, SLOT(modifierCompteClient()));
    pushButton_supprimer->enable(this, SLOT(supprimerCompteClient()));
    pushButton_retour->disable(this);
}


void YerothClientsDetailWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_retour->disable(this);
}

void YerothClientsDetailWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_retour->disable(this);
}


bool YerothClientsDetailWindow::imprimer_document()
{
    _logger->log("imprimer_document");

    QString latexFileNamePrefix("yeroth-erp-fiche-client");

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-customer-account-file");
#endif

    QString texDocument;

    YerothUtils::getLatexCustomerData(texDocument);

    QString data;

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Référence client: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_reference_client->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Nom de l'entreprise: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_nom_entreprise->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Nom du Représentant: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_nom_representant->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Quartier: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_quartier->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Ville: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_ville->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Province / État: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_province_etat->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Pays: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_pays->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Boîte postale: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_boite_postale->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Siège social: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_siege_social->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Émail: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_email->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Numéro de téléphone 1: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_numero_telephone_1->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Numéro de téléphone 2: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_numero_telephone_2->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Numéro de contribuable: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_numero_contribuable->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("RCCN N\\textsuperscript{o}: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_reference_registre_du_commerce->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::trUtf8("Dette maximale: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_dette_maximale->text()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Compte client: ")));
    data.append(QString("%1\\\\\n").arg(lineEdit_clients_details_compte_client->text()));

    data.append("\n\n\\vspace{0.3cm}\n\n");

    data.append(QString("%1").arg(YerothUtils::get_latex_bold_text(QObject::tr("DESCRIPTION CLIENT:"))));
    data.append("\n\n\\vspace{0.3cm}\n\n");

    data.append(QString("%1\\\\").arg(textEdit_client_details_description_du_client->toPlainText()));

    texDocument.replace("CONTENU", data);

    YerothInfoEntreprise & infoEntreprise = YerothUtils::getAllWindows()->getInfoEntreprise();

    QString fileDate(YerothUtils::handleForeignAccents(infoEntreprise.getVilleTex()));

    YerothUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHCLIENT", QString("%1").arg(lineEdit_clients_details_nom_entreprise->text()));
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
	lineEdit_clients_details_numero_contribuable->clear();
	lineEdit_clients_details_dette_maximale->clear();
	lineEdit_clients_details_compte_client->clear();

	textEdit_client_details_description_du_client->clear();

    YerothWindowsCommons::rendreInvisible();
}


void YerothClientsDetailWindow::rendreVisible(int lastSelectedRow,
											  YerothSqlTableModel * clientTableModel,
											  YerothSqlTableModel * stocksTableModel)
{
	_clientLastSelectedRow = lastSelectedRow;

	_curStocksTableModel = stocksTableModel;

	_curClientTableModel = clientTableModel;

    //qDebug() << "++ last selected row: " << _allWindows->getLastSelectedListerRow();
    showClientDetail(lastSelectedRow);

    setVisible(true);
}


void YerothClientsDetailWindow::showClientDetail(int lastSelectedRow)
{
	QSqlRecord record = _curClientTableModel->record(lastSelectedRow);

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

	lineEdit_clients_details_numero_contribuable->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));

	double dette_maximale = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

	lineEdit_clients_details_dette_maximale->setText(GET_CURRENCY_STRING_NUM(dette_maximale));

	double compte_client = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

	lineEdit_clients_details_compte_client->setText(GET_CURRENCY_STRING_NUM(compte_client));

	textEdit_client_details_description_du_client->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_CLIENT));
}


void YerothClientsDetailWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
}

