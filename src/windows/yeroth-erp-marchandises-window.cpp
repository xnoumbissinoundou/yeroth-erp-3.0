/*
 * yeroth-erp-marchandises-window.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
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
 YerothAbstractClassYerothSearchWindow(_allWindows->MARCHANDISES),
 _logger(new YerothLogger("YerothMarchandisesWindow")),
 _valeurTheoriqueDinventaire(0.0),
 _qteTotaleDarticlesEnStock(0.0),
 _currentlyFiltered(false),
 _lastSelectedRow(0),
 _pushButton_filtrer_font(0),
 _curMarchandisesTableModel(&_allWindows->getSqlTableModel_marchandises())
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("les marchandises"));

    setupUi(this);

    mySetupUi(this);

    _yerothTableView_FROM_WINDOWS_COMMONS = tableView_marchandises;

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48);

    setupSelectDBFields(_allWindows->MARCHANDISES);

    _lineEditsToANDContentForSearch.insert(&lineEdit_marchandises_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _lineEditsToANDContentForSearch.insert(&lineEdit_marchandises_reference,
    		YerothDatabaseTableColumn::REFERENCE);

    _lineEditsToANDContentForSearch.insert(&lineEdit_marchandises_designation,
    		YerothDatabaseTableColumn::DESIGNATION);

    _lineEditsToANDContentForSearch.insert(&lineEdit_marchandises_categorie,
    		YerothDatabaseTableColumn::CATEGORIE);

    _lineEditsToANDContentForSearch.insert(&lineEdit_marchandises_nom_entreprise_fournisseur,
    		YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR);

    reinitialiser_champs_db_visibles();

    setupLineEdits();

    localSetupLineEditsQCompleters();

    populateMarchandisesComboBoxes();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_marchandises->setTableName(&YerothERPWindows::MARCHANDISES);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_reinitialiser_filtre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionAfficher_les_stocks_termines, SIGNAL(triggered()), this, SLOT(filtrer_empty_product_stock()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
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

    connect(actionSupprimer_ce_stock, SIGNAL(triggered()), this, SLOT(supprimer_ce_stock()));

    setupShortcuts();
}

YerothMarchandisesWindow::~YerothMarchandisesWindow()
{
	delete _pushButton_filtrer_font;
    delete _logger;
}


double YerothMarchandisesWindow::getValeurTotaleDinventaireEnStock(QString categorie,
							   	   	   	   	   	   	   	     	   QString designation)
{
	double valeurMarchande = 0.0;

	QString sqlSearchStocksTableQueryStr(QString("SELECT %1, (round(%2, 2) - round(%3, 2)) FROM %4 WHERE %5 = '%6' AND %7 = '%8'")
											.arg(YerothDatabaseTableColumn::QUANTITE_TOTALE,
												 YerothDatabaseTableColumn::PRIX_VENTE,
												 YerothDatabaseTableColumn::MONTANT_TVA,
												 _allWindows->STOCKS,
	                                             YerothDatabaseTableColumn::CATEGORIE,
												 categorie,
												 YerothDatabaseTableColumn::DESIGNATION,
												 designation));

	//qDebug() << "++ str: " << sqlSearchStockTableQueryStr;
	QSqlQuery sqlSearchStockTableQuery;

	int querySize = YerothUtils::execQuery(sqlSearchStockTableQuery, sqlSearchStocksTableQueryStr);

	//querySize shall be equal to 0 if product is in 'marchandises' database table.
	if (querySize > 0)
	{
		double qteTotalEnStock = 0.0;
		double montant_dinventaire = 0.0;

		while (sqlSearchStockTableQuery.next())
		{
			qteTotalEnStock = sqlSearchStockTableQuery.value(YerothDatabaseTableColumn::QUANTITE_TOTALE).toDouble();
			montant_dinventaire = sqlSearchStockTableQuery.value(1).toDouble();
			valeurMarchande += (qteTotalEnStock * montant_dinventaire);
		}

		QString updateValeurMarchandeQueryStr(QString("UPDATE %1 SET %2 = '%3' WHERE (%4 = '%5') AND (%6 = '%7')")
				.arg(_allWindows->MARCHANDISES,
						YerothDatabaseTableColumn::VALEUR_DIVENTAIRE,
						QString::number(valeurMarchande),
						YerothDatabaseTableColumn::CATEGORIE,
						categorie,
						YerothDatabaseTableColumn::DESIGNATION,
						designation));

		YerothUtils::execQuery(updateValeurMarchandeQueryStr);
	}

	return valeurMarchande;
}


double YerothMarchandisesWindow::getQuantiteTotaleEnStock(QString categorie,
							   	   	   	   	   	   	   	 QString designation)
{
	double qteTotalEnStock = 0.0;

	QString sqlSearchStockTableQueryStr(QString("SELECT %1 FROM %2 WHERE %3 = '%4' AND %5 = '%6'")
											.arg(YerothDatabaseTableColumn::QUANTITE_TOTALE,
												 _allWindows->STOCKS,
	                                             YerothDatabaseTableColumn::CATEGORIE,
												 categorie,
												 YerothDatabaseTableColumn::DESIGNATION,
												 designation));

	//qDebug() << "++ str: " << sqlSearchStockTableQueryStr;
	QSqlQuery sqlSearchStockTableQuery;

	int querySize = YerothUtils::execQuery(sqlSearchStockTableQuery, sqlSearchStockTableQueryStr);

	if (querySize > 0)
	{
		while(sqlSearchStockTableQuery.next())
		{
			qteTotalEnStock += sqlSearchStockTableQuery.value(YerothDatabaseTableColumn::QUANTITE_TOTALE).toDouble();
		}

		QString updateQteTotalQueryStr(QString("UPDATE %1 SET %2 = '%3' WHERE (%4 = '%5') AND (%6 = '%7')")
											.arg(_allWindows->MARCHANDISES,
												 YerothDatabaseTableColumn::QUANTITE_TOTALE,
												 QString::number(qteTotalEnStock),
												 YerothDatabaseTableColumn::CATEGORIE,
												 categorie,
												 YerothDatabaseTableColumn::DESIGNATION,
												 designation));

		YerothUtils::execQuery(updateQteTotalQueryStr);
	}

	return qteTotalEnStock;
}


double YerothMarchandisesWindow::getQuantiteTotaleEnStock(const QModelIndex &aQModelIndex)
{
	double qteTotalEnStock = 0.0;

	_lastSelectedRow = aQModelIndex.row();

	if (_lastSelectedRow > -1)
	{
		QString designation(YerothUtils::get_text(aQModelIndex.sibling(_lastSelectedRow, 2).data()));

		QString categorie(YerothUtils::get_text(aQModelIndex.sibling(_lastSelectedRow, 3).data()));

		qteTotalEnStock = YerothMarchandisesWindow::getQuantiteTotaleEnStock(categorie, designation);
	}

	return qteTotalEnStock;
}


void YerothMarchandisesWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

    _visibleDBFieldColumnStrList
		<< YerothDatabaseTableColumn::DESIGNATION
		<< YerothDatabaseTableColumn::CATEGORIE
		<< YerothDatabaseTableColumn::QUANTITE_TOTALE
		<< YerothDatabaseTableColumn::REFERENCE
		<< YerothDatabaseTableColumn::VALEUR_DIVENTAIRE;
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
                menu.setPalette(toolBar_inventaireDesStocksWindow->palette());
                menu.addAction(actionSupprimer_ce_stock);
                menu.exec(event->globalPos());
            }
        }
    }
}

void YerothMarchandisesWindow::setupShortcuts()
{
	setupShortcutActionMessageDaide 	(*actionAfficher_les_stocks_termines);
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);

    actionAfficher_les_stocks_termines->setShortcut(YerothUtils::AFFICHER_LES_STOCKS_TERMINES_QKEYSEQUENCE);
    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothMarchandisesWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();
	afficherMarchandises();
}


void YerothMarchandisesWindow::textChangedSearchLineEditsQCompleters()
{
	lineEdit_marchandises_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

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

    _yerothSqlTableModel->yerothSetFilter(_searchFilter);

    if (_yerothSqlTableModel->select())
    {
    	setLastListerSelectedRow(0);
    	afficherMarchandises(*_yerothSqlTableModel);
    }
    else
    {
        qDebug() << QString("++ YerothMarchandisesWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }
}


void YerothMarchandisesWindow::handle_services_checkBox(int state)
{
	if (checkBox_services->isChecked())
	{
		_curMarchandisesTableModel->yerothSetFilter(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
    										  	  	  	   YerothUtils::MYSQL_TRUE_LITERAL));
    }
    else
    {
    	_curMarchandisesTableModel->yerothSetFilter(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
    										  	  	  	   YerothUtils::MYSQL_FALSE_LITERAL));
	}

	_curMarchandisesTableModel->easySelect();

	lineEdit_marchandises_designation->clear();

	if (-1 != state)
	{
		reinitialiser_elements_filtrage();
	}

	afficherMarchandises();

	localSetupLineEditsQCompleters();
}


bool YerothMarchandisesWindow::filtrer_empty_product_stock()
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

	QStringList filterStrings;
	QStringList splittedData;
	QString data;
	QString categorieStr;
	QString designationStr;
	QString filterString;

	for (int k = 0; k < stdItemModel->rowCount(); ++k)
	{
		anItem = stdItemModel->item(k, 0);

		if (0 != anItem)
		{
			if (YerothUtils::YEROTH_RED_COLOR == anItem->foreground().color())
			{
				data = anItem->accessibleText();

				splittedData = data.split("|");

				if (splittedData.size() > 1)
				{
					categorieStr = splittedData.at(0);
					designationStr = splittedData.at(1);

					filterStrings
						.append(QString("(categorie = '%1' AND designation = '%2')")
									.arg(categorieStr, designationStr));

					if (k + 1 < stdItemModel->rowCount())
					{
						filterStrings.append(" OR ");
					}
				}
			}
		}
	}

	if (0 >= filterStrings.size())
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "marchandises - filtrer");

		return false;
	}

	if (YerothUtils::isEqualCaseInsensitive(filterStrings.last(), " OR "))
	{
		filterStrings.removeLast();
	}

	filterString = filterStrings.join(" ");

	//qDebug() << QString("++ filterString: %1")
	//				.arg(filterString);

	if (filterString.isEmpty())
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "marchandises - filtrer");

		return false;
	}

	_curMarchandisesTableModel->yerothSetFilter(filterString);

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


bool YerothMarchandisesWindow::filtrer()
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

	//qDebug() << QString("filterString: %1")
	//				.arg(filterString);

	_curMarchandisesTableModel->yerothSetFilter(filterString);

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

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::VALEUR_DIVENTAIRE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::QUANTITE_TOTALE));

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
	lineEdit_marchandises_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (description de l'article (ou service))"));

    lineEdit_marchandises_reference->enableForSearch(QObject::trUtf8("référence"));
    lineEdit_marchandises_categorie->enableForSearch(QObject::trUtf8("catégorie"));
    lineEdit_marchandises_designation->enableForSearch(QObject::trUtf8("désignation"));
    lineEdit_marchandises_nom_entreprise_fournisseur->enableForSearch(QObject::tr("nom entreprise fournisseur"));

    lineEdit_nombre_de_marchandises->setYerothEnabled(false);
    lineEdit_nombre_darticles->setYerothEnabled(false);
	lineEdit_valeur_totale_dinventaire->setYerothEnabled(false);

	lineEdit_marchandises_element_de_stock_resultat->setValidator(&YerothUtils::DoubleValidator);
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

	_curMarchandisesTableModel->yerothSetFilter(currentFilter);

	afficherMarchandises();

	localSetupLineEditsQCompleters();
}


void YerothMarchandisesWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_reinitialiser_filtre->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_sortir->disable(this);
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_rapports->enable(this, SLOT(tableaux_de_bords()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_rapports->disable(this);
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}

void YerothMarchandisesWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_reinitialiser_filtre->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_sortir->disable(this);
}

void YerothMarchandisesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

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
    setLastListerSelectedRow(0);

    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _curMarchandisesTableModel->yerothSetFilter(filter);

    if (_curMarchandisesTableModel->easySelect() > 0)
    {
        afficherMarchandises();
    }
}


void YerothMarchandisesWindow::supprimer_ce_stock()
{
    _logger->log("supprimer_ce_stock");

    unsigned rowToRemove = tableView_marchandises->lastSelectedRow();

    QSqlRecord record = _curMarchandisesTableModel->record(rowToRemove);

    QString msgSupprimer(QString(QObject::trUtf8("Poursuivre avec la suppression de la marchandise '%1' ?"))
    						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                       QObject::tr("suppression d'une marchandise"),
                                       msgSupprimer,
									   QMessageBox::Cancel,
									   QMessageBox::Ok))
    {
        _logger->debug("supprimer_ce_stock", QString("rowToRemove: %1").arg(rowToRemove));

        bool resRemoved = _curMarchandisesTableModel->removeRow(rowToRemove);
        //qDebug() << "YerothInventaireDesStocksWindow::supprimer_ce_stock() " << resRemoved;
        if (resRemoved && _curMarchandisesTableModel->select())
        {
            msgSupprimer.clear();

            msgSupprimer.append(QString(QObject::trUtf8("Le stock '%1' a été supprimé."))
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)));

            YerothQMessageBox::information(this,
                                          QObject::trUtf8("succès"),
                                          msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();

            msgSupprimer.append(QString(QObject::trUtf8("L'article \"%1\" ne pouvait pas être supprimé !"))
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)));

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


void YerothMarchandisesWindow::reinitialiser_elements_filtrage()
{
    lineEdit_marchandises_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothMarchandisesWindow::reinitialiser_recherche()
{
    lineEdit_marchandises_designation->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *)this);

    handle_services_checkBox(-1);
}


void YerothMarchandisesWindow::afficherMarchandises(YerothSqlTableModel &aYerothSqlTableModel)
{
    tableView_marchandises->lister_les_elements_du_tableau(aYerothSqlTableModel);

    tableView_show_or_hide_columns(*tableView_marchandises);

    int rowCount = tableView_marchandises->rowCount();

    lineEdit_nombre_de_marchandises->setText(GET_NUM_STRING(rowCount));

    lineEdit_nombre_darticles->setText(GET_DOUBLE_STRING(_qteTotaleDarticlesEnStock));

    lineEdit_valeur_totale_dinventaire->setText(GET_CURRENCY_STRING_NUM(_valeurTheoriqueDinventaire));
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


bool YerothMarchandisesWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_marchandises_tex;
	return YerothWindowsCommons::imprimer_pdf_document();
}

