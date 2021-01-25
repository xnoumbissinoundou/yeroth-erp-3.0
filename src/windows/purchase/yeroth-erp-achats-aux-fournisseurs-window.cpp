/*
 * yeroth-erp-achats-aux-fournisseurs-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-achats-aux-fournisseurs-window.hpp"


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


unsigned int YerothAchatsAUXFournisseursWindow::PDF_LISTING_COLUMN_STOCKS_ID(1);

unsigned int YerothAchatsAUXFournisseursWindow::PDF_LISTING_COLUMN_REFERENCE(3);

unsigned int YerothAchatsAUXFournisseursWindow::PDF_LISTING_COLUMN_DESIGNATION(2);

unsigned int YerothAchatsAUXFournisseursWindow::PDF_LISTING_COLUMN_CATEGORIE(4);


YerothAchatsAUXFournisseursWindow::YerothAchatsAUXFournisseursWindow()
:YerothWindowsCommons("yeroth-erp-achats-aux-fournisseurs"),
 YerothAbstractClassYerothSearchWindow(_allWindows->ACHATS_AUX_FOURNISSEURS),
 _logger(new YerothLogger("YerothAchatsAUXFournisseursWindow")),
 _pushButton_achats_aux_fournisseurs_filtrer_font(0),
 _curAchatAUXFournisseursSqlTableModel(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("fiche des achats aux fournisseurs"));

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_achats_aux_fournisseurs);


    _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
		<< pushButton_afficher
		<< pushButton_supprimer;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionExporter_au_format_csv
		<< actionAfficherPDF
		<< actionSupprimerUnAchatAuFournisseur
		<< actionAfficher_achat_au_detail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_achats_aux_fournisseurs)

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_170, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(_allWindows->ACHATS_AUX_FOURNISSEURS);


    _lineEditsToANDContentForSearch.insert(&lineEdit_achats_aux_fournisseurs_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _yeroth_WINDOW_references_dbColumnString.insert(YerothDatabaseTableColumn::REFERENCE);
    _yeroth_WINDOW_references_dbColumnString.insert(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(_allWindows->ACHATS_AUX_FOURNISSEURS);

    reinitialiser_champs_db_visibles();

    _curAchatAUXFournisseursSqlTableModel = &_allWindows->getSqlTableModel_achats_aux_fournisseurs();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setupDateTimeEdits();

    _pushButton_achats_aux_fournisseurs_filtrer_font = new QFont(pushButton_achats_aux_fournisseurs_filtrer->font());

    tableView_achats_aux_fournisseurs->setSqlTableName(&YerothERPWindows::ACHATS_AUX_FOURNISSEURS);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_achats_aux_fournisseurs_filtrer->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerUnAchatAuFournisseur, SIGNAL(triggered()), this, SLOT(supprimer_un_achat_au_fournisseur()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(afficherStocks()));
    connect(actionVentes, SIGNAL(triggered()), this, SLOT(ventes()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));
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

    connect(tableView_achats_aux_fournisseurs, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(YerothDatabaseTableColumn::STOCKS_ID);

    setupShortcuts();
}

YerothAchatsAUXFournisseursWindow::~YerothAchatsAUXFournisseursWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _logger;
}


bool YerothAchatsAUXFournisseursWindow::filtrer_achats()
{
	QString achatsTableColumnValue(comboBox_element_achats_aux_fournisseurs_condition->currentText());

	if (achatsTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
				QObject::trUtf8("filtrer"),
				msg);

		return false;
	}

	reinitialiser_recherche();

	QString achatsTableColumnProperty(comboBox_element_achats_aux_fournisseurs->currentText());

	QString mathOperator(comboBox_element_achats_aux_fournisseurs_condition->currentText());

	QString REAL_DB_ID_NAME_achatsTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(achatsTableColumnProperty));

	QString filterString;

	filterString.append(QString("%1 %2 %3")
			.arg(REAL_DB_ID_NAME_achatsTableColumnProperty,
					mathOperator,
					achatsTableColumnValue));

	_curAchatAUXFournisseursSqlTableModel->yerothSetFilter_WITH_where_clause(filterString);

	int resultRows = _curAchatAUXFournisseursSqlTableModel->easySelect();

	if (resultRows > 0)
	{
		setCurrentlyFiltered(true);

		lister_les_elements_du_tableau(*_curAchatAUXFournisseursSqlTableModel);

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "achats-aux-fournisseurs - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "achats-aux-fournisseurs - filtrer");

		return false;
	}

	set_filtrer_font();

	return false;
}


void YerothAchatsAUXFournisseursWindow::populateComboBoxes()
{
	QStringList aQStringList;

	aQStringList.append(_varchar_dbtable_column_name_list.values());

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

    comboBox_element_achats_aux_fournisseurs->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_element_achats_aux_fournisseurs_condition->addItems(aQStringList);
}


void YerothAchatsAUXFournisseursWindow::setupLineEdits()
{
	lineEdit_achats_aux_fournisseurs_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (désignation de l'article)"));

	lineEdit_nom_element_string_db->enableForSearch(QObject::trUtf8("valeur à rechercher"));

	lineEdit_achats_aux_fournisseurs_nombre_dachats->setYerothEnabled(false);

	comboBox_element_achats_aux_fournisseurs_condition->setValidator(&YerothUtils::DoubleValidator);

	MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_achats_aux_fournisseurs_nombre_de_lignes_par_page, tableView_achats_aux_fournisseurs);
}


void YerothAchatsAUXFournisseursWindow::setupDateTimeEdits()
{
    dateEdit_achats_aux_fournisseurs_debut->setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

    dateEdit_achats_aux_fournisseurs_fin->setStartDate(GET_CURRENT_DATE);

    _achats_aux_fournisseursDateFilter.clear();

    _achats_aux_fournisseursDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    					.arg(YerothDatabaseTableColumn::DATE_DE_COMMANDE,
    						 DATE_TO_DB_FORMAT_STRING(dateEdit_achats_aux_fournisseurs_debut->date()),
							 YerothDatabaseTableColumn::DATE_DE_COMMANDE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_achats_aux_fournisseurs_fin->date())));

    connect(dateEdit_achats_aux_fournisseurs_debut,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_achats_aux_fournisseurs_fin,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));
}


void YerothAchatsAUXFournisseursWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 		(*actionAppeler_aide);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe			(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothAchatsAUXFournisseursWindow::resetFilter(YerothSqlTableModel * achatsTableModel)
{
	_curAchatAUXFournisseursSqlTableModel = achatsTableModel;

    if (0 != _curAchatAUXFournisseursSqlTableModel)
    {
    	_curAchatAUXFournisseursSqlTableModel->resetFilter();
    }

    lineEdit_achats_aux_fournisseurs_terme_recherche->myClear();

    dateEdit_achats_aux_fournisseurs_debut->reset();
    dateEdit_achats_aux_fournisseurs_fin->reset();
}


void YerothAchatsAUXFournisseursWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();

	if (0 != _curAchatAUXFournisseursSqlTableModel)
	{
		lister_les_elements_du_tableau(*_curAchatAUXFournisseursSqlTableModel);
	}
}


void YerothAchatsAUXFournisseursWindow::textChangedSearchLineEditsQCompleters()
{
	comboBox_element_achats_aux_fournisseurs_condition->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_achats_aux_fournisseurs_terme_recherche->text());

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

    QString finalSearchFilter(_achats_aux_fournisseursDateFilter);

    if (!_searchFilter.isEmpty())
    {
    	QString searchFilterWithDate(QString("%1 AND (%2)")
    									.arg(_achats_aux_fournisseursDateFilter,
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
        qDebug() << QString("++ YerothAchatsAUXFournisseursWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerothAchatsAUXFournisseursWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBColumnNameStrList.clear();

	_visibleDBColumnNameStrList
		<< YerothDatabaseTableColumn::DESIGNATION
		<< YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
		<< YerothDatabaseTableColumn::CATEGORIE
		<< YerothDatabaseTableColumn::PRIX_DACHAT
		<< YerothDatabaseTableColumn::MONTANT_TVA
		<< YerothDatabaseTableColumn::PRIX_VENTE
		<< YerothDatabaseTableColumn::MARGE_BENEFICIAIRE
		<< YerothDatabaseTableColumn::DATE_DE_COMMANDE
		<< YerothDatabaseTableColumn::QUANTITE_TOTALE;

}


void YerothAchatsAUXFournisseursWindow::contextMenuEvent(QContextMenuEvent * event)
{
    if (tableView_achats_aux_fournisseurs->rowCount() > 0)
    {
        QMenu menu(this);
        menu.setPalette(toolBar_achats_aux_fournisseursWindow->palette());
        menu.addAction(actionAfficher_achat_au_detail);
        menu.exec(event->globalPos());
    }
}


void YerothAchatsAUXFournisseursWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setYerothSqlTableModel(_curAchatAUXFournisseursSqlTableModel);

    _curStocksTableModel = stocksTableModel;

    lineEdit_achats_aux_fournisseurs_terme_recherche->setFocus();

    setVisible(true);

    afficher_achats();
}


void YerothAchatsAUXFournisseursWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_achats_aux_fournisseurs_filtrer->disable(this);
    pushButton_achats_aux_fournisseurs_reinitialiser_filtre->disable(this);

    pushButton_supprimer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_menu_principal->disable(this);
}

void YerothAchatsAUXFournisseursWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curAchatAUXFournisseursSqlTableModel)

    pushButton_achats_aux_fournisseurs_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_aux_fournisseurs_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));

    pushButton_supprimer->enable(this, SLOT(supprimer_un_achat_au_fournisseur()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
}


void YerothAchatsAUXFournisseursWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_achats_aux_fournisseurs_filtrer->disable(this);
    pushButton_achats_aux_fournisseurs_reinitialiser_filtre->disable(this);

    pushButton_supprimer->enable(this, SLOT(supprimer_un_achat_au_fournisseur()));
    pushButton_afficher->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_menu_principal->disable(this);
}


void YerothAchatsAUXFournisseursWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_achats_aux_fournisseurs_filtrer->disable(this);
    pushButton_achats_aux_fournisseurs_reinitialiser_filtre->disable(this);

    pushButton_supprimer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_menu_principal->disable(this);
}

void YerothAchatsAUXFournisseursWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_achats_aux_fournisseurs_filtrer->disable(this);
    pushButton_achats_aux_fournisseurs_reinitialiser_filtre->disable(this);

    pushButton_supprimer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_menu_principal->disable(this);
}

void YerothAchatsAUXFournisseursWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_achats_aux_fournisseurs_filtrer->disable(this);
    pushButton_achats_aux_fournisseurs_reinitialiser_filtre->disable(this);

    pushButton_supprimer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_menu_principal->disable(this);
}

void YerothAchatsAUXFournisseursWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    if (get_INT_LastListerSelectedRow__ID() > -1 && _curAchatAUXFournisseursSqlTableModel->rowCount() > 0)
    {
        _allWindows->_achatsDetailWindow->rendreVisible(_curStocksTableModel,
														_curAchatAUXFournisseursSqlTableModel);
        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::trUtf8("détails"),
                              QObject::trUtf8("Sélectionnez 1 achat au fournisseur à afficher les détails."));

        return ;
    }
}


void YerothAchatsAUXFournisseursWindow::afficher_au_detail(const QModelIndex & modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    if (_curAchatAUXFournisseursSqlTableModel->rowCount() > 0)
    {
    	//qDebug() << "++ test" << modelIndex.row();
        _allWindows->_achatsDetailWindow->rendreVisible(_curStocksTableModel,
														_curAchatAUXFournisseursSqlTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::trUtf8("détails"),
                              QObject::trUtf8("Sélectionnez 1 achat au fournisseur à afficher les détails."));

        return ;
    }
}


void YerothAchatsAUXFournisseursWindow::refineYerothLineEdits()
{
	_achats_aux_fournisseursDateFilter.clear();

	_achats_aux_fournisseursDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    					.arg(YerothDatabaseTableColumn::DATE_DE_COMMANDE,
    						 DATE_TO_DB_FORMAT_STRING(dateEdit_achats_aux_fournisseurs_debut->date()),
							 YerothDatabaseTableColumn::DATE_DE_COMMANDE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_achats_aux_fournisseurs_fin->date())));

	setupLineEditsQCompleters((QObject *)this);

	afficher_achats();
}


void YerothAchatsAUXFournisseursWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    comboBox_element_achats_aux_fournisseurs_condition->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothAchatsAUXFournisseursWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_nom_element_string_db->clear();

    comboBox_element_achats_aux_fournisseurs_condition->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_achats_aux_fournisseurs());

    resetLineEditsQCompleters((QObject *)this);

    afficher_achats();
}


void YerothAchatsAUXFournisseursWindow::lister_les_elements_du_tableau(YerothSqlTableModel &achatSqlTableModel)
{
    int curAchatsTableModelRowCount = achatSqlTableModel.easySelect();

    double quantite_totale = 0.0;
    double prix_dachat = 0.0;
    double montant_total_achat = 0.0;

    double montant_total = 0.0;

    QSqlRecord aRecord;

    for (int k = 0; k < curAchatsTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = achatSqlTableModel.record(k);

        quantite_totale = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::QUANTITE_TOTALE).toDouble();

        prix_dachat = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::PRIX_DACHAT).toDouble();

        montant_total_achat = prix_dachat * quantite_totale;

        montant_total += montant_total_achat;
    }

    lineEdit_achats_aux_fournisseurs_total_achats->setText(GET_CURRENCY_STRING_NUM(montant_total));


	tableView_achats_aux_fournisseurs->queryYerothTableViewCurrentPageContentRow(achatSqlTableModel);

    tableView_show_or_hide_columns(*tableView_achats_aux_fournisseurs);

    int rowCount = tableView_achats_aux_fournisseurs->rowCount();

    lineEdit_achats_aux_fournisseurs_nombre_dachats->setText(GET_NUM_STRING(rowCount));
}


void YerothAchatsAUXFournisseursWindow::afficher_stock_selectioner(const QString & stockName)
{
    _logger->log("afficher_stock_selectioner(const QString &)");

    //qDebug() << QString("afficher_stock_selectioner(%1)").arg(stockName);

    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _curAchatAUXFournisseursSqlTableModel->yerothSetFilter_WITH_where_clause(filter);

    if (_curAchatAUXFournisseursSqlTableModel->easySelect() > 0)
    {
        lister_les_elements_du_tableau(*_curAchatAUXFournisseursSqlTableModel);
    }
}


void YerothAchatsAUXFournisseursWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
    	_pushButton_achats_aux_fournisseurs_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_achats_aux_fournisseurs_filtrer_font->setUnderline(false);
    }

    pushButton_achats_aux_fournisseurs_filtrer->setFont(*_pushButton_achats_aux_fournisseurs_filtrer_font);
}


bool YerothAchatsAUXFournisseursWindow::supprimer_un_achat_au_fournisseur()
{
    if (get_INT_LastListerSelectedRow__ID() <= -1 ||
    	_curAchatAUXFournisseursSqlTableModel->rowCount() <= 0)
    {
        YerothQMessageBox::information(this, QObject::trUtf8("supprimer"),
                                  	   QObject::trUtf8("Sélectionnez 1 achat (service) au fournisseur à supprimer."));

        return false;
    }

	return false;
}


bool YerothAchatsAUXFournisseursWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_achats_aux_fournisseurs,
										   tableColumnsToIgnore,
										   "yeroth-erp-fiche-achats-aux-fournisseurs",
										   "fiche des achats aux fournisseurs");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_achats_aux_fournisseurs,
										   tableColumnsToIgnore,
										   "yeroth-erp-supplier-purchase-listing",
										   "supplier purchase listing file");
#endif

	return success;
}


bool YerothAchatsAUXFournisseursWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_lister_achats_tex;
	return YerothWindowsCommons::imprimer_pdf_document();
}

