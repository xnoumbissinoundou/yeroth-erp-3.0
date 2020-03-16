/*
   * yeroth-erp-ventes-window.cpp
   *
   *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
   */
#include "yeroth-erp-ventes-window.hpp"

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


const QString YerothVentesWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE, QObject::trUtf8("ventes")));

YerothVentesWindow::YerothVentesWindow()
:YerothWindowsCommons(YerothVentesWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothVentesWindow")),
 _aProcess(0),
 _currentTabView(0),
 _currentlyFiltered(false),
 _pushButton_ventes_filtrer_font(0),
 _searchFilter(""),
 _curStocksVenduTableModel(&_allWindows->getSqlTableModel_stocks_vendu())
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupSelectDBFields(_allWindows->STOCKS_VENDU);

    reinitialiser_champs_db_visibles();

    label_ventes_tva->setText(QString(QObject::tr("TVA (%1)")).arg(YerothERPConfig::currency));
    label_remise_devise->setText(QString(QObject::tr("remise (%1)")).arg(YerothERPConfig::currency));

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);

    _pushButton_ventes_filtrer_font = new QFont(pushButton_ventes_filtrer->font());

    pushButton_ventes_filtrer->disable(this);
    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_retour_ventes->enable(this, SLOT(retourVentes()));


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
    connect(actionAfficherVenteDetail, SIGNAL(triggered()), this, SLOT(afficher_vente_detail()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(tabWidget_ventes, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));
    connect(tableView_ventes, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(afficher_vente_detail()));
    connect(tableView_ventes, SIGNAL(activated(const QModelIndex &)), this, SLOT(afficher_vente_detail()));

    this->setupShortcuts();
}


bool YerothVentesWindow::filtrer_ventes()
{
	QString ventesTableColumnValue(lineEdit_ventes_element_de_vente_resultat->text());

	if (ventesTableColumnValue.isEmpty())
	{
		QString msg(QObject::trUtf8("Veuillez saisir une valeur numérique pour la recherche !"));

		YerothQMessageBox::information(this,
									   QObject::trUtf8("filtrer"),
									   msg);
		return false;

	}

	QString ventesTableColumnProperty(comboBox_ventes_element_de_vente->currentText());

	QString mathOperator(comboBox_ventes_element_de_vente_condition->currentText());

	QString REAL_DB_ID_NAME_ventesTableColumnProperty(
			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(ventesTableColumnProperty));

	QString filterString;

	filterString.append(QString("(%1 >= '%2') AND (%1 <= '%3')")
							.arg(YerothDatabaseTableColumn::DATE_VENTE,
								 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_debut->date()),
								 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_fin->date())));

	filterString.append(QString(" AND (%1 %2 %3)")
							.arg(REAL_DB_ID_NAME_ventesTableColumnProperty,
								 mathOperator,
								 ventesTableColumnValue));

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

	_curStocksVenduTableModel->yerothSetFilter(_searchFilter);

	int resultRows = _curStocksVenduTableModel->easySelect();

	lister_les_elements_du_tableau(*_curStocksVenduTableModel);

	if (resultRows > 0)
	{
		YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows, "ventes - filtrer");

		return true;
	}
	else
	{
		YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "ventes - filtrer");

		return false;
	}

	set_filtrer_font();

	return false;
}


void YerothVentesWindow::set_filtrer_font()
{
    //_logger->log("set_filtrer_font");

    if (isCurrentlyFiltered())
    {
    	_pushButton_ventes_filtrer_font->setUnderline(true);
    }
    else
    {
    	_pushButton_ventes_filtrer_font->setUnderline(false);
    }

    pushButton_ventes_filtrer->setFont(*_pushButton_ventes_filtrer_font);
}


void YerothVentesWindow::setCurrentlyFiltered(bool currentlyFiltered)
{
	_currentlyFiltered = currentlyFiltered;

	set_filtrer_font();
}


void YerothVentesWindow::populateComboBoxes()
{
	_logger->log("populateComboBoxes");

	QStringList aQStringList;

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::PRIX_VENTE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::PRIX_UNITAIRE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::QUANTITE_VENDUE));

	aQStringList.append(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(YerothDatabaseTableColumn::MONTANT_TVA));

	comboBox_ventes_element_de_vente->addItems(aQStringList);


	aQStringList.clear();

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

    comboBox_ventes_element_de_vente_condition->addItems(aQStringList);
}


void YerothVentesWindow::setupLineEdits()
{
    _logger->log("setupLineEdits");

    lineEdit_ventes_recherche->enableForSearch(QObject::trUtf8("référence"));
    lineEdit_ventes_nom_caissier->enableForSearch(QObject::trUtf8("nom du caissier (caissière)"));
    lineEdit_ventes_designation->enableForSearch(QObject::trUtf8("désignation"));
    lineEdit_ventes_categorie_produit->enableForSearch(QObject::trUtf8("nom de la catégorie d'articles"));
    lineEdit_ventes_nom_entreprise_fournisseur->enableForSearch(QObject::trUtf8("nom de l'entreprise fournisseur"));
    lineEdit_ventes_nom_entreprise_client->enableForSearch(QObject::trUtf8("nom de l'entreprise cliente"));
    lineEdit_ventes_reference_recu_vendu->enableForSearch(QObject::trUtf8("référence reçu de vente"));

    lineEdit_type_de_vente->setEnabled(false);
    lineEdit_nom_client->setEnabled(false);
    lineEdit_reference_produit->setEnabled(false);
    lineEdit_designation->setEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setEnabled(false);
    lineEdit_categorie->setEnabled(false);
    lineEdit_remise_prix->setEnabled(false);
    lineEdit_remise_pourcentage->setEnabled(false);
    lineEdit_tva->setEnabled(false);
    lineEdit_vente_detail_montant_total->setEnabled(false);
    lineEdit_nom_caissier->setEnabled(false);
    lineEdit_localisation->setEnabled(false);
    lineEdit_quantite_vendue->setEnabled(false);
    lineEdit_vente_detail_heure_vente->setEnabled(false);
    lineEdit_vente_detail_prix_unitaire->setEnabled(false);
    /*
    lineEdit_ventes_tva->setEnabled(false);
    lineEdit_ventes_remise_totale_currency->setEnabled(false);
    lineEdit_ventes_recette_totale->setEnabled(false);
    lineEdit_ventes_quantite_vendue->setEnabled(false);
    */
    lineEdit_ventes_recherche->setFocus();
    connect(lineEdit_ventes_recherche, SIGNAL(textChanged(const QString &)), this, SLOT(venteRecherche()));
    connect(lineEdit_ventes_nom_caissier, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_ventes_designation, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_ventes_categorie_produit, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_ventes_nom_entreprise_fournisseur, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_ventes_nom_entreprise_client, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_ventes_reference_recu_vendu, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
}

void YerothVentesWindow::setupLineEditsQCompleters()
{
    lineEdit_ventes_recherche->setupMyStaticQCompleter(_allWindows->STOCKS_VENDU, YerothDatabaseTableColumn::REFERENCE, false, false);
    lineEdit_ventes_nom_caissier->setupMyStaticQCompleter(_allWindows->STOCKS_VENDU, YerothDatabaseTableColumn::NOM_CAISSIER);
    lineEdit_ventes_designation->setupMyStaticQCompleter(_allWindows->STOCKS_VENDU, YerothDatabaseTableColumn::DESIGNATION);
    lineEdit_ventes_categorie_produit->setupMyStaticQCompleter(_allWindows->STOCKS_VENDU, YerothDatabaseTableColumn::CATEGORIE);
    lineEdit_ventes_nom_entreprise_fournisseur->setupMyStaticQCompleter(_allWindows->STOCKS_VENDU, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR);
    lineEdit_ventes_nom_entreprise_client->setupMyStaticQCompleter(_allWindows->STOCKS_VENDU, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);
    lineEdit_ventes_reference_recu_vendu->setupMyStaticQCompleter(_allWindows->STOCKS_VENDU, YerothDatabaseTableColumn::REFERENCE_RECU_VENDU);
}

void YerothVentesWindow::setupShortcuts()
{
    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    this->setupShortcutActionAfficherPDF		(*actionAfficherPDF);
    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothVentesWindow::slot_reinitialiser_champs_db_visibles()
{
	reinitialiser_champs_db_visibles();
	rechercher();
}


void YerothVentesWindow::reinitialiser_champs_db_visibles()
{
	_visibleDBFieldColumnStrList.clear();

    _visibleDBFieldColumnStrList
			<< YerothDatabaseTableColumn::DATE_VENTE
			<< YerothDatabaseTableColumn::HEURE_VENTE
			<< YerothDatabaseTableColumn::CATEGORIE
			<< YerothDatabaseTableColumn::DESIGNATION
			<< YerothDatabaseTableColumn::PRIX_UNITAIRE
			<< YerothDatabaseTableColumn::QUANTITE_VENDUE
			<< YerothDatabaseTableColumn::MONTANT_TVA
			<< YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE;
}


void YerothVentesWindow::contextMenuEvent(QContextMenuEvent * event)
{
    if (tableView_ventes->rowCount() > 0)
    {
        QMenu menu(this);
        menu.addAction(actionAfficherVenteDetail);
        menu.setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        menu.exec(event->globalPos());
    }
}

void YerothVentesWindow::clear_all_fields()
{
    lineEdit_nom_client->clearField();
    lineEdit_reference_produit->clearField();
    lineEdit_designation->clearField();
    lineEdit_nom_entreprise_fournisseur->clearField();
    lineEdit_categorie->clearField();
    lineEdit_remise_prix->clearField();
    lineEdit_remise_pourcentage->clearField();
    lineEdit_tva->clearField();
    lineEdit_vente_detail_montant_total->clearField();
    lineEdit_nom_caissier->clearField();
    lineEdit_localisation->clearField();
    lineEdit_quantite_vendue->clearField();
    lineEdit_vente_detail_heure_vente->clearField();
    lineEdit_vente_detail_prix_unitaire->clearField();
    lineEdit_ventes_tva->clearField();
    lineEdit_ventes_remise_totale_currency->clearField();
    lineEdit_ventes_recette_totale->clearField();
    lineEdit_ventes_quantite_vendue->clearField();
    lineEdit_ventes_recherche->clearField();
    /*lineEdit_ventes_nom_caissier->clearField();
    lineEdit_ventes_designation->clearField();
    lineEdit_ventes_categorie_produit->clearField();
    lineEdit_ventes_nom_entreprise_client->clearField();*/
}

void YerothVentesWindow::setupDateTimeEdits()
{
	dateEdit_vente_detail_date_vente->setYerothEnabled(false);
	dateEdit_vente_detail_date_peremption->setYerothEnabled(false);

    dateEdit_ventes_debut->setStartDate(GET_CURRENT_DATE);

    dateEdit_ventes_fin->setStartDate(GET_CURRENT_DATE);

    connect(dateEdit_ventes_debut, SIGNAL(dateChanged(const QDate &)), this, SLOT(rechercher()));
    connect(dateEdit_ventes_fin, SIGNAL(dateChanged(const QDate &)), this, SLOT(rechercher()));
}


void YerothVentesWindow::deconnecter_utilisateur()
{
    this->clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}

void YerothVentesWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_ventes_filtrer->disable(this);
    pushButton_ventes_reinitialiser_filtre->disable(this);
}

void YerothVentesWindow::definirManager()
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

    pushButton_ventes_filtrer->enable(this, SLOT(filtrer_ventes()));
    pushButton_ventes_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothVentesWindow::definirVendeur()
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

    pushButton_ventes_filtrer->enable(this, SLOT(filtrer_ventes()));
    pushButton_ventes_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerothVentesWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_ventes_filtrer->disable(this);
    pushButton_ventes_reinitialiser_filtre->disable(this);
}

void YerothVentesWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_ventes_filtrer->disable(this);
    pushButton_ventes_reinitialiser_filtre->disable(this);
}

void YerothVentesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_ventes_filtrer->disable(this);
    pushButton_ventes_reinitialiser_filtre->disable(this);
}

void YerothVentesWindow::readProcessData()
{
    _logger->log("readProcessData");
    if (!_aProcess)
    {
        return;
    }
    //qDebug() << "\t==>" << _aProcess->readAllStandardOutput();
}

bool YerothVentesWindow::export_csv_file()
{
    _logger->log("export_csv_file");

	bool success = false;

	QList<int> tableColumnsToIgnore;

	fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YEROTH_FRANCAIS_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_ventes,
										   tableColumnsToIgnore,
										   "yeroth-erp-journal-ventes-format-csv",
										   "journal des ventes");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	success = YerothUtils::export_csv_file(*this,
										   *tableView_ventes,
										   tableColumnsToIgnore,
										   "yeroth-erp-cashier-journal-csv-format",
										   "journal of sales");
#endif

	return success;
}

void YerothVentesWindow::getJournalDesVentesTexTableString(QString & texTable_in_out,
        												   QStandardItemModel & tableStandardItemModel,
														   QList<int> &dbFieldNameOfTypeString,
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

    texTable_in_out.append("\\begin{table*}[!htbp]")
    			   .append("\n")
				   .append("\\centering")
				   .append("\n")
				   .append("\\begin{tabular}")
				   .append("{|");

    //Tex table header

    /** We center the id column*/

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
    //qDebug() << "++ texTable caisse: " << texTable_in_out;

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

        if (item)
        {
            QString itemText(item->text());

            if (YerothUtils::isEqualCaseInsensitive(itemText, "Total TTC"))
            {
                itemText.append(" (Chiffre d'affaire)");
            }

            itemText.prepend("\\textbf{").append("}");

            YerothUtils::handleTexTableItemText(tableColumnCount,
                                               texTable_in_out,
                                               k,
                                               itemText);
        }
    }
    YerothUtils::cleanUpTexTableLastString(texTable_in_out);
    texTable_in_out.append("\\\\ \\hline \n");
    //Tex table body
    //qDebug() << "++ fromRowIndex: " << fromRowIndex;
    //qDebug() << "++ toRowIndex: " << toRowIndex;
    /* Variable to print the row's number in the output file */

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
        //qDebug() << "++ lastPage: " << lastPage << ". check j: " << j << ", rowToPrintSums: " << rowToPrintSums;
    }
    //Removes the string "" from Latex output
    texTable_in_out.replace("\"\"", "");
    texTable_in_out.append("\\end{tabular}").append("\n").append("\\end{table*}").append("\n");
}

bool YerothVentesWindow::imprimer_document()
{
    _logger->log("imprimer_document");

    QString texTable;

    QStandardItemModel *tableModel = tableView_ventes->getStandardItemModel();

    QList <int> columnsToIgnore;

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
    //qDebug() << QString("number of pages to print: %1").arg(pageNumber);
    //_logger->log("imprimer_document",
    //                  QString("number of pages to print: %1").arg(pageNumber));

    this->getJournalDesVentesTexTableString(texTable,
    										*tableModel,
											this->_DBFieldNamesToPrintLeftAligned,
											columnsToIgnore,
											0,
                                            (20 >= tableModelRowCount) ? tableModelRowCount : 20,
                                            tableModelRowCount <= 20);
    //qDebug() << "++ texTable: " << texTable;
    if (tableModelRowCount >= 20)
    {
        fromRowIndex = 20;
        toRowIndex = (fromRowIndex >= tableModelRowCount) ? fromRowIndex : fromRowIndex + MAX_TABLE_ROW_COUNT;
        int k = 1;
        do
        {
            //qDebug() << QString("## fromRowIndex: %1, toRowIndex: %2")
            //          .arg(QString::number(fromRowIndex), QString::number(toRowIndex));
            this->getJournalDesVentesTexTableString(texTable,
            										*tableModel,
													this->_DBFieldNamesToPrintLeftAligned,
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


    YerothUtils::getTexLandscapeSellingDocumentString(texDocument, texTable);


    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", factureDate);
    texDocument.replace("YEROTHVENTESDEBUT", DATE_TO_STRING(dateEdit_ventes_debut->date()));
    texDocument.replace("YEROTHVENTESFIN", DATE_TO_STRING(dateEdit_ventes_fin->date()));
    texDocument.replace("YEROTHNOMUTILISATEUR", _allWindows->getUser()->nom_completTex());
    texDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());

    QString yerothFiltres;

    YerothUtils::addFiltre(lineEdit_ventes_nom_caissier, QObject::trUtf8("caissier"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_designation, QObject::trUtf8("désignation"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_categorie_produit, QObject::trUtf8("catégorie"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_nom_entreprise_fournisseur, QObject::trUtf8("fournisseur"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_nom_entreprise_client, QObject::trUtf8("client"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_ventes_reference_recu_vendu, QObject::trUtf8("Nr. reçu"), yerothFiltres);

    int lastIndexOfComa = yerothFiltres.lastIndexOf(",");

    yerothFiltres.remove(lastIndexOfComa, yerothFiltres.length());

    texDocument.replace("YEROTHFILTRES", YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltres));

    //qDebug() << "++ temp file dir: " << YerothConfig::temporaryFilesDir;
    //qDebug() << "++ texDocument: \n" << texDocument << "\n++++++++";

    QString yerothTableauCaissePrefixFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    yerothTableauCaissePrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-journal-ventes"));

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    yerothTableauCaissePrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-cashier-journal"));

#endif


    QFile tmpLatexFile(QString("%1tex")
    					.arg(yerothTableauCaissePrefixFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
    												texDocument);

    QString pdfVentesFileName(YerothERPProcess::compileLatex(yerothTableauCaissePrefixFileName));

    YerothERPProcess::startPdfViewerProcess(pdfVentesFileName);

    return true;
}


void YerothVentesWindow::resetFilter(YerothSqlTableModel * stocksVenduTableModel)
{
    this->_curStocksVenduTableModel = stocksVenduTableModel;

    if (this->_curStocksVenduTableModel)
    {
        this->_curStocksVenduTableModel->resetFilter();
    }

    lineEdit_ventes_recherche->myClear();

    lineEdit_ventes_nom_caissier->myClear();
    lineEdit_ventes_designation->myClear();
    lineEdit_ventes_categorie_produit->myClear();
    lineEdit_ventes_nom_entreprise_fournisseur->myClear();
    lineEdit_ventes_nom_entreprise_client->myClear();
    lineEdit_ventes_reference_recu_vendu->myClear();

    dateEdit_ventes_debut->reset();
    dateEdit_ventes_fin->reset();
}


void YerothVentesWindow::retourVentes()
{
    _currentTabView = TableauDesVentes;
    tabWidget_ventes->setCurrentIndex(TableauDesVentes);
}


void YerothVentesWindow::handleCurrentChanged(int index)
{
    //_logger->log("handleCurrentChanged(int)",
    //                  QString("handleCurrentChanged]. index: %1").arg(index));
    _currentTabView = index;
    switch (index)
    {
    case TableauDesVentes:
        lister_les_elements_du_tableau();
        enableImprimer();
        break;
    case AfficherVenteAuDetail:
        afficher_vente_detail();
        disableImprimer();
        break;
    default:
        break;
    }
}


void YerothVentesWindow::lister_les_elements_du_tableau(YerothSqlTableModel &stocksVenduTableModel)
{
    tableView_ventes->lister_les_elements_du_tableau(stocksVenduTableModel);

    tableView_show_or_hide_columns(*tableView_ventes);

    int curStocksVenduTableModelRowCount = _curStocksVenduTableModel->easySelect();

    double quantite_vendue = 0;
    double quantite_vendue_total = 0.0;
    double remise_prix = 0.0;
    double remise_total_fcfa = 0.0;
    double montant_total = 0.0;
    double montant_total_vente = 0.0;
    double montant_tva = 0.0;
    double total_montant_tva = 0.0;

    QSqlRecord aRecord;
    QString stockId;
    QString strQuery;
    QSqlQuery query;

    for (int k = 0; k < curStocksVenduTableModelRowCount; ++k)
    {
        aRecord.clear();

        query.clear();

        aRecord = _curStocksVenduTableModel->record(k);

        quantite_vendue = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble();

        quantite_vendue_total += quantite_vendue;

        montant_total_vente = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();

        montant_total += montant_total_vente;

        remise_prix = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::REMISE_PRIX).toDouble();

        remise_total_fcfa += (quantite_vendue * remise_prix);

        montant_tva = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();

        total_montant_tva += montant_tva;

        stockId = GET_SQL_RECORD_DATA(aRecord, YerothDatabaseTableColumn::STOCKS_ID);
    }

    if (_allWindows->getUser()->isManager())
    {
        lineEdit_ventes_tva->setText(GET_CURRENCY_STRING_NUM(total_montant_tva));
    }
    else
    {
        lineEdit_ventes_tva->clear();
    }

    lineEdit_ventes_remise_totale_currency->setText(GET_CURRENCY_STRING_NUM(remise_total_fcfa));
    lineEdit_ventes_quantite_vendue->setText(GET_DOUBLE_STRING(quantite_vendue_total));
    lineEdit_ventes_recette_totale->setText(GET_CURRENCY_STRING_NUM(montant_total));

    if (tableView_ventes->rowCount() > 0)
    {
        tabWidget_ventes->setTabEnabled(AfficherVenteAuDetail, true);
    }
    else
    {
        tabWidget_ventes->setTabEnabled(AfficherVenteAuDetail, false);
    }

    tableView_ventes->resizeColumnsToContents();
}


void YerothVentesWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    _curStocksTableModel = stocksTableModel;

    _curStocksVenduTableModel = &_allWindows->getSqlTableModel_stocks_vendu();

    this->setupLineEditsQCompleters();

    tabWidget_ventes->setCurrentIndex(TableauDesVentes);


    label_ventes_tva->setText(QString(QObject::tr("TVA (%1)")).arg(YerothERPConfig::currency));
    label_remise_devise->setText(QString(QObject::tr("remise (%1)")).arg(YerothERPConfig::currency));

    lineEdit_ventes_reference_recu_vendu->setEnabled(true);
    lineEdit_ventes_categorie_produit->setEnabled(true);
    lineEdit_ventes_designation->setEnabled(true);
    lineEdit_ventes_nom_caissier->setEnabled(true);
    lineEdit_ventes_nom_entreprise_fournisseur->setEnabled(true);
    lineEdit_ventes_nom_entreprise_client->setEnabled(true);

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser && aUser->isVendeur())
    {
    	lineEdit_ventes_nom_caissier->setEnabled(false);
    	lineEdit_ventes_nom_caissier->setText(aUser->nom_complet());
    }
    else
    {
    	lineEdit_ventes_nom_caissier->clear();
    	lineEdit_ventes_nom_caissier->setEnabled(true);
    }

    setVisible(true);

    rechercher();

    lineEdit_ventes_recherche->setFocus();
}

void YerothVentesWindow::afficher_vente_detail()
{
	if (tableView_ventes->rowCount() <= 0)
	{
		return ;
	}

    int lastSelectedVentesRow = tableView_ventes->lastSelectedRow();

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord record = _curStocksVenduTableModel->record(lastSelectedVentesRow);

    lineEdit_reference_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));
    lineEdit_designation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    lineEdit_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
    lineEdit_categorie->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));

    double quantite_vendue = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble();

    lineEdit_quantite_vendue->setText(GET_NUM_STRING(quantite_vendue));
    lineEdit_vente_detail_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, "prix_unitaire").toDouble()));
    lineEdit_remise_prix->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REMISE_PRIX).toDouble()));

    QString remise_pourcentage(QString("%1 %")
    								.arg(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REMISE_POURCENTAGE)));

    lineEdit_remise_pourcentage->setText(remise_pourcentage);
    lineEdit_tva->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::TVA).toDouble()));
    lineEdit_vente_detail_montant_total->setText(GET_CURRENCY_STRING_NUM(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble()));

    dateEdit_vente_detail_date_peremption->setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_PEREMPTION)));
    dateEdit_vente_detail_date_vente->setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_VENTE)));

    lineEdit_vente_detail_heure_vente->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::HEURE_VENTE));
    lineEdit_nom_caissier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_CAISSIER));
    lineEdit_localisation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LOCALISATION));

    YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

    QString clientsIdFilter;

    clientsIdFilter.append(QString("%1 = '%2'")
    						 .arg(YerothDatabaseTableColumn::ID,
    							  GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CLIENTS_ID)));

    clientsTableModel.yerothSetFilter(clientsIdFilter);

    int clientsTableModelRowCount = clientsTableModel.easySelect();

    if (clientsTableModelRowCount > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);
        lineEdit_nom_client->setText(GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE));
    }
    else
    {
        lineEdit_nom_client->setText("");
    }

    lineEdit_nom_client->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT));
    lineEdit_type_de_vente->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::TYPE_DE_VENTE));

    tabWidget_ventes->setCurrentIndex(AfficherVenteAuDetail);
}


void YerothVentesWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_ventes_element_de_vente_resultat->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothVentesWindow::reinitialiser_recherche()
{
    //  _logger->log("reinitialiser_recherche");
    lineEdit_ventes_element_de_vente_resultat->clear();

    setCurrentlyFiltered(false);

    this->resetFilter(&_allWindows->getSqlTableModel_stocks_vendu());
    this->rechercher();
    this->lister_les_elements_du_tableau();
    lineEdit_ventes_recherche->setFocus();
}


void YerothVentesWindow::rechercher(bool clearVentesRecherche)
{
    //_logger->log("rechercher");

    lineEdit_ventes_element_de_vente_resultat->clear();

    setCurrentlyFiltered(false);

	_searchFilter.clear();

    if (clearVentesRecherche)
    {
        lineEdit_ventes_recherche->clear();
        lineEdit_ventes_nom_caissier->clear();
        lineEdit_ventes_designation->clear();
        lineEdit_ventes_categorie_produit->clear();
        lineEdit_ventes_nom_entreprise_fournisseur->clear();
        lineEdit_ventes_nom_entreprise_client->clear();
        lineEdit_ventes_reference_recu_vendu->clear();
    }

    QString codebar(lineEdit_ventes_recherche->text());

    if (codebar.isEmpty())
    {
        QString nom_caissier(lineEdit_ventes_nom_caissier->text());

        if (!nom_caissier.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_CAISSIER, nom_caissier));
        }


        QString categorie(lineEdit_ventes_categorie_produit->text());

        if (!categorie.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::CATEGORIE, categorie));
        }


        QString designation(lineEdit_ventes_designation->text());

        if (!designation.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, designation));
        }


        QString nom_entreprise_fournisseur(lineEdit_ventes_nom_entreprise_fournisseur->text());

        if (!nom_entreprise_fournisseur.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, nom_entreprise_fournisseur));
        }


        QString nom_entreprise_client(lineEdit_ventes_nom_entreprise_client->text());

        if (!nom_entreprise_client.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, nom_entreprise_client));
        }

        QString reference_recu_vendu(lineEdit_ventes_reference_recu_vendu->text());

        if (!reference_recu_vendu.isEmpty())
        {
            if (!_searchFilter.isEmpty())
            {
                _searchFilter.append(" AND ");
            }
            _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::REFERENCE_RECU_VENDU, reference_recu_vendu));
        }

        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }

        _searchFilter.append(QString("%1 >= '%2'")
        						.arg(YerothDatabaseTableColumn::DATE_VENTE,
        								DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_debut->date())));

        _searchFilter.append(QString(" AND %1 <= '%2'")
        						.arg(YerothDatabaseTableColumn::DATE_VENTE,
        								DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_fin->date())));
    }
    else
    {
        QString searchCodebar(lineEdit_ventes_recherche->text());

        _searchFilter.append(QString("%1 LIKE '%2%'")
        						.arg(YerothDatabaseTableColumn::REFERENCE,
        							 searchCodebar));
    }

    setFilter();

    _logger->log("rechercher", QString("search filter: %1").arg(_searchFilter));

    if (_curStocksVenduTableModel->easySelect() > 0)
    {
        this->setLastListerSelectedRow(0);
    }
    else
    {
        _logger->log("rechercher",
                     QString("reason for failing: %1").arg(_curStocksVenduTableModel->lastError().text()));
    }

    lister_les_elements_du_tableau();
}
