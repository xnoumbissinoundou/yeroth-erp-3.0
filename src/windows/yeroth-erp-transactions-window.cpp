/*
 * yeroth-erp-transactions-window.cpp
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */
#include "yeroth-erp-transactions-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-utils.hpp"



#include <QtCore/QDir>

#include <QtCore/qmath.h>

#include <QtCore/QFile>

#include <QtCore/QProcessEnvironment>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QDesktopWidget>



const QString YerothTransactionsWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("transactions de stocks")));

YerothTransactionsWindow::YerothTransactionsWindow()
:YerothWindowsCommons(YerothTransactionsWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothTransactionsWindow")),
 _curTransactionsTableModel(0)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_BLUE_77_93_254, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    // Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(imprimer_journal_transactions()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_SERVER
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(tabWidget_transactions, SIGNAL(currentChanged(int)), this, SLOT(handleTabChanged(int)));

    setupShortcuts();
}

void YerothTransactionsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionAfficherPDF		(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

void YerothTransactionsWindow::setFilter()
{
    if (0 == _curTransactionsTableModel)
    {
        _curTransactionsTableModel = &_allWindows->getSqlTableModel_stocks_sorties();
    }

    _curTransactionsTableModel->yerothSetFilter(_searchFilter);
}

void YerothTransactionsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    setupLineEditsQCompleters();

    rechercher();

    this->setVisible(true);
}

void YerothTransactionsWindow::setupLineEdits()
{
    _logger->log("setupLineEdits");

    lineEdit_transactions_quantite_sortie->setEnabled(false);
    lineEdit_transactions_nom_magasinier->enableForSearch(QObject::trUtf8("nom du magasinier"));
    lineEdit_transactions_designation->enableForSearch(QObject::trUtf8("désignation"));
    lineEdit_transactions_nom_categorie->enableForSearch(QObject::trUtf8("nom de la catégorie d'articles"));
    lineEdit_transactions_numero_bon->enableForSearch(QObject::trUtf8("numéro de facture"));
    lineEdit_transactions_nom_recepteur->enableForSearch(QObject::trUtf8("nom du récepteur d'articles"));

    connect(lineEdit_transactions_nom_magasinier, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_transactions_designation, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_transactions_nom_categorie, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_transactions_numero_bon, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
    connect(lineEdit_transactions_nom_recepteur, SIGNAL(textChanged(const QString &)), this, SLOT(rechercher()));
}

void YerothTransactionsWindow::setupLineEditsQCompleters()
{
    lineEdit_transactions_nom_magasinier->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::NOM_MAGASINIER);

    lineEdit_transactions_designation->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::DESIGNATION);

    lineEdit_transactions_nom_categorie->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::CATEGORIE);

    lineEdit_transactions_numero_bon->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::SORTIE_ID);

    lineEdit_transactions_nom_recepteur->
		setupMyStaticQCompleter(_allWindows->STOCKS_SORTIES, YerothDatabaseTableColumn::NOM_RECEPTEUR);
}


void YerothTransactionsWindow::setupDateTimeEdits()
{
    dateEdit_transactions_debut->setStartDate(GET_CURRENT_DATE);
    dateEdit_transactions_fin->setStartDate(GET_CURRENT_DATE);

    connect(dateEdit_transactions_debut, SIGNAL(dateChanged(const QDate &)), this, SLOT(rechercher()));
    connect(dateEdit_transactions_fin, SIGNAL(dateChanged(const QDate &)), this, SLOT(rechercher()));
}


void YerothTransactionsWindow::handleTabChanged(int index)
{
    rechercher();
}

void YerothTransactionsWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}

void YerothTransactionsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}


void YerothTransactionsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}


void YerothTransactionsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothTransactionsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
}

void YerothTransactionsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
}


bool YerothTransactionsWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    int currentTableViewCurrentIndex = -1;

    currentTableViewCurrentIndex = tabWidget_transactions->currentIndex();

    YerothTableView *currentYerothTableView = 0;

    if (0 == currentTableViewCurrentIndex)
    {
    	currentYerothTableView = tableView_sorties_articles;
    }
    else if (1 == currentTableViewCurrentIndex)
    {
    	currentYerothTableView = tableView_transferts_articles;
    }
    else
    {
    	//TODO: insert QDialog message box here
    	return false;
    }

    QStandardItemModel *tableModel = tableView_sorties_articles->getStandardItemModel();

    if (0 == tableModel)
    {
        return false;
    }

    QList < int >columnsToIgnore;

    if (currentYerothTableView == tableView_sorties_articles)
    {

        columnsToIgnore << 0 << 1 << 2
                        << 3 << 4 << 5
                        << 8 << 12 << 13
						<< 14 << 15 << 17
						<< 19 << 21;
    }
    else if (currentYerothTableView == tableView_transferts_articles)
    {
        columnsToIgnore << 0 << 4 << 9
                        << 10 << 11 << 13
                        << 14 << 15 << 16
						<< 19 << 21;

        return false;
    }


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

    yerothStocksListingCSVFileName.append("yeroth-erp-fichier-stock-sortie-format-csv");
    yerothStocksListingCSVFileName = FILE_NAME_USERID_CURRENT_TIME(yerothStocksListingCSVFileName);

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    yerothStocksListingCSVFileName.append("yeroth-erp-stock-transfert-csv-format");
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


void YerothTransactionsWindow::getJournalDesTransactionsTexTableString(QString & texTable_in_out,
        															  QStandardItemModel & tableStandardItemModel,
																	  QList < int >&columnsToIgnore,
																	  int fromRowIndex, int toRowIndex,
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

    int texTableColumnCount = tableStandardItemModel.columnCount() - columnsToIgnore.size();
    //qDebug() << "++ texTableColumnCount: " << texTableColumnCount;
    //Tex table header
    for (int k = 0; k < 9; ++k)
    {
        texTable_in_out.append("r|");
    }
    //we align right the field 'quantie_sortie'
    texTable_in_out.append("r|");
    for (int k = 9; k < texTableColumnCount; ++k)
    {
        texTable_in_out.append("r|");
    }

    texTable_in_out.append("} \\hline \n");
    //qDebug() << "++ texTable caisse: " << texTable_in_out;

    QStandardItem *item;

    int tableColumnCount = 1 + tableStandardItemModel.columnCount();
    /** We add a column named ''id'' for numbering the rows

       * in the Tex table. */
    texTable_in_out.append("\\textbf{id} & ");

    for (int k = 0; k < tableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);

        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{")
            							 .append("}"));

            YerothUtils::handleTexTableItemText(tableStandardItemModel.columnCount(),
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

    int rowToPrintSums = toRowIndex - 1;

    static int b = 1;

    if (0 == fromRowIndex)
    {
        b = 1;
    }

    QString idColumnText;

    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        idColumnText.clear();

        idColumnText.append(QString::number(b))
        			.prepend("\\textbf{")
					.append("}")
					.append(" & ");

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

                           * YerothUtils::STRING_MAX_CHARS

                           */
                QString itemText(item->text());
                if (itemText.length() > YerothUtils::STRING_MAX_CHARS)
                {
                    itemText.truncate(YerothUtils::STRING_MAX_CHARS);
                    itemText.append(".");
                }
                YerothUtils::handleTexTableItemText(tableStandardItemModel.columnCount(), texTable_in_out, k,
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
        if (lastPage && j == rowToPrintSums)
        {
            double quantiteSortie = lineEdit_transactions_quantite_sortie->text().toDouble();

            QString quantiteSortieStr(GET_DOUBLE_STRING(quantiteSortie));

            if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_transactions->currentIndex())
            {
            	texTable_in_out.append("\\multicolumn{").append(QString::number(6)).append("}")
                					   .append("{|c|}")
									   .append("{}")
									   .append(" & ")
									   .append(QString("\\textbf{%1}").arg(quantiteSortieStr))
									   .append(" & ")
									   .append("\\multicolumn{").append(QString::number(2)).append("}").append("{|c|}")
									   .append("{}")
									   .append("\\\\ \\hline \n");
            }
            else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_transactions->currentIndex())
            {
            	texTable_in_out.append("\\multicolumn{").append(QString::number(6)).append("}").append("{|c|}")
                				.append("{}").append(" & ")
								.append(QString("\\textbf{%1}").arg(quantiteSortieStr))
								.append(" & ")
								.append("\\multicolumn{").append(QString::number(3)).append("}").append("{|c|}")
								.append("{}").append("\\\\ \\hline \n");
            }
        }
    }

    //Removes the string "" from Latex output
    texTable_in_out.replace("\"\"", "");

    texTable_in_out.append("\\end{tabular}")
    			   .append("\n")
				   .append("\\end{table*}")
				   .append("\n");
}

void YerothTransactionsWindow::imprimer_journal_transactions()
{
    _logger->log("imprimer_journal_transactions");

    QStandardItemModel *tableModel = 0;

    QList < int >columnsToIgnore;

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_transactions->currentIndex())
    {
        tableModel = tableView_sorties_articles->getStandardItemModel();

        columnsToIgnore << 0 << 1 << 2
        				<< 3 << 4 << 5
						<< 12 << 13 << 14
						<< 15 << 16 << 17
						<< 19 << 21;
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_transactions->currentIndex())
    {
        tableModel = tableView_transferts_articles->getStandardItemModel();

        columnsToIgnore << 0 << 1 << 2
        				<< 3 << 4 << 5
						<< 12 << 13 << 14
						<< 15 << 18 << 19 << 21;
    }

    int tableModelRowCount = tableModel->rowCount();

    if (tableModelRowCount <= 0)
    {
        YerothQMessageBox::information(this,
        							   QObject::trUtf8("impression"),
									   QObject::trUtf8("Il n'y a pas de données à imprimer !"));
        return;
    }

    QString texTable;

    int fromRowIndex = 0;

    int toRowIndex = tableModelRowCount;

    double MAX_TABLE_ROW_COUNT = 35.0;

    int pageNumber = qCeil(tableModelRowCount / MAX_TABLE_ROW_COUNT);

    //qDebug() << QString("number of pages to print: %1").arg(pageNumber);
    //_logger->log("imprimer_document",
    //                  QString("number of pages to print: %1").arg(pageNumber));
    this->getJournalDesTransactionsTexTableString(texTable, *tableModel, columnsToIgnore, 0,
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
            //  .arg(QString::number(fromRowIndex), QString::number(toRowIndex));
            this->getJournalDesTransactionsTexTableString(texTable, *tableModel, columnsToIgnore,
                    (fromRowIndex >=
                     tableModelRowCount) ? tableModelRowCount :
                    fromRowIndex,
                    (toRowIndex >=
                     tableModelRowCount) ? (tableModelRowCount +
                                            1) : toRowIndex,
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

    QString factureDate(YerothUtils::handleForeignAccents(infoEntreprise.getVilleTex()));

    YerothUtils::getCurrentLocaleDate(factureDate);

#ifdef YEROTH_FRANCAIS_LANGUAGE

    YerothUtils::getTexLandscapeFROutgoingDocumentString(texDocument, texTable);

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_transactions->currentIndex())
    {
        texDocument.replace("YEROTHSUBJECT", "Journal des sorties d'articles");
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_transactions->currentIndex())
    {
        texDocument.replace("YEROTHSUBJECT", "Journal des transferts d'articles");
    }

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    YerothUtils::getTexLandscapeENOutgoingDocumentString(texDocument, texTable);

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_transactions->currentIndex())
    {
        texDocument.replace("YEROTHSUBJECT", "Journal of outgoing articles");
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_transactions->currentIndex())
    {
        texDocument.replace("YEROTHSUBJECT", "Journal of article transfers");
    }

#endif

    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", factureDate);
    texDocument.replace("YEROTHVENTESDEBUT", DATE_TO_STRING(dateEdit_transactions_debut->date()));
    texDocument.replace("YEROTHVENTESFIN", DATE_TO_STRING(dateEdit_transactions_fin->date()));
    texDocument.replace("YEROTHNOMUTILISATEUR", _allWindows->getUser()->nom_completTex());
    texDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());

    QString yerothFiltres;

    YerothUtils::addFiltre(lineEdit_transactions_nom_magasinier, QObject::trUtf8("magasinier"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_transactions_designation, QObject::trUtf8("désignation"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_transactions_nom_categorie, QObject::trUtf8("catégorie"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_transactions_numero_bon, QObject::trUtf8("numéro de bon"), yerothFiltres);
    YerothUtils::addFiltre(lineEdit_transactions_nom_recepteur, QObject::trUtf8("récepteur"), yerothFiltres);

    int lastIndexOfComa = yerothFiltres.lastIndexOf(",");

    yerothFiltres.remove(lastIndexOfComa, yerothFiltres.length());

    texDocument.replace("YEROTHFILTRES", YerothUtils::handleForeignAccents(yerothFiltres));

    QString yerothTableauTransactionsFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_transactions->currentIndex())
    {
    	yerothTableauTransactionsFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-journal-sortie-stocks"));
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_transactions->currentIndex())
    {
    	yerothTableauTransactionsFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-journal-transferts-stocks"));
    }

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_transactions->currentIndex())
    {
    	yerothTableauTransactionsFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-journal-outgoing-stocks"));
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_transactions->currentIndex())
    {
    	yerothTableauTransactionsFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-journal-stocks-transfer"));
    }

#endif

    QFile tmpLatexFile(QString("%1tex").arg(yerothTableauTransactionsFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
    											   texDocument);

    QString pdfTableauTransactionsFileName(YerothERPProcess::compileLatex(yerothTableauTransactionsFileName));

    YerothERPProcess::startPdfViewerProcess(pdfTableauTransactionsFileName);
}

void YerothTransactionsWindow::rechercher()
{
    _searchFilter.clear();

    QString nom_magasinier(lineEdit_transactions_nom_magasinier->text());

    if (!nom_magasinier.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT("nom_magasinier", nom_magasinier));
    }

    QString designation(lineEdit_transactions_designation->text());

    if (!designation.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, designation));
    }

    QString nom_categorie(lineEdit_transactions_nom_categorie->text());

    if (!nom_categorie.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::CATEGORIE, nom_categorie));
    }

    QString numero_du_bon(lineEdit_transactions_numero_bon->text());

    if (!numero_du_bon.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT("sortie_id", numero_du_bon));
    }

    QString nom_recepteur(lineEdit_transactions_nom_recepteur->text());

    if (!nom_recepteur.isEmpty())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(" AND ");
        }
        _searchFilter.append(GENERATE_SQL_IS_STMT("nom_recepteur", nom_recepteur));
    }

    this->setFilter();

    _logger->log("rechercher", QString("search filter: %1").arg(_searchFilter));


    if (_curTransactionsTableModel->easySelect() > 0)
    {
        this->setLastListerSelectedRow(0);
    }
    else
    {
        _logger->log("rechercher",
                     QString("reason for failing: %1").arg(_curTransactionsTableModel->lastError().text()));
    }

    lister_les_elements_du_tableau(_searchFilter);
}

void YerothTransactionsWindow::lister_les_elements_du_tableau(QString aSearchFilter)
{
    _logger->log("lister_les_elements_du_tableau");
    _searchFilter = aSearchFilter;
    if (_searchFilter.isEmpty())
    {
        _searchFilter.append(QString(" date_sortie >= '%1' AND date_sortie <= '%2'").
                             arg(DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_debut->date()),
                                 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_fin->date())));
    }
    else
    {
        _searchFilter.append(QString(" AND date_sortie >= '%1' AND date_sortie <= '%2'").
                             arg(DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_debut->date()),
                                 DATE_TO_DB_FORMAT_STRING(dateEdit_transactions_fin->date())));
    }
    if (0 == _curTransactionsTableModel)
    {
        _curTransactionsTableModel = &_allWindows->getSqlTableModel_stocks_sorties();
    }
    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_transactions->currentIndex())
    {
        _searchFilter.append(" AND localisation_entree = ''");
    }
    else
    {
        _searchFilter.append(" AND localisation_entree != ''");
    }
    _curTransactionsTableModel->yerothSetFilter(_searchFilter);
    _logger->log("lister_les_elements_du_tableau", _searchFilter);
    //qDebug() << "++ lister_les_elements_du_tableau, aSearchFilter: " << _curTransactionsTableModel->filter();

    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_transactions->currentIndex())
    {
        tableView_sorties_articles->lister_les_elements_du_tableau(*_curTransactionsTableModel);
        tableView_sorties_articles->hideColumn(0);
        tableView_sorties_articles->hideColumn(1);
        tableView_sorties_articles->hideColumn(2);
        tableView_sorties_articles->hideColumn(3);
        tableView_sorties_articles->hideColumn(4);
        tableView_sorties_articles->hideColumn(5);
        tableView_sorties_articles->hideColumn(12);
        tableView_sorties_articles->hideColumn(13);
        tableView_sorties_articles->hideColumn(14);
        tableView_sorties_articles->hideColumn(15);
        tableView_sorties_articles->hideColumn(17);
        tableView_sorties_articles->hideColumn(19);
        tableView_sorties_articles->hideColumn(21);
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_transactions->currentIndex())
    {
        tableView_transferts_articles->lister_les_elements_du_tableau(*_curTransactionsTableModel);
        tableView_transferts_articles->hideColumn(0);
        tableView_transferts_articles->hideColumn(1);
        tableView_transferts_articles->hideColumn(2);
        tableView_transferts_articles->hideColumn(3);
        tableView_transferts_articles->hideColumn(4);
        tableView_transferts_articles->hideColumn(5);
        tableView_transferts_articles->hideColumn(12);
        tableView_transferts_articles->hideColumn(13);
        tableView_transferts_articles->hideColumn(14);
        tableView_transferts_articles->hideColumn(15);
        tableView_transferts_articles->hideColumn(19);
        tableView_transferts_articles->hideColumn(21);
    }
    int quantite_sortie = 0;
    int quantite_sortie_total = 0;
    int curTransactionsTableModelRowCount = _curTransactionsTableModel->easySelect();
    QSqlRecord aRecord;
    for (int j = 0; j < curTransactionsTableModelRowCount; ++j)
    {
        aRecord = _curTransactionsTableModel->record(j);
        quantite_sortie = GET_SQL_RECORD_DATA(aRecord, "quantite_sortie").toDouble();
        quantite_sortie_total += quantite_sortie;
    }
    lineEdit_transactions_quantite_sortie->setText(QString::number(quantite_sortie_total, 'f', 2));
}

void YerothTransactionsWindow::resetFilter()
{
    _searchFilter.clear();

    if (0 != this->_curTransactionsTableModel)
    {
        this->_curTransactionsTableModel->resetFilter();
    }
    else
    {
        this->_curTransactionsTableModel = &_allWindows->getSqlTableModel_stocks_sorties();
    }

    lineEdit_transactions_nom_magasinier->clear();
    lineEdit_transactions_designation->clear();
    lineEdit_transactions_nom_categorie->clear();
    lineEdit_transactions_numero_bon->clear();
    lineEdit_transactions_nom_recepteur->clear();

    if (_allWindows->getUser()->isManager() || _allWindows->getUser()->isMagasinier())
    {
        dateEdit_transactions_debut->reset();

        dateEdit_transactions_fin->reset();

        lister_les_elements_du_tableau(QString(""));
    }
}

void YerothTransactionsWindow::setLastListerSelectedRow(int row)
{
    if (SUJET_ACTION_SORTIES_STOCKS == tabWidget_transactions->currentIndex())
    {
        tableView_sorties_articles->setLastSelectedRow(row);
    }
    else if (SUJET_ACTION_TRANSFERTS_STOCKS == tabWidget_transactions->currentIndex())
    {
        tableView_transferts_articles->setLastSelectedRow(row);
    }
}

void YerothTransactionsWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");
    resetFilter();
}
