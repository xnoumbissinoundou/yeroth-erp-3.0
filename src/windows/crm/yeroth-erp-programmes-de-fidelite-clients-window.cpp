/*
 * yeroth-erp-programmes-de-fidelite-clients-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-programmes-de-fidelite-clients-window.hpp"

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


YerothERPProgrammesDeFideliteClientsWindow::YerothERPProgrammesDeFideliteClientsWindow()
:YerothWindowsCommons("yeroth-erp-liste-programmes-de-fidelite-clients"),
 YerothAbstractClassYerothSearchWindow(_allWindows->PROGRAMMES_DE_FIDELITE_CLIENTS),
 _logger(new YerothLogger("YerothERPProgrammesDeFideliteClientsWindow")),
 _pushButton_programmes_de_fidelite_clients_filtrer_font(0),
 _curClient_ROYALTY_PROGRAM_TableModel(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("programmes de fidélité clients"));

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_programmes_de_fidelite_clients);

    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_programmes_de_fidelite_clients);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}")
			.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
		<< YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(_allWindows->PROGRAMMES_DE_FIDELITE_CLIENTS);


    _lineEditsToANDContentForSearch.insert(&lineEdit_programmes_de_fidelite_clients_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _yeroth_WINDOW_references_dbColumnString.insert(YerothDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(_allWindows->PROGRAMMES_DE_FIDELITE_CLIENTS);

    reinitialiser_champs_db_visibles();

    _curClient_ROYALTY_PROGRAM_TableModel = &_allWindows->getSqlTableModel_programmes_de_fidelite_clients();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setupDateTimeEdits();

    _pushButton_programmes_de_fidelite_clients_filtrer_font = new QFont(pushButton_programmes_de_fidelite_clients_filtrer->font());

    tableView_programmes_de_fidelite_clients->setSqlTableName(&YerothERPWindows::PROGRAMMES_DE_FIDELITE_CLIENTS);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuClients, false);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_programmes_de_fidelite_clients_filtrer->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_afficher->disable(this);
    pushButton_groupe_de_clients->disable(this);
    pushButton_creer_un_programme_de_fidelite_clients->disable(this);
    pushButton_modifier->disable(this);
    pushButton_reinitialiser->disable(this);

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenuClients, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficher_ce_groupe_au_detail, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionSupprimerGroupesDeClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));
    connect(actionReinitialiserElementsDeFiltrage, SIGNAL(triggered()), this, SLOT(reinitialiser_elements_filtrage()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION
connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
#endif

    connect(actionAfficher_ce_groupe_au_detail, SIGNAL(triggered()),
    		this, SLOT(afficher_au_detail()));

    connect(actionSupprimerGroupesDeClients, SIGNAL(triggered()),
    		this, SLOT(clients()));

    connect(tableView_programmes_de_fidelite_clients, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}


YerothERPProgrammesDeFideliteClientsWindow::~YerothERPProgrammesDeFideliteClientsWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _logger;
}


void YerothERPProgrammesDeFideliteClientsWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();
	afficher_programmes_de_fidelite_clients(*_curClient_ROYALTY_PROGRAM_TableModel);
}


void YerothERPProgrammesDeFideliteClientsWindow::textChangedSearchLineEditsQCompleters()
{
}


void YerothERPProgrammesDeFideliteClientsWindow::populateComboBoxes()
{
}


void YerothERPProgrammesDeFideliteClientsWindow::setupLineEdits()
{
	lineEdit_programmes_de_fidelite_clients_terme_recherche->setFocus();

	lineEdit_programmes_de_fidelite_clients_terme_recherche->
		enableForSearch(QObject::trUtf8("terme à rechercher (désignation du programme fidélité de clients)"));

	MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_programmes_de_fidelite_clients_nombre_de_lignes_par_page,
										tableView_programmes_de_fidelite_clients);
}


void YerothERPProgrammesDeFideliteClientsWindow::setupDateTimeEdits()
{
	dateEdit_programmes_de_fidelite_clients_debut->setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

	dateEdit_programmes_de_fidelite_clients_fin->setStartDate(GET_CURRENT_DATE);

	_client_ROYALTY_PROGRAM_DateCreationFilter.clear();

    _client_ROYALTY_PROGRAM_DateCreationFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    										.arg(YerothDatabaseTableColumn::DATE_CREATION,
    										     DATE_TO_DB_FORMAT_STRING(dateEdit_programmes_de_fidelite_clients_debut->date()),
												 YerothDatabaseTableColumn::DATE_CREATION,
												 DATE_TO_DB_FORMAT_STRING(dateEdit_programmes_de_fidelite_clients_fin->date())));

    connect(dateEdit_programmes_de_fidelite_clients_debut,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_programmes_de_fidelite_clients_fin,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));
}


void YerothERPProgrammesDeFideliteClientsWindow::afficher_programmes_de_fidelite_clients(YerothSqlTableModel &client_ROYALTY_PROGRAM_TableModel)
{
	tableView_programmes_de_fidelite_clients->queryYerothTableViewCurrentPageContentRow(client_ROYALTY_PROGRAM_TableModel);

    tableView_show_or_hide_columns(*tableView_programmes_de_fidelite_clients);

    int rowCount = tableView_programmes_de_fidelite_clients->rowCount();

    lineEdit_nombre_de_groupes->setText(GET_NUM_STRING(rowCount));
}


bool YerothERPProgrammesDeFideliteClientsWindow::filtrer_programmes_de_fidelite_client()
{
	return false;
}


void YerothERPProgrammesDeFideliteClientsWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
			<< YerothDatabaseTableColumn::DATE_CREATION
			<< YerothDatabaseTableColumn::DESIGNATION
			<< YerothDatabaseTableColumn::MONTANT_DU_RABAIS
			<< YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS
			<< YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS_REFEREUR;
}


void YerothERPProgrammesDeFideliteClientsWindow::contextMenuEvent(QContextMenuEvent * event)
{
}


void YerothERPProgrammesDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 		(*actionAppeler_aide);
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe			(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothERPProgrammesDeFideliteClientsWindow::resetFilter()
{
}


void YerothERPProgrammesDeFideliteClientsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setYerothSqlTableModel(_curClient_ROYALTY_PROGRAM_TableModel);

    _curStocksTableModel = stocksTableModel;

	setVisible(true);

    afficher_programmes_de_fidelite_clients(*_curClient_ROYALTY_PROGRAM_TableModel);
}


void YerothERPProgrammesDeFideliteClientsWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

    definirPasDeRole();
}


void YerothERPProgrammesDeFideliteClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuClients, true);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupesDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curClient_ROYALTY_PROGRAM_TableModel)


    pushButton_programmes_de_fidelite_clients_filtrer->enable(this, SLOT(filtrer_programmes_de_fidelite_client()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_groupe_de_clients->enable(this, SLOT(groupes_de_clients()));
    pushButton_creer_un_programme_de_fidelite_clients->enable(this, SLOT(creer_un_programme_de_fidelite_clients()));
    pushButton_modifier->enable(this, SLOT(menu()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
}


void YerothERPProgrammesDeFideliteClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuClients, true);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupesDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curClient_ROYALTY_PROGRAM_TableModel)


    pushButton_programmes_de_fidelite_clients_filtrer->enable(this, SLOT(filtrer_programmes_de_fidelite_client()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_groupe_de_clients->enable(this, SLOT(groupes_de_clients()));
    pushButton_creer_un_programme_de_fidelite_clients->enable(this, SLOT(creer_un_programme_de_fidelite_clients()));
    pushButton_modifier->enable(this, SLOT(menu()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
}


void YerothERPProgrammesDeFideliteClientsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}


void YerothERPProgrammesDeFideliteClientsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}


void YerothERPProgrammesDeFideliteClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuClients, false);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    pushButton_programmes_de_fidelite_clients_filtrer->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_afficher->disable(this);
    pushButton_groupe_de_clients->disable(this);
    pushButton_creer_un_programme_de_fidelite_clients->disable(this);
    pushButton_modifier->disable(this);
    pushButton_reinitialiser->disable(this);
}


void YerothERPProgrammesDeFideliteClientsWindow::afficher_au_detail()
{
    if (_curClient_ROYALTY_PROGRAM_TableModel->rowCount() > 0)
    {
    	//qDebug() << "++ test" << modelIndex.row();
        _allWindows->_detailsDunProgrammeDeFideliteClientsWindow
							->rendreVisible(_curClient_ROYALTY_PROGRAM_TableModel,
											_curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un programme de fidélité clients"),
                                  QObject::trUtf8("Sélectionnez un programme de fidélité clients à afficher les détails !"));
    }
}


void YerothERPProgrammesDeFideliteClientsWindow::refineYerothLineEdits()
{
	_client_ROYALTY_PROGRAM_DateCreationFilter.clear();

	_client_ROYALTY_PROGRAM_DateCreationFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    													.arg(YerothDatabaseTableColumn::DATE_CREATION,
    														 DATE_TO_DB_FORMAT_STRING(dateEdit_programmes_de_fidelite_clients_debut->date()),
															 YerothDatabaseTableColumn::DATE_CREATION,
															 DATE_TO_DB_FORMAT_STRING(dateEdit_programmes_de_fidelite_clients_fin->date())));

	setupLineEditsQCompleters((QObject *)this);

	afficher_programmes_de_fidelite_clients(*_curClient_ROYALTY_PROGRAM_TableModel);
}


void YerothERPProgrammesDeFideliteClientsWindow::reinitialiser_elements_filtrage()
{
	lineEdit_programmes_de_fidelite_clients_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothERPProgrammesDeFideliteClientsWindow::reinitialiser_recherche()
{
}


void YerothERPProgrammesDeFideliteClientsWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
    	_pushButton_programmes_de_fidelite_clients_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_programmes_de_fidelite_clients_filtrer_font->setUnderline(false);
    }

    pushButton_programmes_de_fidelite_clients_filtrer->setFont(*_pushButton_programmes_de_fidelite_clients_filtrer_font);
}


bool YerothERPProgrammesDeFideliteClientsWindow::export_csv_file()
{
	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_programmes_de_fidelite_clients,
										   tableColumnsToIgnore,
										   "yeroth-erp-liste-des-programmes-de-fidelite-clients-format-csv",
										   "fiche des programmes de fidélité clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_programmes_de_fidelite_clients,
										   tableColumnsToIgnore,
										   "yeroth-erp-client-ROYALTY-PROGRAM-group-listing-csv-format",
										   "client ROYALTY PROGRAM listing");
#endif

	return success;
}


bool YerothERPProgrammesDeFideliteClientsWindow::imprimer_pdf_document()
{
//	_latex_template_print_pdf_content = YerothUtils::template_lister_groupes_de_clients_tex;
	return YerothWindowsCommons::imprimer_pdf_document();
}
