/*
 * yeroth-erp-marchandises-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-marchandises-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-style.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


YerothMarchandisesWindow::YerothMarchandisesWindow()
:YerothWindowsCommons("yeroth-erp-marchandises"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::MARCHANDISES),
 _logger(new YerothLogger("YerothMarchandisesWindow")),
 _NON_TERMINEES_MARCHANDISE_RE_ENTRANT(false),
 _qteTotaleDarticlesEnStock(0.0),
 _currentlyFiltered(false),
 _lastSelectedRow__ID(0),
 _pushButton_filtrer_font(0),
 _curMarchandisesTableModel(&_allWindows->getSqlTableModel_marchandises())
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::tr("les marchandises"));

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_marchandises);


//    _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
//		<< ;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionExporter_au_format_csv
		<< action_parametrer_les_impressions
		<< actionAfficherPDF
		<< actionSupprimer_cette_marchandise
		<< actionModifierMarchandise;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_marchandises)

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                   .arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48,
                                		COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
		<< YerothDatabaseTableColumn::STOCK_MINIMUM
		<< YerothDatabaseTableColumn::STOCK_MAXIMUM
		<< YerothDatabaseTableColumn::STOCK_DALERTE
    	<< YerothDatabaseTableColumn::IS_SERVICE;


    setup_select_configure_dbcolumn(YerothDatabase::MARCHANDISES);


    _lineEditsToANDContentForSearch.insert(&lineEdit_marchandises_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _yeroth_WINDOW_references_dbColumnString.insert(YerothDatabaseTableColumn::REFERENCE);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerothDatabase::MARCHANDISES);

    reinitialiser_colones_db_visibles();

    setupLineEdits();

    localSetupLineEditsQCompleters();

    populateMarchandisesComboBoxes();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_marchandises->setSqlTableName(&YerothDatabase::MARCHANDISES);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_cette_marchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_reinitialiser_filtre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer_un_stock()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionAfficher_les_marchandises_non_terminees, SIGNAL(triggered()), this, SLOT(slot_filtrer_non_empty_product_stock()));
    connect(actionAfficher_les_marchandises_terminees, SIGNAL(triggered()), this, SLOT(slot_filter_empty_product_stock()));
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

    connect(checkBox_services, SIGNAL(stateChanged(int)), this, SLOT(handle_services_checkBox(int)));

    connect(actionModifierMarchandise, SIGNAL(triggered()), this, SLOT(modifier_marchandise()));

    connect(actionSupprimer_cette_marchandise, SIGNAL(triggered()), this, SLOT(supprimer_cette_marchandise()));

    setupShortcuts();
}


YerothMarchandisesWindow::~YerothMarchandisesWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

	delete _pushButton_filtrer_font;
    delete _logger;
}


void YerothMarchandisesWindow::reinitialiser_colones_db_visibles()
{
	_visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
		<< YerothDatabaseTableColumn::DESIGNATION
		<< YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
		<< YerothDatabaseTableColumn::CATEGORIE
		<< YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT
		<< YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT
		<< YerothDatabaseTableColumn::REFERENCE;
}


void YerothMarchandisesWindow::contextMenuEvent(QContextMenuEvent * event)
{
    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager() 				||
            aUser->isGestionaireDesStocks() ||
            aUser->isMagasinier())
        {
            if (tableView_marchandises->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_marchandisesWindow->palette());
                menu.addAction(actionModifierMarchandise);
                menu.addAction(actionSupprimer_cette_marchandise);
                menu.exec(event->globalPos());
            }
        }
    }
}

void YerothMarchandisesWindow::setupShortcuts()
{
	setupShortcutActionMessageDaide 		(*actionAfficher_les_marchandises_terminees);
    setupShortcutActionMessageDaide 		(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe			(*actionQui_suis_je);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);

    actionAfficher_les_marchandises_non_terminees->setShortcut(YerothUtils::AFFICHER_LES_MARCHANDISES_NON_TERMINES_QKEYSEQUENCE);
    actionAfficher_les_marchandises_terminees->setShortcut(YerothUtils::AFFICHER_LES_MARCHANDISES_TERMINES_QKEYSEQUENCE);
    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothMarchandisesWindow::slot_reinitialiser_colones_db_visibles()
{
	reinitialiser_colones_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();
	afficherMarchandises();
}


void YerothMarchandisesWindow::textChangedSearchLineEditsQCompleters()
{
	lineEdit_marchandises_element_de_stock_resultat->clear();

	if (!_current_filtering_non_empty_stock_SQL_QUERY.isEmpty())
	{
		setCurrentlyFiltered(false);

		clearSearchFilter();
	}

    QString searchTerm(lineEdit_marchandises_terme_recherche->text());

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

    QMapIterator <YerothLineEdit **, QString> it(_lineEditsToANDContentForSearch);

    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    while (it.hasNext())
    {
    	it.next();

    	aYerothLineEdit = *it.key();

    	correspondingDBFieldKeyValue = it.value();

    	if (0 != aYerothLineEdit)
    	{
    		aTableColumnFieldContentForANDSearch = aYerothLineEdit->text();

    		if (!correspondingDBFieldKeyValue.isEmpty() 	  &&
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

    if (_current_filtering_non_empty_stock_SQL_QUERY.isEmpty())
    {
    	_yerothSqlTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);

    	if (_yerothSqlTableModel->select())
    	{
    		afficherMarchandises(*_yerothSqlTableModel);
    	}
    	else
    	{
    		qDebug() << QString("++ YerothMarchandisesWindow::textChangedSearchLineEditsQCompleters(): %1")
        						.arg(_yerothSqlTableModel->lastError().text());
    	}
    }
    else
    {
    	afficherMarchandises(*_yerothSqlTableModel);
    }

    handle_some_actions_tools_enabled();
}


void YerothMarchandisesWindow::entrer_un_stock()
{
    if (getLastListerSelectedRow__ID_AS_INTEGER() > -1 &&
    	_curMarchandisesTableModel->rowCount() > 0)
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel,
        										  _curMarchandisesTableModel);
        rendreInvisible();
    }
    else
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
        rendreInvisible();
    }
}


void YerothMarchandisesWindow::handle_services_checkBox(int state)
{
	if (checkBox_services->isChecked())
	{
		_curMarchandisesTableModel->yerothSetFilter_WITH_where_clause(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
    										  	  	  	   YerothUtils::MYSQL_TRUE_LITERAL));
    }
    else
    {
    	_curMarchandisesTableModel->yerothSetFilter_WITH_where_clause(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
    										  	  	  	   YerothUtils::MYSQL_FALSE_LITERAL));
	}

	_curMarchandisesTableModel->easySelect();

	lineEdit_nom_element_string_db->clear();

	if (-1 != state)
	{
		reinitialiser_elements_filtrage();
	}

	afficherMarchandises();

	localSetupLineEditsQCompleters();
}


bool YerothMarchandisesWindow::slot_filtrer_non_empty_product_stock()
{
	_curMarchandisesTableModel->resetFilter();

	QString cur_select_stmt = _curMarchandisesTableModel->yerothSelectStatement();

	QString SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS =
			QString(" ((SELECT DISTINCT stocks.reference, "
					"stocks.designation FROM stocks WHERE %1 > 0) limit 0, %2) T "
					"INNER JOIN marchandises ON marchandises.%3 = T.%4")
				.arg(YerothDatabaseTableColumn::QUANTITE_TOTALE,
					 lineEdit_marchandises_nombre_de_lignes_par_page->text(),
					 YerothDatabaseTableColumn::DESIGNATION,
					 YerothDatabaseTableColumn::DESIGNATION);

//	QDEBUG_STRING_OUTPUT_2("SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS",
//						   SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS);

	int from_index = cur_select_stmt.indexOf("from", 0, Qt::CaseInsensitive);

	cur_select_stmt.remove(from_index, cur_select_stmt.length());

	QStringList cur_select_stmt_string_list_splitted = cur_select_stmt.split(',');

	cur_select_stmt.clear();

	QString a_cur_element_split;

	uint cur_select_stmt_string_list_splitted__SIZE = cur_select_stmt_string_list_splitted.size();

	for (uint k = 0; k < cur_select_stmt_string_list_splitted__SIZE; ++k)
	{
		a_cur_element_split = cur_select_stmt_string_list_splitted.at(k).trimmed();

		if (0 == k)
		{
			QStringList select_splitted =
					a_cur_element_split.split(YerothUtils::EMPTY_SPACE_REGEXP);

			cur_select_stmt.append(QString("SELECT %1.%2, ")
					.arg(YerothDatabase::MARCHANDISES,
							select_splitted.at(1)));
		}
		else if (k < cur_select_stmt_string_list_splitted__SIZE - 1)
		{
			cur_select_stmt.append(QString("%1.%2, ")
					.arg(YerothDatabase::MARCHANDISES,
							a_cur_element_split));
		}
		else if (k < cur_select_stmt_string_list_splitted__SIZE)
		{
			cur_select_stmt.append(QString("%1.%2 FROM ")
					.arg(YerothDatabase::MARCHANDISES,
							a_cur_element_split));
		}
	}


	cur_select_stmt.append(SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS);

	_current_filtering_non_empty_stock_SQL_QUERY = cur_select_stmt;


	int resultRows = _curMarchandisesTableModel->yerothSetQueryRowCount(cur_select_stmt);


//	QDEBUG_STRING_OUTPUT_2("_curMarchandisesTableModel", _curMarchandisesTableModel->query().lastQuery());


	if (resultRows >= 0)
	{
		textChangedSearchLineEditsQCompleters();

		setWindowTitle(QObject::trUtf8("%1 - les marchandises NON TERMINÉES")
							.arg(YEROTH_ERP_WINDOW_TITLE));

		MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS;

		setCurrentlyFiltered(true);

		if (!_NON_TERMINEES_MARCHANDISE_RE_ENTRANT)
		{
			YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "non terminées");
		}

		_NON_TERMINEES_MARCHANDISE_RE_ENTRANT = true;

		return true;
	}
	else
	{
		setWindowTitle(QObject::tr("%1 - les marchandises")
							.arg(YEROTH_ERP_WINDOW_TITLE));

		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "non terminées");
	}

	return false;
}


bool YerothMarchandisesWindow::slot_filter_empty_product_stock()
{
	QStandardItem *anItem = 0;

	//This is clear from the way our project is organized
	QStandardItemModel *stdItemModel =
			(QStandardItemModel *) tableView_marchandises->model();

	if (0 == stdItemModel)
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "marchandises - filtrer");

		return false;
	}

	_curMarchandisesTableModel->resetFilter();

	afficherMarchandises();

	QString filter_string;

	QStringList filter_string_list;

	QStringList splittedData;

	QString data;

	QString nom_departement_str;
	QString categorie_str;
	QString designation_str;

//	QDEBUG_STRING_OUTPUT_2("stdItemModel->rowCount()", QString::number(stdItemModel->rowCount()));

	for (int k = 0; k < stdItemModel->rowCount(); ++k)
	{
		anItem = stdItemModel->item(k, 0);

		if (0 != anItem)
		{
			if (YerothUtils::YEROTH_WHITE_COLOR_FOR_EMPTY_MERCHANDISE_FILTERING == anItem->foreground().color())
			{
				data = anItem->accessibleText();

//				QDEBUG_STRING_OUTPUT_2("data", data);

				splittedData = data.split("|");

				if (splittedData.size() > 1)
				{
					nom_departement_str = splittedData.at(0);
					categorie_str 		= splittedData.at(1);
					designation_str 	= splittedData.at(2);

					filter_string_list
						.append(QString("(nom_departement_produit = '%1' AND categorie = '%2' AND designation = '%3')")
								 .arg(nom_departement_str,
									  categorie_str,
									  designation_str));

					if (k + 1 < stdItemModel->rowCount())
					{
						filter_string_list.append(" OR ");
					}
				}
			}
		}
	}


	if (filter_string_list.isEmpty())
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "terminées");

		return false;
	}

	if (YerothUtils::isEqualCaseInsensitive(filter_string_list.last(), " OR "))
	{
		filter_string_list.removeLast();
	}

	filter_string = filter_string_list.join(" ");

	if (filter_string.isEmpty())
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "terminées");

		return false;
	}

	_curMarchandisesTableModel->yerothSetFilter_WITH_where_clause(filter_string);

	int resultRows = _curMarchandisesTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);

		afficherMarchandises(*_curMarchandisesTableModel);

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "terminées");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "terminées");
	}

	return false;
}


bool YerothMarchandisesWindow::slot_filtrer()
{
	QString stockTableColumnValue(lineEdit_marchandises_element_de_stock_resultat->text());

	if (stockTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
									  QObject::trUtf8("filtrer"),
									  msg);
		return false;
	}

	reinitialiser_recherche();

	QString stockTableColumnProperty(comboBox_inventaire_des_stocks_element_de_stock->currentText());

	QString mathOperator(comboBox_inventaire_des_stocks_element_de_stock_condition->currentText());

	QString filterString;

	QString REAL_DB_ID_NAME_stockTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(stockTableColumnProperty));

	filterString.append(QString("%2 %3 '%4'")
							.arg(REAL_DB_ID_NAME_stockTableColumnProperty,
								 mathOperator,
								 stockTableColumnValue));

//	qDebug() << QString("filterString: %1")
//					.arg(filterString);

	_curMarchandisesTableModel->yerothSetFilter_WITH_where_clause(filterString);

//	QDEBUG_STRING_OUTPUT_2("_curMarchandisesTableModel->yerothSelectStatement()",
//			_curMarchandisesTableModel->yerothSelectStatement());

	int resultRows = _curMarchandisesTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);

		afficherMarchandises(*_curMarchandisesTableModel);

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "marchandises - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "marchandises - filtrer");
	}

	return false;
}


void YerothMarchandisesWindow::localSetupLineEditsQCompleters()
{
	QString aConditionStr;

    if (checkBox_services->isChecked())
    {
    	aConditionStr = YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE, "1");
    }
    else
    {
    	aConditionStr = YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE, "0");
    }

    setupLineEditsQCompleters((QObject *)this, aConditionStr);
}


void YerothMarchandisesWindow::set_filtrer_font()
{
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


void YerothMarchandisesWindow::populateMarchandisesComboBoxes()
{
	QStringList aQStringList;

	aQStringList.append(_varchar_dbtable_column_name_list.values());

	aQStringList.removeAll(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT);

	comboBox_element_string_db->addItems(aQStringList,
			YerothDatabaseTableColumn::_tableColumnToUserViewString);

	comboBox_element_string_db->setCurrentIndex(0);


	aQStringList.clear();

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT));

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT));

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::QUANTITE_TOTALE));

    comboBox_inventaire_des_stocks_element_de_stock->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_inventaire_des_stocks_element_de_stock_condition->addItems(aQStringList);
}


void YerothMarchandisesWindow::setupLineEdits()
{
	_QLINEEDIT_nombre_de_lignes_par_page = lineEdit_marchandises_nombre_de_lignes_par_page;

	lineEdit_marchandises_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (description de l'article (ou service))"));

	lineEdit_nom_element_string_db->enableForSearch(QObject::trUtf8("valeur à rechercher"));

    lineEdit_nombre_de_marchandises->setYerothEnabled(false);

    lineEdit_nombre_darticles->setYerothEnabled(false);

	MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_marchandises_nombre_de_lignes_par_page, tableView_marchandises);

	lineEdit_marchandises_element_de_stock_resultat->setValidator(&YerothUtils::POSITIVE_DoubleValidator);
}


void YerothMarchandisesWindow::rendreInvisible()
{
	tableView_marchandises->stopEditingModeSelection();

	YerothWindowsCommons::rendreInvisible();
}


void YerothMarchandisesWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
	YerothPOSUser *aCurrentUser = _allWindows->getUser();

    if (0 != aCurrentUser && !aCurrentUser->isManager())
    {
    	checkBox_services->setChecked(false);

    	checkBox_services->setVisible(false);
    }
    else
    {
    	checkBox_services->setVisible(true);
    }

    setupLineEdits();

    setYerothSqlTableModel(_curMarchandisesTableModel);

    _curStocksTableModel = stocksTableModel;

    lineEdit_marchandises_terme_recherche->setFocus();

	setVisible(true);

	APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

	if (!IS__CURRENTLY__CHECKING__NON__EMPTY__STOCKS())
	{
		reinitialiser__FILTRE__MARCHANDISES__NON__TERMINEES();

		QString currentFilter(_curMarchandisesTableModel->filter());

		if (!currentFilter.isEmpty())
		{
			currentFilter.append(QString(" AND "));
		}

		if (checkBox_services->isChecked())
		{
			currentFilter.append(QString("%1")
					.arg(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
							YerothUtils::MYSQL_TRUE_LITERAL)));
		}
		else
		{
			currentFilter.append(QString("%1")
					.arg(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
							YerothUtils::MYSQL_FALSE_LITERAL)));
		}

		_curMarchandisesTableModel->yerothSetFilter_WITH_where_clause(currentFilter);

		//	QDEBUG_STRING_OUTPUT_2("currentFilter", currentFilter);

		afficherMarchandises();

		localSetupLineEditsQCompleters();
	}
	else
	{
		setCurrentlyFiltered(true);

		slot_filtrer_non_empty_product_stock();
	}
}


void YerothMarchandisesWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

    definirPasDeRole();
}


void YerothMarchandisesWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_cette_marchandise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curMarchandisesTableModel)

    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_filtrer->enable(this, SLOT(slot_filtrer()));
    pushButton_entrer->enable(this, SLOT(entrer_un_stock()));
    pushButton_rapports->enable(this, SLOT(tableaux_de_bords()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}


void YerothMarchandisesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_cette_marchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curMarchandisesTableModel)

    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_filtrer->enable(this, SLOT(slot_filtrer()));
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->disable(this);
    pushButton_sortir->disable(this);
}


void YerothMarchandisesWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_cette_marchandise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curMarchandisesTableModel)

    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_filtrer->enable(this, SLOT(slot_filtrer()));
    pushButton_entrer->enable(this, SLOT(entrer_un_stock()));
    pushButton_rapports->disable(this);
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}

void YerothMarchandisesWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}

void YerothMarchandisesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_cette_marchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_reinitialiser_filtre->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_menu_principal->disable(this);
}


void YerothMarchandisesWindow::afficher_stock_selectioner(const QString & stockName)
{
    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _curMarchandisesTableModel->yerothSetFilter_WITH_where_clause(filter);

    if (_curMarchandisesTableModel->easySelect() > 0)
    {
        afficherMarchandises();
    }
}


void YerothMarchandisesWindow::modifier_marchandise()
{
	if (0 != _curMarchandisesTableModel)
	{
		tableView_marchandises->startEditingModeSelection();
	}
}


void YerothMarchandisesWindow::supprimer_PLUSIEURS_Marchandises(YerothSqlTableModel &aMarchandisesTableModel)
{
	QMapIterator<QString, QString> j(tableView_marchandises->lastSelected_Rows__IDs());

	while (j.hasNext())
	{
		j.next();

		supprimer_cette_marchandise(j.value(), true);
	}

    tableView_marchandises->clearSelection();
}


void YerothMarchandisesWindow::supprimer_cette_marchandise(QString aMarchandiseID /* = YerothUtils::EMPTY_STRING */,
														   bool _reEntrant /* = false */)
{
    if (!_reEntrant && tableView_marchandises->lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
    	supprimer_PLUSIEURS_Marchandises(*_curMarchandisesTableModel);

    	afficherMarchandises();

    	return ;
    }

    QSqlRecord record;

    if (!aMarchandiseID.isEmpty())
    {
    	static QSqlQuery query;

    	query.clear();

    	QString QUERY_MARCHANDISE_DATA(QString("select * from %1 where %2='%3'")
    										.arg(YerothDatabase::MARCHANDISES,
    											 YerothDatabaseTableColumn::ID,
												 aMarchandiseID));

    	int querySize = YerothUtils::execQuery(query, QUERY_MARCHANDISE_DATA);

    	if (query.next())
    	{
    		record = query.record();
    	}
    }
    else
    {
        _allWindows->_marchandisesWindow->
    			SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);
    }

    QString msgSupprimer(QObject::trUtf8("Poursuivre avec la suppression de la marchandise '%1' ?")
    						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                       QObject::tr("suppression"),
                                       msgSupprimer,
									   QMessageBox::Cancel,
									   QMessageBox::Ok))
    {
    	bool resRemoved =  false;

    	if (!aMarchandiseID.isEmpty())
    	{
    		QString DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING
				(QString("DELETE FROM %1 WHERE %2 = '%3'")
    				.arg(YerothDatabase::MARCHANDISES,
    					 YerothDatabaseTableColumn::ID,
						 aMarchandiseID));

    		resRemoved = YerothUtils::execQuery(DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

//    		QDEBUG_STRING_OUTPUT_2(QString("resRemoved: %1, stocksID: %2")
//    									.arg(BOOL_TO_STRING(resRemoved),
//    										 aStockID),
//											 DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);
    	}
    	else
    	{
        	resRemoved = _allWindows->_marchandisesWindow->
        			SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW();
    	}

        //qDebug() << "YerothInventaireDesStocksWindow::supprimer_ce_stock() " << resRemoved;
        if (resRemoved && _curMarchandisesTableModel->select())
        {
            msgSupprimer = QObject::trUtf8("La marchandise '%1' a été supprimée.")
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

            if (!_reEntrant)
            {
            	tableView_marchandises->clearSelection();
            }

            YerothQMessageBox::information(this,
                                          QObject::trUtf8("succès"),
                                          msgSupprimer);
        }
        else
        {
            msgSupprimer = QObject::trUtf8("L'article \"%1\" ne pouvait pas être supprimé !")
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

            YerothQMessageBox::warning(this,
                                       QObject::trUtf8("échec"),
                                       msgSupprimer);
        }

        afficherMarchandises();

        localSetupLineEditsQCompleters();
    }
    else
    {
    }
}


void YerothMarchandisesWindow::reinitialiser__FILTRE__MARCHANDISES__NON__TERMINEES()
{
	MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curMarchandisesTableModel)

	_NON_TERMINEES_MARCHANDISE_RE_ENTRANT = false;

	setWindowTitle(QObject::tr("%1 - les marchandises")
						.arg(YEROTH_ERP_WINDOW_TITLE));

	_current_filtering_non_empty_stock_SQL_QUERY.clear();
}


void YerothMarchandisesWindow::reinitialiser_elements_filtrage()
{
	reinitialiser__FILTRE__MARCHANDISES__NON__TERMINEES();

    lineEdit_marchandises_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothMarchandisesWindow::reinitialiser_recherche()
{
	reinitialiser__FILTRE__MARCHANDISES__NON__TERMINEES();

	lineEdit_nom_element_string_db->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *)this);

    handle_services_checkBox(-1);
}


void YerothMarchandisesWindow::afficherMarchandises(YerothSqlTableModel &aYerothSqlTableModel)
{
	tableView_marchandises->queryYerothTableViewCurrentPageContentRow(aYerothSqlTableModel);

    tableView_show_or_hide_columns(*tableView_marchandises);

    int rowCount = tableView_marchandises->getYerothSqlTableModelTotalRowCount();

    lineEdit_nombre_de_marchandises->setText(GET_NUM_STRING(rowCount));
}


bool YerothMarchandisesWindow::export_csv_file()
{
	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_marchandises,
										   tableColumnsToIgnore,
										   "yeroth-erp-fiche-marchandises",
										   "fiche des marchandises");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_marchandises,
										   tableColumnsToIgnore,
										   "yeroth-erp-merchandise-listing",
										   "merchandise listing file");
#endif

	return success;
}


QString YerothMarchandisesWindow::get_latex_template_print_pdf_content()
{
	_latex_template_print_pdf_content = YerothUtils::template_marchandises_tex;


	if (!IS__CURRENTLY__CHECKING__NON__EMPTY__STOCKS())
	{
		_latex_template_print_pdf_content.replace("YEROTHMARCHANDISESNOMTERMINEES",
				YerothUtils::EMPTY_STRING);
	}
	else
	{
		_latex_template_print_pdf_content.replace("YEROTHMARCHANDISESNOMTERMINEES",
				QObject::trUtf8("NON TERMINÉES"));
	}

	return _latex_template_print_pdf_content;
}


bool YerothMarchandisesWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_marchandises_tex;


	if (!IS__CURRENTLY__CHECKING__NON__EMPTY__STOCKS())
	{
		_latex_template_print_pdf_content.replace("YEROTHMARCHANDISESNOMTERMINEES",
				YerothUtils::EMPTY_STRING);
	}
	else
	{
		_latex_template_print_pdf_content.replace("YEROTHMARCHANDISESNOMTERMINEES",
				QObject::trUtf8("NON TERMINÉES"));
	}

	return YerothWindowsCommons::imprimer_pdf_document();
}

