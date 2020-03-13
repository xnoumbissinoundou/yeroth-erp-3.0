/*
 * yeroth-erp-marchandises-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-clients-window.hpp"

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


const QString YerothERPClientsWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("payer à un compte client")));

YerothERPClientsWindow::YerothERPClientsWindow()
:YerothWindowsCommons(YerothERPClientsWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothERPComptesClientsWindow")),
 _currentlyFiltered(false),
 _lastSelectedRow(0),
 _pushButton_filtrer_font(0),
 _action_RechercherFont(0),
 _searchClientsWidget(0),
 _curClientsTableModel(0),
 _searchClientsTableModel(0)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);

    _logger->log("YerothERPClientsWindow");

    setupSelectDBFields(_allWindows->CLIENTS);

    reinitialiser_champs_db_visibles();

    _curClientsTableModel = &_allWindows->getSqlTableModel_clients();

    _searchClientsWidget = new YerothSearchForm(_allWindows, *this, &_allWindows->getSqlTableModel_clients());

    setupLineEdits();

    setupLineEditsQCompleters();

    populateClientsComboBoxes();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    _action_RechercherFont = new QFont(actionRechercher->font());

    tableView_clients->setTableName(&YerothERPWindows::CLIENTS);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);


    //Menu actions
    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionPayerAuCompteClient, SIGNAL(triggered()), this, SLOT(private_payer_au_compteclient()));
    connect(actionCreerCompteClient, SIGNAL(triggered()), this, SLOT(creerCompteClient()));
    connect(actionModifierCompteClient, SIGNAL(triggered()), this, SLOT(modifierCompteClient()));
    connect(actionSupprimerCompteClient, SIGNAL(triggered()), this, SLOT(supprimerCompteClient()));
    connect(actionAfficherDetailsClient, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_document()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionRechercher, SIGNAL(triggered()), this, SLOT(rechercher()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));
    connect(actionReinitialiserElementsDeFiltrage, SIGNAL(triggered()), this, SLOT(reinitialiser_elements_filtrage()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#endif

    connect(actionConsulterTransactionsClient, SIGNAL(triggered()),
    		this, SLOT(private_slot_afficher_les_transactions_dun_client()));

	connect(actionAfficher_client_au_detail, SIGNAL(triggered()),
			this, SLOT(afficher_au_detail()));

    connect(tableView_clients, SIGNAL(signal_lister(YerothSqlTableModel &)),
    		this, SLOT(set_rechercher_font()));

    connect(tableView_clients, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}

YerothERPClientsWindow::~YerothERPClientsWindow()
{
    delete _action_RechercherFont;
    delete _searchClientsWidget;
    delete _pushButton_filtrer_font;
    delete _logger;
}


void YerothERPClientsWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	afficherClients();
}


void YerothERPClientsWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

    _visibleDBFieldColumnStrList
			<< YerothDatabaseTableColumn::NOM_ENTREPRISE
			<< YerothDatabaseTableColumn::VILLE
			<< YerothDatabaseTableColumn::EMAIL
			<< YerothDatabaseTableColumn::NUMERO_TELEPHONE_1;
}


void YerothERPClientsWindow::contextMenuEvent(QContextMenuEvent * event)
{
	QMenu menu(this);
	menu.setPalette(toolBar_clientsWindow->palette());
	menu.addAction(actionAfficherDetailsClient);
	menu.addAction(actionPayerAuCompteClient);
	menu.addAction(actionConsulterTransactionsClient);
	menu.addAction(actionModifierCompteClient);
	menu.addAction(actionSupprimerCompteClient);
	menu.exec(event->globalPos());
}


void YerothERPClientsWindow::hideEvent(QHideEvent * hideEvent)
{
	_searchClientsWidget->rendreInvisible();

	_allWindows->_transactionsDunClientWindow->close();
}


void YerothERPClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);

    actionRechercher->setShortcut(YerothUtils::RECHERCHER_QKEYSEQUENCE);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothERPClientsWindow::private_payer_au_compteclient()
{
    tableView_clients->selectRow(getLastListerSelectedRow());

    if (getLastListerSelectedRow() > -1 && _curClientsTableModel->rowCount() > 0)
    {
    	rendreInvisible();

    	_allWindows->_payerAuCompteclientWindow->rendreVisible(getLastListerSelectedRow(),
    														   _curClientsTableModel,
    														   _curStocksTableModel);
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("payer à un compte client"),
                                  QObject::trUtf8("Sélectionnez un compte client afin d'effectuer un paiement !"));
    }
}


void YerothERPClientsWindow::private_slot_afficher_les_transactions_dun_client()
{
	int lastSelectedRow = getLastListerSelectedRow();

	//qDebug() << QString("lastSelectedRow: %1")
	//				.arg(QString::number(lastSelectedRow));

	if (0 != _curClientsTableModel && _curClientsTableModel->rowCount() > 0 && lastSelectedRow > -1)
	{
	    QSqlRecord record = _curClientsTableModel->record(lastSelectedRow);

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
	    			    									"%7 as 'Raison', "
	    			    									"CONCAT(date_paiement,' ',heure_paiement) as 'Temps' from %8")
	    											.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
	    												 YerothDatabaseTableColumn::DATE_PAIEMENT,
														 YerothDatabaseTableColumn::HEURE_PAIEMENT,
														 YerothDatabaseTableColumn::MONTANT_PAYE,
														 YerothDatabaseTableColumn::COMPTE_CLIENT,
														 YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
														 YerothDatabaseTableColumn::ENGAGEMENT,
														 _allWindows->PAIEMENTS));

	    QString clientTransactionsStockVenduQueryStr(QString("select %1, "
	    													 "%2 as 'Date transaction', "
	    													 "%3 as 'Heure transaction', "
	    													 "%4 as 'Total transaction', "
	    													 "%5 as 'Compte client (apres)', "
	    													 "%6 as 'Type de paiement', "
	    			    									 "%7 as 'Raison', "
	    			    									 "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %8")
	    											.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
	    												 YerothDatabaseTableColumn::DATE_VENTE,
														 YerothDatabaseTableColumn::HEURE_VENTE,
														 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
														 YerothDatabaseTableColumn::COMPTE_CLIENT,
														 YerothDatabaseTableColumn::TYPE_DE_VENTE,
														 YerothDatabaseTableColumn::REFERENCE,
														 _allWindows->STOCKS_VENDU));

	    QString clientTransactionsUnionQueryStr(QString("SELECT * FROM (%1 UNION %2 ORDER BY Temps ASC) AS U WHERE U.%3 = '%4'")
	    										.arg(clientTransactionsPaiementsQueryStr,
	    											 clientTransactionsStockVenduQueryStr,
													 YerothDatabaseTableColumn::NOM_ENTREPRISE,
													 clientCompanyName));

//	    qDebug() << QString("++ clientTransactionsUnionQueryStr: %1")
//	    				.arg(clientTransactionsUnionQueryStr);

		QSqlQuery sqlClientTransactionsUnionQuery;

		int querySize = YerothUtils::execQuery(sqlClientTransactionsUnionQuery, clientTransactionsUnionQueryStr);

//		qDebug() << QString("++ querySize: %1")
//						.arg(QString::number(querySize));

		YerothUtils::getAllWindows()->_transactionsDunClientWindow
				->listerTransactionsDunClient(clientCompanyName,
											  sqlClientTransactionsUnionQuery);
	}
	else
	{
	    YerothQMessageBox::information(this,
	    		QObject::trUtf8("comptes clients - transactions d'un client"),
				QObject::trUtf8("Il n'y a pas de transactions de ce client à lister !"));
	}
}


void YerothERPClientsWindow::modifierCompteClient()
{
    if (_curClientsTableModel->rowCount() > 0)
    {
    	rendreInvisible();

        _allWindows->_modifierCompteClientWindow->rendreVisible(tableView_clients->lastSelectedRow(),
        														_curClientsTableModel,
																_curStocksTableModel);
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::tr("modifier un compte client"),
                                   QObject::trUtf8("Sélectionnez un stock à modifier."));
    }
}


void YerothERPClientsWindow::supprimerCompteClient()
{
	unsigned rowToRemove = tableView_clients->lastSelectedRow();

    YerothSqlTableModel *clientsTableModel = 0;

    if (_curClientsTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->CLIENTS, _curClientsTableModel->sqlTableName()))
    {
        clientsTableModel = _curClientsTableModel;
    }
    else
    {
        return ;
    }

    QSqlRecord record = clientsTableModel->record(rowToRemove);

    if (record.isEmpty() || record.isNull(YerothDatabaseTableColumn::NOM_ENTREPRISE))
    {
        return;
    }

    QString nom_entreprise(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

    QString msgConfirmation(QString(QObject::trUtf8("Supprimer le client '%1' ?"))
    							.arg(nom_entreprise));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
            							QObject::tr("suppression d'un compte client"),
            							msgConfirmation,
										QMessageBox::Cancel,
										QMessageBox::Ok))
    {
        bool success = clientsTableModel->removeRow(rowToRemove);

        QString msg(QString(QObject::trUtf8("Le client '%1"))
        				.arg(nom_entreprise));

        if (success)
        {
        	afficherClients();

            msg.append(QObject::trUtf8("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("suppression d'un compte client - succès"),
            							   msg,
										   QMessageBox::Ok);
        }
        else
        {
            msg.append(QObject::trUtf8(" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("suppression d'un compte client - échec"),
										   msg,
										   QMessageBox::Ok);
        }
    }
}


void YerothERPClientsWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    tableView_clients->selectRow(getLastListerSelectedRow());

    if (getLastListerSelectedRow() > -1 && _curClientsTableModel->rowCount() > 0)
    {
    	//qDebug() << "++ test" << modelIndex.row();
        _allWindows->_clientsDetailWindow->rendreVisible(getLastListerSelectedRow(),
        												 _curClientsTableModel,
														 _curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un compte client"),
                                  QObject::trUtf8("Sélectionnez un compte client à afficher les détails !"));
    }
}


void YerothERPClientsWindow::afficher_au_detail(const QModelIndex & modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    setLastListerSelectedRow(modelIndex.row());

    tableView_clients->selectRow(getLastListerSelectedRow());

    if (getLastListerSelectedRow() > -1 && _curClientsTableModel->rowCount() > 0)
    {
    	rendreInvisible();

        _allWindows->_clientsDetailWindow->rendreVisible(getLastListerSelectedRow(),
        												 _curClientsTableModel,
														 _curStocksTableModel);
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un compte client"),
                                  QObject::trUtf8("Sélectionnez un compte client à afficher les détails !"));
    }
}


void YerothERPClientsWindow::updateLineEditRechercheNomEntreprise()
{
    lineEdit_recherche_nom_entreprise->enableForSearch(QObject::trUtf8("nom de l'entreprise"));

    lineEdit_recherche_nom_entreprise->setupMyStaticQCompleter(_allWindows->CLIENTS,
															   YerothDatabaseTableColumn::NOM_ENTREPRISE,
															   false,
															   false);

    connect(lineEdit_recherche_nom_entreprise->getMyQCompleter(),
     		SIGNAL(activated(const QString &)),
 			this,
            SLOT(afficher_nom_entreprise_selectioner(const QString &)));
}


bool YerothERPClientsWindow::filtrer()
{
	QString stockTableColumnValue(lineEdit_resultat_filtre->text());

	if (stockTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
									  QObject::trUtf8("filtrer"),
									  msg);
		return false;
	}

	QString stockTableColumnProperty(comboBox_element->currentText());

	QString mathOperator(comboBox_condition->currentText());

	QString filterString;

	QString REAL_DB_ID_NAME_stockTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(stockTableColumnProperty));

	filterString.append(QString("%2 %3 '%4'")
							.arg(REAL_DB_ID_NAME_stockTableColumnProperty,
								 mathOperator,
								 stockTableColumnValue));

	//qDebug() << QString("filterString: %1")
	//				.arg(filterString);

	_curClientsTableModel->yerothSetFilter(filterString);

	int resultRows = _curClientsTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);

		afficherClients(*_curClientsTableModel);

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "clients - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "clients - filtrer");
	}

	return false;
}

void YerothERPClientsWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothERPClientsWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_recherche_nom_entreprise->clear();

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    _searchClientsWidget->reinitialiser();

    setSearchFormSqlTableModel(0);

    set_rechercher_font();
}


void YerothERPClientsWindow::set_rechercher_font()
{
    //_logger->log("set_rechercher_font");
    if (0 != _searchClientsTableModel)
    {
        _action_RechercherFont->setUnderline(true);
    }
    else
    {
        _action_RechercherFont->setUnderline(false);
    }

    actionRechercher->setFont(*_action_RechercherFont);
}


void YerothERPClientsWindow::set_filtrer_font()
{
    //_logger->log("set_filtrer_font");

    if (isCurrentlyFiltered())
    {
    	_pushButton_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_filtrer_font->setUnderline(false);
    }

    pushButton_filtrer->setFont(*_pushButton_filtrer_font);
}


void YerothERPClientsWindow::populateClientsComboBoxes()
{
    _logger->log("populateClientsComboBoxes");

	QStringList aQStringList;

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::COMPTE_CLIENT));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT));

    comboBox_element->addItems(aQStringList);

	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_condition->addItems(aQStringList);
}


void YerothERPClientsWindow::setupLineEdits()
{
	lineEdit_nombre_de_comptes_clients->setEnabled(false);

	lineEdit_resultat_filtre->setValidator(&YerothUtils::DoubleValidator);

	updateLineEditRechercheNomEntreprise();
}


void YerothERPClientsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    setupLineEditsQCompleters();

    _curStocksTableModel = stocksTableModel;

    if (! isCurrentlyFiltered())
    {
        if (0 == _searchClientsTableModel)
        {
            _searchClientsWidget->setSqlTableModel(&_allWindows->getSqlTableModel_clients());
        }
    }

	setVisible(true);

	afficherClients(*_curClientsTableModel);

    lineEdit_recherche_nom_entreprise->setFocus();
}


void YerothERPClientsWindow::rendreInvisible()
{
    lineEdit_recherche_nom_entreprise->clear();
    YerothWindowsCommons::rendreInvisible();
}


void YerothERPClientsWindow::setCurrentlyFiltered(bool currentlyFiltered)
{
	_currentlyFiltered = currentlyFiltered;

	set_filtrer_font();
}


void YerothERPClientsWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_compte_client->enable(this, SLOT(creerCompteClient()));
    pushButton_modifier->enable(this, SLOT(modifierCompteClient()));
    pushButton_supprimer->enable(this, SLOT(supprimerCompteClient()));

    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothERPClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_compte_client->enable(this, SLOT(creerCompteClient()));
    pushButton_modifier->enable(this, SLOT(modifierCompteClient()));
    pushButton_supprimer->enable(this, SLOT(supprimerCompteClient()));

    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothERPClientsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPClientsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPClientsWindow::afficher_nom_entreprise_selectioner(const QString & nomEntreprise)
{
    _logger->log("afficher_nom_entreprise_selectioner(const QString &)");

    setLastListerSelectedRow(0);

    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE, nomEntreprise));

    //qDebug() << QString("filter: %1").arg(filter);

    _curClientsTableModel->yerothSetFilter(filter);

    if (_curClientsTableModel->easySelect() > 0)
    {
        afficherClients(*_curClientsTableModel);
        setSearchFormSqlTableModel(_curClientsTableModel);
        set_rechercher_font();
    }
}


void YerothERPClientsWindow::afficherClients(YerothSqlTableModel &clientSqlTableModel)
{
	_searchClientsWidget->rendreInvisible();

    tableView_clients->lister_les_elements_du_tableau(clientSqlTableModel);

    tableView_show_or_hide_columns(*tableView_clients);

    int rowCount = tableView_clients->rowCount();

    lineEdit_nombre_de_comptes_clients->setText(GET_NUM_STRING(rowCount));
}


void YerothERPClientsWindow::afficherClients()
{
	afficherClients(_allWindows->getSqlTableModel_clients());
}


bool YerothERPClientsWindow::export_csv_file()
{
	_logger->log("export_csv_file");

	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_clients,
										   tableColumnsToIgnore,
										   "yeroth-erp-fiche-clients-format-csv",
										   "fiche des comptes clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_clients,
										   tableColumnsToIgnore,
										   "yeroth-erp-customer-account-listing-csv-format",
										   "customer account listing file");
#endif

	return success;
}


void YerothERPClientsWindow::getComptesClientsListingTexTableString(QString &texTable_in_out,
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


void YerothERPClientsWindow::getComptesClientsTexDocumentString(QString &texDocumentString_in_out,
        											   	   	    QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(YerothUtils::template_comptes_clients_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


bool YerothERPClientsWindow::imprimer_document()
{
    _logger->log("imprimer_document");

    QString latexFileNamePrefix("yeroth-erp-comptes-clients");

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    QString pdfStockFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-comptes-clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-customer-account");
#endif

    pdfStockFileName = YerothUtils::prindDocumentFromTableView(this,
    														   *tableView_clients,
															   tableColumnsToIgnore,
															   &YerothERPClientsWindow::getComptesClientsListingTexTableString,
															   &YerothERPClientsWindow::getComptesClientsTexDocumentString,
															   latexFileNamePrefix);

    if (pdfStockFileName.isEmpty())
    {
    	return false;
    }

    YerothERPProcess::startPdfViewerProcess(pdfStockFileName);

    return true;
}

