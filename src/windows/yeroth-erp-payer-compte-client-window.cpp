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

#include "src/windows/yeroth-erp-search-form.hpp"



#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


const QString YerothPayerCompteClientWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("payer au compte client")));

YerothPayerCompteClientWindow::YerothPayerCompteClientWindow()
:YerothWindowsCommons(YerothPayerCompteClientWindow::_WINDOW_TITLE)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);

    setupLineEdits();

    setupLineEditsQCompleters();

    populatePayerAuCompteClientsComboBoxes();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);

    pushButton_annuler->disable(this);
    pushButton_detail_client->disable(this);
    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);


    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(annuler_paiement_au_compteclient()));
    connect(actionAfficherDetailsCompteClient, SIGNAL(triggered()), this, SLOT(afficher_detail_client()));
    connect(actionPayerAuCompteClient, SIGNAL(triggered()), this, SLOT(putCashIntoCustomerAccount()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
}


YerothPayerCompteClientWindow::~YerothPayerCompteClientWindow()
{
}


void YerothPayerCompteClientWindow::afficher_detail_client()
{
	rendreInvisible();

    _allWindows->_clientsDetailWindow->rendreVisible(_clientLastSelectedRow,
    												 _curClientTableModel,
													 _curStocksTableModel);
}


bool YerothPayerCompteClientWindow::createHistoryPaymentForCustomerAccount(HistoryPaymentInfo &paymentInfo)
{
	YerothSqlTableModel & historiquePaiementsTableModel = _allWindows->getSqlTableModel_paiements();

	QSqlRecord record = historiquePaiementsTableModel.record();

	record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, 		paymentInfo.nom_entreprise);
	record.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR, 		paymentInfo.nom_encaisseur);
	record.setValue(YerothDatabaseTableColumn::DATE_PAIEMENT, 		paymentInfo.date_paiement);
	record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, 		paymentInfo.compte_client);
	record.setValue(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT, 	paymentInfo.type_de_paiement);
	record.setValue(YerothDatabaseTableColumn::NOTES, 				paymentInfo.notes);
	record.setValue(YerothDatabaseTableColumn::ENGAGEMENT, 			paymentInfo.engagement);
	record.setValue(YerothDatabaseTableColumn::MONTANT_PAYE, 		paymentInfo.montant_paye);
	record.setValue(YerothDatabaseTableColumn::HEURE_PAIEMENT, 		CURRENT_TIME);

	bool success = historiquePaiementsTableModel.insertNewRecord(record, this);

	return success;
}


bool YerothPayerCompteClientWindow::putCashIntoCustomerAccount()
{
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


    	QString queryStr;

    	queryStr.append(QString("UPDATE %1  SET %2 = '%3' WHERE %6 = '%7'")
    			.arg(_allWindows->CLIENTS,
    					YerothDatabaseTableColumn::COMPTE_CLIENT,
						QString::number(compte_client),
						YerothDatabaseTableColumn::NOM_ENTREPRISE,
    					_curCompanyName));

    	HistoryPaymentInfo paymentInfo;

    	YerothPOSUser *currentUser = _allWindows->getUser();

    	if (0 != currentUser)
    	{
    		paymentInfo.nom_encaisseur = currentUser->nom_complet();
    	}

    	paymentInfo.notes = textEdit_description->toPlainText();

    	paymentInfo.engagement = lineEdit_comptes_clients_engagement->text();

    	paymentInfo.type_de_paiement = comboBox_clients_typedepaiement->currentText();

    	paymentInfo.nom_entreprise = _curCompanyName;

    	paymentInfo.date_paiement = GET_CURRENT_DATE;

    	paymentInfo.compte_client = compte_client;
    	paymentInfo.montant_paye = cashPaymentAmount;

    	YerothUtils::startTransaction();

    	success = YerothUtils::execQuery(queryStr);

    	success = success && createHistoryPaymentForCustomerAccount(paymentInfo);

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

		lineEdit_comptes_clients_engagement->clear();
		lineEdit_montant_a_payer->clear();
		lineEdit_etablissement_bancaire->clear();

		comboBox_clients_typedepaiement->resetYerothComboBox();


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
	lineEdit_comptes_clients_designation_de_lentreprise->setEnabled(false);

    lineEdit_comptes_clients_valeur_compte_client->setEnabled(false);

	lineEdit_comptes_clients_valeur_dette_maximale->setEnabled(false);

	lineEdit_comptes_clients_reference_registre_du_commerce->setEnabled(false);

	lineEdit_comptes_clients_siege_social->setEnabled(false);

	lineEdit_montant_a_payer->setPlaceholderText(QObject::trUtf8("montant à payer"));

	lineEdit_montant_a_payer->setValidator(&YerothUtils::DoubleValidator);
}


void YerothPayerCompteClientWindow::setupLineEditsQCompleters()
{
	lineEdit_etablissement_bancaire->enableForSearch(QObject::trUtf8("établissement bancaire"));

	lineEdit_comptes_clients_engagement->enableForSearch(QObject::tr("engagement"));

	lineEdit_comptes_clients_engagement->setupMyStaticQCompleter(_allWindows->STOCKS_VENDU,
														 	 	 YerothDatabaseTableColumn::REFERENCE_RECU_VENDU,
																 false,
																 true);
}


void YerothPayerCompteClientWindow::populatePayerAuCompteClientsComboBoxes()
{
    POPULATE_COMBOBOX(comboBox_clients_typedepaiement,
    				  _allWindows->TYPE_DE_PAIEMENT,
					  YerothDatabaseTableColumn::TYPE_DE_PAIEMENT);
}


void YerothPayerCompteClientWindow::updateLineEdits()
{
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
    setupLineEdits();

    setupLineEditsQCompleters();

	_clientLastSelectedRow = lastSelectedRow;

	_curClientTableModel = clientTableModel;

    _curStocksTableModel = stocksTableModel;

    QSqlRecord aQSqlRecord = _curClientTableModel->record(_clientLastSelectedRow);

    _curCompanyName = GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE);

    lineEdit_comptes_clients_designation_de_lentreprise->setText(_curCompanyName);


    updateLineEdits();

	setVisible(true);
}


void YerothPayerCompteClientWindow::rendreInvisible()
{
	_curCompanyName.clear();

	textEdit_description->clear();

	lineEdit_comptes_clients_engagement->clear();
	lineEdit_montant_a_payer->clear();
	lineEdit_etablissement_bancaire->clear();

	comboBox_clients_typedepaiement->resetYerothComboBox();

    YerothWindowsCommons::rendreInvisible();
}


void YerothPayerCompteClientWindow::definirCaissier()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_retour->disable(this);
    pushButton_detail_client->disable(this);
}


void YerothPayerCompteClientWindow::definirManager()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));
    pushButton_annuler->enable(this, SLOT(annuler_paiement_au_compteclient()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_retour->enable(this, SLOT(clients()));
    pushButton_detail_client->enable(this, SLOT(afficher_detail_client()));
}


void YerothPayerCompteClientWindow::definirVendeur()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));
    pushButton_annuler->enable(this, SLOT(annuler_paiement_au_compteclient()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_retour->enable(this, SLOT(clients()));
    pushButton_detail_client->enable(this, SLOT(afficher_detail_client()));
}


void YerothPayerCompteClientWindow::definirGestionaireDesStocks()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_retour->disable(this);
    pushButton_detail_client->disable(this);
}


void YerothPayerCompteClientWindow::definirMagasinier()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_retour->disable(this);
    pushButton_detail_client->disable(this);
}


void YerothPayerCompteClientWindow::definirPasDeRole()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_retour->disable(this);
    pushButton_detail_client->disable(this);
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
