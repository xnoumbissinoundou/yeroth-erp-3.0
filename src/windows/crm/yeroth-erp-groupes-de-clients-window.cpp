/*
 * yeroth-erp-groupes-de-clients-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-groupes-de-clients-window.hpp"

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


YerothGroupesDeClientsWindow::YerothGroupesDeClientsWindow()
:YerothWindowsCommons("yeroth-erp-liste-groupes-de-clients"),
 YerothAbstractClassYerothSearchWindow(_allWindows->GROUPES_DE_CLIENTS),
 _logger(new YerothLogger("YerothGroupesDeClientsWindow")),
 _pushButton_groupes_de_clients_filtrer_font(0),
 _curClientGroupTableModel(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("groupes de clients"));

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_groupes_de_clients);

    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_groupes_de_clients);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}")
			.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
		<< YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(_allWindows->GROUPES_DE_CLIENTS);


    _lineEditsToANDContentForSearch.insert(&lineEdit_groupes_de_clients_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _yeroth_WINDOW_references_dbColumnString.insert(YerothDatabaseTableColumn::REFERENCE_GROUPE);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(_allWindows->GROUPES_DE_CLIENTS);

    reinitialiser_champs_db_visibles();

    _curClientGroupTableModel = &_allWindows->getSqlTableModel_groupes_de_clients();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setupDateTimeEdits();

    _pushButton_groupes_de_clients_filtrer_font = new QFont(pushButton_groupes_de_clients_filtrer->font());

    tableView_groupes_de_clients->setSqlTableName(&YerothERPWindows::GROUPES_DE_CLIENTS);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_groupes_de_clients_filtrer->disable(this);
    pushButton_creer_groupe->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_programmes_de_fidelite_clients->disable(this);
    pushButton_modifier->disable(this);
    pushButton_reinitialiser->disable(this);

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
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

    connect(tableView_groupes_de_clients, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}


YerothGroupesDeClientsWindow::~YerothGroupesDeClientsWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _logger;
}


void YerothGroupesDeClientsWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();
	afficher_groupes_de_clients(*_curClientGroupTableModel);
}


void YerothGroupesDeClientsWindow::textChangedSearchLineEditsQCompleters()
{
}


void YerothGroupesDeClientsWindow::populateComboBoxes()
{
}


void YerothGroupesDeClientsWindow::setupLineEdits()
{
	lineEdit_groupes_de_clients_terme_recherche->setFocus();

	lineEdit_groupes_de_clients_terme_recherche->
		enableForSearch(QObject::trUtf8("terme à rechercher (nom du client)"));

	MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_groupes_de_clients_nombre_de_lignes_par_page,
										tableView_groupes_de_clients);
}


void YerothGroupesDeClientsWindow::setupDateTimeEdits()
{
	dateEdit_groupes_de_clients_debut->setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

	dateEdit_groupes_de_clients_fin->setStartDate(GET_CURRENT_DATE);

	_clientGroupDateCreationFilter.clear();

    _clientGroupDateCreationFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    										.arg(YerothDatabaseTableColumn::DATE_CREATION,
    										     DATE_TO_DB_FORMAT_STRING(dateEdit_groupes_de_clients_debut->date()),
												 YerothDatabaseTableColumn::DATE_CREATION,
												 DATE_TO_DB_FORMAT_STRING(dateEdit_groupes_de_clients_fin->date())));

    connect(dateEdit_groupes_de_clients_debut,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_groupes_de_clients_fin,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));
}


void YerothGroupesDeClientsWindow::afficher_groupes_de_clients(YerothSqlTableModel &clientGroupTableModel)
{
	tableView_groupes_de_clients->queryYerothTableViewCurrentPageContentRow(clientGroupTableModel);

    tableView_show_or_hide_columns(*tableView_groupes_de_clients);

    int rowCount = tableView_groupes_de_clients->rowCount();

    lineEdit_nombre_de_groupes->setText(GET_NUM_STRING(rowCount));
}


bool YerothGroupesDeClientsWindow::filtrer_groupes_de_clients()
{
	return false;
}


void YerothGroupesDeClientsWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
			<< YerothDatabaseTableColumn::DATE_CREATION
			<< YerothDatabaseTableColumn::DESIGNATION
			<< YerothDatabaseTableColumn::REFERENCE_GROUPE
			<< YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES;
}


void YerothGroupesDeClientsWindow::contextMenuEvent(QContextMenuEvent * event)
{
}


void YerothGroupesDeClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 		(*actionAppeler_aide);
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe			(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothGroupesDeClientsWindow::resetFilter()
{
}


void YerothGroupesDeClientsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setYerothSqlTableModel(_curClientGroupTableModel);

    _curStocksTableModel = stocksTableModel;

	setVisible(true);

    afficher_groupes_de_clients(*_curClientGroupTableModel);
}


void YerothGroupesDeClientsWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

    definirPasDeRole();
}


void YerothGroupesDeClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupesDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curClientGroupTableModel)


    pushButton_groupes_de_clients_filtrer->enable(this, SLOT(filtrer_groupes_de_clients()));
    pushButton_programmes_de_fidelite_clients->enable(this, SLOT(programmes_de_fidelite_clients()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_creer_groupe->enable(this, SLOT(creerUnGroupeDeClients()));
    pushButton_modifier->enable(this, SLOT(menu()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
}


void YerothGroupesDeClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupesDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curClientGroupTableModel)


    pushButton_groupes_de_clients_filtrer->enable(this, SLOT(filtrer_groupes_de_clients()));
    pushButton_programmes_de_fidelite_clients->enable(this, SLOT(programmes_de_fidelite_clients()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_creer_groupe->enable(this, SLOT(creerUnGroupeDeClients()));
    pushButton_modifier->enable(this, SLOT(menu()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
}


void YerothGroupesDeClientsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}


void YerothGroupesDeClientsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}


void YerothGroupesDeClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    pushButton_groupes_de_clients_filtrer->disable(this);
    pushButton_programmes_de_fidelite_clients->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_creer_groupe->disable(this);
    pushButton_modifier->disable(this);
    pushButton_reinitialiser->disable(this);
}


void YerothGroupesDeClientsWindow::afficher_au_detail()
{
    if (_curClientGroupTableModel->rowCount() > 0)
    {
    	//qDebug() << "++ test" << modelIndex.row();
        _allWindows->_detailsDunProgrammeDeFideliteClientsWindow
							->rendreVisible(_curClientGroupTableModel,
											_curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un groupe de clients"),
                                  QObject::trUtf8("Sélectionnez un groupe de clients à afficher les détails !"));
    }
}


void YerothGroupesDeClientsWindow::refineYerothLineEdits()
{
	_clientGroupDateCreationFilter.clear();

	_clientGroupDateCreationFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    										.arg(YerothDatabaseTableColumn::DATE_CREATION,
    											 DATE_TO_DB_FORMAT_STRING(dateEdit_groupes_de_clients_debut->date()),
												 YerothDatabaseTableColumn::DATE_CREATION,
												 DATE_TO_DB_FORMAT_STRING(dateEdit_groupes_de_clients_fin->date())));

	setupLineEditsQCompleters((QObject *)this);

	afficher_groupes_de_clients(*_curClientGroupTableModel);
}


void YerothGroupesDeClientsWindow::reinitialiser_elements_filtrage()
{
    lineEdit_groupes_de_clients_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothGroupesDeClientsWindow::reinitialiser_recherche()
{
}


void YerothGroupesDeClientsWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
    	_pushButton_groupes_de_clients_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_groupes_de_clients_filtrer_font->setUnderline(false);
    }

    pushButton_groupes_de_clients_filtrer->setFont(*_pushButton_groupes_de_clients_filtrer_font);
}


bool YerothGroupesDeClientsWindow::export_csv_file()
{
	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_groupes_de_clients,
										   tableColumnsToIgnore,
										   "yeroth-erp-liste-de-clients-groupes-format-csv",
										   "fiche des groupes des clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_groupes_de_clients,
										   tableColumnsToIgnore,
										   "yeroth-erp-client-group-listing-csv-format",
										   "client group listing");
#endif

	return success;
}


bool YerothGroupesDeClientsWindow::imprimer_pdf_document()
{
//	_latex_template_print_pdf_content = YerothUtils::template_lister_groupes_de_clients_tex;
	return YerothWindowsCommons::imprimer_pdf_document();
}
