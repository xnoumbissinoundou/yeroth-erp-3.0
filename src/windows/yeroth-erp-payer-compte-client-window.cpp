/*
 * yeroth-erp-marchandises-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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


const QString YerothPayerCompteClientWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("payer au compte client")));

YerothPayerCompteClientWindow::YerothPayerCompteClientWindow()
:YerothWindowsCommons(YerothPayerCompteClientWindow::_WINDOW_TITLE),
 _curReferenceEngagementResteAPayer(0.0)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);

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


void YerothPayerCompteClientWindow::handleComboBoxClients_Typedepaiement_TextChanged(const QString &currentText)
{
	if (YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedepaiementToUserViewString.value(YerothUtils::VERSEMENT_BANCAIRE)) ||
		YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedepaiementToUserViewString.value(YerothUtils::VERSEMENT_TELEPHONE)) ||
		YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedepaiementToUserViewString.value(YerothUtils::VERSEMENT_VIREMENT_BANCAIRE)))
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

	stocksVenduSqlTableModel.yerothSetFilter(aConditionStr);

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

    _allWindows->_clientsDetailWindow->rendreVisible(_clientLastSelectedRow,
    												 _curClientTableModel,
													 _curStocksTableModel);
}


void YerothPayerCompteClientWindow::updateStocksVeduTable(PaymentInfo &paymentInfo)
{
	YerothSqlTableModel & stocksVenduTableModel = _allWindows->getSqlTableModel_stocks_vendu();

	QString stocksVenduFilter(QString("%1 = '%2' AND %3 = '%4' ")
								.arg(YerothDatabaseTableColumn::REFERENCE,
									 paymentInfo.justification,
									 YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
									 paymentInfo.nom_entreprise));

	stocksVenduTableModel.yerothSetFilter(stocksVenduFilter);

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
														 paymentInfo.justification));

    			if (YerothUtils::execQuery(removeStocksVenduRowQuery))
    			{
    				//handle marchandise table
    				QString removeMarchandisesRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
    													 .arg(_allWindows->MARCHANDISES,
    														  YerothDatabaseTableColumn::REFERENCE,
															  paymentInfo.justification));

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

	record.setValue(YerothDatabaseTableColumn::JUSTIFICATION_TRANSACTION_CLIENT,
			paymentInfo.justification);

	record.setValue(YerothDatabaseTableColumn::MONTANT_PAYE, 		paymentInfo.montant_paye);

	record.setValue(YerothDatabaseTableColumn::HEURE_PAIEMENT, 		CURRENT_TIME);

	record.setValue(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
			paymentInfo.intitule_du_compte_bancaire);

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
	lineEdit_comptes_clients_reference->myClear();
	lineEdit_montant_a_payer->myClear();
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
	//qDebug() << QString("_clientLastSelectedRow: %1")
	//				.arg(QString::number(_clientLastSelectedRow));

	if (0 != _curClientTableModel && _curClientTableModel->rowCount() > 0 && _clientLastSelectedRow > -1)
	{
	    QSqlRecord record = _curClientTableModel->record(_clientLastSelectedRow);

//	    int stockId = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID).toInt();

	    QString clientCompanyName(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

//	    YerothHistoriqueStock::listHistoriqueDuStock(historiqueStockSelectionne,
//	    											 stockReference,
//													 QString::number(stockId),
//													 stockDesignation);


	    QString clientTransactionsPaiementsQueryStr(QString("select %1, "
	    													"%2 as 'Date transaction', "
	    													"%3 as 'Heure transaction', "
	    													"%4 as 'Total transaction', "
	    													"%5 as 'Compte client (apres)', "
	    													"%6 as 'Type de paiement', "
	    			    									"justification_transaction_client as 'Raison', "
	    			    									"CONCAT(date_paiement,' ',heure_paiement) as 'Temps' from %7 "
	    			    									"where date_paiement >= '%8' and date_paiement <= '%9'")
	    											.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
	    												 YerothDatabaseTableColumn::DATE_PAIEMENT,
														 YerothDatabaseTableColumn::HEURE_PAIEMENT,
														 YerothDatabaseTableColumn::MONTANT_PAYE,
														 YerothDatabaseTableColumn::COMPTE_CLIENT,
														 YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
														 _allWindows->PAIEMENTS,
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_debut->date()),
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_fin->date())));

	    QString clientTransactionsServicesCompletesQueryStr(QString("select %1, "
	    													 "%2 as 'Date transaction', "
	    													 "%3 as 'Heure transaction', "
	    													 "%4 as 'Total transaction', "
	    													 "%5 as 'Compte client (apres)', "
	    													 "%6 as 'Type de paiement', "
	    			    									 "reference as 'Raison', "
	    			    									 "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %7 "
	    			    									 "where date_vente >= '%8' and date_vente <= '%9'")
	    											.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
	    												 YerothDatabaseTableColumn::DATE_VENTE,
														 YerothDatabaseTableColumn::HEURE_VENTE,
														 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
														 YerothDatabaseTableColumn::COMPTE_CLIENT,
														 YerothDatabaseTableColumn::TYPE_DE_VENTE,
														 _allWindows->SERVICES_COMPLETES,
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_debut->date()),
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_fin->date())));

	    QString clientTransactionsStockVenduQueryStr(QString("select %1, "
	    													 "%2 as 'Date transaction', "
	    													 "%3 as 'Heure transaction', "
	    													 "%4 as 'Total transaction', "
	    													 "%5 as 'Compte client (apres)', "
	    													 "%6 as 'Type de paiement', "
	    			    									 "reference as 'Raison', "
	    			    									 "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %7 "
	    			    									 "where date_vente >= '%8' and date_vente <= '%9'")
	    											.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
	    												 YerothDatabaseTableColumn::DATE_VENTE,
														 YerothDatabaseTableColumn::HEURE_VENTE,
														 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
														 YerothDatabaseTableColumn::COMPTE_CLIENT,
														 YerothDatabaseTableColumn::TYPE_DE_VENTE,
														 _allWindows->STOCKS_VENDU,
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_debut->date()),
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_compte_client_fin->date())));

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
	if (lineEdit_comptes_clients_reference->text().isEmpty())
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
									   QObject::trUtf8("Vous n'avez choisi aucun établissement bancaire !"));
	}


    YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

    QString clientFilter;
    clientFilter.append(QString("%1 = '%2'")
    						.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
    							 _curCompanyName));

    clientsTableModel.yerothSetFilter(clientFilter);

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

    	paymentInfo.justification = lineEdit_comptes_clients_reference->text();

    	paymentInfo.type_de_paiement = YerothUtils::getComboBoxDatabaseQueryValue(comboBox_clients_typedepaiement->currentText(),
    																 	 	 	  YerothUtils::_typedepaiementToUserViewString);

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

		lineEdit_comptes_clients_reference->clear();
		lineEdit_montant_a_payer->clear();

		comboBox_clients_typedepaiement->resetYerothComboBox();
		comboBox_clients_intitule_du_compte_bancaire->resetYerothComboBox();

		setupLineEditsQCompleters();

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
	lineEdit_comptes_clients_reference->enableForSearch(QObject::trUtf8("référence"));

	QString aConditionStr(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::TYPE_DE_VENTE,
						  	  	  	  	  	  	  	 QString::number(YerothUtils::VENTE_COMPTE_CLIENT)));

	aConditionStr.append(QString(" AND %1 = '%2' AND %3 > '0'")
							.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
								 _curCompanyName,
								 YerothDatabaseTableColumn::MONTANT_A_REMBOURSER));

	lineEdit_comptes_clients_reference->setupMyStaticQCompleter(_allWindows->STOCKS_VENDU,
														 	 	 YerothDatabaseTableColumn::REFERENCE,
																 false,
																 true,
																 aConditionStr);
}


void YerothPayerCompteClientWindow::setupDateTimeEdits()
{
	dateEdit_transactions_compte_client_debut->setStartDate(GET_CURRENT_DATE);

	dateEdit_transactions_compte_client_fin->setStartDate(GET_CURRENT_DATE);
}


void YerothPayerCompteClientWindow::populatePayerAuCompteClientsComboBoxes()
{
	comboBox_clients_intitule_du_compte_bancaire->setYerothEnabled(false);

	comboBox_clients_typedepaiement->setupPopulateNORawString(_allWindows->TYPE_DE_PAIEMENT,
    															YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
																&YerothUtils::_typedepaiementToUserViewString);

	comboBox_clients_typedepaiement->populateComboBoxWithout(YerothUtils::VERSEMENT_ACHAT_ANNULE);

	comboBox_clients_intitule_du_compte_bancaire->populateComboBoxRawString(_allWindows->COMPTES_BANCAIRES,
    																   YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);
}


void YerothPayerCompteClientWindow::updateLineEdits()
{
	lineEdit_comptes_clients_reference_reste_a_payer->clear();

	if (_curClientTableModel->select())
	{
		QSqlRecord aQSqlRecord = _curClientTableModel->record(_clientLastSelectedRow);

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


void YerothPayerCompteClientWindow::rendreVisible(int lastSelectedRow,
		   	   	   	   	   	   	   	   	   	      YerothSqlTableModel *clientTableModel,
												  YerothSqlTableModel *stocksTableModel)
{
	_clientLastSelectedRow = lastSelectedRow;

	_curClientTableModel = clientTableModel;

    _curStocksTableModel = stocksTableModel;

    QSqlRecord aQSqlRecord = _curClientTableModel->record(_clientLastSelectedRow);

    _curCompanyName = GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE);

    setupLineEdits();

    setupLineEditsQCompleters();

	setVisible(true);

    updateLineEdits();

    connect(lineEdit_comptes_clients_reference,
    		SIGNAL(textChanged(const QString &)),
    		this,
            SLOT(handleReferenceChange(const QString &)));
}


void YerothPayerCompteClientWindow::rendreInvisible()
{
	_curCompanyName.clear();

	textEdit_description->clear();

	lineEdit_comptes_clients_reference_reste_a_payer->clear();

	lineEdit_comptes_clients_reference->clear();
	lineEdit_montant_a_payer->clear();

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


void YerothPayerCompteClientWindow::getPayerAuCompteClientListingTexTableString(QString &texTable_in_out,
        										 	 	 	 	 	QStandardItemModel &tableStandardItemModel,
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

    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    			   .append("\\centering").append("\n")
				   .append("\\begin{tabular}")
				   .append("{|");

    int texTableColumnCount = tableStandardItemModel.columnCount() + 1;

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

    texTable_in_out.append("} \\hline");//.append("\n");

    /** We add a column named 'id' for numbering the rows
     * in the Tex table. */
    unsigned int id = fromRowIndex + 1;
    texTable_in_out.append("\\textbf{n\\textsuperscript{o}} & ");

    QStandardItem *item;

    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);

        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{").append("}"));
            YerothUtils::handleTexTableItemText(texTableColumnCount + 1,
                                   	   	   	    texTable_in_out,
												k,
												itemText);
        }
    }

    /** Closing Tex table header */
    YerothUtils::cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append("\\\\ \\hline");

    //qDebug() << QString("++ fromRowIndex: %1, toRowIndex: %2")
    //			.arg(QString::number(fromRowIndex), QString::number(toRowIndex));

    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        texTable_in_out.append(QString::number(id));
        texTable_in_out.append(" &");
        ++id;

        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }

            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                QString itemText(item->text());
                YerothUtils::handleComptesClientsTexTableItemText(texTableColumnCount,
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
                    texTable_in_out.append("\"\"").append("\\hline");
                }
            }
        }

        texTable_in_out = texTable_in_out.trimmed();

        YerothUtils::cleanUpTexTableLastString(texTable_in_out);

        texTable_in_out.append("\\\\ \\hline");
    }

    //Removes the empty character "" from Latex output
    texTable_in_out.replace("\"\"", "");

    texTable_in_out.append("\\end{tabular}").append("\n")
    			   .append("\\end{table*}").append("\n");

    //qDebug() << "++ texTable_in_out in getStocksListingTexTableString: " << texTable_in_out;
}


void YerothPayerCompteClientWindow::getPayerAuCompteClientTexDocumentString(QString &texDocumentString_in_out,
        											   	   	    QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(YerothUtils::template_comptes_clients_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}
