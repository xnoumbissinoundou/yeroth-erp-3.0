/*
 * yeroth-erp-payer-fournisseur-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-payer-fournisseur-window.hpp"

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


YerothPayerFournisseurWindow::YerothPayerFournisseurWindow()
:YerothWindowsCommons(),
 _curReferenceEngagementResteAPayer(0.0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("payer au fournisseur"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                   .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                                		COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    populatePayerAuFournisseursComboBoxes();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);

    pushButton_fournisseurs->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_detail_fournisseur->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);


    pushButton_transactions_fournisseur_consulter->disable(this);
    pushButton_transactions_fournisseur_reinitialiser_consultation->disable(this);



    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionReinitialiser, SIGNAL(triggered()), this, SLOT(reinitialiser_donnees_de_paiement_au_comptefournisseur()));
    connect(actionAfficherDetailsFournisseur, SIGNAL(triggered()), this, SLOT(afficher_detail_fournisseur()));
    connect(actionPayerAuFournisseur, SIGNAL(triggered()), this, SLOT(putCashIntoCustomerAccount()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

    connect(comboBox_fournisseurs_typedepaiement,
    		SIGNAL(currentTextChanged(const QString &)),
			this,
			SLOT(handleComboBoxFournisseurs_Typedepaiement_TextChanged(const QString &)));
}


void YerothPayerFournisseurWindow::hideEvent(QHideEvent * hideEvent)
{
	_allWindows->_transactionsDunFournisseurWindow->close();
}


void YerothPayerFournisseurWindow::handleComboBoxFournisseurs_Typedepaiement_TextChanged(const QString &currentText)
{
	if (YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedencaissementToUserViewString.value(YerothUtils::ENCAISSEMENT_BANCAIRE)) ||
		YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedencaissementToUserViewString.value(YerothUtils::ENCAISSEMENT_TELEPHONE)) ||
		YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedencaissementToUserViewString.value(YerothUtils::ENCAISSEMENT_VIREMENT_BANCAIRE)))
	{
		comboBox_fournisseurs_intitule_du_compte_bancaire->setYerothEnabled(true);
	}
	else
	{
		comboBox_fournisseurs_intitule_du_compte_bancaire->setYerothEnabled(false);
	}
}


void YerothPayerFournisseurWindow::handleReferenceChange(const QString &referenceEngagement)
{
	QString aConditionStr;

	YerothSqlTableModel &stocksVenduSqlTableModel = _allWindows->getSqlTableModel_stocks_vendu();

	aConditionStr = QString("%1 = '%2' AND %3 = '%4'")
						.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
							 lineEdit_fournisseurs_designation_de_lentreprise->text(),
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

	lineEdit_fournisseurs_reference_reste_a_payer->
		setText(GET_CURRENCY_STRING_NUM(_curReferenceEngagementResteAPayer));
}


void YerothPayerFournisseurWindow::afficher_detail_fournisseur()
{
	rendreInvisible();

    _allWindows->_fournisseurDetailsWindow->rendreVisible(_curFournisseurTableModel,
														  _curStocksTableModel);
}


void YerothPayerFournisseurWindow::updateStocksVeduTable(PaymentInfo &paymentInfo)
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


bool YerothPayerFournisseurWindow::createPaymentForCustomerAccount(PaymentInfo &paymentInfo)
{
	YerothSqlTableModel & paiementsTableModel = _allWindows->getSqlTableModel_paiements();

	QSqlRecord record = paiementsTableModel.record();

	record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, 		paymentInfo.nom_entreprise);
	record.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR, 		paymentInfo.nom_encaisseur);
	record.setValue(YerothDatabaseTableColumn::DATE_PAIEMENT, 		paymentInfo.date_paiement);
	record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, 		paymentInfo.fournisseur);
	record.setValue(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT, 	paymentInfo.type_de_paiement);
	record.setValue(YerothDatabaseTableColumn::NOTES, 				paymentInfo.notes);
	record.setValue(YerothDatabaseTableColumn::REFERENCE, 			paymentInfo.reference);
	record.setValue(YerothDatabaseTableColumn::MONTANT_PAYE, 		paymentInfo.montant_paye);
	record.setValue(YerothDatabaseTableColumn::HEURE_PAIEMENT, 		CURRENT_TIME);

	record.setValue(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
			paymentInfo.intitule_du_compte_bancaire);

	int IDforReceipt = YerothERPWindows::getNextIdSqlTableModel_paiements();

	QString referenceRecuPaiementFournisseur(YerothUtils::GET_REFERENCE_RECU_PAIEMENT_CLIENT(QString::number(IDforReceipt)));

	record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT, referenceRecuPaiementFournisseur);

	bool success = paiementsTableModel.insertNewRecord(record, this);

	if (success)
	{
		updateStocksVeduTable(paymentInfo);
	}

	return success;
}


void YerothPayerFournisseurWindow::reinitialiser_donnees_de_paiement_au_comptefournisseur()
{
	textEdit_description->clear();
	lineEdit_montant_a_payer->myClear();
	comboBox_fournisseurs_reference->resetYerothComboBox();
	comboBox_fournisseurs_typedepaiement->resetYerothComboBox();
	comboBox_fournisseurs_intitule_du_compte_bancaire->resetYerothComboBox();
}


void YerothPayerFournisseurWindow::reinitialiser_donnees_de_consultation_des_transactions_financieres()
{
	dateEdit_transactions_fournisseur_debut->reset();
	dateEdit_transactions_fournisseur_fin->reset();
}


void YerothPayerFournisseurWindow::private_slot_afficher_les_transactions_dun_fournisseur()
{
//	qDebug() << QString("_fournisseurLastSelectedRow: %1")
//					.arg(QString::number(_fournisseurLastSelectedRow));

	if (0 != _curFournisseurTableModel 		  &&
		_curFournisseurTableModel->rowCount() > 0)
	{
		QSqlRecord record;

		_allWindows->_fournisseursWindow->
				SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

//	    int stockId = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID).toInt();

	    QString fournisseurCompanyName(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

//	    YerothHistoriqueStock::listHistoriqueDuStock(historiqueStockSelectionne,
//	    											 stockReference,
//													 QString::number(stockId),
//													 stockDesignation);



	    QString fournisseurTransactionsPaiementsQueryStr(QString("select %1, "
	    													"%2 as 'Date transaction', "
	    													"%3 as 'Heure transaction', "
	    													"%4 as 'Type de paiement', "
	    													"%5 as 'Total transaction', "
	    													"%6 as 'Compte fournisseur (apres)', "
	    			    									"reference as 'Raison', "
	    			    									"reference_recu_paiement_fournisseur as 'Recu', "
	    			    									"CONCAT(date_paiement,' ',heure_paiement) as 'Temps' from %7 "
	    			    									"where date_paiement >= '%8' and date_paiement <= '%9'")
	    											.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
	    												 YerothDatabaseTableColumn::DATE_PAIEMENT,
														 YerothDatabaseTableColumn::HEURE_PAIEMENT,
														 YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
														 YerothDatabaseTableColumn::MONTANT_PAYE,
														 YerothDatabaseTableColumn::COMPTE_CLIENT,
														 _allWindows->PAIEMENTS,
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_fournisseur_debut->date()),
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_fournisseur_fin->date())));

	    QString fournisseurTransactionsServicesCompletesQueryStr(QString("select %1, "
	    													 "%2 as 'Date transaction', "
	    													 "%3 as 'Heure transaction', "
	    													 "%4 as 'Type de paiement', "
	    													 "%5 as 'Total transaction', "
	    													 "%6 as 'Compte fournisseur (apres)', "
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
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_fournisseur_debut->date()),
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_fournisseur_fin->date())));

	    QString paiementComptantQueryStr;

	    if (checkBox_ne_pas_inclure_les_ventes_comptant->isChecked())
	    {
	    	paiementComptantQueryStr = QString(" and (%1 != '%2')")
	    									.arg(YerothDatabaseTableColumn::TYPE_DE_VENTE,
	    										 QString::number(YerothUtils::VENTE_COMPTANT));
	    }

	    QString fournisseurTransactionsStockVenduQueryStr(QString("select %1, "
	    													 "%2 as 'Date transaction', "
	    													 "%3 as 'Heure transaction', "
	    													 "%4 as 'Type de paiement', "
	    													 "%5 as 'Total transaction', "
	    													 "fournisseur as 'Compte fournisseur (apres)', "
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
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_fournisseur_debut->date()),
														 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_fournisseur_fin->date()),
														 paiementComptantQueryStr));

	    QString fournisseurTransactionsUnionQueryStr(QString("SELECT * FROM (%1 UNION %2 UNION %3 ORDER BY Temps ASC) AS U WHERE U.%4 = '%5'")
	    										.arg(fournisseurTransactionsPaiementsQueryStr,
	    											 fournisseurTransactionsServicesCompletesQueryStr,
													 fournisseurTransactionsStockVenduQueryStr,
													 YerothDatabaseTableColumn::NOM_ENTREPRISE,
													 fournisseurCompanyName));

//	    qDebug() << QString("++ fournisseurTransactionsUnionQueryStr: %1")
//	    				.arg(fournisseurTransactionsUnionQueryStr);

		QSqlQuery sqlFournisseurTransactionsUnionQuery;

		int querySize = YerothUtils::execQuery(sqlFournisseurTransactionsUnionQuery, fournisseurTransactionsUnionQueryStr);

//		qDebug() << QString("++ querySize: %1")
//						.arg(QString::number(querySize));

		if (querySize > 0)
		{
			YerothUtils::getAllWindows()->_transactionsDunFournisseurWindow
					->listerTransactionsDunFournisseur(dateEdit_transactions_fournisseur_debut->date(),
												  dateEdit_transactions_fournisseur_fin->date(),
												  fournisseurCompanyName,
												  sqlFournisseurTransactionsUnionQuery);
		}
		else
		{
		    YerothQMessageBox::information(this,
		    		QObject::trUtf8("payer au fournisseur - transactions d'un fournisseur"),
					QObject::trUtf8("Il n'y a pas de transactions de ce fournisseur à lister !"));
		}
	}
	else
	{
	    YerothQMessageBox::information(this,
	    		QObject::trUtf8("payer au fournisseur - transactions d'un fournisseur"),
				QObject::trUtf8("Il n'y a pas de transactions de ce fournisseur à lister !"));
	}
}


bool YerothPayerFournisseurWindow::putCashIntoCustomerAccount()
{
	if (comboBox_fournisseurs_reference->currentText().isEmpty())
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

	if (comboBox_fournisseurs_typedepaiement->currentText().isEmpty())
	{
		YerothQMessageBox::information(this,
									   QObject::trUtf8("montant à verser"),
									   QObject::trUtf8("Veuillez choisir le mode de paiement !"));

		return false;
	}

	if (comboBox_fournisseurs_intitule_du_compte_bancaire->isEnabled() &&
		comboBox_fournisseurs_intitule_du_compte_bancaire->currentText().isEmpty())
	{
		YerothQMessageBox::information(this,
									   QObject::trUtf8("montant à verser"),
									   QObject::trUtf8("Vous n'avez choisi aucun compte bancaire !"));
	}


    YerothSqlTableModel & fournisseursTableModel = _allWindows->getSqlTableModel_fournisseurs();

    QString fournisseurFilter;
    fournisseurFilter.append(QString("%1 = '%2'")
    						.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
    							 _curCompanyName));

    fournisseursTableModel.yerothSetFilter_WITH_where_clause(fournisseurFilter);

    bool success = false;

	double fournisseur = 0.0;

    int fournisseursTableModelRowCount = fournisseursTableModel.easySelect();

    if (fournisseursTableModelRowCount > 0)
    {
    	QSqlRecord fournisseursRecord = fournisseursTableModel.record(0);

    	fournisseur = GET_SQL_RECORD_DATA(fournisseursRecord, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

    	double cashPaymentAmount = lineEdit_montant_a_payer->text().toDouble();

    	fournisseur = fournisseur + cashPaymentAmount;

    	fournisseursTableModel.resetFilter();

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


    	double dette_maximale_fournisseur =
    			GET_SQL_RECORD_DATA(fournisseursRecord, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

    	double nouvelle_dette_maximale_fournisseur = dette_maximale_fournisseur - cashPaymentAmount;

    	if (nouvelle_dette_maximale_fournisseur < 0)
    	{
    		nouvelle_dette_maximale_fournisseur = 0.0;
    	}

    	QString queryStr;

    	queryStr.append(QString("UPDATE %1 SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'")
    						.arg(_allWindows->CLIENTS,
    							 YerothDatabaseTableColumn::COMPTE_CLIENT,
								 QString::number(fournisseur),
								 YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
								 QString::number(nouvelle_dette_maximale_fournisseur),
								 YerothDatabaseTableColumn::NOM_ENTREPRISE,
								 _curCompanyName));

    	PaymentInfo paymentInfo;

    	YerothPOSUser *currentUser = _allWindows->getUser();

    	if (0 != currentUser)
    	{
    		paymentInfo.nom_encaisseur = currentUser->nom_complet();
    	}

    	paymentInfo.notes = textEdit_description->toPlainText();

    	paymentInfo.reference = comboBox_fournisseurs_reference->currentText();

    	paymentInfo.type_de_paiement = YerothUtils::getComboBoxDatabaseQueryValue(comboBox_fournisseurs_typedepaiement->currentText(),
    																 	 	 	  YerothUtils::_typedencaissementToUserViewString);

    	paymentInfo.nom_entreprise = _curCompanyName;

    	paymentInfo.date_paiement = GET_CURRENT_DATE;

    	paymentInfo.fournisseur = fournisseur;

    	paymentInfo.montant_paye = cashPaymentAmount;

    	QString curEtablissementBancaire(comboBox_fournisseurs_intitule_du_compte_bancaire->currentText());

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
    	QString msg(QString(QObject::trUtf8("%1 (%2) a été ajouté au fournisseur %3"))
    					.arg(lineEdit_montant_a_payer->text(),
    						 YerothERPConfig::currency,
							 _curCompanyName));

		YerothQMessageBox::information(this, QObject::trUtf8("paiement effectué"), msg);

		textEdit_description->clear();

		lineEdit_fournisseurs_reference_reste_a_payer->clear();

		lineEdit_montant_a_payer->clear();

		comboBox_fournisseurs_reference->resetYerothComboBox();
		comboBox_fournisseurs_typedepaiement->resetYerothComboBox();
		comboBox_fournisseurs_intitule_du_compte_bancaire->resetYerothComboBox();

		populatePayerAuFournisseursComboBoxes();
//		setupLineEditsQCompleters();

		updateLineEdits();
    }
    else
    {
    	QString msg(QString(QObject::trUtf8("Erreur lors du paiement '%1 (%2)' pour le fournisseur '%3'"))
    					.arg(lineEdit_montant_a_payer->text(),
    						 YerothERPConfig::currency,
							 _curCompanyName));

		YerothQMessageBox::information(this, QObject::trUtf8("paiement pas effectué"), msg);
    }

	return success;
}


void YerothPayerFournisseurWindow::setupLineEdits()
{
	lineEdit_fournisseurs_reference_reste_a_payer->setYerothEnabled(false);

	lineEdit_fournisseurs_designation_de_lentreprise->setYerothEnabled(false);

    lineEdit_fournisseurs_valeur_fournisseur->setYerothEnabled(false);

	lineEdit_fournisseurs_valeur_dette_maximale->setYerothEnabled(false);

	lineEdit_fournisseurs_numero_du_contribuable->setYerothEnabled(false);

	lineEdit_fournisseurs_reference_registre_du_commerce->setYerothEnabled(false);

	lineEdit_fournisseurs_siege_social->setYerothEnabled(false);

	lineEdit_montant_a_payer->setPlaceholderText(QObject::trUtf8("montant à payer"));

	lineEdit_montant_a_payer->setValidator(&YerothUtils::DoubleValidator);

	lineEdit_fournisseurs_designation_de_lentreprise->setText(_curCompanyName);
}


void YerothPayerFournisseurWindow::setupLineEditsQCompleters()
{
//	QString aConditionStr(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::TYPE_DE_VENTE,
//						  	  	  	  	  	  	  	 QString::number(YerothUtils::VENTE_COMPTE_CLIENT)));
//
//	aConditionStr.append(QString(" AND %1 = '%2' AND %3 > '0'")
//							.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
//								 _curCompanyName,
//								 YerothDatabaseTableColumn::MONTANT_A_REMBOURSER));
}


void YerothPayerFournisseurWindow::setupDateTimeEdits()
{
	dateEdit_transactions_fournisseur_debut->setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

	dateEdit_transactions_fournisseur_fin->setStartDate(GET_CURRENT_DATE);
}


void YerothPayerFournisseurWindow::populatePayerAuFournisseursComboBoxes()
{
	QString aConditionStr(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::TYPE_DE_VENTE,
						  	  	  	  	  	  	  	 QString::number(YerothUtils::VENTE_COMPTE_CLIENT)));

	aConditionStr.append(QString(" AND %1 = '%2' AND %3 > '0'")
							.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
								 _curCompanyName,
								 YerothDatabaseTableColumn::MONTANT_A_REMBOURSER));

	comboBox_fournisseurs_reference->populateComboBoxRawString(_allWindows->STOCKS_VENDU,
																  YerothDatabaseTableColumn::REFERENCE,
																  aConditionStr);

	comboBox_fournisseurs_intitule_du_compte_bancaire->setYerothEnabled(false);

	comboBox_fournisseurs_typedepaiement->setupPopulateNORawString(_allWindows->TYPE_DE_PAIEMENT,
    															YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
																&YerothUtils::_typedencaissementToUserViewString);

	comboBox_fournisseurs_typedepaiement->populateComboBoxWithout(YerothUtils::ENCAISSEMENT_ACHAT_ANNULE);

	comboBox_fournisseurs_intitule_du_compte_bancaire->populateComboBoxRawString(_allWindows->COMPTES_BANCAIRES,
    																   YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);
}


void YerothPayerFournisseurWindow::updateLineEdits()
{
	lineEdit_fournisseurs_reference_reste_a_payer->clear();

	if (_curFournisseurTableModel->select())
	{
		QSqlRecord aQSqlRecord;

		_allWindows->_fournisseursWindow->
				SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(aQSqlRecord);

		double compteFournisseur = GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

		lineEdit_fournisseurs_valeur_fournisseur->setText(GET_CURRENCY_STRING_NUM(compteFournisseur));

		double detteMaximale = GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

		lineEdit_fournisseurs_valeur_dette_maximale->setText(GET_CURRENCY_STRING_NUM(detteMaximale));

		lineEdit_fournisseurs_reference_registre_du_commerce
		->setText(GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

		lineEdit_fournisseurs_numero_du_contribuable
		->setText(GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));

		lineEdit_fournisseurs_siege_social
		->setText(GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::SIEGE_SOCIAL));
	}
}


void YerothPayerFournisseurWindow::rendreVisible(YerothSqlTableModel *fournisseurTableModel,
												 YerothSqlTableModel *stocksTableModel)
{
	_curFournisseurTableModel = fournisseurTableModel;

    _curStocksTableModel = stocksTableModel;

    QSqlRecord aQSqlRecord;

	_allWindows->_fournisseursWindow->
			SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(aQSqlRecord);

    _curCompanyName = GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE);

    setupLineEdits();

    populatePayerAuFournisseursComboBoxes();

	setVisible(true);

    updateLineEdits();

    connect(comboBox_fournisseurs_reference,
    		SIGNAL(currentTextChanged(const QString &)),
    		this,
            SLOT(handleReferenceChange(const QString &)));

    dateEdit_transactions_fournisseur_debut->setFocus();
}


void YerothPayerFournisseurWindow::rendreInvisible()
{
	_curCompanyName.clear();

	textEdit_description->clear();

	lineEdit_fournisseurs_reference_reste_a_payer->clear();

	lineEdit_montant_a_payer->clear();

	comboBox_fournisseurs_reference->resetYerothComboBox();

	comboBox_fournisseurs_intitule_du_compte_bancaire->resetYerothComboBox();

	comboBox_fournisseurs_typedepaiement->resetYerothComboBox();

    YerothWindowsCommons::rendreInvisible();
}


void YerothPayerFournisseurWindow::definirCaissier()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_fournisseurs->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_detail_fournisseur->disable(this);
    pushButton_transactions_fournisseur_consulter->disable(this);
    pushButton_transactions_fournisseur_reinitialiser_consultation->disable(this);
}


void YerothPayerFournisseurWindow::definirManager()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));
    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_ventes->enable(this, SLOT(ventes()));
	pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_donnees_de_paiement_au_comptefournisseur()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_detail_fournisseur->enable(this, SLOT(afficher_detail_fournisseur()));

    pushButton_transactions_fournisseur_consulter
		->enable(this, SLOT(private_slot_afficher_les_transactions_dun_fournisseur()));

    pushButton_transactions_fournisseur_reinitialiser_consultation
		->enable(this, SLOT(reinitialiser_donnees_de_consultation_des_transactions_financieres()));
}


void YerothPayerFournisseurWindow::definirVendeur()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));
    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_ventes->enable(this, SLOT(ventes()));
	pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_donnees_de_paiement_au_comptefournisseur()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_detail_fournisseur->enable(this, SLOT(afficher_detail_fournisseur()));

    pushButton_transactions_fournisseur_consulter
		->enable(this, SLOT(private_slot_afficher_les_transactions_dun_fournisseur()));

    pushButton_transactions_fournisseur_reinitialiser_consultation
		->enable(this, SLOT(reinitialiser_donnees_de_consultation_des_transactions_financieres()));
}


void YerothPayerFournisseurWindow::definirGestionaireDesStocks()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_fournisseurs->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_detail_fournisseur->disable(this);
    pushButton_transactions_fournisseur_consulter->disable(this);
    pushButton_transactions_fournisseur_reinitialiser_consultation->disable(this);
}


void YerothPayerFournisseurWindow::definirMagasinier()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_fournisseurs->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_detail_fournisseur->disable(this);
    pushButton_transactions_fournisseur_consulter->disable(this);
    pushButton_transactions_fournisseur_reinitialiser_consultation->disable(this);
}


void YerothPayerFournisseurWindow::definirPasDeRole()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_ventes->disable(this);
	pushButton_paiements->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_detail_fournisseur->disable(this);
    pushButton_transactions_fournisseur_consulter->disable(this);
    pushButton_transactions_fournisseur_reinitialiser_consultation->disable(this);
}

