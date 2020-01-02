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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_payer->disable(this);

    pushButton_achats->disable(this);
    pushButton_marchandises->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_historique_paiements->disable(this);

    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);


    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMarchandises, SIGNAL(triggered()), this, SLOT(afficherMarchandises()));
    connect(actionCreerCompteClient, SIGNAL(triggered()), this, SLOT(creerCompteClient()));
    connect(actionPaiements, SIGNAL(triggered()), this, SLOT(paiements()));
    connect(actionVentes, SIGNAL(triggered()), this, SLOT(ventes()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionAchats, SIGNAL(triggered()), this, SLOT(achats()));
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


void YerothERPClientsWindow::contextMenuEvent(QContextMenuEvent * event)
{
    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            QMenu menu(this);
            menu.setPalette(toolBar_clientsWindow->palette());
            menu.addAction(actionCreerCompteClient);
            menu.exec(event->globalPos());
        }
    }
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


void YerothERPClientsWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    if (this->getLastListerSelectedRow() > -1 && _curClientsTableModel->rowCount() > 0)
    {
        _allWindows->_clientsDetailWindow->rendreVisible(getLastListerSelectedRow(),
        												_curStocksTableModel,
														_curClientsTableModel);
        this->rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un compte client"),
                                  QObject::trUtf8("Sélectionnez un compte client à afficher les détails."));
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
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un stock"),
                                  QObject::trUtf8("Sélectionnez un stock à afficher les détails."));
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_payer->disable(this);

    pushButton_achats->disable(this);
    pushButton_marchandises->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_historique_paiements->disable(this);

    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));

    pushButton_achats->enable(this, SLOT(achats()));
    pushButton_marchandises->enable(this, SLOT(afficherMarchandises()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_compte_client->enable(this, SLOT(creerCompteClient()));
    pushButton_historique_paiements->enable(this, SLOT(paiements()));

    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothERPClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));

    pushButton_achats->enable(this, SLOT(achats()));
    pushButton_marchandises->enable(this, SLOT(afficherMarchandises()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_compte_client->enable(this, SLOT(creerCompteClient()));
    pushButton_historique_paiements->enable(this, SLOT(paiements()));

    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothERPClientsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_payer->disable(this);

    pushButton_achats->disable(this);
    pushButton_marchandises->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_historique_paiements->disable(this);

    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPClientsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_payer->disable(this);

    pushButton_achats->disable(this);
    pushButton_marchandises->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_historique_paiements->disable(this);

    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_payer->disable(this);

    pushButton_achats->disable(this);
    pushButton_marchandises->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_historique_paiements->disable(this);

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

    tableView_clients->hideColumn(0);
    tableView_clients->hideColumn(4);
    tableView_clients->hideColumn(5);
    tableView_clients->hideColumn(6);
    tableView_clients->hideColumn(7);
    tableView_clients->hideColumn(8);
    tableView_clients->hideColumn(9);
    tableView_clients->hideColumn(10);
    tableView_clients->hideColumn(11);
    tableView_clients->hideColumn(12);
    tableView_clients->hideColumn(13);
    tableView_clients->hideColumn(14);

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

    tableView_clients->selectRow(tableView_clients->lastSelectedRow());
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

    tableColumnsToIgnore << 0 << 4 << 5 << 6
    					 << 7 << 8 << 9 << 10
						 << 11 << 12 << 13 << 14;

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


bool YerothERPClientsWindow::imprimer_document()
{
    _logger->log("imprimer_document");

    QString latexFileNamePrefix("yeroth-erp-comptes-clients");

    QList<int> tableColumnsToIgnore;

    tableColumnsToIgnore << 0 << 4 << 5 << 6
    					 << 7 << 8 << 9 << 10
						 << 11 << 12 << 13 << 14;

    QString pdfStockFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-comptes-clients");

    pdfStockFileName = YerothUtils::prindDocumentFromTableView(this,
    														   *tableView_clients,
															   tableColumnsToIgnore,
															   &YerothUtils::getComptesClientsListingTexTableString,
															   &YerothUtils::getComptesClientsFRTexDocumentString,
															   latexFileNamePrefix);
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-customer-account");

    pdfStockFileName = YerothUtils::prindDocumentFromTableView(this,
    														   *tableView_clients,
															   tableColumnsToIgnore,
															   &YerothUtils::getComptesClientsListingTexTableString,
															   &YerothUtils::getComptesClientsENTexDocumentString,
															   latexFileNamePrefix);
#endif

    if (pdfStockFileName.isEmpty())
    {
    	return false;
    }

    YerothERPProcess::startPdfViewerProcess(pdfStockFileName);

    return true;
}

