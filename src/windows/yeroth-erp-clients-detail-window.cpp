/*
 * yeroth-erp-clients-detail-window.cpp
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "src/windows/yeroth-erp-clients-detail-window.hpp"

#include "src/yeroth-erp-windows.hpp"

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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_paiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_transactions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_ventes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_menu->disable(this);
    pushButton_transactions->disable(this);
    pushButton_ventes->disable(this);
    pushButton_retour->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficher_les_clients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAfficher_les_paiements, SIGNAL(triggered()), this, SLOT(paiements()));
    connect(actionAfficher_les_transactions, SIGNAL(triggered()), this, SLOT(transactions()));
    connect(actionAfficher_les_ventes, SIGNAL(triggered()), this, SLOT(ventes()));
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

void YerothClientsDetailWindow::setupLineEdits()
{
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_paiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_transactions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_ventes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_menu->disable(this);
    pushButton_transactions->disable(this);
    pushButton_ventes->disable(this);
    pushButton_retour->disable(this);
}

void YerothClientsDetailWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_paiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_transactions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_ventes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_menu->disable(this);
    pushButton_transactions->disable(this);
    pushButton_ventes->disable(this);
    pushButton_retour->disable(this);
}

void YerothClientsDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_paiements, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_transactions, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_ventes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_transactions->enable(this, SLOT(transactions()));
    pushButton_ventes->enable(this, SLOT(ventes()));

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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_paiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_transactions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_ventes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_menu->disable(this);
    pushButton_transactions->disable(this);
    pushButton_ventes->disable(this);
    pushButton_retour->disable(this);
}


void YerothClientsDetailWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_paiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_transactions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_ventes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_menu->disable(this);
    pushButton_transactions->disable(this);
    pushButton_ventes->disable(this);
    pushButton_retour->disable(this);
}

void YerothClientsDetailWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_paiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_transactions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_ventes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_menu->disable(this);
    pushButton_transactions->disable(this);
    pushButton_ventes->disable(this);
    pushButton_retour->disable(this);
}


void YerothClientsDetailWindow::modifier_les_articles()
{
    this->rendreInvisible();
    _allWindows->_modifierWindow->rendreVisible(_curStocksTableModel);
}

void YerothClientsDetailWindow::rendreInvisible()
{
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
}

void YerothClientsDetailWindow::supprimer_ce_stock()
{
    QSqlRecord record = _curStocksTableModel->record(_allWindows->getLastSelectedListerRow());
    QString msgSupprimer("Poursuivre avec la suppression de l'article \"");
    msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    msgSupprimer.append("\" ?");
    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                       QObject::trUtf8
                                       ("suppression d'un article"), msgSupprimer,
                                       QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool resRemoved = _curStocksTableModel->removeRow(_allWindows->getLastSelectedListerRow());
        //qDebug() << "YerothModifierWindow::supprimer_ce_stock() " << resRemoved;
        afficherStocks();
        if (resRemoved)
        {
            msgSupprimer.clear();
            msgSupprimer.append("Le stock \"");
            msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
            msgSupprimer.append(QObject::trUtf8("\" a été supprimé."));
            YerothQMessageBox::information(this, "suppression d'un stock avec succès",
                                          msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();
            msgSupprimer.append("Le stock \"");
            msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
            msgSupprimer.append("\" ne pouvait pas être supprimé.");
            YerothQMessageBox::information(this, "échec de la suppression d'un stock",
                                          msgSupprimer);
        }
    }
    else
    {
    }
}
