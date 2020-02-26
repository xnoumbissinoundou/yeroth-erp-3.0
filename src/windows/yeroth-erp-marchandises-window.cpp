/*
 * yeroth-erp-marchandises-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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

#include "src/windows/yeroth-erp-search-form.hpp"



#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


const QString YerothMarchandisesWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("Marchandises")));


YerothMarchandisesWindow::YerothMarchandisesWindow()
:YerothWindowsCommons(YerothMarchandisesWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothInventaireDesStocksWindow")),
 _currentlyFiltered(false),
 _lastSelectedRow(0),
 _pushButton_filtrer_font(0),
 _action_RechercherFont(0),
 _pushButton_RechercherFont(0),
 _searchMarchandisesWidget(0),
 _searchMarchandisesTableModel(0),
 _curInventaireDesStocksTableModel(&_allWindows->getSqlTableModel_marchandises())
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48);

    _logger->log("YerothInventaireDesStocksWindow");

    setupSelectDBFields(_allWindows->MARCHANDISES);

    reinitialiser_champs_db_visibles();

    _searchMarchandisesWidget = new YerothSearchForm(_allWindows, *this, _curInventaireDesStocksTableModel);

    setupLineEdits();

    populateInventaireDesStocksComboBoxes();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    _action_RechercherFont = new QFont(actionRechercher->font());

    _pushButton_RechercherFont = new QFont(pushButton_rechercher->font());

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
    pushButton_rechercher->enable(this, SLOT(rechercher()));
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
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_document()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionRechercher, SIGNAL(triggered()), this, SLOT(rechercher()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));
    connect(actionReinitialiserElementsDeFiltrage, SIGNAL(triggered()), this, SLOT(reinitialiser_elements_filtrage()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#endif

    connect(actionSupprimer_ce_stock, SIGNAL(triggered()), this, SLOT(supprimer_ce_stock()));

    connect(tableView_marchandises, SIGNAL(signal_lister(YerothSqlTableModel &)), this,
            SLOT(set_rechercher_font()));

    setupShortcuts();
}

YerothMarchandisesWindow::~YerothMarchandisesWindow()
{
	delete _pushButton_filtrer_font;
    delete _action_RechercherFont;
    delete _pushButton_RechercherFont;
    delete _searchMarchandisesWidget;
    delete _logger;
}


void YerothMarchandisesWindow::setSearchFormSqlTableModel(YerothSqlTableModel *searchFormSqlTableModel)
{
	setCurrentlyFiltered(false);

	_searchMarchandisesTableModel = searchFormSqlTableModel;
}



void YerothMarchandisesWindow::updateLineEditDesignation()
{
    lineEdit_recherche_designation->enableForSearch(QObject::trUtf8("désignation"));

    lineEdit_recherche_designation->setupMyQCompleterALL(_allWindows->MARCHANDISES);

    connect(lineEdit_recherche_designation->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
            SLOT(afficher_stock_selectioner(const QString &)));
}


double YerothMarchandisesWindow::getValeurDinventaireEnStock(QString categorie,
							   	   	   	   	   	   	   	     QString designation)
{
	double valeurMarchande = 0.0;

	QString sqlSearchStocksTableQueryStr(QString("SELECT %1, (round(%2, 2) - round(%3, 2)) FROM %4 WHERE %5 = '%6' AND %7 = '%8'")
											.arg(YerothDatabaseTableColumn::QUANTITE_TOTAL,
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
			qteTotalEnStock = sqlSearchStockTableQuery.value(YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();
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


double YerothMarchandisesWindow::getQuantiteTotalEnStock(QString categorie,
							   	   	   	   	   	   	   	 QString designation)
{
	double qteTotalEnStock = 0.0;

	QString sqlSearchStockTableQueryStr(QString("SELECT %1 FROM %2 WHERE %3 = '%4' AND %5 = '%6'")
											.arg(YerothDatabaseTableColumn::QUANTITE_TOTAL,
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
			qteTotalEnStock += sqlSearchStockTableQuery.value(YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();
		}

		QString updateQteTotalQueryStr(QString("UPDATE %1 SET %2 = '%3' WHERE (%4 = '%5') AND (%6 = '%7')")
											.arg(_allWindows->MARCHANDISES,
												 YerothDatabaseTableColumn::QUANTITE_TOTAL,
												 QString::number(qteTotalEnStock),
												 YerothDatabaseTableColumn::CATEGORIE,
												 categorie,
												 YerothDatabaseTableColumn::DESIGNATION,
												 designation));

		YerothUtils::execQuery(updateQteTotalQueryStr);
	}

	return qteTotalEnStock;
}


double YerothMarchandisesWindow::getQuantiteTotalEnStock(const QModelIndex &aQModelIndex)
{
	double qteTotalEnStock = 0.0;

	_lastSelectedRow = aQModelIndex.row();

	if (_lastSelectedRow > -1)
	{
		QString designation(YerothUtils::get_text(aQModelIndex.sibling(_lastSelectedRow, 2).data()));

		QString categorie(YerothUtils::get_text(aQModelIndex.sibling(_lastSelectedRow, 3).data()));

		qteTotalEnStock = YerothMarchandisesWindow::getQuantiteTotalEnStock(categorie, designation);
	}

	return qteTotalEnStock;
}


void YerothMarchandisesWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();

	afficherMarchandises();
}


void YerothMarchandisesWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

    _visibleDBFieldColumnStrList
		<< YerothDatabaseTableColumn::DESIGNATION
		<< YerothDatabaseTableColumn::CATEGORIE
		<< YerothDatabaseTableColumn::QUANTITE_TOTAL
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
    actionRechercher->setShortcut(YerothUtils::RECHERCHER_QKEYSEQUENCE);
    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
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

				if (splittedData.size() > 0)
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

	_curInventaireDesStocksTableModel->yerothSetFilter(filterString);

	int resultRows = _curInventaireDesStocksTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);

		afficherMarchandises(*_curInventaireDesStocksTableModel);

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

	_curInventaireDesStocksTableModel->yerothSetFilter(filterString);

	int resultRows = _curInventaireDesStocksTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);

		afficherMarchandises(*_curInventaireDesStocksTableModel);

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "marchandises - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "marchandises - filtrer");
	}

	return false;
}


void YerothMarchandisesWindow::set_filtrer_font()
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


void YerothMarchandisesWindow::populateInventaireDesStocksComboBoxes()
{
    _logger->log("populateInventaireDesStocksComboBoxes");

	QStringList aQStringList;

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::VALEUR_DIVENTAIRE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::QUANTITE_TOTAL));

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
	lineEdit_marchandises_element_de_stock_resultat->setValidator(&YerothUtils::DoubleValidator);

	updateLineEditDesignation();
}


void YerothMarchandisesWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    _curStocksTableModel = stocksTableModel;

	if (0 == _searchMarchandisesTableModel)
    {
        _searchMarchandisesWidget->setSqlTableModel(_curInventaireDesStocksTableModel);
    }

    lineEdit_recherche_designation->setFocus();

	afficherMarchandises();

	setVisible(true);
}


void YerothMarchandisesWindow::rendreInvisible()
{
    lineEdit_recherche_designation->clear();
    YerothWindowsCommons::rendreInvisible();
}


void YerothMarchandisesWindow::setCurrentlyFiltered(bool currentlyFiltered)
{
	_currentlyFiltered = currentlyFiltered;

	set_filtrer_font();

	set_rechercher_font();
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
    _logger->log("afficher_stock_selectioner(const QString &)");

    //qDebug() << QString("afficher_stock_selectioner(%1)").arg(stockName);

    setLastListerSelectedRow(0);

    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _curInventaireDesStocksTableModel->yerothSetFilter(filter);

    if (_curInventaireDesStocksTableModel->easySelect() > 0)
    {
        this->afficherMarchandises();
        this->setSearchFormSqlTableModel(_curInventaireDesStocksTableModel);
        this->set_rechercher_font();
    }
}


void YerothMarchandisesWindow::supprimer_ce_stock()
{
    _logger->log("supprimer_ce_stock");

    unsigned rowToRemove = tableView_marchandises->lastSelectedRow();

    QSqlRecord record = _curInventaireDesStocksTableModel->record(rowToRemove);

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

        bool resRemoved = _curInventaireDesStocksTableModel->removeRow(rowToRemove);
        //qDebug() << "YerothInventaireDesStocksWindow::supprimer_ce_stock() " << resRemoved;
        if (resRemoved && _curInventaireDesStocksTableModel->select())
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

        updateLineEditDesignation();
    }
    else
    {
    }
}


void YerothMarchandisesWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_marchandises_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothMarchandisesWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    _searchMarchandisesWidget->reinitialiser();

    setSearchFormSqlTableModel(0);

    setCurrentlyFiltered(false);
}


void YerothMarchandisesWindow::afficherMarchandises(YerothSqlTableModel &aYerothSqlTableModel)
{
    _searchMarchandisesWidget->rendreInvisible();

    tableView_marchandises->lister_les_elements_du_tableau(aYerothSqlTableModel);

    tableView_show_or_hide_columns(*tableView_marchandises);
}


void YerothMarchandisesWindow::set_rechercher_font()
{
    //_logger->log("set_rechercher_font");
    if (0 != _searchMarchandisesTableModel)
    {
        _action_RechercherFont->setUnderline(true);
        _pushButton_RechercherFont->setUnderline(true);
    }
    else
    {
        _action_RechercherFont->setUnderline(false);
        _pushButton_RechercherFont->setUnderline(false);
    }

    actionRechercher->setFont(*_action_RechercherFont);

    pushButton_rechercher->setFont(*_pushButton_RechercherFont);
}


bool YerothMarchandisesWindow::export_csv_file()
{
	_logger->log("export_csv_file");

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


void YerothMarchandisesWindow::getInventoryStocksListingTexTableString(QString &texTable_in_out,
        															   QStandardItemModel &tableStandardItemModel,
																	   QList<int> &dbFieldNameOfTypeString,
																	   QList<int> &columnsToIgnore,
																	   int fromRowIndex,
																	   int toRowIndex,
																	   bool lastPage)
{
    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    .append("\\centering").append("\n")
    .append("\\begin{tabular}")
    .append("{|");

    texTable_in_out.append("c|");

    //Tex table header
    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        if (dbFieldNameOfTypeString.contains(k))
        {
        	texTable_in_out.append("l|");
        }
        else
        {
        	texTable_in_out.append("r|");
        }
    }

    texTable_in_out.append("} \\hline").append("\n");

    /** We add a column named 'id' for numbering the rows
     * in the Tex table. */
    unsigned int id = fromRowIndex + 1;

    texTable_in_out.append("\\textbf{n\\textsuperscript{o}} & ");

    QStandardItem *item;

    int tableColumnCount = 1 + tableStandardItemModel.columnCount();

    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);
        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{").append("}"));
            YerothUtils::handleTexTableItemText(tableColumnCount,
                                   	   	   	    texTable_in_out,
												k,
												itemText);
        }
    }
    /** Closing Tex table header */
    YerothUtils::cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append("\\\\ \\hline\n");


    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        texTable_in_out.append(QString::number(id));
        texTable_in_out.append(" &");
        ++id;

        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }

            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                QString itemText(item->text());
                YerothUtils::handleFactureTexTableItemText(tableColumnCount,
                                              	  	  	   texTable_in_out,
														   k,
														   itemText);
            }
            else
            {
                if (k < tableStandardItemModel.columnCount() - 1)
                {
                    texTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    texTable_in_out.append("\"\"").append("\\\\ \\hline\n");
                }
            }
        }

        texTable_in_out = texTable_in_out.trimmed();

        YerothUtils::cleanUpTexTableLastString(texTable_in_out);

        texTable_in_out.append("\\\\ \\hline\n");
    }

    //Removes the empty character "" from Latex output
    texTable_in_out.replace("\"\"", "");

    texTable_in_out.append("\\end{tabular}").append("\n")
    .append("\\end{table*}").append("\n");

    //qDebug() << "++ texTable_in_out in getStocksListingTexTableString: " << texTable_in_out;
}


void YerothMarchandisesWindow::getMarchandisesTexDocumentString(QString &texDocumentString_in_out,
        										   	   	   	    QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(YerothUtils::template_marchandises_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


bool YerothMarchandisesWindow::imprimer_document()
{
    _logger->log("imprimer_document");

    QString latexFileNamePrefix("yeroth-erp-marchandises");

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    QString pdfMerchandiseFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-marchandises");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-merchandises");
#endif

    pdfMerchandiseFileName = YerothUtils::prindDocumentFromTableView(this,
    														   	     *tableView_marchandises,
																	 tableColumnsToIgnore,
																	 &YerothMarchandisesWindow::getInventoryStocksListingTexTableString,
																	 &YerothMarchandisesWindow::getMarchandisesTexDocumentString,
																	 latexFileNamePrefix);

    if (pdfMerchandiseFileName.isEmpty())
    {
    	return false;
    }

    YerothERPProcess::startPdfViewerProcess(pdfMerchandiseFileName);

    return true;
}

