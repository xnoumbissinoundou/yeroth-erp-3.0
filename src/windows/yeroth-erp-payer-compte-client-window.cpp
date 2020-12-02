/*
 * yeroth-erp-marchandises-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-payer-compte-client-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-style.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


YerothPayerCompteClientWindow::YerothPayerCompteClientWindow()
:YerothWindowsCommons(),
 _curReferenceEngagementResteAPayer(0.0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("payer au compte client"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}")
			.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    populatePayerAuCompteClientsComboBoxes();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);

    pushButton_clients->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_detail_client->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);


    pushButton_transactions_compte_client_consulter->disable(this);
    pushButton_transactions_compte_client_reinitialiser_consultation->disable(this);



    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionReinitialiser, SIGNAL(triggered()), this, SLOT(reinitialiser_donnees_de_paiement_au_compteclient()));
    connect(actionAfficherDetailsCompteClient, SIGNAL(triggered()), this, SLOT(afficher_detail_client()));
    connect(actionPayerAuCompteClient, SIGNAL(triggered()), this, SLOT(putCashIntoCustomerAccount()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

    connect(comboBox_clients_typedepaiement,
    		SIGNAL(currentTextChanged(const QString &)),
			this,
			SLOT(handleComboBoxClients_Typedepaiement_TextChanged(const QString &)));
}


void YerothPayerCompteClientWindow::hideEvent(QHideEvent * hideEvent)
{
	_allWindows->_transactionsDunClientWindow->close();
}


void YerothPayerCompteClientWindow::handleComboBoxClients_Typedepaiement_TextChanged(const QString &currentText)
{
	if (YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedencaissementToUserViewString.value(YerothUtils::ENCAISSEMENT_BANCAIRE)) ||
		YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedencaissementToUserViewString.value(YerothUtils::ENCAISSEMENT_TELEPHONE)) ||
		YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedencaissementToUserViewString.value(YerothUtils::ENCAISSEMENT_VIREMENT_BANCAIRE)))
	{
		comboBox_clients_intitule_du_compte_bancaire->setYerothEnabled(true);
	}
	else
	{
		comboBox_clients_intitule_du_compte_bancaire->setYerothEnabled(false);
	}
}


void YerothPayerCompteClientWindow::handleReferenceChange(const QString &referenceEngagement)
{
	QString aConditionStr;

	YerothSqlTableModel &stocksVenduSqlTableModel = _allWindows->getSqlTableModel_stocks_vendu();

	aConditionStr = QString("%1 = '%2' AND %3 = '%4'")
						.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
							 lineEdit_comptes_clients_designation_de_lentreprise->text(),
							 YerothDatabaseTableColumn::REFERENCE,
							 referenceEngagement);

	stocksVenduSqlTableModel.yerothSetFilter_WITH_where_clause(aConditionStr);

	int rows = stocksVenduSqlTableModel.easySelect();

	if (0 < rows)
	{
		int firstRecord = 0;

		QSqlRecord aStocksVenduRecord = stocksVenduSqlTableModel.record(firstRecord);

		_curReferenceEngagementResteAPayer =
				GET_SQL_RECORD_DATA(aStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_A_REMBOURSER).toDouble();
	}
	else
	{
		_curReferenceEngagementResteAPayer = 0.0;
	}

	stocksVenduSqlTableModel.resetFilter();

	lineEdit_comptes_clients_reference_reste_a_payer->
		setText(GET_CURRENCY_STRING_NUM(_curReferenceEngagementResteAPayer));
}


void YerothPayerCompteClientWindow::afficher_detail_client()
{
	rendreInvisible();

    _allWindows->_clientsDetailWindow->rendreVisible(_curClientTableModel,
													 _curStocksTableModel);
}


void YerothPayerCompteClientWindow::updateStocksVeduTable(PaymentInfo &paymentInfo)
{
	YerothSqlTableModel & stocksVenduTableModel = _allWindows->getSqlTableModel_stocks_vendu();

	QString stocksVenduFilter(QString("%1 = '%2' AND %3 = '%4' ")
								.arg(YerothDatabaseTableColumn::REFERENCE,
									 paymentInfo.reference,
									 YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
									 paymentInfo.nom_entreprise));

	stocksVenduTableModel.yerothSetFilter_WITH_where_clause(stocksVenduFilter);

	int rowCount = stocksVenduTableModel.easySelect();

	if (rowCount > 0)
	{
		double montantPaye = paymentInfo.montant_paye;

		YerothUtils::startTransaction();

		QSqlRecord stocksVenduRecord = stocksVenduTableModel.record(0);

		double montantARembourser =
				GET_SQL_RECORD_DATA(stocksVenduRecord, YerothDatabaseTableColumn::MONTANT_A_REMBOURSER).toDouble();

		double nouveau_montant_a_rembourser = montantARembourser - montantPaye;

		if (nouveau_montant_a_rembourser <= 0)
		{
			nouveau_montant_a_rembourser = 0.0;
		}

		stocksVenduRecord.setValue(YerothDatabaseTableColumn::MONTANT_A_REMBOURSER,
								   QString::number(nouveau_montant_a_rembourser));

		stocksVenduTableModel.updateRecord(0, stocksVenduRecord);

		stocksVenduTableModel.resetFilter();

		if (0 == nouveau_montant_a_rembourser)
		{
			QString stocksVenduID(GET_SQL_RECORD_DATA(stocksVenduRecord, YerothDatabaseTableColumn::ID));

			//copy row from 'stocksVendu' to 'services_completes'
			QString copyRowQuery(QString("DROP TABLE IF EXISTS TempData;"
										 "CREATE TEMPORARY TABLE TempData LIKE %1;"
										 "INSERT INTO TempData SELECT * FROM %2 WHERE id = '%3';"
										 "ALTER TABLE TempData CHANGE COLUMN `id` `id` INT(11) NULL, DROP PRIMARY KEY;"
										 "UPDATE TempData SET id = %4;"
										 "INSERT INTO %5 SELECT * FROM TempData;"
										 "DROP TABLE IF EXISTS TempData;")
									.arg(_allWindows->STOCKS_VENDU,
										 _allWindows->STOCKS_VENDU,
										 stocksVenduID,
										 QString::number(YerothERPWindows::getNextIdSqlTableModel_services_completes()),
										 _allWindows->SERVICES_COMPLETES));

			if (YerothUtils::execQuery(copyRowQuery))
			{
    			//handle stocksVendu table
    			QString removeStocksVenduRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
    												.arg(_allWindows->STOCKS_VENDU,
    													 YerothDatabaseTableColumn::REFERENCE,
														 paymentInfo.reference));

    			if (YerothUtils::execQuery(removeStocksVenduRowQuery))
    			{
    				//handle marchandise table
    				QString removeMarchandisesRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
    													 .arg(_allWindows->MARCHANDISES,
    														  YerothDatabaseTableColumn::REFERENCE,
															  paymentInfo.reference));

    				YerothUtils::execQuery(removeMarchandisesRowQuery);
    			}
			}
		}

		YerothUtils::commitTransaction();
	}
}


bool YerothPayerCompteClientWindow::createPaymentForCustomerAccount(PaymentInfo &paymentInfo)
{
	YerothSqlTableModel & paiementsTableModel = _allWindows->getSqlTableModel_paiements();

	QSqlRecord record = paiementsTableModel.record();

	record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, 		paymentInfo.nom_entreprise);
	record.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR, 		paymentInfo.nom_encaisseur);
	record.setValue(YerothDatabaseTableColumn::DATE_PAIEMENT, 		paymentInfo.date_paiement);
	record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, 		paymentInfo.compte_client);
	record.setValue(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT, 	paymentInfo.type_de_paiement);
	record.setValue(YerothDatabaseTableColumn::NOTES, 				paymentInfo.notes);
	record.setValue(YerothDatabaseTableColumn::REFERENCE, 			paymentInfo.reference);
	record.setValue(YerothDatabaseTableColumn::MONTANT_PAYE, 		paymentInfo.montant_paye);
	record.setValue(YerothDatabaseTableColumn::HEURE_PAIEMENT, 		CURRENT_TIME);

	record.setValue(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
			paymentInfo.intitule_du_compte_bancaire);

	int IDforReceipt = YerothERPWindows::getNextIdSqlTableModel_paiements();

	QString referenceRecuPaiementClient(YerothUtils::GET_REFERENCE_RECU_PAIEMENT_CLIENT(QString::number(IDforReceipt)));

	record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT, referenceRecuPaiementClient);

	bool success = paiementsTableModel.insertNewRecord(record, this);

	if (success)
	{
		updateStocksVeduTable(paymentInfo);
	}

	return success;
}


void YerothPayerCompteClientWindow::reinitialiser_donnees_de_paiement_au_compteclient()
{
	textEdit_description->clear();
	lineEdit_montant_a_payer->myClear();
	comboBox_comptes_clients_reference->resetYerothComboBox();
	comboBox_clients_typedepaiement->resetYerothComboBox();
	comboBox_clients_intitule_du_compte_bancaire->resetYerothComboBox();
}


void YerothPayerCompteClientWindow::reinitialiser_donnees_de_consultation_des_transactions_financieres()
{
	dateEdit_transactions_compte_client_debut->reset();
	dateEdit_transactions_compte_client_fin->reset();
}


void YerothPayerCompteClientWindow::private_slot_afficher_les_transactions_dun_client()
{
//	qDebug() << QString("_clientLastSelectedRow: %1")
//					.arg(QString::number(_clientLastSelectedRow));

	int aClientLastSelectedRow = _allWindows->_clientsWindow->get_INT_LastListerSelectedRow__ID();

	if (0 != _curClientTableModel && _curClientTableModel->rowCount() > 0 && aClientLastSelectedRow > -1)
	{
		 QSqlRecord record;

		_allWindows->_clientsWindow->
			SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

//	    int stockId = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID).toInt();

	    QString clientCompanyName(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

//	    YerothHistoriqueStock::listHistoriqueDuStock(historiqueStockSelectionne,
//	    											 stockReference,
//													 QString::number(stockId),
//													 stockDesignation);



	    QString clientTransactionsPaiementsQueryStr(QString("select %1, "
	    													"%2 as 'Date transaction', "
	    													"%3 as 'Heure transaction', "
	    													"%4 as 'Type de paiement', "
	    													"%5 as 'Total transaction', "
	    													"%6 as 'Compte client (apres)', "
	    			    									"reference as 'Raison', "
	    			    									"reference_recu_paiement_client as 'Recu', "
	    			    									"CONCAT(date_paiement,' ',heure_paiement) as 'Temps' from %7 "
	    			    									"where date_paiement >= '%8' and date_paiement <= '%9'")
	    											.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
	    												 YerothDatabaseTableColumn::DATE_PAIEMENT,
														 YerothDatabaseTableColumn::HEURE_PAIEMENT,
														 YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
														 YerothDatabaseTableColumn::MONTANT_PAYE,
														 YerothDatabaseTableColumn::COMPTE_CLIENT,
														 _allWindows->PAIEMENTS,
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_debut->date()),
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_fin->date())));

	    QString clientTransactionsServicesCompletesQueryStr(QString("select %1, "
	    													 "%2 as 'Date transaction', "
	    													 "%3 as 'Heure transaction', "
	    													 "%4 as 'Type de paiement', "
	    													 "%5 as 'Total transaction', "
	    													 "%6 as 'Compte client (apres)', "
	    			    									 "reference as 'Raison', "
	    			    									 "reference_recu_vendu as 'Recu', "
	    			    									 "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %7 "
	    			    									 "where date_vente >= '%8' and date_vente <= '%9'")
	    											.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
	    												 YerothDatabaseTableColumn::DATE_VENTE,
														 YerothDatabaseTableColumn::HEURE_VENTE,
														 YerothDatabaseTableColumn::TYPE_DE_VENTE,
														 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
														 YerothDatabaseTableColumn::COMPTE_CLIENT,
														 _allWindows->SERVICES_COMPLETES,
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_debut->date()),
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_fin->date())));

	    QString paiementComptantQueryStr;

	    if (checkBox_ne_pas_inclure_les_ventes_comptant->isChecked())
	    {
	    	paiementComptantQueryStr = QString(" and (%1 != '%2')")
	    									.arg(YerothDatabaseTableColumn::TYPE_DE_VENTE,
	    										 QString::number(YerothUtils::VENTE_COMPTANT));
	    }

	    QString clientTransactionsStockVenduQueryStr(QString("select %1, "
	    													 "%2 as 'Date transaction', "
	    													 "%3 as 'Heure transaction', "
	    													 "%4 as 'Type de paiement', "
	    													 "%5 as 'Total transaction', "
	    													 "compte_client as 'Compte client (apres)', "
	    			    									 "reference as 'Raison', "
	    			    									 "reference_recu_vendu as 'Recu', "
	    			    									 "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %6 "
	    			    									 "where (date_vente >= '%7' and date_vente <= '%8') "
	    			    									 "%9")
	    											.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
	    												 YerothDatabaseTableColumn::DATE_VENTE,
														 YerothDatabaseTableColumn::HEURE_VENTE,
														 YerothDatabaseTableColumn::TYPE_DE_VENTE,
														 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
														 _allWindows->STOCKS_VENDU,
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_debut->date()),
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_fin->date()),
														 paiementComptantQueryStr));

	    QString clientTransactionsUnionQueryStr(QString("SELECT * FROM (%1 UNION %2 UNION %3 ORDER BY Temps ASC) AS U WHERE U.%4 = '%5'")
	    										.arg(clientTransactionsPaiementsQueryStr,
	    											 clientTransactionsServicesCompletesQueryStr,
													 clientTransactionsStockVenduQueryStr,
													 YerothDatabaseTableColumn::NOM_ENTREPRISE,
													 clientCompanyName));

//	    qDebug() << QString("++ clientTransactionsUnionQueryStr: %1")
//	    				.arg(clientTransactionsUnionQueryStr);

		QSqlQuery sqlClientTransactionsUnionQuery;

		int querySize = YerothUtils::execQuery(sqlClientTransactionsUnionQuery, clientTransactionsUnionQueryStr);

//		qDebug() << QString("++ querySize: %1")
//						.arg(QString::number(querySize));

		if (querySize > 0)
		{
			YerothUtils::getAllWindows()->_transactionsDunClientWindow
					->listerTransactionsDunClient(dateEdit_transactions_compte_client_debut->date(),
												  dateEdit_transactions_compte_client_fin->date(),
												  clientCompanyName,
												  sqlClientTransactionsUnionQuery);
		}
		else
		{
		    YerothQMessageBox::information(this,
		    		QObject::trUtf8("payer au compte client - transactions d'un client"),
					QObject::trUtf8("Il n'y a pas de transactions de ce client à lister !"));
		}
	}
	else
	{
	    YerothQMessageBox::information(this,
	    		QObject::trUtf8("payer au compte client - transactions d'un client"),
				QObject::trUtf8("Il n'y a pas de transactions de ce client à lister !"));
	}
}


bool YerothPayerCompteClientWindow::putCashIntoCustomerAccount()
{
	if (comboBox_comptes_clients_reference->currentText().isEmpty())
	{
		YerothQMessageBox::information(this,
									   QObject::trUtf8("référence"),
									   QObject::trUtf8("Veuillez entrer un engagement (référence) !"));

		return false;
	}

	if (lineEdit_montant_a_payer->text().isEmpty())
	{
		YerothQMessageBox::information(this,
									   QObject::trUtf8("montant à verser"),
									   QObject::trUtf8("Veuillez entrer le montant à verser !"));

		return false;
	}

	if (comboBox_clients_typedepaiement->currentText().isEmpty())
	{
		YerothQMessageBox::information(this,
									   QObject::trUtf8("montant à verser"),
									   QObject::trUtf8("Veuillez choisir le mode de paiement !"));

		return false;
	}

	if (comboBox_clients_intitule_du_compte_bancaire->isEnabled() &&
		comboBox_clients_intitule_du_compte_bancaire->currentText().isEmpty())
	{
		YerothQMessageBox::information(this,
									   QObject::trUtf8("montant à verser"),
									   QObject::trUtf8("Vous n'avez choisi aucun compte bancaire !"));
	}


    YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

    QString clientFilter;
    clientFilter.append(QString("%1 = '%2'")
    						.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
    							 _curCompanyName));

    clientsTableModel.yerothSetFilter_WITH_where_clause(clientFilter);

    bool success = false;

	double compte_client = 0.0;

    int clientsTableModelRowCount = clientsTableModel.easySelect();

    if (clientsTableModelRowCount > 0)
    {
    	QSqlRecord clientsRecord = clientsTableModel.record(0);

    	compte_client = GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

    	double cashPaymentAmount = lineEdit_montant_a_payer->text().toDouble();

    	compte_client = compte_client + cashPaymentAmount;

    	clientsTableModel.resetFilter();

    	QString msg;

    	if (cashPaymentAmount <= 0)
    	{
    		msg.clear();
    		msg.append(QString(QObject::trUtf8("Le montant donné doit être supérieur "
    										   "à %1 !"))
    					 .arg(GET_CURRENCY_STRING_NUM(cashPaymentAmount)));

    		YerothQMessageBox::information(this, QObject::trUtf8("annulation du paiement"),
    									   msg,
										   QMessageBox::Ok);

    		return false;
    	}

    	if (cashPaymentAmount > _curReferenceEngagementResteAPayer)
    	{
    		msg.clear();
    		msg.append(QString(QObject::trUtf8("Le montant donné (%1) ne doit pas être supérieur "
    										   "à la dette restante !"))
    					 .arg(GET_CURRENCY_STRING_NUM(cashPaymentAmount)));

    		YerothQMessageBox::information(this, QObject::trUtf8("annulation du paiement"),
    									   msg,
										   QMessageBox::Ok);

    		return false;
    	}
    	else
    	{
    		msg.clear();
    		msg.append(QString(QObject::trUtf8("Poursuivre avec le paiement de %1 ?"))
    	    				.arg(GET_CURRENCY_STRING_NUM(cashPaymentAmount)));

    	    if (QMessageBox::Cancel ==
    	            YerothQMessageBox::question(this, QObject::trUtf8("paiement"),
    	    									msg,
    											QMessageBox::Cancel,
    											QMessageBox::Ok))
    	    {
    	    	YerothQMessageBox::information(this, QObject::trUtf8("annulation du paiement"),
    	    								    QObject::trUtf8("Vous avez annulé le paiement en cours !"),
												QMessageBox::Ok);

    	    	return false;
    	    }
    	    else
    	    {
    	    }
    	}


    	double dette_maximale_compte_client =
    			GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

    	double nouvelle_dette_maximale_compte_client = dette_maximale_compte_client - cashPaymentAmount;

    	if (nouvelle_dette_maximale_compte_client < 0)
    	{
    		nouvelle_dette_maximale_compte_client = 0.0;
    	}

    	QString queryStr;

    	queryStr.append(QString("UPDATE %1 SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'")
    						.arg(_allWindows->CLIENTS,
    							 YerothDatabaseTableColumn::COMPTE_CLIENT,
								 QString::number(compte_client),
								 YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
								 QString::number(nouvelle_dette_maximale_compte_client),
								 YerothDatabaseTableColumn::NOM_ENTREPRISE,
								 _curCompanyName));

    	PaymentInfo paymentInfo;

    	YerothPOSUser *currentUser = _allWindows->getUser();

    	if (0 != currentUser)
    	{
    		paymentInfo.nom_encaisseur = currentUser->nom_complet();
    	}

    	paymentInfo.notes = textEdit_description->toPlainText();

    	paymentInfo.reference = comboBox_comptes_clients_reference->currentText();

    	paymentInfo.type_de_paiement = YerothUtils::getComboBoxDatabaseQueryValue(comboBox_clients_typedepaiement->currentText(),
    																 	 	 	  YerothUtils::_typedencaissementToUserViewString);

    	paymentInfo.nom_entreprise = _curCompanyName;

    	paymentInfo.date_paiement = GET_CURRENT_DATE;

    	paymentInfo.compte_client = compte_client;

    	paymentInfo.montant_paye = cashPaymentAmount;

    	QString curEtablissementBancaire(comboBox_clients_intitule_du_compte_bancaire->currentText());

    	paymentInfo.intitule_du_compte_bancaire = curEtablissementBancaire;

    	YerothUtils::startTransaction();

    	success = YerothUtils::execQuery(queryStr);

    	success = success && createPaymentForCustomerAccount(paymentInfo);

    	YerothUtils::commitTransaction();
    }
    else
    {
    	QString msg(QString(QObject::trUtf8("l'entreprise '%1' n'existe pas dans la base de données !"))
    					.arg(_curCompanyName));

    	YerothQMessageBox::information(this, QObject::trUtf8("entreprise non existante"), msg);
    }

    if (true == success)
    {
    	QString msg(QString(QObject::trUtf8("%1 (%2) a été ajouté au compte client %3"))
    					.arg(lineEdit_montant_a_payer->text(),
    						 YerothERPConfig::currency,
							 _curCompanyName));

		YerothQMessageBox::information(this, QObject::trUtf8("paiement effectué"), msg);

		textEdit_description->clear();

		lineEdit_comptes_clients_reference_reste_a_payer->clear();

		lineEdit_montant_a_payer->clear();

		comboBox_comptes_clients_reference->resetYerothComboBox();
		comboBox_clients_typedepaiement->resetYerothComboBox();
		comboBox_clients_intitule_du_compte_bancaire->resetYerothComboBox();

		populatePayerAuCompteClientsComboBoxes();
//		setupLineEditsQCompleters();

		updateLineEdits();
    }
    else
    {
    	QString msg(QString(QObject::trUtf8("Erreur lors du paiement '%1 (%2)' pour le compte client '%3'"))
    					.arg(lineEdit_montant_a_payer->text(),
    						 YerothERPConfig::currency,
							 _curCompanyName));

		YerothQMessageBox::information(this, QObject::trUtf8("paiement pas effectué"), msg);
    }

	return success;
}


void YerothPayerCompteClientWindow::setupLineEdits()
{
	lineEdit_comptes_clients_reference_reste_a_payer->setYerothEnabled(false);

	lineEdit_comptes_clients_designation_de_lentreprise->setYerothEnabled(false);

    lineEdit_comptes_clients_valeur_compte_client->setYerothEnabled(false);

	lineEdit_comptes_clients_valeur_dette_maximale->setYerothEnabled(false);

	lineEdit_comptes_clients_numero_du_contribuable->setYerothEnabled(false);

	lineEdit_comptes_clients_reference_registre_du_commerce->setYerothEnabled(false);

	lineEdit_comptes_clients_siege_social->setYerothEnabled(false);

	lineEdit_montant_a_payer->setPlaceholderText(QObject::trUtf8("montant à payer"));

	lineEdit_montant_a_payer->setValidator(&YerothUtils::DoubleValidator);

	lineEdit_comptes_clients_designation_de_lentreprise->setText(_curCompanyName);
}


void YerothPayerCompteClientWindow::setupLineEditsQCompleters()
{
//	QString aConditionStr(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::TYPE_DE_VENTE,
//						  	  	  	  	  	  	  	 QString::number(YerothUtils::VENTE_COMPTE_CLIENT)));
//
//	aConditionStr.append(QString(" AND %1 = '%2' AND %3 > '0'")
//							.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
//								 _curCompanyName,
//								 YerothDatabaseTableColumn::MONTANT_A_REMBOURSER));
}


void YerothPayerCompteClientWindow::setupDateTimeEdits()
{
	dateEdit_transactions_compte_client_debut->setStartDate(GET_CURRENT_DATE);

	dateEdit_transactions_compte_client_fin->setStartDate(GET_CURRENT_DATE);
}


void YerothPayerCompteClientWindow::populatePayerAuCompteClientsComboBoxes()
{
	QString aConditionStr(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::TYPE_DE_VENTE,
						  	  	  	  	  	  	  	 QString::number(YerothUtils::VENTE_COMPTE_CLIENT)));

	aConditionStr.append(QString(" AND %1 = '%2' AND %3 > '0'")
							.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
								 _curCompanyName,
								 YerothDatabaseTableColumn::MONTANT_A_REMBOURSER));

	comboBox_comptes_clients_reference->populateComboBoxRawString(_allWindows->STOCKS_VENDU,
																  YerothDatabaseTableColumn::REFERENCE,
																  aConditionStr);

	comboBox_clients_intitule_du_compte_bancaire->setYerothEnabled(false);

	comboBox_clients_typedepaiement->setupPopulateNORawString(_allWindows->TYPE_DE_PAIEMENT,
    															YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
																&YerothUtils::_typedencaissementToUserViewString);

	comboBox_clients_typedepaiement->populateComboBoxWithout(YerothUtils::ENCAISSEMENT_ACHAT_ANNULE);

	comboBox_clients_intitule_du_compte_bancaire->populateComboBoxRawString(_allWindows->COMPTES_BANCAIRES,
    																   YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);
}


void YerothPayerCompteClientWindow::updateLineEdits()
{
	lineEdit_comptes_clients_reference_reste_a_payer->clear();

	if (_curClientTableModel->select())
	{
		QSqlRecord aQSqlRecord;

		_allWindows->_clientsWindow->
			SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(aQSqlRecord);

		double compteClient = GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

		lineEdit_comptes_clients_valeur_compte_client->setText(GET_CURRENCY_STRING_NUM(compteClient));

		double detteMaximale = GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

		lineEdit_comptes_clients_valeur_dette_maximale->setText(GET_CURRENCY_STRING_NUM(detteMaximale));

		lineEdit_comptes_clients_reference_registre_du_commerce
		->setText(GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

		lineEdit_comptes_clients_numero_du_contribuable
		->setText(GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));

		lineEdit_comptes_clients_siege_social
		->setText(GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::SIEGE_SOCIAL));
	}
}


void YerothPayerCompteClientWindow::rendreVisible(YerothSqlTableModel *clientTableModel,
												  YerothSqlTableModel *stocksTableModel)
{
	_curClientTableModel = clientTableModel;

    _curStocksTableModel = stocksTableModel;

	QSqlRecord aQSqlRecord;

	_allWindows->_clientsWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(aQSqlRecord);

    _curCompanyName = GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE);

    setupLineEdits();

    populatePayerAuCompteClientsComboBoxes();

	setVisible(true);

    updateLineEdits();

    connect(comboBox_comptes_clients_reference,
    		SIGNAL(currentTextChanged(const QString &)),
    		this,
            SLOT(handleReferenceChange(const QString &)));

    dateEdit_transactions_compte_client_debut->setFocus();
}


void YerothPayerCompteClientWindow::rendreInvisible()
{
	_curCompanyName.clear();

	textEdit_description->clear();

	lineEdit_comptes_clients_reference_reste_a_payer->clear();

	lineEdit_montant_a_payer->clear();

	comboBox_comptes_clients_reference->resetYerothComboBox();

	comboBox_clients_intitule_du_compte_bancaire->resetYerothComboBox();

	comboBox_clients_typedepaiement->resetYerothComboBox();

    YerothWindowsCommons::rendreInvisible();
}


void YerothPayerCompteClientWindow::definirCaissier()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_clients->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_detail_client->disable(this);
    pushButton_transactions_compte_client_consulter->disable(this);
    pushButton_transactions_compte_client_reinitialiser_consultation->disable(this);
}


void YerothPayerCompteClientWindow::definirManager()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_ventes->enable(this, SLOT(ventes()));
	pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_donnees_de_paiement_au_compteclient()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_detail_client->enable(this, SLOT(afficher_detail_client()));

    pushButton_transactions_compte_client_consulter
		->enable(this, SLOT(private_slot_afficher_les_transactions_dun_client()));

    pushButton_transactions_compte_client_reinitialiser_consultation
		->enable(this, SLOT(reinitialiser_donnees_de_consultation_des_transactions_financieres()));
}


void YerothPayerCompteClientWindow::definirVendeur()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_ventes->enable(this, SLOT(ventes()));
	pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_donnees_de_paiement_au_compteclient()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_detail_client->enable(this, SLOT(afficher_detail_client()));

    pushButton_transactions_compte_client_consulter
		->enable(this, SLOT(private_slot_afficher_les_transactions_dun_client()));

    pushButton_transactions_compte_client_reinitialiser_consultation
		->enable(this, SLOT(reinitialiser_donnees_de_consultation_des_transactions_financieres()));
}


void YerothPayerCompteClientWindow::definirGestionaireDesStocks()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_clients->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_detail_client->disable(this);
    pushButton_transactions_compte_client_consulter->disable(this);
    pushButton_transactions_compte_client_reinitialiser_consultation->disable(this);
}


void YerothPayerCompteClientWindow::definirMagasinier()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_clients->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_detail_client->disable(this);
    pushButton_transactions_compte_client_consulter->disable(this);
    pushButton_transactions_compte_client_reinitialiser_consultation->disable(this);
}


void YerothPayerCompteClientWindow::definirPasDeRole()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_detail_client->disable(this);
    pushButton_transactions_compte_client_consulter->disable(this);
    pushButton_transactions_compte_client_reinitialiser_consultation->disable(this);
}

