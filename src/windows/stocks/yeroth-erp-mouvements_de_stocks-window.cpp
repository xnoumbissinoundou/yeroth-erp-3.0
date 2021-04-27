/*
 * yeroth-erp-mouvements_de_stocks-window.cpp
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */
#include "yeroth-erp-mouvements_de_stocks-window.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-utils.hpp"



#include <QtCore/QDir>

#include <QtCore/qmath.h>

#include <QtCore/QFile>

#include <QtCore/QProcessEnvironment>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QDesktopWidget>


YerothMouvementsDeStocksWindow::YerothMouvementsDeStocksWindow()
:YerothWindowsCommons("yeroth-erp-mouvements_de_stocks-stocks"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::STOCKS_SORTIES),
 _logger(new YerothLogger("YerothMouvementsDeStocksWindow")),
 _stocksSorties_OU_transferes_DateFilter(YerothUtils::EMPTY_STRING),
 _curMouvementsDeStocksTableModel(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("mouvements de stocks"));

    setupUi(this);

    mySetupUi(this);

    QList<YerothTableView *> allTableViews;

    allTableViews.append(tableView_sorties_articles);
    allTableViews.append(tableView_transferts_articles);

    setYerothTableView_FROM_WINDOWS_COMMONS(allTableViews);


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionExporter_au_format_csv
		<< action_parametrer_les_impressions
		<< actionAfficherPDF;

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_SORTIES_STOCKS);

    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(_curMouvementsDeStocksTableView);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_BLUE_77_93_254, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(YerothDatabase::STOCKS_SORTIES);

    reinitialiser_colones_db_visibles();

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setupDateTimeEdits();

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    // Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this, SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_SERVER
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(tabWidget_mouvementsDeStocks, SIGNAL(currentChanged(int)), this, SLOT(handleTabChanged(int)));

    setupShortcuts();
}


void YerothMouvementsDeStocksWindow::slot_reinitialiser_colones_db_visibles()
{
	reinitialiser_colones_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();
	lister_les_elements_du_tableau();
}


YerothMouvementsDeStocksWindow::~YerothMouvementsDeStocksWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

	delete _logger;
}


void YerothMouvementsDeStocksWindow::textChangedSearchLineEditsQCompleters()
{
	if (0 != _curLineEdit_mouvementsDeStocks_element_de_AAA_resultat)
	{
		_curLineEdit_mouvementsDeStocks_element_de_AAA_resultat->clear();
	}

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm;

    if (0 != _curLineEdit_mouvementsDeStocks_terme_recherche)
    {
    	searchTerm.append(_curLineEdit_mouvementsDeStocks_terme_recherche->text());
    }

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
        							.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::NOTES, partSearchTerm)));

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

    QString finalSearchFilter(_stocksSorties_OU_transferes_DateFilter);

    if (!_searchFilter.isEmpty())
    {
    	QString searchFilterWithDate(QString("%1 AND (%2)")
    									.arg(_stocksSorties_OU_transferes_DateFilter,
    										 _searchFilter));

    	finalSearchFilter = searchFilterWithDate;
    }

    if (_curMouvementsDeStocksTableModel->select())
    {
    	lister_les_elements_du_tableau(finalSearchFilter);
    }
    else
    {
        qDebug() << QString("++ YerothMouvementsDeStocksWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_curMouvementsDeStocksTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerothMouvementsDeStocksWindow::reinitialiser_colones_db_visibles()
{
	_visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
			<< YerothDatabaseTableColumn::DATE_SORTIE
			<< YerothDatabaseTableColumn::HEURE_SORTIE
			<< YerothDatabaseTableColumn::REFERENCE
			<< YerothDatabaseTableColumn::DESIGNATION
			<< YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
			<< YerothDatabaseTableColumn::CATEGORIE
			<< YerothDatabaseTableColumn::QUANTITE_SORTIE
			<< YerothDatabaseTableColumn::LOCALISATION_ENTREE
			<< YerothDatabaseTableColumn::LOCALISATION_SORTIE;
}


void YerothMouvementsDeStocksWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothMouvementsDeStocksWindow::handleTabChanged(int index)
{
    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_SORTIES_STOCKS);
    }
    else
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_TRANSFERTS_STOCKS);
    }

	afficher_stocks_sorties_OU_transferes();
}


void YerothMouvementsDeStocksWindow::setFilter()
{
    if (0 == _curMouvementsDeStocksTableModel)
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_SORTIES_STOCKS);
    }

    _curMouvementsDeStocksTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);
}


void YerothMouvementsDeStocksWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setYerothSqlTableModel(stocksTableModel);

    _curStocksTableModel = stocksTableModel;

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_SORTIES_STOCKS);
    }
    else
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_TRANSFERTS_STOCKS);
    }

    if (0 != _curLineEdit_mouvementsDeStocks_terme_recherche)
    {
    	_curLineEdit_mouvementsDeStocks_terme_recherche->setFocus();
    }

    setVisible(true);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

    afficher_stocks_sorties_OU_transferes();
}


bool YerothMouvementsDeStocksWindow::filtrer_stocks_sorties_OU_transferes()
{

	return false;
}


void YerothMouvementsDeStocksWindow::setupLineEdits()
{
	_QLINEEDIT_nombre_de_lignes_par_page = lineEdit_mouvementsDeStocks_nombre_de_lignes_par_page;

	lineEdit_mouvementsDeStocks_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (notes)"));
	lineEdit_mouvementsDeStocks_terme_recherche_2->enableForSearch(QObject::trUtf8("terme à rechercher (notes)"));

	lineEdit_nom_element_string_db->enableForSearch(QObject::trUtf8("valeur à rechercher"));
    lineEdit_nom_element_string_db_2->enableForSearch(QObject::trUtf8("valeur à rechercher"));

    lineEdit_mouvementsDeStocks_element_de_sortiestocks_resultat->setValidator(&YerothUtils::POSITIVE_DoubleValidator);
    lineEdit_mouvementsDeStocks_element_de_transfertstocks_resultat->setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    lineEdit_mouvementsDeStocks_quantite_sortie->setYerothEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_mouvementsDeStocks_nombre_de_lignes_par_page, tableView_sorties_articles);
    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_mouvementsDeStocks_nombre_de_lignes_par_page_2, tableView_transferts_articles);
}


void YerothMouvementsDeStocksWindow::switchTableWidgetTAB_CONTENT_ELEMENTS(enum TabIndexes aTabIndex)
{
	_curMouvementsDeStocksTableModel = &_allWindows->getSqlTableModel_stocks_sorties();

	switch(aTabIndex)
	{
	case SUJET_ACTION_SORTIES_STOCKS:

		_curDateEdit_debut = dateEdit_mouvementsDeStocks_debut;

		_curDateEdit_fin = dateEdit_mouvementsDeStocks_fin;

		_curMouvementsDeStocksTableView = tableView_sorties_articles;

		_curLineEdit_mouvementsDeStocks_terme_recherche = lineEdit_mouvementsDeStocks_terme_recherche;

		_curLineEdit_mouvementsDeStocks_element_de_AAA_resultat =
				lineEdit_mouvementsDeStocks_element_de_sortiestocks_resultat;

		_curLabel_mouvementsDeStocks_numero_page_derniere = label_mouvementsDeStocks_numero_page_derniere;

		_curLabel_mouvementsDeStocks_numero_page_courante = label_mouvementsDeStocks_numero_page_courante;

		break;

	case SUJET_ACTION_TRANSFERTS_STOCKS:

		_curDateEdit_debut = dateEdit_mouvementsDeStocks_debut_2;

		_curDateEdit_fin = dateEdit_mouvementsDeStocks_fin_2;

		_curMouvementsDeStocksTableView = tableView_transferts_articles;

		_curLineEdit_mouvementsDeStocks_terme_recherche = lineEdit_mouvementsDeStocks_terme_recherche_2;

		_curLineEdit_mouvementsDeStocks_element_de_AAA_resultat =
				lineEdit_mouvementsDeStocks_element_de_transfertstocks_resultat;

		_curLabel_mouvementsDeStocks_numero_page_derniere = label_mouvementsDeStocks_numero_page_derniere_2;

		_curLabel_mouvementsDeStocks_numero_page_courante = label_mouvementsDeStocks_numero_page_courante_2;

		break;

	default:
		break;
	}
}


void YerothMouvementsDeStocksWindow::setupDateTimeEdits()
{
    dateEdit_mouvementsDeStocks_debut->setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

    dateEdit_mouvementsDeStocks_fin->setStartDate(GET_CURRENT_DATE);

    dateEdit_mouvementsDeStocks_debut_2->setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

    dateEdit_mouvementsDeStocks_fin_2->setStartDate(GET_CURRENT_DATE);

    _stocksSorties_OU_transferes_DateFilter.clear();

    _stocksSorties_OU_transferes_DateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    					.arg(YerothDatabaseTableColumn::DATE_SORTIE,
    						 DATE_TO_DB_FORMAT_STRING(_curDateEdit_debut->date()),
							 YerothDatabaseTableColumn::DATE_SORTIE,
							 DATE_TO_DB_FORMAT_STRING(_curDateEdit_fin->date())));

    connect(dateEdit_mouvementsDeStocks_debut,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_mouvementsDeStocks_fin,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_mouvementsDeStocks_debut_2,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_mouvementsDeStocks_fin_2,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));
}


void YerothMouvementsDeStocksWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

    definirPasDeRole();
}

void YerothMouvementsDeStocksWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}


void YerothMouvementsDeStocksWindow::definirVendeur()
{
    _logger->log("definirVendeur - definirPasDeRole()");

    definirPasDeRole();
}


void YerothMouvementsDeStocksWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothMouvementsDeStocksWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothMouvementsDeStocksWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}


bool YerothMouvementsDeStocksWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

    int currentTableViewCurrentIndex = -1;

    currentTableViewCurrentIndex = tabWidget_mouvementsDeStocks->currentIndex();

    YerothTableView *currentYerothTableView = 0;

    if (0 == currentTableViewCurrentIndex)
    {
    	currentYerothTableView = tableView_sorties_articles;
    }
    else if (1 == currentTableViewCurrentIndex)
    {
    	currentYerothTableView = tableView_transferts_articles;
    }
    else
    {
    	//TODO: insert QDialog message box here
    	return success;
    }

    QStandardItemModel *tableModel = tableView_sorties_articles->getStandardItemModel();

    if (0 == tableModel)
    {
        YerothQMessageBox::information(this,
                                      QObject::trUtf8("pas de données à exporter au format csv"),
                                      QObject::trUtf8("Il n'y a pas de données à exporter au format csv !"));
        return success;
    }

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *currentYerothTableView,
										   tableColumnsToIgnore,
										   "yeroth-erp-fichier-stock-sortie-format-csv",
										   "fiche des comptes clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *currentYerothTableView,
										   tableColumnsToIgnore,
										   "yeroth-erp-stock-transfert-csv-format",
										   "customer account listing file");
#endif

	return success;
}


QString YerothMouvementsDeStocksWindow::get_latex_template_print_pdf_content()
{
    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	YerothWindowsCommons::setYerothTableView_FROM_WINDOWS_COMMONS(tableView_sorties_articles);
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	YerothWindowsCommons::setYerothTableView_FROM_WINDOWS_COMMONS(tableView_transferts_articles);
    }

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();

#ifdef YEROTH_FRANCAIS_LANGUAGE

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
			insert("YEROTHSUBJECT", "Journal des sorties d'articles");
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
			insert("YEROTHSUBJECT", "Journal des transferts d'articles");
    }

    _latex_template_print_pdf_content = YerothUtils::FR_template_liste_des_mouvements_de_stocks_tex;

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
			insert("YEROTHSUBJECT", "Journal of outgoing articles");
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	_documentSpecificElements_FOR_PDF_LATEX_PRINTING.
			insert("YEROTHSUBJECT", "Journal of article transfers");
    }

    _latex_template_print_pdf_content = YerothUtils::EN_template_liste_des_mouvements_de_stocks_tex;

#endif

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.
		insert("YEROTHVENTESDEBUT", DATE_TO_STRING(_curDateEdit_debut->date()));

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.
		insert("YEROTHVENTESFIN", DATE_TO_STRING(_curDateEdit_fin->date()));

	return _latex_template_print_pdf_content;
}


void YerothMouvementsDeStocksWindow::lister_les_elements_du_tableau(const QString &aSearchFilter /* = YerothUtils::EMPTY_STRING */)
{
    _logger->log("lister_les_elements_du_tableau");

    if (!aSearchFilter.isEmpty())
    {
    	_searchFilter.append(aSearchFilter);
    }

    if (_searchFilter.isEmpty())
    {
        _searchFilter.append(QString(" date_sortie >= '%1' AND date_sortie <= '%2'").
                             arg(DATE_TO_DB_FORMAT_STRING(dateEdit_mouvementsDeStocks_debut->date()),
                                 DATE_TO_DB_FORMAT_STRING(dateEdit_mouvementsDeStocks_fin->date())));
    }
    else
    {
        _searchFilter.append(QString(" AND date_sortie >= '%1' AND date_sortie <= '%2'").
                             arg(DATE_TO_DB_FORMAT_STRING(dateEdit_mouvementsDeStocks_debut->date()),
                                 DATE_TO_DB_FORMAT_STRING(dateEdit_mouvementsDeStocks_fin->date())));
    }

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_SORTIES_STOCKS);

        _searchFilter.append(" AND localisation_entree = ''");
    }
    else
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_TRANSFERTS_STOCKS);

        _searchFilter.append(" AND localisation_entree != ''");
    }

    _curMouvementsDeStocksTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);

    _logger->log("lister_les_elements_du_tableau", _searchFilter);

    //qDebug() << "++ lister_les_elements_du_tableau, aSearchFilter: " << _curMouvementsDeStocksTableModel->filter();

    tableView_sorties_articles->lister_les_elements_du_tableau(*_curMouvementsDeStocksTableModel);


    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
        if (_visibleDBColumnNameStrList.contains(YerothDatabaseTableColumn::LOCALISATION_ENTREE))
        {
        	_visibleDBColumnNameStrList.removeAll(YerothDatabaseTableColumn::LOCALISATION_ENTREE);
        }

        tableView_sorties_articles->lister_les_elements_du_tableau(*_curMouvementsDeStocksTableModel);

        tableView_show_or_hide_columns(*tableView_sorties_articles);
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
        if (!_visibleDBColumnNameStrList.contains(YerothDatabaseTableColumn::LOCALISATION_ENTREE))
        {
        	_visibleDBColumnNameStrList.append(YerothDatabaseTableColumn::LOCALISATION_ENTREE);
        }

        tableView_transferts_articles->lister_les_elements_du_tableau(*_curMouvementsDeStocksTableModel);

        tableView_show_or_hide_columns(*tableView_transferts_articles);
    }


    double quantite_sortie_OU_transferee = 0;
    double quantite_sortie_OU_transferee_TOTAL = 0;

    int curMouvementsDeStocksTableModelRowCount = _curMouvementsDeStocksTableModel->easySelect();

    QSqlRecord aRecord;

    for (int j = 0; j < curMouvementsDeStocksTableModelRowCount; ++j)
    {
        aRecord = _curMouvementsDeStocksTableModel->record(j);

        quantite_sortie_OU_transferee =
        		GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::QUANTITE_SORTIE).toDouble();

        quantite_sortie_OU_transferee_TOTAL += quantite_sortie_OU_transferee;
    }

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	lineEdit_mouvementsDeStocks_quantite_sortie->setText(GET_DOUBLE_STRING(quantite_sortie_OU_transferee_TOTAL));
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	lineEdit_mouvementsDeStocks_quantite_transferee->setText(GET_DOUBLE_STRING(quantite_sortie_OU_transferee_TOTAL));
    }
}


void YerothMouvementsDeStocksWindow::resetFilter()
{
    _searchFilter.clear();

    if (0 != _curMouvementsDeStocksTableModel)
    {
        _curMouvementsDeStocksTableModel->resetFilter();
    }
    else
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_SORTIES_STOCKS);
    }

    if (0 != _curLineEdit_mouvementsDeStocks_terme_recherche)
    {
    	_curLineEdit_mouvementsDeStocks_terme_recherche->clear();
    }

    if (_allWindows->getUser()->isManager() || _allWindows->getUser()->isMagasinier())
    {
    	_curDateEdit_debut->reset();

    	_curDateEdit_fin->reset();

        lister_les_elements_du_tableau();
    }
}


void YerothMouvementsDeStocksWindow::refineYerothLineEdits()
{
    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_SORTIES_STOCKS);
    }
    else
    {
    	switchTableWidgetTAB_CONTENT_ELEMENTS(SUJET_ACTION_TRANSFERTS_STOCKS);
    }

	_stocksSorties_OU_transferes_DateFilter.clear();

	_stocksSorties_OU_transferes_DateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    					.arg(YerothDatabaseTableColumn::DATE_SORTIE,
    						 DATE_TO_DB_FORMAT_STRING(_curDateEdit_debut->date()),
							 YerothDatabaseTableColumn::DATE_SORTIE,
							 DATE_TO_DB_FORMAT_STRING(_curDateEdit_fin->date())));

	setupLineEditsQCompleters((QObject *)this);

	afficher_stocks_sorties_OU_transferes();
}


void YerothMouvementsDeStocksWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");
    resetFilter();
}
