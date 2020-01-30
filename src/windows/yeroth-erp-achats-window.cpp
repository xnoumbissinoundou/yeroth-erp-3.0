/*
 * yeroth-erp-achats-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-achats-window.hpp"

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


unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_STOCKS_ID(1);

unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_REFERENCE(3);

unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_DESIGNATION(2);

unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_CATEGORIE(4);


const QString YerothAchatsWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
arg(YEROTH_ERP_WINDOW_TITLE,
QObject::trUtf8("fiche des achats")));

YerothAchatsWindow::YerothAchatsWindow()
    :YerothWindowsCommons(YerothAchatsWindow::_WINDOW_TITLE),
     _logger(new YerothLogger("YerothAchatsWindow")),
	 _currentlyFiltered(false),
     _aProcess(0),
	 _pushButton_achats_filtrer_font(0),
	 _pushButton_RechercherFont(0),
     _action_RechercherFont(0),
     _searchAchatsWidget(0),
	 _curAchatSqlTableModel(0),
     _searchAchatsTableModel(0)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_170, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _logger->log("YerothAchatsWindow");

    setupSelectDBFields(_allWindows->ACHATS);

    reinitialiser_champs_db_visibles();

    _curAchatSqlTableModel = &_allWindows->getSqlTableModel_achats();

    _searchAchatsWidget = new YerothSearchForm(_allWindows, *this, &_allWindows->getSqlTableModel_achats());

    populateComboBoxes();

    setupLineEdits();

    _pushButton_achats_filtrer_font = new QFont(pushButton_achats_filtrer->font());

    _action_RechercherFont = new QFont(actionRechercher->font());
    _pushButton_RechercherFont = new QFont(pushButton_rechercher->font());

    tableView_achats->setTableName(&YerothERPWindows::ACHATS);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_achats_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
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
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(afficherStocks()));
    connect(actionVentes, SIGNAL(triggered()), this, SLOT(ventes()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

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

	connect(actionAfficher_achat_au_detail, SIGNAL(triggered()),
			this, SLOT(afficher_au_detail()));

    connect(tableView_achats, SIGNAL(signal_lister(YerothSqlTableModel &)),
    		this, SLOT(set_rechercher_font()));

    connect(tableView_achats, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}

YerothAchatsWindow::~YerothAchatsWindow()
{
    delete _action_RechercherFont;
    delete _pushButton_RechercherFont;
    delete _searchAchatsWidget;
    delete _logger;
}


void YerothAchatsWindow::setCurrentlyFiltered(bool currentlyFiltered)
{
	_currentlyFiltered = currentlyFiltered;

	set_filtrer_font();
}


void YerothAchatsWindow::updateLineEditDesignation()
{
    lineEdit_recherche_designation->enableForSearch(QObject::trUtf8("désignation"));

    lineEdit_recherche_designation->setupMyQCompleterALL(_allWindows->ACHATS);

    connect(lineEdit_recherche_designation->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
            SLOT(afficher_stock_selectioner(const QString &)));
}


bool YerothAchatsWindow::filtrer_achats()
{
	QString achatsTableColumnValue(lineEdit_element_achats_resultat->text());

	if (achatsTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
				QObject::trUtf8("filtrer"),
				msg);

		return false;
	}

	reinitialiser_recherche();

	QString achatsTableColumnProperty(comboBox_element_achats->currentText());

	QString mathOperator(comboBox_element_achats_condition->currentText());

	QString REAL_DB_ID_NAME_achatsTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(achatsTableColumnProperty));

	QString filterString;

	filterString.append(QString("%1 %2 %3")
			.arg(REAL_DB_ID_NAME_achatsTableColumnProperty,
					mathOperator,
					achatsTableColumnValue));

	_curAchatSqlTableModel->yerothSetFilter(filterString);

	int resultRows = _curAchatSqlTableModel->easySelect();

	if (resultRows > 0)
	{
		setCurrentlyFiltered(true);

		afficherAchats(*_curAchatSqlTableModel);

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "achats - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "achats - filtrer");

		return false;
	}

	set_filtrer_font();

	return false;
}


void YerothAchatsWindow::populateComboBoxes()
{
	_logger->log("populateComboBoxes");

	QStringList aQStringList;

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::MONTANT_TVA));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::PRIX_VENTE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::PRIX_UNITAIRE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::QUANTITE_TOTAL));

    comboBox_element_achats->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_element_achats_condition->addItems(aQStringList);
}


void YerothAchatsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);

    actionRechercher->setShortcut(YerothUtils::RECHERCHER_QKEYSEQUENCE);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothAchatsWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	afficherAchats(*_curAchatSqlTableModel);
}


void YerothAchatsWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

	_visibleDBFieldColumnStrList
		<< YerothDatabaseTableColumn::ID
		<< YerothDatabaseTableColumn::DESIGNATION
		<< YerothDatabaseTableColumn::CATEGORIE
		<< YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT
		<< YerothDatabaseTableColumn::PRIX_DACHAT
		<< YerothDatabaseTableColumn::MONTANT_TVA
		<< YerothDatabaseTableColumn::PRIX_VENTE
		<< YerothDatabaseTableColumn::MARGE_BENEFICIAIRE
		<< YerothDatabaseTableColumn::DATE_ENTREE
		<< YerothDatabaseTableColumn::QUANTITE_TOTAL;

}


void YerothAchatsWindow::contextMenuEvent(QContextMenuEvent * event)
{
    if (tableView_achats->rowCount() > 0)
    {
        QMenu menu(this);
        menu.setPalette(toolBar_achatsWindow->palette());
        menu.addAction(actionAfficher_achat_au_detail);
        menu.exec(event->globalPos());
    }
}


void YerothAchatsWindow::hideEvent(QHideEvent * hideEvent)
{
	_searchAchatsWidget->rendreInvisible();
}


void YerothAchatsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    _curStocksTableModel = stocksTableModel;

    if (! isCurrentlyFiltered())
    {
        if (0 == _searchAchatsTableModel)
        {
        	//qDebug() << QString("++ NON ");
            _searchAchatsWidget->setSqlTableModel(&_allWindows->getSqlTableModel_achats());
        }
    }

    lineEdit_recherche_designation->setFocus();

    YerothPOSUser *aCurrentUser = _allWindows->getUser();

    if (0 != aCurrentUser && !aCurrentUser->isManager())
    {
        if (_visibleDBFieldColumnStrList.contains(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE))
        {
        	_visibleDBFieldColumnStrList.removeAll(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE);
        }
    }

    afficherAchats(*_curAchatSqlTableModel);

    setVisible(true);
}


void YerothAchatsWindow::rendreInvisible()
{
    lineEdit_recherche_designation->clear();
    YerothWindowsCommons::rendreInvisible();
}


void YerothAchatsWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_achats_filtrer->disable(this);
    pushButton_achats_reinitialiser_filtre->disable(this);

    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
}

void YerothAchatsWindow::definirManager()
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

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
}


void YerothAchatsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));

    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
}


void YerothAchatsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_ventes->disable(this);
}

void YerothAchatsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));

    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
}

void YerothAchatsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    pushButton_achats_filtrer->disable(this);
    pushButton_achats_reinitialiser_filtre->disable(this);

    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_menu_principal->disable(this);
}

void YerothAchatsWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    if (this->getLastListerSelectedRow() > -1 && _curAchatSqlTableModel->rowCount() > 0)
    {
        _allWindows->_achatsDetailWindow->rendreVisible(getLastListerSelectedRow(),
        												_curStocksTableModel,
														_curAchatSqlTableModel);
        this->rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un stock"),
                                  QObject::trUtf8("Sélectionnez un stock à afficher les détails."));
    }
}


void YerothAchatsWindow::afficher_au_detail(const QModelIndex & modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    this->setLastListerSelectedRow(modelIndex.row());

    tableView_achats->selectRow(this->getLastListerSelectedRow());

    if (this->getLastListerSelectedRow() > -1 && _curAchatSqlTableModel->rowCount() > 0)
    {
    	//qDebug() << "++ test" << modelIndex.row();
        _allWindows->_achatsDetailWindow->rendreVisible(getLastListerSelectedRow(),
        												_curStocksTableModel,
														_curAchatSqlTableModel);

        this->rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un stock"),
                                  QObject::trUtf8("Sélectionnez un stock à afficher les détails."));
    }
}


void YerothAchatsWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_element_achats_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothAchatsWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_recherche_designation->clear();

    lineEdit_element_achats_resultat->clear();

    setCurrentlyFiltered(false);

    _searchAchatsWidget->reinitialiser();

    setSearchFormSqlTableModel(0);

    set_rechercher_font();
}


void YerothAchatsWindow::afficherAchats(YerothSqlTableModel &achatSqlTableModel)
{
    _searchAchatsWidget->rendreInvisible();

    tableView_achats->lister_les_elements_du_tableau(achatSqlTableModel);

    tableView_show_or_hide_columns(*tableView_achats);
}


void YerothAchatsWindow::afficher_stock_selectioner(const QString & stockName)
{
    _logger->log("afficher_stock_selectioner(const QString &)");

    //qDebug() << QString("afficher_stock_selectioner(%1)").arg(stockName);

    setLastListerSelectedRow(0);

    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _curAchatSqlTableModel->yerothSetFilter(filter);

    if (_curAchatSqlTableModel->easySelect() > 0)
    {
        this->afficherAchats(*_curAchatSqlTableModel);
        this->setSearchFormSqlTableModel(_curAchatSqlTableModel);
        this->set_rechercher_font();
    }
}


void YerothAchatsWindow::set_filtrer_font()
{
    //_logger->log("set_filtrer_font");

    if (isCurrentlyFiltered())
    {
    	_pushButton_achats_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_achats_filtrer_font->setUnderline(false);
    }

    pushButton_achats_filtrer->setFont(*_pushButton_achats_filtrer_font);
}


void YerothAchatsWindow::set_rechercher_font()
{
    //_logger->log("set_rechercher_font");
    if (0 != _searchAchatsTableModel)
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


bool YerothAchatsWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_achats,
										   tableColumnsToIgnore,
										   "yeroth-erp-fiche-achats",
										   "fiche des achats");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_achats,
										   tableColumnsToIgnore,
										   "yeroth-erp-stock-buyings-listing",
										   "stock buyings listing file");
#endif

	return success;
}


void YerothAchatsWindow::getAchatsListingTexDocumentString(QString &texDocumentString_in_out,
        												   QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(YerothUtils::template_lister_achats_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothAchatsWindow::getAchatsListingTexTableString(QString &texTable_in_out,
        										 	 	QStandardItemModel &tableStandardItemModel,
														QList<int> &columnsToIgnore,
														int fromRowIndex,
														int toRowIndex,
														bool lastPage)
{
    if (lastPage && toRowIndex > 20)
    {
        toRowIndex -= 1;
    }

    if (fromRowIndex == toRowIndex)
    {
        return ;
    }

    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    			   .append("\\centering").append("\n")
				   .append("\\begin{tabular}")
				   .append("{|");

    int texTableColumnCount = tableStandardItemModel.columnCount() + 1;

    texTable_in_out.append("c|");

    //Tex table header
    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        if (k == YerothAchatsWindow::PDF_LISTING_COLUMN_STOCKS_ID 	||
        	k == YerothAchatsWindow::PDF_LISTING_COLUMN_DESIGNATION ||
        	k == YerothAchatsWindow::PDF_LISTING_COLUMN_CATEGORIE)
        {
        	texTable_in_out.append("l|");
        }
        else if (k == YerothAchatsWindow::PDF_LISTING_COLUMN_REFERENCE)
        {
        	texTable_in_out.append("c|");
        }
        else
        {
        	texTable_in_out.append("r|");
        }
    }

    texTable_in_out.append("} \\hline\n");

    /** We add a column named 'id' for numbering the rows
     * in the Tex table. */
    unsigned int id = fromRowIndex + 1;
    texTable_in_out.append("\\textbf{n\\textsuperscript{o}} & ");

    QStandardItem *item;

    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);

        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{").append("}"));
            YerothUtils::handleTexTableItemText(tableStandardItemModel.columnCount(),
                                   texTable_in_out,
                                   k,
                                   itemText);
        }
    }
    /** Closing Tex table header */
    YerothUtils::cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append(" \\\\ \\hline\n");

    //qDebug() << QString("++ fromRowIndex: %1, toRowIndex: %2")
    //			.arg(QString::number(fromRowIndex), QString::number(toRowIndex));

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
                YerothUtils::handleAchatsTexTableItemText(tableStandardItemModel.columnCount(),
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
                    texTable_in_out.append("\"\"").append("\\hline\n");
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


bool YerothAchatsWindow::imprimer_document()
{
    _logger->log("imprimer_document");

    QString latexFileNamePrefix("yeroth-erp-marchandises");

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    QString pdfBuyingsFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-fichier-achats");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-purchases-listing");
#endif

    pdfBuyingsFileName = YerothUtils::prindDocumentFromTableView(this,
    														     *tableView_achats,
																 tableColumnsToIgnore,
																 &YerothAchatsWindow::getAchatsListingTexTableString,
																 &YerothAchatsWindow::getAchatsListingTexDocumentString,
																 latexFileNamePrefix);

    if (pdfBuyingsFileName.isEmpty())
    {
    	return false;
    }

    YerothERPProcess::startPdfViewerProcess(pdfBuyingsFileName);

    return true;
}
