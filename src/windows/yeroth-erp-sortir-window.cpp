/*
 * yeroth-erp-sortir-window.cpp
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-sortir-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/windows/yeroth-erp-stock-detail-window.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-info-entreprise.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include <QtCore/QThread>

#include <QtCore/QFile>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QCompleter>

#include <QtWidgets/QCalendarWidget>

bool YerothSortirWindow::_qteChangeCodeBar(false);


const QString YerothSortirWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("sortir ou transférer des stocks")));

YerothSortirWindow::YerothSortirWindow()
:YerothWindowsCommons(YerothSortirWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothSortirWindow")),
 _currentFocusSearchBar(0),
 _linuxWhich(0),
 _updateItemConversionError(false),
 _previousPressedQteValue("1"),
 _tvaCheckBoxPreviousState(false),
 _sommeTotal(0.0),
 _tva(0.0),
 _quantiteVendue(0),
 _currentTabView(0),
 actionRechercheArticle(0),
 actionRechercheArticleCodebar(0)
{
    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);

    tableWidget_articles->resizeColumnsToContents();

    _currentFocusSearchBar = INITIAL_FOCUS_SEARCHBAR_SORTIR_WINDOW;

    setRechercheLineEditFocus();

    checkBox_lecteur_de_code_barres->setChecked(false);

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_minus->enable(this, SLOT(enlever_article()));

    // Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(afficher_facture_pdf()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(annuler()));

    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));

    connect(tableWidget_articles, SIGNAL(itemPressed(QTableWidgetItem *)), this,
            SLOT(handleQtePressed(QTableWidgetItem *)));

    connect(tableWidget_articles, SIGNAL(itemChanged(QTableWidgetItem *)), this,
            SLOT(handleQteChange(QTableWidgetItem *)));

    connect(tabWidget_sorties, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));

    connect(tableWidget_articles, SIGNAL(addedYerothTableWidget()), this, SLOT(handleAddedArticle()));

    connect(tableWidget_articles, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this,
            SLOT(afficher_sortie_detail(QTableWidgetItem *)));

    connect(checkBox_lecteur_de_code_barres, SIGNAL(stateChanged(int)),
    		this, SLOT(handleBarCodeScannerCheckBox(int)));

    /* Signals-slots connection for the second tab 'Article au détail' */
    connect(checkBox_tva, SIGNAL(clicked(bool)), this, SLOT(handleTVACheckBox(bool)));

    connect(lineEdit_article_detail_quantite_a_vendre, SIGNAL(textEdited(const QString &)), this,
            SLOT(updateQuantiteAVendre()));

    connect(lineEdit_article_detail_remise_prix, SIGNAL(textChanged(const QString)), this,
            SLOT(calculate_details_window_remise_prix()));

    connect(lineEdit_article_detail_remise_pourcentage, SIGNAL(textChanged(const QString)), this,
            SLOT(calculate_details_window_remise_pourcentage()));

    connect(radioButton_article_detail_remise_prix, SIGNAL(toggled(bool)), this,
            SLOT(activateLineEditRemisePrix(bool)));

    connect(radioButton_article_detail_remise_pourcentage, SIGNAL(toggled(bool)), this,
            SLOT(activateLineEditRemisePourcentage(bool)));

    setupShortcuts();
}


YerothSortirWindow::~YerothSortirWindow()
{
    delete _logger;
    delete actionRechercheArticle;
    delete actionRechercheArticleCodebar;
    deleteArticleVenteInfos();
}


void YerothSortirWindow::deleteArticleVenteInfos()
{
    QMapIterator<int, YerothArticleVenteInfo *> itArticleItemVenteInfo(articleItemToVenteInfo);

    YerothArticleVenteInfo *v = 0;

    while (itArticleItemVenteInfo.hasNext())
    {
        itArticleItemVenteInfo.next();

        v = itArticleItemVenteInfo.value();

        if (0 != v)
        {
            delete v;
        }
    }

    articleItemToVenteInfo.clear();
}


void YerothSortirWindow::handleBarCodeScannerCheckBox(int state)
{
	if (checkBox_lecteur_de_code_barres->isChecked())
	{
		connect_barcode_reader_selection_of_article_item();
	}
	else
	{
		connect_manual_selection_of_article_item();
	}
}


void YerothSortirWindow::disconnect_all_objects_for_stock_article_item_selection()
{
	disconnect(lineEdit_recherche_article, 0, 0, 0);
	disconnect(lineEdit_recherche_article_codebar, 0, 0, 0);
	disconnect(lineEdit_recherche_article->getMyQCompleter(), 0, 0, 0);
	disconnect(lineEdit_recherche_article_codebar->getMyQCompleter(), 0, 0, 0);
}


void YerothSortirWindow::connect_manual_selection_of_article_item()
{
	disconnect_all_objects_for_stock_article_item_selection();

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
    										YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
    											 YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
    											 YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else			//ALL == YerothConfig::salesStrategy
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
}


void YerothSortirWindow::connect_barcode_reader_selection_of_article_item()
{
	disconnect_all_objects_for_stock_article_item_selection();

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
    										YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar, SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
    											 YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar, SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
    											 YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar, SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else			//ALL == YerothConfig::salesStrategy
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar, SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
}


bool YerothSortirWindow::check_fields()
{
    bool nom_recepteur = lineEdit_articles_nom_recepteur->checkField();
    if (false == nom_recepteur)
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("nom du récepteur de(s) stock(s) sortant"),
                                  QObject::trUtf8("Vous n'avez pas saisi le nom"
                                          " du récepteur de(s) stock(s) sortant !"));
        return false;
    }
    QString currentLocalisation(_allWindows->getInfoEntreprise().getLocalisation());
    bool destinationIsCorrect =
        !YerothUtils::isEqualCaseInsensitive(currentLocalisation, lineEdit_articles_transfert->text());
    if (!destinationIsCorrect)
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("localisation"),
                                  QObject::trUtf8("La localisation d'entrée doit"
                                          " être différente de la localisation de sortie !"));
        return false;
    }
    return true;
}

void YerothSortirWindow::clear_all_fields()
{
    tableWidget_articles->myClear();
    lineEdit_recherche_article->myClear();
    lineEdit_recherche_article_codebar->myClear();
    lineEdit_articles_quantite_a_transferer->clear();
    lineEdit_articles_quantite_a_transferer->setText(GET_DOUBLE_STRING(0.0));
    lineEdit_articles_transfert->clear();
    textEdit_articles_notes->clear();
    lineEdit_articles_nom_recepteur->clearField();
}


void YerothSortirWindow::setupLineEditsQCompleters()
{
    lineEdit_articles_transfert
		->setupMyStaticQCompleter(_allWindows->LOCALISATIONS, YerothDatabaseTableColumn::NOM_LOCALISATION);

    lineEdit_articles_nom_client
		->setupMyStaticQCompleter(_allWindows->CLIENTS, YerothDatabaseTableColumn::NOM_ENTREPRISE, true);

    {
        YerothPOSUser *user = _allWindows->getUser();
        if (user && user->isManager())
        {
            lineEdit_articles_nom_client
				->setupMyStaticQCompleter(_allWindows->CLIENTS, YerothDatabaseTableColumn::NOM_ENTREPRISE, true);
        }
        else
        {
            lineEdit_articles_nom_client
				->setupMyStaticQCompleter(_allWindows->CLIENTS, YerothDatabaseTableColumn::NOM_ENTREPRISE);
        }
    }
}


void YerothSortirWindow::setupLineEdits()
{
    lineEdit_recherche_article_codebar->enableForSearch(QObject::trUtf8("référence [ focus avec F11 ]"));
    lineEdit_recherche_article->enableForSearch(QObject::trUtf8("désignation [ focus avec F12 ]"));

    lineEdit_articles_imprimante->setText(YerothERPConfig::printer);
    lineEdit_articles_imprimante->setReadOnly(true);

    lineEdit_article_detail_nom_client->setYerothEnabled(false);
    lineEdit_article_detail_reference_produit->setYerothEnabled(false);
    lineEdit_article_detail_designation->setYerothEnabled(false);
    lineEdit_article_detail_nom_entreprise_fournisseur->setYerothEnabled(false);
    lineEdit_article_detail_categorie->setYerothEnabled(false);
    lineEdit_article_detail_prix_unitaire->setYerothEnabled(false);
    lineEdit_article_detail_tva->setYerothEnabled(false);
    lineEdit_article_detail_destination->setYerothEnabled(false);
    lineEdit_article_detail_nom_magasinier->setYerothEnabled(false);
    lineEdit_article_detail_nom_recepteur->setYerothEnabled(false);
    lineEdit_article_detail_quantite_en_stock->setYerothEnabled(false);
    lineEdit_articles_nom_magasinier->setYerothEnabled(false);
    lineEdit_articles_quantite_a_transferer->setYerothEnabled(false);
    lineEdit_articles_quantite_a_transferer->setText(GET_DOUBLE_STRING(0.0));
    //qDebug() << "++ YerothSortirWindow::setupLineEdits(): " << YerothConfig::salesStrategy;
    connect(lineEdit_articles_transfert, SIGNAL(textChanged(const QString &)), this, SLOT(handleClient()));
    connect(lineEdit_articles_nom_client, SIGNAL(textChanged(const QString &)), this, SLOT(handleDestination()));
    lineEdit_article_detail_quantite_a_vendre->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_article_detail_remise_prix->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_article_detail_remise_pourcentage->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_article_detail_remise_prix->setYerothEnabled(false);
    lineEdit_article_detail_remise_pourcentage->setYerothEnabled(false);
}

void YerothSortirWindow::hideEvent(QHideEvent * hideEvent)
{
    _allWindows->_sortirListStocksWindow->close();
}

void YerothSortirWindow::setupShortcuts()
{
    actionRechercheArticle = new QAction(this);
    actionRechercheArticleCodebar = new QAction(this);
    lineEdit_recherche_article->addAction(actionRechercheArticle);
    lineEdit_recherche_article_codebar->addAction(actionRechercheArticleCodebar);
    connect(actionRechercheArticle, SIGNAL(triggered()), this, SLOT(setRechercheDesignationArticleFocus()));
    connect(actionRechercheArticleCodebar, SIGNAL(triggered()), this, SLOT(setRechercheCodebarArticleFocus()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));

    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionAfficherPDF	(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);

    actionRechercheArticle->setShortcut(Qt::Key_F12);
    actionRechercheArticleCodebar->setShortcut(Qt::Key_F11);
    actionStocks->setShortcut(YerothUtils::LISTER_STOCKS_QKEYSEQUENCE);
}

void YerothSortirWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_article_detail_retour_sorties->disable(this);
    pushButton_annuler->disable(this);
    pushButton_sortir->disable(this);
}

void YerothSortirWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_article_detail_retour_sorties->enable(this, SLOT(retourSorties()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}


void YerothSortirWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_article_detail_retour_sorties->disable(this);
    pushButton_annuler->disable(this);
    pushButton_sortir->disable(this);
}


void YerothSortirWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_article_detail_retour_sorties->enable(this, SLOT(retourSorties()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}

void YerothSortirWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_article_detail_retour_sorties->enable(this, SLOT(retourSorties()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}

void YerothSortirWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_article_detail_retour_sorties->disable(this);
    pushButton_annuler->disable(this);
    pushButton_sortir->disable(this);
}


QString YerothSortirWindow::afficher_facture_pdf(QString referenceRecuSORTIE /* = QString("") */)
{
	QString pdfReceiptFileName;

    if (YerothERPConfig::RECEIPT_FORMAT_PETIT == YerothERPConfig::receiptFormat)
    {
    	pdfReceiptFileName.append(imprimer_facture_petit(referenceRecuSORTIE));
    }
    else
    {
    	pdfReceiptFileName.append(imprimer_facture_grand(referenceRecuSORTIE));
    }

    if (! pdfReceiptFileName.isEmpty())
    {
    	return YerothERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    }

    return "";
}

QString YerothSortirWindow::imprimer_facture(QString referenceRecuSORTIE /* = QString("") */)
{
	QString pdfReceiptFileName;

    if (YerothERPConfig::RECEIPT_FORMAT_PETIT == YerothERPConfig::receiptFormat)
    {
    	pdfReceiptFileName.append(imprimer_facture_petit(referenceRecuSORTIE));

    	if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::printer, YerothUtils::IMPRIMANTE_PDF))
    	{
    		return YerothERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    	}
    }
    else
    {
    	pdfReceiptFileName.append(imprimer_facture_grand(referenceRecuSORTIE));

    	return YerothERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    }

    if (false == YerothUtils::isEqualCaseInsensitive(YerothUtils::IMPRIMANTE_PDF, YerothERPConfig::printer))
    {
    	QString thermalPrinterTxtFileEnding("yeroth-erp-3-0-txt");

    	QString pdfReceiptFileName_txt( QString("%1-%2")
    			.arg(pdfReceiptFileName,
    					thermalPrinterTxtFileEnding));

    	QStringList progOptions;

    	progOptions << "-layout";
    	progOptions << pdfReceiptFileName;
    	progOptions << pdfReceiptFileName_txt;

    	if (YerothERPProcess::startAndWaitForFinished("/usr/bin/pdftotext",
    			progOptions,
				YerothUtils::PRINT_TIMEOUT))
    	{
    		QThread::sleep(0.5);

    		progOptions.clear();

    		progOptions << "-c";
    		progOptions << QString("/bin/echo -e \"\n\n\n\n\n\n\n\n\n\n\" >> %1")
    							 .arg(pdfReceiptFileName_txt);

    		YerothERPProcess::startAndWaitForFinished("/bin/bash",
    												  progOptions,
													  YerothUtils::PRINT_TIMEOUT);

    		QThread::sleep(0.1);

    		progOptions.clear();

    		progOptions << "-c";
    		progOptions << QString("/bin/cat %1 >> %2")
    				    			.arg(pdfReceiptFileName_txt,
    				    				 YerothERPConfig::pathToThermalPrinterDeviceFile);

    		YerothERPProcess::startAndWaitForFinished("/bin/bash",
    												  progOptions,
													  YerothUtils::PRINT_TIMEOUT);

    		return pdfReceiptFileName;
    	}
    }

    return "";
}


QString YerothSortirWindow::imprimer_facture_grand(QString referenceRecuSortieGRAND /* = QString("") */)
{

	_logger->log("imprimer_facture_grand");

	QString factureTexTable;

    if (tableWidget_articles->rowCount() <= 0)
    {
        YerothQMessageBox::information(this,
                                      QObject::trUtf8("sortir des stocks - pas de données !"),
                                      QObject::trUtf8("Il n'y a pas de données à imprimer !"));
        return "";
    }

    YerothUtils::getFactureTexTableString(factureTexTable,
    									 *tableWidget_articles,
										 _quantiteVendue,
										 _tva,
                                         _sommeTotal);

    YerothPOSUser *yerothUser = _allWindows->getUser();

    YerothInfoEntreprise & infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVilleTex());

    YerothUtils::getCurrentLocaleDate(factureDate);

#ifdef YEROTH_FRANCAIS_LANGUAGE
    YerothUtils::getSortieDesStocksFRTexDocumentString(factureTexDocument, factureTexTable);
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    YerothUtils::getSortieDesStocksENTexDocumentString(factureTexDocument, factureTexTable);
#endif

    if (referenceRecuSortieGRAND.isEmpty())
    {
    	factureTexDocument.replace("YEROTHNUMEROSORTIETRANSFERT", QObject::tr("EXEMPLE (*NON VALIDE*)"));
    }
    else
    {
        factureTexDocument.replace("YEROTHNUMEROSORTIETRANSFERT",
                                   YerothUtils::LATEX_IN_OUT_handleForeignAccents(referenceRecuSortieGRAND));
    }


    QString destinationTex(YerothUtils::LATEX_IN_OUT_handleForeignAccents(lineEdit_articles_transfert->text()));

    if (YerothUtils::
            isEqualCaseInsensitive(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER, YerothERPConfig::receiptFormat))
    {
        factureTexDocument.replace("YEROTHPAPERSPEC", "a4paper");
    }

    QString recepteur(lineEdit_articles_nom_recepteur->text());

    factureTexDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    factureTexDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YEROTHLOCALISATION_SORTIE", infoEntreprise.getLocalisationTex());
    factureTexDocument.replace("YEROTHLOCALISATION_ENTREE", destinationTex);
    factureTexDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    factureTexDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    factureTexDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
    factureTexDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    factureTexDocument.replace("YEROTHDATE", factureDate);
    factureTexDocument.replace("YEROTHHEUREVENTE", CURRENT_TIME);
    factureTexDocument.replace("YEROTHMAGASINIER", yerothUser->nom_completTex());
    factureTexDocument.replace("YEROTHRECEPTEUR", YerothUtils::LATEX_IN_OUT_handleForeignAccents(recepteur));
    factureTexDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    factureTexDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());
    factureTexDocument.replace("YEROTHSOMME",
                               YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(_sommeTotal)));

    QString nom_client(lineEdit_articles_nom_client->text());

    QString nom_succursale(lineEdit_articles_transfert->text());

    bool isClient = true;

    if (nom_client.isEmpty())
    {
        if (!nom_succursale.isEmpty())
        {
            isClient = false;
            handle_info_succursale(factureTexDocument, nom_succursale);
        }
        else
        {
            nom_client = "Client \"DIVERS\"";
            factureTexDocument.replace("YEROTHCLIENT", YerothUtils::LATEX_IN_OUT_handleForeignAccents(nom_client));
            factureTexDocument.replace("CLIENTYEROTHPOBOX", "");
            factureTexDocument.replace("CLIENTYEROTHCITY", "");
            factureTexDocument.replace("CLIENTYEROTHMAIL", "");
            factureTexDocument.replace("CLIENTYEROTHPHONE", "");
        }
    }
    else
    {
        handle_info_client(factureTexDocument, nom_client);
    }

    //qDebug() << "++\n" << factureTexDocument;

    QString prefixFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    if (isClient)
    {
        factureTexDocument.replace("YEROTHSORTIETRANSFERT", "Sortie");
        prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-stock-sortie-grand"));
    }
    else
    {
        factureTexDocument.replace("YEROTHSORTIETRANSFERT", "Transfert");
        prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-stock-transfert-grand"));
    }

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    if (isClient)
    {
        factureTexDocument.replace("YEROTHSORTIETRANSFERT", "Outgoing");
        prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-stock-outgoing-big"));
    }
    else
    {
        factureTexDocument.replace("YEROTHSORTIETRANSFERT", "Transfer");
        prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-stock-transfer-big"));
    }

#endif

    QFile tmpLatexFile(prefixFileName + "tex");

    if (tmpLatexFile.open(QFile::WriteOnly))
    {
        tmpLatexFile.write(factureTexDocument.toUtf8());
    }

    tmpLatexFile.close();

    return YerothERPProcess::compileLatex(prefixFileName);
}

QString YerothSortirWindow::imprimer_facture_petit(QString referenceRecuSortiePETIT /* = QString("") */)
{
    _logger->log("imprimer_facture_petit");

    QString factureTexTable;

    if (tableWidget_articles->rowCount() <= 0)
    {
        YerothQMessageBox::information(this,
                                      QObject::trUtf8("impression"),
                                      QObject::trUtf8("Il n'y a pas de données à imprimer !"));
        return "";
    }

    YerothPOSUser *yerothUser = _allWindows->getUser();

    YerothInfoEntreprise & infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVilleTex());

    YerothUtils::getCurrentSimplifiedDate(factureDate);

    YerothUtils::getFactureSmallTexTableString(factureTexTable, *tableWidget_articles, _quantiteVendue, _tva,
            _sommeTotal);

#ifdef YEROTH_FRANCAIS_LANGUAGE

    factureTexTable.append(QString("MERCI!\\"));
    YerothUtils::getSortieDesStocksSmallFRTexDocumentString(factureTexDocument, factureTexTable);

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    factureTexTable.append(QString("Thank you!\\"));
    YerothUtils::getSortieDesStocksSmallENTexDocumentString(factureTexDocument, factureTexTable);

#endif

    if (referenceRecuSortiePETIT.isEmpty())
    {
    	factureTexDocument.replace("YEROTHNUMEROSORTIETRANSFERT", QObject::tr("EXEMPLE (*NON VALIDE*)"));
    }
    else
    {
        factureTexDocument.replace("YEROTHNUMEROSORTIETRANSFERT",
                                   YerothUtils::LATEX_IN_OUT_handleForeignAccents(referenceRecuSortiePETIT));
    }

    //_logger->debug("imprimer_facture_petit",
    //QString("document: %1").arg(factureTexDocument));

    static const double INITIAL_INCH_SIZE = 4.05;

    static const double ITEM_INCH_SIZE = 0.2;

    int articleItemCount = tableWidget_articles->rowCount();

    double factureInchSize = INITIAL_INCH_SIZE + (articleItemCount * ITEM_INCH_SIZE);

    QString minPaperHeight(QString("%1in").arg(QString::number(factureInchSize, 'f', 2)));
    //qDebug() << "\t++minPaperHeight: " << minPaperHeight;

    factureTexDocument.replace("YEROTHFACTURESMALLPAPERHEIGHT", minPaperHeight);

    QString destinationTex(YerothUtils::LATEX_IN_OUT_handleForeignAccents(lineEdit_articles_transfert->text()));

    factureTexDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    factureTexDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YEROTHLOCALISATION_SORTIE", infoEntreprise.getLocalisationTex());
    factureTexDocument.replace("YEROTHLOCALISATION_ENTREE", destinationTex);
    factureTexDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    factureTexDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    factureTexDocument.replace("YEROTHDATE", factureDate);
    factureTexDocument.replace("YEROTHHEUREVENTE", CURRENT_TIME);
    factureTexDocument.replace("YEROTHMAGASINIER", yerothUser->nom_completTex());
    factureTexDocument.replace("YEROTHRECEPTEUR", lineEdit_articles_nom_recepteur->text());

    QString nom_client(lineEdit_articles_nom_client->text());

    QString nom_succursale(lineEdit_articles_transfert->text());

    bool isClient = true;

    if (nom_client.isEmpty())
    {
        if (!nom_succursale.isEmpty())
        {
            isClient = false;
        }
        else
        {
            nom_client.append("Client \"DIVERS\"");
            factureTexDocument.replace("YEROTHCLIENT", YerothUtils::LATEX_IN_OUT_handleForeignAccents(nom_client));
            factureTexDocument.replace("CLIENTYEROTHPOBOX", "");
            factureTexDocument.replace("CLIENTYEROTHCITY", "");
            factureTexDocument.replace("CLIENTYEROTHMAIL", "");
            factureTexDocument.replace("CLIENTYEROTHPHONE", "");
        }
    }
    else
    {
        handle_info_client(factureTexDocument, nom_client);
    }

    //qDebug() << "++\n" << factureTexDocument;

    QString prefixFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    if (isClient)
    {
        factureTexDocument.replace("YEROTHSORTIETRANSFERT", "Sortie");
        prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-stock-sortie-petit"));
    }
    else
    {
        factureTexDocument.replace("YEROTHSORTIETRANSFERT", "Transfert");
        prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-stock-transfert-petit"));
    }

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    if (isClient)
    {
        factureTexDocument.replace("YEROTHSORTIETRANSFERT", "Outgoing");
        prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-stock-outgoing-small"));
    }
    else
    {
        factureTexDocument.replace("YEROTHSORTIETRANSFERT", "Transfer");
        prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-stock-transfer-small"));
    }

#endif


    QFile tmpLatexFile(QString("%1tex")
    					.arg(prefixFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
    												   factureTexDocument);

    return YerothERPProcess::compileLatex(prefixFileName);
}


void YerothSortirWindow::handle_info_succursale(QString & factureTexDocument, const QString nom_succursale)
{
    QString succursale;
    succursale.append("Succursale de ").append("\"").append(nom_succursale).append("\"");
    YerothSqlTableModel & localisationTableModel = _allWindows->getSqlTableModel_localisations();
    localisationTableModel.yerothSetFilter(QString("nom_localisation = '%1'").arg(nom_succursale));
    //qDebug() << "++ nom_succursale: " << nom_succursale;
    factureTexDocument.replace("YEROTHCLIENT", YerothUtils::LATEX_IN_OUT_handleForeignAccents(succursale));
    if (localisationTableModel.easySelect() > 0)
    {
        QSqlRecord record = localisationTableModel.record(0);
        QString clientVille(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
        QString clientPOBox(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
        QString clientEmail(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
        QString clientTel(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
        if (clientTel.isEmpty())
        {
            clientTel.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
        }
        if (!clientVille.isEmpty())
            factureTexDocument.replace("CLIENTYEROTHCITY", YerothUtils::LATEX_IN_OUT_handleForeignAccents(clientVille));
        else
            factureTexDocument.replace("CLIENTYEROTHCITY", "");
        if (!clientPOBox.isEmpty())
            factureTexDocument.replace("CLIENTYEROTHPOBOX", YerothUtils::LATEX_IN_OUT_handleForeignAccents(clientPOBox));
        else
            factureTexDocument.replace("CLIENTYEROTHPOBOX", "N/a");
        if (!clientEmail.isEmpty())
            factureTexDocument.replace("CLIENTYEROTHMAIL", YerothUtils::LATEX_IN_OUT_handleForeignAccents(clientEmail));
        else
            factureTexDocument.replace("CLIENTYEROTHMAIL", "");
        if (!clientTel.isEmpty())
            factureTexDocument.replace("CLIENTYEROTHPHONE", YerothUtils::LATEX_IN_OUT_handleForeignAccents(clientTel));
        else
            factureTexDocument.replace("CLIENTYEROTHPHONE", "");
    }
    localisationTableModel.resetFilter();
}

void YerothSortirWindow::handle_info_client(QString & factureTexDocument, const QString nom_client)
{
    YerothSqlTableModel & clientTableModel = _allWindows->getSqlTableModel_clients();
    clientTableModel.yerothSetFilter(QString("nom_entreprise = '%1'").arg(nom_client));
    //qDebug() << "++ nom_client: " << nom_client;
    factureTexDocument.replace("YEROTHCLIENT", YerothUtils::LATEX_IN_OUT_handleForeignAccents(nom_client));
    if (clientTableModel.easySelect() > 0)
    {
        QSqlRecord record = clientTableModel.record(0);
        QString clientVille(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
        QString clientPOBox(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
        QString clientEmail(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
        QString clientTel(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
        if (clientTel.isEmpty())
        {
            clientTel.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
        }
        if (!clientVille.isEmpty())
            factureTexDocument.replace("CLIENTYEROTHCITY", YerothUtils::LATEX_IN_OUT_handleForeignAccents(clientVille));
        else
            factureTexDocument.replace("CLIENTYEROTHCITY", "");
        if (!clientPOBox.isEmpty())
            factureTexDocument.replace("CLIENTYEROTHPOBOX", YerothUtils::LATEX_IN_OUT_handleForeignAccents(clientPOBox));
        else
            factureTexDocument.replace("CLIENTYEROTHPOBOX", "N/a");
        if (!clientEmail.isEmpty())
            factureTexDocument.replace("CLIENTYEROTHMAIL", YerothUtils::LATEX_IN_OUT_handleForeignAccents(clientEmail));
        else
            factureTexDocument.replace("CLIENTYEROTHMAIL", "");
        if (!clientTel.isEmpty())
            factureTexDocument.replace("CLIENTYEROTHPHONE", YerothUtils::LATEX_IN_OUT_handleForeignAccents(clientTel));
        else
            factureTexDocument.replace("CLIENTYEROTHPHONE", "");
    }
    clientTableModel.resetFilter();
}


void YerothSortirWindow::annuler()
{
    if (tableWidget_articles->rowCount() > 0)
    {
        clear_all_fields();

        deleteArticleVenteInfos();

        setRechercheLineEditFocus();

        handleTabViews();

        YerothQMessageBox::information(this, QObject::trUtf8("annulation de sortie"),
                                      QObject::trUtf8("Vous avez annulé la sortie !"), QMessageBox::Ok);
    }
}

void YerothSortirWindow::lister()
{
    _logger->log("lister");
    _allWindows->_sortirListStocksWindow->listStocks(*_curStocksTableModel);
}

void YerothSortirWindow::retourSorties()
{
    _currentTabView = TableauDesSorties;
    tabWidget_sorties->setCurrentIndex(TableauDesSorties);
}

void YerothSortirWindow::handleAddedArticle()
{
    //_logger->debug("handleAddedArticle()");
    tabWidget_sorties->setTabEnabled(AfficherSortieAuDetail, true);
}


void YerothSortirWindow::handleCurrentChanged(int index)
{
    //_logger->debug("handleCurrentChanged(int)", QString("index: %1").arg(index));
    _currentTabView = index;
    switch (index)
    {
    case TableauDesSorties:
        afficher_tableau_sorties(tableWidget_articles->currentRow());
        enableImprimer();
        break;
    case AfficherSortieAuDetail:
        afficher_sortie_detail(tableWidget_articles->currentRow());
        disableImprimer();
        _logger->debug("handleCurrentChanged(int)",
                       QString("current table row: %1").arg(tableWidget_articles->currentRow()));
        break;
    default:
        break;
    }
}

void YerothSortirWindow::cleanUpAfterVente()
{
    _logger->log("cleanUpAfterVente");

    resetCheckboxTVA();

    tableWidget_articles->myClear();

    articleItemToVenteInfo.clear();

    deleteArticleVenteInfos();

    lineEdit_recherche_article->myClear();
    lineEdit_recherche_article_codebar->myClear();

    tabWidget_sorties->setCurrentIndex(TableauDesSorties);

    handleTabViews();

    lineEdit_articles_quantite_a_transferer->clear();
    lineEdit_articles_transfert->clear();
    lineEdit_articles_nom_client->clear();
    textEdit_articles_notes->clear();
    lineEdit_articles_nom_recepteur->clear();
    lineEdit_article_detail_reference_produit->clear();
    lineEdit_article_detail_designation->clear();
    lineEdit_article_detail_nom_entreprise_fournisseur->clear();
    lineEdit_article_detail_categorie->clear();
    lineEdit_article_detail_prix_unitaire->clear();
    lineEdit_article_detail_tva->clear();
    lineEdit_article_detail_destination->clear();
    lineEdit_article_detail_nom_magasinier->clear();
    lineEdit_article_detail_nom_magasinier->clear();
    lineEdit_article_detail_quantite_en_stock->clear();
    lineEdit_article_detail_quantite_a_vendre->clear();
    lineEdit_article_detail_remise_prix->clear();
    lineEdit_article_detail_remise_pourcentage->clear();

    radioButton_article_detail_remise_prix->clearFocus();
    radioButton_article_detail_remise_pourcentage->clearFocus();

    lineEdit_article_detail_quantite_a_vendre->setFocus();

    YerothUtils::refreshSalesStrategy(*_curStocksTableModel,
    								 lineEdit_recherche_article,
                                     lineEdit_recherche_article_codebar);

    if (isBarCodeReaderSelectionOfArticleItem())
    {
    	connect_barcode_reader_selection_of_article_item();
    }
    else
    {
    	connect_manual_selection_of_article_item();
    }

    setRechercheLineEditFocus();
}

void YerothSortirWindow::rendreInvisible()
{
    _logger->log("rendreInvisible");
    _qteChangeCodeBar = false;
    tableWidget_articles->myClear();
    articleItemToVenteInfo.clear();
    deleteArticleVenteInfos();
    lineEdit_recherche_article->clear();
    lineEdit_recherche_article_codebar->clear();
    lineEdit_articles_quantite_a_transferer->clear();
    lineEdit_articles_transfert->clear();
    lineEdit_articles_nom_client->clear();
    lineEdit_articles_nom_recepteur->clear();
    textEdit_articles_notes->clear();
    YerothWindowsCommons::rendreInvisible();
}


void YerothSortirWindow::handleTabViews()
{
    if (0 == tableWidget_articles->rowCount())
    {
        tabWidget_sorties->setTabEnabled(AfficherSortieAuDetail, false);
    }
}


void YerothSortirWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    _curStocksTableModel = stocksTableModel;

    lineEdit_articles_imprimante->setText(YerothERPConfig::printer);

    setupLineEditsQCompleters();

    tabWidget_sorties->setTabEnabled(AfficherSortieAuDetail, false);

    if (!_curClientName.isEmpty())
    {
        lineEdit_articles_transfert->setText(_curClientName);
        _curClientName.clear();
    }
    else
    {
        lineEdit_articles_transfert->clear();
    }

    resetCheckboxTVA();

    tabWidget_sorties->setCurrentIndex(TableauDesSorties);

    lineEdit_articles_nom_magasinier->setText(_allWindows->getUser()->nom_complet());

    YerothUtils::refreshSalesStrategy(*_curStocksTableModel,
    								 lineEdit_recherche_article,
                                     lineEdit_recherche_article_codebar);

    if (isBarCodeReaderSelectionOfArticleItem())
    {
    	connect_barcode_reader_selection_of_article_item();
    }
    else
    {
    	connect_manual_selection_of_article_item();
    }

    lineEdit_articles_transfert->setupMyStaticQCompleter(_allWindows->LOCALISATIONS, YerothDatabaseTableColumn::NOM_LOCALISATION);

    handleTabViews();

    setRechercheLineEditFocus();

    QString aNewWindowTitle;

    aNewWindowTitle = YerothUtils::getWindowTitleWithStrategy(this);
    aNewWindowTitle = YerothUtils::appendPDFReceiptFormat(aNewWindowTitle);

    setWindowTitle(aNewWindowTitle);
    setVisible(true);
}

void YerothSortirWindow::setRechercheLineEditFocus()
{
    lineEdit_recherche_article->clearQCompleterText();
    lineEdit_recherche_article_codebar->clearQCompleterText();

    if (_currentFocusSearchBar == lineEdit_recherche_article)
    {
        lineEdit_recherche_article_codebar->setFocus();
        lineEdit_recherche_article->setFocus();
    }
    else
    {
        lineEdit_recherche_article->setFocus();
        lineEdit_recherche_article_codebar->setFocus();
    }
}

void YerothSortirWindow::setRechercheDesignationArticleFocus()
{
    _currentFocusSearchBar = lineEdit_recherche_article;
    setRechercheLineEditFocus();
}

void YerothSortirWindow::setRechercheCodebarArticleFocus()
{
    _currentFocusSearchBar = lineEdit_recherche_article_codebar;
    setRechercheLineEditFocus();
}

void YerothSortirWindow::activateLineEditRemisePrix(bool toggled)
{
    if (toggled)
    {
        lineEdit_article_detail_remise_prix->setYerothEnabled(true);
    }
    else
    {
        lineEdit_article_detail_remise_prix->setYerothEnabled(false);
    }
}

void YerothSortirWindow::activateLineEditRemisePourcentage(bool toggled)
{
    if (toggled)
    {
        lineEdit_article_detail_remise_pourcentage->setYerothEnabled(true);
    }
    else
    {
        lineEdit_article_detail_remise_pourcentage->setYerothEnabled(false);
    }
}


void YerothSortirWindow::calculate_details_window_remise_prix()
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);

    if (articleVenteInfo && radioButton_article_detail_remise_prix->isChecked())
    {
        double remise_prix = lineEdit_article_detail_remise_prix->text().toDouble();
        double remise_pourcentage = (100.0 * remise_prix) / articleVenteInfo->_prix_unitaire;

        articleVenteInfo->_remise_prix = remise_prix;

        articleVenteInfo->_remise_pourcentage = remise_pourcentage;

        lineEdit_article_detail_remise_pourcentage
			->setText(QString::number(articleVenteInfo->_remise_pourcentage, 'f', 2));
    }

    actualiser_montant_remise();
}


void YerothSortirWindow::calculate_details_window_remise_pourcentage()
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);

    if (0 != articleVenteInfo && radioButton_article_detail_remise_pourcentage->isChecked())
    {
        double remise_pourcentage = lineEdit_article_detail_remise_pourcentage->text().toDouble();
        double remise_prix = (articleVenteInfo->_prix_unitaire * remise_pourcentage) / 100.0;
        articleVenteInfo->_remise_pourcentage = remise_pourcentage;
        articleVenteInfo->_remise_prix = remise_prix;
        lineEdit_article_detail_remise_prix->setText(articleVenteInfo->remisePrix());
    }

    actualiser_montant_remise();
}

void YerothSortirWindow::actualiser_montant_remise()
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);

    double prix_unitaire = articleVenteInfo->_prix_unitaire - articleVenteInfo->_remise_prix;

    double montant_tva = prix_unitaire * YerothERPConfig::tva_value;

    if (checkBox_tva->isChecked())
    {
        articleVenteInfo->_montant_tva = montant_tva;
    }

    lineEdit_article_detail_tva->setText(articleVenteInfo->montantTva());
    lineEdit_article_detail_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(prix_unitaire));
}

void YerothSortirWindow::resetCheckboxTVA()
{
    _tvaCheckBoxPreviousState = false;
    checkBox_tva->setChecked(false);
}

void YerothSortirWindow::handleTVACheckBox(bool clicked)
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);

    QString rowStockID = tableWidget_articles->getStockID(tableWidgetRow);

    QSqlQuery stockRecordQuery;

    QString stockRecordQueryStr(QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
    								.arg(YerothDatabaseTableColumn::MONTANT_TVA,
    									 _allWindows->STOCKS,
										 YerothDatabaseTableColumn::ID,
										 rowStockID));

    double montant_tva = 0.0;

    int querySize = YerothUtils::execQuery(stockRecordQuery, stockRecordQueryStr, _logger);

    if (querySize > 0 && stockRecordQuery.next())
    {
    	montant_tva = stockRecordQuery.value(YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
    }

    double prix_vente = articleVenteInfo->prix_vente();

    if (clicked)
    {
        if (false == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }

        if (montant_tva <= 0)
        {
            montant_tva = prix_vente * YerothERPConfig::tva_value;
        }

        articleVenteInfo->_montant_tva = montant_tva;
    }
    else
    {
        articleVenteInfo->_montant_tva = 0;
        if (true == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = false;
        }
    }

    actualiser_montant_remise();
}

void YerothSortirWindow::updateQuantiteAVendre()
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);

    if (!articleVenteInfo)
    {
        return;
    }

    double itemNewQteDouble = lineEdit_article_detail_quantite_a_vendre->text().toDouble();

    if (articleVenteInfo->_quantite_en_stock < itemNewQteDouble)
    {
        YerothQMessageBox::warning(this,
                                  QObject::tr("pas assez d'articles en stock"),
                                  QObject::tr("Il n'y a pas assez d'articles en stock !"));
    }
    else
    {
        articleVenteInfo->_quantite_a_vendre = itemNewQteDouble;
    }

    actualiser_montant_remise();
}


void YerothSortirWindow::afficher_tableau_sorties(const int tableWidgetRow)
{
    tableWidget_articles->myClear();

    QMapIterator<int, YerothArticleVenteInfo *> itArticleItemVenteInfo(articleItemToVenteInfo);

    while (itArticleItemVenteInfo.hasNext())
    {
        itArticleItemVenteInfo.next();

        YerothArticleVenteInfo *articleVenteInfo = itArticleItemVenteInfo.value();

        if (articleVenteInfo->_quantite_a_vendre > 0)
        {
            tableWidget_articles->addArticleAfter(*articleVenteInfo);
        }
    }

    if (tableWidget_articles->rowCount() > 0)
    {
        if (tableWidgetRow >= tableWidget_articles->rowCount())
        {
            tableWidget_articles->selectRow(tableWidget_articles->rowCount() - 1);
        }
        else
        {
            tableWidget_articles->selectRow(tableWidgetRow);
        }
    }
}

void YerothSortirWindow::afficher_sortie_detail(const int tableWidgetRow)
{
    _logger->log("afficher_sortie_detail", QString("row: %1").arg(tableWidgetRow));

    if (tableWidgetRow < 0)
    {
        return;
    }

    QString rowStockID = tableWidget_articles->getStockID(tableWidgetRow);

    QSqlQuery stockRecordQuery;

    QString stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'")
    								.arg(_allWindows->STOCKS,
										 YerothDatabaseTableColumn::ID,
										 rowStockID));
    QVariant img;

    QDate date_peremption;

    QString nom_entreprise_fournisseur;

    int querySize = YerothUtils::execQuery(stockRecordQuery, stockRecordQueryStr, _logger);

    if (querySize > 0 && stockRecordQuery.next())
    {
    	img = stockRecordQuery.value(YerothDatabaseTableColumn::IMAGE_PRODUIT);

    	date_peremption =
    			stockRecordQuery.value(YerothDatabaseTableColumn::DATE_PEREMPTION).toDate();

    	nom_entreprise_fournisseur =
    			stockRecordQuery.value(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR).toString();
    }

    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);

    lineEdit_article_detail_reference_produit->setText(articleVenteInfo->_stockReference);

    lineEdit_article_detail_designation->setText(articleVenteInfo->_stockName);

    lineEdit_article_detail_nom_entreprise_fournisseur->setText(nom_entreprise_fournisseur);

    lineEdit_article_detail_categorie->setText(articleVenteInfo->_stockCategorie);

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->clear();
        label_image_produit->setAutoFillBackground(false);
    }

    lineEdit_article_detail_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(articleVenteInfo->_prix_unitaire));
    lineEdit_article_detail_quantite_en_stock->setText(articleVenteInfo->quantiteEnStock());
    lineEdit_article_detail_quantite_a_vendre->setText(articleVenteInfo->quantiteAVendre());
    lineEdit_article_detail_remise_prix->setText(articleVenteInfo->remisePrix());
    lineEdit_article_detail_remise_pourcentage->setText(articleVenteInfo->remisePourcentage());

    if (articleVenteInfo->_montant_tva > 0)
    {
        if (checkBox_tva->isChecked())
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }

        checkBox_tva->setChecked(true);

        lineEdit_article_detail_tva->setText(articleVenteInfo->montantTva());
    }
    else
    {
        if (checkBox_tva->isChecked())
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }

        checkBox_tva->setChecked(false);

        lineEdit_article_detail_tva->setText(YerothLineEdit::EMPTY_STRING);
    }

    dateEdit_article_detail_date_peremption->setDate(date_peremption);

    lineEdit_article_detail_nom_magasinier->setText(_allWindows->getUser()->nom_complet());
    lineEdit_article_detail_nom_recepteur->setText(lineEdit_articles_nom_recepteur->text());
    lineEdit_article_detail_destination->setText(lineEdit_articles_transfert->text());
    lineEdit_article_detail_nom_client->setText(lineEdit_articles_nom_client->text());

    tabWidget_sorties->setCurrentIndex(AfficherSortieAuDetail);
}


void YerothSortirWindow::handleQteChange(QTableWidgetItem * itemChanged)
{
    if (itemChanged && itemChanged->column() == YerothTableWidget::QTE_COLUMN)
    {
        _updateItemConversionError = false;

        double newQteValue = itemChanged->text().toDouble(&_updateItemConversionError);

        if (newQteValue <= 0)
        {
            itemChanged->setText(_previousPressedQteValue);
            return;
        }

        //No conversion error occurred.
        if (true == _updateItemConversionError)
        {
            YerothSqlTableModel *articleSqlTableModel = 0;

            if (_qteChangeCodeBar)
            {
                articleSqlTableModel = lineEdit_recherche_article_codebar->getMySqlTableModel();
            }
            else
            {
                articleSqlTableModel = lineEdit_recherche_article->getMySqlTableModel();
            }

            QString rowStockID = tableWidget_articles->getStockID(itemChanged->row());

            QSqlQuery stockRecordQuery;

            QString stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'")
            								.arg(_allWindows->STOCKS,
        										 YerothDatabaseTableColumn::ID,
        										 rowStockID));

            double montantTva = 0.0;
            double prixVente = 0.0;
            double qteEnStock = 0.0;

            QString stockReference;
            QString stockName;
            QString stockCategorie;

            int querySize = YerothUtils::execQuery(stockRecordQuery, stockRecordQueryStr, _logger);

            if (querySize > 0 && stockRecordQuery.next())
            {
            	montantTva = stockRecordQuery.value(YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
            	prixVente = stockRecordQuery.value(YerothDatabaseTableColumn::PRIX_VENTE).toDouble();
            	qteEnStock = stockRecordQuery.value(YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();

            	stockReference = stockRecordQuery.value(YerothDatabaseTableColumn::REFERENCE).toString();
                stockName = stockRecordQuery.value(YerothDatabaseTableColumn::DESIGNATION).toString();
                stockCategorie = stockRecordQuery.value(YerothDatabaseTableColumn::CATEGORIE).toString();
            }

            YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(itemChanged->row());

            if (0 == articleVenteInfo)
            {
                articleVenteInfo = new YerothArticleVenteInfo;
                articleVenteInfo->_stockReference = stockReference;
                articleVenteInfo->_stockName = stockName;
                articleVenteInfo->_stockCategorie = stockCategorie;
                articleVenteInfo->_stockID = rowStockID;
                articleVenteInfo->_quantite_en_stock = qteEnStock;
                articleVenteInfo->_montant_tva = montantTva;
                articleVenteInfo->_prix_unitaire = prixVente - montantTva;
                articleVenteInfo->_quantite_a_vendre = newQteValue;

                articleItemToVenteInfo.insert(itemChanged->row(), articleVenteInfo);
            }
            else
            {
                if (articleVenteInfo->_quantite_en_stock < newQteValue)
                {
                    YerothQMessageBox::warning(this,
                                              QObject::tr("pas assez d'articles en stock"),
                                              QObject::tr("Il n'y a pas assez d'articles en stock !"));

                    itemChanged->setText(articleVenteInfo->quantiteAVendre());
                }
                else
                {
                    articleVenteInfo->_quantite_a_vendre = newQteValue;
                }
            }
            if (_qteChangeCodeBar)
            {
                actualiser_articles(itemChanged->row(), newQteValue);
            }
            else
            {
                actualiser_articles_codebar(itemChanged->row(), newQteValue);
            }
        }
        else
        {
            itemChanged->setText(YerothTableWidget::QTE_1);
        }
    }

    setRechercheLineEditFocus();
}


bool YerothSortirWindow::article_exist(const QString codeBar, const QString designation)
{
    QTableWidgetItem *anItem = 0;

    int maxItems = tableWidget_articles->rowCount();

    for (int k = 0; k < maxItems; ++k)
    {
        anItem = tableWidget_articles->item(k, YerothTableWidget::REFERENCE_COLUMN);

        if (anItem && YerothUtils::isEqualCaseInsensitive(codeBar, anItem->text()))
        {
            anItem = tableWidget_articles->item(k, YerothTableWidget::DESIGNATION_COLUMN);

            if (anItem && YerothUtils::isEqualCaseInsensitive(designation, anItem->text()))
            {
                anItem = tableWidget_articles->item(k, YerothTableWidget::QTE_COLUMN);

                int previousItemQty = anItem->text().toInt();
                int newItemQty = previousItemQty + 1;

                anItem->setText(QString::number(newItemQty));

                return true;
            }
        }
    }

    return false;
}


void YerothSortirWindow::ajouter_article(const QString & text)
{
    _qteChangeCodeBar = false;

    QMap<QString, QString> stockNameToStockID = lineEdit_recherche_article->getStockNameToStockID();

    if (!stockNameToStockID.contains(text))
    {
    	return ;
    }

    QString stockID = stockNameToStockID.value(text);

    QSqlQuery stockRecordQuery;

    QString stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'")
    								.arg(_allWindows->STOCKS,
										 YerothDatabaseTableColumn::ID,
										 stockID));

    double montantTva = 0.0;
    double prixVente = 0.0;
    double qteEnStock = 0.0;

    QString stockReference;
    QString stockName;
    QString stockCategorie;

    int querySize = YerothUtils::execQuery(stockRecordQuery, stockRecordQueryStr, _logger);

    if (querySize > 0 && stockRecordQuery.next())
    {
    	montantTva = stockRecordQuery.value(YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
    	prixVente = stockRecordQuery.value(YerothDatabaseTableColumn::PRIX_VENTE).toDouble();
    	qteEnStock = stockRecordQuery.value(YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();

    	stockReference = stockRecordQuery.value(YerothDatabaseTableColumn::REFERENCE).toString();
    	stockName = stockRecordQuery.value(YerothDatabaseTableColumn::DESIGNATION).toString();
    	stockCategorie = stockRecordQuery.value(YerothDatabaseTableColumn::CATEGORIE).toString();

    	if (article_exist(stockReference, stockName))
    	{
    		return;
    	}
    }
    else
    {
    	return ;
    }

    double prix_unitaire = prixVente - montantTva;

    //Each call to YerothTableWidget::setItem in method 'YerothTableWidget::addArticle'
    //triggers a call to YerothSortirWindow::handleQteChange
    int lastCurRow =
        tableWidget_articles->addArticle(stockID,
        								 stockReference,
										 stockName,
										 stockCategorie,
                                         QString::number(prix_unitaire, 'f', 2),
										 QString::number(montantTva, 'f', 2),
										 QString::number(prixVente, 'f', 2),
										 YerothTableWidget::QTE_1,
										 QString::number(qteEnStock));

    if (lastCurRow > -1)
    {
        tableWidget_articles->selectRow(lastCurRow);
    }

    setRechercheDesignationArticleFocus();
}


void YerothSortirWindow::ajouter_article_codebar(const QString & text)
{
    QMap<QString, QString> stockReferenceToStockID =
    		lineEdit_recherche_article_codebar->getStockreferenceCodebarToStockID();

    if (!stockReferenceToStockID.contains(text))
    {
    	return ;
    }

    QString stockID = stockReferenceToStockID.value(text);

    QSqlQuery stockRecordQuery;

    QString stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'")
    								.arg(_allWindows->STOCKS,
										 YerothDatabaseTableColumn::ID,
										 stockID));

    double montantTva = 0.0;
    double prixVente = 0.0;
    double qteEnStock = 0.0;

    QString stockReference;
    QString stockName;
    QString stockCategorie;

    int querySize = YerothUtils::execQuery(stockRecordQuery, stockRecordQueryStr, _logger);

    if (querySize > 0 && stockRecordQuery.next())
    {
    	montantTva = stockRecordQuery.value(YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
    	prixVente = stockRecordQuery.value(YerothDatabaseTableColumn::PRIX_VENTE).toDouble();
    	qteEnStock = stockRecordQuery.value(YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();

    	stockReference = stockRecordQuery.value(YerothDatabaseTableColumn::REFERENCE).toString();
    	stockName = stockRecordQuery.value(YerothDatabaseTableColumn::DESIGNATION).toString();
    	stockCategorie = stockRecordQuery.value(YerothDatabaseTableColumn::CATEGORIE).toString();

    	if (article_exist(stockReference, stockName))
    	{
    		return;
    	}
    }
    else
    {
    	return ;
    }

    double prix_unitaire = prixVente - montantTva;

    //Each call to YerothTableWidget::setItem in method 'YerothTableWidget::addArticle'
    //triggers a call to YerothSortirWindow::handleQteChange
    int lastCurRow =
        tableWidget_articles->addArticle(stockID,
        								 stockReference,
										 stockName,
										 stockCategorie,
                                         QString::number(prix_unitaire, 'f', 2),
										 QString::number(montantTva, 'f', 2),
										 QString::number(prixVente, 'f', 2),
										 YerothTableWidget::QTE_1,
										 QString::number(qteEnStock));
    if (lastCurRow > -1)
    {
        tableWidget_articles->selectRow(lastCurRow);
    }

    setRechercheCodebarArticleFocus();
}


/**
 * This method is called by 'YerothSortirWindow::handleQteChange'
 */
void YerothSortirWindow::actualiser_articles(int row, unsigned newItemQte)
{
    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableWidget_articles->rowCount(); ++k)
    {
        YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(k);

        if (row != k)
        {
            curTableWidgetItem = tableWidget_articles->item(k, YerothTableWidget::QTE_COLUMN);
            curTableWidgetItemQte = curTableWidgetItem->text().toDouble();
        }
        else
        {
            //To update the table values after modifying the quantity value of an item
            curTableWidgetItemQte = newItemQte;

            QTableWidgetItem *totalTvaWidgetItem =
                tableWidget_articles->item(k, YerothTableWidget::TOTAL_TVA_COLUMN);

            QTableWidgetItem *totalWidgetItem = tableWidget_articles->item(k, YerothTableWidget::TOTAL_COLUMN);

            if (totalTvaWidgetItem)
            {
                totalTvaWidgetItem->setText(articleVenteInfo->montantTva());
            }
            //We must always keep this goto break to avoid false updates
            if (!totalWidgetItem)
            {
                goto myItemBreak;
            }

            totalWidgetItem->setText(QString::number(articleVenteInfo->prix_vente(), 'f', 2));
        }
myItemBreak:

        quantiteVendue += curTableWidgetItemQte;
        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);
        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteVendue = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    update_lineedits_and_labels();
    tableWidget_articles->resizeColumnsToContents();
}

/**
  * This method is called by 'YerothSortirWindow::handleQteChange'
  */
void YerothSortirWindow::actualiser_articles_codebar(int row, unsigned newItemQte)
{
    _qteChangeCodeBar = true;

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableWidget_articles->rowCount(); ++k)
    {
        YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(k);

        if (row != k)
        {
            curTableWidgetItem = tableWidget_articles->item(k, YerothTableWidget::QTE_COLUMN);
            curTableWidgetItemQte = curTableWidgetItem->text().toDouble();
        }
        else
        {
            //To update the table values after modifying the quantity value of an item
            curTableWidgetItemQte = newItemQte;

            QTableWidgetItem *totalTvaWidgetItem =
                tableWidget_articles->item(k, YerothTableWidget::TOTAL_TVA_COLUMN);

            QTableWidgetItem *totalWidgetItem = tableWidget_articles->item(k, YerothTableWidget::TOTAL_COLUMN);

            if (totalTvaWidgetItem)
            {
                totalTvaWidgetItem->setText(articleVenteInfo->montantTva());
            }

            //We must always keep this goto break to avoid false updates
            if (!totalWidgetItem)
            {
                goto myItemBreak;
            }

            totalWidgetItem->setText(QString::number(articleVenteInfo->prix_vente(), 'f', 2));
        }

myItemBreak:
        quantiteVendue += curTableWidgetItemQte;
        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);
        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteVendue = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    update_lineedits_and_labels();
    tableWidget_articles->resizeColumnsToContents();
}

void YerothSortirWindow::actualiser_tableau_sortie()
{
    int tableRowCount = tableWidget_articles->rowCount();

    _logger->log("actualiser_tableau_sortie", QString("tableRowCount: %1").arg(tableRowCount));

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableRowCount; ++k)
    {
        YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(k);

        curTableWidgetItem = tableWidget_articles->item(k, YerothTableWidget::QTE_COLUMN);
        curTableWidgetItemQte = curTableWidgetItem->text().toDouble();

        quantiteVendue += curTableWidgetItemQte;
        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);
        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteVendue = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    update_lineedits_and_labels();

    tableWidget_articles->resizeColumnsToContents();
}


void YerothSortirWindow::enlever_article()
{
    _logger->log("enlever_article");

    int tableWidgetRow = tableWidget_articles->currentRow();

    _logger->log("enlever_article", QString("row: %1").arg(tableWidgetRow));

    if (tableWidgetRow > -1)
    {
        tableWidget_articles->removeArticle(tableWidgetRow);
        {
            YerothArticleVenteInfo *article = articleItemToVenteInfo.value(tableWidgetRow);
            articleItemToVenteInfo.remove(tableWidgetRow);
            delete article;
        }

        QMap < int, YerothArticleVenteInfo * >newArticleItemToVenteInfo;
        QMapIterator < int, YerothArticleVenteInfo * >i(articleItemToVenteInfo);

        int k = 0;

        while (i.hasNext())
        {
            i.next();
            newArticleItemToVenteInfo.insert(k, i.value());
            _logger->debug("enlever_article",
                           QString("key: %1, value: %2").arg(QString::number(k), i.value()->_stockName));
            ++k;
        }

        articleItemToVenteInfo.clear();
        articleItemToVenteInfo = newArticleItemToVenteInfo;

        actualiser_tableau_sortie();
        setRechercheLineEditFocus();
    }
}

void YerothSortirWindow::sortir()
{
    if (tableWidget_articles->itemCount() <= 0)
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("stocks à sortir"),
                                  QObject::trUtf8("Vous n'avez pas entré de stocks à sortir!"));
        return;
    }

    if (!check_fields())
    {
        return;
    }

    bool sortieSucces = false;

    QString msgVente(QObject::trUtf8("Poursuivre avec la sortie de "));
    msgVente.append(QString::number(_quantiteVendue));
    msgVente.append(QObject::trUtf8(" articles ?"));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
            						   QObject::trUtf8("sortie"),
									   msgVente,
                                       QMessageBox::Cancel,
									   QMessageBox::Ok))
    {
        YerothUtils::startTransaction();

        int stocksSortieID = -1;

    	int IDforReceipt = YerothERPWindows::getNextIdSqlTableModel_stocks_sorties();

    	QString referenceRecuSortie(YerothUtils::GET_REFERENCE_RECU_SORTIE(QString::number(IDforReceipt)));

        QString quantiteQueryStr;

        QSqlQuery quantiteQuery;

        QDate datePeremtion;

        QString historiqueStock;

        QString localisation;

        QString nomEntrepriseFournisseur;

        bool isService = false;

        double quantite_totale_actuelle = 0.0;

    	YerothArticleVenteInfo *articleVenteInfo = 0;

        for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
        {
            articleVenteInfo = articleItemToVenteInfo.value(j);

            if (0 == articleVenteInfo)
            {
            	continue;
            }

            quantiteQueryStr = QString("SELECT * FROM %1 WHERE %2 = '%3'")
                                     .arg(_allWindows->STOCKS,
                                          YerothDatabaseTableColumn::ID,
										  articleVenteInfo->_stockID);

            int querySize = YerothUtils::execQuery(quantiteQuery, quantiteQueryStr, _logger);

            if (querySize > 0 && quantiteQuery.next())
            {
            	isService = quantiteQuery.value(YerothDatabaseTableColumn::IS_SERVICE).toBool();

                quantite_totale_actuelle =
                		quantiteQuery.value(YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();

                datePeremtion =
                		quantiteQuery.value(YerothDatabaseTableColumn::DATE_PEREMPTION).toDate();

                historiqueStock =
                		quantiteQuery.value(YerothDatabaseTableColumn::HISTORIQUE_STOCK).toString();

                localisation =
                		quantiteQuery.value(YerothDatabaseTableColumn::LOCALISATION).toString();

                nomEntrepriseFournisseur =
                		quantiteQuery.value(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR).toString();
            }

            //qDebug() << QString("++ previousHistoriqueStock: %1")
            //        		.arg(historiqueStock);

            YerothSqlTableModel & stocksSortiesTableModel = _allWindows->getSqlTableModel_stocks_sorties();

            QSqlRecord stocksSortiesRecord = stocksSortiesTableModel.record();

            stocksSortieID = YerothERPWindows::getNextIdSqlTableModel_stocks_sorties();

            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::ID, stocksSortieID);

            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_SORTIE, referenceRecuSortie);

            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::REFERENCE, articleVenteInfo->_stockReference);

            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::DESIGNATION, articleVenteInfo->_stockName);
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, datePeremtion);

            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::CATEGORIE, articleVenteInfo->_stockCategorie);
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::QUANTITE_SORTIE, articleVenteInfo->_quantite_a_vendre);
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, articleVenteInfo->_prix_unitaire);
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE, articleVenteInfo->prix_vente());
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA, articleVenteInfo->montant_tva());
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::LOCALISATION_SORTIE, localisation);
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::LOCALISATION_ENTREE, lineEdit_articles_transfert->text());
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::NOM_RECEPTEUR, lineEdit_articles_nom_recepteur->text());

            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, nomEntrepriseFournisseur);

            YerothPOSUser *user = _allWindows->getUser();

            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::NOM_MAGASINIER, user->nom_complet());
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::NOM_UTILISATEUR_MAGASINIER, user->nom_utilisateur());
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::DATE_SORTIE, GET_CURRENT_DATE);
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::HEURE_SORTIE, CURRENT_TIME);
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::NOTES, textEdit_articles_notes->toPlainText());
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::IS_SERVICE, isService);
            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::STOCKS_ID, articleVenteInfo->_stockID);

            double nouvelle_quantite_totale = quantite_totale_actuelle - articleVenteInfo->_quantite_a_vendre;

            QString historiqueStockSortie_transfert;

            if (lineEdit_articles_transfert->text().isEmpty())
            {
            	historiqueStockSortie_transfert.append(YerothHistoriqueStock::creer_mouvement_stock
                			(SORTIE,
                			 stocksSortieID,
							 GET_CURRENT_DATE,
							 quantite_totale_actuelle,
							 articleVenteInfo->_quantite_a_vendre,
							 nouvelle_quantite_totale));
            }
            else
            {
                historiqueStockSortie_transfert.append(YerothHistoriqueStock::creer_mouvement_stock
                			(TRANSFERT,
                			 stocksSortieID,
							 GET_CURRENT_DATE,
							 quantite_totale_actuelle,
							 articleVenteInfo->_quantite_a_vendre,
							 nouvelle_quantite_totale));
            }

            historiqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
            			   .append(historiqueStockSortie_transfert);

            stocksSortiesRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, historiqueStock);

            //qDebug() << QString("++ historiqueStock: %1")
            //        		.arg(historiqueStock);

            bool success1 = stocksSortiesTableModel.insertNewRecord(stocksSortiesRecord, this);

            if (success1)
            {
                if (nouvelle_quantite_totale < 0)
                {
                    nouvelle_quantite_totale = 0;
                }

                if (0 == nouvelle_quantite_totale)
                {
                    QString removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
                                           .arg(_allWindows->STOCKS,
                                                YerothDatabaseTableColumn::ID,
												articleVenteInfo->_stockID));
                    YerothUtils::execQuery(removeRowQuery);
                }

                quantiteQueryStr.clear();
                quantiteQueryStr.append(QString("UPDATE %1 SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'")
                                        .arg(_allWindows->STOCKS,
                                             YerothDatabaseTableColumn::QUANTITE_TOTAL,
                                             QString::number(nouvelle_quantite_totale),
											 YerothDatabaseTableColumn::HISTORIQUE_STOCK,
											 historiqueStock,
                                             YerothDatabaseTableColumn::ID,
											 articleVenteInfo->_stockID));

                bool success2 = YerothUtils::execQuery(quantiteQueryStr, _logger);

                QString sMsg(QString(QObject::trUtf8("Le stock '%1' (%2 pièce(s))"))
                				.arg(articleVenteInfo->_stockName,
                					 articleVenteInfo->_quantite_a_vendre));

                if (success2)
                {
                    sMsg.append(QObject::trUtf8(" à été sorti avec succès."));
                }
                else
                {
                    sMsg.append(QString(QObject::trUtf8(" n'a pas pu être sorti !\n" "Contacter l'administrateur de %1"))
                    				.arg(YerothUtils::APPLICATION_NAME));
                }

                sortieSucces = success1 && success2;

                _logger->log("sortir", sMsg);
            }
        }
        if (sortieSucces)
        {
            QString vMsg(QString(QObject::trUtf8("La sortie de '%1' articles a été effectuée avec succès !"))
            				.arg(QString::number(_quantiteVendue, 'f', 0)));

            if (QMessageBox::Ok ==
                    YerothQMessageBox::information(this,
                    							  QObject::trUtf8("succès"),
                                                  vMsg))
            {
                imprimer_facture(referenceRecuSortie);
            }
        }

        YerothUtils::commitTransaction();

        cleanUpAfterVente();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::trUtf8("annulation"),
                                      QObject::trUtf8("Vous avez annulé la sortie des articles !"),
                                      QMessageBox::Ok);
    }

    tableWidget_articles->resizeColumnsToContents();
}
