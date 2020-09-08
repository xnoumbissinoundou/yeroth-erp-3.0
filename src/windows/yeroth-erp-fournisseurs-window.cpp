/*
 * yeroth-erp-fournisseurs-window.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
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
 YerothAbstractClassYerothSearchWindow(_allWindows->FOURNISSEURS),
 _logger(new YerothLogger("YerothERPFournisseursWindow")),
 _pushButton_filtrer_font(0),
 _curFournisseursTableModel(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("fournisseurs"));

    setupUi(this);

    mySetupUi(this);

    _yerothTableView_FROM_WINDOWS_COMMONS = tableView_fournisseurs;

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                   .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                                		COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupSelectDBFields(_allWindows->FOURNISSEURS);

    _lineEditsToANDContentForSearch.insert(&lineEdit_fournisseurs_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _lineEditsToANDContentForSearch.insert(&lineEdit_recherche_nom_entreprise,
    		YerothDatabaseTableColumn::NOM_ENTREPRISE);

    _lineEditsToANDContentForSearch.insert(&lineEdit_fournisseurs_reference_fournisseur,
    		YerothDatabaseTableColumn::REFERENCE_CLIENT);

    _lineEditsToANDContentForSearch.insert(&lineEdit_fournisseurs_quartier,
    		YerothDatabaseTableColumn::QUARTIER);

    _lineEditsToANDContentForSearch.insert(&lineEdit_fournisseurs_province_etat,
    		YerothDatabaseTableColumn::PROVINCE_ETAT);

    _lineEditsToANDContentForSearch.insert(&lineEdit_fournisseurs_ville,
    		YerothDatabaseTableColumn::VILLE);

    reinitialiser_champs_db_visibles();

    _curFournisseursTableModel = &_allWindows->getSqlTableModel_fournisseurs();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    populateFournisseursComboBoxes();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_fournisseurs->setTableName(&YerothERPWindows::FOURNISSEURS);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_fournisseur->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);


    //Menu actions
    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionPayerAuFournisseur, SIGNAL(triggered()), this, SLOT(private_payer_au_fournisseur()));
    connect(actionCreerFournisseur, SIGNAL(triggered()), this, SLOT(creerFournisseur()));
    connect(actionModifierFournisseur, SIGNAL(triggered()), this, SLOT(modifierFournisseur()));
    connect(actionSupprimerFournisseur, SIGNAL(triggered()), this, SLOT(supprimerFournisseur()));
    connect(actionAfficherDetailsFournisseur, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_pdf_document()));
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

	connect(actionAfficher_fournisseur_au_detail, SIGNAL(triggered()),
			this, SLOT(afficher_au_detail()));

    connect(tableView_fournisseurs, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(private_payer_au_fournisseur(const QModelIndex &)));

    setupShortcuts();
}

YerothERPFournisseursWindow::~YerothERPFournisseursWindow()
{
    delete _pushButton_filtrer_font;
    delete _logger;
}


void YerothERPFournisseursWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

    _visibleDBFieldColumnStrList
			<< YerothDatabaseTableColumn::REFERENCE_CLIENT
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
	menu.addAction(actionPayerAuFournisseur);
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
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);

    actionRechercher->setShortcut(YerothUtils::RECHERCHER_QKEYSEQUENCE);

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


void YerothERPFournisseursWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
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
										 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESCRIPTION_CLIENT, partSearchTerm)));

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
    	afficherFournisseurs(*_yerothSqlTableModel);
    }
    else
    {
        qDebug() << QString("++ YerothERPFournisseursWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }
}


void YerothERPFournisseursWindow::private_payer_au_fournisseur()
{
    if (getLastListerSelectedRow() > -1 && _curFournisseursTableModel->rowCount() > 0)
    {
    	rendreInvisible();

    	_allWindows->_payerAuFournisseurWindow->rendreVisible(getLastListerSelectedRow(),
    														   _curFournisseursTableModel,
    														   _curStocksTableModel);
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("payer à un fournisseur"),
                                  QObject::trUtf8("Sélectionnez un fournisseur afin d'effectuer un paiement !"));
    }
}


void YerothERPFournisseursWindow::private_payer_au_fournisseur(const QModelIndex & aModelIndex)
{
    setLastListerSelectedRow(aModelIndex.row());

    tableView_fournisseurs->selectRow(getLastListerSelectedRow());

    if (getLastListerSelectedRow() > -1 && _curFournisseursTableModel->rowCount() > 0)
    {
    	rendreInvisible();

    	_allWindows->_payerAuFournisseurWindow->rendreVisible(getLastListerSelectedRow(),
    														   _curFournisseursTableModel,
    														   _curStocksTableModel);
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("payer à un fournisseur"),
                                  QObject::trUtf8("Sélectionnez un fournisseur afin d'effectuer un paiement !"));
    }
}


void YerothERPFournisseursWindow::modifierFournisseur()
{
    if (_curFournisseursTableModel->rowCount() > 0)
    {
    	rendreInvisible();

        _allWindows->_modifierFournisseurWindow->rendreVisible(tableView_fournisseurs->lastSelectedRow(),
        														_curFournisseursTableModel,
																_curStocksTableModel);
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::tr("modifier un fournisseur"),
                                   QObject::trUtf8("Sélectionnez un fournisseur à modifier !"));
    }
}


void YerothERPFournisseursWindow::supprimerFournisseur()
{
	unsigned rowToRemove = tableView_fournisseurs->lastSelectedRow();

    YerothSqlTableModel *fournisseursTableModel = 0;

    if (_curFournisseursTableModel
            && YerothUtils::isEqualCaseInsensitive(_allWindows->FOURNISSEURS, _curFournisseursTableModel->sqlTableName()))
    {
        fournisseursTableModel = _curFournisseursTableModel;
    }
    else
    {
        return ;
    }

    QSqlRecord record = fournisseursTableModel->record(rowToRemove);

    if (record.isEmpty() || record.isNull(YerothDatabaseTableColumn::NOM_ENTREPRISE))
    {
        return;
    }

    QString nom_entreprise(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

    QString msgConfirmation(QString(QObject::trUtf8("Supprimer le fournisseur '%1' ?"))
    							.arg(nom_entreprise));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
            							QObject::tr("suppression d'un fournisseur"),
            							msgConfirmation,
										QMessageBox::Cancel,
										QMessageBox::Ok))
    {
        bool success = fournisseursTableModel->removeRow(rowToRemove);

        QString msg(QString(QObject::trUtf8("Le fournisseur '%1"))
        				.arg(nom_entreprise));

        if (success)
        {
        	setupLineEditsQCompleters((QObject *)this);

        	afficherFournisseurs();

            msg.append(QObject::trUtf8("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("suppression d'un fournisseur - succès"),
            							   msg,
										   QMessageBox::Ok);
        }
        else
        {
            msg.append(QObject::trUtf8(" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::tr("suppression d'un fournisseur - échec"),
										   msg,
										   QMessageBox::Ok);
        }
    }
}


void YerothERPFournisseursWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    tableView_fournisseurs->selectRow(getLastListerSelectedRow());

    if (getLastListerSelectedRow() > -1 && _curFournisseursTableModel->rowCount() > 0)
    {
    	//qDebug() << "++ test" << modelIndex.row();
        _allWindows->_fournisseurDetailsWindow->rendreVisible(getLastListerSelectedRow(),
        												 	 _curFournisseursTableModel,
															 _curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un fournisseur"),
                                  QObject::trUtf8("Sélectionnez un fournisseur à afficher les détails !"));
    }
}


void YerothERPFournisseursWindow::afficher_au_detail(const QModelIndex & modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    setLastListerSelectedRow(modelIndex.row());

    tableView_fournisseurs->selectRow(getLastListerSelectedRow());

    if (getLastListerSelectedRow() > -1 && _curFournisseursTableModel->rowCount() > 0)
    {
    	rendreInvisible();

        _allWindows->_fournisseurDetailsWindow->rendreVisible(getLastListerSelectedRow(),
        												 	  _curFournisseursTableModel,
															  _curStocksTableModel);
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un fournisseur"),
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
									  QObject::trUtf8("filtrer"),
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

	_curFournisseursTableModel->yerothSetFilter(filterString);

	int resultRows = _curFournisseursTableModel->easySelect();

	if (resultRows >= 0)
	{
		setCurrentlyFiltered(true);

		afficherFournisseurs(*_curFournisseursTableModel);

		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "fournisseurs - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "fournisseurs - filtrer");
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
    _logger->log("reinitialiser_recherche");

    lineEdit_recherche_nom_entreprise->clear();

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *)this);

    afficherFournisseurs();
}


void YerothERPFournisseursWindow::populateFournisseursComboBoxes()
{
    _logger->log("populateFournisseursComboBoxes");

	QStringList aQStringList;

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::COMPTE_CLIENT));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT));

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
	lineEdit_fournisseurs_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher (émail, réprésentant, description de l'entreprise fournisseure)"));

	lineEdit_recherche_nom_entreprise->enableForSearch(QObject::tr("nom de l'entreprise fournisseure"));
	lineEdit_fournisseurs_reference_fournisseur->enableForSearch(QObject::trUtf8("référence fournisseur"));
	lineEdit_fournisseurs_quartier->enableForSearch(QObject::tr("quartier"));
	lineEdit_fournisseurs_ville->enableForSearch(QObject::tr("ville"));
	lineEdit_fournisseurs_province_etat->enableForSearch(QObject::trUtf8("province / état"));

	lineEdit_nombre_de_fournisseurs->setYerothEnabled(false);

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

	afficherFournisseurs(*_curFournisseursTableModel);

	lineEdit_fournisseurs_terme_recherche->setFocus();
}


void YerothERPFournisseursWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_fournisseur->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPFournisseursWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_fournisseur->enable(this, SLOT(creerFournisseur()));
    pushButton_modifier->enable(this, SLOT(modifierFournisseur()));
    pushButton_supprimer->enable(this, SLOT(supprimerFournisseur()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothERPFournisseursWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_fournisseur->enable(this, SLOT(creerFournisseur()));
    pushButton_modifier->enable(this, SLOT(modifierFournisseur()));
    pushButton_supprimer->enable(this, SLOT(supprimerFournisseur()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothERPFournisseursWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_fournisseur->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPFournisseursWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_fournisseur->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerothERPFournisseursWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

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

    setLastListerSelectedRow(0);

    QString filter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE, nomEntreprise));

    //qDebug() << QString("filter: %1").arg(filter);

    _curFournisseursTableModel->yerothSetFilter(filter);

    if (_curFournisseursTableModel->easySelect() > 0)
    {
        afficherFournisseurs(*_curFournisseursTableModel);
    }
}


void YerothERPFournisseursWindow::afficherFournisseurs(YerothSqlTableModel &fournisseurSqlTableModel)
{
    tableView_fournisseurs->lister_les_elements_du_tableau(fournisseurSqlTableModel);

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


bool YerothERPFournisseursWindow::imprimer_pdf_document()
{
	_latex_template_print_pdf_content = YerothUtils::template_fournisseurs_tex;
	return YerothWindowsCommons::imprimer_pdf_document();
}

