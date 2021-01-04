/*
 * yeroth-erp-alertes-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-alertes-window.hpp"


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


YerothAlertesWindow::YerothAlertesWindow()
:YerothWindowsCommons("yeroth-erp-alertes"),
 YerothAbstractClassYerothSearchWindow(_allWindows->COURRIERS_ALERTES),
 _logger(new YerothLogger("YerothAlertesWindow")),
 _curAlertesSqlTableModel(&_allWindows->getSqlTableModel_courriers_alertes())
{
	setYerothSqlTableModel(_curAlertesSqlTableModel);

    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("fiche des alertes"));

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_alertes);

    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_alertes)

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_ALERT_BLUE_52_101_164,
                            	   COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(_allWindows->COURRIERS_ALERTES);

    _lineEditsToANDContentForSearch.insert(&lineEdit_alertes_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _lineEditsToANDContentForSearch.insert(&lineEdit_alertes_destinataire_2,
        		YerothDatabaseTableColumn::DESTINATAIRE);

    _comboBoxesToANDContentForSearch.insert(&comboBox_alertes_resolue_oui_OU_non,
    		YerothDatabaseTableColumn::ALERTE_RESOLUE);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(_allWindows->COURRIERS_ALERTES);

    reinitialiser_champs_db_visibles();

    _curAlertesSqlTableModel = &_allWindows->getSqlTableModel_courriers_alertes();

    comboBox_alertes_condition->setYerothEnabled(false);

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    tableView_alertes->setSqlTableName(&YerothERPWindows::COURRIERS_ALERTES);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    textEdit_alerte_message->setYerothEnabled(false);

    dateEdit_alertes_date_debut->setYerothEnabled(false);
    dateEdit_alertes_date_fin->setYerothEnabled(false);

    radioButton_alertes_periode_temps->setEnabled(false);
    radioButton_alertes_quantite->setEnabled(false);

    tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, false);

    pushButton_reinitialiser->disable(this);

    connect(tabWidget_alertes, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentTabChanged(int)));
    connect(tableView_alertes, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(afficher_au_detail()));

    /** Menu actions */
    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(afficher_alertes()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionAfficher_au_detail, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));


#ifdef YEROTH_SERVER
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif


    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(YerothDatabaseTableColumn::ALERTE_RESOLUE);

    setRightAligned_FOR_YEROTH_PDF_LATEX_PRINTING(YerothDatabaseTableColumn::CONDITION_ALERTE);

    setupShortcuts();
}

YerothAlertesWindow::~YerothAlertesWindow()
{
	MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _logger;
}


void YerothAlertesWindow::afficher_alertes()
{
	textChangedSearchLineEditsQCompleters();

	if (0 >= tableView_alertes->rowCount())
	{
		return ;
	}
	else
	{
		tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, true);
		tabWidget_alertes->setCurrentIndex(TableauDesAlertes);
	}
}


void YerothAlertesWindow::marquer_resolue()
{
    YerothSqlTableModel & courrierAlertesSqlTableModel = _allWindows->getSqlTableModel_courriers_alertes();

    bool courrierAlertesSqlTableModelFILTERED = false;

    if (_allWindows->getUser()->isMagasinier() || _allWindows->getUser()->isCaissier())
    {
        QString userFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESTINATAIRE,
        										_allWindows->getUser()->nom_utilisateur()));

        courrierAlertesSqlTableModel.yerothSetFilter_WITH_where_clause(userFilter);

        courrierAlertesSqlTableModelFILTERED = true;
    }

    QString userFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESTINATAIRE,
    										_allWindows->getUser()->nom_utilisateur()));

    QSqlRecord courriersAlertesRecord;

    _allWindows->_listerAlertesWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(courriersAlertesRecord);

    QString id_alerte = GET_SQL_RECORD_DATA(courriersAlertesRecord, YerothDatabaseTableColumn::ID_ALERTE);

    YerothSqlTableModel & alertesSqlTableModel = _allWindows->getSqlTableModel_alertes();

    QString alerteFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::ID, id_alerte));

    alertesSqlTableModel.yerothSetFilter_WITH_where_clause(alerteFilter);

    int alertesSqlTableModelRowCount = alertesSqlTableModel.easySelect();

    if (alertesSqlTableModelRowCount <= 0)
    {
    	if (courrierAlertesSqlTableModelFILTERED)
    	{
    		courrierAlertesSqlTableModel.resetFilter();
    	}

    	alertesSqlTableModel.resetFilter();

    	return ;
    }

    //Set in table 'alertes' that this alert has been resolved
    QSqlRecord alertesRecord = alertesSqlTableModel.record(0);

    if (1 == GET_SQL_RECORD_DATA(alertesRecord, YerothDatabaseTableColumn::ALERTE_RESOLUE).toInt())
    {
    	if (courrierAlertesSqlTableModelFILTERED)
    	{
    		courrierAlertesSqlTableModel.resetFilter();
    	}

    	alertesSqlTableModel.resetFilter();

    	YerothQMessageBox::information(this,
    			QObject::trUtf8("marquer résolue"),
				QObject::trUtf8("L'alerte '%1' est déjà marquée résolue !")
				    .arg(GET_SQL_RECORD_DATA(courriersAlertesRecord,
				    	 YerothDatabaseTableColumn::DESIGNATION_ALERTE)));
    	return;
    }

    alertesRecord.setValue(YerothDatabaseTableColumn::ALERTE_RESOLUE, 1);

    bool successAlertes = alertesSqlTableModel.updateRecord(0, alertesRecord);

    //Set in table 'courriers_alertes' that this alert has been resolved
    courriersAlertesRecord.setValue(YerothDatabaseTableColumn::ALERTE_RESOLUE, 1);

    bool successCourriersAlertes = _allWindows->_listerAlertesWindow->
    		SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(courriersAlertesRecord);

//    QDEBUG_STRINGS_OUTPUT_2("successCourriersAlertes", BOOL_TO_STRING(successCourriersAlertes));

//    QDEBUG_STRINGS_OUTPUT_2("successAlertes", BOOL_TO_STRING(successAlertes));

    if (successAlertes && successCourriersAlertes)
    {
    	afficher_alertes();

    	YerothQMessageBox::information(this,
    			QObject::trUtf8("marquer résolue - succès"),
				QObject::trUtf8("L'alerte '%1' a été marquée résolue !")
				    	.arg(GET_SQL_RECORD_DATA(courriersAlertesRecord,
				    		 YerothDatabaseTableColumn::DESIGNATION_ALERTE)));
    }
    else
    {
    	YerothQMessageBox::warning(this,
    			QObject::trUtf8("marquer résolue - échec"),
				QObject::trUtf8("L'alerte '%1' n'a pas pu être marquée résolue !\n "
				    			"Contacter l'administrateur de '%2' !")
				    	.arg(GET_SQL_RECORD_DATA(courriersAlertesRecord,
				    		 YerothDatabaseTableColumn::DESIGNATION_ALERTE),
				    		 YerothUtils::APPLICATION_NAME));
    }

	if (courrierAlertesSqlTableModelFILTERED)
	{
		courrierAlertesSqlTableModel.resetFilter();
	}

    alertesSqlTableModel.resetFilter();
}


void YerothAlertesWindow::supprimer()
{
    //qDebug() << "YerothAlertesWindow::supprimer()";
    if (!_allWindows->getUser()->isManager())
    {
        return;
    }

    QSqlRecord record;

    _allWindows->_listerAlertesWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString designation_alerte(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_ALERTE));

    if (!designation_alerte.isEmpty() && !designation_alerte.isNull())
    {
        QString msgSupprimer(QString(QObject::trUtf8("Poursuivre avec la suppression de l'alerte '%1' ?"))
        						.arg(designation_alerte));

        if (QMessageBox::Ok ==
                YerothQMessageBox::question(this,
                							QObject::tr("suppression d'une alerte"),
                							msgSupprimer,
											QMessageBox::Cancel,
											QMessageBox::Ok))
        {
            bool resRemoved = _allWindows->_listerAlertesWindow->
            		SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW();
            //qDebug() << "YerothStocksWindow::supprimer_du_stock() " << resRemoved;

            QString supprimerAlerteStr(QString("DELETE FROM %1 WHERE %2 = '%3'")
            								.arg(_allWindows->ALERTES,
            									 YerothDatabaseTableColumn::DESIGNATION_ALERTE,
            									 designation_alerte));

            YerothUtils::execQuery(supprimerAlerteStr);

            afficher_alertes();

            if (resRemoved)
            {
                msgSupprimer.clear();
                msgSupprimer.append(QString(QObject::trUtf8("L'alerte '%1' a été supprimée !"))
                						.arg(designation_alerte));

                YerothQMessageBox::information(this,
                							   QObject::tr("suppression d'une alerte"),
											   msgSupprimer);
            }
            else
            {
                msgSupprimer.clear();
                msgSupprimer.append(QString(QObject::trUtf8("L'alerte '%1' ne pouvait pas être supprimée !"))
                						.arg(designation_alerte));

                YerothQMessageBox::information(this,
                							   QObject::trUtf8("suppression d'une alerte"),
											   msgSupprimer);
            }
        }
        else
        {
        }
    }
}


void YerothAlertesWindow::setCurrentUser_NOM_UTILISATEUR(const QString &aUserNomUtilisateur)
{
	lineEdit_alertes_destinataire_2->setYerothEnabled(false);
	lineEdit_alertes_destinataire_2->setText(aUserNomUtilisateur);
}


void YerothAlertesWindow::disable_NOM_UTILISATEUR()
{
	lineEdit_alertes_destinataire_2->setYerothEnabled(false);
	lineEdit_alertes_destinataire_2->myClear();
}


void YerothAlertesWindow::enable_NOM_UTILISATEUR_ONLY_MANAGER()
{
	lineEdit_alertes_destinataire_2->setYerothEnabled(true);
	lineEdit_alertes_destinataire_2->myClear();
}


void YerothAlertesWindow::clear_all_fields()
{
    lineEdit_alertes_nom->clear();
    lineEdit_alertes_designation->clear();
    comboBox_alertes_condition->clear();
    lineEdit_alertes_quantite->clear();
    lineEdit_alertes_destinataire->clear();
    lineEdit_alertes_nom_destinataire->clear();
    textEdit_alerte_message->clear();
}


void YerothAlertesWindow::populateComboBoxes()
{
	comboBox_alertes_resolue_oui_OU_non->addItem(YerothUtils::EMPTY_STRING);

	comboBox_alertes_resolue_oui_OU_non->addItem(YerothUtils::STRING_OUI);

	comboBox_alertes_resolue_oui_OU_non->addItem(YerothUtils::STRING_NON);


	QStringList aQStringList;

	aQStringList.append(_varchar_dbtable_column_name_list.values());

	aQStringList.removeAll(YerothDatabaseTableColumn::ID);
	aQStringList.removeAll(YerothDatabaseTableColumn::ID_ALERTE);
	aQStringList.removeAll(YerothDatabaseTableColumn::DATE_DEBUT);
	aQStringList.removeAll(YerothDatabaseTableColumn::DATE_FIN);
	aQStringList.removeAll(YerothDatabaseTableColumn::MESSAGE_ALERTE);
	aQStringList.removeAll(YerothDatabaseTableColumn::ALERTE_RESOLUE);
	aQStringList.removeAll(YerothDatabaseTableColumn::DESTINATAIRE);
	aQStringList.removeAll(YerothDatabaseTableColumn::CONDITION_ALERTE);

//	qDebug() << "++ test: " << aQStringList;

	QString aDBColumnElementString;

	for (int k = 0; k < aQStringList.size(); ++k)
	{
		aDBColumnElementString = aQStringList.at(k);

		comboBox_element_string_db
			->addItem(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(aDBColumnElementString));
	}

	comboBox_element_string_db->setCurrentIndex(0);
}


void YerothAlertesWindow::setupLineEdits()
{
    lineEdit_alertes_terme_recherche->enableForSearch(
    		QObject::trUtf8("terme à rechercher (message d'alerte)"));

	lineEdit_nom_element_string_db->enableForSearch(QObject::trUtf8("valeur à rechercher"));

	lineEdit_alertes_destinataire_2->enableForSearch(QObject::trUtf8("%1 (nom d'utilisateur)")
															.arg(YerothDatabaseTableColumn::DESTINATAIRE));

	lineEdit_alertes_nombre_dalertes->setYerothEnabled(false);


    lineEdit_alertes_nom->setYerothEnabled(false);
    lineEdit_alertes_quantite->setYerothEnabled(false);
    lineEdit_alertes_resolue->setYerothEnabled(false);
    lineEdit_alertes_designation->setYerothEnabled(false);
    lineEdit_alertes_destinataire->setYerothEnabled(false);
    lineEdit_alertes_nom_destinataire->setYerothEnabled(false);

	MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_alertes_nombre_de_lignes_par_page, tableView_alertes);

	lineEdit_alertes_terme_recherche->setFocus();
}


void YerothAlertesWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothAlertesWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();

	if (0 != _curAlertesSqlTableModel)
	{
		lister_les_elements_du_tableau(*_curAlertesSqlTableModel);
	}
}


void YerothAlertesWindow::textChangedSearchLineEditsQCompleters()
{
	tabWidget_alertes->setCurrentIndex(TableauDesAlertes);

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_alertes_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
        	partSearchTerm = searchTermList.at(k);
//        	qDebug() << "++ searchTermList: " << partSearchTerm;

        	_searchFilter.append(QString("%1")
        							.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::MESSAGE_ALERTE, partSearchTerm)));

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

    		if (!correspondingDBFieldKeyValue.isEmpty() 	 &&
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

    {
    	QString curSearchDBStr;

    	YerothComboBox *aYerothComboBox = 0;

    	QMapIterator <YerothComboBox **, QString> it(_comboBoxesToANDContentForSearch);

    	while (it.hasNext())
    	{
    		it.next();

    		aYerothComboBox = *it.key();

    		correspondingDBFieldKeyValue = it.value();

    		if (0 != aYerothComboBox)
    		{
    			aTableColumnFieldContentForANDSearch = aYerothComboBox->currentText();

    			if (!correspondingDBFieldKeyValue.isEmpty() &&
    					!aTableColumnFieldContentForANDSearch.isEmpty()	)
    			{
    				if (!_searchFilter.isEmpty())
    				{
    					_searchFilter.append(" AND ");
    				}

    				if (aYerothComboBox == comboBox_alertes_resolue_oui_OU_non)
    				{
    					curSearchDBStr = QString::number(YerothUtils::_alerte_OUI_NON_ToUserViewString
    							.key(comboBox_alertes_resolue_oui_OU_non->currentText()));
    				}

    				_searchFilter.append(GENERATE_SQL_IS_STMT(correspondingDBFieldKeyValue,
    														  curSearchDBStr));
    			}
    		}
    	}
    }

    QString finalSearchFilter(_searchFilter);

//    QDEBUG_STRINGS_OUTPUT_2("finalSearchFilter", finalSearchFilter);

    _yerothSqlTableModel->yerothSetFilter_WITH_where_clause(finalSearchFilter);

    if (_yerothSqlTableModel->select())
    {
    	lister_les_elements_du_tableau(*_yerothSqlTableModel);
    }
    else
    {
    	disableImprimer();

        qDebug() << QString("++ YerothAlertesWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }
}


void YerothAlertesWindow::handleCurrentTabChanged(int index)
{
    switch (index)
    {
    case TableauDesAlertes:
    	handle_visibilite_autres_elements(true);
    	afficher_alertes();
        clear_all_fields();
        enableImprimer();
        break;

    case AfficherAlerteAuDetail:
    	handle_visibilite_autres_elements(false);
        if (afficher_au_detail())
        {
        	disableImprimer();
        }
        break;

    default:
        break;
    }
}


void YerothAlertesWindow::handleComboBoxClients_courriers_alertes_resolue_oui_OU_non(const QString &currentText)
{

}


void YerothAlertesWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBColumnNameStrList.clear();

	_visibleDBColumnNameStrList
		<< YerothDatabaseTableColumn::DESIGNATION_ALERTE
		<< YerothDatabaseTableColumn::DESIGNATION
		<< YerothDatabaseTableColumn::CONDITION_ALERTE
		<< YerothDatabaseTableColumn::QUANTITE
		<< YerothDatabaseTableColumn::DATE_DEBUT
		<< YerothDatabaseTableColumn::DATE_FIN
		<< YerothDatabaseTableColumn::ALERTE_RESOLUE;
}


void YerothAlertesWindow::contextMenuEvent(QContextMenuEvent * event)
{
    if (tableView_alertes->rowCount() > 0)
    {
        QMenu menu(this);
        menu.setPalette(toolBar_alertesWindow->palette());
        menu.addAction(actionAfficher_alerte_au_detail);
        menu.exec(event->globalPos());
    }
}


void YerothAlertesWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
    	if (aUser->isManager())
    	{
    		enable_NOM_UTILISATEUR_ONLY_MANAGER();
    	}
    	else
    	{
    		setCurrentUser_NOM_UTILISATEUR(aUser->nom_utilisateur());
    	}
    }
    else
    {
    	disable_NOM_UTILISATEUR();
    }

    setYerothSqlTableModel(_curAlertesSqlTableModel);

    _curStocksTableModel = stocksTableModel;

    setVisible(true);

    afficher_alertes();

    lineEdit_alertes_terme_recherche->setFocus();
}


void YerothAlertesWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

	pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->disable(this);
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->disable(this);
}


void YerothAlertesWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this, _curAlertesSqlTableModel)

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->enable(this, SLOT(supprimer()));
}


void YerothAlertesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);


#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

	pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->disable(this);
}


void YerothAlertesWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

	pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->disable(this);
}


void YerothAlertesWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

	pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->disable(this);
}


void YerothAlertesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


	pushButton_reinitialiser->disable(this);

    pushButton_afficher_detail->disable(this);
    pushButton_marquer_resolue->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
}


bool YerothAlertesWindow::afficher_au_detail()
{
    YerothSqlTableModel & courrierAlertesSqlTableModel = _allWindows->getSqlTableModel_courriers_alertes();

    if (_allWindows->getUser()->isMagasinier() || _allWindows->getUser()->isCaissier())
    {
        QString userFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESTINATAIRE, _allWindows->getUser()->nom_utilisateur()));
        //qDebug() << "++ userFilter: " << userFilter;
        courrierAlertesSqlTableModel.yerothSetFilter_WITH_where_clause(userFilter);
    }

    if (! courrierAlertesSqlTableModel.select())
    {
    	return false;
    }

    QSqlRecord record;

    _allWindows->_listerAlertesWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    lineEdit_alertes_nom->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_ALERTE));

    lineEdit_alertes_designation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

    comboBox_alertes_condition->addItem(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CONDITION_ALERTE));

    int quantite = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE).toInt();

    if (quantite > -1)
    {
    	lineEdit_alertes_quantite->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE));
    	radioButton_alertes_quantite->setChecked(true);
    	radioButton_alertes_periode_temps->setChecked(false);

    	groupBox_alertes_quantite->setVisible(true);

    	groupBox_periode_temps->setVisible(false);
    }
    else
    {
    	QString date_debut(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_DEBUT));
    	QString date_fin(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_FIN));

    	//qDebug() << "++ date_debut " << date_debut;
    	dateEdit_alertes_date_debut->setDate(GET_DATE_FROM_STRING(date_debut));
    	dateEdit_alertes_date_fin->setDate(GET_DATE_FROM_STRING(date_fin));
    	radioButton_alertes_quantite->setChecked(false);
    	radioButton_alertes_periode_temps->setChecked(true);

    	groupBox_alertes_quantite->setVisible(false);

    	groupBox_periode_temps->setVisible(true);
    }

    lineEdit_alertes_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESTINATAIRE));
    lineEdit_alertes_nom_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE));
    textEdit_alerte_message->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MESSAGE_ALERTE));

    int alerte_resolue = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ALERTE_RESOLUE).toInt();

    lineEdit_alertes_resolue->setText(INT_TO_STRING(alerte_resolue));

    tabWidget_alertes->setCurrentIndex(AfficherAlerteAuDetail);

    return true;
}


void YerothAlertesWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_nom_element_string_db->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_courriers_alertes());

    resetLineEditsQCompleters((QObject *)this);

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
    	if (aUser->isManager())
    	{
    		enable_NOM_UTILISATEUR_ONLY_MANAGER();
    	}
    	else
    	{
    		setCurrentUser_NOM_UTILISATEUR(aUser->nom_utilisateur());
    	}
    }
    else
    {
    	disable_NOM_UTILISATEUR();
    }

    afficher_alertes();

    lineEdit_alertes_terme_recherche->setFocus();
}


bool YerothAlertesWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_lister_alertes_tex;
	return YerothWindowsCommons::imprimer_pdf_document();
}


void YerothAlertesWindow::resetFilter(YerothSqlTableModel *alertesSqlTableModel)
{
	_curAlertesSqlTableModel = alertesSqlTableModel;

    if (0 != _curAlertesSqlTableModel)
    {
    	_curAlertesSqlTableModel->resetFilter();
    }

    lineEdit_alertes_terme_recherche->myClear();

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
    	if (aUser->isManager())
    	{
    		enable_NOM_UTILISATEUR_ONLY_MANAGER();
    	}
    	else
    	{
    		setCurrentUser_NOM_UTILISATEUR(aUser->nom_utilisateur());
    	}
    }
    else
    {
    	disable_NOM_UTILISATEUR();
    }
}


void YerothAlertesWindow::lister_les_elements_du_tableau(YerothSqlTableModel &alertesSqlTableModel)
{
	tableView_alertes->queryYerothTableViewCurrentPageContentRow(alertesSqlTableModel);

    tableView_show_or_hide_columns(*tableView_alertes);

    int rowCount = tableView_alertes->rowCount();

    lineEdit_alertes_nombre_dalertes->setText(GET_NUM_STRING(rowCount));

    if (rowCount <= 0)
    {
    	tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, false);
    }
    else
    {
    	tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, true);
    }
}


void YerothAlertesWindow::afficher_alerte_selectioner(const QString & alerteDesignation)
{
    _logger->log("afficher_alerte_selectioner(const QString &)");

    //qDebug() << QString("afficher_alerte_selectioner(%1)").arg(alerteDesignation);

    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION_ALERTE, alerteDesignation));

    //qDebug() << QString("filter: %1").arg(filter);

    _curAlertesSqlTableModel->yerothSetFilter_WITH_where_clause(filter);

    if (_curAlertesSqlTableModel->easySelect() > 0)
    {
        lister_les_elements_du_tableau(*_curAlertesSqlTableModel);
    }
}


void YerothAlertesWindow::handle_visibilite_autres_elements(bool aBoolValue)
{
	if (aBoolValue)
	{
		tabWidget_alertes->setFixedHeight(331);
	}
	else
	{
		tabWidget_alertes->setFixedHeight(392);
	}


	label_alertes_resolue_oui_OU_non->setVisible(aBoolValue);

	label_nombre_de_lignes_par_page->setVisible(aBoolValue);

	label_alertes_numero_page_courante->setVisible(aBoolValue);
	label_alertes_numero_page_derniere->setVisible(aBoolValue);
	label_alertes_separation_page_precedente_ou_suivante->setVisible(aBoolValue);

	label_nombre_dalertes->setVisible(aBoolValue);

	comboBox_element_string_db->setVisible(aBoolValue);
	comboBox_alertes_resolue_oui_OU_non->setVisible(aBoolValue);

	pushButton_page_premiere->setVisible(aBoolValue);
	pushButton_page_derniere->setVisible(aBoolValue);
	pushButton_page_precedente->setVisible(aBoolValue);
	pushButton_page_suivante->setVisible(aBoolValue);

	pushButton_reinitialiser->setVisible(aBoolValue);

	lineEdit_alertes_destinataire_2->setVisible(aBoolValue);

	lineEdit_nom_element_string_db->setVisible(aBoolValue);

	lineEdit_alertes_nombre_de_lignes_par_page->setVisible(aBoolValue);
	lineEdit_alertes_nombre_dalertes->setVisible(aBoolValue);
}

