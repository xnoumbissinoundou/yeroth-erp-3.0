/*
 * yeroth-erp-achats-window.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-achats-window.hpp"


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


unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_STOCKS_ID(1);

unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_REFERENCE(3);

unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_DESIGNATION(2);

unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_CATEGORIE(4);


YerothAchatsWindow::YerothAchatsWindow()
:YerothWindowsCommons("yeroth-erp-marchandises"),
 YerothAbstractClassYerothSearchWindow(_allWindows->ACHATS),
 _logger(new YerothLogger("YerothAchatsWindow")),
 _aProcess(0),
 _pushButton_achats_filtrer_font(0),
 _curAchatSqlTableModel(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("fiche des achats"));

    setupUi(this);

    mySetupUi(this);

    _yerothTableView_FROM_WINDOWS_COMMONS = tableView_achats;

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_170, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupSelectDBFields(_allWindows->ACHATS);

    _lineEditsToANDContentForSearch.insert(&lineEdit_achats_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _lineEditsToANDContentForSearch.insert(&lineEdit_achats_reference,
    		YerothDatabaseTableColumn::REFERENCE);

    _lineEditsToANDContentForSearch.insert(&lineEdit_achats_designation,
    		YerothDatabaseTableColumn::DESIGNATION);

    _lineEditsToANDContentForSearch.insert(&lineEdit_achats_categorie,
    		YerothDatabaseTableColumn::CATEGORIE);

    _lineEditsToANDContentForSearch.insert(&lineEdit_achats_nom_entreprise_fournisseur,
    		YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR);

    reinitialiser_champs_db_visibles();

    _curAchatSqlTableModel = &_allWindows->getSqlTableModel_achats();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    _pushButton_achats_filtrer_font = new QFont(pushButton_achats_filtrer->font());

    tableView_achats->setSqlTableName(&YerothERPWindows::ACHATS);

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

	connect(actionAfficher_achat_au_detail, SIGNAL(triggered()),
			this, SLOT(afficher_au_detail()));

    connect(tableView_achats, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}

YerothAchatsWindow::~YerothAchatsWindow()
{
    delete _logger;
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

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::QUANTITE_TOTALE));

    comboBox_element_achats->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_element_achats_condition->addItems(aQStringList);
}


void YerothAchatsWindow::setupLineEdits()
{
	lineEdit_achats_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (description de l'article)"));
	lineEdit_achats_reference->enableForSearch(QObject::trUtf8("référence"));
	lineEdit_achats_categorie->enableForSearch(QObject::trUtf8("catégorie"));
	lineEdit_achats_designation->enableForSearch(QObject::trUtf8("désignation"));
	lineEdit_achats_nom_entreprise_fournisseur->enableForSearch(QObject::tr("nom entreprise fournisseur"));

	lineEdit_nombre_dachats->setYerothEnabled(false);

	lineEdit_element_achats_resultat->setValidator(&YerothUtils::DoubleValidator);
}


void YerothAchatsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothAchatsWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	resetTableViewHorizontalHeader_DEFAULT_ORDERING();

	if (0 != _curAchatSqlTableModel)
	{
		afficherAchats(*_curAchatSqlTableModel);
	}
}


void YerothAchatsWindow::textChangedSearchLineEditsQCompleters()
{
	lineEdit_element_achats_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_achats_terme_recherche->text());

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
    	afficherAchats(*_yerothSqlTableModel);
    }
    else
    {
        qDebug() << QString("++ YerothAchatsWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }
}


void YerothAchatsWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

	_visibleDBFieldColumnStrList
		<< YerothDatabaseTableColumn::DESIGNATION
		<< YerothDatabaseTableColumn::CATEGORIE
		<< YerothDatabaseTableColumn::PRIX_DACHAT
		<< YerothDatabaseTableColumn::MONTANT_TVA
		<< YerothDatabaseTableColumn::PRIX_VENTE
		<< YerothDatabaseTableColumn::MARGE_BENEFICIAIRE
		<< YerothDatabaseTableColumn::DATE_ENTREE
		<< YerothDatabaseTableColumn::QUANTITE_TOTALE;

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


void YerothAchatsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setYerothSqlTableModel(_curAchatSqlTableModel);

    _curStocksTableModel = stocksTableModel;

    lineEdit_achats_terme_recherche->setFocus();

    YerothPOSUser *aCurrentUser = _allWindows->getUser();

    if (0 != aCurrentUser && !aCurrentUser->isManager())
    {
        if (_visibleDBFieldColumnStrList.contains(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE))
        {
        	_visibleDBFieldColumnStrList.removeAll(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE);
        }
    }

    setVisible(true);

    afficherAchats(*_curAchatSqlTableModel);
}


void YerothAchatsWindow::rendreInvisible()
{
	lineEdit_achats_terme_recherche->clear();
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

    if (getLastListerSelectedRow() > -1 && _curAchatSqlTableModel->rowCount() > 0)
    {
        _allWindows->_achatsDetailWindow->rendreVisible(getLastListerSelectedRow(),
        												_curStocksTableModel,
														_curAchatSqlTableModel);
        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::trUtf8("détails d'un stock"),
                                  QObject::trUtf8("Sélectionnez un stock à afficher les détails."));
    }
}


void YerothAchatsWindow::afficher_au_detail(const QModelIndex & modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    setLastListerSelectedRow(modelIndex.row());

    tableView_achats->selectRow(getLastListerSelectedRow());

    if (getLastListerSelectedRow() > -1 && _curAchatSqlTableModel->rowCount() > 0)
    {
    	//qDebug() << "++ test" << modelIndex.row();
        _allWindows->_achatsDetailWindow->rendreVisible(getLastListerSelectedRow(),
        												_curStocksTableModel,
														_curAchatSqlTableModel);

        rendreInvisible();
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

    lineEdit_achats_terme_recherche->clear();

    lineEdit_element_achats_resultat->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *)this);

    afficherAchats();
}


void YerothAchatsWindow::afficherAchats(YerothSqlTableModel &achatSqlTableModel)
{
    tableView_achats->lister_les_elements_du_tableau(achatSqlTableModel);

    tableView_show_or_hide_columns(*tableView_achats);

    int rowCount = tableView_achats->rowCount();

    lineEdit_nombre_dachats->setText(GET_NUM_STRING(rowCount));
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
        afficherAchats(*_curAchatSqlTableModel);
    }
}


void YerothAchatsWindow::set_filtrer_font()
{
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


bool YerothAchatsWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_lister_achats_tex;
	return YerothWindowsCommons::imprimer_pdf_document();
}

