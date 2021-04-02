/*
 * yeroth-erp-comptabilite-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-comptabilite-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-user-administrateur.hpp"

#include "src/users/yeroth-erp-user-caissier.hpp"

#include "src/users/yeroth-erp-user-magasinier.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/utils/yeroth-erp-info-entreprise.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/dialogs/yeroth-erp-changer-utilisateur-dialog.hpp"

#include "src/dialogs/yeroth-erp-generic-select-db-field-dialog.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>


YerothComptabiliteWindow::YerothComptabiliteWindow()
:YerothWindowsCommons("yeroth-erp-listing-comptabilite"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::COMPTES_BANCAIRES),
 _comptesBancairesDateDebutTransactionFilter(YerothUtils::EMPTY_STRING),
 _logger(new YerothLogger("YerothComptabiliteWindow")),
 _curComptabiliteSqlTableModel(&_allWindows->getSqlTableModel_comptes_bancaires()),
 _pushButton_comptabilite_filtrer_font(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("comptabilité"));

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_comptes_doperations_comptables);


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_comptes_doperations_comptables);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_170,
                                    	 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    setup_select_configure_dbcolumn(YerothDatabase::COMPTES_BANCAIRES);

    _yeroth_WINDOW_references_dbColumnString.insert(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerothDatabase::COMPTES_BANCAIRES);

    reinitialiser_colones_db_visibles();

    setupLineEdits();

    setupDateTimeEdits();

    _pushButton_comptabilite_filtrer_font = new QFont(pushButton_comptabilite_filtrer->font());

    tableView_comptes_doperations_comptables->setSqlTableName(&YerothDatabase::COMPTES_BANCAIRES);


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


	connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));
    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_colones_db_visibles()));
    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

    setupShortcuts();
}


YerothComptabiliteWindow::~YerothComptabiliteWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _logger;
}


void YerothComptabiliteWindow::afficher_comptes_bancaires(YerothSqlTableModel &aYerothSqlTableModel)
{
	tableView_comptes_doperations_comptables->queryYerothTableViewCurrentPageContentRow(aYerothSqlTableModel);

    tableView_show_or_hide_columns(*tableView_comptes_doperations_comptables);

    int rowCount = tableView_comptes_doperations_comptables->getYerothSqlTableModelTotalRowCount();

    lineEdit_nombre_de_comptes_bancaires->setText(GET_NUM_STRING(rowCount));
}


void YerothComptabiliteWindow::reinitialiser_colones_db_visibles()
{
	_visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
		<< YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE
		<< YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE
		<< YerothDatabaseTableColumn::INSTITUT_BANCAIRE
		<< YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE;
}


void YerothComptabiliteWindow::hideEvent(QHideEvent * hideEvent)
{
	_selectExportDBQDialog->close();
}


void YerothComptabiliteWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 		(*actionAppeler_aide);
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe			(*actionQui_suis_je);
}


void YerothComptabiliteWindow::slot_reinitialiser_colones_db_visibles()
{
	reinitialiser_colones_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();
	afficher_comptes_bancaires();
}


void YerothComptabiliteWindow::textChangedSearchLineEditsQCompleters()
{
	lineEdit_comptabilite_element_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_comptabilite_terme_recherche->text());

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
        							.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE, partSearchTerm)));

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

    QString finalSearchFilter(_comptesBancairesDateDebutTransactionFilter);

    if (!_searchFilter.isEmpty())
    {
    	QString searchFilterWithDate(QString("%1 AND (%2)")
    									.arg(_comptesBancairesDateDebutTransactionFilter,
    										 _searchFilter));

    	finalSearchFilter = searchFilterWithDate;
    }

    _yerothSqlTableModel->yerothSetFilter_WITH_where_clause(finalSearchFilter);

//    QDEBUG_STRING_OUTPUT_2("finalSearchFilter", finalSearchFilter);

    if (_yerothSqlTableModel->select())
    {
    	afficher_comptes_bancaires(*_yerothSqlTableModel);
    }
    else
    {
        qDebug() << QString("++ YerothStocksWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }
}


void YerothComptabiliteWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
    	_pushButton_comptabilite_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_comptabilite_filtrer_font->setUnderline(false);
    }

    pushButton_comptabilite_filtrer->setFont(*_pushButton_comptabilite_filtrer_font);
}


void YerothComptabiliteWindow::refineYerothLineEdits()
{
	_comptesBancairesDateDebutTransactionFilter.clear();

//	_comptesBancairesDateDebutTransactionFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
//    													.arg(YerothDatabaseTableColumn::DATE_PAIEMENT,
//    														 DATE_TO_DB_FORMAT_STRING(dateEdit_comptabilite_debut->date()),
//															 YerothDatabaseTableColumn::DATE_PAIEMENT,
//															 DATE_TO_DB_FORMAT_STRING(dateEdit_comptabilite_fin->date())));
//
//	setupLineEditsQCompleters((QObject *)this);

	afficher_comptes_bancaires();
}


void YerothComptabiliteWindow::setupLineEdits()
{
	lineEdit_comptabilite_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (description du compte bancaire)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::trUtf8("valeur à rechercher"));

    lineEdit_comptabilite_element_resultat->setValidator(&YerothUtils::DoubleValidator);

    lineEdit_nombre_de_comptes_bancaires->setYerothEnabled(false);
    lineEdit_comptabilite_encaisse->setYerothEnabled(false);
    lineEdit_comptabilite_debourse->setYerothEnabled(false);

	MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_comptabilite_nombre_de_lignes_par_page, tableView_comptes_doperations_comptables);
}


void YerothComptabiliteWindow::setupDateTimeEdits()
{
    dateEdit_comptabilite_debut->setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

    dateEdit_comptabilite_fin->setStartDate(GET_CURRENT_DATE);

    _comptesBancairesDateDebutTransactionFilter.clear();

//	_comptesBancairesDateDebutTransactionFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
//    													.arg(YerothDatabaseTableColumn::DATE_PAIEMENT,
//    														 DATE_TO_DB_FORMAT_STRING(dateEdit_comptabilite_debut->date()),
//															 YerothDatabaseTableColumn::DATE_PAIEMENT,
//															 DATE_TO_DB_FORMAT_STRING(dateEdit_comptabilite_fin->date())));
//
//    connect(dateEdit_comptabilite_debut,
//    		SIGNAL(dateChanged(const QDate &)),
//			this,
//			SLOT(refineYerothLineEdits()));
//
//    connect(dateEdit_comptabilite_fin,
//    		SIGNAL(dateChanged(const QDate &)),
//			this,
//			SLOT(refineYerothLineEdits()));
}



void YerothComptabiliteWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curComptabiliteSqlTableModel)
}


void YerothComptabiliteWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
}


void YerothComptabiliteWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	_curStocksTableModel = stocksTableModel;

	_curComptabiliteSqlTableModel = &_allWindows->getSqlTableModel_comptes_bancaires();

    setYerothSqlTableModel(_curComptabiliteSqlTableModel);

	setVisible(true);

	afficher_comptes_bancaires();

	lineEdit_comptabilite_terme_recherche->setFocus();
}


bool YerothComptabiliteWindow::export_csv_file()
{
	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_comptes_doperations_comptables,
										   tableColumnsToIgnore,
										   "yeroth-erp-fichier-comptabilite-format-csv",
										   "fiche des stocks");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_comptes_doperations_comptables,
										   tableColumnsToIgnore,
										   "yeroth-erp-bank-account-listing-csv-format",
										   "stock listing");
#endif

	return success;
}


bool YerothComptabiliteWindow::imprimer_pdf_document()
{
//	_latex_template_print_pdf_content = YerothUtils::template_lister_stock_tex;
	return YerothWindowsCommons::imprimer_pdf_document();
}


void YerothComptabiliteWindow::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();

	YerothPOSUser *currentUser = _allWindows->getUser();

	if (0 != currentUser)
	{
		currentUser->setRole(YerothUtils::ROLE_INDEFINI);
	}
}

