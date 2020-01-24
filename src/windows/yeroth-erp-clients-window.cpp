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



unsigned int YerothERPClientsWindow::PDF_LISTING_COLUMN_REFERENCE_CLIENT(1);

unsigned int YerothERPClientsWindow::PDF_LISTING_COLUMN_NOM_ENTREPRISE(2);

unsigned int YerothERPClientsWindow::PDF_LISTING_COLUMN_NOM_REPRESENTANT(3);


const QString YerothERPClientsWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("comptes clients")));

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

    _logger->log("YerothInventaireDesStocksWindow");

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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_payer->disable(this);

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
    		<< YerothDatabaseTableColumn::REFERENCE_CLIENT
			<< YerothDatabaseTableColumn::NOM_ENTREPRISE
			<< YerothDatabaseTableColumn::NOM_REPRESENTANT
			<< YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT
			<< YerothDatabaseTableColumn::COMPTE_CLIENT;
}


void YerothERPClientsWindow::contextMenuEvent(QContextMenuEvent * event)
{
	QMenu menu(this);
	menu.setPalette(toolBar_clientsWindow->palette());
	menu.addAction(actionAfficherDetailsClient);
	menu.addAction(actionModifierCompteClient);
	menu.addAction(actionSupprimerCompteClient);
	menu.exec(event->globalPos());
}


void YerothERPClientsWindow::hideEvent(QHideEvent * hideEvent)
{
	_searchClientsWidget->rendreInvisible();
}


void YerothERPClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);

    actionRechercher->setShortcut(YerothUtils::RECHERCHER_QKEYSEQUENCE);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
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


bool YerothERPClientsWindow::createHistoryPaymentForCustomerAccount(HistoryPaymentInfo &paymentInfo)
{
	YerothSqlTableModel & historiquePaiementsTableModel = _allWindows->getSqlTableModel_paiements();

	QSqlRecord record = historiquePaiementsTableModel.record();

	record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, 				paymentInfo.nom_entreprise);
	record.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR, 				paymentInfo.nom_encaisseur);
	record.setValue(YerothDatabaseTableColumn::NOM_PAYEUR, 					paymentInfo.nom_payeur);
	record.setValue(YerothDatabaseTableColumn::DATE_PAIEMENT, 				paymentInfo.date_paiement);
	record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, 				paymentInfo.compte_client);
	record.setValue(YerothDatabaseTableColumn::MONTANT_PAYE, 				paymentInfo.montant_paye);
	record.setValue(YerothDatabaseTableColumn::HEURE_PAIEMENT, 				CURRENT_TIME);

	bool success = historiquePaiementsTableModel.insertNewRecord(record, this);

	return success;
}


bool YerothERPClientsWindow::putCashIntoCustomerAccount()
{
	if (lineEdit_nom_payeur->text().isEmpty())
	{
		YerothQMessageBox::information(this,
									   QObject::trUtf8("nom du payeur"),
									   QObject::trUtf8("Veuillez entrer le nom du payeur !"));

		return false;
	}

	if (lineEdit_nom_entreprise->text().isEmpty())
	{
		YerothQMessageBox::information(this,
									   QObject::trUtf8("nom de l'entreprise cliente"),
									   QObject::trUtf8("Veuillez entrer le nom de l'entreprise cliente !"));

		return false;
	}

	if (lineEdit_versement_comptant->text().isEmpty())
	{
		YerothQMessageBox::information(this,
									   QObject::trUtf8("montant à verser"),
									   QObject::trUtf8("Veuillez entrer le montant à verser !"));

		return false;
	}

    YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

    QString clientFilter;
    clientFilter.append(QString("%1 = '%2'")
    						.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
    							 lineEdit_nom_entreprise->text()));

    clientsTableModel.yerothSetFilter(clientFilter);

    bool success = false;

		double compte_client = 0.0;

    int clientsTableModelRowCount = clientsTableModel.easySelect();

    if (clientsTableModelRowCount > 0)
    {
    	QSqlRecord clientsRecord = clientsTableModel.record(0);

    	compte_client = GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

    	double cashPaymentAmount = lineEdit_versement_comptant->text().toDouble();

    	compte_client = compte_client + cashPaymentAmount;

    	clientsTableModel.resetFilter();


    	QString queryStr;

    	queryStr.append(QString("UPDATE %1  SET %2 = '%3' WHERE %6 = '%7'")
    			.arg(_allWindows->CLIENTS,
    					YerothDatabaseTableColumn::COMPTE_CLIENT,
						QString::number(compte_client),
						YerothDatabaseTableColumn::NOM_ENTREPRISE,
						lineEdit_nom_entreprise->text()));

    	HistoryPaymentInfo paymentInfo;

    	YerothPOSUser *currentUser = _allWindows->getUser();

    	if (0 != currentUser)
    	{
    		paymentInfo.nom_encaisseur = currentUser->nom_complet();
    	}

    	paymentInfo.nom_entreprise = lineEdit_nom_entreprise->text();

    	paymentInfo.nom_payeur = lineEdit_nom_payeur->text();

    	paymentInfo.date_paiement = GET_CURRENT_DATE;

    	paymentInfo.compte_client = compte_client;
    	paymentInfo.montant_paye = cashPaymentAmount;

    	YerothUtils::startTransaction();

    	success = YerothUtils::execQuery(queryStr, _logger);

    	success = success && createHistoryPaymentForCustomerAccount(paymentInfo);

    	YerothUtils::commitTransaction();
    }
    else
    {
    	QString msg(QString(QObject::trUtf8("l'entreprise '%1' n'existe pas dans la base de données !"))
    					.arg(lineEdit_nom_entreprise->text()));

    	YerothQMessageBox::information(this, QObject::trUtf8("entreprise non existante"), msg);
    }

    if (true == success)
    {
    	QString msg(QString(QObject::trUtf8("%1 (%2) a été ajouté au compte client %3"))
    					.arg(lineEdit_versement_comptant->text(),
    						 YerothERPConfig::currency,
							 lineEdit_nom_entreprise->text()));

    	lineEdit_nom_payeur->clear();
    	lineEdit_nom_entreprise->clear();
    	lineEdit_versement_comptant->clear();

    	afficherClients();

		YerothQMessageBox::information(this, QObject::trUtf8("paiement effectué"), msg);
    }
    else
    {
    	QString msg(QString(QObject::trUtf8("Erreur lors du paiement '%1 (%2)' pour le compte client '%3'"))
    					.arg(lineEdit_versement_comptant->text(),
    						 YerothERPConfig::currency,
							 lineEdit_nom_entreprise->text()));

		YerothQMessageBox::information(this, QObject::trUtf8("paiement pas effectué"), msg);
    }

	return success;
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
	lineEdit_nom_payeur->setPlaceholderText(QObject::tr("nom du payeur"));

	lineEdit_versement_comptant->setPlaceholderText(QObject::tr("montant à payer"));
	lineEdit_versement_comptant->setValidator(&YerothUtils::DoubleValidator);

	lineEdit_resultat_filtre->setValidator(&YerothUtils::DoubleValidator);

	updateLineEditRechercheNomEntreprise();
}


void YerothERPClientsWindow::setupLineEditsQCompleters()
{
	lineEdit_nom_entreprise->enableForSearch(QObject::trUtf8("nom de l'entreprise"));
	lineEdit_nom_entreprise->setupMyStaticQCompleter(_allWindows->CLIENTS,
															YerothDatabaseTableColumn::NOM_ENTREPRISE,
															false,
															true);

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

	afficherClients(*_curClientsTableModel);

    lineEdit_recherche_nom_entreprise->setFocus();

	setVisible(true);
}


void YerothERPClientsWindow::rendreInvisible()
{
	lineEdit_nom_payeur->clear();
	lineEdit_versement_comptant->clear();
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_payer->disable(this);

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

    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));

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

    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));

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

    pushButton_payer->disable(this);

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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_payer->disable(this);

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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_payer->disable(this);

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

    int rowCount = clientSqlTableModel.rowCount();

    double totalCredit = 0.0;
    double curCompteClient = 0.0;
    QSqlRecord aQSqlRecord;

    for (int k = 0; k < rowCount; ++k)
    {
    	aQSqlRecord = clientSqlTableModel.record(k);

    	curCompteClient = GET_SQL_RECORD_DATA(aQSqlRecord, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

    	if (curCompteClient < 0)
    	{
    		totalCredit += curCompteClient;
    	}
    }

    lineEdit_debit_total->setText(GET_CURRENCY_STRING_NUM(totalCredit));
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
    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        if (k == YerothERPClientsWindow::PDF_LISTING_COLUMN_REFERENCE_CLIENT ||
        	k == YerothERPClientsWindow::PDF_LISTING_COLUMN_NOM_ENTREPRISE   ||
			k == YerothERPClientsWindow::PDF_LISTING_COLUMN_NOM_REPRESENTANT )
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

    for (int k = 0; k < texTableColumnCount; ++k)
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

