/*
 * yeroth-erp-fournisseurs-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-fournisseurs-window.hpp"

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


YerothERPFournisseursWindow::YerothERPFournisseursWindow()
:YerothWindowsCommons("yeroth-erp-fournisseurs"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::FOURNISSEURS),
 _logger(new YerothLogger("YerothERPFournisseursWindow")),
 _pushButton_filtrer_font(0),
 _curFournisseursTableModel(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("fournisseurs"));

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_fournisseurs);


    _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
		<< pushButton_afficher
		<< pushButton_modifier
		<< pushButton_supprimer;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionExporter_au_format_csv
		<< action_parametrer_les_impressions
		<< actionAfficherPDF
		<< actionModifierFournisseur
		<< actionSupprimerFournisseur
		<< actionAfficherDetailsFournisseur;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_fournisseurs)

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                   .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                                		COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(YerothDatabase::FOURNISSEURS);


    _lineEditsToANDContentForSearch.insert(&lineEdit_fournisseurs_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerothDatabase::FOURNISSEURS);

    reinitialiser_colones_db_visibles();

    _curFournisseursTableModel = &_allWindows->getSqlTableModel_fournisseurs();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    populateFournisseursComboBoxes();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_fournisseurs->setSqlTableName(&YerothDatabase::FOURNISSEURS);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_fournisseur->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);


    //Menu actions
    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionCreerFournisseur, SIGNAL(triggered()), this, SLOT(creerFournisseur()));
    connect(actionModifierFournisseur, SIGNAL(triggered()), this, SLOT(modifierFournisseur()));
    connect(actionSupprimerFournisseur, SIGNAL(triggered()), this, SLOT(supprimerFournisseur()));
    connect(actionAfficherDetailsFournisseur, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this, SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(print_PDF_PREVIOUSLY_SETUP()));
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

    connect(tableView_fournisseurs, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}

YerothERPFournisseursWindow::~YerothERPFournisseursWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _pushButton_filtrer_font;
    delete _logger;
}


void YerothERPFournisseursWindow::reinitialiser_colones_db_visibles()
{
	_visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
			<< YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR
			<< YerothDatabaseTableColumn::NOM_ENTREPRISE
			<< YerothDatabaseTableColumn::VILLE
			<< YerothDatabaseTableColumn::EMAIL
			<< YerothDatabaseTableColumn::NUMERO_TELEPHONE_1;
}


void YerothERPFournisseursWindow::contextMenuEvent(QContextMenuEvent * event)
{
	QMenu menu(this);
	menu.setPalette(toolBar_fournisseursWindow->palette());
	menu.addAction(actionAfficherDetailsFournisseur);
	menu.addAction(actionModifierFournisseur);
	menu.addAction(actionSupprimerFournisseur);
	menu.exec(event->globalPos());
}


void YerothERPFournisseursWindow::hideEvent(QHideEvent * hideEvent)
{
	_allWindows->_transactionsDunFournisseurWindow->close();
}


void YerothERPFournisseursWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 		(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe			(*actionQui_suis_je);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothERPFournisseursWindow::set_filtrer_font()
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


void YerothERPFournisseursWindow::slot_reinitialiser_colones_db_visibles()
{
	reinitialiser_colones_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();
	afficherFournisseurs();
}


void YerothERPFournisseursWindow::textChangedSearchLineEditsQCompleters()
{
	lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_fournisseurs_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
        	partSearchTerm = searchTermList.at(k);
        	//qDebug() << "++ searchTermList: " << partSearchTerm;

        	_searchFilter.append(QString("(%1 OR %2 OR %3)")
        							.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::EMAIL, partSearchTerm),
        								 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::NOM_REPRESENTANT, partSearchTerm),
										 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR, partSearchTerm)));

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
    	lister_les_elements_du_tableau(*_yerothSqlTableModel);
    }
    else
    {
        qDebug() << QString("++ YerothERPFournisseursWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerothERPFournisseursWindow::modifierFournisseur()
{
    if (_curFournisseursTableModel->rowCount() > 0)
    {
    	rendreInvisible();

        _allWindows->_modifierFournisseurWindow->rendreVisible(_curFournisseursTableModel,
															   _curStocksTableModel);
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::tr("modifier un fournisseur"),
                                   QObject::trUtf8("Sélectionnez un fournisseur à modifier !"));
    }
}


void YerothERPFournisseursWindow::supprimer_PLUSIEURS_Fournisseurs(YerothSqlTableModel &aFournisseursTableModel)
{
	QString YEROTH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING;

	QMapIterator<QString, QString> j(tableView_fournisseurs->lastSelected_Rows__IDs());

	while (j.hasNext())
	{
		j.next();

		YEROTH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING
			.append(QString("DELETE FROM %1 WHERE %2 = '%3';")
				.arg(YerothDatabase::FOURNISSEURS,
					 YerothDatabaseTableColumn::ID,
					 j.value()));
	}

    QString msgConfirmation(QObject::trUtf8("Supprimer les fournisseurs sélectionés ?"));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
            							QObject::tr("suppression de plusieurs fournisseurs"),
            							msgConfirmation,
										QMessageBox::Cancel,
										QMessageBox::Ok))
    {
    	bool success = YerothUtils::execQuery(YEROTH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING);

    	QString msg(QObject::trUtf8("Les fournisseurs sélectionés"));

    	if (success && aFournisseursTableModel.select())
    	{
    		setupLineEditsQCompleters((QObject *)this);

    		msg.append(QObject::trUtf8(" ont été supprimés de la base de données !"));

    		YerothQMessageBox::information(this,
    				QObject::trUtf8("suppression de fournisseurs - succès"),
    				msg,
    				QMessageBox::Ok);
    	}
    	else
    	{
    		msg.append(QObject::trUtf8(" n'ont pas pu être supprimés de la base de données !"));

    		YerothQMessageBox::information(this,
    				QObject::trUtf8("suppression de fournisseurs - échec"),
    				msg,
    				QMessageBox::Ok);
    	}
    }
}


void YerothERPFournisseursWindow::supprimerFournisseur()
{
    YerothSqlTableModel *fournisseursTableModel = 0;

    if (_curFournisseursTableModel
            && YerothUtils::isEqualCaseInsensitive(YerothDatabase::FOURNISSEURS,
            									   _curFournisseursTableModel->sqlTableName()))
    {
        fournisseursTableModel = _curFournisseursTableModel;
    }
    else
    {
        return ;
    }

    if (tableView_fournisseurs->lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
    	supprimer_PLUSIEURS_Fournisseurs(*fournisseursTableModel);

   		tableView_fournisseurs->clearSelection();

    	afficherFournisseurs();

    	return ;
    }

    QSqlRecord record;

    _allWindows->_fournisseursWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    if (record.isEmpty() || record.isNull(YerothDatabaseTableColumn::NOM_ENTREPRISE))
    {
        return;
    }

    QString nom_entreprise(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

    QString msgConfirmation(QObject::trUtf8("Supprimer le fournisseur '%1' ?")
    							.arg(nom_entreprise));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
            							QObject::tr("suppression"),
            							msgConfirmation,
										QMessageBox::Cancel,
										QMessageBox::Ok))
    {
    	bool success = _allWindows->_fournisseursWindow->
				SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW();

        QString msg(QObject::tr("Le fournisseur '%1")
        				.arg(nom_entreprise));

        if (success && fournisseursTableModel->select())
        {
        	setupLineEditsQCompleters((QObject *)this);

        	tableView_fournisseurs->clearSelection();

        	afficherFournisseurs();

            msg.append(QObject::trUtf8("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("suppression - succès"),
            							   msg,
										   QMessageBox::Ok);
        }
        else
        {
            msg.append(QObject::trUtf8(" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("suppression - échec"),
										   msg,
										   QMessageBox::Ok);
        }
    }
}


void YerothERPFournisseursWindow::afficher_au_detail()
{
    if (_curFournisseursTableModel->rowCount() > 0)
    {
    	//qDebug() << "++ test" << modelIndex.row();
        _allWindows->_fournisseurDetailsWindow->rendreVisible(_curFournisseursTableModel,
															  _curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails"),
                                  QObject::trUtf8("Sélectionnez un fournisseur à afficher les détails !"));
    }
}


bool YerothERPFournisseursWindow::filtrer()
{
	QString stockTableColumnValue(lineEdit_resultat_filtre->text());

	if (stockTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
									  QObject::tr("filtrer"),
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

	_curFournisseursTableModel->yerothSetFilter_WITH_where_clause(filterString);

	int resultRows = _curFournisseursTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);

		lister_les_elements_du_tableau(*_curFournisseursTableModel);

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "filtrer");
	}

	return false;
}


void YerothERPFournisseursWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothERPFournisseursWindow::reinitialiser_recherche()
{
	lineEdit_nom_element_string_db->clear();

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *)this);

    afficherFournisseurs();
}


void YerothERPFournisseursWindow::populateFournisseursComboBoxes()
{
    _logger->log("populateFournisseursComboBoxes");

	QStringList aQStringList;

	aQStringList.append(_varchar_dbtable_column_name_list.values());

	aQStringList.removeAll(YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR);

	comboBox_element_string_db->addItems(aQStringList,
			YerothDatabaseTableColumn::_tableColumnToUserViewString);

	comboBox_element_string_db->setCurrentIndex(0);

	aQStringList.clear();

	aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerothDatabaseTableColumn::COMPTE_FOURNISSEUR));

    comboBox_element->addItems(aQStringList);

	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_condition->addItems(aQStringList);
}


void YerothERPFournisseursWindow::setupLineEdits()
{
	_QLINEEDIT_nombre_de_lignes_par_page = lineEdit_fournisseurs_nombre_de_lignes_par_page;

	lineEdit_fournisseurs_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (émail, réprésentant, description de l'entreprise fournisseure)"));

	lineEdit_nom_element_string_db->enableForSearch(QObject::trUtf8("valeur à rechercher"));

	lineEdit_nombre_de_fournisseurs->setYerothEnabled(false);

	MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_fournisseurs_nombre_de_lignes_par_page, tableView_fournisseurs);

	lineEdit_resultat_filtre->setValidator(&YerothUtils::DoubleValidator);
}


void YerothERPFournisseursWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setYerothSqlTableModel(_curFournisseursTableModel);

    _curStocksTableModel = stocksTableModel;

	setVisible(true);

	lister_les_elements_du_tableau(*_curFournisseursTableModel);

	APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

	lineEdit_fournisseurs_terme_recherche->setFocus();
}


void YerothERPFournisseursWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curFournisseursTableModel)

    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_fournisseur->enable(this, SLOT(creerFournisseur()));
    pushButton_modifier->enable(this, SLOT(modifierFournisseur()));
    pushButton_supprimer->enable(this, SLOT(supprimerFournisseur()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothERPFournisseursWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_fournisseur->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPFournisseursWindow::afficher_nom_entreprise_selectioner(const QString & nomEntreprise)
{
    _logger->log("afficher_nom_entreprise_selectioner(const QString &)");

    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE, nomEntreprise));

    //qDebug() << QString("filter: %1").arg(filter);

    _curFournisseursTableModel->yerothSetFilter_WITH_where_clause(filter);

    if (_curFournisseursTableModel->easySelect() > 0)
    {
        lister_les_elements_du_tableau(*_curFournisseursTableModel);
    }
}


void YerothERPFournisseursWindow::lister_les_elements_du_tableau(YerothSqlTableModel &fournisseurSqlTableModel)
{
	tableView_fournisseurs->queryYerothTableViewCurrentPageContentRow(fournisseurSqlTableModel);

    tableView_show_or_hide_columns(*tableView_fournisseurs);

    int rowCount = tableView_fournisseurs->rowCount();

    lineEdit_nombre_de_fournisseurs->setText(GET_NUM_STRING(rowCount));
}


bool YerothERPFournisseursWindow::export_csv_file()
{
	_logger->log("export_csv_file");

	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_fournisseurs,
										   tableColumnsToIgnore,
										   "yeroth-erp-fiche-fournisseurs-format-csv",
										   "fiche des fournisseurs");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_fournisseurs,
										   tableColumnsToIgnore,
										   "yeroth-erp-supplier-account-listing-csv-format",
										   "supplier account listing file");
#endif

	return success;
}


QString YerothERPFournisseursWindow::get_latex_template_print_pdf_content()
{
	_latex_template_print_pdf_content = YerothUtils::template_fournisseurs_tex;

	return _latex_template_print_pdf_content;
}

