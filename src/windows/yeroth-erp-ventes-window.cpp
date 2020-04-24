/*
   * yeroth-erp-ventes-window.cpp
   *
   *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
   */
#include "yeroth-erp-ventes-window.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/windows/yeroth-erp-stock-detail-window.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <QtWidgets/QCalendarWidget>

#include <QtGui/QContextMenuEvent>

#include <QtCore/QThread>

#include <QtCore/QDir>

#include <QtCore/qmath.h>

#include <QtCore/QFile>

#include <QtCore/QProcess>

#include <QtCore/QProcessEnvironment>


#include <unistd.h>


const QString YerothVentesWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE, QObject::trUtf8("ventes")));

YerothVentesWindow::YerothVentesWindow()
:YerothWindowsCommons(YerothVentesWindow::_WINDOW_TITLE),
 _retourVenteTabWidget(0),
 _logger(new YerothLogger("YerothVentesWindow")),
 _aProcess(0),
 _currentlyFiltered(false),
 _pushButton_ventes_filtrer_font(0),
 _ventesDateFilter(YerothUtils::EMPTY_STRING),
 _searchFilter(YerothUtils::EMPTY_STRING),
 _curStocksVenduTableModel(&_allWindows->getSqlTableModel_stocks_vendu())
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _retourVenteTabWidgetTitle = tabWidget_ventes->tabText(RetourDuneVente);

    _retourVenteTabWidget = tabWidget_ventes->widget(RetourDuneVente);

    setupSelectDBFields(_allWindows->STOCKS_VENDU);

    reinitialiser_champs_db_visibles();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);


    _pushButton_ventes_filtrer_font = new QFont(pushButton_ventes_filtrer->font());

    pushButton_ventes_filtrer->disable(this);
    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);


    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_document()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionAnnulerCetteVente, SIGNAL(triggered()), this, SLOT(annuler_cette_vente()));
    connect(actionAfficherVenteDetail, SIGNAL(triggered()), this, SLOT(afficher_vente_detail()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(tabWidget_ventes, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentTabChanged(int)));

    connect(tableView_ventes, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(afficher_vente_detail()));

    connect(tableView_ventes, SIGNAL(activated(const QModelIndex &)), this, SLOT(afficher_vente_detail()));

    connect(tableView_ventes->getStandardItemModel(),
    		SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
			this,
			SLOT(modifier_visibilite_actions_sur_cette_vente()));

    connect(tableView_ventes->getStandardItemModel(),
    		SIGNAL(rowsInserted(const QModelIndex &, int, int)),
			this,
			SLOT(modifier_visibilite_actions_sur_cette_vente()));

    setupShortcuts();
}


void YerothVentesWindow::modifier_visibilite_actions_sur_cette_vente()
{
	if (tableView_ventes->rowCount() > 0)
	{
		actionAfficherVenteDetail->setVisible(true);
		actionAnnulerCetteVente->setVisible(true);
	}
	else
	{
		actionAfficherVenteDetail->setVisible(false);
		actionAnnulerCetteVente->setVisible(false);
	}

}


bool YerothVentesWindow::annuler_cette_vente()
{
	int ventesTableViewRowCount = tableView_ventes->rowCount();

	if (ventesTableViewRowCount <= 0)
	{
		return false;
	}

	tabWidget_ventes->setCurrentIndex(RetourDuneVente);

	QString msg;

	if (lineEdit_retour_vente_quantite_a_retourner->text().isEmpty())
	{
		msg = QObject::trUtf8("Veuillez saisir la quantité d'articles à retourner !");

		YerothQMessageBox::information(this,
									   QObject::trUtf8("saisir la quantité d'articles à retourner"),
									   msg);

		return false;
	}

	double quantite_a_retourner = lineEdit_retour_vente_quantite_a_retourner->text().toInt();

	if (quantite_a_retourner <= 0.0)
	{
		msg = QObject::trUtf8("La quantité d'articles à retourner doit être supérieure zéro !");

		YerothQMessageBox::information(this,
									   QObject::trUtf8("quantité d'articles à retourner"),
									   msg);

		return false;
	}

    int lastSelectedVentesRow = tableView_ventes->lastSelectedRow();

    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord curStocksVenduRecord = _curStocksVenduTableModel->record(lastSelectedVentesRow);

	double curStocksVenduQuantiteVendue = 0.0;

	curStocksVenduQuantiteVendue =
			GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble();

	if (quantite_a_retourner > curStocksVenduQuantiteVendue)
	{
		msg = QObject::trUtf8("La quantité d'articles à retourner doit être "
							  "inférieure ou égale à la quantité d'articles qui a été vendu !");

		YerothQMessageBox::information(this,
									   QObject::trUtf8("quantité d'articles à retourner"),
									   msg);

		return false;
	}


	QString curVenteReferenceRecuVendu(GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::REFERENCE_RECU_VENDU));

	QString curStocksVenduDesignation(GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::DESIGNATION));

	msg = QString(QObject::trUtf8("Poursuivre avec l'annulation de "
								  "la vente avec la désignation '%1', et "
								  "avec la référence 'reçu de vente %2' ?"))
	    	.arg(curStocksVenduDesignation,
	    		 curVenteReferenceRecuVendu);

	if (QMessageBox::Cancel ==
			YerothQMessageBox::question(this, QObject::trUtf8("poursuivre l'annulation de la vente"),
					msg,
					QMessageBox::Cancel,
					QMessageBox::Ok))
	{
		return false;
	}

	QString typeDeVente(QObject::tr("achat-comptant"));

	QString curNomDuClient;

	int clients_id = -1;

	double curMontantTotalVente = 0.0;

	double curMontantARembourserAuClient = 0.0;

	bool rembourserAuCompteClient = false;

	bool successReinsertStock = false;

	double curStockNouvelleQuantiteTotal = 0.0;
	double curStockQuantiteTotal = 0.0;

	QSqlRecord curStockRecord;
	QString curStockTableFilter;

	QString curHistoriqueStock;
	QString curHistoriqueStockRetour;

	QString curStocksVenduID;
	QString curStocksVendu_stocksID;
	QString curStocksVenduCategorie;

	curStocksVenduID =
			GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::ID);

	curStocksVendu_stocksID =
			GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::STOCKS_ID);

	curStocksVenduCategorie =
			GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::CATEGORIE);

	//Je verifie deja si le stock est encore existant
	//dans la base de donnees
	YerothSqlTableModel &curStockTableModel = _allWindows->getSqlTableModel_stocks();

	curStockTableFilter = QString("%1 = '%2'")
										.arg(YerothDatabaseTableColumn::ID,
												curStocksVendu_stocksID);

	curStockTableModel.yerothSetFilter(curStockTableFilter);

	int curStocksTableRowCount = curStockTableModel.easySelect();

	curNomDuClient =
			GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);

	clients_id =
			GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::CLIENTS_ID).toInt();

	typeDeVente =
			GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::TYPE_DE_VENTE);

	if (1 == curStocksTableRowCount)
	{
		curStockRecord = curStockTableModel.record(0);

		curStockQuantiteTotal =
				GET_SQL_RECORD_DATA(curStockRecord, YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();

		curStockNouvelleQuantiteTotal = curStockQuantiteTotal + quantite_a_retourner;

		curMontantTotalVente =
				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();

		curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTAL, curStockNouvelleQuantiteTotal);

		curHistoriqueStock =
				GET_SQL_RECORD_DATA(curStockRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK);

		curHistoriqueStockRetour = YerothHistoriqueStock::creer_mouvement_stock
				(RETOUR_VENTE,
						curStockRecord.value(YerothDatabaseTableColumn::ID).toInt(),
						GET_CURRENT_DATE,
						curStockQuantiteTotal,
						quantite_a_retourner,
						curStockNouvelleQuantiteTotal);

		curHistoriqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
	        					   .append(curHistoriqueStockRetour);

		curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, curHistoriqueStock);

		successReinsertStock = curStockTableModel.updateRecord(0, curStockRecord);
	}
	else
	{
		curStockRecord = curStockTableModel.record();

		curStockRecord.setValue(YerothDatabaseTableColumn::ID, curStocksVendu_stocksID);

		curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK));

		curStockRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::REFERENCE));

		curStockRecord.setValue(YerothDatabaseTableColumn::DESIGNATION, curStocksVenduDesignation);

		curStockRecord.setValue(YerothDatabaseTableColumn::CATEGORIE, curStocksVenduCategorie);

		curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTAL, curStocksVenduQuantiteVendue);

		curMontantTotalVente =
				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();

		//		    qDebug() << QString("++ a rembourser au client: %1")
		//		    				.arg(QString::number(curMontantARembourserAuClient));

		double quantite_total = curStocksVenduQuantiteVendue;

		double montant_total_tva =
				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();

		double montant_tva_unitaire = montant_total_tva / quantite_total;

		double prix_unitaire =
				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::PRIX_UNITAIRE).toDouble();

		double prix_vente = prix_unitaire + montant_tva_unitaire;

		curStockRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, prix_unitaire);

		curStockRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);

		curStockRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA, montant_tva_unitaire);

		curStockRecord.setValue(YerothDatabaseTableColumn::IS_SERVICE,
				YerothUtils::MYSQL_FALSE_LITERAL);

		QString curStocksVenduDatePeremption =
				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::DATE_PEREMPTION);

		QDate formatee(GET_DATE_FROM_STRING(curStocksVenduDatePeremption));

		curStockRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, DATE_TO_DB_FORMAT_STRING(formatee));

		curStockRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

		curStockRecord.setValue(YerothDatabaseTableColumn::LOCALISATION,
				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::LOCALISATION));

		curStockRecord.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, 1);

		curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT, quantite_total);

		curStockRecord.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);

		curHistoriqueStock =
				GET_SQL_RECORD_DATA(curStockRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK);

		curHistoriqueStockRetour = YerothHistoriqueStock::creer_mouvement_stock
				(RETOUR_VENTE,
						curStocksVendu_stocksID.toInt(),
						GET_CURRENT_DATE,
						0.0,
						curStocksVenduQuantiteVendue,
						curStocksVenduQuantiteVendue);

		curHistoriqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
	        					   .append(curHistoriqueStockRetour);

		curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, curHistoriqueStock);

		successReinsertStock = curStockTableModel.insertNewRecord(curStockRecord);
	}

	curStockTableModel.resetFilter();

	if (successReinsertStock)
	{
		double curStocksVenduNouvelleQteVendue = curStocksVenduQuantiteVendue - quantite_a_retourner;

		if (0 != curStocksVenduNouvelleQteVendue)
		{
			curMontantARembourserAuClient =
					quantite_a_retourner * (curMontantTotalVente / curStocksVenduQuantiteVendue);

			double curStocksVenduNouveauMontantTotalVente = curMontantTotalVente - curMontantARembourserAuClient;

			double curStocksVenduMontantTVA =
					GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();

			double curStocksVenduNouveauMontantTVA =
					curStocksVenduNouvelleQteVendue * (curStocksVenduMontantTVA / curStocksVenduQuantiteVendue);

//			qDebug() << QString("++ curStocksVenduNouveauMontantTotalVente: %1, "
//								"curStocksVenduMontantTVA: %2, "
//								"curStocksVenduNouveauMontantTVA: %3, "
//								"curMontantARembourserAuClient: %4")
//							.arg(QString::number(curStocksVenduNouveauMontantTotalVente),
//								 QString::number(curStocksVenduMontantTVA),
//								 QString::number(curStocksVenduNouveauMontantTVA),
//								 QString::number(curMontantARembourserAuClient));

			curStocksVenduRecord.setValue(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE, curStocksVenduNouveauMontantTotalVente);

			curStocksVenduRecord.setValue(YerothDatabaseTableColumn::QUANTITE_VENDUE, curStocksVenduNouvelleQteVendue);

			curStocksVenduRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA, curStocksVenduNouveauMontantTVA);

			_curStocksVenduTableModel->updateRecord(lastSelectedVentesRow, curStocksVenduRecord);
		}
		else
		{
			curMontantARembourserAuClient = curMontantTotalVente;

			QString removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
					.arg(_allWindows->STOCKS_VENDU,
							YerothDatabaseTableColumn::ID,
							curStocksVenduID));

			bool successRemoveRowQuery = YerothUtils::execQuery(removeRowQuery);
		}

		if (-1 != clients_id 	  &&
			YerothUtils::isEqualCaseInsensitive(QObject::tr("achat-compte-client"), typeDeVente))
		{
			rembourserAuCompteClient = handleCompteClient(QString::number(clients_id),
														  curMontantARembourserAuClient);
		}
	}

	YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

	if (successReinsertStock)
	{
		if (rembourserAuCompteClient)
		{
			msg = QString(QObject::trUtf8("La vente (avec référence 'reçu de vente %1') a été "
										  "annulée avec succès !\n\n"
					"(Montant crédité au compte du client '%2': '%3' !)"))
						.arg(curVenteReferenceRecuVendu,
							 curNomDuClient,
							 GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
		}
		else
		{
			msg = QString(QObject::trUtf8("La vente (avec référence 'reçu de vente %1') a été "
										  "annulée avec succès !\n\n"
										  "(Montant à rembourser au client (comptant): '%2' !)"))
						.arg(curVenteReferenceRecuVendu,
							 GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
		}

		YerothQMessageBox::information(this,
									   QObject::trUtf8("succès"),
									   msg);

		tabWidget_ventes->setCurrentIndex(TableauDesVentes);

		lineEdit_retour_vente_quantite_a_retourner->clear();

		reinitialiser_recherche();
	}
	else
	{
		msg = QString(QObject::trUtf8("Échec de l'annulation de la vente "
									  "(avec référence '%1') !"))
				.arg(curVenteReferenceRecuVendu);

		YerothQMessageBox::information(this,
									   QObject::trUtf8("échec"),
									   msg);
	}

	return successReinsertStock;
}


//bool annuler_cette_vente_2()
//{
//	QString msg;
//
//	int ventesTableViewRowCount = tableView_ventes->rowCount();
//
//	QString curVenteReferenceRecuVendu(lineEdit_ventes_reference_recu_vendu->text());
//
//	if (curVenteReferenceRecuVendu.isEmpty() || ventesTableViewRowCount <= 0)
//	{
//		msg = QObject::trUtf8("Veuillez saisir la référence du 'reçu de vente' à annuler !");
//
//		YerothQMessageBox::information(this,
//									   QObject::tr("annuler une vente"),
//									   msg);
//
//		return false;
//	}
//
//    msg = QString(QObject::trUtf8("Poursuivre avec l'annulation de "
//    							  "la vente avec la référence 'reçu de vente "
//    							  "%1' ?"))
//    		.arg(curVenteReferenceRecuVendu);
//
//    if (QMessageBox::Cancel ==
//            YerothQMessageBox::question(this, QObject::trUtf8("poursuivre l'annulation de la vente"),
//    										 msg,
//											 QMessageBox::Cancel,
//											 QMessageBox::Ok))
//    {
//    	return false;
//    }
//
//	YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;
//
//	QString typeDeVente(QObject::tr("achat-comptant"));
//
//	QString curNomDuClient;
//
//	int clients_id = -1;
//
//	double curMontantARembourserAuClient = 0.0;
//
//	bool rembourserAuCompteClient = false;
//
//	bool successReinsertStock = false;
//
//	double curStocksVenduQuantiteVendue = 0.0;
//	double curStockNouvelleQuantiteTotal = 0.0;
//	double curStockQuantiteTotal = 0.0;
//
//	QSqlRecord curStockRecord;
//	QString curStockTableFilter;
//
//    QString curHistoriqueStock;
//    QString curHistoriqueStockRetour;
//
//	QString curStocksVenduID;
//	QString curStocksVendu_stocksID;
//	QString curStocksVenduDesignation;
//	QString curStocksVenduCategorie;
//
//	QSqlRecord curStocksVenduRecord;
//
//	for (int k = 0; k < ventesTableViewRowCount; ++k)
//	{
//		curStocksVenduRecord.clear();
//
//		curStocksVenduRecord = _curStocksVenduTableModel->record(k);
//
//		curStocksVenduID =
//				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::ID);
//
//		curStocksVendu_stocksID =
//				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::STOCKS_ID);
//
//		curStocksVenduDesignation =
//				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::DESIGNATION);
//
//		curStocksVenduCategorie =
//				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::CATEGORIE);
//
//		//Je verifie deja si le stock est encore existant
//		//dans la base de donnees
//		YerothSqlTableModel &curStockTableModel = _allWindows->getSqlTableModel_stocks();
//
//		curStockTableFilter = QString("%1 = '%2'")
//								.arg(YerothDatabaseTableColumn::ID,
//									 curStocksVendu_stocksID);
//
//		curStockTableModel.yerothSetFilter(curStockTableFilter);
//
//		int curStocksTableRowCount = curStockTableModel.easySelect();
//
//		if (0 == k)
//		{
//			curNomDuClient =
//					GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);
//
//			clients_id =
//					GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::CLIENTS_ID).toInt();
//
//			typeDeVente =
//					GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::TYPE_DE_VENTE);
//		}
//
//		curStocksVenduQuantiteVendue =
//				GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble();
//
//		if (1 == curStocksTableRowCount)
//		{
//			curStockRecord = curStockTableModel.record(0);
//
//			curStockQuantiteTotal =
//					GET_SQL_RECORD_DATA(curStockRecord, YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();
//
//			curStockNouvelleQuantiteTotal = curStockQuantiteTotal + curStocksVenduQuantiteVendue;
//
//			curMontantARembourserAuClient +=
//					    		GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();
//
//			curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTAL, curStockNouvelleQuantiteTotal);
//
//			curHistoriqueStock =
//	        		GET_SQL_RECORD_DATA(curStockRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK);
//
//	        curHistoriqueStockRetour = YerothHistoriqueStock::creer_mouvement_stock
//	        			(RETOUR_VENTE,
//	        			 curStockRecord.value(YerothDatabaseTableColumn::ID).toInt(),
//						 GET_CURRENT_DATE,
//						 curStockQuantiteTotal,
//						 curStocksVenduQuantiteVendue,
//						 curStockNouvelleQuantiteTotal);
//
//	        curHistoriqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
//	        			   .append(curHistoriqueStockRetour);
//
//	        curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, curHistoriqueStock);
//
//			successReinsertStock = curStockTableModel.updateRecord(0, curStockRecord);
//		}
//		else
//		{
//			curStockRecord = curStockTableModel.record();
//
//			curStockRecord.setValue(YerothDatabaseTableColumn::ID, curStocksVendu_stocksID);
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
//		    		GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK));
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
//		    		GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::REFERENCE));
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::DESIGNATION, curStocksVenduDesignation);
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::CATEGORIE, curStocksVenduCategorie);
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTAL, curStocksVenduQuantiteVendue);
//
//		    curMontantARembourserAuClient +=
//		    		GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();
//
////		    qDebug() << QString("++ a rembourser au client: %1")
////		    				.arg(QString::number(curMontantARembourserAuClient));
//
//		    double quantite_total = curStocksVenduQuantiteVendue;
//
//		    double montant_total_tva =
//		    		GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
//
//		    double montant_tva_unitaire = montant_total_tva / quantite_total;
//
//		    double prix_unitaire =
//		    		GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::PRIX_UNITAIRE).toDouble();
//
//		    double prix_vente = prix_unitaire + montant_tva_unitaire;
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, prix_unitaire);
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA, montant_tva_unitaire);
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::IS_SERVICE,
//		    		YerothUtils::MYSQL_FALSE_LITERAL);
//
//		    QString curStocksVenduDatePeremption =
//		    		GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::DATE_PEREMPTION);
//
//		    QDate formatee(GET_DATE_FROM_STRING(curStocksVenduDatePeremption));
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, DATE_TO_DB_FORMAT_STRING(formatee));
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
//		    		GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::LOCALISATION,
//		    		GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::LOCALISATION));
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, 1);
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT, quantite_total);
//
//		    curStockRecord.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);
//
//			curHistoriqueStock =
//	        		GET_SQL_RECORD_DATA(curStockRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK);
//
//	        curHistoriqueStockRetour = YerothHistoriqueStock::creer_mouvement_stock
//	        			(RETOUR_VENTE,
//	        			 curStocksVendu_stocksID.toInt(),
//						 GET_CURRENT_DATE,
//						 0.0,
//						 curStocksVenduQuantiteVendue,
//						 curStocksVenduQuantiteVendue);
//
//	        curHistoriqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
//	        			   .append(curHistoriqueStockRetour);
//
//	        curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, curHistoriqueStock);
//
//		    successReinsertStock = curStockTableModel.insertNewRecord(curStockRecord);
//		}
//
//		curStockTableModel.resetFilter();
//
//		if (successReinsertStock)
//		{
//			QString removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
//					.arg(_allWindows->STOCKS_VENDU,
//							YerothDatabaseTableColumn::ID,
//							curStocksVenduID));
//
//			bool successRemoveRowQuery = YerothUtils::execQuery(removeRowQuery);
//
//			if (successRemoveRowQuery &&
//				-1 != clients_id 	  &&
//				YerothUtils::isEqualCaseInsensitive(QObject::tr("achat-compte-client"), typeDeVente))
//			{
//				rembourserAuCompteClient = handleCompteClient(QString::number(clients_id),
//															  curMontantARembourserAuClient);
//			}
//
//			successReinsertStock = successReinsertStock && successRemoveRowQuery;
//		}
//	} //for
//
//	YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
//
//	if (successReinsertStock)
//	{
//		if (rembourserAuCompteClient)
//		{
//			msg = QString(QObject::trUtf8("La vente (avec référence 'reçu de vente %1') a été "
//										  "annulée avec succès !\n\n"
//					"(Montant crédité au compte du client '%2': '%3' !)"))
//						.arg(curVenteReferenceRecuVendu,
//							 curNomDuClient,
//							 GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
//		}
//		else
//		{
//			msg = QString(QObject::trUtf8("La vente (avec référence 'reçu de vente %1') a été "
//										  "annulée avec succès !\n\n"
//										  "(Montant à rembourser au client (comptant): '%2' !)"))
//						.arg(curVenteReferenceRecuVendu,
//							 GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
//		}
//
//		reinitialiser_recherche();
//
//		YerothQMessageBox::information(this,
//									   QObject::trUtf8("succès"),
//									   msg);
//	}
//	else
//	{
//		msg = QString(QObject::trUtf8("Échec de l'annulation de la vente "
//									  "(avec référence '%1') !"))
//				.arg(curVenteReferenceRecuVendu);
//
//		YerothQMessageBox::information(this,
//									   QObject::trUtf8("échec"),
//									   msg);
//	}
//
//	return successReinsertStock;
//}


bool YerothVentesWindow::filtrer_ventes()
{
	QString ventesTableColumnValue(lineEdit_ventes_element_de_vente_resultat->text());

	if (ventesTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
									   QObject::trUtf8("filtrer"),
									   msg);
		return false;

	}

	QString ventesTableColumnProperty(comboBox_ventes_element_de_vente->currentText());

	QString mathOperator(comboBox_ventes_element_de_vente_condition->currentText());

	QString REAL_DB_ID_NAME_ventesTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(ventesTableColumnProperty));

	QString filterString(QString("%1 AND (%2 %3 %4)")
							.arg(_ventesDateFilter,
								 REAL_DB_ID_NAME_ventesTableColumnProperty,
								 mathOperator,
								 ventesTableColumnValue));

	//qDebug() << "++ " << _searchFilter;

	if (!_searchFilter.isEmpty())
	{
		_searchFilter.append(QString(" AND %1").arg(filterString));
	}
	else
	{
		_searchFilter.append(filterString);
	}

	//qDebug() << QString("++ %1").arg(_searchFilter);

	setCurrentlyFiltered(true);

	_curStocksVenduTableModel->yerothSetFilter(_searchFilter);

	int resultRows = _curStocksVenduTableModel->easySelect();

	lister_les_elements_du_tableau(*_curStocksVenduTableModel);

	if (resultRows > 0)
	{
		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "ventes - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "ventes - filtrer");

		return false;
	}

	set_filtrer_font();

	return false;
}


bool YerothVentesWindow::handleCompteClient(QString client_id,
											double curMontantARembourserAuClient)
{
	YerothSqlTableModel &clientsSqlTableModel = _allWindows->getSqlTableModel_clients();

	QString clientsFilter(QString("%1 = '%2'")
							.arg(YerothDatabaseTableColumn::ID,
								 client_id));

	clientsSqlTableModel.yerothSetFilter(clientsFilter);

	int clientsRowCount = clientsSqlTableModel.easySelect();

	if (clientsRowCount > 0)
	{
		QSqlRecord clientsRecord = clientsSqlTableModel.record(0);

		double curCompteClient = clientsRecord.value(YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

		double nouveauCompteClient = curCompteClient + curMontantARembourserAuClient;

		clientsRecord.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, nouveauCompteClient);

		clientsSqlTableModel.updateRecord(0, clientsRecord);

		return true;
	}

	return false;
}


void YerothVentesWindow::set_filtrer_font()
{
    //_logger->log("set_filtrer_font");

    if (isCurrentlyFiltered())
    {
    	_pushButton_ventes_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_ventes_filtrer_font->setUnderline(false);
    }

    pushButton_ventes_filtrer->setFont(*_pushButton_ventes_filtrer_font);
}


void YerothVentesWindow::setCurrentlyFiltered(bool currentlyFiltered)
{
	_currentlyFiltered = currentlyFiltered;

	set_filtrer_font();
}


void YerothVentesWindow::populateComboBoxes()
{
	_logger->log("populateComboBoxes");

	QStringList aQStringList;

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::MONTANT_A_REMBOURSER));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::PRIX_UNITAIRE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::QUANTITE_VENDUE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::MONTANT_TVA));

	comboBox_ventes_element_de_vente->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_ventes_element_de_vente_condition->addItems(aQStringList);
}


void YerothVentesWindow::setupLineEdits()
{
    _logger->log("setupLineEdits");

    lineEdit_ventes_recherche->enableForSearch(QObject::trUtf8("référence"));
    lineEdit_ventes_nom_caissier->enableForSearch(QObject::trUtf8("nom du caissier (caissière)"));
    lineEdit_ventes_designation->enableForSearch(QObject::trUtf8("désignation"));
    lineEdit_ventes_categorie_produit->enableForSearch(QObject::trUtf8("nom de la catégorie d'articles"));
    lineEdit_ventes_nom_entreprise_fournisseur->enableForSearch(QObject::trUtf8("nom de l'entreprise fournisseur"));
    lineEdit_ventes_nom_entreprise_client->enableForSearch(QObject::trUtf8("nom de l'entreprise cliente"));
    lineEdit_ventes_reference_recu_vendu->enableForSearch(QObject::trUtf8("référence reçu de vente"));


    lineEdit_ventes_quantite_vendue->setYerothEnabled(false);
	lineEdit_ventes_tva->setYerothEnabled(false);
	lineEdit_ventes_remise_totale_currency->setYerothEnabled(false);
	lineEdit_ventes_recette_totale->setYerothEnabled(false);


    lineEdit_details_type_de_vente->setYerothEnabled(false);
    lineEdit_details_nom_client->setYerothEnabled(false);
    lineEdit_details_reference_produit->setYerothEnabled(false);
    lineEdit_details_designation->setYerothEnabled(false);
    lineEdit_details_nom_entreprise_fournisseur->setYerothEnabled(false);
    lineEdit_details_categorie->setYerothEnabled(false);
    lineEdit_details_remise_prix->setYerothEnabled(false);
    lineEdit_details_remise_pourcentage->setYerothEnabled(false);
    lineEdit_details_tva->setYerothEnabled(false);
    lineEdit_details_montant_total->setYerothEnabled(false);
    lineEdit_details_nom_caissier->setYerothEnabled(false);
    lineEdit_details_localisation->setYerothEnabled(false);
    lineEdit_details_quantite_vendue->setYerothEnabled(false);
    lineEdit_details_heure_vente->setYerothEnabled(false);
    lineEdit_details_reference_recu_de_vente->setYerothEnabled(false);
    lineEdit_details_prix_unitaire->setYerothEnabled(false);

    lineEdit_retour_vente_quantite_a_retourner->setValidator(&YerothUtils::DoubleValidator);

    lineEdit_retour_vente_type_de_vente->setYerothEnabled(false);
    lineEdit_retour_vente_nom_client->setYerothEnabled(false);
    lineEdit_retour_vente_reference_produit->setYerothEnabled(false);
    lineEdit_retour_vente_designation->setYerothEnabled(false);
    lineEdit_retour_vente_nom_entreprise_fournisseur->setYerothEnabled(false);
    lineEdit_retour_vente_categorie->setYerothEnabled(false);
    lineEdit_retour_vente_remise_prix->setYerothEnabled(false);
    lineEdit_retour_vente_remise_pourcentage->setYerothEnabled(false);
    lineEdit_retour_vente_tva->setYerothEnabled(false);
    lineEdit_retour_vente_montant_total->setYerothEnabled(false);
    lineEdit_retour_vente_nom_caissier->setYerothEnabled(false);
    lineEdit_retour_vente_localisation->setYerothEnabled(false);
    lineEdit_retour_vente_quantite_vendue->setYerothEnabled(false);
    lineEdit_retour_vente_quantite_a_retourner->setYerothEnabled(true);
    lineEdit_retour_vente_heure_vente->setYerothEnabled(false);
    lineEdit_retour_vente_reference_recu_de_vente->setYerothEnabled(false);
    lineEdit_retour_vente_prix_unitaire->setYerothEnabled(false);

    lineEdit_ventes_recherche->setFocus();

    connect(lineEdit_ventes_recherche, SIGNAL(textChanged(const QString &)), this, SLOT(venteRecherche()));

    connect(lineEdit_ventes_nom_caissier, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));

    connect(lineEdit_ventes_designation, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));

    connect(lineEdit_ventes_categorie_produit, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));

    connect(lineEdit_ventes_nom_entreprise_fournisseur, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));

    connect(lineEdit_ventes_nom_entreprise_client, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));

    connect(lineEdit_ventes_reference_recu_vendu, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
}

void YerothVentesWindow::setupLineEditsQCompleters()
{
    lineEdit_ventes_recherche->
		setupMyStaticQCompleter(_allWindows->STOCKS_VENDU,
								YerothDatabaseTableColumn::REFERENCE,
								_ventesDateFilter);

    lineEdit_ventes_nom_caissier->
		setupMyStaticQCompleter(_allWindows->STOCKS_VENDU,
								YerothDatabaseTableColumn::NOM_CAISSIER,
								_ventesDateFilter);

    lineEdit_ventes_designation->
		setupMyStaticQCompleter(_allWindows->STOCKS_VENDU,
								YerothDatabaseTableColumn::DESIGNATION,
								_ventesDateFilter);

    lineEdit_ventes_categorie_produit->
		setupMyStaticQCompleter(_allWindows->STOCKS_VENDU,
								YerothDatabaseTableColumn::CATEGORIE,
								_ventesDateFilter);

    lineEdit_ventes_nom_entreprise_fournisseur->
		setupMyStaticQCompleter(_allWindows->STOCKS_VENDU,
								YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
								_ventesDateFilter);

    lineEdit_ventes_nom_entreprise_client->
		setupMyStaticQCompleter(_allWindows->STOCKS_VENDU,
								YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
								_ventesDateFilter);

    lineEdit_ventes_reference_recu_vendu->
		setupMyStaticQCompleter(_allWindows->STOCKS_VENDU,
								YerothDatabaseTableColumn::REFERENCE_RECU_VENDU,
								_ventesDateFilter);
}

void YerothVentesWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);

    actionAnnulerCetteVente->setShortcut(Qt::Key_F2);
}


void YerothVentesWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	rechercher();
}


void YerothVentesWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

    _visibleDBFieldColumnStrList
			<< YerothDatabaseTableColumn::DATE_VENTE
			<< YerothDatabaseTableColumn::CATEGORIE
			<< YerothDatabaseTableColumn::DESIGNATION
			<< YerothDatabaseTableColumn::PRIX_UNITAIRE
			<< YerothDatabaseTableColumn::QUANTITE_VENDUE
			<< YerothDatabaseTableColumn::REFERENCE
			<< YerothDatabaseTableColumn::TYPE_DE_VENTE
			<< YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE;
}


void YerothVentesWindow::contextMenuEvent(QContextMenuEvent * event)
{
    if (tableView_ventes->rowCount() > 0)
    {
        QMenu menu(this);
        menu.addAction(actionAfficherVenteDetail);
        menu.setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        menu.exec(event->globalPos());
    }
}

void YerothVentesWindow::clear_all_fields()
{
    lineEdit_details_nom_client->clearField();
    lineEdit_details_reference_produit->clearField();
    lineEdit_details_designation->clearField();
    lineEdit_details_nom_entreprise_fournisseur->clearField();
    lineEdit_details_categorie->clearField();
    lineEdit_details_remise_prix->clearField();
    lineEdit_details_remise_pourcentage->clearField();
    lineEdit_details_tva->clearField();
    lineEdit_details_montant_total->clearField();
    lineEdit_details_nom_caissier->clearField();
    lineEdit_details_localisation->clearField();
    lineEdit_details_quantite_vendue->clearField();
    lineEdit_details_reference_recu_de_vente->clearField();
    lineEdit_details_heure_vente->clearField();
    lineEdit_details_prix_unitaire->clearField();


    lineEdit_retour_vente_quantite_a_retourner->clear();

    lineEdit_retour_vente_nom_client->clearField();
    lineEdit_retour_vente_reference_produit->clearField();
    lineEdit_retour_vente_designation->clearField();
    lineEdit_retour_vente_nom_entreprise_fournisseur->clearField();
    lineEdit_retour_vente_categorie->clearField();
    lineEdit_retour_vente_remise_prix->clearField();
    lineEdit_retour_vente_remise_pourcentage->clearField();
    lineEdit_retour_vente_tva->clearField();
    lineEdit_retour_vente_montant_total->clearField();
    lineEdit_retour_vente_nom_caissier->clearField();
    lineEdit_retour_vente_localisation->clearField();
    lineEdit_retour_vente_quantite_vendue->clearField();
    lineEdit_retour_vente_reference_recu_de_vente->clearField();
    lineEdit_retour_vente_heure_vente->clearField();
    lineEdit_retour_vente_prix_unitaire->clearField();

    lineEdit_ventes_tva->clearField();
    lineEdit_ventes_remise_totale_currency->clearField();
    lineEdit_ventes_recette_totale->clearField();
    lineEdit_ventes_quantite_vendue->clearField();
    lineEdit_ventes_recherche->clearField();
}

void YerothVentesWindow::setupDateTimeEdits()
{
	dateEdit_details_date_vente->setYerothEnabled(false);

	dateEdit_details_date_peremption->setYerothEnabled(false);

	dateEdit_retour_vente_date_vente->setYerothEnabled(false);

	dateEdit_retour_vente_date_peremption->setYerothEnabled(false);

    dateEdit_ventes_debut->setStartDate(GET_CURRENT_DATE);

    dateEdit_ventes_fin->setStartDate(GET_CURRENT_DATE);

    _ventesDateFilter.clear();

	_ventesDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    					.arg(YerothDatabaseTableColumn::DATE_VENTE,
    						 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_fin->date())));

    connect(dateEdit_ventes_debut,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_ventes_fin,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));
}


void YerothVentesWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}

void YerothVentesWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    tabWidget_ventes->removeTab(RetourDuneVente);

    pushButton_ventes_filtrer->disable(this);

    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}

void YerothVentesWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    if (tabWidget_ventes->count() < 3)
    {
    	tabWidget_ventes->insertTab(RetourDuneVente,
    								_retourVenteTabWidget,
									_retourVenteTabWidgetTitle);
    }


    pushButton_ventes_filtrer->enable(this, SLOT(filtrer_ventes()));

    pushButton_ventes_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_retour_details->enable(this, SLOT(retourVentes()));

    pushButton_retour_annuler->enable(this, SLOT(retourVentes()));

    pushButton_annuler_vente->enable(this, SLOT(annuler_cette_vente()));
}


void YerothVentesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    tabWidget_ventes->removeTab(RetourDuneVente);

    pushButton_ventes_filtrer->enable(this, SLOT(filtrer_ventes()));

    pushButton_ventes_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_retour_details->enable(this, SLOT(retourVentes()));

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}


void YerothVentesWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    tabWidget_ventes->removeTab(RetourDuneVente);

    pushButton_ventes_filtrer->disable(this);

    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}

void YerothVentesWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    tabWidget_ventes->removeTab(RetourDuneVente);

    pushButton_ventes_filtrer->disable(this);

    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}

void YerothVentesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    tabWidget_ventes->removeTab(RetourDuneVente);

    pushButton_ventes_filtrer->disable(this);

    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}

void YerothVentesWindow::readProcessData()
{
    _logger->log("readProcessData");
    if (!_aProcess)
    {
        return;
    }
    //qDebug() << "\t==>" << _aProcess->readAllStandardOutput();
}

bool YerothVentesWindow::export_csv_file()
{
    _logger->log("export_csv_file");

	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_ventes,
										   tableColumnsToIgnore,
										   "yeroth-erp-journal-ventes-format-csv",
										   "journal des ventes");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_ventes,
										   tableColumnsToIgnore,
										   "yeroth-erp-cashier-journal-csv-format",
										   "journal of sales");
#endif

	return success;
}

void YerothVentesWindow::getJournalDesVentesTexTableString(QString & texTable_in_out,
        												   QStandardItemModel & tableStandardItemModel,
														   QList<int> &dbFieldNameOfTypeString,
														   QList<int> &columnsToIgnore,
														   int fromRowIndex,
														   int toRowIndex,
														   bool lastPage)
{
    if (lastPage && toRowIndex > 20)
    {
        toRowIndex -= 1;
    }

    if (fromRowIndex == toRowIndex)
    {
        return ;
    }

    texTable_in_out.append("\\begin{table*}[!htbp]")
    			   .append("\n")
				   .append("\\centering")
				   .append("\n")
				   .append("\\begin{tabular}")
				   .append("{|");

    //Tex table header

    /** We center the id column*/

    texTable_in_out.append("c|");

    //Tex table header
    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        if (dbFieldNameOfTypeString.contains(k))
        {
        	texTable_in_out.append("l|");
        }
        else
        {
        	texTable_in_out.append("r|");
        }
    }

    texTable_in_out.append("} \\hline \n");
    //qDebug() << "++ texTable caisse: " << texTable_in_out;

    QStandardItem *item;

    int tableColumnCount = 1 + tableStandardItemModel.columnCount();
    /** We add a column named ''n0'' for numbering the rows

     * in the Tex table. */
    texTable_in_out.append("\\textbf{n\\textsuperscript{o}} & ");

    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);

        if (item)
        {
            QString itemText(item->text());

            if (YerothUtils::isEqualCaseInsensitive(itemText, "Total TTC"))
            {
                itemText.append(" (Chiffre d'affaire)");
            }

            itemText.prepend("\\textbf{").append("}");

            YerothUtils::handleTexTableItemText(tableColumnCount,
                                               texTable_in_out,
                                               k,
                                               itemText);
        }
    }
    YerothUtils::cleanUpTexTableLastString(texTable_in_out);
    texTable_in_out.append("\\\\ \\hline \n");
    //Tex table body
    //qDebug() << "++ fromRowIndex: " << fromRowIndex;
    //qDebug() << "++ toRowIndex: " << toRowIndex;
    /* Variable to print the row's number in the output file */

    static int b = 1;
    if (0 == fromRowIndex)
    {
        b = 1;
    }
    QString idColumnText;
    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        idColumnText.clear();
        idColumnText.append(QString::number(b)).prepend("\\textbf{").append("}").append(" & ");
        texTable_in_out.append(idColumnText);
        ++b;
        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }
            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                /**
                 * Any string shall have a length smaller than
                 * YerothERPConfig::max_string_display_length
                 */
                QString itemText(item->text());
                if (itemText.length() > YerothERPConfig::max_string_display_length)
                {
                    itemText.truncate(YerothERPConfig::max_string_display_length);
                    itemText.append(".");
                }
                YerothUtils::handleTexTableItemText(tableColumnCount,
                									texTable_in_out,
													k,
                                                    itemText);
            }
            else
            {
                if (k < tableStandardItemModel.columnCount() - 1)
                {
                    texTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    texTable_in_out.append("\"\" \\hline \n");
                }
            }
        }
        YerothUtils::cleanUpTexTableLastString(texTable_in_out);
        texTable_in_out.append("\\\\ \\hline \n");
        //qDebug() << "++ lastPage: " << lastPage << ". check j: " << j << ", rowToPrintSums: " << rowToPrintSums;
    }
    //Removes the string "" from Latex output
    texTable_in_out.replace("\"\"", "");
    texTable_in_out.append("\\end{tabular}").append("\n").append("\\end{table*}").append("\n");
}

bool YerothVentesWindow::imprimer_document()
{
    _logger->log("imprimer_document");

    QString texTable;

    QStandardItemModel *tableModel = tableView_ventes->getStandardItemModel();

    QList <int> columnsToIgnore;

    fill_table_columns_to_ignore(columnsToIgnore);

    int tableModelRowCount = tableModel->rowCount();

    if (tableModelRowCount <= 0)
    {
        YerothQMessageBox::information(this,
                                       QObject::trUtf8("pas de données à imprimer"),
                                       QObject::trUtf8("Il n'y a pas de données à imprimer !"));
        return false;
    }

    int fromRowIndex = 0;

    int toRowIndex = tableModelRowCount;

    double MAX_TABLE_ROW_COUNT = 35.0;

    int pageNumber = qCeil(tableModelRowCount / MAX_TABLE_ROW_COUNT);
    //qDebug() << QString("number of pages to print: %1").arg(pageNumber);
    //_logger->log("imprimer_document",
    //                  QString("number of pages to print: %1").arg(pageNumber));

    this->getJournalDesVentesTexTableString(texTable,
    										*tableModel,
											this->_DBFieldNamesToPrintLeftAligned,
											columnsToIgnore,
											0,
                                            (20 >= tableModelRowCount) ? tableModelRowCount : 20,
                                            tableModelRowCount <= 20);
    //qDebug() << "++ texTable: " << texTable;
    if (tableModelRowCount >= 20)
    {
        fromRowIndex = 20;
        toRowIndex = (fromRowIndex >= tableModelRowCount) ? fromRowIndex : fromRowIndex + MAX_TABLE_ROW_COUNT;
        int k = 1;
        do
        {
            //qDebug() << QString("## fromRowIndex: %1, toRowIndex: %2")
            //          .arg(QString::number(fromRowIndex), QString::number(toRowIndex));
            this->getJournalDesVentesTexTableString(texTable,
            										*tableModel,
													this->_DBFieldNamesToPrintLeftAligned,
													columnsToIgnore,
                                                    (fromRowIndex >=
                                                            tableModelRowCount) ? tableModelRowCount : fromRowIndex,
                                                    (toRowIndex >=
                                                            tableModelRowCount) ? (tableModelRowCount + 1) : toRowIndex,
                                                    k == pageNumber);
            texTable.append(QString("\\newpage \n"));
            fromRowIndex = toRowIndex;
            toRowIndex =
                (fromRowIndex >= tableModelRowCount) ? (fromRowIndex + 1) : fromRowIndex + MAX_TABLE_ROW_COUNT;
            ++k;
        }
        while (k <= pageNumber && fromRowIndex <= toRowIndex);
    }
    YerothInfoEntreprise & infoEntreprise = _allWindows->getInfoEntreprise();
    QString texDocument;
    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVilleTex()));
    YerothUtils::getCurrentLocaleDate(factureDate);


    YerothUtils::getTexLandscapeSellingDocumentString(texDocument, texTable);


    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", factureDate);
    texDocument.replace("YEROTHVENTESDEBUT", DATE_TO_STRING(dateEdit_ventes_debut->date()));
    texDocument.replace("YEROTHVENTESFIN", DATE_TO_STRING(dateEdit_ventes_fin->date()));
    texDocument.replace("YEROTHNOMUTILISATEUR", _allWindows->getUser()->nom_completTex());
    texDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());

    QString yerothFiltres;

    YerothUtils::addFiltre(lineEdit_ventes_nom_caissier, QObject::trUtf8("caissier"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_designation, QObject::trUtf8("désignation"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_categorie_produit, QObject::trUtf8("catégorie"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_nom_entreprise_fournisseur, QObject::trUtf8("fournisseur"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_nom_entreprise_client, QObject::trUtf8("client"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_reference_recu_vendu, QObject::trUtf8("Nr. reçu"), yerothFiltres);

    int lastIndexOfComa = yerothFiltres.lastIndexOf(",");

    yerothFiltres.remove(lastIndexOfComa, yerothFiltres.length());

    texDocument.replace("YEROTHFILTRES", YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltres));

    //qDebug() << "++ temp file dir: " << YerothConfig::temporaryFilesDir;
    //qDebug() << "++ texDocument: \n" << texDocument << "\n++++++++";

    QString yerothTableauCaissePrefixFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    yerothTableauCaissePrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-journal-ventes"));

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    yerothTableauCaissePrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-cashier-journal"));

#endif


    QFile tmpLatexFile(QString("%1tex")
    					.arg(yerothTableauCaissePrefixFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
    												texDocument);

    QString pdfVentesFileName(YerothERPProcess::compileLatex(yerothTableauCaissePrefixFileName));

    YerothERPProcess::startPdfViewerProcess(pdfVentesFileName);

    return true;
}


void YerothVentesWindow::resetFilter(YerothSqlTableModel * stocksVenduTableModel)
{
    this->_curStocksVenduTableModel = stocksVenduTableModel;

    if (this->_curStocksVenduTableModel)
    {
        this->_curStocksVenduTableModel->resetFilter();
    }

    lineEdit_ventes_recherche->myClear();

    lineEdit_ventes_nom_caissier->myClear();
    lineEdit_ventes_designation->myClear();
    lineEdit_ventes_categorie_produit->myClear();
    lineEdit_ventes_nom_entreprise_fournisseur->myClear();
    lineEdit_ventes_nom_entreprise_client->myClear();
    lineEdit_ventes_reference_recu_vendu->myClear();

    dateEdit_ventes_debut->reset();
    dateEdit_ventes_fin->reset();
}


void YerothVentesWindow::handleTabEnabled()
{
    if (tableView_ventes->rowCount() > 0)
    {
    	tabWidget_ventes->setTabEnabled(RetourDuneVente, true);
        tabWidget_ventes->setTabEnabled(AfficherVenteAuDetail, true);
    }
    else
    {
    	tabWidget_ventes->setTabEnabled(RetourDuneVente, false);
        tabWidget_ventes->setTabEnabled(AfficherVenteAuDetail, false);
    }
}


void YerothVentesWindow::handleCurrentTabChanged(int index)
{
    //_logger->log("handleCurrentChanged(int)",
    //                  QString("handleCurrentChanged]. index: %1").arg(index));

    switch (index)
    {
    case TableauDesVentes:
        lister_les_elements_du_tableau();
        enableImprimer();
        break;

    case AfficherVenteAuDetail:
        afficher_vente_detail();
        disableImprimer();
        break;

    case RetourDuneVente:
    	lineEdit_retour_vente_quantite_a_retourner->setFocus();

    	afficher_retour_vente();
        disableImprimer();
        break;

    default:
        break;
    }

	modifier_visibilite_actions_sur_cette_vente();
}


void YerothVentesWindow::lister_les_elements_du_tableau(YerothSqlTableModel &stocksVenduTableModel)
{
    tableView_ventes->lister_les_elements_du_tableau(stocksVenduTableModel);

    tableView_show_or_hide_columns(*tableView_ventes);

    int curStocksVenduTableModelRowCount = _curStocksVenduTableModel->easySelect();

    double quantite_vendue = 0;
    double quantite_vendue_total = 0.0;
    double remise_prix = 0.0;
    double remise_total_fcfa = 0.0;
    double montant_total = 0.0;
    double montant_total_vente = 0.0;
    double montant_tva = 0.0;
    double total_montant_tva = 0.0;

    QSqlRecord aRecord;
    QString stockId;
    QString strQuery;
    QSqlQuery query;

    for (int k = 0; k < curStocksVenduTableModelRowCount; ++k)
    {
        aRecord.clear();

        query.clear();

        aRecord = _curStocksVenduTableModel->record(k);

        quantite_vendue = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble();

        quantite_vendue_total += quantite_vendue;

        montant_total_vente = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();

        montant_total += montant_total_vente;

        remise_prix = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::REMISE_PRIX).toDouble();

        remise_total_fcfa += (quantite_vendue * remise_prix);

        montant_tva = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();

        total_montant_tva += montant_tva;

        stockId = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::STOCKS_ID);
    }

    if (_allWindows->getUser()->isManager())
    {
        lineEdit_ventes_tva->setText(GET_CURRENCY_STRING_NUM(total_montant_tva));
    }
    else
    {
        lineEdit_ventes_tva->clear();
    }

    lineEdit_ventes_remise_totale_currency->setText(GET_CURRENCY_STRING_NUM(remise_total_fcfa));
    lineEdit_ventes_quantite_vendue->setText(GET_DOUBLE_STRING(quantite_vendue_total));
    lineEdit_ventes_recette_totale->setText(GET_CURRENCY_STRING_NUM(montant_total));

    handleTabEnabled();

    tableView_ventes->resizeColumnsToContents();
}


void YerothVentesWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    _curStocksTableModel = stocksTableModel;

    _curStocksVenduTableModel = &_allWindows->getSqlTableModel_stocks_vendu();

    setupLineEditsQCompleters();

    tabWidget_ventes->setCurrentIndex(TableauDesVentes);

    label_ventes_tva->setText(QString(QObject::tr("TVA (%1)")).arg(YerothERPConfig::currency));

    label_details_tva->setText(QString(QObject::tr("TVA (%1)")).arg(YerothERPConfig::currency));

    label_details_remise_prix->setText(QString(QObject::tr("remise (%1)")).arg(YerothERPConfig::currency));

    label_retour_vente_remise_prix->setText(QString(QObject::tr("remise (%1)")).arg(YerothERPConfig::currency));

    lineEdit_ventes_reference_recu_vendu->setYerothEnabled(true);
    lineEdit_ventes_categorie_produit->setYerothEnabled(true);
    lineEdit_ventes_designation->setYerothEnabled(true);
    lineEdit_ventes_nom_caissier->setYerothEnabled(true);
    lineEdit_ventes_nom_entreprise_fournisseur->setYerothEnabled(true);
    lineEdit_ventes_nom_entreprise_client->setYerothEnabled(true);

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser && aUser->isVendeur())
    {
    	lineEdit_ventes_nom_caissier->setYerothEnabled(false);
    	lineEdit_ventes_nom_caissier->setText(aUser->nom_complet());
    }
    else
    {
    	lineEdit_ventes_nom_caissier->clear();
    	lineEdit_ventes_nom_caissier->setYerothEnabled(true);
    }

    modifier_visibilite_actions_sur_cette_vente();

    setVisible(true);

    rechercher();

    lineEdit_ventes_recherche->setFocus();
}


void YerothVentesWindow::afficher_retour_vente()
{
	if (tableView_ventes->rowCount() <= 0)
	{
		return ;
	}

    int lastSelectedVentesRow = tableView_ventes->lastSelectedRow();

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord record = _curStocksVenduTableModel->record(lastSelectedVentesRow);

    bool isService =  GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::IS_SERVICE).toInt();

    if (isService)
    {
    	QString msg(QObject::trUtf8("Cette fonctionnalité n'est pas disponible pour les 'services' !"));

    		YerothQMessageBox::information(this,
    									   QObject::trUtf8("fonctionnalité pas disponible"),
    									   msg);

    	tabWidget_ventes->setCurrentIndex(TableauDesVentes);

    	return ;
    }

    lineEdit_retour_vente_reference_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));
    lineEdit_retour_vente_designation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    lineEdit_retour_vente_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
    lineEdit_retour_vente_categorie->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));

    double quantite_vendue = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble();

    lineEdit_retour_vente_quantite_vendue->setText(GET_NUM_STRING(quantite_vendue));
    lineEdit_retour_vente_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, "prix_unitaire").toDouble()));
    lineEdit_retour_vente_remise_prix->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REMISE_PRIX).toDouble()));

    QString remise_pourcentage(QString("%1 %")
    								.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REMISE_POURCENTAGE)));

    lineEdit_retour_vente_remise_pourcentage->setText(remise_pourcentage);
    lineEdit_retour_vente_tva->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble()));
    lineEdit_retour_vente_montant_total->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble()));

    dateEdit_retour_vente_date_peremption->setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_PEREMPTION)));
    dateEdit_retour_vente_date_vente->setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_VENTE)));

    lineEdit_retour_vente_heure_vente->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::HEURE_VENTE));

    lineEdit_retour_vente_reference_recu_de_vente->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_RECU_VENDU));

    lineEdit_retour_vente_nom_caissier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_CAISSIER));
    lineEdit_retour_vente_localisation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LOCALISATION));

    YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

    QString clientsIdFilter;

    clientsIdFilter.append(QString("%1 = '%2'")
    						 .arg(YerothDatabaseTableColumn::ID,
    							  GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CLIENTS_ID)));

    clientsTableModel.yerothSetFilter(clientsIdFilter);

    int clientsTableModelRowCount = clientsTableModel.easySelect();

    if (clientsTableModelRowCount > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);
        lineEdit_retour_vente_nom_client->setText(GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE));
    }
    else
    {
        lineEdit_retour_vente_nom_client->setText("");
    }

    lineEdit_retour_vente_nom_client->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT));
    lineEdit_retour_vente_type_de_vente->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::TYPE_DE_VENTE));

    tabWidget_ventes->setCurrentIndex(RetourDuneVente);
}


void YerothVentesWindow::afficher_vente_detail()
{
	if (tableView_ventes->rowCount() <= 0)
	{
		return ;
	}

    int lastSelectedVentesRow = tableView_ventes->lastSelectedRow();

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord record = _curStocksVenduTableModel->record(lastSelectedVentesRow);

    lineEdit_details_reference_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));
    lineEdit_details_designation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    lineEdit_details_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
    lineEdit_details_categorie->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));

    double quantite_vendue = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble();

    lineEdit_details_quantite_vendue->setText(GET_NUM_STRING(quantite_vendue));
    lineEdit_details_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, "prix_unitaire").toDouble()));
    lineEdit_details_remise_prix->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REMISE_PRIX).toDouble()));

    QString remise_pourcentage(QString("%1 %")
    								.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REMISE_POURCENTAGE)));

    lineEdit_details_remise_pourcentage->setText(remise_pourcentage);
    lineEdit_details_tva->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble()));
    lineEdit_details_montant_total->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble()));

    dateEdit_details_date_peremption->setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_PEREMPTION)));
    dateEdit_details_date_vente->setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_VENTE)));

    lineEdit_details_heure_vente->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::HEURE_VENTE));

    lineEdit_details_reference_recu_de_vente->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_RECU_VENDU));

    lineEdit_details_nom_caissier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_CAISSIER));
    lineEdit_details_localisation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LOCALISATION));

    YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

    QString clientsIdFilter;

    clientsIdFilter.append(QString("%1 = '%2'")
    						 .arg(YerothDatabaseTableColumn::ID,
    							  GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CLIENTS_ID)));

    clientsTableModel.yerothSetFilter(clientsIdFilter);

    int clientsTableModelRowCount = clientsTableModel.easySelect();

    if (clientsTableModelRowCount > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);
        lineEdit_details_nom_client->setText(GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE));
    }
    else
    {
        lineEdit_details_nom_client->setText("");
    }

    lineEdit_details_nom_client->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT));
    lineEdit_details_type_de_vente->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::TYPE_DE_VENTE));

    tabWidget_ventes->setCurrentIndex(AfficherVenteAuDetail);
}


void YerothVentesWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_ventes_element_de_vente_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothVentesWindow::reinitialiser_recherche()
{
    //  _logger->log("reinitialiser_recherche");
    lineEdit_ventes_element_de_vente_resultat->clear();

    setCurrentlyFiltered(false);

    this->resetFilter(&_allWindows->getSqlTableModel_stocks_vendu());
    this->rechercher();
    this->lister_les_elements_du_tableau();
    lineEdit_ventes_recherche->setFocus();
}


void YerothVentesWindow::refineYerothLineEdits()
{
	_ventesDateFilter.clear();

	_ventesDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    					.arg(YerothDatabaseTableColumn::DATE_VENTE,
    						 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_fin->date())));

	setupLineEditsQCompleters();

	rechercher();
}


void YerothVentesWindow::rechercher(bool clearVentesRecherche)
{
    //_logger->log("rechercher");

    lineEdit_ventes_element_de_vente_resultat->clear();

    setCurrentlyFiltered(false);

	_searchFilter.clear();

    if (clearVentesRecherche)
    {
        lineEdit_ventes_recherche->clear();
        lineEdit_ventes_nom_caissier->clear();
        lineEdit_ventes_designation->clear();
        lineEdit_ventes_categorie_produit->clear();
        lineEdit_ventes_nom_entreprise_fournisseur->clear();
        lineEdit_ventes_nom_entreprise_client->clear();
        lineEdit_ventes_reference_recu_vendu->clear();
    }

    _searchFilter.append(_ventesDateFilter);

    QString codebar(lineEdit_ventes_recherche->text());

    if (codebar.isEmpty())
    {
        QString nom_caissier(lineEdit_ventes_nom_caissier->text());

        if (!nom_caissier.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_CAISSIER, nom_caissier));
        }


        QString categorie(lineEdit_ventes_categorie_produit->text());

        if (!categorie.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::CATEGORIE, categorie));
        }


        QString designation(lineEdit_ventes_designation->text());

        if (!designation.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, designation));
        }


        QString nom_entreprise_fournisseur(lineEdit_ventes_nom_entreprise_fournisseur->text());

        if (!nom_entreprise_fournisseur.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, nom_entreprise_fournisseur));
        }


        QString nom_entreprise_client(lineEdit_ventes_nom_entreprise_client->text());

        if (!nom_entreprise_client.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, nom_entreprise_client));
        }

        QString reference_recu_vendu(lineEdit_ventes_reference_recu_vendu->text());

        if (!reference_recu_vendu.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::REFERENCE_RECU_VENDU, reference_recu_vendu));
        }

    }
    else
    {
        QString searchCodebar(lineEdit_ventes_recherche->text());

        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(QString(" AND (%1 LIKE '%2')")
            						.arg(YerothDatabaseTableColumn::REFERENCE,
            							 searchCodebar));
        }
    }

    setYerothVentesFilter();

    if (_curStocksVenduTableModel->easySelect() > 0)
    {
        this->setLastListerSelectedRow(0);
    }
    else
    {
        _logger->log("rechercher",
                     QString("reason for failing: %1").arg(_curStocksVenduTableModel->lastError().text()));
    }

    lister_les_elements_du_tableau();
}
