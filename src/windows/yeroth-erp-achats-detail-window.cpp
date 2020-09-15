/*
 * yeroth-erp-achats-detail-window.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#include "src/yeroth-erp-windows.hpp"

#include "src/windows/yeroth-erp-achats-detail-window.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include <QtSql/QSqlRecord>


YerothAchatsDetailWindow::YerothAchatsDetailWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothAchatsDetailWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("détails d'un achat de stock"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_DARK_GRAY_60_60_60,
                            	   COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    dateEdit_date_peremption->setYerothEnabled(false);

    pushButton_entrer->disable(this);
    pushButton_lister->disable(this);
    pushButton_menu->disable(this);
    pushButton_retour->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(afficherStocks()));
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

void YerothAchatsDetailWindow::setupLineEdits()
{
	lineEdit_localisation_produit->setYerothEnabled(false);
    lineEdit_reference_produit->setYerothEnabled(false);
    lineEdit_designation->setYerothEnabled(false);
    lineEdit_quantite_restante->setYerothEnabled(false);
    lineEdit_reference_recu_dachat->setYerothEnabled(false);
    lineEdit_prix_dachat->setYerothEnabled(false);
    lineEdit_prix_unitaire->setYerothEnabled(false);
    lineEdit_prix_vente->setYerothEnabled(false);
    lineEdit_categorie_produit->setYerothEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setYerothEnabled(false);
}


void YerothAchatsDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_menu->disable(this);
    pushButton_entrer->disable(this);
    pushButton_lister->disable(this);
    pushButton_retour->disable(this);
}


void YerothAchatsDetailWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->disable(this);
    pushButton_lister->enable(this, SLOT(afficherStocks()));
    pushButton_menu->disable(this);
    pushButton_retour->disable(this);
}

void YerothAchatsDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_lister->enable(this, SLOT(afficherStocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->enable(this, SLOT(achats()));
}


void YerothAchatsDetailWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_lister->enable(this, SLOT(afficherStocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->enable(this, SLOT(achats()));
}


void YerothAchatsDetailWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_lister->enable(this, SLOT(afficherStocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->disable(this);
}

void YerothAchatsDetailWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->disable(this);
    pushButton_lister->enable(this, SLOT(afficherStocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->disable(this);
}


void YerothAchatsDetailWindow::rendreInvisible()
{
    lineEdit_reference_produit->clear();
    lineEdit_designation->clear();
    lineEdit_categorie_produit->clear();
    lineEdit_quantite_restante->clear();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_reference_recu_dachat->clear();
    lineEdit_prix_dachat->clear();
    lineEdit_prix_unitaire->clear();
    lineEdit_prix_vente->clear();
    lineEdit_localisation_produit->clear();

    YerothWindowsCommons::rendreInvisible();
}


void YerothAchatsDetailWindow::rendreVisible(int lastSelectedRow,
											 YerothSqlTableModel * stocksTableModel,
											 YerothSqlTableModel * achatStocksTableModel)
{
	_achatLastSelectedRow = lastSelectedRow;

	_curStocksTableModel = stocksTableModel;

	_curAchatStocksTableModel = achatStocksTableModel;

    setVisible(true);

    //qDebug() << "++ last selected row: " << _allWindows->getLastSelectedListerRow();
    showItem(lastSelectedRow);
}


void YerothAchatsDetailWindow::showItem(int lastSelectedRow)
{
    QSqlRecord record = _curAchatStocksTableModel->record(lastSelectedRow);

    lineEdit_reference_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));
    lineEdit_designation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    lineEdit_categorie_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));
    lineEdit_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

    double prix_unitaire = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_UNITAIRE).toDouble();

    lineEdit_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(prix_unitaire));

    double prix_dachat = 0.0;

    YerothPOSUser *currentUser = YerothUtils::getAllWindows()->getUser();

    if (0 != currentUser)
    {
    	if (currentUser->isManager() ||
    		currentUser->isGestionaireDesStocks())
    	{
    		prix_dachat = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_DACHAT).toDouble();
    	}
    }

    lineEdit_reference_recu_dachat->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT));

    lineEdit_prix_dachat->setText(GET_CURRENCY_STRING_NUM(prix_dachat));

    double prix_vente = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_VENTE).toDouble();

    lineEdit_prix_vente->setText(GET_CURRENCY_STRING_NUM(prix_vente));

    lineEdit_localisation_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LOCALISATION_STOCK));

    double quantite_restante = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_TOTALE).toDouble();

    lineEdit_quantite_restante->setText(GET_DOUBLE_STRING_P(quantite_restante, 0));

    dateEdit_date_peremption->setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_PEREMPTION)));
}


void YerothAchatsDetailWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}
