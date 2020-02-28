/*
 * yeroth-erp-stock-detail-window.cpp
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "src/yeroth-erp-windows.hpp"

#include "src/windows/yeroth-erp-stock-detail-window.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>


const unsigned int YerothStockDetailWindow::INDEX_ZERO(0);

const QString YerothStockDetailWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("détails d'un stock")));


YerothStockDetailWindow::YerothStockDetailWindow():YerothWindowsCommons(YerothStockDetailWindow::_WINDOW_TITLE),
    _logger(new YerothLogger("YerothDetailWindow"))
{
    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0,
                                    		COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    checkBox_achat->setReadOnly(true);

    this->setupLineEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    dateEdit_date_peremption->setYerothEnabled(false);
    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_retour->enable(this, SLOT(afficherStocks()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionModifier, SIGNAL(triggered()), this, SLOT(modifier_les_articles()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer_ce_stock()));
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

    this->setupShortcuts();
}

void YerothStockDetailWindow::setupLineEdits()
{
	lineEdit_localisation_produit->setEnabled(false);
    lineEdit_reference_produit->setEnabled(false);
    lineEdit_designation->setEnabled(false);
    lineEdit_quantite_restante->setEnabled(false);
    lineEdit_reference_recu_dachat->setEnabled(false);
    lineEdit_prix_dachat->setEnabled(false);
    lineEdit_prix_unitaire->setEnabled(false);
    lineEdit_prix_vente->setEnabled(false);
    textEdit_description->setYerothEnabled(false);
    lineEdit_quantite_initiale->setEnabled(false);
    lineEdit_stock_minimum->setEnabled(false);
    lineEdit_categorie_produit->setEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setEnabled(false);
}

void YerothStockDetailWindow::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_detailWindow->palette());
    menu.addAction(actionSupprimer);
    menu.exec(event->globalPos());
}

void YerothStockDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_menu->disable(this);
    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
}


void YerothStockDetailWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
}


void YerothStockDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_modifier->enable(this, SLOT(modifier_les_articles()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->enable(this, SLOT(supprimer_ce_stock()));
}


void YerothStockDetailWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->disable(this);
}


void YerothStockDetailWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_modifier->enable(this, SLOT(modifier_les_articles()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->disable(this);
}

void YerothStockDetailWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->disable(this);
}

void YerothStockDetailWindow::modifier_les_articles()
{
    this->rendreInvisible();
    _allWindows->_modifierWindow->rendreVisible(_curStocksTableModel);
}

void YerothStockDetailWindow::rendreInvisible()
{
	checkBox_achat->setChecked(true);
    lineEdit_reference_produit->clear();
    lineEdit_designation->clear();
    lineEdit_categorie_produit->clear();
    lineEdit_quantite_restante->clear();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_prix_dachat->clear();
    lineEdit_prix_unitaire->clear();
    lineEdit_prix_vente->clear();
    textEdit_description->clear();
    lineEdit_localisation_produit->clear();
    label_image_produit->clear();

    YerothWindowsCommons::rendreInvisible();
}


void YerothStockDetailWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;
    //qDebug() << "++ last selected row: " << _allWindows->getLastSelectedListerRow();
    this->showItem();
    this->setVisible(true);
}


void YerothStockDetailWindow::showItem()
{
    QSqlRecord record = _curStocksTableModel->record(_allWindows->getLastSelectedListerRow());

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

    textEdit_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_PRODUIT));


    double quantite_restante = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();

    lineEdit_quantite_restante->setText(GET_DOUBLE_STRING_P(quantite_restante, 0));

    double quantite_minimale_en_stock = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::STOCK_MINIMUM).toDouble();

    lineEdit_stock_minimum->setText(GET_DOUBLE_STRING_P(quantite_minimale_en_stock, 0));
    dateEdit_date_peremption->setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_PEREMPTION)));

    int lots_entrant = GET_SQL_RECORD_DATA(record, "lots_entrant").toInt();

    double quantite_par_lot = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_PAR_LOT).toDouble();

    double quantite_initiale = lots_entrant * quantite_par_lot;

    lineEdit_quantite_initiale->setText(GET_DOUBLE_STRING_P(quantite_initiale, 0));

    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_PRODUIT));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }

    QString recordID = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID);

    QString strAchatsQuery(QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
    							.arg(YerothDatabaseTableColumn::ID,
    								 _allWindows->ACHATS,
    								 YerothDatabaseTableColumn::STOCKS_ID,
									 recordID));

    QSqlQuery query;

    int achatQuerySize = YerothUtils::execQuery(query, strAchatsQuery, _logger);

//    qDebug() << QString("++ strAchatsQuery: %1, querySize: %2")
//    				.arg(strAchatsQuery,
//    					 QString::number(achatQuerySize));

    if (1 == achatQuerySize)
    {
    	checkBox_achat->setChecked(true);
    }
    else
    {
    	checkBox_achat->setChecked(false);
    }
}


void YerothStockDetailWindow::setupShortcuts()
{
    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothStockDetailWindow::supprimer_ce_stock()
{
    QSqlRecord record = _curStocksTableModel->record(_allWindows->getLastSelectedListerRow());
    QString msgSupprimer(QString(QObject::trUtf8("Poursuivre avec la suppression de l'article '%1' ?"))
    						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr("suppression d'un article"), msgSupprimer,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool resRemoved = _curStocksTableModel->removeRow(_allWindows->getLastSelectedListerRow());
        //qDebug() << "YerothModifierWindow::supprimer_ce_stock() " << resRemoved;
        afficherStocks();
        if (resRemoved)
        {
            msgSupprimer.clear();
            msgSupprimer.append(QString(QObject::trUtf8("Le stock '%1' a été supprimé !"))
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("suppression d'un stock avec succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();
            msgSupprimer.append(QString(QObject::trUtf8("Le stock '%1' ne pouvait pas être supprimé !"))
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("échec de la suppression d'un stock"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}
