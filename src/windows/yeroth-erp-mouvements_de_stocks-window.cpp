/*
 * yeroth-erp-mouvements_de_stocks-window.cpp
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
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
 _logger(new YerothLogger("YerothMouvementsDeStocksWindow")),
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

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_BLUE_77_93_254, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(_allWindows->STOCKS_SORTIES);

    reinitialiser_champs_db_visibles();

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    // Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
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


void YerothMouvementsDeStocksWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();
	rechercher();
}


void YerothMouvementsDeStocksWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

    _visibleDBFieldColumnStrList
			<< YerothDatabaseTableColumn::DATE_SORTIE
			<< YerothDatabaseTableColumn::HEURE_SORTIE
			<< YerothDatabaseTableColumn::REFERENCE
			<< YerothDatabaseTableColumn::DESIGNATION
			<< YerothDatabaseTableColumn::CATEGORIE
			<< YerothDatabaseTableColumn::QUANTITE_SORTIE
			<< YerothDatabaseTableColumn::LOCALISATION_ENTREE
			<< YerothDatabaseTableColumn::LOCALISATION_SORTIE;
}


void YerothMouvementsDeStocksWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothMouvementsDeStocksWindow::setFilter()
{
    if (0 == _curMouvementsDeStocksTableModel)
    {
        _curMouvementsDeStocksTableModel = &_allWindows->getSqlTableModel_stocks_sorties();
    }

    _curMouvementsDeStocksTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);
}


void YerothMouvementsDeStocksWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    setupLineEditsQCompleters();

    setVisible(true);

    rechercher();
}


void YerothMouvementsDeStocksWindow::setupLineEdits()
{
    _logger->log("setupLineEdits");

    lineEdit_mouvementsDeStocks_quantite_sortie->setYerothEnabled(false);
    lineEdit_mouvementsDeStocks_nom_magasinier->enableForSearch(QObject::trUtf8("nom du magasinier"));
    lineEdit_mouvementsDeStocks_designation->enableForSearch(QObject::trUtf8("désignation"));
    lineEdit_mouvementsDeStocks_nom_categorie->enableForSearch(QObject::trUtf8("nom de la catégorie d'articles"));
    lineEdit_mouvementsDeStocks_reference_recu_sortie->enableForSearch(QObject::trUtf8("référence reçu de sortie"));
    lineEdit_mouvementsDeStocks_nom_recepteur->enableForSearch(QObject::trUtf8("nom du récepteur d'articles"));

    connect(lineEdit_mouvementsDeStocks_nom_magasinier, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_mouvementsDeStocks_designation, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_mouvementsDeStocks_nom_categorie, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_mouvementsDeStocks_reference_recu_sortie, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_mouvementsDeStocks_nom_recepteur, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
}


void YerothMouvementsDeStocksWindow::setupLineEditsQCompleters()
{
    lineEdit_mouvementsDeStocks_nom_magasinier->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::NOM_MAGASINIER);

    lineEdit_mouvementsDeStocks_designation->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::DESIGNATION);

    lineEdit_mouvementsDeStocks_nom_categorie->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::CATEGORIE);

    lineEdit_mouvementsDeStocks_reference_recu_sortie->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::REFERENCE_RECU_SORTIE);

    lineEdit_mouvementsDeStocks_nom_recepteur->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::NOM_RECEPTEUR);
}


void YerothMouvementsDeStocksWindow::setupDateTimeEdits()
{
    dateEdit_mouvementsDeStocks_debut->setStartDate(GET_CURRENT_DATE);
    dateEdit_mouvementsDeStocks_fin->setStartDate(GET_CURRENT_DATE);

    connect(dateEdit_mouvementsDeStocks_debut, SIGNAL(dateChanged(const QDate &)), this, SLOT(rechercher()));
    connect(dateEdit_mouvementsDeStocks_fin, SIGNAL(dateChanged(const QDate &)), this, SLOT(rechercher()));
}


void YerothMouvementsDeStocksWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}

void YerothMouvementsDeStocksWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);

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
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}


void YerothMouvementsDeStocksWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);

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

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothMouvementsDeStocksWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
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


bool YerothMouvementsDeStocksWindow::imprimer_pdf_document()
{
    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	YerothWindowsCommons::setYerothTableView_FROM_WINDOWS_COMMONS(tableView_sorties_articles);
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	YerothWindowsCommons::setYerothTableView_FROM_WINDOWS_COMMONS(tableView_transferts_articles);
    }

    QMap<QString, QString> documentSpecificElements;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	documentSpecificElements.insert("YEROTHSUBJECT", "Journal des sorties d'articles");
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	documentSpecificElements.insert("YEROTHSUBJECT", "Journal des transferts d'articles");
    }

    _latex_template_print_pdf_content = YerothUtils::FR_template_liste_des_mouvements_de_stocks_tex;

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	documentSpecificElements.insert("YEROTHSUBJECT", "Journal of outgoing articles");
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
    	documentSpecificElements.insert("YEROTHSUBJECT", "Journal of article transfers");
    }

    _latex_template_print_pdf_content = YerothUtils::EN_template_liste_des_mouvements_de_stocks_tex;

#endif

    documentSpecificElements.insert("YEROTHVENTESDEBUT", DATE_TO_STRING(dateEdit_mouvementsDeStocks_debut->date()));

    documentSpecificElements.insert("YEROTHVENTESFIN", DATE_TO_STRING(dateEdit_mouvementsDeStocks_fin->date()));

	return YerothWindowsCommons::imprimer_pdf_document(&documentSpecificElements);
}

void YerothMouvementsDeStocksWindow::rechercher()
{
    _searchFilter.clear();

    QString nom_magasinier(lineEdit_mouvementsDeStocks_nom_magasinier->text());

    if (!nom_magasinier.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT("nom_magasinier", nom_magasinier));
    }

    QString designation(lineEdit_mouvementsDeStocks_designation->text());

    if (!designation.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, designation));
    }

    QString nom_categorie(lineEdit_mouvementsDeStocks_nom_categorie->text());

    if (!nom_categorie.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::CATEGORIE, nom_categorie));
    }

    QString numero_du_bon(lineEdit_mouvementsDeStocks_reference_recu_sortie->text());

    if (!numero_du_bon.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT("sortie_id", numero_du_bon));
    }

    QString nom_recepteur(lineEdit_mouvementsDeStocks_nom_recepteur->text());

    if (!nom_recepteur.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT("nom_recepteur", nom_recepteur));
    }

    setFilter();

    lister_les_elements_du_tableau(_searchFilter);
}


void YerothMouvementsDeStocksWindow::lister_les_elements_du_tableau(QString aSearchFilter)
{
    _logger->log("lister_les_elements_du_tableau");

    _searchFilter = aSearchFilter;

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

    if (0 == _curMouvementsDeStocksTableModel)
    {
        _curMouvementsDeStocksTableModel = &_allWindows->getSqlTableModel_stocks_sorties();
    }

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
        _searchFilter.append(" AND localisation_entree = ''");
    }
    else
    {
        _searchFilter.append(" AND localisation_entree != ''");
    }

    _curMouvementsDeStocksTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);

    _logger->log("lister_les_elements_du_tableau", _searchFilter);

    //qDebug() << "++ lister_les_elements_du_tableau, aSearchFilter: " << _curMouvementsDeStocksTableModel->filter();

    tableView_sorties_articles->lister_les_elements_du_tableau(*_curMouvementsDeStocksTableModel);


    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
        if (_visibleDBFieldColumnStrList.contains(YerothDatabaseTableColumn::LOCALISATION_ENTREE))
        {
        	_visibleDBFieldColumnStrList.removeAll(YerothDatabaseTableColumn::LOCALISATION_ENTREE);
        }

        tableView_sorties_articles->lister_les_elements_du_tableau(*_curMouvementsDeStocksTableModel);

        tableView_show_or_hide_columns(*tableView_sorties_articles);
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_mouvementsDeStocks->currentIndex())
    {
        if (!_visibleDBFieldColumnStrList.contains(YerothDatabaseTableColumn::LOCALISATION_ENTREE))
        {
        	_visibleDBFieldColumnStrList.append(YerothDatabaseTableColumn::LOCALISATION_ENTREE);
        }

        tableView_transferts_articles->lister_les_elements_du_tableau(*_curMouvementsDeStocksTableModel);

        tableView_show_or_hide_columns(*tableView_transferts_articles);
    }

    double quantite_sortie = 0;
    double quantite_sortie_total = 0;

    int curMouvementsDeStocksTableModelRowCount = _curMouvementsDeStocksTableModel->easySelect();

    QSqlRecord aRecord;
    for (int j = 0; j < curMouvementsDeStocksTableModelRowCount; ++j)
    {
        aRecord = _curMouvementsDeStocksTableModel->record(j);

        quantite_sortie = GET_SQL_RECORD_DATA(aRecord, "quantite_sortie").toDouble();
        quantite_sortie_total += quantite_sortie;
    }

    lineEdit_mouvementsDeStocks_quantite_sortie->setText(GET_DOUBLE_STRING(quantite_sortie_total));
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
        _curMouvementsDeStocksTableModel = &_allWindows->getSqlTableModel_stocks_sorties();
    }

    lineEdit_mouvementsDeStocks_nom_magasinier->clear();
    lineEdit_mouvementsDeStocks_designation->clear();
    lineEdit_mouvementsDeStocks_nom_categorie->clear();
    lineEdit_mouvementsDeStocks_reference_recu_sortie->clear();
    lineEdit_mouvementsDeStocks_nom_recepteur->clear();

    if (_allWindows->getUser()->isManager() || _allWindows->getUser()->isMagasinier())
    {
        dateEdit_mouvementsDeStocks_debut->reset();

        dateEdit_mouvementsDeStocks_fin->reset();

        lister_les_elements_du_tableau(QString(""));
    }
}


void YerothMouvementsDeStocksWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");
    resetFilter();
}
