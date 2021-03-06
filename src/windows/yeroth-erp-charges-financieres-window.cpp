/*
 * yeroth-erp-charges-financieres-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-charges-financieres-window.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-style.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/QThread>

#include <QtCore/QProcess>

#include <QtCore/qmath.h>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <unistd.h>


/**
 * yeroth-erp-windows.hpp cannot be included in
 * the header file because it will caused a circular
 * dependency that will lead to an unsuccessful
 * compilation.
 */


unsigned int YerothChargesFinancieresWindow::PDF_LISTING_COLUMN_STOCKS_ID(1);

unsigned int YerothChargesFinancieresWindow::PDF_LISTING_COLUMN_REFERENCE(3);

unsigned int YerothChargesFinancieresWindow::PDF_LISTING_COLUMN_DESIGNATION(2);

unsigned int YerothChargesFinancieresWindow::PDF_LISTING_COLUMN_CATEGORIE(4);


YerothChargesFinancieresWindow::YerothChargesFinancieresWindow()
:YerothWindowsCommons("yeroth-erp-achats-de-services"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::CHARGES_FINANCIERES),
 _logger(new YerothLogger("YerothAchatsDEServicesWindow")),
 _pushButton_charges_financieres_filtrer_font(0),
 _curAchatsDEServicesSqlTableModel(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("fiche des charges financières"));

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_charges_financieres);


    _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
		<< pushButton_afficher
		<< pushButton_supprimer;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionExporter_au_format_csv
		<< action_parametrer_les_impressions
		<< actionAfficherPDF
		<< actionSupprimerUnAchatDeService
		<< actionAfficher_achat_au_detail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_charges_financieres)

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_170, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(YerothDatabase::CHARGES_FINANCIERES);


    _lineEditsToANDContentForSearch.insert(&lineEdit_charges_financieres_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _yeroth_WINDOW_references_dbColumnString.insert(YerothDatabaseTableColumn::REFERENCE);
    _yeroth_WINDOW_references_dbColumnString.insert(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerothDatabase::CHARGES_FINANCIERES);

    reinitialiser_colones_db_visibles();

    _curAchatsDEServicesSqlTableModel = &_allWindows->getSqlTableModel_charges_financieres();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setupDateTimeEdits();

    _pushButton_charges_financieres_filtrer_font = new QFont(pushButton_charges_financieres_filtrer->font());

    tableView_charges_financieres->setSqlTableName(&YerothDatabase::CHARGES_FINANCIERES);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_achat_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnAchatDeService, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_charges_financieres_filtrer->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_paiements->disable(this);
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerUnAchatDeService, SIGNAL(triggered()), this, SLOT(supprimer_un_achat_au_fournisseur()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionPaiements, SIGNAL(triggered()), this, SLOT(paiements()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this, SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(print_PDF_PREVIOUSLY_SETUP()));
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

	connect(actionAfficher_achat_au_detail, SIGNAL(triggered()),
			this, SLOT(afficher_au_detail()));

    connect(tableView_charges_financieres, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(YerothDatabaseTableColumn::STOCKS_ID);

    setupShortcuts();
}

YerothChargesFinancieresWindow::~YerothChargesFinancieresWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _logger;
}


bool YerothChargesFinancieresWindow::filtrer_achats()
{
	QString achatsTableColumnValue(comboBox_element_charges_financieres_condition->currentText());

	if (achatsTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
				QObject::trUtf8("filtrer"),
				msg);

		return false;
	}

	reinitialiser_recherche();

	QString achatsTableColumnProperty(comboBox_element_charges_financieres->currentText());

	QString mathOperator(comboBox_element_charges_financieres_condition->currentText());

	QString REAL_DB_ID_NAME_achatsTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(achatsTableColumnProperty));

	QString filterString;

	filterString.append(QString("%1 %2 %3")
			.arg(REAL_DB_ID_NAME_achatsTableColumnProperty,
					mathOperator,
					achatsTableColumnValue));

	_curAchatsDEServicesSqlTableModel->yerothSetFilter_WITH_where_clause(filterString);

	int resultRows = _curAchatsDEServicesSqlTableModel->easySelect();

	if (resultRows > 0)
	{
		setCurrentlyFiltered(true);

		lister_les_elements_du_tableau(*_curAchatsDEServicesSqlTableModel);

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "achats-de-services - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "achats-de-services - filtrer");

		return false;
	}

	set_filtrer_font();

	return false;
}


void YerothChargesFinancieresWindow::populateComboBoxes()
{
	QStringList aQStringList;

	aQStringList.append(_varchar_dbtable_column_name_list.values());

	aQStringList.removeAll(YerothDatabaseTableColumn::STATUT_DE_LACHAT_AU_FOURNISSEUR);
	aQStringList.removeAll(YerothDatabaseTableColumn::NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT);
	aQStringList.removeAll(YerothDatabaseTableColumn::ENREGISTREUR_STOCK);
	aQStringList.removeAll(YerothDatabaseTableColumn::LOCALISATION);
	aQStringList.removeAll(YerothDatabaseTableColumn::LOCALISATION_STOCK);

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

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE));

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::MONTANT_TVA));

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::PRIX_VENTE));

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::PRIX_UNITAIRE));

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::QUANTITE_TOTALE));

    comboBox_element_charges_financieres->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_element_charges_financieres_condition->addItems(aQStringList);
}


void YerothChargesFinancieresWindow::setupLineEdits()
{
	_QLINEEDIT_nombre_de_lignes_par_page = lineEdit_charges_financieres_nombre_de_lignes_par_page;

	lineEdit_charges_financieres_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (désignation de l'article)"));

	lineEdit_nom_element_string_db->enableForSearch(QObject::trUtf8("valeur à rechercher"));

	lineEdit_charges_financieres_nombre_dachats->setYerothEnabled(false);

	lineEdit_charges_financieres_total_achats->setYerothEnabled(false);

	comboBox_element_charges_financieres_condition->setValidator(&YerothUtils::POSITIVE_DoubleValidator);

	MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_charges_financieres_nombre_de_lignes_par_page, tableView_charges_financieres);
}


void YerothChargesFinancieresWindow::setupDateTimeEdits()
{
    dateEdit_charges_financieres_debut->setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

    dateEdit_charges_financieres_fin->setStartDate(GET_CURRENT_DATE);

    _charges_financieresDateFilter.clear();

    _charges_financieresDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    					.arg(YerothDatabaseTableColumn::DATE_DE_COMMANDE,
    						 DATE_TO_DB_FORMAT_STRING(dateEdit_charges_financieres_debut->date()),
							 YerothDatabaseTableColumn::DATE_DE_COMMANDE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_charges_financieres_fin->date())));

    connect(dateEdit_charges_financieres_debut,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_charges_financieres_fin,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));
}


void YerothChargesFinancieresWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 		(*actionAppeler_aide);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe			(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothChargesFinancieresWindow::resetFilter(YerothSqlTableModel * achatsTableModel)
{
	_curAchatsDEServicesSqlTableModel = achatsTableModel;

    if (0 != _curAchatsDEServicesSqlTableModel)
    {
    	_curAchatsDEServicesSqlTableModel->resetFilter();
    }

    lineEdit_charges_financieres_terme_recherche->myClear();

    dateEdit_charges_financieres_debut->reset();
    dateEdit_charges_financieres_fin->reset();
}


void YerothChargesFinancieresWindow::slot_reinitialiser_colones_db_visibles()
{
	reinitialiser_colones_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();

	if (0 != _curAchatsDEServicesSqlTableModel)
	{
		lister_les_elements_du_tableau(*_curAchatsDEServicesSqlTableModel);
	}
}


void YerothChargesFinancieresWindow::textChangedSearchLineEditsQCompleters()
{
	lineEdit_element_charges_financieres_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_charges_financieres_terme_recherche->text());

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
        							.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESIGNATION, partSearchTerm)));

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

    QString finalSearchFilter(_charges_financieresDateFilter);

    if (!_searchFilter.isEmpty())
    {
    	QString searchFilterWithDate(QString("%1 AND (%2)")
    									.arg(_charges_financieresDateFilter,
    										 _searchFilter));

    	finalSearchFilter = searchFilterWithDate;
    }

    _yerothSqlTableModel->yerothSetFilter_WITH_where_clause(finalSearchFilter);

    if (_yerothSqlTableModel->select())
    {
    	lister_les_elements_du_tableau(*_yerothSqlTableModel);
    }
    else
    {
        qDebug() << QString("++ YerothAchatsDEServicesWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerothChargesFinancieresWindow::reinitialiser_colones_db_visibles()
{
	_visibleDBColumnNameStrList.clear();

	_visibleDBColumnNameStrList
		<< YerothDatabaseTableColumn::DESIGNATION
		<< YerothDatabaseTableColumn::CATEGORIE
		<< YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
		<< YerothDatabaseTableColumn::QUANTITE_TOTALE
		<< YerothDatabaseTableColumn::PRIX_DACHAT
		<< YerothDatabaseTableColumn::PRIX_VENTE
		<< YerothDatabaseTableColumn::MARGE_BENEFICIAIRE
		<< YerothDatabaseTableColumn::DATE_DE_COMMANDE;
}


void YerothChargesFinancieresWindow::contextMenuEvent(QContextMenuEvent * event)
{
    if (tableView_charges_financieres->rowCount() > 0)
    {
        QMenu menu(this);
        menu.setPalette(toolBar_charges_financieresWindow->palette());
        menu.addAction(actionAfficher_achat_au_detail);
        menu.exec(event->globalPos());
    }
}


void YerothChargesFinancieresWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setYerothSqlTableModel(_curAchatsDEServicesSqlTableModel);

    _curStocksTableModel = stocksTableModel;

    lineEdit_charges_financieres_terme_recherche->setFocus();

    setVisible(true);

    afficher_achats();

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();
}


void YerothChargesFinancieresWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_achat_au_detail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnAchatDeService, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curAchatsDEServicesSqlTableModel)

    pushButton_charges_financieres_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_charges_financieres_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));

    pushButton_supprimer->enable(this, SLOT(supprimer_un_achat_au_fournisseur()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_paiements->enable(this, SLOT(paiements()));
}


void YerothChargesFinancieresWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_achat_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnAchatDeService, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_charges_financieres_filtrer->disable(this);
    pushButton_charges_financieres_reinitialiser_filtre->disable(this);

    pushButton_supprimer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_stocks->disable(this);
    pushButton_paiements->disable(this);
    pushButton_menu_principal->disable(this);
}

void YerothChargesFinancieresWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    if (getLastListerSelectedRow__ID_AS_INTEGER() > -1 && _curAchatsDEServicesSqlTableModel->rowCount() > 0)
    {
        _allWindows->_achatsDetailWindow->rendreVisible(_curStocksTableModel,
														_curAchatsDEServicesSqlTableModel);
        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::trUtf8("détails"),
                              QObject::trUtf8("Sélectionnez 1 achat au fournisseur à afficher les détails."));

        return ;
    }
}


void YerothChargesFinancieresWindow::afficher_au_detail(const QModelIndex & modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    if (_curAchatsDEServicesSqlTableModel->rowCount() > 0)
    {
    	//qDebug() << "++ test" << modelIndex.row();
        _allWindows->_achatsDetailWindow->rendreVisible(_curStocksTableModel,
														_curAchatsDEServicesSqlTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::trUtf8("détails"),
                              QObject::trUtf8("Sélectionnez 1 achat au fournisseur à afficher les détails."));

        return ;
    }
}


void YerothChargesFinancieresWindow::refineYerothLineEdits()
{
	_charges_financieresDateFilter.clear();

	_charges_financieresDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    					.arg(YerothDatabaseTableColumn::DATE_DE_COMMANDE,
    						 DATE_TO_DB_FORMAT_STRING(dateEdit_charges_financieres_debut->date()),
							 YerothDatabaseTableColumn::DATE_DE_COMMANDE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_charges_financieres_fin->date())));

	setupLineEditsQCompleters((QObject *)this);

	afficher_achats();
}


void YerothChargesFinancieresWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    comboBox_element_charges_financieres_condition->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothChargesFinancieresWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_nom_element_string_db->clear();

    comboBox_element_charges_financieres_condition->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_charges_financieres());

    resetLineEditsQCompleters((QObject *)this);

    afficher_achats();
}


void YerothChargesFinancieresWindow::lister_les_elements_du_tableau(YerothSqlTableModel &achatSqlTableModel)
{
    int curAchatsTableModelRowCount = achatSqlTableModel.easySelect();

    double prix_dachat = 0.0;

    double montant_total = 0.0;

    QSqlRecord aRecord;

    for (int k = 0; k < curAchatsTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = achatSqlTableModel.record(k);

        prix_dachat = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::PRIX_DACHAT).toDouble();

        montant_total += prix_dachat;
    }

    lineEdit_charges_financieres_total_achats->setText(GET_CURRENCY_STRING_NUM(montant_total));


	tableView_charges_financieres->queryYerothTableViewCurrentPageContentRow(achatSqlTableModel);

    tableView_show_or_hide_columns(*tableView_charges_financieres);

    int rowCount = tableView_charges_financieres->rowCount();

    lineEdit_charges_financieres_nombre_dachats->setText(GET_NUM_STRING(rowCount));
}


void YerothChargesFinancieresWindow::afficher_stock_selectioner(const QString & stockName)
{
    _logger->log("afficher_stock_selectioner(const QString &)");

    //qDebug() << QString("afficher_stock_selectioner(%1)").arg(stockName);

    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _curAchatsDEServicesSqlTableModel->yerothSetFilter_WITH_where_clause(filter);

    if (_curAchatsDEServicesSqlTableModel->easySelect() > 0)
    {
        lister_les_elements_du_tableau(*_curAchatsDEServicesSqlTableModel);
    }
}


void YerothChargesFinancieresWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
    	_pushButton_charges_financieres_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_charges_financieres_filtrer_font->setUnderline(false);
    }

    pushButton_charges_financieres_filtrer->setFont(*_pushButton_charges_financieres_filtrer_font);
}


bool YerothChargesFinancieresWindow::supprimer_un_achat_au_fournisseur()
{
    if (getLastListerSelectedRow__ID_AS_INTEGER() <= -1 ||
    	_curAchatsDEServicesSqlTableModel->rowCount() <= 0)
    {
        YerothQMessageBox::information(this, QObject::trUtf8("supprimer"),
                                  	   QObject::trUtf8("Sélectionnez 1 achat (service) au fournisseur à supprimer."));

        return false;
    }

	return false;
}


bool YerothChargesFinancieresWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_charges_financieres,
										   tableColumnsToIgnore,
										   "yeroth-erp-charges-financieres",
										   "fiche des charges financières");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_charges_financieres,
										   tableColumnsToIgnore,
										   "yeroth-erp-FINANCIAL-EXPENSES-listing",
										   "financial expenses file");
#endif

	return success;
}


QString YerothChargesFinancieresWindow::get_latex_template_print_pdf_content()
{
	_latex_template_print_pdf_content = YerothUtils::template_CHARGES_FINANCIERES_tex;

	return _latex_template_print_pdf_content;
}

