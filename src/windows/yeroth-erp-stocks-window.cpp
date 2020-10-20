/*
 * yeroth-erp-stocks-window.cpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-stocks-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-style.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/dialogs/yeroth-erp-generic-select-db-field-dialog.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/QThread>

#include <QtCore/QProcess>

#include <QtCore/qmath.h>

#include <QtWidgets/QCheckBox>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <unistd.h>


/**
 * yeroth-erp-windows.hpp cannot be included in
 * the header file because it will caused a circular
 * dependency that will lead to an unsuccessful
 * compilation.
 */


YerothStocksWindow::YerothStocksWindow()
:YerothWindowsCommons("yeroth-erp-fichier-stocks"),
 YerothAbstractClassYerothSearchWindow(_allWindows->STOCKS),
 _logger(new YerothLogger("YerothStocksWindow")),
 _pushButton_stocks_filtrer_font(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("fiche des stocks"));

    setupUi(this);

    mySetupUi(this);

    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_stocks);

    _yerothTableView_FROM_WINDOWS_COMMONS = tableView_stocks;

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0,
                                    		COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(_allWindows->STOCKS);


    _lineEditsToANDContentForSearch.insert(&lineEdit_stock_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _yeroth_WINDOW_references_dbColumnString.insert(YerothDatabaseTableColumn::REFERENCE);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(_allWindows->STOCKS);

    reinitialiser_champs_db_visibles();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    _pushButton_stocks_filtrer_font = new QFont(pushButton_stocks_filtrer->font());

    tableView_stocks->setSqlTableName(&YerothERPWindows::STOCKS);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, false);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_stocks_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_inventaire_des_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_reinitialiser->disable(this);

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionMarchandises, SIGNAL(triggered()), this, SLOT(afficherMarchandises()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

    connect(actionAfficher_lhistorique_de_ce_stock, SIGNAL(triggered()),
    		this, SLOT(private_slot_afficher_historique_du_stock()));

    connect(actionModifier_ce_stock, SIGNAL(triggered()),
    		this, SLOT(modifier_les_articles()));

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));
    connect(actionReinitialiserElementsDeFiltrage, SIGNAL(triggered()), this, SLOT(reinitialiser_elements_filtrage()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION
connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
#endif

    connect(actionAfficher_stock_au_detail, SIGNAL(triggered()),
    		this, SLOT(afficher_au_detail()));

    connect(actionSupprimer_ce_stock, SIGNAL(triggered()),
    		this, SLOT(supprimer_ce_stock()));

    connect(comboBox_strategie_de_stocks, SIGNAL(currentTextChanged(const QString &)),
    		this, SLOT(gererChoixStrategieGestionDesStocks()));

    connect(tableView_stocks, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    connect(actionAfficher_les_stocks_en_alerte_de_stock, SIGNAL(triggered()),
    		this, SLOT(filtrer_stocks_en_alerte_de_stock()));

    connect(actionAfficher_les_stocks_perimes, SIGNAL(triggered()),
    		this, SLOT(filtrer_stocks_perimes_seulement()));

    setupShortcuts();
}


YerothStocksWindow::~YerothStocksWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _logger;
}


void YerothStocksWindow::private_slot_afficher_historique_du_stock()
{
	//qDebug() << QString("YerothStocksWindow | private_slot_afficher_historique_du_stock");

	int lastSelectedRow__ID = getLastListerSelectedRow__ID();

	//qDebug() << QString("lastSelectedRow__ID: %1")
	//				.arg(QString::number(lastSelectedRow__ID));

	if (0 != _curStocksTableModel && _curStocksTableModel->rowCount() > 0 && lastSelectedRow__ID > -1)
	{
	    QSqlRecord record = _curStocksTableModel->record(lastSelectedRow__ID);

	    QString historiqueStockSelectionne(
	    			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::HISTORIQUE_STOCK));

	    int stockId = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID).toInt();

	    QString stockReference(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));

	    QString stockDesignation(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

//	    qDebug() << QString("++ historiqueStockSelectionne: %1")
//	    				.arg(historiqueStockSelectionne);

	    YerothHistoriqueStock::listHistoriqueDuStock(historiqueStockSelectionne,
	    											 stockReference,
													 QString::number(stockId),
													 stockDesignation);
	}
	else
	{
	    YerothQMessageBox::information(this,
	    		QObject::trUtf8("stocks - historique du stock"),
				QObject::trUtf8("Il n'y a pas de stocks listés !"));
	}
}


bool YerothStocksWindow::filtrer_stocks_en_alerte_de_stock()
{
	QString filterString;

	filterString.append(QString("%1 <= %2")
			.arg(YerothDatabaseTableColumn::QUANTITE_TOTALE,
				 YerothDatabaseTableColumn::STOCK_DALERTE));

//	qDebug() << QString("++ filtrer_stocks_en_alerte_de_stock | filterString: %1")
//    						.arg(filterString);

	_curStocksTableModel->yerothSetFilter_WITH_where_clause(filterString);

	int resultRows = _curStocksTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);
	}

	afficherStocks();

	if (resultRows > 0)
	{
		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "stocks en état d'alerte de stocks");
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "stocks en état d'alerte de stocks");
	}

	set_filtrer_font();

	return true;
}


bool YerothStocksWindow::filtrer_stocks_perimes_seulement()
{
	QString filterString;

	filterString.append(QString("%1 < '%2'")
			.arg(YerothDatabaseTableColumn::DATE_PEREMPTION,
				 DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE)));

	//qDebug() << QString("++ filtrer_stocks_perimes_seulement | filterString: %1")
    //						.arg(filterString);

	reinitialiser_recherche();

	_curStocksTableModel->yerothSetFilter_WITH_where_clause(filterString);

	int resultRows = _curStocksTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);
	}

	afficherStocks();

	if (resultRows > 0)
	{
		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "stocks périmés");
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "stocks périmés");
	}

	set_filtrer_font();

	return true;
}


bool YerothStocksWindow::filtrer_stocks()
{
	QString stockTableColumnValue(lineEdit_stocks_element_de_stock_resultat->text());

	if (stockTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
				QObject::trUtf8("filtrer"),
				msg);
		return false;
	}

	reinitialiser_recherche();

	QString stockTableColumnProperty(comboBox_stocks_element_de_stock->currentText());

	QString mathOperator(comboBox_stocks_element_de_stock_condition->currentText());

	QString REAL_DB_ID_NAME_stockTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(stockTableColumnProperty));

	QString filterString;

	filterString.append(QString("%1 %2 %3")
			.arg(REAL_DB_ID_NAME_stockTableColumnProperty,
					mathOperator,
					stockTableColumnValue));

	_curStocksTableModel->yerothSetFilter_WITH_where_clause(filterString);

	int resultRows = _curStocksTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);

		afficherStocks();

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "stocks - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "stocks - filtrer");

		return false;
	}

	set_filtrer_font();

	return false;
}


void YerothStocksWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();
	afficherStocks();
}


void YerothStocksWindow::activerComboBoxStrategieDeGestionDesStocks()
{
    comboBox_strategie_de_stocks->setVisible(true);
    comboBox_strategie_de_stocks->setYerothEnabled(true);
    lineEdit_stock_terme_recherche->setFixedWidth(663);
}


void YerothStocksWindow::desactiverComboBoxStrategieDeGestionDesStocks()
{
    comboBox_strategie_de_stocks->setVisible(false);
    comboBox_strategie_de_stocks->setYerothEnabled(false);
    lineEdit_stock_terme_recherche->setFixedWidth(778);
}


void YerothStocksWindow::setComboBoxStrategieDeStocks()
{
    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::salesStrategy,
    										YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL))
    {
        comboBox_strategie_de_stocks->setCurrentIndex(YerothUtils::STRATEGIE_ALL_COMBOBOX_INDEX);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy,
            		 	 	 	 	YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO))
    {
        comboBox_strategie_de_stocks->setCurrentIndex(YerothUtils::STRATEGIE_FEFO_COMBOBOX_INDEX);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy,
            		 	 	 	 	YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO))
    {
        comboBox_strategie_de_stocks->setCurrentIndex(YerothUtils::STRATEGIE_FIFO_COMBOBOX_INDEX);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy,
            		 	 	 	 	YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO))
    {
        comboBox_strategie_de_stocks->setCurrentIndex(YerothUtils::STRATEGIE_LIFO_COMBOBOX_INDEX);
    }
}

void YerothStocksWindow::populateComboBoxes()
{
	QStringList aQStringList;

	aQStringList.append(_varchar_dbtable_column_list.values());

	aQStringList.removeAll(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT);
	aQStringList.removeAll(YerothDatabaseTableColumn::ENREGISTREUR_STOCK);
	aQStringList.removeAll(YerothDatabaseTableColumn::HISTORIQUE_STOCK);
	aQStringList.removeAll(YerothDatabaseTableColumn::LOCALISATION);
	aQStringList.removeAll(YerothDatabaseTableColumn::LOCALISATION_STOCK);
	aQStringList.removeAll(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);

//	qDebug() << "++ test: " << aQStringList;

	QString aDBColumnElementString;

	for (int k = 0; k < aQStringList.size(); ++k)
	{
		aDBColumnElementString = aQStringList.at(k);

		if (!YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::REFERENCE, aDBColumnElementString))
		{
			comboBox_element_string_db
				->addItem(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(aDBColumnElementString));
		}
	}

	comboBox_element_string_db
		->insertItem(0, YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::REFERENCE));

	comboBox_element_string_db->setCurrentIndex(0);


	aQStringList.clear();

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::MONTANT_TVA));

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::PRIX_VENTE));

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::PRIX_UNITAIRE));

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::QUANTITE_TOTALE));

	comboBox_stocks_element_de_stock->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_stocks_element_de_stock_condition->addItems(aQStringList);

    comboBox_strategie_de_stocks->clear();
    comboBox_strategie_de_stocks->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

    //index 0 of enum enum_comboBoxStrategyIndex
    comboBox_strategie_de_stocks->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);
    comboBox_strategie_de_stocks->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);
    comboBox_strategie_de_stocks->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    desactiverComboBoxStrategieDeGestionDesStocks();
}


void YerothStocksWindow::setupLineEdits()
{
    lineEdit_stock_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (description de l'article (ou service))"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::trUtf8("valeur à rechercher"));

	lineEdit_stocks_element_de_stock_resultat->setValidator(&YerothUtils::DoubleValidator);

	lineEdit_nombre_de_stocks->setYerothEnabled(false);

	MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_stocks_nombre_de_lignes_par_page, tableView_stocks);
}


void YerothStocksWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothStocksWindow::textChangedSearchLineEditsQCompleters()
{
	lineEdit_stocks_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_stock_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
        	partSearchTerm = searchTermList.at(k);
        	//qDebug() << "++ searchTermList: " << partSearchTerm;

        	_searchFilter.append(QString("%1")
        							.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, partSearchTerm)));

        	if (k != lastIdx)
        	{
        		_searchFilter.append(" AND ");
        	}
        }
    }


    YerothWindowsCommons::setYerothLineEditQCompleterSearchFilter(_searchFilter);


    YerothLineEdit *aYerothLineEdit = 0;

    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    QMapIterator <YerothLineEdit **, QString> it(_lineEditsToANDContentForSearch);

    while (it.hasNext())
    {
    	it.next();

    	aYerothLineEdit = *it.key();

    	correspondingDBFieldKeyValue = it.value();

    	if (0 != aYerothLineEdit)
    	{
    		aTableColumnFieldContentForANDSearch = aYerothLineEdit->text();

    		if (!correspondingDBFieldKeyValue.isEmpty() &&
    				!aTableColumnFieldContentForANDSearch.isEmpty()	)
    		{
    			if (!_searchFilter.isEmpty())
    			{
    				_searchFilter.append(" AND ");
    			}

    			_searchFilter.append(GENERATE_SQL_IS_STMT(correspondingDBFieldKeyValue,
    					aTableColumnFieldContentForANDSearch));
    		}
    	}
    }

    _yerothSqlTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);

    if (_yerothSqlTableModel->select())
    {
    	setLastListerSelectedRow__ID(0);
    	afficherStocks(*_yerothSqlTableModel);
    }
    else
    {
        qDebug() << QString("++ YerothStocksWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }
}


void YerothStocksWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

    _visibleDBFieldColumnStrList
			<< YerothDatabaseTableColumn::REFERENCE
			<< YerothDatabaseTableColumn::DESIGNATION
			<< YerothDatabaseTableColumn::CATEGORIE
			<< YerothDatabaseTableColumn::PRIX_UNITAIRE
			<< YerothDatabaseTableColumn::MONTANT_TVA
			<< YerothDatabaseTableColumn::PRIX_VENTE
			<< YerothDatabaseTableColumn::QUANTITE_TOTALE
			<< YerothDatabaseTableColumn::DATE_ENTREE
			<< YerothDatabaseTableColumn::DATE_PEREMPTION;
}


void YerothStocksWindow::contextMenuEvent(QContextMenuEvent * event)
{
    YerothPOSUser *aUser = _allWindows->getUser();
    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            if (tableView_stocks->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_stocksWindow->palette());
                menu.addAction(actionAfficher_stock_au_detail);
                menu.addAction(actionAfficher_lhistorique_de_ce_stock);
                menu.addAction(actionModifier_ce_stock);
                menu.addAction(actionSupprimer_ce_stock);
                menu.exec(event->globalPos());
            }
        }
        if (aUser->isVendeur())
        {
            if (tableView_stocks->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_stocksWindow->palette());
                menu.addAction(actionAfficher_stock_au_detail);
                menu.exec(event->globalPos());
            }
        }
        else if (aUser->isGestionaireDesStocks())
         {
             if (tableView_stocks->rowCount() > 0)
             {
                 QMenu menu(this);
                 menu.setPalette(toolBar_stocksWindow->palette());
                 menu.addAction(actionAfficher_stock_au_detail);
                 menu.addAction(actionAfficher_lhistorique_de_ce_stock);
                 menu.addAction(actionModifier_ce_stock);
                 menu.addAction(actionSupprimer_ce_stock);
                 menu.exec(event->globalPos());
             }
         }
        else if (aUser->isMagasinier())
         {
             if (tableView_stocks->rowCount() > 0)
             {
                 QMenu menu(this);
                 menu.setPalette(toolBar_stocksWindow->palette());
                 menu.addAction(actionAfficher_stock_au_detail);
                 menu.exec(event->globalPos());
             }
         }
    }
}


void YerothStocksWindow::hideEvent(QHideEvent * hideEvent)
{
	_selectExportDBQDialog->close();

	_allWindows->_historiqueDuStockWindow->close();
}


void YerothStocksWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setYerothSqlTableModel(stocksTableModel);

    _curStocksTableModel = stocksTableModel;

    lineEdit_stock_terme_recherche->setFocus();

    setVisible(true);

    _curStocksTableModel->easySelect();

    afficherStocks(YerothERPConfig::salesStrategy);
}


void YerothStocksWindow::gererChoixStrategieGestionDesStocks()
{
	_localStrategy = comboBox_strategie_de_stocks->currentText();

	afficherStocks(_localStrategy);
}


void YerothStocksWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    desactiverComboBoxStrategieDeGestionDesStocks();

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_reinitialiser->disable(this);
    pushButton_stocks_filtrer->disable(this);
    pushButton_stocks_reinitialiser_filtre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->disable(this);
    pushButton_inventaire_des_stocks->disable(this);
    pushButton_sortir->disable(this);
}

void YerothStocksWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    activerComboBoxStrategieDeGestionDesStocks();

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curStocksTableModel)

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_inventaire_des_stocks->enable(this, SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}


void YerothStocksWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    activerComboBoxStrategieDeGestionDesStocks();

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curStocksTableModel)

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_inventaire_des_stocks->enable(this, SLOT(afficherMarchandises()));
    pushButton_sortir->disable(this);
}


void YerothStocksWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    activerComboBoxStrategieDeGestionDesStocks();

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curStocksTableModel)

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_inventaire_des_stocks->enable(this, SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}

void YerothStocksWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    desactiverComboBoxStrategieDeGestionDesStocks();

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curStocksTableModel)

    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_inventaire_des_stocks->disable(this);
    pushButton_sortir->enable(this, SLOT(sortir()));
}

void YerothStocksWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    desactiverComboBoxStrategieDeGestionDesStocks();

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_reinitialiser->disable(this);
    pushButton_stocks_filtrer->disable(this);
    pushButton_stocks_reinitialiser_filtre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_inventaire_des_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_menu_principal->disable(this);
}


void YerothStocksWindow::afficher_au_detail()
{
    if (getLastListerSelectedRow__ID() > -1 && _curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_detailWindow->rendreVisible(_curStocksTableModel);
        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::trUtf8("détails d'un stock"),
                                  QObject::trUtf8("Sélectionnez un stock à afficher les détails."));
    }
}


void YerothStocksWindow::afficher_au_detail(const QModelIndex & modelIndex)
{
    setLastListerSelectedRow__ID(modelIndex.row());

    tableView_stocks->selectRow(getLastListerSelectedRow__ID());

    if (getLastListerSelectedRow__ID() > -1 && _curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_detailWindow->rendreVisible(_curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un stock"),
                                  QObject::trUtf8("Sélectionnez un stock à afficher les détails."));
    }
}


/**
 * La suppression d'un stock entraine automatiquement la
 * suppression de l'achat correspondant
 */
void YerothStocksWindow::supprimer_ce_stock()
{
    _logger->log("supprimer_ce_stock");

    unsigned rowToRemove = tableView_stocks->lastSelectedRow__ID();

    QSqlRecord record = _curStocksTableModel->record(rowToRemove);

    QString msgSupprimer;

    QString serviceOuArticle;

    bool is_service = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::IS_SERVICE).toInt();

    if (is_service)
    {
    	serviceOuArticle = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE);
        msgSupprimer.append(QString(QObject::trUtf8("Poursuivre avec la suppression du service '%1' ?"))
        						.arg(serviceOuArticle));
    }
    else
    {
    	serviceOuArticle = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION);

        msgSupprimer.append(QString(QObject::trUtf8("Poursuivre avec la suppression du stock '%1' ?"))
        						.arg(serviceOuArticle));
    }

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                       QObject::trUtf8
                                       ("suppression d'un stock (service)"), msgSupprimer,
                                       QMessageBox::Cancel, QMessageBox::Ok))
    {
		_logger->debug("supprimer_ce_stock", QString("rowToRemove: %1").arg(rowToRemove));

        bool resRemoved = _curStocksTableModel->removeRow(rowToRemove);
        //qDebug() << "YerothStocksWindow::supprimer_ce_stock() " << resRemoved;

        if (resRemoved && _curStocksTableModel->select())
        {
        	if (!is_service)
        	{
        		QString deleteAchatsRowQueryStr(QString("DELETE FROM %1 WHERE stocks_id = '%2';")
        											.arg(_allWindows->ACHATS,
        												 GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID)));

        		YerothUtils::execQuery(deleteAchatsRowQueryStr);
        	}

            msgSupprimer.clear();

            msgSupprimer.append(QString(QObject::trUtf8("Le stock (service) '%1' a été supprimé !"))
            						.arg(serviceOuArticle));

            YerothQMessageBox::information(this,
                                          QObject::tr("suppression d'un stock (service)"),
                                          msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();

            msgSupprimer.append(QString(QObject::trUtf8("Le stock (service) '%1' ne pouvait pas être supprimé !"))
            						.arg(serviceOuArticle));

            YerothQMessageBox::warning(this,
                                       QObject::trUtf8("suppression d'un stock (service)"),
                                       msgSupprimer);
        }

        setupLineEditsQCompleters((QObject *)this);

        afficherStocks(*_curStocksTableModel);
    }
    else
    {
    }
}


void YerothStocksWindow::reinitialiser_elements_filtrage()
{
    lineEdit_stocks_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothStocksWindow::reinitialiser_recherche()
{
	lineEdit_nom_element_string_db->clear();

    lineEdit_stocks_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *)this);

    setComboBoxStrategieDeStocks();

    afficherStocks();
}


void YerothStocksWindow::entrer()
{
    if (getLastListerSelectedRow__ID() > -1 && _curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel, true);
        rendreInvisible();
    }
    else
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
        rendreInvisible();
    }
}


void YerothStocksWindow::modifier_les_articles()
{
    if (_curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_modifierWindow->rendreVisible(_curStocksTableModel);
        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, "modifier un stock",
                                  QObject::trUtf8("Sélectionnez un stock à modifier."));
    }
}


void YerothStocksWindow::afficherStocks(YerothSqlTableModel & sqlTableModel,
									    QString localVisibleStrategy /* = YerothUtils::EMPTY_STRING */)
{
    _curStocksTableModel = &sqlTableModel;

    QMultiMap<QString, QString> stockNameToStockID_in_out;

    QString currentStockListingStrategy(localVisibleStrategy);

    if (localVisibleStrategy.isEmpty())
    {
        currentStockListingStrategy = YerothERPConfig::salesStrategy;
    }

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
    										currentStockListingStrategy) 					||
    	YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
    		    							currentStockListingStrategy) 					||
		YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
											currentStockListingStrategy))
    {
    	tableView_stocks->setSortingEnabled(false);
    }
    else	//YerothConfig::STRATEGIE_VENTE_SORTIE_ALL
    {
    	tableView_stocks->setSortingEnabled(true);
    }

    tableView_stocks->queryYerothTableViewCurrentPageContentRow(*_curStocksTableModel,
    															currentStockListingStrategy);

    tableView_show_or_hide_columns(*tableView_stocks);

    int rowCount = tableView_stocks->rowCount();

    lineEdit_nombre_de_stocks->setText(GET_NUM_STRING(rowCount));

    setWindowTitle(YerothUtils::getWindowTitleWithStrategy(this, localVisibleStrategy));
}


void YerothStocksWindow::afficherStocks(QString strategieGlobale /* = YerothUtils::EMPTY_STRING */)
{
	QString curStrategy = strategieGlobale;

	if (curStrategy.isEmpty())
	{
		//set local visible strategy
		curStrategy = _localStrategy;
	}

	comboBox_strategie_de_stocks
		->setCurrentIndex(YerothUtils::getComboBoxDatabaseQueryValue(curStrategy,
						  YerothUtils::_strategieindexToUserViewString));

	afficherStocks(*_curStocksTableModel, curStrategy);
}


void YerothStocksWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
    	_pushButton_stocks_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_stocks_filtrer_font->setUnderline(false);
    }

    pushButton_stocks_filtrer->setFont(*_pushButton_stocks_filtrer_font);
}


bool YerothStocksWindow::export_csv_file()
{
	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_stocks,
										   tableColumnsToIgnore,
										   "yeroth-erp-fichier-stocks-format-csv",
										   "fiche des stocks");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_stocks,
										   tableColumnsToIgnore,
										   "yeroth-erp-stocks-listing-csv-format",
										   "stock listing");
#endif

	return success;
}


bool YerothStocksWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_lister_stock_tex;
	return YerothWindowsCommons::imprimer_pdf_document();
}
