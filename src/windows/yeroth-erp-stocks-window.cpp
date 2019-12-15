/*
 * yeroth-erp-stocks-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-stocks-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-style.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/windows/yeroth-erp-search-form.hpp"

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


const QString YerothStocksWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
arg(YEROTH_ERP_WINDOW_TITLE,
QObject::trUtf8("fiche des stocks")));

YerothStocksWindow::YerothStocksWindow()
    :YerothWindowsCommons(YerothStocksWindow::_WINDOW_TITLE),
     _logger(new YerothLogger("YerothStocksWindow")),
	 _currentlyFiltered(false),
	 _actionRechercheArticleCodebar(0),
     _aProcess(0),
	 _pushButton_stocks_filtrer_font(0),
	 _pushButton_RechercherFont(0),
     _action_RechercherFont(0),
     _searchStocksWidget(0),
     _searchStocksTableModel(0)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0);

    _logger->log("YerothStocksWindow");

    _searchStocksWidget = new YerothSearchForm(_allWindows, *this, &_allWindows->getSqlTableModel_stocks());

    populateComboBoxes();

    setupLineEdits();

    _pushButton_stocks_filtrer_font = new QFont(pushButton_stocks_filtrer->font());

    _action_RechercherFont = new QFont(actionRechercher->font());
    _pushButton_RechercherFont = new QFont(pushButton_rechercher->font());

    tableView_stocks->setTableName(&YerothERPWindows::MARCHANDISES);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInventaireDesStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_stocks_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_inventaire_des_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rechercher->enable(this, SLOT(rechercher()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_connecter_localisation->disable(this);
    pushButton_deconnecter_localisation->disable(this);

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionInventaireDesStocks, SIGNAL(triggered()), this, SLOT(afficherMarchandises()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

    connect(actionAfficher_lhistorique_de_ce_stock, SIGNAL(triggered()),
    		this, SLOT(private_slot_afficher_historique_du_stock()));

    connect(actionModifier_ce_stock, SIGNAL(triggered()),
    		this, SLOT(modifier_les_articles()));

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

    connect(actionAfficher_stock_au_detail, SIGNAL(triggered()),
    		this, SLOT(afficher_au_detail()));

    connect(actionSupprimer_ce_stock, SIGNAL(triggered()),
    		this, SLOT(supprimer_ce_stock()));

    connect(comboBox_strategie_de_stocks, SIGNAL(activated(int)),
    		this, SLOT(gererChoixStrategieGestionDesStocks()));

    connect(tableView_stocks, SIGNAL(signal_lister(YerothSqlTableModel &)),
    		this, SLOT(set_rechercher_font()));

    connect(tableView_stocks, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    connect(actionAfficher_les_stocks_perimes, SIGNAL(triggered()),
    		this, SLOT(filtrer_stocks_perimes_seulement()));

    setupShortcuts();
}

YerothStocksWindow::~YerothStocksWindow()
{
	delete _actionRechercheArticleCodebar;
    delete _action_RechercherFont;
    delete _pushButton_RechercherFont;
    delete _searchStocksWidget;
    delete _logger;
}


void YerothStocksWindow::setCurrentlyFiltered(bool currentlyFiltered)
{
	_currentlyFiltered = currentlyFiltered;

	set_filtrer_font();
}


void YerothStocksWindow::updateLineEditRechercherCodeBar()
{
    lineEdit_recherche_reference->enableForSearch(QObject::trUtf8("référence [ focus avec F11 ]"));

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL,
                                           YerothERPConfig::salesStrategy))
    {
        lineEdit_recherche_reference->setupMyQCompleterCodebarALL(_allWindows->STOCKS);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO,
             YerothERPConfig::salesStrategy))
    {
        lineEdit_recherche_reference->setupMyQCompleterCodebarDEF_DEO(_allWindows->STOCKS);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
             YerothERPConfig::salesStrategy))
    {
        lineEdit_recherche_reference->setupMyQCompleterCodebarFIFO(_allWindows->STOCKS);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
             YerothERPConfig::salesStrategy))
    {
        lineEdit_recherche_reference->setupMyQCompleterCodebarLIFO(_allWindows->STOCKS);
    }

    connect(lineEdit_recherche_reference->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
            SLOT(afficher_stock_selectioner_bar_code(const QString &)));
}


void YerothStocksWindow::private_slot_afficher_historique_du_stock()
{
	//qDebug() << QString("YerothStocksWindow | private_slot_afficher_historique_du_stock");

	int lastSelectedRow = getLastListerSelectedRow();

	//qDebug() << QString("lastSelectedRow: %1")
	//				.arg(QString::number(lastSelectedRow));

	if (0 != _curStocksTableModel &&_curStocksTableModel->rowCount() > 0 && lastSelectedRow > -1)
	{
	    QSqlRecord record = _curStocksTableModel->record(lastSelectedRow);

	    QString historiqueStockSelectionne(
	    			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::HISTORIQUE_STOCK));

	    int stockId = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID).toInt();

	    QString stockReference(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));

	    QString stockDesignation(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

	    YerothHistoriqueStock::listHistoriqueDuStock(historiqueStockSelectionne,
	    											 stockReference,
													 QString::number(stockId),
													 stockDesignation);
	}
	else
	{
	    YerothQMessageBox::information(this,
	    		QObject::trUtf8("stocks - historique du stock"),
				QObject::trUtf8("Il n'y a pas de stocks listés en ce moment !"));
	}

}


bool YerothStocksWindow::filtrer_stocks_perimes_seulement()
{
	QString filterString;

	filterString.append(QString("%1 < '%2'")
			.arg(YerothDatabaseTableColumn::DATE_PEREMPTION,
				 DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE)));

	//qDebug() << QString("++ filtrer_stocks_perimes_seulement | filterString: %1")
    //						.arg(filterString);

	_curStocksTableModel->yerothSetFilter(filterString);

	int resultRows = _curStocksTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);
	}

	afficherStocks();

	if (resultRows > 0)
	{
		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "stocks - filtrer");
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "stocks - filtrer");
	}

	set_filtrer_font();

	return true;
}


bool YerothStocksWindow::filtrer_stocks()
{
	QString stockTableColumnValue(lineEdit_stocks_element_de_stock_resultat->text());

	if (stockTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
				QObject::trUtf8("filtrer"),
				msg);
		return false;
	}

	reinitialiser_recherche();

	QString stockTableColumnProperty(comboBox_stocks_element_de_stock->currentText());

	QString mathOperator(comboBox_stocks_element_de_stock_condition->currentText());

	QString REAL_DB_ID_NAME_stockTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(stockTableColumnProperty));

	QString filterString;

	filterString.append(QString("%1 %2 %3")
			.arg(REAL_DB_ID_NAME_stockTableColumnProperty,
					mathOperator,
					stockTableColumnValue));

	_curStocksTableModel->yerothSetFilter(filterString);

	int resultRows = _curStocksTableModel->easySelect();

	if (resultRows > 0)
	{
		setCurrentlyFiltered(true);

		afficherStocks();

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "stocks - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "stocks - filtrer");

		return false;
	}

	set_filtrer_font();

	return false;
}


void YerothStocksWindow::activerComboBoxStrategieDeGestionDesStocks()
{
    comboBox_strategie_de_stocks->setVisible(true);
    comboBox_strategie_de_stocks->setEnabled(true);
    lineEdit_recherche_reference->setFixedWidth(663);
}


void YerothStocksWindow::desactiverComboBoxStrategieDeGestionDesStocks()
{
    comboBox_strategie_de_stocks->setVisible(false);
    comboBox_strategie_de_stocks->setEnabled(false);
    lineEdit_recherche_reference->setFixedWidth(778);
}


void YerothStocksWindow::setComboBoxStrategieDeStocks()
{
    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL))
    {
        comboBox_strategie_de_stocks->setCurrentIndex(comboBoxStrategyIndex::STRATEGIE_VENTE_SORTIE_ALL);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO))
    {
        comboBox_strategie_de_stocks->setCurrentIndex(comboBoxStrategyIndex::STRATEGIE_VENTE_SORTIE_DPF_DPO);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO))
    {
        comboBox_strategie_de_stocks->setCurrentIndex(comboBoxStrategyIndex::STRATEGIE_VENTE_SORTIE_FIFO);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO))
    {
        comboBox_strategie_de_stocks->setCurrentIndex(comboBoxStrategyIndex::STRATEGIE_VENTE_SORTIE_LIFO);
    }
}

void YerothStocksWindow::populateComboBoxes()
{
	_logger->log("populateComboBoxes");

	QStringList aQStringList;

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::MONTANT_TVA));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::PRIX_VENTE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::PRIX_UNITAIRE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::QUANTITE_TOTAL));

	comboBox_stocks_element_de_stock->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_stocks_element_de_stock_condition->addItems(aQStringList);

    comboBox_strategie_de_stocks->clear();
    comboBox_strategie_de_stocks->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);
    //index 0 of enum enum_comboBoxStrategyIndex
    comboBox_strategie_de_stocks->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO);
    comboBox_strategie_de_stocks->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);
    comboBox_strategie_de_stocks->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);
    desactiverComboBoxStrategieDeGestionDesStocks();
}

void YerothStocksWindow::setupLineEdits()
{
    updateLineEditRechercherCodeBar();

	lineEdit_stocks_element_de_stock_resultat->setValidator(&YerothUtils::DoubleValidator);

    lineEdit_localisation->enableForSearch(QObject::trUtf8("localisation"));
    YerothPOSUser *currentUser = _allWindows->getUser();
    if (0 != currentUser &&
        (currentUser->isManager() ||
         currentUser->isGestionaireDesStocks() ||
		 currentUser->isMagasinier() ||
		 currentUser->isVendeur()))
    {

#ifdef YEROTH_CLIENT
lineEdit_localisation->setVisible(false);
pushButton_connecter_localisation->setVisible(false);
pushButton_deconnecter_localisation->setVisible(false);
#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION
lineEdit_localisation->setVisible(true);
pushButton_connecter_localisation->setVisible(true);
pushButton_deconnecter_localisation->setVisible(true);
lineEdit_localisation->setupMyStaticQCompleter(_allWindows->LOCALISATIONS, "nom_localisation", false);
lineEdit_localisation->setText(YerothERPConfig::_connectedSite);
#endif

    }
    else
    {
        lineEdit_localisation->setVisible(false);
        pushButton_connecter_localisation->setVisible(false);
        pushButton_deconnecter_localisation->setVisible(false);
    }
}


void YerothStocksWindow::setupShortcuts()
{
	_actionRechercheArticleCodebar = new QAction(this);

    lineEdit_recherche_reference->addAction(_actionRechercheArticleCodebar);

    connect(_actionRechercheArticleCodebar, SIGNAL(triggered()),
    		this,SLOT(setRechercheCodebarArticleFocus()));

    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    this->setupShortcutActionAfficherPDF	(*actionAfficherPDF);
    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);

    _actionRechercheArticleCodebar->setShortcut(Qt::Key_F11);

    actionRechercher->setShortcut(YerothUtils::RECHERCHER_QKEYSEQUENCE);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothStocksWindow::contextMenuEvent(QContextMenuEvent * event)
{
    YerothPOSUser *aUser = _allWindows->getUser();
    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            if (tableView_stocks->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_stocksWindow->palette());
                menu.addAction(actionAfficher_stock_au_detail);
                menu.addAction(actionAfficher_lhistorique_de_ce_stock);
                menu.addAction(actionModifier_ce_stock);
                menu.addAction(actionSupprimer_ce_stock);
                menu.exec(event->globalPos());
            }
        }
        if (aUser->isVendeur())
        {
            if (tableView_stocks->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_stocksWindow->palette());
                menu.addAction(actionAfficher_stock_au_detail);
                menu.exec(event->globalPos());
            }
        }
        else if (aUser->isGestionaireDesStocks())
         {
             if (tableView_stocks->rowCount() > 0)
             {
                 QMenu menu(this);
                 menu.setPalette(toolBar_stocksWindow->palette());
                 menu.addAction(actionAfficher_stock_au_detail);
                 menu.addAction(actionAfficher_lhistorique_de_ce_stock);
                 menu.addAction(actionModifier_ce_stock);
                 menu.addAction(actionSupprimer_ce_stock);
                 menu.exec(event->globalPos());
             }
         }
        else if (aUser->isMagasinier())
         {
             if (tableView_stocks->rowCount() > 0)
             {
                 QMenu menu(this);
                 menu.setPalette(toolBar_stocksWindow->palette());
                 menu.addAction(actionAfficher_stock_au_detail);
                 menu.exec(event->globalPos());
             }
         }
    }
}


void YerothStocksWindow::hideEvent(QHideEvent * hideEvent)
{
	_searchStocksWidget->rendreInvisible();
	_allWindows->_historiqueDuStockWindow->close();
}


void YerothStocksWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    _curStocksTableModel = stocksTableModel;

    if (! isCurrentlyFiltered())
    {
        if (0 == _searchStocksTableModel)
        {
        	//qDebug() << QString("++ NON ");
            _searchStocksWidget->setSqlTableModel(&_allWindows->getSqlTableModel_stocks());
        }
    }

    lineEdit_recherche_reference->setFocus();

    setComboBoxStrategieDeStocks();

    afficherStocks();

    setVisible(true);

    if (YerothERPConfig::_distantSiteConnected)
    {
        pushButton_connecter_localisation->setPalette(YerothUtils::YEROTH_GREEN_PALETTE);
    }
    else
    {
        pushButton_connecter_localisation->setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
    }
}

void YerothStocksWindow::rendreInvisible()
{
    lineEdit_recherche_reference->clear();
    YerothWindowsCommons::rendreInvisible();
}

void YerothStocksWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInventaireDesStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    desactiverComboBoxStrategieDeGestionDesStocks();

    pushButton_stocks_filtrer->disable(this);
    pushButton_stocks_reinitialiser_filtre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->disable(this);
    pushButton_inventaire_des_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_connecter_localisation->disable(this);
    pushButton_deconnecter_localisation->disable(this);
}

void YerothStocksWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInventaireDesStocks, true);
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
    activerComboBoxStrategieDeGestionDesStocks();

    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_inventaire_des_stocks->enable(this, SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_connecter_localisation->enable(this, SLOT(connecter_localisation()));
    pushButton_deconnecter_localisation->enable(this, SLOT(deconnecter_localisation()));
}


void YerothStocksWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInventaireDesStocks, true);
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

    activerComboBoxStrategieDeGestionDesStocks();

    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_inventaire_des_stocks->enable(this, SLOT(afficherMarchandises()));
    pushButton_sortir->disable(this);
    pushButton_connecter_localisation->enable(this, SLOT(connecter_localisation()));
    pushButton_deconnecter_localisation->enable(this, SLOT(deconnecter_localisation()));
}


void YerothStocksWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInventaireDesStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    activerComboBoxStrategieDeGestionDesStocks();

    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_inventaire_des_stocks->enable(this, SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_connecter_localisation->enable(this, SLOT(connecter_localisation()));
    pushButton_deconnecter_localisation->enable(this, SLOT(deconnecter_localisation()));
}

void YerothStocksWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInventaireDesStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    desactiverComboBoxStrategieDeGestionDesStocks();

    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_inventaire_des_stocks->disable(this);
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_connecter_localisation->enable(this, SLOT(connecter_localisation()));
    pushButton_deconnecter_localisation->enable(this, SLOT(deconnecter_localisation()));
}

void YerothStocksWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInventaireDesStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    desactiverComboBoxStrategieDeGestionDesStocks();

    pushButton_stocks_filtrer->disable(this);
    pushButton_stocks_reinitialiser_filtre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_inventaire_des_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_connecter_localisation->disable(this);
    pushButton_deconnecter_localisation->disable(this);
}

void YerothStocksWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    if (this->getLastListerSelectedRow() > -1 && _curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_detailWindow->rendreVisible(_curStocksTableModel);
        this->rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un stock"),
                                  QObject::trUtf8("Sélectionnez un stock à afficher les détails."));
    }
}

void YerothStocksWindow::afficher_au_detail(const QModelIndex & modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    this->setLastListerSelectedRow(modelIndex.row());

    tableView_stocks->selectRow(this->getLastListerSelectedRow());

    if (this->getLastListerSelectedRow() > -1 && _curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_detailWindow->rendreVisible(_curStocksTableModel);

        this->rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un stock"),
                                  QObject::trUtf8("Sélectionnez un stock à afficher les détails."));
    }
}

void YerothStocksWindow::afficher_stock_selectioner_bar_code(const QString & stockBarCode)
{
    _logger->log("afficher_stock_selectioner_bar_code(const QString &)");
    this->setLastListerSelectedRow(0);
    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::REFERENCE, stockBarCode));
    //qDebug() << "++ stockName: " << stockName;
    _curStocksTableModel->yerothSetFilter(filter);
    if (_curStocksTableModel->easySelect() > 0)
    {
        this->afficherStocks(*_curStocksTableModel);
        this->setSearchFormSqlTableModel(_curStocksTableModel);
        this->set_rechercher_font();
    }
}

void YerothStocksWindow::afficher_stock_selectioner(const QString & stockName)
{
    _logger->log("afficher_stock_selectioner(const QString &)");
    this->setLastListerSelectedRow(0);
    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, stockName));
    //qDebug() << "++ stockName: " << stockName;
    _curStocksTableModel->yerothSetFilter(filter);
    if (_curStocksTableModel->easySelect() > 0)
    {
        this->afficherStocks(*_curStocksTableModel);
        this->setSearchFormSqlTableModel(_curStocksTableModel);
        this->set_rechercher_font();
    }
}

void YerothStocksWindow::connecter_localisation()
{
    reinitialiser_recherche();
    if (YerothUtils::slot_connecter_localisation(*this, _allWindows, lineEdit_localisation->text()))
    {
        pushButton_connecter_localisation->setPalette(YerothUtils::YEROTH_GREEN_PALETTE);
    }
    else
    {
        lineEdit_localisation->setText(_allWindows->getInfoEntreprise().getLocalisation());
        pushButton_connecter_localisation->setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
    }
    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();
    _searchStocksWidget->setSqlTableModel(&_allWindows->getSqlTableModel_stocks());
}

void YerothStocksWindow::deconnecter_localisation()
{
    reinitialiser_recherche();
    lineEdit_localisation->setText(_allWindows->getInfoEntreprise().getLocalisation());
    pushButton_connecter_localisation->setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
    YerothUtils::slot_deconnecter_localisation(_allWindows);
    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();
    _searchStocksWidget->setSqlTableModel(&_allWindows->getSqlTableModel_stocks());
}

/**
 * La suppression d'un stock entraine automatiquement la
 * suppression de l'achat correspondant
 */
void YerothStocksWindow::supprimer_ce_stock()
{
    _logger->log("supprimer_ce_stock");

    unsigned rowToRemove = tableView_stocks->lastSelectedRow();

    QSqlRecord record = _curStocksTableModel->record(rowToRemove);

    QString msgSupprimer(QObject::tr("Poursuivre avec la suppression de l'article \""));

    msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    msgSupprimer.append("\" ?");

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                       QObject::trUtf8
                                       ("suppression d'un stock"), msgSupprimer,
                                       QMessageBox::Cancel, QMessageBox::Ok))
    {
		_logger->debug("supprimer_ce_stock", QString("rowToRemove: %1").arg(rowToRemove));

        bool resRemoved = _curStocksTableModel->removeRow(rowToRemove);
        //qDebug() << "YerothStocksWindow::supprimer_ce_stock() " << resRemoved;

        if (resRemoved && _curStocksTableModel->select())
        {
            QString deleteAchatsRowQueryStr(QString("DELETE FROM %1 WHERE stocks_id = '%2';")
            									.arg(_allWindows->ACHATS,
            										 GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID)));

            YerothUtils::execQuery(deleteAchatsRowQueryStr);

            msgSupprimer.clear();

            msgSupprimer.append(QString(QObject::trUtf8("Le stock '%1' a été supprimé !"))
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)));

            YerothQMessageBox::information(this,
                                          QObject::tr("suppression d'un stock"),
                                          msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();

            msgSupprimer.append(QString(QObject::trUtf8("Le stock '%1' ne pouvait pas être supprimé !"))
            						.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)));

            YerothQMessageBox::warning(this,
                                       QObject::trUtf8("suppression d'un stock"),
                                       msgSupprimer);
        }

        this->afficherStocks(*_curStocksTableModel);
        this->updateLineEditRechercherCodeBar();
    }
    else
    {
    }
}


void YerothStocksWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_stocks_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothStocksWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_stocks_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    _searchStocksWidget->reinitialiser();

    setSearchFormSqlTableModel(0);

    setComboBoxStrategieDeStocks();

    set_rechercher_font();
}


void YerothStocksWindow::entrer()
{
    _logger->log("entrer");
    if (this->getLastListerSelectedRow() > -1 && _curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel, true);
        this->rendreInvisible();
    }
    else
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
        this->rendreInvisible();
    }
}


void YerothStocksWindow::modifier_les_articles()
{
    _logger->log("modifier_les_articles");
    if (_curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_modifierWindow->rendreVisible(_curStocksTableModel);
        this->rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, "modifier un stock",
                                  QObject::trUtf8("Sélectionnez un stock à modifier."));
    }
}


void YerothStocksWindow::afficherStocks(YerothSqlTableModel & sqlTableModel,
									   QString localVisibleStrategy	/* = "" */)
{
    //_logger->log("afficherStocks(YerothSqlTableModel &)");

    _curStocksTableModel = &sqlTableModel;

    QMultiMap < QString, int >designationToTableRows_in_out;

    QString currentStockListingStrategy(localVisibleStrategy);

    if (localVisibleStrategy.isEmpty())
    {
        currentStockListingStrategy = YerothERPConfig::salesStrategy;
    }
    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO, currentStockListingStrategy))
    {
    	tableView_stocks->setSortingEnabled(false);
        tableView_stocks->lister_FIFO(*_curStocksTableModel, designationToTableRows_in_out);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO, currentStockListingStrategy))
    {
    	tableView_stocks->setSortingEnabled(false);
        tableView_stocks->lister_LIFO(*_curStocksTableModel, designationToTableRows_in_out);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO, currentStockListingStrategy))
    {
    	tableView_stocks->setSortingEnabled(false);
        tableView_stocks->lister_DEF_DEO(*_curStocksTableModel, designationToTableRows_in_out);
    }
    else			//YerothConfig::STRATEGIE_VENTE_SORTIE_ALL
    {
    	tableView_stocks->setSortingEnabled(true);
        tableView_stocks->lister_les_elements_du_tableau(*_curStocksTableModel);
    }

    tableView_stocks->hideColumn(4);
    tableView_stocks->hideColumn(9);
    tableView_stocks->hideColumn(10);
    tableView_stocks->hideColumn(11);
    tableView_stocks->hideColumn(14);
    tableView_stocks->hideColumn(15);
    tableView_stocks->hideColumn(16);
    tableView_stocks->hideColumn(17);
    tableView_stocks->hideColumn(18);
    tableView_stocks->hideColumn(19);
    tableView_stocks->hideColumn(20);
    tableView_stocks->hideColumn(21);

    tableView_stocks->selectRow(tableView_stocks->lastSelectedRow());

    setWindowTitle(YerothUtils::getWindowTitleWithStrategy(this, localVisibleStrategy));
}

void YerothStocksWindow::afficherStocks()
{
    _searchStocksWidget->rendreInvisible();

    QString localVisibleStrategy = comboBox_strategie_de_stocks->currentText();

    afficherStocks(*_curStocksTableModel, localVisibleStrategy);
}


void YerothStocksWindow::set_filtrer_font()
{
    //_logger->log("set_filtrer_font");

    if (isCurrentlyFiltered())
    {
    	_pushButton_stocks_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_stocks_filtrer_font->setUnderline(false);
    }

    pushButton_stocks_filtrer->setFont(*_pushButton_stocks_filtrer_font);
}


void YerothStocksWindow::set_rechercher_font()
{
    //_logger->log("set_rechercher_font");
    if (0 != _searchStocksTableModel)
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


bool YerothStocksWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    QStandardItemModel *tableModel = tableView_stocks->getStandardItemModel();

    if (0 == tableModel)
    {
        return false;
    }

    QList < int >columnsToIgnore;

    columnsToIgnore << 0 << 4 << 9
                    << 11 << 14 << 15
					<< 16 << 17 << 18
					<< 20;

    int tableModelRowCount = tableModel->rowCount();
    int tableModelColumnCount = tableModel->columnCount();

    if (tableModelRowCount    <= 0  ||
            tableModelColumnCount <= 0)
    {
        YerothQMessageBox::information(this,
                                      QObject::trUtf8("pas de données à exporter au format csv"),
                                      QObject::trUtf8("Il n'y a pas de données à exporter au format csv !"));
        return false;
    }

    QString csvFileContent;

    QStandardItem * anItem = 0;
    QString anItemText;

    for (int k = 0; k < tableModelColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        anItem = tableModel->horizontalHeaderItem(k);

        if (0 != anItem)
        {
            anItemText = anItem->text();

            csvFileContent.append( QString("\"%1\", ").arg(anItemText) );
        }
    }

    csvFileContent.remove(csvFileContent.size() - 2, 2)
    .append("\n");

    for (unsigned int j = 0; j < tableModelRowCount; ++j)
    {
        for (unsigned int k = 0; k < tableModelColumnCount; ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }

            anItem = tableModel->item(j, k);

            if (0 != anItem)
            {
                anItemText = anItem->text();

                csvFileContent.append( QString("\"%1\", ").arg(anItemText) );
            }
        }

        csvFileContent.remove(csvFileContent.size() - 2, 2)
        .append("\n");
    }

    QString yerothStocksListingCSVFileName(YerothERPConfig::temporaryFilesDir);

    yerothStocksListingCSVFileName.append("/");

#ifdef YEROTH_FRANCAIS_LANGUAGE

    yerothStocksListingCSVFileName.append("yeroth-erp-fichier-stocks-format-csv");
    yerothStocksListingCSVFileName = FILE_NAME_USERID_CURRENT_TIME(yerothStocksListingCSVFileName);

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    yerothStocksListingCSVFileName.append("yeroth-erp-stocks-listing-csv-format");
    yerothStocksListingCSVFileName = FILE_NAME_USERID_CURRENT_TIME(yerothStocksListingCSVFileName);

#endif

    yerothStocksListingCSVFileName = QFileDialog::getSaveFileName(this, "Saisir le nom du fichier '.csv'",
                                 yerothStocksListingCSVFileName,
                                 QObject::trUtf8("Fichiers des stocks \"*.csv\" (*.csv)"));

    yerothStocksListingCSVFileName.append(".csv");

    QFile tmpFile(yerothStocksListingCSVFileName);

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(csvFileContent.toUtf8());
    }

    tmpFile.close();

    return true;
}

bool YerothStocksWindow::imprimer_document()
{
    _logger->log("imprimer_document");

    QString latexFileNamePrefix("yeroth-erp-fichier-stocks");

    QList<int> tableColumnsToIgnore;

    tableColumnsToIgnore << 0 << 4 << 9
                    	 << 10 << 11 << 15
						 << 14 << 15 << 16
						 << 17 << 18 << 19
						 << 20 << 21;

    QString pdfStockFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-fichier-stocks");

    pdfStockFileName = YerothUtils::prindDocumentFromTableView(this,
    														   *tableView_stocks,
															   tableColumnsToIgnore,
															   &YerothUtils::getStocksListingTexTableString,
															   &YerothUtils::getStocksListingFRTexDocumentString,
															   latexFileNamePrefix);
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-stocks-listing");

    pdfStockFileName = YerothUtils::prindDocumentFromTableView(this,
    														   *tableView_stocks,
															   tableColumnsToIgnore,
															   &YerothUtils::getStocksListingTexTableString,
															   &YerothUtils::getStocksListingENTexDocumentString,
															   latexFileNamePrefix);
#endif

    if (pdfStockFileName.isEmpty())
    {
    	return false;
    }

    YerothERPProcess::startPdfViewerProcess(pdfStockFileName);

    return true;
}
