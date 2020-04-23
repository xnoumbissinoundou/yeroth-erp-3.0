/*
 * yeroth-erp-paiements-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "src/windows/yeroth-erp-paiements-window.hpp"

#include  "src/process/yeroth-erp-process.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/windows/yeroth-erp-stock-detail-window.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include  <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <QtWidgets/QCalendarWidget>

#include <QtGui/QContextMenuEvent>

#include <QtCore/QThread>

#include <QtCore/QDir>

#include <QtCore/qmath.h>

#include <QtCore/QFile>

#include <QtCore/QProcess>

#include <QtCore/QProcessEnvironment>

#include <unistd.h>


const QString YerothPaiementsWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE, QObject::trUtf8("paiements")));


YerothPaiementsWindow::YerothPaiementsWindow()
:YerothWindowsCommons(YerothPaiementsWindow::_WINDOW_TITLE),
 YerothAbstractClassYerothSearchWindow(_allWindows->PAIEMENTS),
 _logger(new YerothLogger("YerothPaiementsWindow")),
 _aProcess(0),
 _currentTabView(0),
 _pushButton_paiements_filtrer_font(0),
 _paiementsDateFilter(YerothUtils::EMPTY_STRING),
 _curPaiementsTableModel(&_allWindows->getSqlTableModel_paiements())
{
    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupSelectDBFields(_allWindows->PAIEMENTS);

    {
    	int columnIndexTypeDePaiement = _toSelectDBFieldNameStrToDBColumnIndex.value(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT);

    	_DBFieldNamesToPrintLeftAligned.append(columnIndexTypeDePaiement);
    }


    _lineEditsToANDContentForSearch.insert(&lineEdit_paiements_terme_recherche,
    		YerothUtils::EMPTY_STRING);

    _lineEditsToANDContentForSearch.insert(&lineEdit_paiements_reference,
    		YerothDatabaseTableColumn::REFERENCE);

    _lineEditsToANDContentForSearch.insert(&lineEdit_paiements_nom_entreprise,
    		YerothDatabaseTableColumn::NOM_ENTREPRISE);

    _comboBoxesToANDContentForSearch.insert(&comboBox_paiements_intitule_du_compte_bancaire,
    		YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);

    _comboBoxesToANDContentForSearch.insert(&comboBox_paiements_type_de_paiement,
    		YerothDatabaseTableColumn::TYPE_DE_PAIEMENT);


    reinitialiser_champs_db_visibles();

    textEdit_description->setYerothEnabled(false);

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *)this);

    setupDateTimeEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);

    _pushButton_paiements_filtrer_font = new QFont(pushButton_paiements_filtrer->font());

    pushButton_paiements_filtrer->disable(this);
    pushButton_paiements_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_retour_paiements->enable(this, SLOT(retourPaiements()));


    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this, SLOT(slot_reinitialiser_champs_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this, SLOT(selectionner_champs_db_visibles()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_document()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionAfficherPaiementAuDetail, SIGNAL(triggered()), this, SLOT(afficher_paiements_detail()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(tabWidget_historique_paiements, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));

    connect(tableView_paiements, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(afficher_paiements_detail()));

    connect(tableView_paiements, SIGNAL(activated(const QModelIndex &)), this, SLOT(afficher_paiements_detail()));

    connect(comboBox_paiements_type_de_paiement,
    		SIGNAL(currentTextChanged(const QString &)),
			this,
			SLOT(handleComboBoxClients_Typedepaiement_TextChanged(const QString &)));

    setupShortcuts();
}


bool YerothPaiementsWindow::filtrer_paiements()
{
	QString paiementsTableColumnValue(lineEdit_paiements_element_de_paiements_resultat->text());

	if (paiementsTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
									   QObject::trUtf8("filtrer"),
									   msg);
		return false;

	}

	QString paiementsTableColumnProperty(comboBox_paiements_element_de_paiements->currentText());

	QString mathOperator(comboBox_paiements_element_de_paiements_condition->currentText());

	QString REAL_DB_ID_NAME_paiementsTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(paiementsTableColumnProperty));

	QString filterString(QString("%1 AND (%2 %3 %4)")
							.arg(_paiementsDateFilter,
								 REAL_DB_ID_NAME_paiementsTableColumnProperty,
								 mathOperator,
								 paiementsTableColumnValue));

	//qDebug() << "++ " << _searchFilter;

	if (!_searchFilter.isEmpty())
	{
		_searchFilter.append(QString(" AND %1").arg(filterString));
	}
	else
	{
		_searchFilter.append(filterString);
	}

	//qDebug() << QString("++ %1").arg(_searchFilter);

	setCurrentlyFiltered(true);

	_curPaiementsTableModel->yerothSetFilter(_searchFilter);

	int resultRows = _curPaiementsTableModel->easySelect();

	lister_les_elements_du_tableau(*_curPaiementsTableModel);

	if (resultRows > 0)
	{
		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "paiements - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "paiements - filtrer");

		return false;
	}

	set_filtrer_font();

	return false;
}


void YerothPaiementsWindow::populateComboBoxes()
{
	_logger->log("populateComboBoxes");

	comboBox_paiements_intitule_du_compte_bancaire->setYerothEnabled(false);

	comboBox_paiements_intitule_du_compte_bancaire->populateComboBoxRawString(_allWindows->COMPTES_BANCAIRES,
																			  YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);

	comboBox_paiements_type_de_paiement->setupPopulateNORawString(_allWindows->TYPE_DE_PAIEMENT,
																   YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
																   &YerothUtils::_typedepaiementToUserViewString);

	comboBox_paiements_type_de_paiement->populateComboBox();

	QStringList aQStringList;

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::MONTANT_PAYE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::COMPTE_CLIENT));

	comboBox_paiements_element_de_paiements->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_paiements_element_de_paiements_condition->addItems(aQStringList);
}


void YerothPaiementsWindow::setupLineEdits()
{
    _logger->log("setupLineEdits");

    lineEdit_paiements_terme_recherche->enableForSearch(QObject::trUtf8("terme à rechercher"));
    lineEdit_paiements_reference->enableForSearch(QObject::trUtf8("référence"));
    lineEdit_paiements_nom_entreprise->enableForSearch(QObject::tr("nom de l'entreprise"));


    lineEdit_details_de_paiement_numero_du_bon_de_paiement->setYerothEnabled(false);
    lineEdit_details_de_paiement_nom_de_lentreprise->setYerothEnabled(false);
    lineEdit_details_de_paiement_reference->setYerothEnabled(false);
    lineEdit_details_de_paiement_typedepaiement->setYerothEnabled(false);
    lineEdit_details_de_paiement_nom_de_lencaisseur->setYerothEnabled(false);
    lineEdit_details_de_paiement_compte_client->setYerothEnabled(false);
    lineEdit_details_de_paiement_montant_paye->setYerothEnabled(false);
    lineEdit_details_de_paiement_heure_de_paiement->setYerothEnabled(false);

    lineEdit_paiements_terme_recherche->setFocus();
}


void YerothPaiementsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionAfficherPDF	(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothPaiementsWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
    	_pushButton_paiements_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_paiements_filtrer_font->setUnderline(false);
    }

    pushButton_paiements_filtrer->setFont(*_pushButton_paiements_filtrer_font);
}


void YerothPaiementsWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	lister_les_elements_du_tableau();
}


void YerothPaiementsWindow::handleComboBoxClients_Typedepaiement_TextChanged(const QString &currentText)
{
	if (YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedepaiementToUserViewString.value(YerothUtils::VERSEMENT_BANCAIRE)) ||
		YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedepaiementToUserViewString.value(YerothUtils::VERSEMENT_TELEPHONE)) ||
		YerothUtils::isEqualCaseInsensitive(currentText,
				YerothUtils::_typedepaiementToUserViewString.value(YerothUtils::VERSEMENT_VIREMENT_BANCAIRE)))
	{
		comboBox_paiements_intitule_du_compte_bancaire->setYerothEnabled(true);
	}
	else
	{
		comboBox_paiements_intitule_du_compte_bancaire->setYerothEnabled(false);
	}
}


void YerothPaiementsWindow::textChangedSearchLineEditsQCompleters()
{
	lineEdit_paiements_element_de_paiements_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_paiements_terme_recherche->text());

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
        							.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::NOTES, partSearchTerm),
        								 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::REFERENCE, partSearchTerm),
										 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE, partSearchTerm)));

        	if (k != lastIdx)
        	{
        		_searchFilter.append(" AND ");
        	}
        }
    }

    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    {
    	YerothLineEdit *aYerothLineEdit = 0;

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

    				if (aYerothComboBox == comboBox_paiements_intitule_du_compte_bancaire)
    				{
    					curSearchDBStr = aTableColumnFieldContentForANDSearch;
    				}
    				else if (aYerothComboBox == comboBox_paiements_type_de_paiement)
    				{
        				int typedepaiement = YerothUtils::getComboBoxDatabaseQueryValue(aTableColumnFieldContentForANDSearch,
        						YerothUtils::_typedepaiementToUserViewString);

        				curSearchDBStr = QString::number(typedepaiement);
    				}

    				_searchFilter.append(GENERATE_SQL_IS_STMT(correspondingDBFieldKeyValue,
    														  curSearchDBStr));
    			}
    		}
    	}
    }

    QString finalSearchFilter(_paiementsDateFilter);

    if (!_searchFilter.isEmpty())
    {
    	QString searchFilterWithDate(QString("%1 AND (%2)")
    									.arg(_paiementsDateFilter,
    										 _searchFilter));

    	finalSearchFilter = searchFilterWithDate;
    }

	_yerothSqlTableModel->yerothSetFilter(finalSearchFilter);

    if (_yerothSqlTableModel->select())
    {
    	setLastListerSelectedRow(0);
    	lister_les_elements_du_tableau();
    }
    else
    {
        qDebug() << QString("++ YerothPaiementsWindow::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerothSqlTableModel->lastError().text());
    }
}


void YerothPaiementsWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

    _visibleDBFieldColumnStrList
    		<< YerothDatabaseTableColumn::DATE_PAIEMENT
			<< YerothDatabaseTableColumn::NOM_ENTREPRISE
			<< YerothDatabaseTableColumn::MONTANT_PAYE
			<< YerothDatabaseTableColumn::TYPE_DE_PAIEMENT
			<< YerothDatabaseTableColumn::COMPTE_CLIENT
			<< YerothDatabaseTableColumn::REFERENCE
			<< YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE;
}


void YerothPaiementsWindow::contextMenuEvent(QContextMenuEvent * event)
{
    if (tableView_paiements->rowCount() > 0)
    {
        QMenu menu(this);
        menu.addAction(actionAfficherPaiementAuDetail);
        menu.setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        menu.exec(event->globalPos());
    }
}

void YerothPaiementsWindow::clear_all_fields()
{
	textEdit_description->clear();

    lineEdit_details_de_paiement_numero_du_bon_de_paiement->clearField();
    lineEdit_details_de_paiement_nom_de_lentreprise->clearField();
    lineEdit_details_de_paiement_reference->clearField();
    lineEdit_details_de_paiement_typedepaiement->clearField();
    lineEdit_details_de_paiement_nom_de_lencaisseur->clearField();
    lineEdit_details_de_paiement_compte_client->clearField();
    lineEdit_details_de_paiement_montant_paye->clearField();
    lineEdit_details_de_paiement_heure_de_paiement->clearField();
}

void YerothPaiementsWindow::setupDateTimeEdits()
{
	dateEdit_details_de_paiement_date_paiement->setYerothEnabled(false);

    dateEdit_paiements_debut->setStartDate(GET_CURRENT_DATE);

    dateEdit_paiements_fin->setStartDate(GET_CURRENT_DATE);

    _paiementsDateFilter.clear();

	_paiementsDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    								.arg(YerothDatabaseTableColumn::DATE_PAIEMENT,
    									 DATE_TO_DB_FORMAT_STRING(dateEdit_paiements_debut->date()),
										 YerothDatabaseTableColumn::DATE_PAIEMENT,
										 DATE_TO_DB_FORMAT_STRING(dateEdit_paiements_fin->date())));

    connect(dateEdit_paiements_debut,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_paiements_fin,
    		SIGNAL(dateChanged(const QDate &)),
			this,
			SLOT(refineYerothLineEdits()));
}


void YerothPaiementsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}


void YerothPaiementsWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_paiements_filtrer->disable(this);
    pushButton_paiements_reinitialiser_filtre->disable(this);
}

void YerothPaiementsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_paiements_filtrer->enable(this, SLOT(filtrer_paiements()));
    pushButton_paiements_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothPaiementsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_paiements_filtrer->enable(this, SLOT(filtrer_paiements()));
    pushButton_paiements_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothPaiementsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_paiements_filtrer->disable(this);
    pushButton_paiements_reinitialiser_filtre->disable(this);
}

void YerothPaiementsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_paiements_filtrer->disable(this);
    pushButton_paiements_reinitialiser_filtre->disable(this);
}

void YerothPaiementsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_paiements_filtrer->disable(this);
    pushButton_paiements_reinitialiser_filtre->disable(this);
}

void YerothPaiementsWindow::readProcessData()
{
    _logger->log("readProcessData");
    if (!_aProcess)
    {
        return;
    }
    //qDebug() << "\t==>" << _aProcess->readAllStandardOutput();
}

bool YerothPaiementsWindow::export_csv_file()
{
	_logger->log("export_csv_file");

	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_paiements,
										   tableColumnsToIgnore,
										   "yeroth-erp-journal-paiements-format-csv",
										   "fiche des paiements");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_paiements,
										   tableColumnsToIgnore,
										   "yeroth-erp-payments-journal-csv-format",
										   "payments listing file");
#endif

	return success;
}

void YerothPaiementsWindow::getJournalDesPaiementsTexTableString(QString & texTable_in_out,
        												  	  	 QStandardItemModel & tableStandardItemModel,
																 QList<int> &dbFieldNameOfTypeString,
																 QList<int> &columnsToIgnore,
																 int fromRowIndex,
																 int toRowIndex, bool lastPage)
{
    if (lastPage && toRowIndex > 20)
    {
        toRowIndex -= 1;
    }

    if (fromRowIndex == toRowIndex)
    {
        return ;
    }

    texTable_in_out.append("\\begin{table*}[!htbp]")
    			   .append("\n")
				   .append("\\centering")
				   .append("\n")
				   .append("\\begin{tabular}")
				   .append("{|");

    //Tex table header

    /** We center the n0 column*/

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

    texTable_in_out.append("} \\hline \n");


    QStandardItem *item;

    int tableColumnCount = 1 + tableStandardItemModel.columnCount();
    /** We add a column named ''n0'' for numbering the rows

     * in the Tex table. */
    texTable_in_out.append("\\textbf{n\\textsuperscript{o}} & ");

    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);

        if (0 != item)
        {
            QString itemText(item->text());

            YerothUtils::handleTexTableItemText(tableColumnCount,
                                                texTable_in_out,
												k,
												itemText);
        }
    }

    YerothUtils::cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append("\\\\ \\hline \n");

    //Tex table body


    static int b = 1;
    if (0 == fromRowIndex)
    {
        b = 1;
    }
    QString idColumnText;
    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        idColumnText.clear();
        idColumnText.append(QString::number(b)).prepend("\\textbf{").append("}").append(" & ");
        texTable_in_out.append(idColumnText);
        ++b;
        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }
            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                /**
                 * Any string shall have a length smaller than
                 * YerothERPConfig::max_string_display_length
                 */
                QString itemText(item->text());
                if (itemText.length() > YerothERPConfig::max_string_display_length)
                {
                    itemText.truncate(YerothERPConfig::max_string_display_length);
                    itemText.append(".");
                }

                YerothUtils::handleTexTableItemText(tableColumnCount,
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
                    texTable_in_out.append("\"\" \\hline \n");
                }
            }
        }
        YerothUtils::cleanUpTexTableLastString(texTable_in_out);
        texTable_in_out.append("\\\\ \\hline \n");
    }

    //Removes string "" from Latex output
    texTable_in_out.replace("\"\"", "");
    texTable_in_out.append("\\end{tabular}\n\\end{table*}\n");
}

bool YerothPaiementsWindow::imprimer_document()
{
    _logger->log("imprimer_document");

    QString texTable;

    QStandardItemModel *tableModel = tableView_paiements->getStandardItemModel();

    QList<int> columnsToIgnore;

    fill_table_columns_to_ignore(columnsToIgnore);

    int tableModelRowCount = tableModel->rowCount();

    if (tableModelRowCount <= 0)
    {
        YerothQMessageBox::information(this,
                                       QObject::trUtf8("pas de données à imprimer"),
									   QObject::trUtf8("Il n'y a pas de données à imprimer !"));
        return false;
    }

    int fromRowIndex = 0;

    int toRowIndex = tableModelRowCount;

    double MAX_TABLE_ROW_COUNT = 35.0;

    int pageNumber = qCeil(tableModelRowCount / MAX_TABLE_ROW_COUNT);

    getJournalDesPaiementsTexTableString(texTable,
    										   *tableModel,
											   _DBFieldNamesToPrintLeftAligned,
											   columnsToIgnore,
											   0,
											   (20 >= tableModelRowCount) ? tableModelRowCount : 20,
											   tableModelRowCount <= 20);

    if (tableModelRowCount >= 20)
    {
        fromRowIndex = 20;
        toRowIndex = (fromRowIndex >= tableModelRowCount) ? fromRowIndex : fromRowIndex + MAX_TABLE_ROW_COUNT;
        int k = 1;
        do
        {
            getJournalDesPaiementsTexTableString(texTable,
            										   *tableModel,
													   _DBFieldNamesToPrintLeftAligned,
													   columnsToIgnore,
                                                       (fromRowIndex >=
                                                            tableModelRowCount) ? tableModelRowCount : fromRowIndex,
                                                       (toRowIndex >=
                                                            tableModelRowCount) ? (tableModelRowCount + 1) : toRowIndex,
                                                       k == pageNumber);
            texTable.append(QString("\\newpage \n"));
            fromRowIndex = toRowIndex;
            toRowIndex =
                (fromRowIndex >= tableModelRowCount) ? (fromRowIndex + 1) : fromRowIndex + MAX_TABLE_ROW_COUNT;
            ++k;
        }
        while (k <= pageNumber && fromRowIndex <= toRowIndex);
    }

    YerothInfoEntreprise & infoEntreprise = _allWindows->getInfoEntreprise();

    QString texDocument;

    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVilleTex()));

    YerothUtils::getCurrentLocaleDate(factureDate);


    YerothUtils::getTexLandscapePaymentsDocumentString(texDocument, texTable);


    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", factureDate);
    texDocument.replace("YEROTHVENTESDEBUT", DATE_TO_STRING(dateEdit_paiements_debut->date()));
    texDocument.replace("YEROTHVENTESFIN", DATE_TO_STRING(dateEdit_paiements_fin->date()));
    texDocument.replace("YEROTHNOMUTILISATEUR", _allWindows->getUser()->nom_completTex());
    texDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());

    QString yerothFiltres;

    YerothUtils::addFiltre(lineEdit_paiements_nom_entreprise, QObject::trUtf8("entreprise"), yerothFiltres);

    int lastIndexOfComa = yerothFiltres.lastIndexOf(",");

    yerothFiltres.remove(lastIndexOfComa, yerothFiltres.length());

    texDocument.replace("YEROTHFILTRES", YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltres));

    //qDebug() << "++ temp file dir: " << YerothConfig::temporaryFilesDir;
    //qDebug() << "++ texDocument: \n" << texDocument << "\n++++++++";

    QString yerothTableauCaissePrefixFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    yerothTableauCaissePrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-journal-paiements"));

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    yerothTableauCaissePrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-cashier-journal"));

#endif

    QFile tmpLatexFile(QString("%1tex")
    		.arg(yerothTableauCaissePrefixFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
    		texDocument);

    QString pdfPaiementsFileName(YerothERPProcess::compileLatex(yerothTableauCaissePrefixFileName));

    YerothERPProcess::startPdfViewerProcess(pdfPaiementsFileName);

    return true;
}


void YerothPaiementsWindow::resetFilter(YerothSqlTableModel * historiquePaiementsTableModel)
{
    _curPaiementsTableModel = historiquePaiementsTableModel;

    if (_curPaiementsTableModel)
    {
        _curPaiementsTableModel->resetFilter();
    }

    dateEdit_paiements_debut->reset();
    dateEdit_paiements_fin->reset();
}


void YerothPaiementsWindow::retourPaiements()
{
    _currentTabView = TableauDesPaiements;
    tabWidget_historique_paiements->setCurrentIndex(TableauDesPaiements);
}


void YerothPaiementsWindow::handleCurrentChanged(int index)
{
    //_logger->log("handleCurrentChanged(int)",
    //                  QString("handleCurrentChanged]. index: %1").arg(index));
    _currentTabView = index;
    switch (index)
    {
    case TableauDesPaiements:
        lister_les_elements_du_tableau();
        enableImprimer();
        break;
    case AfficherPaiementAuDetail:
        afficher_paiements_detail();
        disableImprimer();
        break;
    default:
        break;
    }
}


void YerothPaiementsWindow::lister_les_elements_du_tableau(YerothSqlTableModel &historiquePaiementsTableModel)
{
    tableView_paiements->lister_les_elements_du_tableau(historiquePaiementsTableModel);

    int curPaiementsTableModelRowCount = _curPaiementsTableModel->easySelect();

    if (curPaiementsTableModelRowCount < 0)
    {
    	curPaiementsTableModelRowCount = 0;
    }

    double montant_total = 0.0;
    double montant_paye = 0.0;

    QSqlRecord aRecord;

    for (int k = 0; k < curPaiementsTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = _curPaiementsTableModel->record(k);

        montant_paye = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::MONTANT_PAYE).toDouble();

        montant_total += montant_paye;
    }

    lineEdit_paiements_nombre_paiements->setText(GET_NUM_STRING(curPaiementsTableModelRowCount));
    lineEdit_paiements_total->setText(GET_CURRENCY_STRING_NUM(montant_total));

    if (tableView_paiements->rowCount() > 0)
    {
        tabWidget_historique_paiements->setTabEnabled(AfficherPaiementAuDetail, true);
    }
    else
    {
        tabWidget_historique_paiements->setTabEnabled(AfficherPaiementAuDetail, false);
    }

    tableView_show_or_hide_columns(*tableView_paiements);

    tableView_paiements->resizeColumnsToContents();
}


void YerothPaiementsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    _curStocksTableModel = stocksTableModel;

    _curPaiementsTableModel = &_allWindows->getSqlTableModel_paiements();

    setYerothSqlTableModel(_curPaiementsTableModel);

    setupLineEditsQCompleters((QObject *)this);

    tabWidget_historique_paiements->setCurrentIndex(TableauDesPaiements);

    lineEdit_paiements_nom_entreprise->setYerothEnabled(true);
    lineEdit_paiements_reference->setYerothEnabled(true);

    setVisible(true);

    afficher_paiements();

    lineEdit_paiements_terme_recherche->setFocus();
}


void YerothPaiementsWindow::afficher_paiements_detail()
{
	if (tableView_paiements->rowCount() <= 0)
	{
		return ;
	}

    int lastSelectedPaiementsRow = tableView_paiements->lastSelectedRow();

    //_logger->log("afficher_paiements_detail]", QString("row: %1").arg(lastSelectedPaiementsRow));
    QSqlRecord record = _curPaiementsTableModel->record(lastSelectedPaiementsRow);

    textEdit_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOTES));

    lineEdit_details_de_paiement_numero_du_bon_de_paiement->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID));
    lineEdit_details_de_paiement_nom_de_lentreprise->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));
    lineEdit_details_de_paiement_nom_de_lencaisseur->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENCAISSEUR));

    lineEdit_details_de_paiement_heure_de_paiement->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::HEURE_PAIEMENT));

    lineEdit_details_de_paiement_reference->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));

    int typeDePaiementIntValue = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt();

    lineEdit_details_de_paiement_typedepaiement->setText(YerothUtils::_typedepaiementToUserViewString.value(typeDePaiementIntValue));

    double aDoubleValue = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

    lineEdit_details_de_paiement_compte_client->setText(GET_CURRENCY_STRING_NUM(aDoubleValue));

    aDoubleValue = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_PAYE).toDouble();

    lineEdit_details_de_paiement_montant_paye->setText(GET_CURRENCY_STRING_NUM(aDoubleValue));

    dateEdit_details_de_paiement_date_paiement->setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_PAIEMENT)));

    tabWidget_historique_paiements->setCurrentIndex(AfficherPaiementAuDetail);
}


void YerothPaiementsWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_paiements_element_de_paiements_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothPaiementsWindow::reinitialiser_recherche()
{
    //  _logger->log("reinitialiser_recherche");
    lineEdit_paiements_element_de_paiements_resultat->clear();

    resetFilter(&_allWindows->getSqlTableModel_paiements());

    resetLineEditsQCompleters((QObject *)this);

    afficher_paiements();

    lineEdit_paiements_terme_recherche->setFocus();
}


void YerothPaiementsWindow::refineYerothLineEdits()
{
	_paiementsDateFilter.clear();

	_paiementsDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    								.arg(YerothDatabaseTableColumn::DATE_PAIEMENT,
    									 DATE_TO_DB_FORMAT_STRING(dateEdit_paiements_debut->date()),
										 YerothDatabaseTableColumn::DATE_PAIEMENT,
										 DATE_TO_DB_FORMAT_STRING(dateEdit_paiements_fin->date())));

	setupLineEditsQCompleters((QObject *)this);

	afficher_paiements();
}

