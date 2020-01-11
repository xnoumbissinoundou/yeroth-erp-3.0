/*
   * yeroth-erp-pointdevente-window.cpp
   *
   *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
   */

#include "yeroth-erp-pointdevente-window.hpp"

#include "src/windows/tests/yeroth-erp-test-vente-window.hpp"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/windows/yeroth-erp-pointdevente-liststocks-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/windows/yeroth-erp-stock-detail-window.hpp"

#include "src/widgets/yeroth-erp-line-edit.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-info-entreprise.hpp"

#include "src/utils/yeroth-erp-style.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include <QtCore/QThread>

#include <QtCore/QFile>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QCompleter>

#include <QtWidgets/QCalendarWidget>

#include <unistd.h>




bool YerothPointDeVenteWindow::_qteChangeCodeBar(false);

const QString YerothPointDeVenteWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE, QObject::tr("point de vente")));


YerothPointDeVenteWindow::YerothPointDeVenteWindow()

:YerothWindowsCommons(YerothPointDeVenteWindow::_WINDOW_TITLE),

 _logger(new YerothLogger("YerothPointDeVenteWindow")),
 _paiement_carte_credit_carte_debit(false),
 _paiment_comptant(false),
 _paiement_compte_client(false),
 _currentFocusSearchBar(0),
 _currentCreditCardInfo(0),
 _updateItemConversionError(false),
 _previousPressedQteValue("1"),
 _tvaCheckBoxPreviousState(false),
 _sommeTotal(0.0),
 _remise_somme_total_prix(0.0),
 _remise_somme_total_pourcentage(0.0),
 _tva(0.0),
 _montantRecu(0.0),
 _montantARembourser(0.0),
 _quantiteAVendre(0),
 _currentTabView(0),
 actionRechercheArticle(0),
 actionRechercheArticleCodebar(0)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}").arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70);

    tableWidget_articles->resizeColumnsToContents();

    label_total_ttc->
    		setStyleSheet(YerothPosStyle::getColorStyleSheetString(QColor(COLOUR_RGB_YEROTH_ORANGE_243_162_0)));

    _currentFocusSearchBar = INITIAL_FOCUS_SEARCHBAR_VENTE_WINDOW;

    _currentCreditCardInfo = new YerothPOSCreditCardInfo;

    setRechercheLineEditFocus();

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_minus->enable(this, SLOT(enlever_article()));

    // Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionGuideUtilisateur, SIGNAL(triggered()), this, SLOT(getManuelUtilisateurPDF()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
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
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(choisir_methode_paiment()));

    connect(this, SIGNAL(SELLING()),
    		_allWindows->_pdVenteMethodePaiementCarteCreditDialog, SLOT(reset_all_fields()));

    connect(_allWindows->_pdVenteMethodePaiementCarteCreditDialog,
    			SIGNAL(SIGNAL_CREDIT_CARD_INFO_ALL_PROCESSED()),
			this,
				SLOT(PRE__PROCESS_CREDIT_CARD_PAYMENT()));

    connect(_allWindows->_pdVenteMethodePaiementComptantEntreeDialog->lineEdit_vente_montant_recu,
    			SIGNAL(textChanged(const QString &)),
			this,
				SLOT(handleMontantRecu()));

    connect(_allWindows->_pdVenteMethodePaiementComptantEntreeDialog->lineEdit_vente_montant_recu,
    			SIGNAL(textEdited(const QString)),
			this,
				SLOT(handleMontantRecu()));

    connect(tableWidget_articles, SIGNAL(itemPressed(QTableWidgetItem *)), this,
            SLOT(handleQtePressed(QTableWidgetItem *)));

    connect(tableWidget_articles, SIGNAL(itemChanged(QTableWidgetItem *)), this,
            SLOT(handleQteChange(QTableWidgetItem *)));

    connect(tabWidget_vente, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));

    connect(tableWidget_articles, SIGNAL(addedYerothTableWidget()), this, SLOT(handleAddedArticle()));

    connect(tableWidget_articles, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this,
            SLOT(afficher_vente_detail(QTableWidgetItem *)));

    /* Signals-slots connection for the second tab 'Article au détail' */
    connect(checkBox_tva, SIGNAL(clicked(bool)), this, SLOT(handleTVACheckBox(bool)));

    connect(lineEdit_article_detail_quantite_a_vendre, SIGNAL(textEdited(const QString &)), this,
            SLOT(updateQuantiteAVendre()));

    connect(lineEdit_article_detail_remise_prix, SIGNAL(textEdited(const QString)), this,
            SLOT(calculate_details_window_remise_prix()));

    connect(lineEdit_article_detail_remise_pourcentage, SIGNAL(textEdited(const QString)), this,
            SLOT(calculate_details_window_remise_pourcentage()));

    connect(radioButton_article_detail_remise_prix, SIGNAL(toggled(bool)), this,
            SLOT(activateLineEditRemisePrix(bool)));

    connect(radioButton_article_detail_remise_pourcentage, SIGNAL(toggled(bool)), this,
            SLOT(activateLineEditRemisePourcentage(bool)));

    connect(actionSet_reference_as_standard_input, SIGNAL(triggered()), this,
            SLOT(setBarcodeAsStandardInput()));

    connect(actionSet_stock_item_name_as_standard_input, SIGNAL(triggered()), this,
            SLOT(setStockItemNameAsStandardInput()));

    this->setupShortcuts();
}

YerothPointDeVenteWindow::~YerothPointDeVenteWindow()
{
    deleteArticleVenteInfos();

    _paiement_carte_credit_carte_debit = false;

    _paiment_comptant = false;

    _paiement_compte_client = false;

    delete _currentCreditCardInfo;

    delete actionRechercheArticle;

    delete actionRechercheArticleCodebar;

    delete _logger;
}

void YerothPointDeVenteWindow::setupRemises(bool aValue)
{
    radioButton_article_detail_remise_prix->setEnabled(aValue);
    radioButton_article_detail_remise_pourcentage->setEnabled(aValue);
    lineEdit_article_detail_remise_prix->setEnabled(aValue);
    lineEdit_article_detail_remise_pourcentage->setEnabled(aValue);
}

void YerothPointDeVenteWindow::deleteArticleVenteInfos()
{
    //qDebug() << "[YerothPointDeVenteWindow][deleteArticleVenteInfos]";
    QMapIterator < int, YerothArticleVenteInfo * >i(articleItemToVenteInfo);
    while (i.hasNext())
    {
        i.next();
        YerothArticleVenteInfo *v = i.value();
        if (0 != v)
        {
            delete v;
        }
    }
    articleItemToVenteInfo.clear();
}


void YerothPointDeVenteWindow::setBarcodeAsStandardInput()
{
	_currentFocusSearchBar = lineEdit_recherche_article_codebar;
	lineEdit_recherche_article_codebar->setFocus();
}


void YerothPointDeVenteWindow::setStockItemNameAsStandardInput()
{
	_currentFocusSearchBar = lineEdit_recherche_article;
	lineEdit_recherche_article->setFocus();
}


void YerothPointDeVenteWindow::setupLineEdits()
{
    setupRemises(false);

    lineEdit_recherche_article->enableForSearch(QObject::trUtf8("désignation [ focus avec F12 ]"));

    lineEdit_recherche_article_codebar->enableForSearch(QObject::trUtf8("référence [ focus avec F11 ]"));

    lineEdit_articles_nom_client->enableForSearch(QObject::trUtf8("nom du client"));

    lineEdit_articles_imprimante->setText(YerothERPConfig::printer);
    lineEdit_articles_imprimante->setReadOnly(true);

    lineEdit_article_detail_nom_client->setEnabled(false);
    lineEdit_article_detail_reference_produit->setEnabled(false);
    lineEdit_article_detail_designation->setEnabled(false);
    lineEdit_article_detail_nom_entreprise_fournisseur->setEnabled(false);
    lineEdit_article_detail_categorie->setEnabled(false);
    lineEdit_article_detail_prix_unitaire->setEnabled(false);
    lineEdit_article_detail_tva->setEnabled(false);
    lineEdit_article_detail_localisation->setEnabled(false);
    lineEdit_article_detail_nom_caissier->setEnabled(false);
    lineEdit_articles_nom_caissier->setEnabled(false);
    lineEdit_articles_total->setEnabled(false);
    lineEdit_articles_tva->setEnabled(false);
    lineEdit_articles_somme_total->setEnabled(false);
    lineEdit_article_detail_quantite_en_stock->setEnabled(false);
    lineEdit_articles_quantite_a_vendre->setEnabled(false);
    lineEdit_articles_montant_a_rembourser->setEnabled(false);
    lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM(0.0));

    connect(lineEdit_articles_nom_client,
    		SIGNAL(textChanged(const QString &)),
			this,
            SLOT(handleClientName(const QString &)));

    lineEdit_article_detail_quantite_a_vendre->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_article_detail_remise_prix->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_article_detail_remise_pourcentage->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_article_detail_remise_prix->setEnabled(false);
    lineEdit_article_detail_remise_pourcentage->setEnabled(false);
}


void YerothPointDeVenteWindow::setupLineEditsQCompleters()
{
	lineEdit_articles_nom_client->setupMyStaticQCompleter(_allWindows->CLIENTS, YerothDatabaseTableColumn::NOM_ENTREPRISE, true);
}


bool YerothPointDeVenteWindow::PROCESS_CREDIT_CARD_PAYMENT()
{
	_logger->log("PROCESS_CREDIT_CARD_PAYMENT");

    //QNetworkAccessManager
	//_currentCreditCardInfo;

	return true;
}


bool YerothPointDeVenteWindow::PRE__PROCESS_CREDIT_CARD_PAYMENT()
{
	_logger->log("PRE__PROCESS_CREDIT_CARD_PAYMENT");

	//QNetworkAccessManager
	//_currentCreditCardInfo;

	if (true)
	{

		if (QMessageBox::Ok ==
				YerothQMessageBox::warning(this,
						QObject::trUtf8("PRE__PROCESS_CREDIT_CARD_PAYMENT"),
						QObject::trUtf8("Le paiement par \"carte de crédit\""
								" tel quel, ne peut être effectué.")))
		{
		}
		else
		{
		}

		return false;
	}

	_allWindows->_pdVenteMethodePaiementCarteCreditDialog->rendreInvisible();

	effectuer_check_out_carte_credit_carte_debit();

	return true;
}


void YerothPointDeVenteWindow::hideEvent(QHideEvent * event)
{
	_allWindows->_pdVenteMethodePaiementComptantEntreeDialog->rendreInvisible();
    _allWindows->_pdVenteListStocksWindow->close();
}


void YerothPointDeVenteWindow::setupShortcuts()
{
    actionRechercheArticle = new QAction(this);

    actionRechercheArticleCodebar = new QAction(this);

    //actionListerStocks = new QAction(this);

    lineEdit_recherche_article->addAction(actionRechercheArticle);

    lineEdit_recherche_article_codebar->addAction(actionRechercheArticleCodebar);

    //lineEdit_recherche_article_codebar->      addAction(actionListerStocks);

    connect(actionRechercheArticle, SIGNAL(triggered()), this, SLOT(setRechercheDesignationArticleFocus()));

    connect(actionRechercheArticleCodebar, SIGNAL(triggered()), this, SLOT(setRechercheCodebarArticleFocus()));

    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));

    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);

    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);

    this->setupShortcutActionAfficherPDF	(*actionAfficherPDF);

    actionRechercheArticle->setShortcut(Qt::Key_F12);

    actionRechercheArticleCodebar->setShortcut(Qt::Key_F11);

    actionStocks->setShortcut(YerothUtils::LISTER_STOCKS_QKEYSEQUENCE);
}

void YerothPointDeVenteWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    setupRemises(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_article_detail_retour_ventes->enable(this, SLOT(retourVentes()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_vendre->enable(this, SLOT(choisir_methode_paiment()));
}

void YerothPointDeVenteWindow::definirManager()
{
    _logger->log("definirManager");

    setupRemises(true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    lineEdit_article_detail_remise_prix->setEnabled(true);
    lineEdit_article_detail_remise_pourcentage->setEnabled(true);
    pushButton_article_detail_retour_ventes->enable(this, SLOT(retourVentes()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_vendre->enable(this, SLOT(choisir_methode_paiment()));
}


void YerothPointDeVenteWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    setupRemises(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    lineEdit_article_detail_remise_prix->setEnabled(false);
    lineEdit_article_detail_remise_pourcentage->setEnabled(false);

    pushButton_article_detail_retour_ventes->enable(this, SLOT(retourVentes()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_vendre->enable(this, SLOT(choisir_methode_paiment()));
}


void YerothPointDeVenteWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    setupRemises(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_article_detail_retour_ventes->enable(this, SLOT(retourVentes()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_vendre->enable(this, SLOT(choisir_methode_paiment()));
}

void YerothPointDeVenteWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    setupRemises(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_article_detail_retour_ventes->disable(this);
    pushButton_annuler->disable(this);
    pushButton_vendre->disable(this);
}

void YerothPointDeVenteWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    setupRemises(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_article_detail_retour_ventes->disable(this);
    pushButton_annuler->disable(this);
    pushButton_vendre->disable(this);
}


QString YerothPointDeVenteWindow::afficher_facture_pdf(int vente_id)
{
	QString pdfReceiptFileName;

    if (YerothERPConfig::RECEIPT_FORMAT_PETIT == YerothERPConfig::receiptFormat)
    {
    	pdfReceiptFileName.append(imprimer_facture_petit(vente_id));
    }
    else
    {
    	pdfReceiptFileName.append(imprimer_facture_grand(vente_id));
    }

    if (! pdfReceiptFileName.isEmpty())
    {
    	return YerothERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    }

    return "";
}


QString YerothPointDeVenteWindow::imprimer_facture(int vente_id)
{
	QString pdfReceiptFileName;

    if (YerothERPConfig::RECEIPT_FORMAT_PETIT == YerothERPConfig::receiptFormat)
    {
    	pdfReceiptFileName.append(imprimer_facture_petit(vente_id));

    	if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::printer, YerothUtils::IMPRIMANTE_PDF))
    	{
    		return YerothERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    	}
    }
    else
    {
    	pdfReceiptFileName.append(imprimer_facture_grand(vente_id));

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

    		if (YerothERPConfig::ouvrirRegistreDeCaisse)
    		{
    			QThread::sleep(0.5);

    			progOptions.clear();

    			progOptions << "-c";
    			progOptions << QString("/bin/echo -e -n \"\\x1b\\x70\\x00\\x19\\xfa\" >> %1")
    								.arg(YerothERPConfig::pathToThermalPrinterDeviceFile);

    			QProcess::startDetached("/bin/bash", progOptions, YerothERPConfig::temporaryFilesDir);
    		}

    		return pdfReceiptFileName;
    	}
    }

    return "";
}


QString YerothPointDeVenteWindow::imprimer_facture_grand(int vente_id)
{
    _logger->log("imprimer_facture_grand");

    QString factureTexTable;

    if (tableWidget_articles->rowCount() <= 0)
    {
        YerothQMessageBox::information(this, QObject::trUtf8("impression"),
                                      QObject::trUtf8("Il n'y a pas de données à imprimer !"));

        return "";
    }

    YerothUtils::getFactureTexTableString(factureTexTable, *tableWidget_articles, _quantiteAVendre, _tva,
                                         _sommeTotal);

    YerothPOSUser *yerothUser = _allWindows->getUser();

    YerothInfoEntreprise & infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVilleTex());

    YerothUtils::getCurrentLocaleDate(factureDate);

#ifdef YEROTH_FRANCAIS_LANGUAGE
    YerothUtils::getFactureFRTexDocumentString(factureTexDocument, factureTexTable);
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    YerothUtils::getFactureENTexDocumentString(factureTexDocument, factureTexTable);
#endif

    factureTexDocument.replace("YEROTHTYPEPAIEMENT", YerothUtils::handleForeignAccents(_typeDeVente));

    if (-1 != vente_id)
    {
        factureTexDocument.replace("YEROTHNUMEROSORTIETRANSFERT",
                                   YerothUtils::handleForeignAccents(GET_NUM_STRING(vente_id)));
    }
    else
    {
        factureTexDocument.replace("YEROTHNUMEROSORTIETRANSFERT", QObject::tr("EXEMPLE (*NON VALIDE*)"));
    }

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER,
    									   YerothERPConfig::receiptFormat))
    {
        factureTexDocument.replace("YEROTHPAPERSPEC", "a4paper");
    }

    factureTexDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    factureTexDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    factureTexDocument.replace("YEROTHLOCALISATION_SORTIE", infoEntreprise.getLocalisationTex());
    factureTexDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    factureTexDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
    factureTexDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    factureTexDocument.replace("YEROTHDATE", factureDate);
    factureTexDocument.replace("YEROTHHEUREVENTE", CURRENT_TIME);
    factureTexDocument.replace("YEROTHVENDEUR", yerothUser->nom_completTex());
    factureTexDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    factureTexDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());
    factureTexDocument.replace("YEROTHSOMME",
                               YerothUtils::handleForeignAccents(GET_CURRENCY_STRING_NUM(_sommeTotal)));

    QString nomClient(lineEdit_articles_nom_client->text());

    if (nomClient.isEmpty())
    {
        nomClient.append("Client \"DIVERS\"");
        factureTexDocument.replace("YEROTHCLIENT", YerothUtils::handleForeignAccents(nomClient));
        factureTexDocument.replace("CLIENTYEROTHREPRESENTANT", "");
        factureTexDocument.replace("CLIENTYEROTHCITY", "");
        factureTexDocument.replace("CLIENTYEROTHPOBOX", "");
        factureTexDocument.replace("CLIENTYEROTHMAIL", "");
        factureTexDocument.replace("CLIENTYEROTHPHONE", "");
    }
    else
    {
        YerothSqlTableModel & clientTableModel = _allWindows->getSqlTableModel_clients();

        clientTableModel.yerothSetFilter(QString("nom_entreprise = '%1'").arg(nomClient));
        factureTexDocument.replace("YEROTHCLIENT", YerothUtils::handleForeignAccents(nomClient));

        if (clientTableModel.easySelect() > 0)
        {
            QSqlRecord record = clientTableModel.record(0);
            QString clientRepresentant(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_REPRESENTANT));
            QString clientVille(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
            QString clientPOBox(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
            QString clientEmail(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
            QString clientTel(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

            if (clientTel.isEmpty())
            {
                clientTel.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
            }

            //qDebug() << "++ clientEmail: " << clientEmail;
            if (!clientRepresentant.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHREPRESENTANT",
                                           YerothUtils::handleForeignAccents(clientRepresentant));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHREPRESENTANT", "");
            }

            if (!clientVille.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHCITY", YerothUtils::handleForeignAccents(clientVille));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHCITY", "");
            }

            if (!clientPOBox.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHPOBOX", YerothUtils::handleForeignAccents(clientPOBox));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHPOBOX", "N/a");
            }

            if (!clientEmail.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHMAIL", YerothUtils::handleForeignAccents(clientEmail));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHMAIL", "");
            }

            if (!clientTel.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHPHONE", YerothUtils::handleForeignAccents(clientTel));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHPHONE", "");
            }
        }

        clientTableModel.resetFilter();
    }

    QString prefixFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-vente-facture-grand"));

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-sale-receipt-big"));

#endif				//qDebug() << "++\n" << factureTexDocument;


    QFile tmpLatexFile(prefixFileName + "tex");

    if (tmpLatexFile.open(QFile::WriteOnly))
    {
        tmpLatexFile.write(factureTexDocument.toUtf8());
    }

    tmpLatexFile.close();

    return YerothERPProcess::compileLatex(prefixFileName);
}

QString YerothPointDeVenteWindow::imprimer_facture_petit(int vente_id)
{
    _logger->log("imprimer_facture_petit");

    QString factureTexTable;

    if (tableWidget_articles->rowCount() <= 0)
    {
        YerothQMessageBox::information(this, QObject::trUtf8("impression"),
                                      QObject::trUtf8("Il n'y a pas de données à imprimer !"));
        return "";
    }

    YerothPOSUser *yerothUser = _allWindows->getUser();

    YerothInfoEntreprise & infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVilleTex());

	YerothUtils::getCurrentLocaleDate(factureDate);

	YerothUtils::getFactureSmallTexTableString(factureTexTable,
											  *tableWidget_articles,
											  _quantiteAVendre,
											  _tva,
											  _sommeTotal,
											  _montantRecu,
											  _montantARembourser);

#ifdef YEROTH_FRANCAIS_LANGUAGE

    factureTexTable.append(QString("MERCI!\\\\"));
    YerothUtils::getFactureSmallFRTexDocumentString(factureTexDocument, factureTexTable);

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    factureTexTable.append(QString("Thank you!\\\\"));
    YerothUtils::getFactureSmallENTexDocumentString(factureTexDocument, factureTexTable);

#endif

    if (-1 != vente_id)
    {
        factureTexDocument.replace("YEROTHNUMEROSORTIETRANSFERT",
                                   YerothUtils::handleForeignAccents(GET_NUM_STRING(vente_id)));
    }
    else
    {
        factureTexDocument.replace("YEROTHNUMEROSORTIETRANSFERT", QObject::tr("EXEMPLE (*NON VALIDE*)"));
    }

    //_logger->debug("imprimer_facture_petit",
    //QString("document: %1").arg(factureTexDocument));

    static const double INITIAL_INCH_SIZE = 4.05;

    static const double ITEM_INCH_SIZE = 0.23;

    int articleItemCount = tableWidget_articles->rowCount();

    double factureInchSize = INITIAL_INCH_SIZE + (articleItemCount * ITEM_INCH_SIZE);

    QString minPaperHeight(QString("%1in").arg(QString::number(factureInchSize, 'f', 2)));
    //qDebug() << "\t++minPaperHeight: " << minPaperHeight;

	factureTexDocument.replace("YEROTHTYPEPAIEMENT", YerothUtils::handleForeignAccents(_typeDeVente));
    factureTexDocument.replace("YEROTHFACTURESMALLPAPERHEIGHT", minPaperHeight);
    factureTexDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    factureTexDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    factureTexDocument.replace("YEROTHLOCALISATION_SORTIE", infoEntreprise.getLocalisationTex());
    factureTexDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
    factureTexDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    factureTexDocument.replace("YEROTHDATE", factureDate);
    factureTexDocument.replace("YEROTHHEUREVENTE", CURRENT_TIME);
    factureTexDocument.replace("YEROTHVENDEUR", yerothUser->nom_completTex());

    QString nomClient(lineEdit_articles_nom_client->text());

    if (nomClient.isEmpty())
    {
        nomClient.append("\"DIVERS\"");
    }

    factureTexDocument.replace("YEROTHCLIENT", YerothUtils::handleForeignAccents(nomClient));

    //qDebug() << "++\n" << factureTexDocument;

    QString prefixFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-vente-facture-petit"));

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-sale-receipt-small"));

#endif				//qDebug() << "++\n" << factureTexDocument;


    QFile tmpLatexFile(QString("%1tex")
    					.arg(prefixFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
    												   factureTexDocument);

    return YerothERPProcess::compileLatex(prefixFileName);
}

void YerothPointDeVenteWindow::annuler()
{
    if (tableWidget_articles->rowCount() > 0)
    {
        tableWidget_articles->myClear();
        lineEdit_recherche_article->myClear();
        lineEdit_recherche_article_codebar->myClear();
        lineEdit_articles_total->setText(GET_CURRENCY_STRING_NUM(0.0));
        lineEdit_articles_somme_total->setText(GET_CURRENCY_STRING_NUM(0.0));
        label_total_ttc->setText(GET_CURRENCY_STRING_NUM(0.0));
        lineEdit_articles_quantite_a_vendre->clear();
        lineEdit_articles_tva->setText(GET_CURRENCY_STRING_NUM(0.0));
        lineEdit_articles_nom_client->clear();
        lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM(0.0));

        tableWidget_articles->resizeColumnsToContents();

        deleteArticleVenteInfos();

        handleTabViews();

        setRechercheLineEditFocus();

        YerothQMessageBox::information(this, QObject::trUtf8("annulation de la vente"),
                                      QObject::trUtf8("Vous avez annulé la vente !"), QMessageBox::Ok);
    }
}

void YerothPointDeVenteWindow::setRechercheLineEditFocus()
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

void YerothPointDeVenteWindow::setRechercheDesignationArticleFocus()
{
    _currentFocusSearchBar = lineEdit_recherche_article;
    setRechercheLineEditFocus();
}

void YerothPointDeVenteWindow::setRechercheCodebarArticleFocus()
{
    _currentFocusSearchBar = lineEdit_recherche_article_codebar;
    setRechercheLineEditFocus();
}

void YerothPointDeVenteWindow::handleClientName(const QString &text)
{
    //_logger->log("handleClientName(const QString&)");
    if (YerothUtils::isEqualCaseInsensitive(text, YerothUtils::NOUVEAU_CLIENT))
    {
        _allWindows->_creerNouveauClientWindow->rendreVisible(_curStocksTableModel);
        this->rendreInvisibleAvecConservation();
    }
}

void YerothPointDeVenteWindow::lister()
{
    _logger->log("lister");
    _allWindows->_pdVenteListStocksWindow->listStocks(*_curStocksTableModel);
}

void YerothPointDeVenteWindow::retourVentes()
{
    _currentTabView = TableauDesVentes;
    tabWidget_vente->setCurrentIndex(TableauDesVentes);
}

void YerothPointDeVenteWindow::handleAddedArticle()
{
    //_logger->debug("handleAddedArticle()");
    //qDebug() << QString("handleAddedArticle()");
    tabWidget_vente->setTabEnabled(AfficherVenteAuDetail, true);
}


void YerothPointDeVenteWindow::handleCurrentChanged(int index)
{
    //_logger->debug("handleCurrentChanged(int)", QString("index: %1").arg(index));
    _currentTabView = index;
    switch (index)
    {
    case TableauDesVentes:
        afficher_tableau_ventes(tableWidget_articles->currentRow());
        enableImprimer();
        break;
    case AfficherVenteAuDetail:
        afficher_vente_detail(tableWidget_articles->currentRow());
        disableImprimer();
        _logger->debug("handleCurrentChanged(int)",
                       QString("current table row: %1").arg(tableWidget_articles->currentRow()));
        break;
    default:
        break;
    }
}

void YerothPointDeVenteWindow::cleanUpAfterVente()
{
    _logger->log("cleanUpAfterVente");

	set_paiement_par_carte_credit_carte_debit(false);

	set_paiement_comptant(false);

	set_paiment_compte_client(false);

    this->resetCheckboxTVA();

    tableWidget_articles->myClear();
    articleItemToVenteInfo.clear();

    deleteArticleVenteInfos();

    lineEdit_recherche_article->myClear();
    lineEdit_recherche_article_codebar->myClear();

    tabWidget_vente->setCurrentIndex(TableauDesVentes);

    handleTabViews();

    _allWindows->_pdVenteMethodePaiementComptantEntreeDialog->clearLineEditValue();

    lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM(0.0));
    lineEdit_articles_tva->setText(GET_CURRENCY_STRING_NUM(0.0));
    lineEdit_articles_total->setText(GET_CURRENCY_STRING_NUM(0.0));
    lineEdit_articles_somme_total->setText(GET_CURRENCY_STRING_NUM(0.0));
    label_total_ttc->setText(GET_CURRENCY_STRING_NUM(0.0));
    lineEdit_articles_quantite_a_vendre->setText(GET_DOUBLE_STRING(0.0));

    lineEdit_articles_nom_client->clear();
    lineEdit_article_detail_reference_produit->clear();
    lineEdit_article_detail_designation->clear();
    lineEdit_article_detail_nom_entreprise_fournisseur->clear();
    lineEdit_article_detail_categorie->clear();
    lineEdit_article_detail_prix_unitaire->clear();
    lineEdit_article_detail_tva->clear();
    lineEdit_article_detail_localisation->clear();
    lineEdit_article_detail_nom_caissier->clear();
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

    if (YerothUtils::
            isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO, YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO, YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO, YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else			//CMUP == YerothConfig::salesStrategy
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }

    setRechercheLineEditFocus();
}

void YerothPointDeVenteWindow::rendreInvisible()
{
    _logger->log("rendreInvisible");

	set_paiement_par_carte_credit_carte_debit(false);

	set_paiement_comptant(false);

	set_paiment_compte_client(false);

    tableWidget_articles->myClear();

    articleItemToVenteInfo.clear();

    deleteArticleVenteInfos();

    lineEdit_recherche_article->myClear();

    lineEdit_recherche_article_codebar->myClear();

    _allWindows->_pdVenteMethodePaiementComptantEntreeDialog->clearLineEditValue();

    _remise_somme_total_prix = 0.0;

    _remise_somme_total_pourcentage = 0.0;

    _montantRecu = 0.0;

    _montantARembourser = 0.0;

    _qteChangeCodeBar = false;

    _sommeTotal = 0.0;

    _tva = 0.0;

    _quantiteAVendre = 0;

    lineEdit_articles_total->clear();

    lineEdit_articles_somme_total->clear();

    label_total_ttc->clear();

    lineEdit_articles_quantite_a_vendre->clear();

    lineEdit_articles_tva->clear();

    lineEdit_articles_nom_client->clear();

    lineEdit_articles_montant_a_rembourser->clear();

    YerothWindowsCommons::rendreInvisible();
}


void YerothPointDeVenteWindow::handleTabViews()
{
    if (0 == tableWidget_articles->rowCount())
    {
        tabWidget_vente->setTabEnabled(AfficherVenteAuDetail, false);
    }
}


void YerothPointDeVenteWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible");

    radioButton_article_detail_remise_prix->setText(QString(QObject::trUtf8("remise (%1)"))
    													.arg(YerothERPConfig::currency));

    lineEdit_articles_imprimante->setText(YerothERPConfig::printer);

    label_articles_tva->setText(YerothUtils::getTVALabelStringWithPercent());

    _curStocksTableModel = stocksTableModel;

    this->setupLineEditsQCompleters();

    if (!_curClientName.isEmpty())
    {
        lineEdit_articles_nom_client->setText(_curClientName);
        _curClientName.clear();
    }
    else
    {
        lineEdit_articles_nom_client->clear();
    }

    this->resetCheckboxTVA();

    tabWidget_vente->setCurrentIndex(TableauDesVentes);

    lineEdit_articles_nom_caissier->setText(_allWindows->getUser()->nom_complet());

    YerothUtils::refreshSalesStrategy(*_curStocksTableModel,
    								 lineEdit_recherche_article,
                                     lineEdit_recherche_article_codebar);

    if (YerothUtils::
            isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO, YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO, YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO, YerothERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else			//CMUP == YerothConfig::salesStrategy
    {
        connect(lineEdit_recherche_article->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }

    handleTabViews();

    setRechercheLineEditFocus();

    QString aNewWindowTitle;

    aNewWindowTitle = YerothUtils::getWindowTitleWithStrategy(this);

    aNewWindowTitle = YerothUtils::appendPDFReceiptFormat(aNewWindowTitle);

    this->setWindowTitle(aNewWindowTitle);

    actualiser_toutes_valeurs();

    tableWidget_articles->resizeColumnsToContents();

    this->setVisible(true);
}

void YerothPointDeVenteWindow::activateLineEditRemisePrix(bool toggled)
{
    if (toggled)
    {
        lineEdit_article_detail_remise_prix->setEnabled(true);
    }
    else
    {
        lineEdit_article_detail_remise_prix->setEnabled(false);
    }
}

void YerothPointDeVenteWindow::activateLineEditRemisePourcentage(bool toggled)
{
    if (toggled)
    {
        lineEdit_article_detail_remise_pourcentage->setEnabled(true);
    }
    else
    {
        lineEdit_article_detail_remise_pourcentage->setEnabled(false);
    }
}

void YerothPointDeVenteWindow::calculate_details_window_remise_prix()
{
    //_logger->log("calculate_details_window_remise_prix");
    int tableWidgetRow = tableWidget_articles->currentRow();
    if (tableWidgetRow < 0)
    {
        return;
    }
    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);
    if (articleVenteInfo && radioButton_article_detail_remise_prix->isChecked())
    {
        double remise_prix = lineEdit_article_detail_remise_prix->text().toDouble();
        double remise_pourcentage = (100.0 * remise_prix) / articleVenteInfo->prix_unitaire;
        if (remise_prix > articleVenteInfo->prix_unitaire)
        {
            lineEdit_article_detail_remise_prix->setText(QString::number(0.0, 'f', 2));
            lineEdit_article_detail_remise_pourcentage->setText(QString::number(0.0, 'f', 2));
            YerothQMessageBox::warning(this, QObject::trUtf8("montant d'une remise"),
                                      QString::
                                      fromUtf8("Le montant d'une remise sur un article doit être inférieur"
                                               " au prix unitaire de cet article !"));
            return;
        }
        articleVenteInfo->remise_prix = remise_prix;
        articleVenteInfo->remise_pourcentage = remise_pourcentage;
        lineEdit_article_detail_remise_pourcentage->
        setText(QString::number(articleVenteInfo->remise_pourcentage, 'f', 2));
    }
    actualiser_montant_remise();
}

void YerothPointDeVenteWindow::calculate_details_window_remise_pourcentage()
{
    //_logger->log("calculate_details_window_remise_pourcentage");
    int tableWidgetRow = tableWidget_articles->currentRow();
    if (tableWidgetRow < 0)
    {
        return;
    }
    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);
    if (articleVenteInfo && radioButton_article_detail_remise_pourcentage->isChecked())
    {
        double remise_pourcentage = lineEdit_article_detail_remise_pourcentage->text().toDouble();
        double remise_prix = (articleVenteInfo->prix_unitaire * remise_pourcentage) / 100.0;
        if (remise_prix > articleVenteInfo->prix_unitaire)
        {
            lineEdit_article_detail_remise_prix->setText(QString::number(0.0, 'f', 2));
            lineEdit_article_detail_remise_pourcentage->setText(QString::number(0.0, 'f', 2));
            YerothQMessageBox::warning(this, QObject::trUtf8("montant d'une remise"),
                                      QString::
                                      fromUtf8("Le montant d'une remise sur un article doit être supérieure"
                                               " au prix unitaire de cet article !"));
            return;
        }
        articleVenteInfo->remise_pourcentage = remise_pourcentage;
        articleVenteInfo->remise_prix = remise_prix;
        lineEdit_article_detail_remise_prix->setText(articleVenteInfo->remisePrix());
    }
    actualiser_montant_remise();
}

void YerothPointDeVenteWindow::actualiser_montant_remise()
{
    //_logger->log("actualiser_montant_remise");
    int tableWidgetRow = tableWidget_articles->currentRow();
    if (tableWidgetRow < 0)
    {
        return;
    }
    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);
    double prix_unitaire = articleVenteInfo->prix_unitaire - articleVenteInfo->remise_prix;
    double montant_tva = prix_unitaire * YerothERPConfig::tva_value;
    if (checkBox_tva->isChecked())
    {
        articleVenteInfo->_montant_tva = montant_tva;
    }
    lineEdit_article_detail_tva->setText(articleVenteInfo->montantTva());
    lineEdit_article_detail_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(prix_unitaire));
    actualiser_toutes_valeurs();
}

void YerothPointDeVenteWindow::resetCheckboxTVA()
{
    _tvaCheckBoxPreviousState = false;
    checkBox_tva->setChecked(false);
}

void YerothPointDeVenteWindow::handleTVACheckBox(bool clicked)
{
    //_logger->debug("handleTVACheckBox(int)", QString("state: %1").arg(state));
    int tableWidgetRow = tableWidget_articles->currentRow();
    if (tableWidgetRow < 0)
    {
        return;
    }
    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);
    int lastSelectedVentesRow = tableWidget_articles->getSqlTableModelIndex(tableWidgetRow);
    QSqlRecord record = lineEdit_recherche_article->getMySqlTableModel()->record(lastSelectedVentesRow);
    double montant_tva = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
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

void YerothPointDeVenteWindow::updateQuantiteAVendre()
{
    int tableWidgetRow = tableWidget_articles->currentRow();
    _logger->log("updateQuantiteAVendre", QString("tableWidgetRow: %1").arg(tableWidgetRow));
    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);
    if (!articleVenteInfo)
    {
        return;
    }
    double itemNewQteDouble = lineEdit_article_detail_quantite_a_vendre->text().toDouble();
    if (articleVenteInfo->quantite_en_stock < itemNewQteDouble)
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("articles en stock"),
                                  QObject::trUtf8("Il n'y a pas assez de articles en stock |"));
    }
    else
    {
        articleVenteInfo->quantite_a_vendre = itemNewQteDouble;
    }
    this->actualiser_montant_remise();
}

void YerothPointDeVenteWindow::afficher_tableau_ventes(const int tableWidgetRow)
{
    //_logger->log("afficher_tableau_ventes",
    //                  QString("tableWidgetRow: %1").arg(tableWidgetRow));
    tableWidget_articles->myClear();
    QMapIterator < int, YerothArticleVenteInfo * >i(articleItemToVenteInfo);
    while (i.hasNext())
    {
        i.next();
        YerothArticleVenteInfo *articleVenteInfo = i.value();
        if (articleVenteInfo->quantite_a_vendre > 0)
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
    actualiser_toutes_valeurs();
}

void YerothPointDeVenteWindow::afficher_vente_detail(const int tableWidgetRow)
{
    _logger->log("afficher_vente_detail", QString("row: %1").arg(tableWidgetRow));
    if (tableWidgetRow < 0)
    {
        return;
    }
    checkBox_tva->setText(YerothUtils::getTVALabelStringWithPercent());
    int lastSelectedVentesRow = tableWidget_articles->getSqlTableModelIndex(tableWidgetRow);
    //qDebug() << QString("\t++ lastSelectedVentesRow: %1, tableWidgetRow: %2")
    //                  .arg(lastSelectedVentesRow)
    //                          .arg(tableWidgetRow);
    YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(tableWidgetRow);
    QSqlRecord record = lineEdit_recherche_article->getMySqlTableModel()->record(lastSelectedVentesRow);
    lineEdit_article_detail_reference_produit->setText(articleVenteInfo->reference);
    lineEdit_article_detail_designation->setText(articleVenteInfo->designation);
    lineEdit_article_detail_nom_entreprise_fournisseur->
    setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
    lineEdit_article_detail_categorie->setText(articleVenteInfo->categorie);
    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_PRODUIT));
    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->clear();
        label_image_produit->setAutoFillBackground(false);
    }
    lineEdit_article_detail_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(articleVenteInfo->prix_unitaire));
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
    dateEdit_article_detail_date_peremption->
    setDate(GET_DATE_FROM_STRING(GET_SQL_RECORD_DATA(record, "date_peremption")));
    lineEdit_article_detail_nom_caissier->setText(_allWindows->getUser()->nom_complet());
    lineEdit_article_detail_localisation->setText(GET_SQL_RECORD_DATA(record, "localisation"));
    lineEdit_article_detail_nom_client->setText(lineEdit_articles_nom_client->text());
    tabWidget_vente->setCurrentIndex(AfficherVenteAuDetail);
}

void YerothPointDeVenteWindow::handleQteChange(QTableWidgetItem * itemChanged)
{
    //_logger->debug("handleQteChange(QTableWidgetItem *)");
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
            int itemSqlTableModelIndex = tableWidget_articles->getSqlTableModelIndex(itemChanged->row());
            QSqlRecord record = articleSqlTableModel->record(itemSqlTableModelIndex);
            YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(itemChanged->row());
            if (!articleVenteInfo)
            {
                articleVenteInfo = new YerothArticleVenteInfo;
                articleVenteInfo->reference = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE);
                articleVenteInfo->designation =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION);
                articleVenteInfo->categorie = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE);
                articleVenteInfo->sqlTableModelIndex = itemSqlTableModelIndex;
                articleVenteInfo->quantite_en_stock =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();
                articleVenteInfo->_montant_tva =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
                articleVenteInfo->prix_unitaire =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_VENTE).toDouble() - articleVenteInfo->_montant_tva;
                articleVenteInfo->quantite_a_vendre = newQteValue;
                articleItemToVenteInfo.insert(itemChanged->row(), articleVenteInfo);
            }
            else
            {
                if (articleVenteInfo->quantite_en_stock < newQteValue)
                {
                    YerothQMessageBox::warning(this,
                                              QObject::trUtf8
                                              ("articles en stock"),
                                              QObject::trUtf8("Il n'y a pas assez de articles en stock !"));
                    itemChanged->setText(articleVenteInfo->quantiteAVendre());
                }
                else
                {
                    articleVenteInfo->quantite_a_vendre = newQteValue;
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
            //There was a conversion error because the current
            //value is not an integer
            _logger->debug("handleQteChange(QTableWidgetItem *)", "conversion error");
            itemChanged->setText(YerothTableWidget::QTE_1);
        }
    }
    setRechercheLineEditFocus();
}

bool YerothPointDeVenteWindow::article_exist(const QString codeBar, const QString designation)
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

void YerothPointDeVenteWindow::ajouter_article(const QString & text)
{
    _qteChangeCodeBar = false;

    QMap < QString, int >designationToTableRows = lineEdit_recherche_article->getDesignationToTableRows();
    //qDebug() << "++ YerothPointDeVenteWindow::ajouter_article, text:" << text;

    int selectedTableRow = designationToTableRows[text];

    _logger->log("ajouter_article(const QModelIndex &)", QString("model index: %1").arg(selectedTableRow));

    YerothTableView & articleTableView = *lineEdit_recherche_article->getMyTableView();
    articleTableView.setLastSelectedRow(selectedTableRow);

    lineEdit_recherche_article->getMySqlTableModel()->select();

    QSqlRecord record = lineEdit_recherche_article->getMySqlTableModel()->record(selectedTableRow);

    QString codeBar(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));
    QString designation(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    QString categorie(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));

    if (article_exist(codeBar, designation))
    {
        return;
    }

    QString qte_en_stock(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_TOTAL));

    double prix_vente = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_VENTE).toDouble();
    double montant_tva = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
    double prix_unitaire = prix_vente - montant_tva;

    if (prix_unitaire <= 0)
    {
        QString criticalMsg(QString("Erreur de calcul du prix de"
                                    " vente pour l'article %1!\n\n Contactez le développeur de yeroth!").
                            arg(designation));
        if (QMessageBox::Ok ==
                QMessageBox::critical(this,
                                      QObject::trUtf8
                                      ("erreur"),
                                      criticalMsg, QMessageBox::Cancel, QMessageBox::Ok))
        {
            return;
        }
    }
    //Each call to YerothTableWidget::setItem in method 'YerothTableWidget::addArticle'
    //triggers a call to YerothPointDeVenteWindow::handleQteChange
    int lastCurRow =
        tableWidget_articles->addArticle(selectedTableRow, codeBar, designation, categorie,
                                         QString::number(prix_unitaire, 'f', 2),
										 QString::number(montant_tva, 'f', 2),
										 QString::number(prix_vente, 'f', 2),
										 YerothTableWidget::QTE_1,
                                         qte_en_stock);
    if (lastCurRow > -1)
    {
        tableWidget_articles->selectRow(lastCurRow);
    }
}

void YerothPointDeVenteWindow::ajouter_article(const QModelIndex & modelIndex)
{
    //qDebug() << "++ test ajouter_article: " << modelIndex.row();
    _qteChangeCodeBar = false;

    int selectedTableRow = modelIndex.row();

    _logger->log("ajouter_article(const QModelIndex &)", QString("model index: %1").arg(selectedTableRow));

    YerothTableView & articleTableView = *lineEdit_recherche_article->getMyTableView();
    articleTableView.setLastSelectedRow(selectedTableRow);

    lineEdit_recherche_article->getMySqlTableModel()->select();

    QSqlRecord record = lineEdit_recherche_article->getMySqlTableModel()->record(selectedTableRow);

    QString codeBar(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));
    QString designation(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    QString categorie(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));

    if (article_exist(codeBar, designation))
    {
        return;
    }

    QString qte_en_stock(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_TOTAL));

    double prix_vente = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_VENTE).toDouble();
    double montant_tva = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
    double prix_unitaire = prix_vente - montant_tva;

    if (prix_unitaire <= 0)
    {
        QString criticalMsg(QString("Erreur de calcul du prix de"
                                    " vente pour l'article %1!\n\n Contactez le développeur de yeroth!")
        						.arg(designation));

        if (QMessageBox::Ok ==
                QMessageBox::critical(this,
                                      QObject::trUtf8
                                      ("erreur"),
                                      criticalMsg, QMessageBox::Cancel, QMessageBox::Ok))
        {
            return;
        }
    }
    //Each call to YerothTableWidget::setItem in method 'YerothTableWidget::addArticle'
    //triggers a call to YerothPointDeVenteWindow::handleQteChange
    int lastCurRow =
        tableWidget_articles->addArticle(selectedTableRow, codeBar, designation, categorie,
                                         QString::number(prix_unitaire, 'f', 2),
										 QString::number(montant_tva, 'f', 2),
										 QString::number(prix_vente, 'f', 2),
										 YerothTableWidget::QTE_1,
                                         qte_en_stock);
    if (lastCurRow > -1)
    {
        tableWidget_articles->selectRow(lastCurRow);
    }
}

void YerothPointDeVenteWindow::ajouter_article_codebar(const QString & text)
{
    QMap < QString, int >codebarToTableRows = lineEdit_recherche_article_codebar->getCodebarToTableRows();

    int selectedTableRow = codebarToTableRows[text];

    _logger->log("ajouter_article_codebar(const QString &)", QString("model index: %1").arg(selectedTableRow));
    //qDebug() << "YerothPointDeVenteWindow::ajouter_article_codebar(const QString &), "
    // << QString::number(selectedTableRow);

    YerothTableView & articleTableView = *lineEdit_recherche_article_codebar->getMyTableView();

    articleTableView.setLastSelectedRow(selectedTableRow);

    lineEdit_recherche_article_codebar->getMySqlTableModel()->select();

    QSqlRecord record = lineEdit_recherche_article_codebar->getMySqlTableModel()->record(selectedTableRow);

    QString codeBar(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));
    QString designation(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    QString categorie(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));

    if (article_exist(codeBar, designation))
    {
        return;
    }

    QString qte_en_stock(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_TOTAL));

    double prix_vente = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_VENTE).toDouble();
    double montant_tva = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
    double prix_unitaire = prix_vente - montant_tva;

    if (prix_unitaire <= 0)
    {
        QString criticalMsg(QString("Erreur de calcul du prix de"
                                    " vente pour l'article %1!\n\n Contactez le développeur de yeroth!")
                            	.arg(designation));

        if (QMessageBox::Ok ==
                QMessageBox::critical(this,
                                      QObject::trUtf8
                                      ("erreur"),
                                      criticalMsg, QMessageBox::Cancel, QMessageBox::Ok))
        {
            return;
        }
    }
    //Each call to YerothTableWidget::setItem in method 'YerothTableWidget::addArticle'
    //triggers a call to YerothPointDeVenteWindow::handleQteChange
    int lastCurRow =
        tableWidget_articles->addArticle(selectedTableRow, codeBar, designation, categorie,
                                         QString::number(prix_unitaire, 'f', 2),
										 QString::number(montant_tva, 'f', 2),
										 QString::number(prix_vente, 'f', 2),
										 YerothTableWidget::QTE_1,
                                         qte_en_stock);
    if (lastCurRow > -1)
    {
        tableWidget_articles->selectRow(lastCurRow);
    }
}

void YerothPointDeVenteWindow::ajouter_article_codebar(const QModelIndex & modelIndex)
{
    int selectedTableRow = modelIndex.row();

    _logger->log("ajouter_article_codebar(const QModelIndex &)",
                 QString("model index: %1").arg(selectedTableRow));

    YerothTableView & articleTableView = *(lineEdit_recherche_article_codebar->getMyTableView());

    articleTableView.setLastSelectedRow(selectedTableRow);

    qDebug() << QString("model index %1, articleTableView.selected: %2 ")
    				.arg(QString::number(selectedTableRow),
    					 QString::number(lineEdit_recherche_article_codebar->getMySqlTableModel()->easySelect()));

    //lineEdit_recherche_article_codebar->getMySqlTableModel()->select();
    QSqlRecord record = lineEdit_recherche_article_codebar->getMySqlTableModel()->record(selectedTableRow);

    QString codeBar(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));
    QString designation(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    QString categorie(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));

    qDebug() << QString("++ :ajouter_article_codebar, sqlTable: %1, selectedTableRow: %2, codeBar: %3")
    				.arg(lineEdit_recherche_article_codebar->getMySqlTableModel()->sqlTableName(),
    					 QString::number(selectedTableRow), codeBar);

    if (article_exist(codeBar, designation))
    {
        qDebug() << QString("++ codeBar: %1, designation: %2 before return.")
        				.arg(codeBar, designation);
        return;
    }

    qDebug() << QString("++ codeBar: %1, designation: %2 after return.")
    				.arg(codeBar, designation);

    QString qte_en_stock(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_TOTAL));

    double prix_vente = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_VENTE).toDouble();
    double montant_tva = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
    double prix_unitaire = prix_vente - montant_tva;

    if (prix_unitaire <= 0)
    {
        QString criticalMsg(QString("Erreur de calcul du prix de"
                                    " vente pour l'article %1!\n\n Contactez le développeur de yeroth!")
        						.arg(designation));

        if (QMessageBox::Ok ==
                QMessageBox::critical(this,
                                      QObject::trUtf8("erreur"),
                                      criticalMsg,
									  QMessageBox::Cancel,
									  QMessageBox::Ok))
        {
            return;
        }
    }
    //Each call to YerothTableWidget::setItem in method 'YerothTableWidget::addArticle'
    //triggers a call to YerothPointDeVenteWindow::handleQteChange
    int lastCurRow =
        tableWidget_articles->addArticle(selectedTableRow, codeBar, designation, categorie,
                                         QString::number(prix_unitaire, 'f', 2),
										 QString::number(montant_tva, 'f', 2),
										 QString::number(prix_vente, 'f', 2),
										 YerothTableWidget::QTE_1,
                                         qte_en_stock);
    if (lastCurRow > -1)
    {
        tableWidget_articles->selectRow(lastCurRow);
    }
}

/**
   * This method is called by 'YerothPointDeVenteWindow::handleQteChange'
   */

void YerothPointDeVenteWindow::actualiser_articles_codebar(int row, unsigned newItemQte)
{
    _logger->log("actualiser_articles(int, unsigned)",
                 QString("row: %1, quantite: %2")
				 	 .arg(QString::number(row),
				 			 QString::number(newItemQte)));

    _qteChangeCodeBar = true;
    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;
    QTableWidgetItem *curTableWidgetItem = 0;
    for (int k = 0; k < tableWidget_articles->rowCount(); ++k)
    {
        YerothSqlTableModel & articleSqlTableModel = *lineEdit_recherche_article_codebar->getMySqlTableModel();
        QSqlRecord record = articleSqlTableModel.record(tableWidget_articles->getSqlTableModelIndex(k));
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

            totalWidgetItem->setText(GET_CURRENCY_STRING_NUM(articleVenteInfo->prix_vente()));
        }

myItemBreak:
        quantiteVendue += curTableWidgetItemQte;
        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);
        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteAVendre = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    double total = _sommeTotal - _tva;

    update_lineedits_and_labels(total);

    this->tableWidget_articles->resizeColumnsToContents();
}

/**
  * This method is called by 'YerothPointDeVenteWindow::handleQteChange'
  */

void YerothPointDeVenteWindow::actualiser_articles(int row, unsigned newItemQte)
{
    _logger->log("actualiser_articles(int, unsigned)",
                 QString("row: %1, quantite: %2")
				 	 .arg(QString::number(row),
				 		  QString::number(newItemQte)));

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableWidget_articles->rowCount(); ++k)
    {
        YerothSqlTableModel & articleSqlTableModel = *lineEdit_recherche_article->getMySqlTableModel();
        QSqlRecord record = articleSqlTableModel.record(tableWidget_articles->getSqlTableModelIndex(k));

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

    _quantiteAVendre = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    double total = _sommeTotal - _tva;

    update_lineedits_and_labels(total);

    this->tableWidget_articles->resizeColumnsToContents();
}

void YerothPointDeVenteWindow::actualiser_tableau_vente()
{
    int tableRowCount = tableWidget_articles->rowCount();

    _logger->log("actualiser_tableau_vente", QString("tableRowCount: %1").arg(tableRowCount));

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableRowCount; ++k)
    {
        YerothSqlTableModel & articleSqlTableModel = *lineEdit_recherche_article->getMySqlTableModel();

        QSqlRecord record = articleSqlTableModel.record(tableWidget_articles->getSqlTableModelIndex(k));

        YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(k);

        curTableWidgetItem = tableWidget_articles->item(k, YerothTableWidget::QTE_COLUMN);

        curTableWidgetItemQte = curTableWidgetItem->text().toDouble();

        quantiteVendue += curTableWidgetItemQte;

        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);

        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteAVendre = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    double total = _sommeTotal - _tva;

    update_lineedits_and_labels(total);

    this->tableWidget_articles->resizeColumnsToContents();
}

void YerothPointDeVenteWindow::actualiser_toutes_valeurs()
{
    int tableRowCount = tableWidget_articles->rowCount();

    _logger->log("actualiser_toutes_valeurs", QString("tableRowCount: %1").arg(tableRowCount));

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;

    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableRowCount; ++k)
    {
        YerothSqlTableModel & articleSqlTableModel = *lineEdit_recherche_article->getMySqlTableModel();

        QSqlRecord record = articleSqlTableModel.record(tableWidget_articles->getSqlTableModelIndex(k));

        YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(k);

        curTableWidgetItem = tableWidget_articles->item(k, YerothTableWidget::QTE_COLUMN);
        curTableWidgetItemQte = curTableWidgetItem->text().toDouble();

        quantiteVendue += curTableWidgetItemQte;

        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);

        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteAVendre = quantiteVendue;

    _tva = tva;

    _sommeTotal = sommeTotal;

    double total = _sommeTotal - _tva;

    if (_remise_somme_total_prix < sommeTotal)
    {
        _sommeTotal = _sommeTotal - _remise_somme_total_prix;
    }

    update_lineedits_and_labels(total);
}

void YerothPointDeVenteWindow::update_lineedits_and_labels(double total)
{
    lineEdit_articles_quantite_a_vendre->setText(GET_NUM_STRING(_quantiteAVendre));
    lineEdit_articles_tva->setText(GET_CURRENCY_STRING_NUM(_tva));
    lineEdit_articles_total->setText(GET_CURRENCY_STRING_NUM(total));
    lineEdit_articles_somme_total->setText(GET_CURRENCY_STRING_NUM(_sommeTotal));
    label_total_ttc->setText(GET_CURRENCY_STRING_NUM(_sommeTotal));

    handleMontantRecu();
}

void YerothPointDeVenteWindow::handleMontantRecu()
{
	_montantRecu =
			_allWindows->
				_pdVenteMethodePaiementComptantEntreeDialog->
					lineEdit_vente_montant_recu->text().toDouble();

	_montantARembourser = _montantRecu - _sommeTotal;

	if (_montantARembourser > 0)
	{
		lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM(_montantARembourser));
	}
	else
	{
		lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM(0.0));
	}
}

void YerothPointDeVenteWindow::enlever_article()
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
                           QString("key: %1, value: %2").arg(QString::number(k), i.value()->designation));
            ++k;
        }
        articleItemToVenteInfo.clear();
        articleItemToVenteInfo = newArticleItemToVenteInfo;
        actualiser_tableau_vente();
        setRechercheLineEditFocus();
    }
}


void YerothPointDeVenteWindow::choisir_methode_paiment()
{
	_logger->log("choisir_methode_paiment");

	_allWindows->_pdVenteMethodePaiementDialog->rendreInvisible();

	double total_prix_vente = 0.0;

    if (tableWidget_articles->itemCount() <= 0)
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("vendre"),
                                  QObject::trUtf8("Vous n'avez pas choisi d'articles à vendre !"));
        return;
    }
    else
    {
    	YerothArticleVenteInfo *articleVenteInfo = 0;
    	for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
    	{
    		articleVenteInfo = articleItemToVenteInfo.value(j);
    		if (0 != articleVenteInfo)
    		{
    			total_prix_vente += articleVenteInfo->prix_vente();
    		}
    	}
    }

    if (!lineEdit_articles_nom_client->text().isEmpty())
    {
        YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

        QString nom_entreprise_filter("nom_entreprise = '");

        nom_entreprise_filter.append(lineEdit_articles_nom_client->text()).append("'");

        clientsTableModel.yerothSetFilter(nom_entreprise_filter);

        int clientsTableModelRowCount = clientsTableModel.rowCount();

        if (clientsTableModelRowCount > 0)
        {
        	QSqlRecord record = clientsTableModel.record(0);

        	double dette_maximale_compte_client = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();
        	double compte_client = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

        	double dette_compte_client_disponible = dette_maximale_compte_client - qFabs(compte_client);

        	if (dette_compte_client_disponible >= total_prix_vente ||
        		compte_client >= total_prix_vente)
        	{
        		_allWindows->_pdVenteMethodePaiementDialog->setPushbuttonCompteClientEnabled(true);
        	}
        	else
        	{
        		_allWindows->_pdVenteMethodePaiementDialog->setPushbuttonCompteClientEnabled(false);
        	}
        }
        else
        {
            YerothQMessageBox::warning(this, QObject::trUtf8("vendre"),
                                      QObject::trUtf8("Le client entré n'existe pas dans la base de données !"));

            return ;
        }
    }
    else
    {
    	_allWindows->_pdVenteMethodePaiementDialog->setPushbuttonCompteClientEnabled(false);
    }

    _allWindows->_pdVenteMethodePaiementDialog->showAsModalDialogWithParent(*this);
}


unsigned int YerothPointDeVenteWindow::effectuer_check_out_carte_credit_carte_debit()
{
	_logger->log("vendre");

	QString msgVente(QObject::trUtf8("Poursuivre avec la vente de "));

	msgVente.append(QString::number(this->_quantiteAVendre, 'f', 0));
	msgVente.append(QObject::trUtf8(" articles (carte de crédit / débit) ?"));

	if (QMessageBox::Ok ==
			YerothQMessageBox::question(this, QObject::trUtf8("vendre"),
					msgVente,
					QMessageBox::Cancel,
					QMessageBox::Ok))
	{

		YerothUtils::startTransaction();

		bool paiementParCarteDeCredit_CarteDebit_Success = PROCESS_CREDIT_CARD_PAYMENT();

		int vente_id = YerothUtils::getNextVentesIdFromTable("stocks_vendu");

		for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
		{
			YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(j);

			QSqlRecord stockRecord = _curStocksTableModel->record(articleVenteInfo->sqlTableModelIndex);

			QString stockRecordId = GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::ID);
			QString quantiteQueryStr("SELECT quantite_total FROM ");
			quantiteQueryStr.append(_allWindows->STOCKS).append(" WHERE id = '").append(stockRecordId).
					append("'");

			QSqlQuery quantiteQuery;

			double quantite_actuelle = 0.0;

			int querySize = YerothUtils::execQuery(quantiteQuery, quantiteQueryStr, _logger);

			if (querySize > 0 && quantiteQuery.next())
			{
				quantite_actuelle = quantiteQuery.value(0).toDouble();
			}

			YerothSqlTableModel & stocksVenduTableModel = _allWindows->getSqlTableModel_stocks_vendu();

			QSqlRecord record = stocksVenduTableModel.record();

			record.setValue(YerothDatabaseTableColumn::ID, YerothUtils::getNextIdFromTable(_allWindows->STOCKS_VENDU));
			record.setValue(YerothDatabaseTableColumn::VENTE_ID, vente_id);
			record.setValue(YerothDatabaseTableColumn::REFERENCE, articleVenteInfo->reference);
			record.setValue(YerothDatabaseTableColumn::DESIGNATION, articleVenteInfo->designation);

			record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
					GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::DATE_PEREMPTION));

			record.setValue(YerothDatabaseTableColumn::CATEGORIE, articleVenteInfo->categorie);
			record.setValue(YerothDatabaseTableColumn::QUANTITE_VENDUE, articleVenteInfo->quantite_a_vendre);
			record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, articleVenteInfo->prix_unitaire);
			record.setValue(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE, articleVenteInfo->prix_vente());
			record.setValue(YerothDatabaseTableColumn::REMISE_PRIX, articleVenteInfo->remise_prix);
			record.setValue(YerothDatabaseTableColumn::REMISE_POURCENTAGE, articleVenteInfo->remise_pourcentage);
			record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, articleVenteInfo->montant_tva());
			record.setValue(YerothDatabaseTableColumn::LOCALISATION, GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::LOCALISATION));
			record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
					GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

			YerothPOSUser *user = _allWindows->getUser();

			record.setValue(YerothDatabaseTableColumn::NOM_CAISSIER, user->nom_complet());
			record.setValue(YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER, user->nom_utilisateur());
			record.setValue(YerothDatabaseTableColumn::DATE_VENTE, GET_CURRENT_DATE);
			record.setValue("heure_vente", CURRENT_TIME);
			record.setValue(YerothDatabaseTableColumn::STOCKS_ID, GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::ID));
			record.setValue("montant_recu", _montantRecu);
			record.setValue("montant_a_rembourser", lineEdit_articles_montant_a_rembourser->text().toDouble());

			YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

			QString clientFilter;
			clientFilter.append("nom_entreprise = '").append(lineEdit_articles_nom_client->text()).append("'");
			clientsTableModel.yerothSetFilter(clientFilter);

			int clientsTableModelRowCount = clientsTableModel.easySelect();
			if (clientsTableModelRowCount > 0)
			{
				QSqlRecord clientsRecord = clientsTableModel.record(0);
				QString clients_id(GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::ID));
				record.setValue(YerothDatabaseTableColumn::CLIENTS_ID, clients_id);
				record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, lineEdit_articles_nom_client->text());
				clientsTableModel.resetFilter();
			}
			else
			{
				record.setValue(YerothDatabaseTableColumn::CLIENTS_ID, -1);
				record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, "DIVERS");
			}

			bool success1 = stocksVenduTableModel.insertNewRecord(record, this);

			if (success1)
			{
				double nouvelle_quantite = quantite_actuelle - articleVenteInfo->quantite_a_vendre;

				if (nouvelle_quantite < 0)
				{
					nouvelle_quantite = 0;
				}

				stockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTAL, nouvelle_quantite);

				if (0 == nouvelle_quantite)
				{
					QString removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
							.arg(_allWindows->STOCKS,
									YerothDatabaseTableColumn::ID,
									stockRecordId));
					YerothUtils::execQuery(removeRowQuery);
				}

				quantiteQueryStr.clear();
				quantiteQueryStr.append(QString("UPDATE %1 SET %2 = %3 WHERE %4 = '%5'")
						.arg(_allWindows->STOCKS,
								YerothDatabaseTableColumn::QUANTITE_TOTAL,
								QString::number(nouvelle_quantite),
								YerothDatabaseTableColumn::ID,
								stockRecordId));

				bool success2 = YerothUtils::execQuery(quantiteQueryStr, _logger);

				QString sMsg(QObject::trUtf8("La quantité en stock de l'article '"));

				sMsg.append(articleVenteInfo->designation).append("'")
                    		.append(QString(QObject::trUtf8(" (%1 pièce(s))"))
                    				.arg(articleVenteInfo->quantite_a_vendre));

				if (success2)
				{
					sMsg.append(QObject::trUtf8(" a été actualisée avec succès."));
				}
				else
				{
					sMsg.append(QObject::trUtf8(" n'a pas pu être actualisée!\n" "Contacter 'YEROTH'"));
				}

				_logger->log("vendre", sMsg);
			}
			else
			{
				//TODO MESSAGE D'ERREUR DANS LE FICHIER DE LOGS
			}
		}
		if (paiementParCarteDeCredit_CarteDebit_Success)
		{
			emit SELLING();

			QString vMsg(QObject::trUtf8("La vente de '"));

			vMsg.append(QString::number(_quantiteAVendre))
            			.append(QObject::trUtf8("' articles a été éffectuée avec succès."));

			if (QMessageBox::Ok ==
					YerothQMessageBox::information(this,
							QObject::trUtf8("succès d'une vente"),
							vMsg))
			{
				this->imprimer_facture(vente_id);
			}
		}

		YerothUtils::commitTransaction();

		this->cleanUpAfterVente();

	}
	else
	{
		YerothQMessageBox::information(this, QObject::trUtf8("annulation d'une vente"),
				QObject::trUtf8("Vous avez annulé la vente !"), QMessageBox::Ok);
	}

	tableWidget_articles->resizeColumnsToContents();

	return 0;
}


void YerothPointDeVenteWindow::executer_la_vente_comptant()
{
    int vente_id = YerothUtils::getNextVentesIdFromTable("stocks_vendu");

    for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
    {
        YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(j);

        QSqlRecord stockRecord = _curStocksTableModel->record(articleVenteInfo->sqlTableModelIndex);

        QString stockRecordId = GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::ID);
        QString quantiteQueryStr("SELECT quantite_total FROM ");
        quantiteQueryStr.append(_allWindows->STOCKS).append(" WHERE id = '").append(stockRecordId).
        append("'");

        QSqlQuery quantiteQuery;

        double quantite_actuelle = 0.0;

        int querySize = YerothUtils::execQuery(quantiteQuery, quantiteQueryStr, _logger);

        if (querySize > 0 && quantiteQuery.next())
        {
            quantite_actuelle = quantiteQuery.value(0).toDouble();
        }

        QString historiqueStock =
        		GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK);

        //qDebug() << QString("++ previousHistoriquestock: %1")
        //        		.arg(historiqueStock);

        YerothSqlTableModel & stocksVenduTableModel = _allWindows->getSqlTableModel_stocks_vendu();

        QSqlRecord record = stocksVenduTableModel.record();

#ifdef YEROTH_FRANCAIS_LANGUAGE
        _typeDeVente = "comptant";
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
        _typeDeVente = "cash";
#endif

        int stock_id_to_save = YerothUtils::getNextIdFromTable(_allWindows->STOCKS_VENDU);

        record.setValue(YerothDatabaseTableColumn::ID, stock_id_to_save);

        record.setValue(YerothDatabaseTableColumn::TYPE_DE_VENTE, _typeDeVente);

        record.setValue(YerothDatabaseTableColumn::VENTE_ID, vente_id);
        record.setValue(YerothDatabaseTableColumn::REFERENCE, articleVenteInfo->reference);
        record.setValue(YerothDatabaseTableColumn::DESIGNATION, articleVenteInfo->designation);

        record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                        GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::DATE_PEREMPTION));

        record.setValue(YerothDatabaseTableColumn::CATEGORIE, articleVenteInfo->categorie);
        record.setValue(YerothDatabaseTableColumn::QUANTITE_VENDUE, articleVenteInfo->quantite_a_vendre);
        record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, articleVenteInfo->prix_unitaire);
        record.setValue(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE, articleVenteInfo->prix_vente());
        record.setValue(YerothDatabaseTableColumn::REMISE_PRIX, articleVenteInfo->remise_prix);
        record.setValue(YerothDatabaseTableColumn::REMISE_POURCENTAGE, articleVenteInfo->remise_pourcentage);
        record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, articleVenteInfo->montant_tva());
        record.setValue(YerothDatabaseTableColumn::LOCALISATION, GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::LOCALISATION));
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                        GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

        YerothPOSUser *user = _allWindows->getUser();

        record.setValue(YerothDatabaseTableColumn::NOM_CAISSIER, user->nom_complet());
        record.setValue(YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER, user->nom_utilisateur());
        record.setValue(YerothDatabaseTableColumn::DATE_VENTE, GET_CURRENT_DATE);
        record.setValue(YerothDatabaseTableColumn::HEURE_VENTE, CURRENT_TIME);

        record.setValue(YerothDatabaseTableColumn::STOCKS_ID,
        		GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::ID));

        record.setValue(YerothDatabaseTableColumn::MONTANT_RECU, _montantRecu);

        record.setValue(YerothDatabaseTableColumn::MONTANT_A_REMBOURSER,
        		lineEdit_articles_montant_a_rembourser->text().toDouble());

        double nouvelle_quantite = quantite_actuelle - articleVenteInfo->quantite_a_vendre;

        QString historiqueStockVendu(YerothHistoriqueStock::creer_mouvement_stock
        			(VENTE,
        			 stock_id_to_save,
					 GET_CURRENT_DATE,
					 quantite_actuelle,
					 articleVenteInfo->quantite_a_vendre,
					 nouvelle_quantite));

        historiqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
        			   .append(historiqueStockVendu);

        //qDebug() << QString("++ test: %1")
         //       		.arg(historiqueStock);

        YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

        QString clientFilter;
        clientFilter.append("nom_entreprise = '").append(lineEdit_articles_nom_client->text()).append("'");
        clientsTableModel.yerothSetFilter(clientFilter);

        int clientsTableModelRowCount = clientsTableModel.easySelect();
        if (clientsTableModelRowCount > 0)
        {
            QSqlRecord clientsRecord = clientsTableModel.record(0);
            QString clients_id(GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::ID));
            record.setValue(YerothDatabaseTableColumn::CLIENTS_ID, clients_id);
            record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, lineEdit_articles_nom_client->text());
            clientsTableModel.resetFilter();
        }
        else
        {
            record.setValue(YerothDatabaseTableColumn::CLIENTS_ID, -1);
            record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, "DIVERS");
        }

        bool success1 = stocksVenduTableModel.insertNewRecord(record, this);

        if (success1)
        {
            if (nouvelle_quantite < 0)
            {
                nouvelle_quantite = 0;
            }

            if (0 == nouvelle_quantite)
            {
                QString removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
                                       .arg(_allWindows->STOCKS,
                                            YerothDatabaseTableColumn::ID,
                                            stockRecordId));
                YerothUtils::execQuery(removeRowQuery);
            }

            quantiteQueryStr.clear();
            quantiteQueryStr.append(QString("UPDATE %1  SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'")
                                    .arg(_allWindows->STOCKS,
                                         YerothDatabaseTableColumn::QUANTITE_TOTAL,
                                         QString::number(nouvelle_quantite),
										 YerothDatabaseTableColumn::HISTORIQUE_STOCK,
										 historiqueStock,
                                         YerothDatabaseTableColumn::ID,
                                         stockRecordId));

            //qDebug() << QString("++ quantiteQueryStr: %1")
            	//			.arg(quantiteQueryStr);

            bool success2 = YerothUtils::execQuery(quantiteQueryStr, _logger);

			QString sMsg(QObject::trUtf8("La quantité en stock de l'article '"));

			sMsg.append(articleVenteInfo->designation).append("'")
                		.append(QString(QObject::trUtf8(" (%1 pièce(s))"))
                				.arg(articleVenteInfo->quantite_a_vendre));

			if (success2)
			{
				sMsg.append(QObject::trUtf8(" a été actualisée avec succès."));
			}
			else
			{
				sMsg.append(QObject::trUtf8(" n'a pas pu être actualisée!\n" "Contacter 'YEROTH'"));
			}

            _logger->log("vendre", sMsg);
        }
		else
		{
			//TODO MESSAGE D'ERREUR DANS LE FICHIER DE LOGS
		}
    }

    emit SELLING();

    QString vMsg(QObject::trUtf8("La vente de '"));

    vMsg.append(QString::number(_quantiteAVendre))
        			.append(QObject::trUtf8("' articles a été éffectuée avec succès."));

    if (QMessageBox::Ok ==
    		YerothQMessageBox::information(this,
    				QObject::trUtf8("succès d'une vente"),
					vMsg))
    {
    	this->imprimer_facture(vente_id);
    }
}


unsigned int YerothPointDeVenteWindow::effectuer_check_out_comptant()
{
	_logger->log("vendre");

	if (_montantRecu <= 0 || _montantRecu < _sommeTotal)
	{
		if (QMessageBox::Ok == YerothQMessageBox::warning(this, QObject::trUtf8("montant reçu du client"),
				QObject::trUtf8("Le montant reçu du client doit être supérieure"
						" à la somme total !")))
		{
			return -1;
		}
		else
		{
		}
	}

	_allWindows->_pdVenteMethodePaiementComptantEntreeDialog->rendreInvisible();

	handleMontantRecu();

    QString msgVente(QObject::trUtf8("Poursuivre avec la vente de "));

    msgVente.append(QString::number(this->_quantiteAVendre, 'f', 0));
    msgVente.append(QObject::trUtf8(" articles (comptant) ?"));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::trUtf8("vendre"),
    										 msgVente,
											 QMessageBox::Cancel,
											 QMessageBox::Ok))
    {
YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    	executer_la_vente_comptant();

YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        this->cleanUpAfterVente();
    }
    else
    {
    	YerothQMessageBox::information(this, QObject::trUtf8("annulation d'une vente"),
    			QObject::trUtf8("Vous avez annulé la vente !"), QMessageBox::Ok);
    }

    tableWidget_articles->resizeColumnsToContents();

    return 0;
}


void YerothPointDeVenteWindow::updateCompteClient(double total_prix_vente)
{
    YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

    QString clientFilter;
    clientFilter.append(QString("%1 = '%2'")
    						.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
    							 lineEdit_articles_nom_client->text()));

    clientsTableModel.yerothSetFilter(clientFilter);

    double compte_client = 0.0;
    double dette_maximale_compte_client = 0.0;

    int clientsTableModelRowCount = clientsTableModel.easySelect();
    if (clientsTableModelRowCount > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);
        dette_maximale_compte_client = GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();
        compte_client = GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();
        compte_client = compte_client - total_prix_vente;
        clientsTableModel.resetFilter();
    }

	QString queryStr;

	queryStr.append(QString("UPDATE %1  SET %2 = '%3' WHERE %6 = '%7'")
                            .arg(_allWindows->CLIENTS,
                                 YerothDatabaseTableColumn::COMPTE_CLIENT,
                                 QString::number(compte_client),
								 YerothDatabaseTableColumn::NOM_ENTREPRISE,
								 lineEdit_articles_nom_client->text()));

    bool success = YerothUtils::execQuery(queryStr, _logger);
}


void YerothPointDeVenteWindow::executer_la_vente_compte_client()
{
    int vente_id = YerothUtils::getNextVentesIdFromTable("stocks_vendu");

    double total_prix_vente = 0.0;

    for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
    {
        YerothArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(j);

        QSqlRecord stockRecord = _curStocksTableModel->record(articleVenteInfo->sqlTableModelIndex);

        QString stockRecordId = GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::ID);
        QString quantiteQueryStr("SELECT quantite_total FROM ");
        quantiteQueryStr.append(_allWindows->STOCKS).append(" WHERE id = '").append(stockRecordId).
        append("'");

        QSqlQuery quantiteQuery;

        double quantite_actuelle = 0.0;

        int querySize = YerothUtils::execQuery(quantiteQuery, quantiteQueryStr, _logger);

        if (querySize > 0 && quantiteQuery.next())
        {
            quantite_actuelle = quantiteQuery.value(0).toDouble();
        }

        QString historiqueStock =
        		GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK);

        //qDebug() << QString("++ previousHistoriquestock: %1")
        //        		.arg(historiqueStock);

        YerothSqlTableModel & stocksVenduTableModel = _allWindows->getSqlTableModel_stocks_vendu();

        QSqlRecord record = stocksVenduTableModel.record();

#ifdef YEROTH_FRANCAIS_LANGUAGE
        _typeDeVente = "compte_client";
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
        _typeDeVente = "customer_account";
#endif

        int stock_id_to_save = YerothUtils::getNextIdFromTable(_allWindows->STOCKS_VENDU);

        record.setValue(YerothDatabaseTableColumn::ID, stock_id_to_save);

        record.setValue(YerothDatabaseTableColumn::TYPE_DE_VENTE, _typeDeVente);

        record.setValue(YerothDatabaseTableColumn::VENTE_ID, vente_id);
        record.setValue(YerothDatabaseTableColumn::REFERENCE, articleVenteInfo->reference);
        record.setValue(YerothDatabaseTableColumn::DESIGNATION, articleVenteInfo->designation);

        record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                        GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::DATE_PEREMPTION));

        record.setValue(YerothDatabaseTableColumn::CATEGORIE, articleVenteInfo->categorie);
        record.setValue(YerothDatabaseTableColumn::QUANTITE_VENDUE, articleVenteInfo->quantite_a_vendre);
        record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, articleVenteInfo->prix_unitaire);

        total_prix_vente += articleVenteInfo->prix_vente();

        record.setValue(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE, articleVenteInfo->prix_vente());
        record.setValue(YerothDatabaseTableColumn::REMISE_PRIX, articleVenteInfo->remise_prix);
        record.setValue(YerothDatabaseTableColumn::REMISE_POURCENTAGE, articleVenteInfo->remise_pourcentage);
        record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, articleVenteInfo->montant_tva());
        record.setValue(YerothDatabaseTableColumn::LOCALISATION, GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::LOCALISATION));
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                        GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

        YerothPOSUser *user = _allWindows->getUser();

        record.setValue(YerothDatabaseTableColumn::NOM_CAISSIER, user->nom_complet());
        record.setValue(YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER, user->nom_utilisateur());
        record.setValue(YerothDatabaseTableColumn::DATE_VENTE, GET_CURRENT_DATE);
        record.setValue(YerothDatabaseTableColumn::HEURE_VENTE, CURRENT_TIME);

        record.setValue(YerothDatabaseTableColumn::STOCKS_ID,
        		GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::ID));

        record.setValue(YerothDatabaseTableColumn::MONTANT_RECU, _montantRecu);

        record.setValue(YerothDatabaseTableColumn::MONTANT_A_REMBOURSER,
        		lineEdit_articles_montant_a_rembourser->text().toDouble());

        double nouvelle_quantite = quantite_actuelle - articleVenteInfo->quantite_a_vendre;

        QString historiqueStockVendu(YerothHistoriqueStock::creer_mouvement_stock
        			(VENTE,
        			 stock_id_to_save,
					 GET_CURRENT_DATE,
					 quantite_actuelle,
					 articleVenteInfo->quantite_a_vendre,
					 nouvelle_quantite));

        historiqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
        			   .append(historiqueStockVendu);

        //qDebug() << QString("++ test: %1")
         //       		.arg(historiqueStock);

        YerothSqlTableModel & clientsTableModel = _allWindows->getSqlTableModel_clients();

        QString clientFilter;
        clientFilter.append("nom_entreprise = '").append(lineEdit_articles_nom_client->text()).append("'");
        clientsTableModel.yerothSetFilter(clientFilter);

        int clientsTableModelRowCount = clientsTableModel.easySelect();
        if (clientsTableModelRowCount > 0)
        {
            QSqlRecord clientsRecord = clientsTableModel.record(0);
            QString clients_id(GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::ID));
            record.setValue(YerothDatabaseTableColumn::CLIENTS_ID, clients_id);
            record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, lineEdit_articles_nom_client->text());
            clientsTableModel.resetFilter();
        }
        else
        {
            record.setValue(YerothDatabaseTableColumn::CLIENTS_ID, -1);
            record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, "DIVERS");
        }

        bool success1 = stocksVenduTableModel.insertNewRecord(record, this);

        if (success1)
        {
            if (nouvelle_quantite < 0)
            {
                nouvelle_quantite = 0;
            }

            if (0 == nouvelle_quantite)
            {
                QString removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
                                       .arg(_allWindows->STOCKS,
                                            YerothDatabaseTableColumn::ID,
                                            stockRecordId));
                YerothUtils::execQuery(removeRowQuery);
            }

            quantiteQueryStr.clear();
            quantiteQueryStr.append(QString("UPDATE %1  SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'")
                                    .arg(_allWindows->STOCKS,
                                         YerothDatabaseTableColumn::QUANTITE_TOTAL,
                                         QString::number(nouvelle_quantite),
										 YerothDatabaseTableColumn::HISTORIQUE_STOCK,
										 historiqueStock,
                                         YerothDatabaseTableColumn::ID,
                                         stockRecordId));

            //qDebug() << QString("++ quantiteQueryStr: %1")
            	//			.arg(quantiteQueryStr);

            bool success2 = YerothUtils::execQuery(quantiteQueryStr, _logger);

			QString sMsg(QObject::trUtf8("La quantité en stock de l'article '"));

			sMsg.append(articleVenteInfo->designation).append("'")
                		.append(QString(QObject::trUtf8(" (%1 pièce(s))"))
                				.arg(articleVenteInfo->quantite_a_vendre));

			if (success2)
			{
				sMsg.append(QObject::trUtf8(" a été actualisée avec succès."));
			}
			else
			{
				sMsg.append(QObject::trUtf8(" n'a pas pu être actualisée !\n" "Contacter 'YEROTH'"));
			}

            _logger->log("vendre", sMsg);
        }
		else
		{
			//TODO MESSAGE D'ERREUR DANS LE FICHIER DE LOGS
		}
    }

    updateCompteClient(total_prix_vente);

    emit SELLING();

    QString vMsg(QString(QObject::trUtf8("La vente de '%1' articles a été éffectuée avec succès !"))
    				.arg(QString::number(_quantiteAVendre)));

    if (QMessageBox::Ok ==
    		YerothQMessageBox::information(this,
    				QObject::trUtf8("succès d'une vente"),
					vMsg))
    {
    	this->imprimer_facture(vente_id);
    }
}


unsigned int YerothPointDeVenteWindow::effectuer_check_out_compte_client()
{
    QString msgVente(QObject::trUtf8("Poursuivre avec la vente de "));

    msgVente.append(QString::number(this->_quantiteAVendre, 'f', 0));
    msgVente.append(QObject::trUtf8(" articles (compte client) ?"));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::trUtf8("vendre"),
    										 msgVente,
											 QMessageBox::Cancel,
											 QMessageBox::Ok))
    {
    	YerothUtils::startTransaction();

    	executer_la_vente_compte_client();

    	YerothUtils::commitTransaction();

        this->cleanUpAfterVente();
    }
    else
    {
    	YerothQMessageBox::information(this, QObject::trUtf8("annulation d'une vente"),
    			QObject::trUtf8("Vous avez annulé la vente !"), QMessageBox::Ok);
    }

    tableWidget_articles->resizeColumnsToContents();

	return 0;
}
